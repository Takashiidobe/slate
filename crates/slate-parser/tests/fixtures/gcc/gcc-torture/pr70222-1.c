/* PR rtl-optimization/70222 */

int          a = 1;
unsigned int b = 2;
int          c = 0;
int          d = 0;

void foo() {
  int e = ((-(c >= c)) < b) > ((int)(-1ULL >> ((a / a) * 15)));
  d     = -e;
}

__attribute__((noinline, noclone)) void bar(int x) {
  if (x != -1)
    __builtin_abort();
}

int main() {
#if __CHAR_BIT__ == 8 && __SIZEOF_INT__ == 4 && __SIZEOF_LONG_LONG__ == 8
  foo();
  bar(d);
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
// DEFAULT-NEXT:     global %0 a: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %1 b: u32 [storage=static] = reinterpret<u32, reason=assign, fits=always>(const<i32>(2)) [linkage=external];
// DEFAULT-NEXT:     global %2 c: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %3 d: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     fn %4 @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %5 e: i32 [storage=automatic] = from_bool<i32, reason=assign>(gt<i32>(from_bool<i32, reason=promotion>(lt<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(neg<i32, overflow=ub>(from_bool<i32, reason=promotion>(ge<i32>(read<i32>(%2), read<i32>(%2))))), read<u32>(%1))), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(neg<u64, overflow=wrap>(const<u64>(1)), mul<i32, overflow=ub>(div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%0), read<i32>(%0)), const<i32>(15)))))));
// DEFAULT-NEXT:         write<i32>(%3, neg<i32, overflow=ub>(read<i32>(%5)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @bar(%7 x: i32) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%7), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%6, read<i32>(%3));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
