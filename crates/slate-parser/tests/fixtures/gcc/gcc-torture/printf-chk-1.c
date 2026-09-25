/* { dg-skip-if "requires io" { freestanding } }  */
/* { dg-xfail-run-if {unexpected PTX 'vprintf' return value} { nvptx-*-* } } */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

volatile int should_optimize;

int __attribute__((noinline)) __printf_chk(int flag, const char *fmt, ...) {
  va_list ap;
  int     ret;
#ifdef __OPTIMIZE__
  if (should_optimize)
    abort();
#endif
  should_optimize = 1;
  va_start(ap, fmt);
  ret = vprintf(fmt, ap);
  va_end(ap);
  return ret;
}

int main(void) {
#define test(ret, opt, args...)                                                \
  should_optimize = opt;                                                       \
  __printf_chk(1, args);                                                       \
  if (!should_optimize)                                                        \
    abort();                                                                   \
  should_optimize = 0;                                                         \
  if (__printf_chk(1, args) != ret)                                            \
    abort();                                                                   \
  if (!should_optimize)                                                        \
    abort();
  test(5, 0, "hello");
  test(6, 1, "hello\n");
  test(1, 1, "a");
  test(0, 1, "");
  test(5, 0, "%s", "hello");
  test(6, 1, "%s", "hello\n");
  test(1, 1, "%s", "a");
  test(0, 1, "%s", "");
  test(1, 1, "%c", 'x');
  test(7, 1, "%s\n", "hello\n");
  test(2, 0, "%d\n", 0);
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
// DEFAULT-NEXT:     type @type0 __gnuc_va_list = va_list;
// DEFAULT-NEXT:     type @type1 va_list = va_list;
// DEFAULT-NEXT:     type @type2 va_list = va_list;
// DEFAULT-NEXT:     global %4 should_optimize: volatile i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %13 .str13: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([104, 101, 108, 108, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %14 .str14: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([104, 101, 108, 108, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %15 .str15: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([104, 101, 108, 108, 111, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %16 .str16: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([104, 101, 108, 108, 111, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %17 .str17: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([97, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %18 .str18: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([97, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %19 .str19: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %20 .str20: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %21 .str21: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %22 .str22: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %23 .str23: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %24 .str24: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %25 .str25: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %26 .str26: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %27 .str27: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %28 .str28: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %29 .str29: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %30 .str30: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %31 .str31: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %32 .str32: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %33 .str33: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %34 .str34: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %2 @vprintf(%11 __format: ptr<const i8> [restrict], %12 __arg: va_list) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %3 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %5 @__printf_chk(%6 flag: i32, %7 fmt: ptr<const i8>, ...) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %8 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %9 ret: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32, volatile>(%4, const<i32>(1));
// DEFAULT-NEXT:         va_start(%8);
// DEFAULT-NEXT:         write<i32>(%9, call<i32, signature=fn(ptr<const i8>, va_list) -> i32>(%2, read<ptr<const i8>>(%7), read<va_list>(%8)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, va_list) -> i32>(%2, read<ptr<const i8>>(%7), read<va_list>(%8));
// DEFAULT-NEXT:         va_end(%8);
// DEFAULT-NEXT:         return read<i32>(%9);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<i32, volatile>(%4, const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, ptr<const i8>, ...) -> i32>(%5, const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%13)));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%4), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         write<i32, volatile>(%4, const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, ptr<const i8>, ...) -> i32>(%5, const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%14))), const<i32>(5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%4), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         write<i32, volatile>(%4, const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, ptr<const i8>, ...) -> i32>(%5, const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%15)));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%4), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         write<i32, volatile>(%4, const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, ptr<const i8>, ...) -> i32>(%5, const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%16))), const<i32>(6))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%4), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         write<i32, volatile>(%4, const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, ptr<const i8>, ...) -> i32>(%5, const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%17)));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%4), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         write<i32, volatile>(%4, const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, ptr<const i8>, ...) -> i32>(%5, const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%18))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%4), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         write<i32, volatile>(%4, const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, ptr<const i8>, ...) -> i32>(%5, const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%19)));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%4), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         write<i32, volatile>(%4, const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, ptr<const i8>, ...) -> i32>(%5, const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%20))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%4), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         write<i32, volatile>(%4, const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, ptr<const i8>, ...) -> i32>(%5, const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%21)));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%4), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         write<i32, volatile>(%4, const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, ptr<const i8>, ...) -> i32>(%5, const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%22))), const<i32>(5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%4), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         write<i32, volatile>(%4, const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, ptr<const i8>, ...) -> i32>(%5, const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%23)));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%4), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         write<i32, volatile>(%4, const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, ptr<const i8>, ...) -> i32>(%5, const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%24))), const<i32>(6))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%4), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         write<i32, volatile>(%4, const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, ptr<const i8>, ...) -> i32>(%5, const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%25)));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%4), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         write<i32, volatile>(%4, const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, ptr<const i8>, ...) -> i32>(%5, const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%26))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%4), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         write<i32, volatile>(%4, const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, ptr<const i8>, ...) -> i32>(%5, const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%27)));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%4), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         write<i32, volatile>(%4, const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, ptr<const i8>, ...) -> i32>(%5, const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%28))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%4), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         write<i32, volatile>(%4, const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, ptr<const i8>, ...) -> i32>(%5, const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%29)));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%4), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         write<i32, volatile>(%4, const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, ptr<const i8>, ...) -> i32>(%5, const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%30))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%4), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         write<i32, volatile>(%4, const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, ptr<const i8>, ...) -> i32>(%5, const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%31)));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%4), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         write<i32, volatile>(%4, const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, ptr<const i8>, ...) -> i32>(%5, const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%32))), const<i32>(7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%4), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         write<i32, volatile>(%4, const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, ptr<const i8>, ...) -> i32>(%5, const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%33)));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%4), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         write<i32, volatile>(%4, const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, ptr<const i8>, ...) -> i32>(%5, const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%34))), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%4), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
