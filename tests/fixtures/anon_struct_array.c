#include <stdio.h>

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
// REWRITES-DAG: struct __slate_anonymous_struct_0(i32, *mut i8, f64);
// REWRITES-DAG: error_log: aligned::Aligned<aligned::A16, [__slate_anonymous_struct_0; 3]>
// REWRITES-DAG: __slate_anonymous_struct_0(404,
// REWRITES-DAG: error_log[0].0
// REWRITES-NOT: anon_0
// REWRITES-NOT: error_log[0].code
