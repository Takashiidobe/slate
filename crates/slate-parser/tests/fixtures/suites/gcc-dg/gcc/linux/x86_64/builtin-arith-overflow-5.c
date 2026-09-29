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
// DEFAULT-NEXT:     fn %[[VALUE_f1:[0-9]+]] @f1(%[[VALUE_a:[0-9]+]] a: i32, %[[VALUE_b:[0-9]+]] b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: u64 [storage=automatic];
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(overflow_mul<bool>(read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]]), deref(addr_of<ptr<u64>>(%[[VALUE_c]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f2:[0-9]+]] @f2(%[[VALUE_a_2:[0-9]+]] a: i32, %[[VALUE_b_2:[0-9]+]] b: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_c_2:[0-9]+]] c: u64 [storage=automatic];
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(overflow_mul<bool>(read<i32>(%[[VALUE_a_2]]), read<u32>(%[[VALUE_b_2]]), deref(addr_of<ptr<u64>>(%[[VALUE_c_2]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f3:[0-9]+]] @f3(%[[VALUE_a_3:[0-9]+]] a: u32, %[[VALUE_b_3:[0-9]+]] b: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_c_3:[0-9]+]] c: i64 [storage=automatic];
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(overflow_mul<bool>(read<u32>(%[[VALUE_a_3]]), read<u32>(%[[VALUE_b_3]]), deref(addr_of<ptr<i64>>(%[[VALUE_c_3]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f4:[0-9]+]] @f4(%[[VALUE_a_4:[0-9]+]] a: i32, %[[VALUE_b_4:[0-9]+]] b: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_c_4:[0-9]+]] c: i64 [storage=automatic];
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(overflow_mul<bool>(read<i32>(%[[VALUE_a_4]]), read<u32>(%[[VALUE_b_4]]), deref(addr_of<ptr<i64>>(%[[VALUE_c_4]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f5:[0-9]+]] @f5(%[[VALUE_a_5:[0-9]+]] a: i16, %[[VALUE_b_5:[0-9]+]] b: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_c_5:[0-9]+]] c: u32 [storage=automatic];
// DEFAULT-NEXT:         return from_bool<i16, reason=return>(overflow_mul<bool>(read<i16>(%[[VALUE_a_5]]), read<i16>(%[[VALUE_b_5]]), deref(addr_of<ptr<u32>>(%[[VALUE_c_5]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f6:[0-9]+]] @f6(%[[VALUE_a_6:[0-9]+]] a: i16, %[[VALUE_b_6:[0-9]+]] b: u16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_c_6:[0-9]+]] c: u32 [storage=automatic];
// DEFAULT-NEXT:         return from_bool<i16, reason=return>(overflow_mul<bool>(read<i16>(%[[VALUE_a_6]]), read<u16>(%[[VALUE_b_6]]), deref(addr_of<ptr<u32>>(%[[VALUE_c_6]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f7:[0-9]+]] @f7(%[[VALUE_a_7:[0-9]+]] a: u16, %[[VALUE_b_7:[0-9]+]] b: u16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_c_7:[0-9]+]] c: i32 [storage=automatic];
// DEFAULT-NEXT:         return from_bool<i16, reason=return>(overflow_mul<bool>(read<u16>(%[[VALUE_a_7]]), read<u16>(%[[VALUE_b_7]]), deref(addr_of<ptr<i32>>(%[[VALUE_c_7]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f8:[0-9]+]] @f8(%[[VALUE_a_8:[0-9]+]] a: i16, %[[VALUE_b_8:[0-9]+]] b: u16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_c_8:[0-9]+]] c: i32 [storage=automatic];
// DEFAULT-NEXT:         return from_bool<i16, reason=return>(overflow_mul<bool>(read<i16>(%[[VALUE_a_8]]), read<u16>(%[[VALUE_b_8]]), deref(addr_of<ptr<i32>>(%[[VALUE_c_8]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f9:[0-9]+]] @f9(%[[VALUE_a_9:[0-9]+]] a: i8, %[[VALUE_b_9:[0-9]+]] b: i8) -> i8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_c_9:[0-9]+]] c: u16 [storage=automatic];
// DEFAULT-NEXT:         return from_bool<i8, reason=return>(overflow_mul<bool>(read<i8>(%[[VALUE_a_9]]), read<i8>(%[[VALUE_b_9]]), deref(addr_of<ptr<u16>>(%[[VALUE_c_9]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f10:[0-9]+]] @f10(%[[VALUE_a_10:[0-9]+]] a: i8, %[[VALUE_b_10:[0-9]+]] b: u8) -> i8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_c_10:[0-9]+]] c: u16 [storage=automatic];
// DEFAULT-NEXT:         return from_bool<i8, reason=return>(overflow_mul<bool>(read<i8>(%[[VALUE_a_10]]), read<u8>(%[[VALUE_b_10]]), deref(addr_of<ptr<u16>>(%[[VALUE_c_10]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f11:[0-9]+]] @f11(%[[VALUE_a_11:[0-9]+]] a: u8, %[[VALUE_b_11:[0-9]+]] b: u8) -> i8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_c_11:[0-9]+]] c: i16 [storage=automatic];
// DEFAULT-NEXT:         return from_bool<i8, reason=return>(overflow_mul<bool>(read<u8>(%[[VALUE_a_11]]), read<u8>(%[[VALUE_b_11]]), deref(addr_of<ptr<i16>>(%[[VALUE_c_11]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f12:[0-9]+]] @f12(%[[VALUE_a_12:[0-9]+]] a: i8, %[[VALUE_b_12:[0-9]+]] b: u8) -> i8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_c_12:[0-9]+]] c: i16 [storage=automatic];
// DEFAULT-NEXT:         return from_bool<i8, reason=return>(overflow_mul<bool>(read<i8>(%[[VALUE_a_12]]), read<u8>(%[[VALUE_b_12]]), deref(addr_of<ptr<i16>>(%[[VALUE_c_12]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
