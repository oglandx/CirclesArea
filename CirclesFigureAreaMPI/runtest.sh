#!/bin/sh

max_cores=6

for i in `seq 1 $max_cores`
do
	mpirun -np $i ./build/dist/CirclesFigureArea
done
