#include <stdio.h>

int main(void) {
  // @rewrite-begin
  // @lowering-begin
  puts("hello");
  // @lowering-end
  // @rewrite-end
  printf("%d\n", 42);
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i8 = b"hello\0".as_ptr() as *mut i8;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = unsafe { puts({{_v[0-9]+}} as *const i8) };
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: unsafe { puts(b"hello\0".as_ptr() as *const i8) };
// SLATE-FILECHECK-END rewrites
