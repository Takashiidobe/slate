typedef unsigned __attribute__((__vector_size__(1 * sizeof(unsigned)))) V;

V             v;
unsigned char c;

int main(void) {
  V                   x = (v > 0) > (v != c);
  volatile signed int t = x[0];
  if (t)
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
// DEFAULT-NEXT:     type @type0 V = vector<u32, 1>;
// DEFAULT-NEXT:     global %1 v: vector<u32, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 c: u8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %4 x: vector<u32, 1> [storage=automatic] = vector_bit_cast<vector<u32, 1>, reason=assign>(gt<vector<i32, 1>, result=vector<i32, 1>>(gt<vector<u32, 1>, result=vector<i32, 1>>(read<vector<u32, 1>>(%1), vector_splat<vector<u32, 1>, reason=usual_arith>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), ne<vector<u32, 1>, result=vector<i32, 1>>(read<vector<u32, 1>>(%1), vector_splat<vector<u32, 1>, reason=usual_arith>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%2))))))));
// DEFAULT-NEXT:         let %5 t: volatile i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(read<u32>(lane(%4, const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(read<i32, volatile>(%5), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
