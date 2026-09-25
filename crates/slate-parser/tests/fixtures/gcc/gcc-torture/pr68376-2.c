/* PR rtl-optimization/68376 */

extern void abort(void);

__attribute__((noinline, noclone)) int f1(int x) { return x < 0 ? ~x : x; }

__attribute__((noinline, noclone)) int f2(int x) { return x < 0 ? x : ~x; }

__attribute__((noinline, noclone)) int f3(int x) { return x <= 0 ? ~x : x; }

__attribute__((noinline, noclone)) int f4(int x) { return x <= 0 ? x : ~x; }

__attribute__((noinline, noclone)) int f5(int x) { return x >= 0 ? ~x : x; }

__attribute__((noinline, noclone)) int f6(int x) { return x >= 0 ? x : ~x; }

__attribute__((noinline, noclone)) int f7(int x) { return x > 0 ? ~x : x; }

__attribute__((noinline, noclone)) int f8(int x) { return x > 0 ? x : ~x; }

int main() {
  if (f1(5) != 5 || f1(-5) != 4 || f1(0) != 0)
    abort();
  if (f2(5) != -6 || f2(-5) != -5 || f2(0) != -1)
    abort();
  if (f3(5) != 5 || f3(-5) != 4 || f3(0) != -1)
    abort();
  if (f4(5) != -6 || f4(-5) != -5 || f4(0) != 0)
    abort();
  if (f5(5) != -6 || f5(-5) != -5 || f5(0) != -1)
    abort();
  if (f6(5) != 5 || f6(-5) != 4 || f6(0) != 0)
    abort();
  if (f7(5) != -6 || f7(-5) != -5 || f7(0) != 0)
    abort();
  if (f8(5) != 5 || f8(-5) != 4 || f8(0) != -1)
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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @f1(%2 x: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i32>(lt<i32>(read<i32>(%2), const<i32>(0)), not<i32>(read<i32>(%2)), read<i32>(%2));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @f2(%4 x: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i32>(lt<i32>(read<i32>(%4), const<i32>(0)), read<i32>(%4), not<i32>(read<i32>(%4)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @f3(%6 x: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i32>(le<i32>(read<i32>(%6), const<i32>(0)), not<i32>(read<i32>(%6)), read<i32>(%6));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @f4(%8 x: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i32>(le<i32>(read<i32>(%8), const<i32>(0)), read<i32>(%8), not<i32>(read<i32>(%8)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @f5(%10 x: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i32>(ge<i32>(read<i32>(%10), const<i32>(0)), not<i32>(read<i32>(%10)), read<i32>(%10));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @f6(%12 x: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i32>(ge<i32>(read<i32>(%12), const<i32>(0)), read<i32>(%12), not<i32>(read<i32>(%12)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @f7(%14 x: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i32>(gt<i32>(read<i32>(%14), const<i32>(0)), not<i32>(read<i32>(%14)), read<i32>(%14));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @f8(%16 x: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i32>(gt<i32>(read<i32>(%16), const<i32>(0)), read<i32>(%16), not<i32>(read<i32>(%16)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %18: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%1, const<i32>(5)), const<i32>(5))
// DEFAULT-NEXT:             write<bool>(%18, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%18, ne<i32>(call<i32, signature=fn(i32) -> i32>(%1, neg<i32, overflow=ub>(const<i32>(5))), const<i32>(4)));
// DEFAULT-NEXT:         let %19: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%18)
// DEFAULT-NEXT:             write<bool>(%19, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%19, ne<i32>(call<i32, signature=fn(i32) -> i32>(%1, const<i32>(0)), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%19)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %20: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%3, const<i32>(5)), neg<i32, overflow=ub>(const<i32>(6)))
// DEFAULT-NEXT:             write<bool>(%20, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%20, ne<i32>(call<i32, signature=fn(i32) -> i32>(%3, neg<i32, overflow=ub>(const<i32>(5))), neg<i32, overflow=ub>(const<i32>(5))));
// DEFAULT-NEXT:         let %21: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%20)
// DEFAULT-NEXT:             write<bool>(%21, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%21, ne<i32>(call<i32, signature=fn(i32) -> i32>(%3, const<i32>(0)), neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         if read<bool>(%21)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %22: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%5, const<i32>(5)), const<i32>(5))
// DEFAULT-NEXT:             write<bool>(%22, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%22, ne<i32>(call<i32, signature=fn(i32) -> i32>(%5, neg<i32, overflow=ub>(const<i32>(5))), const<i32>(4)));
// DEFAULT-NEXT:         let %23: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%22)
// DEFAULT-NEXT:             write<bool>(%23, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%23, ne<i32>(call<i32, signature=fn(i32) -> i32>(%5, const<i32>(0)), neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         if read<bool>(%23)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %24: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%7, const<i32>(5)), neg<i32, overflow=ub>(const<i32>(6)))
// DEFAULT-NEXT:             write<bool>(%24, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%24, ne<i32>(call<i32, signature=fn(i32) -> i32>(%7, neg<i32, overflow=ub>(const<i32>(5))), neg<i32, overflow=ub>(const<i32>(5))));
// DEFAULT-NEXT:         let %25: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%24)
// DEFAULT-NEXT:             write<bool>(%25, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%25, ne<i32>(call<i32, signature=fn(i32) -> i32>(%7, const<i32>(0)), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%25)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %26: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%9, const<i32>(5)), neg<i32, overflow=ub>(const<i32>(6)))
// DEFAULT-NEXT:             write<bool>(%26, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%26, ne<i32>(call<i32, signature=fn(i32) -> i32>(%9, neg<i32, overflow=ub>(const<i32>(5))), neg<i32, overflow=ub>(const<i32>(5))));
// DEFAULT-NEXT:         let %27: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%26)
// DEFAULT-NEXT:             write<bool>(%27, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%27, ne<i32>(call<i32, signature=fn(i32) -> i32>(%9, const<i32>(0)), neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         if read<bool>(%27)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %28: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%11, const<i32>(5)), const<i32>(5))
// DEFAULT-NEXT:             write<bool>(%28, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%28, ne<i32>(call<i32, signature=fn(i32) -> i32>(%11, neg<i32, overflow=ub>(const<i32>(5))), const<i32>(4)));
// DEFAULT-NEXT:         let %29: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%28)
// DEFAULT-NEXT:             write<bool>(%29, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%29, ne<i32>(call<i32, signature=fn(i32) -> i32>(%11, const<i32>(0)), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%29)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %30: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%13, const<i32>(5)), neg<i32, overflow=ub>(const<i32>(6)))
// DEFAULT-NEXT:             write<bool>(%30, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%30, ne<i32>(call<i32, signature=fn(i32) -> i32>(%13, neg<i32, overflow=ub>(const<i32>(5))), neg<i32, overflow=ub>(const<i32>(5))));
// DEFAULT-NEXT:         let %31: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%30)
// DEFAULT-NEXT:             write<bool>(%31, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%31, ne<i32>(call<i32, signature=fn(i32) -> i32>(%13, const<i32>(0)), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%31)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %32: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%15, const<i32>(5)), const<i32>(5))
// DEFAULT-NEXT:             write<bool>(%32, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%32, ne<i32>(call<i32, signature=fn(i32) -> i32>(%15, neg<i32, overflow=ub>(const<i32>(5))), const<i32>(4)));
// DEFAULT-NEXT:         let %33: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%32)
// DEFAULT-NEXT:             write<bool>(%33, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%33, ne<i32>(call<i32, signature=fn(i32) -> i32>(%15, const<i32>(0)), neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         if read<bool>(%33)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
