/* { dg-skip-if "requires io" { freestanding } }  */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

volatile int should_optimize;

int __attribute__((noinline)) __fprintf_chk(FILE *f, int flag, const char *fmt,
                                            ...) {
  va_list ap;
  int     ret;
#ifdef __OPTIMIZE__
  if (should_optimize)
    abort();
#endif
  should_optimize = 1;
  va_start(ap, fmt);
  ret = vfprintf(f, fmt, ap);
  va_end(ap);
  return ret;
}

int main(void) {
#define test(ret, opt, args...)                                                \
  should_optimize = opt;                                                       \
  __fprintf_chk(stdout, 1, args);                                              \
  if (!should_optimize)                                                        \
    abort();                                                                   \
  should_optimize = 0;                                                         \
  if (__fprintf_chk(stdout, 1, args) != ret)                                   \
    abort();                                                                   \
  if (!should_optimize)                                                        \
    abort();
  test(5, 1, "hello");
  test(6, 1, "hello\n");
  test(1, 1, "a");
  test(0, 1, "");
  test(5, 1, "%s", "hello");
  test(6, 1, "%s", "hello\n");
  test(1, 1, "%s", "a");
  test(0, 1, "%s", "");
  test(1, 1, "%c", 'x');
  test(7, 0, "%s\n", "hello\n");
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
// DEFAULT-NEXT:     global %14 should_optimize: volatile i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %25 .str25: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([104, 101, 108, 108, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %26 .str26: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([104, 101, 108, 108, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %27 .str27: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([104, 101, 108, 108, 111, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %28 .str28: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([104, 101, 108, 108, 111, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %29 .str29: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([97, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %30 .str30: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([97, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %31 .str31: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %32 .str32: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %33 .str33: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %34 .str34: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %35 .str35: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %36 .str36: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %37 .str37: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %38 .str38: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %39 .str39: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %40 .str40: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %41 .str41: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %42 .str42: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %43 .str43: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %44 .str44: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %45 .str45: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %46 .str46: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %12 @vfprintf(%22 __s: ptr<@type5> [restrict], %23 __format: ptr<const i8> [restrict], %24 __arg: va_list) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %13 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %15 @__fprintf_chk(%16 f: ptr<@type5>, %17 flag: i32, %18 fmt: ptr<const i8>, ...) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %19 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %20 ret: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32, volatile>(%14, const<i32>(1));
// DEFAULT-NEXT:         va_start(%19);
// DEFAULT-NEXT:         write<i32>(%20, call<i32, signature=fn(ptr<@type5>, ptr<const i8>, va_list) -> i32>(%12, read<ptr<@type5>>(%16), read<ptr<const i8>>(%18), read<va_list>(%19)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type5>, ptr<const i8>, va_list) -> i32>(%12, read<ptr<@type5>>(%16), read<ptr<const i8>>(%18), read<va_list>(%19));
// DEFAULT-NEXT:         va_end(%19);
// DEFAULT-NEXT:         return read<i32>(%20);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<i32, volatile>(%14, const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, ...) -> i32>(%15, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%25)));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%14), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         write<i32, volatile>(%14, const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, ...) -> i32>(%15, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%26))), const<i32>(5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%14), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         write<i32, volatile>(%14, const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, ...) -> i32>(%15, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%27)));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%14), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         write<i32, volatile>(%14, const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, ...) -> i32>(%15, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%28))), const<i32>(6))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%14), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         write<i32, volatile>(%14, const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, ...) -> i32>(%15, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%29)));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%14), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         write<i32, volatile>(%14, const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, ...) -> i32>(%15, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%30))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%14), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         write<i32, volatile>(%14, const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, ...) -> i32>(%15, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%31)));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%14), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         write<i32, volatile>(%14, const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, ...) -> i32>(%15, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%32))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%14), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         write<i32, volatile>(%14, const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, ...) -> i32>(%15, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%33)));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%14), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         write<i32, volatile>(%14, const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, ...) -> i32>(%15, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%34))), const<i32>(5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%14), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         write<i32, volatile>(%14, const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, ...) -> i32>(%15, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%35)));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%14), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         write<i32, volatile>(%14, const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, ...) -> i32>(%15, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%36))), const<i32>(6))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%14), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         write<i32, volatile>(%14, const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, ...) -> i32>(%15, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%37)));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%14), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         write<i32, volatile>(%14, const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, ...) -> i32>(%15, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%38))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%14), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         write<i32, volatile>(%14, const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, ...) -> i32>(%15, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%39)));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%14), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         write<i32, volatile>(%14, const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, ...) -> i32>(%15, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%40))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%14), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         write<i32, volatile>(%14, const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, ...) -> i32>(%15, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%41)));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%14), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         write<i32, volatile>(%14, const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, ...) -> i32>(%15, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%42))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%14), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         write<i32, volatile>(%14, const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, ...) -> i32>(%15, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%43)));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%14), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         write<i32, volatile>(%14, const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, ...) -> i32>(%15, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%44))), const<i32>(7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%14), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         write<i32, volatile>(%14, const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, ...) -> i32>(%15, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%45)));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%14), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         write<i32, volatile>(%14, const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, ...) -> i32>(%15, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%46))), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%14), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
