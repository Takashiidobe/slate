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
// REWRITES-MACOS-DAG:     let {{__v[0-9]+}}: *mut i8 = c"%zu %zu\n".as_ptr() as *mut i8;
// REWRITES-MACOS-DAG:     let {{__v[0-9]+}}: u64 = 8;
// REWRITES-MACOS-DAG:     let {{__v[0-9]+}}: u64 = 8;
// REWRITES-MACOS-DAG:     unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// REWRITES-MACOS-DAG:     let {{__v[0-9]+}}: *mut i8 = c"%zu %zu %zu %zu\n".as_ptr() as *mut i8;
// REWRITES-MACOS-DAG:     let {{__v[0-9]+}}: u64 = std::mem::size_of::<ld_box>() as u64;
// REWRITES-MACOS-DAG:     let {{__v[0-9]+}}: u64 = std::mem::align_of::<ld_box>() as u64;
// REWRITES-MACOS-DAG:     let {{__v[0-9]+}}: u64 = std::mem::offset_of!(ld_box, value) as u64;
// REWRITES-MACOS-DAG:     let {{__v[0-9]+}}: u64 = std::mem::offset_of!(ld_box, tail) as u64;
// REWRITES-MACOS-DAG:     unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// REWRITES-MACOS-DAG:     let {{__v[0-9]+}}: *mut i8 = c"%zu %zu\n".as_ptr() as *mut i8;
// REWRITES-MACOS-DAG:     let {{__v[0-9]+}}: u64 = std::mem::size_of::<ld_union>() as u64;
// REWRITES-MACOS-DAG:     let {{__v[0-9]+}}: u64 = std::mem::align_of::<ld_union>() as u64;
// REWRITES-MACOS-DAG:     unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// REWRITES-MACOS-DAG:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-MACOS-DAG:     std::process::exit({{__v[0-9]+}} as i32);
// REWRITES-MACOS-DAG: }
// SLATE-FILECHECK-END rewrites-macos
