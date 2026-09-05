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

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn main() {
// REWRITES-DAG:     let mut error_log: aligned::Aligned<aligned::A16, [{{anon_[0-9]+}}; 3]> = aligned::Aligned(
// REWRITES-DAG:         [{{anon_[0-9]+}} {
// REWRITES-DAG:             code: 0,
// REWRITES-DAG:             __slate_anon_1: std::ptr::null_mut(),
// REWRITES-DAG:             __slate_anon_2: 0.0,
// REWRITES-DAG:         }; 3],
// REWRITES-DAG:     );
// REWRITES-DAG:     *error_log = [
// REWRITES-DAG:         {{anon_[0-9]+}} {
// REWRITES-DAG:             code: 404,
// REWRITES-DAG:             __slate_anon_1: c"Not Found".as_ptr() as *mut i8,
// REWRITES-DAG:             __slate_anon_2: 0.99,
// REWRITES-DAG:         },
// REWRITES-DAG:         {{anon_[0-9]+}} {
// REWRITES-DAG:             code: 500,
// REWRITES-DAG:             __slate_anon_1: c"Internal Server Error".as_ptr() as *mut i8,
// REWRITES-DAG:             __slate_anon_2: 0.85,
// REWRITES-DAG:         },
// REWRITES-DAG:         {{anon_[0-9]+}} {
// REWRITES-DAG:             code: 200,
// REWRITES-DAG:             __slate_anon_1: c"OK".as_ptr() as *mut i8,
// REWRITES-DAG:             __slate_anon_2: 1.0,
// REWRITES-DAG:         },
// REWRITES-DAG:     ];
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = error_log[0].code;
// REWRITES-DAG:     unsafe { printf(c"%d\n".as_ptr(), {{__v[0-9]+}}) };
// REWRITES-DAG:     std::process::exit(0 as i32);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
