#include <limits.h>

extern void abort(void);
extern void exit(int);

#if __LONG_LONG_MAX__ == 9223372036854775807LL
#define BITS 64

static unsigned long long const data[64] = {
    0x123456789abcdefULL,  0x2468acf13579bdeULL,  0x48d159e26af37bcULL,
    0x91a2b3c4d5e6f78ULL,  0x123456789abcdef0ULL, 0x2468acf13579bde0ULL,
    0x48d159e26af37bc0ULL, 0x91a2b3c4d5e6f780ULL, 0x23456789abcdef00ULL,
    0x468acf13579bde00ULL, 0x8d159e26af37bc00ULL, 0x1a2b3c4d5e6f7800ULL,
    0x3456789abcdef000ULL, 0x68acf13579bde000ULL, 0xd159e26af37bc000ULL,
    0xa2b3c4d5e6f78000ULL, 0x456789abcdef0000ULL, 0x8acf13579bde0000ULL,
    0x159e26af37bc0000ULL, 0x2b3c4d5e6f780000ULL, 0x56789abcdef00000ULL,
    0xacf13579bde00000ULL, 0x59e26af37bc00000ULL, 0xb3c4d5e6f7800000ULL,
    0x6789abcdef000000ULL, 0xcf13579bde000000ULL, 0x9e26af37bc000000ULL,
    0x3c4d5e6f78000000ULL, 0x789abcdef0000000ULL, 0xf13579bde0000000ULL,
    0xe26af37bc0000000ULL, 0xc4d5e6f780000000ULL, 0x89abcdef00000000ULL,
    0x13579bde00000000ULL, 0x26af37bc00000000ULL, 0x4d5e6f7800000000ULL,
    0x9abcdef000000000ULL, 0x3579bde000000000ULL, 0x6af37bc000000000ULL,
    0xd5e6f78000000000ULL, 0xabcdef0000000000ULL, 0x579bde0000000000ULL,
    0xaf37bc0000000000ULL, 0x5e6f780000000000ULL, 0xbcdef00000000000ULL,
    0x79bde00000000000ULL, 0xf37bc00000000000ULL, 0xe6f7800000000000ULL,
    0xcdef000000000000ULL, 0x9bde000000000000ULL, 0x37bc000000000000ULL,
    0x6f78000000000000ULL, 0xdef0000000000000ULL, 0xbde0000000000000ULL,
    0x7bc0000000000000ULL, 0xf780000000000000ULL, 0xef00000000000000ULL,
    0xde00000000000000ULL, 0xbc00000000000000ULL, 0x7800000000000000ULL,
    0xf000000000000000ULL, 0xe000000000000000ULL, 0xc000000000000000ULL,
    0x8000000000000000ULL};

#elif __LONG_LONG_MAX__ == 2147483647LL
#define BITS 32

static unsigned long long const data[32] = {
    0x1234567fULL, 0x2468acfeULL, 0x48d159fcULL, 0x91a2b3f8ULL, 0x234567f0ULL,
    0x468acfe0ULL, 0x8d159fc0ULL, 0x1a2b3f80ULL, 0x34567f00ULL, 0x68acfe00ULL,
    0xd159fc00ULL, 0xa2b3f800ULL, 0x4567f000ULL, 0x8acfe000ULL, 0x159fc000ULL,
    0x2b3f8000ULL, 0x567f0000ULL, 0xacfe0000ULL, 0x59fc0000ULL, 0xb3f80000ULL,
    0x67f00000ULL, 0xcfe00000ULL, 0x9fc00000ULL, 0x3f800000ULL, 0x7f000000ULL,
    0xfe000000ULL, 0xfc000000ULL, 0xf8000000ULL, 0xf0000000ULL, 0xe0000000ULL,
    0xc0000000ULL, 0x80000000ULL};

#else
#error "Update the test case."
#endif

static unsigned long long variable_shift(unsigned long long x, int i) {
  return x << i;
}

static unsigned long long constant_shift(unsigned long long x, int i) {
  switch (i) {
  case 0:
    x = x << 0;
    break;
  case 1:
    x = x << 1;
    break;
  case 2:
    x = x << 2;
    break;
  case 3:
    x = x << 3;
    break;
  case 4:
    x = x << 4;
    break;
  case 5:
    x = x << 5;
    break;
  case 6:
    x = x << 6;
    break;
  case 7:
    x = x << 7;
    break;
  case 8:
    x = x << 8;
    break;
  case 9:
    x = x << 9;
    break;
  case 10:
    x = x << 10;
    break;
  case 11:
    x = x << 11;
    break;
  case 12:
    x = x << 12;
    break;
  case 13:
    x = x << 13;
    break;
  case 14:
    x = x << 14;
    break;
  case 15:
    x = x << 15;
    break;
  case 16:
    x = x << 16;
    break;
  case 17:
    x = x << 17;
    break;
  case 18:
    x = x << 18;
    break;
  case 19:
    x = x << 19;
    break;
  case 20:
    x = x << 20;
    break;
  case 21:
    x = x << 21;
    break;
  case 22:
    x = x << 22;
    break;
  case 23:
    x = x << 23;
    break;
  case 24:
    x = x << 24;
    break;
  case 25:
    x = x << 25;
    break;
  case 26:
    x = x << 26;
    break;
  case 27:
    x = x << 27;
    break;
  case 28:
    x = x << 28;
    break;
  case 29:
    x = x << 29;
    break;
  case 30:
    x = x << 30;
    break;
  case 31:
    x = x << 31;
    break;
#if BITS > 32
  case 32:
    x = x << 32;
    break;
  case 33:
    x = x << 33;
    break;
  case 34:
    x = x << 34;
    break;
  case 35:
    x = x << 35;
    break;
  case 36:
    x = x << 36;
    break;
  case 37:
    x = x << 37;
    break;
  case 38:
    x = x << 38;
    break;
  case 39:
    x = x << 39;
    break;
  case 40:
    x = x << 40;
    break;
  case 41:
    x = x << 41;
    break;
  case 42:
    x = x << 42;
    break;
  case 43:
    x = x << 43;
    break;
  case 44:
    x = x << 44;
    break;
  case 45:
    x = x << 45;
    break;
  case 46:
    x = x << 46;
    break;
  case 47:
    x = x << 47;
    break;
  case 48:
    x = x << 48;
    break;
  case 49:
    x = x << 49;
    break;
  case 50:
    x = x << 50;
    break;
  case 51:
    x = x << 51;
    break;
  case 52:
    x = x << 52;
    break;
  case 53:
    x = x << 53;
    break;
  case 54:
    x = x << 54;
    break;
  case 55:
    x = x << 55;
    break;
  case 56:
    x = x << 56;
    break;
  case 57:
    x = x << 57;
    break;
  case 58:
    x = x << 58;
    break;
  case 59:
    x = x << 59;
    break;
  case 60:
    x = x << 60;
    break;
  case 61:
    x = x << 61;
    break;
  case 62:
    x = x << 62;
    break;
  case 63:
    x = x << 63;
    break;
#endif

  default:
    abort();
  }
  return x;
}

int main() {
  int i;

  for (i = 0; i < BITS; ++i) {
    unsigned long long y = variable_shift(data[0], i);
    if (y != data[i])
      abort();
  }
  for (i = 0; i < BITS; ++i) {
    unsigned long long y = constant_shift(data[0], i);
    if (y != data[i])
      abort();
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
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut data: __SlateAlign16<[u64; 64]> = __SlateAlign16([
// CHECK-NEXT:     81985529216486895 as u64,
// CHECK-NEXT:     163971058432973790 as u64,
// CHECK-NEXT:     327942116865947580 as u64,
// CHECK-NEXT:     655884233731895160 as u64,
// CHECK-NEXT:     1311768467463790320 as u64,
// CHECK-NEXT:     2623536934927580640 as u64,
// CHECK-NEXT:     5247073869855161280 as u64,
// CHECK-NEXT:     10494147739710322560 as u64,
// CHECK-NEXT:     2541551405711093504 as u64,
// CHECK-NEXT:     5083102811422187008 as u64,
// CHECK-NEXT:     10166205622844374016 as u64,
// CHECK-NEXT:     1885667171979196416 as u64,
// CHECK-NEXT:     3771334343958392832 as u64,
// CHECK-NEXT:     7542668687916785664 as u64,
// CHECK-NEXT:     15085337375833571328 as u64,
// CHECK-NEXT:     11723930677957591040 as u64,
// CHECK-NEXT:     5001117282205630464 as u64,
// CHECK-NEXT:     10002234564411260928 as u64,
// CHECK-NEXT:     1557725055112970240 as u64,
// CHECK-NEXT:     3115450110225940480 as u64,
// CHECK-NEXT:     6230900220451880960 as u64,
// CHECK-NEXT:     12461800440903761920 as u64,
// CHECK-NEXT:     6476856808097972224 as u64,
// CHECK-NEXT:     12953713616195944448 as u64,
// CHECK-NEXT:     7460683158682337280 as u64,
// CHECK-NEXT:     14921366317364674560 as u64,
// CHECK-NEXT:     11395988561019797504 as u64,
// CHECK-NEXT:     4345233048330043392 as u64,
// CHECK-NEXT:     8690466096660086784 as u64,
// CHECK-NEXT:     17380932193320173568 as u64,
// CHECK-NEXT:     16315120312930795520 as u64,
// CHECK-NEXT:     14183496552152039424 as u64,
// CHECK-NEXT:     9920249030594527232 as u64,
// CHECK-NEXT:     1393753987479502848 as u64,
// CHECK-NEXT:     2787507974959005696 as u64,
// CHECK-NEXT:     5575015949918011392 as u64,
// CHECK-NEXT:     11150031899836022784 as u64,
// CHECK-NEXT:     3853319725962493952 as u64,
// CHECK-NEXT:     7706639451924987904 as u64,
// CHECK-NEXT:     15413278903849975808 as u64,
// CHECK-NEXT:     12379813733990400000 as u64,
// CHECK-NEXT:     6312883394271248384 as u64,
// CHECK-NEXT:     12625766788542496768 as u64,
// CHECK-NEXT:     6804789503375441920 as u64,
// CHECK-NEXT:     13609579006750883840 as u64,
// CHECK-NEXT:     8772413939792216064 as u64,
// CHECK-NEXT:     17544827879584432128 as u64,
// CHECK-NEXT:     16642911685459312640 as u64,
// CHECK-NEXT:     14839079297209073664 as u64,
// CHECK-NEXT:     11231414520708595712 as u64,
// CHECK-NEXT:     4016084967707639808 as u64,
// CHECK-NEXT:     8032169935415279616 as u64,
// CHECK-NEXT:     16064339870830559232 as u64,
// CHECK-NEXT:     13681935667951566848 as u64,
// CHECK-NEXT:     8917127262193582080 as u64,
// CHECK-NEXT:     17834254524387164160 as u64,
// CHECK-NEXT:     17221764975064776704 as u64,
// CHECK-NEXT:     15996785876420001792 as u64,
// CHECK-NEXT:     13546827679130451968 as u64,
// CHECK-NEXT:     8646911284551352320 as u64,
// CHECK-NEXT:     17293822569102704640 as u64,
// CHECK-NEXT:     16140901064495857664 as u64,
// CHECK-NEXT:     13835058055282163712 as u64,
// CHECK-NEXT:     9223372036854775808 as u64,
// CHECK-NEXT: ]);
// CHECK-NEXT: fn variable_shift(mut x: u64, mut i: i32) -> u64 {
// CHECK-NEXT:     x << i
// CHECK-NEXT: }
// CHECK-NEXT: fn constant_shift(mut x: u64, mut i: i32) -> u64 {
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: {
// CHECK-NEXT:         match i {
// CHECK-NEXT:             0 => {
// CHECK-NEXT:                 x <<= 0 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             1 => {
// CHECK-NEXT:                 x <<= 1 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             2 => {
// CHECK-NEXT:                 x <<= 2 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             3 => {
// CHECK-NEXT:                 x <<= 3 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             4 => {
// CHECK-NEXT:                 x <<= 4 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             5 => {
// CHECK-NEXT:                 x <<= 5 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             6 => {
// CHECK-NEXT:                 x <<= 6 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             7 => {
// CHECK-NEXT:                 x <<= 7 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             8 => {
// CHECK-NEXT:                 x <<= 8 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             9 => {
// CHECK-NEXT:                 x <<= 9 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             10 => {
// CHECK-NEXT:                 x <<= 10 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             11 => {
// CHECK-NEXT:                 x <<= 11 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             12 => {
// CHECK-NEXT:                 x <<= 12 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             13 => {
// CHECK-NEXT:                 x <<= 13 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             14 => {
// CHECK-NEXT:                 x <<= 14 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             15 => {
// CHECK-NEXT:                 x <<= 15 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             16 => {
// CHECK-NEXT:                 x <<= 16 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             17 => {
// CHECK-NEXT:                 x <<= 17 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             18 => {
// CHECK-NEXT:                 x <<= 18 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             19 => {
// CHECK-NEXT:                 x <<= 19 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             20 => {
// CHECK-NEXT:                 x <<= 20 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             21 => {
// CHECK-NEXT:                 x <<= 21 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             22 => {
// CHECK-NEXT:                 x <<= 22 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             23 => {
// CHECK-NEXT:                 x <<= 23 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             24 => {
// CHECK-NEXT:                 x <<= 24 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             25 => {
// CHECK-NEXT:                 x <<= 25 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             26 => {
// CHECK-NEXT:                 x <<= 26 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             27 => {
// CHECK-NEXT:                 x <<= 27 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             28 => {
// CHECK-NEXT:                 x <<= 28 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             29 => {
// CHECK-NEXT:                 x <<= 29 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             30 => {
// CHECK-NEXT:                 x <<= 30 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             31 => {
// CHECK-NEXT:                 x <<= 31 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             32 => {
// CHECK-NEXT:                 x <<= 32 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             33 => {
// CHECK-NEXT:                 x <<= 33 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             34 => {
// CHECK-NEXT:                 x <<= 34 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             35 => {
// CHECK-NEXT:                 x <<= 35 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             36 => {
// CHECK-NEXT:                 x <<= 36 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             37 => {
// CHECK-NEXT:                 x <<= 37 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             38 => {
// CHECK-NEXT:                 x <<= 38 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             39 => {
// CHECK-NEXT:                 x <<= 39 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             40 => {
// CHECK-NEXT:                 x <<= 40 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             41 => {
// CHECK-NEXT:                 x <<= 41 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             42 => {
// CHECK-NEXT:                 x <<= 42 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             43 => {
// CHECK-NEXT:                 x <<= 43 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             44 => {
// CHECK-NEXT:                 x <<= 44 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             45 => {
// CHECK-NEXT:                 x <<= 45 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             46 => {
// CHECK-NEXT:                 x <<= 46 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             47 => {
// CHECK-NEXT:                 x <<= 47 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             48 => {
// CHECK-NEXT:                 x <<= 48 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             49 => {
// CHECK-NEXT:                 x <<= 49 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             50 => {
// CHECK-NEXT:                 x <<= 50 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             51 => {
// CHECK-NEXT:                 x <<= 51 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             52 => {
// CHECK-NEXT:                 x <<= 52 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             53 => {
// CHECK-NEXT:                 x <<= 53 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             54 => {
// CHECK-NEXT:                 x <<= 54 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             55 => {
// CHECK-NEXT:                 x <<= 55 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             56 => {
// CHECK-NEXT:                 x <<= 56 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             57 => {
// CHECK-NEXT:                 x <<= 57 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             58 => {
// CHECK-NEXT:                 x <<= 58 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             59 => {
// CHECK-NEXT:                 x <<= 59 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             60 => {
// CHECK-NEXT:                 x <<= 60 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             61 => {
// CHECK-NEXT:                 x <<= 61 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             62 => {
// CHECK-NEXT:                 x <<= 62 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             63 => {
// CHECK-NEXT:                 x <<= 63 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             _ => {
// CHECK-NEXT:                 unsafe { abort() };
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     x
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while i < (64 as i32) {
// CHECK-NEXT:         let mut y: u64 = variable_shift(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of!(data.0) as *const u64 }
// CHECK-NEXT:                         .offset((0 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:             },
// CHECK-NEXT:             i,
// CHECK-NEXT:         );
// CHECK-NEXT:         if y
// CHECK-NEXT:             != unsafe {
// CHECK-NEXT:                 *unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of!(data.0) as *const u64 }
// CHECK-NEXT:                         .offset(i as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:         i = [[V_2]];
// CHECK-NEXT:     }
// CHECK-NEXT:     i = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while i < (64 as i32) {
// CHECK-NEXT:         let mut y: u64 = constant_shift(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of!(data.0) as *const u64 }
// CHECK-NEXT:                         .offset((0 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:             },
// CHECK-NEXT:             i,
// CHECK-NEXT:         );
// CHECK-NEXT:         if y
// CHECK-NEXT:             != unsafe {
// CHECK-NEXT:                 *unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of!(data.0) as *const u64 }
// CHECK-NEXT:                         .offset(i as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = [[V_3]] + (1 as i32);
// CHECK-NEXT:         i = [[V_4]];
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
