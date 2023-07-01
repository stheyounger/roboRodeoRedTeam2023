#!/bin/bash
set -e
set -x
while :
do
 POSITION=$[ $RANDOM % 100 + 0 ]
./servoController 1 $POSITION
sleep .005
done
