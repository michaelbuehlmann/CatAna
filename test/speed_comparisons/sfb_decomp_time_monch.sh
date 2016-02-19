#!/bin/bash

PARTITION=dphys_compute
OUTPUT_DIR=~/MT/sfb_decomp_timing
TIME=02:00:00

LMAX=10
A_NMAX=( 20 40 80 160 )
A_NSIDE=( 32 64 128 256 )
A_CORES=( 1 2 4 8 16 32 )
A_METHODS=( raw hp hp_fft )

for n in "${A_CORES[@]}"
do
    for nside in "${A_NSIDE[@]}"
    do
        for nmax in "${A_NMlAX[@]}"
        do
            for method in "${A_METHODS[@]}"
            do
                echo ${n} ${nside} ${nmax} ${method}
                NAME=${n}_${nmax}_${nside}_${method}
                srun -n ${n} --exclusive --partition ${PARTITION} --time ${TIME} --job-name ${NAME} \
                    env OMP_NUM_THREADS=${n} \
                    "./sfb_time" "${LMAX}" "${nmax}" "${nside}" "${method}"  > ${OUTPUT_DIR}/n${n}_nmax${nmax}_nside${nside}_${method}.txt &
            done
        done

    done
done
