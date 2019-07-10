#!/bin/bash

./compile.sh ### Compiles the executable

<<<<<<< HEAD
=======
touch data.txt  # Creates an empty txt file IF it does not exits
echo > data.txt # Empties the txt file

for ((i = 20; i <= 320; i += 10));
>>>>>>> 13bd10442debaa5ed2177701db5091614364ac5f
do
    ./wls 50.0 50.0 $i.0
done
