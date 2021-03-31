# Now we simulate the expexted sizes. Light simulated all over the plate.

../wls -s square -p 3.81 -x 50 -y 50 -b 7 --air -m -a 200 --fname 50x50a.root
../wls -s square -p 3.81 -x 50 -y 50 -b 7 --air -m -a 200 --p3 --fname 50x50ap.root

#../wls -s square -p 3.81 -x 60 -y 60 -b 2 --air -m --fname 60x60.root
../wls -s square -p 3.81 -x 50 -y 50 -b 2 --air -m --fname 50x50.root
#../wls -s square -p 3.81 -x 40 -y 40 -b 2 --air -m --fname 40x40.root
#../wls -s square -p 3.81 -x 30 -y 30 -b 2 --air -m --fname 30x30.root
