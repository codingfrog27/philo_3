#!/bin/bash

# Number of times to run the command
num_runs=10

# Loop to run the command multiple times
for ((i=1; i<=$num_runs; i++))
do
    # Run the command and count the number of lines containing "is eating"
    output=$(./philo 4 410 200 200 10 | grep "is eating" | wc -l)

    # Check if the output is 40 or more
    if [ $output -ge 40 ]; then
        echo "Run $i: Output is $output, which is 40 or more."
    else
        echo "Run $i: Output is $output, which is less than 40. Error!"
    fi
done
