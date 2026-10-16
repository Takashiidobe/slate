extern void abort(void);
struct T {
  int t;
  int r[8];
};
struct S {
  int      a;
  int      b;
  int      c[6];
  struct T d;
};

__attribute__((noinline)) void foo(struct S *s) {
  *s = (struct S){s->b, s->a, {0, 0, 0, 0, 0, 0}, s->d};
}

int main(void) {
  struct S s = {6, 12, {1, 2, 3, 4, 5, 6}, {7, {8, 9, 10, 11, 12, 13, 14, 15}}};
  foo(&s);
  if (s.a != 12 || s.b != 6 || s.c[0] || s.c[1] || s.c[2] || s.c[3] || s.c[4] ||
      s.c[5])
    abort();
  if (s.d.t != 7 || s.d.r[0] != 8 || s.d.r[1] != 9 || s.d.r[2] != 10 ||
      s.d.r[3] != 11 || s.d.r[4] != 12 || s.d.r[5] != 13 || s.d.r[6] != 14 ||
      s.d.r[7] != 15)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct T {
// CHECK-NEXT:     t: i32,
// CHECK-NEXT:     r: [i32; 8],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
// CHECK-NEXT:     a: i32,
// CHECK-NEXT:     b: i32,
// CHECK-NEXT:     c: [i32; 6],
// CHECK-NEXT:     d: T,
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(mut s: &mut S) {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(s as *mut S) = unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_0]] = S {
// CHECK-NEXT:                     a: unsafe { (*(s as *mut S)).b },
// CHECK-NEXT:                     b: unsafe { (*(s as *mut S)).a },
// CHECK-NEXT:                     c: [0 as i32, 0 as i32, 0 as i32, 0 as i32, 0 as i32, 0 as i32],
// CHECK-NEXT:                     d: unsafe { (*(s as *mut S)).d },
// CHECK-NEXT:                 };
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut s: S = S {
// CHECK-NEXT:         a: 6 as i32,
// CHECK-NEXT:         b: 12 as i32,
// CHECK-NEXT:         c: [1 as i32, 2 as i32, 3 as i32, 4 as i32, 5 as i32, 6 as i32],
// CHECK-NEXT:         d: T {
// CHECK-NEXT:             t: 7 as i32,
// CHECK-NEXT:             r: [
// CHECK-NEXT:                 8 as i32,
// CHECK-NEXT:                 9 as i32,
// CHECK-NEXT:                 10 as i32,
// CHECK-NEXT:                 11 as i32,
// CHECK-NEXT:                 12 as i32,
// CHECK-NEXT:                 13 as i32,
// CHECK-NEXT:                 14 as i32,
// CHECK-NEXT:                 15 as i32,
// CHECK-NEXT:             ],
// CHECK-NEXT:         },
// CHECK-NEXT:     };
// CHECK-NEXT:     foo(unsafe { &mut (*std::ptr::addr_of_mut!(s)) });
// CHECK-NEXT:     if s.a != (12 as i32) || s.b != (6 as i32)
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             *unsafe { (s.c.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) }
// CHECK-NEXT:         }) != (0 as i32)
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             *unsafe { (s.c.as_mut_ptr() as *mut i32).offset((1 as i32) as isize) }
// CHECK-NEXT:         }) != (0 as i32)
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             *unsafe { (s.c.as_mut_ptr() as *mut i32).offset((2 as i32) as isize) }
// CHECK-NEXT:         }) != (0 as i32)
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             *unsafe { (s.c.as_mut_ptr() as *mut i32).offset((3 as i32) as isize) }
// CHECK-NEXT:         }) != (0 as i32)
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             *unsafe { (s.c.as_mut_ptr() as *mut i32).offset((4 as i32) as isize) }
// CHECK-NEXT:         }) != (0 as i32)
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             *unsafe { (s.c.as_mut_ptr() as *mut i32).offset((5 as i32) as isize) }
// CHECK-NEXT:         }) != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if s.d.t != (7 as i32)
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             *unsafe { (s.d.r.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) }
// CHECK-NEXT:         }) != (8 as i32)
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             *unsafe { (s.d.r.as_mut_ptr() as *mut i32).offset((1 as i32) as isize) }
// CHECK-NEXT:         }) != (9 as i32)
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             *unsafe { (s.d.r.as_mut_ptr() as *mut i32).offset((2 as i32) as isize) }
// CHECK-NEXT:         }) != (10 as i32)
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             *unsafe { (s.d.r.as_mut_ptr() as *mut i32).offset((3 as i32) as isize) }
// CHECK-NEXT:         }) != (11 as i32)
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             *unsafe { (s.d.r.as_mut_ptr() as *mut i32).offset((4 as i32) as isize) }
// CHECK-NEXT:         }) != (12 as i32)
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             *unsafe { (s.d.r.as_mut_ptr() as *mut i32).offset((5 as i32) as isize) }
// CHECK-NEXT:         }) != (13 as i32)
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             *unsafe { (s.d.r.as_mut_ptr() as *mut i32).offset((6 as i32) as isize) }
// CHECK-NEXT:         }) != (14 as i32)
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             *unsafe { (s.d.r.as_mut_ptr() as *mut i32).offset((7 as i32) as isize) }
// CHECK-NEXT:         }) != (15 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
