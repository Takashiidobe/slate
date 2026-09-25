// SLATE-FILECHECK-DEFINES DEFAULT

/* PR optimization/8334 */
/* Verify that GCC produces valid operands
   after simplifying an addition. */

void foo(int m, int n, double *f)
{
  int i, j, k = 1;

  for (j = 0; j < n; j++) {
    for (i = k; i < m; i++) {
      f[i] = (double) (i * j);
      f[i + j] = (double) ((i + 1) * j);
    }
  }
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
// DEFAULT-NEXT:     fn %0 @foo(%1 m: i32, %2 n: i32, %3 f: ptr<f64>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %4 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %5 j: i32 [storage=automatic];
// DEFAULT-NEXT:         let %6 k: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         for %7
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%5, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%5), read<i32>(%2))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %9: i32 [synthetic] = read<i32>(%5);
// DEFAULT-NEXT:                 let %10: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%9), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%5, read<i32>(%10));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     for %8
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             write<i32>(%4, read<i32>(%6));
// DEFAULT-NEXT:                         condition: lt<i32>(read<i32>(%4), read<i32>(%1))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %11: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:                             let %12: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%11), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%4, read<i32>(%12));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%3), read<i32>(%4))), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(mul<i32, overflow=ub>(read<i32>(%4), read<i32>(%5))));
// DEFAULT-NEXT:                                 write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%3), add<i32, overflow=ub>(read<i32>(%4), read<i32>(%5)))), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(mul<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(%4), const<i32>(1)), read<i32>(%5))));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
