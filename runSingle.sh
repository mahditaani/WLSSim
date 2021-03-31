#k=EljenCircleAtt100.root
#k=EljenRectangleAtt400.root
k=EljenSquareNoAtt.root
#./wls --fname ${k} -s rectangle -p 3.81 -x 23 -y 46 -n 1000000 --air -b 7 -m 
#./wls --fname ${k} -s rectangle -p 3.81 -x 23 -y 46 -n 1000000 --air -b 2 -m 
#./wls --fname ${k} -s rectangle -p 3.81 -x 23 -y 46 -n 1000000 --air -b 7 -m -a 400 --p3
#./wls --fname ${k} -s square -p 3.81 -x 23 -y 23 -n 1000000 --air -b 7 -m -a 400 --p3
#./wls --fname ${k} -s square -p 3.81 -x 23 -y 23 -n 1000000 --air -b 2  
./wls --fname ${k} -s square -p 3.81 -x 23 -y 23 -n 1000000 --air -m -b 7  
#root -l 'SeePlots.C("'${k}'")'
