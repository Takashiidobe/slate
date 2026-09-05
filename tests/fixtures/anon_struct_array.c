#include <stdio.h>

// @rewrite-fn-begin
int main(void) {
  struct {
    int         code;
    const char *message;
    double      confidence;
  } error_log[] = {
      {404, "Not Found", 0.99},
      {500, "Internal Server Error", 0.85},
      {200, "OK", 1.00},
  };

  printf("%d\n", error_log[0].code);
  return 0;
}
// @rewrite-fn-end

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: fn main() {
// COMMON-REWRITES-DAG:         [{{anon_[0-9]+}} {
// COMMON-REWRITES-DAG:             code: 0,
// COMMON-REWRITES-DAG:             __slate_anon_1: std::ptr::null_mut(),
// COMMON-REWRITES-DAG:             __slate_anon_2: 0.0,
// COMMON-REWRITES-DAG:         }; 3],
// COMMON-REWRITES-DAG:     );
// COMMON-REWRITES-DAG:     *error_log = [
// COMMON-REWRITES-DAG:         {{anon_[0-9]+}} {
// COMMON-REWRITES-DAG:             code: 404,
// COMMON-REWRITES-DAG:             __slate_anon_2: 0.99,
// COMMON-REWRITES-DAG:         },
// COMMON-REWRITES-DAG:         {{anon_[0-9]+}} {
// COMMON-REWRITES-DAG:             code: 500,
// COMMON-REWRITES-DAG:             __slate_anon_2: 0.85,
// COMMON-REWRITES-DAG:         },
// COMMON-REWRITES-DAG:         {{anon_[0-9]+}} {
// COMMON-REWRITES-DAG:             code: 200,
// COMMON-REWRITES-DAG:             __slate_anon_2: 1.0,
// COMMON-REWRITES-DAG:         },
// COMMON-REWRITES-DAG:     ];
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = error_log[0].code;
// COMMON-REWRITES-DAG:     unsafe { printf(c"%d\n".as_ptr(), {{__v[0-9]+}}) };
// COMMON-REWRITES-DAG:     std::process::exit(0 as i32);
// COMMON-REWRITES-DAG: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-DAG:     let mut error_log: aligned::Aligned<aligned::A16, [{{anon_[0-9]+}}; 3]> = aligned::Aligned(
// REWRITES-X86_64-GNU-DAG:             __slate_anon_1: c"Not Found".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-DAG:             __slate_anon_1: c"Internal Server Error".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-DAG:             __slate_anon_1: c"OK".as_ptr() as *mut i8,
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-DAG:     let mut error_log: aligned::Aligned<aligned::A8, [{{anon_[0-9]+}}; 3]> = aligned::Aligned(
// REWRITES-AARCH64-GNU-DAG:             __slate_anon_1: c"Not Found".as_ptr() as *mut u8,
// REWRITES-AARCH64-GNU-DAG:             __slate_anon_1: c"Internal Server Error".as_ptr() as *mut u8,
// REWRITES-AARCH64-GNU-DAG:             __slate_anon_1: c"OK".as_ptr() as *mut u8,
// SLATE-FILECHECK-END rewrites-aarch64-gnu
