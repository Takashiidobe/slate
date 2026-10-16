/* PR middle-end/24109 */

extern void abort(void);

struct A {
  int i;
  int j;
};
struct B {
  struct A *a;
  struct A *b;
};
struct C {
  struct B *c;
  struct A *d;
};
struct C e = {&(struct B){&(struct A){1, 2}, &(struct A){3, 4}},
              &(struct A){5, 6}};

int main(void) {
  if (e.c->a->i != 1 || e.c->a->j != 2)
    abort();
  if (e.c->b->i != 3 || e.c->b->j != 4)
    abort();
  if (e.d->i != 5 || e.d->j != 6)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR middle-end/24109
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct A {
// CHECK-NEXT:     i: i32,
// CHECK-NEXT:     j: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct B {
// CHECK-NEXT:     a: *mut A,
// CHECK-NEXT:     b: *mut A,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct C {
// CHECK-NEXT:     c: *mut B,
// CHECK-NEXT:     d: *mut A,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut e: C = C {
// CHECK-NEXT:     c: unsafe { std::ptr::addr_of_mut!([[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]) },
// CHECK-NEXT:     d: unsafe { std::ptr::addr_of_mut!([[SLATE_COMPOUND_1:__slate_compound_[0-9]+]]) },
// CHECK-NEXT: };
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if (unsafe { (*unsafe { (*unsafe { e.c }).a }).i }) != (1 as i32)
// CHECK-NEXT:         || (unsafe { (*unsafe { (*unsafe { e.c }).a }).j }) != (2 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { (*unsafe { (*unsafe { e.c }).b }).i }) != (3 as i32)
// CHECK-NEXT:         || (unsafe { (*unsafe { (*unsafe { e.c }).b }).j }) != (4 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { (*unsafe { e.d }).i }) != (5 as i32)
// CHECK-NEXT:         || (unsafe { (*unsafe { e.d }).j }) != (6 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut [[SLATE_COMPOUND_2:__slate_compound_[0-9]+]]: A = A {
// CHECK-NEXT:     i: 1 as i32,
// CHECK-NEXT:     j: 2 as i32,
// CHECK-NEXT: };
// CHECK-EMPTY:
// CHECK-NEXT: static mut [[SLATE_COMPOUND_3:__slate_compound_[0-9]+]]: A = A {
// CHECK-NEXT:     i: 3 as i32,
// CHECK-NEXT:     j: 4 as i32,
// CHECK-NEXT: };
// CHECK-EMPTY:
// CHECK-NEXT: static mut [[SLATE_COMPOUND_0]]: B = B {
// CHECK-NEXT:     a: unsafe { std::ptr::addr_of_mut!([[SLATE_COMPOUND_2]]) },
// CHECK-NEXT:     b: unsafe { std::ptr::addr_of_mut!([[SLATE_COMPOUND_3]]) },
// CHECK-NEXT: };
// CHECK-EMPTY:
// CHECK-NEXT: static mut [[SLATE_COMPOUND_1]]: A = A {
// CHECK-NEXT:     i: 5 as i32,
// CHECK-NEXT:     j: 6 as i32,
// CHECK-NEXT: };
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
