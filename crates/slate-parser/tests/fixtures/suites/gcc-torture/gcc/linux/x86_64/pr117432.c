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
// DEFAULT-NEXT:     type @type[[TYPE___gnuc_va_list:[0-9]+]] __gnuc_va_list = va_list;
// DEFAULT-NEXT:     type @type[[TYPE_va_list:[0-9]+]] va_list = va_list;
// DEFAULT-NEXT:     global %[[VALUE_r:[0-9]+]] r: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_baz:[0-9]+]] @baz(%[[VALUE_tag:[0-9]+]] tag: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap]]);
// DEFAULT-NEXT:         if not<bool>(ne<i64>(read<i64>(%[[VALUE_r]]), const<i64>(0)))
// DEFAULT-NEXT:             write<i64>(%[[VALUE_r]], va_arg<i64>(%[[VALUE_ap]]));
// DEFAULT-NEXT:             va_arg<i64>(%[[VALUE_ap]]);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<i64>(%[[VALUE_r]], widen<i64, reason=assign>(va_arg<i32>(%[[VALUE_ap]])));
// DEFAULT-NEXT:             widen<i64, reason=assign>(va_arg<i32>(%[[VALUE_ap]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%[[VALUE_baz]], const<i32>(1), neg<i32, overflow=ub>(const<i32>(1)), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%[[VALUE_baz]], const<i32>(1), neg<i64, overflow=ub>(const<i64>(1)), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_qux:[0-9]+]] @qux(...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_2:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_2]]);
// DEFAULT-NEXT:         if not<bool>(ne<i64>(read<i64>(%[[VALUE_r]]), const<i64>(0)))
// DEFAULT-NEXT:             write<i64>(%[[VALUE_r]], va_arg<i64>(%[[VALUE_ap_2]]));
// DEFAULT-NEXT:             va_arg<i64>(%[[VALUE_ap_2]]);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<i64>(%[[VALUE_r]], widen<i64, reason=assign>(va_arg<i32>(%[[VALUE_ap_2]])));
// DEFAULT-NEXT:             widen<i64, reason=assign>(va_arg<i32>(%[[VALUE_ap_2]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_corge:[0-9]+]] @corge() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(...) -> void>(%[[VALUE_qux]], neg<i32, overflow=ub>(const<i32>(2)), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fred:[0-9]+]] @fred() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(...) -> void>(%[[VALUE_qux]], neg<i64, overflow=ub>(const<i64>(2)), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_bar]]);
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%[[VALUE_r]]), neg<i64, overflow=ub>(const<i64>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_foo]]);
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%[[VALUE_r]]), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         write<i64>(%[[VALUE_r]], widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_fred]]);
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%[[VALUE_r]]), neg<i64, overflow=ub>(const<i64>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_corge]]);
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%[[VALUE_r]]), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(2))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
