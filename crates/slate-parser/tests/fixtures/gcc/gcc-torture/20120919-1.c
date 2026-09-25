/* PR rtl-optimization/54290 */
/* Testcase by Eric Volk <eriksnga@gmail.com> */
/* { dg-require-effective-target int32plus } */

double  vd[2] = {1., 0.};
int     vi[2] = {1234567890, 0};
double *pd    = vd;
int    *pi    = vi;

extern void abort(void);

void init(int *n, int *dummy) __attribute__((noinline, noclone));

void init(int *n, int *dummy) {
  if (0 == n)
    dummy[0] = 0;
}

int main(void) {
  int dummy[1532];
  int i = -1, n = 1, s = 0;
  init(&n, dummy);
  while (i < n) {
    if (i == 0) {
      if (pd[i] > 0) {
        if (pi[i] > 0) {
          s += pi[i];
        }
      }
      pd[i] = pi[i];
    }
    ++i;
  }
  if (s != 1234567890)
    abort();
  return 0;
}



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
// DEFAULT-NEXT:     global %0 vd: array<f64, 2> [storage=static] = aggregate<array<f64, 2>, zero_fill=false>(index0 = const<f64>(1.0), index1 = const<f64>(0.0)) [linkage=external];
// DEFAULT-NEXT:     global %1 vi: array<i32, 2> [storage=static] = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(1234567890), index1 = const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     global %2 pd: ptr<f64> [storage=static] = array_decay<ptr<f64>, length=Some(2)>(%0) [linkage=external];
// DEFAULT-NEXT:     global %3 pi: ptr<i32> [storage=static] = array_decay<ptr<i32>, length=Some(2)>(%1) [linkage=external];
// DEFAULT-NEXT:     fn %4 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %5 @init(%6 n: ptr<i32>, %7 dummy: ptr<i32>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<ptr<i32>>(null<ptr<i32>>, read<ptr<i32>>(%6))
// DEFAULT-NEXT:             write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%7), const<i32>(0))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %9 dummy: array<i32, 1532> [storage=automatic];
// DEFAULT-NEXT:         let %10 i: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:         let %11 n: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         let %12 s: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>, ptr<i32>) -> void>(%5, addr_of<ptr<i32>>(%11), array_decay<ptr<i32>, length=Some(1532)>(%9));
// DEFAULT-NEXT:         while %15 lt<i32>(read<i32>(%10), read<i32>(%11))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if eq<i32>(read<i32>(%10), const<i32>(0))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if gt<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%2), read<i32>(%10)))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 if gt<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%3), read<i32>(%10)))), const<i32>(0))
// DEFAULT-NEXT:                                     {
// DEFAULT-NEXT:                                         let %16: i32 [synthetic] = read<i32>(%12);
// DEFAULT-NEXT:                                         let %17: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%16), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%3), read<i32>(%10)))));
// DEFAULT-NEXT:                                         write<i32>(%12, read<i32>(%17));
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%2), read<i32>(%10))), int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%3), read<i32>(%10))))));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 let %18: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                 let %19: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%18), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%10, read<i32>(%19));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%12), const<i32>(1234567890))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
