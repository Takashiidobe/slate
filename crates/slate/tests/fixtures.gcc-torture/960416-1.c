void abort(void);
void exit(int);

typedef unsigned long int  st;
typedef unsigned long long dt;
typedef union {
  dt d;
  struct {
    st h, l;
  } s;
} t_be;

typedef union {
  dt d;
  struct {
    st l, h;
  } s;
} t_le;

#define df(f, t)                                                               \
  int f(t afh, t bfh) {                                                        \
    t   hh;                                                                    \
    t   hp, lp, dp, m;                                                         \
    st  ad, bd;                                                                \
    int s;                                                                     \
    s  = 0;                                                                    \
    ad = afh.s.h - afh.s.l;                                                    \
    bd = bfh.s.l - bfh.s.h;                                                    \
    if (bd > bfh.s.l) {                                                        \
      bd = -bd;                                                                \
      s  = ~s;                                                                 \
    }                                                                          \
    lp.d  = (dt)afh.s.l * bfh.s.l;                                             \
    hp.d  = (dt)afh.s.h * bfh.s.h;                                             \
    dp.d  = (dt)ad * bd;                                                       \
    dp.d ^= s;                                                                 \
    hh.d  = hp.d + hp.s.h + lp.s.h + dp.s.h;                                   \
    m.d   = (dt)lp.s.h + hp.s.l + lp.s.l + dp.s.l;                             \
    return hh.s.l + m.s.l;                                                     \
  }

df(f_le, t_le) df(f_be, t_be)

    int main(void) {
  t_be x;
  x.s.h = 0x10000000U;
  x.s.l = 0xe0000000U;
  if (x.d == 0x10000000e0000000ULL &&
      f_be((t_be)0x100000000ULL, (t_be)0x100000000ULL) != -1)
    abort();
  if (x.d == 0xe000000010000000ULL &&
      f_le((t_le)0x100000000ULL, (t_le)0x100000000ULL) != -1)
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
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     d: u64,
// CHECK-NEXT:     s: [[SLATERECORD_1:__SlateRecord[0-9]+]],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_1]] {
// CHECK-NEXT:     h: u64,
// CHECK-NEXT:     l: u64,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union [[SLATERECORD_2:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     d: u64,
// CHECK-NEXT:     s: [[SLATERECORD_3:__SlateRecord[0-9]+]],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_3]] {
// CHECK-NEXT:     l: u64,
// CHECK-NEXT:     h: u64,
// CHECK-NEXT: }
// CHECK-NEXT: fn f_le(mut afh: [[SLATERECORD_2]], mut bfh: [[SLATERECORD_2]]) -> i32 {
// CHECK-NEXT:     let mut hh: [[SLATERECORD_2]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut hp: [[SLATERECORD_2]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut lp: [[SLATERECORD_2]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut dp: [[SLATERECORD_2]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut m: [[SLATERECORD_2]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut ad: u64 = 0 as u64;
// CHECK-NEXT:     let mut bd: u64 = 0 as u64;
// CHECK-NEXT:     let mut s: i32 = 0 as i32;
// CHECK-NEXT:     ad = unsafe { afh.s.h }.wrapping_sub(unsafe { afh.s.l });
// CHECK-NEXT:     bd = unsafe { bfh.s.l }.wrapping_sub(unsafe { bfh.s.h });
// CHECK-NEXT:     if bd > unsafe { bfh.s.l } {
// CHECK-NEXT:         bd = bd.wrapping_neg();
// CHECK-NEXT:         s = !s;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         lp.d = unsafe { afh.s.l }.wrapping_mul(unsafe { bfh.s.l });
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         hp.d = unsafe { afh.s.h }.wrapping_mul(unsafe { bfh.s.h });
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         dp.d = ad.wrapping_mul(bd);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: u64 = unsafe { dp.d };
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: u64 = [[V_1]] ^ ((s as i64) as u64);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         dp.d = [[V_2]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         hh.d = unsafe { hp.d }
// CHECK-NEXT:             .wrapping_add(unsafe { hp.s.h })
// CHECK-NEXT:             .wrapping_add(unsafe { lp.s.h })
// CHECK-NEXT:             .wrapping_add(unsafe { dp.s.h });
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         m.d = unsafe { lp.s.h }
// CHECK-NEXT:             .wrapping_add(unsafe { hp.s.l })
// CHECK-NEXT:             .wrapping_add(unsafe { lp.s.l })
// CHECK-NEXT:             .wrapping_add(unsafe { dp.s.l });
// CHECK-NEXT:     }
// CHECK-NEXT:     (unsafe { hh.s.l }.wrapping_add(unsafe { m.s.l }) as u32) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn f_be(mut afh: [[SLATERECORD_0]], mut bfh: [[SLATERECORD_0]]) -> i32 {
// CHECK-NEXT:     let mut hh: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut hp: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut lp: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut dp: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut m: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut ad: u64 = 0 as u64;
// CHECK-NEXT:     let mut bd: u64 = 0 as u64;
// CHECK-NEXT:     let mut s: i32 = 0 as i32;
// CHECK-NEXT:     ad = unsafe { afh.s.h }.wrapping_sub(unsafe { afh.s.l });
// CHECK-NEXT:     bd = unsafe { bfh.s.l }.wrapping_sub(unsafe { bfh.s.h });
// CHECK-NEXT:     if bd > unsafe { bfh.s.l } {
// CHECK-NEXT:         bd = bd.wrapping_neg();
// CHECK-NEXT:         s = !s;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         lp.d = unsafe { afh.s.l }.wrapping_mul(unsafe { bfh.s.l });
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         hp.d = unsafe { afh.s.h }.wrapping_mul(unsafe { bfh.s.h });
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         dp.d = ad.wrapping_mul(bd);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: u64 = unsafe { dp.d };
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: u64 = [[V_3]] ^ ((s as i64) as u64);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         dp.d = [[V_4]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         hh.d = unsafe { hp.d }
// CHECK-NEXT:             .wrapping_add(unsafe { hp.s.h })
// CHECK-NEXT:             .wrapping_add(unsafe { lp.s.h })
// CHECK-NEXT:             .wrapping_add(unsafe { dp.s.h });
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         m.d = unsafe { lp.s.h }
// CHECK-NEXT:             .wrapping_add(unsafe { hp.s.l })
// CHECK-NEXT:             .wrapping_add(unsafe { lp.s.l })
// CHECK-NEXT:             .wrapping_add(unsafe { dp.s.l });
// CHECK-NEXT:     }
// CHECK-NEXT:     (unsafe { hh.s.l }.wrapping_add(unsafe { m.s.l }) as u32) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         x.s.h = (268435456 as u32) as u64;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         x.s.l = (3758096384 as u32) as u64;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_5:__v[0-9]+]]: bool;
// CHECK-NEXT:     if (unsafe { x.d }) == (1152921508364943360 as u64) {
// CHECK-NEXT:         [[V_5]] = f_be(
// CHECK-NEXT:             {
// CHECK-NEXT:                 let mut [[T_0:__t[0-9]+]]: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:                 [[T_0]].d = 4294967296 as u64;
// CHECK-NEXT:                 [[T_0]]
// CHECK-NEXT:             },
// CHECK-NEXT:             {
// CHECK-NEXT:                 let mut [[T_1:__t[0-9]+]]: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:                 [[T_1]].d = 4294967296 as u64;
// CHECK-NEXT:                 [[T_1]]
// CHECK-NEXT:             },
// CHECK-NEXT:         ) != -(1 as i32);
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_5]] = false as bool;
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_5]] {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_6:__v[0-9]+]]: bool;
// CHECK-NEXT:     if (unsafe { x.d }) == (16140901064764293120 as u64) {
// CHECK-NEXT:         [[V_6]] = f_le(
// CHECK-NEXT:             {
// CHECK-NEXT:                 let mut [[T_2:__t[0-9]+]]: [[SLATERECORD_2]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:                 [[T_2]].d = 4294967296 as u64;
// CHECK-NEXT:                 [[T_2]]
// CHECK-NEXT:             },
// CHECK-NEXT:             {
// CHECK-NEXT:                 let mut [[T_3:__t[0-9]+]]: [[SLATERECORD_2]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:                 [[T_3]].d = 4294967296 as u64;
// CHECK-NEXT:                 [[T_3]]
// CHECK-NEXT:             },
// CHECK-NEXT:         ) != -(1 as i32);
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_6]] = false as bool;
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_6]] {
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
