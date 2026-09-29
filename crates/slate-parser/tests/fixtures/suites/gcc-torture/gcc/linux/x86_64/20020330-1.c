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
// DEFAULT-NEXT:     fn %[[VALUE_baz:[0-9]+]] @baz(%[[VALUE0:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_w:[0-9]+]] w: i64, %[[VALUE_x:[0-9]+]] x: i64, %[[VALUE_y:[0-9]+]] y: i64, %[[VALUE_z:[0-9]+]] z: i64) -> ptr<ptr<f64>> [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: i64 [storage=automatic] = add<i64, overflow=ub>(sub<i64, overflow=ub>(read<i64>(%[[VALUE_x]]), read<i64>(%[[VALUE_w]])), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: i64 [storage=automatic] = add<i64, overflow=ub>(sub<i64, overflow=ub>(read<i64>(%[[VALUE_z]]), read<i64>(%[[VALUE_y]])), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE_m:[0-9]+]] m: ptr<ptr<f64>> [storage=automatic] = pointer_cast<ptr<ptr<f64>>, reason=explicit>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_baz]], mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(add<i64, overflow=ub>(read<i64>(%[[VALUE_a]]), widen<i64, reason=usual_arith>(const<i32>(1)))))));
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: ptr<ptr<f64>> [synthetic] = read<ptr<ptr<f64>>>(%[[VALUE_m]]);
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: ptr<ptr<f64>> [synthetic] = ptr_offset<ptr<ptr<f64>>, subtract=false, element=ptr<f64>, overflow=ub>(read<ptr<ptr<f64>>>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<ptr<f64>>>(%[[VALUE_m]], read<ptr<ptr<f64>>>(%[[VALUE2]]));
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: ptr<ptr<f64>> [synthetic] = read<ptr<ptr<f64>>>(%[[VALUE_m]]);
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: ptr<ptr<f64>> [synthetic] = ptr_offset<ptr<ptr<f64>>, subtract=true, element=ptr<f64>, overflow=ub>(read<ptr<ptr<f64>>>(%[[VALUE3]]), read<i64>(%[[VALUE_w]]));
// DEFAULT-NEXT:         write<ptr<ptr<f64>>>(%[[VALUE_m]], read<ptr<ptr<f64>>>(%[[VALUE4]]));
// DEFAULT-NEXT:         write<ptr<f64>>(deref(ptr_offset<ptr<ptr<f64>>, subtract=false, element=ptr<f64>, overflow=ub>(read<ptr<ptr<f64>>>(%[[VALUE_m]]), read<i64>(%[[VALUE_w]]))), pointer_cast<ptr<f64>, reason=explicit>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_baz]], mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(add<i64, overflow=ub>(mul<i64, overflow=ub>(read<i64>(%[[VALUE_a]]), read<i64>(%[[VALUE_b]])), widen<i64, reason=usual_arith>(const<i32>(1))))))));
// DEFAULT-NEXT:         pointer_cast<ptr<f64>, reason=explicit>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_baz]], mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(add<i64, overflow=ub>(mul<i64, overflow=ub>(read<i64>(%[[VALUE_a]]), read<i64>(%[[VALUE_b]])), widen<i64, reason=usual_arith>(const<i32>(1)))))));
// DEFAULT-NEXT:         for %[[VALUE5:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i64>(%[[VALUE_i]], add<i64, overflow=ub>(read<i64>(%[[VALUE_w]]), widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:             condition: le<i64>(read<i64>(%[[VALUE_i]]), read<i64>(%[[VALUE_x]]))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE6]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(%[[VALUE_i]], read<i64>(%[[VALUE7]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<ptr<f64>>(deref(ptr_offset<ptr<ptr<f64>>, subtract=false, element=ptr<f64>, overflow=ub>(read<ptr<ptr<f64>>>(%[[VALUE_m]]), read<i64>(%[[VALUE_i]]))), ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(deref(ptr_offset<ptr<ptr<f64>>, subtract=false, element=ptr<f64>, overflow=ub>(read<ptr<ptr<f64>>>(%[[VALUE_m]]), sub<i64, overflow=ub>(read<i64>(%[[VALUE_i]]), widen<i64, reason=usual_arith>(const<i32>(1)))))), read<i64>(%[[VALUE_b]])));
// DEFAULT-NEXT:         return read<ptr<ptr<f64>>>(%[[VALUE_m]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_w_2:[0-9]+]] w: ptr<f64>, %[[VALUE_x_2:[0-9]+]] x: i32, %[[VALUE_y_2:[0-9]+]] y: ptr<f64>, %[[VALUE_z_2:[0-9]+]] z: ptr<f64>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i_2:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a_2:[0-9]+]] a: ptr<ptr<f64>> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<ptr<f64>>>(%[[VALUE_a_2]], call<ptr<ptr<f64>>, signature=fn(i64, i64, i64, i64) -> ptr<ptr<f64>>>(%[[VALUE_bar]], widen<i64, reason=arg>(const<i32>(1)), widen<i64, reason=arg>(const<i32>(50)), widen<i64, reason=arg>(const<i32>(1)), widen<i64, reason=arg>(const<i32>(50))));
// DEFAULT-NEXT:         call<ptr<ptr<f64>>, signature=fn(i64, i64, i64, i64) -> ptr<ptr<f64>>>(%[[VALUE_bar]], widen<i64, reason=arg>(const<i32>(1)), widen<i64, reason=arg>(const<i32>(50)), widen<i64, reason=arg>(const<i32>(1)), widen<i64, reason=arg>(const<i32>(50)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
