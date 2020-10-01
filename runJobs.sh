
# First we simulate the size of the measured plates. Light simulated along a line
./wls -s square -p 3.81 -x 23 -y 23 -b 2 --air --line -m --fname EljenSquareReflectorActual.root
./wls -s square -p 3.81 -x 25 -y 25 -b 2 --air --line -m --fname KuraraySquareReflectorActual.root
./wls -s square -p 3.81 -x 21 -y 21 -b 2 --air --line -m --fname SKODSquareReflectorActual.root

./wls -s square -p 3.81 -x 23 -y 23 -b 2 --air --line --fname EljenSquareActual.root
./wls -s square -p 3.81 -x 25 -y 25 -b 2 --air --line --fname KuraraySquareActual.root

./wls -s rectangle -p 3.81 -x 7.62 -y 36 -b 2 --air --line -m --fname SKODRectanglePetalReflectorActual.root
./wls -s rectangle -p 3.81 -x 21 -y 62 -b 2 --air --line -m --fname SKODRectangleReflectorActual.root
./wls -s rectangle -p 3.81 -x 30 -y 49 -b 2 --air --line -m --fname EljenRectangleReflectorActual.root

./wls -s circle -p 3.81 -x 21 -y 21 -b 2 --air --line -m --fname SKODCircleReflectorActual.root



# Now we simulate the expexted sizes. Light simulated all over the plate.

./wls -s square -p 3.81 -x 50 -y 50 -b 2 --air -m --fname EljenSquareReflectorScaled.root
./wls -s square -p 3.81 -x 50 -y 50 -b 2 --air -m --fname KuraraySquareReflectorScaled.root
./wls -s square -p 3.81 -x 50 -y 50 -b 2 --air -m --fname SKODSquareReflectorScaled.root

./wls -s square -p 3.81 -x 50 -y 50 -b 2 --air --fname EljenSquareScaled.root
./wls -s square -p 3.81 -x 50 -y 50 -b 2 --air --fname KuraraySquareScaled.root

./wls -s square -p 3.81 -x 50 -y 50 -b 2 --air --fname SKODRectanglePetalReflectorScaled.root
./wls -s square -p 3.81 -x 50 -y 50 -b 2 --air --fname SKODRectangleReflectorScaled.root
./wls -s square -p 3.81 -x 50 -y 50 -b 2 --air --fname EljenRectangleReflectorScaled.root

./wls -s circle -p 3.81 -x 50 -y 50 -b 2 --air --fname SKODCircleReflectorScaled.root

