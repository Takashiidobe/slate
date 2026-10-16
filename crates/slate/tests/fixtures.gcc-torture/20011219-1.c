/* This testcase failed on IA-32 at -O and above, because combine attached
   a REG_LABEL note to jump instruction already using JUMP_LABEL.  */

extern void abort(void);
extern void exit(int);

enum X { A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q };

void bar(const char *x, int y, const char *z) {}

long foo(enum X x, const void *y) {
  long a;

  switch (x) {
  case K:
    a = *(long *)y;
    break;
  case L:
    a = *(long *)y;
    break;
  case M:
    a = *(long *)y;
    break;
  case N:
    a = *(long *)y;
    break;
  case O:
    a = *(long *)y;
    break;
  default:
    bar("foo", 1, "bar");
  }
  return a;
}

int main() {
  long i = 24;
  if (foo(N, &i) != 24)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! This testcase failed on IA-32 at -O and above, because combine attached
// CHECK-NEXT: //! a REG_LABEL note to jump instruction already using JUMP_LABEL.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut x: *const i8, mut y: i32, mut z: *const i8) {}
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: u32, mut y: &()) -> i64 {
// CHECK-NEXT:     let mut a: i64 = 0 as i64;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: {
// CHECK-NEXT:         match x as u32 {
// CHECK-NEXT:             10 => {
// CHECK-NEXT:                 a = unsafe { *((y as *const ()) as *mut i64) };
// CHECK-NEXT:             }
// CHECK-NEXT:             11 => {
// CHECK-NEXT:                 a = unsafe { *((y as *const ()) as *mut i64) };
// CHECK-NEXT:             }
// CHECK-NEXT:             12 => {
// CHECK-NEXT:                 a = unsafe { *((y as *const ()) as *mut i64) };
// CHECK-NEXT:             }
// CHECK-NEXT:             13 => {
// CHECK-NEXT:                 a = unsafe { *((y as *const ()) as *mut i64) };
// CHECK-NEXT:             }
// CHECK-NEXT:             14 => {
// CHECK-NEXT:                 a = unsafe { *((y as *const ()) as *mut i64) };
// CHECK-NEXT:             }
// CHECK-NEXT:             _ => {
// CHECK-NEXT:                 bar(
// CHECK-NEXT:                     c"foo".as_ptr() as *const i8,
// CHECK-NEXT:                     1 as i32,
// CHECK-NEXT:                     c"bar".as_ptr() as *const i8,
// CHECK-NEXT:                 );
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     a
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i64 = (24 as i32) as i64;
// CHECK-NEXT:     if foo((13 as i32) as u32, unsafe {
// CHECK-NEXT:         &(*(std::ptr::addr_of_mut!(i) as *const ()))
// CHECK-NEXT:     }) != ((24 as i32) as i64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
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
