/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-optimized" } */

#if __SIZEOF_INT__ < 4
#define int __INT32_TYPE__
#endif

int test_s32_0_1(int x) { return __builtin_bswap32(x) & 1; }
int test_s32_0_2(int x) { return __builtin_bswap32(x) & 2; }
int test_s32_0_240(int x) { return __builtin_bswap32(x) & 240; }
int test_s32_0_255(int x) { return __builtin_bswap32(x) & 255; }
int test_s32_1_1(int x) { return (__builtin_bswap32(x) >> 1) & 1; }
int test_s32_7_1(int x) { return (__builtin_bswap32(x) >> 7) & 1; }
int test_s32_8_1(int x) { return (__builtin_bswap32(x) >> 8) & 1; }
int test_s32_8_240(int x) { return (__builtin_bswap32(x) >> 8) & 240; }
int test_s32_8_255(int x) { return (__builtin_bswap32(x) >> 8) & 255; }
int test_s32_15_1(int x) { return (__builtin_bswap32(x) >> 15) & 1; }
int test_s32_16_1(int x) { return (__builtin_bswap32(x) >> 16) & 1; }
int test_s32_16_240(int x) { return (__builtin_bswap32(x) >> 16) & 240; }
int test_s32_16_255(int x) { return (__builtin_bswap32(x) >> 16) & 255; }
int test_s32_24_1(int x) { return (__builtin_bswap32(x) >> 24) & 1; }
int test_s32_24_240(int x) { return (__builtin_bswap32(x) >> 24) & 240; }
int test_s32_24_255(int x) { return (__builtin_bswap32(x) >> 24) & 255; }
int test_s32_31_1(int x) { return (__builtin_bswap32(x) >> 31) & 1; }

int test_S32_0_1(int x) { return (int)__builtin_bswap32(x) & 1; }
int test_S32_0_2(int x) { return (int)__builtin_bswap32(x) & 2; }
int test_S32_0_240(int x) { return (int)__builtin_bswap32(x) & 240; }
int test_S32_0_255(int x) { return (int)__builtin_bswap32(x) & 255; }
int test_S32_1_1(int x) { return ((int)__builtin_bswap32(x) >> 1) & 1; }
int test_S32_7_1(int x) { return ((int)__builtin_bswap32(x) >> 7) & 1; }
int test_S32_8_1(int x) { return ((int)__builtin_bswap32(x) >> 8) & 1; }
int test_S32_8_240(int x) { return ((int)__builtin_bswap32(x) >> 8) & 240; }
int test_S32_8_255(int x) { return ((int)__builtin_bswap32(x) >> 8) & 255; }
int test_S32_15_1(int x) { return ((int)__builtin_bswap32(x) >> 15) & 1; }
int test_S32_16_1(int x) { return ((int)__builtin_bswap32(x) >> 16) & 1; }
int test_S32_16_240(int x) { return ((int)__builtin_bswap32(x) >> 16) & 240; }
int test_S32_16_255(int x) { return ((int)__builtin_bswap32(x) >> 16) & 255; }
int test_S32_24_1(int x) { return ((int)__builtin_bswap32(x) >> 24) & 1; }
int test_S32_24_240(int x) { return ((int)__builtin_bswap32(x) >> 24) & 240; }
int test_S32_24_255(int x) { return ((int)__builtin_bswap32(x) >> 24) & 255; }
int test_S32_31_1(int x) { return ((int)__builtin_bswap32(x) >> 31) & 1; }

unsigned int test_u32_24_255(unsigned int x) {
  return (__builtin_bswap32(x) >> 24) & 255;
}

long long test_s64_0_1(long long x) {
  return __builtin_bswap64(x) & 1;
}
long long test_s64_0_2(long long x) {
  return __builtin_bswap64(x) & 2;
}
long long test_s64_0_240(long long x) {
  return __builtin_bswap64(x) & 240;
}
long long test_s64_0_255(long long x) {
  return __builtin_bswap64(x) & 255;
}
long long test_s64_7_1(long long x) {
  return (__builtin_bswap64(x) >> 7) & 1;
}
long long test_s64_8_1(long long x) {
  return (__builtin_bswap64(x) >> 8) & 1;
}
long long test_s64_8_240(long long x) {
  return (__builtin_bswap64(x) >> 56) & 240;
}
long long test_s64_8_255(long long x) {
  return (__builtin_bswap64(x) >> 8) & 255;
}
long long test_s64_9_1(long long x) {
  return (__builtin_bswap64(x) >> 9) & 1;
}
long long test_s64_31_1(long long x) {
  return (__builtin_bswap64(x) >> 31) & 1;
}
long long test_s64_32_1(long long x) {
  return (__builtin_bswap64(x) >> 32) & 1;
}
long long test_s64_32_240(long long x) {
  return (__builtin_bswap64(x) >> 32) & 240;
}
long long test_s64_32_255(long long x) {
  return (__builtin_bswap64(x) >> 32) & 255;
}
long long test_s64_33_1(long long x) {
  return (__builtin_bswap64(x) >> 33) & 1;
}
long long test_s64_48_1(long long x) {
  return (__builtin_bswap64(x) >> 48) & 1;
}
long long test_s64_48_240(long long x) {
  return (__builtin_bswap64(x) >> 48) & 240;
}
long long test_s64_48_255(long long x) {
  return (__builtin_bswap64(x) >> 48) & 255;
}
long long test_s64_56_1(long long x) {
  return (__builtin_bswap64(x) >> 56) & 1;
}
long long test_s64_56_240(long long x) {
  return (__builtin_bswap64(x) >> 56) & 240;
}
long long test_s64_56_255(long long x) {
  return (__builtin_bswap64(x) >> 56) & 255;
}
long long test_s64_57_1(long long x) {
  return (__builtin_bswap64(x) >> 57) & 1;
}
long long test_s64_63_1(long long x) {
  return (__builtin_bswap64(x) >> 63) & 1;
}

long long test_S64_0_1(long long x) {
  return (long long)__builtin_bswap64(x) & 1;
}
long long test_S64_0_2(long long x) {
  return (long long)__builtin_bswap64(x) & 2;
}
long long test_S64_0_240(long long x) {
  return (long long)__builtin_bswap64(x) & 240;
}
long long test_S64_0_255(long long x) {
  return (long long)__builtin_bswap64(x) & 255;
}
long long test_S64_7_1(long long x) {
  return ((long long)__builtin_bswap64(x) >> 7) & 1;
}
long long test_S64_8_1(long long x) {
  return ((long long)__builtin_bswap64(x) >> 8) & 1;
}
long long test_S64_8_240(long long x) {
  return ((long long)__builtin_bswap64(x) >> 56) & 240;
}
long long test_S64_8_255(long long x) {
  return ((long long)__builtin_bswap64(x) >> 8) & 255;
}
long long test_S64_9_1(long long x) {
  return ((long long)__builtin_bswap64(x) >> 9) & 1;
}
long long test_S64_31_1(long long x) {
  return ((long long)__builtin_bswap64(x) >> 31) & 1;
}
long long test_S64_32_1(long long x) {
  return ((long long)__builtin_bswap64(x) >> 32) & 1;
}
long long test_S64_32_240(long long x) {
  return ((long long)__builtin_bswap64(x) >> 32) & 240;
}
long long test_S64_32_255(long long x) {
  return ((long long)__builtin_bswap64(x) >> 32) & 255;
}
long long test_S64_33_1(long long x) {
  return ((long long)__builtin_bswap64(x) >> 33) & 1;
}
long long test_S64_48_1(long long x) {
  return ((long long)__builtin_bswap64(x) >> 48) & 1;
}
long long test_S64_48_240(long long x) {
  return ((long long)__builtin_bswap64(x) >> 48) & 240;
}
long long test_S64_48_255(long long x) {
  return ((long long)__builtin_bswap64(x) >> 48) & 255;
}
long long test_S64_56_1(long long x) {
  return ((long long)__builtin_bswap64(x) >> 56) & 1;
}
long long test_S64_56_240(long long x) {
  return ((long long)__builtin_bswap64(x) >> 56) & 240;
}
long long test_S64_56_255(long long x) {
  return ((long long)__builtin_bswap64(x) >> 56) & 255;
}
long long test_S64_57_1(long long x) {
  return ((long long)__builtin_bswap64(x) >> 57) & 1;
}
long long test_S64_63_1(long long x) {
  return ((long long)__builtin_bswap64(x) >> 63) & 1;
}

unsigned long long test_u64_56_255(unsigned long long x) {
  return (__builtin_bswap64(x) >> 56) & 255;
}

short test_s16_0_1(short x) {
  return __builtin_bswap16(x) & 1;
}
short test_s16_0_240(short x) {
  return __builtin_bswap16(x) & 240;
}
short test_s16_0_255(short x) {
  return __builtin_bswap16(x) & 255;
}
short test_s16_1_1(short x) {
  return (__builtin_bswap16(x) >> 1) & 1;
}
short test_s16_7_1(short x) {
  return (__builtin_bswap16(x) >> 7) & 1;
}
short test_s16_8_1(short x) {
  return (__builtin_bswap16(x) >> 8) & 1;
}
short test_s16_8_240(short x) {
  return (__builtin_bswap16(x) >> 8) & 240;
}
short test_s16_8_255(short x) {
  return (__builtin_bswap16(x) >> 8) & 255;
}
short test_s16_9_1(short x) {
  return (__builtin_bswap16(x) >> 9) & 1;
}
short test_s16_15_1(short x) {
  return (__builtin_bswap16(x) >> 15) & 1;
}

short test_S16_0_1(short x) {
  return (short)__builtin_bswap16(x) & 1;
}
short test_S16_0_240(short x) {
  return (short)__builtin_bswap16(x) & 240;
}
short test_S16_0_255(short x) {
  return (short)__builtin_bswap16(x) & 255;
}
short test_S16_1_1(short x) {
  return ((short)__builtin_bswap16(x) >> 1) & 1;
}
short test_S16_7_1(short x) {
  return ((short)__builtin_bswap16(x) >> 7) & 1;
}
short test_S16_8_1(short x) {
  return ((short)__builtin_bswap16(x) >> 8) & 1;
}
short test_S16_8_240(short x) {
  return ((short)__builtin_bswap16(x) >> 8) & 240;
}
short test_S16_8_255(short x) {
  return ((short)__builtin_bswap16(x) >> 8) & 255;
}
short test_S16_9_1(short x) {
  return ((short)__builtin_bswap16(x) >> 9) & 1;
}
short test_S16_15_1(short x) {
  return ((short)__builtin_bswap16(x) >> 15) & 1;
}

unsigned short test_u16_8_255(unsigned short x) {
  return (__builtin_bswap16(x) >> 8) & 255;
}


/* Shifts only */
int test_s32_24(int x) {
  return __builtin_bswap32(x) >> 24;
}
int test_s32_25(int x) {
  return __builtin_bswap32(x) >> 25;
}
int test_s32_30(int x) {
  return __builtin_bswap32(x) >> 30;
}
int test_s32_31(int x) {
  return __builtin_bswap32(x) >> 31;
}

unsigned int test_u32_24(unsigned int x) {
 return __builtin_bswap32(x) >> 24;
}
unsigned int test_u32_25(unsigned int x) {
 return __builtin_bswap32(x) >> 25;
}
unsigned int test_u32_30(unsigned int x) {
 return __builtin_bswap32(x) >> 30;
}
unsigned int test_u32_31(unsigned int x) {
 return __builtin_bswap32(x) >> 31;
}

long long test_s64_56(long long x) {
  return __builtin_bswap64(x) >> 56;
}
long long test_s64_57(long long x) {
  return __builtin_bswap64(x) >> 57;
}
long long test_s64_62(long long x) {
  return __builtin_bswap64(x) >> 62;
}
long long test_s64_63(long long x) {
  return __builtin_bswap64(x) >> 63;
}

unsigned long long test_u64_56(unsigned long long x) {
  return __builtin_bswap64(x) >> 56;
}
unsigned long long test_u64_57(unsigned long long x) {
  return __builtin_bswap64(x) >> 57;
}
unsigned long long test_u64_62(unsigned long long x) {
  return __builtin_bswap64(x) >> 62;
}
unsigned long long test_u64_63(unsigned long long x) {
  return __builtin_bswap64(x) >> 63;
}

short test_s16_8(short x) {
  return __builtin_bswap16(x) >> 8;
}
short test_s16_9(short x) {
  return __builtin_bswap16(x) >> 9;
}
short test_s16_14(short x) {
  return __builtin_bswap16(x) >> 14;
}
short test_s16_15(short x) {
  return __builtin_bswap16(x) >> 15;
}

unsigned short test_u16_8(unsigned short x) {
  return __builtin_bswap16(x) >> 8;
}
unsigned short test_u16_9(unsigned short x) {
  return __builtin_bswap16(x) >> 9;
}
unsigned short test_u16_14(unsigned short x) {
  return __builtin_bswap16(x) >> 14;
}
unsigned short test_u16_15(unsigned short x) {
  return __builtin_bswap16(x) >> 15;
}

/* { dg-final { scan-tree-dump-not "__builtin_bswap" "optimized" } } */


// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     fn %251 @__builtin_bswap32(%250 <unnamed>: u32) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %0 @test_s32_0_1(%1 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(and<u32>(call<u32, signature=fn(u32) -> u32>(%251, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @test_s32_0_2(%3 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(and<u32>(call<u32, signature=fn(u32) -> u32>(%251, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%3))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @test_s32_0_240(%5 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(and<u32>(call<u32, signature=fn(u32) -> u32>(%251, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%5))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(240))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @test_s32_0_255(%7 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(and<u32>(call<u32, signature=fn(u32) -> u32>(%251, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%7))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(255))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @test_s32_1_1(%9 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%251, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%9))), const<i32>(1)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @test_s32_7_1(%11 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%251, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%11))), const<i32>(7)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @test_s32_8_1(%13 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%251, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%13))), const<i32>(8)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @test_s32_8_240(%15 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%251, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%15))), const<i32>(8)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(240))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @test_s32_8_255(%17 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%251, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%17))), const<i32>(8)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(255))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @test_s32_15_1(%19 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%251, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%19))), const<i32>(15)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @test_s32_16_1(%21 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%251, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%21))), const<i32>(16)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @test_s32_16_240(%23 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%251, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%23))), const<i32>(16)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(240))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %24 @test_s32_16_255(%25 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%251, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%25))), const<i32>(16)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(255))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %26 @test_s32_24_1(%27 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%251, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%27))), const<i32>(24)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %28 @test_s32_24_240(%29 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%251, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%29))), const<i32>(24)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(240))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %30 @test_s32_24_255(%31 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%251, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%31))), const<i32>(24)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(255))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %32 @test_s32_31_1(%33 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%251, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%33))), const<i32>(31)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %34 @test_S32_0_1(%35 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i32>(reinterpret<i32, reason=explicit, fits=unknown>(call<u32, signature=fn(u32) -> u32>(%251, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%35)))), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %36 @test_S32_0_2(%37 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i32>(reinterpret<i32, reason=explicit, fits=unknown>(call<u32, signature=fn(u32) -> u32>(%251, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%37)))), const<i32>(2));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %38 @test_S32_0_240(%39 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i32>(reinterpret<i32, reason=explicit, fits=unknown>(call<u32, signature=fn(u32) -> u32>(%251, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%39)))), const<i32>(240));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %40 @test_S32_0_255(%41 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i32>(reinterpret<i32, reason=explicit, fits=unknown>(call<u32, signature=fn(u32) -> u32>(%251, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%41)))), const<i32>(255));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %42 @test_S32_1_1(%43 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=explicit, fits=unknown>(call<u32, signature=fn(u32) -> u32>(%251, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%43)))), const<i32>(1)), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %44 @test_S32_7_1(%45 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=explicit, fits=unknown>(call<u32, signature=fn(u32) -> u32>(%251, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%45)))), const<i32>(7)), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %46 @test_S32_8_1(%47 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=explicit, fits=unknown>(call<u32, signature=fn(u32) -> u32>(%251, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%47)))), const<i32>(8)), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %48 @test_S32_8_240(%49 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=explicit, fits=unknown>(call<u32, signature=fn(u32) -> u32>(%251, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%49)))), const<i32>(8)), const<i32>(240));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %50 @test_S32_8_255(%51 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=explicit, fits=unknown>(call<u32, signature=fn(u32) -> u32>(%251, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%51)))), const<i32>(8)), const<i32>(255));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %52 @test_S32_15_1(%53 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=explicit, fits=unknown>(call<u32, signature=fn(u32) -> u32>(%251, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%53)))), const<i32>(15)), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %54 @test_S32_16_1(%55 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=explicit, fits=unknown>(call<u32, signature=fn(u32) -> u32>(%251, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%55)))), const<i32>(16)), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %56 @test_S32_16_240(%57 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=explicit, fits=unknown>(call<u32, signature=fn(u32) -> u32>(%251, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%57)))), const<i32>(16)), const<i32>(240));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %58 @test_S32_16_255(%59 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=explicit, fits=unknown>(call<u32, signature=fn(u32) -> u32>(%251, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%59)))), const<i32>(16)), const<i32>(255));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %60 @test_S32_24_1(%61 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=explicit, fits=unknown>(call<u32, signature=fn(u32) -> u32>(%251, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%61)))), const<i32>(24)), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %62 @test_S32_24_240(%63 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=explicit, fits=unknown>(call<u32, signature=fn(u32) -> u32>(%251, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%63)))), const<i32>(24)), const<i32>(240));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %64 @test_S32_24_255(%65 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=explicit, fits=unknown>(call<u32, signature=fn(u32) -> u32>(%251, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%65)))), const<i32>(24)), const<i32>(255));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %66 @test_S32_31_1(%67 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=explicit, fits=unknown>(call<u32, signature=fn(u32) -> u32>(%251, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%67)))), const<i32>(31)), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %68 @test_u32_24_255(%69 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%251, read<u32>(%69)), const<i32>(24)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(255)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %253 @__builtin_bswap64(%252 <unnamed>: u64) -> u64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %70 @test_s64_0_1(%71 x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i64, reason=return, fits=unknown>(and<u64>(call<u64, signature=fn(u64) -> u64>(%253, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%71))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %72 @test_s64_0_2(%73 x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i64, reason=return, fits=unknown>(and<u64>(call<u64, signature=fn(u64) -> u64>(%253, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%73))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %74 @test_s64_0_240(%75 x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i64, reason=return, fits=unknown>(and<u64>(call<u64, signature=fn(u64) -> u64>(%253, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%75))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(240)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %76 @test_s64_0_255(%77 x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i64, reason=return, fits=unknown>(and<u64>(call<u64, signature=fn(u64) -> u64>(%253, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%77))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(255)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %78 @test_s64_7_1(%79 x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i64, reason=return, fits=unknown>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%253, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%79))), const<i32>(7)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %80 @test_s64_8_1(%81 x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i64, reason=return, fits=unknown>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%253, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%81))), const<i32>(8)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %82 @test_s64_8_240(%83 x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i64, reason=return, fits=unknown>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%253, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%83))), const<i32>(56)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(240)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %84 @test_s64_8_255(%85 x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i64, reason=return, fits=unknown>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%253, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%85))), const<i32>(8)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(255)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %86 @test_s64_9_1(%87 x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i64, reason=return, fits=unknown>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%253, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%87))), const<i32>(9)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %88 @test_s64_31_1(%89 x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i64, reason=return, fits=unknown>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%253, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%89))), const<i32>(31)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %90 @test_s64_32_1(%91 x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i64, reason=return, fits=unknown>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%253, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%91))), const<i32>(32)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %92 @test_s64_32_240(%93 x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i64, reason=return, fits=unknown>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%253, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%93))), const<i32>(32)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(240)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %94 @test_s64_32_255(%95 x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i64, reason=return, fits=unknown>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%253, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%95))), const<i32>(32)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(255)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %96 @test_s64_33_1(%97 x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i64, reason=return, fits=unknown>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%253, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%97))), const<i32>(33)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %98 @test_s64_48_1(%99 x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i64, reason=return, fits=unknown>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%253, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%99))), const<i32>(48)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %100 @test_s64_48_240(%101 x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i64, reason=return, fits=unknown>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%253, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%101))), const<i32>(48)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(240)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %102 @test_s64_48_255(%103 x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i64, reason=return, fits=unknown>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%253, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%103))), const<i32>(48)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(255)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %104 @test_s64_56_1(%105 x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i64, reason=return, fits=unknown>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%253, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%105))), const<i32>(56)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %106 @test_s64_56_240(%107 x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i64, reason=return, fits=unknown>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%253, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%107))), const<i32>(56)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(240)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %108 @test_s64_56_255(%109 x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i64, reason=return, fits=unknown>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%253, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%109))), const<i32>(56)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(255)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %110 @test_s64_57_1(%111 x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i64, reason=return, fits=unknown>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%253, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%111))), const<i32>(57)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %112 @test_s64_63_1(%113 x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i64, reason=return, fits=unknown>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%253, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%113))), const<i32>(63)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %114 @test_S64_0_1(%115 x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i64>(reinterpret<i64, reason=explicit, fits=unknown>(call<u64, signature=fn(u64) -> u64>(%253, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%115)))), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %116 @test_S64_0_2(%117 x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i64>(reinterpret<i64, reason=explicit, fits=unknown>(call<u64, signature=fn(u64) -> u64>(%253, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%117)))), widen<i64, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %118 @test_S64_0_240(%119 x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i64>(reinterpret<i64, reason=explicit, fits=unknown>(call<u64, signature=fn(u64) -> u64>(%253, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%119)))), widen<i64, reason=usual_arith>(const<i32>(240)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %120 @test_S64_0_255(%121 x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i64>(reinterpret<i64, reason=explicit, fits=unknown>(call<u64, signature=fn(u64) -> u64>(%253, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%121)))), widen<i64, reason=usual_arith>(const<i32>(255)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %122 @test_S64_7_1(%123 x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i64, reason=explicit, fits=unknown>(call<u64, signature=fn(u64) -> u64>(%253, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%123)))), const<i32>(7)), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %124 @test_S64_8_1(%125 x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i64, reason=explicit, fits=unknown>(call<u64, signature=fn(u64) -> u64>(%253, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%125)))), const<i32>(8)), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %126 @test_S64_8_240(%127 x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i64, reason=explicit, fits=unknown>(call<u64, signature=fn(u64) -> u64>(%253, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%127)))), const<i32>(56)), widen<i64, reason=usual_arith>(const<i32>(240)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %128 @test_S64_8_255(%129 x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i64, reason=explicit, fits=unknown>(call<u64, signature=fn(u64) -> u64>(%253, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%129)))), const<i32>(8)), widen<i64, reason=usual_arith>(const<i32>(255)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %130 @test_S64_9_1(%131 x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i64, reason=explicit, fits=unknown>(call<u64, signature=fn(u64) -> u64>(%253, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%131)))), const<i32>(9)), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %132 @test_S64_31_1(%133 x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i64, reason=explicit, fits=unknown>(call<u64, signature=fn(u64) -> u64>(%253, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%133)))), const<i32>(31)), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %134 @test_S64_32_1(%135 x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i64, reason=explicit, fits=unknown>(call<u64, signature=fn(u64) -> u64>(%253, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%135)))), const<i32>(32)), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %136 @test_S64_32_240(%137 x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i64, reason=explicit, fits=unknown>(call<u64, signature=fn(u64) -> u64>(%253, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%137)))), const<i32>(32)), widen<i64, reason=usual_arith>(const<i32>(240)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %138 @test_S64_32_255(%139 x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i64, reason=explicit, fits=unknown>(call<u64, signature=fn(u64) -> u64>(%253, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%139)))), const<i32>(32)), widen<i64, reason=usual_arith>(const<i32>(255)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %140 @test_S64_33_1(%141 x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i64, reason=explicit, fits=unknown>(call<u64, signature=fn(u64) -> u64>(%253, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%141)))), const<i32>(33)), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %142 @test_S64_48_1(%143 x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i64, reason=explicit, fits=unknown>(call<u64, signature=fn(u64) -> u64>(%253, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%143)))), const<i32>(48)), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %144 @test_S64_48_240(%145 x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i64, reason=explicit, fits=unknown>(call<u64, signature=fn(u64) -> u64>(%253, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%145)))), const<i32>(48)), widen<i64, reason=usual_arith>(const<i32>(240)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %146 @test_S64_48_255(%147 x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i64, reason=explicit, fits=unknown>(call<u64, signature=fn(u64) -> u64>(%253, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%147)))), const<i32>(48)), widen<i64, reason=usual_arith>(const<i32>(255)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %148 @test_S64_56_1(%149 x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i64, reason=explicit, fits=unknown>(call<u64, signature=fn(u64) -> u64>(%253, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%149)))), const<i32>(56)), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %150 @test_S64_56_240(%151 x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i64, reason=explicit, fits=unknown>(call<u64, signature=fn(u64) -> u64>(%253, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%151)))), const<i32>(56)), widen<i64, reason=usual_arith>(const<i32>(240)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %152 @test_S64_56_255(%153 x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i64, reason=explicit, fits=unknown>(call<u64, signature=fn(u64) -> u64>(%253, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%153)))), const<i32>(56)), widen<i64, reason=usual_arith>(const<i32>(255)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %154 @test_S64_57_1(%155 x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i64, reason=explicit, fits=unknown>(call<u64, signature=fn(u64) -> u64>(%253, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%155)))), const<i32>(57)), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %156 @test_S64_63_1(%157 x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i64, reason=explicit, fits=unknown>(call<u64, signature=fn(u64) -> u64>(%253, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%157)))), const<i32>(63)), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %158 @test_u64_56_255(%159 x: u64) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%253, read<u64>(%159)), const<i32>(56)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(255))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %255 @__builtin_bswap16(%254 <unnamed>: u16) -> u16 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %160 @test_s16_0_1(%161 x: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%255, reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%161))))), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %162 @test_s16_0_240(%163 x: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%255, reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%163))))), const<i32>(240)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %164 @test_s16_0_255(%165 x: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%255, reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%165))))), const<i32>(255)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %166 @test_s16_1_1(%167 x: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%255, reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%167))))), const<i32>(1)), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %168 @test_s16_7_1(%169 x: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%255, reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%169))))), const<i32>(7)), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %170 @test_s16_8_1(%171 x: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%255, reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%171))))), const<i32>(8)), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %172 @test_s16_8_240(%173 x: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%255, reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%173))))), const<i32>(8)), const<i32>(240)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %174 @test_s16_8_255(%175 x: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%255, reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%175))))), const<i32>(8)), const<i32>(255)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %176 @test_s16_9_1(%177 x: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%255, reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%177))))), const<i32>(9)), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %178 @test_s16_15_1(%179 x: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%255, reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%179))))), const<i32>(15)), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %180 @test_S16_0_1(%181 x: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(and<i32>(widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(call<u16, signature=fn(u16) -> u16>(%255, reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%181))))), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %182 @test_S16_0_240(%183 x: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(and<i32>(widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(call<u16, signature=fn(u16) -> u16>(%255, reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%183))))), const<i32>(240)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %184 @test_S16_0_255(%185 x: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(and<i32>(widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(call<u16, signature=fn(u16) -> u16>(%255, reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%185))))), const<i32>(255)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %186 @test_S16_1_1(%187 x: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(call<u16, signature=fn(u16) -> u16>(%255, reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%187))))), const<i32>(1)), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %188 @test_S16_7_1(%189 x: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(call<u16, signature=fn(u16) -> u16>(%255, reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%189))))), const<i32>(7)), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %190 @test_S16_8_1(%191 x: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(call<u16, signature=fn(u16) -> u16>(%255, reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%191))))), const<i32>(8)), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %192 @test_S16_8_240(%193 x: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(call<u16, signature=fn(u16) -> u16>(%255, reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%193))))), const<i32>(8)), const<i32>(240)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %194 @test_S16_8_255(%195 x: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(call<u16, signature=fn(u16) -> u16>(%255, reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%195))))), const<i32>(8)), const<i32>(255)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %196 @test_S16_9_1(%197 x: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(call<u16, signature=fn(u16) -> u16>(%255, reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%197))))), const<i32>(9)), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %198 @test_S16_15_1(%199 x: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(call<u16, signature=fn(u16) -> u16>(%255, reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%199))))), const<i32>(15)), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %200 @test_u16_8_255(%201 x: u16) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%255, read<u16>(%201)))), const<i32>(8)), const<i32>(255))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %202 @test_s32_24(%203 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%251, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%203))), const<i32>(24)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %204 @test_s32_25(%205 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%251, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%205))), const<i32>(25)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %206 @test_s32_30(%207 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%251, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%207))), const<i32>(30)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %208 @test_s32_31(%209 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%251, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%209))), const<i32>(31)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %210 @test_u32_24(%211 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%251, read<u32>(%211)), const<i32>(24));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %212 @test_u32_25(%213 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%251, read<u32>(%213)), const<i32>(25));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %214 @test_u32_30(%215 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%251, read<u32>(%215)), const<i32>(30));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %216 @test_u32_31(%217 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%251, read<u32>(%217)), const<i32>(31));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %218 @test_s64_56(%219 x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i64, reason=return, fits=unknown>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%253, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%219))), const<i32>(56)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %220 @test_s64_57(%221 x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i64, reason=return, fits=unknown>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%253, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%221))), const<i32>(57)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %222 @test_s64_62(%223 x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i64, reason=return, fits=unknown>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%253, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%223))), const<i32>(62)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %224 @test_s64_63(%225 x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i64, reason=return, fits=unknown>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%253, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%225))), const<i32>(63)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %226 @test_u64_56(%227 x: u64) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%253, read<u64>(%227)), const<i32>(56));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %228 @test_u64_57(%229 x: u64) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%253, read<u64>(%229)), const<i32>(57));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %230 @test_u64_62(%231 x: u64) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%253, read<u64>(%231)), const<i32>(62));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %232 @test_u64_63(%233 x: u64) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%253, read<u64>(%233)), const<i32>(63));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %234 @test_s16_8(%235 x: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%255, reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%235))))), const<i32>(8)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %236 @test_s16_9(%237 x: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%255, reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%237))))), const<i32>(9)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %238 @test_s16_14(%239 x: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%255, reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%239))))), const<i32>(14)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %240 @test_s16_15(%241 x: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%255, reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%241))))), const<i32>(15)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %242 @test_u16_8(%243 x: u16) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%255, read<u16>(%243)))), const<i32>(8))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %244 @test_u16_9(%245 x: u16) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%255, read<u16>(%245)))), const<i32>(9))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %246 @test_u16_14(%247 x: u16) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%255, read<u16>(%247)))), const<i32>(14))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %248 @test_u16_15(%249 x: u16) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%255, read<u16>(%249)))), const<i32>(15))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
