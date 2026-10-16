extern void abort(void);

short g_3;

int main(void) {
  int l_2;
  for (l_2 = -1; l_2 != 0; l_2 = (unsigned char)(l_2 - 1))
    g_3 |= l_2;
  if (g_3 != -1)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: static mut g_3: i16 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut l_2: i32 = -(1 as i32);
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while l_2 != (0 as i32) {
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i16 = unsafe { g_3 };
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i16 = (([[V_0]] as i32) | l_2) as i16;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             g_3 = [[V_1]];
// CHECK-NEXT:         }
// CHECK-NEXT:         l_2 = ((((l_2 - (1 as i32)) as i8) as u8) as u32) as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((unsafe { g_3 }) as i32) != -(1 as i32) {
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
