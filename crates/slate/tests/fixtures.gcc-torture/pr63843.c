/* PR rtl-optimization/63843 */

static inline __attribute__((always_inline)) unsigned short
foo(unsigned short v) {
  return (v << 8) | (v >> 8);
}

unsigned short __attribute__((noinline, noclone, hot)) bar(unsigned char *x) {
  unsigned int   a;
  unsigned short b;
  __builtin_memcpy(&a, &x[0], sizeof(a));
  a ^= 0x80808080U;
  __builtin_memcpy(&x[0], &a, sizeof(a));
  __builtin_memcpy(&b, &x[2], sizeof(b));
  return foo(b);
}

int main() {
  unsigned char x[8] = {0x01, 0x01, 0x01, 0x01};
  if (__CHAR_BIT__ == 8 && sizeof(short) == 2 && sizeof(int) == 4 &&
      bar(x) != 0x8181U)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/63843
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "memcpy"]
// CHECK-NEXT:     fn __builtin_memcpy([[V_0:__v[0-9]+]]: *mut (), [[V_1:__v[0-9]+]]: *const (), [[V_2:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut v: u16) -> u16 {
// CHECK-NEXT:     ((((v as u32) as i32) << (8 as i32) | ((v as u32) as i32) >> (8 as i32)) as i16) as u16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut x: *mut u8) -> u16 {
// CHECK-NEXT:     let mut a: u32 = 0 as u32;
// CHECK-NEXT:     let mut b: u16 = 0 as u16;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memcpy(
// CHECK-NEXT:             std::ptr::addr_of_mut!(a) as *mut (),
// CHECK-NEXT:             (unsafe { x.offset((0 as i32) as isize) }) as *const (),
// CHECK-NEXT:             4 as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: u32 = a;
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: u32 = [[V_3]] ^ (2155905152 as u32);
// CHECK-NEXT:     a = [[V_4]];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memcpy(
// CHECK-NEXT:             (unsafe { x.offset((0 as i32) as isize) }) as *mut (),
// CHECK-NEXT:             std::ptr::addr_of_mut!(a) as *const (),
// CHECK-NEXT:             4 as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memcpy(
// CHECK-NEXT:             std::ptr::addr_of_mut!(b) as *mut (),
// CHECK-NEXT:             (unsafe { x.offset((2 as i32) as isize) }) as *const (),
// CHECK-NEXT:             2 as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     foo(b)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: [u8; 8] = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: [u8; 8] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]][0] = ((1 as i32) as i8) as u8;
// CHECK-NEXT:         [[T_0]][1] = ((1 as i32) as i8) as u8;
// CHECK-NEXT:         [[T_0]][2] = ((1 as i32) as i8) as u8;
// CHECK-NEXT:         [[T_0]][3] = ((1 as i32) as i8) as u8;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[V_5:__v[0-9]+]]: bool;
// CHECK-NEXT:     if (8 as i32) == (8 as i32)
// CHECK-NEXT:         && (2 as u64) == (((2 as i32) as i64) as u64)
// CHECK-NEXT:         && (4 as u64) == (((4 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         [[V_5]] = (((bar(x.as_mut_ptr() as *mut u8) as u32) as i32) as u32) != (33153 as u32);
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_5]] = false as bool;
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_5]] {
// CHECK-NEXT:         unsafe { __builtin_abort() };
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
