/* This code shows up in worse_state in ipa-pure-const.cc:
   *looping = MAX (*looping, looping2);
   was miscompiling it as just `return 1` though instead of
   `MAX_EXPR<*a, b>` (which should be transformed into `*a | b`
   note MAX_EXPR<bool, bool> is really `bool | bool` so we
   use that to compare against here.
 */
#define bool _Bool
bool __attribute__((noipa)) f(bool *a, bool b) {
  bool t = *a;
  if (t <= b)
    return b;
  return t;
}
bool __attribute__((noipa)) f1(bool *a, bool b) { return *a | b; }

int main() {
  int i = 0;
  int j = 0;

  for (i = 0; i <= 1; i++)
    for (j = 0; j <= 1; j++) {
      bool a = i;
      if (f(&a, j) != f1(&a, j))
        __builtin_abort();
    }
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! This code shows up in worse_state in ipa-pure-const.cc:
// CHECK-NEXT: //! *looping = MAX (*looping, looping2);
// CHECK-NEXT: //! was miscompiling it as just `return 1` though instead of
// CHECK-NEXT: //! `MAX_EXPR<*a, b>` (which should be transformed into `*a | b`
// CHECK-NEXT: //! note MAX_EXPR<bool, bool> is really `bool | bool` so we
// CHECK-NEXT: //! use that to compare against here.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f(mut a: &bool, mut b: bool) -> bool {
// CHECK-NEXT:     let mut t: bool = unsafe { *(a as *const bool) };
// CHECK-NEXT:     if (t as i32) <= (b as i32) {
// CHECK-NEXT:         return b;
// CHECK-NEXT:     }
// CHECK-NEXT:     t
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f1(mut a: &bool, mut b: bool) -> bool {
// CHECK-NEXT:     ((unsafe { *(a as *const bool) }) as i32) | (b as i32) != (0 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut j: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i <= (1 as i32) {
// CHECK-NEXT:         j = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while j <= (1 as i32) {
// CHECK-NEXT:             let mut a: bool = i != (0 as i32);
// CHECK-NEXT:             if (f(unsafe { &(*std::ptr::addr_of_mut!(a)) }, j != (0 as i32)) as i32)
// CHECK-NEXT:                 != (f1(unsafe { &(*std::ptr::addr_of_mut!(a)) }, j != (0 as i32)) as i32)
// CHECK-NEXT:             {
// CHECK-NEXT:                 unsafe { __builtin_abort() };
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_0:__v[0-9]+]]: i32 = j;
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:             j = [[V_1]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:         i = [[V_3]];
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
