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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_nge:[0-9]+]] @nge(%[[VALUE_a:[0-9]+]] a: i32, %[[VALUE_b:[0-9]+]] b: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return neg<i32, overflow=ub>(from_bool<i32, reason=promotion>(ge<i32>(read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ngt:[0-9]+]] @ngt(%[[VALUE_a_2:[0-9]+]] a: i32, %[[VALUE_b_2:[0-9]+]] b: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return neg<i32, overflow=ub>(from_bool<i32, reason=promotion>(gt<i32>(read<i32>(%[[VALUE_a_2]]), read<i32>(%[[VALUE_b_2]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_nle:[0-9]+]] @nle(%[[VALUE_a_3:[0-9]+]] a: i32, %[[VALUE_b_3:[0-9]+]] b: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return neg<i32, overflow=ub>(from_bool<i32, reason=promotion>(le<i32>(read<i32>(%[[VALUE_a_3]]), read<i32>(%[[VALUE_b_3]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_nlt:[0-9]+]] @nlt(%[[VALUE_a_4:[0-9]+]] a: i32, %[[VALUE_b_4:[0-9]+]] b: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return neg<i32, overflow=ub>(from_bool<i32, reason=promotion>(lt<i32>(read<i32>(%[[VALUE_a_4]]), read<i32>(%[[VALUE_b_4]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_neq:[0-9]+]] @neq(%[[VALUE_a_5:[0-9]+]] a: i32, %[[VALUE_b_5:[0-9]+]] b: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return neg<i32, overflow=ub>(from_bool<i32, reason=promotion>(eq<i32>(read<i32>(%[[VALUE_a_5]]), read<i32>(%[[VALUE_b_5]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_nne:[0-9]+]] @nne(%[[VALUE_a_6:[0-9]+]] a: i32, %[[VALUE_b_6:[0-9]+]] b: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return neg<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32>(read<i32>(%[[VALUE_a_6]]), read<i32>(%[[VALUE_b_6]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ngeu:[0-9]+]] @ngeu(%[[VALUE_a_7:[0-9]+]] a: u32, %[[VALUE_b_7:[0-9]+]] b: u32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return neg<i32, overflow=ub>(from_bool<i32, reason=promotion>(ge<u32>(read<u32>(%[[VALUE_a_7]]), read<u32>(%[[VALUE_b_7]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ngtu:[0-9]+]] @ngtu(%[[VALUE_a_8:[0-9]+]] a: u32, %[[VALUE_b_8:[0-9]+]] b: u32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return neg<i32, overflow=ub>(from_bool<i32, reason=promotion>(gt<u32>(read<u32>(%[[VALUE_a_8]]), read<u32>(%[[VALUE_b_8]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_nleu:[0-9]+]] @nleu(%[[VALUE_a_9:[0-9]+]] a: u32, %[[VALUE_b_9:[0-9]+]] b: u32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return neg<i32, overflow=ub>(from_bool<i32, reason=promotion>(le<u32>(read<u32>(%[[VALUE_a_9]]), read<u32>(%[[VALUE_b_9]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_nltu:[0-9]+]] @nltu(%[[VALUE_a_10:[0-9]+]] a: u32, %[[VALUE_b_10:[0-9]+]] b: u32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return neg<i32, overflow=ub>(from_bool<i32, reason=promotion>(lt<u32>(read<u32>(%[[VALUE_a_10]]), read<u32>(%[[VALUE_b_10]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_nge]], sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(2147483647)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_nge]], const<i32>(2147483647), sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_ngt]], sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(2147483647)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_ngt]], const<i32>(2147483647), sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_nle]], sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(2147483647)), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_nle]], const<i32>(2147483647), sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_nlt]], sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(2147483647)), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_nlt]], const<i32>(2147483647), sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_neq]], sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(2147483647)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_neq]], const<i32>(2147483647), sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_nne]], sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(2147483647)), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_nne]], const<i32>(2147483647), sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32, u32) -> i32>(%[[VALUE_ngeu]], reinterpret<u32, reason=arg, fits=always>(const<i32>(0)), not<u32>(const<u32>(0))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32, u32) -> i32>(%[[VALUE_ngeu]], not<u32>(const<u32>(0)), reinterpret<u32, reason=arg, fits=always>(const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32, u32) -> i32>(%[[VALUE_ngtu]], reinterpret<u32, reason=arg, fits=always>(const<i32>(0)), not<u32>(const<u32>(0))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32, u32) -> i32>(%[[VALUE_ngtu]], not<u32>(const<u32>(0)), reinterpret<u32, reason=arg, fits=always>(const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32, u32) -> i32>(%[[VALUE_nleu]], reinterpret<u32, reason=arg, fits=always>(const<i32>(0)), not<u32>(const<u32>(0))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32, u32) -> i32>(%[[VALUE_nleu]], not<u32>(const<u32>(0)), reinterpret<u32, reason=arg, fits=always>(const<i32>(0))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32, u32) -> i32>(%[[VALUE_nltu]], reinterpret<u32, reason=arg, fits=always>(const<i32>(0)), not<u32>(const<u32>(0))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32, u32) -> i32>(%[[VALUE_nltu]], not<u32>(const<u32>(0)), reinterpret<u32, reason=arg, fits=always>(const<i32>(0))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
