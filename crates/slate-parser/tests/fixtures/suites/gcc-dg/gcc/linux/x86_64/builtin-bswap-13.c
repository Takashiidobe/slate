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
// DEFAULT-NEXT:     fn %[[VALUE___builtin_bswap32:[0-9]+]] @__builtin_bswap32(%[[VALUE0:[0-9]+]] <unnamed>: u32) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_test_s32_0_1:[0-9]+]] @test_s32_0_1(%[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(and<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x]]))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s32_0_2:[0-9]+]] @test_s32_0_2(%[[VALUE_x_2:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(and<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_2]]))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s32_0_240:[0-9]+]] @test_s32_0_240(%[[VALUE_x_3:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(and<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_3]]))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(240))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s32_0_255:[0-9]+]] @test_s32_0_255(%[[VALUE_x_4:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(and<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_4]]))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(255))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s32_1_1:[0-9]+]] @test_s32_1_1(%[[VALUE_x_5:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_5]]))), const<i32>(1)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s32_7_1:[0-9]+]] @test_s32_7_1(%[[VALUE_x_6:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_6]]))), const<i32>(7)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s32_8_1:[0-9]+]] @test_s32_8_1(%[[VALUE_x_7:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_7]]))), const<i32>(8)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s32_8_240:[0-9]+]] @test_s32_8_240(%[[VALUE_x_8:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_8]]))), const<i32>(8)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(240))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s32_8_255:[0-9]+]] @test_s32_8_255(%[[VALUE_x_9:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_9]]))), const<i32>(8)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(255))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s32_15_1:[0-9]+]] @test_s32_15_1(%[[VALUE_x_10:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_10]]))), const<i32>(15)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s32_16_1:[0-9]+]] @test_s32_16_1(%[[VALUE_x_11:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_11]]))), const<i32>(16)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s32_16_240:[0-9]+]] @test_s32_16_240(%[[VALUE_x_12:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_12]]))), const<i32>(16)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(240))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s32_16_255:[0-9]+]] @test_s32_16_255(%[[VALUE_x_13:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_13]]))), const<i32>(16)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(255))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s32_24_1:[0-9]+]] @test_s32_24_1(%[[VALUE_x_14:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_14]]))), const<i32>(24)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s32_24_240:[0-9]+]] @test_s32_24_240(%[[VALUE_x_15:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_15]]))), const<i32>(24)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(240))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s32_24_255:[0-9]+]] @test_s32_24_255(%[[VALUE_x_16:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_16]]))), const<i32>(24)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(255))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s32_31_1:[0-9]+]] @test_s32_31_1(%[[VALUE_x_17:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_17]]))), const<i32>(31)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_S32_0_1:[0-9]+]] @test_S32_0_1(%[[VALUE_x_18:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i32>(reinterpret<i32, reason=explicit, fits=unknown>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_18]])))), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_S32_0_2:[0-9]+]] @test_S32_0_2(%[[VALUE_x_19:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i32>(reinterpret<i32, reason=explicit, fits=unknown>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_19]])))), const<i32>(2));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_S32_0_240:[0-9]+]] @test_S32_0_240(%[[VALUE_x_20:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i32>(reinterpret<i32, reason=explicit, fits=unknown>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_20]])))), const<i32>(240));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_S32_0_255:[0-9]+]] @test_S32_0_255(%[[VALUE_x_21:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i32>(reinterpret<i32, reason=explicit, fits=unknown>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_21]])))), const<i32>(255));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_S32_1_1:[0-9]+]] @test_S32_1_1(%[[VALUE_x_22:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=explicit, fits=unknown>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_22]])))), const<i32>(1)), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_S32_7_1:[0-9]+]] @test_S32_7_1(%[[VALUE_x_23:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=explicit, fits=unknown>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_23]])))), const<i32>(7)), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_S32_8_1:[0-9]+]] @test_S32_8_1(%[[VALUE_x_24:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=explicit, fits=unknown>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_24]])))), const<i32>(8)), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_S32_8_240:[0-9]+]] @test_S32_8_240(%[[VALUE_x_25:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=explicit, fits=unknown>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_25]])))), const<i32>(8)), const<i32>(240));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_S32_8_255:[0-9]+]] @test_S32_8_255(%[[VALUE_x_26:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=explicit, fits=unknown>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_26]])))), const<i32>(8)), const<i32>(255));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_S32_15_1:[0-9]+]] @test_S32_15_1(%[[VALUE_x_27:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=explicit, fits=unknown>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_27]])))), const<i32>(15)), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_S32_16_1:[0-9]+]] @test_S32_16_1(%[[VALUE_x_28:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=explicit, fits=unknown>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_28]])))), const<i32>(16)), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_S32_16_240:[0-9]+]] @test_S32_16_240(%[[VALUE_x_29:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=explicit, fits=unknown>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_29]])))), const<i32>(16)), const<i32>(240));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_S32_16_255:[0-9]+]] @test_S32_16_255(%[[VALUE_x_30:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=explicit, fits=unknown>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_30]])))), const<i32>(16)), const<i32>(255));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_S32_24_1:[0-9]+]] @test_S32_24_1(%[[VALUE_x_31:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=explicit, fits=unknown>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_31]])))), const<i32>(24)), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_S32_24_240:[0-9]+]] @test_S32_24_240(%[[VALUE_x_32:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=explicit, fits=unknown>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_32]])))), const<i32>(24)), const<i32>(240));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_S32_24_255:[0-9]+]] @test_S32_24_255(%[[VALUE_x_33:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=explicit, fits=unknown>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_33]])))), const<i32>(24)), const<i32>(255));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_S32_31_1:[0-9]+]] @test_S32_31_1(%[[VALUE_x_34:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=explicit, fits=unknown>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_34]])))), const<i32>(31)), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_u32_24_255:[0-9]+]] @test_u32_24_255(%[[VALUE_x_35:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], read<u32>(%[[VALUE_x_35]])), const<i32>(24)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(255)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_bswap64:[0-9]+]] @__builtin_bswap64(%[[VALUE1:[0-9]+]] <unnamed>: u64) -> u64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_test_s64_0_1:[0-9]+]] @test_s64_0_1(%[[VALUE_x_36:[0-9]+]] x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i64, reason=return, fits=unknown>(and<u64>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_36]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s64_0_2:[0-9]+]] @test_s64_0_2(%[[VALUE_x_37:[0-9]+]] x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i64, reason=return, fits=unknown>(and<u64>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_37]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s64_0_240:[0-9]+]] @test_s64_0_240(%[[VALUE_x_38:[0-9]+]] x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i64, reason=return, fits=unknown>(and<u64>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_38]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(240)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s64_0_255:[0-9]+]] @test_s64_0_255(%[[VALUE_x_39:[0-9]+]] x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i64, reason=return, fits=unknown>(and<u64>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_39]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(255)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s64_7_1:[0-9]+]] @test_s64_7_1(%[[VALUE_x_40:[0-9]+]] x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i64, reason=return, fits=unknown>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_40]]))), const<i32>(7)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s64_8_1:[0-9]+]] @test_s64_8_1(%[[VALUE_x_41:[0-9]+]] x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i64, reason=return, fits=unknown>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_41]]))), const<i32>(8)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s64_8_240:[0-9]+]] @test_s64_8_240(%[[VALUE_x_42:[0-9]+]] x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i64, reason=return, fits=unknown>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_42]]))), const<i32>(56)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(240)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s64_8_255:[0-9]+]] @test_s64_8_255(%[[VALUE_x_43:[0-9]+]] x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i64, reason=return, fits=unknown>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_43]]))), const<i32>(8)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(255)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s64_9_1:[0-9]+]] @test_s64_9_1(%[[VALUE_x_44:[0-9]+]] x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i64, reason=return, fits=unknown>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_44]]))), const<i32>(9)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s64_31_1:[0-9]+]] @test_s64_31_1(%[[VALUE_x_45:[0-9]+]] x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i64, reason=return, fits=unknown>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_45]]))), const<i32>(31)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s64_32_1:[0-9]+]] @test_s64_32_1(%[[VALUE_x_46:[0-9]+]] x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i64, reason=return, fits=unknown>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_46]]))), const<i32>(32)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s64_32_240:[0-9]+]] @test_s64_32_240(%[[VALUE_x_47:[0-9]+]] x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i64, reason=return, fits=unknown>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_47]]))), const<i32>(32)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(240)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s64_32_255:[0-9]+]] @test_s64_32_255(%[[VALUE_x_48:[0-9]+]] x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i64, reason=return, fits=unknown>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_48]]))), const<i32>(32)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(255)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s64_33_1:[0-9]+]] @test_s64_33_1(%[[VALUE_x_49:[0-9]+]] x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i64, reason=return, fits=unknown>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_49]]))), const<i32>(33)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s64_48_1:[0-9]+]] @test_s64_48_1(%[[VALUE_x_50:[0-9]+]] x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i64, reason=return, fits=unknown>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_50]]))), const<i32>(48)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s64_48_240:[0-9]+]] @test_s64_48_240(%[[VALUE_x_51:[0-9]+]] x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i64, reason=return, fits=unknown>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_51]]))), const<i32>(48)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(240)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s64_48_255:[0-9]+]] @test_s64_48_255(%[[VALUE_x_52:[0-9]+]] x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i64, reason=return, fits=unknown>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_52]]))), const<i32>(48)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(255)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s64_56_1:[0-9]+]] @test_s64_56_1(%[[VALUE_x_53:[0-9]+]] x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i64, reason=return, fits=unknown>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_53]]))), const<i32>(56)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s64_56_240:[0-9]+]] @test_s64_56_240(%[[VALUE_x_54:[0-9]+]] x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i64, reason=return, fits=unknown>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_54]]))), const<i32>(56)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(240)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s64_56_255:[0-9]+]] @test_s64_56_255(%[[VALUE_x_55:[0-9]+]] x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i64, reason=return, fits=unknown>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_55]]))), const<i32>(56)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(255)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s64_57_1:[0-9]+]] @test_s64_57_1(%[[VALUE_x_56:[0-9]+]] x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i64, reason=return, fits=unknown>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_56]]))), const<i32>(57)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s64_63_1:[0-9]+]] @test_s64_63_1(%[[VALUE_x_57:[0-9]+]] x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i64, reason=return, fits=unknown>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_57]]))), const<i32>(63)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_S64_0_1:[0-9]+]] @test_S64_0_1(%[[VALUE_x_58:[0-9]+]] x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i64>(reinterpret<i64, reason=explicit, fits=unknown>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_58]])))), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_S64_0_2:[0-9]+]] @test_S64_0_2(%[[VALUE_x_59:[0-9]+]] x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i64>(reinterpret<i64, reason=explicit, fits=unknown>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_59]])))), widen<i64, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_S64_0_240:[0-9]+]] @test_S64_0_240(%[[VALUE_x_60:[0-9]+]] x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i64>(reinterpret<i64, reason=explicit, fits=unknown>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_60]])))), widen<i64, reason=usual_arith>(const<i32>(240)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_S64_0_255:[0-9]+]] @test_S64_0_255(%[[VALUE_x_61:[0-9]+]] x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i64>(reinterpret<i64, reason=explicit, fits=unknown>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_61]])))), widen<i64, reason=usual_arith>(const<i32>(255)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_S64_7_1:[0-9]+]] @test_S64_7_1(%[[VALUE_x_62:[0-9]+]] x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i64, reason=explicit, fits=unknown>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_62]])))), const<i32>(7)), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_S64_8_1:[0-9]+]] @test_S64_8_1(%[[VALUE_x_63:[0-9]+]] x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i64, reason=explicit, fits=unknown>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_63]])))), const<i32>(8)), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_S64_8_240:[0-9]+]] @test_S64_8_240(%[[VALUE_x_64:[0-9]+]] x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i64, reason=explicit, fits=unknown>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_64]])))), const<i32>(56)), widen<i64, reason=usual_arith>(const<i32>(240)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_S64_8_255:[0-9]+]] @test_S64_8_255(%[[VALUE_x_65:[0-9]+]] x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i64, reason=explicit, fits=unknown>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_65]])))), const<i32>(8)), widen<i64, reason=usual_arith>(const<i32>(255)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_S64_9_1:[0-9]+]] @test_S64_9_1(%[[VALUE_x_66:[0-9]+]] x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i64, reason=explicit, fits=unknown>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_66]])))), const<i32>(9)), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_S64_31_1:[0-9]+]] @test_S64_31_1(%[[VALUE_x_67:[0-9]+]] x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i64, reason=explicit, fits=unknown>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_67]])))), const<i32>(31)), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_S64_32_1:[0-9]+]] @test_S64_32_1(%[[VALUE_x_68:[0-9]+]] x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i64, reason=explicit, fits=unknown>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_68]])))), const<i32>(32)), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_S64_32_240:[0-9]+]] @test_S64_32_240(%[[VALUE_x_69:[0-9]+]] x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i64, reason=explicit, fits=unknown>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_69]])))), const<i32>(32)), widen<i64, reason=usual_arith>(const<i32>(240)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_S64_32_255:[0-9]+]] @test_S64_32_255(%[[VALUE_x_70:[0-9]+]] x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i64, reason=explicit, fits=unknown>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_70]])))), const<i32>(32)), widen<i64, reason=usual_arith>(const<i32>(255)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_S64_33_1:[0-9]+]] @test_S64_33_1(%[[VALUE_x_71:[0-9]+]] x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i64, reason=explicit, fits=unknown>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_71]])))), const<i32>(33)), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_S64_48_1:[0-9]+]] @test_S64_48_1(%[[VALUE_x_72:[0-9]+]] x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i64, reason=explicit, fits=unknown>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_72]])))), const<i32>(48)), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_S64_48_240:[0-9]+]] @test_S64_48_240(%[[VALUE_x_73:[0-9]+]] x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i64, reason=explicit, fits=unknown>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_73]])))), const<i32>(48)), widen<i64, reason=usual_arith>(const<i32>(240)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_S64_48_255:[0-9]+]] @test_S64_48_255(%[[VALUE_x_74:[0-9]+]] x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i64, reason=explicit, fits=unknown>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_74]])))), const<i32>(48)), widen<i64, reason=usual_arith>(const<i32>(255)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_S64_56_1:[0-9]+]] @test_S64_56_1(%[[VALUE_x_75:[0-9]+]] x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i64, reason=explicit, fits=unknown>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_75]])))), const<i32>(56)), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_S64_56_240:[0-9]+]] @test_S64_56_240(%[[VALUE_x_76:[0-9]+]] x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i64, reason=explicit, fits=unknown>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_76]])))), const<i32>(56)), widen<i64, reason=usual_arith>(const<i32>(240)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_S64_56_255:[0-9]+]] @test_S64_56_255(%[[VALUE_x_77:[0-9]+]] x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i64, reason=explicit, fits=unknown>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_77]])))), const<i32>(56)), widen<i64, reason=usual_arith>(const<i32>(255)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_S64_57_1:[0-9]+]] @test_S64_57_1(%[[VALUE_x_78:[0-9]+]] x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i64, reason=explicit, fits=unknown>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_78]])))), const<i32>(57)), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_S64_63_1:[0-9]+]] @test_S64_63_1(%[[VALUE_x_79:[0-9]+]] x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i64, reason=explicit, fits=unknown>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_79]])))), const<i32>(63)), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_u64_56_255:[0-9]+]] @test_u64_56_255(%[[VALUE_x_80:[0-9]+]] x: u64) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], read<u64>(%[[VALUE_x_80]])), const<i32>(56)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(255))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_bswap16:[0-9]+]] @__builtin_bswap16(%[[VALUE2:[0-9]+]] <unnamed>: u16) -> u16 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_test_s16_0_1:[0-9]+]] @test_s16_0_1(%[[VALUE_x_81:[0-9]+]] x: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_bswap16]], reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%[[VALUE_x_81]]))))), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s16_0_240:[0-9]+]] @test_s16_0_240(%[[VALUE_x_82:[0-9]+]] x: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_bswap16]], reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%[[VALUE_x_82]]))))), const<i32>(240)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s16_0_255:[0-9]+]] @test_s16_0_255(%[[VALUE_x_83:[0-9]+]] x: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_bswap16]], reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%[[VALUE_x_83]]))))), const<i32>(255)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s16_1_1:[0-9]+]] @test_s16_1_1(%[[VALUE_x_84:[0-9]+]] x: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_bswap16]], reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%[[VALUE_x_84]]))))), const<i32>(1)), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s16_7_1:[0-9]+]] @test_s16_7_1(%[[VALUE_x_85:[0-9]+]] x: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_bswap16]], reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%[[VALUE_x_85]]))))), const<i32>(7)), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s16_8_1:[0-9]+]] @test_s16_8_1(%[[VALUE_x_86:[0-9]+]] x: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_bswap16]], reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%[[VALUE_x_86]]))))), const<i32>(8)), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s16_8_240:[0-9]+]] @test_s16_8_240(%[[VALUE_x_87:[0-9]+]] x: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_bswap16]], reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%[[VALUE_x_87]]))))), const<i32>(8)), const<i32>(240)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s16_8_255:[0-9]+]] @test_s16_8_255(%[[VALUE_x_88:[0-9]+]] x: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_bswap16]], reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%[[VALUE_x_88]]))))), const<i32>(8)), const<i32>(255)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s16_9_1:[0-9]+]] @test_s16_9_1(%[[VALUE_x_89:[0-9]+]] x: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_bswap16]], reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%[[VALUE_x_89]]))))), const<i32>(9)), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s16_15_1:[0-9]+]] @test_s16_15_1(%[[VALUE_x_90:[0-9]+]] x: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_bswap16]], reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%[[VALUE_x_90]]))))), const<i32>(15)), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_S16_0_1:[0-9]+]] @test_S16_0_1(%[[VALUE_x_91:[0-9]+]] x: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(and<i32>(widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_bswap16]], reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%[[VALUE_x_91]]))))), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_S16_0_240:[0-9]+]] @test_S16_0_240(%[[VALUE_x_92:[0-9]+]] x: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(and<i32>(widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_bswap16]], reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%[[VALUE_x_92]]))))), const<i32>(240)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_S16_0_255:[0-9]+]] @test_S16_0_255(%[[VALUE_x_93:[0-9]+]] x: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(and<i32>(widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_bswap16]], reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%[[VALUE_x_93]]))))), const<i32>(255)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_S16_1_1:[0-9]+]] @test_S16_1_1(%[[VALUE_x_94:[0-9]+]] x: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_bswap16]], reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%[[VALUE_x_94]]))))), const<i32>(1)), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_S16_7_1:[0-9]+]] @test_S16_7_1(%[[VALUE_x_95:[0-9]+]] x: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_bswap16]], reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%[[VALUE_x_95]]))))), const<i32>(7)), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_S16_8_1:[0-9]+]] @test_S16_8_1(%[[VALUE_x_96:[0-9]+]] x: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_bswap16]], reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%[[VALUE_x_96]]))))), const<i32>(8)), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_S16_8_240:[0-9]+]] @test_S16_8_240(%[[VALUE_x_97:[0-9]+]] x: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_bswap16]], reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%[[VALUE_x_97]]))))), const<i32>(8)), const<i32>(240)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_S16_8_255:[0-9]+]] @test_S16_8_255(%[[VALUE_x_98:[0-9]+]] x: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_bswap16]], reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%[[VALUE_x_98]]))))), const<i32>(8)), const<i32>(255)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_S16_9_1:[0-9]+]] @test_S16_9_1(%[[VALUE_x_99:[0-9]+]] x: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_bswap16]], reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%[[VALUE_x_99]]))))), const<i32>(9)), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_S16_15_1:[0-9]+]] @test_S16_15_1(%[[VALUE_x_100:[0-9]+]] x: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_bswap16]], reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%[[VALUE_x_100]]))))), const<i32>(15)), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_u16_8_255:[0-9]+]] @test_u16_8_255(%[[VALUE_x_101:[0-9]+]] x: u16) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_bswap16]], read<u16>(%[[VALUE_x_101]])))), const<i32>(8)), const<i32>(255))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s32_24:[0-9]+]] @test_s32_24(%[[VALUE_x_102:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_102]]))), const<i32>(24)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s32_25:[0-9]+]] @test_s32_25(%[[VALUE_x_103:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_103]]))), const<i32>(25)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s32_30:[0-9]+]] @test_s32_30(%[[VALUE_x_104:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_104]]))), const<i32>(30)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s32_31:[0-9]+]] @test_s32_31(%[[VALUE_x_105:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_105]]))), const<i32>(31)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_u32_24:[0-9]+]] @test_u32_24(%[[VALUE_x_106:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], read<u32>(%[[VALUE_x_106]])), const<i32>(24));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_u32_25:[0-9]+]] @test_u32_25(%[[VALUE_x_107:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], read<u32>(%[[VALUE_x_107]])), const<i32>(25));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_u32_30:[0-9]+]] @test_u32_30(%[[VALUE_x_108:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], read<u32>(%[[VALUE_x_108]])), const<i32>(30));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_u32_31:[0-9]+]] @test_u32_31(%[[VALUE_x_109:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], read<u32>(%[[VALUE_x_109]])), const<i32>(31));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s64_56:[0-9]+]] @test_s64_56(%[[VALUE_x_110:[0-9]+]] x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i64, reason=return, fits=unknown>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_110]]))), const<i32>(56)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s64_57:[0-9]+]] @test_s64_57(%[[VALUE_x_111:[0-9]+]] x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i64, reason=return, fits=unknown>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_111]]))), const<i32>(57)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s64_62:[0-9]+]] @test_s64_62(%[[VALUE_x_112:[0-9]+]] x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i64, reason=return, fits=unknown>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_112]]))), const<i32>(62)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s64_63:[0-9]+]] @test_s64_63(%[[VALUE_x_113:[0-9]+]] x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i64, reason=return, fits=unknown>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_113]]))), const<i32>(63)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_u64_56:[0-9]+]] @test_u64_56(%[[VALUE_x_114:[0-9]+]] x: u64) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], read<u64>(%[[VALUE_x_114]])), const<i32>(56));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_u64_57:[0-9]+]] @test_u64_57(%[[VALUE_x_115:[0-9]+]] x: u64) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], read<u64>(%[[VALUE_x_115]])), const<i32>(57));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_u64_62:[0-9]+]] @test_u64_62(%[[VALUE_x_116:[0-9]+]] x: u64) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], read<u64>(%[[VALUE_x_116]])), const<i32>(62));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_u64_63:[0-9]+]] @test_u64_63(%[[VALUE_x_117:[0-9]+]] x: u64) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], read<u64>(%[[VALUE_x_117]])), const<i32>(63));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s16_8:[0-9]+]] @test_s16_8(%[[VALUE_x_118:[0-9]+]] x: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_bswap16]], reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%[[VALUE_x_118]]))))), const<i32>(8)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s16_9:[0-9]+]] @test_s16_9(%[[VALUE_x_119:[0-9]+]] x: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_bswap16]], reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%[[VALUE_x_119]]))))), const<i32>(9)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s16_14:[0-9]+]] @test_s16_14(%[[VALUE_x_120:[0-9]+]] x: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_bswap16]], reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%[[VALUE_x_120]]))))), const<i32>(14)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_s16_15:[0-9]+]] @test_s16_15(%[[VALUE_x_121:[0-9]+]] x: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_bswap16]], reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%[[VALUE_x_121]]))))), const<i32>(15)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_u16_8:[0-9]+]] @test_u16_8(%[[VALUE_x_122:[0-9]+]] x: u16) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_bswap16]], read<u16>(%[[VALUE_x_122]])))), const<i32>(8))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_u16_9:[0-9]+]] @test_u16_9(%[[VALUE_x_123:[0-9]+]] x: u16) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_bswap16]], read<u16>(%[[VALUE_x_123]])))), const<i32>(9))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_u16_14:[0-9]+]] @test_u16_14(%[[VALUE_x_124:[0-9]+]] x: u16) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_bswap16]], read<u16>(%[[VALUE_x_124]])))), const<i32>(14))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_u16_15:[0-9]+]] @test_u16_15(%[[VALUE_x_125:[0-9]+]] x: u16) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_bswap16]], read<u16>(%[[VALUE_x_125]])))), const<i32>(15))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
