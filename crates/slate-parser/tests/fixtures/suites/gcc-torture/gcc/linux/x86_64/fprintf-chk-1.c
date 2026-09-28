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
// DEFAULT-NEXT:         field12 _markers: ptr<@type8>;
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
// DEFAULT-NEXT:         field23 _codecvt: ptr<@type9>;
// DEFAULT-NEXT:         field24 _wide_data: ptr<@type10>;
// DEFAULT-NEXT:         field25 _freeres_list: ptr<@type5>;
// DEFAULT-NEXT:         field26 _freeres_buf: ptr<void>;
// DEFAULT-NEXT:         field27 _prevchain: ptr<ptr<@type5>>;
// DEFAULT-NEXT:         field28 _mode: i32;
// DEFAULT-NEXT:         field29 _unused3: i32;
// DEFAULT-NEXT:         field30 _total_written: u64;
// DEFAULT-NEXT:         field31 _unused2: array<i8, 8>;
// DEFAULT-NEXT:     } [size=216, align=8, offsets=[0, 8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 96, 104, 112, 116, 119, 120, 128, 130, 131, 136, 144, 152, 160, 168, 176, 184, 192, 196, 200, 208], bit_offsets=[None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, Some(928), None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None], bit_units=[(116, 3)], field_units=[None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, Some(0), None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None]];
// DEFAULT-NEXT:     type @type6 FILE = @type5;
// DEFAULT-NEXT:     type @type7 _IO_lock_t = void;
// DEFAULT-NEXT:     type @type8 _IO_marker = struct incomplete;
// DEFAULT-NEXT:     type @type9 _IO_codecvt = struct incomplete;
// DEFAULT-NEXT:     type @type10 _IO_wide_data = struct incomplete;
// DEFAULT-NEXT:     extern %11 stdout: ptr<@type5> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %17 should_optimize: volatile i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %28 .str28: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([104, 101, 108, 108, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %29 .str29: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([104, 101, 108, 108, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %30 .str30: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([104, 101, 108, 108, 111, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %31 .str31: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([104, 101, 108, 108, 111, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %32 .str32: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([97, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %33 .str33: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([97, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %34 .str34: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %35 .str35: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %36 .str36: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %37 .str37: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %38 .str38: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %39 .str39: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %40 .str40: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %41 .str41: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %42 .str42: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %43 .str43: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %44 .str44: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %45 .str45: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %46 .str46: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %47 .str47: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %48 .str48: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %49 .str49: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %15 @vfprintf(%25 __s: ptr<@type5> [restrict], %26 __format: ptr<const i8> [restrict], %27 __arg: va_list) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %16 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %18 @__fprintf_chk(%19 f: ptr<@type5>, %20 flag: i32, %21 fmt: ptr<const i8>, ...) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %22 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %23 ret: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32, volatile>(%17, const<i32>(1));
// DEFAULT-NEXT:         va_start(%22);
// DEFAULT-NEXT:         write<i32>(%23, call<i32, signature=fn(ptr<@type5>, ptr<const i8>, va_list) -> i32>(%15, read<ptr<@type5>>(%19), read<ptr<const i8>>(%21), read<va_list>(%22)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type5>, ptr<const i8>, va_list) -> i32>(%15, read<ptr<@type5>>(%19), read<ptr<const i8>>(%21), read<va_list>(%22));
// DEFAULT-NEXT:         va_end(%22);
// DEFAULT-NEXT:         return read<i32>(%23);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %24 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<i32, volatile>(%17, const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, ...) -> i32>(%18, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%28)));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%17), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:         write<i32, volatile>(%17, const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, ...) -> i32>(%18, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%29))), const<i32>(5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%17), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         write<i32, volatile>(%17, const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, ...) -> i32>(%18, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%30)));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%17), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:         write<i32, volatile>(%17, const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, ...) -> i32>(%18, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%31))), const<i32>(6))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%17), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         write<i32, volatile>(%17, const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, ...) -> i32>(%18, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%32)));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%17), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:         write<i32, volatile>(%17, const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, ...) -> i32>(%18, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%33))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%17), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         write<i32, volatile>(%17, const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, ...) -> i32>(%18, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%34)));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%17), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:         write<i32, volatile>(%17, const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, ...) -> i32>(%18, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%35))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%17), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         write<i32, volatile>(%17, const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, ...) -> i32>(%18, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%36)));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%17), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:         write<i32, volatile>(%17, const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, ...) -> i32>(%18, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%37))), const<i32>(5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%17), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         write<i32, volatile>(%17, const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, ...) -> i32>(%18, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%38)));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%17), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:         write<i32, volatile>(%17, const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, ...) -> i32>(%18, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%39))), const<i32>(6))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%17), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         write<i32, volatile>(%17, const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, ...) -> i32>(%18, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%40)));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%17), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:         write<i32, volatile>(%17, const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, ...) -> i32>(%18, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%41))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%17), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         write<i32, volatile>(%17, const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, ...) -> i32>(%18, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%42)));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%17), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:         write<i32, volatile>(%17, const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, ...) -> i32>(%18, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%43))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%17), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         write<i32, volatile>(%17, const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, ...) -> i32>(%18, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%44)));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%17), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:         write<i32, volatile>(%17, const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, ...) -> i32>(%18, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%45))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%17), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         write<i32, volatile>(%17, const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, ...) -> i32>(%18, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%46)));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%17), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:         write<i32, volatile>(%17, const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, ...) -> i32>(%18, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%47))), const<i32>(7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%17), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         write<i32, volatile>(%17, const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, ...) -> i32>(%18, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%48)));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%17), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:         write<i32, volatile>(%17, const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type5>, i32, ptr<const i8>, ...) -> i32>(%18, read<ptr<@type5>>(%11), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%49))), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%17), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
