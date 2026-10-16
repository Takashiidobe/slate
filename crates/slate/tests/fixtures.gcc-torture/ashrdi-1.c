#include <limits.h>

extern void abort(void);
extern void exit(int);

#if __LONG_LONG_MAX__ == 9223372036854775807LL
#define BITS 64

static long long const zext[64] = {0x7654321fedcba980LL,
                                   0x3b2a190ff6e5d4c0LL,
                                   0x1d950c87fb72ea60LL,
                                   0xeca8643fdb97530LL,
                                   0x7654321fedcba98LL,
                                   0x3b2a190ff6e5d4cLL,
                                   0x1d950c87fb72ea6LL,
                                   0xeca8643fdb9753LL,
                                   0x7654321fedcba9LL,
                                   0x3b2a190ff6e5d4LL,
                                   0x1d950c87fb72eaLL,
                                   0xeca8643fdb975LL,
                                   0x7654321fedcbaLL,
                                   0x3b2a190ff6e5dLL,
                                   0x1d950c87fb72eLL,
                                   0xeca8643fdb97LL,
                                   0x7654321fedcbLL,
                                   0x3b2a190ff6e5LL,
                                   0x1d950c87fb72LL,
                                   0xeca8643fdb9LL,
                                   0x7654321fedcLL,
                                   0x3b2a190ff6eLL,
                                   0x1d950c87fb7LL,
                                   0xeca8643fdbLL,
                                   0x7654321fedLL,
                                   0x3b2a190ff6LL,
                                   0x1d950c87fbLL,
                                   0xeca8643fdLL,
                                   0x7654321feLL,
                                   0x3b2a190ffLL,
                                   0x1d950c87fLL,
                                   0xeca8643fLL,
                                   0x7654321fLL,
                                   0x3b2a190fLL,
                                   0x1d950c87LL,
                                   0xeca8643LL,
                                   0x7654321LL,
                                   0x3b2a190LL,
                                   0x1d950c8LL,
                                   0xeca864LL,
                                   0x765432LL,
                                   0x3b2a19LL,
                                   0x1d950cLL,
                                   0xeca86LL,
                                   0x76543LL,
                                   0x3b2a1LL,
                                   0x1d950LL,
                                   0xeca8LL,
                                   0x7654LL,
                                   0x3b2aLL,
                                   0x1d95LL,
                                   0xecaLL,
                                   0x765LL,
                                   0x3b2LL,
                                   0x1d9LL,
                                   0xecLL,
                                   0x76LL,
                                   0x3bLL,
                                   0x1dLL,
                                   0xeLL,
                                   0x7LL,
                                   0x3LL,
                                   0x1LL,
                                   0LL};

static long long const sext[64] = {
    0x8edcba9f76543210LL, 0xc76e5d4fbb2a1908LL, 0xe3b72ea7dd950c84LL,
    0xf1db9753eeca8642LL, 0xf8edcba9f7654321LL, 0xfc76e5d4fbb2a190LL,
    0xfe3b72ea7dd950c8LL, 0xff1db9753eeca864LL, 0xff8edcba9f765432LL,
    0xffc76e5d4fbb2a19LL, 0xffe3b72ea7dd950cLL, 0xfff1db9753eeca86LL,
    0xfff8edcba9f76543LL, 0xfffc76e5d4fbb2a1LL, 0xfffe3b72ea7dd950LL,
    0xffff1db9753eeca8LL, 0xffff8edcba9f7654LL, 0xffffc76e5d4fbb2aLL,
    0xffffe3b72ea7dd95LL, 0xfffff1db9753eecaLL, 0xfffff8edcba9f765LL,
    0xfffffc76e5d4fbb2LL, 0xfffffe3b72ea7dd9LL, 0xffffff1db9753eecLL,
    0xffffff8edcba9f76LL, 0xffffffc76e5d4fbbLL, 0xffffffe3b72ea7ddLL,
    0xfffffff1db9753eeLL, 0xfffffff8edcba9f7LL, 0xfffffffc76e5d4fbLL,
    0xfffffffe3b72ea7dLL, 0xffffffff1db9753eLL, 0xffffffff8edcba9fLL,
    0xffffffffc76e5d4fLL, 0xffffffffe3b72ea7LL, 0xfffffffff1db9753LL,
    0xfffffffff8edcba9LL, 0xfffffffffc76e5d4LL, 0xfffffffffe3b72eaLL,
    0xffffffffff1db975LL, 0xffffffffff8edcbaLL, 0xffffffffffc76e5dLL,
    0xffffffffffe3b72eLL, 0xfffffffffff1db97LL, 0xfffffffffff8edcbLL,
    0xfffffffffffc76e5LL, 0xfffffffffffe3b72LL, 0xffffffffffff1db9LL,
    0xffffffffffff8edcLL, 0xffffffffffffc76eLL, 0xffffffffffffe3b7LL,
    0xfffffffffffff1dbLL, 0xfffffffffffff8edLL, 0xfffffffffffffc76LL,
    0xfffffffffffffe3bLL, 0xffffffffffffff1dLL, 0xffffffffffffff8eLL,
    0xffffffffffffffc7LL, 0xffffffffffffffe3LL, 0xfffffffffffffff1LL,
    0xfffffffffffffff8LL, 0xfffffffffffffffcLL, 0xfffffffffffffffeLL,
    0xffffffffffffffffLL};

#elif __LONG_LONG_MAX__ == 2147483647LL
#define BITS 32

static long long const zext[32] = {
    0x76543218LL, 0x3b2a190cLL, 0x1d950c86LL, 0xeca8643LL, 0x7654321LL,
    0x3b2a190LL,  0x1d950c8LL,  0xeca864LL,   0x765432LL,  0x3b2a19LL,
    0x1d950cLL,   0xeca86LL,    0x76543LL,    0x3b2a1LL,   0x1d950LL,
    0xeca8LL,     0x7654LL,     0x3b2aLL,     0x1d95LL,    0xecaLL,
    0x765LL,      0x3b2LL,      0x1d9LL,      0xecLL,      0x76LL,
    0x3bLL,       0x1dLL,       0xeLL,        0x7LL,       0x3LL,
    0x1LL,        0LL};

static long long const sext[64] = {
    0x87654321LL, 0xc3b2a190LL, 0xe1d950c8LL, 0xf0eca864LL, 0xf8765432LL,
    0xfc3b2a19LL, 0xfe1d950cLL, 0xff0eca86LL, 0xff876543LL, 0xffc3b2a1LL,
    0xffe1d950LL, 0xfff0eca8LL, 0xfff87654LL, 0xfffc3b2aLL, 0xfffe1d95LL,
    0xffff0ecaLL, 0xffff8765LL, 0xffffc3b2LL, 0xffffe1d9LL, 0xfffff0ecLL,
    0xfffff876LL, 0xfffffc3bLL, 0xfffffe1dLL, 0xffffff0eLL, 0xffffff87LL,
    0xffffffc3LL, 0xffffffe1LL, 0xfffffff0LL, 0xfffffff8LL, 0xfffffffcLL,
    0xfffffffeLL, 0xffffffffLL};

#else
#error "Update the test case."
#endif

static long long variable_shift(long long x, int i) { return x >> i; }

static long long constant_shift(long long x, int i) {
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
    long long y = variable_shift(zext[0], i);
    if (y != zext[i])
      abort();
  }
  for (i = 0; i < BITS; ++i) {
    long long y = variable_shift(sext[0], i);
    if (y != sext[i])
      abort();
  }
  for (i = 0; i < BITS; ++i) {
    long long y = constant_shift(zext[0], i);
    if (y != zext[i])
      abort();
  }
  for (i = 0; i < BITS; ++i) {
    long long y = constant_shift(sext[0], i);
    if (y != sext[i])
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
// CHECK-NEXT: static mut zext: __SlateAlign16<[i64; 64]> = __SlateAlign16([
// CHECK-NEXT:     8526495107234113920 as i64,
// CHECK-NEXT:     4263247553617056960 as i64,
// CHECK-NEXT:     2131623776808528480 as i64,
// CHECK-NEXT:     1065811888404264240 as i64,
// CHECK-NEXT:     532905944202132120 as i64,
// CHECK-NEXT:     266452972101066060 as i64,
// CHECK-NEXT:     133226486050533030 as i64,
// CHECK-NEXT:     66613243025266515 as i64,
// CHECK-NEXT:     33306621512633257 as i64,
// CHECK-NEXT:     16653310756316628 as i64,
// CHECK-NEXT:     8326655378158314 as i64,
// CHECK-NEXT:     4163327689079157 as i64,
// CHECK-NEXT:     2081663844539578 as i64,
// CHECK-NEXT:     1040831922269789 as i64,
// CHECK-NEXT:     520415961134894 as i64,
// CHECK-NEXT:     260207980567447 as i64,
// CHECK-NEXT:     130103990283723 as i64,
// CHECK-NEXT:     65051995141861 as i64,
// CHECK-NEXT:     32525997570930 as i64,
// CHECK-NEXT:     16262998785465 as i64,
// CHECK-NEXT:     8131499392732 as i64,
// CHECK-NEXT:     4065749696366 as i64,
// CHECK-NEXT:     2032874848183 as i64,
// CHECK-NEXT:     1016437424091 as i64,
// CHECK-NEXT:     508218712045 as i64,
// CHECK-NEXT:     254109356022 as i64,
// CHECK-NEXT:     127054678011 as i64,
// CHECK-NEXT:     63527339005 as i64,
// CHECK-NEXT:     31763669502 as i64,
// CHECK-NEXT:     15881834751 as i64,
// CHECK-NEXT:     7940917375 as i64,
// CHECK-NEXT:     3970458687 as i64,
// CHECK-NEXT:     1985229343 as i64,
// CHECK-NEXT:     992614671 as i64,
// CHECK-NEXT:     496307335 as i64,
// CHECK-NEXT:     248153667 as i64,
// CHECK-NEXT:     124076833 as i64,
// CHECK-NEXT:     62038416 as i64,
// CHECK-NEXT:     31019208 as i64,
// CHECK-NEXT:     15509604 as i64,
// CHECK-NEXT:     7754802 as i64,
// CHECK-NEXT:     3877401 as i64,
// CHECK-NEXT:     1938700 as i64,
// CHECK-NEXT:     969350 as i64,
// CHECK-NEXT:     484675 as i64,
// CHECK-NEXT:     242337 as i64,
// CHECK-NEXT:     121168 as i64,
// CHECK-NEXT:     60584 as i64,
// CHECK-NEXT:     30292 as i64,
// CHECK-NEXT:     15146 as i64,
// CHECK-NEXT:     7573 as i64,
// CHECK-NEXT:     3786 as i64,
// CHECK-NEXT:     1893 as i64,
// CHECK-NEXT:     946 as i64,
// CHECK-NEXT:     473 as i64,
// CHECK-NEXT:     236 as i64,
// CHECK-NEXT:     118 as i64,
// CHECK-NEXT:     59 as i64,
// CHECK-NEXT:     29 as i64,
// CHECK-NEXT:     14 as i64,
// CHECK-NEXT:     7 as i64,
// CHECK-NEXT:     3 as i64,
// CHECK-NEXT:     1 as i64,
// CHECK-NEXT:     0 as i64,
// CHECK-NEXT: ]);
// CHECK-NEXT: static mut sext: __SlateAlign16<[i64; 64]> = __SlateAlign16([
// CHECK-NEXT:     (10294308042309906960 as u64) as i64,
// CHECK-NEXT:     (14370526058009729288 as u64) as i64,
// CHECK-NEXT:     (16408635065859640452 as u64) as i64,
// CHECK-NEXT:     (17427689569784596034 as u64) as i64,
// CHECK-NEXT:     (17937216821747073825 as u64) as i64,
// CHECK-NEXT:     (18191980447728312720 as u64) as i64,
// CHECK-NEXT:     (18319362260718932168 as u64) as i64,
// CHECK-NEXT:     (18383053167214241892 as u64) as i64,
// CHECK-NEXT:     (18414898620461896754 as u64) as i64,
// CHECK-NEXT:     (18430821347085724185 as u64) as i64,
// CHECK-NEXT:     (18438782710397637900 as u64) as i64,
// CHECK-NEXT:     (18442763392053594758 as u64) as i64,
// CHECK-NEXT:     (18444753732881573187 as u64) as i64,
// CHECK-NEXT:     (18445748903295562401 as u64) as i64,
// CHECK-NEXT:     (18446246488502557008 as u64) as i64,
// CHECK-NEXT:     (18446495281106054312 as u64) as i64,
// CHECK-NEXT:     (18446619677407802964 as u64) as i64,
// CHECK-NEXT:     (18446681875558677290 as u64) as i64,
// CHECK-NEXT:     (18446712974634114453 as u64) as i64,
// CHECK-NEXT:     (18446728524171833034 as u64) as i64,
// CHECK-NEXT:     (18446736298940692325 as u64) as i64,
// CHECK-NEXT:     (18446740186325121970 as u64) as i64,
// CHECK-NEXT:     (18446742130017336793 as u64) as i64,
// CHECK-NEXT:     (18446743101863444204 as u64) as i64,
// CHECK-NEXT:     (18446743587786497910 as u64) as i64,
// CHECK-NEXT:     (18446743830748024763 as u64) as i64,
// CHECK-NEXT:     (18446743952228788189 as u64) as i64,
// CHECK-NEXT:     (18446744012969169902 as u64) as i64,
// CHECK-NEXT:     (18446744043339360759 as u64) as i64,
// CHECK-NEXT:     (18446744058524456187 as u64) as i64,
// CHECK-NEXT:     (18446744066117003901 as u64) as i64,
// CHECK-NEXT:     (18446744069913277758 as u64) as i64,
// CHECK-NEXT:     (18446744071811414687 as u64) as i64,
// CHECK-NEXT:     (18446744072760483151 as u64) as i64,
// CHECK-NEXT:     (18446744073235017383 as u64) as i64,
// CHECK-NEXT:     (18446744073472284499 as u64) as i64,
// CHECK-NEXT:     (18446744073590918057 as u64) as i64,
// CHECK-NEXT:     (18446744073650234836 as u64) as i64,
// CHECK-NEXT:     (18446744073679893226 as u64) as i64,
// CHECK-NEXT:     (18446744073694722421 as u64) as i64,
// CHECK-NEXT:     (18446744073702137018 as u64) as i64,
// CHECK-NEXT:     (18446744073705844317 as u64) as i64,
// CHECK-NEXT:     (18446744073707697966 as u64) as i64,
// CHECK-NEXT:     (18446744073708624791 as u64) as i64,
// CHECK-NEXT:     (18446744073709088203 as u64) as i64,
// CHECK-NEXT:     (18446744073709319909 as u64) as i64,
// CHECK-NEXT:     (18446744073709435762 as u64) as i64,
// CHECK-NEXT:     (18446744073709493689 as u64) as i64,
// CHECK-NEXT:     (18446744073709522652 as u64) as i64,
// CHECK-NEXT:     (18446744073709537134 as u64) as i64,
// CHECK-NEXT:     (18446744073709544375 as u64) as i64,
// CHECK-NEXT:     (18446744073709547995 as u64) as i64,
// CHECK-NEXT:     (18446744073709549805 as u64) as i64,
// CHECK-NEXT:     (18446744073709550710 as u64) as i64,
// CHECK-NEXT:     (18446744073709551163 as u64) as i64,
// CHECK-NEXT:     (18446744073709551389 as u64) as i64,
// CHECK-NEXT:     (18446744073709551502 as u64) as i64,
// CHECK-NEXT:     (18446744073709551559 as u64) as i64,
// CHECK-NEXT:     (18446744073709551587 as u64) as i64,
// CHECK-NEXT:     (18446744073709551601 as u64) as i64,
// CHECK-NEXT:     (18446744073709551608 as u64) as i64,
// CHECK-NEXT:     (18446744073709551612 as u64) as i64,
// CHECK-NEXT:     (18446744073709551614 as u64) as i64,
// CHECK-NEXT:     (18446744073709551615 as u64) as i64,
// CHECK-NEXT: ]);
// CHECK-NEXT: fn variable_shift(mut x: i64, mut i: i32) -> i64 {
// CHECK-NEXT:     x >> i
// CHECK-NEXT: }
// CHECK-NEXT: fn constant_shift(mut x: i64, mut i: i32) -> i64 {
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
// CHECK-NEXT:         let mut y: i64 = variable_shift(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of!(zext.0) as *const i64 }
// CHECK-NEXT:                         .offset((0 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:             },
// CHECK-NEXT:             i,
// CHECK-NEXT:         );
// CHECK-NEXT:         if y
// CHECK-NEXT:             != unsafe {
// CHECK-NEXT:                 *unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of!(zext.0) as *const i64 }
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
// CHECK-NEXT:         let mut y: i64 = variable_shift(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of!(sext.0) as *const i64 }
// CHECK-NEXT:                         .offset((0 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:             },
// CHECK-NEXT:             i,
// CHECK-NEXT:         );
// CHECK-NEXT:         if y
// CHECK-NEXT:             != unsafe {
// CHECK-NEXT:                 *unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of!(sext.0) as *const i64 }
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
// CHECK-NEXT:     i = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_3:__slate_break_[0-9]+]]: while i < (64 as i32) {
// CHECK-NEXT:         let mut y: i64 = constant_shift(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of!(zext.0) as *const i64 }
// CHECK-NEXT:                         .offset((0 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:             },
// CHECK-NEXT:             i,
// CHECK-NEXT:         );
// CHECK-NEXT:         if y
// CHECK-NEXT:             != unsafe {
// CHECK-NEXT:                 *unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of!(zext.0) as *const i64 }
// CHECK-NEXT:                         .offset(i as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: i32 = [[V_5]] + (1 as i32);
// CHECK-NEXT:         i = [[V_6]];
// CHECK-NEXT:     }
// CHECK-NEXT:     i = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_4:__slate_break_[0-9]+]]: while i < (64 as i32) {
// CHECK-NEXT:         let mut y: i64 = constant_shift(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of!(sext.0) as *const i64 }
// CHECK-NEXT:                         .offset((0 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:             },
// CHECK-NEXT:             i,
// CHECK-NEXT:         );
// CHECK-NEXT:         if y
// CHECK-NEXT:             != unsafe {
// CHECK-NEXT:                 *unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of!(sext.0) as *const i64 }
// CHECK-NEXT:                         .offset(i as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_7:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_8:__v[0-9]+]]: i32 = [[V_7]] + (1 as i32);
// CHECK-NEXT:         i = [[V_8]];
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
