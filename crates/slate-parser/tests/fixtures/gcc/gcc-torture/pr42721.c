/* PR c/42721 */

extern void abort(void);

static unsigned long long foo(unsigned long long x, unsigned long long y) {
  return x / y;
}

static int a, b;

int main(void) {
  unsigned long long c  = 1;
  b                    ^= c && (foo(a, -1ULL) != 1L);
  if (b != 1)
    abort();
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
// DEFAULT-NEXT:     global %4 a: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %5 b: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @foo(%2 x: u64, %3 y: u64) -> u64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return div<u64, by_zero=ub>(read<u64>(%2), read<u64>(%3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %7 c: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:         let %8: i32 [synthetic] = read<i32>(%5);
// DEFAULT-NEXT:         let %9: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(read<u64>(%7), const<u64>(0))
// DEFAULT-NEXT:             write<bool>(%9, ne<u64>(call<u64, signature=fn(u64, u64) -> u64>(%1, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%4))), neg<u64, overflow=wrap>(const<u64>(1))), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(1))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%9, const<bool>(false));
// DEFAULT-NEXT:         let %10: i32 [synthetic] = xor<i32>(read<i32>(%8), from_bool<i32, reason=promotion>(read<bool>(%9)));
// DEFAULT-NEXT:         write<i32>(%5, read<i32>(%10));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%5), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
