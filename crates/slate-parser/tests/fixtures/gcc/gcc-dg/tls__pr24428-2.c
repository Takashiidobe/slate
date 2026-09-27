/* { dg-do run } */
/* { dg-options "-O2" } */
/* { dg-require-effective-target tls_runtime } */
/* { dg-add-options tls } */

__thread double thrtest[81];
int main ()
{
  double *p, *e;
  e = &thrtest[81];
  for (p = &thrtest[0]; p < e; ++p)
    *p = 1.0;
  return 0;
}

// SLATE-FILECHECK-FLAVOR gcc
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
// DEFAULT-NEXT:     global %0 thrtest: array<f64, 81> [storage=thread] [align=16] [linkage=external];
// DEFAULT-NEXT:     fn %1 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %2 p: ptr<f64> [storage=automatic];
// DEFAULT-NEXT:         let %3 e: ptr<f64> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<f64>>(%3, addr_of<ptr<f64>>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(81)>(%0), const<i32>(81)))));
// DEFAULT-NEXT:         for %4
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<ptr<f64>>(%2, addr_of<ptr<f64>>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(81)>(%0), const<i32>(0)))));
// DEFAULT-NEXT:             condition: lt<ptr<f64>>(read<ptr<f64>>(%2), read<ptr<f64>>(%3))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %5: ptr<f64> [synthetic] = read<ptr<f64>>(%2);
// DEFAULT-NEXT:                 let %6: ptr<f64> [synthetic] = ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%5), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<f64>>(%2, read<ptr<f64>>(%6));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<f64>(deref(read<ptr<f64>>(%2)), const<f64>(1.0));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
