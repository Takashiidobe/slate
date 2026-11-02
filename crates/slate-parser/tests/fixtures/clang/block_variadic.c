// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ARGS -fblocks --dump-ir --compact-ir

typedef __builtin_va_list va_list;

int (^sum)(int, ...) = ^(int n, ...) {
  va_list ap;
  __builtin_va_start(ap, n);
  int s = 0;
  for (int i = 0; i < n; i++)
    s += __builtin_va_arg(ap, int);
  __builtin_va_end(ap);
  return s;
};

int call(int (^f)(int, ...)) { return f(2, (char)1, (short)2); }

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
// DEFAULT-NEXT:     type @type[[TYPE_va_list:[0-9]+]] va_list = va_list;
// DEFAULT-NEXT:     global %[[VALUE_sum:[0-9]+]] sum: block<fn(i32, ...) -> i32> [storage=static] = block_literal<block<fn(i32, ...) -> i32>, captures=[], statements=6> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_call:[0-9]+]] @call(%[[VALUE_f:[0-9]+]] f: block<fn(i32, ...) -> i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32>(read<block<fn(i32, ...) -> i32>>(%[[VALUE_f]]), const<i32>(2), widen<i32>(truncate<i8>(const<i32>(1))), widen<i32>(truncate<i16>(const<i32>(2))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
