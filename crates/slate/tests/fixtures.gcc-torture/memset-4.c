/* Test to make sure memset of small old size works
   correctly. */
#define SIZE 15

void f(char *a) __attribute__((noinline));
void f(char *a) { __builtin_memset(a, 0, SIZE); }

int main(void) {
  int  i;
  char b[SIZE];
  for (i = 0; i < sizeof(b); i++) {
    b[i] = i;
  }
  f(b);
  for (i = 0; i < sizeof(b); i++) {
    if (0 != b[i])
      __builtin_abort();
  }
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Test to make sure memset of small old size works
// CHECK-NEXT: //! correctly.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "memset"]
// CHECK-NEXT:     fn __builtin_memset([[V_0:__v[0-9]+]]: *mut (), [[V_1:__v[0-9]+]]: i32, [[V_2:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f(mut a: *mut i8) {
// CHECK-NEXT:     unsafe { __builtin_memset(a as *mut (), 0 as i32, ((15 as i32) as i64) as u64) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut b: [i8; 15] = [0 as i8; 15];
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while ((i as i64) as u64) < (15 as u64) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (b.as_mut_ptr() as *mut i8).offset(i as isize) } = i as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = [[V_3]] + (1 as i32);
// CHECK-NEXT:         i = [[V_4]];
// CHECK-NEXT:     }
// CHECK-NEXT:     f(b.as_mut_ptr() as *mut i8);
// CHECK-NEXT:     i = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while ((i as i64) as u64) < (15 as u64) {
// CHECK-NEXT:         if (0 as i32)
// CHECK-NEXT:             != ((unsafe { *unsafe { (b.as_mut_ptr() as *mut i8).offset(i as isize) } }) as i32)
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: i32 = [[V_5]] + (1 as i32);
// CHECK-NEXT:         i = [[V_6]];
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
