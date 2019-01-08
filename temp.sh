#!/bin/bash
DEV=/dev/ttyACM0
FILE=temp.csv

cat $DEV &>/dev/null
socat stdio $DEV | tee -a $FILE &
sleep 2
date -d'+1 hour' '+T%s\n' >$DEV
wait
