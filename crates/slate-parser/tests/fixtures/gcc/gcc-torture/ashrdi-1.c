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



// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     global %2 zext: array<i64, 64> [storage=static] [const] = aggregate<array<i64, 64>, zero_fill=false>(index0 = const<i64>(8526495107234113920), index1 = const<i64>(4263247553617056960), index2 = const<i64>(2131623776808528480), index3 = const<i64>(1065811888404264240), index4 = const<i64>(532905944202132120), index5 = const<i64>(266452972101066060), index6 = const<i64>(133226486050533030), index7 = const<i64>(66613243025266515), index8 = const<i64>(33306621512633257), index9 = const<i64>(16653310756316628), index10 = const<i64>(8326655378158314), index11 = const<i64>(4163327689079157), index12 = const<i64>(2081663844539578), index13 = const<i64>(1040831922269789), index14 = const<i64>(520415961134894), index15 = const<i64>(260207980567447), index16 = const<i64>(130103990283723), index17 = const<i64>(65051995141861), index18 = const<i64>(32525997570930), index19 = const<i64>(16262998785465), index20 = const<i64>(8131499392732), index21 = const<i64>(4065749696366), index22 = const<i64>(2032874848183), index23 = const<i64>(1016437424091), index24 = const<i64>(508218712045), index25 = const<i64>(254109356022), index26 = const<i64>(127054678011), index27 = const<i64>(63527339005), index28 = const<i64>(31763669502), index29 = const<i64>(15881834751), index30 = const<i64>(7940917375), index31 = const<i64>(3970458687), index32 = const<i64>(1985229343), index33 = const<i64>(992614671), index34 = const<i64>(496307335), index35 = const<i64>(248153667), index36 = const<i64>(124076833), index37 = const<i64>(62038416), index38 = const<i64>(31019208), index39 = const<i64>(15509604), index40 = const<i64>(7754802), index41 = const<i64>(3877401), index42 = const<i64>(1938700), index43 = const<i64>(969350), index44 = const<i64>(484675), index45 = const<i64>(242337), index46 = const<i64>(121168), index47 = const<i64>(60584), index48 = const<i64>(30292), index49 = const<i64>(15146), index50 = const<i64>(7573), index51 = const<i64>(3786), index52 = const<i64>(1893), index53 = const<i64>(946), index54 = const<i64>(473), index55 = const<i64>(236), index56 = const<i64>(118), index57 = const<i64>(59), index58 = const<i64>(29), index59 = const<i64>(14), index60 = const<i64>(7), index61 = const<i64>(3), index62 = const<i64>(1), index63 = const<i64>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %3 sext: array<i64, 64> [storage=static] [const] = aggregate<array<i64, 64>, zero_fill=false>(index0 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(10294308042309906960)), index1 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(14370526058009729288)), index2 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(16408635065859640452)), index3 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(17427689569784596034)), index4 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(17937216821747073825)), index5 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18191980447728312720)), index6 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18319362260718932168)), index7 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18383053167214241892)), index8 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18414898620461896754)), index9 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18430821347085724185)), index10 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18438782710397637900)), index11 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18442763392053594758)), index12 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18444753732881573187)), index13 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18445748903295562401)), index14 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18446246488502557008)), index15 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18446495281106054312)), index16 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18446619677407802964)), index17 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18446681875558677290)), index18 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18446712974634114453)), index19 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18446728524171833034)), index20 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18446736298940692325)), index21 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18446740186325121970)), index22 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18446742130017336793)), index23 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18446743101863444204)), index24 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18446743587786497910)), index25 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18446743830748024763)), index26 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18446743952228788189)), index27 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18446744012969169902)), index28 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18446744043339360759)), index29 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18446744058524456187)), index30 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18446744066117003901)), index31 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18446744069913277758)), index32 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18446744071811414687)), index33 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18446744072760483151)), index34 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18446744073235017383)), index35 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18446744073472284499)), index36 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18446744073590918057)), index37 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18446744073650234836)), index38 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18446744073679893226)), index39 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18446744073694722421)), index40 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18446744073702137018)), index41 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18446744073705844317)), index42 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18446744073707697966)), index43 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18446744073708624791)), index44 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18446744073709088203)), index45 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18446744073709319909)), index46 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18446744073709435762)), index47 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18446744073709493689)), index48 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18446744073709522652)), index49 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18446744073709537134)), index50 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18446744073709544375)), index51 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18446744073709547995)), index52 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18446744073709549805)), index53 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18446744073709550710)), index54 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18446744073709551163)), index55 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18446744073709551389)), index56 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18446744073709551502)), index57 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18446744073709551559)), index58 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18446744073709551587)), index59 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18446744073709551601)), index60 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18446744073709551608)), index61 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18446744073709551612)), index62 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18446744073709551614)), index63 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18446744073709551615))) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%16 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %4 @variable_shift(%5 x: i64, %6 i: i32) -> i64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%5), read<i32>(%6));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @constant_shift(%8 x: i64, %9 i: i32) -> i64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         switch %17 read<i32>(%9)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %17 const<i32>(0):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(0)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(1):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(1)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(2):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(2)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(3):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(3)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(4):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(4)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(5):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(5)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(6):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(6)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(7):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(7)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(8):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(8)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(9):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(9)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(10):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(10)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(11):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(11)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(12):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(12)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(13):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(13)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(14):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(14)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(15):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(15)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(16):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(16)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(17):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(17)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(18):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(18)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(19):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(19)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(20):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(20)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(21):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(21)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(22):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(22)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(23):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(23)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(24):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(24)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(25):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(25)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(26):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(26)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(27):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(27)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(28):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(28)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(29):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(29)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(30):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(30)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(31):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(31)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(32):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(32)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(33):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(33)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(34):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(34)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(35):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(35)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(36):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(36)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(37):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(37)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(38):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(38)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(39):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(39)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(40):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(40)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(41):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(41)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(42):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(42)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(43):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(43)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(44):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(44)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(45):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(45)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(46):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(46)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(47):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(47)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(48):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(48)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(49):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(49)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(50):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(50)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(51):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(51)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(52):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(52)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(53):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(53)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(54):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(54)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(55):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(55)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(56):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(56)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(57):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(57)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(58):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(58)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(59):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(59)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(60):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(60)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(61):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(61)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(62):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(62)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 case %17 const<i32>(63):
// DEFAULT-NEXT:                     write<i64>(%8, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%8), const<i32>(63)));
// DEFAULT-NEXT:                 break %17;
// DEFAULT-NEXT:                 default %17:
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<i64>(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %11 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %18
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%11, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%11), const<i32>(64))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %22: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:                 let %23: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%22), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%11, read<i32>(%23));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %12 y: i64 [storage=automatic] = call<i64, signature=fn(i64, i32) -> i64>(%4, read<i64>(deref(ptr_offset<ptr<const i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<const i64>, length=Some(64)>(%2), const<i32>(0)))), read<i32>(%11));
// DEFAULT-NEXT:                     if ne<i64>(read<i64>(%12), read<i64>(deref(ptr_offset<ptr<const i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<const i64>, length=Some(64)>(%2), read<i32>(%11)))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %19
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%11, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%11), const<i32>(64))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %24: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:                 let %25: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%24), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%11, read<i32>(%25));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %13 y: i64 [storage=automatic] = call<i64, signature=fn(i64, i32) -> i64>(%4, read<i64>(deref(ptr_offset<ptr<const i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<const i64>, length=Some(64)>(%3), const<i32>(0)))), read<i32>(%11));
// DEFAULT-NEXT:                     if ne<i64>(read<i64>(%13), read<i64>(deref(ptr_offset<ptr<const i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<const i64>, length=Some(64)>(%3), read<i32>(%11)))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %20
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%11, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%11), const<i32>(64))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %26: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:                 let %27: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%26), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%11, read<i32>(%27));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %14 y: i64 [storage=automatic] = call<i64, signature=fn(i64, i32) -> i64>(%7, read<i64>(deref(ptr_offset<ptr<const i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<const i64>, length=Some(64)>(%2), const<i32>(0)))), read<i32>(%11));
// DEFAULT-NEXT:                     if ne<i64>(read<i64>(%14), read<i64>(deref(ptr_offset<ptr<const i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<const i64>, length=Some(64)>(%2), read<i32>(%11)))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %21
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%11, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%11), const<i32>(64))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %28: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:                 let %29: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%28), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%11, read<i32>(%29));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %15 y: i64 [storage=automatic] = call<i64, signature=fn(i64, i32) -> i64>(%7, read<i64>(deref(ptr_offset<ptr<const i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<const i64>, length=Some(64)>(%3), const<i32>(0)))), read<i32>(%11));
// DEFAULT-NEXT:                     if ne<i64>(read<i64>(%15), read<i64>(deref(ptr_offset<ptr<const i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<const i64>, length=Some(64)>(%3), read<i32>(%11)))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
