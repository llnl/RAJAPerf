window.BENCHMARK_DATA = {
  "lastUpdate": 1774906361679,
  "repoUrl": "https://github.com/llnl/RAJAPerf",
  "entries": {
    "clang_14_0_6_mpi_caliper": [
      {
        "commit": {
          "author": {
            "name": "Steven Smith",
            "username": "smithsg84",
            "email": "smith84@llnl.gov"
          },
          "committer": {
            "name": "GitHub",
            "username": "web-flow",
            "email": "noreply@github.com"
          },
          "id": "4131efc7c49dc473160817ab2666aacef87948f0",
          "message": "Merge pull request #638 from llnl/use-components-caliper-ci\n\nPerformance dashboard in CI",
          "timestamp": "2026-03-30T18:25:38Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/4131efc7c49dc473160817ab2666aacef87948f0"
        },
        "date": 1774902959254,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_Seq-default_Basic_ARRAY_OF_PTRS",
            "value": 0.5036107142857142,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 14.101s"
          },
          {
            "name": "RAJA_Seq-default_Basic_COPY8",
            "value": 0.47130714285714287,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 13.197s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY",
            "value": 0.0537075,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 112, total_time: 15.038s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY_ATOMIC",
            "value": 0.053709285714285716,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 112, total_time: 15.039s"
          },
          {
            "name": "RAJA_Seq-default_Basic_EMPTY",
            "value": 0.0030089285714285713,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 112, total_time: 1.685s"
          },
          {
            "name": "RAJA_Seq-default_Basic_IF_QUAD",
            "value": 0.14256646825396824,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 112, total_time: 14.371s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INDEXLIST_3LOOP",
            "value": 0.11958803571428572,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 112, total_time: 6.697s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT3",
            "value": 0.1563557142857143,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 112, total_time: 43.780s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D",
            "value": 0.030390785714285714,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 112, total_time: 42.547s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.030463071428571426,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 112, total_time: 42.648s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MAT_MAT_SHARED",
            "value": 2.8660142857142854,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 112, total_time: 8.025s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULADDSUB",
            "value": 0.1563530612244898,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 112, total_time: 30.645s"
          },
          {
            "name": "RAJA_Seq-default_Basic_NESTED_INIT",
            "value": 0.030435000000000004,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 112, total_time: 17.044s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_ATOMIC",
            "value": 0.012136357142857141,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.340s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_REDUCE",
            "value": 0.012134321428571427,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.340s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE3_INT",
            "value": 0.00825007142857143,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.231s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE_STRUCT",
            "value": 0.0344485,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.965s"
          },
          {
            "name": "RAJA_Seq-default_Basic_TRAP_INT",
            "value": 0.030393464285714288,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.851s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULTI_REDUCE",
            "value": 0.034986392857142855,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.980s"
          },
          {
            "name": "RAJA_Seq-new_Basic_PI_REDUCE",
            "value": 0.012096607142857142,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.339s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE3_INT",
            "value": 0.004323714285714286,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.121s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE_STRUCT",
            "value": 0.03313,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.928s"
          },
          {
            "name": "RAJA_Seq-new_Basic_TRAP_INT",
            "value": 0.030833392857142854,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.863s"
          }
        ]
      }
    ],
    "clang_14_0_6_openmp_caliper": [
      {
        "commit": {
          "author": {
            "name": "Steven Smith",
            "username": "smithsg84",
            "email": "smith84@llnl.gov"
          },
          "committer": {
            "name": "GitHub",
            "username": "web-flow",
            "email": "noreply@github.com"
          },
          "id": "4131efc7c49dc473160817ab2666aacef87948f0",
          "message": "Merge pull request #638 from llnl/use-components-caliper-ci\n\nPerformance dashboard in CI",
          "timestamp": "2026-03-30T18:25:38Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/4131efc7c49dc473160817ab2666aacef87948f0"
        },
        "date": 1774905785138,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_OpenMP-default_Basic_ARRAY_OF_PTRS",
            "value": 5.01548,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.254s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_COPY8",
            "value": 3.638944,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.910s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY",
            "value": 0.15139439999999998,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.378s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY_ATOMIC",
            "value": 1.6893760000000002,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 4.223s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_EMPTY",
            "value": 0.1020562,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 0.510s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_IF_QUAD",
            "value": 2.0181,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 1.816s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INDEXLIST_3LOOP",
            "value": 1.522502,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.761s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT3",
            "value": 0.7845880000000001,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 1.961s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D",
            "value": 0.20934719999999998,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 2.617s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.1576376,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 1.970s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MAT_MAT_SHARED",
            "value": 155.262,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 3.882s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULADDSUB",
            "value": 0.7821885714285713,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 1.369s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_NESTED_INIT",
            "value": 0.447722,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 2.239s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_ATOMIC",
            "value": 251.3456,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 62.836s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_REDUCE",
            "value": 0.349292,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.087s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE3_INT",
            "value": 0.23506,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.059s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE_STRUCT",
            "value": 0.450288,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.113s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_TRAP_INT",
            "value": 0.859048,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.215s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULTI_REDUCE",
            "value": 0.688904,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.172s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_PI_REDUCE",
            "value": 0.3431068,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.086s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE3_INT",
            "value": 0.1807288,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.045s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE_STRUCT",
            "value": 0.449056,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.112s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_TRAP_INT",
            "value": 0.863776,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.216s"
          },
          {
            "name": "RAJA_Seq-default_Basic_ARRAY_OF_PTRS",
            "value": 37.06376,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 9.266s"
          },
          {
            "name": "RAJA_Seq-default_Basic_COPY8",
            "value": 31.27904,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 7.820s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY",
            "value": 3.947968,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 9.870s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY_ATOMIC",
            "value": 3.948348,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 9.871s"
          },
          {
            "name": "RAJA_Seq-default_Basic_EMPTY",
            "value": 1.318644,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 6.593s"
          },
          {
            "name": "RAJA_Seq-default_Basic_IF_QUAD",
            "value": 34.28422222222222,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 30.856s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INDEXLIST_3LOOP",
            "value": 26.271,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 13.136s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT3",
            "value": 11.58148,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 28.954s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D",
            "value": 2.384272,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 29.803s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 2.990728,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 37.384s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MAT_MAT_SHARED",
            "value": 2571.212,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 64.280s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULADDSUB",
            "value": 12.616685714285715,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 22.079s"
          },
          {
            "name": "RAJA_Seq-default_Basic_NESTED_INIT",
            "value": 3.18304,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 15.915s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_ATOMIC",
            "value": 5.33488,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.334s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_REDUCE",
            "value": 5.3364,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.334s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE3_INT",
            "value": 3.117112,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.779s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE_STRUCT",
            "value": 7.13868,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.785s"
          },
          {
            "name": "RAJA_Seq-default_Basic_TRAP_INT",
            "value": 13.65708,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.414s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULTI_REDUCE",
            "value": 11.12836,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 2.782s"
          },
          {
            "name": "RAJA_Seq-new_Basic_PI_REDUCE",
            "value": 5.33532,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.334s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE3_INT",
            "value": 1.294908,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.324s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE_STRUCT",
            "value": 6.21168,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.553s"
          },
          {
            "name": "RAJA_Seq-new_Basic_TRAP_INT",
            "value": 13.37188,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.343s"
          }
        ]
      }
    ],
    "gcc_10_3_1_openmp_caliper": [
      {
        "commit": {
          "author": {
            "name": "Steven Smith",
            "username": "smithsg84",
            "email": "smith84@llnl.gov"
          },
          "committer": {
            "name": "GitHub",
            "username": "web-flow",
            "email": "noreply@github.com"
          },
          "id": "4131efc7c49dc473160817ab2666aacef87948f0",
          "message": "Merge pull request #638 from llnl/use-components-caliper-ci\n\nPerformance dashboard in CI",
          "timestamp": "2026-03-30T18:25:38Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/4131efc7c49dc473160817ab2666aacef87948f0"
        },
        "date": 1774906359253,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_OpenMP-default_Basic_ARRAY_OF_PTRS",
            "value": 4.43332,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.108s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_COPY8",
            "value": 3.646376,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.912s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY",
            "value": 0.16553479999999998,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.414s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY_ATOMIC",
            "value": 1.6076160000000002,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 4.019s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_EMPTY",
            "value": 0.0481354,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 0.241s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_IF_QUAD",
            "value": 1.9532555555555555,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 1.758s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INDEXLIST_3LOOP",
            "value": 1.651004,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.826s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT3",
            "value": 0.7506079999999999,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 1.877s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D",
            "value": 0.1040384,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 1.300s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.10327199999999999,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 1.291s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MAT_MAT_SHARED",
            "value": 198.85119999999998,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 4.971s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULADDSUB",
            "value": 0.76944,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 1.347s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_NESTED_INIT",
            "value": 0.1884136,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 0.942s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_ATOMIC",
            "value": 271.8428,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 67.961s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_REDUCE",
            "value": 0.355542,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.089s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE3_INT",
            "value": 0.1168152,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.029s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE_STRUCT",
            "value": 0.3810692,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.095s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_TRAP_INT",
            "value": 0.86342,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.216s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULTI_REDUCE",
            "value": 0.68832,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.172s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_PI_REDUCE",
            "value": 0.3501432,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.088s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE3_INT",
            "value": 0.1027328,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.026s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE_STRUCT",
            "value": 0.3512824,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.088s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_TRAP_INT",
            "value": 0.85648,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.214s"
          },
          {
            "name": "RAJA_Seq-default_Basic_ARRAY_OF_PTRS",
            "value": 67.5016,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 16.875s"
          },
          {
            "name": "RAJA_Seq-default_Basic_COPY8",
            "value": 38.74504,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 9.686s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY",
            "value": 4.10116,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 10.253s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY_ATOMIC",
            "value": 4.1018799999999995,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 10.255s"
          },
          {
            "name": "RAJA_Seq-default_Basic_EMPTY",
            "value": 1.32168,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 6.608s"
          },
          {
            "name": "RAJA_Seq-default_Basic_IF_QUAD",
            "value": 32.288222222222224,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 29.059s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INDEXLIST_3LOOP",
            "value": 28.9962,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 14.498s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT3",
            "value": 10.9874,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 27.468s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D",
            "value": 2.9916,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 37.395s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 2.976704,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 37.209s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MAT_MAT_SHARED",
            "value": 3034.3,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 75.858s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULADDSUB",
            "value": 11.34622857142857,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 19.856s"
          },
          {
            "name": "RAJA_Seq-default_Basic_NESTED_INIT",
            "value": 3.53546,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 17.677s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_ATOMIC",
            "value": 5.277520000000001,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.319s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_REDUCE",
            "value": 10.5648,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 2.641s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE3_INT",
            "value": 2.71934,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.680s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE_STRUCT",
            "value": 6.93456,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.734s"
          },
          {
            "name": "RAJA_Seq-default_Basic_TRAP_INT",
            "value": 13.30584,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.326s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULTI_REDUCE",
            "value": 10.84368,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 2.711s"
          },
          {
            "name": "RAJA_Seq-new_Basic_PI_REDUCE",
            "value": 5.355600000000001,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.339s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE3_INT",
            "value": 1.449548,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.362s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE_STRUCT",
            "value": 12.25176,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.063s"
          },
          {
            "name": "RAJA_Seq-new_Basic_TRAP_INT",
            "value": 13.24128,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.310s"
          }
        ]
      }
    ]
  }
}