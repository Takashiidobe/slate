/* Portable assumptions */
/* { dg-do run } */
/* { dg-options "-std=c23" } */

int
f1 (int i)
{
  [[gnu::assume (i == 42)]];
  return i;
}

int
f2 (int i)
{
  __attribute__ ((assume (++i == 44)));
  return i;
}

int a;
int *volatile c;

int
f3 ()
{
  ++a;
  return 1;
}

int
f4 (double x)
{
  [[gnu::assume (__builtin_isfinite (x) && x >= 0.0)]];
  return __builtin_isfinite (__builtin_sqrt (x));
}

double
f5 (double x)
{
  __attribute__((assume (__builtin_isfinite (__builtin_sqrt (x)))));
  return __builtin_sqrt (x);
}

int
f6 (int x)
{
  [[gnu::assume (x == 93 ? 1 : 0)]];
  return x;
}

int
main ()
{
  int b = 42;
  double d = 42.0, e = 43.0;
  c = &b;
  [[__gnu__::__assume__ (f3 ())]];
  if (a)
    __builtin_abort ();
  [[gnu::assume (++b == 43)]];
  if (b != 42 || *c != 42)
    __builtin_abort ();
  __attribute__((assume (d < e)));
  int i = 90, j = 91, k = 92;
  [[gnu::__assume__ (i == 90), gnu::assume (j <= 91)]] [[gnu::assume (k >= 92)]]
  ;
  __attribute__((__assume__ (i == 90), assume (j <= 91))) __attribute__((assume (k >= 92)));
  if (f6 (93) != 93)
    __builtin_abort ();
}

// SLATE-FILECHECK-STD DEFAULT c23
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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: volatile ptr<i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f1:[0-9]+]] @f1(%[[VALUE_i:[0-9]+]] i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f2:[0-9]+]] @f2(%[[VALUE_i_2:[0-9]+]] i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f3:[0-9]+]] @f3() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE0]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE1]]));
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_sqrt:[0-9]+]] @__builtin_sqrt(%[[VALUE2:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f4:[0-9]+]] @f4(%[[VALUE_x:[0-9]+]] x: f64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(float_class<bool, test=finite>(call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_sqrt]], read<f64>(%[[VALUE_x]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f5:[0-9]+]] @f5(%[[VALUE_x_2:[0-9]+]] x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_sqrt]], read<f64>(%[[VALUE_x_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f6:[0-9]+]] @f6(%[[VALUE_x_3:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_x_3]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: i32 [storage=automatic] = const<i32>(42);
// DEFAULT-NEXT:         let %[[VALUE_d:[0-9]+]] d: f64 [storage=automatic] = const<f64>(42.0);
// DEFAULT-NEXT:         let %[[VALUE_e:[0-9]+]] e: f64 [storage=automatic] = const<f64>(43.0);
// DEFAULT-NEXT:         write<ptr<i32>, volatile>(%[[VALUE_c]], addr_of<ptr<i32>>(%[[VALUE_b]]));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_a]]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(%[[VALUE_b]]), const<i32>(42)), ne<i32>(read<i32>(deref(read<ptr<i32>, volatile>(%[[VALUE_c]]))), const<i32>(42)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         let %[[VALUE_i_3:[0-9]+]] i: i32 [storage=automatic] = const<i32>(90);
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic] = const<i32>(91);
// DEFAULT-NEXT:         let %[[VALUE_k:[0-9]+]] k: i32 [storage=automatic] = const<i32>(92);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_f6]], const<i32>(93)), const<i32>(93))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
