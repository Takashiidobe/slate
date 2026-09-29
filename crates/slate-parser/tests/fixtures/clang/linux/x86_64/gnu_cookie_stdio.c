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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     type @type1 __uint64_t = u64;
// DEFAULT-NEXT:     type @type2 __off_t = i64;
// DEFAULT-NEXT:     type @type3 __off64_t = i64;
// DEFAULT-NEXT:     type @type4 __ssize_t = i64;
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
// DEFAULT-NEXT:     type @type11 cookie_read_function_t = fn(ptr<void>, ptr<i8>, u64) -> i64;
// DEFAULT-NEXT:     type @type12 cookie_write_function_t = fn(ptr<void>, ptr<const i8>, u64) -> i64;
// DEFAULT-NEXT:     type @type13 cookie_seek_function_t = fn(ptr<void>, ptr<i64>, i32) -> i32;
// DEFAULT-NEXT:     type @type14 cookie_close_function_t = fn(ptr<void>) -> i32;
// DEFAULT-NEXT:     type @type15 _IO_cookie_io_functions_t = struct {
// DEFAULT-NEXT:         field0 read: ptr<fn(ptr<void>, ptr<i8>, u64) -> i64>;
// DEFAULT-NEXT:         field1 write: ptr<fn(ptr<void>, ptr<const i8>, u64) -> i64>;
// DEFAULT-NEXT:         field2 seek: ptr<fn(ptr<void>, ptr<i64>, i32) -> i32>;
// DEFAULT-NEXT:         field3 close: ptr<fn(ptr<void>) -> i32>;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 16, 24]];
// DEFAULT-NEXT:     type @type16 cookie_io_functions_t = @type15;
// DEFAULT-NEXT:     type @type17 ssize_t = i64;
// DEFAULT-NEXT:     type @type18 GNUCookie = struct {
// DEFAULT-NEXT:         field0 bytes: array<i8, 32>;
// DEFAULT-NEXT:         field1 length: u64;
// DEFAULT-NEXT:         field2 closed: i32;
// DEFAULT-NEXT:     } [size=48, align=8, offsets=[0, 32, 40]];
// DEFAULT-NEXT:     global %75 .str75: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([119, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %76 .str76: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([37, 115, 58, 37, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %77 .str77: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([99, 111, 111, 107, 105, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %78 .str78: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([99, 111, 111, 107, 105, 101, 58, 55, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %79 .str79: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %29 @fclose(%62 __stream: ptr<@type5>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %33 @fopencookie(%63 __magic_cookie: ptr<void> [restrict], %64 __modes: ptr<const i8> [restrict], %65 __io_funcs: @type15) -> ptr<@type5> [linkage=external] [abi=sysv64(scalar, scalar, native_c) -> scalar];
// DEFAULT-NEXT:     fn %36 @fprintf(%66 __stream: ptr<@type5> [restrict], %67 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %38 @printf(%68 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %42 @memcpy(%69 __dest: ptr<void> [restrict], %70 __src: ptr<const void> [restrict], %71 __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %46 @memcmp(%72 __s1: ptr<const void>, %73 __s2: ptr<const void>, %74 __n: u64) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %48 @gnu_cookie_write(%49 state: ptr<void>, %50 buffer: ptr<const i8>, %51 size: u64) -> i64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %52 cookie: ptr<@type18> [storage=automatic] = pointer_cast<ptr<@type18>, reason=assign>(read<ptr<void>>(%49));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%42, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(32)>(field0(deref(read<ptr<@type18>>(%52)))), read<u64>(field1(deref(read<ptr<@type18>>(%52)))))), pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i8>>(%50)), read<u64>(%51));
// DEFAULT-NEXT:         let %80: ptr<@type18> [synthetic] = read<ptr<@type18>>(%52);
// DEFAULT-NEXT:         let %81: u64 [synthetic] = read<u64>(field1(deref(read<ptr<@type18>>(%80))));
// DEFAULT-NEXT:         let %82: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%81), read<u64>(%51));
// DEFAULT-NEXT:         write<u64>(field1(deref(read<ptr<@type18>>(%80))), read<u64>(%82));
// DEFAULT-NEXT:         return reinterpret<i64, reason=explicit, fits=unknown>(read<u64>(%51));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %53 @gnu_cookie_close(%54 state: ptr<void>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %55 cookie: ptr<@type18> [storage=automatic] = pointer_cast<ptr<@type18>, reason=assign>(read<ptr<void>>(%54));
// DEFAULT-NEXT:         write<i32>(field2(deref(read<ptr<@type18>>(%55))), const<i32>(1));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %56 @gnu_cookie_stdio() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %57 cookie: @type18 [storage=automatic] = aggregate<@type18, zero_fill=true>();
// DEFAULT-NEXT:         let %58 functions: @type15 [storage=automatic] = aggregate<@type15, zero_fill=true>();
// DEFAULT-NEXT:         let %59 stream: ptr<@type5> [storage=automatic];
// DEFAULT-NEXT:         let %60 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         write<ptr<fn(ptr<void>, ptr<const i8>, u64) -> i64>>(field1(%58), function_decay<ptr<fn(ptr<void>, ptr<const i8>, u64) -> i64>>(%48));
// DEFAULT-NEXT:         write<ptr<fn(ptr<void>) -> i32>>(field3(%58), function_decay<ptr<fn(ptr<void>) -> i32>>(%53));
// DEFAULT-NEXT:         write<ptr<@type5>>(%59, call<ptr<@type5>, signature=fn(ptr<void>, ptr<const i8>, @type15) -> ptr<@type5>, abi=sysv64(scalar, scalar, native_c) -> scalar>(%33, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type18>>(%57)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%75)), copy<@type15, reason=arg>(read<@type15>(%58))));
// DEFAULT-NEXT:         call<ptr<@type5>, signature=fn(ptr<void>, ptr<const i8>, @type15) -> ptr<@type5>, abi=sysv64(scalar, scalar, native_c) -> scalar>(%33, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type18>>(%57)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%75)), copy<@type15, reason=arg>(read<@type15>(%58)));
// DEFAULT-NEXT:         let %83: i32 [synthetic] = read<i32>(%60);
// DEFAULT-NEXT:         let %84: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%83), from_bool<i32, reason=promotion>(ne<ptr<@type5>>(read<ptr<@type5>>(%59), null<ptr<@type5>>)));
// DEFAULT-NEXT:         write<i32>(%60, read<i32>(%84));
// DEFAULT-NEXT:         let %85: i32 [synthetic] = read<i32>(%60);
// DEFAULT-NEXT:         let %86: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%85), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<@type5>, ptr<const i8>, ...) -> i32>(%36, read<ptr<@type5>>(%59), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%76)), array_decay<ptr<i8>, length=Some(7)>(%77), const<i32>(7)), const<i32>(8))));
// DEFAULT-NEXT:         write<i32>(%60, read<i32>(%86));
// DEFAULT-NEXT:         let %87: i32 [synthetic] = read<i32>(%60);
// DEFAULT-NEXT:         let %88: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%87), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<@type5>) -> i32>(%29, read<ptr<@type5>>(%59)), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%60, read<i32>(%88));
// DEFAULT-NEXT:         let %89: i32 [synthetic] = read<i32>(%60);
// DEFAULT-NEXT:         let %90: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%89), from_bool<i32, reason=promotion>(eq<i32>(read<i32>(field2(%57)), const<i32>(1))));
// DEFAULT-NEXT:         write<i32>(%60, read<i32>(%90));
// DEFAULT-NEXT:         let %91: i32 [synthetic] = read<i32>(%60);
// DEFAULT-NEXT:         let %92: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%91), from_bool<i32, reason=promotion>(eq<u64>(read<u64>(field1(%57)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))));
// DEFAULT-NEXT:         write<i32>(%60, read<i32>(%92));
// DEFAULT-NEXT:         let %93: i32 [synthetic] = read<i32>(%60);
// DEFAULT-NEXT:         let %94: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%93), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%46, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(32)>(field0(%57))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%78)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8)))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%60, read<i32>(%94));
// DEFAULT-NEXT:         return read<i32>(%60);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %61 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%38, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%79)), call<i32, signature=fn() -> i32>(%56));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
