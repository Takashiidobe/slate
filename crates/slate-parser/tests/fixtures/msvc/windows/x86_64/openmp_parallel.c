#define THREADS 4
#define PARALLEL parallel

int global_counter;

void clauses(int n) {
  int a = 1, b = 2, c = 3;
#pragma omp parallel private(a) firstprivate(b) shared(c, global_counter) default(shared) num_threads(n + 1) if(n)
  {
    a = b + c;
    global_counter += a;
  }
}

void implicit_sharing(int parameter) {
  int outer = 0;
  static int counter;
#pragma omp parallel
  {
    int inner = parameter;
    outer += inner;
    counter++;
  }
}

void privatize_global_and_parameter(int parameter) {
#pragma omp parallel private(global_counter) firstprivate(parameter)
  global_counter = parameter;
}

void default_none(void) {
  int listed = 0;
  int values[4] = {0};
#pragma omp parallel default(none) shared(listed) firstprivate(values) num_threads(THREADS)
  {
    int local = values[0];
    listed += local;
  }
}

void nested(void) {
  int a = 0;
#pragma omp PARALLEL shared(a)
#pragma omp parallel private(a)
  a = 1;
}

void positions(int flag) {
  if (flag)
#pragma omp parallel if(flag > 1)
    ;
label:
#pragma omp parallel
  ;
  goto label;
}

void inner_branches(void) {
#pragma omp parallel
  {
    for (int i = 0; i < 4; i++) {
      if (i == 2)
        break;
      continue;
    }
    goto done;
  done:;
  }
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
// DEFAULT-NEXT:     global %[[VALUE_global_counter:[0-9]+]] global_counter: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_counter:[0-9]+]] counter: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_clauses:[0-9]+]] @clauses(%[[VALUE_n:[0-9]+]] n: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: i32 [storage=automatic] = const<i32>(3);
// DEFAULT-NEXT:         omp_parallel
// DEFAULT-NEXT:             num_threads: reinterpret<u32>(add<i32>(read<i32>(%[[VALUE_n]]), const<i32>(1)))
// DEFAULT-NEXT:             if: ne<i32>(read<i32>(%[[VALUE_n]]), const<i32>(0))
// DEFAULT-NEXT:             shared: %[[VALUE_c]]
// DEFAULT-NEXT:             private %[[VALUE_a]]: %[[VALUE_a_2:[0-9]+]] a: i32 [storage=automatic]
// DEFAULT-NEXT:             firstprivate %[[VALUE_b]]: %[[VALUE_b_2:[0-9]+]] b: i32 [storage=automatic]
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_a_2]], add<i32>(read<i32>(%[[VALUE_b_2]]), read<i32>(%[[VALUE_c]])));
// DEFAULT-NEXT:                     let %[[VALUE0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_global_counter]]);
// DEFAULT-NEXT:                     let %[[VALUE1:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE0]]), read<i32>(%[[VALUE_a_2]]));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_global_counter]], read<i32>(%[[VALUE1]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_implicit_sharing:[0-9]+]] @implicit_sharing(%[[VALUE_parameter:[0-9]+]] parameter: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_outer:[0-9]+]] outer: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         omp_parallel
// DEFAULT-NEXT:             shared: %[[VALUE_parameter]], %[[VALUE_outer]]
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_inner:[0-9]+]] inner: i32 [storage=automatic] = read<i32>(%[[VALUE_parameter]]);
// DEFAULT-NEXT:                     let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_outer]]);
// DEFAULT-NEXT:                     let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE2]]), read<i32>(%[[VALUE_inner]]));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_outer]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:                     let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_counter]]);
// DEFAULT-NEXT:                     let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_counter]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_privatize_global_and_parameter:[0-9]+]] @privatize_global_and_parameter(%[[VALUE_parameter_2:[0-9]+]] parameter: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         omp_parallel
// DEFAULT-NEXT:             private %[[VALUE_global_counter]]: %[[VALUE_global_counter_2:[0-9]+]] global_counter: i32 [storage=automatic]
// DEFAULT-NEXT:             firstprivate %[[VALUE_parameter_2]]: %[[VALUE_parameter_3:[0-9]+]] parameter: i32 [storage=automatic]
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_global_counter_2]], read<i32>(%[[VALUE_parameter_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_default_none:[0-9]+]] @default_none() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_listed:[0-9]+]] listed: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_values:[0-9]+]] values: array<i32, 4> [storage=automatic] [align=16] = aggregate<array<i32, 4>, zero_fill=true>(index0 = const<i32>(0));
// DEFAULT-NEXT:         omp_parallel
// DEFAULT-NEXT:             num_threads: reinterpret<u32>(const<i32>(4))
// DEFAULT-NEXT:             shared: %[[VALUE_listed]]
// DEFAULT-NEXT:             firstprivate %[[VALUE_values]]: %[[VALUE_values_2:[0-9]+]] values: array<i32, 4> [storage=automatic]
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_local:[0-9]+]] local: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false>(array_decay<ptr<i32>, length=Some(4)>(%[[VALUE_values_2]]), const<i32>(0))));
// DEFAULT-NEXT:                     let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_listed]]);
// DEFAULT-NEXT:                     let %[[VALUE7:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE6]]), read<i32>(%[[VALUE_local]]));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_listed]], read<i32>(%[[VALUE7]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_nested:[0-9]+]] @nested() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_a_3:[0-9]+]] a: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         omp_parallel
// DEFAULT-NEXT:             shared: %[[VALUE_a_3]]
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 omp_parallel
// DEFAULT-NEXT:                     private %[[VALUE_a_3]]: %[[VALUE_a_4:[0-9]+]] a: i32 [storage=automatic]
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_a_4]], const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_positions:[0-9]+]] @positions(%[[VALUE_flag:[0-9]+]] flag: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_flag]]), const<i32>(0))
// DEFAULT-NEXT:             omp_parallel
// DEFAULT-NEXT:                 if: gt<i32>(read<i32>(%[[VALUE_flag]]), const<i32>(1))
// DEFAULT-NEXT:                 body:
// DEFAULT-NEXT:                     ;
// DEFAULT-NEXT:         label %[[VALUE_label:[0-9]+]] label:
// DEFAULT-NEXT:             omp_parallel
// DEFAULT-NEXT:                 body:
// DEFAULT-NEXT:                     ;
// DEFAULT-NEXT:         goto %[[VALUE_label]];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_inner_branches:[0-9]+]] @inner_branches() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         omp_parallel
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     for %[[VALUE8:[0-9]+]]
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                         condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(4))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %[[VALUE9:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                             let %[[VALUE10:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE9]]), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE10]]));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 if eq<i32>(read<i32>(%[[VALUE_i]]), const<i32>(2))
// DEFAULT-NEXT:                                     break %[[VALUE8]];
// DEFAULT-NEXT:                                 continue %[[VALUE8]];
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     goto %[[VALUE_done:[0-9]+]];
// DEFAULT-NEXT:                     label %[[VALUE_done]] done:
// DEFAULT-NEXT:                         ;
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
