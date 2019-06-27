#!/bin/bash

./compile.sh ### Compiles the executable

for i in {26..110}
do
    ./wls $i.0 $i.0
done
