extern void abort(void);

int main() {
  int            hicount = 0;
  unsigned char *c;
  char          *str = "\x7f\xff";
  for (c = (unsigned char *)str; *c; c++) {
    if (!(((unsigned int)(*c)) < 0x80))
      hicount++;
  }
  if (hicount != 1)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut hicount: i32 = 0 as i32;
// CHECK-NEXT:     let mut c: *mut u8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut str: *mut i8 = c"\x7f\xff".as_ptr() as *mut i8;
// CHECK-NEXT:     c = str as *mut u8;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (unsafe { *c }) != (0 as u8) {
// CHECK-NEXT:         if !(((unsafe { *c }) as u32) < ((128 as i32) as u32)) {
// CHECK-NEXT:             let mut [[V_0:__v[0-9]+]]: i32 = hicount;
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:             hicount = [[V_1]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: *mut u8 = c;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: *mut u8 = unsafe { [[V_2]].offset((1 as i32) as isize) };
// CHECK-NEXT:         c = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if hicount != (1 as i32) {
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
