#include <string.h>

int main(void) {
  char buf[8];
  char *p = buf;
  // @lowering-begin
  // @rewrite-begin
  memset((void *)p, 0, 8);
  // @rewrite-end
  // @lowering-end
  return p[0];
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG: let {{_v[0-9]+}}: u64 = 8;
// LOWERING-DAG: let {{_v[0-9]+}}: *mut core::ffi::c_void =
// LOWERING-DAG: unsafe { memset({{_v[0-9]+}} as *mut core::ffi::c_void, {{_v[0-9]+}} as i32, {{_v[0-9]+}} as usize) };
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: unsafe { std::ptr::write_bytes({{_v[0-9]+}} as *mut u8, (0 as i32) as u8, (8 as u64) as usize) };
// SLATE-FILECHECK-END rewrites
