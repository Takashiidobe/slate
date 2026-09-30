#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>

struct GNUCookie {
  char   bytes[32];
  size_t length;
  int    closed;
};

static ssize_t gnu_cookie_write(void *state, const char *buffer, size_t size) {
  struct GNUCookie *cookie = state;
  memcpy(cookie->bytes + cookie->length, buffer, size);
  cookie->length += size;
  return (ssize_t)size;
}

static int gnu_cookie_close(void *state) {
  struct GNUCookie *cookie = state;
  cookie->closed           = 1;
  return 0;
}

static int gnu_cookie_stdio(void) {
  struct GNUCookie      cookie    = {};
  cookie_io_functions_t functions = {};
  FILE                 *stream;
  int                   total = 0;

  functions.write  = gnu_cookie_write;
  functions.close  = gnu_cookie_close;
  stream           = fopencookie(&cookie, "w", functions);
  total           += stream != NULL;
  total           += fprintf(stream, "%s:%d", "cookie", 7) == 8;
  total           += fclose(stream) == 0;
  total           += cookie.closed == 1;
  total           += cookie.length == 8;
  total           += memcmp(cookie.bytes, "cookie:7", 8) == 0;
  return total;
}

int main(void) {
  printf("%d\n", gnu_cookie_stdio());
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
// DEFAULT-NEXT:     type @type[[TYPE___ssize_t:[0-9]+]] __ssize_t = i64;
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
// DEFAULT-NEXT:     type @type[[TYPE_cookie_read_function_t:[0-9]+]] cookie_read_function_t = fn(ptr<void>, ptr<i8>, u64) -> i64;
// DEFAULT-NEXT:     type @type[[TYPE_cookie_write_function_t:[0-9]+]] cookie_write_function_t = fn(ptr<void>, ptr<const i8>, u64) -> i64;
// DEFAULT-NEXT:     type @type[[TYPE_cookie_seek_function_t:[0-9]+]] cookie_seek_function_t = fn(ptr<void>, ptr<i64>, i32) -> i32;
// DEFAULT-NEXT:     type @type[[TYPE_cookie_close_function_t:[0-9]+]] cookie_close_function_t = fn(ptr<void>) -> i32;
// DEFAULT-NEXT:     type @type[[TYPE__IO_cookie_io_functions_t:[0-9]+]] _IO_cookie_io_functions_t = struct {
// DEFAULT-NEXT:         field0 read: ptr<fn(ptr<void>, ptr<i8>, u64) -> i64>;
// DEFAULT-NEXT:         field1 write: ptr<fn(ptr<void>, ptr<const i8>, u64) -> i64>;
// DEFAULT-NEXT:         field2 seek: ptr<fn(ptr<void>, ptr<i64>, i32) -> i32>;
// DEFAULT-NEXT:         field3 close: ptr<fn(ptr<void>) -> i32>;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 16, 24]];
// DEFAULT-NEXT:     type @type[[TYPE_cookie_io_functions_t:[0-9]+]] cookie_io_functions_t = @type[[TYPE__IO_cookie_io_functions_t]];
// DEFAULT-NEXT:     type @type[[TYPE_ssize_t:[0-9]+]] ssize_t = i64;
// DEFAULT-NEXT:     type @type[[TYPE_GNUCookie:[0-9]+]] GNUCookie = struct {
// DEFAULT-NEXT:         field0 bytes: array<i8, 32>;
// DEFAULT-NEXT:         field1 length: u64;
// DEFAULT-NEXT:         field2 closed: i32;
// DEFAULT-NEXT:     } [size=48, align=8, offsets=[0, 32, 40]];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([119, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([37, 115, 58, 37, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([99, 111, 111, 107, 105, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([99, 111, 111, 107, 105, 101, 58, 55, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_fclose:[0-9]+]] @fclose(%[[VALUE___stream:[0-9]+]] __stream: ptr<@type[[TYPE__IO_FILE]]>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fopencookie:[0-9]+]] @fopencookie(%[[VALUE___magic_cookie:[0-9]+]] __magic_cookie: ptr<void> [restrict], %[[VALUE___modes:[0-9]+]] __modes: ptr<const i8> [restrict], %[[VALUE___io_funcs:[0-9]+]] __io_funcs: @type[[TYPE__IO_cookie_io_functions_t]]) -> ptr<@type[[TYPE__IO_FILE]]> [linkage=external] [abi=sysv64(scalar, scalar, native_c) -> scalar];
// DEFAULT-NEXT:     fn %[[VALUE_fprintf:[0-9]+]] @fprintf(%[[VALUE___stream_2:[0-9]+]] __stream: ptr<@type[[TYPE__IO_FILE]]> [restrict], %[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format_2:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_memcpy:[0-9]+]] @memcpy(%[[VALUE___dest:[0-9]+]] __dest: ptr<void> [restrict], %[[VALUE___src:[0-9]+]] __src: ptr<const void> [restrict], %[[VALUE___n:[0-9]+]] __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_memcmp:[0-9]+]] @memcmp(%[[VALUE___s1:[0-9]+]] __s1: ptr<const void>, %[[VALUE___s2:[0-9]+]] __s2: ptr<const void>, %[[VALUE___n_2:[0-9]+]] __n: u64) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_gnu_cookie_write:[0-9]+]] @gnu_cookie_write(%[[VALUE_state:[0-9]+]] state: ptr<void>, %[[VALUE_buffer:[0-9]+]] buffer: ptr<const i8>, %[[VALUE_size:[0-9]+]] size: u64) -> i64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_cookie:[0-9]+]] cookie: ptr<@type[[TYPE_GNUCookie]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE_GNUCookie]]>, reason=assign>(read<ptr<void>>(%[[VALUE_state]]));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(32)>(field0(deref(read<ptr<@type[[TYPE_GNUCookie]]>>(%[[VALUE_cookie]])))), read<u64>(field1(deref(read<ptr<@type[[TYPE_GNUCookie]]>>(%[[VALUE_cookie]])))))), pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i8>>(%[[VALUE_buffer]])), read<u64>(%[[VALUE_size]]));
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: ptr<@type[[TYPE_GNUCookie]]> [synthetic] = read<ptr<@type[[TYPE_GNUCookie]]>>(%[[VALUE_cookie]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: u64 [synthetic] = read<u64>(field1(deref(read<ptr<@type[[TYPE_GNUCookie]]>>(%[[VALUE0]]))));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE1]]), read<u64>(%[[VALUE_size]]));
// DEFAULT-NEXT:         write<u64>(field1(deref(read<ptr<@type[[TYPE_GNUCookie]]>>(%[[VALUE0]]))), read<u64>(%[[VALUE2]]));
// DEFAULT-NEXT:         return reinterpret<i64, reason=explicit, fits=unknown>(read<u64>(%[[VALUE_size]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gnu_cookie_close:[0-9]+]] @gnu_cookie_close(%[[VALUE_state_2:[0-9]+]] state: ptr<void>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_cookie_2:[0-9]+]] cookie: ptr<@type[[TYPE_GNUCookie]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE_GNUCookie]]>, reason=assign>(read<ptr<void>>(%[[VALUE_state_2]]));
// DEFAULT-NEXT:         write<i32>(field2(deref(read<ptr<@type[[TYPE_GNUCookie]]>>(%[[VALUE_cookie_2]]))), const<i32>(1));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gnu_cookie_stdio:[0-9]+]] @gnu_cookie_stdio() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_cookie_3:[0-9]+]] cookie: @type[[TYPE_GNUCookie]] [storage=automatic] = aggregate<@type[[TYPE_GNUCookie]], zero_fill=true>();
// DEFAULT-NEXT:         let %[[VALUE_functions:[0-9]+]] functions: @type[[TYPE__IO_cookie_io_functions_t]] [storage=automatic] = aggregate<@type[[TYPE__IO_cookie_io_functions_t]], zero_fill=true>();
// DEFAULT-NEXT:         let %[[VALUE_stream:[0-9]+]] stream: ptr<@type[[TYPE__IO_FILE]]> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_total:[0-9]+]] total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         write<ptr<fn(ptr<void>, ptr<const i8>, u64) -> i64>>(field1(%[[VALUE_functions]]), function_decay<ptr<fn(ptr<void>, ptr<const i8>, u64) -> i64>>(%[[VALUE_gnu_cookie_write]]));
// DEFAULT-NEXT:         write<ptr<fn(ptr<void>) -> i32>>(field3(%[[VALUE_functions]]), function_decay<ptr<fn(ptr<void>) -> i32>>(%[[VALUE_gnu_cookie_close]]));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_stream]], call<ptr<@type[[TYPE__IO_FILE]]>, signature=fn(ptr<void>, ptr<const i8>, @type[[TYPE__IO_cookie_io_functions_t]]) -> ptr<@type[[TYPE__IO_FILE]]>, abi=sysv64(scalar, scalar, native_c) -> scalar>(%[[VALUE_fopencookie]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type[[TYPE_GNUCookie]]>>(%[[VALUE_cookie_3]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str]])), copy<@type[[TYPE__IO_cookie_io_functions_t]], reason=arg>(read<@type[[TYPE__IO_cookie_io_functions_t]]>(%[[VALUE_functions]]))));
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE3]]), from_bool<i32, reason=promotion>(ne<ptr<@type[[TYPE__IO_FILE]]>>(read<ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_stream]]), null<ptr<@type[[TYPE__IO_FILE]]>>)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE5]]), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<@type[[TYPE__IO_FILE]]>, ptr<const i8>, ...) -> i32>(%[[VALUE_fprintf]], read<ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_stream]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_2]])), array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str_3]]), const<i32>(7)), const<i32>(8))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE6]]));
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE7]]), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<@type[[TYPE__IO_FILE]]>) -> i32>(%[[VALUE_fclose]], read<ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_stream]])), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE8]]));
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE9]]), from_bool<i32, reason=promotion>(eq<i32>(read<i32>(field2(%[[VALUE_cookie_3]])), const<i32>(1))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE10]]));
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE11]]), from_bool<i32, reason=promotion>(eq<u64>(read<u64>(field1(%[[VALUE_cookie_3]])), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE12]]));
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE14:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE13]]), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(32)>(field0(%[[VALUE_cookie_3]]))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%[[VALUE_str_4]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8)))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE14]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_5]])), call<i32, signature=fn() -> i32>(%[[VALUE_gnu_cookie_stdio]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
