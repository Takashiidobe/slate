void abort(void);
void exit(int);

struct tiny {
  char c;
  char d;
};

void f(int n, struct tiny x, struct tiny y, struct tiny z, long l) {
  if (x.c != 10)
    abort();
  if (x.d != 20)
    abort();

  if (y.c != 11)
    abort();
  if (y.d != 21)
    abort();

  if (z.c != 12)
    abort();
  if (z.d != 22)
    abort();

  if (l != 123)
    abort();
}

int main(void) {
  struct tiny x[3];
  x[0].c = 10;
  x[1].c = 11;
  x[2].c = 12;
  x[0].d = 20;
  x[1].d = 21;
  x[2].d = 22;
  f(3, x[0], x[1], x[2], (long)123);
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct tiny {
// CHECK-NEXT:     c: i8,
// CHECK-NEXT:     d: i8,
// CHECK-NEXT: }
// CHECK-NEXT: fn f(mut n: i32, mut x: tiny, mut y: tiny, mut z: tiny, mut l: i64) {
// CHECK-NEXT:     if (x.c as i32) != (10 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (x.d as i32) != (20 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (y.c as i32) != (11 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (y.d as i32) != (21 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (z.c as i32) != (12 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (z.d as i32) != (22 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if l != ((123 as i32) as i64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: [tiny; 3] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe { (x.as_mut_ptr() as *mut tiny).offset((0 as i32) as isize) }).c = (10
// CHECK-NEXT:             as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe { (x.as_mut_ptr() as *mut tiny).offset((1 as i32) as isize) }).c = (11
// CHECK-NEXT:             as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe { (x.as_mut_ptr() as *mut tiny).offset((2 as i32) as isize) }).c = (12
// CHECK-NEXT:             as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe { (x.as_mut_ptr() as *mut tiny).offset((0 as i32) as isize) }).d = (20
// CHECK-NEXT:             as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe { (x.as_mut_ptr() as *mut tiny).offset((1 as i32) as isize) }).d = (21
// CHECK-NEXT:             as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe { (x.as_mut_ptr() as *mut tiny).offset((2 as i32) as isize) }).d = (22
// CHECK-NEXT:             as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     f(
// CHECK-NEXT:         3 as i32,
// CHECK-NEXT:         unsafe { *unsafe { (x.as_mut_ptr() as *mut tiny).offset((0 as i32) as isize) } },
// CHECK-NEXT:         unsafe { *unsafe { (x.as_mut_ptr() as *mut tiny).offset((1 as i32) as isize) } },
// CHECK-NEXT:         unsafe { *unsafe { (x.as_mut_ptr() as *mut tiny).offset((2 as i32) as isize) } },
// CHECK-NEXT:         (123 as i32) as i64,
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
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
