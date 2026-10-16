/* PR tree-optimization/56962 */

extern void abort(void);
long long   v[144];

__attribute__((noinline, noclone)) void bar(long long *x) {
  if (x != &v[29])
    abort();
}

__attribute__((noinline, noclone)) void foo(long long *x, long y, long z) {
  long long a, b, c;
  a        = x[z * 4 + y * 3];
  b        = x[z * 5 + y * 3];
  c        = x[z * 5 + y * 4];
  x[y * 4] = a;
  bar(&x[z * 5 + y]);
  x[z * 5 + y * 5] = b + c;
}

int main() {
  foo(v, 24, 1);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/56962
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut v: __SlateAlign16<[i64; 144]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut x: *mut i64) {
// CHECK-NEXT:     if x != unsafe {
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(v.0) as *mut i64 }.offset((29 as i32) as isize)
// CHECK-NEXT:     } {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: *mut i64, mut y: i64, mut z: i64) {
// CHECK-NEXT:     let mut a: i64 = unsafe {
// CHECK-NEXT:         *unsafe { x.offset((z * ((4 as i32) as i64) + y * ((3 as i32) as i64)) as isize) }
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut b: i64 = unsafe {
// CHECK-NEXT:         *unsafe { x.offset((z * ((5 as i32) as i64) + y * ((3 as i32) as i64)) as isize) }
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut c: i64 = unsafe {
// CHECK-NEXT:         *unsafe { x.offset((z * ((5 as i32) as i64) + y * ((4 as i32) as i64)) as isize) }
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { x.offset((y * ((4 as i32) as i64)) as isize) } = a;
// CHECK-NEXT:     }
// CHECK-NEXT:     bar(unsafe { x.offset((z * ((5 as i32) as i64) + y) as isize) });
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { x.offset((z * ((5 as i32) as i64) + y * ((5 as i32) as i64)) as isize) } = b + c;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     foo(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(v.0) as *mut i64 },
// CHECK-NEXT:         (24 as i32) as i64,
// CHECK-NEXT:         (1 as i32) as i64,
// CHECK-NEXT:     );
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
