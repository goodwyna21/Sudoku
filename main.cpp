#include <iostream>
#include <ncurses.h>
#include <fstream>
#include <string>

using namespace std;

#define filled 1
#define unfilled 2
#define blank 3
#define select 4
#define placed 5

const string FNAME = "Boards/Medium.txt";

struct Cell{
    int n;
    bool vals[9];
    int x,y;

    Cell(){}
    Cell(int X, int Y) : x(X), y(Y), n(-1) {
        for(int i = 0; i < 9; i++){
            vals[i] = true;
        }
    }

    void draw(){
        if(n!=-1){
            attron(COLOR_PAIR(filled));
            mvaddstr(y*4,x*4,"   ");
            mvaddch(y*4+1,x*4,' ');
            addch(to_string(n)[0]);
            addch(' ');
            mvaddstr(y*4+2,x*4,"   ");
            attroff(COLOR_PAIR(filled));
        }else{
            for(int i = 1; i <= 9; i++){
                if(i%3 == 1){
                    move(y*4+((i==4)?1:((i==7)?2:0)),x*4);
                }
                attron(COLOR_PAIR(vals[i-1]?unfilled:blank));
                addstr(to_string(i).c_str());
                attroff(COLOR_PAIR(vals[i-1]?unfilled:blank));
            }
        }
    }

    void draw_select(){
        attron(COLOR_PAIR(select));
        if(n!=-1){
            mvaddstr(y*4,x*4,"   ");
            mvaddch(y*4+1,x*4,' ');
            addch(to_string(n)[0]);
            addch(' ');
            mvaddstr(y*4+2,x*4,"   ");
        }else{
            for(int i = 1; i <= 9; i++){
                if(i%3 == 1){
                    move(y*4+((i==4)?1:((i==7)?2:0)),x*4);
                }
                addstr(vals[i-1]?to_string(i).c_str():" ");
            }
        }
        attron(COLOR_PAIR(select));
    }

    void draw_placed(){
        attron(COLOR_PAIR(placed));
        if(n!=-1){
            mvaddstr(y*4,x*4,"   ");
            mvaddch(y*4+1,x*4,' ');
            addch(to_string(n)[0]);
            addch(' ');
            mvaddstr(y*4+2,x*4,"   ");
        }else{
            for(int i = 1; i <= 9; i++){
                if(i%3 == 1){
                    move(y*4+((i==4)?1:((i==7)?2:0)),x*4);
                }
                addstr(vals[i-1]?to_string(i).c_str():" ");
            }
        }
        attron(COLOR_PAIR(placed));
    }
};

struct Board{
    Cell board[9][9];
    int placedx,placedy;

    Board(){
        for(int y = 0; y < 9; y++){
            for(int x = 0; x < 9; x++){
                board[x][y] = Cell(x,y);
            }
        }
        placedx=0;
        placedy=0;
    }

    void draw(){
        for(int y = 0; y < 9; y++){
            for(int x = 0; x < 9; x++){
                board[x][y].draw();
            }
        }
    }

    void add(uint n, uint x, uint y){
        board[x][y].n = n;
        for(int i = 0; i < 9; i++){
            board[i][y].vals[n-1] = false;
            board[x][i].vals[n-1] = false;
        }
        for(int x1 = 0; x1 < 3; x1++){
            for(int y1 = 0; y1 < 3; y1++){
                board[((int)(x/3))*3 + x1][((int)(y/3))*3 + y1].vals[n-1] = false;
            }
        }
        draw();
        board[x][y].draw_placed();
    }

    bool autoadd(){
        int fnd[9];

        //search for cells with one option
        int cnt;
        int val;
        for(int y = 0; y < 9; y++){
            for(int x = 0; x < 9; x++){
                cnt = 0;
                if(board[x][y].n!=-1){continue;}
                for(int i = 0; i < 9; i++){
                    if(board[x][y].vals[i]){
                        cnt++;
                        val=i+1;
                    }
                }
                if(cnt==1){
                    add(val,x,y);
                    placedx=x;
                    placedy=y;
                    return true;
                }
            }
        }

        //search rows
        int xs[9];
        for(int y = 0; y < 9; y++){
            for(int i = 0; i < 9; i++){
                fnd[i] = 0;
            }
            for(int x = 0; x < 9; x++){
                for(int i = 0; i < 9; i++){
                    if(board[x][y].vals[i] && board[x][y].n==-1){
                        fnd[i]++;
                        xs[i]=x;
                    }
                }
            }
            for(int i = 0; i < 9; i++){
                if(fnd[i]==1){
                    add(i+1,xs[i],y);
                    placedx=xs[i];
                    placedy=y;
                    return true;
                }
            }
        }

        //search collumns
        int ys[9];
        for(int x = 0; x < 9; x++){
            for(int i = 0; i < 9; i++){
                fnd[i] = 0;
            }
            for(int y = 0; y < 9; y++){
                for(int i = 0; i < 9; i++){
                    if(board[x][y].vals[i] && board[x][y].n==-1){
                        fnd[i]++;
                        ys[i]=y;
                    }
                }
            }
            for(int i = 0; i < 9; i++){
                if(fnd[i]==1){
                    add(i+1,x,ys[i]);
                    placedx=x;
                    placedy=ys[i];
                    return true;
                }
            }
        }

        //search cells
        for(int i = 0; i < 9; i++){
            xs[i] = 0;
            ys[i] = 0;
        }
        for(int n = 0; n < 9; n++){
            for(int i = 0; i < 9; i++){
                fnd[i] = 0;
            }
            for(int y = 0; y < 3; y++){
                for(int x = 0; x < 3; x++){
                    for(int i = 0; i < 9; i++){
                        if(board[(n%3)*3+x][((int)(n/3))*3+y].vals[i] && board[(n%3)*3+x][((int)(n/3))*3+y].n==-1){
                            fnd[i]++;
                            ys[i]=((int)(n/3))*3+y;
                            xs[i]=(n%3)*3+x;
                        }
                    }
                }
            }
            for(int i = 0; i < 9; i++){
                if(fnd[i]==1){
                    add(i+1,xs[i],ys[i]);
                    placedx=xs[i];
                    placedy=ys[i];
                    return true;
                }
            }
        }
    return false;
    }

    bool verify(){
        bool line[9];
        bool row[9];
        for(int n = 0; n < 9; n++){
            for(int i = 0; i < 9; i++){
                line[i] = false;
                row[i] = false;
            }
            for(int m = 0; m < 9; m++){
                if(board[n][m].n==-1){return false;}

                line[board[n][m].n-1] = true;
                row[board[m][n].n-1] = true;
            }
            for(int i = 0; i < 9; i++){
                if(!line[i] || !row[i]){
                    return false;
                }
            }
        }
        return true;
    }
};

void draw_outline(){
    for(int k = 0; k < 3; k++){
        for(int y = 0; y < 11; y++){
            move(k*12+y,0);
            for(int i = 0; i < 3; i++){
                if(y%4==3){
                    addstr(" - + - + - ");
                }else if(y%2==1){
                    addstr("   |   |   ");
                }else{
                    addstr("           ");
                }
                if(i < 2){
                    addch(ACS_VLINE);
                }
            }
        }
        move(k*12+11,0);
        if(k < 2){
            for(int n = 0; n < 3; n++){
                for(int i = 0; i < 11; i++){
                    addch(ACS_HLINE);
                }
                if(n < 2){
                    addch(ACS_PLUS);
                }
            }
        }
    }
}

Board openFile(string fname){
    ifstream inf(fname);
    string line;
    Board b;
    int y = 0;
    string chars = "123456789";
    while(getline(inf,line)){
        for(int x = 0; x < 9; x++){
            if(chars.find(line[x])!=string::npos){
                b.add(stoi(string(1,line[x])),x,y);
            }
        }
        y++;
    }
    inf.close();
    return b;
}

int main(int argc, char* argv[]){
    initscr();cbreak();noecho();keypad(stdscr,TRUE);curs_set(0);start_color();//nodelay(stdscr,TRUE);
    init_pair(filled,COLOR_BLACK,COLOR_CYAN);
    init_pair(unfilled,COLOR_BLACK,COLOR_WHITE);
    init_pair(blank,COLOR_BLACK,COLOR_BLACK);
    init_pair(select,COLOR_WHITE,COLOR_RED);
    init_pair(placed,COLOR_WHITE,COLOR_GREEN);
    int cursx = 0;
    int cursy = 0;
    bool noteToggle = false;
    //srand (time(NULL));

    draw_outline();
    Board b = openFile((argc>=2)?argv[1]:FNAME);
    b.draw();
    b.board[cursx][cursy].draw_select();
    refresh();
    int inp;
    while(true){
        inp = getch();
        if(inp == KEY_UP){
            b.board[cursx][cursy].draw();
            if(cursy > 0){
                cursy--;
            }
            b.board[cursx][cursy].draw_select();
        }
        if(inp == KEY_DOWN){
            b.board[cursx][cursy].draw();
            if(cursy < 8){
                cursy++;
            }
            b.board[cursx][cursy].draw_select();
        }
        if(inp == KEY_LEFT){
            b.board[cursx][cursy].draw();
            if(cursx > 0){
                cursx--;
            }
            b.board[cursx][cursy].draw_select();
        }
        if(inp == KEY_RIGHT){
            b.board[cursx][cursy].draw();
            if(cursx < 8){
                cursx++;
            }
            b.board[cursx][cursy].draw_select();
        }
        if(inp == 'q'){
            break;
        }
        if(inp == 'v'){
            if(b.verify()){
                attron(COLOR_PAIR(placed));
                mvaddstr(1,36,"completed");
                attroff(COLOR_PAIR(placed));
            }else{
                attron(COLOR_PAIR(select));
                mvaddstr(1,36,"invalid");
                attroff(COLOR_PAIR(select));
            }
        }
        if(inp == 'f'){
            int fnd = -1;
            for(int i = 0; i < 9; i++){
                if(b.board[cursx][cursy].vals[i]){
                    if(fnd==-1){
                        fnd=i;
                    }else{
                        fnd=-2;
                    }
                }
            }
            if(fnd>=0){
                b.add(fnd+1,cursx,cursy);
            }
        }
        if(inp == 't'){
            noteToggle=!noteToggle;
            attron(COLOR_PAIR(noteToggle?filled:blank));
            mvaddstr(0,36,"Note Mode");
            attroff(COLOR_PAIR(noteToggle?filled:blank));
        }
        if(inp == 'a'){
            b.autoadd();
        }
        if(string("123456789").find(inp)!=string::npos){
            if(noteToggle){
                b.board[cursx][cursy].vals[(int)string("123456789").find(inp)] = !b.board[cursx][cursy].vals[(int)string("123456789").find(inp)];
                b.board[cursx][cursy].draw_select();
            }else{
                b.add((int)string("123456789").find(inp)+1,cursx,cursy);
            }
        }
        if(string("!@#$%^&*(").find(inp)!=string::npos){
            if(noteToggle){
                b.add((int)string("!@#$%^&*(").find(inp)+1,cursx,cursy);
            }else{
                b.board[cursx][cursy].vals[(int)string("!@#$%^&*(").find(inp)] = !b.board[cursx][cursy].vals[(int)string("!@#$%^&*(").find(inp)];
                b.board[cursx][cursy].draw_select();
            }
        }
    }

    endwin();
    return 0;
}
