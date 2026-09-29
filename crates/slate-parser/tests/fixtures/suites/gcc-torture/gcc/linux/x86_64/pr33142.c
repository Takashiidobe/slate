int         abs(int j);
extern void abort(void);

__attribute__((noinline)) int lisp_atan2(long dy, long dx) {
  if (dx <= 0)
    if (dy > 0)
      return abs(dx) <= abs(dy);
  return 0;
}

int main() {
  volatile long dy = 63, dx = -77;
  if (lisp_atan2(dy, dx))
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
// DEFAULT-NEXT:     fn %[[VALUE_abs:[0-9]+]] @abs(%[[VALUE_j:[0-9]+]] j: i32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_lisp_atan2:[0-9]+]] @lisp_atan2(%[[VALUE_dy:[0-9]+]] dy: i64, %[[VALUE_dx:[0-9]+]] dx: i64) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if le<i64>(read<i64>(%[[VALUE_dx]]), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             if gt<i64>(read<i64>(%[[VALUE_dy]]), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                 return from_bool<i32, reason=return>(le<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_abs]], truncate<i32, reason=arg, fits=unknown>(read<i64>(%[[VALUE_dx]]))), call<i32, signature=fn(i32) -> i32>(%[[VALUE_abs]], truncate<i32, reason=arg, fits=unknown>(read<i64>(%[[VALUE_dy]])))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_dy_2:[0-9]+]] dy: volatile i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(63));
// DEFAULT-NEXT:         let %[[VALUE_dx_2:[0-9]+]] dx: volatile i64 [storage=automatic] = widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(77)));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64, i64) -> i32>(%[[VALUE_lisp_atan2]], read<i64, volatile>(%[[VALUE_dy_2]]), read<i64, volatile>(%[[VALUE_dx_2]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
