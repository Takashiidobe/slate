/* { dg-skip-if "requires io" { freestanding } }  */
/* { dg-xfail-run-if {unexpected PTX 'vprintf' return value} { nvptx-*-* } } */

// SLATE-FILECHECK-DEFINES DEFAULT

#ifndef test
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

volatile int should_optimize;

int __attribute__((noinline)) __vprintf_chk(int flag, const char *fmt,
                                            va_list ap) {
#ifdef __OPTIMIZE__
  if (should_optimize)
    abort();
#endif
  should_optimize = 1;
  return vprintf(fmt, ap);
}

void inner(int x, ...) {
  va_list ap, ap2;
  va_start(ap, x);
  va_start(ap2, x);

  switch (x) {
#define test(n, ret, opt, fmt, args)                                           \
  case n:                                                                      \
    should_optimize = opt;                                                     \
    __vprintf_chk(1, fmt, ap);                                                 \
    if (!should_optimize)                                                      \
      abort();                                                                 \
    should_optimize = 0;                                                       \
    if (__vprintf_chk(1, fmt, ap2) != ret)                                     \
      abort();                                                                 \
    if (!should_optimize)                                                      \
      abort();                                                                 \
    break;
#include "vprintf-chk-1.c"
#undef test
  default:
    abort();
  }

  va_end(ap);
  va_end(ap2);
}

int main(void) {
#define test(n, ret, opt, fmt, args) inner args;
#include "vprintf-chk-1.c"
#undef test
  return 0;
}

#else
test(0, 5, 0, "hello", (0));
test(1, 6, 1, "hello\n", (1));
test(2, 1, 1, "a", (2));
test(3, 0, 1, "", (3));
test(4, 5, 0, "%s", (4, "hello"));
test(5, 6, 0, "%s", (5, "hello\n"));
test(6, 1, 0, "%s", (6, "a"));
test(7, 0, 0, "%s", (7, ""));
test(8, 1, 0, "%c", (8, 'x'));
test(9, 7, 0, "%s\n", (9, "hello\n"));
test(10, 2, 0, "%d\n", (10, 0));
#endif

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
// DEFAULT-NEXT:     global %17 .str17: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([104, 101, 108, 108, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %18 .str18: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([104, 101, 108, 108, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %19 .str19: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([104, 101, 108, 108, 111, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %20 .str20: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([104, 101, 108, 108, 111, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %21 .str21: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([97, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %22 .str22: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([97, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %23 .str23: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %24 .str24: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %25 .str25: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %26 .str26: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %27 .str27: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %28 .str28: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %29 .str29: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %30 .str30: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %31 .str31: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %32 .str32: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %33 .str33: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %34 .str34: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %35 .str35: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %36 .str36: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %37 .str37: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %38 .str38: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %39 .str39: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([104, 101, 108, 108, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %40 .str40: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([104, 101, 108, 108, 111, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %41 .str41: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([97, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %42 .str42: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %43 .str43: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([104, 101, 108, 108, 111, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %2 @vprintf(%14 __format: ptr<const i8> [restrict], %15 __arg: va_list) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %3 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %5 @__vprintf_chk(%6 flag: i32, %7 fmt: ptr<const i8>, %8 ap: va_list) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32, volatile>(%4, const<i32>(1));
// DEFAULT-NEXT:         return call<i32, signature=fn(ptr<const i8>, va_list) -> i32>(%2, read<ptr<const i8>>(%7), read<va_list>(%8));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @inner(%10 x: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %11 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %12 ap2: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%11);
// DEFAULT-NEXT:         va_start(%12);
// DEFAULT-NEXT:         switch %16 read<i32>(%10)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %16 const<i32>(0):
// DEFAULT-NEXT:                     write<i32, volatile>(%4, const<i32>(0));
// DEFAULT-NEXT:                 call<i32, signature=fn(i32, ptr<const i8>, va_list) -> i32>(%5, const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%17)), read<va_list>(%11));
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%4), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:                 write<i32, volatile>(%4, const<i32>(0));
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(i32, ptr<const i8>, va_list) -> i32>(%5, const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%18)), read<va_list>(%12)), const<i32>(5))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%4), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:                 break %16;
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %16 const<i32>(1):
// DEFAULT-NEXT:                     write<i32, volatile>(%4, const<i32>(1));
// DEFAULT-NEXT:                 call<i32, signature=fn(i32, ptr<const i8>, va_list) -> i32>(%5, const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%19)), read<va_list>(%11));
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%4), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:                 write<i32, volatile>(%4, const<i32>(0));
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(i32, ptr<const i8>, va_list) -> i32>(%5, const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%20)), read<va_list>(%12)), const<i32>(6))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%4), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:                 break %16;
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %16 const<i32>(2):
// DEFAULT-NEXT:                     write<i32, volatile>(%4, const<i32>(1));
// DEFAULT-NEXT:                 call<i32, signature=fn(i32, ptr<const i8>, va_list) -> i32>(%5, const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%21)), read<va_list>(%11));
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%4), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:                 write<i32, volatile>(%4, const<i32>(0));
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(i32, ptr<const i8>, va_list) -> i32>(%5, const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%22)), read<va_list>(%12)), const<i32>(1))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%4), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:                 break %16;
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %16 const<i32>(3):
// DEFAULT-NEXT:                     write<i32, volatile>(%4, const<i32>(1));
// DEFAULT-NEXT:                 call<i32, signature=fn(i32, ptr<const i8>, va_list) -> i32>(%5, const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%23)), read<va_list>(%11));
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%4), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:                 write<i32, volatile>(%4, const<i32>(0));
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(i32, ptr<const i8>, va_list) -> i32>(%5, const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%24)), read<va_list>(%12)), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%4), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:                 break %16;
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %16 const<i32>(4):
// DEFAULT-NEXT:                     write<i32, volatile>(%4, const<i32>(0));
// DEFAULT-NEXT:                 call<i32, signature=fn(i32, ptr<const i8>, va_list) -> i32>(%5, const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%25)), read<va_list>(%11));
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%4), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:                 write<i32, volatile>(%4, const<i32>(0));
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(i32, ptr<const i8>, va_list) -> i32>(%5, const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%26)), read<va_list>(%12)), const<i32>(5))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%4), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:                 break %16;
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %16 const<i32>(5):
// DEFAULT-NEXT:                     write<i32, volatile>(%4, const<i32>(0));
// DEFAULT-NEXT:                 call<i32, signature=fn(i32, ptr<const i8>, va_list) -> i32>(%5, const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%27)), read<va_list>(%11));
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%4), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:                 write<i32, volatile>(%4, const<i32>(0));
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(i32, ptr<const i8>, va_list) -> i32>(%5, const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%28)), read<va_list>(%12)), const<i32>(6))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%4), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:                 break %16;
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %16 const<i32>(6):
// DEFAULT-NEXT:                     write<i32, volatile>(%4, const<i32>(0));
// DEFAULT-NEXT:                 call<i32, signature=fn(i32, ptr<const i8>, va_list) -> i32>(%5, const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%29)), read<va_list>(%11));
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%4), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:                 write<i32, volatile>(%4, const<i32>(0));
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(i32, ptr<const i8>, va_list) -> i32>(%5, const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%30)), read<va_list>(%12)), const<i32>(1))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%4), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:                 break %16;
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %16 const<i32>(7):
// DEFAULT-NEXT:                     write<i32, volatile>(%4, const<i32>(0));
// DEFAULT-NEXT:                 call<i32, signature=fn(i32, ptr<const i8>, va_list) -> i32>(%5, const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%31)), read<va_list>(%11));
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%4), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:                 write<i32, volatile>(%4, const<i32>(0));
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(i32, ptr<const i8>, va_list) -> i32>(%5, const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%32)), read<va_list>(%12)), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%4), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:                 break %16;
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %16 const<i32>(8):
// DEFAULT-NEXT:                     write<i32, volatile>(%4, const<i32>(0));
// DEFAULT-NEXT:                 call<i32, signature=fn(i32, ptr<const i8>, va_list) -> i32>(%5, const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%33)), read<va_list>(%11));
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%4), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:                 write<i32, volatile>(%4, const<i32>(0));
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(i32, ptr<const i8>, va_list) -> i32>(%5, const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%34)), read<va_list>(%12)), const<i32>(1))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%4), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:                 break %16;
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %16 const<i32>(9):
// DEFAULT-NEXT:                     write<i32, volatile>(%4, const<i32>(0));
// DEFAULT-NEXT:                 call<i32, signature=fn(i32, ptr<const i8>, va_list) -> i32>(%5, const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%35)), read<va_list>(%11));
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%4), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:                 write<i32, volatile>(%4, const<i32>(0));
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(i32, ptr<const i8>, va_list) -> i32>(%5, const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%36)), read<va_list>(%12)), const<i32>(7))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%4), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:                 break %16;
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %16 const<i32>(10):
// DEFAULT-NEXT:                     write<i32, volatile>(%4, const<i32>(0));
// DEFAULT-NEXT:                 call<i32, signature=fn(i32, ptr<const i8>, va_list) -> i32>(%5, const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%37)), read<va_list>(%11));
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%4), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:                 write<i32, volatile>(%4, const<i32>(0));
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(i32, ptr<const i8>, va_list) -> i32>(%5, const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%38)), read<va_list>(%12)), const<i32>(2))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%4), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:                 break %16;
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 default %16:
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         va_end(%11);
// DEFAULT-NEXT:         va_end(%12);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%9, const<i32>(0));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%9, const<i32>(1));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%9, const<i32>(2));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%9, const<i32>(3));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%9, const<i32>(4), array_decay<ptr<i8>, length=Some(6)>(%39));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%9, const<i32>(5), array_decay<ptr<i8>, length=Some(7)>(%40));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%9, const<i32>(6), array_decay<ptr<i8>, length=Some(2)>(%41));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%9, const<i32>(7), array_decay<ptr<i8>, length=Some(1)>(%42));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%9, const<i32>(8), const<i32>(120));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%9, const<i32>(9), array_decay<ptr<i8>, length=Some(7)>(%43));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%9, const<i32>(10), const<i32>(0));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
