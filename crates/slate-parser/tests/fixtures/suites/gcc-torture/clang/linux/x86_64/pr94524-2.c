/* PR tree-optimization/94524 */

typedef signed char __attribute__((__vector_size__(16))) V;

static __attribute__((__noinline__, __noclone__)) V foo(V c) {
  c %= (signed char)-128;
  return (V)c;
}

int main() {
  V x = foo((V){-128});
  if (x[0] != 0)
    __builtin_abort();
  x = foo((V){-127});
  if (x[0] != -127)
    __builtin_abort();
  x = foo((V){127});
  if (x[0] != 127)
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
// DEFAULT-NEXT:     type @type[[TYPE_V:[0-9]+]] V = vector<i8, 16>;
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_c:[0-9]+]] c: vector<i8, 16>) -> vector<i8, 16> [linkage=internal] [inline=never] [definition=emitted] [abi=sysv64(direct) -> direct] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: vector<i8, 16> [synthetic] = read<vector<i8, 16>>(%[[VALUE_c]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: vector<i8, 16> [synthetic] = rem<vector<i8, 16>, elementwise=true, by_zero=ub, min_by_neg_one=ub>(read<vector<i8, 16>>(%[[VALUE0]]), vector_splat<vector<i8, 16>, reason=usual_arith>(truncate<i8, reason=usual_arith, fits=unknown>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(128)))))));
// DEFAULT-NEXT:         write<vector<i8, 16>>(%[[VALUE_c]], read<vector<i8, 16>>(%[[VALUE1]]));
// DEFAULT-NEXT:         return read<vector<i8, 16>>(%[[VALUE_c]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: vector<i8, 16> [storage=automatic] = call<vector<i8, 16>, signature=fn(vector<i8, 16>) -> vector<i8, 16>, abi=sysv64(direct) -> direct>(%[[VALUE_foo]], read<vector<i8, 16>>(compound_literal %[[VALUE2:[0-9]+]] [storage=automatic] = aggregate<vector<i8, 16>, zero_fill=true>(index0 = truncate<i8, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(128))))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(lane(%[[VALUE_x]], const<i32>(0)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         write<vector<i8, 16>>(%[[VALUE_x]], call<vector<i8, 16>, signature=fn(vector<i8, 16>) -> vector<i8, 16>, abi=sysv64(direct) -> direct>(%[[VALUE_foo]], read<vector<i8, 16>>(compound_literal %[[VALUE3:[0-9]+]] [storage=automatic] = aggregate<vector<i8, 16>, zero_fill=true>(index0 = truncate<i8, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(127)))))));
// DEFAULT-NEXT:         call<vector<i8, 16>, signature=fn(vector<i8, 16>) -> vector<i8, 16>, abi=sysv64(direct) -> direct>(%[[VALUE_foo]], read<vector<i8, 16>>(compound_literal %[[VALUE3]] [storage=automatic] = aggregate<vector<i8, 16>, zero_fill=true>(index0 = truncate<i8, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(127))))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(lane(%[[VALUE_x]], const<i32>(0)))), neg<i32, overflow=ub>(const<i32>(127)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         write<vector<i8, 16>>(%[[VALUE_x]], call<vector<i8, 16>, signature=fn(vector<i8, 16>) -> vector<i8, 16>, abi=sysv64(direct) -> direct>(%[[VALUE_foo]], read<vector<i8, 16>>(compound_literal %[[VALUE4:[0-9]+]] [storage=automatic] = aggregate<vector<i8, 16>, zero_fill=true>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(127))))));
// DEFAULT-NEXT:         call<vector<i8, 16>, signature=fn(vector<i8, 16>) -> vector<i8, 16>, abi=sysv64(direct) -> direct>(%[[VALUE_foo]], read<vector<i8, 16>>(compound_literal %[[VALUE4]] [storage=automatic] = aggregate<vector<i8, 16>, zero_fill=true>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(127)))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(lane(%[[VALUE_x]], const<i32>(0)))), const<i32>(127))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
