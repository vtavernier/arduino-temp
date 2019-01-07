#!/bin/bash
DEV=/dev/ttyACM0
FILE=temp.csv

echo >temp.gpi <<GNUPLOT
set xdata time
set timefmt '%Y-%m-%dT%H:%M:%S'
set format x "%H:%M %d"
set yrange [20:35]

plot '$FILE' u 1:(\$2/10) w l
GNUPLOT

cat $DEV &>/dev/null
socat stdio $DEV | tee -a $FILE &
sleep 2
date -d'+1 hour' '+T%s\n' >$DEV
wait
