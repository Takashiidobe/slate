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
// DEFAULT-NEXT:     type @type0 V = vector<u16, 16>;
// DEFAULT-NEXT:     global %1 a: u16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 b: u16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 c: u16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 d: u16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %5 @foo(%6 m: vector<u16, 16>, %7 ret: ptr<u16>) -> void [linkage=external] [abi=sysv64(byval<align=32>, scalar) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %8 v: vector<u16, 16> [storage=automatic] = vector_bit_cast<vector<u16, 16>, reason=assign>(gt<vector<u16, 16>, result=vector<i16, 16>>(vector_splat<vector<u16, 16>, reason=usual_arith>(reinterpret<u16, reason=usual_arith, fits=unknown>(truncate<i16, reason=usual_arith, fits=always>(const<i32>(6)))), and<vector<u16, 16>, elementwise=true>(read<vector<u16, 16>>(compound_literal %12 [storage=automatic] = aggregate<vector<u16, 16>, zero_fill=true>(index0 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(2124))), index1 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(8))))), read<vector<u16, 16>>(%6))));
// DEFAULT-NEXT:         let %9 uc: u16 [storage=automatic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(lane(%8, const<i32>(0))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%1)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%2)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%3)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%4))))));
// DEFAULT-NEXT:         write<u16>(deref(read<ptr<u16>>(%7)), read<u16>(%9));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %11 x: u16 [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(vector<u16, 16>, ptr<u16>) -> void, abi=sysv64(byval<align=32>, scalar) -> void>(%5, read<vector<u16, 16>>(compound_literal %13 [storage=automatic] = aggregate<vector<u16, 16>, zero_fill=true>(index0 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0))), index1 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(15))))), addr_of<ptr<u16>>(%11));
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%11))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(not<i32>(const<i32>(0)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
