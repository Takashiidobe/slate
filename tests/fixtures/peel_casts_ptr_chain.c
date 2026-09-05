#include <string.h>

int main(void) {
  char  buf[8];
  char *p = buf;
  // @lowering-begin
  // @rewrite-begin
  memset((void *)p, 0, 8);
  // @rewrite-end
  // @lowering-end
  return p[0];
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG: let {{__v[0-9]+}}: u64 = 8;
// LOWERING-DAG: let {{__v[0-9]+}}: *mut core::ffi::c_void =
// LOWERING-DAG:     unsafe { memset({{__v[0-9]+}} as *mut core::ffi::c_void, {{__v[0-9]+}} as i32, {{__v[0-9]+}} as usize) };
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-X86_64-GNU-DAG: unsafe { buf[(0usize..8usize)].fill((0 as i32) as i8) };
// REWRITES-AARCH64-GNU-DAG: unsafe { buf[(0usize..8usize)].fill((0 as i32) as u8) };
// SLATE-FILECHECK-END rewrites
