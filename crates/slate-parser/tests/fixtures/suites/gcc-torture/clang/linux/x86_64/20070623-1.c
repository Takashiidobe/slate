#include <limits.h>

void abort(void);
void exit(int);

int __attribute__((noinline)) nge(int a, int b) { return -(a >= b); }
int __attribute__((noinline)) ngt(int a, int b) { return -(a > b); }
int __attribute__((noinline)) nle(int a, int b) { return -(a <= b); }
int __attribute__((noinline)) nlt(int a, int b) { return -(a < b); }
int __attribute__((noinline)) neq(int a, int b) { return -(a == b); }
int __attribute__((noinline)) nne(int a, int b) { return -(a != b); }
int __attribute__((noinline)) ngeu(unsigned a, unsigned b) { return -(a >= b); }
int __attribute__((noinline)) ngtu(unsigned a, unsigned b) { return -(a > b); }
int __attribute__((noinline)) nleu(unsigned a, unsigned b) { return -(a <= b); }
int __attribute__((noinline)) nltu(unsigned a, unsigned b) { return -(a < b); }

int main() {
  if (nge(INT_MIN, INT_MAX) != 0)
    abort();
  if (nge(INT_MAX, INT_MIN) != -1)
    abort();
  if (ngt(INT_MIN, INT_MAX) != 0)
    abort();
  if (ngt(INT_MAX, INT_MIN) != -1)
    abort();
  if (nle(INT_MIN, INT_MAX) != -1)
    abort();
  if (nle(INT_MAX, INT_MIN) != 0)
    abort();
  if (nlt(INT_MIN, INT_MAX) != -1)
    abort();
  if (nlt(INT_MAX, INT_MIN) != 0)
    abort();

  if (neq(INT_MIN, INT_MAX) != 0)
    abort();
  if (neq(INT_MAX, INT_MIN) != 0)
    abort();
  if (nne(INT_MIN, INT_MAX) != -1)
    abort();
  if (nne(INT_MAX, INT_MIN) != -1)
    abort();

  if (ngeu(0, ~0U) != 0)
    abort();
  if (ngeu(~0U, 0) != -1)
    abort();
  if (ngtu(0, ~0U) != 0)
    abort();
  if (ngtu(~0U, 0) != -1)
    abort();
  if (nleu(0, ~0U) != -1)
    abort();
  if (nleu(~0U, 0) != 0)
    abort();
  if (nltu(0, ~0U) != -1)
    abort();
  if (nltu(~0U, 0) != 0)
    abort();

  exit(0);
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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%33 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @nge(%3 a: i32, %4 b: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return neg<i32, overflow=ub>(from_bool<i32, reason=promotion>(ge<i32>(read<i32>(%3), read<i32>(%4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @ngt(%6 a: i32, %7 b: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return neg<i32, overflow=ub>(from_bool<i32, reason=promotion>(gt<i32>(read<i32>(%6), read<i32>(%7))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @nle(%9 a: i32, %10 b: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return neg<i32, overflow=ub>(from_bool<i32, reason=promotion>(le<i32>(read<i32>(%9), read<i32>(%10))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @nlt(%12 a: i32, %13 b: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return neg<i32, overflow=ub>(from_bool<i32, reason=promotion>(lt<i32>(read<i32>(%12), read<i32>(%13))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @neq(%15 a: i32, %16 b: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return neg<i32, overflow=ub>(from_bool<i32, reason=promotion>(eq<i32>(read<i32>(%15), read<i32>(%16))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @nne(%18 a: i32, %19 b: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return neg<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32>(read<i32>(%18), read<i32>(%19))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @ngeu(%21 a: u32, %22 b: u32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return neg<i32, overflow=ub>(from_bool<i32, reason=promotion>(ge<u32>(read<u32>(%21), read<u32>(%22))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @ngtu(%24 a: u32, %25 b: u32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return neg<i32, overflow=ub>(from_bool<i32, reason=promotion>(gt<u32>(read<u32>(%24), read<u32>(%25))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %26 @nleu(%27 a: u32, %28 b: u32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return neg<i32, overflow=ub>(from_bool<i32, reason=promotion>(le<u32>(read<u32>(%27), read<u32>(%28))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %29 @nltu(%30 a: u32, %31 b: u32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return neg<i32, overflow=ub>(from_bool<i32, reason=promotion>(lt<u32>(read<u32>(%30), read<u32>(%31))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %32 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%2, sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(2147483647)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%2, const<i32>(2147483647), sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%5, sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(2147483647)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%5, const<i32>(2147483647), sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%8, sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(2147483647)), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%8, const<i32>(2147483647), sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%11, sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(2147483647)), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%11, const<i32>(2147483647), sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%14, sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(2147483647)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%14, const<i32>(2147483647), sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%17, sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(2147483647)), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%17, const<i32>(2147483647), sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32, u32) -> i32>(%20, reinterpret<u32, reason=arg, fits=always>(const<i32>(0)), not<u32>(const<u32>(0))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32, u32) -> i32>(%20, not<u32>(const<u32>(0)), reinterpret<u32, reason=arg, fits=always>(const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32, u32) -> i32>(%23, reinterpret<u32, reason=arg, fits=always>(const<i32>(0)), not<u32>(const<u32>(0))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32, u32) -> i32>(%23, not<u32>(const<u32>(0)), reinterpret<u32, reason=arg, fits=always>(const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32, u32) -> i32>(%26, reinterpret<u32, reason=arg, fits=always>(const<i32>(0)), not<u32>(const<u32>(0))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32, u32) -> i32>(%26, not<u32>(const<u32>(0)), reinterpret<u32, reason=arg, fits=always>(const<i32>(0))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32, u32) -> i32>(%29, reinterpret<u32, reason=arg, fits=always>(const<i32>(0)), not<u32>(const<u32>(0))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32, u32) -> i32>(%29, not<u32>(const<u32>(0)), reinterpret<u32, reason=arg, fits=always>(const<i32>(0))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
