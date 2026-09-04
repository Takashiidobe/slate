#include <stdio.h>

struct Bits {
  unsigned a : 3;
  int      b : 5;
  unsigned c : 1;
  unsigned d : 12;
};

struct Wide {
  unsigned long long x : 40;
  long long          y : 40;
};

int main(void) {
  struct Bits s;
  // @lowering-begin
  // @rewrite-begin
  s.a = 5;
  s.b = -3;
  s.c = 1;
  s.d = 4000;
  printf("%u %d %u %u\n", s.a, s.b, s.c, s.d);

  s.a = 13;
  s.b = 20;
  s.c = 3;
  printf("%u %d %u\n", s.a, s.b, s.c);

  s.a += 4;
  s.b -= 1;
  printf("%u %d\n", s.a, s.b);

  s.a = 7;
  s.b = 15;
  s.c = 0;
  s.d = 4095;
  printf("%u %d %u %u\n", s.a, s.b, s.c, s.d);
  // @rewrite-end
  // @lowering-end

  struct Wide w;
  // @lowering-begin
  // @rewrite-begin
  w.x = 1099511627775ULL;
  w.y = -500000;
  printf("%llu %lld\n", w.x, w.y);
  w.x += 1;
  printf("%llu\n", w.x);
  // @rewrite-end
  // @lowering-end

  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = 5;
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = ({{_v[0-9]+}} as u32) << 29 >> 29;
// LOWERING-DAG: s.__bitfield_0.set_a(({{_v[0-9]+}} as u32) << 29 >> 29);
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = -3;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = ({{_v[0-9]+}} as i32) << 27 >> 27;
// LOWERING-DAG: s.__bitfield_0.set_b(({{_v[0-9]+}} as i32) << 27 >> 27);
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = 1;
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = ({{_v[0-9]+}} as u32) << 31 >> 31;
// LOWERING-DAG: s.__bitfield_0.set_c(({{_v[0-9]+}} as u32) << 31 >> 31);
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = 4000;
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = ({{_v[0-9]+}} as u32) << 20 >> 20;
// LOWERING-DAG: s.__bitfield_0.set_d(({{_v[0-9]+}} as u32) << 20 >> 20);
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i8 = b"%u %d %u %u\n\0".as_ptr() as *mut i8;
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = (s.__bitfield_0.a() as u32) << 29 >> 29;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = (s.__bitfield_0.b() as i32) << 27 >> 27;
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = (s.__bitfield_0.c() as u32) << 31 >> 31;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = (s.__bitfield_0.d() as u32) << 20 >> 20;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = 13;
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = ({{_v[0-9]+}} as u32) << 29 >> 29;
// LOWERING-DAG: s.__bitfield_0.set_a(({{_v[0-9]+}} as u32) << 29 >> 29);
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = 20;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = ({{_v[0-9]+}} as i32) << 27 >> 27;
// LOWERING-DAG: s.__bitfield_0.set_b(({{_v[0-9]+}} as i32) << 27 >> 27);
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = 3;
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = ({{_v[0-9]+}} as u32) << 31 >> 31;
// LOWERING-DAG: s.__bitfield_0.set_c(({{_v[0-9]+}} as u32) << 31 >> 31);
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i8 = b"%u %d %u\n\0".as_ptr() as *mut i8;
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = (s.__bitfield_0.a() as u32) << 29 >> 29;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = (s.__bitfield_0.b() as i32) << 27 >> 27;
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = (s.__bitfield_0.c() as u32) << 31 >> 31;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = 4;
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = (s.__bitfield_0.a() as u32) << 29 >> 29;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} as u32;
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = ({{_v[0-9]+}} as u32) << 29 >> 29;
// LOWERING-DAG: s.__bitfield_0.set_a(({{_v[0-9]+}} as u32) << 29 >> 29);
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = 1;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = (s.__bitfield_0.b() as i32) << 27 >> 27;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = ({{_v[0-9]+}} as i32) << 27 >> 27;
// LOWERING-DAG: s.__bitfield_0.set_b(({{_v[0-9]+}} as i32) << 27 >> 27);
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i8 = b"%u %d\n\0".as_ptr() as *mut i8;
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = (s.__bitfield_0.a() as u32) << 29 >> 29;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = (s.__bitfield_0.b() as i32) << 27 >> 27;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = 7;
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = ({{_v[0-9]+}} as u32) << 29 >> 29;
// LOWERING-DAG: s.__bitfield_0.set_a(({{_v[0-9]+}} as u32) << 29 >> 29);
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = 15;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = ({{_v[0-9]+}} as i32) << 27 >> 27;
// LOWERING-DAG: s.__bitfield_0.set_b(({{_v[0-9]+}} as i32) << 27 >> 27);
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = 0;
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = ({{_v[0-9]+}} as u32) << 31 >> 31;
// LOWERING-DAG: s.__bitfield_0.set_c(({{_v[0-9]+}} as u32) << 31 >> 31);
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = 4095;
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = ({{_v[0-9]+}} as u32) << 20 >> 20;
// LOWERING-DAG: s.__bitfield_0.set_d(({{_v[0-9]+}} as u32) << 20 >> 20);
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i8 = b"%u %d %u %u\n\0".as_ptr() as *mut i8;
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = (s.__bitfield_0.a() as u32) << 29 >> 29;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = (s.__bitfield_0.b() as i32) << 27 >> 27;
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = (s.__bitfield_0.c() as u32) << 31 >> 31;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = (s.__bitfield_0.d() as u32) << 20 >> 20;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-DAG: let {{_v[0-9]+}}: u64 = 1099511627775u64;
// LOWERING-DAG: let {{_v[0-9]+}}: u64 = ({{_v[0-9]+}} as u64) << 24 >> 24;
// LOWERING-DAG: w.__bitfield_0.set_x(({{_v[0-9]+}} as u64) << 24 >> 24);
// LOWERING-DAG: let {{_v[0-9]+}}: i64 = -500000;
// LOWERING-DAG: let {{_v[0-9]+}}: i64 = ({{_v[0-9]+}} as i64) << 24 >> 24;
// LOWERING-DAG: w.__bitfield_1.set_y(({{_v[0-9]+}} as i64) << 24 >> 24);
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i8 = b"%llu %lld\n\0".as_ptr() as *mut i8;
// LOWERING-DAG: let {{_v[0-9]+}}: u64 = (w.__bitfield_0.x() as u64) << 24 >> 24;
// LOWERING-DAG: let {{_v[0-9]+}}: i64 = (w.__bitfield_1.y() as i64) << 24 >> 24;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-DAG: let {{_v[0-9]+}}: u64 = 1;
// LOWERING-DAG: let {{_v[0-9]+}}: u64 = (w.__bitfield_0.x() as u64) << 24 >> 24;
// LOWERING-DAG: let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: u64 = ({{_v[0-9]+}} as u64) << 24 >> 24;
// LOWERING-DAG: w.__bitfield_0.set_x(({{_v[0-9]+}} as u64) << 24 >> 24);
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i8 = b"%llu\n\0".as_ptr() as *mut i8;
// LOWERING-DAG: let {{_v[0-9]+}}: u64 = (w.__bitfield_0.x() as u64) << 24 >> 24;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: s.__bitfield_0.set_a((5 as u32) << 29 >> 29);
// REWRITES-DAG: s.__bitfield_0.set_b((-3 as i32) << 27 >> 27);
// REWRITES-DAG: s.__bitfield_0.set_c((1 as u32) << 31 >> 31);
// REWRITES-DAG: s.__bitfield_0.set_d((4000 as u32) << 20 >> 20);
// REWRITES-DAG: let {{_v[0-9]+}}: *mut i8 = c"%u %d %u %u\n".as_ptr() as *mut i8;
// REWRITES-DAG: let {{_v[0-9]+}}: u32 = (s.__bitfield_0.a() as u32) << 29 >> 29;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = (s.__bitfield_0.b() as i32) << 27 >> 27;
// REWRITES-DAG: let {{_v[0-9]+}}: u32 = (s.__bitfield_0.c() as u32) << 31 >> 31;
// REWRITES-DAG: unsafe {
// REWRITES-DAG:     printf(
// REWRITES-DAG:         {{_v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-DAG:         {{_v[0-9]+}},
// REWRITES-DAG:         {{_v[0-9]+}},
// REWRITES-DAG:         {{_v[0-9]+}} as i32,
// REWRITES-DAG:         ((s.__bitfield_0.d() as u32) << 20 >> 20) as i32,
// REWRITES-DAG:     )
// REWRITES-DAG: };
// REWRITES-DAG: s.__bitfield_0.set_a((13 as u32) << 29 >> 29);
// REWRITES-DAG: s.__bitfield_0.set_b((20 as i32) << 27 >> 27);
// REWRITES-DAG: s.__bitfield_0.set_c((3 as u32) << 31 >> 31);
// REWRITES-DAG: let {{_v[0-9]+}}: *mut i8 = c"%u %d %u\n".as_ptr() as *mut i8;
// REWRITES-DAG: let {{_v[0-9]+}}: u32 = (s.__bitfield_0.a() as u32) << 29 >> 29;
// REWRITES-DAG: unsafe {
// REWRITES-DAG:     printf(
// REWRITES-DAG:         {{_v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-DAG:         {{_v[0-9]+}} as i32,
// REWRITES-DAG:         (s.__bitfield_0.b() as i32) << 27 >> 27,
// REWRITES-DAG:         ((s.__bitfield_0.c() as u32) << 31 >> 31) as i32,
// REWRITES-DAG:     )
// REWRITES-DAG: };
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = 4;
// REWRITES-DAG: let {{_v[0-9]+}}: u32 = (s.__bitfield_0.a() as u32) << 29 >> 29;
// REWRITES-DAG: s.__bitfield_0
// REWRITES-DAG:     .set_a(((({{_v[0-9]+}} as i32) + {{_v[0-9]+}}) as u32) << 29 >> 29);
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = (s.__bitfield_0.b() as i32) << 27 >> 27;
// REWRITES-DAG: s.__bitfield_0.set_b((({{_v[0-9]+}} - {{_v[0-9]+}}) as i32) << 27 >> 27);
// REWRITES-DAG: let {{_v[0-9]+}}: *mut i8 = c"%u %d\n".as_ptr() as *mut i8;
// REWRITES-DAG: let {{_v[0-9]+}}: u32 = (s.__bitfield_0.a() as u32) << 29 >> 29;
// REWRITES-DAG: unsafe {
// REWRITES-DAG:     printf(
// REWRITES-DAG:         {{_v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-DAG:         {{_v[0-9]+}} as i32,
// REWRITES-DAG:         (s.__bitfield_0.b() as i32) << 27 >> 27,
// REWRITES-DAG:     )
// REWRITES-DAG: };
// REWRITES-DAG: s.__bitfield_0.set_a((7 as u32) << 29 >> 29);
// REWRITES-DAG: s.__bitfield_0.set_b((15 as i32) << 27 >> 27);
// REWRITES-DAG: s.__bitfield_0.set_c((0 as u32) << 31 >> 31);
// REWRITES-DAG: s.__bitfield_0.set_d((4095 as u32) << 20 >> 20);
// REWRITES-DAG: let {{_v[0-9]+}}: *mut i8 = c"%u %d %u %u\n".as_ptr() as *mut i8;
// REWRITES-DAG: let {{_v[0-9]+}}: u32 = (s.__bitfield_0.a() as u32) << 29 >> 29;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = (s.__bitfield_0.b() as i32) << 27 >> 27;
// REWRITES-DAG: let {{_v[0-9]+}}: u32 = (s.__bitfield_0.c() as u32) << 31 >> 31;
// REWRITES-DAG: unsafe {
// REWRITES-DAG:     printf(
// REWRITES-DAG:         {{_v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-DAG:         {{_v[0-9]+}},
// REWRITES-DAG:         {{_v[0-9]+}},
// REWRITES-DAG:         {{_v[0-9]+}} as i32,
// REWRITES-DAG:         ((s.__bitfield_0.d() as u32) << 20 >> 20) as i32,
// REWRITES-DAG:     )
// REWRITES-DAG: };
// REWRITES-DAG: let {{_v[0-9]+}}: u64 = 1099511627775u64;
// REWRITES-DAG: w.__bitfield_0.set_x(({{_v[0-9]+}} as u64) << 24 >> 24);
// REWRITES-DAG: w.__bitfield_1.set_y((-500000 as i64) << 24 >> 24);
// REWRITES-DAG: unsafe {
// REWRITES-DAG:     printf(
// REWRITES-DAG:         c"%llu %lld\n".as_ptr(),
// REWRITES-DAG:         (w.__bitfield_0.x() as u64) << 24 >> 24,
// REWRITES-DAG:         (w.__bitfield_1.y() as i64) << 24 >> 24,
// REWRITES-DAG:     )
// REWRITES-DAG: };
// REWRITES-DAG: let {{_v[0-9]+}}: u64 = 1;
// REWRITES-DAG: let {{_v[0-9]+}}: u64 = (w.__bitfield_0.x() as u64) << 24 >> 24;
// REWRITES-DAG: w.__bitfield_0.set_x((({{_v[0-9]+}} + {{_v[0-9]+}}) as u64) << 24 >> 24);
// REWRITES-DAG: unsafe { printf(c"%llu\n".as_ptr(), (w.__bitfield_0.x() as u64) << 24 >> 24) };
// SLATE-FILECHECK-END rewrites
