#! /bin/bash

source=$1

binary=${2-a.out}

mpicc -o $binary $source
