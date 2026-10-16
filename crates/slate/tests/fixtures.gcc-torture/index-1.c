/* { dg-skip-if "strict reloc overflow checking" { msp430-*-* } { "*" } {
 * "-mcpu=msp430" "-mlarge"} } */

void abort(void);
void exit(int);

int a[] = {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13,
           14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27,
           28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39};

int f(long n) { return a[n - 100000]; }

int main(void) {
  if (f(100030L) != 30)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-skip-if "strict reloc overflow checking" { msp430-*-* } { "*" } {
// CHECK-NEXT: //! "-mcpu=msp430" "-mlarge"} }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: __SlateAlign16<[i32; 40]> = __SlateAlign16([
// CHECK-NEXT:     0 as i32, 1 as i32, 2 as i32, 3 as i32, 4 as i32, 5 as i32, 6 as i32, 7 as i32, 8 as i32,
// CHECK-NEXT:     9 as i32, 10 as i32, 11 as i32, 12 as i32, 13 as i32, 14 as i32, 15 as i32, 16 as i32,
// CHECK-NEXT:     17 as i32, 18 as i32, 19 as i32, 20 as i32, 21 as i32, 22 as i32, 23 as i32, 24 as i32,
// CHECK-NEXT:     25 as i32, 26 as i32, 27 as i32, 28 as i32, 29 as i32, 30 as i32, 31 as i32, 32 as i32,
// CHECK-NEXT:     33 as i32, 34 as i32, 35 as i32, 36 as i32, 37 as i32, 38 as i32, 39 as i32,
// CHECK-NEXT: ]);
// CHECK-EMPTY:
// CHECK-NEXT: fn f(mut n: i64) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(a.0) as *mut i32 }
// CHECK-NEXT:                 .offset((n - ((100000 as i32) as i64)) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if f(100030 as i64) != (30 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
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
