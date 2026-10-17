#include <limits.h>
#include <stdio.h>

struct Counter {
  int hits;
  double total;
};

static int global_count = 10;
static int calls;

static int *next_slot(int *slots) {
  calls++;
  return slots + calls;
}

int main(void) {
  unsigned u = UINT_MAX;
  unsigned before = u++;
  unsigned after  = u;
  unsigned back   = --u;
  printf("%u %u %u\n", before, after, back);

  signed char sc = 127;
  sc++;
  printf("%d\n", sc);
  unsigned short us = 0;
  int old_us = us--;
  printf("%d %u\n", old_us, (unsigned)us);

  long long ll = LLONG_MIN + 1;
  long long old_ll = ll--;
  printf("%lld %lld\n", old_ll, ll);

  double d = 1.5;
  double d1 = d++;
  double d2 = ++d;
  double d3 = d--;
  printf("%g %g %g %g\n", d1, d2, d3, d);
  float f = -0.5f;
  float f1 = ++f;
  float f2 = f++;
  printf("%g %g %g\n", f1, f2, f);

  int values[5] = {0, 10, 20, 30, 40};
  int *p        = values;
  int *old      = p++;
  int at_p = *p;
  int at_next = *++p;
  printf("%d %d %d\n", *old, at_p, at_next);
  int was = (int)(p-- - values);
  printf("%d %d\n", was, (int)(p - values));
  printf("%d\n", (*p)++);
  printf("%d\n", *p--);
  printf("%d\n", values[1]);

  int i = 1;
  int r = values[i++]++;
  printf("%d %d %d\n", r, i, values[1]);
  r = ++values[i--];
  printf("%d %d %d\n", r, i, values[2]);

  int slots[4] = {0, 0, 0, 0};
  (*next_slot(slots))++;
  ++*next_slot(slots);
  printf("%d %d %d %d calls=%d\n", slots[0], slots[1], slots[2], slots[3], calls);

  struct Counter counter = {0, 0.0};
  struct Counter *cp     = &counter;
  counter.hits++;
  ++cp->hits;
  cp->total++;
  int hits = cp->hits++;
  printf("%d %d %g\n", hits, counter.hits, counter.total);

  int g1 = global_count--;
  int g2 = --global_count;
  printf("%d %d\n", g1, g2);

  _Bool flag = 0;
  flag++;
  _Bool still = flag++;
  printf("%d %d\n", flag, still);
  flag--;
  printf("%d\n", flag);
  flag--;
  printf("%d\n", flag);

  int n = 3, total = 0;
  while (n--)
    total += n;
  printf("%d %d\n", n, total);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Counter {
// CHECK-NEXT:     hits: i32,
// CHECK-NEXT:     total: f64,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut global_count: i32 = 10 as i32;
// CHECK-NEXT: static mut calls: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn next_slot(slots: *mut i32) -> *mut i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         calls = (unsafe { calls }) + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { slots.offset((unsafe { calls }) as isize) }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut u: u32 = (2147483647 as u32).wrapping_mul(2 as u32).wrapping_add(1);
// CHECK-NEXT:     let mut before: u32 = 0;
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: u32 = u;
// CHECK-NEXT:     u = [[V_0]].wrapping_add(1);
// CHECK-NEXT:     before = [[V_0]];
// CHECK-NEXT:     let after: u32 = u;
// CHECK-NEXT:     let mut back: u32 = 0;
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: u32 = u.wrapping_sub(1);
// CHECK-NEXT:     u = [[V_1]];
// CHECK-NEXT:     back = [[V_1]];
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%u %u %u\n".as_ptr(), before, after, back) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut sc: i8 = 127;
// CHECK-NEXT:     sc = ((sc as i32) + 1) as i8;
// CHECK-NEXT:     println!("{}", sc as i32);
// CHECK-NEXT:     let mut us: u16 = 0;
// CHECK-NEXT:     let mut old_us: i32 = 0;
// CHECK-NEXT:     let [[V_2:__v[0-9]+]]: u16 = us;
// CHECK-NEXT:     us = (((([[V_2]] as u32) as i32) - 1) as i16) as u16;
// CHECK-NEXT:     old_us = ([[V_2]] as u32) as i32;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%d %u\n".as_ptr(), old_us, us as u32) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut ll: i64 = -9223372036854775807 - 1 + 1;
// CHECK-NEXT:     let mut old_ll: i64 = 0;
// CHECK-NEXT:     let [[V_3:__v[0-9]+]]: i64 = ll;
// CHECK-NEXT:     ll = [[V_3]] - 1;
// CHECK-NEXT:     old_ll = [[V_3]];
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld %lld\n".as_ptr(), old_ll, ll) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut d: f64 = 1.5f64;
// CHECK-NEXT:     let mut d1: f64 = 0 as f64;
// CHECK-NEXT:     let [[V_4:__v[0-9]+]]: f64 = d;
// CHECK-NEXT:     d = [[V_4]] + ((1 as i32) as f64);
// CHECK-NEXT:     d1 = [[V_4]];
// CHECK-NEXT:     let mut d2: f64 = 0 as f64;
// CHECK-NEXT:     let [[V_5:__v[0-9]+]]: f64 = d + ((1 as i32) as f64);
// CHECK-NEXT:     d = [[V_5]];
// CHECK-NEXT:     d2 = [[V_5]];
// CHECK-NEXT:     let mut d3: f64 = 0 as f64;
// CHECK-NEXT:     let [[V_6:__v[0-9]+]]: f64 = d;
// CHECK-NEXT:     d = [[V_6]] - ((1 as i32) as f64);
// CHECK-NEXT:     d3 = [[V_6]];
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%g %g %g %g\n".as_ptr(), d1, d2, d3, d) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut f: f32 = -0.5f32;
// CHECK-NEXT:     let mut f1: f32 = 0 as f32;
// CHECK-NEXT:     let [[V_7:__v[0-9]+]]: f32 = f + ((1 as i32) as f32);
// CHECK-NEXT:     f = [[V_7]];
// CHECK-NEXT:     f1 = [[V_7]];
// CHECK-NEXT:     let mut f2: f32 = 0 as f32;
// CHECK-NEXT:     let [[V_8:__v[0-9]+]]: f32 = f;
// CHECK-NEXT:     f = [[V_8]] + ((1 as i32) as f32);
// CHECK-NEXT:     f2 = [[V_8]];
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%g %g %g\n".as_ptr(), f1 as f64, f2 as f64, f as f64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut values: __SlateAlign16<[i32; 5]> = __SlateAlign16([
// CHECK-NEXT:         0 as i32,
// CHECK-NEXT:         10 as i32,
// CHECK-NEXT:         20 as i32,
// CHECK-NEXT:         30 as i32,
// CHECK-NEXT:         40 as i32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut p: *mut i32 = values.0.as_mut_ptr() as *mut i32;
// CHECK-NEXT:     let [[V_9:__v[0-9]+]]: *mut i32 = p;
// CHECK-NEXT:     p = unsafe { [[V_9]].offset(1 as isize) };
// CHECK-NEXT:     let at_p: i32 = unsafe { *p };
// CHECK-NEXT:     let mut at_next: i32 = 0;
// CHECK-NEXT:     let [[V_10:__v[0-9]+]]: *mut i32 = p;
// CHECK-NEXT:     let [[V_11:__v[0-9]+]]: *mut i32 = unsafe { [[V_10]].offset(1 as isize) };
// CHECK-NEXT:     p = [[V_11]];
// CHECK-NEXT:     at_next = unsafe { *[[V_11]] };
// CHECK-NEXT:     println!("{} {} {}", unsafe { * [[V_9]] }, at_p, at_next);
// CHECK-NEXT:     let mut was: i32 = 0;
// CHECK-NEXT:     let [[V_12:__v[0-9]+]]: *mut i32 = p;
// CHECK-NEXT:     p = unsafe { [[V_12]].offset(-(1 as isize)) };
// CHECK-NEXT:     was = ((unsafe { [[V_12]].offset_from(values.0.as_mut_ptr() as *mut i32) }) as i64)
// CHECK-NEXT:         as i32;
// CHECK-NEXT:     let __slate_printf_arg1 = ((unsafe {
// CHECK-NEXT:         p.offset_from(values.0.as_mut_ptr() as *mut i32)
// CHECK-NEXT:     }) as i64) as i32;
// CHECK-NEXT:     println!("{} {}", was, __slate_printf_arg1);
// CHECK-NEXT:     let [[V_13:__v[0-9]+]]: i32 = unsafe { *p };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *p = [[V_13]] + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("{}", [[V_13]]);
// CHECK-NEXT:     let [[V_14:__v[0-9]+]]: *mut i32 = p;
// CHECK-NEXT:     p = unsafe { [[V_14]].offset(-(1 as isize)) };
// CHECK-NEXT:     println!("{}", unsafe { * [[V_14]] });
// CHECK-NEXT:     println!("{}", values.0[1]);
// CHECK-NEXT:     let mut i: i32 = 1;
// CHECK-NEXT:     let mut r: i32 = 0;
// CHECK-NEXT:     let [[V_15:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:     i = [[V_15]] + 1;
// CHECK-NEXT:     let [[V_16:__v[0-9]+]]: *mut i32 = unsafe {
// CHECK-NEXT:         (values.0.as_mut_ptr() as *mut i32).offset([[V_15]] as isize)
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[V_17:__v[0-9]+]]: i32 = unsafe { *[[V_16]] };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[V_16]] = [[V_17]] + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     r = [[V_17]];
// CHECK-NEXT:     println!("{} {} {}", r, i, values.0[1]);
// CHECK-NEXT:     let [[V_18:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:     i = [[V_18]] - 1;
// CHECK-NEXT:     let [[V_19:__v[0-9]+]]: *mut i32 = unsafe {
// CHECK-NEXT:         (values.0.as_mut_ptr() as *mut i32).offset([[V_18]] as isize)
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[V_20:__v[0-9]+]]: i32 = (unsafe { *[[V_19]] }) + 1;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[V_19]] = [[V_20]];
// CHECK-NEXT:     }
// CHECK-NEXT:     r = [[V_20]];
// CHECK-NEXT:     println!("{} {} {}", r, i, values.0[2]);
// CHECK-NEXT:     let mut slots: __SlateAlign16<[i32; 4]> = __SlateAlign16([
// CHECK-NEXT:         0 as i32,
// CHECK-NEXT:         0 as i32,
// CHECK-NEXT:         0 as i32,
// CHECK-NEXT:         0 as i32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let [[V_21:__v[0-9]+]]: *mut i32 = next_slot(slots.0.as_mut_ptr() as *mut i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[V_21]] = (unsafe { *[[V_21]] }) + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     let [[V_22:__v[0-9]+]]: *mut i32 = next_slot(slots.0.as_mut_ptr() as *mut i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[V_22]] = (unsafe { *[[V_22]] }) + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {} calls={}", slots.0[0], slots.0[1], slots.0[2], slots.0[3], unsafe {
// CHECK-NEXT:         calls }
// CHECK-NEXT:     );
// CHECK-NEXT:     let mut counter: Counter = Counter { hits: 0, total: 0.0f64 };
// CHECK-NEXT:     let cp: *mut Counter = std::ptr::addr_of_mut!(counter);
// CHECK-NEXT:     counter.hits += 1;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*cp).hits = (unsafe { (*cp).hits }) + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*cp).total = (unsafe { (*cp).total }) + ((1 as i32) as f64);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut hits: i32 = 0;
// CHECK-NEXT:     let [[V_23:__v[0-9]+]]: i32 = unsafe { (*cp).hits };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*cp).hits = [[V_23]] + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     hits = [[V_23]];
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%d %d %g\n".as_ptr(), hits, counter.hits, counter.total) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut g1: i32 = 0;
// CHECK-NEXT:     let [[V_24:__v[0-9]+]]: i32 = unsafe { global_count };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         global_count = [[V_24]] - 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     g1 = [[V_24]];
// CHECK-NEXT:     let mut g2: i32 = 0;
// CHECK-NEXT:     let [[V_25:__v[0-9]+]]: i32 = (unsafe { global_count }) - 1;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         global_count = [[V_25]];
// CHECK-NEXT:     }
// CHECK-NEXT:     g2 = [[V_25]];
// CHECK-NEXT:     println!("{} {}", g1, g2);
// CHECK-NEXT:     let mut flag: bool = (0 as i32) != (0 as i32);
// CHECK-NEXT:     flag = (flag as i32) + 1 != 0;
// CHECK-NEXT:     let mut still: bool = false;
// CHECK-NEXT:     let [[V_26:__v[0-9]+]]: bool = flag;
// CHECK-NEXT:     flag = ([[V_26]] as i32) + 1 != 0;
// CHECK-NEXT:     still = [[V_26]];
// CHECK-NEXT:     println!("{} {}", flag as i32, still as i32);
// CHECK-NEXT:     flag = (flag as i32) - 1 != 0;
// CHECK-NEXT:     println!("{}", flag as i32);
// CHECK-NEXT:     flag = (flag as i32) - 1 != 0;
// CHECK-NEXT:     println!("{}", flag as i32);
// CHECK-NEXT:     let mut n: i32 = 3;
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let [[V_27:__v[0-9]+]]: i32 = n;
// CHECK-NEXT:         n = [[V_27]] - 1;
// CHECK-NEXT:         if !([[V_27]] != 0) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:         total += n;
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("{} {}", n, total);
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
