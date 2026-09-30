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
// DEFAULT-NEXT:     type @type[[TYPE___gnuc_va_list:[0-9]+]] __gnuc_va_list = va_list;
// DEFAULT-NEXT:     type @type[[TYPE_va_list:[0-9]+]] va_list = va_list;
// DEFAULT-NEXT:     type @type[[TYPE_va_list_2:[0-9]+]] va_list = va_list;
// DEFAULT-NEXT:     global %[[VALUE_should_optimize:[0-9]+]] should_optimize: volatile i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([104, 101, 108, 108, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([104, 101, 108, 108, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([104, 101, 108, 108, 111, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([104, 101, 108, 108, 111, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([97, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_6:[0-9]+]] .str[[VALUE_str_6]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([97, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_7:[0-9]+]] .str[[VALUE_str_7]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_8:[0-9]+]] .str[[VALUE_str_8]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_9:[0-9]+]] .str[[VALUE_str_9]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_10:[0-9]+]] .str[[VALUE_str_10]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_11:[0-9]+]] .str[[VALUE_str_11]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_12:[0-9]+]] .str[[VALUE_str_12]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_13:[0-9]+]] .str[[VALUE_str_13]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_14:[0-9]+]] .str[[VALUE_str_14]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_15:[0-9]+]] .str[[VALUE_str_15]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_16:[0-9]+]] .str[[VALUE_str_16]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_17:[0-9]+]] .str[[VALUE_str_17]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_18:[0-9]+]] .str[[VALUE_str_18]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_19:[0-9]+]] .str[[VALUE_str_19]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_20:[0-9]+]] .str[[VALUE_str_20]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_21:[0-9]+]] .str[[VALUE_str_21]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_22:[0-9]+]] .str[[VALUE_str_22]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_23:[0-9]+]] .str[[VALUE_str_23]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([104, 101, 108, 108, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_24:[0-9]+]] .str[[VALUE_str_24]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([104, 101, 108, 108, 111, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_25:[0-9]+]] .str[[VALUE_str_25]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([97, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_26:[0-9]+]] .str[[VALUE_str_26]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_27:[0-9]+]] .str[[VALUE_str_27]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([104, 101, 108, 108, 111, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_vprintf:[0-9]+]] @vprintf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], %[[VALUE___arg:[0-9]+]] __arg: va_list) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE___vprintf_chk:[0-9]+]] @__vprintf_chk(%[[VALUE_flag:[0-9]+]] flag: i32, %[[VALUE_fmt:[0-9]+]] fmt: ptr<const i8>, %[[VALUE_ap:[0-9]+]] ap: va_list) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32, volatile>(%[[VALUE_should_optimize]], const<i32>(1));
// DEFAULT-NEXT:         return call<i32, signature=fn(ptr<const i8>, va_list) -> i32>(%[[VALUE_vprintf]], read<ptr<const i8>>(%[[VALUE_fmt]]), read<va_list>(%[[VALUE_ap]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_inner:[0-9]+]] @inner(%[[VALUE_x:[0-9]+]] x: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_2:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ap2:[0-9]+]] ap2: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_2]]);
// DEFAULT-NEXT:         va_start(%[[VALUE_ap2]]);
// DEFAULT-NEXT:         switch %[[VALUE0:[0-9]+]] read<i32>(%[[VALUE_x]])
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(0):
// DEFAULT-NEXT:                     write<i32, volatile>(%[[VALUE_should_optimize]], const<i32>(0));
// DEFAULT-NEXT:                 call<i32, signature=fn(i32, ptr<const i8>, va_list) -> i32>(%[[VALUE___vprintf_chk]], const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str]])), read<va_list>(%[[VALUE_ap_2]]));
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%[[VALUE_should_optimize]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 write<i32, volatile>(%[[VALUE_should_optimize]], const<i32>(0));
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(i32, ptr<const i8>, va_list) -> i32>(%[[VALUE___vprintf_chk]], const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_2]])), read<va_list>(%[[VALUE_ap2]])), const<i32>(5))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%[[VALUE_should_optimize]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(1):
// DEFAULT-NEXT:                     write<i32, volatile>(%[[VALUE_should_optimize]], const<i32>(1));
// DEFAULT-NEXT:                 call<i32, signature=fn(i32, ptr<const i8>, va_list) -> i32>(%[[VALUE___vprintf_chk]], const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str_3]])), read<va_list>(%[[VALUE_ap_2]]));
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%[[VALUE_should_optimize]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 write<i32, volatile>(%[[VALUE_should_optimize]], const<i32>(0));
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(i32, ptr<const i8>, va_list) -> i32>(%[[VALUE___vprintf_chk]], const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str_4]])), read<va_list>(%[[VALUE_ap2]])), const<i32>(6))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%[[VALUE_should_optimize]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(2):
// DEFAULT-NEXT:                     write<i32, volatile>(%[[VALUE_should_optimize]], const<i32>(1));
// DEFAULT-NEXT:                 call<i32, signature=fn(i32, ptr<const i8>, va_list) -> i32>(%[[VALUE___vprintf_chk]], const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_5]])), read<va_list>(%[[VALUE_ap_2]]));
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%[[VALUE_should_optimize]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 write<i32, volatile>(%[[VALUE_should_optimize]], const<i32>(0));
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(i32, ptr<const i8>, va_list) -> i32>(%[[VALUE___vprintf_chk]], const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_6]])), read<va_list>(%[[VALUE_ap2]])), const<i32>(1))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%[[VALUE_should_optimize]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(3):
// DEFAULT-NEXT:                     write<i32, volatile>(%[[VALUE_should_optimize]], const<i32>(1));
// DEFAULT-NEXT:                 call<i32, signature=fn(i32, ptr<const i8>, va_list) -> i32>(%[[VALUE___vprintf_chk]], const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_7]])), read<va_list>(%[[VALUE_ap_2]]));
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%[[VALUE_should_optimize]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 write<i32, volatile>(%[[VALUE_should_optimize]], const<i32>(0));
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(i32, ptr<const i8>, va_list) -> i32>(%[[VALUE___vprintf_chk]], const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_8]])), read<va_list>(%[[VALUE_ap2]])), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%[[VALUE_should_optimize]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(4):
// DEFAULT-NEXT:                     write<i32, volatile>(%[[VALUE_should_optimize]], const<i32>(0));
// DEFAULT-NEXT:                 call<i32, signature=fn(i32, ptr<const i8>, va_list) -> i32>(%[[VALUE___vprintf_chk]], const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str_9]])), read<va_list>(%[[VALUE_ap_2]]));
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%[[VALUE_should_optimize]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 write<i32, volatile>(%[[VALUE_should_optimize]], const<i32>(0));
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(i32, ptr<const i8>, va_list) -> i32>(%[[VALUE___vprintf_chk]], const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str_10]])), read<va_list>(%[[VALUE_ap2]])), const<i32>(5))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%[[VALUE_should_optimize]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(5):
// DEFAULT-NEXT:                     write<i32, volatile>(%[[VALUE_should_optimize]], const<i32>(0));
// DEFAULT-NEXT:                 call<i32, signature=fn(i32, ptr<const i8>, va_list) -> i32>(%[[VALUE___vprintf_chk]], const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str_11]])), read<va_list>(%[[VALUE_ap_2]]));
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%[[VALUE_should_optimize]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 write<i32, volatile>(%[[VALUE_should_optimize]], const<i32>(0));
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(i32, ptr<const i8>, va_list) -> i32>(%[[VALUE___vprintf_chk]], const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str_12]])), read<va_list>(%[[VALUE_ap2]])), const<i32>(6))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%[[VALUE_should_optimize]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(6):
// DEFAULT-NEXT:                     write<i32, volatile>(%[[VALUE_should_optimize]], const<i32>(0));
// DEFAULT-NEXT:                 call<i32, signature=fn(i32, ptr<const i8>, va_list) -> i32>(%[[VALUE___vprintf_chk]], const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str_13]])), read<va_list>(%[[VALUE_ap_2]]));
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%[[VALUE_should_optimize]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 write<i32, volatile>(%[[VALUE_should_optimize]], const<i32>(0));
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(i32, ptr<const i8>, va_list) -> i32>(%[[VALUE___vprintf_chk]], const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str_14]])), read<va_list>(%[[VALUE_ap2]])), const<i32>(1))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%[[VALUE_should_optimize]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(7):
// DEFAULT-NEXT:                     write<i32, volatile>(%[[VALUE_should_optimize]], const<i32>(0));
// DEFAULT-NEXT:                 call<i32, signature=fn(i32, ptr<const i8>, va_list) -> i32>(%[[VALUE___vprintf_chk]], const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str_15]])), read<va_list>(%[[VALUE_ap_2]]));
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%[[VALUE_should_optimize]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 write<i32, volatile>(%[[VALUE_should_optimize]], const<i32>(0));
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(i32, ptr<const i8>, va_list) -> i32>(%[[VALUE___vprintf_chk]], const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str_16]])), read<va_list>(%[[VALUE_ap2]])), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%[[VALUE_should_optimize]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(8):
// DEFAULT-NEXT:                     write<i32, volatile>(%[[VALUE_should_optimize]], const<i32>(0));
// DEFAULT-NEXT:                 call<i32, signature=fn(i32, ptr<const i8>, va_list) -> i32>(%[[VALUE___vprintf_chk]], const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str_17]])), read<va_list>(%[[VALUE_ap_2]]));
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%[[VALUE_should_optimize]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 write<i32, volatile>(%[[VALUE_should_optimize]], const<i32>(0));
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(i32, ptr<const i8>, va_list) -> i32>(%[[VALUE___vprintf_chk]], const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str_18]])), read<va_list>(%[[VALUE_ap2]])), const<i32>(1))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%[[VALUE_should_optimize]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(9):
// DEFAULT-NEXT:                     write<i32, volatile>(%[[VALUE_should_optimize]], const<i32>(0));
// DEFAULT-NEXT:                 call<i32, signature=fn(i32, ptr<const i8>, va_list) -> i32>(%[[VALUE___vprintf_chk]], const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_19]])), read<va_list>(%[[VALUE_ap_2]]));
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%[[VALUE_should_optimize]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 write<i32, volatile>(%[[VALUE_should_optimize]], const<i32>(0));
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(i32, ptr<const i8>, va_list) -> i32>(%[[VALUE___vprintf_chk]], const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_20]])), read<va_list>(%[[VALUE_ap2]])), const<i32>(7))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%[[VALUE_should_optimize]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(10):
// DEFAULT-NEXT:                     write<i32, volatile>(%[[VALUE_should_optimize]], const<i32>(0));
// DEFAULT-NEXT:                 call<i32, signature=fn(i32, ptr<const i8>, va_list) -> i32>(%[[VALUE___vprintf_chk]], const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_21]])), read<va_list>(%[[VALUE_ap_2]]));
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%[[VALUE_should_optimize]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 write<i32, volatile>(%[[VALUE_should_optimize]], const<i32>(0));
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(i32, ptr<const i8>, va_list) -> i32>(%[[VALUE___vprintf_chk]], const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_22]])), read<va_list>(%[[VALUE_ap2]])), const<i32>(2))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%[[VALUE_should_optimize]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 default %[[VALUE0]]:
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_2]]);
// DEFAULT-NEXT:         va_end(%[[VALUE_ap2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%[[VALUE_inner]], const<i32>(0));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%[[VALUE_inner]], const<i32>(1));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%[[VALUE_inner]], const<i32>(2));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%[[VALUE_inner]], const<i32>(3));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%[[VALUE_inner]], const<i32>(4), array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_23]]));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%[[VALUE_inner]], const<i32>(5), array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str_24]]));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%[[VALUE_inner]], const<i32>(6), array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_25]]));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%[[VALUE_inner]], const<i32>(7), array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_26]]));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%[[VALUE_inner]], const<i32>(8), const<i32>(120));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%[[VALUE_inner]], const<i32>(9), array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str_27]]));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%[[VALUE_inner]], const<i32>(10), const<i32>(0));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
