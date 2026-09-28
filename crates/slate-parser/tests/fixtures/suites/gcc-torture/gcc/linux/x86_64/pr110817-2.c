
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
// DEFAULT-NEXT:     type @type0 u8 = u8;
// DEFAULT-NEXT:     type @type1 V = vector<u32, 2>;
// DEFAULT-NEXT:     global %2 v: vector<u32, 2> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 c: u8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %6 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %5 x: vector<u32, 2> [storage=automatic] = vector_bit_cast<vector<u32, 2>, reason=assign>(gt<vector<i32, 2>, result=vector<i32, 2>>(gt<vector<u32, 2>, result=vector<i32, 2>>(read<vector<u32, 2>>(%2), vector_splat<vector<u32, 2>, reason=usual_arith>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), ne<vector<u32, 2>, result=vector<i32, 2>>(read<vector<u32, 2>>(%2), vector_splat<vector<u32, 2>, reason=usual_arith>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%3))))))));
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(read<u32>(lane(%5, const<i32>(0))), const<u32>(0)), ne<u32>(read<u32>(lane(%5, const<i32>(1))), const<u32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
