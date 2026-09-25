/* PR rtl-optimization/79450 */

unsigned int foo(unsigned char x, unsigned long long y) {
  do {
    x &= !y;
    x %= 24;
  } while (x < y);
  return x + y;
}

int main(void) {
  unsigned int x = foo(1, 0);
  if (x != 1)
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
// DEFAULT-NEXT:     fn %0 @foo(%1 x: u8, %2 y: u64) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         do %5
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %6: u8 [synthetic] = read<u8>(%1);
// DEFAULT-NEXT:                 let %7: u8 [synthetic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%6))), from_bool<i32, reason=promotion>(not<bool>(ne<u64>(read<u64>(%2), const<u64>(0)))))));
// DEFAULT-NEXT:                 write<u8>(%1, read<u8>(%7));
// DEFAULT-NEXT:                 let %8: u8 [synthetic] = read<u8>(%1);
// DEFAULT-NEXT:                 let %9: u8 [synthetic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%8))), const<i32>(24))));
// DEFAULT-NEXT:                 write<u8>(%1, read<u8>(%9));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%1))))), read<u64>(%2));
// DEFAULT-NEXT:         return truncate<u32, reason=return, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%1))))), read<u64>(%2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %4 x: u32 [storage=automatic] = call<u32, signature=fn(u8, u64) -> u32>(%0, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%4), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
