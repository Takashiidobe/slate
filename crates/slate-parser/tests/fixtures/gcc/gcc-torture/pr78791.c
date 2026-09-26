/* PR target/78791 */

__attribute__((used, noinline, noclone)) unsigned long long
foo(unsigned long long x, unsigned long long y, unsigned long long z) {
  unsigned long long a  = x / y;
  unsigned long long b  = x % y;
  a                    |= z;
  b                    ^= z;
  return a + b;
}

int main() {
  if (foo(64, 7, 0) != 10 || foo(28, 3, 2) != 14)
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
// DEFAULT-NEXT:     fn %0 @foo(%1 x: u64, %2 y: u64, %3 z: u64) -> u64 [linkage=external] [used] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %4 a: u64 [storage=automatic] = div<u64, by_zero=ub>(read<u64>(%1), read<u64>(%2));
// DEFAULT-NEXT:         let %5 b: u64 [storage=automatic] = rem<u64, by_zero=ub>(read<u64>(%1), read<u64>(%2));
// DEFAULT-NEXT:         let %8: u64 [synthetic] = read<u64>(%4);
// DEFAULT-NEXT:         let %9: u64 [synthetic] = or<u64>(read<u64>(%8), read<u64>(%3));
// DEFAULT-NEXT:         write<u64>(%4, read<u64>(%9));
// DEFAULT-NEXT:         let %10: u64 [synthetic] = read<u64>(%5);
// DEFAULT-NEXT:         let %11: u64 [synthetic] = xor<u64>(read<u64>(%10), read<u64>(%3));
// DEFAULT-NEXT:         write<u64>(%5, read<u64>(%11));
// DEFAULT-NEXT:         return add<u64, overflow=wrap>(read<u64>(%4), read<u64>(%5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %12: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(u64, u64, u64) -> u64>(%0, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(64))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(7))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(10))))
// DEFAULT-NEXT:             write<bool>(%12, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%12, ne<u64>(call<u64, signature=fn(u64, u64, u64) -> u64>(%0, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(28))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(14)))));
// DEFAULT-NEXT:         if read<bool>(%12)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%7);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
