typedef __SIZE_TYPE__ size_t;
static int            mymemcmp1(unsigned long int, unsigned long int)
    __attribute__((__nothrow__));

__inline static int mymemcmp1(unsigned long int a, unsigned long int b) {
  long int          srcp1 = (long int)&a;
  long int          srcp2 = (long int)&b;
  unsigned long int a0, b0;
  do {
    a0     = ((unsigned char *)srcp1)[0];
    b0     = ((unsigned char *)srcp2)[0];
    srcp1 += 1;
    srcp2 += 1;
  } while (a0 == b0);
  return a0 - b0;
}

static int mymemcmp2(long, long, size_t) __attribute__((__nothrow__));

static int mymemcmp2(long int srcp1, long int srcp2, size_t len) {
  unsigned long int a0, a1;
  unsigned long int b0, b1;
  switch (len % 4) {
  default:
  case 2:
    a0     = ((unsigned long int *)srcp1)[0];
    b0     = ((unsigned long int *)srcp2)[0];
    srcp1 -= 2 * (sizeof(unsigned long int));
    srcp2 -= 2 * (sizeof(unsigned long int));
    len   += 2;
    goto do1;
  case 3:
    a1     = ((unsigned long int *)srcp1)[0];
    b1     = ((unsigned long int *)srcp2)[0];
    srcp1 -= (sizeof(unsigned long int));
    srcp2 -= (sizeof(unsigned long int));
    len   += 1;
    goto do2;
  case 0:
    if (16 <= 3 * (sizeof(unsigned long int)) && len == 0)
      return 0;
    a0 = ((unsigned long int *)srcp1)[0];
    b0 = ((unsigned long int *)srcp2)[0];
    goto do3;
  case 1:
    a1     = ((unsigned long int *)srcp1)[0];
    b1     = ((unsigned long int *)srcp2)[0];
    srcp1 += (sizeof(unsigned long int));
    srcp2 += (sizeof(unsigned long int));
    len   -= 1;
    if (16 <= 3 * (sizeof(unsigned long int)) && len == 0)
      goto do0;
  }
  do {
    a0 = ((unsigned long int *)srcp1)[0];
    b0 = ((unsigned long int *)srcp2)[0];
    if (a1 != b1)
      return mymemcmp1((a1), (b1));
  do3:
    a1 = ((unsigned long int *)srcp1)[1];
    b1 = ((unsigned long int *)srcp2)[1];
    if (a0 != b0)
      return mymemcmp1((a0), (b0));
  do2:
    a0 = ((unsigned long int *)srcp1)[2];
    b0 = ((unsigned long int *)srcp2)[2];
    if (a1 != b1)
      return mymemcmp1((a1), (b1));
  do1:
    a1 = ((unsigned long int *)srcp1)[3];
    b1 = ((unsigned long int *)srcp2)[3];
    if (a0 != b0)
      return mymemcmp1((a0), (b0));
    srcp1 += 4 * (sizeof(unsigned long int));
    srcp2 += 4 * (sizeof(unsigned long int));
    len   -= 4;
  } while (len != 0);
do0:
  if (a1 != b1)
    return mymemcmp1((a1), (b1));
  return 0;
}

static int mymemcmp3(long, long, size_t) __attribute__((__nothrow__));

static int mymemcmp3(long int srcp1, long int srcp2, size_t len) {
  unsigned long int a0, a1, a2, a3;
  unsigned long int b0, b1, b2, b3;
  unsigned long int x;
  int               shl, shr;
  shl    = 8 * (srcp1 % (sizeof(unsigned long int)));
  shr    = 8 * (sizeof(unsigned long int)) - shl;
  srcp1 &= -(sizeof(unsigned long int));
  switch (len % 4) {
  default:
  case 2:
    a1     = ((unsigned long int *)srcp1)[0];
    a2     = ((unsigned long int *)srcp1)[1];
    b2     = ((unsigned long int *)srcp2)[0];
    srcp1 -= 1 * (sizeof(unsigned long int));
    srcp2 -= 2 * (sizeof(unsigned long int));
    len   += 2;
    goto do1;
  case 3:
    a0     = ((unsigned long int *)srcp1)[0];
    a1     = ((unsigned long int *)srcp1)[1];
    b1     = ((unsigned long int *)srcp2)[0];
    srcp2 -= 1 * (sizeof(unsigned long int));
    len   += 1;
    goto do2;
  case 0:
    if (16 <= 3 * (sizeof(unsigned long int)) && len == 0)
      return 0;
    a3     = ((unsigned long int *)srcp1)[0];
    a0     = ((unsigned long int *)srcp1)[1];
    b0     = ((unsigned long int *)srcp2)[0];
    srcp1 += 1 * (sizeof(unsigned long int));
    goto do3;
  case 1:
    a2     = ((unsigned long int *)srcp1)[0];
    a3     = ((unsigned long int *)srcp1)[1];
    b3     = ((unsigned long int *)srcp2)[0];
    srcp1 += 2 * (sizeof(unsigned long int));
    srcp2 += 1 * (sizeof(unsigned long int));
    len   -= 1;
    if (16 <= 3 * (sizeof(unsigned long int)) && len == 0)
      goto do0;
  }
  do {
    a0 = ((unsigned long int *)srcp1)[0];
    b0 = ((unsigned long int *)srcp2)[0];
    x  = (((a2) >> (shl)) | ((a3) << (shr)));
    if (x != b3)
      return mymemcmp1((x), (b3));
  do3:
    a1 = ((unsigned long int *)srcp1)[1];
    b1 = ((unsigned long int *)srcp2)[1];
    x  = (((a3) >> (shl)) | ((a0) << (shr)));
    if (x != b0)
      return mymemcmp1((x), (b0));
  do2:
    a2 = ((unsigned long int *)srcp1)[2];
    b2 = ((unsigned long int *)srcp2)[2];
    x  = (((a0) >> (shl)) | ((a1) << (shr)));
    if (x != b1)
      return mymemcmp1((x), (b1));
  do1:
    a3 = ((unsigned long int *)srcp1)[3];
    b3 = ((unsigned long int *)srcp2)[3];
    x  = (((a1) >> (shl)) | ((a2) << (shr)));
    if (x != b2)
      return mymemcmp1((x), (b2));
    srcp1 += 4 * (sizeof(unsigned long int));
    srcp2 += 4 * (sizeof(unsigned long int));
    len   -= 4;
  } while (len != 0);
do0:
  x = (((a2) >> (shl)) | ((a3) << (shr)));
  if (x != b3)
    return mymemcmp1((x), (b3));
  return 0;
}

__attribute__((noinline)) int mymemcmp(const void *s1, const void *s2,
                                       size_t len) {
  unsigned long int a0;
  unsigned long int b0;
  long int          srcp1 = (long int)s1;
  long int          srcp2 = (long int)s2;
  if (srcp1 % (sizeof(unsigned long int)) == 0)
    return mymemcmp2(srcp1, srcp2, len / (sizeof(unsigned long int)));
  else
    return mymemcmp3(srcp1, srcp2, len / (sizeof(unsigned long int)));
}

char buf[256];

int main(void) {
  char *p;
  union {
    long int l;
    char     c[sizeof(long int)];
  } u;

  /* The test above assumes little endian and long being the same size
     as pointer.  */
  if (sizeof(long int) != sizeof(void *) || sizeof(long int) < 4)
    return 0;
  u.l = 0x12345678L;
  if (u.c[0] != 0x78 || u.c[1] != 0x56 || u.c[2] != 0x34 || u.c[3] != 0x12)
    return 0;

  p = buf + 16 - (((long int)buf) & 15);
  __builtin_memcpy(
      p + 9, "\x1\x37\x82\xa7\x55\x49\x9d\xbf\xf8\x44\xb6\x55\x17\x8e\xf9", 15);
  __builtin_memcpy(
      p + 128 + 24,
      "\x1\x37\x82\xa7\x55\x49\xd0\xf3\xb7\x2a\x6d\x23\x71\x49\x6a", 15);
  if (mymemcmp(p + 9, p + 128 + 24, 33) != -51)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "memcpy"]
// CHECK-NEXT:     fn __builtin_memcpy([[V_0:__v[0-9]+]]: *mut (), [[V_1:__v[0-9]+]]: *const (), [[V_2:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     l: i64,
// CHECK-NEXT:     c: [i8; 8],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: fn mymemcmp1(mut a: u64, mut b: u64) -> i32 {
// CHECK-NEXT:     let mut srcp1: i64 = std::ptr::addr_of_mut!(a) as i64;
// CHECK-NEXT:     let mut srcp2: i64 = std::ptr::addr_of_mut!(b) as i64;
// CHECK-NEXT:     let mut a0: u64 = 0 as u64;
// CHECK-NEXT:     let mut b0: u64 = 0 as u64;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         a0 = (unsafe { *unsafe { (srcp1 as *mut u8).offset((0 as i32) as isize) } }) as u64;
// CHECK-NEXT:         b0 = (unsafe { *unsafe { (srcp2 as *mut u8).offset((0 as i32) as isize) } }) as u64;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i64 = srcp1;
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i64 = [[V_3]] + ((1 as i32) as i64);
// CHECK-NEXT:         srcp1 = [[V_4]];
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: i64 = srcp2;
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: i64 = [[V_5]] + ((1 as i32) as i64);
// CHECK-NEXT:         srcp2 = [[V_6]];
// CHECK-NEXT:         if !(a0 == b0) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     (a0.wrapping_sub(b0) as u32) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn mymemcmp2(mut srcp1: i64, mut srcp2: i64, mut len: u64) -> i32 {
// CHECK-NEXT:     let mut [[SLATE_STORAGE_0:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_0:__slate_slot_[0-9]+]]: *mut u64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_0]]) as *mut u64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_1:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_1:__slate_slot_[0-9]+]]: *mut u64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_1]]) as *mut u64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_2:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_2:__slate_slot_[0-9]+]]: *mut i64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_2]]) as *mut i64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_3:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_3:__slate_slot_[0-9]+]]: *mut i64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_3]]) as *mut i64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_4:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_4:__slate_slot_[0-9]+]]: *mut i64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_4]]) as *mut i64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_5:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_5:__slate_slot_[0-9]+]]: *mut i64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_5]]) as *mut i64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_6:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_6:__slate_slot_[0-9]+]]: *mut u64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_6]]) as *mut u64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_7:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_7:__slate_slot_[0-9]+]]: *mut u64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_7]]) as *mut u64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_8:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_8:__slate_slot_[0-9]+]]: *mut i64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_8]]) as *mut i64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_9:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_9:__slate_slot_[0-9]+]]: *mut i64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_9]]) as *mut i64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_10:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_10:__slate_slot_[0-9]+]]: *mut i64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_10]]) as *mut i64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_11:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_11:__slate_slot_[0-9]+]]: *mut i64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_11]]) as *mut i64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_12:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_12:__slate_slot_[0-9]+]]: *mut u64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_12]]) as *mut u64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_13:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_13:__slate_slot_[0-9]+]]: *mut u64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_13]]) as *mut u64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_14:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_14:__slate_slot_[0-9]+]]: *mut i64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_14]]) as *mut i64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_15:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_15:__slate_slot_[0-9]+]]: *mut i64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_15]]) as *mut i64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_16:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_16:__slate_slot_[0-9]+]]: *mut i64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_16]]) as *mut i64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_17:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_17:__slate_slot_[0-9]+]]: *mut i64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_17]]) as *mut i64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_18:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_18:__slate_slot_[0-9]+]]: *mut u64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_18]]) as *mut u64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_19:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_19:__slate_slot_[0-9]+]]: *mut u64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_19]]) as *mut u64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_20:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_20:__slate_slot_[0-9]+]]: *mut i64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_20]]) as *mut i64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_21:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_21:__slate_slot_[0-9]+]]: *mut i64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_21]]) as *mut i64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_22:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_22:__slate_slot_[0-9]+]]: *mut i64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_22]]) as *mut i64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_23:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_23:__slate_slot_[0-9]+]]: *mut i64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_23]]) as *mut i64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_24:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_24:__slate_slot_[0-9]+]]: *mut u64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_24]]) as *mut u64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_25:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_25:__slate_slot_[0-9]+]]: *mut u64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_25]]) as *mut u64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_26:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_26:__slate_slot_[0-9]+]]: *mut u64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_26]]) as *mut u64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_27:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_27:__slate_slot_[0-9]+]]: *mut u64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_27]]) as *mut u64;
// CHECK-NEXT:     let mut __slate_state: usize = 18usize;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         '[[JOIN_0:__join_[0-9]+]]: {
// CHECK-NEXT:             '[[JOIN_1:__join_[0-9]+]]: {
// CHECK-NEXT:                 '[[JOIN_2:__join_[0-9]+]]: {
// CHECK-NEXT:                     let [[T_0:__t[0-9]+]]: u64 = len % (((4 as i32) as i64) as u64);
// CHECK-NEXT:                     if [[T_0]] == (2 as u64) {
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         if [[T_0]] == (3 as u64) {
// CHECK-NEXT:                             *[[SLATE_SLOT_26]] = unsafe {
// CHECK-NEXT:                                 *unsafe { (srcp1 as *mut u64).offset((0 as i32) as isize) }
// CHECK-NEXT:                             };
// CHECK-NEXT:                             *[[SLATE_SLOT_24]] = unsafe {
// CHECK-NEXT:                                 *unsafe { (srcp2 as *mut u64).offset((0 as i32) as isize) }
// CHECK-NEXT:                             };
// CHECK-NEXT:                             std::ptr::write([[SLATE_SLOT_17]], srcp1);
// CHECK-NEXT:                             std::ptr::write(
// CHECK-NEXT:                                 [[SLATE_SLOT_16]],
// CHECK-NEXT:                                 (*[[SLATE_SLOT_17]] as u64).wrapping_sub(8 as u64) as i64,
// CHECK-NEXT:                             );
// CHECK-NEXT:                             srcp1 = *[[SLATE_SLOT_16]];
// CHECK-NEXT:                             std::ptr::write([[SLATE_SLOT_15]], srcp2);
// CHECK-NEXT:                             std::ptr::write(
// CHECK-NEXT:                                 [[SLATE_SLOT_14]],
// CHECK-NEXT:                                 (*[[SLATE_SLOT_15]] as u64).wrapping_sub(8 as u64) as i64,
// CHECK-NEXT:                             );
// CHECK-NEXT:                             srcp2 = *[[SLATE_SLOT_14]];
// CHECK-NEXT:                             std::ptr::write([[SLATE_SLOT_13]], len);
// CHECK-NEXT:                             std::ptr::write(
// CHECK-NEXT:                                 [[SLATE_SLOT_12]],
// CHECK-NEXT:                                 (*[[SLATE_SLOT_13]]).wrapping_add(((1 as i32) as i64) as u64),
// CHECK-NEXT:                             );
// CHECK-NEXT:                             len = *[[SLATE_SLOT_12]];
// CHECK-NEXT:                             __slate_state = 7;
// CHECK-NEXT:                             break '[[JOIN_1]];
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             if [[T_0]] == (0 as u64) {
// CHECK-NEXT:                                 if (((16 as i32) as i64) as u64)
// CHECK-NEXT:                                     <= (((3 as i32) as i64) as u64).wrapping_mul(8 as u64)
// CHECK-NEXT:                                     && len == (((0 as i32) as i64) as u64)
// CHECK-NEXT:                                 {
// CHECK-NEXT:                                     return 0 as i32;
// CHECK-NEXT:                                 } else {
// CHECK-NEXT:                                     *[[SLATE_SLOT_27]] = unsafe {
// CHECK-NEXT:                                         *unsafe { (srcp1 as *mut u64).offset((0 as i32) as isize) }
// CHECK-NEXT:                                     };
// CHECK-NEXT:                                     *[[SLATE_SLOT_25]] = unsafe {
// CHECK-NEXT:                                         *unsafe { (srcp2 as *mut u64).offset((0 as i32) as isize) }
// CHECK-NEXT:                                     };
// CHECK-NEXT:                                     __slate_state = 9;
// CHECK-NEXT:                                     break '[[JOIN_1]];
// CHECK-NEXT:                                 }
// CHECK-NEXT:                             } else {
// CHECK-NEXT:                                 if [[T_0]] == (1 as u64) {
// CHECK-NEXT:                                     *[[SLATE_SLOT_26]] = unsafe {
// CHECK-NEXT:                                         *unsafe { (srcp1 as *mut u64).offset((0 as i32) as isize) }
// CHECK-NEXT:                                     };
// CHECK-NEXT:                                     *[[SLATE_SLOT_24]] = unsafe {
// CHECK-NEXT:                                         *unsafe { (srcp2 as *mut u64).offset((0 as i32) as isize) }
// CHECK-NEXT:                                     };
// CHECK-NEXT:                                     std::ptr::write([[SLATE_SLOT_11]], srcp1);
// CHECK-NEXT:                                     std::ptr::write(
// CHECK-NEXT:                                         [[SLATE_SLOT_10]],
// CHECK-NEXT:                                         (*[[SLATE_SLOT_11]] as u64).wrapping_add(8 as u64) as i64,
// CHECK-NEXT:                                     );
// CHECK-NEXT:                                     srcp1 = *[[SLATE_SLOT_10]];
// CHECK-NEXT:                                     std::ptr::write([[SLATE_SLOT_9]], srcp2);
// CHECK-NEXT:                                     std::ptr::write(
// CHECK-NEXT:                                         [[SLATE_SLOT_8]],
// CHECK-NEXT:                                         (*[[SLATE_SLOT_9]] as u64).wrapping_add(8 as u64) as i64,
// CHECK-NEXT:                                     );
// CHECK-NEXT:                                     srcp2 = *[[SLATE_SLOT_8]];
// CHECK-NEXT:                                     std::ptr::write([[SLATE_SLOT_7]], len);
// CHECK-NEXT:                                     std::ptr::write(
// CHECK-NEXT:                                         [[SLATE_SLOT_6]],
// CHECK-NEXT:                                         (*[[SLATE_SLOT_7]]).wrapping_sub(((1 as i32) as i64) as u64),
// CHECK-NEXT:                                     );
// CHECK-NEXT:                                     len = *[[SLATE_SLOT_6]];
// CHECK-NEXT:                                     if (((16 as i32) as i64) as u64)
// CHECK-NEXT:                                         <= (((3 as i32) as i64) as u64).wrapping_mul(8 as u64)
// CHECK-NEXT:                                         && len == (((0 as i32) as i64) as u64)
// CHECK-NEXT:                                     {
// CHECK-NEXT:                                         break '[[JOIN_0]];
// CHECK-NEXT:                                     } else {
// CHECK-NEXT:                                         __slate_state = 11;
// CHECK-NEXT:                                         break '[[JOIN_1]];
// CHECK-NEXT:                                     }
// CHECK-NEXT:                                 }
// CHECK-NEXT:                             }
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 *[[SLATE_SLOT_27]] =
// CHECK-NEXT:                     unsafe { *unsafe { (srcp1 as *mut u64).offset((0 as i32) as isize) } };
// CHECK-NEXT:                 *[[SLATE_SLOT_25]] =
// CHECK-NEXT:                     unsafe { *unsafe { (srcp2 as *mut u64).offset((0 as i32) as isize) } };
// CHECK-NEXT:                 std::ptr::write([[SLATE_SLOT_23]], srcp1);
// CHECK-NEXT:                 std::ptr::write(
// CHECK-NEXT:                     [[SLATE_SLOT_22]],
// CHECK-NEXT:                     (*[[SLATE_SLOT_23]] as u64)
// CHECK-NEXT:                         .wrapping_sub((((2 as i32) as i64) as u64).wrapping_mul(8 as u64))
// CHECK-NEXT:                         as i64,
// CHECK-NEXT:                 );
// CHECK-NEXT:                 srcp1 = *[[SLATE_SLOT_22]];
// CHECK-NEXT:                 std::ptr::write([[SLATE_SLOT_21]], srcp2);
// CHECK-NEXT:                 std::ptr::write(
// CHECK-NEXT:                     [[SLATE_SLOT_20]],
// CHECK-NEXT:                     (*[[SLATE_SLOT_21]] as u64)
// CHECK-NEXT:                         .wrapping_sub((((2 as i32) as i64) as u64).wrapping_mul(8 as u64))
// CHECK-NEXT:                         as i64,
// CHECK-NEXT:                 );
// CHECK-NEXT:                 srcp2 = *[[SLATE_SLOT_20]];
// CHECK-NEXT:                 std::ptr::write([[SLATE_SLOT_19]], len);
// CHECK-NEXT:                 std::ptr::write(
// CHECK-NEXT:                     [[SLATE_SLOT_18]],
// CHECK-NEXT:                     (*[[SLATE_SLOT_19]]).wrapping_add(((2 as i32) as i64) as u64),
// CHECK-NEXT:                 );
// CHECK-NEXT:                 len = *[[SLATE_SLOT_18]];
// CHECK-NEXT:                 __slate_state = 5;
// CHECK-NEXT:             }
// CHECK-NEXT:             loop {
// CHECK-NEXT:                 match __slate_state {
// CHECK-NEXT:                     5 => {
// CHECK-NEXT:                         *[[SLATE_SLOT_26]] =
// CHECK-NEXT:                             unsafe { *unsafe { (srcp1 as *mut u64).offset((3 as i32) as isize) } };
// CHECK-NEXT:                         *[[SLATE_SLOT_24]] =
// CHECK-NEXT:                             unsafe { *unsafe { (srcp2 as *mut u64).offset((3 as i32) as isize) } };
// CHECK-NEXT:                         if *[[SLATE_SLOT_27]] != *[[SLATE_SLOT_25]] {
// CHECK-NEXT:                             return mymemcmp1(*[[SLATE_SLOT_27]], *[[SLATE_SLOT_25]]);
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             std::ptr::write([[SLATE_SLOT_5]], srcp1);
// CHECK-NEXT:                             std::ptr::write(
// CHECK-NEXT:                                 [[SLATE_SLOT_4]],
// CHECK-NEXT:                                 (*[[SLATE_SLOT_5]] as u64).wrapping_add(
// CHECK-NEXT:                                     (((4 as i32) as i64) as u64).wrapping_mul(8 as u64),
// CHECK-NEXT:                                 ) as i64,
// CHECK-NEXT:                             );
// CHECK-NEXT:                             srcp1 = *[[SLATE_SLOT_4]];
// CHECK-NEXT:                             std::ptr::write([[SLATE_SLOT_3]], srcp2);
// CHECK-NEXT:                             std::ptr::write(
// CHECK-NEXT:                                 [[SLATE_SLOT_2]],
// CHECK-NEXT:                                 (*[[SLATE_SLOT_3]] as u64).wrapping_add(
// CHECK-NEXT:                                     (((4 as i32) as i64) as u64).wrapping_mul(8 as u64),
// CHECK-NEXT:                                 ) as i64,
// CHECK-NEXT:                             );
// CHECK-NEXT:                             srcp2 = *[[SLATE_SLOT_2]];
// CHECK-NEXT:                             std::ptr::write([[SLATE_SLOT_1]], len);
// CHECK-NEXT:                             std::ptr::write(
// CHECK-NEXT:                                 [[SLATE_SLOT_0]],
// CHECK-NEXT:                                 (*[[SLATE_SLOT_1]]).wrapping_sub(((4 as i32) as i64) as u64),
// CHECK-NEXT:                             );
// CHECK-NEXT:                             len = *[[SLATE_SLOT_0]];
// CHECK-NEXT:                             if len != (((0 as i32) as i64) as u64) {
// CHECK-NEXT:                                 __slate_state = 11;
// CHECK-NEXT:                             } else {
// CHECK-NEXT:                                 break '[[JOIN_0]];
// CHECK-NEXT:                             }
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                     7 => {
// CHECK-NEXT:                         *[[SLATE_SLOT_27]] =
// CHECK-NEXT:                             unsafe { *unsafe { (srcp1 as *mut u64).offset((2 as i32) as isize) } };
// CHECK-NEXT:                         *[[SLATE_SLOT_25]] =
// CHECK-NEXT:                             unsafe { *unsafe { (srcp2 as *mut u64).offset((2 as i32) as isize) } };
// CHECK-NEXT:                         if *[[SLATE_SLOT_26]] != *[[SLATE_SLOT_24]] {
// CHECK-NEXT:                             return mymemcmp1(*[[SLATE_SLOT_26]], *[[SLATE_SLOT_24]]);
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             __slate_state = 5;
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                     9 => {
// CHECK-NEXT:                         *[[SLATE_SLOT_26]] =
// CHECK-NEXT:                             unsafe { *unsafe { (srcp1 as *mut u64).offset((1 as i32) as isize) } };
// CHECK-NEXT:                         *[[SLATE_SLOT_24]] =
// CHECK-NEXT:                             unsafe { *unsafe { (srcp2 as *mut u64).offset((1 as i32) as isize) } };
// CHECK-NEXT:                         if *[[SLATE_SLOT_27]] != *[[SLATE_SLOT_25]] {
// CHECK-NEXT:                             return mymemcmp1(*[[SLATE_SLOT_27]], *[[SLATE_SLOT_25]]);
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             __slate_state = 7;
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                     _ => {
// CHECK-NEXT:                         *[[SLATE_SLOT_27]] =
// CHECK-NEXT:                             unsafe { *unsafe { (srcp1 as *mut u64).offset((0 as i32) as isize) } };
// CHECK-NEXT:                         *[[SLATE_SLOT_25]] =
// CHECK-NEXT:                             unsafe { *unsafe { (srcp2 as *mut u64).offset((0 as i32) as isize) } };
// CHECK-NEXT:                         if *[[SLATE_SLOT_26]] != *[[SLATE_SLOT_24]] {
// CHECK-NEXT:                             break;
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             __slate_state = 9;
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             return mymemcmp1(*[[SLATE_SLOT_26]], *[[SLATE_SLOT_24]]);
// CHECK-NEXT:         }
// CHECK-NEXT:         if *[[SLATE_SLOT_26]] != *[[SLATE_SLOT_24]] {
// CHECK-NEXT:             return mymemcmp1(*[[SLATE_SLOT_26]], *[[SLATE_SLOT_24]]);
// CHECK-NEXT:         } else {
// CHECK-NEXT:             return 0 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn mymemcmp3(mut srcp1: i64, mut srcp2: i64, mut len: u64) -> i32 {
// CHECK-NEXT:     let mut [[SLATE_STORAGE_28:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_28:__slate_slot_[0-9]+]]: *mut u64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_28]]) as *mut u64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_29:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_29:__slate_slot_[0-9]+]]: *mut u64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_29]]) as *mut u64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_30:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_30:__slate_slot_[0-9]+]]: *mut i64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_30]]) as *mut i64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_31:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_31:__slate_slot_[0-9]+]]: *mut i64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_31]]) as *mut i64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_32:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_32:__slate_slot_[0-9]+]]: *mut i64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_32]]) as *mut i64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_33:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_33:__slate_slot_[0-9]+]]: *mut i64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_33]]) as *mut i64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_34:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_34:__slate_slot_[0-9]+]]: *mut u64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_34]]) as *mut u64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_35:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_35:__slate_slot_[0-9]+]]: *mut u64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_35]]) as *mut u64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_36:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_36:__slate_slot_[0-9]+]]: *mut i64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_36]]) as *mut i64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_37:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_37:__slate_slot_[0-9]+]]: *mut i64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_37]]) as *mut i64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_38:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_38:__slate_slot_[0-9]+]]: *mut i64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_38]]) as *mut i64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_39:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_39:__slate_slot_[0-9]+]]: *mut i64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_39]]) as *mut i64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_40:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_40:__slate_slot_[0-9]+]]: *mut i64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_40]]) as *mut i64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_41:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_41:__slate_slot_[0-9]+]]: *mut i64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_41]]) as *mut i64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_42:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_42:__slate_slot_[0-9]+]]: *mut u64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_42]]) as *mut u64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_43:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_43:__slate_slot_[0-9]+]]: *mut u64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_43]]) as *mut u64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_44:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_44:__slate_slot_[0-9]+]]: *mut i64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_44]]) as *mut i64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_45:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_45:__slate_slot_[0-9]+]]: *mut i64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_45]]) as *mut i64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_46:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_46:__slate_slot_[0-9]+]]: *mut u64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_46]]) as *mut u64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_47:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_47:__slate_slot_[0-9]+]]: *mut u64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_47]]) as *mut u64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_48:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_48:__slate_slot_[0-9]+]]: *mut i64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_48]]) as *mut i64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_49:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_49:__slate_slot_[0-9]+]]: *mut i64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_49]]) as *mut i64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_50:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_50:__slate_slot_[0-9]+]]: *mut i64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_50]]) as *mut i64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_51:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_51:__slate_slot_[0-9]+]]: *mut i64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_51]]) as *mut i64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_52:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_52:__slate_slot_[0-9]+]]: *mut i64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_52]]) as *mut i64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_53:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_53:__slate_slot_[0-9]+]]: *mut i64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_53]]) as *mut i64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_54:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_54:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_54]]) as *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_55:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_55:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_55]]) as *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_56:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_56:__slate_slot_[0-9]+]]: *mut u64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_56]]) as *mut u64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_57:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_57:__slate_slot_[0-9]+]]: *mut u64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_57]]) as *mut u64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_58:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_58:__slate_slot_[0-9]+]]: *mut u64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_58]]) as *mut u64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_59:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_59:__slate_slot_[0-9]+]]: *mut u64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_59]]) as *mut u64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_60:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_60:__slate_slot_[0-9]+]]: *mut u64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_60]]) as *mut u64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_61:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_61:__slate_slot_[0-9]+]]: *mut u64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_61]]) as *mut u64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_62:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_62:__slate_slot_[0-9]+]]: *mut u64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_62]]) as *mut u64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_63:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_63:__slate_slot_[0-9]+]]: *mut u64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_63]]) as *mut u64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_64:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_64:__slate_slot_[0-9]+]]: *mut u64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_64]]) as *mut u64;
// CHECK-NEXT:     let mut __slate_state: usize = 18usize;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         '[[JOIN_0]]: {
// CHECK-NEXT:             '[[JOIN_1]]: {
// CHECK-NEXT:                 '[[JOIN_2]]: {
// CHECK-NEXT:                     *[[SLATE_SLOT_55]] = ((((8 as i32) as i64) as u64)
// CHECK-NEXT:                         .wrapping_mul((srcp1 as u64) % (8 as u64))
// CHECK-NEXT:                         as u32) as i32;
// CHECK-NEXT:                     *[[SLATE_SLOT_54]] = ((((8 as i32) as i64) as u64)
// CHECK-NEXT:                         .wrapping_mul(8 as u64)
// CHECK-NEXT:                         .wrapping_sub((*[[SLATE_SLOT_55]] as i64) as u64)
// CHECK-NEXT:                         as u32) as i32;
// CHECK-NEXT:                     std::ptr::write([[SLATE_SLOT_53]], srcp1);
// CHECK-NEXT:                     std::ptr::write(
// CHECK-NEXT:                         [[SLATE_SLOT_52]],
// CHECK-NEXT:                         ((*[[SLATE_SLOT_53]] as u64) & (8 as u64).wrapping_neg()) as i64,
// CHECK-NEXT:                     );
// CHECK-NEXT:                     srcp1 = *[[SLATE_SLOT_52]];
// CHECK-NEXT:                     let [[T_0]]: u64 = len % (((4 as i32) as i64) as u64);
// CHECK-NEXT:                     if [[T_0]] == (2 as u64) {
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         if [[T_0]] == (3 as u64) {
// CHECK-NEXT:                             *[[SLATE_SLOT_64]] = unsafe {
// CHECK-NEXT:                                 *unsafe { (srcp1 as *mut u64).offset((0 as i32) as isize) }
// CHECK-NEXT:                             };
// CHECK-NEXT:                             *[[SLATE_SLOT_63]] = unsafe {
// CHECK-NEXT:                                 *unsafe { (srcp1 as *mut u64).offset((1 as i32) as isize) }
// CHECK-NEXT:                             };
// CHECK-NEXT:                             *[[SLATE_SLOT_59]] = unsafe {
// CHECK-NEXT:                                 *unsafe { (srcp2 as *mut u64).offset((0 as i32) as isize) }
// CHECK-NEXT:                             };
// CHECK-NEXT:                             std::ptr::write([[SLATE_SLOT_45]], srcp2);
// CHECK-NEXT:                             std::ptr::write(
// CHECK-NEXT:                                 [[SLATE_SLOT_44]],
// CHECK-NEXT:                                 (*[[SLATE_SLOT_45]] as u64).wrapping_sub(
// CHECK-NEXT:                                     (((1 as i32) as i64) as u64).wrapping_mul(8 as u64),
// CHECK-NEXT:                                 ) as i64,
// CHECK-NEXT:                             );
// CHECK-NEXT:                             srcp2 = *[[SLATE_SLOT_44]];
// CHECK-NEXT:                             std::ptr::write([[SLATE_SLOT_43]], len);
// CHECK-NEXT:                             std::ptr::write(
// CHECK-NEXT:                                 [[SLATE_SLOT_42]],
// CHECK-NEXT:                                 (*[[SLATE_SLOT_43]]).wrapping_add(((1 as i32) as i64) as u64),
// CHECK-NEXT:                             );
// CHECK-NEXT:                             len = *[[SLATE_SLOT_42]];
// CHECK-NEXT:                             __slate_state = 7;
// CHECK-NEXT:                             break '[[JOIN_1]];
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             if [[T_0]] == (0 as u64) {
// CHECK-NEXT:                                 if (((16 as i32) as i64) as u64)
// CHECK-NEXT:                                     <= (((3 as i32) as i64) as u64).wrapping_mul(8 as u64)
// CHECK-NEXT:                                     && len == (((0 as i32) as i64) as u64)
// CHECK-NEXT:                                 {
// CHECK-NEXT:                                     return 0 as i32;
// CHECK-NEXT:                                 } else {
// CHECK-NEXT:                                     *[[SLATE_SLOT_61]] = unsafe {
// CHECK-NEXT:                                         *unsafe { (srcp1 as *mut u64).offset((0 as i32) as isize) }
// CHECK-NEXT:                                     };
// CHECK-NEXT:                                     *[[SLATE_SLOT_64]] = unsafe {
// CHECK-NEXT:                                         *unsafe { (srcp1 as *mut u64).offset((1 as i32) as isize) }
// CHECK-NEXT:                                     };
// CHECK-NEXT:                                     *[[SLATE_SLOT_60]] = unsafe {
// CHECK-NEXT:                                         *unsafe { (srcp2 as *mut u64).offset((0 as i32) as isize) }
// CHECK-NEXT:                                     };
// CHECK-NEXT:                                     std::ptr::write([[SLATE_SLOT_41]], srcp1);
// CHECK-NEXT:                                     std::ptr::write(
// CHECK-NEXT:                                         [[SLATE_SLOT_40]],
// CHECK-NEXT:                                         (*[[SLATE_SLOT_41]] as u64).wrapping_add(
// CHECK-NEXT:                                             (((1 as i32) as i64) as u64).wrapping_mul(8 as u64),
// CHECK-NEXT:                                         ) as i64,
// CHECK-NEXT:                                     );
// CHECK-NEXT:                                     srcp1 = *[[SLATE_SLOT_40]];
// CHECK-NEXT:                                     __slate_state = 9;
// CHECK-NEXT:                                     break '[[JOIN_1]];
// CHECK-NEXT:                                 }
// CHECK-NEXT:                             } else {
// CHECK-NEXT:                                 if [[T_0]] == (1 as u64) {
// CHECK-NEXT:                                     *[[SLATE_SLOT_62]] = unsafe {
// CHECK-NEXT:                                         *unsafe { (srcp1 as *mut u64).offset((0 as i32) as isize) }
// CHECK-NEXT:                                     };
// CHECK-NEXT:                                     *[[SLATE_SLOT_61]] = unsafe {
// CHECK-NEXT:                                         *unsafe { (srcp1 as *mut u64).offset((1 as i32) as isize) }
// CHECK-NEXT:                                     };
// CHECK-NEXT:                                     *[[SLATE_SLOT_57]] = unsafe {
// CHECK-NEXT:                                         *unsafe { (srcp2 as *mut u64).offset((0 as i32) as isize) }
// CHECK-NEXT:                                     };
// CHECK-NEXT:                                     std::ptr::write([[SLATE_SLOT_39]], srcp1);
// CHECK-NEXT:                                     std::ptr::write(
// CHECK-NEXT:                                         [[SLATE_SLOT_38]],
// CHECK-NEXT:                                         (*[[SLATE_SLOT_39]] as u64).wrapping_add(
// CHECK-NEXT:                                             (((2 as i32) as i64) as u64).wrapping_mul(8 as u64),
// CHECK-NEXT:                                         ) as i64,
// CHECK-NEXT:                                     );
// CHECK-NEXT:                                     srcp1 = *[[SLATE_SLOT_38]];
// CHECK-NEXT:                                     std::ptr::write([[SLATE_SLOT_37]], srcp2);
// CHECK-NEXT:                                     std::ptr::write(
// CHECK-NEXT:                                         [[SLATE_SLOT_36]],
// CHECK-NEXT:                                         (*[[SLATE_SLOT_37]] as u64).wrapping_add(
// CHECK-NEXT:                                             (((1 as i32) as i64) as u64).wrapping_mul(8 as u64),
// CHECK-NEXT:                                         ) as i64,
// CHECK-NEXT:                                     );
// CHECK-NEXT:                                     srcp2 = *[[SLATE_SLOT_36]];
// CHECK-NEXT:                                     std::ptr::write([[SLATE_SLOT_35]], len);
// CHECK-NEXT:                                     std::ptr::write(
// CHECK-NEXT:                                         [[SLATE_SLOT_34]],
// CHECK-NEXT:                                         (*[[SLATE_SLOT_35]])
// CHECK-NEXT:                                             .wrapping_sub(((1 as i32) as i64) as u64),
// CHECK-NEXT:                                     );
// CHECK-NEXT:                                     len = *[[SLATE_SLOT_34]];
// CHECK-NEXT:                                     if (((16 as i32) as i64) as u64)
// CHECK-NEXT:                                         <= (((3 as i32) as i64) as u64).wrapping_mul(8 as u64)
// CHECK-NEXT:                                         && len == (((0 as i32) as i64) as u64)
// CHECK-NEXT:                                     {
// CHECK-NEXT:                                         break '[[JOIN_0]];
// CHECK-NEXT:                                     } else {
// CHECK-NEXT:                                         __slate_state = 11;
// CHECK-NEXT:                                         break '[[JOIN_1]];
// CHECK-NEXT:                                     }
// CHECK-NEXT:                                 }
// CHECK-NEXT:                             }
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 *[[SLATE_SLOT_63]] =
// CHECK-NEXT:                     unsafe { *unsafe { (srcp1 as *mut u64).offset((0 as i32) as isize) } };
// CHECK-NEXT:                 *[[SLATE_SLOT_62]] =
// CHECK-NEXT:                     unsafe { *unsafe { (srcp1 as *mut u64).offset((1 as i32) as isize) } };
// CHECK-NEXT:                 *[[SLATE_SLOT_58]] =
// CHECK-NEXT:                     unsafe { *unsafe { (srcp2 as *mut u64).offset((0 as i32) as isize) } };
// CHECK-NEXT:                 std::ptr::write([[SLATE_SLOT_51]], srcp1);
// CHECK-NEXT:                 std::ptr::write(
// CHECK-NEXT:                     [[SLATE_SLOT_50]],
// CHECK-NEXT:                     (*[[SLATE_SLOT_51]] as u64)
// CHECK-NEXT:                         .wrapping_sub((((1 as i32) as i64) as u64).wrapping_mul(8 as u64))
// CHECK-NEXT:                         as i64,
// CHECK-NEXT:                 );
// CHECK-NEXT:                 srcp1 = *[[SLATE_SLOT_50]];
// CHECK-NEXT:                 std::ptr::write([[SLATE_SLOT_49]], srcp2);
// CHECK-NEXT:                 std::ptr::write(
// CHECK-NEXT:                     [[SLATE_SLOT_48]],
// CHECK-NEXT:                     (*[[SLATE_SLOT_49]] as u64)
// CHECK-NEXT:                         .wrapping_sub((((2 as i32) as i64) as u64).wrapping_mul(8 as u64))
// CHECK-NEXT:                         as i64,
// CHECK-NEXT:                 );
// CHECK-NEXT:                 srcp2 = *[[SLATE_SLOT_48]];
// CHECK-NEXT:                 std::ptr::write([[SLATE_SLOT_47]], len);
// CHECK-NEXT:                 std::ptr::write(
// CHECK-NEXT:                     [[SLATE_SLOT_46]],
// CHECK-NEXT:                     (*[[SLATE_SLOT_47]]).wrapping_add(((2 as i32) as i64) as u64),
// CHECK-NEXT:                 );
// CHECK-NEXT:                 len = *[[SLATE_SLOT_46]];
// CHECK-NEXT:                 __slate_state = 5;
// CHECK-NEXT:             }
// CHECK-NEXT:             loop {
// CHECK-NEXT:                 match __slate_state {
// CHECK-NEXT:                     5 => {
// CHECK-NEXT:                         *[[SLATE_SLOT_61]] =
// CHECK-NEXT:                             unsafe { *unsafe { (srcp1 as *mut u64).offset((3 as i32) as isize) } };
// CHECK-NEXT:                         *[[SLATE_SLOT_57]] =
// CHECK-NEXT:                             unsafe { *unsafe { (srcp2 as *mut u64).offset((3 as i32) as isize) } };
// CHECK-NEXT:                         *[[SLATE_SLOT_56]] = *[[SLATE_SLOT_63]] >> *[[SLATE_SLOT_55]]
// CHECK-NEXT:                             | *[[SLATE_SLOT_62]] << *[[SLATE_SLOT_54]];
// CHECK-NEXT:                         if *[[SLATE_SLOT_56]] != *[[SLATE_SLOT_58]] {
// CHECK-NEXT:                             return mymemcmp1(*[[SLATE_SLOT_56]], *[[SLATE_SLOT_58]]);
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             std::ptr::write([[SLATE_SLOT_33]], srcp1);
// CHECK-NEXT:                             std::ptr::write(
// CHECK-NEXT:                                 [[SLATE_SLOT_32]],
// CHECK-NEXT:                                 (*[[SLATE_SLOT_33]] as u64).wrapping_add(
// CHECK-NEXT:                                     (((4 as i32) as i64) as u64).wrapping_mul(8 as u64),
// CHECK-NEXT:                                 ) as i64,
// CHECK-NEXT:                             );
// CHECK-NEXT:                             srcp1 = *[[SLATE_SLOT_32]];
// CHECK-NEXT:                             std::ptr::write([[SLATE_SLOT_31]], srcp2);
// CHECK-NEXT:                             std::ptr::write(
// CHECK-NEXT:                                 [[SLATE_SLOT_30]],
// CHECK-NEXT:                                 (*[[SLATE_SLOT_31]] as u64).wrapping_add(
// CHECK-NEXT:                                     (((4 as i32) as i64) as u64).wrapping_mul(8 as u64),
// CHECK-NEXT:                                 ) as i64,
// CHECK-NEXT:                             );
// CHECK-NEXT:                             srcp2 = *[[SLATE_SLOT_30]];
// CHECK-NEXT:                             std::ptr::write([[SLATE_SLOT_29]], len);
// CHECK-NEXT:                             std::ptr::write(
// CHECK-NEXT:                                 [[SLATE_SLOT_28]],
// CHECK-NEXT:                                 (*[[SLATE_SLOT_29]]).wrapping_sub(((4 as i32) as i64) as u64),
// CHECK-NEXT:                             );
// CHECK-NEXT:                             len = *[[SLATE_SLOT_28]];
// CHECK-NEXT:                             if len != (((0 as i32) as i64) as u64) {
// CHECK-NEXT:                                 __slate_state = 11;
// CHECK-NEXT:                             } else {
// CHECK-NEXT:                                 break '[[JOIN_0]];
// CHECK-NEXT:                             }
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                     7 => {
// CHECK-NEXT:                         *[[SLATE_SLOT_62]] =
// CHECK-NEXT:                             unsafe { *unsafe { (srcp1 as *mut u64).offset((2 as i32) as isize) } };
// CHECK-NEXT:                         *[[SLATE_SLOT_58]] =
// CHECK-NEXT:                             unsafe { *unsafe { (srcp2 as *mut u64).offset((2 as i32) as isize) } };
// CHECK-NEXT:                         *[[SLATE_SLOT_56]] = *[[SLATE_SLOT_64]] >> *[[SLATE_SLOT_55]]
// CHECK-NEXT:                             | *[[SLATE_SLOT_63]] << *[[SLATE_SLOT_54]];
// CHECK-NEXT:                         if *[[SLATE_SLOT_56]] != *[[SLATE_SLOT_59]] {
// CHECK-NEXT:                             return mymemcmp1(*[[SLATE_SLOT_56]], *[[SLATE_SLOT_59]]);
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             __slate_state = 5;
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                     9 => {
// CHECK-NEXT:                         *[[SLATE_SLOT_63]] =
// CHECK-NEXT:                             unsafe { *unsafe { (srcp1 as *mut u64).offset((1 as i32) as isize) } };
// CHECK-NEXT:                         *[[SLATE_SLOT_59]] =
// CHECK-NEXT:                             unsafe { *unsafe { (srcp2 as *mut u64).offset((1 as i32) as isize) } };
// CHECK-NEXT:                         *[[SLATE_SLOT_56]] = *[[SLATE_SLOT_61]] >> *[[SLATE_SLOT_55]]
// CHECK-NEXT:                             | *[[SLATE_SLOT_64]] << *[[SLATE_SLOT_54]];
// CHECK-NEXT:                         if *[[SLATE_SLOT_56]] != *[[SLATE_SLOT_60]] {
// CHECK-NEXT:                             return mymemcmp1(*[[SLATE_SLOT_56]], *[[SLATE_SLOT_60]]);
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             __slate_state = 7;
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                     _ => {
// CHECK-NEXT:                         *[[SLATE_SLOT_64]] =
// CHECK-NEXT:                             unsafe { *unsafe { (srcp1 as *mut u64).offset((0 as i32) as isize) } };
// CHECK-NEXT:                         *[[SLATE_SLOT_60]] =
// CHECK-NEXT:                             unsafe { *unsafe { (srcp2 as *mut u64).offset((0 as i32) as isize) } };
// CHECK-NEXT:                         *[[SLATE_SLOT_56]] = *[[SLATE_SLOT_62]] >> *[[SLATE_SLOT_55]]
// CHECK-NEXT:                             | *[[SLATE_SLOT_61]] << *[[SLATE_SLOT_54]];
// CHECK-NEXT:                         if *[[SLATE_SLOT_56]] != *[[SLATE_SLOT_57]] {
// CHECK-NEXT:                             break;
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             __slate_state = 9;
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             return mymemcmp1(*[[SLATE_SLOT_56]], *[[SLATE_SLOT_57]]);
// CHECK-NEXT:         }
// CHECK-NEXT:         *[[SLATE_SLOT_56]] =
// CHECK-NEXT:             *[[SLATE_SLOT_62]] >> *[[SLATE_SLOT_55]] | *[[SLATE_SLOT_61]] << *[[SLATE_SLOT_54]];
// CHECK-NEXT:         if *[[SLATE_SLOT_56]] != *[[SLATE_SLOT_57]] {
// CHECK-NEXT:             return mymemcmp1(*[[SLATE_SLOT_56]], *[[SLATE_SLOT_57]]);
// CHECK-NEXT:         } else {
// CHECK-NEXT:             return 0 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn mymemcmp(mut s1: *const (), mut s2: *const (), mut len: u64) -> i32 {
// CHECK-NEXT:     let mut srcp1: i64 = s1 as i64;
// CHECK-NEXT:     let mut srcp2: i64 = s2 as i64;
// CHECK-NEXT:     if (srcp1 as u64) % (8 as u64) == (((0 as i32) as i64) as u64) {
// CHECK-NEXT:         return mymemcmp2(srcp1, srcp2, len / (8 as u64));
// CHECK-NEXT:     } else {
// CHECK-NEXT:         return mymemcmp3(srcp1, srcp2, len / (8 as u64));
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut buf: __SlateAlign16<[i8; 256]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut p: *mut i8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut u: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     // The test above assumes little endian and long being the same size
// CHECK-NEXT:     // as pointer.
// CHECK-NEXT:     if (8 as u64) != (8 as u64) || (8 as u64) < (((4 as i32) as i64) as u64) {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         u.l = 305419896 as i64;
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((unsafe { *unsafe { unsafe { u.c.as_mut_ptr() as *mut i8 }.offset((0 as i32) as isize) } })
// CHECK-NEXT:         as i32)
// CHECK-NEXT:         != (120 as i32)
// CHECK-NEXT:         || ((unsafe {
// CHECK-NEXT:             *unsafe { unsafe { u.c.as_mut_ptr() as *mut i8 }.offset((1 as i32) as isize) }
// CHECK-NEXT:         }) as i32)
// CHECK-NEXT:             != (86 as i32)
// CHECK-NEXT:         || ((unsafe {
// CHECK-NEXT:             *unsafe { unsafe { u.c.as_mut_ptr() as *mut i8 }.offset((2 as i32) as isize) }
// CHECK-NEXT:         }) as i32)
// CHECK-NEXT:             != (52 as i32)
// CHECK-NEXT:         || ((unsafe {
// CHECK-NEXT:             *unsafe { unsafe { u.c.as_mut_ptr() as *mut i8 }.offset((3 as i32) as isize) }
// CHECK-NEXT:         }) as i32)
// CHECK-NEXT:             != (18 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     p = unsafe {
// CHECK-NEXT:         unsafe { unsafe { std::ptr::addr_of_mut!(buf.0) as *mut i8 }.offset((16 as i32) as isize) }
// CHECK-NEXT:             .offset(
// CHECK-NEXT:                 -((((unsafe { std::ptr::addr_of_mut!(buf.0) as *mut i8 }) as i64)
// CHECK-NEXT:                     & ((15 as i32) as i64)) as isize),
// CHECK-NEXT:             )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memcpy(
// CHECK-NEXT:             (unsafe { p.offset((9 as i32) as isize) }) as *mut (),
// CHECK-NEXT:             b"\x017\x82\xa7UI\x9d\xbf\xf8D\xb6U\x17\x8e\xf9\0".as_ptr() as *const (),
// CHECK-NEXT:             ((15 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memcpy(
// CHECK-NEXT:             (unsafe { unsafe { p.offset((128 as i32) as isize) }.offset((24 as i32) as isize) })
// CHECK-NEXT:                 as *mut (),
// CHECK-NEXT:             b"\x017\x82\xa7UI\xd0\xf3\xb7*m#qIj\0".as_ptr() as *const (),
// CHECK-NEXT:             ((15 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     if mymemcmp(
// CHECK-NEXT:         (unsafe { p.offset((9 as i32) as isize) }) as *const (),
// CHECK-NEXT:         (unsafe { unsafe { p.offset((128 as i32) as isize) }.offset((24 as i32) as isize) })
// CHECK-NEXT:             as *const (),
// CHECK-NEXT:         ((33 as i32) as i64) as u64,
// CHECK-NEXT:     ) != -(51 as i32)
// CHECK-NEXT:     {
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
