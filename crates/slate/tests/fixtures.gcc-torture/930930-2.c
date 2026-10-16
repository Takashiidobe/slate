void abort(void);
void exit(int);

int test_endianness() {
  union doubleword {
    double        d;
    unsigned long u[2];
  } dw;
  dw.d = 10;
  return dw.u[0] != 0 ? 1 : 0;
}

int test_endianness_vol() {
  union doubleword {
    volatile double d;
    volatile long   u[2];
  } dw;
  dw.d = 10;
  return dw.u[0] != 0 ? 1 : 0;
}

int main(void) {
  if (test_endianness() != test_endianness_vol())
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union [[DOUBLEWORD_0:doubleword_[0-9]+]] {
// CHECK-NEXT:     d: f64,
// CHECK-NEXT:     u: [u64; 2],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union [[DOUBLEWORD_1:doubleword_[0-9]+]] {
// CHECK-NEXT:     d: f64,
// CHECK-NEXT:     u: [i64; 2],
// CHECK-NEXT: }
// CHECK-NEXT: fn test_endianness() -> i32 {
// CHECK-NEXT:     let mut dw: [[DOUBLEWORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         dw.d = (10 as i32) as f64;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         *unsafe { unsafe { dw.u.as_mut_ptr() as *mut u64 }.offset((0 as i32) as isize) }
// CHECK-NEXT:     }) != (((0 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         1 as i32
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0 as i32
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn test_endianness_vol() -> i32 {
// CHECK-NEXT:     let mut dw: [[DOUBLEWORD_1]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::write_volatile(std::ptr::addr_of_mut!(dw.d), (10 as i32) as f64)
// CHECK-NEXT:     };
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         std::ptr::read_volatile(unsafe {
// CHECK-NEXT:             unsafe { dw.u.as_mut_ptr() as *mut i64 }.offset((0 as i32) as isize)
// CHECK-NEXT:         })
// CHECK-NEXT:     }) != ((0 as i32) as i64)
// CHECK-NEXT:     {
// CHECK-NEXT:         1 as i32
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0 as i32
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if test_endianness() != test_endianness_vol() {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
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
// SLATE-FILECHECK-END
