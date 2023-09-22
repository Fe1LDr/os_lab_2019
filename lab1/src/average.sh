#!/bin/bash
#average.sh
while read arg 
do
    sum=$(( $sum + $arg ))
    count=$(( $count + 1 ))
done
average=$(( $sum / $count ))
echo "Среднее арифметическое = $average"
echo "Количество = $count"