#!/bin/bash

for ((i = 20 ; i <= 60; i=i+4));
do
    ./wls -s Circle -p 10. -x $i -y $i -a 100 -n 1000000 --air -v 1 --seed 1236 -b 2 -m
done
