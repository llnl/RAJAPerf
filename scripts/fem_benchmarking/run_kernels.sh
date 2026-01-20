#!/usr/bin/env bash

## Run all benchmark kernels for GPU, non-lambda variants only
## on 4 MPI ranks and dump the results in the specified directory.
rm -rf RPBenchmarkTestMPI
flux alloc -xN1 -t 20 bash -c '

OUTDIR=RPBenchmarkTestMPI

# Collection of problem size factors between 0.5 and 6
FACTORS=(0.5 1.0 2.0 3.0 4.0 5.0 6.0)

#FACTORS=(4.0)

# List of kernels to run
KERNELS=("MASS3DEA" "DIFFUSION3DPA" "MASS3DPA_ATOMIC" "MASSVEC3DPA" "CONVECTION3DPA" "MASS3DPA")

for KERNEL_NAME in "${KERNELS[@]}"; do
    echo "Running kernel: $KERNEL_NAME"

    for factor in "${FACTORS[@]}"; do
        echo "  Running with sizefact = $factor"
        flux run -xN1 -n4 ./bin/raja-perf.exe \
            -k "$KERNEL_NAME" \
            --npasses 1 \
            --npasses-combiners Average Minimum Maximum \
            --outdir ${OUTDIR} \
            --outfile "${KERNEL_NAME}_factor_${factor}" \
            --sizefact "$factor" \
            --warmup-perfrun-same \
            -ev Seq Lambda
    done
done
'
