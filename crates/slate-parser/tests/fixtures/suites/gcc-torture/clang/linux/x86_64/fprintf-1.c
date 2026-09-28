/* { dg-skip-if "requires io" { freestanding } }  */

#include <stdio.h>
#include <stdlib.h>

int main(void) {
#define test(ret, args...)                                                     \
  fprintf(stdout, args);                                                       \
  if (fprintf(stdout, args) != ret)                                            \
    abort();
  test(5, "hello");
  test(6, "hello\n");
  test(1, "a");
  test(0, "");
  test(5, "%s", "hello");
  test(6, "%s", "hello\n");
  test(1, "%s", "a");
  test(0, "%s", "");
  test(1, "%c", 'x');
  test(7, "%s\n", "hello\n");
  test(2, "%d\n", 0);
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
// DEFAULT-NEXT:     type @type0 __uint64_t = u64;
// DEFAULT-NEXT:     type @type1 __off_t = i64;
// DEFAULT-NEXT:     type @type2 __off64_t = i64;
// DEFAULT-NEXT:     type @type3 _IO_FILE = struct {
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
// DEFAULT-NEXT:         field12 _markers: ptr<@type6>;
// DEFAULT-NEXT:         field13 _chain: ptr<@type3>;
// DEFAULT-NEXT:         field14 _fileno: i32;
// DEFAULT-NEXT:         field15 _flags2: i32 : 24;
// DEFAULT-NEXT:         field16 _short_backupbuf: array<i8, 1>;
// DEFAULT-NEXT:         field17 _old_offset: i64;
// DEFAULT-NEXT:         field18 _cur_column: u16;
// DEFAULT-NEXT:         field19 _vtable_offset: i8;
// DEFAULT-NEXT:         field20 _shortbuf: array<i8, 1>;
// DEFAULT-NEXT:         field21 _lock: ptr<void>;
// DEFAULT-NEXT:         field22 _offset: i64;
// DEFAULT-NEXT:         field23 _codecvt: ptr<@type7>;
// DEFAULT-NEXT:         field24 _wide_data: ptr<@type8>;
// DEFAULT-NEXT:         field25 _freeres_list: ptr<@type3>;
// DEFAULT-NEXT:         field26 _freeres_buf: ptr<void>;
// DEFAULT-NEXT:         field27 _prevchain: ptr<ptr<@type3>>;
// DEFAULT-NEXT:         field28 _mode: i32;
// DEFAULT-NEXT:         field29 _unused3: i32;
// DEFAULT-NEXT:         field30 _total_written: u64;
// DEFAULT-NEXT:         field31 _unused2: array<i8, 8>;
// DEFAULT-NEXT:     } [size=216, align=8, offsets=[0, 8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 96, 104, 112, 116, 119, 120, 128, 130, 131, 136, 144, 152, 160, 168, 176, 184, 192, 196, 200, 208], bit_offsets=[None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, Some(928), None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None], bit_units=[(116, 3)], field_units=[None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, Some(0), None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None]];
// DEFAULT-NEXT:     type @type4 FILE = @type3;
// DEFAULT-NEXT:     type @type5 _IO_lock_t = void;
// DEFAULT-NEXT:     type @type6 _IO_marker = struct incomplete;
// DEFAULT-NEXT:     type @type7 _IO_codecvt = struct incomplete;
// DEFAULT-NEXT:     type @type8 _IO_wide_data = struct incomplete;
// DEFAULT-NEXT:     extern %9 stdout: ptr<@type3> [storage=static] [linkage=external];
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
// DEFAULT-NEXT:     fn %12 @fprintf(%15 __stream: ptr<@type3> [restrict], %16 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %13 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %14 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type3>, ptr<const i8>, ...) -> i32>(%12, read<ptr<@type3>>(%9), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%17)));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type3>, ptr<const i8>, ...) -> i32>(%12, read<ptr<@type3>>(%9), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%18))), const<i32>(5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%13);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type3>, ptr<const i8>, ...) -> i32>(%12, read<ptr<@type3>>(%9), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%19)));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type3>, ptr<const i8>, ...) -> i32>(%12, read<ptr<@type3>>(%9), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%20))), const<i32>(6))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%13);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type3>, ptr<const i8>, ...) -> i32>(%12, read<ptr<@type3>>(%9), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%21)));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type3>, ptr<const i8>, ...) -> i32>(%12, read<ptr<@type3>>(%9), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%22))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%13);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type3>, ptr<const i8>, ...) -> i32>(%12, read<ptr<@type3>>(%9), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%23)));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type3>, ptr<const i8>, ...) -> i32>(%12, read<ptr<@type3>>(%9), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%24))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%13);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type3>, ptr<const i8>, ...) -> i32>(%12, read<ptr<@type3>>(%9), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%25)));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type3>, ptr<const i8>, ...) -> i32>(%12, read<ptr<@type3>>(%9), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%26))), const<i32>(5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%13);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type3>, ptr<const i8>, ...) -> i32>(%12, read<ptr<@type3>>(%9), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%27)));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type3>, ptr<const i8>, ...) -> i32>(%12, read<ptr<@type3>>(%9), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%28))), const<i32>(6))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%13);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type3>, ptr<const i8>, ...) -> i32>(%12, read<ptr<@type3>>(%9), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%29)));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type3>, ptr<const i8>, ...) -> i32>(%12, read<ptr<@type3>>(%9), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%30))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%13);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type3>, ptr<const i8>, ...) -> i32>(%12, read<ptr<@type3>>(%9), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%31)));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type3>, ptr<const i8>, ...) -> i32>(%12, read<ptr<@type3>>(%9), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%32))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%13);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type3>, ptr<const i8>, ...) -> i32>(%12, read<ptr<@type3>>(%9), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%33)));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type3>, ptr<const i8>, ...) -> i32>(%12, read<ptr<@type3>>(%9), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%34))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%13);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type3>, ptr<const i8>, ...) -> i32>(%12, read<ptr<@type3>>(%9), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%35)));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type3>, ptr<const i8>, ...) -> i32>(%12, read<ptr<@type3>>(%9), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%36))), const<i32>(7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%13);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type3>, ptr<const i8>, ...) -> i32>(%12, read<ptr<@type3>>(%9), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%37)));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type3>, ptr<const i8>, ...) -> i32>(%12, read<ptr<@type3>>(%9), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%38))), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%13);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
