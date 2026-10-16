/* PR tree-optimization/22043 */

extern void abort(void);

struct A {
  int i;
  int j;
  int k;
  int l;
};
struct B {
  struct A a;
  int      r[1];
};
struct C {
  struct A a;
  int      r[0];
};
struct D {
  struct A a;
  int      r[];
};

void foo(struct A *x) {
  if (x->i != 0 || x->j != 5 || x->k != 0 || x->l != 0)
    abort();
}

int main() {
  struct B b = {.a.j = 5};
  struct C c = {.a.j = 5};
  struct D d = {.a.j = 5};
  foo(&b.a);
  foo(&c.a);
  foo(&d.a);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/22043
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct A {
// CHECK-NEXT:     i: i32,
// CHECK-NEXT:     j: i32,
// CHECK-NEXT:     k: i32,
// CHECK-NEXT:     l: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct B {
// CHECK-NEXT:     a: A,
// CHECK-NEXT:     r: [i32; 1],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct C {
// CHECK-NEXT:     a: A,
// CHECK-NEXT:     r: [i32; 0],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct D {
// CHECK-NEXT:     a: A,
// CHECK-NEXT:     r: [i32; 0],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: &A) {
// CHECK-NEXT:     if (unsafe { (*(x as *const A)).i }) != (0 as i32)
// CHECK-NEXT:         || (unsafe { (*(x as *const A)).j }) != (5 as i32)
// CHECK-NEXT:         || (unsafe { (*(x as *const A)).k }) != (0 as i32)
// CHECK-NEXT:         || (unsafe { (*(x as *const A)).l }) != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut b: B = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: B = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].a = {
// CHECK-NEXT:             let mut [[T_1:__t[0-9]+]]: A = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_1]].j = 5 as i32;
// CHECK-NEXT:             [[T_1]]
// CHECK-NEXT:         };
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut c: C = {
// CHECK-NEXT:         let mut [[T_2:__t[0-9]+]]: C = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_2]].a = {
// CHECK-NEXT:             let mut [[T_3:__t[0-9]+]]: A = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_3]].j = 5 as i32;
// CHECK-NEXT:             [[T_3]]
// CHECK-NEXT:         };
// CHECK-NEXT:         [[T_2]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut d: D = {
// CHECK-NEXT:         let mut [[T_4:__t[0-9]+]]: D = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_4]].a = {
// CHECK-NEXT:             let mut [[T_5:__t[0-9]+]]: A = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_5]].j = 5 as i32;
// CHECK-NEXT:             [[T_5]]
// CHECK-NEXT:         };
// CHECK-NEXT:         [[T_4]]
// CHECK-NEXT:     };
// CHECK-NEXT:     foo(unsafe { &(*std::ptr::addr_of_mut!(b.a)) });
// CHECK-NEXT:     foo(unsafe { &(*std::ptr::addr_of_mut!(c.a)) });
// CHECK-NEXT:     foo(unsafe { &(*std::ptr::addr_of_mut!(d.a)) });
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
