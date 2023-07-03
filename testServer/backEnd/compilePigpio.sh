#!/bin/bash 

set -e

echo Compiling $1

outname=${1%.*}

set -x
g++ $1 -o $outname -lpigpio
