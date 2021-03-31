pathfile="../"
${pathfile}wls --fname EljenBigSquareNoAtt.root -s square -p 3.81 -x 50 -y 50 -n 1000000 --air -m -b 7
${pathfile}wls --fname EljenBigSquareNoAtt_2Ref.root -s square -p 3.81 -x 50 -y 50 -n 1000000 --air -m -b 2
${pathfile}wls --fname EljenBigSquareAtt100.root -s square -p 3.81 -x 50 -y 50 -n 1000000 --air -b 7 -m -a 100 --p3
${pathfile}wls --fname EljenBigSquareAtt200.root -s square -p 3.81 -x 50 -y 50 -n 1000000 --air -b 7 -m -a 200 --p3
${pathfile}wls --fname EljenBigSquareAtt300.root -s square -p 3.81 -x 50 -y 50 -n 1000000 --air -b 7 -m -a 300 --p3
${pathfile}wls --fname EljenBigSquareAtt400.root -s square -p 3.81 -x 50 -y 50 -n 1000000 --air -b 7 -m -a 400 --p3
