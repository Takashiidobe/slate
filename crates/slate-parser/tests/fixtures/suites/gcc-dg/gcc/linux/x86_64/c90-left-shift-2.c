/* { dg-do compile } */
/* { dg-options "-std=iso9899:1990 -pedantic-errors" } */

#define INTM2 (sizeof (int) * __CHAR_BIT__ - 2)

enum { A = 10 << INTM2 };
int k = 10 << INTM2;

void
fn (int i)
{
  switch (i)
  case 10 << INTM2: break;
}

// SLATE-FILECHECK-STD DEFAULT iso9899:1990
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
// DEFAULT-NEXT:     type @type0 = enum : i32 {
// DEFAULT-NEXT:         %0 A = const<i32>(-2147483648);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     global %2 k: i32 [storage=static] = shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(10), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))) [linkage=external];
// DEFAULT-NEXT:     fn %3 @fn(%4 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         switch %5 read<i32>(%4)
// DEFAULT-NEXT:             case %5 const<i32>(-2147483648):
// DEFAULT-NEXT:                 break %5;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
