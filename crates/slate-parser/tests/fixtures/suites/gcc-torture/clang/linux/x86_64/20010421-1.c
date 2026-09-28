// SLATE-FILECHECK-DEFINES DEFAULT

int j;

void residual ()
{
  long double s;
  for (j = 3; j < 9; j++)
    s -= 3;
}

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
// DEFAULT-NEXT:     global %0 j: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %1 @residual() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %2 s: f80 [storage=automatic];
// DEFAULT-NEXT:         for %3
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%0, const<i32>(3));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%0), const<i32>(9))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %4: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                 let %5: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%4), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%0, read<i32>(%5));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %6: f80 [synthetic] = read<f80>(%2);
// DEFAULT-NEXT:                 let %7: f80 [synthetic] = sub<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%6), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(3)));
// DEFAULT-NEXT:                 write<f80>(%2, read<f80>(%7));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
