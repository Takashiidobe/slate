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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: u64 = 8;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: *mut core::ffi::c_void =
// COMMON-LOWERING-DAG:     unsafe { memset({{__v[0-9]+}} as *mut core::ffi::c_void, {{__v[0-9]+}} as i32, {{__v[0-9]+}} as usize) };
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-DAG: unsafe { buf[(0usize..8usize)].fill((0 as i32) as i8) };
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-DAG: unsafe { buf[(0usize..8usize)].fill((0 as i32) as u8) };
// SLATE-FILECHECK-END rewrites-aarch64-gnu
