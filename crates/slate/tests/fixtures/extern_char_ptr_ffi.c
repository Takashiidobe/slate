#include <stdio.h>
#include <string.h>

int main(void) {
  // @rewrite-begin
  // @lowering-begin
  const char *msg = "hello";
  size_t      n   = strlen(msg);
  puts(msg);
  // @lowering-end
  // @rewrite-end
  printf("%zu\n", n);
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i8 = b"hello\0".as_ptr() as *mut i8;
// LOWERING-DAG: let {{_v[0-9]+}}: u64 = (unsafe { strlen({{_v[0-9]+}} as *const core::ffi::c_char) }) as u64;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = unsafe { puts({{_v[0-9]+}} as *const core::ffi::c_char) };
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: let {{_v[0-9]+}}: *mut i8 = c"hello".as_ptr() as *mut i8;
// REWRITES-DAG: let {{_v[0-9]+}}: u64 = (unsafe { "hello".len() as u64 }) as u64;
// REWRITES-DAG: unsafe { puts({{_v[0-9]+}} as *const core::ffi::c_char) };
// SLATE-FILECHECK-END rewrites
