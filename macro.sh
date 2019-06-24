#!/bin/bash

./compile.sh ### Compiles the executable

touch data.txt  # Creates an empty txt file IF it does not exits
echo > data.txt # Empties the txt file

for i in {62..62}
do
    ./wls $i.0 $i.0
done
