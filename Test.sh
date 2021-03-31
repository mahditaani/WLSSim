
# First we simulate the size of the measured plates. Light simulated along a line
./wls -s square -p 3.81 -x 23 -y 23 -b 7 --air --line -m --p3 -a 700 --fname EljenSquareReflector700a.root
./wls -s square -p 3.81 -x 25 -y 25 -b 7 --air --line -m --p3 -a 700 --fname KuraraySquareReflector700a.root
./wls -s square -p 3.81 -x 21 -y 21 -b 7 --air --line -m --p3 -a 700 --fname SKODSquareReflector700a.root

./wls -s square -p 3.81 -x 23 -y 23 -b 7 --air --line -m --p3 -a 200 --fname EljenSquareReflector200a.root
./wls -s square -p 3.81 -x 25 -y 25 -b 7 --air --line -m --p3 -a 200 --fname KuraraySquareReflector200a.root
./wls -s square -p 3.81 -x 21 -y 21 -b 7 --air --line -m --p3 -a 200 --fname SKODSquareReflector200a.root

