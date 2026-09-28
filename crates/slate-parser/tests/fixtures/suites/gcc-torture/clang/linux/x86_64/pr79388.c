/* PR rtl-optimization/79388 */
/* { dg-additional-options "-fno-tree-coalesce-vars" } */

unsigned int a, c;

__attribute__((noinline, noclone)) unsigned int foo(unsigned int p) {
  p |= 1;
  p &= 0xfffe;
  p %= 0xffff;
  c  = p;
  return a + p;
}

int main(void) {
  int x = foo(6);
  if (x != 6)
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
// DEFAULT-NEXT:     global %0 a: u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 c: u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %2 @foo(%3 p: u32) -> u32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %7: u32 [synthetic] = read<u32>(%3);
// DEFAULT-NEXT:         let %8: u32 [synthetic] = or<u32>(read<u32>(%7), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(%3, read<u32>(%8));
// DEFAULT-NEXT:         let %9: u32 [synthetic] = read<u32>(%3);
// DEFAULT-NEXT:         let %10: u32 [synthetic] = and<u32>(read<u32>(%9), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(65534)));
// DEFAULT-NEXT:         write<u32>(%3, read<u32>(%10));
// DEFAULT-NEXT:         let %11: u32 [synthetic] = read<u32>(%3);
// DEFAULT-NEXT:         let %12: u32 [synthetic] = rem<u32, by_zero=ub>(read<u32>(%11), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(65535)));
// DEFAULT-NEXT:         write<u32>(%3, read<u32>(%12));
// DEFAULT-NEXT:         write<u32>(%1, read<u32>(%3));
// DEFAULT-NEXT:         return add<u32, overflow=wrap>(read<u32>(%0), read<u32>(%3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %5 x: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(call<u32, signature=fn(u32) -> u32>(%2, reinterpret<u32, reason=arg, fits=always>(const<i32>(6))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%5), const<i32>(6))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
