void abort(void);
void exit(int);

typedef unsigned short Uint16;
typedef unsigned int   Uint;

Uint f() {
  Uint16        token;
  Uint          count;
  static Uint16 values[1] = {0x9300};

  token = values[0];
  count = token >> 8;

  return count;
}

int main() {
  if (f() != 0x93)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: fn f() -> u32 {
// CHECK-NEXT:     let mut token: u16 = unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(values) as *mut u16 }
// CHECK-NEXT:                 .offset((0 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut count: u32 = (((token as u32) as i32) >> (8 as i32)) as u32;
// CHECK-NEXT:     count
// CHECK-NEXT: }
// CHECK-NEXT: static mut values: [u16; 1] = [((37632 as i32) as i16) as u16];
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if f() != ((147 as i32) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
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
