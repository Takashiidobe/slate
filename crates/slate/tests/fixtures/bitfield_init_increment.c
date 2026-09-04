#include <stdio.h>

typedef struct {
  int a : 10;
  int b : 10;
  int c : 10;
} Bits;

int main(void) {
  Bits x = {1, 2, 3};
  // @lowering-begin
  // @rewrite-begin
  printf("%d %d %d %d\n", x.a++, x.b++, x.c++, sizeof(x));
  printf("%d %d %d\n", ++x.a, ++x.b, ++x.c);
  // @rewrite-end
  // @lowering-end
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i8 = b"%d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = (x.__bitfield_0.a() as i32) << 22 >> 22;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = ({{_v[0-9]+}} as i32) << 22 >> 22;
// LOWERING-DAG: x.__bitfield_0.set_a(({{_v[0-9]+}} as i32) << 22 >> 22);
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = (x.__bitfield_0.b() as i32) << 22 >> 22;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = ({{_v[0-9]+}} as i32) << 22 >> 22;
// LOWERING-DAG: x.__bitfield_0.set_b(({{_v[0-9]+}} as i32) << 22 >> 22);
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = (x.__bitfield_0.c() as i32) << 22 >> 22;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = ({{_v[0-9]+}} as i32) << 22 >> 22;
// LOWERING-DAG: x.__bitfield_0.set_c(({{_v[0-9]+}} as i32) << 22 >> 22);
// LOWERING-DAG: let {{_v[0-9]+}}: u64 = 4;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = (x.__bitfield_0.a() as i32) << 22 >> 22;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = ({{_v[0-9]+}} as i32) << 22 >> 22;
// LOWERING-DAG: x.__bitfield_0.set_a(({{_v[0-9]+}} as i32) << 22 >> 22);
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = (x.__bitfield_0.b() as i32) << 22 >> 22;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = ({{_v[0-9]+}} as i32) << 22 >> 22;
// LOWERING-DAG: x.__bitfield_0.set_b(({{_v[0-9]+}} as i32) << 22 >> 22);
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = (x.__bitfield_0.c() as i32) << 22 >> 22;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = ({{_v[0-9]+}} as i32) << 22 >> 22;
// LOWERING-DAG: x.__bitfield_0.set_c(({{_v[0-9]+}} as i32) << 22 >> 22);
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: let {{_v[0-9]+}}: *mut i8 = c"%d %d %d %d\n".as_ptr() as *mut i8;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = (x.__bitfield_0.a() as i32) << 22 >> 22;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// REWRITES-DAG: x.__bitfield_0.set_a(({{_v[0-9]+}} as i32) << 22 >> 22);
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = (x.__bitfield_0.b() as i32) << 22 >> 22;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// REWRITES-DAG: x.__bitfield_0.set_b(({{_v[0-9]+}} as i32) << 22 >> 22);
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = (x.__bitfield_0.c() as i32) << 22 >> 22;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// REWRITES-DAG: x.__bitfield_0.set_c(({{_v[0-9]+}} as i32) << 22 >> 22);
// REWRITES-DAG: unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, 4 as u64) };
// REWRITES-DAG: let {{_v[0-9]+}}: *mut i8 = c"%d %d %d\n".as_ptr() as *mut i8;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = (x.__bitfield_0.a() as i32) << 22 >> 22;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = ({{_v[0-9]+}} as i32) << 22 >> 22;
// REWRITES-DAG: x.__bitfield_0.set_a(({{_v[0-9]+}} as i32) << 22 >> 22);
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = (x.__bitfield_0.b() as i32) << 22 >> 22;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = ({{_v[0-9]+}} as i32) << 22 >> 22;
// REWRITES-DAG: x.__bitfield_0.set_b(({{_v[0-9]+}} as i32) << 22 >> 22);
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = (x.__bitfield_0.c() as i32) << 22 >> 22;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = ({{_v[0-9]+}} as i32) << 22 >> 22;
// REWRITES-DAG: x.__bitfield_0.set_c(({{_v[0-9]+}} as i32) << 22 >> 22);
// REWRITES-DAG: unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// SLATE-FILECHECK-END rewrites
