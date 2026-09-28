/* PR rtl-optimization/95862 */
/* { dg-do compile } */
/* { dg-options "-O2" } */

int
f1 (int a, int b)
{
  unsigned long long c;
  return __builtin_mul_overflow (a, b, &c);
}

int
f2 (int a, unsigned b)
{
  unsigned long long c;
  return __builtin_mul_overflow (a, b, &c);
}

int
f3 (unsigned a, unsigned b)
{
  long long c;
  return __builtin_mul_overflow (a, b, &c);
}

int
f4 (int a, unsigned b)
{
  long long c;
  return __builtin_mul_overflow (a, b, &c);
}

short
f5 (short a, short b)
{
  unsigned c;
  return __builtin_mul_overflow (a, b, &c);
}

short
f6 (short a, unsigned short b)
{
  unsigned c;
  return __builtin_mul_overflow (a, b, &c);
}

short
f7 (unsigned short a, unsigned short b)
{
  int c;
  return __builtin_mul_overflow (a, b, &c);
}

short
f8 (short a, unsigned short b)
{
  int c;
  return __builtin_mul_overflow (a, b, &c);
}

signed char
f9 (signed char a, signed char b)
{
  unsigned short c;
  return __builtin_mul_overflow (a, b, &c);
}

signed char
f10 (signed char a, unsigned char b)
{
  unsigned short c;
  return __builtin_mul_overflow (a, b, &c);
}

signed char
f11 (unsigned char a, unsigned char b)
{
  short c;
  return __builtin_mul_overflow (a, b, &c);
}

signed char
f12 (signed char a, unsigned char b)
{
  short c;
  return __builtin_mul_overflow (a, b, &c);
}

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
// DEFAULT-NEXT:     fn %0 @f1(%1 a: i32, %2 b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 c: u64 [storage=automatic];
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(overflow_mul<bool>(read<i32>(%1), read<i32>(%2), deref(addr_of<ptr<u64>>(%3))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @f2(%5 a: i32, %6 b: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %7 c: u64 [storage=automatic];
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(overflow_mul<bool>(read<i32>(%5), read<u32>(%6), deref(addr_of<ptr<u64>>(%7))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @f3(%9 a: u32, %10 b: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %11 c: i64 [storage=automatic];
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(overflow_mul<bool>(read<u32>(%9), read<u32>(%10), deref(addr_of<ptr<i64>>(%11))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @f4(%13 a: i32, %14 b: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %15 c: i64 [storage=automatic];
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(overflow_mul<bool>(read<i32>(%13), read<u32>(%14), deref(addr_of<ptr<i64>>(%15))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @f5(%17 a: i16, %18 b: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %19 c: u32 [storage=automatic];
// DEFAULT-NEXT:         return from_bool<i16, reason=return>(overflow_mul<bool>(read<i16>(%17), read<i16>(%18), deref(addr_of<ptr<u32>>(%19))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @f6(%21 a: i16, %22 b: u16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %23 c: u32 [storage=automatic];
// DEFAULT-NEXT:         return from_bool<i16, reason=return>(overflow_mul<bool>(read<i16>(%21), read<u16>(%22), deref(addr_of<ptr<u32>>(%23))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %24 @f7(%25 a: u16, %26 b: u16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %27 c: i32 [storage=automatic];
// DEFAULT-NEXT:         return from_bool<i16, reason=return>(overflow_mul<bool>(read<u16>(%25), read<u16>(%26), deref(addr_of<ptr<i32>>(%27))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %28 @f8(%29 a: i16, %30 b: u16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %31 c: i32 [storage=automatic];
// DEFAULT-NEXT:         return from_bool<i16, reason=return>(overflow_mul<bool>(read<i16>(%29), read<u16>(%30), deref(addr_of<ptr<i32>>(%31))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %32 @f9(%33 a: i8, %34 b: i8) -> i8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %35 c: u16 [storage=automatic];
// DEFAULT-NEXT:         return from_bool<i8, reason=return>(overflow_mul<bool>(read<i8>(%33), read<i8>(%34), deref(addr_of<ptr<u16>>(%35))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %36 @f10(%37 a: i8, %38 b: u8) -> i8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %39 c: u16 [storage=automatic];
// DEFAULT-NEXT:         return from_bool<i8, reason=return>(overflow_mul<bool>(read<i8>(%37), read<u8>(%38), deref(addr_of<ptr<u16>>(%39))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %40 @f11(%41 a: u8, %42 b: u8) -> i8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %43 c: i16 [storage=automatic];
// DEFAULT-NEXT:         return from_bool<i8, reason=return>(overflow_mul<bool>(read<u8>(%41), read<u8>(%42), deref(addr_of<ptr<i16>>(%43))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %44 @f12(%45 a: i8, %46 b: u8) -> i8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %47 c: i16 [storage=automatic];
// DEFAULT-NEXT:         return from_bool<i8, reason=return>(overflow_mul<bool>(read<i8>(%45), read<u8>(%46), deref(addr_of<ptr<i16>>(%47))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
