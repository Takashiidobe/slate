/* { dg-require-stack-size "0x12000" } */

void abort(void);
void exit(int);

#if __INT_MAX__ < 32768
int main() { exit(0); }
#else
int a[2] = {2, 3};

static int __attribute__((noinline)) bar(int x, void *b) {
  a[0]++;
  return x;
}

static int __attribute__((noinline)) foo(int x) {
  char buf[0x10000];
  int  y = a[0];
  a[1]   = y;
  x      = bar(x, buf);
  y      = bar(y, buf);
  return x + y;
}

int main() {
  if (foo(100) != 102)
    abort();
  exit(0);
}
#endif

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-require-stack-size "0x12000" }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: [i32; 2] = [2 as i32, 3 as i32];
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut x: i32, mut b: *mut ()) -> i32 {
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: *mut i32 =
// CHECK-NEXT:         unsafe { unsafe { std::ptr::addr_of_mut!(a) as *mut i32 }.offset((0 as i32) as isize) };
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: i32 = unsafe { *[[V_1]] };
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[V_1]] = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     x
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: i32) -> i32 {
// CHECK-NEXT:     let mut buf: __SlateAlign16<[i8; 65536]> = __SlateAlign16([0 as i8; 65536]);
// CHECK-NEXT:     let mut y: i32 = unsafe {
// CHECK-NEXT:         *unsafe { unsafe { std::ptr::addr_of_mut!(a) as *mut i32 }.offset((0 as i32) as isize) }
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { unsafe { std::ptr::addr_of_mut!(a) as *mut i32 }.offset((1 as i32) as isize) } =
// CHECK-NEXT:             y;
// CHECK-NEXT:     }
// CHECK-NEXT:     x = bar(x, buf.0.as_mut_ptr() as *mut ());
// CHECK-NEXT:     y = bar(y, buf.0.as_mut_ptr() as *mut ());
// CHECK-NEXT:     x + y
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if foo(100 as i32) != (102 as i32) {
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
