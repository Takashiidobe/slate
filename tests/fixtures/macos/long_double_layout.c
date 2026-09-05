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

// @rewrite-fn-begin
int main(void) {
  printf("%zu %zu\n", sizeof(long double), _Alignof(long double));
  printf("%zu %zu %zu %zu\n", sizeof(struct ld_box), _Alignof(struct ld_box),
         offsetof(struct ld_box, value), offsetof(struct ld_box, tail));
  printf("%zu %zu\n", sizeof(union ld_union), _Alignof(union ld_union));
  return 0;
}
// @rewrite-fn-end

// SLATE-FILECHECK-BEGIN rewrites-macos
// REWRITES-MACOS-DAG: fn main() {
// REWRITES-MACOS-DAG:     unsafe { printf(c"%zu %zu\n".as_ptr(), 8 as u64, 8 as u64) };
// REWRITES-MACOS-DAG:     unsafe {
// REWRITES-MACOS-DAG:         printf(
// REWRITES-MACOS-DAG:             c"%zu %zu %zu %zu\n".as_ptr(),
// REWRITES-MACOS-DAG:             std::mem::size_of::<ld_box>() as u64,
// REWRITES-MACOS-DAG:             std::mem::align_of::<ld_box>() as u64,
// REWRITES-MACOS-DAG:             std::mem::offset_of!(ld_box, value) as u64,
// REWRITES-MACOS-DAG:             std::mem::offset_of!(ld_box, tail) as u64,
// REWRITES-MACOS-DAG:         )
// REWRITES-MACOS-DAG:     };
// REWRITES-MACOS-DAG:     unsafe {
// REWRITES-MACOS-DAG:         printf(
// REWRITES-MACOS-DAG:             c"%zu %zu\n".as_ptr(),
// REWRITES-MACOS-DAG:             std::mem::size_of::<ld_union>() as u64,
// REWRITES-MACOS-DAG:             std::mem::align_of::<ld_union>() as u64,
// REWRITES-MACOS-DAG:         )
// REWRITES-MACOS-DAG:     };
// REWRITES-MACOS-DAG:     std::process::exit(0 as i32);
// REWRITES-MACOS-DAG: }
// SLATE-FILECHECK-END rewrites-macos
