#!/bin/bash
set -e
set -x
g++ servoController.cpp -o servoController -lwiringPi
