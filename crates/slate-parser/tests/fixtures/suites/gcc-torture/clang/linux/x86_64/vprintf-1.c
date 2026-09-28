/* { dg-skip-if "requires io" { freestanding } }  */
/* { dg-xfail-run-if {unexpected PTX 'vprintf' return value} { nvptx-*-* } } */

// SLATE-FILECHECK-DEFINES DEFAULT

#ifndef test
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void inner(int x, ...) {
  va_list ap, ap2;
  va_start(ap, x);
  va_start(ap2, x);

  switch (x) {
#define test(n, ret, fmt, args)                                                \
  case n:                                                                      \
    vprintf(fmt, ap);                                                          \
    if (vprintf(fmt, ap2) != ret)                                              \
      abort();                                                                 \
    break;
#include "vprintf-1.c"
#undef test
  default:
    abort();
  }

  va_end(ap);
  va_end(ap2);
}

int main(void) {
#define test(n, ret, fmt, args) inner args;
#include "vprintf-1.c"
#undef test
  return 0;
}

#else
test(0, 5, "hello", (0));
test(1, 6, "hello\n", (1));
test(2, 1, "a", (2));
test(3, 0, "", (3));
test(4, 5, "%s", (4, "hello"));
test(5, 6, "%s", (5, "hello\n"));
test(6, 1, "%s", (6, "a"));
test(7, 0, "%s", (7, ""));
test(8, 1, "%c", (8, 'x'));
test(9, 7, "%s\n", (9, "hello\n"));
test(10, 2, "%d\n", (10, 0));
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
// DEFAULT-NEXT:     global %14 .str14: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([104, 101, 108, 108, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %15 .str15: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([104, 101, 108, 108, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %16 .str16: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([104, 101, 108, 108, 111, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %17 .str17: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([104, 101, 108, 108, 111, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %18 .str18: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([97, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %19 .str19: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([97, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %20 .str20: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %21 .str21: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %22 .str22: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %23 .str23: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %24 .str24: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %25 .str25: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %26 .str26: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %27 .str27: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %28 .str28: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %29 .str29: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %30 .str30: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %31 .str31: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %32 .str32: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %33 .str33: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %34 .str34: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %35 .str35: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %36 .str36: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([104, 101, 108, 108, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %37 .str37: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([104, 101, 108, 108, 111, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %38 .str38: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([97, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %39 .str39: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %40 .str40: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([104, 101, 108, 108, 111, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %4 @vprintf(%11 __format: ptr<const i8> [restrict], %12 __arg: va_list) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %5 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %6 @inner(%7 x: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %8 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %9 ap2: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%8);
// DEFAULT-NEXT:         va_start(%9);
// DEFAULT-NEXT:         switch %13 read<i32>(%7)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %13 const<i32>(0):
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, va_list) -> i32>(%4, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%14)), read<va_list>(%8));
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<const i8>, va_list) -> i32>(%4, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%15)), read<va_list>(%9)), const<i32>(5))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                 break %13;
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %13 const<i32>(1):
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, va_list) -> i32>(%4, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%16)), read<va_list>(%8));
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<const i8>, va_list) -> i32>(%4, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%17)), read<va_list>(%9)), const<i32>(6))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                 break %13;
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %13 const<i32>(2):
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, va_list) -> i32>(%4, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%18)), read<va_list>(%8));
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<const i8>, va_list) -> i32>(%4, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%19)), read<va_list>(%9)), const<i32>(1))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                 break %13;
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %13 const<i32>(3):
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, va_list) -> i32>(%4, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%20)), read<va_list>(%8));
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<const i8>, va_list) -> i32>(%4, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%21)), read<va_list>(%9)), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                 break %13;
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %13 const<i32>(4):
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, va_list) -> i32>(%4, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%22)), read<va_list>(%8));
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<const i8>, va_list) -> i32>(%4, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%23)), read<va_list>(%9)), const<i32>(5))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                 break %13;
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %13 const<i32>(5):
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, va_list) -> i32>(%4, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%24)), read<va_list>(%8));
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<const i8>, va_list) -> i32>(%4, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%25)), read<va_list>(%9)), const<i32>(6))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                 break %13;
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %13 const<i32>(6):
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, va_list) -> i32>(%4, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%26)), read<va_list>(%8));
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<const i8>, va_list) -> i32>(%4, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%27)), read<va_list>(%9)), const<i32>(1))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                 break %13;
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %13 const<i32>(7):
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, va_list) -> i32>(%4, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%28)), read<va_list>(%8));
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<const i8>, va_list) -> i32>(%4, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%29)), read<va_list>(%9)), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                 break %13;
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %13 const<i32>(8):
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, va_list) -> i32>(%4, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%30)), read<va_list>(%8));
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<const i8>, va_list) -> i32>(%4, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%31)), read<va_list>(%9)), const<i32>(1))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                 break %13;
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %13 const<i32>(9):
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, va_list) -> i32>(%4, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%32)), read<va_list>(%8));
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<const i8>, va_list) -> i32>(%4, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%33)), read<va_list>(%9)), const<i32>(7))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                 break %13;
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %13 const<i32>(10):
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, va_list) -> i32>(%4, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%34)), read<va_list>(%8));
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<const i8>, va_list) -> i32>(%4, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%35)), read<va_list>(%9)), const<i32>(2))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                 break %13;
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 default %13:
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         va_end(%8);
// DEFAULT-NEXT:         va_end(%9);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%6, const<i32>(0));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%6, const<i32>(1));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%6, const<i32>(2));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%6, const<i32>(3));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%6, const<i32>(4), array_decay<ptr<i8>, length=Some(6)>(%36));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%6, const<i32>(5), array_decay<ptr<i8>, length=Some(7)>(%37));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%6, const<i32>(6), array_decay<ptr<i8>, length=Some(2)>(%38));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%6, const<i32>(7), array_decay<ptr<i8>, length=Some(1)>(%39));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%6, const<i32>(8), const<i32>(120));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%6, const<i32>(9), array_decay<ptr<i8>, length=Some(7)>(%40));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%6, const<i32>(10), const<i32>(0));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
