#! /bin/bash

source=${1-./main.c}
binary=${2-./a.out}
np_count=${3-10}

# clear previous binary if exists
rm binary 2> /dev/null

# build binary
mpicc -o $binary $source

# run
mpirun -np $np_count $binary
