/* PR middle-end/22141 */

extern void abort(void);

struct S {
  struct T {
    char a;
    char b;
    char c;
    char d;
  } t;
} u __attribute__((aligned));

struct U {
  struct S s[4];
};

void __attribute__((noinline)) c1(struct T *p) {
  if (p->a != 1 || p->b != 2 || p->c != 3 || p->d != 4)
    abort();
  __builtin_memset(p, 0xaa, sizeof(*p));
}

void __attribute__((noinline)) c2(struct S *p) { c1(&p->t); }

void __attribute__((noinline)) c3(struct U *p) { c2(&p->s[2]); }

void __attribute__((noinline)) f1(void) { u = (struct S){{1, 2, 3, 4}}; }

void __attribute__((noinline)) f2(void) {
  u.t.a = 1;
  u.t.b = 2;
  u.t.c = 3;
  u.t.d = 4;
}

void __attribute__((noinline)) f3(void) {
  u.t.d = 4;
  u.t.b = 2;
  u.t.a = 1;
  u.t.c = 3;
}

void __attribute__((noinline)) f4(void) {
  struct S v __attribute__((aligned));
  v.t.a = 1;
  v.t.b = 2;
  v.t.c = 3;
  v.t.d = 4;
  c2(&v);
}

void __attribute__((noinline)) f5(struct S *p) {
  p->t.a = 1;
  p->t.c = 3;
  p->t.d = 4;
  p->t.b = 2;
}

void __attribute__((noinline)) f6(void) {
  struct U v __attribute__((aligned));
  v.s[2].t.a = 1;
  v.s[2].t.b = 2;
  v.s[2].t.c = 3;
  v.s[2].t.d = 4;
  c3(&v);
}

void __attribute__((noinline)) f7(struct U *p) {
  p->s[2].t.a = 1;
  p->s[2].t.c = 3;
  p->s[2].t.d = 4;
  p->s[2].t.b = 2;
}

int main(void) {
  struct U w __attribute__((aligned));
  f1();
  c2(&u);
  f2();
  c1(&u.t);
  f3();
  c2(&u);
  f4();
  f5(&u);
  c2(&u);
  f6();
  f7(&w);
  c3(&w);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR middle-end/22141
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     #[link_name = "memset"]
// CHECK-NEXT:     fn __builtin_memset([[V_0:__v[0-9]+]]: *mut (), [[V_1:__v[0-9]+]]: i32, [[V_2:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
// CHECK-NEXT:     t: T,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct T {
// CHECK-NEXT:     a: i8,
// CHECK-NEXT:     b: i8,
// CHECK-NEXT:     c: i8,
// CHECK-NEXT:     d: i8,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut u: __SlateAlign16<S> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct U {
// CHECK-NEXT:     s: [S; 4],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn c1(mut p: *mut T) {
// CHECK-NEXT:     if ((unsafe { (*p).a }) as i32) != (1 as i32)
// CHECK-NEXT:         || ((unsafe { (*p).b }) as i32) != (2 as i32)
// CHECK-NEXT:         || ((unsafe { (*p).c }) as i32) != (3 as i32)
// CHECK-NEXT:         || ((unsafe { (*p).d }) as i32) != (4 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { __builtin_memset(p as *mut (), 170 as i32, 4 as u64) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn c2(mut p: *mut S) {
// CHECK-NEXT:     c1(unsafe { std::ptr::addr_of_mut!((*p).t) });
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn c3(mut p: &mut U) {
// CHECK-NEXT:     c2(unsafe { unsafe { (*(p as *mut U)).s.as_mut_ptr() as *mut S }.offset((2 as i32) as isize) });
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f1() {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         u.0 = unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_0]] = S {
// CHECK-NEXT:                     t: T {
// CHECK-NEXT:                         a: (1 as i32) as i8,
// CHECK-NEXT:                         b: (2 as i32) as i8,
// CHECK-NEXT:                         c: (3 as i32) as i8,
// CHECK-NEXT:                         d: (4 as i32) as i8,
// CHECK-NEXT:                     },
// CHECK-NEXT:                 };
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f2() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         u.0.t.a = (1 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         u.0.t.b = (2 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         u.0.t.c = (3 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         u.0.t.d = (4 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f3() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         u.0.t.d = (4 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         u.0.t.b = (2 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         u.0.t.a = (1 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         u.0.t.c = (3 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f4() {
// CHECK-NEXT:     let mut v: __SlateAlign16<S> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-NEXT:     v.0.t.a = (1 as i32) as i8;
// CHECK-NEXT:     v.0.t.b = (2 as i32) as i8;
// CHECK-NEXT:     v.0.t.c = (3 as i32) as i8;
// CHECK-NEXT:     v.0.t.d = (4 as i32) as i8;
// CHECK-NEXT:     c2(std::ptr::addr_of_mut!(v.0));
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f5(mut p: *mut S) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*p).t.a = (1 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*p).t.c = (3 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*p).t.d = (4 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*p).t.b = (2 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f6() {
// CHECK-NEXT:     let mut v: __SlateAlign16<U> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe { (v.0.s.as_mut_ptr() as *mut S).offset((2 as i32) as isize) })
// CHECK-NEXT:             .t
// CHECK-NEXT:             .a = (1 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe { (v.0.s.as_mut_ptr() as *mut S).offset((2 as i32) as isize) })
// CHECK-NEXT:             .t
// CHECK-NEXT:             .b = (2 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe { (v.0.s.as_mut_ptr() as *mut S).offset((2 as i32) as isize) })
// CHECK-NEXT:             .t
// CHECK-NEXT:             .c = (3 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe { (v.0.s.as_mut_ptr() as *mut S).offset((2 as i32) as isize) })
// CHECK-NEXT:             .t
// CHECK-NEXT:             .d = (4 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     c3(unsafe { &mut (*std::ptr::addr_of_mut!(v.0)) });
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f7(mut p: &mut U) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe {
// CHECK-NEXT:             unsafe { (*(p as *mut U)).s.as_mut_ptr() as *mut S }.offset((2 as i32) as isize)
// CHECK-NEXT:         })
// CHECK-NEXT:         .t
// CHECK-NEXT:         .a = (1 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe {
// CHECK-NEXT:             unsafe { (*(p as *mut U)).s.as_mut_ptr() as *mut S }.offset((2 as i32) as isize)
// CHECK-NEXT:         })
// CHECK-NEXT:         .t
// CHECK-NEXT:         .c = (3 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe {
// CHECK-NEXT:             unsafe { (*(p as *mut U)).s.as_mut_ptr() as *mut S }.offset((2 as i32) as isize)
// CHECK-NEXT:         })
// CHECK-NEXT:         .t
// CHECK-NEXT:         .d = (4 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe {
// CHECK-NEXT:             unsafe { (*(p as *mut U)).s.as_mut_ptr() as *mut S }.offset((2 as i32) as isize)
// CHECK-NEXT:         })
// CHECK-NEXT:         .t
// CHECK-NEXT:         .b = (2 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut w: __SlateAlign16<U> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-NEXT:     f1();
// CHECK-NEXT:     c2(unsafe { std::ptr::addr_of_mut!(u.0) });
// CHECK-NEXT:     f2();
// CHECK-NEXT:     c1(unsafe { std::ptr::addr_of_mut!(u.0.t) });
// CHECK-NEXT:     f3();
// CHECK-NEXT:     c2(unsafe { std::ptr::addr_of_mut!(u.0) });
// CHECK-NEXT:     f4();
// CHECK-NEXT:     f5(unsafe { std::ptr::addr_of_mut!(u.0) });
// CHECK-NEXT:     c2(unsafe { std::ptr::addr_of_mut!(u.0) });
// CHECK-NEXT:     f6();
// CHECK-NEXT:     f7(unsafe { &mut (*std::ptr::addr_of_mut!(w.0)) });
// CHECK-NEXT:     c3(unsafe { &mut (*std::ptr::addr_of_mut!(w.0)) });
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
