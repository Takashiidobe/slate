/* PR target/109040 */

typedef unsigned short __attribute__((__vector_size__(32))) V;

unsigned short a, b, c, d;

void foo(V m, unsigned short *ret) {
  V              v  = 6 > ((V){2124, 8} & m);
  unsigned short uc = v[0] + a + b + c + d;
  *ret              = uc;
}

int main() {
  unsigned short x;
  foo((V){0, 15}, &x);
  if (x != (unsigned short)~0)
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
// DEFAULT-NEXT:     type @type[[TYPE_V:[0-9]+]] V = vector<u16, 16>;
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: u16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: u16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: u16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: u16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_m:[0-9]+]] m: vector<u16, 16>, %[[VALUE_ret:[0-9]+]] ret: ptr<u16>) -> void [linkage=external] [abi=sysv64(byval<align=32>, scalar) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_v:[0-9]+]] v: vector<u16, 16> [storage=automatic] = vector_bit_cast<vector<u16, 16>, reason=assign>(gt<vector<u16, 16>, result=vector<i16, 16>>(vector_splat<vector<u16, 16>, reason=usual_arith>(reinterpret<u16, reason=usual_arith, fits=unknown>(truncate<i16, reason=usual_arith, fits=always>(const<i32>(6)))), and<vector<u16, 16>, elementwise=true>(read<vector<u16, 16>>(compound_literal %[[VALUE0:[0-9]+]] [storage=automatic] = aggregate<vector<u16, 16>, zero_fill=true>(index0 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(2124))), index1 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(8))))), read<vector<u16, 16>>(%[[VALUE_m]]))));
// DEFAULT-NEXT:         let %[[VALUE_uc:[0-9]+]] uc: u16 [storage=automatic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(lane(%[[VALUE_v]], const<i32>(0))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_a]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_b]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_c]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_d]]))))));
// DEFAULT-NEXT:         write<u16>(deref(read<ptr<u16>>(%[[VALUE_ret]])), read<u16>(%[[VALUE_uc]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: u16 [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(vector<u16, 16>, ptr<u16>) -> void, abi=sysv64(byval<align=32>, scalar) -> void>(%[[VALUE_foo]], read<vector<u16, 16>>(compound_literal %[[VALUE1:[0-9]+]] [storage=automatic] = aggregate<vector<u16, 16>, zero_fill=true>(index0 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0))), index1 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(15))))), addr_of<ptr<u16>>(%[[VALUE_x]]));
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_x]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(not<i32>(const<i32>(0)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
