/* PR rtl-optimization/28386 */
/* Origin: Volker Reichelt <reichelt@gcc.gnu.org> */

extern void abort(void);

volatile char s[256][3];

char g;

static void dummy(char a) { g = a; }

static int foo(void) {
  int i, j = 0;

  for (i = 0; i < 256; i++)
    if (i >= 128 && i < 256) {
      dummy(s[i - 128][0]);
      ++j;
    }

  return j;
}

int main(void) {
  if (foo() != 128)
    abort();

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/28386
// CHECK-NEXT: //!
// CHECK-NEXT: //! Origin: Volker Reichelt <reichelt@gcc.gnu.org>
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut s: __SlateAlign16<{{\[\[}}i8; 3]; 256]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: static mut g: i8 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn dummy(mut a: i8) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         g = a;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo() -> i32 {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut j: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (256 as i32) {
// CHECK-NEXT:         if i >= (128 as i32) && i < (256 as i32) {
// CHECK-NEXT:             dummy(unsafe {
// CHECK-NEXT:                 std::ptr::read_volatile(unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of_mut!(s.0) as *mut [i8; 3] }
// CHECK-NEXT:                                 .offset((i - (128 as i32)) as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                         .as_mut_ptr() as *mut i8
// CHECK-NEXT:                     }
// CHECK-NEXT:                     .offset((0 as i32) as isize)
// CHECK-NEXT:                 })
// CHECK-NEXT:             });
// CHECK-NEXT:             let mut [[V_0:__v[0-9]+]]: i32 = j;
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:             j = [[V_1]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:         i = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     j
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if foo() != (128 as i32) {
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
