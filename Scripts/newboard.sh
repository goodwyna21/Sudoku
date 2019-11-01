#!/bin/bash
if [ -z "$1" ]
then
      fname="newboard.txt"
else
      fname="$1"
fi
var="/home/archer/Projects/Small/Sudoku/Boards/$fname"
touch "$var"
echo "*********" > "$var"
echo "*********" >> "$var"
echo "*********" >> "$var"
echo "*********" >> "$var"
echo "*********" >> "$var"
echo "*********" >> "$var"
echo "*********" >> "$var"
echo "*********" >> "$var"
echo "*********" >> "$var"
vim "$var"

