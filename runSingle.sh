k=EljenSquareNoAtt.root
./wls --fname ${k} -s square -p 3.81 -x 23 -y 23 -n 1000000 --air -b 7 -m 
root -l 'SeePlots.C("'${k}'")'
