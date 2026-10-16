/* PR c/5430 */
/* Verify that the multiplicative folding code is not fooled
   by the mix between signed variables and unsigned constants. */

extern void abort(void);
extern void exit(int);

int main(void) {
  int          my_int = 924;
  unsigned int result;

  result = ((my_int * 2 + 4) - 8U) / 2;
  if (result != 922U)
    abort();

  result = ((my_int * 2 - 4U) + 2) / 2;
  if (result != 923U)
    abort();

  result = (((my_int + 2) * 2) - 8U - 4) / 2;
  if (result != 920U)
    abort();
  result = (((my_int + 2) * 2) - (8U + 4)) / 2;
  if (result != 920U)
    abort();

  result = ((my_int * 4 + 2U) - 4U) / 2;
  if (result != 1847U)
    abort();

  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR c/5430
// CHECK-NEXT: //!
// CHECK-NEXT: //! Verify that the multiplicative folding code is not fooled
// CHECK-NEXT: //! by the mix between signed variables and unsigned constants.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut my_int: i32 = 924 as i32;
// CHECK-NEXT:     let mut result: u32 =
// CHECK-NEXT:         ((my_int * (2 as i32) + (4 as i32)) as u32).wrapping_sub(8 as u32) / ((2 as i32) as u32);
// CHECK-NEXT:     if result != (922 as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     result = ((my_int * (2 as i32)) as u32)
// CHECK-NEXT:         .wrapping_sub(4 as u32)
// CHECK-NEXT:         .wrapping_add((2 as i32) as u32)
// CHECK-NEXT:         / ((2 as i32) as u32);
// CHECK-NEXT:     if result != (923 as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     result = (((my_int + (2 as i32)) * (2 as i32)) as u32)
// CHECK-NEXT:         .wrapping_sub(8 as u32)
// CHECK-NEXT:         .wrapping_sub((4 as i32) as u32)
// CHECK-NEXT:         / ((2 as i32) as u32);
// CHECK-NEXT:     if result != (920 as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     result = (((my_int + (2 as i32)) * (2 as i32)) as u32)
// CHECK-NEXT:         .wrapping_sub((8 as u32).wrapping_add((4 as i32) as u32))
// CHECK-NEXT:         / ((2 as i32) as u32);
// CHECK-NEXT:     if result != (920 as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     result = ((my_int * (4 as i32)) as u32)
// CHECK-NEXT:         .wrapping_add(2 as u32)
// CHECK-NEXT:         .wrapping_sub(4 as u32)
// CHECK-NEXT:         / ((2 as i32) as u32);
// CHECK-NEXT:     if result != (1847 as u32) {
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
