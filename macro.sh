#!/bin/bash

./compile.sh ### Compiles the executable

for ((i = 30; i <= 60; i ++));
do
    ./wls -s Square -p 3.8 -x $i -y $i -a 100 -n 1000000 --air -v --seed 1236 -b 2
done
