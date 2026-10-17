#include <stdio.h>

struct S156 {
  unsigned _BitInt(156) a : 135;
  _BitInt(156) b          : 2;
};

struct T156 {
  _BitInt(156) a          : 2;
  unsigned _BitInt(156) b : 135;
  _BitInt(156) c          : 2;
};

struct Mixed {
  int tag : 5;
  _BitInt(100) mid : 70;
  unsigned char flag : 1;
  unsigned _BitInt(200) top : 190;
  _BitInt(7) small : 7;
};

struct Narrow {
  _BitInt(100) x : 3;
  unsigned _BitInt(100) y : 64;
};

static _BitInt(156) combine(struct S156 *p, struct T156 *q, struct T156 *r, int n) {
  r[0].b = p[0].a + q[0].b;
  r[1].b = p[1].a * q[1].b;
  r[2].a = p[2].a == q[2].b;
  r[3].a = p[3].a < q[3].b;
  r[4].b = p[4].a << n;
  return p[5].a + q[5].b;
}

static void dump(struct Mixed *m) {
  printf("%d %lld %d %llu %d\n", m->tag, (long long)m->mid, m->flag,
         (unsigned long long)m->top, (int)m->small);
  printf("%llu\n", (unsigned long long)(m->top >> 100));
}

int main(void) {
  struct S156 s[6];
  struct T156 t[6], r[5] = {0};
  for (int i = 0; i < 6; ++i) {
    s[i].a = (unsigned _BitInt(156))1000 * (i + 3) + 7;
    s[i].b = (i & 1) ? -1 : 1;
    t[i].a = -1;
    t[i].b = ((unsigned _BitInt(156))1 << 130) + i;
    t[i].c = 1;
  }
  _BitInt(156) ret = combine(s, t, r, 5);
  printf("%d\n", (int)ret);
  for (int i = 0; i < 5; ++i)
    printf("%d %d %d %llu\n", (int)r[i].a, (int)r[i].c, (int)r[i].b,
           (unsigned long long)(r[i].b >> 120));

  struct Mixed m = {0};
  m.tag = -9;
  m.mid = -123456789012345LL;
  m.flag = 1;
  m.top = 1234567890123456789ULL;
  m.top <<= 60;
  m.top += 12345;
  m.small = -50;
  dump(&m);
  struct Mixed copy = m;
  copy.mid += 1;
  copy.top -= 5;
  copy.small = 63;
  dump(&copy);
  dump(&m);

  struct Narrow n = {-4, 0xfedcba9876543210ULL};
  printf("%d %llx\n", (int)n.x, (unsigned long long)n.y);
  n.x = 3;
  n.y += 1;
  printf("%d %llx %zu %zu\n", (int)n.x, (unsigned long long)n.y,
         sizeof(struct Mixed), sizeof(struct S156));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(8))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S156 {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits0U0,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(8))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct T156 {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits1U0,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(8))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Mixed {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits2U0,
// CHECK-NEXT:     __slate_bits_1: __slate_bits::__SlateBits2U1,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(8))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Narrow {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits3U0,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[derive(Clone, Copy)]
// CHECK-NEXT:     pub struct __SlateBits0U0 {
// CHECK-NEXT:         pub bytes: [u8; 18],
// CHECK-NEXT:     }
// CHECK-NEXT:     #[derive(Clone, Copy)]
// CHECK-NEXT:     pub struct __SlateBits1U0 {
// CHECK-NEXT:         pub bytes: [u8; 18],
// CHECK-NEXT:     }
// CHECK-NEXT:     #[derive(Clone, Copy)]
// CHECK-NEXT:     pub struct __SlateBits2U0 {
// CHECK-NEXT:         pub bytes: [u8; 34],
// CHECK-NEXT:     }
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u8,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits2U1 {
// CHECK-NEXT:         #[bits(7)]
// CHECK-NEXT:         pub small: i8,
// CHECK-NEXT:         #[bits(1, access = na)]
// CHECK-NEXT:         pub __slate_pad_1: u8,
// CHECK-NEXT:     }
// CHECK-NEXT:     #[derive(Clone, Copy)]
// CHECK-NEXT:     pub struct __SlateBits3U0 {
// CHECK-NEXT:         pub bytes: [u8; 9],
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl __slate_bits::__SlateBits0U0 {
// CHECK-NEXT:     const fn __get_a(&self) -> bitint::BUint<156, 3, 24, bitint::A8> {
// CHECK-NEXT:         <bitint::BUint<156, 3, 24, bitint::A8>>::from_bit_range(&self.bytes, 0, 135)
// CHECK-NEXT:     }
// CHECK-NEXT:     const fn __set_a(&mut self, value: bitint::BUint<156, 3, 24, bitint::A8>) {
// CHECK-NEXT:         value.write_bit_range(&mut self.bytes, 0, 135)
// CHECK-NEXT:     }
// CHECK-NEXT:     const fn __get_b(&self) -> bitint::BInt<156, 3, 24, bitint::A8> {
// CHECK-NEXT:         <bitint::BInt<156, 3, 24, bitint::A8>>::from_bit_range(&self.bytes, 135, 2)
// CHECK-NEXT:     }
// CHECK-NEXT:     const fn __set_b(&mut self, value: bitint::BInt<156, 3, 24, bitint::A8>) {
// CHECK-NEXT:         value.write_bit_range(&mut self.bytes, 135, 2)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl __slate_bits::__SlateBits1U0 {
// CHECK-NEXT:     const fn __get_a(&self) -> bitint::BInt<156, 3, 24, bitint::A8> {
// CHECK-NEXT:         <bitint::BInt<156, 3, 24, bitint::A8>>::from_bit_range(&self.bytes, 0, 2)
// CHECK-NEXT:     }
// CHECK-NEXT:     const fn __set_a(&mut self, value: bitint::BInt<156, 3, 24, bitint::A8>) {
// CHECK-NEXT:         value.write_bit_range(&mut self.bytes, 0, 2)
// CHECK-NEXT:     }
// CHECK-NEXT:     const fn __get_b(&self) -> bitint::BUint<156, 3, 24, bitint::A8> {
// CHECK-NEXT:         <bitint::BUint<156, 3, 24, bitint::A8>>::from_bit_range(&self.bytes, 2, 135)
// CHECK-NEXT:     }
// CHECK-NEXT:     const fn __set_b(&mut self, value: bitint::BUint<156, 3, 24, bitint::A8>) {
// CHECK-NEXT:         value.write_bit_range(&mut self.bytes, 2, 135)
// CHECK-NEXT:     }
// CHECK-NEXT:     const fn __get_c(&self) -> bitint::BInt<156, 3, 24, bitint::A8> {
// CHECK-NEXT:         <bitint::BInt<156, 3, 24, bitint::A8>>::from_bit_range(&self.bytes, 137, 2)
// CHECK-NEXT:     }
// CHECK-NEXT:     const fn __set_c(&mut self, value: bitint::BInt<156, 3, 24, bitint::A8>) {
// CHECK-NEXT:         value.write_bit_range(&mut self.bytes, 137, 2)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl __slate_bits::__SlateBits2U0 {
// CHECK-NEXT:     const fn __get_tag(&self) -> i32 {
// CHECK-NEXT:         bitint::load_bits_signed(&self.bytes, 0, 5) as i32
// CHECK-NEXT:     }
// CHECK-NEXT:     const fn __set_tag(&mut self, value: i32) {
// CHECK-NEXT:         bitint::store_bits(&mut self.bytes, 0, 5, value as u128)
// CHECK-NEXT:     }
// CHECK-NEXT:     const fn __get_mid(&self) -> bitint::BInt<100, 2, 16, bitint::A8> {
// CHECK-NEXT:         <bitint::BInt<100, 2, 16, bitint::A8>>::from_bit_range(&self.bytes, 5, 70)
// CHECK-NEXT:     }
// CHECK-NEXT:     const fn __set_mid(&mut self, value: bitint::BInt<100, 2, 16, bitint::A8>) {
// CHECK-NEXT:         value.write_bit_range(&mut self.bytes, 5, 70)
// CHECK-NEXT:     }
// CHECK-NEXT:     const fn __get_flag(&self) -> u8 {
// CHECK-NEXT:         bitint::load_bits(&self.bytes, 75, 1) as u8
// CHECK-NEXT:     }
// CHECK-NEXT:     const fn __set_flag(&mut self, value: u8) {
// CHECK-NEXT:         bitint::store_bits(&mut self.bytes, 75, 1, value as u128)
// CHECK-NEXT:     }
// CHECK-NEXT:     const fn __get_top(&self) -> bitint::BUint<200, 4, 32, bitint::A8> {
// CHECK-NEXT:         <bitint::BUint<200, 4, 32, bitint::A8>>::from_bit_range(&self.bytes, 76, 190)
// CHECK-NEXT:     }
// CHECK-NEXT:     const fn __set_top(&mut self, value: bitint::BUint<200, 4, 32, bitint::A8>) {
// CHECK-NEXT:         value.write_bit_range(&mut self.bytes, 76, 190)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl __slate_bits::__SlateBits3U0 {
// CHECK-NEXT:     const fn __get_x(&self) -> bitint::BInt<100, 2, 16, bitint::A8> {
// CHECK-NEXT:         <bitint::BInt<100, 2, 16, bitint::A8>>::from_bit_range(&self.bytes, 0, 3)
// CHECK-NEXT:     }
// CHECK-NEXT:     const fn __set_x(&mut self, value: bitint::BInt<100, 2, 16, bitint::A8>) {
// CHECK-NEXT:         value.write_bit_range(&mut self.bytes, 0, 3)
// CHECK-NEXT:     }
// CHECK-NEXT:     const fn __get_y(&self) -> bitint::BUint<100, 2, 16, bitint::A8> {
// CHECK-NEXT:         <bitint::BUint<100, 2, 16, bitint::A8>>::from_bit_range(&self.bytes, 3, 64)
// CHECK-NEXT:     }
// CHECK-NEXT:     const fn __set_y(&mut self, value: bitint::BUint<100, 2, 16, bitint::A8>) {
// CHECK-NEXT:         value.write_bit_range(&mut self.bytes, 3, 64)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn combine(
// CHECK-NEXT:     p: *mut S156,
// CHECK-NEXT:     q: *mut T156,
// CHECK-NEXT:     r: *mut T156,
// CHECK-NEXT:     n: i32,
// CHECK-NEXT: ) -> bitint::BInt<156, 3, 24, bitint::A8> {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]] = unsafe { (*unsafe { p.offset(0 as isize) }).__slate_bits_0.__get_a() }
// CHECK-NEXT:             .wrapping_add(unsafe {
// CHECK-NEXT:                 (*unsafe { q.offset(0 as isize) }).__slate_bits_0.__get_b()
// CHECK-NEXT:             });
// CHECK-NEXT:         (*unsafe { r.offset(0 as isize) }).__slate_bits_0.__set_b([[T_0]]);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_1:__t[0-9]+]] = unsafe { (*unsafe { p.offset(1 as isize) }).__slate_bits_0.__get_a() }
// CHECK-NEXT:             .wrapping_mul(unsafe {
// CHECK-NEXT:                 (*unsafe { q.offset(1 as isize) }).__slate_bits_0.__get_b()
// CHECK-NEXT:             });
// CHECK-NEXT:         (*unsafe { r.offset(1 as isize) }).__slate_bits_0.__set_b([[T_1]]);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_2:__t[0-9]+]] = <bitint::BInt<
// CHECK-NEXT:             156,
// CHECK-NEXT:             3,
// CHECK-NEXT:             24,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_u128(
// CHECK-NEXT:             (unsafe { (*unsafe { p.offset(2 as isize) }).__slate_bits_0.__get_a() }
// CHECK-NEXT:                 .compare(unsafe {
// CHECK-NEXT:                     (*unsafe { q.offset(2 as isize) }).__slate_bits_0.__get_b()
// CHECK-NEXT:                 }) == 0) as u128,
// CHECK-NEXT:         );
// CHECK-NEXT:         (*unsafe { r.offset(2 as isize) }).__slate_bits_0.__set_a([[T_2]]);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_3:__t[0-9]+]] = <bitint::BInt<
// CHECK-NEXT:             156,
// CHECK-NEXT:             3,
// CHECK-NEXT:             24,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_u128(
// CHECK-NEXT:             (unsafe { (*unsafe { p.offset(3 as isize) }).__slate_bits_0.__get_a() }
// CHECK-NEXT:                 .compare(unsafe {
// CHECK-NEXT:                     (*unsafe { q.offset(3 as isize) }).__slate_bits_0.__get_b()
// CHECK-NEXT:                 }) < 0) as u128,
// CHECK-NEXT:         );
// CHECK-NEXT:         (*unsafe { r.offset(3 as isize) }).__slate_bits_0.__set_a([[T_3]]);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_4:__t[0-9]+]] = unsafe { (*unsafe { p.offset(4 as isize) }).__slate_bits_0.__get_a() }
// CHECK-NEXT:             .shl_bits(n as usize);
// CHECK-NEXT:         (*unsafe { r.offset(4 as isize) }).__slate_bits_0.__set_b([[T_4]]);
// CHECK-NEXT:     }
// CHECK-NEXT:     return <bitint::BInt<
// CHECK-NEXT:         156,
// CHECK-NEXT:         3,
// CHECK-NEXT:         24,
// CHECK-NEXT:         bitint::A8,
// CHECK-NEXT:     >>::from_buint(
// CHECK-NEXT:         unsafe { (*unsafe { p.offset(5 as isize) }).__slate_bits_0.__get_a() }
// CHECK-NEXT:             .wrapping_add(unsafe {
// CHECK-NEXT:                 (*unsafe { q.offset(5 as isize) }).__slate_bits_0.__get_b()
// CHECK-NEXT:             }),
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: fn dump(m: &mut Mixed) {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %lld %d %llu %d\n".as_ptr(),
// CHECK-NEXT:             unsafe { (*(m as *mut Mixed)).__slate_bits_0.__get_tag() },
// CHECK-NEXT:             unsafe { (*(m as *mut Mixed)).__slate_bits_0.__get_mid() }.to_i128() as i64,
// CHECK-NEXT:             ((unsafe { (*(m as *mut Mixed)).__slate_bits_0.__get_flag() }) as u32)
// CHECK-NEXT:                 as i32,
// CHECK-NEXT:             unsafe { (*(m as *mut Mixed)).__slate_bits_0.__get_top() }.to_u128() as u64,
// CHECK-NEXT:             (unsafe { (*(m as *mut Mixed)).__slate_bits_1.__get_small() }) as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%llu\n".as_ptr(),
// CHECK-NEXT:             unsafe { (*(m as *mut Mixed)).__slate_bits_0.__get_top() }
// CHECK-NEXT:                 .shr_bits(100 as usize)
// CHECK-NEXT:                 .to_u128() as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut s: __SlateAlign16<[S156; 6]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-NEXT:     let mut t: __SlateAlign16<[T156; 6]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-NEXT:     let mut r: __SlateAlign16<[T156; 5]> = __SlateAlign16({
// CHECK-NEXT:         let mut [[T_0]]: [T156; 5] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]][0] = {
// CHECK-NEXT:             let mut [[T_1]]: T156 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_1]].__slate_bits_0
// CHECK-NEXT:                 .__set_a(<bitint::BInt<156, 3, 24, bitint::A8>>::from_i128(0 as i128));
// CHECK-NEXT:             [[T_1]]
// CHECK-NEXT:         };
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     });
// CHECK-NEXT:     for i in 0 as i32..6 as i32 {
// CHECK-NEXT:         let [[T_2]] = <bitint::BUint<
// CHECK-NEXT:             156,
// CHECK-NEXT:             3,
// CHECK-NEXT:             24,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint(<bitint::BInt<156, 3, 24, bitint::A8>>::from_i128(1000 as i128))
// CHECK-NEXT:             .wrapping_mul(
// CHECK-NEXT:                 <bitint::BUint<
// CHECK-NEXT:                     156,
// CHECK-NEXT:                     3,
// CHECK-NEXT:                     24,
// CHECK-NEXT:                     bitint::A8,
// CHECK-NEXT:                 >>::from_bint(
// CHECK-NEXT:                     <bitint::BInt<156, 3, 24, bitint::A8>>::from_i128((i + 3) as i128),
// CHECK-NEXT:                 ),
// CHECK-NEXT:             )
// CHECK-NEXT:             .wrapping_add(
// CHECK-NEXT:                 <bitint::BUint<
// CHECK-NEXT:                     156,
// CHECK-NEXT:                     3,
// CHECK-NEXT:                     24,
// CHECK-NEXT:                     bitint::A8,
// CHECK-NEXT:                 >>::from_bint(
// CHECK-NEXT:                     <bitint::BInt<156, 3, 24, bitint::A8>>::from_i128(7 as i128),
// CHECK-NEXT:                 ),
// CHECK-NEXT:             );
// CHECK-NEXT:         s.0[i as usize].__slate_bits_0.__set_a([[T_2]]);
// CHECK-NEXT:         let [[T_3]] = <bitint::BInt<
// CHECK-NEXT:             156,
// CHECK-NEXT:             3,
// CHECK-NEXT:             24,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_i128((if i & 1 != 0 { -(1 as i32) } else { 1 as i32 }) as i128);
// CHECK-NEXT:         s.0[i as usize].__slate_bits_0.__set_b([[T_3]]);
// CHECK-NEXT:         let [[T_4]] = <bitint::BInt<156, 3, 24, bitint::A8>>::from_i128(-1 as i128);
// CHECK-NEXT:         t.0[i as usize].__slate_bits_0.__set_a([[T_4]]);
// CHECK-NEXT:         let [[T_5:__t[0-9]+]] = <bitint::BUint<
// CHECK-NEXT:             156,
// CHECK-NEXT:             3,
// CHECK-NEXT:             24,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint(<bitint::BInt<156, 3, 24, bitint::A8>>::from_i128(1 as i128))
// CHECK-NEXT:             .shl_bits(130 as usize)
// CHECK-NEXT:             .wrapping_add(
// CHECK-NEXT:                 <bitint::BUint<
// CHECK-NEXT:                     156,
// CHECK-NEXT:                     3,
// CHECK-NEXT:                     24,
// CHECK-NEXT:                     bitint::A8,
// CHECK-NEXT:                 >>::from_bint(
// CHECK-NEXT:                     <bitint::BInt<156, 3, 24, bitint::A8>>::from_i128(i as i128),
// CHECK-NEXT:                 ),
// CHECK-NEXT:             );
// CHECK-NEXT:         t.0[i as usize].__slate_bits_0.__set_b([[T_5]]);
// CHECK-NEXT:         let [[T_6:__t[0-9]+]] = <bitint::BInt<156, 3, 24, bitint::A8>>::from_i128(1 as i128);
// CHECK-NEXT:         t.0[i as usize].__slate_bits_0.__set_c([[T_6]]);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut ret: bitint::BInt<156, 3, 24, bitint::A8> = combine(
// CHECK-NEXT:         s.0.as_mut_ptr() as *mut S156,
// CHECK-NEXT:         t.0.as_mut_ptr() as *mut T156,
// CHECK-NEXT:         r.0.as_mut_ptr() as *mut T156,
// CHECK-NEXT:         5 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{}", ret.to_i128() as i32);
// CHECK-NEXT:     for i in 0 as i32..5 as i32 {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"%d %d %d %llu\n".as_ptr(),
// CHECK-NEXT:                 r.0[i as usize].__slate_bits_0.__get_a().to_i128() as i32,
// CHECK-NEXT:                 r.0[i as usize].__slate_bits_0.__get_c().to_i128() as i32,
// CHECK-NEXT:                 (r.0[i as usize].__slate_bits_0.__get_b().to_u128() as u32) as i32,
// CHECK-NEXT:                 r.0[i as usize].__slate_bits_0.__get_b().shr_bits(120 as usize).to_u128()
// CHECK-NEXT:                     as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut m: Mixed = {
// CHECK-NEXT:         let mut [[T_7:__t[0-9]+]]: Mixed = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_7]].__slate_bits_0.__set_tag(0 as i32);
// CHECK-NEXT:         [[T_7]]
// CHECK-NEXT:     };
// CHECK-NEXT:     m.__slate_bits_0.__set_tag(-(9 as i32));
// CHECK-NEXT:     let [[T_8:__t[0-9]+]] = <bitint::BInt<
// CHECK-NEXT:         100,
// CHECK-NEXT:         2,
// CHECK-NEXT:         16,
// CHECK-NEXT:         bitint::A8,
// CHECK-NEXT:     >>::from_i128(-123456789012345 as i128);
// CHECK-NEXT:     m.__slate_bits_0.__set_mid([[T_8]]);
// CHECK-NEXT:     m.__slate_bits_0.__set_flag(1 as u8);
// CHECK-NEXT:     let [[T_9:__t[0-9]+]] = <bitint::BUint<
// CHECK-NEXT:         200,
// CHECK-NEXT:         4,
// CHECK-NEXT:         32,
// CHECK-NEXT:         bitint::A8,
// CHECK-NEXT:     >>::from_u128(1234567890123456789 as u128);
// CHECK-NEXT:     m.__slate_bits_0.__set_top([[T_9]]);
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: bitint::BUint<200, 4, 32, bitint::A8> = m.__slate_bits_0.__get_top();
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: bitint::BUint<200, 4, 32, bitint::A8> = [[V_0]].shl_bits(60 as usize);
// CHECK-NEXT:     m.__slate_bits_0.__set_top([[V_1]]);
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: bitint::BUint<200, 4, 32, bitint::A8> = m.__slate_bits_0.__get_top();
// CHECK-NEXT:     let [[V_3:__v[0-9]+]]: bitint::BUint<200, 4, 32, bitint::A8> = [[V_2]]
// CHECK-NEXT:         .wrapping_add(
// CHECK-NEXT:             <bitint::BUint<
// CHECK-NEXT:                 200,
// CHECK-NEXT:                 4,
// CHECK-NEXT:                 32,
// CHECK-NEXT:                 bitint::A8,
// CHECK-NEXT:             >>::from_bint(
// CHECK-NEXT:                 <bitint::BInt<200, 4, 32, bitint::A8>>::from_i128(12345 as i128),
// CHECK-NEXT:             ),
// CHECK-NEXT:         );
// CHECK-NEXT:     m.__slate_bits_0.__set_top([[V_3]]);
// CHECK-NEXT:     let [[T_10:__t[0-9]+]] = (-50 as i8) << 1 >> 1;
// CHECK-NEXT:     m.__slate_bits_1.__set_small([[T_10]]);
// CHECK-NEXT:     dump(unsafe { &mut (*std::ptr::addr_of_mut!(m)) });
// CHECK-NEXT:     let mut copy: Mixed = m;
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: bitint::BInt<100, 2, 16, bitint::A8> = copy
// CHECK-NEXT:         .__slate_bits_0
// CHECK-NEXT:         .__get_mid();
// CHECK-NEXT:     let [[V_5:__v[0-9]+]]: bitint::BInt<100, 2, 16, bitint::A8> = [[V_4]]
// CHECK-NEXT:         .wrapping_add(<bitint::BInt<100, 2, 16, bitint::A8>>::from_i128(1 as i128));
// CHECK-NEXT:     copy.__slate_bits_0.__set_mid([[V_5]]);
// CHECK-NEXT:     let mut [[V_6:__v[0-9]+]]: bitint::BUint<200, 4, 32, bitint::A8> = copy
// CHECK-NEXT:         .__slate_bits_0
// CHECK-NEXT:         .__get_top();
// CHECK-NEXT:     let [[V_7:__v[0-9]+]]: bitint::BUint<200, 4, 32, bitint::A8> = [[V_6]]
// CHECK-NEXT:         .wrapping_sub(
// CHECK-NEXT:             <bitint::BUint<
// CHECK-NEXT:                 200,
// CHECK-NEXT:                 4,
// CHECK-NEXT:                 32,
// CHECK-NEXT:                 bitint::A8,
// CHECK-NEXT:             >>::from_bint(<bitint::BInt<200, 4, 32, bitint::A8>>::from_i128(5 as i128)),
// CHECK-NEXT:         );
// CHECK-NEXT:     copy.__slate_bits_0.__set_top([[V_7]]);
// CHECK-NEXT:     let [[T_11:__t[0-9]+]] = (63 as i8) << 1 >> 1;
// CHECK-NEXT:     copy.__slate_bits_1.__set_small([[T_11]]);
// CHECK-NEXT:     dump(unsafe { &mut (*std::ptr::addr_of_mut!(copy)) });
// CHECK-NEXT:     dump(unsafe { &mut (*std::ptr::addr_of_mut!(m)) });
// CHECK-NEXT:     let mut n: Narrow = {
// CHECK-NEXT:         let mut [[T_12:__t[0-9]+]]: Narrow = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_12]]
// CHECK-NEXT:             .__slate_bits_0
// CHECK-NEXT:             .__set_x(<bitint::BInt<100, 2, 16, bitint::A8>>::from_i128(-4 as i128));
// CHECK-NEXT:         [[T_12]]
// CHECK-NEXT:             .__slate_bits_0
// CHECK-NEXT:             .__set_y(
// CHECK-NEXT:                 <bitint::BUint<
// CHECK-NEXT:                     100,
// CHECK-NEXT:                     2,
// CHECK-NEXT:                     16,
// CHECK-NEXT:                     bitint::A8,
// CHECK-NEXT:                 >>::from_u128(18364758544493064720 as u128),
// CHECK-NEXT:             );
// CHECK-NEXT:         [[T_12]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %llx\n".as_ptr(),
// CHECK-NEXT:             n.__slate_bits_0.__get_x().to_i128() as i32,
// CHECK-NEXT:             n.__slate_bits_0.__get_y().to_u128() as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let [[T_13:__t[0-9]+]] = <bitint::BInt<100, 2, 16, bitint::A8>>::from_i128(3 as i128);
// CHECK-NEXT:     n.__slate_bits_0.__set_x([[T_13]]);
// CHECK-NEXT:     let mut [[V_8:__v[0-9]+]]: bitint::BUint<100, 2, 16, bitint::A8> = n.__slate_bits_0.__get_y();
// CHECK-NEXT:     let [[V_9:__v[0-9]+]]: bitint::BUint<100, 2, 16, bitint::A8> = [[V_8]]
// CHECK-NEXT:         .wrapping_add(
// CHECK-NEXT:             <bitint::BUint<
// CHECK-NEXT:                 100,
// CHECK-NEXT:                 2,
// CHECK-NEXT:                 16,
// CHECK-NEXT:                 bitint::A8,
// CHECK-NEXT:             >>::from_bint(<bitint::BInt<100, 2, 16, bitint::A8>>::from_i128(1 as i128)),
// CHECK-NEXT:         );
// CHECK-NEXT:     n.__slate_bits_0.__set_y([[V_9]]);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %llx %zu %zu\n".as_ptr(),
// CHECK-NEXT:             n.__slate_bits_0.__get_x().to_i128() as i32,
// CHECK-NEXT:             n.__slate_bits_0.__get_y().to_u128() as u64,
// CHECK-NEXT:             40 as u64,
// CHECK-NEXT:             24 as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
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
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
