/* PR tree-optimization/51466 */

extern void abort(void);

__attribute__((noinline, noclone)) int foo(int i) {
  volatile int v[4];
  int         *p;
  v[i] = 6;
  p    = (int *)&v[i];
  return *p;
}

__attribute__((noinline, noclone)) int bar(int i) {
  volatile int v[4];
  int         *p;
  v[i] = 6;
  p    = (int *)&v[i];
  *p   = 8;
  return v[i];
}

__attribute__((noinline, noclone)) int baz(int i) {
  volatile int v[4];
  int         *p;
  v[i] = 6;
  p    = (int *)&v[0];
  *p   = 8;
  return v[i];
}

int main() {
  if (foo(3) != 6 || bar(2) != 8 || baz(0) != 8 || baz(1) != 6)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/51466
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut i: i32) -> i32 {
// CHECK-NEXT:     let mut v: __SlateAlign16<[i32; 4]> = __SlateAlign16([0 as i32; 4]);
// CHECK-NEXT:     let mut p: *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::write_volatile(
// CHECK-NEXT:             unsafe { (v.0.as_mut_ptr() as *mut i32).offset(i as isize) },
// CHECK-NEXT:             6 as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     p = (unsafe { (v.0.as_mut_ptr() as *mut i32).offset(i as isize) }) as *mut i32;
// CHECK-NEXT:     unsafe { *p }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut i: i32) -> i32 {
// CHECK-NEXT:     let mut v: __SlateAlign16<[i32; 4]> = __SlateAlign16([0 as i32; 4]);
// CHECK-NEXT:     let mut p: *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::write_volatile(
// CHECK-NEXT:             unsafe { (v.0.as_mut_ptr() as *mut i32).offset(i as isize) },
// CHECK-NEXT:             6 as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     p = (unsafe { (v.0.as_mut_ptr() as *mut i32).offset(i as isize) }) as *mut i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *p = 8 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::ptr::read_volatile(unsafe { (v.0.as_mut_ptr() as *mut i32).offset(i as isize) }) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn baz(mut i: i32) -> i32 {
// CHECK-NEXT:     let mut v: __SlateAlign16<[i32; 4]> = __SlateAlign16([0 as i32; 4]);
// CHECK-NEXT:     let mut p: *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::write_volatile(
// CHECK-NEXT:             unsafe { (v.0.as_mut_ptr() as *mut i32).offset(i as isize) },
// CHECK-NEXT:             6 as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     p = (unsafe { (v.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) }) as *mut i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *p = 8 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::ptr::read_volatile(unsafe { (v.0.as_mut_ptr() as *mut i32).offset(i as isize) }) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: bool;
// CHECK-NEXT:     if foo(3 as i32) != (6 as i32) {
// CHECK-NEXT:         [[V_0]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_0]] = bar(2 as i32) != (8 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_0]] {
// CHECK-NEXT:         [[V_1]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_1]] = baz(0 as i32) != (8 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_1]] {
// CHECK-NEXT:         [[V_2]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_2]] = baz(1 as i32) != (6 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_2]] {
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
