// SLATE-FILECHECK-DEFINES DEFAULT

/* PR 5446 */
/* This testcase is similar to gcc.c-torture/compile/20011219-1.c except
   with parts of it omitted, causing an ICE with -O3 on IA-64.  */

void * baz (unsigned long);
static inline double **
bar (long w, long x, long y, long z)
{
  long i, a = x - w + 1, b = z - y + 1;
  double **m = (double **) baz (sizeof (double *) * (a + 1));

  m += 1;
  m -= w;
  m[w] = (double *) baz (sizeof (double) * (a * b + 1));
  for (i = w + 1; i <= x; i++)
    m[i] = m[i - 1] + b;
  return m;
}

void
foo (double w[], int x, double y[], double z[])
{
  int i;
  double **a;

  a = bar (1, 50, 1, 50);
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
// DEFAULT-NEXT:     fn %0 @baz(%17 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %1 @bar(%2 w: i64, %3 x: i64, %4 y: i64, %5 z: i64) -> ptr<ptr<f64>> [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %6 i: i64 [storage=automatic];
// DEFAULT-NEXT:         let %7 a: i64 [storage=automatic] = add<i64, overflow=ub>(sub<i64, overflow=ub>(read<i64>(%3), read<i64>(%2)), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         let %8 b: i64 [storage=automatic] = add<i64, overflow=ub>(sub<i64, overflow=ub>(read<i64>(%5), read<i64>(%4)), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         let %9 m: ptr<ptr<f64>> [storage=automatic] = pointer_cast<ptr<ptr<f64>>, reason=explicit>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%0, mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(add<i64, overflow=ub>(read<i64>(%7), widen<i64, reason=usual_arith>(const<i32>(1)))))));
// DEFAULT-NEXT:         let %19: ptr<ptr<f64>> [synthetic] = read<ptr<ptr<f64>>>(%9);
// DEFAULT-NEXT:         let %20: ptr<ptr<f64>> [synthetic] = ptr_offset<ptr<ptr<f64>>, subtract=false, element=ptr<f64>, overflow=ub>(read<ptr<ptr<f64>>>(%19), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<ptr<f64>>>(%9, read<ptr<ptr<f64>>>(%20));
// DEFAULT-NEXT:         let %21: ptr<ptr<f64>> [synthetic] = read<ptr<ptr<f64>>>(%9);
// DEFAULT-NEXT:         let %22: ptr<ptr<f64>> [synthetic] = ptr_offset<ptr<ptr<f64>>, subtract=true, element=ptr<f64>, overflow=ub>(read<ptr<ptr<f64>>>(%21), read<i64>(%2));
// DEFAULT-NEXT:         write<ptr<ptr<f64>>>(%9, read<ptr<ptr<f64>>>(%22));
// DEFAULT-NEXT:         write<ptr<f64>>(deref(ptr_offset<ptr<ptr<f64>>, subtract=false, element=ptr<f64>, overflow=ub>(read<ptr<ptr<f64>>>(%9), read<i64>(%2))), pointer_cast<ptr<f64>, reason=explicit>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%0, mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(add<i64, overflow=ub>(mul<i64, overflow=ub>(read<i64>(%7), read<i64>(%8)), widen<i64, reason=usual_arith>(const<i32>(1))))))));
// DEFAULT-NEXT:         pointer_cast<ptr<f64>, reason=explicit>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%0, mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(add<i64, overflow=ub>(mul<i64, overflow=ub>(read<i64>(%7), read<i64>(%8)), widen<i64, reason=usual_arith>(const<i32>(1)))))));
// DEFAULT-NEXT:         for %18
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i64>(%6, add<i64, overflow=ub>(read<i64>(%2), widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:             condition: le<i64>(read<i64>(%6), read<i64>(%3))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %23: i64 [synthetic] = read<i64>(%6);
// DEFAULT-NEXT:                 let %24: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%23), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(%6, read<i64>(%24));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<ptr<f64>>(deref(ptr_offset<ptr<ptr<f64>>, subtract=false, element=ptr<f64>, overflow=ub>(read<ptr<ptr<f64>>>(%9), read<i64>(%6))), ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(deref(ptr_offset<ptr<ptr<f64>>, subtract=false, element=ptr<f64>, overflow=ub>(read<ptr<ptr<f64>>>(%9), sub<i64, overflow=ub>(read<i64>(%6), widen<i64, reason=usual_arith>(const<i32>(1)))))), read<i64>(%8)));
// DEFAULT-NEXT:         return read<ptr<ptr<f64>>>(%9);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @foo(%11 w: ptr<f64>, %12 x: i32, %13 y: ptr<f64>, %14 z: ptr<f64>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %15 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %16 a: ptr<ptr<f64>> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<ptr<f64>>>(%16, call<ptr<ptr<f64>>, signature=fn(i64, i64, i64, i64) -> ptr<ptr<f64>>>(%1, widen<i64, reason=arg>(const<i32>(1)), widen<i64, reason=arg>(const<i32>(50)), widen<i64, reason=arg>(const<i32>(1)), widen<i64, reason=arg>(const<i32>(50))));
// DEFAULT-NEXT:         call<ptr<ptr<f64>>, signature=fn(i64, i64, i64, i64) -> ptr<ptr<f64>>>(%1, widen<i64, reason=arg>(const<i32>(1)), widen<i64, reason=arg>(const<i32>(50)), widen<i64, reason=arg>(const<i32>(1)), widen<i64, reason=arg>(const<i32>(50)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
