/* PR tree-optimization/114965 */

static void foo(const char *x) {

  char a = '0';
  while (1) {
    switch (*x) {
    case '_':
    case '+':
      a = *x;
      x++;
      continue;
    default:
      break;
    }
    break;
  }
  if (a == '0' || a == '+')
    __builtin_abort();
}

int main() { foo("_"); }

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/114965
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: *const i8) {
// CHECK-NEXT:     let mut a: i8 = (48 as i32) as i8;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (1 as i32) != (0 as i32) {
// CHECK-NEXT:         '[[SLATE_CONTINUE_0:__slate_continue_[0-9]+]]: {
// CHECK-NEXT:             '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: {
// CHECK-NEXT:                 match (unsafe { *x }) as i32 {
// CHECK-NEXT:                     43 | 95 => {
// CHECK-NEXT:                         a = unsafe { *x };
// CHECK-NEXT:                         let mut [[V_0:__v[0-9]+]]: *const i8 = x;
// CHECK-NEXT:                         let mut [[V_1:__v[0-9]+]]: *const i8 = unsafe { [[V_0]].offset((1 as i32) as isize) };
// CHECK-NEXT:                         x = [[V_1]];
// CHECK-NEXT:                         break '[[SLATE_CONTINUE_0]];
// CHECK-NEXT:                     }
// CHECK-NEXT:                     _ => {}
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             break '[[SLATE_BREAK_0]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if (a as i32) == (48 as i32) || (a as i32) == (43 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     foo(c"_".as_ptr() as *const i8);
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
