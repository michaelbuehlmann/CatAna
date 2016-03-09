#!/bin/bash

PARTITION=dphys_compute
OUTPUT_DIR=~/MT/sfb_decomp_timing
TIME=05:00:00

# A: PARAM_SCALING
if [ "$1" = "A" ]; then
    A_LMAX=( 5 10 20 50 )
    A_NMAX=( 20 60 180 300 )
    A_NSIDE=( 32 64 128 256 )
    A_METHODS=( raw reverse reverse_fft )
    A_THREADS=32
    A_N=2000000

    for lmax in "${A_LMAX[@]}"
    do
        for nmax in "${A_NMAX[@]}"
        do
            for method in "${A_METHODS[@]}"
            do
                if [ "${method}" = "raw" ]; then
                    NAME=A_${lmax}_${nmax}_${method}_${A_N}_${A_THREADS}
                    srun -N1 --exclusive --partition ${PARTITION} --time ${TIME} --job-name ${NAME} \
                        env OMP_NUM_THREADS=${A_THREADS} \
                        "./sfb_time" "${lmax}" "${nmax}" "128" "True" "True" "${method}" "${A_N}" > ${OUTPUT_DIR}/${NAME}.txt &
                else
                    for nside in "${A_NSIDE[@]}"
                    do
                        NAME=A_${lmax}_${nmax}_${method}_${nside}_${A_N}_${A_THREADS}
                        srun -N1 --exclusive --partition ${PARTITION} --time ${TIME} --job-name ${NAME} \
                            env OMP_NUM_THREADS=${A_THREADS} \
                            "./sfb_time" "${lmax}" "${nmax}" "${nside}" "True" "True" "${method}" "${A_N}" > ${OUTPUT_DIR}/${NAME}.txt &
                    done
                fi
            done
        done
    done


# B: MULTITHREAD_SCALING
elif [ "$1" = "B" ]; then
    B_THREADS=( 1 2 4 6 8 10 12 16 20 24 28 32 36 40 )
    B_METHODS=( raw reverse reverse_fft )
    B_N=2000000
    B_NMAX=200
    B_LMAX=50
    B_NSIDE=128

    for n in "${B_THREADS[@]}"
    do
        for method in "${B_METHODS[@]}"
        do
            NAME=B_${B_LMAX}_${B_NMAX}_${method}_${B_NSIDE}_${B_N}_${n}
            srun -N1 --exclusive --partition ${PARTITION} --time ${TIME} --job-name ${NAME} \
                    env OMP_NUM_THREADS=${n} \
                    "./sfb_time" "${B_LMAX}" "${B_NMAX}" "${B_NSIDE}" "True" "True" "${method}" "${B_N}" > ${OUTPUT_DIR}/${NAME}.txt &
        done
    done


# C: N_SCALING
elif [ "$1" = "C" ]; then
    C_N=( 1000 10000 100000 500000 1000000 2000000 )
    C_THREADS=32
    C_METHODS=( raw reverse reverse_fft )
    C_NMAX=200
    C_LMAX=50
    C_NSIDE=128

    for N in "${C_N[@]}"
    do
        for method in "${C_METHODS[@]}"
        do
            NAME=C_${C_LMAX}_${C_NMAX}_${method}_${C_NSIDE}_${N}_${C_THREADS}
            srun -N1 --exclusive --partition ${PARTITION} --time ${TIME} --job-name ${NAME} \
                    env OMP_NUM_THREADS=${C_THREADS} \
                    "./sfb_time" "${C_LMAX}" "${C_NMAX}" "${C_NSIDE}" "True" "True" "${method}" "${N}" > ${OUTPUT_DIR}/${NAME}.txt &
        done
    done


# D: SPEED_IMPROVEMENT
elif [ "$1" = "D" ]; then
    D_N=2000000
    D_LMAX=20
    D_NMAX=100
    D_NSIDE=128

    #1 RAW, 1-CORE, NO_INTERP
    METHOD=raw
    INTERP=False
    THREADS=1
    NAME=D_${D_LMAX}_${D_NMAX}_${METHOD}_${D_NSIDE}_${D_N}_${THREADS}_I${INTERP}
    srun -N1 --exclusive --partition ${PARTITION} --time ${TIME} --job-name ${NAME} \
        env OMP_NUM_THREADS=${THREADS} \
        "./sfb_time" "${D_LMAX}" "${D_NMAX}" "${D_NSIDE}" "True" "${INTERP}" "${METHOD}" "${D_N}" > ${OUTPUT_DIR}/${NAME}.txt &

    #2 RAW, 1-CORE, INTERP
    METHOD=raw
    INTERP=True
    THREADS=1
    NAME=D_${D_LMAX}_${D_NMAX}_${METHOD}_${D_NSIDE}_${D_N}_${THREADS}_I${INTERP}
    srun -N1 --exclusive --partition ${PARTITION} --time ${TIME} --job-name ${NAME} \
        env OMP_NUM_THREADS=${THREADS} \
        "./sfb_time" "${D_LMAX}" "${D_NMAX}" "${D_NSIDE}" "True" "${INTERP}" "${METHOD}" "${D_N}" > ${OUTPUT_DIR}/${NAME}.txt &

    #3 REVERSE, 1-CORE, INTERP
    METHOD=reverse
    INTERP=True
    THREADS=1
    NAME=D_${D_LMAX}_${D_NMAX}_${METHOD}_${D_NSIDE}_${D_N}_${THREADS}_I${INTERP}
    srun -N1 --exclusive --partition ${PARTITION} --time ${TIME} --job-name ${NAME} \
        env OMP_NUM_THREADS=${THREADS} \
        "./sfb_time" "${D_LMAX}" "${D_NMAX}" "${D_NSIDE}" "True" "${INTERP}" "${METHOD}" "${D_N}" > ${OUTPUT_DIR}/${NAME}.txt &

    #4 REVERSE_FFT, 1-CORE, INTERP
    METHOD=reverse_fft
    INTERP=True
    THREADS=1
    NAME=D_${D_LMAX}_${D_NMAX}_${METHOD}_${D_NSIDE}_${D_N}_${THREADS}_I${INTERP}
    srun -N1 --exclusive --partition ${PARTITION} --time ${TIME} --job-name ${NAME} \
        env OMP_NUM_THREADS=${THREADS} \
        "./sfb_time" "${D_LMAX}" "${D_NMAX}" "${D_NSIDE}" "True" "${INTERP}" "${METHOD}" "${D_N}" > ${OUTPUT_DIR}/${NAME}.txt &

    #5 REVERSE_FFT, 40-CORE, INTERP
    METHOD=reverse_fft
    INTERP=True
    THREADS=40
    NAME=D_${D_LMAX}_${D_NMAX}_${METHOD}_${D_NSIDE}_${D_N}_${THREADS}_I${INTERP}
    srun -N1 --exclusive --partition ${PARTITION} --time ${TIME} --job-name ${NAME} \
        env OMP_NUM_THREADS=${THREADS} \
        "./sfb_time" "${D_LMAX}" "${D_NMAX}" "${D_NSIDE}" "True" "${INTERP}" "${METHOD}" "${D_N}" > ${OUTPUT_DIR}/${NAME}.txt &

else
    echo "Argument not understood: $1"
    echo "Use either A,B,C,D"
fi