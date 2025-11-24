window.BENCHMARK_DATA = {
  "lastUpdate": 1763996177616,
  "repoUrl": "https://github.com/LLNL/RAJAPerf",
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
      }
    ]
  }
}