#!/bin/bash
set -ex

whoami
./compileMotorControllerProgram.sh motorControlProgram.cpp
node serverApp.js | ./motorControlProgram
echo "I'm done - last command exited with $?"
