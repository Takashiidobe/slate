/* PR target/105613 */
/* { dg-do run { target int128 } } */

typedef unsigned __int128 __attribute__((__vector_size__(16))) V;

void foo(V v, V *r) { *r = v != 0; }

int main() {
  V r;
  foo((V){5}, &r);
  if (r[0] != ~(unsigned __int128)0)
    __builtin_abort();
  foo((V){0x500000005ULL}, &r);
  if (r[0] != ~(unsigned __int128)0)
    __builtin_abort();
  foo((V){0}, &r);
  if (r[0] != 0)
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
// DEFAULT-NEXT:     type @type[[TYPE_V:[0-9]+]] V = vector<u128, 1>;
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_v:[0-9]+]] v: vector<u128, 1>, %[[VALUE_r:[0-9]+]] r: ptr<vector<u128, 1>>) -> void [linkage=external] [abi=sysv64(direct, scalar) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<vector<u128, 1>>(deref(read<ptr<vector<u128, 1>>>(%[[VALUE_r]])), vector_bit_cast<vector<u128, 1>, reason=assign>(ne<vector<u128, 1>, result=vector<i128, 1>>(read<vector<u128, 1>>(%[[VALUE_v]]), vector_splat<vector<u128, 1>, reason=usual_arith>(reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(0)))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_r_2:[0-9]+]] r: vector<u128, 1> [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(vector<u128, 1>, ptr<vector<u128, 1>>) -> void, abi=sysv64(direct, scalar) -> void>(%[[VALUE_foo]], read<vector<u128, 1>>(compound_literal %[[VALUE0:[0-9]+]] [storage=automatic] = aggregate<vector<u128, 1>, zero_fill=false>(index0 = reinterpret<u128, reason=assign, fits=unknown>(widen<i128, reason=assign>(const<i32>(5))))), addr_of<ptr<vector<u128, 1>>>(%[[VALUE_r_2]]));
// DEFAULT-NEXT:         if ne<u128>(read<u128>(lane(%[[VALUE_r_2]], const<i32>(0))), not<u128>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(vector<u128, 1>, ptr<vector<u128, 1>>) -> void, abi=sysv64(direct, scalar) -> void>(%[[VALUE_foo]], read<vector<u128, 1>>(compound_literal %[[VALUE1:[0-9]+]] [storage=automatic] = aggregate<vector<u128, 1>, zero_fill=false>(index0 = widen<u128, reason=assign>(const<u64>(21474836485)))), addr_of<ptr<vector<u128, 1>>>(%[[VALUE_r_2]]));
// DEFAULT-NEXT:         if ne<u128>(read<u128>(lane(%[[VALUE_r_2]], const<i32>(0))), not<u128>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(vector<u128, 1>, ptr<vector<u128, 1>>) -> void, abi=sysv64(direct, scalar) -> void>(%[[VALUE_foo]], read<vector<u128, 1>>(compound_literal %[[VALUE2:[0-9]+]] [storage=automatic] = aggregate<vector<u128, 1>, zero_fill=false>(index0 = reinterpret<u128, reason=assign, fits=unknown>(widen<i128, reason=assign>(const<i32>(0))))), addr_of<ptr<vector<u128, 1>>>(%[[VALUE_r_2]]));
// DEFAULT-NEXT:         if ne<u128>(read<u128>(lane(%[[VALUE_r_2]], const<i32>(0))), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
