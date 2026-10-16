void add_unwind_adjustsp(long);
void abort(void);

unsigned char bytes[5];

void add_unwind_adjustsp(long offset) {
  int           n;
  unsigned long o;

  o = (long)((offset - 0x204) >> 2);

  n = 0;
  while (o) {
    bytes[n]   = o & 0x7f;
    o        >>= 7;
    if (o)
      bytes[n] |= 0x80;
    n++;
  }
}

int main(void) {
  add_unwind_adjustsp(4132);
  if (bytes[0] != 0x88 || bytes[1] != 0x07)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: static mut bytes: [u8; 5] = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn add_unwind_adjustsp(mut offset: i64) {
// CHECK-NEXT:     let mut n: i32 = 0 as i32;
// CHECK-NEXT:     let mut o: u64 = (offset - ((516 as i32) as i64) >> (2 as i32)) as u64;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while o != (0 as u64) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(bytes) as *mut u8 }.offset(n as isize)
// CHECK-NEXT:             } = (o & (((127 as i32) as i64) as u64)) as u8;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: u64 = o;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: u64 = [[V_0]] >> (7 as i32);
// CHECK-NEXT:         o = [[V_1]];
// CHECK-NEXT:         if o != (0 as u64) {
// CHECK-NEXT:             let mut [[V_2:__v[0-9]+]]: *mut u8 = unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(bytes) as *mut u8 }.offset(n as isize)
// CHECK-NEXT:             };
// CHECK-NEXT:             let mut [[V_3:__v[0-9]+]]: u8 = unsafe { *[[V_2]] };
// CHECK-NEXT:             let mut [[V_4:__v[0-9]+]]: u8 = (((([[V_3]] as u32) as i32) | (128 as i32)) as i8) as u8;
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *[[V_2]] = [[V_4]];
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: i32 = n;
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: i32 = [[V_5]] + (1 as i32);
// CHECK-NEXT:         n = [[V_6]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     add_unwind_adjustsp((4132 as i32) as i64);
// CHECK-NEXT:     if (((unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(bytes) as *mut u8 }
// CHECK-NEXT:                 .offset((0 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     }) as u32) as i32) != (136 as i32)
// CHECK-NEXT:         || (((unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(bytes) as *mut u8 }
// CHECK-NEXT:                     .offset((1 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) as u32) as i32) != (7 as i32)
// CHECK-NEXT:     {
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
