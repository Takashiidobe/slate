/* Test that we don't let stmt.c think that the enumeration's values are
   the entire set of possibilities.  Such an assumption is false for C,
   but true for other languages.  */

void abort(void);

enum X { X1 = 1, X2, X3, X4 };
static volatile enum X test = 0;
static void            y(int);

int main() {
  switch (test) {
  case X1:
    y(1);
    break;
  case X2:
    y(2);
    break;
  case X3:
    y(3);
    break;
  case X4:
    y(4);
    break;
  }
  return 0;
}

static void y(int x) { abort(); }

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Test that we don't let stmt.c think that the enumeration's values are
// CHECK-NEXT: //! the entire set of possibilities.  Such an assumption is false for C,
// CHECK-NEXT: //! but true for other languages.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut test: u32 = ((0 as i32) as u32) as u32;
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: {
// CHECK-NEXT:         match (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(test)) }) as u32 {
// CHECK-NEXT:             1 => {
// CHECK-NEXT:                 y(1 as i32);
// CHECK-NEXT:             }
// CHECK-NEXT:             2 => {
// CHECK-NEXT:                 y(2 as i32);
// CHECK-NEXT:             }
// CHECK-NEXT:             3 => {
// CHECK-NEXT:                 y(3 as i32);
// CHECK-NEXT:             }
// CHECK-NEXT:             4 => {
// CHECK-NEXT:                 y(4 as i32);
// CHECK-NEXT:             }
// CHECK-NEXT:             _ => {}
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn y(mut x: i32) {
// CHECK-NEXT:     unsafe { abort() };
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
