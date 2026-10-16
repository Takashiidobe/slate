/* PR target/21297 */
typedef __SIZE_TYPE__ size_t;
extern int            memcmp(const char *, const char *, size_t);
extern void           abort();

void foo(char *x) {
  int i;
  for (i = 0; i < 2; i++)
    ;
  x[i + i] = '\0';
}

void bar(char *x) {
  int i;
  for (i = 0; i < 2; i++)
    ;
  x[i + i + i + i] = '\0';
}

int main(void) {
  char x[] = "IJKLMNOPQR";
  foo(x);
  if (memcmp(x, "IJKL\0NOPQR", sizeof x) != 0)
    abort();
  x[4] = 'M';
  bar(x);
  if (memcmp(x, "IJKLMNOP\0R", sizeof x) != 0)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn memcmp([[V_0:__v[0-9]+]]: *const i8, [[V_1:__v[0-9]+]]: *const i8, [[V_2:__v[0-9]+]]: u64) -> i32;
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // PR target/21297
// CHECK-NEXT: fn foo(mut x: *mut i8) {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (2 as i32) {
// CHECK-NEXT:         {}
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = [[V_3]] + (1 as i32);
// CHECK-NEXT:         i = [[V_4]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { x.offset((i + i) as isize) } = (0 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut x: *mut i8) {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while i < (2 as i32) {
// CHECK-NEXT:         {}
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: i32 = [[V_5]] + (1 as i32);
// CHECK-NEXT:         i = [[V_6]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { x.offset((i + i + i + i) as isize) } = (0 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: [i8; 11] = [
// CHECK-NEXT:         73 as i8, 74 as i8, 75 as i8, 76 as i8, 77 as i8, 78 as i8, 79 as i8, 80 as i8, 81 as i8,
// CHECK-NEXT:         82 as i8, 0 as i8,
// CHECK-NEXT:     ];
// CHECK-NEXT:     foo(x.as_mut_ptr() as *mut i8);
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         memcmp(
// CHECK-NEXT:             x.as_mut_ptr() as *const i8,
// CHECK-NEXT:             b"IJKL\0NOPQR\0".as_ptr() as *const i8,
// CHECK-NEXT:             11 as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (x.as_mut_ptr() as *mut i8).offset((4 as i32) as isize) } = (77 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     bar(x.as_mut_ptr() as *mut i8);
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         memcmp(
// CHECK-NEXT:             x.as_mut_ptr() as *const i8,
// CHECK-NEXT:             b"IJKLMNOP\0R\0".as_ptr() as *const i8,
// CHECK-NEXT:             11 as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) != (0 as i32)
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
