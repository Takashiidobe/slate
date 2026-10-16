void abort(void);
void exit(int);

long simple_rand() {
  static unsigned long seed = 47114711;
  unsigned long this        = seed * 1103515245 + 12345;
  seed                      = this;
  return this >> 8;
}

unsigned long int random_bitstring() {
  unsigned long int x;
  int               n_bits;
  long              ran;
  int               tot_bits = 0;

  x = 0;
  for (;;) {
    ran       = simple_rand();
    n_bits    = (ran >> 1) % 16;
    tot_bits += n_bits;

    if (n_bits == 0)
      return x;
    else {
      x <<= n_bits;
      if (ran & 1)
        x |= (1 << n_bits) - 1;

      if (tot_bits > 8 * sizeof(long) + 6)
        return x;
    }
  }
}

#define ABS(x) ((x) >= 0 ? (x) : -(x))

int main(void) {
  long int i;

  for (i = 0; i < 1000; i++) {
    unsigned long x, y;
    x = random_bitstring();
    y = random_bitstring();

    if (sizeof(int) == sizeof(long))
      goto save_time;

    {
      unsigned long xx = x, yy = y, r1, r2;
      if (yy == 0)
        continue;
      r1 = xx / yy;
      r2 = xx % yy;
      if (r2 >= yy || r1 * yy + r2 != xx)
        abort();
    }
    {
      signed long xx = x, yy = y, r1, r2;
      if ((unsigned long)xx << 1 == 0 && yy == -1)
        continue;
      r1 = xx / yy;
      r2 = xx % yy;
      if (ABS(r2) >= (unsigned long)ABS(yy) ||
          (signed long)(r1 * yy + r2) != xx)
        abort();
    }
  save_time: {
    unsigned int xx = x, yy = y, r1, r2;
    if (yy == 0)
      continue;
    r1 = xx / yy;
    r2 = xx % yy;
    if (r2 >= yy || r1 * yy + r2 != xx)
      abort();
  }
    {
      signed int xx = x, yy = y, r1, r2;
      if ((unsigned int)xx << 1 == 0 && yy == -1)
        continue;
      r1 = xx / yy;
      r2 = xx % yy;
      if (ABS(r2) >= (unsigned int)ABS(yy) || (signed int)(r1 * yy + r2) != xx)
        abort();
    }
    {
      unsigned short xx = x, yy = y, r1, r2;
      if (yy == 0)
        continue;
      r1 = xx / yy;
      r2 = xx % yy;
      if (r2 >= yy || r1 * yy + r2 != xx)
        abort();
    }
    {
      signed short xx = x, yy = y, r1, r2;
      r1 = xx / yy;
      r2 = xx % yy;
      if (ABS(r2) >= (unsigned short)ABS(yy) ||
          (signed short)(r1 * yy + r2) != xx)
        abort();
    }
    {
      unsigned char xx = x, yy = y, r1, r2;
      if (yy == 0)
        continue;
      r1 = xx / yy;
      r2 = xx % yy;
      if (r2 >= yy || r1 * yy + r2 != xx)
        abort();
    }
    {
      signed char xx = x, yy = y, r1, r2;
      r1 = xx / yy;
      r2 = xx % yy;
      if (ABS(r2) >= (unsigned char)ABS(yy) ||
          (signed char)(r1 * yy + r2) != xx)
        abort();
    }
  }

  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: fn simple_rand() -> i64 {
// CHECK-NEXT:     let mut this: u64 = unsafe { seed }
// CHECK-NEXT:         .wrapping_mul(((1103515245 as i32) as i64) as u64)
// CHECK-NEXT:         .wrapping_add(((12345 as i32) as i64) as u64);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         seed = this;
// CHECK-NEXT:     }
// CHECK-NEXT:     (this >> (8 as i32)) as i64
// CHECK-NEXT: }
// CHECK-NEXT: static mut seed: u64 = ((47114711 as i32) as i64) as u64;
// CHECK-NEXT: fn random_bitstring() -> u64 {
// CHECK-NEXT:     let mut x: u64 = ((0 as i32) as i64) as u64;
// CHECK-NEXT:     let mut n_bits: i32 = 0 as i32;
// CHECK-NEXT:     let mut ran: i64 = 0 as i64;
// CHECK-NEXT:     let mut tot_bits: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         ran = simple_rand();
// CHECK-NEXT:         n_bits = ((ran >> (1 as i32)) % ((16 as i32) as i64)) as i32;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = tot_bits;
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + n_bits;
// CHECK-NEXT:         tot_bits = [[V_2]];
// CHECK-NEXT:         if n_bits == (0 as i32) {
// CHECK-NEXT:             return x;
// CHECK-NEXT:         } else {
// CHECK-NEXT:             let mut [[V_3:__v[0-9]+]]: u64 = x;
// CHECK-NEXT:             let mut [[V_4:__v[0-9]+]]: u64 = [[V_3]] << n_bits;
// CHECK-NEXT:             x = [[V_4]];
// CHECK-NEXT:             if ran & ((1 as i32) as i64) != (0 as i64) {
// CHECK-NEXT:                 let mut [[V_5:__v[0-9]+]]: u64 = x;
// CHECK-NEXT:                 let mut [[V_6:__v[0-9]+]]: u64 = [[V_5]]
// CHECK-NEXT:                     | (((((1 as i32) << n_bits) - (1 as i32)) as i64) as u64);
// CHECK-NEXT:                 x = [[V_6]];
// CHECK-NEXT:             }
// CHECK-NEXT:             if ((tot_bits as i64) as u64)
// CHECK-NEXT:                 > (((8 as i32) as i64) as u64)
// CHECK-NEXT:                     .wrapping_mul(8 as u64)
// CHECK-NEXT:                     .wrapping_add(((6 as i32) as i64) as u64)
// CHECK-NEXT:             {
// CHECK-NEXT:                 return x;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[SLATE_STORAGE_0:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_0:__slate_slot_[0-9]+]]: *mut i64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_0]])
// CHECK-NEXT:         as *mut i64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_1:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_1:__slate_slot_[0-9]+]]: *mut i64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_1]])
// CHECK-NEXT:         as *mut i64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_2:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i8> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_2:__slate_slot_[0-9]+]]: *mut i8 = std::ptr::addr_of_mut!([[SLATE_STORAGE_2]]) as *mut i8;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_3:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i8> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_3:__slate_slot_[0-9]+]]: *mut i8 = std::ptr::addr_of_mut!([[SLATE_STORAGE_3]]) as *mut i8;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_4:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i8> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_4:__slate_slot_[0-9]+]]: *mut i8 = std::ptr::addr_of_mut!([[SLATE_STORAGE_4]]) as *mut i8;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_5:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i8> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_5:__slate_slot_[0-9]+]]: *mut i8 = std::ptr::addr_of_mut!([[SLATE_STORAGE_5]]) as *mut i8;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_6:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u8> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_6:__slate_slot_[0-9]+]]: *mut u8 = std::ptr::addr_of_mut!([[SLATE_STORAGE_6]]) as *mut u8;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_7:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u8> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_7:__slate_slot_[0-9]+]]: *mut u8 = std::ptr::addr_of_mut!([[SLATE_STORAGE_7]]) as *mut u8;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_8:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u8> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_8:__slate_slot_[0-9]+]]: *mut u8 = std::ptr::addr_of_mut!([[SLATE_STORAGE_8]]) as *mut u8;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_9:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u8> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_9:__slate_slot_[0-9]+]]: *mut u8 = std::ptr::addr_of_mut!([[SLATE_STORAGE_9]]) as *mut u8;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_10:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i16> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_10:__slate_slot_[0-9]+]]: *mut i16 = std::ptr::addr_of_mut!([[SLATE_STORAGE_10]])
// CHECK-NEXT:         as *mut i16;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_11:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i16> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_11:__slate_slot_[0-9]+]]: *mut i16 = std::ptr::addr_of_mut!([[SLATE_STORAGE_11]])
// CHECK-NEXT:         as *mut i16;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_12:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i16> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_12:__slate_slot_[0-9]+]]: *mut i16 = std::ptr::addr_of_mut!([[SLATE_STORAGE_12]])
// CHECK-NEXT:         as *mut i16;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_13:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i16> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_13:__slate_slot_[0-9]+]]: *mut i16 = std::ptr::addr_of_mut!([[SLATE_STORAGE_13]])
// CHECK-NEXT:         as *mut i16;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_14:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u16> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_14:__slate_slot_[0-9]+]]: *mut u16 = std::ptr::addr_of_mut!([[SLATE_STORAGE_14]])
// CHECK-NEXT:         as *mut u16;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_15:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u16> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_15:__slate_slot_[0-9]+]]: *mut u16 = std::ptr::addr_of_mut!([[SLATE_STORAGE_15]])
// CHECK-NEXT:         as *mut u16;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_16:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u16> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_16:__slate_slot_[0-9]+]]: *mut u16 = std::ptr::addr_of_mut!([[SLATE_STORAGE_16]])
// CHECK-NEXT:         as *mut u16;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_17:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u16> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_17:__slate_slot_[0-9]+]]: *mut u16 = std::ptr::addr_of_mut!([[SLATE_STORAGE_17]])
// CHECK-NEXT:         as *mut u16;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_18:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_18:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_18]])
// CHECK-NEXT:         as *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_19:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_19:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_19]])
// CHECK-NEXT:         as *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_20:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_20:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_20]])
// CHECK-NEXT:         as *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_21:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_21:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_21]])
// CHECK-NEXT:         as *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_22:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_22:__slate_slot_[0-9]+]]: *mut u32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_22]])
// CHECK-NEXT:         as *mut u32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_23:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_23:__slate_slot_[0-9]+]]: *mut u32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_23]])
// CHECK-NEXT:         as *mut u32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_24:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_24:__slate_slot_[0-9]+]]: *mut u32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_24]])
// CHECK-NEXT:         as *mut u32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_25:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_25:__slate_slot_[0-9]+]]: *mut u32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_25]])
// CHECK-NEXT:         as *mut u32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_26:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_26:__slate_slot_[0-9]+]]: *mut i64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_26]])
// CHECK-NEXT:         as *mut i64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_27:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_27:__slate_slot_[0-9]+]]: *mut i64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_27]])
// CHECK-NEXT:         as *mut i64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_28:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_28:__slate_slot_[0-9]+]]: *mut i64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_28]])
// CHECK-NEXT:         as *mut i64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_29:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_29:__slate_slot_[0-9]+]]: *mut i64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_29]])
// CHECK-NEXT:         as *mut i64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_30:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_30:__slate_slot_[0-9]+]]: *mut u64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_30]])
// CHECK-NEXT:         as *mut u64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_31:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_31:__slate_slot_[0-9]+]]: *mut u64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_31]])
// CHECK-NEXT:         as *mut u64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_32:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_32:__slate_slot_[0-9]+]]: *mut u64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_32]])
// CHECK-NEXT:         as *mut u64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_33:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_33:__slate_slot_[0-9]+]]: *mut u64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_33]])
// CHECK-NEXT:         as *mut u64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_34:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_34:__slate_slot_[0-9]+]]: *mut u64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_34]])
// CHECK-NEXT:         as *mut u64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_35:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_35:__slate_slot_[0-9]+]]: *mut u64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_35]])
// CHECK-NEXT:         as *mut u64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_36:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_36:__slate_slot_[0-9]+]]: *mut i64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_36]])
// CHECK-NEXT:         as *mut i64;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[SLATE_SLOT_36]] = (0 as i32) as i64;
// CHECK-NEXT:         loop {
// CHECK-NEXT:             if *[[SLATE_SLOT_36]] < ((1000 as i32) as i64) {
// CHECK-NEXT:                 '[[JOIN_0:__join_[0-9]+]]: {
// CHECK-NEXT:                     *[[SLATE_SLOT_35]] = random_bitstring();
// CHECK-NEXT:                     *[[SLATE_SLOT_34]] = random_bitstring();
// CHECK-NEXT:                     if (4 as u64) == (8 as u64) {} else {
// CHECK-NEXT:                         std::ptr::write([[SLATE_SLOT_33]], *[[SLATE_SLOT_35]]);
// CHECK-NEXT:                         std::ptr::write([[SLATE_SLOT_32]], *[[SLATE_SLOT_34]]);
// CHECK-NEXT:                         if *[[SLATE_SLOT_32]] == (((0 as i32) as i64) as u64) {
// CHECK-NEXT:                             break '[[JOIN_0]];
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             *[[SLATE_SLOT_31]] = *[[SLATE_SLOT_33]] / *[[SLATE_SLOT_32]];
// CHECK-NEXT:                             *[[SLATE_SLOT_30]] = *[[SLATE_SLOT_33]] % *[[SLATE_SLOT_32]];
// CHECK-NEXT:                             if *[[SLATE_SLOT_30]] >= *[[SLATE_SLOT_32]]
// CHECK-NEXT:                                 || (*[[SLATE_SLOT_31]])
// CHECK-NEXT:                                     .wrapping_mul(*[[SLATE_SLOT_32]])
// CHECK-NEXT:                                     .wrapping_add(*[[SLATE_SLOT_30]]) != *[[SLATE_SLOT_33]]
// CHECK-NEXT:                             {
// CHECK-NEXT:                                 unsafe { abort() };
// CHECK-NEXT:                             }
// CHECK-NEXT:                             std::ptr::write([[SLATE_SLOT_29]], *[[SLATE_SLOT_35]] as i64);
// CHECK-NEXT:                             std::ptr::write([[SLATE_SLOT_28]], *[[SLATE_SLOT_34]] as i64);
// CHECK-NEXT:                             if (*[[SLATE_SLOT_29]] as u64) << (1 as i32)
// CHECK-NEXT:                                 == (((0 as i32) as i64) as u64)
// CHECK-NEXT:                                 && *[[SLATE_SLOT_28]] == (-(1 as i32) as i64)
// CHECK-NEXT:                             {
// CHECK-NEXT:                                 break '[[JOIN_0]];
// CHECK-NEXT:                             } else {
// CHECK-NEXT:                                 *[[SLATE_SLOT_27]] = *[[SLATE_SLOT_29]] / *[[SLATE_SLOT_28]];
// CHECK-NEXT:                                 *[[SLATE_SLOT_26]] = *[[SLATE_SLOT_29]] % *[[SLATE_SLOT_28]];
// CHECK-NEXT:                                 if ((if *[[SLATE_SLOT_26]] >= ((0 as i32) as i64) {
// CHECK-NEXT:                                     *[[SLATE_SLOT_26]]
// CHECK-NEXT:                                 } else {
// CHECK-NEXT:                                     -(*[[SLATE_SLOT_26]])
// CHECK-NEXT:                                 }) as u64)
// CHECK-NEXT:                                     >= ((if *[[SLATE_SLOT_28]] >= ((0 as i32) as i64) {
// CHECK-NEXT:                                         *[[SLATE_SLOT_28]]
// CHECK-NEXT:                                     } else {
// CHECK-NEXT:                                         -(*[[SLATE_SLOT_28]])
// CHECK-NEXT:                                     }) as u64)
// CHECK-NEXT:                                     || *[[SLATE_SLOT_27]] * *[[SLATE_SLOT_28]] + *[[SLATE_SLOT_26]]
// CHECK-NEXT:                                         != *[[SLATE_SLOT_29]]
// CHECK-NEXT:                                 {
// CHECK-NEXT:                                     unsafe { abort() };
// CHECK-NEXT:                                 }
// CHECK-NEXT:                             }
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                     std::ptr::write([[SLATE_SLOT_25]], *[[SLATE_SLOT_35]] as u32);
// CHECK-NEXT:                     std::ptr::write([[SLATE_SLOT_24]], *[[SLATE_SLOT_34]] as u32);
// CHECK-NEXT:                     if *[[SLATE_SLOT_24]] == ((0 as i32) as u32) {} else {
// CHECK-NEXT:                         *[[SLATE_SLOT_23]] = *[[SLATE_SLOT_25]] / *[[SLATE_SLOT_24]];
// CHECK-NEXT:                         *[[SLATE_SLOT_22]] = *[[SLATE_SLOT_25]] % *[[SLATE_SLOT_24]];
// CHECK-NEXT:                         if *[[SLATE_SLOT_22]] >= *[[SLATE_SLOT_24]]
// CHECK-NEXT:                             || (*[[SLATE_SLOT_23]])
// CHECK-NEXT:                                 .wrapping_mul(*[[SLATE_SLOT_24]])
// CHECK-NEXT:                                 .wrapping_add(*[[SLATE_SLOT_22]]) != *[[SLATE_SLOT_25]]
// CHECK-NEXT:                         {
// CHECK-NEXT:                             unsafe { abort() };
// CHECK-NEXT:                         }
// CHECK-NEXT:                         std::ptr::write(
// CHECK-NEXT:                             [[SLATE_SLOT_21]],
// CHECK-NEXT:                             (*[[SLATE_SLOT_35]] as u32) as i32,
// CHECK-NEXT:                         );
// CHECK-NEXT:                         std::ptr::write(
// CHECK-NEXT:                             [[SLATE_SLOT_20]],
// CHECK-NEXT:                             (*[[SLATE_SLOT_34]] as u32) as i32,
// CHECK-NEXT:                         );
// CHECK-NEXT:                         if (*[[SLATE_SLOT_21]] as u32) << (1 as i32) == ((0 as i32) as u32)
// CHECK-NEXT:                             && *[[SLATE_SLOT_20]] == -(1 as i32)
// CHECK-NEXT:                         {} else {
// CHECK-NEXT:                             *[[SLATE_SLOT_19]] = *[[SLATE_SLOT_21]] / *[[SLATE_SLOT_20]];
// CHECK-NEXT:                             *[[SLATE_SLOT_18]] = *[[SLATE_SLOT_21]] % *[[SLATE_SLOT_20]];
// CHECK-NEXT:                             if ((if *[[SLATE_SLOT_18]] >= (0 as i32) {
// CHECK-NEXT:                                 *[[SLATE_SLOT_18]]
// CHECK-NEXT:                             } else {
// CHECK-NEXT:                                 -(*[[SLATE_SLOT_18]])
// CHECK-NEXT:                             }) as u32)
// CHECK-NEXT:                                 >= ((if *[[SLATE_SLOT_20]] >= (0 as i32) {
// CHECK-NEXT:                                     *[[SLATE_SLOT_20]]
// CHECK-NEXT:                                 } else {
// CHECK-NEXT:                                     -(*[[SLATE_SLOT_20]])
// CHECK-NEXT:                                 }) as u32)
// CHECK-NEXT:                                 || *[[SLATE_SLOT_19]] * *[[SLATE_SLOT_20]] + *[[SLATE_SLOT_18]]
// CHECK-NEXT:                                     != *[[SLATE_SLOT_21]]
// CHECK-NEXT:                             {
// CHECK-NEXT:                                 unsafe { abort() };
// CHECK-NEXT:                             }
// CHECK-NEXT:                             std::ptr::write([[SLATE_SLOT_17]], *[[SLATE_SLOT_35]] as u16);
// CHECK-NEXT:                             std::ptr::write([[SLATE_SLOT_16]], *[[SLATE_SLOT_34]] as u16);
// CHECK-NEXT:                             if ((*[[SLATE_SLOT_16]] as u32) as i32) == (0 as i32) {} else {
// CHECK-NEXT:                                 *[[SLATE_SLOT_15]] = ((((*[[SLATE_SLOT_17]] as u32) as i32)
// CHECK-NEXT:                                     / ((*[[SLATE_SLOT_16]] as u32) as i32)) as i16) as u16;
// CHECK-NEXT:                                 *[[SLATE_SLOT_14]] = ((((*[[SLATE_SLOT_17]] as u32) as i32)
// CHECK-NEXT:                                     % ((*[[SLATE_SLOT_16]] as u32) as i32)) as i16) as u16;
// CHECK-NEXT:                                 if ((*[[SLATE_SLOT_14]] as u32) as i32)
// CHECK-NEXT:                                     >= ((*[[SLATE_SLOT_16]] as u32) as i32)
// CHECK-NEXT:                                     || ((*[[SLATE_SLOT_15]] as u32) as i32)
// CHECK-NEXT:                                         * ((*[[SLATE_SLOT_16]] as u32) as i32)
// CHECK-NEXT:                                         + ((*[[SLATE_SLOT_14]] as u32) as i32)
// CHECK-NEXT:                                         != ((*[[SLATE_SLOT_17]] as u32) as i32)
// CHECK-NEXT:                                 {
// CHECK-NEXT:                                     unsafe { abort() };
// CHECK-NEXT:                                 }
// CHECK-NEXT:                                 std::ptr::write(
// CHECK-NEXT:                                     [[SLATE_SLOT_13]],
// CHECK-NEXT:                                     (*[[SLATE_SLOT_35]] as u16) as i16,
// CHECK-NEXT:                                 );
// CHECK-NEXT:                                 std::ptr::write(
// CHECK-NEXT:                                     [[SLATE_SLOT_12]],
// CHECK-NEXT:                                     (*[[SLATE_SLOT_34]] as u16) as i16,
// CHECK-NEXT:                                 );
// CHECK-NEXT:                                 *[[SLATE_SLOT_11]] = ((*[[SLATE_SLOT_13]] as i32)
// CHECK-NEXT:                                     / (*[[SLATE_SLOT_12]] as i32)) as i16;
// CHECK-NEXT:                                 *[[SLATE_SLOT_10]] = ((*[[SLATE_SLOT_13]] as i32)
// CHECK-NEXT:                                     % (*[[SLATE_SLOT_12]] as i32)) as i16;
// CHECK-NEXT:                                 if (if (*[[SLATE_SLOT_10]] as i32) >= (0 as i32) {
// CHECK-NEXT:                                     *[[SLATE_SLOT_10]] as i32
// CHECK-NEXT:                                 } else {
// CHECK-NEXT:                                     -(*[[SLATE_SLOT_10]] as i32)
// CHECK-NEXT:                                 })
// CHECK-NEXT:                                     >= (((((if (*[[SLATE_SLOT_12]] as i32) >= (0 as i32) {
// CHECK-NEXT:                                         *[[SLATE_SLOT_12]] as i32
// CHECK-NEXT:                                     } else {
// CHECK-NEXT:                                         -(*[[SLATE_SLOT_12]] as i32)
// CHECK-NEXT:                                     }) as i16) as u16) as u32) as i32)
// CHECK-NEXT:                                     || ((((*[[SLATE_SLOT_11]] as i32) * (*[[SLATE_SLOT_12]] as i32)
// CHECK-NEXT:                                         + (*[[SLATE_SLOT_10]] as i32)) as i16) as i32)
// CHECK-NEXT:                                         != (*[[SLATE_SLOT_13]] as i32)
// CHECK-NEXT:                                 {
// CHECK-NEXT:                                     unsafe { abort() };
// CHECK-NEXT:                                 }
// CHECK-NEXT:                                 std::ptr::write([[SLATE_SLOT_9]], *[[SLATE_SLOT_35]] as u8);
// CHECK-NEXT:                                 std::ptr::write([[SLATE_SLOT_8]], *[[SLATE_SLOT_34]] as u8);
// CHECK-NEXT:                                 if ((*[[SLATE_SLOT_8]] as u32) as i32) == (0 as i32)
// CHECK-NEXT:                                 {} else {
// CHECK-NEXT:                                     *[[SLATE_SLOT_7]] = ((((*[[SLATE_SLOT_9]] as u32) as i32)
// CHECK-NEXT:                                         / ((*[[SLATE_SLOT_8]] as u32) as i32)) as i8) as u8;
// CHECK-NEXT:                                     *[[SLATE_SLOT_6]] = ((((*[[SLATE_SLOT_9]] as u32) as i32)
// CHECK-NEXT:                                         % ((*[[SLATE_SLOT_8]] as u32) as i32)) as i8) as u8;
// CHECK-NEXT:                                     if ((*[[SLATE_SLOT_6]] as u32) as i32)
// CHECK-NEXT:                                         >= ((*[[SLATE_SLOT_8]] as u32) as i32)
// CHECK-NEXT:                                         || ((*[[SLATE_SLOT_7]] as u32) as i32)
// CHECK-NEXT:                                             * ((*[[SLATE_SLOT_8]] as u32) as i32)
// CHECK-NEXT:                                             + ((*[[SLATE_SLOT_6]] as u32) as i32)
// CHECK-NEXT:                                             != ((*[[SLATE_SLOT_9]] as u32) as i32)
// CHECK-NEXT:                                     {
// CHECK-NEXT:                                         unsafe { abort() };
// CHECK-NEXT:                                     }
// CHECK-NEXT:                                     std::ptr::write(
// CHECK-NEXT:                                         [[SLATE_SLOT_5]],
// CHECK-NEXT:                                         (*[[SLATE_SLOT_35]] as u8) as i8,
// CHECK-NEXT:                                     );
// CHECK-NEXT:                                     std::ptr::write(
// CHECK-NEXT:                                         [[SLATE_SLOT_4]],
// CHECK-NEXT:                                         (*[[SLATE_SLOT_34]] as u8) as i8,
// CHECK-NEXT:                                     );
// CHECK-NEXT:                                     *[[SLATE_SLOT_3]] = ((*[[SLATE_SLOT_5]] as i32)
// CHECK-NEXT:                                         / (*[[SLATE_SLOT_4]] as i32)) as i8;
// CHECK-NEXT:                                     *[[SLATE_SLOT_2]] = ((*[[SLATE_SLOT_5]] as i32)
// CHECK-NEXT:                                         % (*[[SLATE_SLOT_4]] as i32)) as i8;
// CHECK-NEXT:                                     if (if (*[[SLATE_SLOT_2]] as i32) >= (0 as i32) {
// CHECK-NEXT:                                         *[[SLATE_SLOT_2]] as i32
// CHECK-NEXT:                                     } else {
// CHECK-NEXT:                                         -(*[[SLATE_SLOT_2]] as i32)
// CHECK-NEXT:                                     })
// CHECK-NEXT:                                         >= (((((if (*[[SLATE_SLOT_4]] as i32) >= (0 as i32) {
// CHECK-NEXT:                                             *[[SLATE_SLOT_4]] as i32
// CHECK-NEXT:                                         } else {
// CHECK-NEXT:                                             -(*[[SLATE_SLOT_4]] as i32)
// CHECK-NEXT:                                         }) as i8) as u8) as u32) as i32)
// CHECK-NEXT:                                         || ((((*[[SLATE_SLOT_3]] as i32) * (*[[SLATE_SLOT_4]] as i32)
// CHECK-NEXT:                                             + (*[[SLATE_SLOT_2]] as i32)) as i8) as i32)
// CHECK-NEXT:                                             != (*[[SLATE_SLOT_5]] as i32)
// CHECK-NEXT:                                     {
// CHECK-NEXT:                                         unsafe { abort() };
// CHECK-NEXT:                                     }
// CHECK-NEXT:                                 }
// CHECK-NEXT:                             }
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 std::ptr::write([[SLATE_SLOT_1]], *[[SLATE_SLOT_36]]);
// CHECK-NEXT:                 std::ptr::write([[SLATE_SLOT_0]], *[[SLATE_SLOT_1]] + ((1 as i32) as i64));
// CHECK-NEXT:                 *[[SLATE_SLOT_36]] = *[[SLATE_SLOT_0]];
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe { exit(0 as i32) };
// CHECK-NEXT:     }
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
