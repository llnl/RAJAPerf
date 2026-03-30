window.BENCHMARK_DATA = {
  "lastUpdate": 1774902961489,
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
    ]
  }
}