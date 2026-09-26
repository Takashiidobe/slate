/* PR tree-optimization/113774 */
/* { dg-do run { target bitint } } */
/* { dg-options "-std=c23 -pedantic-errors" } */
/* { dg-skip-if "" { ! run_expensive_tests }  { "*" } { "-O0" "-O2" } } */
/* { dg-skip-if "" { ! run_expensive_tests } { "-flto" } { "" } } */

#if __BITINT_MAXWIDTH__ >= 512
unsigned _BitInt(512) u;
unsigned _BitInt(512) v;

void foo(unsigned _BitInt(255) a, unsigned _BitInt(257) b,
         unsigned _BitInt(512) * r) {
  b                       += v;
  b                       |= a - b;
  unsigned _BitInt(512) c  = b * 6;
  unsigned _BitInt(512) h  = c >> u;
  *r                       = h;
}
#endif

int main() {
#if __BITINT_MAXWIDTH__ >= 512
  unsigned _BitInt(512) x;
  foo(0x10000000000000000wb, 0x10000000000000001wb, &x);
  if (x !=
      0x1fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffawb)
    __builtin_abort();
#endif
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
// DEFAULT-NEXT:     global %0 u: u512b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 v: u512b [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %2 @foo(%3 a: u255b, %4 b: u257b, %5 r: ptr<u512b>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %11: u257b [synthetic] = read<u257b>(%4);
// DEFAULT-NEXT:         let %12: u257b [synthetic] = truncate<u257b, reason=assign, fits=unknown>(add<u512b, overflow=wrap>(widen<u512b, reason=usual_arith>(read<u257b>(%11)), read<u512b>(%1)));
// DEFAULT-NEXT:         write<u257b>(%4, read<u257b>(%12));
// DEFAULT-NEXT:         let %13: u257b [synthetic] = read<u257b>(%4);
// DEFAULT-NEXT:         let %14: u257b [synthetic] = or<u257b>(read<u257b>(%13), sub<u257b, overflow=wrap>(widen<u257b, reason=usual_arith>(read<u255b>(%3)), read<u257b>(%4)));
// DEFAULT-NEXT:         write<u257b>(%4, read<u257b>(%14));
// DEFAULT-NEXT:         let %6 c: u512b [storage=automatic] = widen<u512b, reason=assign>(mul<u257b, overflow=wrap>(read<u257b>(%4), reinterpret<u257b, reason=usual_arith, fits=unknown>(widen<i257b, reason=usual_arith>(const<i32>(6)))));
// DEFAULT-NEXT:         let %7 h: u512b [storage=automatic] = shr<u512b, amount_out_of_range=ub, fill=zero_extend>(read<u512b>(%6), read<u512b>(%0));
// DEFAULT-NEXT:         write<u512b>(deref(read<ptr<u512b>>(%5)), read<u512b>(%7));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %9 x: u512b [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(u255b, u257b, ptr<u512b>) -> void>(%2, reinterpret<u255b, reason=arg, fits=unknown>(widen<i255b, reason=arg>(const<i66b>(18446744073709551616))), reinterpret<u257b, reason=arg, fits=unknown>(widen<i257b, reason=arg>(const<i66b>(18446744073709551617))), addr_of<ptr<u512b>>(%9));
// DEFAULT-NEXT:         if ne<u512b>(read<u512b>(%9), reinterpret<u512b, reason=usual_arith, fits=unknown>(widen<i512b, reason=usual_arith>(const<i258b>(231584178474632390847141970017375815706539969331281128078915168015826259279866))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%10);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
