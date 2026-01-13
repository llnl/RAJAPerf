#!/usr/bin/env bash

## Run all benchmark kernels for GPU, non-lambda variants only
## on 4 MPI ranks and dump the results in the specified directory.

flux alloc -xN1 -t 20 bash -c '

OUTDIR=RPBenchmarkTestMPI

# Collection of problem size factors between 0.5 and 6
FACTORS=(0.5 1.0 2.0 3.0 4.0 5.0 6.0)

# List of kernels to run
KERNELS=("CONVECTION3DPA"
         "DEL_DOT_VEC_2D"
         "DIFFUSION3DPA"
         "EDGE3D"
         "ENERGY"
         "INTSC_HEXHEX"
         "INTSC_HEXRECT"
         "LTIMES"
         "MASS3DEA"
         "MASSVEC3DPA"
         "MATVEC_3D_STENCIL"
         "NODAL_ACCUMULATION_3D"
         "VOL3D"
         "MULTI_REDUCE"
         "REDUCE_STRUCT"
         "HALO_EXCHANGE_FUSED")

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
