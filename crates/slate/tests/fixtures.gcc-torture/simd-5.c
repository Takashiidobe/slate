/* Test saving and restoring of SIMD registers.  */

void abort(void);

typedef short Q __attribute__((vector_size(8)));

Q q1 = {1, 2}, q2 = {3, 4}, q3 = {5, 6}, q4 = {7, 8};

Q w1, w2, w3, w4;
Q z1, z2, z3, z4;

volatile int dummy;

void __attribute__((__noinline__)) func0(void) { dummy = 1; }

void __attribute__((__noinline__)) func1(void) {
  Q a, b;
  a  = q1 * q2;
  b  = q3 * q4;
  w1 = a;
  w2 = b;
  func0();
  w3 = a;
  w4 = b;
}

void __attribute__((__noinline__)) func2(void) {
  Q a, b;
  a  = q1 + q2;
  b  = q3 - q4;
  z1 = a;
  z2 = b;
  func1();
  z3 = a;
  z4 = b;
}

int main(void) {
  func2();

  if (__builtin_memcmp(&w1, &w3, sizeof(Q)) != 0)
    abort();
  if (__builtin_memcmp(&w2, &w4, sizeof(Q)) != 0)
    abort();
  if (__builtin_memcmp(&z1, &z3, sizeof(Q)) != 0)
    abort();
  if (__builtin_memcmp(&z2, &z4, sizeof(Q)) != 0)
    abort();

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(portable_simd)]
// CHECK-NEXT: #![feature(simd_ffi)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Test saving and restoring of SIMD registers.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     #[link_name = "memcmp"]
// CHECK-NEXT:     fn __builtin_memcmp([[V_0:__v[0-9]+]]: *const (), [[V_1:__v[0-9]+]]: *const (), [[V_2:__v[0-9]+]]: u64) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut q1: std::simd::Simd<i16, 4> = std::simd::Simd::from_array({
// CHECK-NEXT:     let mut [[T_0:__t[0-9]+]]: [i16; 4] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]][0] = (1 as i32) as i16;
// CHECK-NEXT:     [[T_0]][1] = (2 as i32) as i16;
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: });
// CHECK-EMPTY:
// CHECK-NEXT: static mut q2: std::simd::Simd<i16, 4> = std::simd::Simd::from_array({
// CHECK-NEXT:     let mut [[T_0]]: [i16; 4] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]][0] = (3 as i32) as i16;
// CHECK-NEXT:     [[T_0]][1] = (4 as i32) as i16;
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: });
// CHECK-EMPTY:
// CHECK-NEXT: static mut q3: std::simd::Simd<i16, 4> = std::simd::Simd::from_array({
// CHECK-NEXT:     let mut [[T_0]]: [i16; 4] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]][0] = (5 as i32) as i16;
// CHECK-NEXT:     [[T_0]][1] = (6 as i32) as i16;
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: });
// CHECK-EMPTY:
// CHECK-NEXT: static mut q4: std::simd::Simd<i16, 4> = std::simd::Simd::from_array({
// CHECK-NEXT:     let mut [[T_0]]: [i16; 4] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]][0] = (7 as i32) as i16;
// CHECK-NEXT:     [[T_0]][1] = (8 as i32) as i16;
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: });
// CHECK-EMPTY:
// CHECK-NEXT: static mut w1: std::simd::Simd<i16, 4> = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut w2: std::simd::Simd<i16, 4> = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut w3: std::simd::Simd<i16, 4> = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut w4: std::simd::Simd<i16, 4> = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut z1: std::simd::Simd<i16, 4> = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut z2: std::simd::Simd<i16, 4> = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut z3: std::simd::Simd<i16, 4> = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut z4: std::simd::Simd<i16, 4> = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut dummy: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn func0() {
// CHECK-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(dummy), 1 as i32) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn func1() {
// CHECK-NEXT:     let mut a: std::simd::Simd<i16, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut b: std::simd::Simd<i16, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     a = (unsafe { q1 }) * unsafe { q2 };
// CHECK-NEXT:     b = (unsafe { q3 }) * unsafe { q4 };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         w1 = a;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         w2 = b;
// CHECK-NEXT:     }
// CHECK-NEXT:     func0();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         w3 = a;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         w4 = b;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn func2() {
// CHECK-NEXT:     let mut a: std::simd::Simd<i16, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut b: std::simd::Simd<i16, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     a = (unsafe { q1 }) + unsafe { q2 };
// CHECK-NEXT:     b = (unsafe { q3 }) - unsafe { q4 };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         z1 = a;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         z2 = b;
// CHECK-NEXT:     }
// CHECK-NEXT:     func1();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         z3 = a;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         z4 = b;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     func2();
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         __builtin_memcmp(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(w1) }) as *const (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(w3) }) as *const (),
// CHECK-NEXT:             8 as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         __builtin_memcmp(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(w2) }) as *const (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(w4) }) as *const (),
// CHECK-NEXT:             8 as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         __builtin_memcmp(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(z1) }) as *const (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(z3) }) as *const (),
// CHECK-NEXT:             8 as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         __builtin_memcmp(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(z2) }) as *const (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(z4) }) as *const (),
// CHECK-NEXT:             8 as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
