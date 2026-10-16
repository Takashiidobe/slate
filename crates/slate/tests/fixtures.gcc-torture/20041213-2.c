/* PR tree-optimization/18694

   The dominator optimization didn't take the PHI evaluation order
   into account when threading an edge.  */

extern void abort(void) __attribute__((noreturn));
extern void exit(int) __attribute__((noreturn));

void __attribute__((noinline)) foo(int i) {
  int next_n = 1;
  int j      = 0;

  for (; i != 0; i--) {
    int n;

    for (n = next_n; j < n; j++)
      next_n++;

    if (j != n)
      abort();
  }
}

int main(void) {
  foo(2);
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/18694
// CHECK-NEXT: //!
// CHECK-NEXT: //! The dominator optimization didn't take the PHI evaluation order
// CHECK-NEXT: //! into account when threading an edge.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut i: i32) {
// CHECK-NEXT:     let mut next_n: i32 = 1 as i32;
// CHECK-NEXT:     let mut j: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i != (0 as i32) {
// CHECK-NEXT:         let mut n: i32 = next_n;
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while j < n {
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: i32 = next_n;
// CHECK-NEXT:             let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:             next_n = [[V_2]];
// CHECK-NEXT:             let mut [[V_3:__v[0-9]+]]: i32 = j;
// CHECK-NEXT:             let mut [[V_4:__v[0-9]+]]: i32 = [[V_3]] + (1 as i32);
// CHECK-NEXT:             j = [[V_4]];
// CHECK-NEXT:         }
// CHECK-NEXT:         if j != n {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: i32 = [[V_5]] - (1 as i32);
// CHECK-NEXT:         i = [[V_6]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     foo(2 as i32);
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
