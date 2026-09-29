/* PR rtl-optimization/104839 */

__attribute__((noipa)) short foo(void) { return -1; }

__attribute__((noipa)) int bar(void) {
  short i = foo();
  if (i == -2)
    return 2;
  long          k = i;
  int           j = -1;
  volatile long s = 300;
  if (k < 0) {
    k += s;
    if (k < 0)
      j = 0;
  } else if (k >= s)
    j = 0;
  if (j != -1)
    return 1;
  return 0;
}

int main() {
  if (bar() != 0)
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
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i16 [storage=automatic] = call<i16, signature=fn() -> i16>(%[[VALUE_foo]]);
// DEFAULT-NEXT:         if eq<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_i]])), neg<i32, overflow=ub>(const<i32>(2)))
// DEFAULT-NEXT:             return const<i32>(2);
// DEFAULT-NEXT:         let %[[VALUE_k:[0-9]+]] k: i64 [storage=automatic] = widen<i64, reason=assign>(read<i16>(%[[VALUE_i]]));
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE_s:[0-9]+]] s: volatile i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(300));
// DEFAULT-NEXT:         if lt<i64>(read<i64>(%[[VALUE_k]]), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE0:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_k]]);
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE0]]), read<i64, volatile>(%[[VALUE_s]]));
// DEFAULT-NEXT:                 write<i64>(%[[VALUE_k]], read<i64>(%[[VALUE1]]));
// DEFAULT-NEXT:                 if lt<i64>(read<i64>(%[[VALUE_k]]), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_j]], const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ge<i64>(read<i64>(%[[VALUE_k]]), read<i64, volatile>(%[[VALUE_s]]))
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j]], const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_j]]), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             return const<i32>(1);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_bar]]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
