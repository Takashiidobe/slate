#include <stddef.h>
#include <stdio.h>

struct ld_box {
  char        tag;
  long double value;
  int         tail;
};

union ld_union {
  long double ld;
  char        bytes[sizeof(long double)];
};

int main(void) {
  printf("%zu %zu\n", sizeof(long double), _Alignof(long double));
  printf("%zu %zu %zu %zu\n", sizeof(struct ld_box), _Alignof(struct ld_box),
         offsetof(struct ld_box, value), offsetof(struct ld_box, tail));
  printf("%zu %zu\n", sizeof(union ld_union), _Alignof(union ld_union));
  return 0;
}
// REWRITES-MACOS-DAG: value: f64
// REWRITES-MACOS-DAG: ld: f64
// REWRITES-MACOS: let _v{{[0-9]+}}: u64 = 8;
// REWRITES-MACOS-NEXT: let _v{{[0-9]+}}: u64 = 8;
// REWRITES-MACOS-DAG: std::mem::offset_of!(ld_box, value) as u64
// REWRITES-MACOS-NOT: LongDouble
