/* PR middle-end/29695 */

extern void abort(void);

int           a = 128;
unsigned char b = 128;
long long     c = 0x80000000LL;
unsigned int  d = 0x80000000;

int f1(void) { return (a & 0x80) ? 0x80 : 0; }

int f2(void) { return (b & 0x80) ? 0x80 : 0; }

int f3(void) { return (b & 0x80) ? 0x380 : 0; }

int f4(void) { return (b & 0x80) ? -128 : 0; }

long long f5(void) { return (c & 0x80000000) ? 0x80000000LL : 0LL; }

long long f6(void) { return (d & 0x80000000) ? 0x80000000LL : 0LL; }

long long f7(void) { return (d & 0x80000000) ? 0x380000000LL : 0LL; }

long long f8(void) { return (d & 0x80000000) ? -2147483648LL : 0LL; }

int main(void) {
  if ((char)128 != -128 || (int)0x80000000 != -2147483648)
    return 0;
  if (f1() != 128)
    abort();
  if (f2() != 128)
    abort();
  if (f3() != 896)
    abort();
  if (f4() != -128)
    abort();
  if (f5() != 0x80000000LL)
    abort();
  if (f6() != 0x80000000LL)
    abort();
  if (f7() != 0x380000000LL)
    abort();
  if (f8() != -2147483648LL)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR middle-end/29695
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: i32 = 128 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: u8 = ((128 as i32) as i8) as u8;
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: i64 = 2147483648 as i64;
// CHECK-EMPTY:
// CHECK-NEXT: static mut d: u32 = 2147483648 as u32;
// CHECK-EMPTY:
// CHECK-NEXT: fn f1() -> i32 {
// CHECK-NEXT:     if (unsafe { a }) & (128 as i32) != (0 as i32) {
// CHECK-NEXT:         128 as i32
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0 as i32
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f2() -> i32 {
// CHECK-NEXT:     if (((unsafe { b }) as u32) as i32) & (128 as i32) != (0 as i32) {
// CHECK-NEXT:         128 as i32
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0 as i32
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f3() -> i32 {
// CHECK-NEXT:     if (((unsafe { b }) as u32) as i32) & (128 as i32) != (0 as i32) {
// CHECK-NEXT:         896 as i32
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0 as i32
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f4() -> i32 {
// CHECK-NEXT:     if (((unsafe { b }) as u32) as i32) & (128 as i32) != (0 as i32) {
// CHECK-NEXT:         -(128 as i32)
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0 as i32
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f5() -> i64 {
// CHECK-NEXT:     if (unsafe { c }) & (((2147483648 as u32) as u64) as i64) != (0 as i64) {
// CHECK-NEXT:         2147483648 as i64
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0 as i64
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f6() -> i64 {
// CHECK-NEXT:     if (unsafe { d }) & (2147483648 as u32) != (0 as u32) {
// CHECK-NEXT:         2147483648 as i64
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0 as i64
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f7() -> i64 {
// CHECK-NEXT:     if (unsafe { d }) & (2147483648 as u32) != (0 as u32) {
// CHECK-NEXT:         15032385536 as i64
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0 as i64
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f8() -> i64 {
// CHECK-NEXT:     if (unsafe { d }) & (2147483648 as u32) != (0 as u32) {
// CHECK-NEXT:         -(2147483648 as i64)
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0 as i64
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if (((128 as i32) as i8) as i32) != -(128 as i32)
// CHECK-NEXT:         || (((2147483648 as u32) as i32) as i64) != -(2147483648 as i64)
// CHECK-NEXT:     {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if f1() != (128 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if f2() != (128 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if f3() != (896 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if f4() != -(128 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if f5() != (2147483648 as i64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if f6() != (2147483648 as i64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if f7() != (15032385536 as i64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if f8() != -(2147483648 as i64) {
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
