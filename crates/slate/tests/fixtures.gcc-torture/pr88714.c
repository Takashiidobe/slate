/* PR bootstrap/88714 */

struct S {
  int  a, b, c;
  int *d;
};
struct T {
  int *e, *f, *g;
}   *t = 0;
int *o = 0;

__attribute__((noipa)) void bar(int *x, int y, int z, int w) {
  if (w == -1) {
    if (x != 0 || y != 0 || z != 0)
      __builtin_abort();
  } else if (w != 0 || x != t->g || y != 0 || z != 12)
    __builtin_abort();
}

__attribute__((noipa)) void foo(struct S *x, struct S *y, int *z, int w) {
  *o = w;
  if (w)
    bar(0, 0, 0, -1);
  x->d = z;
  if (y->d)
    y->c = y->c + y->d[0];
  bar(t->g, 0, y->c, 0);
}

int main() {
  int      a[4] = {8, 9, 10, 11};
  struct S s    = {1, 2, 3, &a[0]};
  struct T u    = {0, 0, &a[3]};
  o             = &a[2];
  t             = &u;
  foo(&s, &s, &a[1], 5);
  if (s.c != 12 || s.d != &a[1])
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR bootstrap/88714
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
// CHECK-NEXT:     a: i32,
// CHECK-NEXT:     b: i32,
// CHECK-NEXT:     c: i32,
// CHECK-NEXT:     d: *mut i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct T {
// CHECK-NEXT:     e: *mut i32,
// CHECK-NEXT:     f: *mut i32,
// CHECK-NEXT:     g: *mut i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut t: *mut T = std::ptr::null_mut::<T>();
// CHECK-EMPTY:
// CHECK-NEXT: static mut o: *mut i32 = std::ptr::null_mut::<i32>();
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut x: *mut i32, mut y: i32, mut z: i32, mut w: i32) {
// CHECK-NEXT:     if w == -(1 as i32) {
// CHECK-NEXT:         if x != std::ptr::null_mut::<i32>() || y != (0 as i32) || z != (0 as i32) {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if w != (0 as i32)
// CHECK-NEXT:             || x != unsafe { (*unsafe { t }).g }
// CHECK-NEXT:             || y != (0 as i32)
// CHECK-NEXT:             || z != (12 as i32)
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: &mut S, mut y: &mut S, mut z: *mut i32, mut w: i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { o } = w;
// CHECK-NEXT:     }
// CHECK-NEXT:     if w != (0 as i32) {
// CHECK-NEXT:         bar(std::ptr::null_mut::<i32>(), 0 as i32, 0 as i32, -(1 as i32));
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(x as *mut S)).d = z;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { (*(y as *mut S)).d }) != std::ptr::null_mut::<i32>() {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*(y as *mut S)).c = (unsafe { (*(y as *mut S)).c })
// CHECK-NEXT:                 + unsafe { *unsafe { unsafe { (*(y as *mut S)).d }.offset((0 as i32) as isize) } };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     bar(
// CHECK-NEXT:         unsafe { (*unsafe { t }).g },
// CHECK-NEXT:         0 as i32,
// CHECK-NEXT:         unsafe { (*(y as *mut S)).c },
// CHECK-NEXT:         0 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: __SlateAlign16<[i32; 4]> =
// CHECK-NEXT:         __SlateAlign16([8 as i32, 9 as i32, 10 as i32, 11 as i32]);
// CHECK-NEXT:     let mut s: S = S {
// CHECK-NEXT:         a: 1 as i32,
// CHECK-NEXT:         b: 2 as i32,
// CHECK-NEXT:         c: 3 as i32,
// CHECK-NEXT:         d: unsafe { (a.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) },
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut u: T = T {
// CHECK-NEXT:         e: std::ptr::null_mut::<i32>(),
// CHECK-NEXT:         f: std::ptr::null_mut::<i32>(),
// CHECK-NEXT:         g: unsafe { (a.0.as_mut_ptr() as *mut i32).offset((3 as i32) as isize) },
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         o = unsafe { (a.0.as_mut_ptr() as *mut i32).offset((2 as i32) as isize) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         t = std::ptr::addr_of_mut!(u);
// CHECK-NEXT:     }
// CHECK-NEXT:     foo(
// CHECK-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(s)) },
// CHECK-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(s)) },
// CHECK-NEXT:         unsafe { (a.0.as_mut_ptr() as *mut i32).offset((1 as i32) as isize) },
// CHECK-NEXT:         5 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     if s.c != (12 as i32)
// CHECK-NEXT:         || s.d != unsafe { (a.0.as_mut_ptr() as *mut i32).offset((1 as i32) as isize) }
// CHECK-NEXT:     {
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
