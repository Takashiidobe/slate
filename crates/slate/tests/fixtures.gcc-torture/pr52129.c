/* PR target/52129 */

extern void abort(void);
struct S {
  void        *p;
  unsigned int q;
};
struct T {
  char a[64];
  char b[64];
} t;

__attribute__((noinline, noclone)) int foo(void *x, struct S s, void *y,
                                           void *z) {
  if (x != &t.a[2] || s.p != &t.b[5] || s.q != 27 || y != &t.a[17] ||
      z != &t.b[17])
    abort();
  return 29;
}

__attribute__((noinline, noclone)) int bar(void *x, void *y, void *z,
                                           struct S s, int t, struct T *u) {
  return foo(x, s, &u->a[t], &u->b[t]);
}

int main() {
  struct S s = {&t.b[5], 27};
  if (bar(&t.a[2], (void *)0, (void *)0, s, 17, &t) != 29)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR target/52129
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
// CHECK-NEXT:     p: *mut (),
// CHECK-NEXT:     q: u32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct T {
// CHECK-NEXT:     a: [i8; 64],
// CHECK-NEXT:     b: [i8; 64],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut t: T = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: *mut (), mut s: S, mut y: *mut (), mut z: *mut ()) -> i32 {
// CHECK-NEXT:     if x != ((unsafe {
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(t.a) as *mut i8 }.offset((2 as i32) as isize)
// CHECK-NEXT:     }) as *mut ())
// CHECK-NEXT:         || s.p
// CHECK-NEXT:             != ((unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(t.b) as *mut i8 }.offset((5 as i32) as isize)
// CHECK-NEXT:             }) as *mut ())
// CHECK-NEXT:         || s.q != ((27 as i32) as u32)
// CHECK-NEXT:         || y != ((unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(t.a) as *mut i8 }.offset((17 as i32) as isize)
// CHECK-NEXT:         }) as *mut ())
// CHECK-NEXT:         || z != ((unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(t.b) as *mut i8 }.offset((17 as i32) as isize)
// CHECK-NEXT:         }) as *mut ())
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     29 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(
// CHECK-NEXT:     mut x: *mut (),
// CHECK-NEXT:     mut y: *mut (),
// CHECK-NEXT:     mut z: *mut (),
// CHECK-NEXT:     mut s: S,
// CHECK-NEXT:     mut [[T_0:t_[0-9]+]]: i32,
// CHECK-NEXT:     mut u: *mut T,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     foo(
// CHECK-NEXT:         x,
// CHECK-NEXT:         s,
// CHECK-NEXT:         (unsafe { unsafe { (*u).a.as_mut_ptr() as *mut i8 }.offset([[T_0]] as isize) }) as *mut (),
// CHECK-NEXT:         (unsafe { unsafe { (*u).b.as_mut_ptr() as *mut i8 }.offset([[T_0]] as isize) }) as *mut (),
// CHECK-NEXT:     )
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut s: S = S {
// CHECK-NEXT:         p: (unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(t.b) as *mut i8 }.offset((5 as i32) as isize)
// CHECK-NEXT:         }) as *mut (),
// CHECK-NEXT:         q: (27 as i32) as u32,
// CHECK-NEXT:     };
// CHECK-NEXT:     if bar(
// CHECK-NEXT:         (unsafe { unsafe { std::ptr::addr_of_mut!(t.a) as *mut i8 }.offset((2 as i32) as isize) })
// CHECK-NEXT:             as *mut (),
// CHECK-NEXT:         std::ptr::null_mut::<()>(),
// CHECK-NEXT:         std::ptr::null_mut::<()>(),
// CHECK-NEXT:         s,
// CHECK-NEXT:         17 as i32,
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(t) },
// CHECK-NEXT:     ) != (29 as i32)
// CHECK-NEXT:     {
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
