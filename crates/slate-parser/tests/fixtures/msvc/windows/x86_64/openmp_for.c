#define CHUNK 4

int next(void);

void schedules(int n, int *out) {
  int i;
#pragma omp for schedule(static)
  for (i = 0; i < n; i++)
    out[i] = i;
#pragma omp for schedule(static, CHUNK) nowait
  for (i = 0; i < n; i++)
    out[i] = i;
#pragma omp for schedule(dynamic, n ? 2 : 4)
  for (i = 0; i < n; i++)
    out[i] = i;
#pragma omp for schedule(guided)
  for (i = 0; i < n; i++)
    out[i] = i;
#pragma omp for schedule(runtime)
  for (i = 0; i < n; i++)
    out[i] = i;
}

void iterators(int count, double *first) {
#pragma omp parallel
  {
    int u;
    long k;
    short i;
#pragma omp for
    for (u = 0; u <= count; u += 2)
      first[u] = 0;
#pragma omp for
    for (k = count; k > 0; --k)
      first[k] = 2;
#pragma omp for
    for (i = next(); i < 10; i = 1 + i)
      first[i] = 3;
  }
}

void combined(int n, int m, int *grid) {
  int i, j, sum = 0;
#pragma omp parallel for collapse(2) private(sum) schedule(static, 2) num_threads(4)
  for (i = 0; i < n; i++)
    for (j = m - 1; j >= 0; j -= 1) {
      sum = i * m + j;
      if (sum < 0)
        continue;
      grid[sum] = sum;
    }
#pragma omp parallel for private(i) firstprivate(m)
  for (i = 0; i < n; i++)
    grid[i] = m;
}

void ignored_collapse(int n, int *out) {
  int i, j;
#pragma omp parallel for collapse(n) collapse(2)
  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++)
      out[i] += j;
}

void chunk_capture(int chunk, int *out) {
  int i;
#pragma omp parallel for schedule(static, chunk) firstprivate(chunk)
  for (i = 0; i < 8; i++)
    out[i] = chunk;
}

// SLATE-FILECHECK-ARGS --compact-ir /Zc:preprocessor /openmp
// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f64;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_next:[0-9]+]] @next() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_schedules:[0-9]+]] @schedules(%[[VALUE_n:[0-9]+]] n: i32, %[[VALUE_out:[0-9]+]] out: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         omp_for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             schedule: static
// DEFAULT-NEXT:             loop %[[VALUE_i_2:[0-9]+]] i: i32 [storage=automatic]
// DEFAULT-NEXT:                 lower: const<i32>(0)
// DEFAULT-NEXT:                 condition: lt read<i32>(%[[VALUE_n]])
// DEFAULT-NEXT:                 step: add const<i32>(1)
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(deref(ptr_offset<ptr<i32>, subtract=false>(read<ptr<i32>>(%[[VALUE_out]]), read<i32>(%[[VALUE_i_2]]))), read<i32>(%[[VALUE_i_2]]));
// DEFAULT-NEXT:         omp_for %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             schedule: static, chunk const<i32>(4)
// DEFAULT-NEXT:             nowait
// DEFAULT-NEXT:             loop %[[VALUE_i_3:[0-9]+]] i: i32 [storage=automatic]
// DEFAULT-NEXT:                 lower: const<i32>(0)
// DEFAULT-NEXT:                 condition: lt read<i32>(%[[VALUE_n]])
// DEFAULT-NEXT:                 step: add const<i32>(1)
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(deref(ptr_offset<ptr<i32>, subtract=false>(read<ptr<i32>>(%[[VALUE_out]]), read<i32>(%[[VALUE_i_3]]))), read<i32>(%[[VALUE_i_3]]));
// DEFAULT-NEXT:         omp_for %[[VALUE2:[0-9]+]]
// DEFAULT-NEXT:             schedule: dynamic, chunk conditional<i32>(ne<i32>(read<i32>(%[[VALUE_n]]), const<i32>(0)), const<i32>(2), const<i32>(4))
// DEFAULT-NEXT:             loop %[[VALUE_i_4:[0-9]+]] i: i32 [storage=automatic]
// DEFAULT-NEXT:                 lower: const<i32>(0)
// DEFAULT-NEXT:                 condition: lt read<i32>(%[[VALUE_n]])
// DEFAULT-NEXT:                 step: add const<i32>(1)
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(deref(ptr_offset<ptr<i32>, subtract=false>(read<ptr<i32>>(%[[VALUE_out]]), read<i32>(%[[VALUE_i_4]]))), read<i32>(%[[VALUE_i_4]]));
// DEFAULT-NEXT:         omp_for %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:             schedule: guided
// DEFAULT-NEXT:             loop %[[VALUE_i_5:[0-9]+]] i: i32 [storage=automatic]
// DEFAULT-NEXT:                 lower: const<i32>(0)
// DEFAULT-NEXT:                 condition: lt read<i32>(%[[VALUE_n]])
// DEFAULT-NEXT:                 step: add const<i32>(1)
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(deref(ptr_offset<ptr<i32>, subtract=false>(read<ptr<i32>>(%[[VALUE_out]]), read<i32>(%[[VALUE_i_5]]))), read<i32>(%[[VALUE_i_5]]));
// DEFAULT-NEXT:         omp_for %[[VALUE4:[0-9]+]]
// DEFAULT-NEXT:             schedule: runtime
// DEFAULT-NEXT:             loop %[[VALUE_i_6:[0-9]+]] i: i32 [storage=automatic]
// DEFAULT-NEXT:                 lower: const<i32>(0)
// DEFAULT-NEXT:                 condition: lt read<i32>(%[[VALUE_n]])
// DEFAULT-NEXT:                 step: add const<i32>(1)
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(deref(ptr_offset<ptr<i32>, subtract=false>(read<ptr<i32>>(%[[VALUE_out]]), read<i32>(%[[VALUE_i_6]]))), read<i32>(%[[VALUE_i_6]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_iterators:[0-9]+]] @iterators(%[[VALUE_count:[0-9]+]] count: i32, %[[VALUE_first:[0-9]+]] first: ptr<f64>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         omp_parallel
// DEFAULT-NEXT:             shared: %[[VALUE_count]], %[[VALUE_first]]
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_u:[0-9]+]] u: i32 [storage=automatic];
// DEFAULT-NEXT:                     let %[[VALUE_k:[0-9]+]] k: i32 [storage=automatic];
// DEFAULT-NEXT:                     let %[[VALUE_i_7:[0-9]+]] i: i16 [storage=automatic];
// DEFAULT-NEXT:                     omp_for %[[VALUE5:[0-9]+]]
// DEFAULT-NEXT:                         loop %[[VALUE_u_2:[0-9]+]] u: i32 [storage=automatic]
// DEFAULT-NEXT:                             lower: const<i32>(0)
// DEFAULT-NEXT:                             condition: le read<i32>(%[[VALUE_count]])
// DEFAULT-NEXT:                             step: add const<i32>(2)
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             write<f64>(deref(ptr_offset<ptr<f64>, subtract=false>(read<ptr<f64>>(%[[VALUE_first]]), read<i32>(%[[VALUE_u_2]]))), int_to_float<f64>(const<i32>(0)));
// DEFAULT-NEXT:                     omp_for %[[VALUE6:[0-9]+]]
// DEFAULT-NEXT:                         loop %[[VALUE_k_2:[0-9]+]] k: i32 [storage=automatic]
// DEFAULT-NEXT:                             lower: read<i32>(%[[VALUE_count]])
// DEFAULT-NEXT:                             condition: gt const<i32>(0)
// DEFAULT-NEXT:                             step: sub const<i32>(1)
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             write<f64>(deref(ptr_offset<ptr<f64>, subtract=false>(read<ptr<f64>>(%[[VALUE_first]]), read<i32>(%[[VALUE_k_2]]))), int_to_float<f64>(const<i32>(2)));
// DEFAULT-NEXT:                     omp_for %[[VALUE7:[0-9]+]]
// DEFAULT-NEXT:                         loop %[[VALUE_i_8:[0-9]+]] i: i16 [storage=automatic]
// DEFAULT-NEXT:                             lower: truncate<i16>(call<i32>(%[[VALUE_next]]))
// DEFAULT-NEXT:                             condition: lt truncate<i16>(const<i32>(10))
// DEFAULT-NEXT:                             step: add truncate<i16>(const<i32>(1))
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             write<f64>(deref(ptr_offset<ptr<f64>, subtract=false>(read<ptr<f64>>(%[[VALUE_first]]), widen<i32>(read<i16>(%[[VALUE_i_8]])))), int_to_float<f64>(const<i32>(3)));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_combined:[0-9]+]] @combined(%[[VALUE_n_2:[0-9]+]] n: i32, %[[VALUE_m:[0-9]+]] m: i32, %[[VALUE_grid:[0-9]+]] grid: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i_9:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_sum:[0-9]+]] sum: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         omp_parallel
// DEFAULT-NEXT:             num_threads: reinterpret<u32>(const<i32>(4))
// DEFAULT-NEXT:             shared: %[[VALUE_n_2]], %[[VALUE_j]], %[[VALUE_m]], %[[VALUE_grid]]
// DEFAULT-NEXT:             private %[[VALUE_sum]]: %[[VALUE_sum_2:[0-9]+]] sum: i32 [storage=automatic]
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 omp_for %[[VALUE8:[0-9]+]]
// DEFAULT-NEXT:                     schedule: static, chunk const<i32>(2)
// DEFAULT-NEXT:                     loop %[[VALUE_i_10:[0-9]+]] i: i32 [storage=automatic]
// DEFAULT-NEXT:                         lower: const<i32>(0)
// DEFAULT-NEXT:                         condition: lt read<i32>(%[[VALUE_n_2]])
// DEFAULT-NEXT:                         step: add const<i32>(1)
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         for %[[VALUE9:[0-9]+]]
// DEFAULT-NEXT:                             init:
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_j]], sub<i32>(read<i32>(%[[VALUE_m]]), const<i32>(1)));
// DEFAULT-NEXT:                             condition: ge<i32>(read<i32>(%[[VALUE_j]]), const<i32>(0))
// DEFAULT-NEXT:                             increment: {
// DEFAULT-NEXT:                                 let %[[VALUE10:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j]]);
// DEFAULT-NEXT:                                 let %[[VALUE11:[0-9]+]]: i32 [synthetic] = sub<i32>(read<i32>(%[[VALUE10]]), const<i32>(1));
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_j]], read<i32>(%[[VALUE11]]));
// DEFAULT-NEXT:                                 yield void;
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                             body:
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     write<i32>(%[[VALUE_sum_2]], add<i32>(mul<i32>(read<i32>(%[[VALUE_i_10]]), read<i32>(%[[VALUE_m]])), read<i32>(%[[VALUE_j]])));
// DEFAULT-NEXT:                                     if lt<i32>(read<i32>(%[[VALUE_sum_2]]), const<i32>(0))
// DEFAULT-NEXT:                                         continue %[[VALUE9]];
// DEFAULT-NEXT:                                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false>(read<ptr<i32>>(%[[VALUE_grid]]), read<i32>(%[[VALUE_sum_2]]))), read<i32>(%[[VALUE_sum_2]]));
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:         omp_parallel
// DEFAULT-NEXT:             shared: %[[VALUE_n_2]], %[[VALUE_grid]]
// DEFAULT-NEXT:             firstprivate %[[VALUE_m]]: %[[VALUE_m_2:[0-9]+]] m: i32 [storage=automatic]
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 omp_for %[[VALUE12:[0-9]+]]
// DEFAULT-NEXT:                     loop %[[VALUE_i_11:[0-9]+]] i: i32 [storage=automatic]
// DEFAULT-NEXT:                         lower: const<i32>(0)
// DEFAULT-NEXT:                         condition: lt read<i32>(%[[VALUE_n_2]])
// DEFAULT-NEXT:                         step: add const<i32>(1)
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false>(read<ptr<i32>>(%[[VALUE_grid]]), read<i32>(%[[VALUE_i_11]]))), read<i32>(%[[VALUE_m_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ignored_collapse:[0-9]+]] @ignored_collapse(%[[VALUE_n_3:[0-9]+]] n: i32, %[[VALUE_out_2:[0-9]+]] out: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i_12:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_j_2:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         omp_parallel
// DEFAULT-NEXT:             shared: %[[VALUE_n_3]], %[[VALUE_j_2]], %[[VALUE_out_2]]
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 omp_for %[[VALUE13:[0-9]+]]
// DEFAULT-NEXT:                     loop %[[VALUE_i_13:[0-9]+]] i: i32 [storage=automatic]
// DEFAULT-NEXT:                         lower: const<i32>(0)
// DEFAULT-NEXT:                         condition: lt read<i32>(%[[VALUE_n_3]])
// DEFAULT-NEXT:                         step: add const<i32>(1)
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         for %[[VALUE14:[0-9]+]]
// DEFAULT-NEXT:                             init:
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_j_2]], const<i32>(0));
// DEFAULT-NEXT:                             condition: lt<i32>(read<i32>(%[[VALUE_j_2]]), read<i32>(%[[VALUE_n_3]]))
// DEFAULT-NEXT:                             increment: {
// DEFAULT-NEXT:                                 let %[[VALUE15:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_2]]);
// DEFAULT-NEXT:                                 let %[[VALUE16:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE15]]), const<i32>(1));
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_j_2]], read<i32>(%[[VALUE16]]));
// DEFAULT-NEXT:                                 yield void;
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                             body:
// DEFAULT-NEXT:                                 let %[[VALUE17:[0-9]+]]: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false>(read<ptr<i32>>(%[[VALUE_out_2]]), read<i32>(%[[VALUE_i_13]]));
// DEFAULT-NEXT:                                 let %[[VALUE18:[0-9]+]]: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%[[VALUE17]])));
// DEFAULT-NEXT:                                 let %[[VALUE19:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE18]]), read<i32>(%[[VALUE_j_2]]));
// DEFAULT-NEXT:                                 write<i32>(deref(read<ptr<i32>>(%[[VALUE17]])), read<i32>(%[[VALUE19]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_chunk_capture:[0-9]+]] @chunk_capture(%[[VALUE_chunk:[0-9]+]] chunk: i32, %[[VALUE_out_3:[0-9]+]] out: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i_14:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         omp_parallel
// DEFAULT-NEXT:             shared: %[[VALUE_chunk]], %[[VALUE_out_3]]
// DEFAULT-NEXT:             firstprivate %[[VALUE_chunk]]: %[[VALUE_chunk_2:[0-9]+]] chunk: i32 [storage=automatic]
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 omp_for %[[VALUE20:[0-9]+]]
// DEFAULT-NEXT:                     schedule: static, chunk read<i32>(%[[VALUE_chunk]])
// DEFAULT-NEXT:                     loop %[[VALUE_i_15:[0-9]+]] i: i32 [storage=automatic]
// DEFAULT-NEXT:                         lower: const<i32>(0)
// DEFAULT-NEXT:                         condition: lt const<i32>(8)
// DEFAULT-NEXT:                         step: add const<i32>(1)
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false>(read<ptr<i32>>(%[[VALUE_out_3]]), read<i32>(%[[VALUE_i_15]]))), read<i32>(%[[VALUE_chunk_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
