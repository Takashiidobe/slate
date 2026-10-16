#include <limits.h>

extern void abort(void);
extern void exit(int);

#if __LONG_LONG_MAX__ == 9223372036854775807LL
#define BITS 64

static unsigned long long const zext[64] = {0x87654321fedcba90ULL,
                                            0x43b2a190ff6e5d48ULL,
                                            0x21d950c87fb72ea4ULL,
                                            0x10eca8643fdb9752ULL,
                                            0x87654321fedcba9ULL,
                                            0x43b2a190ff6e5d4ULL,
                                            0x21d950c87fb72eaULL,
                                            0x10eca8643fdb975ULL,
                                            0x87654321fedcbaULL,
                                            0x43b2a190ff6e5dULL,
                                            0x21d950c87fb72eULL,
                                            0x10eca8643fdb97ULL,
                                            0x87654321fedcbULL,
                                            0x43b2a190ff6e5ULL,
                                            0x21d950c87fb72ULL,
                                            0x10eca8643fdb9ULL,
                                            0x87654321fedcULL,
                                            0x43b2a190ff6eULL,
                                            0x21d950c87fb7ULL,
                                            0x10eca8643fdbULL,
                                            0x87654321fedULL,
                                            0x43b2a190ff6ULL,
                                            0x21d950c87fbULL,
                                            0x10eca8643fdULL,
                                            0x87654321feULL,
                                            0x43b2a190ffULL,
                                            0x21d950c87fULL,
                                            0x10eca8643fULL,
                                            0x87654321fULL,
                                            0x43b2a190fULL,
                                            0x21d950c87ULL,
                                            0x10eca8643ULL,
                                            0x87654321ULL,
                                            0x43b2a190ULL,
                                            0x21d950c8ULL,
                                            0x10eca864ULL,
                                            0x8765432ULL,
                                            0x43b2a19ULL,
                                            0x21d950cULL,
                                            0x10eca86ULL,
                                            0x876543ULL,
                                            0x43b2a1ULL,
                                            0x21d950ULL,
                                            0x10eca8ULL,
                                            0x87654ULL,
                                            0x43b2aULL,
                                            0x21d95ULL,
                                            0x10ecaULL,
                                            0x8765ULL,
                                            0x43b2ULL,
                                            0x21d9ULL,
                                            0x10ecULL,
                                            0x876ULL,
                                            0x43bULL,
                                            0x21dULL,
                                            0x10eULL,
                                            0x87ULL,
                                            0x43ULL,
                                            0x21ULL,
                                            0x10ULL,
                                            0x8ULL,
                                            0x4ULL,
                                            0x2ULL,
                                            0x1ULL};

#elif __LONG_LONG_MAX__ == 2147483647LL
#define BITS 32

static unsigned long long const zext[32] = {
    0x87654321ULL, 0x43b2a190ULL, 0x21d950c8ULL, 0x10eca864ULL, 0x8765432ULL,
    0x43b2a19ULL,  0x21d950cULL,  0x10eca86ULL,  0x876543ULL,   0x43b2a1ULL,
    0x21d950ULL,   0x10eca8ULL,   0x87654ULL,    0x43b2aULL,    0x21d95ULL,
    0x10ecaULL,    0x8765ULL,     0x43b2ULL,     0x21d9ULL,     0x10ecULL,
    0x876ULL,      0x43bULL,      0x21dULL,      0x10eULL,      0x87ULL,
    0x43ULL,       0x21ULL,       0x10ULL,       0x8ULL,        0x4ULL,
    0x2ULL,        0x1ULL,
};

#else
#error "Update the test case."
#endif

static unsigned long long variable_shift(unsigned long long x, int i) {
  return x >> i;
}

static unsigned long long constant_shift(unsigned long long x, int i) {
  switch (i) {
  case 0:
    x = x >> 0;
    break;
  case 1:
    x = x >> 1;
    break;
  case 2:
    x = x >> 2;
    break;
  case 3:
    x = x >> 3;
    break;
  case 4:
    x = x >> 4;
    break;
  case 5:
    x = x >> 5;
    break;
  case 6:
    x = x >> 6;
    break;
  case 7:
    x = x >> 7;
    break;
  case 8:
    x = x >> 8;
    break;
  case 9:
    x = x >> 9;
    break;
  case 10:
    x = x >> 10;
    break;
  case 11:
    x = x >> 11;
    break;
  case 12:
    x = x >> 12;
    break;
  case 13:
    x = x >> 13;
    break;
  case 14:
    x = x >> 14;
    break;
  case 15:
    x = x >> 15;
    break;
  case 16:
    x = x >> 16;
    break;
  case 17:
    x = x >> 17;
    break;
  case 18:
    x = x >> 18;
    break;
  case 19:
    x = x >> 19;
    break;
  case 20:
    x = x >> 20;
    break;
  case 21:
    x = x >> 21;
    break;
  case 22:
    x = x >> 22;
    break;
  case 23:
    x = x >> 23;
    break;
  case 24:
    x = x >> 24;
    break;
  case 25:
    x = x >> 25;
    break;
  case 26:
    x = x >> 26;
    break;
  case 27:
    x = x >> 27;
    break;
  case 28:
    x = x >> 28;
    break;
  case 29:
    x = x >> 29;
    break;
  case 30:
    x = x >> 30;
    break;
  case 31:
    x = x >> 31;
    break;
#if BITS > 32
  case 32:
    x = x >> 32;
    break;
  case 33:
    x = x >> 33;
    break;
  case 34:
    x = x >> 34;
    break;
  case 35:
    x = x >> 35;
    break;
  case 36:
    x = x >> 36;
    break;
  case 37:
    x = x >> 37;
    break;
  case 38:
    x = x >> 38;
    break;
  case 39:
    x = x >> 39;
    break;
  case 40:
    x = x >> 40;
    break;
  case 41:
    x = x >> 41;
    break;
  case 42:
    x = x >> 42;
    break;
  case 43:
    x = x >> 43;
    break;
  case 44:
    x = x >> 44;
    break;
  case 45:
    x = x >> 45;
    break;
  case 46:
    x = x >> 46;
    break;
  case 47:
    x = x >> 47;
    break;
  case 48:
    x = x >> 48;
    break;
  case 49:
    x = x >> 49;
    break;
  case 50:
    x = x >> 50;
    break;
  case 51:
    x = x >> 51;
    break;
  case 52:
    x = x >> 52;
    break;
  case 53:
    x = x >> 53;
    break;
  case 54:
    x = x >> 54;
    break;
  case 55:
    x = x >> 55;
    break;
  case 56:
    x = x >> 56;
    break;
  case 57:
    x = x >> 57;
    break;
  case 58:
    x = x >> 58;
    break;
  case 59:
    x = x >> 59;
    break;
  case 60:
    x = x >> 60;
    break;
  case 61:
    x = x >> 61;
    break;
  case 62:
    x = x >> 62;
    break;
  case 63:
    x = x >> 63;
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
    unsigned long long y = variable_shift(zext[0], i);
    if (y != zext[i])
      abort();
  }
  for (i = 0; i < BITS; ++i) {
    unsigned long long y = constant_shift(zext[0], i);
    if (y != zext[i])
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
// CHECK-NEXT: static mut zext: __SlateAlign16<[u64; 64]> = __SlateAlign16([
// CHECK-NEXT:     9756277981056907920 as u64,
// CHECK-NEXT:     4878138990528453960 as u64,
// CHECK-NEXT:     2439069495264226980 as u64,
// CHECK-NEXT:     1219534747632113490 as u64,
// CHECK-NEXT:     609767373816056745 as u64,
// CHECK-NEXT:     304883686908028372 as u64,
// CHECK-NEXT:     152441843454014186 as u64,
// CHECK-NEXT:     76220921727007093 as u64,
// CHECK-NEXT:     38110460863503546 as u64,
// CHECK-NEXT:     19055230431751773 as u64,
// CHECK-NEXT:     9527615215875886 as u64,
// CHECK-NEXT:     4763807607937943 as u64,
// CHECK-NEXT:     2381903803968971 as u64,
// CHECK-NEXT:     1190951901984485 as u64,
// CHECK-NEXT:     595475950992242 as u64,
// CHECK-NEXT:     297737975496121 as u64,
// CHECK-NEXT:     148868987748060 as u64,
// CHECK-NEXT:     74434493874030 as u64,
// CHECK-NEXT:     37217246937015 as u64,
// CHECK-NEXT:     18608623468507 as u64,
// CHECK-NEXT:     9304311734253 as u64,
// CHECK-NEXT:     4652155867126 as u64,
// CHECK-NEXT:     2326077933563 as u64,
// CHECK-NEXT:     1163038966781 as u64,
// CHECK-NEXT:     581519483390 as u64,
// CHECK-NEXT:     290759741695 as u64,
// CHECK-NEXT:     145379870847 as u64,
// CHECK-NEXT:     72689935423 as u64,
// CHECK-NEXT:     36344967711 as u64,
// CHECK-NEXT:     18172483855 as u64,
// CHECK-NEXT:     9086241927 as u64,
// CHECK-NEXT:     4543120963 as u64,
// CHECK-NEXT:     2271560481 as u64,
// CHECK-NEXT:     1135780240 as u64,
// CHECK-NEXT:     567890120 as u64,
// CHECK-NEXT:     283945060 as u64,
// CHECK-NEXT:     141972530 as u64,
// CHECK-NEXT:     70986265 as u64,
// CHECK-NEXT:     35493132 as u64,
// CHECK-NEXT:     17746566 as u64,
// CHECK-NEXT:     8873283 as u64,
// CHECK-NEXT:     4436641 as u64,
// CHECK-NEXT:     2218320 as u64,
// CHECK-NEXT:     1109160 as u64,
// CHECK-NEXT:     554580 as u64,
// CHECK-NEXT:     277290 as u64,
// CHECK-NEXT:     138645 as u64,
// CHECK-NEXT:     69322 as u64,
// CHECK-NEXT:     34661 as u64,
// CHECK-NEXT:     17330 as u64,
// CHECK-NEXT:     8665 as u64,
// CHECK-NEXT:     4332 as u64,
// CHECK-NEXT:     2166 as u64,
// CHECK-NEXT:     1083 as u64,
// CHECK-NEXT:     541 as u64,
// CHECK-NEXT:     270 as u64,
// CHECK-NEXT:     135 as u64,
// CHECK-NEXT:     67 as u64,
// CHECK-NEXT:     33 as u64,
// CHECK-NEXT:     16 as u64,
// CHECK-NEXT:     8 as u64,
// CHECK-NEXT:     4 as u64,
// CHECK-NEXT:     2 as u64,
// CHECK-NEXT:     1 as u64,
// CHECK-NEXT: ]);
// CHECK-NEXT: fn variable_shift(mut x: u64, mut i: i32) -> u64 {
// CHECK-NEXT:     x >> i
// CHECK-NEXT: }
// CHECK-NEXT: fn constant_shift(mut x: u64, mut i: i32) -> u64 {
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: {
// CHECK-NEXT:         match i {
// CHECK-NEXT:             0 => {
// CHECK-NEXT:                 x >>= 0 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             1 => {
// CHECK-NEXT:                 x >>= 1 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             2 => {
// CHECK-NEXT:                 x >>= 2 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             3 => {
// CHECK-NEXT:                 x >>= 3 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             4 => {
// CHECK-NEXT:                 x >>= 4 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             5 => {
// CHECK-NEXT:                 x >>= 5 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             6 => {
// CHECK-NEXT:                 x >>= 6 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             7 => {
// CHECK-NEXT:                 x >>= 7 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             8 => {
// CHECK-NEXT:                 x >>= 8 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             9 => {
// CHECK-NEXT:                 x >>= 9 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             10 => {
// CHECK-NEXT:                 x >>= 10 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             11 => {
// CHECK-NEXT:                 x >>= 11 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             12 => {
// CHECK-NEXT:                 x >>= 12 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             13 => {
// CHECK-NEXT:                 x >>= 13 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             14 => {
// CHECK-NEXT:                 x >>= 14 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             15 => {
// CHECK-NEXT:                 x >>= 15 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             16 => {
// CHECK-NEXT:                 x >>= 16 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             17 => {
// CHECK-NEXT:                 x >>= 17 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             18 => {
// CHECK-NEXT:                 x >>= 18 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             19 => {
// CHECK-NEXT:                 x >>= 19 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             20 => {
// CHECK-NEXT:                 x >>= 20 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             21 => {
// CHECK-NEXT:                 x >>= 21 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             22 => {
// CHECK-NEXT:                 x >>= 22 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             23 => {
// CHECK-NEXT:                 x >>= 23 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             24 => {
// CHECK-NEXT:                 x >>= 24 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             25 => {
// CHECK-NEXT:                 x >>= 25 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             26 => {
// CHECK-NEXT:                 x >>= 26 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             27 => {
// CHECK-NEXT:                 x >>= 27 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             28 => {
// CHECK-NEXT:                 x >>= 28 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             29 => {
// CHECK-NEXT:                 x >>= 29 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             30 => {
// CHECK-NEXT:                 x >>= 30 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             31 => {
// CHECK-NEXT:                 x >>= 31 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             32 => {
// CHECK-NEXT:                 x >>= 32 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             33 => {
// CHECK-NEXT:                 x >>= 33 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             34 => {
// CHECK-NEXT:                 x >>= 34 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             35 => {
// CHECK-NEXT:                 x >>= 35 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             36 => {
// CHECK-NEXT:                 x >>= 36 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             37 => {
// CHECK-NEXT:                 x >>= 37 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             38 => {
// CHECK-NEXT:                 x >>= 38 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             39 => {
// CHECK-NEXT:                 x >>= 39 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             40 => {
// CHECK-NEXT:                 x >>= 40 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             41 => {
// CHECK-NEXT:                 x >>= 41 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             42 => {
// CHECK-NEXT:                 x >>= 42 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             43 => {
// CHECK-NEXT:                 x >>= 43 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             44 => {
// CHECK-NEXT:                 x >>= 44 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             45 => {
// CHECK-NEXT:                 x >>= 45 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             46 => {
// CHECK-NEXT:                 x >>= 46 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             47 => {
// CHECK-NEXT:                 x >>= 47 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             48 => {
// CHECK-NEXT:                 x >>= 48 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             49 => {
// CHECK-NEXT:                 x >>= 49 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             50 => {
// CHECK-NEXT:                 x >>= 50 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             51 => {
// CHECK-NEXT:                 x >>= 51 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             52 => {
// CHECK-NEXT:                 x >>= 52 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             53 => {
// CHECK-NEXT:                 x >>= 53 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             54 => {
// CHECK-NEXT:                 x >>= 54 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             55 => {
// CHECK-NEXT:                 x >>= 55 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             56 => {
// CHECK-NEXT:                 x >>= 56 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             57 => {
// CHECK-NEXT:                 x >>= 57 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             58 => {
// CHECK-NEXT:                 x >>= 58 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             59 => {
// CHECK-NEXT:                 x >>= 59 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             60 => {
// CHECK-NEXT:                 x >>= 60 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             61 => {
// CHECK-NEXT:                 x >>= 61 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             62 => {
// CHECK-NEXT:                 x >>= 62 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             63 => {
// CHECK-NEXT:                 x >>= 63 as i32;
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
// CHECK-NEXT:                     unsafe { std::ptr::addr_of!(zext.0) as *const u64 }
// CHECK-NEXT:                         .offset((0 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:             },
// CHECK-NEXT:             i,
// CHECK-NEXT:         );
// CHECK-NEXT:         if y
// CHECK-NEXT:             != unsafe {
// CHECK-NEXT:                 *unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of!(zext.0) as *const u64 }
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
// CHECK-NEXT:                     unsafe { std::ptr::addr_of!(zext.0) as *const u64 }
// CHECK-NEXT:                         .offset((0 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:             },
// CHECK-NEXT:             i,
// CHECK-NEXT:         );
// CHECK-NEXT:         if y
// CHECK-NEXT:             != unsafe {
// CHECK-NEXT:                 *unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of!(zext.0) as *const u64 }
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
