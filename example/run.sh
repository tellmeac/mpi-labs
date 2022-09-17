#! /bin/bash

source=${1-./main.c}
binary=${2-./a.out}

# clear previous binary if exists
rm binary 2> /dev/null

# build binary
mpicc -o $binary $source

# run
mpirun -np 4 $binary
