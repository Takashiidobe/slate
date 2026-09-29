/* PR tree-optimization/98727 */

__attribute__((noipa)) long int foo(long int x, long int y) {
  long int z = (unsigned long)x * y;
  if (x != z / y)
    return -1;
  return z;
}

int main() {
  if (foo(4, 24) != 96 || foo(124, 126) != 124L * 126 ||
      foo(__LONG_MAX__ / 16, 17) != -1)
    __builtin_abort();
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
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: i64, %[[VALUE_y:[0-9]+]] y: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_z:[0-9]+]] z: i64 [storage=automatic] = reinterpret<i64, reason=assign, fits=unknown>(mul<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(read<i64>(%[[VALUE_x]])), reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE_y]]))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%[[VALUE_x]]), div<i64, by_zero=ub, min_by_neg_one=ub>(read<i64>(%[[VALUE_z]]), read<i64>(%[[VALUE_y]])))
// DEFAULT-NEXT:             return widen<i64, reason=return>(neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:         return read<i64>(%[[VALUE_z]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<i64>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_foo]], widen<i64, reason=arg>(const<i32>(4)), widen<i64, reason=arg>(const<i32>(24))), widen<i64, reason=usual_arith>(const<i32>(96)))
// DEFAULT-NEXT:             write<bool>(%[[VALUE0]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE0]], ne<i64>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_foo]], widen<i64, reason=arg>(const<i32>(124)), widen<i64, reason=arg>(const<i32>(126))), mul<i64, overflow=ub>(const<i64>(124), widen<i64, reason=usual_arith>(const<i32>(126)))));
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE0]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE1]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE1]], ne<i64>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_foo]], div<i64, by_zero=ub, min_by_neg_one=ub>(const<i64>(9223372036854775807), widen<i64, reason=usual_arith>(const<i32>(16))), widen<i64, reason=arg>(const<i32>(17))), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE1]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
