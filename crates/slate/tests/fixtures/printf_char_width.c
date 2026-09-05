#include <stdio.h>

int main(void) {
  // @lowering-begin
  printf("%3c|%-3c|%c\n", 'a', 'b', 'c');
  // @lowering-end
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: let {{__v[0-9]+}}: *mut i8 = b"%3c|%-3c|%c\n\0".as_ptr() as *mut i8;
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = 97;
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = 98;
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = 99;
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// SLATE-FILECHECK-END lowering
