#!/bin/bash

# Number of times to run the command
num_runs=10
meals_to_eat=5
nbr_of_philos=4
req_prints=$((meals_to_eat * nbr_of_philos))

# Loop to run the command multiple times
for ((i=1; i<=$num_runs; i++))
do
    # Run the command and count the number of lines containing "is eating"
    output=$(./philo $nbr_of_philos 410 200 200 $meals_to_eat | grep "is eating" | wc -l)

    # Check if the output is 40 or more
    if [ $output -lt $req_prints ]; then
        echo "ERROR Run $i: printed $output times, should've been at least $req_prints."
	# else
	# 	echo "run $i SUCCESS: printed $output times, (min == $req_prints)"
    fi
done
