/* { dg-do run } */
/* Tail call optimizations would convert func() into the moral equivalent of:

       double acc = 0.0;
       for (int i = 0; i <= n; i++)
         acc += d;
       return acc;

   which mishandles the case where 'd' is -0.  They also initialised 'acc'
   to a zero int rather than a zero double.  */

void abort(void);
void exit(int);

double func(double d, int n) {
  if (n == 0)
    return d;
  else
    return d + func(d, n - 1);
}

int main() {
  if (__builtin_copysign(1.0, func(0.0 / -5.0, 10)) != -1.0)
    abort();
  exit(0);
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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_func:[0-9]+]] @func(%[[VALUE_d:[0-9]+]] d: f64, %[[VALUE_n:[0-9]+]] n: i32) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%[[VALUE_n]]), const<i32>(0))
// DEFAULT-NEXT:             return read<f64>(%[[VALUE_d]]);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             return add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE_d]]), call<f64, signature=fn(f64, i32) -> f64>(%[[VALUE_func]], read<f64>(%[[VALUE_d]]), sub<i32, overflow=ub>(read<i32>(%[[VALUE_n]]), const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_copysign:[0-9]+]] @__builtin_copysign(%[[VALUE1:[0-9]+]] <unnamed>: f64, %[[VALUE2:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE___builtin_copysign]], const<f64>(1.0), call<f64, signature=fn(f64, i32) -> f64>(%[[VALUE_func]], div<f64, rounding=nearest_even, exceptions=observable, contract=fast>(const<f64>(0.0), neg<f64>(const<f64>(5.0))), const<i32>(10))), neg<f64>(const<f64>(1.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
