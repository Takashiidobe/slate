/* PR tree-optimization/65215 */

__attribute__((noinline, noclone)) unsigned int foo(unsigned char *p) {
  return ((unsigned int)p[0] << 24) | (p[1] << 16) | (p[2] << 8) | p[3];
}

__attribute__((noinline, noclone)) unsigned int bar(unsigned char *p) {
  return ((unsigned int)p[3] << 24) | (p[2] << 16) | (p[1] << 8) | p[0];
}

struct S {
  unsigned int  a;
  unsigned char b[5];
};

int main() {
  struct S s = {1, {2, 3, 4, 5, 6}};
  if (__CHAR_BIT__ != 8 || sizeof(unsigned int) != 4)
    return 0;
  if (foo(&s.b[1]) != 0x03040506U || bar(&s.b[1]) != 0x06050403U)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/65215
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut p: *mut u8) -> u32 {
// CHECK-NEXT:     ((unsafe { *unsafe { p.offset((0 as i32) as isize) } }) as u32) << (24 as i32)
// CHECK-NEXT:         | (((((unsafe { *unsafe { p.offset((1 as i32) as isize) } }) as u32) as i32) << (16 as i32))
// CHECK-NEXT:             as u32)
// CHECK-NEXT:         | (((((unsafe { *unsafe { p.offset((2 as i32) as isize) } }) as u32) as i32) << (8 as i32))
// CHECK-NEXT:             as u32)
// CHECK-NEXT:         | ((((unsafe { *unsafe { p.offset((3 as i32) as isize) } }) as u32) as i32) as u32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut p: *mut u8) -> u32 {
// CHECK-NEXT:     ((unsafe { *unsafe { p.offset((3 as i32) as isize) } }) as u32) << (24 as i32)
// CHECK-NEXT:         | (((((unsafe { *unsafe { p.offset((2 as i32) as isize) } }) as u32) as i32) << (16 as i32))
// CHECK-NEXT:             as u32)
// CHECK-NEXT:         | (((((unsafe { *unsafe { p.offset((1 as i32) as isize) } }) as u32) as i32) << (8 as i32))
// CHECK-NEXT:             as u32)
// CHECK-NEXT:         | ((((unsafe { *unsafe { p.offset((0 as i32) as isize) } }) as u32) as i32) as u32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
// CHECK-NEXT:     a: u32,
// CHECK-NEXT:     b: [u8; 5],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut s: S = S {
// CHECK-NEXT:         a: (1 as i32) as u32,
// CHECK-NEXT:         b: [
// CHECK-NEXT:             ((2 as i32) as i8) as u8,
// CHECK-NEXT:             ((3 as i32) as i8) as u8,
// CHECK-NEXT:             ((4 as i32) as i8) as u8,
// CHECK-NEXT:             ((5 as i32) as i8) as u8,
// CHECK-NEXT:             ((6 as i32) as i8) as u8,
// CHECK-NEXT:         ],
// CHECK-NEXT:     };
// CHECK-NEXT:     if (8 as i32) != (8 as i32) || (4 as u64) != (((4 as i32) as i64) as u64) {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: bool;
// CHECK-NEXT:     if foo(unsafe { (s.b.as_mut_ptr() as *mut u8).offset((1 as i32) as isize) })
// CHECK-NEXT:         != (50595078 as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         [[V_0]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_0]] = bar(unsafe { (s.b.as_mut_ptr() as *mut u8).offset((1 as i32) as isize) })
// CHECK-NEXT:             != (100992003 as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_0]] {
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
