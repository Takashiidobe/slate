/* PR tree-optimization/83269 */

int main() {
#if __SIZEOF_INT__ == 4 && __SIZEOF_LONG_LONG__ > 4 && __CHAR_BIT__ == 8
  volatile unsigned char a = 1;
  long long              b = 0x80000000L;
  int                    c = -((int)(-b) - (-0x7fffffff * a));
  if (c != 1)
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
// DEFAULT-NEXT:     fn %0 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %1 a: volatile u8 [storage=automatic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         let %2 b: i64 [storage=automatic] = const<i64>(2147483648);
// DEFAULT-NEXT:         let %3 c: i32 [storage=automatic] = neg<i32, overflow=ub>(sub<i32, overflow=ub>(truncate<i32, reason=explicit, fits=unknown>(neg<i64, overflow=ub>(read<i64>(%2))), mul<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile>(%1))))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%3), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
