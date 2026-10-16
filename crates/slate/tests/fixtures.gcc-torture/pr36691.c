unsigned char g_5;

void func_1(void) {
  for (g_5 = 9; g_5 >= 4; g_5 -= 5)
    ;
}

extern void abort(void);
int         main(void) {
  func_1();
  if (g_5 != 0)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: static mut g_5: u8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn func_1() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         g_5 = ((9 as i32) as i8) as u8;
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (((unsafe { g_5 }) as u32) as i32) >= (4 as i32) {
// CHECK-NEXT:         {}
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: u8 = unsafe { g_5 };
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: u8 = (((([[V_0]] as u32) as i32) - (5 as i32)) as i8) as u8;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             g_5 = [[V_1]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     func_1();
// CHECK-NEXT:     if (((unsafe { g_5 }) as u32) as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
