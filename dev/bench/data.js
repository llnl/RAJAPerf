window.BENCHMARK_DATA = {
  "lastUpdate": 1773168185129,
  "repoUrl": "https://github.com/llnl/RAJAPerf",
  "entries": {
    "clang_14_0_6_mpi_caliper": [
      {
        "commit": {
          "author": {
            "name": "Adrien M. BERNEDE",
            "username": "adrienbernede",
            "email": "51493078+adrienbernede@users.noreply.github.com"
          },
          "committer": {
            "name": "Adrien M. BERNEDE",
            "username": "adrienbernede",
            "email": "51493078+adrienbernede@users.noreply.github.com"
          },
          "id": "36517681508b47429f1380c4d698c87986125922",
          "message": "Merge branch 'develop' into woptim/caliper-ci",
          "timestamp": "2025-11-24T13:25:35Z",
          "url": "https://github.com/LLNL/RAJAPerf/commit/36517681508b47429f1380c4d698c87986125922"
        },
        "date": 1763996171475,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_Seq-default_Basic_ARRAY_OF_PTRS",
            "value": 9.626,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-default_Basic_COPY8",
            "value": 8.73552,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY",
            "value": 0.6821480000000001,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY_ATOMIC",
            "value": 0.68628,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-default_Basic_EMPTY",
            "value": 0.26454399999999995,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-default_Basic_IF_QUAD",
            "value": 6.9701666666666675,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-default_Basic_INDEXLIST_3LOOP",
            "value": 4.598080000000001,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT3",
            "value": 3.1048,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D",
            "value": 0.476548,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.35820480000000005,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-default_Basic_MAT_MAT_SHARED",
            "value": 175.31799999999998,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULADDSUB",
            "value": 3.1181142857142854,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-default_Basic_NESTED_INIT",
            "value": 0.571552,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_ATOMIC",
            "value": 1.06693,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_REDUCE",
            "value": 1.0673,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE3_INT",
            "value": 0.236598,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE_STRUCT",
            "value": 1.525686,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-default_Basic_TRAP_INT",
            "value": 2.76896,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULTI_REDUCE",
            "value": 2.28768,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-new_Basic_PI_REDUCE",
            "value": 1.067056,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE3_INT",
            "value": 0.236842,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE_STRUCT",
            "value": 1.33719,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-new_Basic_TRAP_INT",
            "value": 2.76858,
            "unit": "ms/rep"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "Tara Drwenski",
            "username": "tdrwenski",
            "email": "drwenski1@llnl.gov"
          },
          "committer": {
            "name": "Tara Drwenski",
            "username": "tdrwenski",
            "email": "drwenski1@llnl.gov"
          },
          "id": "310c67d9054afbb06868828ee33f5ea88ce02c21",
          "message": "Fix environment variable",
          "timestamp": "2026-02-11T23:37:34Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/310c67d9054afbb06868828ee33f5ea88ce02c21"
        },
        "date": 1771002351865,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_Seq-default_Basic_EMPTY",
            "value": 0.016705249999999998,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 16, total_time: 1.336s"
          },
          {
            "name": "RAJA_Seq-default_Basic_IF_QUAD",
            "value": 0.46129791666666664,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 16, total_time: 6.643s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INDEXLIST_3LOOP",
            "value": 0.28997625,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 16, total_time: 2.320s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT3",
            "value": 0.23203849999999998,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 16, total_time: 9.282s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D",
            "value": 0.02997855,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 16, total_time: 5.996s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.0222601,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 16, total_time: 4.452s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MAT_MAT_SHARED",
            "value": 11.992449999999998,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 16, total_time: 4.797s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULADDSUB",
            "value": 0.22080428571428573,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 16, total_time: 6.183s"
          },
          {
            "name": "RAJA_Seq-default_Basic_NESTED_INIT",
            "value": 0.03622600000000001,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 16, total_time: 2.898s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_ATOMIC",
            "value": 0.067336,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 16, total_time: 0.269s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_REDUCE",
            "value": 0.06669775,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 16, total_time: 0.267s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE3_INT",
            "value": 0.03679675,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 16, total_time: 0.147s"
          },
          {
            "name": "RAJA_Seq-default_Basic_TRAP_INT",
            "value": 0.171181,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 16, total_time: 0.685s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULTI_REDUCE",
            "value": 0.1416865,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 16, total_time: 0.567s"
          },
          {
            "name": "RAJA_Seq-new_Basic_PI_REDUCE",
            "value": 0.06672075,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 16, total_time: 0.267s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE3_INT",
            "value": 0.014428725,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 16, total_time: 0.058s"
          },
          {
            "name": "RAJA_Seq-new_Basic_TRAP_INT",
            "value": 0.17139225,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 16, total_time: 0.686s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "Tara Drwenski",
            "username": "tdrwenski",
            "email": "drwenski1@llnl.gov"
          },
          "committer": {
            "name": "Tara Drwenski",
            "username": "tdrwenski",
            "email": "drwenski1@llnl.gov"
          },
          "id": "e6f5aed9aa87888f68cb92d755cb04fbc7e80009",
          "message": "Rename variable in caliper to json script",
          "timestamp": "2026-02-13T17:19:15Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/e6f5aed9aa87888f68cb92d755cb04fbc7e80009"
        },
        "date": 1771009565059,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_Seq-default_Basic_EMPTY",
            "value": 0.01654375,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 16, total_time: 1.323s"
          },
          {
            "name": "RAJA_Seq-default_Basic_IF_QUAD",
            "value": 0.45779166666666665,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 16, total_time: 6.592s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INDEXLIST_3LOOP",
            "value": 0.28829,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 16, total_time: 2.306s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT3",
            "value": 0.19632975,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 16, total_time: 7.853s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D",
            "value": 0.029718150000000002,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 16, total_time: 5.944s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.02231255,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 16, total_time: 4.463s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MAT_MAT_SHARED",
            "value": 10.944799999999997,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 16, total_time: 4.378s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULADDSUB",
            "value": 0.19611464285714286,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 16, total_time: 5.491s"
          },
          {
            "name": "RAJA_Seq-default_Basic_NESTED_INIT",
            "value": 0.03594125,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 16, total_time: 2.875s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_ATOMIC",
            "value": 0.0667235,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 16, total_time: 0.267s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_REDUCE",
            "value": 0.0667195,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 16, total_time: 0.267s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE3_INT",
            "value": 0.03699,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 16, total_time: 0.148s"
          },
          {
            "name": "RAJA_Seq-default_Basic_TRAP_INT",
            "value": 0.16721075,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 16, total_time: 0.669s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULTI_REDUCE",
            "value": 0.1340915,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 16, total_time: 0.536s"
          },
          {
            "name": "RAJA_Seq-new_Basic_PI_REDUCE",
            "value": 0.06672825,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 16, total_time: 0.267s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE3_INT",
            "value": 0.013665625,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 16, total_time: 0.055s"
          },
          {
            "name": "RAJA_Seq-new_Basic_TRAP_INT",
            "value": 0.167128,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 16, total_time: 0.669s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "Tara Drwenski",
            "username": "tdrwenski",
            "email": "drwenski1@llnl.gov"
          },
          "committer": {
            "name": "Tara Drwenski",
            "username": "tdrwenski",
            "email": "drwenski1@llnl.gov"
          },
          "id": "a527f32a46b0afbde514b7905da77b52e8bb253a",
          "message": "Exclude unstable tests",
          "timestamp": "2026-03-09T18:08:34Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/a527f32a46b0afbde514b7905da77b52e8bb253a"
        },
        "date": 1773095546123,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_Seq-default_Basic_ARRAY_OF_PTRS",
            "value": 0.49978214285714284,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 13.994s"
          },
          {
            "name": "RAJA_Seq-default_Basic_COPY8",
            "value": 0.4695785714285714,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 13.148s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY",
            "value": 0.053868571428571425,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 112, total_time: 15.083s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY_ATOMIC",
            "value": 0.05350821428571429,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 112, total_time: 14.982s"
          },
          {
            "name": "RAJA_Seq-default_Basic_EMPTY",
            "value": 0.0030077142857142854,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 112, total_time: 1.684s"
          },
          {
            "name": "RAJA_Seq-default_Basic_IF_QUAD",
            "value": 0.14283333333333334,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 112, total_time: 14.398s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INDEXLIST_3LOOP",
            "value": 0.12131571428571429,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 112, total_time: 6.794s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT3",
            "value": 0.15598607142857143,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 112, total_time: 43.676s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D",
            "value": 0.030416999999999996,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 112, total_time: 42.584s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.030526714285714285,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 112, total_time: 42.737s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MAT_MAT_SHARED",
            "value": 2.8503857142857147,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 112, total_time: 7.981s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULADDSUB",
            "value": 0.1560076530612245,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 112, total_time: 30.578s"
          },
          {
            "name": "RAJA_Seq-default_Basic_NESTED_INIT",
            "value": 0.030440178571428573,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 112, total_time: 17.047s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_ATOMIC",
            "value": 0.012139142857142855,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.340s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_REDUCE",
            "value": 0.012125428571428572,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.340s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE3_INT",
            "value": 0.008291999999999999,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.232s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE_STRUCT",
            "value": 0.034239035714285715,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.959s"
          },
          {
            "name": "RAJA_Seq-default_Basic_TRAP_INT",
            "value": 0.03044567857142857,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.852s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULTI_REDUCE",
            "value": 0.03512075,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.983s"
          },
          {
            "name": "RAJA_Seq-new_Basic_PI_REDUCE",
            "value": 0.012097499999999999,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.339s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE3_INT",
            "value": 0.004135821428571429,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.116s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE_STRUCT",
            "value": 0.032541964285714285,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.911s"
          },
          {
            "name": "RAJA_Seq-new_Basic_TRAP_INT",
            "value": 0.030530571428571428,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.855s"
          }
        ]
      }
    ],
    "gcc_10_3_1_openmp_caliper": [
      {
        "commit": {
          "author": {
            "name": "Adrien M. BERNEDE",
            "username": "adrienbernede",
            "email": "51493078+adrienbernede@users.noreply.github.com"
          },
          "committer": {
            "name": "Adrien M. BERNEDE",
            "username": "adrienbernede",
            "email": "51493078+adrienbernede@users.noreply.github.com"
          },
          "id": "36517681508b47429f1380c4d698c87986125922",
          "message": "Merge branch 'develop' into woptim/caliper-ci",
          "timestamp": "2025-11-24T13:25:35Z",
          "url": "https://github.com/LLNL/RAJAPerf/commit/36517681508b47429f1380c4d698c87986125922"
        },
        "date": 1763996173134,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_OpenMP-default_Basic_ARRAY_OF_PTRS",
            "value": 0.636712,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_COPY8",
            "value": 0.25655,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY",
            "value": 0.023474000000000002,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY_ATOMIC",
            "value": 0.322934,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_EMPTY",
            "value": 0.0231742,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_IF_QUAD",
            "value": 0.37247444444444444,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INDEXLIST_3LOOP",
            "value": 0.342561,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT3",
            "value": 0.09104499999999999,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D",
            "value": 0.0263312,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.02631236,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MAT_MAT_SHARED",
            "value": 13.37622,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULADDSUB",
            "value": 0.091616,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_NESTED_INIT",
            "value": 0.0435142,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_ATOMIC",
            "value": 57.972,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_REDUCE",
            "value": 0.0831448,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE3_INT",
            "value": 0.0418344,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE_STRUCT",
            "value": 0.1030568,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_TRAP_INT",
            "value": 0.1820694,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULTI_REDUCE",
            "value": 0.1509836,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_PI_REDUCE",
            "value": 0.0807844,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE3_INT",
            "value": 0.0323642,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE_STRUCT",
            "value": 0.0810324,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_TRAP_INT",
            "value": 0.1799954,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-default_Basic_ARRAY_OF_PTRS",
            "value": 8.4128,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-default_Basic_COPY8",
            "value": 5.87198,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY",
            "value": 0.461398,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY_ATOMIC",
            "value": 0.46141400000000005,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-default_Basic_EMPTY",
            "value": 0.264453,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-default_Basic_IF_QUAD",
            "value": 6.409555555555556,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-default_Basic_INDEXLIST_3LOOP",
            "value": 5.25791,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT3",
            "value": 1.7536880000000001,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D",
            "value": 0.3090928,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.50352,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-default_Basic_MAT_MAT_SHARED",
            "value": 210.122,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULADDSUB",
            "value": 1.7668314285714286,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-default_Basic_NESTED_INIT",
            "value": 0.53332,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_ATOMIC",
            "value": 1.05747,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_REDUCE",
            "value": 2.11608,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE3_INT",
            "value": 0.566546,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE_STRUCT",
            "value": 1.228762,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-default_Basic_TRAP_INT",
            "value": 2.72888,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULTI_REDUCE",
            "value": 2.14364,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-new_Basic_PI_REDUCE",
            "value": 1.064924,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE3_INT",
            "value": 0.288402,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE_STRUCT",
            "value": 2.4479,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-new_Basic_TRAP_INT",
            "value": 2.66062,
            "unit": "ms/rep"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "Tara Drwenski",
            "username": "tdrwenski",
            "email": "drwenski1@llnl.gov"
          },
          "committer": {
            "name": "Tara Drwenski",
            "username": "tdrwenski",
            "email": "drwenski1@llnl.gov"
          },
          "id": "310c67d9054afbb06868828ee33f5ea88ce02c21",
          "message": "Fix environment variable",
          "timestamp": "2026-02-11T23:37:34Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/310c67d9054afbb06868828ee33f5ea88ce02c21"
        },
        "date": 1771002354036,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY_ATOMIC",
            "value": 0.3247484,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.812s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_IF_QUAD",
            "value": 0.37053888888888886,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 0.333s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INDEXLIST_3LOOP",
            "value": 0.342416,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.171s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MAT_MAT_SHARED",
            "value": 12.904919999999999,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 0.323s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_NESTED_INIT",
            "value": 0.049948599999999996,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 0.250s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_TRAP_INT",
            "value": 0.1829796,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.046s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULTI_REDUCE",
            "value": 0.1503604,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.038s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_TRAP_INT",
            "value": 0.1785384,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.045s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY_ATOMIC",
            "value": 0.464812,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 1.162s"
          },
          {
            "name": "RAJA_Seq-default_Basic_IF_QUAD",
            "value": 6.232544444444445,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 5.609s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INDEXLIST_3LOOP",
            "value": 5.00456,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 2.502s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MAT_MAT_SHARED",
            "value": 218.09599999999998,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 5.452s"
          },
          {
            "name": "RAJA_Seq-default_Basic_NESTED_INIT",
            "value": 0.533356,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 2.667s"
          },
          {
            "name": "RAJA_Seq-default_Basic_TRAP_INT",
            "value": 2.729492,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.682s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULTI_REDUCE",
            "value": 2.143692,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.536s"
          },
          {
            "name": "RAJA_Seq-new_Basic_TRAP_INT",
            "value": 2.668716,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.667s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "Tara Drwenski",
            "username": "tdrwenski",
            "email": "drwenski1@llnl.gov"
          },
          "committer": {
            "name": "Tara Drwenski",
            "username": "tdrwenski",
            "email": "drwenski1@llnl.gov"
          },
          "id": "e6f5aed9aa87888f68cb92d755cb04fbc7e80009",
          "message": "Rename variable in caliper to json script",
          "timestamp": "2026-02-13T17:19:15Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/e6f5aed9aa87888f68cb92d755cb04fbc7e80009"
        },
        "date": 1771009590467,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY_ATOMIC",
            "value": 0.3300204,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.825s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_IF_QUAD",
            "value": 0.3742488888888889,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 0.337s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INDEXLIST_3LOOP",
            "value": 0.356934,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.178s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MAT_MAT_SHARED",
            "value": 13.57528,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 0.339s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_NESTED_INIT",
            "value": 0.048424,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 0.242s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_TRAP_INT",
            "value": 0.185406,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.046s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULTI_REDUCE",
            "value": 0.1554544,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.039s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_TRAP_INT",
            "value": 0.1831716,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.046s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY_ATOMIC",
            "value": 0.443812,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 1.110s"
          },
          {
            "name": "RAJA_Seq-default_Basic_IF_QUAD",
            "value": 6.445433333333334,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 5.801s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INDEXLIST_3LOOP",
            "value": 4.94118,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 2.471s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MAT_MAT_SHARED",
            "value": 208.81759999999997,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 5.220s"
          },
          {
            "name": "RAJA_Seq-default_Basic_NESTED_INIT",
            "value": 0.53316,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 2.666s"
          },
          {
            "name": "RAJA_Seq-default_Basic_TRAP_INT",
            "value": 2.7178,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.679s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULTI_REDUCE",
            "value": 2.137008,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.534s"
          },
          {
            "name": "RAJA_Seq-new_Basic_TRAP_INT",
            "value": 2.689636,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.672s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "Tara Drwenski",
            "username": "tdrwenski",
            "email": "drwenski1@llnl.gov"
          },
          "committer": {
            "name": "Tara Drwenski",
            "username": "tdrwenski",
            "email": "drwenski1@llnl.gov"
          },
          "id": "a527f32a46b0afbde514b7905da77b52e8bb253a",
          "message": "Exclude unstable tests",
          "timestamp": "2026-03-09T18:08:34Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/a527f32a46b0afbde514b7905da77b52e8bb253a"
        },
        "date": 1773096015185,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_OpenMP-default_Basic_COPY8",
            "value": 0.2805724,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.070s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY_ATOMIC",
            "value": 0.32482160000000004,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.812s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_IF_QUAD",
            "value": 0.37319111111111114,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 0.336s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INDEXLIST_3LOOP",
            "value": 0.344746,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.172s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MAT_MAT_SHARED",
            "value": 13.4406,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 0.336s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULADDSUB",
            "value": 0.09239542857142857,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 0.162s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_REDUCE",
            "value": 0.0836524,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.021s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE_STRUCT",
            "value": 0.1069644,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.027s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULTI_REDUCE",
            "value": 0.1518336,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.038s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_PI_REDUCE",
            "value": 0.0808952,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.020s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE_STRUCT",
            "value": 0.0803388,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.020s"
          },
          {
            "name": "RAJA_Seq-default_Basic_COPY8",
            "value": 5.62668,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.407s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY_ATOMIC",
            "value": 0.46707599999999994,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 1.168s"
          },
          {
            "name": "RAJA_Seq-default_Basic_IF_QUAD",
            "value": 6.269555555555555,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 5.643s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INDEXLIST_3LOOP",
            "value": 4.83082,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 2.415s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MAT_MAT_SHARED",
            "value": 206.56400000000002,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 5.164s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULADDSUB",
            "value": 1.7206057142857143,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 3.011s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_REDUCE",
            "value": 2.112664,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.528s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE_STRUCT",
            "value": 1.17962,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.295s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULTI_REDUCE",
            "value": 2.147108,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.537s"
          },
          {
            "name": "RAJA_Seq-new_Basic_PI_REDUCE",
            "value": 1.071112,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.268s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE_STRUCT",
            "value": 2.445584,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.611s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "Tara Drwenski",
            "username": "tdrwenski",
            "email": "drwenski1@llnl.gov"
          },
          "committer": {
            "name": "Tara Drwenski",
            "username": "tdrwenski",
            "email": "drwenski1@llnl.gov"
          },
          "id": "d101901b2a41c4e8826ccf006019b36d3f42ce3c",
          "message": "Turn off fail on alert on GitHub performance job",
          "timestamp": "2026-03-10T17:20:55Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/d101901b2a41c4e8826ccf006019b36d3f42ce3c"
        },
        "date": 1773168182151,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_OpenMP-default_Basic_ARRAY_OF_PTRS",
            "value": 4.36444,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.091s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_COPY8",
            "value": 3.633192,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.908s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY",
            "value": 0.1507644,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.377s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY_ATOMIC",
            "value": 1.607032,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 4.018s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_EMPTY",
            "value": 0.0785336,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 0.393s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_IF_QUAD",
            "value": 1.9501111111111114,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 1.755s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INDEXLIST_3LOOP",
            "value": 1.658476,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.829s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT3",
            "value": 0.741312,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 1.853s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D",
            "value": 0.1725248,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 2.157s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.10360559999999999,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 1.295s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MAT_MAT_SHARED",
            "value": 199.99880000000002,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 5.000s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULADDSUB",
            "value": 0.7483542857142857,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 1.310s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_NESTED_INIT",
            "value": 0.23522799999999996,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 1.176s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_ATOMIC",
            "value": 299.0652,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 74.766s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_REDUCE",
            "value": 0.3557044,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.089s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE3_INT",
            "value": 0.114278,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.029s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE_STRUCT",
            "value": 0.3820052,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.096s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_TRAP_INT",
            "value": 0.852776,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.213s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULTI_REDUCE",
            "value": 0.689076,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.172s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_PI_REDUCE",
            "value": 0.3529084,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.088s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE3_INT",
            "value": 0.1012756,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.025s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE_STRUCT",
            "value": 0.3500124,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.088s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_TRAP_INT",
            "value": 0.856296,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.214s"
          },
          {
            "name": "RAJA_Seq-default_Basic_ARRAY_OF_PTRS",
            "value": 42.4032,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 10.601s"
          },
          {
            "name": "RAJA_Seq-default_Basic_COPY8",
            "value": 39.11504,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 9.779s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY",
            "value": 3.930052,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 9.825s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY_ATOMIC",
            "value": 3.927624,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 9.819s"
          },
          {
            "name": "RAJA_Seq-default_Basic_EMPTY",
            "value": 1.322142,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 6.611s"
          },
          {
            "name": "RAJA_Seq-default_Basic_IF_QUAD",
            "value": 31.50311111111111,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 28.353s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INDEXLIST_3LOOP",
            "value": 27.7214,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 13.861s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT3",
            "value": 10.8812,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 27.203s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D",
            "value": 3.103616,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 38.795s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 3.113168,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 38.915s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MAT_MAT_SHARED",
            "value": 3038.4960000000005,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 75.962s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULADDSUB",
            "value": 11.241657142857141,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 19.673s"
          },
          {
            "name": "RAJA_Seq-default_Basic_NESTED_INIT",
            "value": 3.4037,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 17.018s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_ATOMIC",
            "value": 5.27732,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.319s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_REDUCE",
            "value": 10.56404,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 2.641s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE3_INT",
            "value": 2.721252,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.680s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE_STRUCT",
            "value": 6.75224,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.688s"
          },
          {
            "name": "RAJA_Seq-default_Basic_TRAP_INT",
            "value": 13.30812,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.327s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULTI_REDUCE",
            "value": 10.81432,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 2.704s"
          },
          {
            "name": "RAJA_Seq-new_Basic_PI_REDUCE",
            "value": 5.355359999999999,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.339s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE3_INT",
            "value": 1.449344,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.362s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE_STRUCT",
            "value": 12.24648,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.062s"
          },
          {
            "name": "RAJA_Seq-new_Basic_TRAP_INT",
            "value": 13.24028,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.310s"
          }
        ]
      }
    ],
    "clang_14_0_6_openmp_caliper": [
      {
        "commit": {
          "author": {
            "name": "Adrien M. BERNEDE",
            "username": "adrienbernede",
            "email": "51493078+adrienbernede@users.noreply.github.com"
          },
          "committer": {
            "name": "Adrien M. BERNEDE",
            "username": "adrienbernede",
            "email": "51493078+adrienbernede@users.noreply.github.com"
          },
          "id": "36517681508b47429f1380c4d698c87986125922",
          "message": "Merge branch 'develop' into woptim/caliper-ci",
          "timestamp": "2025-11-24T13:25:35Z",
          "url": "https://github.com/LLNL/RAJAPerf/commit/36517681508b47429f1380c4d698c87986125922"
        },
        "date": 1763996174455,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_OpenMP-default_Basic_ARRAY_OF_PTRS",
            "value": 0.602732,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_COPY8",
            "value": 0.255424,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY",
            "value": 0.01668814,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY_ATOMIC",
            "value": 0.336306,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_EMPTY",
            "value": 0.0200332,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_IF_QUAD",
            "value": 0.3758038888888889,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INDEXLIST_3LOOP",
            "value": 0.275596,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT3",
            "value": 0.11381980000000001,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D",
            "value": 0.044422800000000005,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.03419636,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MAT_MAT_SHARED",
            "value": 10.342319999999999,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULADDSUB",
            "value": 0.11311028571428572,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_NESTED_INIT",
            "value": 0.0967584,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_ATOMIC",
            "value": 56.2042,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_REDUCE",
            "value": 0.0798078,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE3_INT",
            "value": 0.045114,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE_STRUCT",
            "value": 0.1294806,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_TRAP_INT",
            "value": 0.179136,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULTI_REDUCE",
            "value": 0.1489728,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_PI_REDUCE",
            "value": 0.07307260000000002,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE3_INT",
            "value": 0.0326184,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE_STRUCT",
            "value": 0.0915574,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_TRAP_INT",
            "value": 0.17418240000000001,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-default_Basic_ARRAY_OF_PTRS",
            "value": 8.74092,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-default_Basic_COPY8",
            "value": 6.065,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY",
            "value": 0.46459400000000006,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY_ATOMIC",
            "value": 0.4647,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-default_Basic_EMPTY",
            "value": 0.265124,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-default_Basic_IF_QUAD",
            "value": 6.773722222222222,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-default_Basic_INDEXLIST_3LOOP",
            "value": 4.4117,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT3",
            "value": 2.0344,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D",
            "value": 0.47299199999999997,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.34313119999999997,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-default_Basic_MAT_MAT_SHARED",
            "value": 173.87599999999998,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULADDSUB",
            "value": 2.0387028571428574,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-default_Basic_NESTED_INIT",
            "value": 0.5712679999999999,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_ATOMIC",
            "value": 1.066528,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_REDUCE",
            "value": 1.06665,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE3_INT",
            "value": 0.251576,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE_STRUCT",
            "value": 1.151854,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-default_Basic_TRAP_INT",
            "value": 2.6779,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULTI_REDUCE",
            "value": 2.1415,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-new_Basic_PI_REDUCE",
            "value": 1.0661679999999998,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE3_INT",
            "value": 0.220164,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE_STRUCT",
            "value": 1.063294,
            "unit": "ms/rep"
          },
          {
            "name": "RAJA_Seq-new_Basic_TRAP_INT",
            "value": 2.71016,
            "unit": "ms/rep"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "Tara Drwenski",
            "username": "tdrwenski",
            "email": "drwenski1@llnl.gov"
          },
          "committer": {
            "name": "Tara Drwenski",
            "username": "tdrwenski",
            "email": "drwenski1@llnl.gov"
          },
          "id": "310c67d9054afbb06868828ee33f5ea88ce02c21",
          "message": "Fix environment variable",
          "timestamp": "2026-02-11T23:37:34Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/310c67d9054afbb06868828ee33f5ea88ce02c21"
        },
        "date": 1771002354940,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY",
            "value": 0.01856148,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.046s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY_ATOMIC",
            "value": 0.3389864,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.847s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_EMPTY",
            "value": 0.0211076,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 0.106s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_IF_QUAD",
            "value": 0.37492555555555557,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 0.337s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INDEXLIST_3LOOP",
            "value": 0.26867,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.134s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT3",
            "value": 0.1187052,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.297s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D",
            "value": 0.04518008,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 0.565s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.03484096,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 0.436s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MAT_MAT_SHARED",
            "value": 11.05156,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 0.276s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULADDSUB",
            "value": 0.11491028571428571,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 0.201s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_NESTED_INIT",
            "value": 0.0921352,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 0.461s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_REDUCE",
            "value": 0.08036,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.020s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_TRAP_INT",
            "value": 0.1797612,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.045s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULTI_REDUCE",
            "value": 0.147538,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.037s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_PI_REDUCE",
            "value": 0.0754652,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.019s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_TRAP_INT",
            "value": 0.1810528,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.045s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY",
            "value": 0.43654400000000004,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 1.091s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY_ATOMIC",
            "value": 0.4345,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 1.086s"
          },
          {
            "name": "RAJA_Seq-default_Basic_EMPTY",
            "value": 0.263802,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 1.319s"
          },
          {
            "name": "RAJA_Seq-default_Basic_IF_QUAD",
            "value": 6.729699999999999,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 6.057s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INDEXLIST_3LOOP",
            "value": 4.262,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 2.131s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT3",
            "value": 1.9500279999999999,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 4.875s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D",
            "value": 0.4714744,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 5.893s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.34094479999999994,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 4.262s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MAT_MAT_SHARED",
            "value": 171.18079999999998,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 4.280s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULADDSUB",
            "value": 1.9491371428571427,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 3.411s"
          },
          {
            "name": "RAJA_Seq-default_Basic_NESTED_INIT",
            "value": 0.569288,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 2.846s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_REDUCE",
            "value": 1.063516,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.266s"
          },
          {
            "name": "RAJA_Seq-default_Basic_TRAP_INT",
            "value": 2.674928,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.669s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULTI_REDUCE",
            "value": 2.139092,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.535s"
          },
          {
            "name": "RAJA_Seq-new_Basic_PI_REDUCE",
            "value": 1.063508,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.266s"
          },
          {
            "name": "RAJA_Seq-new_Basic_TRAP_INT",
            "value": 2.768532,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.692s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "Tara Drwenski",
            "username": "tdrwenski",
            "email": "drwenski1@llnl.gov"
          },
          "committer": {
            "name": "Tara Drwenski",
            "username": "tdrwenski",
            "email": "drwenski1@llnl.gov"
          },
          "id": "e6f5aed9aa87888f68cb92d755cb04fbc7e80009",
          "message": "Rename variable in caliper to json script",
          "timestamp": "2026-02-13T17:19:15Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/e6f5aed9aa87888f68cb92d755cb04fbc7e80009"
        },
        "date": 1771009590871,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY",
            "value": 0.01768972,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.044s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY_ATOMIC",
            "value": 0.33905680000000005,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.848s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_EMPTY",
            "value": 0.0203466,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 0.102s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_IF_QUAD",
            "value": 0.3962788888888889,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 0.357s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INDEXLIST_3LOOP",
            "value": 0.264888,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.132s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT3",
            "value": 0.12039200000000001,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.301s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D",
            "value": 0.04458688,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 0.557s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.034446159999999997,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 0.431s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MAT_MAT_SHARED",
            "value": 10.40084,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 0.260s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULADDSUB",
            "value": 0.11732628571428572,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 0.205s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_NESTED_INIT",
            "value": 0.095277,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 0.476s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_REDUCE",
            "value": 0.0801288,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.020s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_TRAP_INT",
            "value": 0.1803684,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.045s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULTI_REDUCE",
            "value": 0.1483116,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.037s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_PI_REDUCE",
            "value": 0.075602,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.019s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_TRAP_INT",
            "value": 0.1812256,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.045s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY",
            "value": 0.43528399999999995,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 1.088s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY_ATOMIC",
            "value": 0.433856,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 1.085s"
          },
          {
            "name": "RAJA_Seq-default_Basic_EMPTY",
            "value": 0.263748,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 1.319s"
          },
          {
            "name": "RAJA_Seq-default_Basic_IF_QUAD",
            "value": 6.7216555555555555,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 6.049s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INDEXLIST_3LOOP",
            "value": 4.27634,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 2.138s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT3",
            "value": 1.9517520000000002,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 4.879s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D",
            "value": 0.47124720000000003,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 5.891s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.3409864000000001,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 4.262s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MAT_MAT_SHARED",
            "value": 171.194,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 4.280s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULADDSUB",
            "value": 1.9487885714285715,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 3.410s"
          },
          {
            "name": "RAJA_Seq-default_Basic_NESTED_INIT",
            "value": 0.569178,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 2.846s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_REDUCE",
            "value": 1.06346,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.266s"
          },
          {
            "name": "RAJA_Seq-default_Basic_TRAP_INT",
            "value": 2.638216,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.660s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULTI_REDUCE",
            "value": 2.115272,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.529s"
          },
          {
            "name": "RAJA_Seq-new_Basic_PI_REDUCE",
            "value": 1.063508,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.266s"
          },
          {
            "name": "RAJA_Seq-new_Basic_TRAP_INT",
            "value": 2.638312,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.660s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "Tara Drwenski",
            "username": "tdrwenski",
            "email": "drwenski1@llnl.gov"
          },
          "committer": {
            "name": "Tara Drwenski",
            "username": "tdrwenski",
            "email": "drwenski1@llnl.gov"
          },
          "id": "a527f32a46b0afbde514b7905da77b52e8bb253a",
          "message": "Exclude unstable tests",
          "timestamp": "2026-03-09T18:08:34Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/a527f32a46b0afbde514b7905da77b52e8bb253a"
        },
        "date": 1773095905633,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY",
            "value": 0.016757400000000002,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.042s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY_ATOMIC",
            "value": 0.3377096,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.844s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_EMPTY",
            "value": 0.0203646,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 0.102s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_IF_QUAD",
            "value": 0.37935,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 0.341s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INDEXLIST_3LOOP",
            "value": 0.267448,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.134s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT3",
            "value": 0.11232439999999999,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.281s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D",
            "value": 0.0445408,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 0.557s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.03420824,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 0.428s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MAT_MAT_SHARED",
            "value": 10.37668,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 0.259s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULADDSUB",
            "value": 0.11336171428571429,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 0.198s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_NESTED_INIT",
            "value": 0.0956414,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 0.478s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_REDUCE",
            "value": 0.0802712,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.020s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_TRAP_INT",
            "value": 0.1831952,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.046s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULTI_REDUCE",
            "value": 0.147142,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.037s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_PI_REDUCE",
            "value": 0.0755476,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.019s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_TRAP_INT",
            "value": 0.1764272,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.044s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY",
            "value": 0.45301199999999997,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 1.133s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY_ATOMIC",
            "value": 0.43728,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 1.093s"
          },
          {
            "name": "RAJA_Seq-default_Basic_EMPTY",
            "value": 0.263738,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 1.319s"
          },
          {
            "name": "RAJA_Seq-default_Basic_IF_QUAD",
            "value": 6.729333333333333,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 6.056s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INDEXLIST_3LOOP",
            "value": 4.29294,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 2.146s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT3",
            "value": 1.9532599999999998,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 4.883s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D",
            "value": 0.4714368,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 5.893s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.34104239999999997,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 4.263s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MAT_MAT_SHARED",
            "value": 171.7272,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 4.293s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULADDSUB",
            "value": 1.9434285714285715,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 3.401s"
          },
          {
            "name": "RAJA_Seq-default_Basic_NESTED_INIT",
            "value": 0.569178,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 2.846s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_REDUCE",
            "value": 1.063464,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.266s"
          },
          {
            "name": "RAJA_Seq-default_Basic_TRAP_INT",
            "value": 2.769004,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.692s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULTI_REDUCE",
            "value": 2.129692,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.532s"
          },
          {
            "name": "RAJA_Seq-new_Basic_PI_REDUCE",
            "value": 1.06348,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.266s"
          },
          {
            "name": "RAJA_Seq-new_Basic_TRAP_INT",
            "value": 2.637144,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.659s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "Tara Drwenski",
            "username": "tdrwenski",
            "email": "drwenski1@llnl.gov"
          },
          "committer": {
            "name": "Tara Drwenski",
            "username": "tdrwenski",
            "email": "drwenski1@llnl.gov"
          },
          "id": "d101901b2a41c4e8826ccf006019b36d3f42ce3c",
          "message": "Turn off fail on alert on GitHub performance job",
          "timestamp": "2026-03-10T17:20:55Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/d101901b2a41c4e8826ccf006019b36d3f42ce3c"
        },
        "date": 1773168179734,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_OpenMP-default_Basic_ARRAY_OF_PTRS",
            "value": 5.0636,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.266s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_COPY8",
            "value": 4.71752,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.179s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY",
            "value": 0.1642728,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.411s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY_ATOMIC",
            "value": 1.689932,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 4.225s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_EMPTY",
            "value": 0.087338,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 0.437s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_IF_QUAD",
            "value": 2.020888888888889,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 1.819s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INDEXLIST_3LOOP",
            "value": 1.521246,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.761s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT3",
            "value": 0.807484,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 2.019s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D",
            "value": 0.21018799999999999,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 2.627s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.15823040000000002,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 1.978s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MAT_MAT_SHARED",
            "value": 156.3688,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 3.909s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULADDSUB",
            "value": 0.8334514285714285,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 1.459s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_NESTED_INIT",
            "value": 0.4229879999999999,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 2.115s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_ATOMIC",
            "value": 279.486,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 69.871s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_REDUCE",
            "value": 0.3492928,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.087s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE3_INT",
            "value": 0.2593892,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.065s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE_STRUCT",
            "value": 0.452956,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.113s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_TRAP_INT",
            "value": 0.858016,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.215s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULTI_REDUCE",
            "value": 0.68874,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.172s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_PI_REDUCE",
            "value": 0.3447932,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.086s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE3_INT",
            "value": 0.1512596,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.038s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE_STRUCT",
            "value": 0.432072,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.108s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_TRAP_INT",
            "value": 0.8598600000000001,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.215s"
          },
          {
            "name": "RAJA_Seq-default_Basic_ARRAY_OF_PTRS",
            "value": 37.97252,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 9.493s"
          },
          {
            "name": "RAJA_Seq-default_Basic_COPY8",
            "value": 31.41244,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 7.853s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY",
            "value": 3.9922159999999995,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 9.981s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY_ATOMIC",
            "value": 3.989992,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 9.975s"
          },
          {
            "name": "RAJA_Seq-default_Basic_EMPTY",
            "value": 1.3187300000000002,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 6.594s"
          },
          {
            "name": "RAJA_Seq-default_Basic_IF_QUAD",
            "value": 34.278,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 30.850s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INDEXLIST_3LOOP",
            "value": 26.5236,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 13.262s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT3",
            "value": 11.698319999999999,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 29.246s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D",
            "value": 2.9091680000000006,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 36.365s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 3.0391440000000003,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 37.989s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MAT_MAT_SHARED",
            "value": 2622.0159999999996,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 65.550s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULADDSUB",
            "value": 12.457314285714284,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 21.800s"
          },
          {
            "name": "RAJA_Seq-default_Basic_NESTED_INIT",
            "value": 3.20376,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 16.019s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_ATOMIC",
            "value": 5.31764,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.329s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_REDUCE",
            "value": 5.31756,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.329s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE3_INT",
            "value": 3.193852,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.798s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE_STRUCT",
            "value": 6.98212,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.746s"
          },
          {
            "name": "RAJA_Seq-default_Basic_TRAP_INT",
            "value": 13.6268,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.407s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULTI_REDUCE",
            "value": 10.76192,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 2.690s"
          },
          {
            "name": "RAJA_Seq-new_Basic_PI_REDUCE",
            "value": 5.31756,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.329s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE3_INT",
            "value": 1.2654,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.316s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE_STRUCT",
            "value": 5.91444,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.479s"
          },
          {
            "name": "RAJA_Seq-new_Basic_TRAP_INT",
            "value": 13.84184,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.460s"
          }
        ]
      }
    ],
    "rocmcc_6_4_1_hip_openmp_caliper": [
      {
        "commit": {
          "author": {
            "name": "Tara Drwenski",
            "username": "tdrwenski",
            "email": "drwenski1@llnl.gov"
          },
          "committer": {
            "name": "Tara Drwenski",
            "username": "tdrwenski",
            "email": "drwenski1@llnl.gov"
          },
          "id": "310c67d9054afbb06868828ee33f5ea88ce02c21",
          "message": "Fix environment variable",
          "timestamp": "2026-02-11T23:37:34Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/310c67d9054afbb06868828ee33f5ea88ce02c21"
        },
        "date": 1771002354809,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_HIP-block_256_Basic_DAXPY",
            "value": 0.00939224,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.005s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_DAXPY_ATOMIC",
            "value": 0.0065360800000000005,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INIT3",
            "value": 0.01233806,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.006s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INIT_VIEW1D",
            "value": 0.00526372,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.013s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.00550388,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.014s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_MULADDSUB",
            "value": 0.015141942857142859,
            "unit": "ms/rep",
            "extra": "reps: 350, passes: 1, total_time: 0.005s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_NESTED_INIT",
            "value": 0.0069131,
            "unit": "ms/rep",
            "extra": "reps: 1000, passes: 1, total_time: 0.007s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_PI_ATOMIC",
            "value": 0.154018,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.008s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "Tara Drwenski",
            "username": "tdrwenski",
            "email": "drwenski1@llnl.gov"
          },
          "committer": {
            "name": "Tara Drwenski",
            "username": "tdrwenski",
            "email": "drwenski1@llnl.gov"
          },
          "id": "e6f5aed9aa87888f68cb92d755cb04fbc7e80009",
          "message": "Rename variable in caliper to json script",
          "timestamp": "2026-02-13T17:19:15Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/e6f5aed9aa87888f68cb92d755cb04fbc7e80009"
        },
        "date": 1771009573516,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_HIP-block_256_Basic_DAXPY",
            "value": 0.0091146,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.005s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_DAXPY_ATOMIC",
            "value": 0.00647338,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INIT3",
            "value": 0.01222596,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.006s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INIT_VIEW1D",
            "value": 0.005188519999999999,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.013s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.005353119999999999,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.013s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_MULADDSUB",
            "value": 0.015024457142857143,
            "unit": "ms/rep",
            "extra": "reps: 350, passes: 1, total_time: 0.005s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_NESTED_INIT",
            "value": 0.00706513,
            "unit": "ms/rep",
            "extra": "reps: 1000, passes: 1, total_time: 0.007s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_PI_ATOMIC",
            "value": 0.1541018,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.008s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "Tara Drwenski",
            "username": "tdrwenski",
            "email": "drwenski1@llnl.gov"
          },
          "committer": {
            "name": "Tara Drwenski",
            "username": "tdrwenski",
            "email": "drwenski1@llnl.gov"
          },
          "id": "a527f32a46b0afbde514b7905da77b52e8bb253a",
          "message": "Exclude unstable tests",
          "timestamp": "2026-03-09T18:08:34Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/a527f32a46b0afbde514b7905da77b52e8bb253a"
        },
        "date": 1773096036547,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_HIP-atomic_direct_256_Basic_MULTI_REDUCE",
            "value": 0.045041599999999994,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-atomic_occgs_256_Basic_MULTI_REDUCE",
            "value": 0.044214399999999994,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_Basic_PI_REDUCE",
            "value": 0.039271999999999994,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_Basic_REDUCE3_INT",
            "value": 0.062326200000000005,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_Basic_REDUCE_STRUCT",
            "value": 0.19200419999999999,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.010s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_Basic_PI_REDUCE",
            "value": 0.0235316,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_Basic_REDUCE3_INT",
            "value": 0.0252716,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_Basic_REDUCE_STRUCT",
            "value": 0.0547896,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_Basic_PI_REDUCE",
            "value": 0.04781660000000001,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_Basic_REDUCE3_INT",
            "value": 0.08911060000000001,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.004s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_Basic_REDUCE_STRUCT",
            "value": 0.217286,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.011s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_Basic_PI_REDUCE",
            "value": 0.49675600000000003,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.025s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_Basic_REDUCE3_INT",
            "value": 1.468368,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.073s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_Basic_REDUCE_STRUCT",
            "value": 3.03386,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.152s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_Basic_PI_REDUCE",
            "value": 0.024073400000000002,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_Basic_REDUCE3_INT",
            "value": 0.030005800000000003,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_Basic_REDUCE_STRUCT",
            "value": 0.060815799999999996,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_Basic_PI_REDUCE",
            "value": 0.0601988,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_Basic_REDUCE3_INT",
            "value": 0.1434922,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.007s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_Basic_REDUCE_STRUCT",
            "value": 0.28779,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.014s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_ARRAY_OF_PTRS",
            "value": 0.342694,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.017s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_COPY8",
            "value": 0.20358,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.010s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_DAXPY",
            "value": 0.04134619999999999,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.021s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_DAXPY_ATOMIC",
            "value": 0.0235736,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.012s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_IF_QUAD",
            "value": 0.05980222222222222,
            "unit": "ms/rep",
            "extra": "reps: 180, passes: 1, total_time: 0.011s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INDEXLIST_3LOOP",
            "value": 0.0890799,
            "unit": "ms/rep",
            "extra": "reps: 100, passes: 1, total_time: 0.009s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INIT3",
            "value": 0.0516764,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.026s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INIT_VIEW1D",
            "value": 0.01651568,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.041s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.016801559999999997,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.042s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_MAT_MAT_SHARED",
            "value": 5.2471,
            "unit": "ms/rep",
            "extra": "reps: 5, passes: 1, total_time: 0.026s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_MULADDSUB",
            "value": 0.05814457142857143,
            "unit": "ms/rep",
            "extra": "reps: 350, passes: 1, total_time: 0.020s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_NESTED_INIT",
            "value": 0.0218538,
            "unit": "ms/rep",
            "extra": "reps: 1000, passes: 1, total_time: 0.022s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_PI_ATOMIC",
            "value": 0.718838,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.036s"
          },
          {
            "name": "RAJA_HIP-direct_256_Basic_EMPTY",
            "value": 0.0103534,
            "unit": "ms/rep",
            "extra": "reps: 1000, passes: 1, total_time: 0.010s"
          },
          {
            "name": "RAJA_HIP-occgs_256_Basic_EMPTY",
            "value": 0.00377121,
            "unit": "ms/rep",
            "extra": "reps: 1000, passes: 1, total_time: 0.004s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "Tara Drwenski",
            "username": "tdrwenski",
            "email": "drwenski1@llnl.gov"
          },
          "committer": {
            "name": "Tara Drwenski",
            "username": "tdrwenski",
            "email": "drwenski1@llnl.gov"
          },
          "id": "d101901b2a41c4e8826ccf006019b36d3f42ce3c",
          "message": "Turn off fail on alert on GitHub performance job",
          "timestamp": "2026-03-10T17:20:55Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/d101901b2a41c4e8826ccf006019b36d3f42ce3c"
        },
        "date": 1773168182216,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_HIP-atomic_direct_256_Basic_MULTI_REDUCE",
            "value": 0.04495360000000001,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-atomic_occgs_256_Basic_MULTI_REDUCE",
            "value": 0.043322599999999996,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_Basic_PI_REDUCE",
            "value": 0.040594200000000004,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_Basic_REDUCE3_INT",
            "value": 0.063849,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_Basic_REDUCE_STRUCT",
            "value": 0.1963458,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.010s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_Basic_TRAP_INT",
            "value": 0.04524619999999999,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_Basic_PI_REDUCE",
            "value": 0.022666600000000002,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_Basic_REDUCE3_INT",
            "value": 0.025561600000000004,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_Basic_REDUCE_STRUCT",
            "value": 0.0544086,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_Basic_TRAP_INT",
            "value": 0.026153399999999997,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_Basic_PI_REDUCE",
            "value": 0.0491482,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_Basic_REDUCE3_INT",
            "value": 0.08825579999999998,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.004s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_Basic_REDUCE_STRUCT",
            "value": 0.2201,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.011s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_Basic_TRAP_INT",
            "value": 0.050126,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_Basic_PI_REDUCE",
            "value": 0.498538,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.025s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_Basic_REDUCE3_INT",
            "value": 1.474882,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.074s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_Basic_REDUCE_STRUCT",
            "value": 3.05618,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.153s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_Basic_TRAP_INT",
            "value": 0.504586,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.025s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_Basic_PI_REDUCE",
            "value": 0.0240232,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_Basic_REDUCE3_INT",
            "value": 0.030419,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_Basic_REDUCE_STRUCT",
            "value": 0.0615118,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_Basic_TRAP_INT",
            "value": 0.028604599999999997,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_Basic_PI_REDUCE",
            "value": 0.059013,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_Basic_REDUCE3_INT",
            "value": 0.1423464,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.007s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_Basic_REDUCE_STRUCT",
            "value": 0.29142399999999996,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.015s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_Basic_TRAP_INT",
            "value": 0.060316,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_ARRAY_OF_PTRS",
            "value": 0.35129800000000005,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.018s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_COPY8",
            "value": 0.20472200000000002,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.010s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_DAXPY",
            "value": 0.0408084,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.020s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_DAXPY_ATOMIC",
            "value": 0.0236458,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.012s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_IF_QUAD",
            "value": 0.06525777777777779,
            "unit": "ms/rep",
            "extra": "reps: 180, passes: 1, total_time: 0.012s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INDEXLIST_3LOOP",
            "value": 0.08897250000000001,
            "unit": "ms/rep",
            "extra": "reps: 100, passes: 1, total_time: 0.009s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INIT3",
            "value": 0.0540882,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.027s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INIT_VIEW1D",
            "value": 0.016673240000000002,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.042s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.0170576,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.043s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_MAT_MAT_SHARED",
            "value": 5.37024,
            "unit": "ms/rep",
            "extra": "reps: 5, passes: 1, total_time: 0.027s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_MULADDSUB",
            "value": 0.059252,
            "unit": "ms/rep",
            "extra": "reps: 350, passes: 1, total_time: 0.021s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_NESTED_INIT",
            "value": 0.0220245,
            "unit": "ms/rep",
            "extra": "reps: 1000, passes: 1, total_time: 0.022s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_PI_ATOMIC",
            "value": 0.7187399999999999,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.036s"
          },
          {
            "name": "RAJA_HIP-direct_256_Basic_EMPTY",
            "value": 0.0103238,
            "unit": "ms/rep",
            "extra": "reps: 1000, passes: 1, total_time: 0.010s"
          },
          {
            "name": "RAJA_HIP-occgs_256_Basic_EMPTY",
            "value": 0.00345018,
            "unit": "ms/rep",
            "extra": "reps: 1000, passes: 1, total_time: 0.003s"
          }
        ]
      }
    ]
  }
}