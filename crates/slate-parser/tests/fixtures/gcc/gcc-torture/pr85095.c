/* PR target/85095 */

__attribute__((noipa)) unsigned long f1(unsigned long a, unsigned long b) {
  unsigned long i = __builtin_add_overflow(a, b, &a);
  return a + i;
}

__attribute__((noipa)) unsigned long f2(unsigned long a, unsigned long b) {
  unsigned long i = __builtin_add_overflow(a, b, &a);
  return a - i;
}

__attribute__((noipa)) unsigned long f3(unsigned int a, unsigned int b) {
  unsigned int i = __builtin_add_overflow(a, b, &a);
  return a + i;
}

__attribute__((noipa)) unsigned long f4(unsigned int a, unsigned int b) {
  unsigned int i = __builtin_add_overflow(a, b, &a);
  return a - i;
}

int main() {
  if (f1(16UL, -18UL) != -2UL || f1(16UL, -17UL) != -1UL ||
      f1(16UL, -16UL) != 1UL || f1(16UL, -15UL) != 2UL ||
      f2(24UL, -26UL) != -2UL || f2(24UL, -25UL) != -1UL ||
      f2(24UL, -24UL) != -1UL || f2(24UL, -23UL) != 0UL ||
      f3(32U, -34U) != -2U || f3(32U, -33U) != -1U || f3(32U, -32U) != 1U ||
      f3(32U, -31U) != 2U || f4(35U, -37U) != -2U || f4(35U, -36U) != -1U ||
      f4(35U, -35U) != -1U || f4(35U, -34U) != 0U)
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
// DEFAULT-NEXT:     fn %0 @f1(%1 a: u64, %2 b: u64) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 i: u64 [storage=automatic] = from_bool<u64, reason=assign>(overflow_add<bool>(read<u64>(%1), read<u64>(%2), deref(addr_of<ptr<u64>>(%1))));
// DEFAULT-NEXT:         return add<u64, overflow=wrap>(read<u64>(%1), read<u64>(%3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @f2(%5 a: u64, %6 b: u64) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %7 i: u64 [storage=automatic] = from_bool<u64, reason=assign>(overflow_add<bool>(read<u64>(%5), read<u64>(%6), deref(addr_of<ptr<u64>>(%5))));
// DEFAULT-NEXT:         return sub<u64, overflow=wrap>(read<u64>(%5), read<u64>(%7));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @f3(%9 a: u32, %10 b: u32) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %11 i: u32 [storage=automatic] = from_bool<u32, reason=assign>(overflow_add<bool>(read<u32>(%9), read<u32>(%10), deref(addr_of<ptr<u32>>(%9))));
// DEFAULT-NEXT:         return widen<u64, reason=return>(add<u32, overflow=wrap>(read<u32>(%9), read<u32>(%11)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @f4(%13 a: u32, %14 b: u32) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %15 i: u32 [storage=automatic] = from_bool<u32, reason=assign>(overflow_add<bool>(read<u32>(%13), read<u32>(%14), deref(addr_of<ptr<u32>>(%13))));
// DEFAULT-NEXT:         return widen<u64, reason=return>(sub<u32, overflow=wrap>(read<u32>(%13), read<u32>(%15)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %17: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(u64, u64) -> u64>(%0, const<u64>(16), neg<u64, overflow=wrap>(const<u64>(18))), neg<u64, overflow=wrap>(const<u64>(2)))
// DEFAULT-NEXT:             write<bool>(%17, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%17, ne<u64>(call<u64, signature=fn(u64, u64) -> u64>(%0, const<u64>(16), neg<u64, overflow=wrap>(const<u64>(17))), neg<u64, overflow=wrap>(const<u64>(1))));
// DEFAULT-NEXT:         let %18: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%17)
// DEFAULT-NEXT:             write<bool>(%18, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%18, ne<u64>(call<u64, signature=fn(u64, u64) -> u64>(%0, const<u64>(16), neg<u64, overflow=wrap>(const<u64>(16))), const<u64>(1)));
// DEFAULT-NEXT:         let %19: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%18)
// DEFAULT-NEXT:             write<bool>(%19, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%19, ne<u64>(call<u64, signature=fn(u64, u64) -> u64>(%0, const<u64>(16), neg<u64, overflow=wrap>(const<u64>(15))), const<u64>(2)));
// DEFAULT-NEXT:         let %20: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%19)
// DEFAULT-NEXT:             write<bool>(%20, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%20, ne<u64>(call<u64, signature=fn(u64, u64) -> u64>(%4, const<u64>(24), neg<u64, overflow=wrap>(const<u64>(26))), neg<u64, overflow=wrap>(const<u64>(2))));
// DEFAULT-NEXT:         let %21: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%20)
// DEFAULT-NEXT:             write<bool>(%21, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%21, ne<u64>(call<u64, signature=fn(u64, u64) -> u64>(%4, const<u64>(24), neg<u64, overflow=wrap>(const<u64>(25))), neg<u64, overflow=wrap>(const<u64>(1))));
// DEFAULT-NEXT:         let %22: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%21)
// DEFAULT-NEXT:             write<bool>(%22, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%22, ne<u64>(call<u64, signature=fn(u64, u64) -> u64>(%4, const<u64>(24), neg<u64, overflow=wrap>(const<u64>(24))), neg<u64, overflow=wrap>(const<u64>(1))));
// DEFAULT-NEXT:         let %23: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%22)
// DEFAULT-NEXT:             write<bool>(%23, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%23, ne<u64>(call<u64, signature=fn(u64, u64) -> u64>(%4, const<u64>(24), neg<u64, overflow=wrap>(const<u64>(23))), const<u64>(0)));
// DEFAULT-NEXT:         let %24: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%23)
// DEFAULT-NEXT:             write<bool>(%24, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%24, ne<u64>(call<u64, signature=fn(u32, u32) -> u64>(%8, const<u32>(32), neg<u32, overflow=wrap>(const<u32>(34))), widen<u64, reason=usual_arith>(neg<u32, overflow=wrap>(const<u32>(2)))));
// DEFAULT-NEXT:         let %25: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%24)
// DEFAULT-NEXT:             write<bool>(%25, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%25, ne<u64>(call<u64, signature=fn(u32, u32) -> u64>(%8, const<u32>(32), neg<u32, overflow=wrap>(const<u32>(33))), widen<u64, reason=usual_arith>(neg<u32, overflow=wrap>(const<u32>(1)))));
// DEFAULT-NEXT:         let %26: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%25)
// DEFAULT-NEXT:             write<bool>(%26, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%26, ne<u64>(call<u64, signature=fn(u32, u32) -> u64>(%8, const<u32>(32), neg<u32, overflow=wrap>(const<u32>(32))), widen<u64, reason=usual_arith>(const<u32>(1))));
// DEFAULT-NEXT:         let %27: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%26)
// DEFAULT-NEXT:             write<bool>(%27, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%27, ne<u64>(call<u64, signature=fn(u32, u32) -> u64>(%8, const<u32>(32), neg<u32, overflow=wrap>(const<u32>(31))), widen<u64, reason=usual_arith>(const<u32>(2))));
// DEFAULT-NEXT:         let %28: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%27)
// DEFAULT-NEXT:             write<bool>(%28, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%28, ne<u64>(call<u64, signature=fn(u32, u32) -> u64>(%12, const<u32>(35), neg<u32, overflow=wrap>(const<u32>(37))), widen<u64, reason=usual_arith>(neg<u32, overflow=wrap>(const<u32>(2)))));
// DEFAULT-NEXT:         let %29: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%28)
// DEFAULT-NEXT:             write<bool>(%29, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%29, ne<u64>(call<u64, signature=fn(u32, u32) -> u64>(%12, const<u32>(35), neg<u32, overflow=wrap>(const<u32>(36))), widen<u64, reason=usual_arith>(neg<u32, overflow=wrap>(const<u32>(1)))));
// DEFAULT-NEXT:         let %30: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%29)
// DEFAULT-NEXT:             write<bool>(%30, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%30, ne<u64>(call<u64, signature=fn(u32, u32) -> u64>(%12, const<u32>(35), neg<u32, overflow=wrap>(const<u32>(35))), widen<u64, reason=usual_arith>(neg<u32, overflow=wrap>(const<u32>(1)))));
// DEFAULT-NEXT:         let %31: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%30)
// DEFAULT-NEXT:             write<bool>(%31, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%31, ne<u64>(call<u64, signature=fn(u32, u32) -> u64>(%12, const<u32>(35), neg<u32, overflow=wrap>(const<u32>(34))), widen<u64, reason=usual_arith>(const<u32>(0))));
// DEFAULT-NEXT:         if read<bool>(%31)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
