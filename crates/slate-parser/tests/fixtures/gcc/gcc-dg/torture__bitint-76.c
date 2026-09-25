/* PR tree-optimization/119707 */
/* { dg-do run { target bitint } } */

#if __BITINT_MAXWIDTH__ >= 256
__attribute__((noipa)) unsigned _BitInt(256)
    foo(unsigned _BitInt(256) x, _BitInt(129) y) {
  return x + (unsigned _BitInt(255))y;
}
#endif

int main() {
#if __BITINT_MAXWIDTH__ >= 256
  if (foo(0, -1) !=
      0x7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffuwb)
    __builtin_abort();
#endif
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
// DEFAULT-NEXT:     fn %0 @foo(%1 x: u256b, %2 y: i129b) -> u256b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<u256b, overflow=wrap>(read<u256b>(%1), widen<u256b, reason=usual_arith>(reinterpret<u255b, reason=explicit, fits=unknown>(widen<i255b, reason=explicit>(read<i129b>(%2)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<u256b>(call<u256b, signature=fn(u256b, i129b) -> u256b>(%0, reinterpret<u256b, reason=arg, fits=unknown>(widen<i256b, reason=arg>(const<i32>(0))), widen<i129b, reason=arg>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u256b, reason=usual_arith>(const<u255b>(57896044618658097711785492504343953926634992332820282019728792003956564819967)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
