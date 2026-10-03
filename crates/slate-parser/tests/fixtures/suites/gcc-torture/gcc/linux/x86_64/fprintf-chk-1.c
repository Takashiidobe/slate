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
// DEFAULT-NEXT:     type @type[[TYPE___gnuc_va_list:[0-9]+]] __gnuc_va_list = va_list;
// DEFAULT-NEXT:     type @type[[TYPE_va_list:[0-9]+]] va_list = va_list;
// DEFAULT-NEXT:     type @type[[TYPE___uint64_t:[0-9]+]] __uint64_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE___off_t:[0-9]+]] __off_t = i64;
// DEFAULT-NEXT:     type @type[[TYPE___off64_t:[0-9]+]] __off64_t = i64;
// DEFAULT-NEXT:     type @type[[TYPE__IO_FILE:[0-9]+]] _IO_FILE = struct {
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
// DEFAULT-NEXT:         field12 _markers: ptr<@type[[TYPE__IO_marker:[0-9]+]]>;
// DEFAULT-NEXT:         field13 _chain: ptr<@type[[TYPE__IO_FILE]]>;
// DEFAULT-NEXT:         field14 _fileno: i32;
// DEFAULT-NEXT:         field15 _flags2: i32 : 24;
// DEFAULT-NEXT:         field16 _short_backupbuf: array<i8, 1>;
// DEFAULT-NEXT:         field17 _old_offset: i64;
// DEFAULT-NEXT:         field18 _cur_column: u16;
// DEFAULT-NEXT:         field19 _vtable_offset: i8;
// DEFAULT-NEXT:         field20 _shortbuf: array<i8, 1>;
// DEFAULT-NEXT:         field21 _lock: ptr<void>;
// DEFAULT-NEXT:         field22 _offset: i64;
// DEFAULT-NEXT:         field23 _codecvt: ptr<@type[[TYPE__IO_codecvt:[0-9]+]]>;
// DEFAULT-NEXT:         field24 _wide_data: ptr<@type[[TYPE__IO_wide_data:[0-9]+]]>;
// DEFAULT-NEXT:         field25 _freeres_list: ptr<@type[[TYPE__IO_FILE]]>;
// DEFAULT-NEXT:         field26 _freeres_buf: ptr<void>;
// DEFAULT-NEXT:         field27 _prevchain: ptr<ptr<@type[[TYPE__IO_FILE]]>>;
// DEFAULT-NEXT:         field28 _mode: i32;
// DEFAULT-NEXT:         field29 _unused3: i32;
// DEFAULT-NEXT:         field30 _total_written: u64;
// DEFAULT-NEXT:         field31 _unused2: array<i8, 8>;
// DEFAULT-NEXT:     } [size=216, align=8, offsets=[0, 8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 96, 104, 112, 116, 119, 120, 128, 130, 131, 136, 144, 152, 160, 168, 176, 184, 192, 196, 200, 208], bit_offsets=[None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, Some(928), None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None], bit_units=[(116, 3)], field_units=[None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, Some(0), None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None]];
// DEFAULT-NEXT:     type @type[[TYPE_FILE:[0-9]+]] FILE = @type[[TYPE__IO_FILE]];
// DEFAULT-NEXT:     type @type[[TYPE__IO_marker]] _IO_marker = struct incomplete;
// DEFAULT-NEXT:     type @type[[TYPE__IO_codecvt]] _IO_codecvt = struct incomplete;
// DEFAULT-NEXT:     type @type[[TYPE__IO_wide_data]] _IO_wide_data = struct incomplete;
// DEFAULT-NEXT:     type @type[[TYPE__IO_lock_t:[0-9]+]] _IO_lock_t = void;
// DEFAULT-NEXT:     extern %[[VALUE_stdout:[0-9]+]] stdout: ptr<@type[[TYPE__IO_FILE]]> [storage=static] [linkage=external];
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
// DEFAULT-NEXT:     global %[[VALUE_str_10:[0-9]+]] .str[[VALUE_str_10]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([104, 101, 108, 108, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_11:[0-9]+]] .str[[VALUE_str_11]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_12:[0-9]+]] .str[[VALUE_str_12]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([104, 101, 108, 108, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_13:[0-9]+]] .str[[VALUE_str_13]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_14:[0-9]+]] .str[[VALUE_str_14]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([104, 101, 108, 108, 111, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_15:[0-9]+]] .str[[VALUE_str_15]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_16:[0-9]+]] .str[[VALUE_str_16]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([104, 101, 108, 108, 111, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_17:[0-9]+]] .str[[VALUE_str_17]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_18:[0-9]+]] .str[[VALUE_str_18]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([97, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_19:[0-9]+]] .str[[VALUE_str_19]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_20:[0-9]+]] .str[[VALUE_str_20]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([97, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_21:[0-9]+]] .str[[VALUE_str_21]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_22:[0-9]+]] .str[[VALUE_str_22]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_23:[0-9]+]] .str[[VALUE_str_23]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_24:[0-9]+]] .str[[VALUE_str_24]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_25:[0-9]+]] .str[[VALUE_str_25]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_26:[0-9]+]] .str[[VALUE_str_26]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_27:[0-9]+]] .str[[VALUE_str_27]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_28:[0-9]+]] .str[[VALUE_str_28]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([104, 101, 108, 108, 111, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_29:[0-9]+]] .str[[VALUE_str_29]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_30:[0-9]+]] .str[[VALUE_str_30]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([104, 101, 108, 108, 111, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_31:[0-9]+]] .str[[VALUE_str_31]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_32:[0-9]+]] .str[[VALUE_str_32]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_vfprintf:[0-9]+]] @vfprintf(%[[VALUE___s:[0-9]+]] __s: ptr<@type[[TYPE__IO_FILE]]> [restrict], %[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], %[[VALUE___arg:[0-9]+]] __arg: va_list) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE___fprintf_chk:[0-9]+]] @__fprintf_chk(%[[VALUE_f:[0-9]+]] f: ptr<@type[[TYPE__IO_FILE]]>, %[[VALUE_flag:[0-9]+]] flag: i32, %[[VALUE_fmt:[0-9]+]] fmt: ptr<const i8>, ...) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_ap:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ret:[0-9]+]] ret: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32, volatile>(%[[VALUE_should_optimize]], const<i32>(1));
// DEFAULT-NEXT:         va_start(%[[VALUE_ap]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_ret]], call<i32, signature=fn(ptr<@type[[TYPE__IO_FILE]]>, ptr<const i8>, va_list) -> i32>(%[[VALUE_vfprintf]], read<ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_f]]), read<ptr<const i8>>(%[[VALUE_fmt]]), read<va_list>(%[[VALUE_ap]])));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap]]);
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_ret]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<i32, volatile>(%[[VALUE_should_optimize]], const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type[[TYPE__IO_FILE]]>, i32, ptr<const i8>, ...) -> i32>(%[[VALUE___fprintf_chk]], read<ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_stdout]]), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str]])));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%[[VALUE_should_optimize]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i32, volatile>(%[[VALUE_should_optimize]], const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type[[TYPE__IO_FILE]]>, i32, ptr<const i8>, ...) -> i32>(%[[VALUE___fprintf_chk]], read<ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_stdout]]), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_2]]))), const<i32>(5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%[[VALUE_should_optimize]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         write<i32, volatile>(%[[VALUE_should_optimize]], const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type[[TYPE__IO_FILE]]>, i32, ptr<const i8>, ...) -> i32>(%[[VALUE___fprintf_chk]], read<ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_stdout]]), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str_3]])));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%[[VALUE_should_optimize]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i32, volatile>(%[[VALUE_should_optimize]], const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type[[TYPE__IO_FILE]]>, i32, ptr<const i8>, ...) -> i32>(%[[VALUE___fprintf_chk]], read<ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_stdout]]), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str_4]]))), const<i32>(6))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%[[VALUE_should_optimize]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         write<i32, volatile>(%[[VALUE_should_optimize]], const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type[[TYPE__IO_FILE]]>, i32, ptr<const i8>, ...) -> i32>(%[[VALUE___fprintf_chk]], read<ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_stdout]]), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_5]])));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%[[VALUE_should_optimize]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i32, volatile>(%[[VALUE_should_optimize]], const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type[[TYPE__IO_FILE]]>, i32, ptr<const i8>, ...) -> i32>(%[[VALUE___fprintf_chk]], read<ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_stdout]]), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_6]]))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%[[VALUE_should_optimize]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         write<i32, volatile>(%[[VALUE_should_optimize]], const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type[[TYPE__IO_FILE]]>, i32, ptr<const i8>, ...) -> i32>(%[[VALUE___fprintf_chk]], read<ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_stdout]]), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_7]])));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%[[VALUE_should_optimize]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i32, volatile>(%[[VALUE_should_optimize]], const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type[[TYPE__IO_FILE]]>, i32, ptr<const i8>, ...) -> i32>(%[[VALUE___fprintf_chk]], read<ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_stdout]]), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_8]]))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%[[VALUE_should_optimize]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         write<i32, volatile>(%[[VALUE_should_optimize]], const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type[[TYPE__IO_FILE]]>, i32, ptr<const i8>, ...) -> i32>(%[[VALUE___fprintf_chk]], read<ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_stdout]]), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str_9]])), array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_10]]));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%[[VALUE_should_optimize]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i32, volatile>(%[[VALUE_should_optimize]], const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type[[TYPE__IO_FILE]]>, i32, ptr<const i8>, ...) -> i32>(%[[VALUE___fprintf_chk]], read<ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_stdout]]), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str_11]])), array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_12]])), const<i32>(5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%[[VALUE_should_optimize]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         write<i32, volatile>(%[[VALUE_should_optimize]], const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type[[TYPE__IO_FILE]]>, i32, ptr<const i8>, ...) -> i32>(%[[VALUE___fprintf_chk]], read<ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_stdout]]), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str_13]])), array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str_14]]));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%[[VALUE_should_optimize]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i32, volatile>(%[[VALUE_should_optimize]], const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type[[TYPE__IO_FILE]]>, i32, ptr<const i8>, ...) -> i32>(%[[VALUE___fprintf_chk]], read<ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_stdout]]), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str_15]])), array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str_16]])), const<i32>(6))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%[[VALUE_should_optimize]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         write<i32, volatile>(%[[VALUE_should_optimize]], const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type[[TYPE__IO_FILE]]>, i32, ptr<const i8>, ...) -> i32>(%[[VALUE___fprintf_chk]], read<ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_stdout]]), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str_17]])), array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_18]]));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%[[VALUE_should_optimize]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i32, volatile>(%[[VALUE_should_optimize]], const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type[[TYPE__IO_FILE]]>, i32, ptr<const i8>, ...) -> i32>(%[[VALUE___fprintf_chk]], read<ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_stdout]]), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str_19]])), array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_20]])), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%[[VALUE_should_optimize]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         write<i32, volatile>(%[[VALUE_should_optimize]], const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type[[TYPE__IO_FILE]]>, i32, ptr<const i8>, ...) -> i32>(%[[VALUE___fprintf_chk]], read<ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_stdout]]), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str_21]])), array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_22]]));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%[[VALUE_should_optimize]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i32, volatile>(%[[VALUE_should_optimize]], const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type[[TYPE__IO_FILE]]>, i32, ptr<const i8>, ...) -> i32>(%[[VALUE___fprintf_chk]], read<ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_stdout]]), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str_23]])), array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_24]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%[[VALUE_should_optimize]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         write<i32, volatile>(%[[VALUE_should_optimize]], const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type[[TYPE__IO_FILE]]>, i32, ptr<const i8>, ...) -> i32>(%[[VALUE___fprintf_chk]], read<ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_stdout]]), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str_25]])), const<i32>(120));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%[[VALUE_should_optimize]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i32, volatile>(%[[VALUE_should_optimize]], const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type[[TYPE__IO_FILE]]>, i32, ptr<const i8>, ...) -> i32>(%[[VALUE___fprintf_chk]], read<ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_stdout]]), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str_26]])), const<i32>(120)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%[[VALUE_should_optimize]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         write<i32, volatile>(%[[VALUE_should_optimize]], const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type[[TYPE__IO_FILE]]>, i32, ptr<const i8>, ...) -> i32>(%[[VALUE___fprintf_chk]], read<ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_stdout]]), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_27]])), array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str_28]]));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%[[VALUE_should_optimize]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i32, volatile>(%[[VALUE_should_optimize]], const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type[[TYPE__IO_FILE]]>, i32, ptr<const i8>, ...) -> i32>(%[[VALUE___fprintf_chk]], read<ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_stdout]]), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_29]])), array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str_30]])), const<i32>(7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%[[VALUE_should_optimize]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         write<i32, volatile>(%[[VALUE_should_optimize]], const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type[[TYPE__IO_FILE]]>, i32, ptr<const i8>, ...) -> i32>(%[[VALUE___fprintf_chk]], read<ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_stdout]]), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_31]])), const<i32>(0));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%[[VALUE_should_optimize]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i32, volatile>(%[[VALUE_should_optimize]], const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type[[TYPE__IO_FILE]]>, i32, ptr<const i8>, ...) -> i32>(%[[VALUE___fprintf_chk]], read<ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_stdout]]), const<i32>(1), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_32]])), const<i32>(0)), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32, volatile>(%[[VALUE_should_optimize]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
