#!/bin/bash
if [ -z "$var" ]
then
      echo "\$var is empty"
else
      echo "\$var is NOT empty"
fi
fname="newboard.txt"
touch "Boards/$fname"
echo "*********" > "Boards/$fname"
echo "*********" >> "Boards/$fname"
echo "*********" >> "Boards/$fname"
echo "*********" >> "Boards/$fname"
echo "*********" >> "Boards/$fname"
echo "*********" >> "Boards/$fname"
echo "*********" >> "Boards/$fname"
echo "*********" >> "Boards/$fname"
echo "*********" >> "Boards/$fname"
vim "Boards/$fname"

