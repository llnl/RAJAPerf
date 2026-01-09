#!/usr/bin/env bash

#Clean directory
rm -rf *.csv *.txt

# Collection of float factors between 0.5 and 10
FACTORS=(0.5 1.0 2.5 5.0 7.5 10.0)

# List of kernels to run
KERNELS=("MASS3DPA" "DEL_DOT_VEC_2D")

for KERNEL_NAME in "${KERNELS[@]}"; do
    echo "Running kernel: $KERNEL_NAME"

    for factor in "${FACTORS[@]}"; do
        echo "  Running with sizefact = $factor"
        ./bin/raja-perf.exe \
            -k "$KERNEL_NAME" \
            --npasses 3 \
            --npasses-combiners Average Minimum Maximum \
            --outfile "${KERNEL_NAME}_factor_${factor}" \
            --sizefact "$factor" \
            --warmup-perfrun-same
    done
done
