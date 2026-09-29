
typedef unsigned char                                u8;
typedef unsigned __attribute__((__vector_size__(8))) V;

V             v;
unsigned char c;

int main(void) {
  V x = (v > 0) > (v != c);
  // V x = foo ();
  if (x[0] || x[1])
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
// DEFAULT-NEXT:     type @type[[TYPE_u8:[0-9]+]] u8 = u8;
// DEFAULT-NEXT:     type @type[[TYPE_V:[0-9]+]] V = vector<u32, 2>;
// DEFAULT-NEXT:     global %[[VALUE_v:[0-9]+]] v: vector<u32, 2> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: u8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: vector<u32, 2> [storage=automatic] = vector_bit_cast<vector<u32, 2>, reason=assign>(gt<vector<i32, 2>, result=vector<i32, 2>>(gt<vector<u32, 2>, result=vector<i32, 2>>(read<vector<u32, 2>>(%[[VALUE_v]]), vector_splat<vector<u32, 2>, reason=usual_arith>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), ne<vector<u32, 2>, result=vector<i32, 2>>(read<vector<u32, 2>>(%[[VALUE_v]]), vector_splat<vector<u32, 2>, reason=usual_arith>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_c]]))))))));
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(read<u32>(lane(%[[VALUE_x]], const<i32>(0))), const<u32>(0)), ne<u32>(read<u32>(lane(%[[VALUE_x]], const<i32>(1))), const<u32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
