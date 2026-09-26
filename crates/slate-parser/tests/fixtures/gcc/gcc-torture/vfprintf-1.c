/* { dg-skip-if "requires io" { freestanding } }  */

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
    vfprintf(stdout, fmt, ap);                                                 \
    if (vfprintf(stdout, fmt, ap2) != ret)                                     \
      abort();                                                                 \
    break;
#include "vfprintf-1.c"
#undef test
  default:
    abort();
  }

  va_end(ap);
  va_end(ap2);
}

int main(void) {
#define test(n, ret, fmt, args) inner args;
#include "vfprintf-1.c"
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
// DEFAULT-NEXT:     type @type2 __uint64_t = u64;
// DEFAULT-NEXT:     type @type3 __off_t = i64;
// DEFAULT-NEXT:     type @type4 __off64_t = i64;
// DEFAULT-NEXT:     type @type5 _IO_FILE = struct incomplete;
// DEFAULT-NEXT:     type @type6 FILE = @type5;
// DEFAULT-NEXT:     type @type7 _IO_lock_t = void;
// DEFAULT-NEXT:     type @type8 _IO_marker = struct incomplete;
// DEFAULT-NEXT:     type @type9 _IO_codecvt = struct incomplete;
// DEFAULT-NEXT:     type @type10 _IO_wide_data = struct incomplete;
// DEFAULT-NEXT:     type @type11 va_list = va_list;
// DEFAULT-NEXT:     extern %11 stdout: ptr<@type5> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %23 .str23: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([104, 101, 108, 108, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %24 .str24: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([104, 101, 108, 108, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %25 .str25: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([104, 101, 108, 108, 111, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %26 .str26: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([104, 101, 108, 108, 111, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %27 .str27: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([97, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %28 .str28: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([97, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %29 .str29: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %30 .str30: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %31 .str31: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %32 .str32: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %33 .str33: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %34 .str34: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %35 .str35: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %36 .str36: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %37 .str37: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %38 .str38: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %39 .str39: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %40 .str40: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %41 .str41: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %42 .str42: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %43 .str43: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %44 .str44: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %45 .str45: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([104, 101, 108, 108, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %46 .str46: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([104, 101, 108, 108, 111, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %47 .str47: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([97, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %48 .str48: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %49 .str49: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([104, 101, 108, 108, 111, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %12 @vfprintf(%19 __s: ptr<@type5> [restrict], %20 __format: ptr<const i8> [restrict], %21 __arg: va_list) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %13 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %14 @inner(%15 x: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %16 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %17 ap2: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%16);
// DEFAULT-NEXT:         va_start(%17);
// DEFAULT-NEXT:         switch %22 read<i32>(%15)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %22 const<i32>(0):
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<@type5>, ptr<const i8>, va_list) -> i32>(%12, read<ptr<@type5>>(%11), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%23)), read<va_list>(%16));
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<@type5>, ptr<const i8>, va_list) -> i32>(%12, read<ptr<@type5>>(%11), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%24)), read<va_list>(%17)), const<i32>(5))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%13);
// DEFAULT-NEXT:                 break %22;
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %22 const<i32>(1):
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<@type5>, ptr<const i8>, va_list) -> i32>(%12, read<ptr<@type5>>(%11), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%25)), read<va_list>(%16));
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<@type5>, ptr<const i8>, va_list) -> i32>(%12, read<ptr<@type5>>(%11), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%26)), read<va_list>(%17)), const<i32>(6))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%13);
// DEFAULT-NEXT:                 break %22;
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %22 const<i32>(2):
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<@type5>, ptr<const i8>, va_list) -> i32>(%12, read<ptr<@type5>>(%11), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%27)), read<va_list>(%16));
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<@type5>, ptr<const i8>, va_list) -> i32>(%12, read<ptr<@type5>>(%11), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%28)), read<va_list>(%17)), const<i32>(1))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%13);
// DEFAULT-NEXT:                 break %22;
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %22 const<i32>(3):
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<@type5>, ptr<const i8>, va_list) -> i32>(%12, read<ptr<@type5>>(%11), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%29)), read<va_list>(%16));
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<@type5>, ptr<const i8>, va_list) -> i32>(%12, read<ptr<@type5>>(%11), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%30)), read<va_list>(%17)), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%13);
// DEFAULT-NEXT:                 break %22;
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %22 const<i32>(4):
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<@type5>, ptr<const i8>, va_list) -> i32>(%12, read<ptr<@type5>>(%11), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%31)), read<va_list>(%16));
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<@type5>, ptr<const i8>, va_list) -> i32>(%12, read<ptr<@type5>>(%11), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%32)), read<va_list>(%17)), const<i32>(5))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%13);
// DEFAULT-NEXT:                 break %22;
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %22 const<i32>(5):
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<@type5>, ptr<const i8>, va_list) -> i32>(%12, read<ptr<@type5>>(%11), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%33)), read<va_list>(%16));
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<@type5>, ptr<const i8>, va_list) -> i32>(%12, read<ptr<@type5>>(%11), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%34)), read<va_list>(%17)), const<i32>(6))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%13);
// DEFAULT-NEXT:                 break %22;
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %22 const<i32>(6):
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<@type5>, ptr<const i8>, va_list) -> i32>(%12, read<ptr<@type5>>(%11), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%35)), read<va_list>(%16));
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<@type5>, ptr<const i8>, va_list) -> i32>(%12, read<ptr<@type5>>(%11), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%36)), read<va_list>(%17)), const<i32>(1))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%13);
// DEFAULT-NEXT:                 break %22;
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %22 const<i32>(7):
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<@type5>, ptr<const i8>, va_list) -> i32>(%12, read<ptr<@type5>>(%11), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%37)), read<va_list>(%16));
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<@type5>, ptr<const i8>, va_list) -> i32>(%12, read<ptr<@type5>>(%11), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%38)), read<va_list>(%17)), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%13);
// DEFAULT-NEXT:                 break %22;
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %22 const<i32>(8):
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<@type5>, ptr<const i8>, va_list) -> i32>(%12, read<ptr<@type5>>(%11), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%39)), read<va_list>(%16));
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<@type5>, ptr<const i8>, va_list) -> i32>(%12, read<ptr<@type5>>(%11), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%40)), read<va_list>(%17)), const<i32>(1))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%13);
// DEFAULT-NEXT:                 break %22;
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %22 const<i32>(9):
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<@type5>, ptr<const i8>, va_list) -> i32>(%12, read<ptr<@type5>>(%11), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%41)), read<va_list>(%16));
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<@type5>, ptr<const i8>, va_list) -> i32>(%12, read<ptr<@type5>>(%11), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%42)), read<va_list>(%17)), const<i32>(7))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%13);
// DEFAULT-NEXT:                 break %22;
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %22 const<i32>(10):
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<@type5>, ptr<const i8>, va_list) -> i32>(%12, read<ptr<@type5>>(%11), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%43)), read<va_list>(%16));
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<@type5>, ptr<const i8>, va_list) -> i32>(%12, read<ptr<@type5>>(%11), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%44)), read<va_list>(%17)), const<i32>(2))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%13);
// DEFAULT-NEXT:                 break %22;
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 default %22:
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%13);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         va_end(%16);
// DEFAULT-NEXT:         va_end(%17);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%14, const<i32>(0));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%14, const<i32>(1));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%14, const<i32>(2));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%14, const<i32>(3));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%14, const<i32>(4), array_decay<ptr<i8>, length=Some(6)>(%45));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%14, const<i32>(5), array_decay<ptr<i8>, length=Some(7)>(%46));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%14, const<i32>(6), array_decay<ptr<i8>, length=Some(2)>(%47));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%14, const<i32>(7), array_decay<ptr<i8>, length=Some(1)>(%48));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%14, const<i32>(8), const<i32>(120));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%14, const<i32>(9), array_decay<ptr<i8>, length=Some(7)>(%49));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%14, const<i32>(10), const<i32>(0));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
