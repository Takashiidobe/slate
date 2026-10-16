int g_21;
int g_211;
int g_261;

static void __attribute__((noinline, noclone)) func_32(int b) {
  if (b) {
  lbl_370:
    g_21 = 1;
  }

  for (g_261 = -1; g_261 > -2; g_261--) {
    if (g_211 + 1) {
      return;
    } else {
      g_21 = 1;
      goto lbl_370;
    }
  }
}

extern void abort(void);

int main(void) {
  func_32(0);
  if (g_261 != -1)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: static mut g_21: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut g_211: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut g_261: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn func_32(mut b: i32) {
// CHECK-NEXT:     let mut __slate_state: usize = 6usize;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         '[[JOIN_0:__join_[0-9]+]]: {
// CHECK-NEXT:             if b != (0 as i32) {
// CHECK-NEXT:                 __slate_state = 5;
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 __slate_state = 4;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         loop {
// CHECK-NEXT:             match __slate_state {
// CHECK-NEXT:                 4 => {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         g_261 = -(1 as i32);
// CHECK-NEXT:                     }
// CHECK-NEXT:                     if (unsafe { g_261 }) > -(2 as i32) {
// CHECK-NEXT:                         if (unsafe { g_211 }) + (1 as i32) != (0 as i32) {
// CHECK-NEXT:                             return;
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 g_21 = 1 as i32;
// CHECK-NEXT:                             }
// CHECK-NEXT:                             __slate_state = 5;
// CHECK-NEXT:                         }
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         break;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 _ => {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         g_21 = 1 as i32;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     __slate_state = 4;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     func_32(0 as i32);
// CHECK-NEXT:     if (unsafe { g_261 }) != -(1 as i32) {
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
