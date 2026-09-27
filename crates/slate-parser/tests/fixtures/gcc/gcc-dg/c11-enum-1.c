/* Test C23 enumerations with values not representable in int are diagnosed for
   C11.  */
/* { dg-do compile } */
/* { dg-options "-std=c11 -pedantic-errors" } */

enum e1 { e1a = -__LONG_LONG_MAX__ - 1 }; /* { dg-error "ISO C restricts enumerator values" } */

enum e2 { e2a = __LONG_LONG_MAX__ }; /* { dg-error "ISO C restricts enumerator values" } */

enum e3 { e3a = (unsigned int) -1 }; /* { dg-error "ISO C restricts enumerator values" } */

enum e4 { e4a = (long long) -__INT_MAX__ - 1, e4b = (unsigned int) __INT_MAX__ };

enum e5 { e5a = __INT_MAX__, e5b }; /* { dg-error "ISO C restricts enumerator values" } */

// SLATE-FILECHECK-FLAVOR gcc
// SLATE-FILECHECK-STD DEFAULT c11
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
// DEFAULT-NEXT:     type @type0 e1 = enum : i64 {
// DEFAULT-NEXT:         %0 e1a = const<i64>(-9223372036854775808);
// DEFAULT-NEXT:     } [size=8, align=8];
// DEFAULT-NEXT:     type @type1 e2 = enum : u64 {
// DEFAULT-NEXT:         %0 e2a = const<u64>(9223372036854775807);
// DEFAULT-NEXT:     } [size=8, align=8];
// DEFAULT-NEXT:     type @type2 e3 = enum : u32 {
// DEFAULT-NEXT:         %0 e3a = const<u32>(4294967295);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type3 e4 = enum : i32 {
// DEFAULT-NEXT:         %0 e4a = const<i32>(-2147483648);
// DEFAULT-NEXT:         %1 e4b = const<i32>(2147483647);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type4 e5 = enum : u32 {
// DEFAULT-NEXT:         %0 e5a = const<u32>(2147483647);
// DEFAULT-NEXT:         %1 e5b = const<u32>(2147483648);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
