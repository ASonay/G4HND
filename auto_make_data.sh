#/bin/bash


step=20
for ((i=650; i<=1000; i++))
do
    PE=$(($i*$step))
    root -l -b -q 'simulation_his.C( '$PE' )'
done
