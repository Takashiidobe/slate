/* PR tree-optimization/86231 */

#define ONE ((void *)1)
#define TWO ((void *)2)

__attribute__((noipa)) int foo(void *p, int x) {
  if (p == ONE)
    return 0;
  if (!p)
    p = x ? TWO : ONE;
  return p == ONE ? 0 : 1;
}

int v[8];

int main() {
  if (foo((void *)0, 0) != 0 || foo((void *)0, 1) != 1 || foo(ONE, 0) != 0 ||
      foo(ONE, 1) != 0 || foo(TWO, 0) != 1 || foo(TWO, 1) != 1 ||
      foo(&v[7], 0) != 1 || foo(&v[7], 1) != 1)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/86231
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut p: *mut (), mut x: i32) -> i32 {
// CHECK-NEXT:     if p == ((1 as i32) as *mut ()) {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if !(p != std::ptr::null_mut::<()>()) {
// CHECK-NEXT:         p = if x != (0 as i32) {
// CHECK-NEXT:             (2 as i32) as *mut ()
// CHECK-NEXT:         } else {
// CHECK-NEXT:             (1 as i32) as *mut ()
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     if p == ((1 as i32) as *mut ()) {
// CHECK-NEXT:         0 as i32
// CHECK-NEXT:     } else {
// CHECK-NEXT:         1 as i32
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut v: __SlateAlign16<[i32; 8]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: bool;
// CHECK-NEXT:     if foo(std::ptr::null_mut::<()>(), 0 as i32) != (0 as i32) {
// CHECK-NEXT:         [[V_0]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_0]] = foo(std::ptr::null_mut::<()>(), 1 as i32) != (1 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_0]] {
// CHECK-NEXT:         [[V_1]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_1]] = foo((1 as i32) as *mut (), 0 as i32) != (0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_1]] {
// CHECK-NEXT:         [[V_2]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_2]] = foo((1 as i32) as *mut (), 1 as i32) != (0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_2]] {
// CHECK-NEXT:         [[V_3]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_3]] = foo((2 as i32) as *mut (), 0 as i32) != (1 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_3]] {
// CHECK-NEXT:         [[V_4]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_4]] = foo((2 as i32) as *mut (), 1 as i32) != (1 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_5:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_4]] {
// CHECK-NEXT:         [[V_5]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_5]] = foo(
// CHECK-NEXT:             (unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(v.0) as *mut i32 }.offset((7 as i32) as isize)
// CHECK-NEXT:             }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:         ) != (1 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_6:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_5]] {
// CHECK-NEXT:         [[V_6]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_6]] = foo(
// CHECK-NEXT:             (unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(v.0) as *mut i32 }.offset((7 as i32) as isize)
// CHECK-NEXT:             }) as *mut (),
// CHECK-NEXT:             1 as i32,
// CHECK-NEXT:         ) != (1 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_6]] {
// CHECK-NEXT:         unsafe { __builtin_abort() };
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
