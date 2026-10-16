typedef __INT_LEAST8_TYPE__   int8_t;
typedef __UINT_LEAST32_TYPE__ uint32_t;
typedef int                   ssize_t;
typedef struct {
  int8_t v1;
  int8_t v2;
  int8_t v3;
  int8_t v4;
} neon_s8;

uint32_t helper_neon_rshl_s8(uint32_t arg1, uint32_t arg2);

uint32_t helper_neon_rshl_s8(uint32_t arg1, uint32_t arg2) {
  uint32_t res;
  neon_s8  vsrc1;
  neon_s8  vsrc2;
  neon_s8  vdest;
  do {
    union {
      neon_s8  v;
      uint32_t i;
    } conv_u;
    conv_u.i = (arg1);
    vsrc1    = conv_u.v;
  } while (0);
  do {
    union {
      neon_s8  v;
      uint32_t i;
    } conv_u;
    conv_u.i = (arg2);
    vsrc2    = conv_u.v;
  } while (0);
  do {
    int8_t tmp;
    tmp = (int8_t)vsrc2.v1;
    if (tmp >= (ssize_t)sizeof(vsrc1.v1) * 8) {
      vdest.v1 = 0;
    } else if (tmp < -(ssize_t)sizeof(vsrc1.v1) * 8) {
      vdest.v1 = vsrc1.v1 >> (sizeof(vsrc1.v1) * 8 - 1);
    } else if (tmp == -(ssize_t)sizeof(vsrc1.v1) * 8) {
      vdest.v1 = vsrc1.v1 >> (tmp - 1);
      vdest.v1++;
      vdest.v1 >>= 1;
    } else if (tmp < 0) {
      vdest.v1 = (vsrc1.v1 + (1 << (-1 - tmp))) >> -tmp;
    } else {
      vdest.v1 = vsrc1.v1 << tmp;
    }
  } while (0);
  do {
    int8_t tmp;
    tmp = (int8_t)vsrc2.v2;
    if (tmp >= (ssize_t)sizeof(vsrc1.v2) * 8) {
      vdest.v2 = 0;
    } else if (tmp < -(ssize_t)sizeof(vsrc1.v2) * 8) {
      vdest.v2 = vsrc1.v2 >> (sizeof(vsrc1.v2) * 8 - 1);
    } else if (tmp == -(ssize_t)sizeof(vsrc1.v2) * 8) {
      vdest.v2 = vsrc1.v2 >> (tmp - 1);
      vdest.v2++;
      vdest.v2 >>= 1;
    } else if (tmp < 0) {
      vdest.v2 = (vsrc1.v2 + (1 << (-1 - tmp))) >> -tmp;
    } else {
      vdest.v2 = vsrc1.v2 << tmp;
    }
  } while (0);
  do {
    int8_t tmp;
    tmp = (int8_t)vsrc2.v3;
    if (tmp >= (ssize_t)sizeof(vsrc1.v3) * 8) {
      vdest.v3 = 0;
    } else if (tmp < -(ssize_t)sizeof(vsrc1.v3) * 8) {
      vdest.v3 = vsrc1.v3 >> (sizeof(vsrc1.v3) * 8 - 1);
    } else if (tmp == -(ssize_t)sizeof(vsrc1.v3) * 8) {
      vdest.v3 = vsrc1.v3 >> (tmp - 1);
      vdest.v3++;
      vdest.v3 >>= 1;
    } else if (tmp < 0) {
      vdest.v3 = (vsrc1.v3 + (1 << (-1 - tmp))) >> -tmp;
    } else {
      vdest.v3 = vsrc1.v3 << tmp;
    }
  } while (0);
  do {
    int8_t tmp;
    tmp = (int8_t)vsrc2.v4;
    if (tmp >= (ssize_t)sizeof(vsrc1.v4) * 8) {
      vdest.v4 = 0;
    } else if (tmp < -(ssize_t)sizeof(vsrc1.v4) * 8) {
      vdest.v4 = vsrc1.v4 >> (sizeof(vsrc1.v4) * 8 - 1);
    } else if (tmp == -(ssize_t)sizeof(vsrc1.v4) * 8) {
      vdest.v4 = vsrc1.v4 >> (tmp - 1);
      vdest.v4++;
      vdest.v4 >>= 1;
    } else if (tmp < 0) {
      vdest.v4 = (vsrc1.v4 + (1 << (-1 - tmp))) >> -tmp;
    } else {
      vdest.v4 = vsrc1.v4 << tmp;
    }
  } while (0);
  ;
  do {
    union {
      neon_s8  v;
      uint32_t i;
    } conv_u;
    conv_u.v = (vdest);
    res      = conv_u.i;
  } while (0);
  return res;
}

extern void abort(void);

int main() {
  uint32_t r = helper_neon_rshl_s8(0x05050505, 0x01010101);
  if (r != 0x0a0a0a0a)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     v1: i8,
// CHECK-NEXT:     v2: i8,
// CHECK-NEXT:     v3: i8,
// CHECK-NEXT:     v4: i8,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union [[SLATERECORD_1:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     v: [[SLATERECORD_0]],
// CHECK-NEXT:     i: u32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union [[SLATERECORD_2:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     v: [[SLATERECORD_0]],
// CHECK-NEXT:     i: u32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union [[SLATERECORD_3:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     v: [[SLATERECORD_0]],
// CHECK-NEXT:     i: u32,
// CHECK-NEXT: }
// CHECK-NEXT: fn helper_neon_rshl_s8(mut arg1: u32, mut arg2: u32) -> u32 {
// CHECK-NEXT:     let mut res: u32 = 0 as u32;
// CHECK-NEXT:     let mut vsrc1: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut vsrc2: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut vdest: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut conv_u: [[SLATERECORD_1]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             conv_u.i = arg1;
// CHECK-NEXT:         }
// CHECK-NEXT:         vsrc1 = unsafe { conv_u.v };
// CHECK-NEXT:         if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut conv_u: [[SLATERECORD_2]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             conv_u.i = arg2;
// CHECK-NEXT:         }
// CHECK-NEXT:         vsrc2 = unsafe { conv_u.v };
// CHECK-NEXT:         if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut tmp: i8 = vsrc2.v1;
// CHECK-NEXT:         if (tmp as i32) >= (((1 as u64) as u32) as i32) * (8 as i32) {
// CHECK-NEXT:             vdest.v1 = (0 as i32) as i8;
// CHECK-NEXT:         } else {
// CHECK-NEXT:             if (tmp as i32) < -(((1 as u64) as u32) as i32) * (8 as i32) {
// CHECK-NEXT:                 vdest.v1 = ((vsrc1.v1 as i32)
// CHECK-NEXT:                     >> (1 as u64)
// CHECK-NEXT:                         .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                         .wrapping_sub(((1 as i32) as i64) as u64)) as i8;
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 if (tmp as i32) == -(((1 as u64) as u32) as i32) * (8 as i32) {
// CHECK-NEXT:                     vdest.v1 = ((vsrc1.v1 as i32) >> (tmp as i32) - (1 as i32)) as i8;
// CHECK-NEXT:                     let mut [[V_0:__v[0-9]+]]: i8 = vdest.v1;
// CHECK-NEXT:                     let mut [[V_1:__v[0-9]+]]: i8 = (([[V_0]] as i32) + (1 as i32)) as i8;
// CHECK-NEXT:                     vdest.v1 = [[V_1]];
// CHECK-NEXT:                     let mut [[V_2:__v[0-9]+]]: i8 = vdest.v1;
// CHECK-NEXT:                     let mut [[V_3:__v[0-9]+]]: i8 = (([[V_2]] as i32) >> (1 as i32)) as i8;
// CHECK-NEXT:                     vdest.v1 = [[V_3]];
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     if (tmp as i32) < (0 as i32) {
// CHECK-NEXT:                         vdest.v1 = ((vsrc1.v1 as i32)
// CHECK-NEXT:                             + ((1 as i32) << -(1 as i32) - (tmp as i32))
// CHECK-NEXT:                             >> -(tmp as i32)) as i8;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         vdest.v1 = ((vsrc1.v1 as i32) << (tmp as i32)) as i8;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_3:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut tmp: i8 = vsrc2.v2;
// CHECK-NEXT:         if (tmp as i32) >= (((1 as u64) as u32) as i32) * (8 as i32) {
// CHECK-NEXT:             vdest.v2 = (0 as i32) as i8;
// CHECK-NEXT:         } else {
// CHECK-NEXT:             if (tmp as i32) < -(((1 as u64) as u32) as i32) * (8 as i32) {
// CHECK-NEXT:                 vdest.v2 = ((vsrc1.v2 as i32)
// CHECK-NEXT:                     >> (1 as u64)
// CHECK-NEXT:                         .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                         .wrapping_sub(((1 as i32) as i64) as u64)) as i8;
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 if (tmp as i32) == -(((1 as u64) as u32) as i32) * (8 as i32) {
// CHECK-NEXT:                     vdest.v2 = ((vsrc1.v2 as i32) >> (tmp as i32) - (1 as i32)) as i8;
// CHECK-NEXT:                     let mut [[V_4:__v[0-9]+]]: i8 = vdest.v2;
// CHECK-NEXT:                     let mut [[V_5:__v[0-9]+]]: i8 = (([[V_4]] as i32) + (1 as i32)) as i8;
// CHECK-NEXT:                     vdest.v2 = [[V_5]];
// CHECK-NEXT:                     let mut [[V_6:__v[0-9]+]]: i8 = vdest.v2;
// CHECK-NEXT:                     let mut [[V_7:__v[0-9]+]]: i8 = (([[V_6]] as i32) >> (1 as i32)) as i8;
// CHECK-NEXT:                     vdest.v2 = [[V_7]];
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     if (tmp as i32) < (0 as i32) {
// CHECK-NEXT:                         vdest.v2 = ((vsrc1.v2 as i32)
// CHECK-NEXT:                             + ((1 as i32) << -(1 as i32) - (tmp as i32))
// CHECK-NEXT:                             >> -(tmp as i32)) as i8;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         vdest.v2 = ((vsrc1.v2 as i32) << (tmp as i32)) as i8;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_4:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut tmp: i8 = vsrc2.v3;
// CHECK-NEXT:         if (tmp as i32) >= (((1 as u64) as u32) as i32) * (8 as i32) {
// CHECK-NEXT:             vdest.v3 = (0 as i32) as i8;
// CHECK-NEXT:         } else {
// CHECK-NEXT:             if (tmp as i32) < -(((1 as u64) as u32) as i32) * (8 as i32) {
// CHECK-NEXT:                 vdest.v3 = ((vsrc1.v3 as i32)
// CHECK-NEXT:                     >> (1 as u64)
// CHECK-NEXT:                         .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                         .wrapping_sub(((1 as i32) as i64) as u64)) as i8;
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 if (tmp as i32) == -(((1 as u64) as u32) as i32) * (8 as i32) {
// CHECK-NEXT:                     vdest.v3 = ((vsrc1.v3 as i32) >> (tmp as i32) - (1 as i32)) as i8;
// CHECK-NEXT:                     let mut [[V_8:__v[0-9]+]]: i8 = vdest.v3;
// CHECK-NEXT:                     let mut [[V_9:__v[0-9]+]]: i8 = (([[V_8]] as i32) + (1 as i32)) as i8;
// CHECK-NEXT:                     vdest.v3 = [[V_9]];
// CHECK-NEXT:                     let mut [[V_10:__v[0-9]+]]: i8 = vdest.v3;
// CHECK-NEXT:                     let mut [[V_11:__v[0-9]+]]: i8 = (([[V_10]] as i32) >> (1 as i32)) as i8;
// CHECK-NEXT:                     vdest.v3 = [[V_11]];
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     if (tmp as i32) < (0 as i32) {
// CHECK-NEXT:                         vdest.v3 = ((vsrc1.v3 as i32)
// CHECK-NEXT:                             + ((1 as i32) << -(1 as i32) - (tmp as i32))
// CHECK-NEXT:                             >> -(tmp as i32)) as i8;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         vdest.v3 = ((vsrc1.v3 as i32) << (tmp as i32)) as i8;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_5:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut tmp: i8 = vsrc2.v4;
// CHECK-NEXT:         if (tmp as i32) >= (((1 as u64) as u32) as i32) * (8 as i32) {
// CHECK-NEXT:             vdest.v4 = (0 as i32) as i8;
// CHECK-NEXT:         } else {
// CHECK-NEXT:             if (tmp as i32) < -(((1 as u64) as u32) as i32) * (8 as i32) {
// CHECK-NEXT:                 vdest.v4 = ((vsrc1.v4 as i32)
// CHECK-NEXT:                     >> (1 as u64)
// CHECK-NEXT:                         .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                         .wrapping_sub(((1 as i32) as i64) as u64)) as i8;
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 if (tmp as i32) == -(((1 as u64) as u32) as i32) * (8 as i32) {
// CHECK-NEXT:                     vdest.v4 = ((vsrc1.v4 as i32) >> (tmp as i32) - (1 as i32)) as i8;
// CHECK-NEXT:                     let mut [[V_12:__v[0-9]+]]: i8 = vdest.v4;
// CHECK-NEXT:                     let mut [[V_13:__v[0-9]+]]: i8 = (([[V_12]] as i32) + (1 as i32)) as i8;
// CHECK-NEXT:                     vdest.v4 = [[V_13]];
// CHECK-NEXT:                     let mut [[V_14:__v[0-9]+]]: i8 = vdest.v4;
// CHECK-NEXT:                     let mut [[V_15:__v[0-9]+]]: i8 = (([[V_14]] as i32) >> (1 as i32)) as i8;
// CHECK-NEXT:                     vdest.v4 = [[V_15]];
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     if (tmp as i32) < (0 as i32) {
// CHECK-NEXT:                         vdest.v4 = ((vsrc1.v4 as i32)
// CHECK-NEXT:                             + ((1 as i32) << -(1 as i32) - (tmp as i32))
// CHECK-NEXT:                             >> -(tmp as i32)) as i8;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         vdest.v4 = ((vsrc1.v4 as i32) << (tmp as i32)) as i8;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     '[[SLATE_BREAK_6:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut conv_u: [[SLATERECORD_3]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             conv_u.v = vdest;
// CHECK-NEXT:         }
// CHECK-NEXT:         res = unsafe { conv_u.i };
// CHECK-NEXT:         if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     res
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut r: u32 = helper_neon_rshl_s8(
// CHECK-NEXT:         (84215045 as i32) as u32,
// CHECK-NEXT:         (16843009 as i32) as u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     if r != ((168430090 as i32) as u32) {
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
