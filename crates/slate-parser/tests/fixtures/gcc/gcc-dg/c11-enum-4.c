/* Test C23 enumerations with fixed underlying type are diagnosed for C11.  */
/* { dg-do compile } */
/* { dg-options "-std=c11 -pedantic-errors" } */

enum e1 : int; /* { dg-error "ISO C does not support specifying 'enum' underlying types" } */
enum e2 : short { A }; /* { dg-error "ISO C does not support specifying 'enum' underlying types" } */
enum : short { B }; /* { dg-error "ISO C does not support specifying 'enum' underlying types" } */

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
// DEFAULT-NEXT:     type @type0 e1 = enum : i32 incomplete [size=4, align=4];
// DEFAULT-NEXT:     type @type1 e2 = enum : i16 {
// DEFAULT-NEXT:         %0 A = const<i16>(0);
// DEFAULT-NEXT:     } [size=2, align=2];
// DEFAULT-NEXT:     type @type2 = enum : i16 {
// DEFAULT-NEXT:         %0 B = const<i16>(0);
// DEFAULT-NEXT:     } [size=2, align=2];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
