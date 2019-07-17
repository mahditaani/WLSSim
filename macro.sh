#!/bin/bash

./compile.sh ### Compiles the executable

for ((i = 30; i <= 60; i ++));
do
    ./wls $i.0 $i.0 100
done
