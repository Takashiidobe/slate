/* This testcase failed at -O2 on IA-64, because scheduling did not take
   into account conditional execution when using cselib for alias
   analysis.  */

struct D {
  int       d1;
  struct D *d2;
};
struct C {
  struct D c1;
  long     c2, c3, c4, c5, c6;
};
struct A {
  struct A *a1;
  struct C *a2;
};
struct B {
  struct C  b1;
  struct A *b2;
};

extern void abort(void);
extern void exit(int);

void foo(struct B *x, struct B *y) {
  if (x->b2 == 0) {
    struct A *a;

    x->b2 = a = y->b2;
    y->b2     = 0;
    for (; a; a = a->a1)
      a->a2 = &x->b1;
  }

  if (y->b2 != 0)
    abort();

  if (x->b1.c3 == -1) {
    x->b1.c3 = y->b1.c3;
    x->b1.c4 = y->b1.c4;
    y->b1.c3 = -1;
    y->b1.c4 = 0;
  }

  if (y->b1.c3 != -1)
    abort();
}

struct B x, y;

int main() {
  y.b1.c1.d1 = 6;
  y.b1.c3    = 145;
  y.b1.c4    = 2448;
  x.b1.c3    = -1;
  foo(&x, &y);
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! This testcase failed at -O2 on IA-64, because scheduling did not take
// CHECK-NEXT: //! into account conditional execution when using cselib for alias
// CHECK-NEXT: //! analysis.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct D {
// CHECK-NEXT:     d1: i32,
// CHECK-NEXT:     d2: *mut D,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct C {
// CHECK-NEXT:     c1: D,
// CHECK-NEXT:     c2: i64,
// CHECK-NEXT:     c3: i64,
// CHECK-NEXT:     c4: i64,
// CHECK-NEXT:     c5: i64,
// CHECK-NEXT:     c6: i64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct A {
// CHECK-NEXT:     a1: *mut A,
// CHECK-NEXT:     a2: *mut C,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct B {
// CHECK-NEXT:     b1: C,
// CHECK-NEXT:     b2: *mut A,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut [[X_0:x_[0-9]+]]: *mut B, mut [[Y_0:y_[0-9]+]]: *mut B) {
// CHECK-NEXT:     if (unsafe { (*[[X_0]]).b2 }) == std::ptr::null_mut::<A>() {
// CHECK-NEXT:         let mut a: *mut A = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: *mut A = unsafe { (*[[Y_0]]).b2 };
// CHECK-NEXT:         a = [[V_1]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*[[X_0]]).b2 = [[V_1]];
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*[[Y_0]]).b2 = std::ptr::null_mut::<A>();
// CHECK-NEXT:         }
// CHECK-NEXT:         '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while a != std::ptr::null_mut::<A>() {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*a).a2 = unsafe { std::ptr::addr_of_mut!((*[[X_0]]).b1) };
// CHECK-NEXT:             }
// CHECK-NEXT:             a = unsafe { (*a).a1 };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { (*[[Y_0]]).b2 }) != std::ptr::null_mut::<A>() {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { (*[[X_0]]).b1.c3 }) == (-(1 as i32) as i64) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*[[X_0]]).b1.c3 = unsafe { (*[[Y_0]]).b1.c3 };
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*[[X_0]]).b1.c4 = unsafe { (*[[Y_0]]).b1.c4 };
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*[[Y_0]]).b1.c3 = -(1 as i32) as i64;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*[[Y_0]]).b1.c4 = (0 as i32) as i64;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { (*[[Y_0]]).b1.c3 }) != (-(1 as i32) as i64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut x: B = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut y: B = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         y.b1.c1.d1 = 6 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         y.b1.c3 = (145 as i32) as i64;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         y.b1.c4 = (2448 as i32) as i64;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         x.b1.c3 = -(1 as i32) as i64;
// CHECK-NEXT:     }
// CHECK-NEXT:     foo(unsafe { std::ptr::addr_of_mut!(x) }, unsafe {
// CHECK-NEXT:         std::ptr::addr_of_mut!(y)
// CHECK-NEXT:     });
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
