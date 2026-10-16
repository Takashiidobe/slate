/* PR rtl-optimization/65401 */

struct S {
  unsigned short s[64];
};

__attribute__((noinline, noclone)) void foo(struct S *x) {
  unsigned int   i;
  unsigned char *s;

  s = (unsigned char *)x->s;
  for (i = 0; i < 64; i++)
    x->s[i] = s[i * 2] | (s[i * 2 + 1] << 8);
}

__attribute__((noinline, noclone)) void bar(struct S *x) {
  unsigned int   i;
  unsigned char *s;

  s = (unsigned char *)x->s;
  for (i = 0; i < 64; i++)
    x->s[i] = (s[i * 2] << 8) | s[i * 2 + 1];
}

int main() {
  unsigned int i;
  struct S     s;
  if (sizeof(unsigned short) != 2)
    return 0;
  for (i = 0; i < 64; i++)
    s.s[i] = i + ((64 - i) << 8);
  foo(&s);
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
  for (i = 0; i < 64; i++)
    if (s.s[i] != (64 - i) + (i << 8))
      __builtin_abort();
#elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  for (i = 0; i < 64; i++)
    if (s.s[i] != i + ((64 - i) << 8))
      __builtin_abort();
#endif
  for (i = 0; i < 64; i++)
    s.s[i] = i + ((64 - i) << 8);
  bar(&s);
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  for (i = 0; i < 64; i++)
    if (s.s[i] != (64 - i) + (i << 8))
      __builtin_abort();
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
  for (i = 0; i < 64; i++)
    if (s.s[i] != i + ((64 - i) << 8))
      __builtin_abort();
#endif
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/65401
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
// CHECK-NEXT:     s: [u16; 64],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: &mut S) {
// CHECK-NEXT:     let mut i: u32 = (0 as i32) as u32;
// CHECK-NEXT:     let mut s: *mut u8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     s = (unsafe { (*(x as *mut S)).s.as_mut_ptr() as *mut u16 }) as *mut u8;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < ((64 as i32) as u32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { unsafe { (*(x as *mut S)).s.as_mut_ptr() as *mut u16 }.offset(i as isize) } =
// CHECK-NEXT:                 (((((unsafe { *unsafe { s.offset(i.wrapping_mul((2 as i32) as u32) as isize) } })
// CHECK-NEXT:                     as u32) as i32)
// CHECK-NEXT:                     | (((unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             s.offset(
// CHECK-NEXT:                                 i.wrapping_mul((2 as i32) as u32)
// CHECK-NEXT:                                     .wrapping_add((1 as i32) as u32)
// CHECK-NEXT:                                     as isize,
// CHECK-NEXT:                             )
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }) as u32) as i32)
// CHECK-NEXT:                         << (8 as i32)) as i16) as u16;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: u32 = i;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: u32 = [[V_0]].wrapping_add((1 as i32) as u32);
// CHECK-NEXT:         i = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut x: &mut S) {
// CHECK-NEXT:     let mut i: u32 = (0 as i32) as u32;
// CHECK-NEXT:     let mut s: *mut u8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     s = (unsafe { (*(x as *mut S)).s.as_mut_ptr() as *mut u16 }) as *mut u8;
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while i < ((64 as i32) as u32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { unsafe { (*(x as *mut S)).s.as_mut_ptr() as *mut u16 }.offset(i as isize) } =
// CHECK-NEXT:                 (((((unsafe { *unsafe { s.offset(i.wrapping_mul((2 as i32) as u32) as isize) } })
// CHECK-NEXT:                     as u32) as i32)
// CHECK-NEXT:                     << (8 as i32)
// CHECK-NEXT:                     | (((unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             s.offset(
// CHECK-NEXT:                                 i.wrapping_mul((2 as i32) as u32)
// CHECK-NEXT:                                     .wrapping_add((1 as i32) as u32)
// CHECK-NEXT:                                     as isize,
// CHECK-NEXT:                             )
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }) as u32) as i32)) as i16) as u16;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: u32 = i;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: u32 = [[V_2]].wrapping_add((1 as i32) as u32);
// CHECK-NEXT:         i = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: u32 = (0 as i32) as u32;
// CHECK-NEXT:     let mut s: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     if (2 as u64) != (((2 as i32) as i64) as u64) {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while i < ((64 as i32) as u32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (s.s.as_mut_ptr() as *mut u16).offset(i as isize) } =
// CHECK-NEXT:                 i.wrapping_add(((64 as i32) as u32).wrapping_sub(i) << (8 as i32)) as u16;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: u32 = i;
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: u32 = [[V_4]].wrapping_add((1 as i32) as u32);
// CHECK-NEXT:         i = [[V_5]];
// CHECK-NEXT:     }
// CHECK-NEXT:     foo(unsafe { &mut (*std::ptr::addr_of_mut!(s)) });
// CHECK-NEXT:     i = (0 as i32) as u32;
// CHECK-NEXT:     '[[SLATE_BREAK_3:__slate_break_[0-9]+]]: while i < ((64 as i32) as u32) {
// CHECK-NEXT:         if ((((unsafe { *unsafe { (s.s.as_mut_ptr() as *mut u16).offset(i as isize) } }) as u32)
// CHECK-NEXT:             as i32) as u32)
// CHECK-NEXT:             != i.wrapping_add(((64 as i32) as u32).wrapping_sub(i) << (8 as i32))
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: u32 = i;
// CHECK-NEXT:         let mut [[V_7:__v[0-9]+]]: u32 = [[V_6]].wrapping_add((1 as i32) as u32);
// CHECK-NEXT:         i = [[V_7]];
// CHECK-NEXT:     }
// CHECK-NEXT:     i = (0 as i32) as u32;
// CHECK-NEXT:     '[[SLATE_BREAK_4:__slate_break_[0-9]+]]: while i < ((64 as i32) as u32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (s.s.as_mut_ptr() as *mut u16).offset(i as isize) } =
// CHECK-NEXT:                 i.wrapping_add(((64 as i32) as u32).wrapping_sub(i) << (8 as i32)) as u16;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_8:__v[0-9]+]]: u32 = i;
// CHECK-NEXT:         let mut [[V_9:__v[0-9]+]]: u32 = [[V_8]].wrapping_add((1 as i32) as u32);
// CHECK-NEXT:         i = [[V_9]];
// CHECK-NEXT:     }
// CHECK-NEXT:     bar(unsafe { &mut (*std::ptr::addr_of_mut!(s)) });
// CHECK-NEXT:     i = (0 as i32) as u32;
// CHECK-NEXT:     '[[SLATE_BREAK_5:__slate_break_[0-9]+]]: while i < ((64 as i32) as u32) {
// CHECK-NEXT:         if ((((unsafe { *unsafe { (s.s.as_mut_ptr() as *mut u16).offset(i as isize) } }) as u32)
// CHECK-NEXT:             as i32) as u32)
// CHECK-NEXT:             != ((64 as i32) as u32)
// CHECK-NEXT:                 .wrapping_sub(i)
// CHECK-NEXT:                 .wrapping_add(i << (8 as i32))
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_10:__v[0-9]+]]: u32 = i;
// CHECK-NEXT:         let mut [[V_11:__v[0-9]+]]: u32 = [[V_10]].wrapping_add((1 as i32) as u32);
// CHECK-NEXT:         i = [[V_11]];
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
