#! /bin/bash

source=${1-./main.cpp}
binary=${2-./a.out}
np_count=${3-4}

# clear previous binary if exists
rm binary 2> /dev/null

# build binary
echo "build binary"
mpic++ -ldl -lm -o $binary $source 

# run
echo "run program"
mpirun -np $np_count $binary