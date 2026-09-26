/* PR tree-optimization/126503 */
/* { dg-do run { target bitint575 } } */

[[gnu::noipa]] unsigned _BitInt(400) foo(_BitInt(257) a) {
  return (unsigned _BitInt(400))(-a);
}

[[gnu::noipa]] unsigned _BitInt(300) bar(_BitInt(7) a, unsigned _BitInt(17) b) {
  _BitInt(257) x = (_BitInt(257))a + -1;
  return ~((unsigned _BitInt(300))x ^ (unsigned _BitInt(300))b);
}

[[gnu::noipa]] _BitInt(129) baz(_BitInt(129) x) {
  return x - 24;
}

[[gnu::noipa]] int qux(_BitInt(129) x, _BitInt(129) y) {
  return x == y;
}

int main() {
  if (foo(-1wb) != 1uwb)
    __builtin_abort();
  if (bar(1wb, 3uwb) != (unsigned _BitInt(300)) - 4wb)
    __builtin_abort();
  if (!qux(baz(100), 76))
    __builtin_abort();
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
// DEFAULT-NEXT:     fn %0 @foo(%1 a: i257b) -> u400b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u400b, reason=explicit, fits=unknown>(widen<i400b, reason=explicit>(neg<i257b, overflow=ub>(read<i257b>(%1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @bar(%3 a: i7b, %4 b: u17b) -> u300b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %5 x: i257b [storage=automatic] = add<i257b, overflow=ub>(widen<i257b, reason=explicit>(read<i7b>(%3)), widen<i257b, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         return not<u300b>(xor<u300b>(reinterpret<u300b, reason=explicit, fits=unknown>(widen<i300b, reason=explicit>(read<i257b>(%5))), widen<u300b, reason=explicit>(read<u17b>(%4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @baz(%7 x: i129b) -> i129b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return sub<i129b, overflow=ub>(read<i129b>(%7), widen<i129b, reason=usual_arith>(const<i32>(24)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @qux(%9 x: i129b, %10 y: i129b) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i129b>(read<i129b>(%9), read<i129b>(%10)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<u400b>(call<u400b, signature=fn(i257b) -> u400b>(%0, widen<i257b, reason=arg>(neg<i2b, overflow=ub>(const<i2b>(1)))), widen<u400b, reason=usual_arith>(const<u1b>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%12);
// DEFAULT-NEXT:         if ne<u300b>(call<u300b, signature=fn(i7b, u17b) -> u300b>(%2, widen<i7b, reason=arg>(const<i2b>(1)), widen<u17b, reason=arg>(const<u2b>(3))), reinterpret<u300b, reason=explicit, fits=unknown>(widen<i300b, reason=explicit>(neg<i4b, overflow=ub>(const<i4b>(4)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%12);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(i129b, i129b) -> i32>(%8, call<i129b, signature=fn(i129b) -> i129b>(%6, widen<i129b, reason=arg>(const<i32>(100))), widen<i129b, reason=arg>(const<i32>(76))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%12);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
