/* PR target/91635 */

#if __CHAR_BIT__ == 8 && __SIZEOF_SHORT__ == 2 && __SIZEOF_INT__ == 4 &&       \
    __SIZEOF_LONG_LONG__ == 8
unsigned short b, c;
int            u, v, w, x;

__attribute__((noipa)) int foo(unsigned short c) {
  c <<= __builtin_add_overflow(-c, -1, &b);
  c >>= 1;
  return c;
}

__attribute__((noipa)) int bar(unsigned short b) {
  b <<= -14 & 15;
  b   = b >> -~1;
  return b;
}

__attribute__((noipa)) int baz(unsigned short e) {
  e <<= 1;
  e >>= __builtin_add_overflow(8719476735, u, &v);
  return e;
}

__attribute__((noipa)) int qux(unsigned int e) {
  c  = ~1;
  c *= e;
  c  = c >> (-15 & 5);
  return c + w + x;
}
#endif

int main() {
#if __CHAR_BIT__ == 8 && __SIZEOF_SHORT__ == 2 && __SIZEOF_INT__ == 4 &&       \
    __SIZEOF_LONG_LONG__ == 8
  if (foo(0xffff) != 0x7fff)
    __builtin_abort();
  if (bar(5) != 5)
    __builtin_abort();
  if (baz(~0) != 0x7fff)
    __builtin_abort();
  if (qux(2) != 0x7ffe)
    __builtin_abort();
#endif
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR target/91635
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: u16 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: u16 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut u: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut v: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut w: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut x: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut [[C_0:c_[0-9]+]]: u16) -> i32 {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: u16 = [[C_0]];
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: u16 = (((([[V_0]] as u32) as i32)
// CHECK-NEXT:         << (({
// CHECK-NEXT:             let [[T_0:__t[0-9]+]]: i128 = -(([[C_0]] as u32) as i32) as i128;
// CHECK-NEXT:             let [[T_1:__t[0-9]+]]: i128 = -(1 as i32) as i128;
// CHECK-NEXT:             let [[T_2:__t[0-9]+]]: i128 = [[T_0]].wrapping_add([[T_1]]);
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *unsafe { std::ptr::addr_of_mut!(b) } = [[T_2]] as u16;
// CHECK-NEXT:             }
// CHECK-NEXT:             [[T_0]].checked_add([[T_1]]).is_none() || (([[T_2]] as u16) as i128) != [[T_2]]
// CHECK-NEXT:         }) as i32)) as i16) as u16;
// CHECK-NEXT:     [[C_0]] = [[V_1]];
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: u16 = [[C_0]];
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: u16 = (((([[V_2]] as u32) as i32) >> (1 as i32)) as i16) as u16;
// CHECK-NEXT:     [[C_0]] = [[V_3]];
// CHECK-NEXT:     ([[C_0]] as u32) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut [[B_0:b_[0-9]+]]: u16) -> i32 {
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: u16 = [[B_0]];
// CHECK-NEXT:     let mut [[V_5:__v[0-9]+]]: u16 = (((([[V_4]] as u32) as i32) << (-(14 as i32) & (15 as i32))) as i16) as u16;
// CHECK-NEXT:     [[B_0]] = [[V_5]];
// CHECK-NEXT:     [[B_0]] = (((([[B_0]] as u32) as i32) >> -(!(1 as i32))) as i16) as u16;
// CHECK-NEXT:     ([[B_0]] as u32) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn baz(mut e: u16) -> i32 {
// CHECK-NEXT:     let mut [[V_6:__v[0-9]+]]: u16 = e;
// CHECK-NEXT:     let mut [[V_7:__v[0-9]+]]: u16 = (((([[V_6]] as u32) as i32) << (1 as i32)) as i16) as u16;
// CHECK-NEXT:     e = [[V_7]];
// CHECK-NEXT:     let mut [[V_8:__v[0-9]+]]: u16 = e;
// CHECK-NEXT:     let mut [[V_9:__v[0-9]+]]: u16 = (((([[V_8]] as u32) as i32)
// CHECK-NEXT:         >> (({
// CHECK-NEXT:             let [[T_0]]: i128 = (8719476735 as i64) as i128;
// CHECK-NEXT:             let [[T_1]]: i128 = (unsafe { u }) as i128;
// CHECK-NEXT:             let [[T_2]]: i128 = [[T_0]].wrapping_add([[T_1]]);
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *unsafe { std::ptr::addr_of_mut!(v) } = [[T_2]] as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             [[T_0]].checked_add([[T_1]]).is_none() || (([[T_2]] as i32) as i128) != [[T_2]]
// CHECK-NEXT:         }) as i32)) as i16) as u16;
// CHECK-NEXT:     e = [[V_9]];
// CHECK-NEXT:     (e as u32) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn qux(mut e: u32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         c = (!(1 as i32) as i16) as u16;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_10:__v[0-9]+]]: u16 = unsafe { c };
// CHECK-NEXT:     let mut [[V_11:__v[0-9]+]]: u16 = ((([[V_10]] as u32) as i32) as u32).wrapping_mul(e) as u16;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         c = [[V_11]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         c = (((((unsafe { c }) as u32) as i32) >> (-(15 as i32) & (5 as i32))) as i16) as u16;
// CHECK-NEXT:     }
// CHECK-NEXT:     (((unsafe { c }) as u32) as i32) + unsafe { w } + unsafe { x }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if foo(((65535 as i32) as i16) as u16) != (32767 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if bar(((5 as i32) as i16) as u16) != (5 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if baz((!(0 as i32) as i16) as u16) != (32767 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if qux((2 as i32) as u32) != (32766 as i32) {
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
