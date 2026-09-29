/* { dg-skip-if "requires io" { freestanding } }  */

// SLATE-FILECHECK-DEFINES DEFAULT

#ifndef test
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

volatile int should_optimize;

int __attribute__((noinline)) __vfprintf_chk(FILE *f, int flag, const char *fmt,
                                             va_list ap) {
#ifdef __OPTIMIZE__
  if (should_optimize)
    abort();
#endif
  should_optimize = 1;
  return vfprintf(f, fmt, ap);
}

void inner(int x, ...) {
  va_list ap, ap2;
  va_start(ap, x);
  va_start(ap2, x);

  switch (x) {
#define test(n, ret, opt, fmt, args)                                           \
  case n:                                                                      \
    should_optimize = opt;                                                     \
    __vfprintf_chk(stdout, 1, fmt, ap);                                        \
    if (!should_optimize)                                                      \
      abort();                                                                 \
    should_optimize = 0;                                                       \
    if (__vfprintf_chk(stdout, 1, fmt, ap2) != ret)                            \
      abort();                                                                 \
    if (!should_optimize)                                                      \
      abort();                                                                 \
    break;
#include "vfprintf-chk-1.c"
#undef test
  default:
    abort();
  }

  va_end(ap);
  va_end(ap2);
}

int main(void) {
#define test(n, ret, opt, fmt, args) inner args;
#include "vfprintf-chk-1.c"
#undef test
  return 0;
}

#else
test(0, 5, 1, "hello", (0));
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
// DEFAULT-NEXT:     type @type2 __uint64_t = u64;
// DEFAULT-NEXT:     type @type3 __off_t = i64;
// DEFAULT-NEXT:     type @type4 __off64_t = i64;
// DEFAULT-NEXT:     type @type5 _IO_FILE = struct {
// DEFAULT-NEXT:         field0 _flags: i32;
// DEFAULT-NEXT:         field1 _IO_read_ptr: ptr<i8>;
// DEFAULT-NEXT:         field2 _IO_read_end: ptr<i8>;
// DEFAULT-NEXT:         field3 _IO_read_base: ptr<i8>;
// DEFAULT-NEXT:         field4 _IO_write_base: ptr<i8>;
// DEFAULT-NEXT:         field5 _IO_write_ptr: ptr<i8>;
// DEFAULT-NEXT:         field6 _IO_write_end: ptr<i8>;
// DEFAULT-NEXT:         field7 _IO_buf_base: ptr<i8>;
// DEFAULT-NEXT:         field8 _IO_buf_end: ptr<i8>;
// DEFAULT-NEXT:         field9 _IO_save_base: ptr<i8>;
// DEFAULT-NEXT:         field10 _IO_backup_base: ptr<i8>;
// DEFAULT-NEXT:         field11 _IO_save_end: ptr<i8>;
// DEFAULT-NEXT:         field12 _markers: ptr<@type7>;
// DEFAULT-NEXT:         field13 _chain: ptr<@type5>;
// DEFAULT-NEXT:         field14 _fileno: i32;
// DEFAULT-NEXT:         field15 _flags2: i32 : 24;
// DEFAULT-NEXT:         field16 _short_backupbuf: array<i8, 1>;
// DEFAULT-NEXT:         field17 _old_offset: i64;
// DEFAULT-NEXT:         field18 _cur_column: u16;
// DEFAULT-NEXT:         field19 _vtable_offset: i8;
// DEFAULT-NEXT:         field20 _shortbuf: array<i8, 1>;
// DEFAULT-NEXT:         field21 _lock: ptr<void>;
// DEFAULT-NEXT:         field22 _offset: i64;
// DEFAULT-NEXT:         field23 _codecvt: ptr<@type8>;
// DEFAULT-NEXT:         field24 _wide_data: ptr<@type9>;
// DEFAULT-NEXT:         field25 _freeres_list: ptr<@type5>;
// DEFAULT-NEXT:         field26 _freeres_buf: ptr<void>;
// DEFAULT-NEXT:         field27 _prevchain: ptr<ptr<@type5>>;
// DEFAULT-NEXT:         field28 _mode: i32;
// DEFAULT-NEXT:         field29 _unused3: i32;
// DEFAULT-NEXT:         field30 _total_written: u64;
// DEFAULT-NEXT:         field31 _unused2: array<i8, 8>;
// DEFAULT-NEXT:     } [size=216, align=8, offsets=[0, 8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 96, 104, 112, 116, 119, 120, 128, 130, 131, 136, 144, 152, 160, 168, 176, 184, 192, 196, 200, 208], bit_offsets=[None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, Some(928), None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None], bit_units=[(116, 3)], field_units=[None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, Some(0), None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None]];
// DEFAULT-NEXT:     type @type6 FILE = @type5;
// DEFAULT-NEXT:     type @type7 _IO_marker = struct incomplete;
// DEFAULT-NEXT:     type @type8 _IO_codecvt = struct incomplete;
// DEFAULT-NEXT:     type @type9 _IO_wide_data = struct incomplete;
// DEFAULT-NEXT:     type @type10 _IO_lock_t = void;
// DEFAULT-NEXT:     type @type11 va_list = va_list;
// DEFAULT-NEXT:     extern %11 stdout: ptr<@type5> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %17 should_optimize: volatile i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %32 .str32: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([104, 101, 108, 108, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %33 .str33: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([104, 101, 108, 108, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %34 .str34: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([104, 101, 108, 108, 111, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %35 .str35: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([104, 101, 108, 108, 111, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %36 .str36: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([97, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %37 .str37: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([97, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %38 .str38: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %39 .str39: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %40 .str40: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %41 .str41: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %42 .str42: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %43 .str43: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %44 .str44: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %45 .str45: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %46 .str46: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %47 .str47: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %48 .str48: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %49 .str49: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %50 .str50: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %51 .str51: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %52 .str52: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %53 .str53: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %54 .str54: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([104, 101, 108, 108, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %55 .str55: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([104, 101, 108, 108, 111, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %56 .str56: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([97, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %57 .str57: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %58 .str58: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([104, 101, 108, 108, 111, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %15 @vfprintf(%28 __s: ptr<@type5> [restrict], %29 __format: ptr<const i8> [restrict], %30 __arg: va_list) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %16 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %18 @__vfprintf_chk(%19 f: ptr<@type5>, %20 flag: i32, %21 fmt: ptr<const i8>, %22 ap: va_list) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32, volatile>(%17, const<i32>(1));
// DEFAULT-NEXT:         return call<i32, signature=fn(ptr<@type5>, ptr<const i8>, va_list) -> i32>(%15, read<ptr<@type5>>(%19), read<ptr<const i8>>(%21), read<va_list>(%22));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @inner(%24 x: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %25 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %26 ap2: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%25);
// DEFAULT-NEXT:         va_start(%26);
// DEFAULT-NEXT:         switch %31 read<i32>(%24)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %31 const<i32>(0):
// DEFAULT-NEXT:                     write<i32, volatile>(%17, const<i32>(1));
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, va_list) -> i32>(%18, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%32)), read<va_list>(%25));
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%17), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 write<i32, volatile>(%17, const<i32>(0));
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, va_list) -> i32>(%18, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%33)), read<va_list>(%26)), const<i32>(5))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%17), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 break %31;
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %31 const<i32>(1):
// DEFAULT-NEXT:                     write<i32, volatile>(%17, const<i32>(1));
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, va_list) -> i32>(%18, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%34)), read<va_list>(%25));
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%17), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 write<i32, volatile>(%17, const<i32>(0));
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, va_list) -> i32>(%18, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%35)), read<va_list>(%26)), const<i32>(6))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%17), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 break %31;
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %31 const<i32>(2):
// DEFAULT-NEXT:                     write<i32, volatile>(%17, const<i32>(1));
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, va_list) -> i32>(%18, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%36)), read<va_list>(%25));
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%17), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 write<i32, volatile>(%17, const<i32>(0));
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, va_list) -> i32>(%18, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%37)), read<va_list>(%26)), const<i32>(1))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%17), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 break %31;
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %31 const<i32>(3):
// DEFAULT-NEXT:                     write<i32, volatile>(%17, const<i32>(1));
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, va_list) -> i32>(%18, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%38)), read<va_list>(%25));
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%17), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 write<i32, volatile>(%17, const<i32>(0));
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, va_list) -> i32>(%18, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%39)), read<va_list>(%26)), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%17), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 break %31;
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %31 const<i32>(4):
// DEFAULT-NEXT:                     write<i32, volatile>(%17, const<i32>(0));
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, va_list) -> i32>(%18, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%40)), read<va_list>(%25));
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%17), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 write<i32, volatile>(%17, const<i32>(0));
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, va_list) -> i32>(%18, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%41)), read<va_list>(%26)), const<i32>(5))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%17), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 break %31;
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %31 const<i32>(5):
// DEFAULT-NEXT:                     write<i32, volatile>(%17, const<i32>(0));
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, va_list) -> i32>(%18, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%42)), read<va_list>(%25));
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%17), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 write<i32, volatile>(%17, const<i32>(0));
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, va_list) -> i32>(%18, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%43)), read<va_list>(%26)), const<i32>(6))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%17), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 break %31;
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %31 const<i32>(6):
// DEFAULT-NEXT:                     write<i32, volatile>(%17, const<i32>(0));
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, va_list) -> i32>(%18, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%44)), read<va_list>(%25));
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%17), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 write<i32, volatile>(%17, const<i32>(0));
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, va_list) -> i32>(%18, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%45)), read<va_list>(%26)), const<i32>(1))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%17), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 break %31;
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %31 const<i32>(7):
// DEFAULT-NEXT:                     write<i32, volatile>(%17, const<i32>(0));
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, va_list) -> i32>(%18, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%46)), read<va_list>(%25));
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%17), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 write<i32, volatile>(%17, const<i32>(0));
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, va_list) -> i32>(%18, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%47)), read<va_list>(%26)), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%17), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 break %31;
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %31 const<i32>(8):
// DEFAULT-NEXT:                     write<i32, volatile>(%17, const<i32>(0));
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, va_list) -> i32>(%18, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%48)), read<va_list>(%25));
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%17), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 write<i32, volatile>(%17, const<i32>(0));
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, va_list) -> i32>(%18, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%49)), read<va_list>(%26)), const<i32>(1))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%17), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 break %31;
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %31 const<i32>(9):
// DEFAULT-NEXT:                     write<i32, volatile>(%17, const<i32>(0));
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, va_list) -> i32>(%18, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%50)), read<va_list>(%25));
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%17), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 write<i32, volatile>(%17, const<i32>(0));
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, va_list) -> i32>(%18, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%51)), read<va_list>(%26)), const<i32>(7))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%17), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 break %31;
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %31 const<i32>(10):
// DEFAULT-NEXT:                     write<i32, volatile>(%17, const<i32>(0));
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, va_list) -> i32>(%18, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%52)), read<va_list>(%25));
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%17), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 write<i32, volatile>(%17, const<i32>(0));
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, va_list) -> i32>(%18, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%53)), read<va_list>(%26)), const<i32>(2))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32, volatile>(%17), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:                 break %31;
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 default %31:
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         va_end(%25);
// DEFAULT-NEXT:         va_end(%26);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %27 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%23, const<i32>(0));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%23, const<i32>(1));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%23, const<i32>(2));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%23, const<i32>(3));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%23, const<i32>(4), array_decay<ptr<i8>, length=Some(6)>(%54));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%23, const<i32>(5), array_decay<ptr<i8>, length=Some(7)>(%55));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%23, const<i32>(6), array_decay<ptr<i8>, length=Some(2)>(%56));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%23, const<i32>(7), array_decay<ptr<i8>, length=Some(1)>(%57));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%23, const<i32>(8), const<i32>(120));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%23, const<i32>(9), array_decay<ptr<i8>, length=Some(7)>(%58));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%23, const<i32>(10), const<i32>(0));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
