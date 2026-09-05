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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = (x.__bitfield_0.a() as i32) << 22 >> 22;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} as i32) << 22 >> 22;
// COMMON-LOWERING-DAG: x.__bitfield_0.set_a(({{__v[0-9]+}} as i32) << 22 >> 22);
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = (x.__bitfield_0.b() as i32) << 22 >> 22;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} as i32) << 22 >> 22;
// COMMON-LOWERING-DAG: x.__bitfield_0.set_b(({{__v[0-9]+}} as i32) << 22 >> 22);
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = (x.__bitfield_0.c() as i32) << 22 >> 22;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} as i32) << 22 >> 22;
// COMMON-LOWERING-DAG: x.__bitfield_0.set_c(({{__v[0-9]+}} as i32) << 22 >> 22);
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: u64 = 4;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = (x.__bitfield_0.a() as i32) << 22 >> 22;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} as i32) << 22 >> 22;
// COMMON-LOWERING-DAG: x.__bitfield_0.set_a(({{__v[0-9]+}} as i32) << 22 >> 22);
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = (x.__bitfield_0.b() as i32) << 22 >> 22;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} as i32) << 22 >> 22;
// COMMON-LOWERING-DAG: x.__bitfield_0.set_b(({{__v[0-9]+}} as i32) << 22 >> 22);
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = (x.__bitfield_0.c() as i32) << 22 >> 22;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} as i32) << 22 >> 22;
// COMMON-LOWERING-DAG: x.__bitfield_0.set_c(({{__v[0-9]+}} as i32) << 22 >> 22);
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = b"%d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: *mut u8 = b"%d %d %d %d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: *mut u8 = b"%d %d %d\n\0".as_ptr() as *mut u8;
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: let {{__v[0-9]+}}: i32 = (x.__bitfield_0.a() as i32) << 22 >> 22;
// COMMON-REWRITES-DAG: let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-REWRITES-DAG: x.__bitfield_0.set_a(({{__v[0-9]+}} as i32) << 22 >> 22);
// COMMON-REWRITES-DAG: let {{__v[0-9]+}}: i32 = (x.__bitfield_0.b() as i32) << 22 >> 22;
// COMMON-REWRITES-DAG: let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-REWRITES-DAG: x.__bitfield_0.set_b(({{__v[0-9]+}} as i32) << 22 >> 22);
// COMMON-REWRITES-DAG: let {{__v[0-9]+}}: i32 = (x.__bitfield_0.c() as i32) << 22 >> 22;
// COMMON-REWRITES-DAG: let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-REWRITES-DAG: x.__bitfield_0.set_c(({{__v[0-9]+}} as i32) << 22 >> 22);
// COMMON-REWRITES-DAG: unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}, 4 as u64) };
// COMMON-REWRITES-DAG: let {{__v[0-9]+}}: i32 = (x.__bitfield_0.a() as i32) << 22 >> 22;
// COMMON-REWRITES-DAG: let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-REWRITES-DAG: let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} as i32) << 22 >> 22;
// COMMON-REWRITES-DAG: x.__bitfield_0.set_a(({{__v[0-9]+}} as i32) << 22 >> 22);
// COMMON-REWRITES-DAG: let {{__v[0-9]+}}: i32 = (x.__bitfield_0.b() as i32) << 22 >> 22;
// COMMON-REWRITES-DAG: let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-REWRITES-DAG: let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} as i32) << 22 >> 22;
// COMMON-REWRITES-DAG: x.__bitfield_0.set_b(({{__v[0-9]+}} as i32) << 22 >> 22);
// COMMON-REWRITES-DAG: let {{__v[0-9]+}}: i32 = (x.__bitfield_0.c() as i32) << 22 >> 22;
// COMMON-REWRITES-DAG: let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-REWRITES-DAG: let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} as i32) << 22 >> 22;
// COMMON-REWRITES-DAG: x.__bitfield_0.set_c(({{__v[0-9]+}} as i32) << 22 >> 22);
// COMMON-REWRITES-DAG: unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = c"%d %d %d %d\n".as_ptr() as *mut i8;
// REWRITES-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = c"%d %d %d\n".as_ptr() as *mut i8;
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-DAG: let {{__v[0-9]+}}: *mut u8 = c"%d %d %d %d\n".as_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-DAG: let {{__v[0-9]+}}: *mut u8 = c"%d %d %d\n".as_ptr() as *mut u8;
// SLATE-FILECHECK-END rewrites-aarch64-gnu
