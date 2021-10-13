#!/bin/bash

python3 process.py 10 2 &
python3 process.py 10 2 &

wait

cp resultado.txt test_0/resultado_2.txt
rm resultado.txt

cp coordinator.txt test_0/coordinator.txt
rm coordinator.txt
touch coordinator.txt

for i in 2 4 8 16 32; do
    for j in $(seq $i); do
        python3 process.py 10 2 &
    done
    wait
    cp resultado.txt test_1/resultado_$i.txt
    rm resultado.txt
done

cp coordinator.txt test_1/coordinator.txt
rm coordinator.txt
touch coordinator.txt

for i in 2 4 8 16 32 64; do
    for j in $(seq $i); do
        python3 process.py 5 1 &
    done
    wait
    cp resultado.txt test_2/resultado_$i.txt
    rm resultado.txt
done

cp coordinator.txt test_2/coordinator.txt
rm coordinator.txt
touch coordinator.txt

for i in 2 4 8 16 32 64 128; do
    for j in $(seq $i); do
        python3 process.py 3 0 &
    done
    wait
    cp resultado.txt test_3/resultado_$i.txt
    rm resultado.txt
done

cp coordinator.txt test_3/coordinator.txt
rm coordinator.txt
touch coordinator.txt