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
// DEFAULT-NEXT:     type @type5 _IO_FILE = struct incomplete;
// DEFAULT-NEXT:     type @type6 FILE = @type5;
// DEFAULT-NEXT:     type @type7 _IO_lock_t = void;
// DEFAULT-NEXT:     type @type8 _IO_marker = struct incomplete;
// DEFAULT-NEXT:     type @type9 _IO_codecvt = struct incomplete;
// DEFAULT-NEXT:     type @type10 _IO_wide_data = struct incomplete;
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
// DEFAULT-NEXT:     global %52 .str52: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([119, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %53 .str53: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([37, 115, 58, 37, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %54 .str54: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([99, 111, 111, 107, 105, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %55 .str55: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([99, 111, 111, 107, 105, 101, 58, 55, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %56 .str56: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %18 @fclose(%39 __stream: ptr<@type5>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %19 @fopencookie(%40 __magic_cookie: ptr<void> [restrict], %41 __modes: ptr<const i8> [restrict], %42 __io_funcs: @type15) -> ptr<@type5> [linkage=external] [abi=sysv64(scalar, scalar, byval<align=8>) -> scalar];
// DEFAULT-NEXT:     fn %20 @fprintf(%43 __stream: ptr<@type5> [restrict], %44 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %21 @printf(%45 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %22 @memcpy(%46 __dest: ptr<void> [restrict], %47 __src: ptr<const void> [restrict], %48 __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %23 @memcmp(%49 __s1: ptr<const void>, %50 __s2: ptr<const void>, %51 __n: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %25 @gnu_cookie_write(%26 state: ptr<void>, %27 buffer: ptr<const i8>, %28 size: u64) -> i64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %29 cookie: ptr<@type18> [storage=automatic] = pointer_cast<ptr<@type18>, reason=assign>(read<ptr<void>>(%26));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%22, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(32)>(field0(deref(read<ptr<@type18>>(%29)))), read<u64>(field1(deref(read<ptr<@type18>>(%29)))))), pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i8>>(%27)), read<u64>(%28));
// DEFAULT-NEXT:         let %57: ptr<@type18> [synthetic] = read<ptr<@type18>>(%29);
// DEFAULT-NEXT:         let %58: u64 [synthetic] = read<u64>(field1(deref(read<ptr<@type18>>(%57))));
// DEFAULT-NEXT:         let %59: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%58), read<u64>(%28));
// DEFAULT-NEXT:         write<u64>(field1(deref(read<ptr<@type18>>(%57))), read<u64>(%59));
// DEFAULT-NEXT:         return reinterpret<i64, reason=explicit, fits=unknown>(read<u64>(%28));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %30 @gnu_cookie_close(%31 state: ptr<void>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %32 cookie: ptr<@type18> [storage=automatic] = pointer_cast<ptr<@type18>, reason=assign>(read<ptr<void>>(%31));
// DEFAULT-NEXT:         write<i32>(field2(deref(read<ptr<@type18>>(%32))), const<i32>(1));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %33 @gnu_cookie_stdio() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %34 cookie: @type18 [storage=automatic] = aggregate<@type18, zero_fill=true>();
// DEFAULT-NEXT:         let %35 functions: @type15 [storage=automatic] = aggregate<@type15, zero_fill=true>();
// DEFAULT-NEXT:         let %36 stream: ptr<@type5> [storage=automatic];
// DEFAULT-NEXT:         let %37 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         write<ptr<fn(ptr<void>, ptr<const i8>, u64) -> i64>>(field1(%35), function_decay<ptr<fn(ptr<void>, ptr<const i8>, u64) -> i64>>(%25));
// DEFAULT-NEXT:         write<ptr<fn(ptr<void>) -> i32>>(field3(%35), function_decay<ptr<fn(ptr<void>) -> i32>>(%30));
// DEFAULT-NEXT:         write<ptr<@type5>>(%36, call<ptr<@type5>, signature=fn(ptr<void>, ptr<const i8>, @type15) -> ptr<@type5>, abi=sysv64(scalar, scalar, byval<align=8>) -> scalar>(%19, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type18>>(%34)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%52)), copy<@type15, reason=arg>(read<@type15>(%35))));
// DEFAULT-NEXT:         call<ptr<@type5>, signature=fn(ptr<void>, ptr<const i8>, @type15) -> ptr<@type5>, abi=sysv64(scalar, scalar, byval<align=8>) -> scalar>(%19, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type18>>(%34)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%52)), copy<@type15, reason=arg>(read<@type15>(%35)));
// DEFAULT-NEXT:         let %60: i32 [synthetic] = read<i32>(%37);
// DEFAULT-NEXT:         let %61: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%60), from_bool<i32, reason=promotion>(ne<ptr<@type5>>(read<ptr<@type5>>(%36), null<ptr<@type5>>)));
// DEFAULT-NEXT:         write<i32>(%37, read<i32>(%61));
// DEFAULT-NEXT:         let %62: i32 [synthetic] = read<i32>(%37);
// DEFAULT-NEXT:         let %63: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%62), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<@type5>, ptr<const i8>, ...) -> i32>(%20, read<ptr<@type5>>(%36), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%53)), array_decay<ptr<i8>, length=Some(7)>(%54), const<i32>(7)), const<i32>(8))));
// DEFAULT-NEXT:         write<i32>(%37, read<i32>(%63));
// DEFAULT-NEXT:         let %64: i32 [synthetic] = read<i32>(%37);
// DEFAULT-NEXT:         let %65: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%64), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<@type5>) -> i32>(%18, read<ptr<@type5>>(%36)), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%37, read<i32>(%65));
// DEFAULT-NEXT:         let %66: i32 [synthetic] = read<i32>(%37);
// DEFAULT-NEXT:         let %67: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%66), from_bool<i32, reason=promotion>(eq<i32>(read<i32>(field2(%34)), const<i32>(1))));
// DEFAULT-NEXT:         write<i32>(%37, read<i32>(%67));
// DEFAULT-NEXT:         let %68: i32 [synthetic] = read<i32>(%37);
// DEFAULT-NEXT:         let %69: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%68), from_bool<i32, reason=promotion>(eq<u64>(read<u64>(field1(%34)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))));
// DEFAULT-NEXT:         write<i32>(%37, read<i32>(%69));
// DEFAULT-NEXT:         let %70: i32 [synthetic] = read<i32>(%37);
// DEFAULT-NEXT:         let %71: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%70), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%23, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(32)>(field0(%34))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%55)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8)))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%37, read<i32>(%71));
// DEFAULT-NEXT:         return read<i32>(%37);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %38 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%21, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%56)), call<i32, signature=fn() -> i32>(%33));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
