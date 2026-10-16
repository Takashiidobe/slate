/* PR middle-end/90025 */

__attribute__((noipa)) void bar(char *p) {
  int i;
  for (i = 0; i < 6; i++)
    if (p[i] != "foobar"[i])
      __builtin_abort();
  for (; i < 32; i++)
    if (p[i] != '\0')
      __builtin_abort();
}

__attribute__((noipa)) void foo(__UINT32_TYPE__ x) {
  char s[32]                = {'f', 'o', 'o', 'b', 'a', 'r', 0};
  ((__UINT32_TYPE__ *)s)[2] = __builtin_bswap32(x);
  bar(s);
}

int main() {
  foo(0);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(link_llvm_intrinsics)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR middle-end/90025
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "llvm-intrinsic" {
// CHECK-NEXT:     #[link_name = "llvm.bswap.i32"]
// CHECK-NEXT:     fn __slate_intrinsic_e94e9cf815168b51([[ARG_0:arg[0-9]+]]: u32) -> u32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut p: *mut i8) {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (6 as i32) {
// CHECK-NEXT:         if ((unsafe { *unsafe { p.offset(i as isize) } }) as i32)
// CHECK-NEXT:             != ((unsafe { *unsafe { (c"foobar".as_ptr() as *mut i8).offset(i as isize) } }) as i32)
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         i = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while i < (32 as i32) {
// CHECK-NEXT:         if ((unsafe { *unsafe { p.offset(i as isize) } }) as i32) != (0 as i32) {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:         i = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: u32) {
// CHECK-NEXT:     let mut s: __SlateAlign16<[i8; 32]> = __SlateAlign16({
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: [i8; 32] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]][0] = (102 as i32) as i8;
// CHECK-NEXT:         [[T_0]][1] = (111 as i32) as i8;
// CHECK-NEXT:         [[T_0]][2] = (111 as i32) as i8;
// CHECK-NEXT:         [[T_0]][3] = (98 as i32) as i8;
// CHECK-NEXT:         [[T_0]][4] = (97 as i32) as i8;
// CHECK-NEXT:         [[T_0]][5] = (114 as i32) as i8;
// CHECK-NEXT:         [[T_0]][6] = (0 as i32) as i8;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     });
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (s.0.as_mut_ptr() as *mut u32).offset((2 as i32) as isize) } =
// CHECK-NEXT:             unsafe { __slate_intrinsic_e94e9cf815168b51(x) };
// CHECK-NEXT:     }
// CHECK-NEXT:     bar(s.0.as_mut_ptr() as *mut i8);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     foo((0 as i32) as u32);
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
