#!/bin/bash
set +e
set +x

./compileMotorControllerProgram.sh motorControlProgram.cpp
node serverApp.js | ./motorControlProgram
