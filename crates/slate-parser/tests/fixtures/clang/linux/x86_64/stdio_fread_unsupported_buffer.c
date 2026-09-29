#include <stdio.h>
#include <stdlib.h>

int main(void) {
  remove("slate_stdio_fread_unsupported_buffer.tmp");
  FILE *f = fopen("slate_stdio_fread_unsupported_buffer.tmp", "w");
  if (!f) {
    puts("open-fail");
    return 0;
  }
  fputs("heap-owned", f);
  fclose(f);

  FILE *g = fopen("slate_stdio_fread_unsupported_buffer.tmp", "r");
  if (!g) {
    puts("reopen-fail");
    return 0;
  }
  char *buf = malloc(16);
  buf[0]    = 0;
  size_t n  = fread(buf, 1, 10, g);
  buf[n]    = 0;
  printf("%zu %s\n", n, buf);
  free(buf);
  fclose(g);
  remove("slate_stdio_fread_unsupported_buffer.tmp");
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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
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
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 41> [storage=static] = code_units<array<i8, 41>>([115, 108, 97, 116, 101, 95, 115, 116, 100, 105, 111, 95, 102, 114, 101, 97, 100, 95, 117, 110, 115, 117, 112, 112, 111, 114, 116, 101, 100, 95, 98, 117, 102, 102, 101, 114, 46, 116, 109, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 41> [storage=static] = code_units<array<i8, 41>>([115, 108, 97, 116, 101, 95, 115, 116, 100, 105, 111, 95, 102, 114, 101, 97, 100, 95, 117, 110, 115, 117, 112, 112, 111, 114, 116, 101, 100, 95, 98, 117, 102, 102, 101, 114, 46, 116, 109, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([119, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([111, 112, 101, 110, 45, 102, 97, 105, 108, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([104, 101, 97, 112, 45, 111, 119, 110, 101, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_6:[0-9]+]] .str[[VALUE_str_6]]: array<i8, 41> [storage=static] = code_units<array<i8, 41>>([115, 108, 97, 116, 101, 95, 115, 116, 100, 105, 111, 95, 102, 114, 101, 97, 100, 95, 117, 110, 115, 117, 112, 112, 111, 114, 116, 101, 100, 95, 98, 117, 102, 102, 101, 114, 46, 116, 109, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_7:[0-9]+]] .str[[VALUE_str_7]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_8:[0-9]+]] .str[[VALUE_str_8]]: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([114, 101, 111, 112, 101, 110, 45, 102, 97, 105, 108, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_9:[0-9]+]] .str[[VALUE_str_9]]: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([37, 122, 117, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_10:[0-9]+]] .str[[VALUE_str_10]]: array<i8, 41> [storage=static] = code_units<array<i8, 41>>([115, 108, 97, 116, 101, 95, 115, 116, 100, 105, 111, 95, 102, 114, 101, 97, 100, 95, 117, 110, 115, 117, 112, 112, 111, 114, 116, 101, 100, 95, 98, 117, 102, 102, 101, 114, 46, 116, 109, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_remove:[0-9]+]] @remove(%[[VALUE___filename:[0-9]+]] __filename: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fclose:[0-9]+]] @fclose(%[[VALUE___stream:[0-9]+]] __stream: ptr<@type[[TYPE__IO_FILE]]>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fopen:[0-9]+]] @fopen(%[[VALUE___filename_2:[0-9]+]] __filename: ptr<const i8> [restrict], %[[VALUE___modes:[0-9]+]] __modes: ptr<const i8> [restrict]) -> ptr<@type[[TYPE__IO_FILE]]> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fputs:[0-9]+]] @fputs(%[[VALUE___s:[0-9]+]] __s: ptr<const i8> [restrict], %[[VALUE___stream_2:[0-9]+]] __stream: ptr<@type[[TYPE__IO_FILE]]> [restrict]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_puts:[0-9]+]] @puts(%[[VALUE___s_2:[0-9]+]] __s: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fread:[0-9]+]] @fread(%[[VALUE___ptr:[0-9]+]] __ptr: ptr<void> [restrict], %[[VALUE___size:[0-9]+]] __size: u64, %[[VALUE___n:[0-9]+]] __n: u64, %[[VALUE___stream_3:[0-9]+]] __stream: ptr<@type[[TYPE__IO_FILE]]> [restrict]) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_malloc:[0-9]+]] @malloc(%[[VALUE___size_2:[0-9]+]] __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_free:[0-9]+]] @free(%[[VALUE___ptr_2:[0-9]+]] __ptr: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>) -> i32>(%[[VALUE_remove]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(41)>(%[[VALUE_str]])));
// DEFAULT-NEXT:         let %[[VALUE_f:[0-9]+]] f: ptr<@type[[TYPE__IO_FILE]]> [storage=automatic] = call<ptr<@type[[TYPE__IO_FILE]]>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_fopen]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(41)>(%[[VALUE_str_2]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_3]])));
// DEFAULT-NEXT:         if not<bool>(ne<ptr<@type[[TYPE__IO_FILE]]>>(read<ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_f]]), null<ptr<@type[[TYPE__IO_FILE]]>>))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>) -> i32>(%[[VALUE_puts]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_4]])));
// DEFAULT-NEXT:                 return const<i32>(0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ptr<@type[[TYPE__IO_FILE]]>) -> i32>(%[[VALUE_fputs]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_5]])), read<ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type[[TYPE__IO_FILE]]>) -> i32>(%[[VALUE_fclose]], read<ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_f]]));
// DEFAULT-NEXT:         let %[[VALUE_g:[0-9]+]] g: ptr<@type[[TYPE__IO_FILE]]> [storage=automatic] = call<ptr<@type[[TYPE__IO_FILE]]>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_fopen]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(41)>(%[[VALUE_str_6]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_7]])));
// DEFAULT-NEXT:         if not<bool>(ne<ptr<@type[[TYPE__IO_FILE]]>>(read<ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_g]]), null<ptr<@type[[TYPE__IO_FILE]]>>))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>) -> i32>(%[[VALUE_puts]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(12)>(%[[VALUE_str_8]])));
// DEFAULT-NEXT:                 return const<i32>(0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         let %[[VALUE_buf:[0-9]+]] buf: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_malloc]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16)))));
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_buf]]), const<i32>(0))), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE_n:[0-9]+]] n: u64 [storage=automatic] = call<u64, signature=fn(ptr<void>, u64, u64, ptr<@type[[TYPE__IO_FILE]]>) -> u64>(%[[VALUE_fread]], pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%[[VALUE_buf]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(10))), read<ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_g]]));
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_buf]]), read<u64>(%[[VALUE_n]]))), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_str_9]])), read<u64>(%[[VALUE_n]]), read<ptr<i8>>(%[[VALUE_buf]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%[[VALUE_buf]])));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type[[TYPE__IO_FILE]]>) -> i32>(%[[VALUE_fclose]], read<ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_g]]));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>) -> i32>(%[[VALUE_remove]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(41)>(%[[VALUE_str_10]])));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
