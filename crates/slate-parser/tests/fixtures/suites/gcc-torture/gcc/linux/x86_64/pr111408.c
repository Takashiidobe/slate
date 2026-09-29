/* PR target/111408 */

int   a, b, c, d;
short e;

int foo() {
  c = a % (sizeof(int) * 8);
  if (b & 1 << c)
    return -1;
  return 0;
}

int main() {
  for (; e != 1; e++) {
    int g = foo();
    if (g + d - 9 + d)
      continue;
    for (;;)
      __builtin_abort();
  }
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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(%[[VALUE_c]], reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(rem<u64, by_zero=ub>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_a]]))), mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))))));
// DEFAULT-NEXT:         if ne<i32>(and<i32>(read<i32>(%[[VALUE_b]]), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), read<i32>(%[[VALUE_c]]))), const<i32>(0))
// DEFAULT-NEXT:             return neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_e]])), const<i32>(1))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i16 [synthetic] = read<i16>(%[[VALUE_e]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE1]])), const<i32>(1)));
// DEFAULT-NEXT:                 write<i16>(%[[VALUE_e]], read<i16>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_g:[0-9]+]] g: i32 [storage=automatic] = call<i32, signature=fn() -> i32>(%[[VALUE_foo]]);
// DEFAULT-NEXT:                     if ne<i32>(add<i32, overflow=ub>(sub<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(%[[VALUE_g]]), read<i32>(%[[VALUE_d]])), const<i32>(9)), read<i32>(%[[VALUE_d]])), const<i32>(0))
// DEFAULT-NEXT:                         continue %[[VALUE0]];
// DEFAULT-NEXT:                     for %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                         condition: omitted
// DEFAULT-NEXT:                         increment: omitted
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
