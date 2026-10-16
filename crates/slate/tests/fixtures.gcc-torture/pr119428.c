/* PR target/119428 */

__attribute__((noipa)) void foo(unsigned int x, unsigned char *y) {
  y  += x >> 3;
  *y &= (unsigned char)~(1 << (x & 0x07));
}

int main() {
  unsigned char buf[8];
  __builtin_memset(buf, 0xff, 8);
  foo(8, buf);
  if (buf[1] != 0xfe)
    __builtin_abort();
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR target/119428
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "memset"]
// CHECK-NEXT:     fn __builtin_memset([[V_0:__v[0-9]+]]: *mut (), [[V_1:__v[0-9]+]]: i32, [[V_2:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: u32, mut y: *mut u8) {
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: *mut u8 = y;
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: *mut u8 = unsafe { [[V_3]].offset((x >> (3 as i32)) as isize) };
// CHECK-NEXT:     y = [[V_4]];
// CHECK-NEXT:     let mut [[V_5:__v[0-9]+]]: u8 = unsafe { *y };
// CHECK-NEXT:     let mut [[V_6:__v[0-9]+]]: u8 = (((([[V_5]] as u32) as i32)
// CHECK-NEXT:         & ((((!((1 as i32) << (x & ((7 as i32) as u32))) as i8) as u8) as u32) as i32))
// CHECK-NEXT:         as i8) as u8;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *y = [[V_6]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut buf: [u8; 8] = [0 as u8; 8];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memset(
// CHECK-NEXT:             buf.as_mut_ptr() as *mut (),
// CHECK-NEXT:             255 as i32,
// CHECK-NEXT:             ((8 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     foo((8 as i32) as u32, buf.as_mut_ptr() as *mut u8);
// CHECK-NEXT:     if (((unsafe { *unsafe { (buf.as_mut_ptr() as *mut u8).offset((1 as i32) as isize) } }) as u32)
// CHECK-NEXT:         as i32)
// CHECK-NEXT:         != (254 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
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
