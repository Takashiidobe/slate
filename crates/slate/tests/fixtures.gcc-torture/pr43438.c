extern void abort(void);

static unsigned char g_2 = 1;
static int           g_9;
static int          *l_8 = &g_9;

static void func_12(int p_13) {
  int *l_17  = &g_9;
  *l_17     &= 0 < p_13;
}

int main(void) {
  unsigned char l_11  = 254;
  *l_8               |= g_2;
  l_11               |= *l_8;
  func_12(l_11);
  if (g_9 != 1)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: static mut g_2: u8 = ((1 as i32) as i8) as u8;
// CHECK-NEXT: static mut g_9: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut l_8: *mut i32 = unsafe { std::ptr::addr_of_mut!(g_9) };
// CHECK-NEXT: fn func_12(mut p_13: i32) {
// CHECK-NEXT:     let mut l_17: *mut i32 = unsafe { std::ptr::addr_of_mut!(g_9) };
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: i32 = unsafe { *l_17 };
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] & (((0 as i32) < p_13) as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *l_17 = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut l_11: u8 = ((254 as i32) as i8) as u8;
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: *mut i32 = unsafe { l_8 };
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: i32 = unsafe { *[[V_2]] };
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: i32 = [[V_3]] | (((unsafe { g_2 }) as u32) as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[V_2]] = [[V_4]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_5:__v[0-9]+]]: u8 = l_11;
// CHECK-NEXT:     let mut [[V_6:__v[0-9]+]]: u8 = (((([[V_5]] as u32) as i32) | unsafe { *unsafe { l_8 } }) as i8)
// CHECK-NEXT:         as u8;
// CHECK-NEXT:     l_11 = [[V_6]];
// CHECK-NEXT:     func_12((l_11 as u32) as i32);
// CHECK-NEXT:     if (unsafe { g_9 }) != (1 as i32) {
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
