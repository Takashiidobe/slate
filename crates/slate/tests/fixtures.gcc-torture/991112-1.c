/* This code was miscompiled at -O3 on x86.
   Reported by Jim Meyering; distilled from bash.  */

void abort(void);
int  isprint(int);

int rl_show_char(int c) { return 0; }

int rl_character_len(int c, int pos) { return isprint(c) ? 1 : 2; }

int main(void) {
  int (*x)(int, int) = rl_character_len;
  if (x('a', 1) != 1)
    abort();
  if (x('\002', 1) != 2)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! This code was miscompiled at -O3 on x86.
// CHECK-NEXT: //! Reported by Jim Meyering; distilled from bash.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn isprint([[V_0:__v[0-9]+]]: i32) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn rl_show_char(mut c: i32) -> i32 {
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: extern "C-unwind" fn rl_character_len(mut c: i32, mut pos: i32) -> i32 {
// CHECK-NEXT:     if (unsafe { isprint(c) }) != (0 as i32) {
// CHECK-NEXT:         1 as i32
// CHECK-NEXT:     } else {
// CHECK-NEXT:         2 as i32
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: Option<unsafe extern "C-unwind" fn(i32, i32) -> i32> = Some(rl_character_len);
// CHECK-NEXT:     if (unsafe { x.unwrap()(97 as i32, 1 as i32) }) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { x.unwrap()(2 as i32, 1 as i32) }) != (2 as i32) {
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
