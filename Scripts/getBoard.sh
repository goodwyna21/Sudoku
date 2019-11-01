#!/bin/bash

savepath="Boards/APIboard.txt"
#difficulty options: easy medium hard random
if [ -z ${1+"yes"} ]; then 
    diff="hard"; else 
    diff=$1; fi

echo -n "Requesting new board..."
wget -qO $savepath "https://sugoku.herokuapp.com/board?difficulty=$diff"
echo "done"
sed -i 's/],\[/\n/g' $savepath
sed -i 's/{"board":\[\[//' $savepath
sed -i 's/]]}//' $savepath
sed -i 's/,//g' $savepath

./Sudoku.o $savepath
