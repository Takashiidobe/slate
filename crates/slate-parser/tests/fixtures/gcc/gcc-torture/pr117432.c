/* PR ipa/117432 */

#include <stdarg.h>

long long r;

__attribute__((noipa)) void baz(int tag, ...) {
  va_list ap;
  va_start(ap, tag);
  if (!r)
    r = va_arg(ap, long long);
  else
    r = va_arg(ap, int);
  va_end(ap);
}

void foo(void) { baz(1, -1, 0); }

void bar(void) { baz(1, -1LL, 0); }

__attribute__((noipa)) void qux(...) {
  va_list ap;
  va_start(ap);
  if (!r)
    r = va_arg(ap, long long);
  else
    r = va_arg(ap, int);
  va_end(ap);
}

void corge(void) { qux(-2, 0); }

void fred(void) { qux(-2LL, 0); }

int main() {
  bar();
  if (r != -1LL)
    __builtin_abort();
  foo();
  if (r != -1)
    __builtin_abort();
  r = 0;
  fred();
  if (r != -2LL)
    __builtin_abort();
  corge();
  if (r != -2)
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
// DEFAULT-NEXT:     type @type0 va_list = va_list;
// DEFAULT-NEXT:     global %1 r: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %2 @baz(%3 tag: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %4 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%4);
// DEFAULT-NEXT:         if not<bool>(ne<i64>(read<i64>(%1), const<i64>(0)))
// DEFAULT-NEXT:             write<i64>(%1, va_arg<i64>(%4));
// DEFAULT-NEXT:             va_arg<i64>(%4);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<i64>(%1, widen<i64, reason=assign>(va_arg<i32>(%4)));
// DEFAULT-NEXT:             widen<i64, reason=assign>(va_arg<i32>(%4));
// DEFAULT-NEXT:         va_end(%4);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%2, const<i32>(1), neg<i32, overflow=ub>(const<i32>(1)), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @bar() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%2, const<i32>(1), neg<i64, overflow=ub>(const<i64>(1)), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @qux(...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %8 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%8);
// DEFAULT-NEXT:         if not<bool>(ne<i64>(read<i64>(%1), const<i64>(0)))
// DEFAULT-NEXT:             write<i64>(%1, va_arg<i64>(%8));
// DEFAULT-NEXT:             va_arg<i64>(%8);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<i64>(%1, widen<i64, reason=assign>(va_arg<i32>(%8)));
// DEFAULT-NEXT:             widen<i64, reason=assign>(va_arg<i32>(%8));
// DEFAULT-NEXT:         va_end(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @corge() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(...) -> void>(%7, neg<i32, overflow=ub>(const<i32>(2)), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @fred() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(...) -> void>(%7, neg<i64, overflow=ub>(const<i64>(2)), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%1), neg<i64, overflow=ub>(const<i64>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%1), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         write<i64>(%1, widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%10);
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%1), neg<i64, overflow=ub>(const<i64>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%1), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(2))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
