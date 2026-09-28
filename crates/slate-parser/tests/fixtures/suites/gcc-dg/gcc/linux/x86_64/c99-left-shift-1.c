/* { dg-do compile } */
/* { dg-options "-std=iso9899:1999 -pedantic-errors" } */

enum E { A = -2 << 1 }; /* { dg-error "constant expression" } */
int i = -1 << 0; /* { dg-error "constant expression" } */

int
f (int i)
{
  switch (i)
  case -1 << 0: break; /* { dg-error "constant expression" } */
}

// SLATE-FILECHECK-STD DEFAULT iso9899:1999
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
// DEFAULT-NEXT:     type @type0 E = enum : i32 {
// DEFAULT-NEXT:         %0 A = const<i32>(-4);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     global %2 i: i32 [storage=static] = shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     fn %3 @f(%4 i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         switch %5 read<i32>(%4)
// DEFAULT-NEXT:             case %5 const<i32>(-1):
// DEFAULT-NEXT:                 break %5;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
