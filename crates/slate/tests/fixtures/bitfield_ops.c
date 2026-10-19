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

// @check-fn-begin
int main(void) {
  struct Bits s;
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

  struct Wide w;
  w.x = 1099511627775ULL;
  w.y = -500000;
  printf("%llu %lld\n", w.x, w.y);
  w.x += 1;
  printf("%llu\n", w.x);

  return 0;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut s: Bits = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     s.__slate_bits_0.__set_a(5u32);
// CHECK-NEXT:     s.__slate_bits_0.__set_b(-3i32);
// CHECK-NEXT:     s.__slate_bits_0.__set_c(1u32);
// CHECK-NEXT:     s.__slate_bits_0.__set_d(4000u32);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%u %d %u %u\n".as_ptr(),
// CHECK-NEXT:             s.__slate_bits_0.__get_a() as i32,
// CHECK-NEXT:             s.__slate_bits_0.__get_b(),
// CHECK-NEXT:             s.__slate_bits_0.__get_c() as i32,
// CHECK-NEXT:             s.__slate_bits_0.__get_d() as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     s.__slate_bits_0.__set_a(13u32);
// CHECK-NEXT:     s.__slate_bits_0.__set_b(20i32);
// CHECK-NEXT:     s.__slate_bits_0.__set_c(3u32);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%u %d %u\n".as_ptr(),
// CHECK-NEXT:             s.__slate_bits_0.__get_a() as i32,
// CHECK-NEXT:             s.__slate_bits_0.__get_b(),
// CHECK-NEXT:             s.__slate_bits_0.__get_c() as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: u32 = s.__slate_bits_0.__get_a();
// CHECK-NEXT:     s.__slate_bits_0.__set_a((([[V_0]] as i32) + 4) as u32);
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: i32 = s.__slate_bits_0.__get_b();
// CHECK-NEXT:     s.__slate_bits_0.__set_b([[V_1]] - 1);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%u %d\n".as_ptr(),
// CHECK-NEXT:             s.__slate_bits_0.__get_a() as i32,
// CHECK-NEXT:             s.__slate_bits_0.__get_b(),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     s.__slate_bits_0.__set_a(7u32);
// CHECK-NEXT:     s.__slate_bits_0.__set_b(15i32);
// CHECK-NEXT:     s.__slate_bits_0.__set_c(0u32);
// CHECK-NEXT:     s.__slate_bits_0.__set_d(4095u32);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%u %d %u %u\n".as_ptr(),
// CHECK-NEXT:             s.__slate_bits_0.__get_a() as i32,
// CHECK-NEXT:             s.__slate_bits_0.__get_b(),
// CHECK-NEXT:             s.__slate_bits_0.__get_c() as i32,
// CHECK-NEXT:             s.__slate_bits_0.__get_d() as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut w: Wide = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     w.__slate_bits_0.__set_x(1099511627775u64);
// CHECK-NEXT:     w.__slate_bits_1.__set_y(-500000i64);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%llu %lld\n".as_ptr(),
// CHECK-NEXT:             w.__slate_bits_0.__get_x(),
// CHECK-NEXT:             w.__slate_bits_1.__get_y(),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let [[V_2:__v[0-9]+]]: u64 = w.__slate_bits_0.__get_x();
// CHECK-NEXT:     let [[V_3:__v[0-9]+]]: u64 = [[V_2]].wrapping_add(1);
// CHECK-NEXT:     w.__slate_bits_0.__set_x([[V_3]]);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu\n".as_ptr(), w.__slate_bits_0.__get_x()) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK: {{^}}}
// SLATE-FILECHECK-END
