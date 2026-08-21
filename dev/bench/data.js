window.BENCHMARK_DATA = {
  "lastUpdate": 1787353113697,
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
      },
      {
        "commit": {
          "author": {
            "name": "Rich Hornung",
            "username": "rhornung67",
            "email": "hornung1@llnl.gov"
          },
          "committer": {
            "name": "GitHub",
            "username": "web-flow",
            "email": "noreply@github.com"
          },
          "id": "2e4038b7fee213ead90d2108b8458b77c6ac0006",
          "message": "Merge pull request #678 from llnl/main-mergeback\n\nMain mergeback to develop after v2025.12.1 release",
          "timestamp": "2026-03-30T22:11:27Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/2e4038b7fee213ead90d2108b8458b77c6ac0006"
        },
        "date": 1774914047933,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_Seq-default_Basic_ARRAY_OF_PTRS",
            "value": 0.5057535714285714,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 14.161s"
          },
          {
            "name": "RAJA_Seq-default_Basic_COPY8",
            "value": 0.4719392857142857,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 13.214s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY",
            "value": 0.05353642857142857,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 112, total_time: 14.990s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY_ATOMIC",
            "value": 0.05356464285714286,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 112, total_time: 14.998s"
          },
          {
            "name": "RAJA_Seq-default_Basic_EMPTY",
            "value": 0.0030062142857142857,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 112, total_time: 1.683s"
          },
          {
            "name": "RAJA_Seq-default_Basic_IF_QUAD",
            "value": 0.14314682539682538,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 112, total_time: 14.429s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INDEXLIST_3LOOP",
            "value": 0.12021339285714287,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 112, total_time: 6.732s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT3",
            "value": 0.15628,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 112, total_time: 43.758s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D",
            "value": 0.03046707142857143,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 112, total_time: 42.654s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.030590285714285716,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 112, total_time: 42.826s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MAT_MAT_SHARED",
            "value": 2.8664428571428573,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 112, total_time: 8.026s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULADDSUB",
            "value": 0.15632653061224488,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 112, total_time: 30.640s"
          },
          {
            "name": "RAJA_Seq-default_Basic_NESTED_INIT",
            "value": 0.03049732142857143,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 112, total_time: 17.078s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_ATOMIC",
            "value": 0.0122795,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.344s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_REDUCE",
            "value": 0.012112571428571427,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.339s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE3_INT",
            "value": 0.00819192857142857,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.229s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE_STRUCT",
            "value": 0.03452525,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.967s"
          },
          {
            "name": "RAJA_Seq-default_Basic_TRAP_INT",
            "value": 0.03041107142857143,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.852s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULTI_REDUCE",
            "value": 0.03635142857142857,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 1.018s"
          },
          {
            "name": "RAJA_Seq-new_Basic_PI_REDUCE",
            "value": 0.01209382142857143,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.339s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE3_INT",
            "value": 0.004427,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.124s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE_STRUCT",
            "value": 0.03317439285714286,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.929s"
          },
          {
            "name": "RAJA_Seq-new_Basic_TRAP_INT",
            "value": 0.030791535714285716,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.862s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "Rich Hornung",
            "username": "rhornung67",
            "email": "hornung1@llnl.gov"
          },
          "committer": {
            "name": "GitHub",
            "username": "web-flow",
            "email": "noreply@github.com"
          },
          "id": "3111bb9edb2a22dcc64c7589d94a080912db06be",
          "message": "Merge pull request #679 from llnl/task/rhornung67/fix-docs\n\nDoc fixes",
          "timestamp": "2026-04-01T15:59:31Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/3111bb9edb2a22dcc64c7589d94a080912db06be"
        },
        "date": 1775063668749,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_Seq-default_Basic_ARRAY_OF_PTRS",
            "value": 0.5010428571428571,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 14.029s"
          },
          {
            "name": "RAJA_Seq-default_Basic_COPY8",
            "value": 0.4707678571428571,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 13.181s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY",
            "value": 0.05359964285714285,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 112, total_time: 15.008s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY_ATOMIC",
            "value": 0.05358892857142857,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 112, total_time: 15.005s"
          },
          {
            "name": "RAJA_Seq-default_Basic_EMPTY",
            "value": 0.0030081785714285714,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 112, total_time: 1.685s"
          },
          {
            "name": "RAJA_Seq-default_Basic_IF_QUAD",
            "value": 0.14258829365079365,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 112, total_time: 14.373s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INDEXLIST_3LOOP",
            "value": 0.12147321428571428,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 112, total_time: 6.803s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT3",
            "value": 0.15639214285714287,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 112, total_time: 43.790s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D",
            "value": 0.030468428571428573,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 112, total_time: 42.656s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.030564714285714285,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 112, total_time: 42.791s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MAT_MAT_SHARED",
            "value": 2.8825214285714282,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 112, total_time: 8.071s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULADDSUB",
            "value": 0.15641530612244897,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 112, total_time: 30.657s"
          },
          {
            "name": "RAJA_Seq-default_Basic_NESTED_INIT",
            "value": 0.03091732142857143,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 112, total_time: 17.314s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_ATOMIC",
            "value": 0.012130285714285715,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.340s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_REDUCE",
            "value": 0.012125571428571427,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.340s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE3_INT",
            "value": 0.008345071428571429,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.234s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE_STRUCT",
            "value": 0.03438982142857143,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.963s"
          },
          {
            "name": "RAJA_Seq-default_Basic_TRAP_INT",
            "value": 0.030434892857142855,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.852s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULTI_REDUCE",
            "value": 0.03487317857142857,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.976s"
          },
          {
            "name": "RAJA_Seq-new_Basic_PI_REDUCE",
            "value": 0.012136607142857142,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.340s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE3_INT",
            "value": 0.004424,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.124s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE_STRUCT",
            "value": 0.03295892857142857,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.923s"
          },
          {
            "name": "RAJA_Seq-new_Basic_TRAP_INT",
            "value": 0.030775857142857143,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.862s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "Adrien Bernede",
            "username": "adrienbernede",
            "email": "51493078+adrienbernede@users.noreply.github.com"
          },
          "committer": {
            "name": "GitHub",
            "username": "web-flow",
            "email": "noreply@github.com"
          },
          "id": "95c360ddf585410e6123956c72a2c648111ece29",
          "message": "Merge pull request #652 from llnl/woptim/rocm7",
          "timestamp": "2026-04-16T19:31:14Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/95c360ddf585410e6123956c72a2c648111ece29"
        },
        "date": 1776372139358,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_Seq-default_Basic_ARRAY_OF_PTRS",
            "value": 0.5074607142857143,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 14.209s"
          },
          {
            "name": "RAJA_Seq-default_Basic_COPY8",
            "value": 0.4722857142857143,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 13.224s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY",
            "value": 0.053861785714285716,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 112, total_time: 15.081s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY_ATOMIC",
            "value": 0.05387571428571429,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 112, total_time: 15.085s"
          },
          {
            "name": "RAJA_Seq-default_Basic_EMPTY",
            "value": 0.0030040535714285716,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 112, total_time: 1.682s"
          },
          {
            "name": "RAJA_Seq-default_Basic_IF_QUAD",
            "value": 0.1420267857142857,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 112, total_time: 14.316s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INDEXLIST_3LOOP",
            "value": 0.12207875,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 112, total_time: 6.836s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT3",
            "value": 0.15636321428571429,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 112, total_time: 43.782s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D",
            "value": 0.030378428571428574,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 112, total_time: 42.530s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.03050207142857143,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 112, total_time: 42.703s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MAT_MAT_SHARED",
            "value": 2.8306714285714287,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 112, total_time: 7.926s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULADDSUB",
            "value": 0.15622448979591838,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 112, total_time: 30.620s"
          },
          {
            "name": "RAJA_Seq-default_Basic_NESTED_INIT",
            "value": 0.030426249999999995,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 112, total_time: 17.039s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_ATOMIC",
            "value": 0.012104392857142857,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.339s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_REDUCE",
            "value": 0.012106928571428572,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.339s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE3_INT",
            "value": 0.007981035714285715,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.223s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE_STRUCT",
            "value": 0.03438764285714286,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.963s"
          },
          {
            "name": "RAJA_Seq-default_Basic_TRAP_INT",
            "value": 0.03040646428571429,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.851s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULTI_REDUCE",
            "value": 0.03668428571428572,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 1.027s"
          },
          {
            "name": "RAJA_Seq-new_Basic_PI_REDUCE",
            "value": 0.012091285714285716,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.339s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE3_INT",
            "value": 0.004331678571428571,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.121s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE_STRUCT",
            "value": 0.03314728571428571,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.928s"
          },
          {
            "name": "RAJA_Seq-new_Basic_TRAP_INT",
            "value": 0.030811142857142856,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.863s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "Jason Burmark",
            "username": "MrBurmark",
            "email": "MrBurmark@users.noreply.github.com"
          },
          "committer": {
            "name": "GitHub",
            "username": "web-flow",
            "email": "noreply@github.com"
          },
          "id": "9bc67910045283981831de42fe96bd151eee0761",
          "message": "Merge pull request #681 from llnl/pr-from-fork/680\n\nPr from fork/680",
          "timestamp": "2026-04-20T16:10:32Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/9bc67910045283981831de42fe96bd151eee0761"
        },
        "date": 1776704784471,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_Seq-default_Basic_ARRAY_OF_PTRS",
            "value": 0.5014178571428571,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 14.040s"
          },
          {
            "name": "RAJA_Seq-default_Basic_COPY8",
            "value": 0.4703178571428572,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 13.169s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY",
            "value": 0.05365642857142857,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 112, total_time: 15.024s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY_ATOMIC",
            "value": 0.05358285714285714,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 112, total_time: 15.003s"
          },
          {
            "name": "RAJA_Seq-default_Basic_EMPTY",
            "value": 0.0030057142857142856,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 112, total_time: 1.683s"
          },
          {
            "name": "RAJA_Seq-default_Basic_IF_QUAD",
            "value": 0.14209126984126985,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 112, total_time: 14.323s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INDEXLIST_3LOOP",
            "value": 0.12105285714285714,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 112, total_time: 6.779s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT3",
            "value": 0.15631964285714287,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 112, total_time: 43.770s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D",
            "value": 0.030393857142857143,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 112, total_time: 42.551s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.030493214285714287,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 112, total_time: 42.691s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MAT_MAT_SHARED",
            "value": 2.881507142857143,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 112, total_time: 8.068s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULADDSUB",
            "value": 0.1563030612244898,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 112, total_time: 30.635s"
          },
          {
            "name": "RAJA_Seq-default_Basic_NESTED_INIT",
            "value": 0.030425357142857143,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 112, total_time: 17.038s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_ATOMIC",
            "value": 0.012181392857142858,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.341s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_REDUCE",
            "value": 0.012118821428571428,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.339s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE3_INT",
            "value": 0.008116428571428571,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.227s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE_STRUCT",
            "value": 0.03439335714285714,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.963s"
          },
          {
            "name": "RAJA_Seq-default_Basic_TRAP_INT",
            "value": 0.030403714285714284,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.851s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULTI_REDUCE",
            "value": 0.036773214285714284,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 1.030s"
          },
          {
            "name": "RAJA_Seq-new_Basic_PI_REDUCE",
            "value": 0.01210057142857143,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.339s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE3_INT",
            "value": 0.004372678571428572,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.122s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE_STRUCT",
            "value": 0.032998285714285716,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.924s"
          },
          {
            "name": "RAJA_Seq-new_Basic_TRAP_INT",
            "value": 0.03044817857142857,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.853s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "Jason Burmark",
            "username": "MrBurmark",
            "email": "MrBurmark@users.noreply.github.com"
          },
          "committer": {
            "name": "GitHub",
            "username": "web-flow",
            "email": "noreply@github.com"
          },
          "id": "9bc67910045283981831de42fe96bd151eee0761",
          "message": "Merge pull request #681 from llnl/pr-from-fork/680\n\nPr from fork/680",
          "timestamp": "2026-04-20T16:10:32Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/9bc67910045283981831de42fe96bd151eee0761"
        },
        "date": 1779379130402,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_Seq-default_Basic_ARRAY_OF_PTRS",
            "value": 0.5018428571428571,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 14.052s"
          },
          {
            "name": "RAJA_Seq-default_Basic_COPY8",
            "value": 0.47077142857142856,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 13.182s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY",
            "value": 0.05361428571428571,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 112, total_time: 15.012s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY_ATOMIC",
            "value": 0.05361035714285714,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 112, total_time: 15.011s"
          },
          {
            "name": "RAJA_Seq-default_Basic_EMPTY",
            "value": 0.0030051785714285714,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 112, total_time: 1.683s"
          },
          {
            "name": "RAJA_Seq-default_Basic_IF_QUAD",
            "value": 0.14193948412698412,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 112, total_time: 14.307s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INDEXLIST_3LOOP",
            "value": 0.12157714285714286,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 112, total_time: 6.808s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT3",
            "value": 0.15643642857142856,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 112, total_time: 43.802s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D",
            "value": 0.030449857142857147,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 112, total_time: 42.630s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.03054607142857143,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 112, total_time: 42.764s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MAT_MAT_SHARED",
            "value": 2.877707142857143,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 112, total_time: 8.058s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULADDSUB",
            "value": 0.15645816326530612,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 112, total_time: 30.666s"
          },
          {
            "name": "RAJA_Seq-default_Basic_NESTED_INIT",
            "value": 0.030528214285714284,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 112, total_time: 17.096s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_ATOMIC",
            "value": 0.012356857142857143,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.346s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_REDUCE",
            "value": 0.012140464285714286,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.340s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE3_INT",
            "value": 0.007690714285714286,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.215s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE_STRUCT",
            "value": 0.03344285714285714,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.936s"
          },
          {
            "name": "RAJA_Seq-default_Basic_TRAP_INT",
            "value": 0.030834892857142856,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.863s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULTI_REDUCE",
            "value": 0.03451532142857143,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.966s"
          },
          {
            "name": "RAJA_Seq-new_Basic_PI_REDUCE",
            "value": 0.012134142857142859,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.340s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE3_INT",
            "value": 0.004199714285714286,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.118s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE_STRUCT",
            "value": 0.032440107142857146,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.908s"
          },
          {
            "name": "RAJA_Seq-new_Basic_TRAP_INT",
            "value": 0.031054821428571428,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.870s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "Adrien Bernede",
            "username": "adrienbernede",
            "email": "51493078+adrienbernede@users.noreply.github.com"
          },
          "committer": {
            "name": "GitHub",
            "username": "web-flow",
            "email": "noreply@github.com"
          },
          "id": "1f660e77acdb6f12df7a4e39a25172328afb7352",
          "message": "Merge pull request #645 from llnl/woptim/ci-logs-and-errors\n\nImprove CI jobs logs and error handling",
          "timestamp": "2026-06-16T16:08:18Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/1f660e77acdb6f12df7a4e39a25172328afb7352"
        },
        "date": 1781641311944,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_Seq-default_Basic_ARRAY_OF_PTRS",
            "value": 0.4985285714285714,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 13.959s"
          },
          {
            "name": "RAJA_Seq-default_Basic_COPY8",
            "value": 0.4669857142857143,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 13.076s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY",
            "value": 0.05295785714285715,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 112, total_time: 14.828s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY_ATOMIC",
            "value": 0.052947857142857144,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 112, total_time: 14.825s"
          },
          {
            "name": "RAJA_Seq-default_Basic_EMPTY",
            "value": 0.0030008392857142855,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 112, total_time: 1.680s"
          },
          {
            "name": "RAJA_Seq-default_Basic_IF_QUAD",
            "value": 0.14104662698412698,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 112, total_time: 14.217s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INDEXLIST_3LOOP",
            "value": 0.11878214285714285,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 112, total_time: 6.652s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT3",
            "value": 0.1547282142857143,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 112, total_time: 43.324s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D",
            "value": 0.030153428571428567,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 112, total_time: 42.215s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.030281071428571428,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 112, total_time: 42.394s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MAT_MAT_SHARED",
            "value": 2.8486428571428575,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 112, total_time: 7.976s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULADDSUB",
            "value": 0.1547464285714286,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 112, total_time: 30.330s"
          },
          {
            "name": "RAJA_Seq-default_Basic_NESTED_INIT",
            "value": 0.030180357142857144,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 112, total_time: 16.901s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_ATOMIC",
            "value": 0.012039285714285716,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.337s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_REDUCE",
            "value": 0.012036892857142857,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.337s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE3_INT",
            "value": 0.007289500000000001,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.204s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE_STRUCT",
            "value": 0.03278492857142857,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.918s"
          },
          {
            "name": "RAJA_Seq-default_Basic_TRAP_INT",
            "value": 0.03131782142857143,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.877s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULTI_REDUCE",
            "value": 0.033792642857142854,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.946s"
          },
          {
            "name": "RAJA_Seq-new_Basic_PI_REDUCE",
            "value": 0.012036678571428573,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.337s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE3_INT",
            "value": 0.0038204285714285714,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.107s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE_STRUCT",
            "value": 0.03193353571428571,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.894s"
          },
          {
            "name": "RAJA_Seq-new_Basic_TRAP_INT",
            "value": 0.03131914285714286,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.877s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "Jason Burmark",
            "username": "MrBurmark",
            "email": "MrBurmark@users.noreply.github.com"
          },
          "committer": {
            "name": "GitHub",
            "username": "web-flow",
            "email": "noreply@github.com"
          },
          "id": "3d72e32abcb569a20f48069f2b639663f54ca04f",
          "message": "Merge pull request #690 from llnl/bugfix/burmark1/flop_counts\n\nFix flop counts in MASS3DEA and DIFFUSION3DPA",
          "timestamp": "2026-06-22T15:32:25Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/3d72e32abcb569a20f48069f2b639663f54ca04f"
        },
        "date": 1782145928266,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_Seq-default_Basic_ARRAY_OF_PTRS",
            "value": 0.50005,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 14.001s"
          },
          {
            "name": "RAJA_Seq-default_Basic_COPY8",
            "value": 0.4669035714285714,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 13.073s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY",
            "value": 0.05316928571428571,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 112, total_time: 14.887s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY_ATOMIC",
            "value": 0.05316464285714286,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 112, total_time: 14.886s"
          },
          {
            "name": "RAJA_Seq-default_Basic_EMPTY",
            "value": 0.002998982142857143,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 112, total_time: 1.679s"
          },
          {
            "name": "RAJA_Seq-default_Basic_IF_QUAD",
            "value": 0.1415922619047619,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 112, total_time: 14.273s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INDEXLIST_3LOOP",
            "value": 0.11848803571428572,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 112, total_time: 6.635s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT3",
            "value": 0.15465357142857142,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 112, total_time: 43.303s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D",
            "value": 0.030226928571428575,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 112, total_time: 42.318s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.030363000000000005,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 112, total_time: 42.508s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MAT_MAT_SHARED",
            "value": 2.896960714285714,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 112, total_time: 8.111s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULADDSUB",
            "value": 0.15468520408163264,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 112, total_time: 30.318s"
          },
          {
            "name": "RAJA_Seq-default_Basic_NESTED_INIT",
            "value": 0.030282678571428575,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 112, total_time: 16.958s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_ATOMIC",
            "value": 0.012252285714285714,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.343s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_REDUCE",
            "value": 0.012036,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.337s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE3_INT",
            "value": 0.007654964285714286,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.214s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE_STRUCT",
            "value": 0.032806214285714286,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.919s"
          },
          {
            "name": "RAJA_Seq-default_Basic_TRAP_INT",
            "value": 0.03138914285714286,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.879s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULTI_REDUCE",
            "value": 0.03380039285714286,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.946s"
          },
          {
            "name": "RAJA_Seq-new_Basic_PI_REDUCE",
            "value": 0.012035392857142857,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.337s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE3_INT",
            "value": 0.004102821428571428,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.115s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE_STRUCT",
            "value": 0.031915035714285715,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.894s"
          },
          {
            "name": "RAJA_Seq-new_Basic_TRAP_INT",
            "value": 0.03131707142857143,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.877s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "Jason Burmark",
            "username": "MrBurmark",
            "email": "MrBurmark@users.noreply.github.com"
          },
          "committer": {
            "name": "GitHub",
            "username": "web-flow",
            "email": "noreply@github.com"
          },
          "id": "1d167298cf1ca50f6717c31f54fac8a34527c3d0",
          "message": "Merge pull request #700 from llnl/jacobi-2d-launches\n\nJacobi 2D Seq, OMP, and OMPTarget launch twice",
          "timestamp": "2026-08-06T01:57:56Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/1d167298cf1ca50f6717c31f54fac8a34527c3d0"
        },
        "date": 1785985304750,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_Seq-default_Basic_ARRAY_OF_PTRS",
            "value": 0.4984321428571428,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 13.956s"
          },
          {
            "name": "RAJA_Seq-default_Basic_COPY8",
            "value": 0.46647857142857146,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 13.061s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY",
            "value": 0.05303178571428571,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 112, total_time: 14.849s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY_ATOMIC",
            "value": 0.05301535714285715,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 112, total_time: 14.844s"
          },
          {
            "name": "RAJA_Seq-default_Basic_EMPTY",
            "value": 0.002991089285714286,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 112, total_time: 1.675s"
          },
          {
            "name": "RAJA_Seq-default_Basic_IF_QUAD",
            "value": 0.14094940476190476,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 112, total_time: 14.208s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INDEXLIST_3LOOP",
            "value": 0.11936892857142857,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 112, total_time: 6.685s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT3",
            "value": 0.15465392857142857,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 112, total_time: 43.303s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D",
            "value": 0.030177571428571432,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 112, total_time: 42.249s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.030298000000000002,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 112, total_time: 42.417s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MAT_MAT_SHARED",
            "value": 2.868532142857143,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 112, total_time: 8.032s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULADDSUB",
            "value": 0.15460000000000002,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 112, total_time: 30.302s"
          },
          {
            "name": "RAJA_Seq-default_Basic_NESTED_INIT",
            "value": 0.030206607142857143,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 112, total_time: 16.916s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_ATOMIC",
            "value": 0.012035035714285713,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.337s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_REDUCE",
            "value": 0.01203925,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.337s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE3_INT",
            "value": 0.007251607142857144,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.203s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE_STRUCT",
            "value": 0.032766642857142855,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.917s"
          },
          {
            "name": "RAJA_Seq-default_Basic_TRAP_INT",
            "value": 0.03131871428571429,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.877s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULTI_REDUCE",
            "value": 0.03391525,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.950s"
          },
          {
            "name": "RAJA_Seq-new_Basic_PI_REDUCE",
            "value": 0.012034928571428571,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.337s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE3_INT",
            "value": 0.004162714285714286,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.117s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE_STRUCT",
            "value": 0.03179917857142857,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.890s"
          },
          {
            "name": "RAJA_Seq-new_Basic_TRAP_INT",
            "value": 0.031312571428571426,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.877s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "Jason Burmark",
            "username": "MrBurmark",
            "email": "MrBurmark@users.noreply.github.com"
          },
          "committer": {
            "name": "GitHub",
            "username": "web-flow",
            "email": "noreply@github.com"
          },
          "id": "338cfeb7266607648bd06815ba103f61e357c2e5",
          "message": "Merge pull request #695 from llnl/multi-kernel-regions\n\nCaliper: Annotate kernels separately",
          "timestamp": "2026-08-10T20:40:11Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/338cfeb7266607648bd06815ba103f61e357c2e5"
        },
        "date": 1786406887030,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_Seq-default_Basic_ARRAY_OF_PTRS",
            "value": 0.4984535714285714,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 13.957s"
          },
          {
            "name": "RAJA_Seq-default_ARRAY_OF_PTRS_1",
            "value": 0.0019778,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 28000, total_time: 13.845s"
          },
          {
            "name": "RAJA_Seq-default_Basic_COPY8",
            "value": 0.4665071428571429,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 13.062s"
          },
          {
            "name": "RAJA_Seq-default_COPY8_1",
            "value": 0.0018559571428571427,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 28000, total_time: 12.992s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY",
            "value": 0.052920357142857144,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 112, total_time: 14.818s"
          },
          {
            "name": "RAJA_Seq-default_DAXPY_1",
            "value": 0.000021044428571428573,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 280000, total_time: 14.731s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY_ATOMIC",
            "value": 0.05289,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 112, total_time: 14.809s"
          },
          {
            "name": "RAJA_Seq-default_DAXPY_ATOMIC_1",
            "value": 0.000021038428571428574,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 280000, total_time: 14.727s"
          },
          {
            "name": "RAJA_Seq-default_Basic_EMPTY",
            "value": 0.0030036785714285716,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 112, total_time: 1.682s"
          },
          {
            "name": "RAJA_Seq-default_EMPTY_1",
            "value": 5.976000000000001e-7,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 560000, total_time: 1.673s"
          },
          {
            "name": "RAJA_Seq-default_Basic_IF_QUAD",
            "value": 0.14161408730158728,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 112, total_time: 14.275s"
          },
          {
            "name": "RAJA_Seq-default_IF_QUAD_1",
            "value": 0.00014464175485008818,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 100800, total_time: 13.122s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INDEXLIST_3LOOP",
            "value": 0.12008589285714284,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 112, total_time: 6.725s"
          },
          {
            "name": "RAJA_Seq-default_INDEXLIST_3LOOP_1",
            "value": 0.00007433571428571429,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 56000, total_time: 2.081s"
          },
          {
            "name": "RAJA_Seq-default_INDEXLIST_3LOOP_2",
            "value": 0.00010560964285714284,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 56000, total_time: 2.957s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT3",
            "value": 0.15478785714285714,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 112, total_time: 43.341s"
          },
          {
            "name": "RAJA_Seq-default_INIT3_1",
            "value": 0.0000614897142857143,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 280000, total_time: 43.043s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D",
            "value": 0.030277571428571428,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 112, total_time: 42.389s"
          },
          {
            "name": "RAJA_Seq-default_INIT_VIEW1D_1",
            "value": 0.0000024066685714285718,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1400000, total_time: 42.117s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.03040107142857143,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 112, total_time: 42.562s"
          },
          {
            "name": "RAJA_Seq-default_INIT_VIEW1D_OFFSET_1",
            "value": 0.000002414965714285714,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1400000, total_time: 42.262s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MAT_MAT_SHARED",
            "value": 2.8489,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 112, total_time: 7.977s"
          },
          {
            "name": "RAJA_Seq-default_MAT_MAT_SHARED_1",
            "value": 0.1066517142857143,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 2800, total_time: 7.466s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULADDSUB",
            "value": 0.15479183673469388,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 112, total_time: 30.339s"
          },
          {
            "name": "RAJA_Seq-default_MULADDSUB_1",
            "value": 0.00008779650145772595,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 196000, total_time: 30.114s"
          },
          {
            "name": "RAJA_Seq-default_Basic_NESTED_INIT",
            "value": 0.03029160714285714,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 112, total_time: 16.963s"
          },
          {
            "name": "RAJA_Seq-default_NESTED_INIT_1",
            "value": 0.000006019178571428572,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 560000, total_time: 16.854s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_ATOMIC",
            "value": 0.012046892857142857,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.337s"
          },
          {
            "name": "RAJA_Seq-default_PI_ATOMIC_1",
            "value": 0.00004813357142857143,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 28000, total_time: 0.337s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_REDUCE",
            "value": 0.012047571428571428,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.337s"
          },
          {
            "name": "RAJA_Seq-default_PI_REDUCE_1",
            "value": 0.00004813414285714285,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 28000, total_time: 0.337s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE3_INT",
            "value": 0.007532535714285715,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.211s"
          },
          {
            "name": "RAJA_Seq-default_REDUCE3_INT_1",
            "value": 0.000027508,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 28000, total_time: 0.193s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE_STRUCT",
            "value": 0.03310278571428571,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.927s"
          },
          {
            "name": "RAJA_Seq-default_REDUCE_STRUCT_1",
            "value": 0.00012522542857142856,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 28000, total_time: 0.877s"
          },
          {
            "name": "RAJA_Seq-default_Basic_TRAP_INT",
            "value": 0.031140964285714286,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.872s"
          },
          {
            "name": "RAJA_Seq-default_TRAP_INT_1",
            "value": 0.00012239371428571428,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 28000, total_time: 0.857s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULTI_REDUCE",
            "value": 0.03438921428571429,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.963s"
          },
          {
            "name": "RAJA_Seq-default_MULTI_REDUCE_1",
            "value": 0.00012609271428571427,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 28000, total_time: 0.883s"
          },
          {
            "name": "RAJA_Seq-new_Basic_PI_REDUCE",
            "value": 0.012047392857142857,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.337s"
          },
          {
            "name": "RAJA_Seq-new_PI_REDUCE_1",
            "value": 0.000048134428571428566,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 28000, total_time: 0.337s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE3_INT",
            "value": 0.004009607142857143,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.112s"
          },
          {
            "name": "RAJA_Seq-new_REDUCE3_INT_1",
            "value": 0.000013529842857142857,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 28000, total_time: 0.095s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE_STRUCT",
            "value": 0.032156142857142855,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.900s"
          },
          {
            "name": "RAJA_Seq-new_REDUCE_STRUCT_1",
            "value": 0.00012216085714285716,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 28000, total_time: 0.855s"
          },
          {
            "name": "RAJA_Seq-new_Basic_TRAP_INT",
            "value": 0.03130332142857142,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.876s"
          },
          {
            "name": "RAJA_Seq-new_TRAP_INT_1",
            "value": 0.00012179528571428572,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 28000, total_time: 0.853s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "Jason Burmark",
            "username": "MrBurmark",
            "email": "MrBurmark@users.noreply.github.com"
          },
          "committer": {
            "name": "GitHub",
            "username": "web-flow",
            "email": "noreply@github.com"
          },
          "id": "5301486b1ea58280079a4fe252710db5e01e758a",
          "message": "Merge pull request #687 from llnl/pr-from-fork/686\n\nPr from fork/686",
          "timestamp": "2026-08-20T19:54:32Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/5301486b1ea58280079a4fe252710db5e01e758a"
        },
        "date": 1787293819179,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_Seq-default_Basic_ARRAY_OF_PTRS",
            "value": 0.49634285714285714,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 13.898s"
          },
          {
            "name": "RAJA_Seq-default_ARRAY_OF_PTRS_1",
            "value": 0.0019704285714285713,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 28000, total_time: 13.793s"
          },
          {
            "name": "RAJA_Seq-default_Basic_COPY8",
            "value": 0.46580000000000005,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 13.042s"
          },
          {
            "name": "RAJA_Seq-default_COPY8_1",
            "value": 0.0018560999999999999,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 28000, total_time: 12.993s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY",
            "value": 0.05314714285714285,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 112, total_time: 14.881s"
          },
          {
            "name": "RAJA_Seq-default_DAXPY_1",
            "value": 0.000021026142857142855,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 280000, total_time: 14.718s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY_ATOMIC",
            "value": 0.05312321428571429,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 112, total_time: 14.874s"
          },
          {
            "name": "RAJA_Seq-default_DAXPY_ATOMIC_1",
            "value": 0.000021047000000000004,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 280000, total_time: 14.733s"
          },
          {
            "name": "RAJA_Seq-default_Basic_EMPTY",
            "value": 0.0030006428571428575,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 112, total_time: 1.680s"
          },
          {
            "name": "RAJA_Seq-default_EMPTY_1",
            "value": 5.975750000000001e-7,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 560000, total_time: 1.673s"
          },
          {
            "name": "RAJA_Seq-default_Basic_IF_QUAD",
            "value": 0.13972321428571427,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 112, total_time: 14.084s"
          },
          {
            "name": "RAJA_Seq-default_IF_QUAD_1",
            "value": 0.000144456569664903,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 100800, total_time: 13.105s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INDEXLIST_3LOOP",
            "value": 0.11991678571428571,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 112, total_time: 6.715s"
          },
          {
            "name": "RAJA_Seq-default_INDEXLIST_3LOOP_1",
            "value": 0.00007458464285714285,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 56000, total_time: 2.088s"
          },
          {
            "name": "RAJA_Seq-default_INDEXLIST_3LOOP_2",
            "value": 0.00010511321428571428,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 56000, total_time: 2.943s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT3",
            "value": 0.1543657142857143,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 112, total_time: 43.222s"
          },
          {
            "name": "RAJA_Seq-default_INIT3_1",
            "value": 0.00006123785714285715,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 280000, total_time: 42.867s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D",
            "value": 0.030261714285714288,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 112, total_time: 42.366s"
          },
          {
            "name": "RAJA_Seq-default_INIT_VIEW1D_1",
            "value": 0.000002402245714285714,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1400000, total_time: 42.039s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.03037607142857143,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 112, total_time: 42.526s"
          },
          {
            "name": "RAJA_Seq-default_INIT_VIEW1D_OFFSET_1",
            "value": 0.0000024102514285714288,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1400000, total_time: 42.179s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MAT_MAT_SHARED",
            "value": 2.8232214285714288,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 112, total_time: 7.905s"
          },
          {
            "name": "RAJA_Seq-default_MAT_MAT_SHARED_1",
            "value": 0.10652728571428571,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 2800, total_time: 7.457s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULADDSUB",
            "value": 0.15428163265306122,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 112, total_time: 30.239s"
          },
          {
            "name": "RAJA_Seq-default_MULADDSUB_1",
            "value": 0.0000875271137026239,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 196000, total_time: 30.022s"
          },
          {
            "name": "RAJA_Seq-default_Basic_NESTED_INIT",
            "value": 0.03030625,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 112, total_time: 16.971s"
          },
          {
            "name": "RAJA_Seq-default_NESTED_INIT_1",
            "value": 0.0000060086428571428575,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 560000, total_time: 16.824s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_ATOMIC",
            "value": 0.012046535714285714,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.337s"
          },
          {
            "name": "RAJA_Seq-default_PI_ATOMIC_1",
            "value": 0.000048131857142857136,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 28000, total_time: 0.337s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_REDUCE",
            "value": 0.012047607142857141,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.337s"
          },
          {
            "name": "RAJA_Seq-default_PI_REDUCE_1",
            "value": 0.000048132571428571425,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 28000, total_time: 0.337s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE3_INT",
            "value": 0.007600142857142857,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.213s"
          },
          {
            "name": "RAJA_Seq-default_REDUCE3_INT_1",
            "value": 0.000027422714285714285,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 28000, total_time: 0.192s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE_STRUCT",
            "value": 0.032782928571428574,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.918s"
          },
          {
            "name": "RAJA_Seq-default_REDUCE_STRUCT_1",
            "value": 0.00012460171428571427,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 28000, total_time: 0.872s"
          },
          {
            "name": "RAJA_Seq-default_Basic_TRAP_INT",
            "value": 0.031200964285714287,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.874s"
          },
          {
            "name": "RAJA_Seq-default_TRAP_INT_1",
            "value": 0.00012250642857142857,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 28000, total_time: 0.858s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULTI_REDUCE",
            "value": 0.03413525,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.956s"
          },
          {
            "name": "RAJA_Seq-default_MULTI_REDUCE_1",
            "value": 0.00012585,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 28000, total_time: 0.881s"
          },
          {
            "name": "RAJA_Seq-new_Basic_PI_REDUCE",
            "value": 0.012047464285714286,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.337s"
          },
          {
            "name": "RAJA_Seq-new_PI_REDUCE_1",
            "value": 0.000048133285714285714,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 28000, total_time: 0.337s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE3_INT",
            "value": 0.003986571428571429,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.112s"
          },
          {
            "name": "RAJA_Seq-new_REDUCE3_INT_1",
            "value": 0.00001374187142857143,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 28000, total_time: 0.096s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE_STRUCT",
            "value": 0.032065392857142855,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.898s"
          },
          {
            "name": "RAJA_Seq-new_REDUCE_STRUCT_1",
            "value": 0.000121688,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 28000, total_time: 0.852s"
          },
          {
            "name": "RAJA_Seq-new_Basic_TRAP_INT",
            "value": 0.03126392857142857,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.875s"
          },
          {
            "name": "RAJA_Seq-new_TRAP_INT_1",
            "value": 0.00012165285714285715,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 28000, total_time: 0.852s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "Rich Hornung",
            "username": "rhornung67",
            "email": "hornung1@llnl.gov"
          },
          "committer": {
            "name": "GitHub",
            "username": "web-flow",
            "email": "noreply@github.com"
          },
          "id": "4f9e9a85ff9e5e0ce75c230eed7244b9f305cfb0",
          "message": "Merge pull request #688 from llnl/woptim/rsc-2026-6-0\n\nFrom RAJA: Point at RSC release candidate",
          "timestamp": "2026-08-21T21:34:01Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/4f9e9a85ff9e5e0ce75c230eed7244b9f305cfb0"
        },
        "date": 1787352075373,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_Seq-default_Basic_ARRAY_OF_PTRS",
            "value": 0.49781785714285715,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 13.939s"
          },
          {
            "name": "RAJA_Seq-default_ARRAY_OF_PTRS_1",
            "value": 0.0019764857142857144,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 28000, total_time: 13.835s"
          },
          {
            "name": "RAJA_Seq-default_Basic_COPY8",
            "value": 0.46651071428571433,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 13.062s"
          },
          {
            "name": "RAJA_Seq-default_COPY8_1",
            "value": 0.0018578285714285713,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 28000, total_time: 13.005s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY",
            "value": 0.053094642857142854,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 112, total_time: 14.867s"
          },
          {
            "name": "RAJA_Seq-default_DAXPY_1",
            "value": 0.000021092,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 280000, total_time: 14.764s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY_ATOMIC",
            "value": 0.053119285714285716,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 112, total_time: 14.873s"
          },
          {
            "name": "RAJA_Seq-default_DAXPY_ATOMIC_1",
            "value": 0.000021102714285714285,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 280000, total_time: 14.772s"
          },
          {
            "name": "RAJA_Seq-default_Basic_EMPTY",
            "value": 0.0030003928571428573,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 112, total_time: 1.680s"
          },
          {
            "name": "RAJA_Seq-default_EMPTY_1",
            "value": 5.976142857142857e-7,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 560000, total_time: 1.673s"
          },
          {
            "name": "RAJA_Seq-default_Basic_IF_QUAD",
            "value": 0.1397470238095238,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 112, total_time: 14.086s"
          },
          {
            "name": "RAJA_Seq-default_IF_QUAD_1",
            "value": 0.00014487544091710758,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 100800, total_time: 13.143s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INDEXLIST_3LOOP",
            "value": 0.1198730357142857,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 112, total_time: 6.713s"
          },
          {
            "name": "RAJA_Seq-default_INDEXLIST_3LOOP_1",
            "value": 0.00007479785714285713,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 56000, total_time: 2.094s"
          },
          {
            "name": "RAJA_Seq-default_INDEXLIST_3LOOP_2",
            "value": 0.00010520285714285714,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 56000, total_time: 2.946s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT3",
            "value": 0.1548357142857143,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 112, total_time: 43.354s"
          },
          {
            "name": "RAJA_Seq-default_INIT3_1",
            "value": 0.00006131814285714285,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 280000, total_time: 42.923s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D",
            "value": 0.030302428571428574,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 112, total_time: 42.423s"
          },
          {
            "name": "RAJA_Seq-default_INIT_VIEW1D_1",
            "value": 0.000002405691428571429,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1400000, total_time: 42.100s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.03042371428571429,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 112, total_time: 42.593s"
          },
          {
            "name": "RAJA_Seq-default_INIT_VIEW1D_OFFSET_1",
            "value": 0.00000241444,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1400000, total_time: 42.253s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MAT_MAT_SHARED",
            "value": 2.8582821428571425,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 112, total_time: 8.003s"
          },
          {
            "name": "RAJA_Seq-default_MAT_MAT_SHARED_1",
            "value": 0.10554028571428571,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 2800, total_time: 7.388s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULADDSUB",
            "value": 0.15466938775510206,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 112, total_time: 30.315s"
          },
          {
            "name": "RAJA_Seq-default_MULADDSUB_1",
            "value": 0.00008761545189504373,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 196000, total_time: 30.052s"
          },
          {
            "name": "RAJA_Seq-default_Basic_NESTED_INIT",
            "value": 0.030326964285714283,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 112, total_time: 16.983s"
          },
          {
            "name": "RAJA_Seq-default_NESTED_INIT_1",
            "value": 0.000006020321428571428,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 560000, total_time: 16.857s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_ATOMIC",
            "value": 0.012051714285714287,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.337s"
          },
          {
            "name": "RAJA_Seq-default_PI_ATOMIC_1",
            "value": 0.000048148,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 28000, total_time: 0.337s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_REDUCE",
            "value": 0.012047821428571429,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.337s"
          },
          {
            "name": "RAJA_Seq-default_PI_REDUCE_1",
            "value": 0.00004813314285714286,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 28000, total_time: 0.337s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE3_INT",
            "value": 0.007638142857142857,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.214s"
          },
          {
            "name": "RAJA_Seq-default_REDUCE3_INT_1",
            "value": 0.000028116285714285713,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 28000, total_time: 0.197s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE_STRUCT",
            "value": 0.03299178571428572,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.924s"
          },
          {
            "name": "RAJA_Seq-default_REDUCE_STRUCT_1",
            "value": 0.00012485257142857143,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 28000, total_time: 0.874s"
          },
          {
            "name": "RAJA_Seq-default_Basic_TRAP_INT",
            "value": 0.031273,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.876s"
          },
          {
            "name": "RAJA_Seq-default_TRAP_INT_1",
            "value": 0.00012110585714285714,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 28000, total_time: 0.848s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULTI_REDUCE",
            "value": 0.03431242857142857,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.961s"
          },
          {
            "name": "RAJA_Seq-default_MULTI_REDUCE_1",
            "value": 0.00012619442857142858,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 28000, total_time: 0.883s"
          },
          {
            "name": "RAJA_Seq-new_Basic_PI_REDUCE",
            "value": 0.012048428571428571,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.337s"
          },
          {
            "name": "RAJA_Seq-new_PI_REDUCE_1",
            "value": 0.00004813514285714286,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 28000, total_time: 0.337s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE3_INT",
            "value": 0.004040321428571428,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.113s"
          },
          {
            "name": "RAJA_Seq-new_REDUCE3_INT_1",
            "value": 0.000013661114285714285,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 28000, total_time: 0.096s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE_STRUCT",
            "value": 0.03217242857142857,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.901s"
          },
          {
            "name": "RAJA_Seq-new_REDUCE_STRUCT_1",
            "value": 0.00012184857142857144,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 28000, total_time: 0.853s"
          },
          {
            "name": "RAJA_Seq-new_Basic_TRAP_INT",
            "value": 0.031320321428571433,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 112, total_time: 0.877s"
          },
          {
            "name": "RAJA_Seq-new_TRAP_INT_1",
            "value": 0.00012210014285714285,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 28000, total_time: 0.855s"
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
      },
      {
        "commit": {
          "author": {
            "name": "Rich Hornung",
            "username": "rhornung67",
            "email": "hornung1@llnl.gov"
          },
          "committer": {
            "name": "GitHub",
            "username": "web-flow",
            "email": "noreply@github.com"
          },
          "id": "2e4038b7fee213ead90d2108b8458b77c6ac0006",
          "message": "Merge pull request #678 from llnl/main-mergeback\n\nMain mergeback to develop after v2025.12.1 release",
          "timestamp": "2026-03-30T22:11:27Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/2e4038b7fee213ead90d2108b8458b77c6ac0006"
        },
        "date": 1774915329461,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_OpenMP-default_Basic_ARRAY_OF_PTRS",
            "value": 4.9616,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.240s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_COPY8",
            "value": 4.73992,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.185s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY",
            "value": 0.1541464,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.385s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY_ATOMIC",
            "value": 1.6881399999999998,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 4.220s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_EMPTY",
            "value": 0.10067999999999999,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 0.503s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_IF_QUAD",
            "value": 2.0723444444444445,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 1.865s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INDEXLIST_3LOOP",
            "value": 1.570602,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.785s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT3",
            "value": 0.811056,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 2.028s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D",
            "value": 0.20938079999999998,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 2.617s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.1576792,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 1.971s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MAT_MAT_SHARED",
            "value": 157.884,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 3.947s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULADDSUB",
            "value": 0.8242971428571428,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 1.443s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_NESTED_INIT",
            "value": 0.51576,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 2.579s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_ATOMIC",
            "value": 278.832,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 69.708s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_REDUCE",
            "value": 0.3490724,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.087s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE3_INT",
            "value": 0.2408828,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.060s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE_STRUCT",
            "value": 0.451308,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.113s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_TRAP_INT",
            "value": 0.858292,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.215s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULTI_REDUCE",
            "value": 0.687852,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.172s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_PI_REDUCE",
            "value": 0.3421244,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.086s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE3_INT",
            "value": 0.1587,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.040s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE_STRUCT",
            "value": 0.45098,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.113s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_TRAP_INT",
            "value": 0.867596,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.217s"
          },
          {
            "name": "RAJA_Seq-default_Basic_ARRAY_OF_PTRS",
            "value": 37.06504,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 9.266s"
          },
          {
            "name": "RAJA_Seq-default_Basic_COPY8",
            "value": 31.41976,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 7.855s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY",
            "value": 3.9623759999999995,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 9.906s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY_ATOMIC",
            "value": 3.961324,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 9.903s"
          },
          {
            "name": "RAJA_Seq-default_Basic_EMPTY",
            "value": 1.318614,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 6.593s"
          },
          {
            "name": "RAJA_Seq-default_Basic_IF_QUAD",
            "value": 34.02444444444444,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 30.622s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INDEXLIST_3LOOP",
            "value": 26.5144,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 13.257s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT3",
            "value": 11.59644,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 28.991s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D",
            "value": 2.867232,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 35.840s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 3.007552,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 37.594s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MAT_MAT_SHARED",
            "value": 2614.928,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 65.373s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULADDSUB",
            "value": 12.549771428571427,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 21.962s"
          },
          {
            "name": "RAJA_Seq-default_Basic_NESTED_INIT",
            "value": 3.2291600000000003,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 16.146s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_ATOMIC",
            "value": 5.31744,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.329s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_REDUCE",
            "value": 5.31836,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.330s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE3_INT",
            "value": 2.959496,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.740s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE_STRUCT",
            "value": 6.59352,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.648s"
          },
          {
            "name": "RAJA_Seq-default_Basic_TRAP_INT",
            "value": 13.61852,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.405s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULTI_REDUCE",
            "value": 10.7832,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 2.696s"
          },
          {
            "name": "RAJA_Seq-new_Basic_PI_REDUCE",
            "value": 5.32796,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.332s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE3_INT",
            "value": 1.080364,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.270s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE_STRUCT",
            "value": 5.89396,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.473s"
          },
          {
            "name": "RAJA_Seq-new_Basic_TRAP_INT",
            "value": 13.84256,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.461s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "Rich Hornung",
            "username": "rhornung67",
            "email": "hornung1@llnl.gov"
          },
          "committer": {
            "name": "GitHub",
            "username": "web-flow",
            "email": "noreply@github.com"
          },
          "id": "3111bb9edb2a22dcc64c7589d94a080912db06be",
          "message": "Merge pull request #679 from llnl/task/rhornung67/fix-docs\n\nDoc fixes",
          "timestamp": "2026-04-01T15:59:31Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/3111bb9edb2a22dcc64c7589d94a080912db06be"
        },
        "date": 1775065761040,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_OpenMP-default_Basic_ARRAY_OF_PTRS",
            "value": 6.3836,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.596s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_COPY8",
            "value": 4.73688,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.184s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY",
            "value": 0.165202,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.413s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY_ATOMIC",
            "value": 1.6887759999999998,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 4.222s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_EMPTY",
            "value": 0.1024586,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 0.512s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_IF_QUAD",
            "value": 2.0671,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 1.860s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INDEXLIST_3LOOP",
            "value": 1.575346,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.788s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT3",
            "value": 0.818532,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 2.046s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D",
            "value": 0.2092408,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 2.616s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.1576072,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 1.970s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MAT_MAT_SHARED",
            "value": 158.2692,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 3.957s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULADDSUB",
            "value": 0.8463142857142857,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 1.481s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_NESTED_INIT",
            "value": 0.46983800000000003,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 2.349s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_ATOMIC",
            "value": 279.7104,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 69.928s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_REDUCE",
            "value": 0.3485528,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.087s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE3_INT",
            "value": 0.2408064,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.060s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE_STRUCT",
            "value": 0.450308,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.113s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_TRAP_INT",
            "value": 0.85908,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.215s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULTI_REDUCE",
            "value": 0.687524,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.172s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_PI_REDUCE",
            "value": 0.34387,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.086s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE3_INT",
            "value": 0.145358,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.036s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE_STRUCT",
            "value": 0.45176,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.113s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_TRAP_INT",
            "value": 0.863164,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.216s"
          },
          {
            "name": "RAJA_Seq-default_Basic_ARRAY_OF_PTRS",
            "value": 37.96456,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 9.491s"
          },
          {
            "name": "RAJA_Seq-default_Basic_COPY8",
            "value": 31.27316,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 7.818s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY",
            "value": 3.9555079999999996,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 9.889s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY_ATOMIC",
            "value": 3.9578200000000003,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 9.895s"
          },
          {
            "name": "RAJA_Seq-default_Basic_EMPTY",
            "value": 1.31865,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 6.593s"
          },
          {
            "name": "RAJA_Seq-default_Basic_IF_QUAD",
            "value": 34.05177777777778,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 30.647s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INDEXLIST_3LOOP",
            "value": 26.496,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 13.248s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT3",
            "value": 11.61712,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 29.043s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D",
            "value": 2.875288,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 35.941s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 2.9869760000000003,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 37.337s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MAT_MAT_SHARED",
            "value": 2615.208,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 65.380s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULADDSUB",
            "value": 12.455542857142857,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 21.797s"
          },
          {
            "name": "RAJA_Seq-default_Basic_NESTED_INIT",
            "value": 3.2218200000000006,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 16.109s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_ATOMIC",
            "value": 5.31752,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.329s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_REDUCE",
            "value": 5.31828,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.330s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE3_INT",
            "value": 2.94894,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.737s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE_STRUCT",
            "value": 6.54476,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.636s"
          },
          {
            "name": "RAJA_Seq-default_Basic_TRAP_INT",
            "value": 13.46312,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.366s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULTI_REDUCE",
            "value": 10.7776,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 2.694s"
          },
          {
            "name": "RAJA_Seq-new_Basic_PI_REDUCE",
            "value": 5.31776,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.329s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE3_INT",
            "value": 1.085452,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.271s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE_STRUCT",
            "value": 5.90416,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.476s"
          },
          {
            "name": "RAJA_Seq-new_Basic_TRAP_INT",
            "value": 13.5282,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.382s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "Adrien Bernede",
            "username": "adrienbernede",
            "email": "51493078+adrienbernede@users.noreply.github.com"
          },
          "committer": {
            "name": "GitHub",
            "username": "web-flow",
            "email": "noreply@github.com"
          },
          "id": "95c360ddf585410e6123956c72a2c648111ece29",
          "message": "Merge pull request #652 from llnl/woptim/rocm7",
          "timestamp": "2026-04-16T19:31:14Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/95c360ddf585410e6123956c72a2c648111ece29"
        },
        "date": 1776372464841,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_OpenMP-default_Basic_ARRAY_OF_PTRS",
            "value": 4.32612,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.082s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_COPY8",
            "value": 3.630848,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.908s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY",
            "value": 0.1518096,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.380s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY_ATOMIC",
            "value": 1.6871,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 4.218s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_EMPTY",
            "value": 0.10416220000000001,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 0.521s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_IF_QUAD",
            "value": 2.0684333333333336,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 1.862s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INDEXLIST_3LOOP",
            "value": 1.578798,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.789s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT3",
            "value": 0.8204560000000002,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 2.051s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D",
            "value": 0.20836,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 2.604s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.1565928,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 1.957s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MAT_MAT_SHARED",
            "value": 158.70600000000002,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 3.968s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULADDSUB",
            "value": 0.8348285714285715,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 1.461s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_NESTED_INIT",
            "value": 0.49732200000000004,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 2.487s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_ATOMIC",
            "value": 278.384,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 69.596s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_REDUCE",
            "value": 0.3481696,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.087s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE3_INT",
            "value": 0.2344704,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.059s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE_STRUCT",
            "value": 0.451756,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.113s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_TRAP_INT",
            "value": 0.855224,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.214s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULTI_REDUCE",
            "value": 0.687452,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.172s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_PI_REDUCE",
            "value": 0.3419032,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.085s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE3_INT",
            "value": 0.1416296,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.035s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE_STRUCT",
            "value": 0.44752,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.112s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_TRAP_INT",
            "value": 0.862924,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.216s"
          },
          {
            "name": "RAJA_Seq-default_Basic_ARRAY_OF_PTRS",
            "value": 37.97784,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 9.494s"
          },
          {
            "name": "RAJA_Seq-default_Basic_COPY8",
            "value": 31.40244,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 7.851s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY",
            "value": 3.9560239999999998,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 9.890s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY_ATOMIC",
            "value": 3.947804,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 9.870s"
          },
          {
            "name": "RAJA_Seq-default_Basic_EMPTY",
            "value": 1.318644,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 6.593s"
          },
          {
            "name": "RAJA_Seq-default_Basic_IF_QUAD",
            "value": 33.80122222222222,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 30.421s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INDEXLIST_3LOOP",
            "value": 26.484599999999997,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 13.242s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT3",
            "value": 11.57552,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 28.939s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D",
            "value": 2.8638959999999996,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 35.799s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 2.9892880000000006,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 37.366s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MAT_MAT_SHARED",
            "value": 2620.1319999999996,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 65.503s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULADDSUB",
            "value": 12.548914285714284,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 21.961s"
          },
          {
            "name": "RAJA_Seq-default_Basic_NESTED_INIT",
            "value": 3.20446,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 16.022s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_ATOMIC",
            "value": 5.31752,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.329s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_REDUCE",
            "value": 5.31816,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.330s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE3_INT",
            "value": 2.9587,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.740s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE_STRUCT",
            "value": 6.30228,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.576s"
          },
          {
            "name": "RAJA_Seq-default_Basic_TRAP_INT",
            "value": 13.368,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.342s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULTI_REDUCE",
            "value": 10.78004,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 2.695s"
          },
          {
            "name": "RAJA_Seq-new_Basic_PI_REDUCE",
            "value": 5.31752,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.329s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE3_INT",
            "value": 1.082572,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.271s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE_STRUCT",
            "value": 5.88552,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.471s"
          },
          {
            "name": "RAJA_Seq-new_Basic_TRAP_INT",
            "value": 13.18612,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.297s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "Jason Burmark",
            "username": "MrBurmark",
            "email": "MrBurmark@users.noreply.github.com"
          },
          "committer": {
            "name": "GitHub",
            "username": "web-flow",
            "email": "noreply@github.com"
          },
          "id": "9bc67910045283981831de42fe96bd151eee0761",
          "message": "Merge pull request #681 from llnl/pr-from-fork/680\n\nPr from fork/680",
          "timestamp": "2026-04-20T16:10:32Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/9bc67910045283981831de42fe96bd151eee0761"
        },
        "date": 1776704788321,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_OpenMP-default_Basic_ARRAY_OF_PTRS",
            "value": 6.43064,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.608s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_COPY8",
            "value": 4.75448,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.189s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY",
            "value": 0.1689012,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.422s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY_ATOMIC",
            "value": 1.6885,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 4.221s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_EMPTY",
            "value": 0.0999718,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 0.500s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_IF_QUAD",
            "value": 2.0997777777777777,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 1.890s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INDEXLIST_3LOOP",
            "value": 1.585322,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.793s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT3",
            "value": 0.813252,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 2.033s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D",
            "value": 0.2133168,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 2.666s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.1581448,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 1.977s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MAT_MAT_SHARED",
            "value": 157.0984,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 3.927s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULADDSUB",
            "value": 1.1692171428571427,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 2.046s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_NESTED_INIT",
            "value": 0.470354,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 2.352s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_ATOMIC",
            "value": 274.348,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 68.587s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_REDUCE",
            "value": 0.3485988,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.087s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE3_INT",
            "value": 0.2488364,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.062s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE_STRUCT",
            "value": 0.453516,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.113s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_TRAP_INT",
            "value": 0.858516,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.215s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULTI_REDUCE",
            "value": 0.688776,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.172s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_PI_REDUCE",
            "value": 0.343274,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.086s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE3_INT",
            "value": 0.1467472,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.037s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE_STRUCT",
            "value": 0.448148,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.112s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_TRAP_INT",
            "value": 0.864548,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.216s"
          },
          {
            "name": "RAJA_Seq-default_Basic_ARRAY_OF_PTRS",
            "value": 37.05296,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 9.263s"
          },
          {
            "name": "RAJA_Seq-default_Basic_COPY8",
            "value": 31.24556,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 7.811s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY",
            "value": 3.9769039999999993,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 9.942s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY_ATOMIC",
            "value": 3.9783560000000007,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 9.946s"
          },
          {
            "name": "RAJA_Seq-default_Basic_EMPTY",
            "value": 1.318568,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 6.593s"
          },
          {
            "name": "RAJA_Seq-default_Basic_IF_QUAD",
            "value": 33.772222222222226,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 30.395s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INDEXLIST_3LOOP",
            "value": 26.538,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 13.269s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT3",
            "value": 11.67188,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 29.180s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D",
            "value": 2.9016719999999996,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 36.271s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 3.0311360000000005,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 37.889s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MAT_MAT_SHARED",
            "value": 2591.28,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 64.782s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULADDSUB",
            "value": 12.430914285714287,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 21.754s"
          },
          {
            "name": "RAJA_Seq-default_Basic_NESTED_INIT",
            "value": 3.21732,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 16.087s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_ATOMIC",
            "value": 5.31724,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.329s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_REDUCE",
            "value": 5.31728,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.329s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE3_INT",
            "value": 2.962116,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.741s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE_STRUCT",
            "value": 6.62036,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.655s"
          },
          {
            "name": "RAJA_Seq-default_Basic_TRAP_INT",
            "value": 13.18548,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.296s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULTI_REDUCE",
            "value": 10.7784,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 2.695s"
          },
          {
            "name": "RAJA_Seq-new_Basic_PI_REDUCE",
            "value": 5.3172,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.329s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE3_INT",
            "value": 1.08566,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.271s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE_STRUCT",
            "value": 5.88632,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.472s"
          },
          {
            "name": "RAJA_Seq-new_Basic_TRAP_INT",
            "value": 13.695,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.424s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "Jason Burmark",
            "username": "MrBurmark",
            "email": "MrBurmark@users.noreply.github.com"
          },
          "committer": {
            "name": "GitHub",
            "username": "web-flow",
            "email": "noreply@github.com"
          },
          "id": "9bc67910045283981831de42fe96bd151eee0761",
          "message": "Merge pull request #681 from llnl/pr-from-fork/680\n\nPr from fork/680",
          "timestamp": "2026-04-20T16:10:32Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/9bc67910045283981831de42fe96bd151eee0761"
        },
        "date": 1779383177847,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_OpenMP-default_Basic_ARRAY_OF_PTRS",
            "value": 5.10748,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.277s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_COPY8",
            "value": 4.65004,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.163s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY",
            "value": 0.1486896,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.372s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY_ATOMIC",
            "value": 1.6891559999999997,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 4.223s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_EMPTY",
            "value": 0.1040386,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 0.520s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_IF_QUAD",
            "value": 2.0374666666666665,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 1.834s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INDEXLIST_3LOOP",
            "value": 1.557096,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.779s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT3",
            "value": 0.719676,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 1.799s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D",
            "value": 0.209232,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 2.615s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.1570512,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 1.963s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MAT_MAT_SHARED",
            "value": 140.0108,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 3.500s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULADDSUB",
            "value": 0.7450399999999999,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 1.304s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_NESTED_INIT",
            "value": 0.47436999999999996,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 2.372s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_ATOMIC",
            "value": 257.3104,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 64.328s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_REDUCE",
            "value": 0.3477548,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.087s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE3_INT",
            "value": 0.2111464,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.053s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE_STRUCT",
            "value": 0.43196,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.108s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_TRAP_INT",
            "value": 0.858872,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.215s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULTI_REDUCE",
            "value": 0.68954,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.172s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_PI_REDUCE",
            "value": 0.3427928,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.086s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE3_INT",
            "value": 0.135334,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.034s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE_STRUCT",
            "value": 0.444204,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.111s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_TRAP_INT",
            "value": 0.860664,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.215s"
          },
          {
            "name": "RAJA_Seq-default_Basic_ARRAY_OF_PTRS",
            "value": 37.2086,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 9.302s"
          },
          {
            "name": "RAJA_Seq-default_Basic_COPY8",
            "value": 31.23856,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 7.810s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY",
            "value": 3.9652239999999996,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 9.913s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY_ATOMIC",
            "value": 3.958392,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 9.896s"
          },
          {
            "name": "RAJA_Seq-default_Basic_EMPTY",
            "value": 1.324458,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 6.622s"
          },
          {
            "name": "RAJA_Seq-default_Basic_IF_QUAD",
            "value": 34.083666666666666,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 30.675s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INDEXLIST_3LOOP",
            "value": 26.4772,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 13.239s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT3",
            "value": 11.57604,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 28.940s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D",
            "value": 2.8664240000000003,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 35.830s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 2.991064,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 37.388s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MAT_MAT_SHARED",
            "value": 2620.156,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 65.504s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULADDSUB",
            "value": 12.356171428571429,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 21.623s"
          },
          {
            "name": "RAJA_Seq-default_Basic_NESTED_INIT",
            "value": 3.2340800000000005,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 16.170s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_ATOMIC",
            "value": 5.31768,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.329s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_REDUCE",
            "value": 5.31804,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.330s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE3_INT",
            "value": 2.97494,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.744s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE_STRUCT",
            "value": 6.56568,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.641s"
          },
          {
            "name": "RAJA_Seq-default_Basic_TRAP_INT",
            "value": 13.32092,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.330s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULTI_REDUCE",
            "value": 10.77532,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 2.694s"
          },
          {
            "name": "RAJA_Seq-new_Basic_PI_REDUCE",
            "value": 5.3176,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.329s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE3_INT",
            "value": 1.087444,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.272s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE_STRUCT",
            "value": 5.8818,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.470s"
          },
          {
            "name": "RAJA_Seq-new_Basic_TRAP_INT",
            "value": 13.570560000000002,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.393s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "Adrien Bernede",
            "username": "adrienbernede",
            "email": "51493078+adrienbernede@users.noreply.github.com"
          },
          "committer": {
            "name": "GitHub",
            "username": "web-flow",
            "email": "noreply@github.com"
          },
          "id": "1f660e77acdb6f12df7a4e39a25172328afb7352",
          "message": "Merge pull request #645 from llnl/woptim/ci-logs-and-errors\n\nImprove CI jobs logs and error handling",
          "timestamp": "2026-06-16T16:08:18Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/1f660e77acdb6f12df7a4e39a25172328afb7352"
        },
        "date": 1781641548070,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_OpenMP-default_Basic_ARRAY_OF_PTRS",
            "value": 6.3732,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.593s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_COPY8",
            "value": 4.63696,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.159s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY",
            "value": 0.16902759999999997,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.423s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY_ATOMIC",
            "value": 1.6884440000000003,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 4.221s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_EMPTY",
            "value": 0.103675,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 0.518s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_IF_QUAD",
            "value": 2.036888888888889,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 1.833s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INDEXLIST_3LOOP",
            "value": 1.549462,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.775s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT3",
            "value": 0.710072,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 1.775s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D",
            "value": 0.21009519999999995,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 2.626s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.1579016,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 1.974s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MAT_MAT_SHARED",
            "value": 140.1408,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 3.504s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULADDSUB",
            "value": 0.7456342857142857,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 1.305s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_NESTED_INIT",
            "value": 0.4733,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 2.366s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_ATOMIC",
            "value": 260.5264,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 65.132s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_REDUCE",
            "value": 0.34806159999999997,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.087s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE3_INT",
            "value": 0.2205816,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.055s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE_STRUCT",
            "value": 0.434616,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.109s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_TRAP_INT",
            "value": 0.858408,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.215s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULTI_REDUCE",
            "value": 0.689368,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.172s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_PI_REDUCE",
            "value": 0.342322,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.086s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE3_INT",
            "value": 0.1561204,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.039s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE_STRUCT",
            "value": 0.448992,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.112s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_TRAP_INT",
            "value": 0.863288,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.216s"
          },
          {
            "name": "RAJA_Seq-default_Basic_ARRAY_OF_PTRS",
            "value": 37.10828,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 9.277s"
          },
          {
            "name": "RAJA_Seq-default_Basic_COPY8",
            "value": 31.2864,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 7.822s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY",
            "value": 3.9488280000000007,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 9.872s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY_ATOMIC",
            "value": 3.944968,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 9.862s"
          },
          {
            "name": "RAJA_Seq-default_Basic_EMPTY",
            "value": 1.3222099999999999,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 6.611s"
          },
          {
            "name": "RAJA_Seq-default_Basic_IF_QUAD",
            "value": 33.781,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 30.403s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INDEXLIST_3LOOP",
            "value": 26.4712,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 13.236s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT3",
            "value": 11.567440000000001,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 28.919s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D",
            "value": 2.8555040000000003,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 35.694s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 2.9857119999999995,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 37.321s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MAT_MAT_SHARED",
            "value": 2623.0640000000003,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 65.577s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULADDSUB",
            "value": 12.369428571428571,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 21.646s"
          },
          {
            "name": "RAJA_Seq-default_Basic_NESTED_INIT",
            "value": 3.1959800000000005,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 15.980s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_ATOMIC",
            "value": 5.3176,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.329s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_REDUCE",
            "value": 5.31748,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.329s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE3_INT",
            "value": 2.965736,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.741s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE_STRUCT",
            "value": 6.57748,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.644s"
          },
          {
            "name": "RAJA_Seq-default_Basic_TRAP_INT",
            "value": 13.1888,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.297s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULTI_REDUCE",
            "value": 10.773,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 2.693s"
          },
          {
            "name": "RAJA_Seq-new_Basic_PI_REDUCE",
            "value": 5.31752,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.329s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE3_INT",
            "value": 1.087656,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.272s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE_STRUCT",
            "value": 5.88912,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.472s"
          },
          {
            "name": "RAJA_Seq-new_Basic_TRAP_INT",
            "value": 13.1882,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.297s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "Jason Burmark",
            "username": "MrBurmark",
            "email": "MrBurmark@users.noreply.github.com"
          },
          "committer": {
            "name": "GitHub",
            "username": "web-flow",
            "email": "noreply@github.com"
          },
          "id": "3d72e32abcb569a20f48069f2b639663f54ca04f",
          "message": "Merge pull request #690 from llnl/bugfix/burmark1/flop_counts\n\nFix flop counts in MASS3DEA and DIFFUSION3DPA",
          "timestamp": "2026-06-22T15:32:25Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/3d72e32abcb569a20f48069f2b639663f54ca04f"
        },
        "date": 1782145878403,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_OpenMP-default_Basic_ARRAY_OF_PTRS",
            "value": 5.93896,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.485s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_COPY8",
            "value": 4.65484,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.164s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY",
            "value": 0.1485092,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.371s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY_ATOMIC",
            "value": 1.687916,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 4.220s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_EMPTY",
            "value": 0.1003924,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 0.502s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_IF_QUAD",
            "value": 2.011355555555556,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 1.810s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INDEXLIST_3LOOP",
            "value": 1.490162,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.745s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT3",
            "value": 0.710724,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 1.777s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D",
            "value": 0.209136,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 2.614s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.157216,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 1.965s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MAT_MAT_SHARED",
            "value": 140.85119999999998,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 3.521s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULADDSUB",
            "value": 0.7368514285714286,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 1.289s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_NESTED_INIT",
            "value": 0.44700199999999995,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 2.235s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_ATOMIC",
            "value": 327.7296,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 81.932s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_REDUCE",
            "value": 0.3473708,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.087s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE3_INT",
            "value": 0.2078916,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.052s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE_STRUCT",
            "value": 0.43022,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.108s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_TRAP_INT",
            "value": 0.858604,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.215s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULTI_REDUCE",
            "value": 0.689584,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.172s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_PI_REDUCE",
            "value": 0.3422468,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.086s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE3_INT",
            "value": 0.1743024,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.044s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE_STRUCT",
            "value": 0.427268,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.107s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_TRAP_INT",
            "value": 0.86012,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.215s"
          },
          {
            "name": "RAJA_Seq-default_Basic_ARRAY_OF_PTRS",
            "value": 37.134,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 9.284s"
          },
          {
            "name": "RAJA_Seq-default_Basic_COPY8",
            "value": 31.3404,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 7.835s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY",
            "value": 4.00448,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 10.011s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY_ATOMIC",
            "value": 3.996828,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 9.992s"
          },
          {
            "name": "RAJA_Seq-default_Basic_EMPTY",
            "value": 1.32081,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 6.604s"
          },
          {
            "name": "RAJA_Seq-default_Basic_IF_QUAD",
            "value": 34.31088888888889,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 30.880s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INDEXLIST_3LOOP",
            "value": 26.5206,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 13.260s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT3",
            "value": 11.67972,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 29.199s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D",
            "value": 2.901616,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 36.270s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 2.99788,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 37.474s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MAT_MAT_SHARED",
            "value": 2612.324,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 65.308s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULADDSUB",
            "value": 12.335714285714285,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 21.587s"
          },
          {
            "name": "RAJA_Seq-default_Basic_NESTED_INIT",
            "value": 3.2845799999999996,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 16.423s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_ATOMIC",
            "value": 5.3192,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.330s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_REDUCE",
            "value": 5.31912,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.330s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE3_INT",
            "value": 2.9914,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.748s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE_STRUCT",
            "value": 6.75056,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.688s"
          },
          {
            "name": "RAJA_Seq-default_Basic_TRAP_INT",
            "value": 13.18872,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.297s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULTI_REDUCE",
            "value": 10.77564,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 2.694s"
          },
          {
            "name": "RAJA_Seq-new_Basic_PI_REDUCE",
            "value": 5.31936,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.330s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE3_INT",
            "value": 1.138224,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.285s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE_STRUCT",
            "value": 5.94524,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.486s"
          },
          {
            "name": "RAJA_Seq-new_Basic_TRAP_INT",
            "value": 13.50056,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.375s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "Jason Burmark",
            "username": "MrBurmark",
            "email": "MrBurmark@users.noreply.github.com"
          },
          "committer": {
            "name": "GitHub",
            "username": "web-flow",
            "email": "noreply@github.com"
          },
          "id": "1d167298cf1ca50f6717c31f54fac8a34527c3d0",
          "message": "Merge pull request #700 from llnl/jacobi-2d-launches\n\nJacobi 2D Seq, OMP, and OMPTarget launch twice",
          "timestamp": "2026-08-06T01:57:56Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/1d167298cf1ca50f6717c31f54fac8a34527c3d0"
        },
        "date": 1785985306306,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_OpenMP-default_Basic_ARRAY_OF_PTRS",
            "value": 4.97072,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.243s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_COPY8",
            "value": 4.60912,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.152s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY",
            "value": 0.15052559999999998,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.376s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY_ATOMIC",
            "value": 1.731268,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 4.328s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_EMPTY",
            "value": 0.09818059999999999,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 0.491s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_IF_QUAD",
            "value": 1.9981666666666666,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 1.798s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INDEXLIST_3LOOP",
            "value": 1.505022,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.753s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT3",
            "value": 0.708056,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 1.770s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D",
            "value": 0.20931280000000002,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 2.616s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.1575912,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 1.970s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MAT_MAT_SHARED",
            "value": 140.24200000000002,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 3.506s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULADDSUB",
            "value": 0.7392685714285715,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 1.294s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_NESTED_INIT",
            "value": 0.44760999999999995,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 2.238s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_ATOMIC",
            "value": 256.2828,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 64.071s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_REDUCE",
            "value": 0.347418,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.087s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE3_INT",
            "value": 0.2124368,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.053s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE_STRUCT",
            "value": 0.4307,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.108s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_TRAP_INT",
            "value": 0.859012,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.215s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULTI_REDUCE",
            "value": 0.689288,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.172s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_PI_REDUCE",
            "value": 0.3432644,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.086s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE3_INT",
            "value": 0.1751932,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.044s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE_STRUCT",
            "value": 0.429568,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.107s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_TRAP_INT",
            "value": 0.869728,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.217s"
          },
          {
            "name": "RAJA_Seq-default_Basic_ARRAY_OF_PTRS",
            "value": 43.8632,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 10.966s"
          },
          {
            "name": "RAJA_Seq-default_Basic_COPY8",
            "value": 32.15144,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 8.038s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY",
            "value": 4.016120000000001,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 10.040s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY_ATOMIC",
            "value": 4.0154,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 10.039s"
          },
          {
            "name": "RAJA_Seq-default_Basic_EMPTY",
            "value": 1.32151,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 6.608s"
          },
          {
            "name": "RAJA_Seq-default_Basic_IF_QUAD",
            "value": 34.36033333333333,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 30.924s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INDEXLIST_3LOOP",
            "value": 26.5542,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 13.277s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT3",
            "value": 11.841679999999998,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 29.604s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D",
            "value": 2.87336,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 35.917s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 2.99116,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 37.389s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MAT_MAT_SHARED",
            "value": 2629.256,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 65.731s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULADDSUB",
            "value": 12.684171428571428,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 22.197s"
          },
          {
            "name": "RAJA_Seq-default_Basic_NESTED_INIT",
            "value": 3.2614199999999998,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 16.307s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_ATOMIC",
            "value": 5.32876,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.332s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_REDUCE",
            "value": 5.32756,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.332s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE3_INT",
            "value": 2.816492,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.704s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE_STRUCT",
            "value": 6.62664,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.657s"
          },
          {
            "name": "RAJA_Seq-default_Basic_TRAP_INT",
            "value": 13.42772,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.357s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULTI_REDUCE",
            "value": 10.84984,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 2.712s"
          },
          {
            "name": "RAJA_Seq-new_Basic_PI_REDUCE",
            "value": 5.32676,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.332s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE3_INT",
            "value": 1.120148,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.280s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE_STRUCT",
            "value": 6.00452,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.501s"
          },
          {
            "name": "RAJA_Seq-new_Basic_TRAP_INT",
            "value": 13.53512,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.384s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "Jason Burmark",
            "username": "MrBurmark",
            "email": "MrBurmark@users.noreply.github.com"
          },
          "committer": {
            "name": "GitHub",
            "username": "web-flow",
            "email": "noreply@github.com"
          },
          "id": "338cfeb7266607648bd06815ba103f61e357c2e5",
          "message": "Merge pull request #695 from llnl/multi-kernel-regions\n\nCaliper: Annotate kernels separately",
          "timestamp": "2026-08-10T20:40:11Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/338cfeb7266607648bd06815ba103f61e357c2e5"
        },
        "date": 1786408347987,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_OpenMP-default_Basic_ARRAY_OF_PTRS",
            "value": 4.97808,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.245s"
          },
          {
            "name": "RAJA_OpenMP-default_ARRAY_OF_PTRS_1",
            "value": 0.01989952,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 1.244s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_COPY8",
            "value": 4.7322,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.183s"
          },
          {
            "name": "RAJA_OpenMP-default_COPY8_1",
            "value": 0.01891584,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 1.182s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY",
            "value": 0.1551652,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.388s"
          },
          {
            "name": "RAJA_OpenMP-default_DAXPY_1",
            "value": 0.00006174352,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 2500, total_time: 0.386s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY_ATOMIC",
            "value": 1.709628,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 4.274s"
          },
          {
            "name": "RAJA_OpenMP-default_DAXPY_ATOMIC_1",
            "value": 0.000683488,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 2500, total_time: 4.272s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_EMPTY",
            "value": 0.0898364,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 0.449s"
          },
          {
            "name": "RAJA_OpenMP-default_EMPTY_1",
            "value": 0.00001788196,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 5000, total_time: 0.447s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_IF_QUAD",
            "value": 2.0050777777777777,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 1.805s"
          },
          {
            "name": "RAJA_OpenMP-default_IF_QUAD_1",
            "value": 0.002225777777777778,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 900, total_time: 1.803s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INDEXLIST_3LOOP",
            "value": 1.582866,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.791s"
          },
          {
            "name": "RAJA_OpenMP-default_INDEXLIST_3LOOP_1",
            "value": 0.00033434999999999997,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 500, total_time: 0.084s"
          },
          {
            "name": "RAJA_OpenMP-default_INDEXLIST_3LOOP_2",
            "value": 0.0024596279999999997,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 500, total_time: 0.615s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT3",
            "value": 0.71842,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 1.796s"
          },
          {
            "name": "RAJA_OpenMP-default_INIT3_1",
            "value": 0.0002866544,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 2500, total_time: 1.792s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D",
            "value": 0.2129392,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 2.662s"
          },
          {
            "name": "RAJA_OpenMP-default_INIT_VIEW1D_1",
            "value": 0.000016979136,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 12500, total_time: 2.653s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.15964560000000003,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 1.996s"
          },
          {
            "name": "RAJA_OpenMP-default_INIT_VIEW1D_OFFSET_1",
            "value": 0.000012718208,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 12500, total_time: 1.987s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MAT_MAT_SHARED",
            "value": 140.9116,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 3.523s"
          },
          {
            "name": "RAJA_OpenMP-default_MAT_MAT_SHARED_1",
            "value": 5.636368,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 25, total_time: 3.523s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULADDSUB",
            "value": 0.7440514285714285,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 1.302s"
          },
          {
            "name": "RAJA_OpenMP-default_MULADDSUB_1",
            "value": 0.00042413387755102044,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1750, total_time: 1.299s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_NESTED_INIT",
            "value": 0.45662600000000003,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 2.283s"
          },
          {
            "name": "RAJA_OpenMP-default_NESTED_INIT_1",
            "value": 0.0000911292,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 5000, total_time: 2.278s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_ATOMIC",
            "value": 294.1516,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 73.538s"
          },
          {
            "name": "RAJA_OpenMP-default_PI_ATOMIC_1",
            "value": 1.1766048,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 73.538s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_REDUCE",
            "value": 0.3480008,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.087s"
          },
          {
            "name": "RAJA_OpenMP-default_PI_REDUCE_1",
            "value": 0.0013902416000000002,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.087s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE3_INT",
            "value": 0.2115624,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.053s"
          },
          {
            "name": "RAJA_OpenMP-default_REDUCE3_INT_1",
            "value": 0.0008444416,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.053s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE_STRUCT",
            "value": 0.492244,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.123s"
          },
          {
            "name": "RAJA_OpenMP-default_REDUCE_STRUCT_1",
            "value": 0.00196568,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.123s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_TRAP_INT",
            "value": 0.858412,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.215s"
          },
          {
            "name": "RAJA_OpenMP-default_TRAP_INT_1",
            "value": 0.00343192,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.214s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULTI_REDUCE",
            "value": 0.690776,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.173s"
          },
          {
            "name": "RAJA_OpenMP-default_MULTI_REDUCE_1",
            "value": 0.002759728,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.172s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_PI_REDUCE",
            "value": 0.3425752000000001,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.086s"
          },
          {
            "name": "RAJA_OpenMP-new_PI_REDUCE_1",
            "value": 0.0013686144,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.086s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE3_INT",
            "value": 0.1598468,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.040s"
          },
          {
            "name": "RAJA_OpenMP-new_REDUCE3_INT_1",
            "value": 0.0006375568000000001,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.040s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE_STRUCT",
            "value": 0.430184,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.108s"
          },
          {
            "name": "RAJA_OpenMP-new_REDUCE_STRUCT_1",
            "value": 0.001717312,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.107s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_TRAP_INT",
            "value": 0.857044,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.214s"
          },
          {
            "name": "RAJA_OpenMP-new_TRAP_INT_1",
            "value": 0.0034264480000000003,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.214s"
          },
          {
            "name": "RAJA_Seq-default_Basic_ARRAY_OF_PTRS",
            "value": 37.153,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 9.288s"
          },
          {
            "name": "RAJA_Seq-default_ARRAY_OF_PTRS_1",
            "value": 0.14860208,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 9.288s"
          },
          {
            "name": "RAJA_Seq-default_Basic_COPY8",
            "value": 31.3286,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 7.832s"
          },
          {
            "name": "RAJA_Seq-default_COPY8_1",
            "value": 0.125304,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 7.832s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY",
            "value": 3.959808,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 9.900s"
          },
          {
            "name": "RAJA_Seq-default_DAXPY_1",
            "value": 0.001582904,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 2500, total_time: 9.893s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY_ATOMIC",
            "value": 3.9587440000000003,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 9.897s"
          },
          {
            "name": "RAJA_Seq-default_DAXPY_ATOMIC_1",
            "value": 0.0015825696,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 2500, total_time: 9.891s"
          },
          {
            "name": "RAJA_Seq-default_Basic_EMPTY",
            "value": 1.332528,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 6.663s"
          },
          {
            "name": "RAJA_Seq-default_EMPTY_1",
            "value": 0.0002664352,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 5000, total_time: 6.661s"
          },
          {
            "name": "RAJA_Seq-default_Basic_IF_QUAD",
            "value": 33.83688888888889,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 30.453s"
          },
          {
            "name": "RAJA_Seq-default_IF_QUAD_1",
            "value": 0.03759358024691358,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 900, total_time: 30.451s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INDEXLIST_3LOOP",
            "value": 26.3448,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 13.172s"
          },
          {
            "name": "RAJA_Seq-default_INDEXLIST_3LOOP_1",
            "value": 0.00973236,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 500, total_time: 2.433s"
          },
          {
            "name": "RAJA_Seq-default_INDEXLIST_3LOOP_2",
            "value": 0.03664703999999999,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 500, total_time: 9.162s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT3",
            "value": 11.594,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 28.985s"
          },
          {
            "name": "RAJA_Seq-default_INIT3_1",
            "value": 0.004636752,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 2500, total_time: 28.980s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D",
            "value": 2.8884960000000004,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 36.106s"
          },
          {
            "name": "RAJA_Seq-default_INIT_VIEW1D_1",
            "value": 0.00023092415999999996,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 12500, total_time: 36.082s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 2.991824,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 37.398s"
          },
          {
            "name": "RAJA_Seq-default_INIT_VIEW1D_OFFSET_1",
            "value": 0.00023921536,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 12500, total_time: 37.377s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MAT_MAT_SHARED",
            "value": 2583.068,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 64.577s"
          },
          {
            "name": "RAJA_Seq-default_MAT_MAT_SHARED_1",
            "value": 103.32272,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 25, total_time: 64.577s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULADDSUB",
            "value": 12.370800000000001,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 21.649s"
          },
          {
            "name": "RAJA_Seq-default_MULADDSUB_1",
            "value": 0.0070675918367346935,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1750, total_time: 21.645s"
          },
          {
            "name": "RAJA_Seq-default_Basic_NESTED_INIT",
            "value": 3.25644,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 16.282s"
          },
          {
            "name": "RAJA_Seq-default_NESTED_INIT_1",
            "value": 0.000650892,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 5000, total_time: 16.272s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_ATOMIC",
            "value": 5.3184,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.330s"
          },
          {
            "name": "RAJA_Seq-default_PI_ATOMIC_1",
            "value": 0.02127216,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 1.330s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_REDUCE",
            "value": 5.31844,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.330s"
          },
          {
            "name": "RAJA_Seq-default_PI_REDUCE_1",
            "value": 0.02127232,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 1.330s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE3_INT",
            "value": 2.990672,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.748s"
          },
          {
            "name": "RAJA_Seq-default_REDUCE3_INT_1",
            "value": 0.011959552,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.747s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE_STRUCT",
            "value": 6.57164,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.643s"
          },
          {
            "name": "RAJA_Seq-default_REDUCE_STRUCT_1",
            "value": 0.026277280000000004,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 1.642s"
          },
          {
            "name": "RAJA_Seq-default_Basic_TRAP_INT",
            "value": 13.3064,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.327s"
          },
          {
            "name": "RAJA_Seq-default_TRAP_INT_1",
            "value": 0.053224,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 3.326s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULTI_REDUCE",
            "value": 10.78244,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 2.696s"
          },
          {
            "name": "RAJA_Seq-default_MULTI_REDUCE_1",
            "value": 0.04311968,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 2.695s"
          },
          {
            "name": "RAJA_Seq-new_Basic_PI_REDUCE",
            "value": 5.31836,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.330s"
          },
          {
            "name": "RAJA_Seq-new_PI_REDUCE_1",
            "value": 0.021272,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 1.329s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE3_INT",
            "value": 1.132672,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.283s"
          },
          {
            "name": "RAJA_Seq-new_REDUCE3_INT_1",
            "value": 0.004527408,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.283s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE_STRUCT",
            "value": 5.88904,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.472s"
          },
          {
            "name": "RAJA_Seq-new_REDUCE_STRUCT_1",
            "value": 0.02354816,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 1.472s"
          },
          {
            "name": "RAJA_Seq-new_Basic_TRAP_INT",
            "value": 13.51216,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.378s"
          },
          {
            "name": "RAJA_Seq-new_TRAP_INT_1",
            "value": 0.0540472,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 3.378s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "Jason Burmark",
            "username": "MrBurmark",
            "email": "MrBurmark@users.noreply.github.com"
          },
          "committer": {
            "name": "GitHub",
            "username": "web-flow",
            "email": "noreply@github.com"
          },
          "id": "5301486b1ea58280079a4fe252710db5e01e758a",
          "message": "Merge pull request #687 from llnl/pr-from-fork/686\n\nPr from fork/686",
          "timestamp": "2026-08-20T19:54:32Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/5301486b1ea58280079a4fe252710db5e01e758a"
        },
        "date": 1787293823309,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_OpenMP-default_Basic_ARRAY_OF_PTRS",
            "value": 6.358,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.589s"
          },
          {
            "name": "RAJA_OpenMP-default_ARRAY_OF_PTRS_1",
            "value": 0.02542016,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 1.589s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_COPY8",
            "value": 4.66296,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.166s"
          },
          {
            "name": "RAJA_OpenMP-default_COPY8_1",
            "value": 0.018639199999999998,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 1.165s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY",
            "value": 0.1623548,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.406s"
          },
          {
            "name": "RAJA_OpenMP-default_DAXPY_1",
            "value": 0.00006455248000000001,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 2500, total_time: 0.403s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY_ATOMIC",
            "value": 1.6910920000000003,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 4.228s"
          },
          {
            "name": "RAJA_OpenMP-default_DAXPY_ATOMIC_1",
            "value": 0.0006760912000000002,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 2500, total_time: 4.226s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_EMPTY",
            "value": 0.0887836,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 0.444s"
          },
          {
            "name": "RAJA_OpenMP-default_EMPTY_1",
            "value": 0.00001767224,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 5000, total_time: 0.442s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_IF_QUAD",
            "value": 2.0085888888888888,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 1.808s"
          },
          {
            "name": "RAJA_OpenMP-default_IF_QUAD_1",
            "value": 0.0022292098765432098,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 900, total_time: 1.806s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INDEXLIST_3LOOP",
            "value": 1.568662,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.784s"
          },
          {
            "name": "RAJA_OpenMP-default_INDEXLIST_3LOOP_1",
            "value": 0.0003463864,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 500, total_time: 0.087s"
          },
          {
            "name": "RAJA_OpenMP-default_INDEXLIST_3LOOP_2",
            "value": 0.0024153679999999998,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 500, total_time: 0.604s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT3",
            "value": 0.7275520000000001,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 1.819s"
          },
          {
            "name": "RAJA_OpenMP-default_INIT3_1",
            "value": 0.00029021280000000003,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 2500, total_time: 1.814s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D",
            "value": 0.2120312,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 2.650s"
          },
          {
            "name": "RAJA_OpenMP-default_INIT_VIEW1D_1",
            "value": 0.000016901312,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 12500, total_time: 2.641s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.15997199999999998,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 2.000s"
          },
          {
            "name": "RAJA_OpenMP-default_INIT_VIEW1D_OFFSET_1",
            "value": 0.000012741184,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 12500, total_time: 1.991s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MAT_MAT_SHARED",
            "value": 141.7916,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 3.545s"
          },
          {
            "name": "RAJA_OpenMP-default_MAT_MAT_SHARED_1",
            "value": 5.671568,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 25, total_time: 3.545s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULADDSUB",
            "value": 0.7565257142857144,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 1.324s"
          },
          {
            "name": "RAJA_OpenMP-default_MULADDSUB_1",
            "value": 0.00043108244897959184,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1750, total_time: 1.320s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_NESTED_INIT",
            "value": 0.45258999999999994,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 2.263s"
          },
          {
            "name": "RAJA_OpenMP-default_NESTED_INIT_1",
            "value": 0.0000903656,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 5000, total_time: 2.259s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_ATOMIC",
            "value": 291.18,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 72.795s"
          },
          {
            "name": "RAJA_OpenMP-default_PI_ATOMIC_1",
            "value": 1.1647184,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 72.795s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_REDUCE",
            "value": 0.3487444,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.087s"
          },
          {
            "name": "RAJA_OpenMP-default_PI_REDUCE_1",
            "value": 0.001393224,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.087s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE3_INT",
            "value": 0.2195132,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.055s"
          },
          {
            "name": "RAJA_OpenMP-default_REDUCE3_INT_1",
            "value": 0.0008762096000000001,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.055s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE_STRUCT",
            "value": 0.432808,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.108s"
          },
          {
            "name": "RAJA_OpenMP-default_REDUCE_STRUCT_1",
            "value": 0.001726976,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.108s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_TRAP_INT",
            "value": 0.85844,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.215s"
          },
          {
            "name": "RAJA_OpenMP-default_TRAP_INT_1",
            "value": 0.0034320320000000002,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.215s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULTI_REDUCE",
            "value": 0.69176,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.173s"
          },
          {
            "name": "RAJA_OpenMP-default_MULTI_REDUCE_1",
            "value": 0.0027633920000000004,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.173s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_PI_REDUCE",
            "value": 0.3444188,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.086s"
          },
          {
            "name": "RAJA_OpenMP-new_PI_REDUCE_1",
            "value": 0.0013759808,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.086s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE3_INT",
            "value": 0.1764592,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.044s"
          },
          {
            "name": "RAJA_OpenMP-new_REDUCE3_INT_1",
            "value": 0.0007039424000000001,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.044s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE_STRUCT",
            "value": 0.434132,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.109s"
          },
          {
            "name": "RAJA_OpenMP-new_REDUCE_STRUCT_1",
            "value": 0.0017328,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.108s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_TRAP_INT",
            "value": 0.86788,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.217s"
          },
          {
            "name": "RAJA_OpenMP-new_TRAP_INT_1",
            "value": 0.003469776,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.217s"
          },
          {
            "name": "RAJA_Seq-default_Basic_ARRAY_OF_PTRS",
            "value": 37.13756,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 9.284s"
          },
          {
            "name": "RAJA_Seq-default_ARRAY_OF_PTRS_1",
            "value": 0.14854144,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 9.284s"
          },
          {
            "name": "RAJA_Seq-default_Basic_COPY8",
            "value": 31.33072,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 7.833s"
          },
          {
            "name": "RAJA_Seq-default_COPY8_1",
            "value": 0.1253128,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 7.832s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY",
            "value": 3.9477,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 9.869s"
          },
          {
            "name": "RAJA_Seq-default_DAXPY_1",
            "value": 0.0015780992000000002,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 2500, total_time: 9.863s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY_ATOMIC",
            "value": 3.9395839999999995,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 9.849s"
          },
          {
            "name": "RAJA_Seq-default_DAXPY_ATOMIC_1",
            "value": 0.0015750064000000002,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 2500, total_time: 9.844s"
          },
          {
            "name": "RAJA_Seq-default_Basic_EMPTY",
            "value": 1.3194599999999999,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 6.597s"
          },
          {
            "name": "RAJA_Seq-default_EMPTY_1",
            "value": 0.0002638216,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 5000, total_time: 6.596s"
          },
          {
            "name": "RAJA_Seq-default_Basic_IF_QUAD",
            "value": 33.76988888888889,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 30.393s"
          },
          {
            "name": "RAJA_Seq-default_IF_QUAD_1",
            "value": 0.037519753086419756,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 900, total_time: 30.391s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INDEXLIST_3LOOP",
            "value": 26.406,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 13.203s"
          },
          {
            "name": "RAJA_Seq-default_INDEXLIST_3LOOP_1",
            "value": 0.00972524,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 500, total_time: 2.431s"
          },
          {
            "name": "RAJA_Seq-default_INDEXLIST_3LOOP_2",
            "value": 0.0368176,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 500, total_time: 9.204s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT3",
            "value": 11.5786,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 28.947s"
          },
          {
            "name": "RAJA_Seq-default_INIT3_1",
            "value": 0.00463064,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 2500, total_time: 28.942s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D",
            "value": 2.867792,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 35.847s"
          },
          {
            "name": "RAJA_Seq-default_INIT_VIEW1D_1",
            "value": 0.00022928767999999997,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 12500, total_time: 35.826s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 2.99696,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 37.462s"
          },
          {
            "name": "RAJA_Seq-default_INIT_VIEW1D_OFFSET_1",
            "value": 0.00023962944000000005,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 12500, total_time: 37.442s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MAT_MAT_SHARED",
            "value": 2602.952,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 65.074s"
          },
          {
            "name": "RAJA_Seq-default_MAT_MAT_SHARED_1",
            "value": 104.11792000000001,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 25, total_time: 65.074s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULADDSUB",
            "value": 12.5244,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 21.918s"
          },
          {
            "name": "RAJA_Seq-default_MULADDSUB_1",
            "value": 0.007155428571428571,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1750, total_time: 21.913s"
          },
          {
            "name": "RAJA_Seq-default_Basic_NESTED_INIT",
            "value": 3.24862,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 16.243s"
          },
          {
            "name": "RAJA_Seq-default_NESTED_INIT_1",
            "value": 0.0006494240000000001,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 5000, total_time: 16.236s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_ATOMIC",
            "value": 5.31832,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.330s"
          },
          {
            "name": "RAJA_Seq-default_PI_ATOMIC_1",
            "value": 0.02127184,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 1.329s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_REDUCE",
            "value": 5.31824,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.330s"
          },
          {
            "name": "RAJA_Seq-default_PI_REDUCE_1",
            "value": 0.021271520000000002,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 1.329s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE3_INT",
            "value": 2.775032,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.694s"
          },
          {
            "name": "RAJA_Seq-default_REDUCE3_INT_1",
            "value": 0.01109656,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.694s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE_STRUCT",
            "value": 6.5638,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.641s"
          },
          {
            "name": "RAJA_Seq-default_REDUCE_STRUCT_1",
            "value": 0.02624816,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 1.641s"
          },
          {
            "name": "RAJA_Seq-default_Basic_TRAP_INT",
            "value": 13.55064,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.388s"
          },
          {
            "name": "RAJA_Seq-default_TRAP_INT_1",
            "value": 0.054201120000000005,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 3.388s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULTI_REDUCE",
            "value": 10.78108,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 2.695s"
          },
          {
            "name": "RAJA_Seq-default_MULTI_REDUCE_1",
            "value": 0.04311632,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 2.695s"
          },
          {
            "name": "RAJA_Seq-new_Basic_PI_REDUCE",
            "value": 5.31828,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.330s"
          },
          {
            "name": "RAJA_Seq-new_PI_REDUCE_1",
            "value": 0.02127168,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 1.329s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE3_INT",
            "value": 1.088172,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.272s"
          },
          {
            "name": "RAJA_Seq-new_REDUCE3_INT_1",
            "value": 0.004349712,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.272s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE_STRUCT",
            "value": 5.89952,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.475s"
          },
          {
            "name": "RAJA_Seq-new_REDUCE_STRUCT_1",
            "value": 0.023590720000000003,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 1.474s"
          },
          {
            "name": "RAJA_Seq-new_Basic_TRAP_INT",
            "value": 13.40724,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.352s"
          },
          {
            "name": "RAJA_Seq-new_TRAP_INT_1",
            "value": 0.05362752,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 3.352s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "Rich Hornung",
            "username": "rhornung67",
            "email": "hornung1@llnl.gov"
          },
          "committer": {
            "name": "GitHub",
            "username": "web-flow",
            "email": "noreply@github.com"
          },
          "id": "4f9e9a85ff9e5e0ce75c230eed7244b9f305cfb0",
          "message": "Merge pull request #688 from llnl/woptim/rsc-2026-6-0\n\nFrom RAJA: Point at RSC release candidate",
          "timestamp": "2026-08-21T21:34:01Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/4f9e9a85ff9e5e0ce75c230eed7244b9f305cfb0"
        },
        "date": 1787352179415,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_OpenMP-default_Basic_ARRAY_OF_PTRS",
            "value": 5.3314,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.333s"
          },
          {
            "name": "RAJA_OpenMP-default_ARRAY_OF_PTRS_1",
            "value": 0.021310720000000002,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 1.332s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_COPY8",
            "value": 3.592864,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.898s"
          },
          {
            "name": "RAJA_OpenMP-default_COPY8_1",
            "value": 0.014357680000000001,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.897s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY",
            "value": 0.1529724,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.382s"
          },
          {
            "name": "RAJA_OpenMP-default_DAXPY_1",
            "value": 0.00006080864,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 2500, total_time: 0.380s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY_ATOMIC",
            "value": 1.6902439999999999,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 4.226s"
          },
          {
            "name": "RAJA_OpenMP-default_DAXPY_ATOMIC_1",
            "value": 0.0006757072,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 2500, total_time: 4.223s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_EMPTY",
            "value": 0.0880382,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 0.440s"
          },
          {
            "name": "RAJA_OpenMP-default_EMPTY_1",
            "value": 0.00001751596,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 5000, total_time: 0.438s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_IF_QUAD",
            "value": 1.9859444444444443,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 1.787s"
          },
          {
            "name": "RAJA_OpenMP-default_IF_QUAD_1",
            "value": 0.0022042469135802466,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 900, total_time: 1.785s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INDEXLIST_3LOOP",
            "value": 1.548274,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.774s"
          },
          {
            "name": "RAJA_OpenMP-default_INDEXLIST_3LOOP_1",
            "value": 0.0003328716,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 500, total_time: 0.083s"
          },
          {
            "name": "RAJA_OpenMP-default_INDEXLIST_3LOOP_2",
            "value": 0.0024075,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 500, total_time: 0.602s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT3",
            "value": 0.71432,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 1.786s"
          },
          {
            "name": "RAJA_OpenMP-default_INIT3_1",
            "value": 0.0002848864,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 2500, total_time: 1.781s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D",
            "value": 0.21099039999999997,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 2.637s"
          },
          {
            "name": "RAJA_OpenMP-default_INIT_VIEW1D_1",
            "value": 0.00001681472,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 12500, total_time: 2.627s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.1588608,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 1.986s"
          },
          {
            "name": "RAJA_OpenMP-default_INIT_VIEW1D_OFFSET_1",
            "value": 0.000012633024,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 12500, total_time: 1.974s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MAT_MAT_SHARED",
            "value": 140.74280000000002,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 3.519s"
          },
          {
            "name": "RAJA_OpenMP-default_MAT_MAT_SHARED_1",
            "value": 5.629616,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 25, total_time: 3.519s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULADDSUB",
            "value": 0.7493599999999999,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 1.311s"
          },
          {
            "name": "RAJA_OpenMP-default_MULADDSUB_1",
            "value": 0.00042717061224489795,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1750, total_time: 1.308s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_NESTED_INIT",
            "value": 0.423404,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 2.117s"
          },
          {
            "name": "RAJA_OpenMP-default_NESTED_INIT_1",
            "value": 0.0000844988,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 5000, total_time: 2.112s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_ATOMIC",
            "value": 303.6812,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 75.920s"
          },
          {
            "name": "RAJA_OpenMP-default_PI_ATOMIC_1",
            "value": 1.2147216,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 75.920s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_REDUCE",
            "value": 0.3483992,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.087s"
          },
          {
            "name": "RAJA_OpenMP-default_PI_REDUCE_1",
            "value": 0.0013917584000000002,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.087s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE3_INT",
            "value": 0.211632,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.053s"
          },
          {
            "name": "RAJA_OpenMP-default_REDUCE3_INT_1",
            "value": 0.0008446288,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.053s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE_STRUCT",
            "value": 0.43297599999999997,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.108s"
          },
          {
            "name": "RAJA_OpenMP-default_REDUCE_STRUCT_1",
            "value": 0.001727792,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.108s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_TRAP_INT",
            "value": 0.85856,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.215s"
          },
          {
            "name": "RAJA_OpenMP-default_TRAP_INT_1",
            "value": 0.0034324159999999998,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.215s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULTI_REDUCE",
            "value": 0.692276,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.173s"
          },
          {
            "name": "RAJA_OpenMP-default_MULTI_REDUCE_1",
            "value": 0.00276512,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.173s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_PI_REDUCE",
            "value": 0.3427276,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.086s"
          },
          {
            "name": "RAJA_OpenMP-new_PI_REDUCE_1",
            "value": 0.0013690704000000001,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.086s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE3_INT",
            "value": 0.1358952,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.034s"
          },
          {
            "name": "RAJA_OpenMP-new_REDUCE3_INT_1",
            "value": 0.0005417264,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.034s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE_STRUCT",
            "value": 0.420836,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.105s"
          },
          {
            "name": "RAJA_OpenMP-new_REDUCE_STRUCT_1",
            "value": 0.00167968,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.105s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_TRAP_INT",
            "value": 0.864336,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.216s"
          },
          {
            "name": "RAJA_OpenMP-new_TRAP_INT_1",
            "value": 0.003455472,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.216s"
          },
          {
            "name": "RAJA_Seq-default_Basic_ARRAY_OF_PTRS",
            "value": 37.23408,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 9.309s"
          },
          {
            "name": "RAJA_Seq-default_ARRAY_OF_PTRS_1",
            "value": 0.1489272,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 9.308s"
          },
          {
            "name": "RAJA_Seq-default_Basic_COPY8",
            "value": 31.2172,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 7.804s"
          },
          {
            "name": "RAJA_Seq-default_COPY8_1",
            "value": 0.12485776,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 7.804s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY",
            "value": 3.955344,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 9.888s"
          },
          {
            "name": "RAJA_Seq-default_DAXPY_1",
            "value": 0.00158116,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 2500, total_time: 9.882s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY_ATOMIC",
            "value": 3.950632,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 9.877s"
          },
          {
            "name": "RAJA_Seq-default_DAXPY_ATOMIC_1",
            "value": 0.0015793488,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 2500, total_time: 9.871s"
          },
          {
            "name": "RAJA_Seq-default_Basic_EMPTY",
            "value": 1.319966,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 6.600s"
          },
          {
            "name": "RAJA_Seq-default_EMPTY_1",
            "value": 0.00026392040000000004,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 5000, total_time: 6.598s"
          },
          {
            "name": "RAJA_Seq-default_Basic_IF_QUAD",
            "value": 33.77688888888889,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 30.399s"
          },
          {
            "name": "RAJA_Seq-default_IF_QUAD_1",
            "value": 0.037527407407407405,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 900, total_time: 30.397s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INDEXLIST_3LOOP",
            "value": 26.4676,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 13.234s"
          },
          {
            "name": "RAJA_Seq-default_INDEXLIST_3LOOP_1",
            "value": 0.00977852,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 500, total_time: 2.445s"
          },
          {
            "name": "RAJA_Seq-default_INDEXLIST_3LOOP_2",
            "value": 0.03685968,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 500, total_time: 9.215s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT3",
            "value": 11.59088,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 28.977s"
          },
          {
            "name": "RAJA_Seq-default_INIT3_1",
            "value": 0.004635504,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 2500, total_time: 28.972s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D",
            "value": 2.8777839999999997,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 35.972s"
          },
          {
            "name": "RAJA_Seq-default_INIT_VIEW1D_1",
            "value": 0.00023007615999999997,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 12500, total_time: 35.949s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 2.996592,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 37.457s"
          },
          {
            "name": "RAJA_Seq-default_INIT_VIEW1D_OFFSET_1",
            "value": 0.00023959424,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 12500, total_time: 37.437s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MAT_MAT_SHARED",
            "value": 2600.324,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 65.008s"
          },
          {
            "name": "RAJA_Seq-default_MAT_MAT_SHARED_1",
            "value": 104.01279999999998,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 25, total_time: 65.008s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULADDSUB",
            "value": 12.481428571428571,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 21.843s"
          },
          {
            "name": "RAJA_Seq-default_MULADDSUB_1",
            "value": 0.007130873469387755,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1750, total_time: 21.838s"
          },
          {
            "name": "RAJA_Seq-default_Basic_NESTED_INIT",
            "value": 3.25072,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 16.254s"
          },
          {
            "name": "RAJA_Seq-default_NESTED_INIT_1",
            "value": 0.0006498000000000001,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 5000, total_time: 16.245s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_ATOMIC",
            "value": 5.31824,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.330s"
          },
          {
            "name": "RAJA_Seq-default_PI_ATOMIC_1",
            "value": 0.021271520000000002,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 1.329s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_REDUCE",
            "value": 5.31828,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.330s"
          },
          {
            "name": "RAJA_Seq-default_PI_REDUCE_1",
            "value": 0.021271520000000002,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 1.329s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE3_INT",
            "value": 2.8884599999999994,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.722s"
          },
          {
            "name": "RAJA_Seq-default_REDUCE3_INT_1",
            "value": 0.01155008,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.722s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE_STRUCT",
            "value": 6.56308,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.641s"
          },
          {
            "name": "RAJA_Seq-default_REDUCE_STRUCT_1",
            "value": 0.02624448,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 1.640s"
          },
          {
            "name": "RAJA_Seq-default_Basic_TRAP_INT",
            "value": 13.677,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.419s"
          },
          {
            "name": "RAJA_Seq-default_TRAP_INT_1",
            "value": 0.0547064,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 3.419s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULTI_REDUCE",
            "value": 10.78728,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 2.697s"
          },
          {
            "name": "RAJA_Seq-default_MULTI_REDUCE_1",
            "value": 0.04314079999999999,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 2.696s"
          },
          {
            "name": "RAJA_Seq-new_Basic_PI_REDUCE",
            "value": 5.321,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.330s"
          },
          {
            "name": "RAJA_Seq-new_PI_REDUCE_1",
            "value": 0.021282399999999996,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 1.330s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE3_INT",
            "value": 1.127876,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.282s"
          },
          {
            "name": "RAJA_Seq-new_REDUCE3_INT_1",
            "value": 0.004507152,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.282s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE_STRUCT",
            "value": 5.90096,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.475s"
          },
          {
            "name": "RAJA_Seq-new_REDUCE_STRUCT_1",
            "value": 0.02359728,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 1.475s"
          },
          {
            "name": "RAJA_Seq-new_Basic_TRAP_INT",
            "value": 13.45368,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.363s"
          },
          {
            "name": "RAJA_Seq-new_TRAP_INT_1",
            "value": 0.05381311999999999,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 3.363s"
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
      },
      {
        "commit": {
          "author": {
            "name": "Rich Hornung",
            "username": "rhornung67",
            "email": "hornung1@llnl.gov"
          },
          "committer": {
            "name": "GitHub",
            "username": "web-flow",
            "email": "noreply@github.com"
          },
          "id": "2e4038b7fee213ead90d2108b8458b77c6ac0006",
          "message": "Merge pull request #678 from llnl/main-mergeback\n\nMain mergeback to develop after v2025.12.1 release",
          "timestamp": "2026-03-30T22:11:27Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/2e4038b7fee213ead90d2108b8458b77c6ac0006"
        },
        "date": 1774915408205,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_OpenMP-default_Basic_ARRAY_OF_PTRS",
            "value": 4.36636,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.092s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_COPY8",
            "value": 3.63434,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.909s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY",
            "value": 0.1550608,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.388s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY_ATOMIC",
            "value": 1.6065000000000003,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 4.016s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_EMPTY",
            "value": 0.0480602,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 0.240s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_IF_QUAD",
            "value": 1.9486333333333332,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 1.754s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INDEXLIST_3LOOP",
            "value": 1.661642,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.831s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT3",
            "value": 0.747368,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 1.868s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D",
            "value": 0.1042352,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 1.303s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.1033752,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 1.292s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MAT_MAT_SHARED",
            "value": 197.0564,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 4.926s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULADDSUB",
            "value": 0.7499028571428571,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 1.312s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_NESTED_INIT",
            "value": 0.1882018,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 0.941s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_ATOMIC",
            "value": 292.192,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 73.048s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_REDUCE",
            "value": 0.3559372,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.089s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE3_INT",
            "value": 0.1184372,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.030s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE_STRUCT",
            "value": 0.385546,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.096s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_TRAP_INT",
            "value": 0.861356,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.215s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULTI_REDUCE",
            "value": 0.69018,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.173s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_PI_REDUCE",
            "value": 0.3495372,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.087s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE3_INT",
            "value": 0.1019732,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.025s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE_STRUCT",
            "value": 0.3494316,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.087s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_TRAP_INT",
            "value": 0.855676,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.214s"
          },
          {
            "name": "RAJA_Seq-default_Basic_ARRAY_OF_PTRS",
            "value": 75.48,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 18.870s"
          },
          {
            "name": "RAJA_Seq-default_Basic_COPY8",
            "value": 39.59004,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 9.898s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY",
            "value": 4.133,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 10.332s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY_ATOMIC",
            "value": 4.1302,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 10.325s"
          },
          {
            "name": "RAJA_Seq-default_Basic_EMPTY",
            "value": 1.3227980000000001,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 6.614s"
          },
          {
            "name": "RAJA_Seq-default_Basic_IF_QUAD",
            "value": 32.51422222222222,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 29.263s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INDEXLIST_3LOOP",
            "value": 29.3134,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 14.657s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT3",
            "value": 11.13628,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 27.841s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D",
            "value": 3.017992,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 37.725s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 3.09356,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 38.669s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MAT_MAT_SHARED",
            "value": 3068.116,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 76.703s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULADDSUB",
            "value": 11.497085714285715,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 20.120s"
          },
          {
            "name": "RAJA_Seq-default_Basic_NESTED_INIT",
            "value": 3.6378799999999996,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 18.189s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_ATOMIC",
            "value": 5.292799999999999,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.323s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_REDUCE",
            "value": 10.59128,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 2.648s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE3_INT",
            "value": 2.847688,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.712s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE_STRUCT",
            "value": 7.0196,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.755s"
          },
          {
            "name": "RAJA_Seq-default_Basic_TRAP_INT",
            "value": 13.50984,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.377s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULTI_REDUCE",
            "value": 10.90648,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 2.727s"
          },
          {
            "name": "RAJA_Seq-new_Basic_PI_REDUCE",
            "value": 5.37116,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.343s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE3_INT",
            "value": 1.455032,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.364s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE_STRUCT",
            "value": 12.31684,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.079s"
          },
          {
            "name": "RAJA_Seq-new_Basic_TRAP_INT",
            "value": 13.55156,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.388s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "Rich Hornung",
            "username": "rhornung67",
            "email": "hornung1@llnl.gov"
          },
          "committer": {
            "name": "GitHub",
            "username": "web-flow",
            "email": "noreply@github.com"
          },
          "id": "3111bb9edb2a22dcc64c7589d94a080912db06be",
          "message": "Merge pull request #679 from llnl/task/rhornung67/fix-docs\n\nDoc fixes",
          "timestamp": "2026-04-01T15:59:31Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/3111bb9edb2a22dcc64c7589d94a080912db06be"
        },
        "date": 1775069470199,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_OpenMP-default_Basic_ARRAY_OF_PTRS",
            "value": 4.339840000000001,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.085s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_COPY8",
            "value": 3.642152,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.911s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY",
            "value": 0.1626548,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.407s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY_ATOMIC",
            "value": 1.605328,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 4.013s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_EMPTY",
            "value": 0.0479088,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 0.240s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_IF_QUAD",
            "value": 1.9493222222222222,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 1.754s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INDEXLIST_3LOOP",
            "value": 1.64528,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.823s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT3",
            "value": 0.746484,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 1.866s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D",
            "value": 0.1039888,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 1.300s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.10370480000000001,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 1.296s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MAT_MAT_SHARED",
            "value": 199.4028,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 4.985s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULADDSUB",
            "value": 0.7633428571428571,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 1.336s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_NESTED_INIT",
            "value": 0.18832159999999998,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 0.942s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_ATOMIC",
            "value": 286.596,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 71.649s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_REDUCE",
            "value": 0.3562832,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.089s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE3_INT",
            "value": 0.1142908,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.029s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE_STRUCT",
            "value": 0.386342,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.097s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_TRAP_INT",
            "value": 0.861708,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.215s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULTI_REDUCE",
            "value": 0.68956,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.172s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_PI_REDUCE",
            "value": 0.35038040000000004,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.088s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE3_INT",
            "value": 0.101106,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.025s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE_STRUCT",
            "value": 0.3508708,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.088s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_TRAP_INT",
            "value": 0.852732,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.213s"
          },
          {
            "name": "RAJA_Seq-default_Basic_ARRAY_OF_PTRS",
            "value": 80.6952,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 20.174s"
          },
          {
            "name": "RAJA_Seq-default_Basic_COPY8",
            "value": 39.55648,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 9.889s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY",
            "value": 4.256239999999999,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 10.641s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY_ATOMIC",
            "value": 4.25076,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 10.627s"
          },
          {
            "name": "RAJA_Seq-default_Basic_EMPTY",
            "value": 1.3232979999999999,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 6.616s"
          },
          {
            "name": "RAJA_Seq-default_Basic_IF_QUAD",
            "value": 32.50222222222222,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 29.252s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INDEXLIST_3LOOP",
            "value": 29.4418,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 14.721s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT3",
            "value": 11.01792,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 27.545s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D",
            "value": 2.855,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 35.688s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 2.9158720000000002,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 36.448s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MAT_MAT_SHARED",
            "value": 3040.396,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 76.010s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULADDSUB",
            "value": 11.313485714285715,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 19.799s"
          },
          {
            "name": "RAJA_Seq-default_Basic_NESTED_INIT",
            "value": 3.57378,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 17.869s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_ATOMIC",
            "value": 5.2772,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.319s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_REDUCE",
            "value": 10.56396,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 2.641s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE3_INT",
            "value": 2.8289,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.707s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE_STRUCT",
            "value": 6.96224,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.741s"
          },
          {
            "name": "RAJA_Seq-default_Basic_TRAP_INT",
            "value": 13.30924,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.327s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULTI_REDUCE",
            "value": 10.84472,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 2.711s"
          },
          {
            "name": "RAJA_Seq-new_Basic_PI_REDUCE",
            "value": 5.35532,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.339s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE3_INT",
            "value": 1.446252,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.362s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE_STRUCT",
            "value": 12.25036,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.063s"
          },
          {
            "name": "RAJA_Seq-new_Basic_TRAP_INT",
            "value": 13.24996,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.312s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "Adrien Bernede",
            "username": "adrienbernede",
            "email": "51493078+adrienbernede@users.noreply.github.com"
          },
          "committer": {
            "name": "GitHub",
            "username": "web-flow",
            "email": "noreply@github.com"
          },
          "id": "95c360ddf585410e6123956c72a2c648111ece29",
          "message": "Merge pull request #652 from llnl/woptim/rocm7",
          "timestamp": "2026-04-16T19:31:14Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/95c360ddf585410e6123956c72a2c648111ece29"
        },
        "date": 1776372480796,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_OpenMP-default_Basic_ARRAY_OF_PTRS",
            "value": 4.35872,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.090s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_COPY8",
            "value": 3.661032,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.915s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY",
            "value": 0.1649592,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.412s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY_ATOMIC",
            "value": 1.60732,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 4.018s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_EMPTY",
            "value": 0.0479314,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 0.240s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_IF_QUAD",
            "value": 1.956777777777778,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 1.761s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INDEXLIST_3LOOP",
            "value": 1.661122,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.831s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT3",
            "value": 0.75066,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 1.877s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D",
            "value": 0.10340880000000001,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 1.293s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.10369360000000001,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 1.296s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MAT_MAT_SHARED",
            "value": 199.21399999999997,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 4.980s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULADDSUB",
            "value": 0.7653657142857143,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 1.339s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_NESTED_INIT",
            "value": 0.18819840000000002,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 0.941s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_ATOMIC",
            "value": 280.6972,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 70.174s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_REDUCE",
            "value": 0.3569888,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.089s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE3_INT",
            "value": 0.1302684,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.033s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE_STRUCT",
            "value": 0.3855972,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.096s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_TRAP_INT",
            "value": 0.862848,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.216s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULTI_REDUCE",
            "value": 0.68924,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.172s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_PI_REDUCE",
            "value": 0.3508324,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.088s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE3_INT",
            "value": 0.113108,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.028s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE_STRUCT",
            "value": 0.3514588,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.088s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_TRAP_INT",
            "value": 0.857348,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.214s"
          },
          {
            "name": "RAJA_Seq-default_Basic_ARRAY_OF_PTRS",
            "value": 70.8052,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 17.701s"
          },
          {
            "name": "RAJA_Seq-default_Basic_COPY8",
            "value": 39.4952,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 9.874s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY",
            "value": 4.0972,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 10.243s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY_ATOMIC",
            "value": 4.09944,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 10.249s"
          },
          {
            "name": "RAJA_Seq-default_Basic_EMPTY",
            "value": 1.3224679999999998,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 6.612s"
          },
          {
            "name": "RAJA_Seq-default_Basic_IF_QUAD",
            "value": 32.50411111111111,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 29.254s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INDEXLIST_3LOOP",
            "value": 29.207,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 14.604s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT3",
            "value": 11.00072,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 27.502s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D",
            "value": 2.9612560000000006,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 37.016s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 3.1018079999999997,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 38.773s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MAT_MAT_SHARED",
            "value": 3050.188,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 76.255s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULADDSUB",
            "value": 11.370228571428573,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 19.898s"
          },
          {
            "name": "RAJA_Seq-default_Basic_NESTED_INIT",
            "value": 3.56952,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 17.848s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_ATOMIC",
            "value": 5.27748,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.319s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_REDUCE",
            "value": 10.56432,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 2.641s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE3_INT",
            "value": 2.720616,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.680s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE_STRUCT",
            "value": 6.93576,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.734s"
          },
          {
            "name": "RAJA_Seq-default_Basic_TRAP_INT",
            "value": 13.31092,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.328s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULTI_REDUCE",
            "value": 10.8266,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 2.707s"
          },
          {
            "name": "RAJA_Seq-new_Basic_PI_REDUCE",
            "value": 5.35556,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.339s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE3_INT",
            "value": 1.44568,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.361s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE_STRUCT",
            "value": 12.2516,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.063s"
          },
          {
            "name": "RAJA_Seq-new_Basic_TRAP_INT",
            "value": 13.25504,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.314s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "Jason Burmark",
            "username": "MrBurmark",
            "email": "MrBurmark@users.noreply.github.com"
          },
          "committer": {
            "name": "GitHub",
            "username": "web-flow",
            "email": "noreply@github.com"
          },
          "id": "9bc67910045283981831de42fe96bd151eee0761",
          "message": "Merge pull request #681 from llnl/pr-from-fork/680\n\nPr from fork/680",
          "timestamp": "2026-04-20T16:10:32Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/9bc67910045283981831de42fe96bd151eee0761"
        },
        "date": 1776704792824,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_OpenMP-default_Basic_ARRAY_OF_PTRS",
            "value": 4.37612,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.094s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_COPY8",
            "value": 3.642256,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.911s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY",
            "value": 0.15808519999999998,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.395s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY_ATOMIC",
            "value": 1.6065519999999998,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 4.016s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_EMPTY",
            "value": 0.04862639999999999,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 0.243s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_IF_QUAD",
            "value": 1.9538555555555557,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 1.758s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INDEXLIST_3LOOP",
            "value": 1.664564,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.832s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT3",
            "value": 0.7341719999999999,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 1.835s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D",
            "value": 0.1043816,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 1.305s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.1036464,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 1.296s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MAT_MAT_SHARED",
            "value": 197.9088,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 4.948s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULADDSUB",
            "value": 0.7524971428571428,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 1.317s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_NESTED_INIT",
            "value": 0.1883444,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 0.942s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_ATOMIC",
            "value": 301.6136,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 75.403s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_REDUCE",
            "value": 0.3557784,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.089s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE3_INT",
            "value": 0.1259124,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.031s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE_STRUCT",
            "value": 0.3804248,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.095s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_TRAP_INT",
            "value": 0.861516,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.215s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULTI_REDUCE",
            "value": 0.68906,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.172s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_PI_REDUCE",
            "value": 0.3504436,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.088s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE3_INT",
            "value": 0.1123604,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.028s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE_STRUCT",
            "value": 0.3507824,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.088s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_TRAP_INT",
            "value": 0.853524,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.213s"
          },
          {
            "name": "RAJA_Seq-default_Basic_ARRAY_OF_PTRS",
            "value": 69.3124,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 17.328s"
          },
          {
            "name": "RAJA_Seq-default_Basic_COPY8",
            "value": 39.48824,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 9.872s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY",
            "value": 3.935712,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 9.839s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY_ATOMIC",
            "value": 3.9336480000000003,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 9.834s"
          },
          {
            "name": "RAJA_Seq-default_Basic_EMPTY",
            "value": 1.321342,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 6.607s"
          },
          {
            "name": "RAJA_Seq-default_Basic_IF_QUAD",
            "value": 32.288666666666664,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 29.060s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INDEXLIST_3LOOP",
            "value": 28.6116,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 14.306s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT3",
            "value": 10.90916,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 27.273s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D",
            "value": 2.866712,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 35.834s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 2.990896,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 37.386s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MAT_MAT_SHARED",
            "value": 3019.092,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 75.477s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULADDSUB",
            "value": 11.239885714285714,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 19.670s"
          },
          {
            "name": "RAJA_Seq-default_Basic_NESTED_INIT",
            "value": 3.4599,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 17.299s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_ATOMIC",
            "value": 5.277399999999999,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.319s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_REDUCE",
            "value": 10.56364,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 2.641s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE3_INT",
            "value": 2.82856,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.707s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE_STRUCT",
            "value": 6.65616,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.664s"
          },
          {
            "name": "RAJA_Seq-default_Basic_TRAP_INT",
            "value": 13.31196,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.328s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULTI_REDUCE",
            "value": 10.81824,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 2.705s"
          },
          {
            "name": "RAJA_Seq-new_Basic_PI_REDUCE",
            "value": 5.355479999999999,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.339s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE3_INT",
            "value": 1.449824,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.362s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE_STRUCT",
            "value": 12.24964,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.062s"
          },
          {
            "name": "RAJA_Seq-new_Basic_TRAP_INT",
            "value": 13.25244,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.313s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "Jason Burmark",
            "username": "MrBurmark",
            "email": "MrBurmark@users.noreply.github.com"
          },
          "committer": {
            "name": "GitHub",
            "username": "web-flow",
            "email": "noreply@github.com"
          },
          "id": "9bc67910045283981831de42fe96bd151eee0761",
          "message": "Merge pull request #681 from llnl/pr-from-fork/680\n\nPr from fork/680",
          "timestamp": "2026-04-20T16:10:32Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/9bc67910045283981831de42fe96bd151eee0761"
        },
        "date": 1779383410410,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_OpenMP-default_Basic_ARRAY_OF_PTRS",
            "value": 4.19532,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.049s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_COPY8",
            "value": 3.561416,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.890s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY",
            "value": 0.1504788,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.376s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY_ATOMIC",
            "value": 1.60118,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 4.003s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_EMPTY",
            "value": 0.048312799999999996,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 0.242s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_IF_QUAD",
            "value": 1.9185444444444446,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 1.727s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INDEXLIST_3LOOP",
            "value": 1.60392,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.802s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT3",
            "value": 0.671112,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 1.678s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D",
            "value": 0.10338960000000001,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 1.292s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.1033808,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 1.292s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MAT_MAT_SHARED",
            "value": 163.63559999999998,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 4.091s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULADDSUB",
            "value": 0.6778171428571429,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 1.186s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_NESTED_INIT",
            "value": 0.1878952,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 0.939s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_ATOMIC",
            "value": 281.5144,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 70.379s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_REDUCE",
            "value": 0.3565056,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.089s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE3_INT",
            "value": 0.1108148,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.028s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE_STRUCT",
            "value": 0.3750176,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.094s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_TRAP_INT",
            "value": 0.845052,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.211s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULTI_REDUCE",
            "value": 0.692556,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.173s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_PI_REDUCE",
            "value": 0.349544,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.087s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE3_INT",
            "value": 0.1004432,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.025s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE_STRUCT",
            "value": 0.3474152,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.087s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_TRAP_INT",
            "value": 0.850376,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.213s"
          },
          {
            "name": "RAJA_Seq-default_Basic_ARRAY_OF_PTRS",
            "value": 72.2236,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 18.056s"
          },
          {
            "name": "RAJA_Seq-default_Basic_COPY8",
            "value": 39.1368,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 9.784s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY",
            "value": 4.115880000000001,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 10.290s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY_ATOMIC",
            "value": 4.118760000000001,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 10.297s"
          },
          {
            "name": "RAJA_Seq-default_Basic_EMPTY",
            "value": 1.321194,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 6.606s"
          },
          {
            "name": "RAJA_Seq-default_Basic_IF_QUAD",
            "value": 32.28344444444444,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 29.055s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INDEXLIST_3LOOP",
            "value": 29.0272,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 14.514s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT3",
            "value": 10.98796,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 27.470s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D",
            "value": 3.011488,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 37.644s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 3.0135359999999998,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 37.669s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MAT_MAT_SHARED",
            "value": 3041.0759999999996,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 76.027s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULADDSUB",
            "value": 11.34702857142857,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 19.857s"
          },
          {
            "name": "RAJA_Seq-default_Basic_NESTED_INIT",
            "value": 3.5558,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 17.779s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_ATOMIC",
            "value": 5.27736,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.319s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_REDUCE",
            "value": 10.56572,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 2.641s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE3_INT",
            "value": 2.825184,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.706s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE_STRUCT",
            "value": 6.95504,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.739s"
          },
          {
            "name": "RAJA_Seq-default_Basic_TRAP_INT",
            "value": 13.31016,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.328s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULTI_REDUCE",
            "value": 10.83932,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 2.710s"
          },
          {
            "name": "RAJA_Seq-new_Basic_PI_REDUCE",
            "value": 5.3554,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.339s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE3_INT",
            "value": 1.448704,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.362s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE_STRUCT",
            "value": 12.26,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.065s"
          },
          {
            "name": "RAJA_Seq-new_Basic_TRAP_INT",
            "value": 13.24904,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.312s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "Adrien Bernede",
            "username": "adrienbernede",
            "email": "51493078+adrienbernede@users.noreply.github.com"
          },
          "committer": {
            "name": "GitHub",
            "username": "web-flow",
            "email": "noreply@github.com"
          },
          "id": "1f660e77acdb6f12df7a4e39a25172328afb7352",
          "message": "Merge pull request #645 from llnl/woptim/ci-logs-and-errors\n\nImprove CI jobs logs and error handling",
          "timestamp": "2026-06-16T16:08:18Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/1f660e77acdb6f12df7a4e39a25172328afb7352"
        },
        "date": 1781641870004,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_OpenMP-default_Basic_ARRAY_OF_PTRS",
            "value": 4.09564,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.024s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_COPY8",
            "value": 3.54526,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.886s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY",
            "value": 0.15113279999999998,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.378s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY_ATOMIC",
            "value": 1.6015640000000002,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 4.004s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_EMPTY",
            "value": 0.0482848,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 0.241s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_IF_QUAD",
            "value": 1.9208222222222222,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 1.729s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INDEXLIST_3LOOP",
            "value": 1.604504,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.802s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT3",
            "value": 0.66662,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 1.667s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D",
            "value": 0.10394719999999999,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 1.299s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.103624,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 1.295s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MAT_MAT_SHARED",
            "value": 162.9932,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 4.075s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULADDSUB",
            "value": 0.6764457142857143,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 1.184s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_NESTED_INIT",
            "value": 0.18848700000000002,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 0.942s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_ATOMIC",
            "value": 305.8168,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 76.454s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_REDUCE",
            "value": 0.3567408,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.089s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE3_INT",
            "value": 0.1092836,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.027s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE_STRUCT",
            "value": 0.37315,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.093s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_TRAP_INT",
            "value": 0.847084,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.212s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULTI_REDUCE",
            "value": 0.692136,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.173s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_PI_REDUCE",
            "value": 0.350162,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.088s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE3_INT",
            "value": 0.10101,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.025s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE_STRUCT",
            "value": 0.3471308,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.087s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_TRAP_INT",
            "value": 0.850564,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.213s"
          },
          {
            "name": "RAJA_Seq-default_Basic_ARRAY_OF_PTRS",
            "value": 71.3432,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 17.836s"
          },
          {
            "name": "RAJA_Seq-default_Basic_COPY8",
            "value": 39.00452,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 9.751s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY",
            "value": 4.09968,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 10.249s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY_ATOMIC",
            "value": 4.0974,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 10.243s"
          },
          {
            "name": "RAJA_Seq-default_Basic_EMPTY",
            "value": 1.321148,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 6.606s"
          },
          {
            "name": "RAJA_Seq-default_Basic_IF_QUAD",
            "value": 32.29222222222222,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 29.063s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INDEXLIST_3LOOP",
            "value": 28.9316,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 14.466s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT3",
            "value": 10.982280000000001,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 27.456s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D",
            "value": 2.9312959999999997,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 36.641s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 2.9402,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 36.752s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MAT_MAT_SHARED",
            "value": 3036.0680000000007,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 75.902s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULADDSUB",
            "value": 11.366685714285715,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 19.892s"
          },
          {
            "name": "RAJA_Seq-default_Basic_NESTED_INIT",
            "value": 3.5130800000000004,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 17.565s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_ATOMIC",
            "value": 5.27696,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.319s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_REDUCE",
            "value": 10.56276,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 2.641s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE3_INT",
            "value": 2.825008,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.706s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE_STRUCT",
            "value": 6.84888,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.712s"
          },
          {
            "name": "RAJA_Seq-default_Basic_TRAP_INT",
            "value": 13.30944,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.327s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULTI_REDUCE",
            "value": 10.82564,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 2.706s"
          },
          {
            "name": "RAJA_Seq-new_Basic_PI_REDUCE",
            "value": 5.35752,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.339s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE3_INT",
            "value": 1.448232,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.362s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE_STRUCT",
            "value": 12.24952,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.062s"
          },
          {
            "name": "RAJA_Seq-new_Basic_TRAP_INT",
            "value": 13.2496,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.312s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "Jason Burmark",
            "username": "MrBurmark",
            "email": "MrBurmark@users.noreply.github.com"
          },
          "committer": {
            "name": "GitHub",
            "username": "web-flow",
            "email": "noreply@github.com"
          },
          "id": "3d72e32abcb569a20f48069f2b639663f54ca04f",
          "message": "Merge pull request #690 from llnl/bugfix/burmark1/flop_counts\n\nFix flop counts in MASS3DEA and DIFFUSION3DPA",
          "timestamp": "2026-06-22T15:32:25Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/3d72e32abcb569a20f48069f2b639663f54ca04f"
        },
        "date": 1782145979558,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_OpenMP-default_Basic_ARRAY_OF_PTRS",
            "value": 4.06376,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.016s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_COPY8",
            "value": 3.564952,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.891s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY",
            "value": 0.16061720000000002,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.402s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY_ATOMIC",
            "value": 1.601276,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 4.003s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_EMPTY",
            "value": 0.0481728,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 0.241s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_IF_QUAD",
            "value": 1.918088888888889,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 1.726s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INDEXLIST_3LOOP",
            "value": 1.6065400000000003,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.803s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT3",
            "value": 0.6772199999999999,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 1.693s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D",
            "value": 0.1039824,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 1.300s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.1030944,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 1.289s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MAT_MAT_SHARED",
            "value": 164.10399999999998,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 4.103s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULADDSUB",
            "value": 0.6787771428571427,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 1.188s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_NESTED_INIT",
            "value": 0.1884044,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 0.942s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_ATOMIC",
            "value": 276.9216,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 69.230s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_REDUCE",
            "value": 0.3555428,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.089s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE3_INT",
            "value": 0.1091736,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.027s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE_STRUCT",
            "value": 0.3738888,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.093s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_TRAP_INT",
            "value": 0.845116,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.211s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULTI_REDUCE",
            "value": 0.691632,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.173s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_PI_REDUCE",
            "value": 0.3515568,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.088s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE3_INT",
            "value": 0.100758,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.025s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE_STRUCT",
            "value": 0.3470436,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.087s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_TRAP_INT",
            "value": 0.849112,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.212s"
          },
          {
            "name": "RAJA_Seq-default_Basic_ARRAY_OF_PTRS",
            "value": 66.626,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 16.657s"
          },
          {
            "name": "RAJA_Seq-default_Basic_COPY8",
            "value": 39.530559999999994,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 9.883s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY",
            "value": 4.0942,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 10.236s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY_ATOMIC",
            "value": 4.09296,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 10.232s"
          },
          {
            "name": "RAJA_Seq-default_Basic_EMPTY",
            "value": 1.321358,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 6.607s"
          },
          {
            "name": "RAJA_Seq-default_Basic_IF_QUAD",
            "value": 32.29388888888889,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 29.064s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INDEXLIST_3LOOP",
            "value": 28.9426,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 14.471s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT3",
            "value": 10.99408,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 27.485s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D",
            "value": 2.8071839999999995,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 35.090s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 2.877672,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 35.971s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MAT_MAT_SHARED",
            "value": 3032.5879999999997,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 75.815s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULADDSUB",
            "value": 11.309714285714286,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 19.792s"
          },
          {
            "name": "RAJA_Seq-default_Basic_NESTED_INIT",
            "value": 3.5398200000000006,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 17.699s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_ATOMIC",
            "value": 5.277520000000001,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.319s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_REDUCE",
            "value": 10.564,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 2.641s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE3_INT",
            "value": 2.82628,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.707s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE_STRUCT",
            "value": 6.84528,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.711s"
          },
          {
            "name": "RAJA_Seq-default_Basic_TRAP_INT",
            "value": 13.30892,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.327s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULTI_REDUCE",
            "value": 10.83388,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 2.708s"
          },
          {
            "name": "RAJA_Seq-new_Basic_PI_REDUCE",
            "value": 5.35556,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.339s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE3_INT",
            "value": 1.44894,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.362s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE_STRUCT",
            "value": 12.25232,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.063s"
          },
          {
            "name": "RAJA_Seq-new_Basic_TRAP_INT",
            "value": 13.2456,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.311s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "Jason Burmark",
            "username": "MrBurmark",
            "email": "MrBurmark@users.noreply.github.com"
          },
          "committer": {
            "name": "GitHub",
            "username": "web-flow",
            "email": "noreply@github.com"
          },
          "id": "1d167298cf1ca50f6717c31f54fac8a34527c3d0",
          "message": "Merge pull request #700 from llnl/jacobi-2d-launches\n\nJacobi 2D Seq, OMP, and OMPTarget launch twice",
          "timestamp": "2026-08-06T01:57:56Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/1d167298cf1ca50f6717c31f54fac8a34527c3d0"
        },
        "date": 1785985314033,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_OpenMP-default_Basic_ARRAY_OF_PTRS",
            "value": 4.16224,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.041s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_COPY8",
            "value": 3.545084,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.886s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY",
            "value": 0.1580452,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.395s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY_ATOMIC",
            "value": 1.602488,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 4.006s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_EMPTY",
            "value": 0.0894868,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 0.447s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_IF_QUAD",
            "value": 1.9115777777777778,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 1.720s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INDEXLIST_3LOOP",
            "value": 1.611634,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.806s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT3",
            "value": 0.683368,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 1.708s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D",
            "value": 0.17262560000000002,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 2.158s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.10356880000000002,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 1.295s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MAT_MAT_SHARED",
            "value": 159.94160000000002,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 3.999s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULADDSUB",
            "value": 0.6813828571428572,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 1.192s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_NESTED_INIT",
            "value": 0.23055799999999999,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 1.153s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_ATOMIC",
            "value": 309.7984,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 77.450s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_REDUCE",
            "value": 0.3570872,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.089s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE3_INT",
            "value": 0.1100236,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.028s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE_STRUCT",
            "value": 0.3745968,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.094s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_TRAP_INT",
            "value": 0.858796,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.215s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULTI_REDUCE",
            "value": 0.692356,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.173s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_PI_REDUCE",
            "value": 0.3535016,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.088s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE3_INT",
            "value": 0.1012804,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.025s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE_STRUCT",
            "value": 0.3476432,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.087s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_TRAP_INT",
            "value": 0.85932,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.215s"
          },
          {
            "name": "RAJA_Seq-default_Basic_ARRAY_OF_PTRS",
            "value": 42.3976,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 10.599s"
          },
          {
            "name": "RAJA_Seq-default_Basic_COPY8",
            "value": 38.8352,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 9.709s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY",
            "value": 4.0252,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 10.063s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY_ATOMIC",
            "value": 4.0202800000000005,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 10.051s"
          },
          {
            "name": "RAJA_Seq-default_Basic_EMPTY",
            "value": 1.321346,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 6.607s"
          },
          {
            "name": "RAJA_Seq-default_Basic_IF_QUAD",
            "value": 31.519888888888886,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 28.368s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INDEXLIST_3LOOP",
            "value": 27.857,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 13.928s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT3",
            "value": 10.982439999999999,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 27.456s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D",
            "value": 3.2152319999999994,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 40.190s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 3.1995440000000004,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 39.994s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MAT_MAT_SHARED",
            "value": 3046.376,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 76.159s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULADDSUB",
            "value": 11.344,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 19.852s"
          },
          {
            "name": "RAJA_Seq-default_Basic_NESTED_INIT",
            "value": 3.4683599999999997,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 17.342s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_ATOMIC",
            "value": 5.28196,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.320s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_REDUCE",
            "value": 10.56392,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 2.641s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE3_INT",
            "value": 2.825108,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.706s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE_STRUCT",
            "value": 6.86808,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.717s"
          },
          {
            "name": "RAJA_Seq-default_Basic_TRAP_INT",
            "value": 13.30816,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.327s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULTI_REDUCE",
            "value": 10.83004,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 2.708s"
          },
          {
            "name": "RAJA_Seq-new_Basic_PI_REDUCE",
            "value": 5.355479999999999,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.339s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE3_INT",
            "value": 1.448036,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.362s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE_STRUCT",
            "value": 12.24488,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.061s"
          },
          {
            "name": "RAJA_Seq-new_Basic_TRAP_INT",
            "value": 13.24076,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.310s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "Jason Burmark",
            "username": "MrBurmark",
            "email": "MrBurmark@users.noreply.github.com"
          },
          "committer": {
            "name": "GitHub",
            "username": "web-flow",
            "email": "noreply@github.com"
          },
          "id": "338cfeb7266607648bd06815ba103f61e357c2e5",
          "message": "Merge pull request #695 from llnl/multi-kernel-regions\n\nCaliper: Annotate kernels separately",
          "timestamp": "2026-08-10T20:40:11Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/338cfeb7266607648bd06815ba103f61e357c2e5"
        },
        "date": 1786408570357,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_OpenMP-default_Basic_ARRAY_OF_PTRS",
            "value": 4.03148,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.008s"
          },
          {
            "name": "RAJA_OpenMP-default_ARRAY_OF_PTRS_1",
            "value": 0.0161152,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 1.007s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_COPY8",
            "value": 3.60734,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.902s"
          },
          {
            "name": "RAJA_OpenMP-default_COPY8_1",
            "value": 0.014418768,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.901s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY",
            "value": 0.16358399999999998,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.409s"
          },
          {
            "name": "RAJA_OpenMP-default_DAXPY_1",
            "value": 0.00006512416,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 2500, total_time: 0.407s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY_ATOMIC",
            "value": 1.603688,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 4.009s"
          },
          {
            "name": "RAJA_OpenMP-default_DAXPY_ATOMIC_1",
            "value": 0.0006411376,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 2500, total_time: 4.007s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_EMPTY",
            "value": 0.11238760000000002,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 0.562s"
          },
          {
            "name": "RAJA_OpenMP-default_EMPTY_1",
            "value": 0.000022384360000000003,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 5000, total_time: 0.560s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_IF_QUAD",
            "value": 1.928911111111111,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 1.736s"
          },
          {
            "name": "RAJA_OpenMP-default_IF_QUAD_1",
            "value": 0.002141148148148148,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 900, total_time: 1.734s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INDEXLIST_3LOOP",
            "value": 1.65608,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.828s"
          },
          {
            "name": "RAJA_OpenMP-default_INDEXLIST_3LOOP_1",
            "value": 0.0003802772,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 500, total_time: 0.095s"
          },
          {
            "name": "RAJA_OpenMP-default_INDEXLIST_3LOOP_2",
            "value": 0.002559748,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 500, total_time: 0.640s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT3",
            "value": 0.685152,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 1.713s"
          },
          {
            "name": "RAJA_OpenMP-default_INIT3_1",
            "value": 0.0002732784,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 2500, total_time: 1.708s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D",
            "value": 0.1736608,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 2.171s"
          },
          {
            "name": "RAJA_OpenMP-default_INIT_VIEW1D_1",
            "value": 0.000013837823999999999,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 12500, total_time: 2.162s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.1051424,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 1.314s"
          },
          {
            "name": "RAJA_OpenMP-default_INIT_VIEW1D_OFFSET_1",
            "value": 0.000008360384000000002,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 12500, total_time: 1.306s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MAT_MAT_SHARED",
            "value": 167.1512,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 4.179s"
          },
          {
            "name": "RAJA_OpenMP-default_MAT_MAT_SHARED_1",
            "value": 6.685967999999999,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 25, total_time: 4.179s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULADDSUB",
            "value": 0.6894800000000001,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 1.207s"
          },
          {
            "name": "RAJA_OpenMP-default_MULADDSUB_1",
            "value": 0.00039295020408163265,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1750, total_time: 1.203s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_NESTED_INIT",
            "value": 0.19537659999999998,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 0.977s"
          },
          {
            "name": "RAJA_OpenMP-default_NESTED_INIT_1",
            "value": 0.00003892184,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 5000, total_time: 0.973s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_ATOMIC",
            "value": 286.142,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 71.535s"
          },
          {
            "name": "RAJA_OpenMP-default_PI_ATOMIC_1",
            "value": 1.1445568000000002,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 71.535s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_REDUCE",
            "value": 0.3577364,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.089s"
          },
          {
            "name": "RAJA_OpenMP-default_PI_REDUCE_1",
            "value": 0.0014280336,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.089s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE3_INT",
            "value": 0.1114284,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.028s"
          },
          {
            "name": "RAJA_OpenMP-default_REDUCE3_INT_1",
            "value": 0.00044373759999999997,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.028s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE_STRUCT",
            "value": 0.3750836,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.094s"
          },
          {
            "name": "RAJA_OpenMP-default_REDUCE_STRUCT_1",
            "value": 0.0014960848,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.094s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_TRAP_INT",
            "value": 0.860116,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.215s"
          },
          {
            "name": "RAJA_OpenMP-default_TRAP_INT_1",
            "value": 0.0034376000000000003,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.215s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULTI_REDUCE",
            "value": 0.696148,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.174s"
          },
          {
            "name": "RAJA_OpenMP-default_MULTI_REDUCE_1",
            "value": 0.002780768,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.174s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_PI_REDUCE",
            "value": 0.3523564,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.088s"
          },
          {
            "name": "RAJA_OpenMP-new_PI_REDUCE_1",
            "value": 0.0014075168000000002,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.088s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE3_INT",
            "value": 0.1021044,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.026s"
          },
          {
            "name": "RAJA_OpenMP-new_REDUCE3_INT_1",
            "value": 0.00040635199999999996,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.025s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE_STRUCT",
            "value": 0.3491584,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.087s"
          },
          {
            "name": "RAJA_OpenMP-new_REDUCE_STRUCT_1",
            "value": 0.0013932176,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.087s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_TRAP_INT",
            "value": 0.855092,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.214s"
          },
          {
            "name": "RAJA_OpenMP-new_TRAP_INT_1",
            "value": 0.003418448,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.214s"
          },
          {
            "name": "RAJA_Seq-default_Basic_ARRAY_OF_PTRS",
            "value": 42.7244,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 10.681s"
          },
          {
            "name": "RAJA_Seq-default_ARRAY_OF_PTRS_1",
            "value": 0.17088640000000002,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 10.680s"
          },
          {
            "name": "RAJA_Seq-default_Basic_COPY8",
            "value": 37.53924,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 9.385s"
          },
          {
            "name": "RAJA_Seq-default_COPY8_1",
            "value": 0.15014672,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 9.384s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY",
            "value": 4.06736,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 10.168s"
          },
          {
            "name": "RAJA_Seq-default_DAXPY_1",
            "value": 0.001625968,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 2500, total_time: 10.162s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY_ATOMIC",
            "value": 4.0762,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 10.191s"
          },
          {
            "name": "RAJA_Seq-default_DAXPY_ATOMIC_1",
            "value": 0.001629456,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 2500, total_time: 10.184s"
          },
          {
            "name": "RAJA_Seq-default_Basic_EMPTY",
            "value": 1.322224,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 6.611s"
          },
          {
            "name": "RAJA_Seq-default_EMPTY_1",
            "value": 0.00026434799999999997,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 5000, total_time: 6.609s"
          },
          {
            "name": "RAJA_Seq-default_Basic_IF_QUAD",
            "value": 32.315777777777775,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 29.084s"
          },
          {
            "name": "RAJA_Seq-default_IF_QUAD_1",
            "value": 0.03590308641975309,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 900, total_time: 29.081s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INDEXLIST_3LOOP",
            "value": 28.4316,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 14.216s"
          },
          {
            "name": "RAJA_Seq-default_INDEXLIST_3LOOP_1",
            "value": 0.00921964,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 500, total_time: 2.305s"
          },
          {
            "name": "RAJA_Seq-default_INDEXLIST_3LOOP_2",
            "value": 0.041081599999999996,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 500, total_time: 10.270s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT3",
            "value": 11.505679999999998,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 28.764s"
          },
          {
            "name": "RAJA_Seq-default_INIT3_1",
            "value": 0.004601216,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 2500, total_time: 28.758s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D",
            "value": 3.0316,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 37.895s"
          },
          {
            "name": "RAJA_Seq-default_INIT_VIEW1D_1",
            "value": 0.00024237696,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 12500, total_time: 37.871s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 3.0981840000000003,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 38.727s"
          },
          {
            "name": "RAJA_Seq-default_INIT_VIEW1D_OFFSET_1",
            "value": 0.00024770368,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 12500, total_time: 38.704s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MAT_MAT_SHARED",
            "value": 3064.72,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 76.618s"
          },
          {
            "name": "RAJA_Seq-default_MAT_MAT_SHARED_1",
            "value": 122.58864000000001,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 25, total_time: 76.618s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULADDSUB",
            "value": 11.957485714285713,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 20.926s"
          },
          {
            "name": "RAJA_Seq-default_MULADDSUB_1",
            "value": 0.006831314285714285,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1750, total_time: 20.921s"
          },
          {
            "name": "RAJA_Seq-default_Basic_NESTED_INIT",
            "value": 3.6049,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 18.024s"
          },
          {
            "name": "RAJA_Seq-default_NESTED_INIT_1",
            "value": 0.0007206119999999999,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 5000, total_time: 18.015s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_ATOMIC",
            "value": 5.28428,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.321s"
          },
          {
            "name": "RAJA_Seq-default_PI_ATOMIC_1",
            "value": 0.0211352,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 1.321s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_REDUCE",
            "value": 10.58036,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 2.645s"
          },
          {
            "name": "RAJA_Seq-default_PI_REDUCE_1",
            "value": 0.04231952,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 2.645s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE3_INT",
            "value": 2.853768,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.713s"
          },
          {
            "name": "RAJA_Seq-default_REDUCE3_INT_1",
            "value": 0.011410640000000001,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.713s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE_STRUCT",
            "value": 6.7912,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.698s"
          },
          {
            "name": "RAJA_Seq-default_REDUCE_STRUCT_1",
            "value": 0.02715712,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 1.697s"
          },
          {
            "name": "RAJA_Seq-default_Basic_TRAP_INT",
            "value": 13.461319999999999,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.365s"
          },
          {
            "name": "RAJA_Seq-default_TRAP_INT_1",
            "value": 0.05384336,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 3.365s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULTI_REDUCE",
            "value": 10.76304,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 2.691s"
          },
          {
            "name": "RAJA_Seq-default_MULTI_REDUCE_1",
            "value": 0.04304304,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 2.690s"
          },
          {
            "name": "RAJA_Seq-new_Basic_PI_REDUCE",
            "value": 5.36676,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.342s"
          },
          {
            "name": "RAJA_Seq-new_PI_REDUCE_1",
            "value": 0.021465119999999997,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 1.342s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE3_INT",
            "value": 1.485012,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.371s"
          },
          {
            "name": "RAJA_Seq-new_REDUCE3_INT_1",
            "value": 0.005935663999999999,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.371s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE_STRUCT",
            "value": 12.2674,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.067s"
          },
          {
            "name": "RAJA_Seq-new_REDUCE_STRUCT_1",
            "value": 0.04906352,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 3.066s"
          },
          {
            "name": "RAJA_Seq-new_Basic_TRAP_INT",
            "value": 13.445200000000002,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.361s"
          },
          {
            "name": "RAJA_Seq-new_TRAP_INT_1",
            "value": 0.053778879999999994,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 3.361s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "Jason Burmark",
            "username": "MrBurmark",
            "email": "MrBurmark@users.noreply.github.com"
          },
          "committer": {
            "name": "GitHub",
            "username": "web-flow",
            "email": "noreply@github.com"
          },
          "id": "5301486b1ea58280079a4fe252710db5e01e758a",
          "message": "Merge pull request #687 from llnl/pr-from-fork/686\n\nPr from fork/686",
          "timestamp": "2026-08-20T19:54:32Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/5301486b1ea58280079a4fe252710db5e01e758a"
        },
        "date": 1787293823280,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_OpenMP-default_Basic_ARRAY_OF_PTRS",
            "value": 4.10488,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.026s"
          },
          {
            "name": "RAJA_OpenMP-default_ARRAY_OF_PTRS_1",
            "value": 0.01640832,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 1.026s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_COPY8",
            "value": 3.574948,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.894s"
          },
          {
            "name": "RAJA_OpenMP-default_COPY8_1",
            "value": 0.014288432,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.893s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY",
            "value": 0.1541232,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.385s"
          },
          {
            "name": "RAJA_OpenMP-default_DAXPY_1",
            "value": 0.0000613352,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 2500, total_time: 0.383s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY_ATOMIC",
            "value": 1.6037800000000002,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 4.009s"
          },
          {
            "name": "RAJA_OpenMP-default_DAXPY_ATOMIC_1",
            "value": 0.0006411792,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 2500, total_time: 4.007s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_EMPTY",
            "value": 0.0492338,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 0.246s"
          },
          {
            "name": "RAJA_OpenMP-default_EMPTY_1",
            "value": 0.00000975364,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 5000, total_time: 0.244s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_IF_QUAD",
            "value": 1.9523222222222223,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 1.757s"
          },
          {
            "name": "RAJA_OpenMP-default_IF_QUAD_1",
            "value": 0.0021669753086419755,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 900, total_time: 1.755s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INDEXLIST_3LOOP",
            "value": 1.612618,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.806s"
          },
          {
            "name": "RAJA_OpenMP-default_INDEXLIST_3LOOP_1",
            "value": 0.0003764668,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 500, total_time: 0.094s"
          },
          {
            "name": "RAJA_OpenMP-default_INDEXLIST_3LOOP_2",
            "value": 0.002477164,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 500, total_time: 0.619s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT3",
            "value": 0.679624,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 1.699s"
          },
          {
            "name": "RAJA_OpenMP-default_INIT3_1",
            "value": 0.000271112,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 2500, total_time: 1.694s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D",
            "value": 0.1052432,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 1.316s"
          },
          {
            "name": "RAJA_OpenMP-default_INIT_VIEW1D_1",
            "value": 0.000008366912,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 12500, total_time: 1.307s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.1735696,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 2.170s"
          },
          {
            "name": "RAJA_OpenMP-default_INIT_VIEW1D_OFFSET_1",
            "value": 0.00001383136,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 12500, total_time: 2.161s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MAT_MAT_SHARED",
            "value": 163.5968,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 4.090s"
          },
          {
            "name": "RAJA_OpenMP-default_MAT_MAT_SHARED_1",
            "value": 6.543775999999999,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 25, total_time: 4.090s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULADDSUB",
            "value": 0.6889485714285714,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 1.206s"
          },
          {
            "name": "RAJA_OpenMP-default_MULADDSUB_1",
            "value": 0.0003925812244897959,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1750, total_time: 1.202s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_NESTED_INIT",
            "value": 0.209196,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 1.046s"
          },
          {
            "name": "RAJA_OpenMP-default_NESTED_INIT_1",
            "value": 0.0000417012,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 5000, total_time: 1.043s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_ATOMIC",
            "value": 269.6572,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 67.414s"
          },
          {
            "name": "RAJA_OpenMP-default_PI_ATOMIC_1",
            "value": 1.0786176,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 67.414s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_REDUCE",
            "value": 0.3575436,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.089s"
          },
          {
            "name": "RAJA_OpenMP-default_PI_REDUCE_1",
            "value": 0.001427512,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.089s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE3_INT",
            "value": 0.1100704,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.028s"
          },
          {
            "name": "RAJA_OpenMP-default_REDUCE3_INT_1",
            "value": 0.0004383184,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.027s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE_STRUCT",
            "value": 0.3750508,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.094s"
          },
          {
            "name": "RAJA_OpenMP-default_REDUCE_STRUCT_1",
            "value": 0.0014965296,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.094s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_TRAP_INT",
            "value": 0.86016,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.215s"
          },
          {
            "name": "RAJA_OpenMP-default_TRAP_INT_1",
            "value": 0.003437952,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.215s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULTI_REDUCE",
            "value": 0.694392,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.174s"
          },
          {
            "name": "RAJA_OpenMP-default_MULTI_REDUCE_1",
            "value": 0.002774432,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.173s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_PI_REDUCE",
            "value": 0.352796,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.088s"
          },
          {
            "name": "RAJA_OpenMP-new_PI_REDUCE_1",
            "value": 0.0014092784,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.088s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE3_INT",
            "value": 0.10257,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.026s"
          },
          {
            "name": "RAJA_OpenMP-new_REDUCE3_INT_1",
            "value": 0.0004081248,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.026s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE_STRUCT",
            "value": 0.3502276,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.088s"
          },
          {
            "name": "RAJA_OpenMP-new_REDUCE_STRUCT_1",
            "value": 0.0013977727999999999,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.087s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_TRAP_INT",
            "value": 0.85934,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.215s"
          },
          {
            "name": "RAJA_OpenMP-new_TRAP_INT_1",
            "value": 0.0034354719999999997,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.215s"
          },
          {
            "name": "RAJA_Seq-default_Basic_ARRAY_OF_PTRS",
            "value": 42.64,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 10.660s"
          },
          {
            "name": "RAJA_Seq-default_ARRAY_OF_PTRS_1",
            "value": 0.1705504,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 10.659s"
          },
          {
            "name": "RAJA_Seq-default_Basic_COPY8",
            "value": 39.32212,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 9.831s"
          },
          {
            "name": "RAJA_Seq-default_COPY8_1",
            "value": 0.15727792,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 9.830s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY",
            "value": 4.09112,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 10.228s"
          },
          {
            "name": "RAJA_Seq-default_DAXPY_1",
            "value": 0.001635472,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 2500, total_time: 10.222s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY_ATOMIC",
            "value": 4.09308,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 10.233s"
          },
          {
            "name": "RAJA_Seq-default_DAXPY_ATOMIC_1",
            "value": 0.0016362880000000003,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 2500, total_time: 10.227s"
          },
          {
            "name": "RAJA_Seq-default_Basic_EMPTY",
            "value": 1.32371,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 6.619s"
          },
          {
            "name": "RAJA_Seq-default_EMPTY_1",
            "value": 0.0002646484,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 5000, total_time: 6.616s"
          },
          {
            "name": "RAJA_Seq-default_Basic_IF_QUAD",
            "value": 31.76588888888889,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 28.589s"
          },
          {
            "name": "RAJA_Seq-default_IF_QUAD_1",
            "value": 0.03529234567901234,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 900, total_time: 28.587s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INDEXLIST_3LOOP",
            "value": 28.129,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 14.065s"
          },
          {
            "name": "RAJA_Seq-default_INDEXLIST_3LOOP_1",
            "value": 0.009426799999999999,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 500, total_time: 2.357s"
          },
          {
            "name": "RAJA_Seq-default_INDEXLIST_3LOOP_2",
            "value": 0.040111600000000004,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 500, total_time: 10.028s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT3",
            "value": 11.14344,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 27.859s"
          },
          {
            "name": "RAJA_Seq-default_INIT3_1",
            "value": 0.0044564,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 2500, total_time: 27.852s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D",
            "value": 3.01912,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 37.739s"
          },
          {
            "name": "RAJA_Seq-default_INIT_VIEW1D_1",
            "value": 0.00024138047999999998,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 12500, total_time: 37.716s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 3.2902159999999996,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 41.128s"
          },
          {
            "name": "RAJA_Seq-default_INIT_VIEW1D_OFFSET_1",
            "value": 0.00026307392,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 12500, total_time: 41.105s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MAT_MAT_SHARED",
            "value": 3093.9559999999997,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 77.349s"
          },
          {
            "name": "RAJA_Seq-default_MAT_MAT_SHARED_1",
            "value": 123.75823999999999,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 25, total_time: 77.349s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULADDSUB",
            "value": 11.49737142857143,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 20.120s"
          },
          {
            "name": "RAJA_Seq-default_MULADDSUB_1",
            "value": 0.006568391836734694,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1750, total_time: 20.116s"
          },
          {
            "name": "RAJA_Seq-default_Basic_NESTED_INIT",
            "value": 3.5541,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 17.770s"
          },
          {
            "name": "RAJA_Seq-default_NESTED_INIT_1",
            "value": 0.00071048,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 5000, total_time: 17.762s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_ATOMIC",
            "value": 5.29156,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.323s"
          },
          {
            "name": "RAJA_Seq-default_PI_ATOMIC_1",
            "value": 0.02116432,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 1.323s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_REDUCE",
            "value": 10.55776,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 2.639s"
          },
          {
            "name": "RAJA_Seq-default_PI_REDUCE_1",
            "value": 0.04222928000000001,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 2.639s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE3_INT",
            "value": 2.834988,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.709s"
          },
          {
            "name": "RAJA_Seq-default_REDUCE3_INT_1",
            "value": 0.011336288,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.709s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE_STRUCT",
            "value": 6.64012,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.660s"
          },
          {
            "name": "RAJA_Seq-default_REDUCE_STRUCT_1",
            "value": 0.02655296,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 1.660s"
          },
          {
            "name": "RAJA_Seq-default_Basic_TRAP_INT",
            "value": 13.2874,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.322s"
          },
          {
            "name": "RAJA_Seq-default_TRAP_INT_1",
            "value": 0.05314784,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 3.322s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULTI_REDUCE",
            "value": 10.71396,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 2.678s"
          },
          {
            "name": "RAJA_Seq-default_MULTI_REDUCE_1",
            "value": 0.0428472,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 2.678s"
          },
          {
            "name": "RAJA_Seq-new_Basic_PI_REDUCE",
            "value": 5.3582,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.340s"
          },
          {
            "name": "RAJA_Seq-new_PI_REDUCE_1",
            "value": 0.021430879999999996,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 1.339s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE3_INT",
            "value": 1.474572,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.369s"
          },
          {
            "name": "RAJA_Seq-new_REDUCE3_INT_1",
            "value": 0.00589432,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.368s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE_STRUCT",
            "value": 12.20572,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.051s"
          },
          {
            "name": "RAJA_Seq-new_REDUCE_STRUCT_1",
            "value": 0.048815840000000006,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 3.051s"
          },
          {
            "name": "RAJA_Seq-new_Basic_TRAP_INT",
            "value": 13.3538,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.338s"
          },
          {
            "name": "RAJA_Seq-new_TRAP_INT_1",
            "value": 0.05341328,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 3.338s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "Rich Hornung",
            "username": "rhornung67",
            "email": "hornung1@llnl.gov"
          },
          "committer": {
            "name": "GitHub",
            "username": "web-flow",
            "email": "noreply@github.com"
          },
          "id": "4f9e9a85ff9e5e0ce75c230eed7244b9f305cfb0",
          "message": "Merge pull request #688 from llnl/woptim/rsc-2026-6-0\n\nFrom RAJA: Point at RSC release candidate",
          "timestamp": "2026-08-21T21:34:01Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/4f9e9a85ff9e5e0ce75c230eed7244b9f305cfb0"
        },
        "date": 1787353110842,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_OpenMP-default_Basic_ARRAY_OF_PTRS",
            "value": 4.05408,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.014s"
          },
          {
            "name": "RAJA_OpenMP-default_ARRAY_OF_PTRS_1",
            "value": 0.01620512,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 1.013s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_COPY8",
            "value": 3.57348,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.893s"
          },
          {
            "name": "RAJA_OpenMP-default_COPY8_1",
            "value": 0.014282608,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.893s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY",
            "value": 0.154004,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.385s"
          },
          {
            "name": "RAJA_OpenMP-default_DAXPY_1",
            "value": 0.00006118528,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 2500, total_time: 0.382s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_DAXPY_ATOMIC",
            "value": 1.6034400000000002,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 4.009s"
          },
          {
            "name": "RAJA_OpenMP-default_DAXPY_ATOMIC_1",
            "value": 0.0006409536,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 2500, total_time: 4.006s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_EMPTY",
            "value": 0.049386,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 0.247s"
          },
          {
            "name": "RAJA_OpenMP-default_EMPTY_1",
            "value": 0.00000974492,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 5000, total_time: 0.244s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_IF_QUAD",
            "value": 1.9423333333333332,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 1.748s"
          },
          {
            "name": "RAJA_OpenMP-default_IF_QUAD_1",
            "value": 0.0021559506172839507,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 900, total_time: 1.746s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INDEXLIST_3LOOP",
            "value": 1.643342,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.822s"
          },
          {
            "name": "RAJA_OpenMP-default_INDEXLIST_3LOOP_1",
            "value": 0.00037248559999999997,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 500, total_time: 0.093s"
          },
          {
            "name": "RAJA_OpenMP-default_INDEXLIST_3LOOP_2",
            "value": 0.0025496720000000002,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 500, total_time: 0.637s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT3",
            "value": 0.684964,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 1.712s"
          },
          {
            "name": "RAJA_OpenMP-default_INIT3_1",
            "value": 0.0002731648,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 2500, total_time: 1.707s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D",
            "value": 0.10658799999999999,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 1.332s"
          },
          {
            "name": "RAJA_OpenMP-default_INIT_VIEW1D_1",
            "value": 0.000008457984,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 12500, total_time: 1.322s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.10524480000000001,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 1.316s"
          },
          {
            "name": "RAJA_OpenMP-default_INIT_VIEW1D_OFFSET_1",
            "value": 0.000008351488,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 12500, total_time: 1.305s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MAT_MAT_SHARED",
            "value": 161.798,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 4.045s"
          },
          {
            "name": "RAJA_OpenMP-default_MAT_MAT_SHARED_1",
            "value": 6.471824,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 25, total_time: 4.045s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULADDSUB",
            "value": 0.6910971428571427,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 1.209s"
          },
          {
            "name": "RAJA_OpenMP-default_MULADDSUB_1",
            "value": 0.00039377306122448976,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1750, total_time: 1.206s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_NESTED_INIT",
            "value": 0.1982856,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 0.991s"
          },
          {
            "name": "RAJA_OpenMP-default_NESTED_INIT_1",
            "value": 0.000039521600000000005,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 5000, total_time: 0.988s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_ATOMIC",
            "value": 296.458,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 74.115s"
          },
          {
            "name": "RAJA_OpenMP-default_PI_ATOMIC_1",
            "value": 1.1858208000000001,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 74.114s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_PI_REDUCE",
            "value": 0.3568816,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.089s"
          },
          {
            "name": "RAJA_OpenMP-default_PI_REDUCE_1",
            "value": 0.0014246528,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.089s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE3_INT",
            "value": 0.1167636,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.029s"
          },
          {
            "name": "RAJA_OpenMP-default_REDUCE3_INT_1",
            "value": 0.0004651408,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.029s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_REDUCE_STRUCT",
            "value": 0.3761476,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.094s"
          },
          {
            "name": "RAJA_OpenMP-default_REDUCE_STRUCT_1",
            "value": 0.0015012544,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.094s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_TRAP_INT",
            "value": 0.85274,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.213s"
          },
          {
            "name": "RAJA_OpenMP-default_TRAP_INT_1",
            "value": 0.003408384,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.213s"
          },
          {
            "name": "RAJA_OpenMP-default_Basic_MULTI_REDUCE",
            "value": 0.6940920000000002,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.174s"
          },
          {
            "name": "RAJA_OpenMP-default_MULTI_REDUCE_1",
            "value": 0.002772176,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.173s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_PI_REDUCE",
            "value": 0.353878,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.088s"
          },
          {
            "name": "RAJA_OpenMP-new_PI_REDUCE_1",
            "value": 0.00141368,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.088s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE3_INT",
            "value": 0.1031388,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.026s"
          },
          {
            "name": "RAJA_OpenMP-new_REDUCE3_INT_1",
            "value": 0.000410656,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.026s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_REDUCE_STRUCT",
            "value": 0.35046,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.088s"
          },
          {
            "name": "RAJA_OpenMP-new_REDUCE_STRUCT_1",
            "value": 0.0013983424,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.087s"
          },
          {
            "name": "RAJA_OpenMP-new_Basic_TRAP_INT",
            "value": 0.858244,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.215s"
          },
          {
            "name": "RAJA_OpenMP-new_TRAP_INT_1",
            "value": 0.0034310720000000003,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.214s"
          },
          {
            "name": "RAJA_Seq-default_Basic_ARRAY_OF_PTRS",
            "value": 42.6816,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 10.670s"
          },
          {
            "name": "RAJA_Seq-default_ARRAY_OF_PTRS_1",
            "value": 0.17071680000000003,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 10.670s"
          },
          {
            "name": "RAJA_Seq-default_Basic_COPY8",
            "value": 39.55004,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 9.888s"
          },
          {
            "name": "RAJA_Seq-default_COPY8_1",
            "value": 0.15819023999999998,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 9.887s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY",
            "value": 4.10896,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 10.272s"
          },
          {
            "name": "RAJA_Seq-default_DAXPY_1",
            "value": 0.0016426079999999998,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 2500, total_time: 10.266s"
          },
          {
            "name": "RAJA_Seq-default_Basic_DAXPY_ATOMIC",
            "value": 4.0934800000000005,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 10.234s"
          },
          {
            "name": "RAJA_Seq-default_DAXPY_ATOMIC_1",
            "value": 0.0016364480000000002,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 2500, total_time: 10.228s"
          },
          {
            "name": "RAJA_Seq-default_Basic_EMPTY",
            "value": 1.3233359999999998,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 6.617s"
          },
          {
            "name": "RAJA_Seq-default_EMPTY_1",
            "value": 0.00026457200000000003,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 5000, total_time: 6.614s"
          },
          {
            "name": "RAJA_Seq-default_Basic_IF_QUAD",
            "value": 31.505444444444446,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 1, total_time: 28.355s"
          },
          {
            "name": "RAJA_Seq-default_IF_QUAD_1",
            "value": 0.03500283950617284,
            "unit": "ms/rep",
            "extra": "reps: 900, passes: 900, total_time: 28.352s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INDEXLIST_3LOOP",
            "value": 30.1504,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 15.075s"
          },
          {
            "name": "RAJA_Seq-default_INDEXLIST_3LOOP_1",
            "value": 0.009317439999999998,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 500, total_time: 2.329s"
          },
          {
            "name": "RAJA_Seq-default_INDEXLIST_3LOOP_2",
            "value": 0.044274799999999996,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 500, total_time: 11.069s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT3",
            "value": 10.98448,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 27.461s"
          },
          {
            "name": "RAJA_Seq-default_INIT3_1",
            "value": 0.004392783999999999,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 2500, total_time: 27.455s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D",
            "value": 2.8308720000000003,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 35.386s"
          },
          {
            "name": "RAJA_Seq-default_INIT_VIEW1D_1",
            "value": 0.00022629376000000002,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 12500, total_time: 35.358s"
          },
          {
            "name": "RAJA_Seq-default_Basic_INIT_VIEW1D_OFFSET",
            "value": 2.918232,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 1, total_time: 36.478s"
          },
          {
            "name": "RAJA_Seq-default_INIT_VIEW1D_OFFSET_1",
            "value": 0.000233296,
            "unit": "ms/rep",
            "extra": "reps: 12500, passes: 12500, total_time: 36.453s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MAT_MAT_SHARED",
            "value": 3113.1560000000004,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 1, total_time: 77.829s"
          },
          {
            "name": "RAJA_Seq-default_MAT_MAT_SHARED_1",
            "value": 124.52608000000001,
            "unit": "ms/rep",
            "extra": "reps: 25, passes: 25, total_time: 77.829s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULADDSUB",
            "value": 11.359885714285713,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1, total_time: 19.880s"
          },
          {
            "name": "RAJA_Seq-default_MULADDSUB_1",
            "value": 0.006489828571428571,
            "unit": "ms/rep",
            "extra": "reps: 1750, passes: 1750, total_time: 19.875s"
          },
          {
            "name": "RAJA_Seq-default_Basic_NESTED_INIT",
            "value": 3.4948,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 1, total_time: 17.474s"
          },
          {
            "name": "RAJA_Seq-default_NESTED_INIT_1",
            "value": 0.0006985439999999999,
            "unit": "ms/rep",
            "extra": "reps: 5000, passes: 5000, total_time: 17.464s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_ATOMIC",
            "value": 5.27836,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.320s"
          },
          {
            "name": "RAJA_Seq-default_PI_ATOMIC_1",
            "value": 0.02111136,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 1.319s"
          },
          {
            "name": "RAJA_Seq-default_Basic_PI_REDUCE",
            "value": 10.56392,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 2.641s"
          },
          {
            "name": "RAJA_Seq-default_PI_REDUCE_1",
            "value": 0.04225376,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 2.641s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE3_INT",
            "value": 2.830116,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.708s"
          },
          {
            "name": "RAJA_Seq-default_REDUCE3_INT_1",
            "value": 0.011316719999999999,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.707s"
          },
          {
            "name": "RAJA_Seq-default_Basic_REDUCE_STRUCT",
            "value": 7.05492,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.764s"
          },
          {
            "name": "RAJA_Seq-default_REDUCE_STRUCT_1",
            "value": 0.0282104,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 1.763s"
          },
          {
            "name": "RAJA_Seq-default_Basic_TRAP_INT",
            "value": 13.320440000000001,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.330s"
          },
          {
            "name": "RAJA_Seq-default_TRAP_INT_1",
            "value": 0.053279839999999995,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 3.330s"
          },
          {
            "name": "RAJA_Seq-default_Basic_MULTI_REDUCE",
            "value": 10.8398,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 2.710s"
          },
          {
            "name": "RAJA_Seq-default_MULTI_REDUCE_1",
            "value": 0.04334944,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 2.709s"
          },
          {
            "name": "RAJA_Seq-new_Basic_PI_REDUCE",
            "value": 5.35644,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 1.339s"
          },
          {
            "name": "RAJA_Seq-new_PI_REDUCE_1",
            "value": 0.02142384,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 1.339s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE3_INT",
            "value": 1.45146,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 0.363s"
          },
          {
            "name": "RAJA_Seq-new_REDUCE3_INT_1",
            "value": 0.005801856,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 0.363s"
          },
          {
            "name": "RAJA_Seq-new_Basic_REDUCE_STRUCT",
            "value": 12.26112,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.065s"
          },
          {
            "name": "RAJA_Seq-new_REDUCE_STRUCT_1",
            "value": 0.049035840000000004,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 3.065s"
          },
          {
            "name": "RAJA_Seq-new_Basic_TRAP_INT",
            "value": 13.26312,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 1, total_time: 3.316s"
          },
          {
            "name": "RAJA_Seq-new_TRAP_INT_1",
            "value": 0.05305056,
            "unit": "ms/rep",
            "extra": "reps: 250, passes: 250, total_time: 3.316s"
          }
        ]
      }
    ],
    "rocmcc_6_4_1_hip_openmp_caliper": [
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
        "date": 1774907510413,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_HIP-atomic_direct_256_Basic_MULTI_REDUCE",
            "value": 0.04443339999999999,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-atomic_occgs_256_Basic_MULTI_REDUCE",
            "value": 0.0444318,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_Basic_PI_REDUCE",
            "value": 0.0416594,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_Basic_REDUCE3_INT",
            "value": 0.0654576,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_Basic_REDUCE_STRUCT",
            "value": 0.1994698,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.010s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_Basic_TRAP_INT",
            "value": 0.0561782,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_Basic_PI_REDUCE",
            "value": 0.022694799999999998,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_Basic_REDUCE3_INT",
            "value": 0.026945399999999998,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_Basic_REDUCE_STRUCT",
            "value": 0.0560748,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_Basic_TRAP_INT",
            "value": 0.026608800000000002,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_Basic_PI_REDUCE",
            "value": 0.048041799999999996,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_Basic_REDUCE3_INT",
            "value": 0.0897298,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.004s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_Basic_REDUCE_STRUCT",
            "value": 0.22262,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.011s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_Basic_TRAP_INT",
            "value": 0.053733199999999995,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_Basic_PI_REDUCE",
            "value": 0.494552,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.025s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_Basic_REDUCE3_INT",
            "value": 1.473654,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.074s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_Basic_REDUCE_STRUCT",
            "value": 3.0460599999999998,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.152s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_Basic_TRAP_INT",
            "value": 0.50236,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.025s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_Basic_PI_REDUCE",
            "value": 0.024344800000000003,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_Basic_REDUCE3_INT",
            "value": 0.030678399999999998,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_Basic_REDUCE_STRUCT",
            "value": 0.0626622,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_Basic_TRAP_INT",
            "value": 0.031118,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_Basic_PI_REDUCE",
            "value": 0.060294600000000004,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_Basic_REDUCE3_INT",
            "value": 0.1399124,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.007s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_Basic_REDUCE_STRUCT",
            "value": 0.28489000000000003,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.014s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_Basic_TRAP_INT",
            "value": 0.0612236,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_ARRAY_OF_PTRS",
            "value": 0.356386,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.018s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_COPY8",
            "value": 0.204848,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.010s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_DAXPY",
            "value": 0.0410806,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.021s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_DAXPY_ATOMIC",
            "value": 0.0233596,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.012s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_IF_QUAD",
            "value": 0.06093888888888888,
            "unit": "ms/rep",
            "extra": "reps: 180, passes: 1, total_time: 0.011s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INDEXLIST_3LOOP",
            "value": 0.09009489999999999,
            "unit": "ms/rep",
            "extra": "reps: 100, passes: 1, total_time: 0.009s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INIT3",
            "value": 0.0523,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.026s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INIT_VIEW1D",
            "value": 0.016929559999999996,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.042s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.01721492,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.043s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_MAT_MAT_SHARED",
            "value": 5.36934,
            "unit": "ms/rep",
            "extra": "reps: 5, passes: 1, total_time: 0.027s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_MULADDSUB",
            "value": 0.058063714285714295,
            "unit": "ms/rep",
            "extra": "reps: 350, passes: 1, total_time: 0.020s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_NESTED_INIT",
            "value": 0.0221505,
            "unit": "ms/rep",
            "extra": "reps: 1000, passes: 1, total_time: 0.022s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_PI_ATOMIC",
            "value": 0.7188940000000001,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.036s"
          },
          {
            "name": "RAJA_HIP-direct_256_Basic_EMPTY",
            "value": 0.0103307,
            "unit": "ms/rep",
            "extra": "reps: 1000, passes: 1, total_time: 0.010s"
          },
          {
            "name": "RAJA_HIP-occgs_256_Basic_EMPTY",
            "value": 0.00375666,
            "unit": "ms/rep",
            "extra": "reps: 1000, passes: 1, total_time: 0.004s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "Rich Hornung",
            "username": "rhornung67",
            "email": "hornung1@llnl.gov"
          },
          "committer": {
            "name": "GitHub",
            "username": "web-flow",
            "email": "noreply@github.com"
          },
          "id": "2e4038b7fee213ead90d2108b8458b77c6ac0006",
          "message": "Merge pull request #678 from llnl/main-mergeback\n\nMain mergeback to develop after v2025.12.1 release",
          "timestamp": "2026-03-30T22:11:27Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/2e4038b7fee213ead90d2108b8458b77c6ac0006"
        },
        "date": 1774915430322,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_HIP-atomic_direct_256_Basic_MULTI_REDUCE",
            "value": 0.0444794,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-atomic_occgs_256_Basic_MULTI_REDUCE",
            "value": 0.0425838,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_Basic_PI_REDUCE",
            "value": 0.039581200000000004,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_Basic_REDUCE3_INT",
            "value": 0.0630586,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_Basic_REDUCE_STRUCT",
            "value": 0.191345,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.010s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_Basic_TRAP_INT",
            "value": 0.045785400000000004,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_Basic_PI_REDUCE",
            "value": 0.020745999999999997,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_Basic_REDUCE3_INT",
            "value": 0.025434799999999997,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_Basic_REDUCE_STRUCT",
            "value": 0.054028799999999995,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_Basic_TRAP_INT",
            "value": 0.026987800000000003,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_Basic_PI_REDUCE",
            "value": 0.0451356,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_Basic_REDUCE3_INT",
            "value": 0.08827120000000001,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.004s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_Basic_REDUCE_STRUCT",
            "value": 0.21919,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.011s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_Basic_TRAP_INT",
            "value": 0.0514074,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_Basic_PI_REDUCE",
            "value": 0.493908,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.025s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_Basic_REDUCE3_INT",
            "value": 1.466182,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.073s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_Basic_REDUCE_STRUCT",
            "value": 3.0238400000000003,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.151s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_Basic_TRAP_INT",
            "value": 0.495848,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.025s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_Basic_PI_REDUCE",
            "value": 0.0231026,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_Basic_REDUCE3_INT",
            "value": 0.0294576,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_Basic_REDUCE_STRUCT",
            "value": 0.060384200000000006,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_Basic_TRAP_INT",
            "value": 0.0281954,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_Basic_PI_REDUCE",
            "value": 0.05715920000000001,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_Basic_REDUCE3_INT",
            "value": 0.1402266,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.007s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_Basic_REDUCE_STRUCT",
            "value": 0.28675,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.014s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_Basic_TRAP_INT",
            "value": 0.0594452,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_ARRAY_OF_PTRS",
            "value": 0.34998799999999997,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.017s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_COPY8",
            "value": 0.20481200000000002,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.010s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_DAXPY",
            "value": 0.0413248,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.021s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_DAXPY_ATOMIC",
            "value": 0.023389,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.012s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_IF_QUAD",
            "value": 0.06017333333333333,
            "unit": "ms/rep",
            "extra": "reps: 180, passes: 1, total_time: 0.011s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INDEXLIST_3LOOP",
            "value": 0.09128489999999999,
            "unit": "ms/rep",
            "extra": "reps: 100, passes: 1, total_time: 0.009s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INIT3",
            "value": 0.0525582,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.026s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INIT_VIEW1D",
            "value": 0.016706,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.042s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.017020360000000002,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.043s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_MAT_MAT_SHARED",
            "value": 5.35002,
            "unit": "ms/rep",
            "extra": "reps: 5, passes: 1, total_time: 0.027s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_MULADDSUB",
            "value": 0.05789428571428571,
            "unit": "ms/rep",
            "extra": "reps: 350, passes: 1, total_time: 0.020s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_NESTED_INIT",
            "value": 0.0220572,
            "unit": "ms/rep",
            "extra": "reps: 1000, passes: 1, total_time: 0.022s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_PI_ATOMIC",
            "value": 0.718558,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.036s"
          },
          {
            "name": "RAJA_HIP-direct_256_Basic_EMPTY",
            "value": 0.0103514,
            "unit": "ms/rep",
            "extra": "reps: 1000, passes: 1, total_time: 0.010s"
          },
          {
            "name": "RAJA_HIP-occgs_256_Basic_EMPTY",
            "value": 0.00370184,
            "unit": "ms/rep",
            "extra": "reps: 1000, passes: 1, total_time: 0.004s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "Rich Hornung",
            "username": "rhornung67",
            "email": "hornung1@llnl.gov"
          },
          "committer": {
            "name": "GitHub",
            "username": "web-flow",
            "email": "noreply@github.com"
          },
          "id": "3111bb9edb2a22dcc64c7589d94a080912db06be",
          "message": "Merge pull request #679 from llnl/task/rhornung67/fix-docs\n\nDoc fixes",
          "timestamp": "2026-04-01T15:59:31Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/3111bb9edb2a22dcc64c7589d94a080912db06be"
        },
        "date": 1775069479910,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_HIP-atomic_direct_256_Basic_MULTI_REDUCE",
            "value": 0.045224,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-atomic_occgs_256_Basic_MULTI_REDUCE",
            "value": 0.04318040000000001,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_Basic_PI_REDUCE",
            "value": 0.0410912,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_Basic_REDUCE3_INT",
            "value": 0.0624932,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_Basic_REDUCE_STRUCT",
            "value": 0.200154,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.010s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_Basic_TRAP_INT",
            "value": 0.047251,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_Basic_PI_REDUCE",
            "value": 0.022482199999999997,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_Basic_REDUCE3_INT",
            "value": 0.0263232,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_Basic_REDUCE_STRUCT",
            "value": 0.0561214,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_Basic_TRAP_INT",
            "value": 0.0286814,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_Basic_PI_REDUCE",
            "value": 0.046528400000000004,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_Basic_REDUCE3_INT",
            "value": 0.08797340000000001,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.004s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_Basic_REDUCE_STRUCT",
            "value": 0.215178,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.011s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_Basic_TRAP_INT",
            "value": 0.0521918,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_Basic_PI_REDUCE",
            "value": 0.49075599999999997,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.025s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_Basic_REDUCE3_INT",
            "value": 1.451506,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.073s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_Basic_REDUCE_STRUCT",
            "value": 2.98832,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.149s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_Basic_TRAP_INT",
            "value": 0.49227,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.025s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_Basic_PI_REDUCE",
            "value": 0.0244748,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_Basic_REDUCE3_INT",
            "value": 0.032013,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_Basic_REDUCE_STRUCT",
            "value": 0.062145200000000005,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_Basic_TRAP_INT",
            "value": 0.0308012,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_Basic_PI_REDUCE",
            "value": 0.06079280000000001,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_Basic_REDUCE3_INT",
            "value": 0.143392,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.007s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_Basic_REDUCE_STRUCT",
            "value": 0.28930999999999996,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.014s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_Basic_TRAP_INT",
            "value": 0.0608274,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_ARRAY_OF_PTRS",
            "value": 0.34995600000000004,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.017s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_COPY8",
            "value": 0.203792,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.010s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_DAXPY",
            "value": 0.0397328,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.020s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_DAXPY_ATOMIC",
            "value": 0.0233426,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.012s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_IF_QUAD",
            "value": 0.05977333333333333,
            "unit": "ms/rep",
            "extra": "reps: 180, passes: 1, total_time: 0.011s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INDEXLIST_3LOOP",
            "value": 0.0885782,
            "unit": "ms/rep",
            "extra": "reps: 100, passes: 1, total_time: 0.009s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INIT3",
            "value": 0.0514354,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.026s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INIT_VIEW1D",
            "value": 0.01659616,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.041s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.01694276,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.042s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_MAT_MAT_SHARED",
            "value": 5.2486,
            "unit": "ms/rep",
            "extra": "reps: 5, passes: 1, total_time: 0.026s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_MULADDSUB",
            "value": 0.056211142857142855,
            "unit": "ms/rep",
            "extra": "reps: 350, passes: 1, total_time: 0.020s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_NESTED_INIT",
            "value": 0.0220805,
            "unit": "ms/rep",
            "extra": "reps: 1000, passes: 1, total_time: 0.022s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_PI_ATOMIC",
            "value": 0.7184179999999999,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.036s"
          },
          {
            "name": "RAJA_HIP-direct_256_Basic_EMPTY",
            "value": 0.0103498,
            "unit": "ms/rep",
            "extra": "reps: 1000, passes: 1, total_time: 0.010s"
          },
          {
            "name": "RAJA_HIP-occgs_256_Basic_EMPTY",
            "value": 0.00375516,
            "unit": "ms/rep",
            "extra": "reps: 1000, passes: 1, total_time: 0.004s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "Adrien Bernede",
            "username": "adrienbernede",
            "email": "51493078+adrienbernede@users.noreply.github.com"
          },
          "committer": {
            "name": "GitHub",
            "username": "web-flow",
            "email": "noreply@github.com"
          },
          "id": "95c360ddf585410e6123956c72a2c648111ece29",
          "message": "Merge pull request #652 from llnl/woptim/rocm7",
          "timestamp": "2026-04-16T19:31:14Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/95c360ddf585410e6123956c72a2c648111ece29"
        },
        "date": 1776372817929,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_HIP-atomic_direct_256_Basic_MULTI_REDUCE",
            "value": 0.0451732,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-atomic_occgs_256_Basic_MULTI_REDUCE",
            "value": 0.044426200000000006,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_Basic_PI_REDUCE",
            "value": 0.041711,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_Basic_REDUCE3_INT",
            "value": 0.06350040000000001,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_Basic_REDUCE_STRUCT",
            "value": 0.1968252,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.010s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_Basic_TRAP_INT",
            "value": 0.0459954,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_Basic_PI_REDUCE",
            "value": 0.0222222,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_Basic_REDUCE3_INT",
            "value": 0.0258448,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_Basic_REDUCE_STRUCT",
            "value": 0.0555386,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_Basic_TRAP_INT",
            "value": 0.0266424,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_Basic_PI_REDUCE",
            "value": 0.0480678,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_Basic_REDUCE3_INT",
            "value": 0.0862314,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.004s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_Basic_REDUCE_STRUCT",
            "value": 0.220018,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.011s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_Basic_TRAP_INT",
            "value": 0.05096640000000001,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_Basic_PI_REDUCE",
            "value": 0.49056599999999995,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.025s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_Basic_REDUCE3_INT",
            "value": 1.449886,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.072s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_Basic_REDUCE_STRUCT",
            "value": 3.0013,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.150s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_Basic_TRAP_INT",
            "value": 0.49227600000000005,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.025s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_Basic_PI_REDUCE",
            "value": 0.024865400000000003,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_Basic_REDUCE3_INT",
            "value": 0.030318799999999996,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_Basic_REDUCE_STRUCT",
            "value": 0.0616322,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_Basic_TRAP_INT",
            "value": 0.030027399999999996,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_Basic_PI_REDUCE",
            "value": 0.0589876,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_Basic_REDUCE3_INT",
            "value": 0.14265419999999998,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.007s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_Basic_REDUCE_STRUCT",
            "value": 0.285514,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.014s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_Basic_TRAP_INT",
            "value": 0.060861799999999994,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_ARRAY_OF_PTRS",
            "value": 0.35403599999999996,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.018s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_COPY8",
            "value": 0.20398,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.010s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_DAXPY",
            "value": 0.04453639999999999,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.022s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_DAXPY_ATOMIC",
            "value": 0.023530799999999998,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.012s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_IF_QUAD",
            "value": 0.06062722222222223,
            "unit": "ms/rep",
            "extra": "reps: 180, passes: 1, total_time: 0.011s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INDEXLIST_3LOOP",
            "value": 0.0892992,
            "unit": "ms/rep",
            "extra": "reps: 100, passes: 1, total_time: 0.009s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INIT3",
            "value": 0.0522694,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.026s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INIT_VIEW1D",
            "value": 0.01695176,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.042s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.017238919999999998,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.043s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_MAT_MAT_SHARED",
            "value": 5.36614,
            "unit": "ms/rep",
            "extra": "reps: 5, passes: 1, total_time: 0.027s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_MULADDSUB",
            "value": 0.05721657142857143,
            "unit": "ms/rep",
            "extra": "reps: 350, passes: 1, total_time: 0.020s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_NESTED_INIT",
            "value": 0.0221221,
            "unit": "ms/rep",
            "extra": "reps: 1000, passes: 1, total_time: 0.022s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_PI_ATOMIC",
            "value": 0.718796,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.036s"
          },
          {
            "name": "RAJA_HIP-direct_256_Basic_EMPTY",
            "value": 0.010319,
            "unit": "ms/rep",
            "extra": "reps: 1000, passes: 1, total_time: 0.010s"
          },
          {
            "name": "RAJA_HIP-occgs_256_Basic_EMPTY",
            "value": 0.00368065,
            "unit": "ms/rep",
            "extra": "reps: 1000, passes: 1, total_time: 0.004s"
          }
        ]
      }
    ],
    "rocmcc_6_4_3_hip_openmp_caliper": [
      {
        "commit": {
          "author": {
            "name": "Adrien Bernede",
            "username": "adrienbernede",
            "email": "51493078+adrienbernede@users.noreply.github.com"
          },
          "committer": {
            "name": "GitHub",
            "username": "web-flow",
            "email": "noreply@github.com"
          },
          "id": "95c360ddf585410e6123956c72a2c648111ece29",
          "message": "Merge pull request #652 from llnl/woptim/rocm7",
          "timestamp": "2026-04-16T19:31:14Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/95c360ddf585410e6123956c72a2c648111ece29"
        },
        "date": 1776372836665,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_HIP-atomic_direct_256_Basic_MULTI_REDUCE",
            "value": 0.043518999999999995,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-atomic_occgs_256_Basic_MULTI_REDUCE",
            "value": 0.042409999999999996,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_Basic_PI_REDUCE",
            "value": 0.036599,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_Basic_REDUCE3_INT",
            "value": 0.0619014,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_Basic_REDUCE_STRUCT",
            "value": 0.1934884,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.010s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_Basic_TRAP_INT",
            "value": 0.0487958,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_Basic_PI_REDUCE",
            "value": 0.01980854,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_Basic_REDUCE3_INT",
            "value": 0.0244422,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_Basic_REDUCE_STRUCT",
            "value": 0.055265800000000004,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_Basic_TRAP_INT",
            "value": 0.023941000000000004,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_Basic_PI_REDUCE",
            "value": 0.043185,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_Basic_REDUCE3_INT",
            "value": 0.0844666,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.004s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_Basic_REDUCE_STRUCT",
            "value": 0.21980800000000003,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.011s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_Basic_TRAP_INT",
            "value": 0.047137200000000004,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_Basic_PI_REDUCE",
            "value": 0.48950399999999994,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.024s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_Basic_REDUCE3_INT",
            "value": 1.4666640000000002,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.073s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_Basic_REDUCE_STRUCT",
            "value": 3.0471799999999996,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.152s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_Basic_TRAP_INT",
            "value": 0.49285,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.025s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_Basic_PI_REDUCE",
            "value": 0.01999294,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_Basic_REDUCE3_INT",
            "value": 0.026207,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_Basic_REDUCE_STRUCT",
            "value": 0.058242199999999994,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_Basic_TRAP_INT",
            "value": 0.0238984,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_Basic_PI_REDUCE",
            "value": 0.0545982,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_Basic_REDUCE3_INT",
            "value": 0.13592500000000002,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.007s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_Basic_REDUCE_STRUCT",
            "value": 0.28012,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.014s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_Basic_TRAP_INT",
            "value": 0.0559196,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_ARRAY_OF_PTRS",
            "value": 0.355264,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.018s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_COPY8",
            "value": 0.20665599999999998,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.010s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_DAXPY",
            "value": 0.0416212,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.021s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_DAXPY_ATOMIC",
            "value": 0.0234114,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.012s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_IF_QUAD",
            "value": 0.06038055555555555,
            "unit": "ms/rep",
            "extra": "reps: 180, passes: 1, total_time: 0.011s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INDEXLIST_3LOOP",
            "value": 0.09860489999999998,
            "unit": "ms/rep",
            "extra": "reps: 100, passes: 1, total_time: 0.010s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INIT3",
            "value": 0.0521092,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.026s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INIT_VIEW1D",
            "value": 0.017057120000000002,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.043s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.0172456,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.043s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_MAT_MAT_SHARED",
            "value": 5.431520000000001,
            "unit": "ms/rep",
            "extra": "reps: 5, passes: 1, total_time: 0.027s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_MULADDSUB",
            "value": 0.05898114285714286,
            "unit": "ms/rep",
            "extra": "reps: 350, passes: 1, total_time: 0.021s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_NESTED_INIT",
            "value": 0.0228857,
            "unit": "ms/rep",
            "extra": "reps: 1000, passes: 1, total_time: 0.023s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_PI_ATOMIC",
            "value": 0.7165,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.036s"
          },
          {
            "name": "RAJA_HIP-direct_256_Basic_EMPTY",
            "value": 0.0103519,
            "unit": "ms/rep",
            "extra": "reps: 1000, passes: 1, total_time: 0.010s"
          },
          {
            "name": "RAJA_HIP-occgs_256_Basic_EMPTY",
            "value": 0.00368519,
            "unit": "ms/rep",
            "extra": "reps: 1000, passes: 1, total_time: 0.004s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "Jason Burmark",
            "username": "MrBurmark",
            "email": "MrBurmark@users.noreply.github.com"
          },
          "committer": {
            "name": "GitHub",
            "username": "web-flow",
            "email": "noreply@github.com"
          },
          "id": "9bc67910045283981831de42fe96bd151eee0761",
          "message": "Merge pull request #681 from llnl/pr-from-fork/680\n\nPr from fork/680",
          "timestamp": "2026-04-20T16:10:32Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/9bc67910045283981831de42fe96bd151eee0761"
        },
        "date": 1776704789189,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_HIP-atomic_direct_256_Basic_MULTI_REDUCE",
            "value": 0.0434136,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-atomic_occgs_256_Basic_MULTI_REDUCE",
            "value": 0.042463,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_Basic_PI_REDUCE",
            "value": 0.0378166,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_Basic_REDUCE3_INT",
            "value": 0.0618176,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_Basic_REDUCE_STRUCT",
            "value": 0.1941844,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.010s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_Basic_TRAP_INT",
            "value": 0.0424302,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_Basic_PI_REDUCE",
            "value": 0.01976132,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_Basic_REDUCE3_INT",
            "value": 0.0242276,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_Basic_REDUCE_STRUCT",
            "value": 0.05591480000000001,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_Basic_TRAP_INT",
            "value": 0.023746399999999997,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_Basic_PI_REDUCE",
            "value": 0.043013199999999994,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_Basic_REDUCE3_INT",
            "value": 0.0865458,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.004s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_Basic_REDUCE_STRUCT",
            "value": 0.22052200000000002,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.011s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_Basic_TRAP_INT",
            "value": 0.047278799999999996,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_Basic_PI_REDUCE",
            "value": 0.48899000000000004,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.024s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_Basic_REDUCE3_INT",
            "value": 1.457184,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.073s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_Basic_REDUCE_STRUCT",
            "value": 3.02656,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.151s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_Basic_TRAP_INT",
            "value": 0.490726,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.025s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_Basic_PI_REDUCE",
            "value": 0.0206466,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_Basic_REDUCE3_INT",
            "value": 0.0268154,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_Basic_REDUCE_STRUCT",
            "value": 0.05823980000000001,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_Basic_TRAP_INT",
            "value": 0.0247764,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_Basic_PI_REDUCE",
            "value": 0.0560494,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_Basic_REDUCE3_INT",
            "value": 0.138493,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.007s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_Basic_REDUCE_STRUCT",
            "value": 0.28933,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.014s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_Basic_TRAP_INT",
            "value": 0.056494199999999994,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_ARRAY_OF_PTRS",
            "value": 0.35969,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.018s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_COPY8",
            "value": 0.20538,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.010s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_DAXPY",
            "value": 0.0412532,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.021s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_DAXPY_ATOMIC",
            "value": 0.0236914,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.012s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_IF_QUAD",
            "value": 0.06028777777777778,
            "unit": "ms/rep",
            "extra": "reps: 180, passes: 1, total_time: 0.011s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INDEXLIST_3LOOP",
            "value": 0.0972804,
            "unit": "ms/rep",
            "extra": "reps: 100, passes: 1, total_time: 0.010s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INIT3",
            "value": 0.052297,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.026s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INIT_VIEW1D",
            "value": 0.01699172,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.042s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.017252840000000002,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.043s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_MAT_MAT_SHARED",
            "value": 5.41484,
            "unit": "ms/rep",
            "extra": "reps: 5, passes: 1, total_time: 0.027s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_MULADDSUB",
            "value": 0.05880428571428571,
            "unit": "ms/rep",
            "extra": "reps: 350, passes: 1, total_time: 0.021s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_NESTED_INIT",
            "value": 0.0221932,
            "unit": "ms/rep",
            "extra": "reps: 1000, passes: 1, total_time: 0.022s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_PI_ATOMIC",
            "value": 0.7163240000000001,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.036s"
          },
          {
            "name": "RAJA_HIP-direct_256_Basic_EMPTY",
            "value": 0.0103585,
            "unit": "ms/rep",
            "extra": "reps: 1000, passes: 1, total_time: 0.010s"
          },
          {
            "name": "RAJA_HIP-occgs_256_Basic_EMPTY",
            "value": 0.00366559,
            "unit": "ms/rep",
            "extra": "reps: 1000, passes: 1, total_time: 0.004s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "Jason Burmark",
            "username": "MrBurmark",
            "email": "MrBurmark@users.noreply.github.com"
          },
          "committer": {
            "name": "GitHub",
            "username": "web-flow",
            "email": "noreply@github.com"
          },
          "id": "9bc67910045283981831de42fe96bd151eee0761",
          "message": "Merge pull request #681 from llnl/pr-from-fork/680\n\nPr from fork/680",
          "timestamp": "2026-04-20T16:10:32Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/9bc67910045283981831de42fe96bd151eee0761"
        },
        "date": 1779383179874,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_HIP-atomic_direct_256_Basic_MULTI_REDUCE",
            "value": 0.043098,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-atomic_occgs_256_Basic_MULTI_REDUCE",
            "value": 0.041663200000000004,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_Basic_PI_REDUCE",
            "value": 0.0375902,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_Basic_REDUCE3_INT",
            "value": 0.061478200000000004,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_Basic_REDUCE_STRUCT",
            "value": 0.1991324,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.010s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_Basic_TRAP_INT",
            "value": 0.0430794,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_Basic_PI_REDUCE",
            "value": 0.019798519999999997,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_Basic_REDUCE3_INT",
            "value": 0.0250768,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_Basic_REDUCE_STRUCT",
            "value": 0.057047799999999996,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_Basic_TRAP_INT",
            "value": 0.023857399999999997,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_Basic_PI_REDUCE",
            "value": 0.0430588,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_Basic_REDUCE3_INT",
            "value": 0.08576699999999998,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.004s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_Basic_REDUCE_STRUCT",
            "value": 0.22112600000000002,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.011s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_Basic_TRAP_INT",
            "value": 0.0472504,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_Basic_PI_REDUCE",
            "value": 0.488842,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.024s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_Basic_REDUCE3_INT",
            "value": 1.456156,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.073s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_Basic_REDUCE_STRUCT",
            "value": 3.03082,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.152s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_Basic_TRAP_INT",
            "value": 0.4914,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.025s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_Basic_PI_REDUCE",
            "value": 0.0201342,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_Basic_REDUCE3_INT",
            "value": 0.0265928,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_Basic_REDUCE_STRUCT",
            "value": 0.0586724,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_Basic_TRAP_INT",
            "value": 0.024703199999999995,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_Basic_PI_REDUCE",
            "value": 0.05518239999999999,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_Basic_REDUCE3_INT",
            "value": 0.1360458,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.007s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_Basic_REDUCE_STRUCT",
            "value": 0.28717,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.014s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_Basic_TRAP_INT",
            "value": 0.056776799999999995,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_ARRAY_OF_PTRS",
            "value": 0.35881399999999997,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.018s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_COPY8",
            "value": 0.20671599999999998,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.010s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_DAXPY",
            "value": 0.0421402,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.021s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_DAXPY_ATOMIC",
            "value": 0.0234264,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.012s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_IF_QUAD",
            "value": 0.06109166666666666,
            "unit": "ms/rep",
            "extra": "reps: 180, passes: 1, total_time: 0.011s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INDEXLIST_3LOOP",
            "value": 0.09731899999999999,
            "unit": "ms/rep",
            "extra": "reps: 100, passes: 1, total_time: 0.010s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INIT3",
            "value": 0.052377,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.026s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INIT_VIEW1D",
            "value": 0.017025799999999997,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.043s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.01729808,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.043s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_MAT_MAT_SHARED",
            "value": 5.44612,
            "unit": "ms/rep",
            "extra": "reps: 5, passes: 1, total_time: 0.027s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_MULADDSUB",
            "value": 0.059728857142857146,
            "unit": "ms/rep",
            "extra": "reps: 350, passes: 1, total_time: 0.021s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_NESTED_INIT",
            "value": 0.0221678,
            "unit": "ms/rep",
            "extra": "reps: 1000, passes: 1, total_time: 0.022s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_PI_ATOMIC",
            "value": 0.71677,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.036s"
          },
          {
            "name": "RAJA_HIP-direct_256_Basic_EMPTY",
            "value": 0.010362600000000001,
            "unit": "ms/rep",
            "extra": "reps: 1000, passes: 1, total_time: 0.010s"
          },
          {
            "name": "RAJA_HIP-occgs_256_Basic_EMPTY",
            "value": 0.0037215,
            "unit": "ms/rep",
            "extra": "reps: 1000, passes: 1, total_time: 0.004s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "Adrien Bernede",
            "username": "adrienbernede",
            "email": "51493078+adrienbernede@users.noreply.github.com"
          },
          "committer": {
            "name": "GitHub",
            "username": "web-flow",
            "email": "noreply@github.com"
          },
          "id": "1f660e77acdb6f12df7a4e39a25172328afb7352",
          "message": "Merge pull request #645 from llnl/woptim/ci-logs-and-errors\n\nImprove CI jobs logs and error handling",
          "timestamp": "2026-06-16T16:08:18Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/1f660e77acdb6f12df7a4e39a25172328afb7352"
        },
        "date": 1781641887987,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_HIP-atomic_direct_256_Basic_MULTI_REDUCE",
            "value": 0.0432236,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-atomic_occgs_256_Basic_MULTI_REDUCE",
            "value": 0.0414196,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_Basic_PI_REDUCE",
            "value": 0.036368199999999996,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_Basic_REDUCE3_INT",
            "value": 0.061192199999999995,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_Basic_REDUCE_STRUCT",
            "value": 0.1946512,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.010s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_Basic_TRAP_INT",
            "value": 0.0473098,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_Basic_PI_REDUCE",
            "value": 0.01880496,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_Basic_REDUCE3_INT",
            "value": 0.023365199999999996,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_Basic_REDUCE_STRUCT",
            "value": 0.0541076,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_Basic_TRAP_INT",
            "value": 0.023422600000000002,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_Basic_PI_REDUCE",
            "value": 0.042768600000000004,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_Basic_REDUCE3_INT",
            "value": 0.08519080000000001,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.004s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_Basic_REDUCE_STRUCT",
            "value": 0.215786,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.011s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_Basic_TRAP_INT",
            "value": 0.0468938,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_Basic_PI_REDUCE",
            "value": 0.487166,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.024s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_Basic_REDUCE3_INT",
            "value": 1.4617060000000002,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.073s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_Basic_REDUCE_STRUCT",
            "value": 3.2437799999999997,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.162s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_Basic_TRAP_INT",
            "value": 0.490332,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.025s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_Basic_PI_REDUCE",
            "value": 0.01990116,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_Basic_REDUCE3_INT",
            "value": 0.0259412,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_Basic_REDUCE_STRUCT",
            "value": 0.05771540000000001,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_Basic_TRAP_INT",
            "value": 0.0239138,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_Basic_PI_REDUCE",
            "value": 0.0550884,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_Basic_REDUCE3_INT",
            "value": 0.1384586,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.007s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_Basic_REDUCE_STRUCT",
            "value": 0.29255,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.015s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_Basic_TRAP_INT",
            "value": 0.05590280000000001,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_ARRAY_OF_PTRS",
            "value": 0.34300800000000004,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.017s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_COPY8",
            "value": 0.20484,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.010s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_DAXPY",
            "value": 0.0401664,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.020s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_DAXPY_ATOMIC",
            "value": 0.023513,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.012s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_IF_QUAD",
            "value": 0.05991,
            "unit": "ms/rep",
            "extra": "reps: 180, passes: 1, total_time: 0.011s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INDEXLIST_3LOOP",
            "value": 0.10421899999999999,
            "unit": "ms/rep",
            "extra": "reps: 100, passes: 1, total_time: 0.010s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INIT3",
            "value": 0.051267,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.026s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INIT_VIEW1D",
            "value": 0.016492760000000002,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.041s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.01679428,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.042s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_MAT_MAT_SHARED",
            "value": 5.30012,
            "unit": "ms/rep",
            "extra": "reps: 5, passes: 1, total_time: 0.027s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_MULADDSUB",
            "value": 0.05650800000000001,
            "unit": "ms/rep",
            "extra": "reps: 350, passes: 1, total_time: 0.020s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_NESTED_INIT",
            "value": 0.0220029,
            "unit": "ms/rep",
            "extra": "reps: 1000, passes: 1, total_time: 0.022s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_PI_ATOMIC",
            "value": 0.7160920000000001,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.036s"
          },
          {
            "name": "RAJA_HIP-direct_256_Basic_EMPTY",
            "value": 0.0103419,
            "unit": "ms/rep",
            "extra": "reps: 1000, passes: 1, total_time: 0.010s"
          },
          {
            "name": "RAJA_HIP-occgs_256_Basic_EMPTY",
            "value": 0.00380994,
            "unit": "ms/rep",
            "extra": "reps: 1000, passes: 1, total_time: 0.004s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "Jason Burmark",
            "username": "MrBurmark",
            "email": "MrBurmark@users.noreply.github.com"
          },
          "committer": {
            "name": "GitHub",
            "username": "web-flow",
            "email": "noreply@github.com"
          },
          "id": "3d72e32abcb569a20f48069f2b639663f54ca04f",
          "message": "Merge pull request #690 from llnl/bugfix/burmark1/flop_counts\n\nFix flop counts in MASS3DEA and DIFFUSION3DPA",
          "timestamp": "2026-06-22T15:32:25Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/3d72e32abcb569a20f48069f2b639663f54ca04f"
        },
        "date": 1782145988166,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_HIP-atomic_direct_256_Basic_MULTI_REDUCE",
            "value": 0.04246899999999999,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-atomic_occgs_256_Basic_MULTI_REDUCE",
            "value": 0.041339600000000004,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_Basic_PI_REDUCE",
            "value": 0.03616,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_Basic_REDUCE3_INT",
            "value": 0.060747,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_Basic_REDUCE_STRUCT",
            "value": 0.200352,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.010s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_Basic_TRAP_INT",
            "value": 0.0424878,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_Basic_PI_REDUCE",
            "value": 0.018565960000000003,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_Basic_REDUCE3_INT",
            "value": 0.023481600000000002,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_Basic_REDUCE_STRUCT",
            "value": 0.0562366,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_Basic_TRAP_INT",
            "value": 0.023371199999999995,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_Basic_PI_REDUCE",
            "value": 0.0435044,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_Basic_REDUCE3_INT",
            "value": 0.0846532,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.004s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_Basic_REDUCE_STRUCT",
            "value": 0.21630400000000002,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.011s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_Basic_TRAP_INT",
            "value": 0.046266800000000004,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_Basic_PI_REDUCE",
            "value": 0.48814599999999997,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.024s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_Basic_REDUCE3_INT",
            "value": 1.443348,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.072s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_Basic_REDUCE_STRUCT",
            "value": 2.99244,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.150s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_Basic_TRAP_INT",
            "value": 0.489388,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.024s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_Basic_PI_REDUCE",
            "value": 0.0202692,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_Basic_REDUCE3_INT",
            "value": 0.0265896,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_Basic_REDUCE_STRUCT",
            "value": 0.0585372,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_Basic_TRAP_INT",
            "value": 0.0243786,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_Basic_PI_REDUCE",
            "value": 0.055364000000000003,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_Basic_REDUCE3_INT",
            "value": 0.138487,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.007s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_Basic_REDUCE_STRUCT",
            "value": 0.288734,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.014s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_Basic_TRAP_INT",
            "value": 0.05632759999999999,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_ARRAY_OF_PTRS",
            "value": 0.34542,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.017s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_COPY8",
            "value": 0.206274,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.010s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_DAXPY",
            "value": 0.0412292,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.021s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_DAXPY_ATOMIC",
            "value": 0.0234442,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.012s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_IF_QUAD",
            "value": 0.05976833333333333,
            "unit": "ms/rep",
            "extra": "reps: 180, passes: 1, total_time: 0.011s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INDEXLIST_3LOOP",
            "value": 0.0973542,
            "unit": "ms/rep",
            "extra": "reps: 100, passes: 1, total_time: 0.010s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INIT3",
            "value": 0.0522156,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.026s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INIT_VIEW1D",
            "value": 0.01654744,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.041s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.01698056,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.042s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_MAT_MAT_SHARED",
            "value": 5.3337,
            "unit": "ms/rep",
            "extra": "reps: 5, passes: 1, total_time: 0.027s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_MULADDSUB",
            "value": 0.05678571428571429,
            "unit": "ms/rep",
            "extra": "reps: 350, passes: 1, total_time: 0.020s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_NESTED_INIT",
            "value": 0.0219855,
            "unit": "ms/rep",
            "extra": "reps: 1000, passes: 1, total_time: 0.022s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_PI_ATOMIC",
            "value": 0.7164439999999999,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.036s"
          },
          {
            "name": "RAJA_HIP-direct_256_Basic_EMPTY",
            "value": 0.0103379,
            "unit": "ms/rep",
            "extra": "reps: 1000, passes: 1, total_time: 0.010s"
          },
          {
            "name": "RAJA_HIP-occgs_256_Basic_EMPTY",
            "value": 0.00345093,
            "unit": "ms/rep",
            "extra": "reps: 1000, passes: 1, total_time: 0.003s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "Jason Burmark",
            "username": "MrBurmark",
            "email": "MrBurmark@users.noreply.github.com"
          },
          "committer": {
            "name": "GitHub",
            "username": "web-flow",
            "email": "noreply@github.com"
          },
          "id": "1d167298cf1ca50f6717c31f54fac8a34527c3d0",
          "message": "Merge pull request #700 from llnl/jacobi-2d-launches\n\nJacobi 2D Seq, OMP, and OMPTarget launch twice",
          "timestamp": "2026-08-06T01:57:56Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/1d167298cf1ca50f6717c31f54fac8a34527c3d0"
        },
        "date": 1785985312637,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_HIP-atomic_direct_256_Basic_MULTI_REDUCE",
            "value": 0.04280080000000001,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-atomic_occgs_256_Basic_MULTI_REDUCE",
            "value": 0.04200480000000001,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_Basic_PI_REDUCE",
            "value": 0.0355904,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_Basic_REDUCE3_INT",
            "value": 0.06067600000000001,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_Basic_REDUCE_STRUCT",
            "value": 0.19458099999999998,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.010s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_Basic_TRAP_INT",
            "value": 0.047158,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_Basic_PI_REDUCE",
            "value": 0.01902938,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_Basic_REDUCE3_INT",
            "value": 0.0236138,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_Basic_REDUCE_STRUCT",
            "value": 0.0557206,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_Basic_TRAP_INT",
            "value": 0.023365,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_Basic_PI_REDUCE",
            "value": 0.0429984,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_Basic_REDUCE3_INT",
            "value": 0.08555240000000001,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.004s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_Basic_REDUCE_STRUCT",
            "value": 0.217576,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.011s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_Basic_TRAP_INT",
            "value": 0.046754,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_Basic_PI_REDUCE",
            "value": 0.48469999999999996,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.024s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_Basic_REDUCE3_INT",
            "value": 1.454174,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.073s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_Basic_REDUCE_STRUCT",
            "value": 3.0164000000000004,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.151s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_Basic_TRAP_INT",
            "value": 0.486978,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.024s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_Basic_PI_REDUCE",
            "value": 0.019945380000000002,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_Basic_REDUCE3_INT",
            "value": 0.0263984,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_Basic_REDUCE_STRUCT",
            "value": 0.0582086,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_Basic_TRAP_INT",
            "value": 0.0245668,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_Basic_PI_REDUCE",
            "value": 0.0546082,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_Basic_REDUCE3_INT",
            "value": 0.1357614,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.007s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_Basic_REDUCE_STRUCT",
            "value": 0.279532,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.014s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_Basic_TRAP_INT",
            "value": 0.0557024,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_ARRAY_OF_PTRS",
            "value": 0.35262,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.018s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_COPY8",
            "value": 0.208548,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.010s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_DAXPY",
            "value": 0.0426206,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.021s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_DAXPY_ATOMIC",
            "value": 0.0235622,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.012s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_IF_QUAD",
            "value": 0.06000833333333334,
            "unit": "ms/rep",
            "extra": "reps: 180, passes: 1, total_time: 0.011s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INDEXLIST_3LOOP",
            "value": 0.09628980000000001,
            "unit": "ms/rep",
            "extra": "reps: 100, passes: 1, total_time: 0.010s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INIT3",
            "value": 0.0514564,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.026s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INIT_VIEW1D",
            "value": 0.016543279999999997,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.041s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.01692708,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.042s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_MAT_MAT_SHARED",
            "value": 5.31388,
            "unit": "ms/rep",
            "extra": "reps: 5, passes: 1, total_time: 0.027s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_MULADDSUB",
            "value": 0.056816571428571425,
            "unit": "ms/rep",
            "extra": "reps: 350, passes: 1, total_time: 0.020s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_NESTED_INIT",
            "value": 0.021984,
            "unit": "ms/rep",
            "extra": "reps: 1000, passes: 1, total_time: 0.022s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_PI_ATOMIC",
            "value": 0.7165520000000001,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.036s"
          },
          {
            "name": "RAJA_HIP-direct_256_Basic_EMPTY",
            "value": 0.0103676,
            "unit": "ms/rep",
            "extra": "reps: 1000, passes: 1, total_time: 0.010s"
          },
          {
            "name": "RAJA_HIP-occgs_256_Basic_EMPTY",
            "value": 0.00394165,
            "unit": "ms/rep",
            "extra": "reps: 1000, passes: 1, total_time: 0.004s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "Jason Burmark",
            "username": "MrBurmark",
            "email": "MrBurmark@users.noreply.github.com"
          },
          "committer": {
            "name": "GitHub",
            "username": "web-flow",
            "email": "noreply@github.com"
          },
          "id": "338cfeb7266607648bd06815ba103f61e357c2e5",
          "message": "Merge pull request #695 from llnl/multi-kernel-regions\n\nCaliper: Annotate kernels separately",
          "timestamp": "2026-08-10T20:40:11Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/338cfeb7266607648bd06815ba103f61e357c2e5"
        },
        "date": 1786408925508,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_HIP-atomic_direct_256_Basic_MULTI_REDUCE",
            "value": 0.043670400000000005,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-atomic_direct_256_MULTI_REDUCE_1",
            "value": 0.0008619200000000001,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-atomic_occgs_256_Basic_MULTI_REDUCE",
            "value": 0.0433078,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-atomic_occgs_256_MULTI_REDUCE_1",
            "value": 0.000854684,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_Basic_PI_REDUCE",
            "value": 0.0383528,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_PI_REDUCE_1",
            "value": 0.00075742,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_Basic_REDUCE3_INT",
            "value": 0.0623442,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_REDUCE3_INT_1",
            "value": 0.001234816,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_Basic_REDUCE_STRUCT",
            "value": 0.1961328,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.010s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_REDUCE_STRUCT_1",
            "value": 0.003907316,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.010s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_Basic_TRAP_INT",
            "value": 0.0477774,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_TRAP_INT_1",
            "value": 0.0009447799999999999,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_Basic_PI_REDUCE",
            "value": 0.020717600000000003,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_PI_REDUCE_1",
            "value": 0.000405,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_Basic_REDUCE3_INT",
            "value": 0.025255999999999997,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_REDUCE3_INT_1",
            "value": 0.000493828,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_Basic_REDUCE_STRUCT",
            "value": 0.0575334,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_REDUCE_STRUCT_1",
            "value": 0.0011335,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_Basic_TRAP_INT",
            "value": 0.025211,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_TRAP_INT_1",
            "value": 0.000494252,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_Basic_PI_REDUCE",
            "value": 0.0431654,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_PI_REDUCE_1",
            "value": 0.00085448,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_Basic_REDUCE3_INT",
            "value": 0.084382,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.004s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_REDUCE3_INT_1",
            "value": 0.00167178,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.004s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_Basic_REDUCE_STRUCT",
            "value": 0.215384,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.011s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_REDUCE_STRUCT_1",
            "value": 0.00429148,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.011s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_Basic_TRAP_INT",
            "value": 0.047492799999999995,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_TRAP_INT_1",
            "value": 0.0009396039999999999,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_Basic_PI_REDUCE",
            "value": 0.485946,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.024s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_PI_REDUCE_1",
            "value": 0.0097084,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.024s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_Basic_REDUCE3_INT",
            "value": 1.433616,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.072s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_REDUCE3_INT_1",
            "value": 0.0286584,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.072s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_Basic_REDUCE_STRUCT",
            "value": 2.9761599999999997,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.149s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_REDUCE_STRUCT_1",
            "value": 0.05950640000000001,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.149s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_Basic_TRAP_INT",
            "value": 0.48831399999999997,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.024s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_TRAP_INT_1",
            "value": 0.009755360000000001,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.024s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_Basic_PI_REDUCE",
            "value": 0.0212206,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_PI_REDUCE_1",
            "value": 0.00041561200000000005,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_Basic_REDUCE3_INT",
            "value": 0.02734,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_REDUCE3_INT_1",
            "value": 0.0005355440000000001,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_Basic_REDUCE_STRUCT",
            "value": 0.05937880000000001,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_REDUCE_STRUCT_1",
            "value": 0.001172128,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_Basic_TRAP_INT",
            "value": 0.0251006,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_TRAP_INT_1",
            "value": 0.000491872,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_Basic_PI_REDUCE",
            "value": 0.056056600000000005,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_PI_REDUCE_1",
            "value": 0.001112864,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_Basic_REDUCE3_INT",
            "value": 0.137342,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.007s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_REDUCE3_INT_1",
            "value": 0.002736176,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.007s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_Basic_REDUCE_STRUCT",
            "value": 0.287774,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.014s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_REDUCE_STRUCT_1",
            "value": 0.00573992,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.014s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_Basic_TRAP_INT",
            "value": 0.0571104,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_TRAP_INT_1",
            "value": 0.0011333159999999999,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_ARRAY_OF_PTRS",
            "value": 0.342448,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.017s"
          },
          {
            "name": "RAJA_HIP-block_256_ARRAY_OF_PTRS_1",
            "value": 0.0000749088,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.000s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_COPY8",
            "value": 0.20461400000000002,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.010s"
          },
          {
            "name": "RAJA_HIP-block_256_COPY8_1",
            "value": 0.0000761364,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.000s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_DAXPY",
            "value": 0.0399824,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.020s"
          },
          {
            "name": "RAJA_HIP-block_256_DAXPY_1",
            "value": 0.00000558936,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 500, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_DAXPY_ATOMIC",
            "value": 0.0236886,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.012s"
          },
          {
            "name": "RAJA_HIP-block_256_DAXPY_ATOMIC_1",
            "value": 0.00000643268,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 500, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_IF_QUAD",
            "value": 0.06000444444444444,
            "unit": "ms/rep",
            "extra": "reps: 180, passes: 1, total_time: 0.011s"
          },
          {
            "name": "RAJA_HIP-block_256_IF_QUAD_1",
            "value": 0.000022799907407407405,
            "unit": "ms/rep",
            "extra": "reps: 180, passes: 180, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INDEXLIST_3LOOP",
            "value": 0.0989817,
            "unit": "ms/rep",
            "extra": "reps: 100, passes: 1, total_time: 0.010s"
          },
          {
            "name": "RAJA_HIP-block_256_INDEXLIST_3LOOP_1",
            "value": 0.0000335562,
            "unit": "ms/rep",
            "extra": "reps: 100, passes: 100, total_time: 0.000s"
          },
          {
            "name": "RAJA_HIP-block_256_INDEXLIST_3LOOP_2",
            "value": 0.000030316099999999996,
            "unit": "ms/rep",
            "extra": "reps: 100, passes: 100, total_time: 0.000s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INIT3",
            "value": 0.0515974,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.026s"
          },
          {
            "name": "RAJA_HIP-block_256_INIT3_1",
            "value": 0.00000715684,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 500, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INIT_VIEW1D",
            "value": 0.0165518,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.041s"
          },
          {
            "name": "RAJA_HIP-block_256_INIT_VIEW1D_1",
            "value": 0.0000013251695999999999,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 2500, total_time: 0.008s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.01688968,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.042s"
          },
          {
            "name": "RAJA_HIP-block_256_INIT_VIEW1D_OFFSET_1",
            "value": 0.0000012939007999999997,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 2500, total_time: 0.008s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_MAT_MAT_SHARED",
            "value": 5.28662,
            "unit": "ms/rep",
            "extra": "reps: 5, passes: 1, total_time: 0.026s"
          },
          {
            "name": "RAJA_HIP-block_256_MAT_MAT_SHARED_1",
            "value": 0.0025404,
            "unit": "ms/rep",
            "extra": "reps: 5, passes: 5, total_time: 0.000s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_MULADDSUB",
            "value": 0.056513142857142866,
            "unit": "ms/rep",
            "extra": "reps: 350, passes: 1, total_time: 0.020s"
          },
          {
            "name": "RAJA_HIP-block_256_MULADDSUB_1",
            "value": 0.000010311183673469388,
            "unit": "ms/rep",
            "extra": "reps: 350, passes: 350, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_NESTED_INIT",
            "value": 0.022006,
            "unit": "ms/rep",
            "extra": "reps: 1000, passes: 1, total_time: 0.022s"
          },
          {
            "name": "RAJA_HIP-block_256_NESTED_INIT_1",
            "value": 0.00000368804,
            "unit": "ms/rep",
            "extra": "reps: 1000, passes: 1000, total_time: 0.004s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_PI_ATOMIC",
            "value": 0.717994,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.036s"
          },
          {
            "name": "RAJA_HIP-block_256_PI_ATOMIC_1",
            "value": 0.014347720000000001,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.036s"
          },
          {
            "name": "RAJA_HIP-direct_256_Basic_EMPTY",
            "value": 0.0103577,
            "unit": "ms/rep",
            "extra": "reps: 1000, passes: 1, total_time: 0.010s"
          },
          {
            "name": "RAJA_HIP-direct_256_EMPTY_1",
            "value": 0.00000300149,
            "unit": "ms/rep",
            "extra": "reps: 1000, passes: 1000, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-occgs_256_Basic_EMPTY",
            "value": 0.0045838,
            "unit": "ms/rep",
            "extra": "reps: 1000, passes: 1, total_time: 0.005s"
          },
          {
            "name": "RAJA_HIP-occgs_256_EMPTY_1",
            "value": 0.00000417117,
            "unit": "ms/rep",
            "extra": "reps: 1000, passes: 1000, total_time: 0.004s"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "Jason Burmark",
            "username": "MrBurmark",
            "email": "MrBurmark@users.noreply.github.com"
          },
          "committer": {
            "name": "GitHub",
            "username": "web-flow",
            "email": "noreply@github.com"
          },
          "id": "5301486b1ea58280079a4fe252710db5e01e758a",
          "message": "Merge pull request #687 from llnl/pr-from-fork/686\n\nPr from fork/686",
          "timestamp": "2026-08-20T19:54:32Z",
          "url": "https://github.com/llnl/RAJAPerf/commit/5301486b1ea58280079a4fe252710db5e01e758a"
        },
        "date": 1787293826680,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "RAJA_HIP-atomic_direct_256_Basic_MULTI_REDUCE",
            "value": 0.04499160000000001,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-atomic_direct_256_MULTI_REDUCE_1",
            "value": 0.000887808,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-atomic_occgs_256_Basic_MULTI_REDUCE",
            "value": 0.04304159999999999,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-atomic_occgs_256_MULTI_REDUCE_1",
            "value": 0.0008496679999999999,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_Basic_PI_REDUCE",
            "value": 0.037843,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_PI_REDUCE_1",
            "value": 0.000747192,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_Basic_REDUCE3_INT",
            "value": 0.06207940000000001,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_REDUCE3_INT_1",
            "value": 0.0012299479999999998,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_Basic_REDUCE_STRUCT",
            "value": 0.1974146,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.010s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_REDUCE_STRUCT_1",
            "value": 0.00393182,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.010s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_Basic_TRAP_INT",
            "value": 0.04819999999999999,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkatm_direct_256_TRAP_INT_1",
            "value": 0.000952628,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_Basic_PI_REDUCE",
            "value": 0.0207294,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_PI_REDUCE_1",
            "value": 0.00040557999999999995,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_Basic_REDUCE3_INT",
            "value": 0.0251368,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_REDUCE3_INT_1",
            "value": 0.000491408,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_Basic_REDUCE_STRUCT",
            "value": 0.05722259999999999,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_REDUCE_STRUCT_1",
            "value": 0.00112576,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_Basic_TRAP_INT",
            "value": 0.025223,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkatm_occgs_256_TRAP_INT_1",
            "value": 0.0004940560000000001,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_Basic_PI_REDUCE",
            "value": 0.0441456,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_PI_REDUCE_1",
            "value": 0.0008738320000000002,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_Basic_REDUCE3_INT",
            "value": 0.0870134,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.004s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_REDUCE3_INT_1",
            "value": 0.001726464,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.004s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_Basic_REDUCE_STRUCT",
            "value": 0.219584,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.011s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_REDUCE_STRUCT_1",
            "value": 0.00437464,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.011s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_Basic_TRAP_INT",
            "value": 0.04899,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_256_TRAP_INT_1",
            "value": 0.0009696959999999999,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_Basic_PI_REDUCE",
            "value": 0.490026,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.025s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_PI_REDUCE_1",
            "value": 0.00979064,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.024s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_Basic_REDUCE3_INT",
            "value": 1.4580899999999999,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.073s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_REDUCE3_INT_1",
            "value": 0.02914744,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.073s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_Basic_REDUCE_STRUCT",
            "value": 3.0178,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.151s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_REDUCE_STRUCT_1",
            "value": 0.0603408,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.151s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_Basic_TRAP_INT",
            "value": 0.49479399999999996,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.025s"
          },
          {
            "name": "RAJA_HIP-blkdev_direct_new_256_TRAP_INT_1",
            "value": 0.00988408,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.025s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_Basic_PI_REDUCE",
            "value": 0.0216176,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_PI_REDUCE_1",
            "value": 0.000423516,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_Basic_REDUCE3_INT",
            "value": 0.028515,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_REDUCE3_INT_1",
            "value": 0.0005604719999999999,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_Basic_REDUCE_STRUCT",
            "value": 0.059775400000000006,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_REDUCE_STRUCT_1",
            "value": 0.0011804880000000002,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_Basic_TRAP_INT",
            "value": 0.025707999999999998,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_256_TRAP_INT_1",
            "value": 0.000504212,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_Basic_PI_REDUCE",
            "value": 0.05710899999999999,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_PI_REDUCE_1",
            "value": 0.001133828,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_Basic_REDUCE3_INT",
            "value": 0.137859,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.007s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_REDUCE3_INT_1",
            "value": 0.0027465799999999998,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.007s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_Basic_REDUCE_STRUCT",
            "value": 0.28795000000000004,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.014s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_REDUCE_STRUCT_1",
            "value": 0.005742560000000001,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.014s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_Basic_TRAP_INT",
            "value": 0.058102999999999995,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-blkdev_occgs_new_256_TRAP_INT_1",
            "value": 0.0011532679999999998,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_ARRAY_OF_PTRS",
            "value": 0.346408,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.017s"
          },
          {
            "name": "RAJA_HIP-block_256_ARRAY_OF_PTRS_1",
            "value": 0.00008227640000000001,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.000s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_COPY8",
            "value": 0.204708,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.010s"
          },
          {
            "name": "RAJA_HIP-block_256_COPY8_1",
            "value": 0.00009112840000000001,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.000s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_DAXPY",
            "value": 0.0396412,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.020s"
          },
          {
            "name": "RAJA_HIP-block_256_DAXPY_1",
            "value": 0.000005662480000000001,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 500, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_DAXPY_ATOMIC",
            "value": 0.0237238,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.012s"
          },
          {
            "name": "RAJA_HIP-block_256_DAXPY_ATOMIC_1",
            "value": 0.000006504720000000001,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 500, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_IF_QUAD",
            "value": 0.06379722222222223,
            "unit": "ms/rep",
            "extra": "reps: 180, passes: 1, total_time: 0.011s"
          },
          {
            "name": "RAJA_HIP-block_256_IF_QUAD_1",
            "value": 0.000021697716049382716,
            "unit": "ms/rep",
            "extra": "reps: 180, passes: 180, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INDEXLIST_3LOOP",
            "value": 0.0980553,
            "unit": "ms/rep",
            "extra": "reps: 100, passes: 1, total_time: 0.010s"
          },
          {
            "name": "RAJA_HIP-block_256_INDEXLIST_3LOOP_1",
            "value": 0.000032876299999999996,
            "unit": "ms/rep",
            "extra": "reps: 100, passes: 100, total_time: 0.000s"
          },
          {
            "name": "RAJA_HIP-block_256_INDEXLIST_3LOOP_2",
            "value": 0.000028781199999999995,
            "unit": "ms/rep",
            "extra": "reps: 100, passes: 100, total_time: 0.000s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INIT3",
            "value": 0.0541112,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 1, total_time: 0.027s"
          },
          {
            "name": "RAJA_HIP-block_256_INIT3_1",
            "value": 0.0000069394,
            "unit": "ms/rep",
            "extra": "reps: 500, passes: 500, total_time: 0.002s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INIT_VIEW1D",
            "value": 0.01656764,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.041s"
          },
          {
            "name": "RAJA_HIP-block_256_INIT_VIEW1D_1",
            "value": 0.0000013213952000000001,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 2500, total_time: 0.008s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_INIT_VIEW1D_OFFSET",
            "value": 0.01694544,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 1, total_time: 0.042s"
          },
          {
            "name": "RAJA_HIP-block_256_INIT_VIEW1D_OFFSET_1",
            "value": 0.0000013081328,
            "unit": "ms/rep",
            "extra": "reps: 2500, passes: 2500, total_time: 0.008s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_MAT_MAT_SHARED",
            "value": 5.291919999999999,
            "unit": "ms/rep",
            "extra": "reps: 5, passes: 1, total_time: 0.026s"
          },
          {
            "name": "RAJA_HIP-block_256_MAT_MAT_SHARED_1",
            "value": 0.0023912,
            "unit": "ms/rep",
            "extra": "reps: 5, passes: 5, total_time: 0.000s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_MULADDSUB",
            "value": 0.06194000000000001,
            "unit": "ms/rep",
            "extra": "reps: 350, passes: 1, total_time: 0.022s"
          },
          {
            "name": "RAJA_HIP-block_256_MULADDSUB_1",
            "value": 0.000010560081632653061,
            "unit": "ms/rep",
            "extra": "reps: 350, passes: 350, total_time: 0.001s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_NESTED_INIT",
            "value": 0.0220837,
            "unit": "ms/rep",
            "extra": "reps: 1000, passes: 1, total_time: 0.022s"
          },
          {
            "name": "RAJA_HIP-block_256_NESTED_INIT_1",
            "value": 0.00000384626,
            "unit": "ms/rep",
            "extra": "reps: 1000, passes: 1000, total_time: 0.004s"
          },
          {
            "name": "RAJA_HIP-block_256_Basic_PI_ATOMIC",
            "value": 0.717984,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 1, total_time: 0.036s"
          },
          {
            "name": "RAJA_HIP-block_256_PI_ATOMIC_1",
            "value": 0.01434812,
            "unit": "ms/rep",
            "extra": "reps: 50, passes: 50, total_time: 0.036s"
          },
          {
            "name": "RAJA_HIP-direct_256_Basic_EMPTY",
            "value": 0.0103473,
            "unit": "ms/rep",
            "extra": "reps: 1000, passes: 1, total_time: 0.010s"
          },
          {
            "name": "RAJA_HIP-direct_256_EMPTY_1",
            "value": 0.0000029879899999999997,
            "unit": "ms/rep",
            "extra": "reps: 1000, passes: 1000, total_time: 0.003s"
          },
          {
            "name": "RAJA_HIP-occgs_256_Basic_EMPTY",
            "value": 0.00486121,
            "unit": "ms/rep",
            "extra": "reps: 1000, passes: 1, total_time: 0.005s"
          },
          {
            "name": "RAJA_HIP-occgs_256_EMPTY_1",
            "value": 0.000004420010000000001,
            "unit": "ms/rep",
            "extra": "reps: 1000, passes: 1000, total_time: 0.004s"
          }
        ]
      }
    ]
  }
}