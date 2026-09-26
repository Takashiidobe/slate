#define _GNU_SOURCE
#include <printf.h>
#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
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

static int gnu_allocating_stdio(void) {
  char  *formatted   = NULL;
  char  *stream_data = NULL;
  size_t stream_size = 0;
  FILE  *stream;
  int    total = 0;

  total += asprintf(&formatted, "%s:%d", "gnu", 23) == 6;
  total += strcmp(formatted, "gnu:23") == 0;
  free(formatted);

  stream  = open_memstream(&stream_data, &stream_size);
  total  += stream != NULL;
  total  += fprintf(stream, "%s-%d", "slate", 24) == 8;
  total  += __fwriting(stream) != 0;
  total  += __fpending(stream) > 0;
  total  += fflush(stream) == 0;
  total  += stream_size == 8;
  total  += strcmp(stream_data, "slate-24") == 0;
  total  += fclose(stream) == 0;
  free(stream_data);
  return total;
}

static int gnu_memory_stdio(void) {
  char   source[] = "alpha|beta\n";
  char  *line     = NULL;
  size_t capacity = 0;
  FILE  *stream   = fmemopen(source, strlen(source), "r");
  int    total    = 0;

  total += stream != NULL;
  total += getdelim(&line, &capacity, '|', stream) == 6;
  total += strcmp(line, "alpha|") == 0;
  total += getline(&line, &capacity, stream) == 5;
  total += strcmp(line, "beta\n") == 0;
  total += __freading(stream) != 0;
  total += __fbufsize(stream) > 0;
  total += fclose(stream) == 0;
  free(line);
  return total;
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

static int gnu_printf_introspection(void) {
  int    types[4]  = {};
  size_t arguments = parse_printf_format("%2$d %1$s", 4, types);
  return arguments == 2 && (types[0] & ~PA_FLAG_MASK) == PA_STRING &&
         (types[1] & ~PA_FLAG_MASK) == PA_INT;
}

int main(void) {
  printf("%d %d %d %d\n", gnu_allocating_stdio(), gnu_memory_stdio(),
         gnu_cookie_stdio(), gnu_printf_introspection());
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
// DEFAULT-NEXT:     type @type0 _IO_FILE = struct incomplete;
// DEFAULT-NEXT:     type @type1 FILE = @type0;
// DEFAULT-NEXT:     type @type2 size_t = u64;
// DEFAULT-NEXT:     type @type3 = enum : u32 {
// DEFAULT-NEXT:         %0 PA_INT = const<i32>(0);
// DEFAULT-NEXT:         %1 PA_CHAR = const<i32>(1);
// DEFAULT-NEXT:         %2 PA_WCHAR = const<i32>(2);
// DEFAULT-NEXT:         %3 PA_STRING = const<i32>(3);
// DEFAULT-NEXT:         %4 PA_WSTRING = const<i32>(4);
// DEFAULT-NEXT:         %5 PA_POINTER = const<i32>(5);
// DEFAULT-NEXT:         %6 PA_FLOAT = const<i32>(6);
// DEFAULT-NEXT:         %7 PA_DOUBLE = const<i32>(7);
// DEFAULT-NEXT:         %8 PA_LAST = const<i32>(8);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type4 __uint64_t = u64;
// DEFAULT-NEXT:     type @type5 __off_t = i64;
// DEFAULT-NEXT:     type @type6 __off64_t = i64;
// DEFAULT-NEXT:     type @type7 __ssize_t = i64;
// DEFAULT-NEXT:     type @type8 _IO_lock_t = void;
// DEFAULT-NEXT:     type @type9 _IO_marker = struct incomplete;
// DEFAULT-NEXT:     type @type10 _IO_codecvt = struct incomplete;
// DEFAULT-NEXT:     type @type11 _IO_wide_data = struct incomplete;
// DEFAULT-NEXT:     type @type12 cookie_read_function_t = fn(ptr<void>, ptr<i8>, u64) -> i64;
// DEFAULT-NEXT:     type @type13 cookie_write_function_t = fn(ptr<void>, ptr<const i8>, u64) -> i64;
// DEFAULT-NEXT:     type @type14 cookie_seek_function_t = fn(ptr<void>, ptr<i64>, i32) -> i32;
// DEFAULT-NEXT:     type @type15 cookie_close_function_t = fn(ptr<void>) -> i32;
// DEFAULT-NEXT:     type @type16 _IO_cookie_io_functions_t = struct {
// DEFAULT-NEXT:         field0 read: ptr<fn(ptr<void>, ptr<i8>, u64) -> i64>;
// DEFAULT-NEXT:         field1 write: ptr<fn(ptr<void>, ptr<const i8>, u64) -> i64>;
// DEFAULT-NEXT:         field2 seek: ptr<fn(ptr<void>, ptr<i64>, i32) -> i32>;
// DEFAULT-NEXT:         field3 close: ptr<fn(ptr<void>) -> i32>;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 16, 24]];
// DEFAULT-NEXT:     type @type17 cookie_io_functions_t = @type16;
// DEFAULT-NEXT:     type @type18 ssize_t = i64;
// DEFAULT-NEXT:     type @type19 GNUCookie = struct {
// DEFAULT-NEXT:         field0 bytes: array<i8, 32>;
// DEFAULT-NEXT:         field1 length: u64;
// DEFAULT-NEXT:         field2 closed: i32;
// DEFAULT-NEXT:     } [size=48, align=8, offsets=[0, 32, 40]];
// DEFAULT-NEXT:     global %117 .str117: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([37, 115, 58, 37, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %118 .str118: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([103, 110, 117, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %119 .str119: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([103, 110, 117, 58, 50, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %120 .str120: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([37, 115, 45, 37, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %121 .str121: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([115, 108, 97, 116, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %122 .str122: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([115, 108, 97, 116, 101, 45, 50, 52, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %123 .str123: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %124 .str124: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([97, 108, 112, 104, 97, 124, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %125 .str125: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([98, 101, 116, 97, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %126 .str126: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([119, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %127 .str127: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([37, 115, 58, 37, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %128 .str128: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([99, 111, 111, 107, 105, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %129 .str129: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([99, 111, 111, 107, 105, 101, 58, 55, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %130 .str130: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([37, 50, 36, 100, 32, 37, 49, 36, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %131 .str131: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %3 @parse_printf_format(%78 __fmt: ptr<const i8> [restrict], %79 __n: u64, %80 __argtypes: ptr<i32> [restrict]) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %29 @fclose(%81 __stream: ptr<@type0>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %30 @fflush(%82 __stream: ptr<@type0>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %31 @fopencookie(%83 __magic_cookie: ptr<void> [restrict], %84 __modes: ptr<const i8> [restrict], %85 __io_funcs: @type16) -> ptr<@type0> [linkage=external] [abi=sysv64(scalar, scalar, byval<align=8>) -> scalar];
// DEFAULT-NEXT:     fn %32 @fmemopen(%86 __s: ptr<void>, %87 __len: u64, %88 __modes: ptr<const i8>) -> ptr<@type0> [linkage=external];
// DEFAULT-NEXT:     fn %33 @open_memstream(%89 __bufloc: ptr<ptr<i8>>, %90 __sizeloc: ptr<u64>) -> ptr<@type0> [linkage=external];
// DEFAULT-NEXT:     fn %34 @fprintf(%91 __stream: ptr<@type0> [restrict], %92 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %35 @printf(%93 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %36 @asprintf(%94 __ptr: ptr<ptr<i8>> [restrict], %95 __fmt: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %37 @getdelim(%96 __lineptr: ptr<ptr<i8>> [restrict], %97 __n: ptr<u64> [restrict], %98 __delimiter: i32, %99 __stream: ptr<@type0> [restrict]) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %38 @getline(%100 __lineptr: ptr<ptr<i8>> [restrict], %101 __n: ptr<u64> [restrict], %102 __stream: ptr<@type0> [restrict]) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %39 @__fbufsize(%103 __fp: ptr<@type0>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %40 @__freading(%104 __fp: ptr<@type0>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %41 @__fwriting(%105 __fp: ptr<@type0>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %42 @__fpending(%106 __fp: ptr<@type0>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %43 @free(%107 __ptr: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %44 @memcpy(%108 __dest: ptr<void> [restrict], %109 __src: ptr<const void> [restrict], %110 __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %45 @memcmp(%111 __s1: ptr<const void>, %112 __s2: ptr<const void>, %113 __n: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %46 @strcmp(%114 __s1: ptr<const i8>, %115 __s2: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %47 @strlen(%116 __s: ptr<const i8>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %49 @gnu_cookie_write(%50 state: ptr<void>, %51 buffer: ptr<const i8>, %52 size: u64) -> i64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %53 cookie: ptr<@type19> [storage=automatic] = pointer_cast<ptr<@type19>, reason=assign>(read<ptr<void>>(%50));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%44, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(32)>(field0(deref(read<ptr<@type19>>(%53)))), read<u64>(field1(deref(read<ptr<@type19>>(%53)))))), pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i8>>(%51)), read<u64>(%52));
// DEFAULT-NEXT:         let %132: ptr<@type19> [synthetic] = read<ptr<@type19>>(%53);
// DEFAULT-NEXT:         let %133: u64 [synthetic] = read<u64>(field1(deref(read<ptr<@type19>>(%132))));
// DEFAULT-NEXT:         let %134: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%133), read<u64>(%52));
// DEFAULT-NEXT:         write<u64>(field1(deref(read<ptr<@type19>>(%132))), read<u64>(%134));
// DEFAULT-NEXT:         return reinterpret<i64, reason=explicit, fits=unknown>(read<u64>(%52));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %54 @gnu_cookie_close(%55 state: ptr<void>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %56 cookie: ptr<@type19> [storage=automatic] = pointer_cast<ptr<@type19>, reason=assign>(read<ptr<void>>(%55));
// DEFAULT-NEXT:         write<i32>(field2(deref(read<ptr<@type19>>(%56))), const<i32>(1));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %57 @gnu_allocating_stdio() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %58 formatted: ptr<i8> [storage=automatic] = null<ptr<i8>>;
// DEFAULT-NEXT:         let %59 stream_data: ptr<i8> [storage=automatic] = null<ptr<i8>>;
// DEFAULT-NEXT:         let %60 stream_size: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %61 stream: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:         let %62 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %135: i32 [synthetic] = read<i32>(%62);
// DEFAULT-NEXT:         let %136: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%135), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<ptr<i8>>, ptr<const i8>, ...) -> i32>(%36, addr_of<ptr<ptr<i8>>>(%58), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%117)), array_decay<ptr<i8>, length=Some(4)>(%118), const<i32>(23)), const<i32>(6))));
// DEFAULT-NEXT:         write<i32>(%62, read<i32>(%136));
// DEFAULT-NEXT:         let %137: i32 [synthetic] = read<i32>(%62);
// DEFAULT-NEXT:         let %138: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%137), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%46, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%58)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%119))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%62, read<i32>(%138));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%43, pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%58)));
// DEFAULT-NEXT:         write<ptr<@type0>>(%61, call<ptr<@type0>, signature=fn(ptr<ptr<i8>>, ptr<u64>) -> ptr<@type0>>(%33, addr_of<ptr<ptr<i8>>>(%59), addr_of<ptr<u64>>(%60)));
// DEFAULT-NEXT:         call<ptr<@type0>, signature=fn(ptr<ptr<i8>>, ptr<u64>) -> ptr<@type0>>(%33, addr_of<ptr<ptr<i8>>>(%59), addr_of<ptr<u64>>(%60));
// DEFAULT-NEXT:         let %139: i32 [synthetic] = read<i32>(%62);
// DEFAULT-NEXT:         let %140: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%139), from_bool<i32, reason=promotion>(ne<ptr<@type0>>(read<ptr<@type0>>(%61), null<ptr<@type0>>)));
// DEFAULT-NEXT:         write<i32>(%62, read<i32>(%140));
// DEFAULT-NEXT:         let %141: i32 [synthetic] = read<i32>(%62);
// DEFAULT-NEXT:         let %142: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%141), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<@type0>, ptr<const i8>, ...) -> i32>(%34, read<ptr<@type0>>(%61), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%120)), array_decay<ptr<i8>, length=Some(6)>(%121), const<i32>(24)), const<i32>(8))));
// DEFAULT-NEXT:         write<i32>(%62, read<i32>(%142));
// DEFAULT-NEXT:         let %143: i32 [synthetic] = read<i32>(%62);
// DEFAULT-NEXT:         let %144: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%143), from_bool<i32, reason=promotion>(ne<i32>(call<i32, signature=fn(ptr<@type0>) -> i32>(%41, read<ptr<@type0>>(%61)), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%62, read<i32>(%144));
// DEFAULT-NEXT:         let %145: i32 [synthetic] = read<i32>(%62);
// DEFAULT-NEXT:         let %146: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%145), from_bool<i32, reason=promotion>(gt<u64>(call<u64, signature=fn(ptr<@type0>) -> u64>(%42, read<ptr<@type0>>(%61)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))));
// DEFAULT-NEXT:         write<i32>(%62, read<i32>(%146));
// DEFAULT-NEXT:         let %147: i32 [synthetic] = read<i32>(%62);
// DEFAULT-NEXT:         let %148: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%147), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<@type0>) -> i32>(%30, read<ptr<@type0>>(%61)), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%62, read<i32>(%148));
// DEFAULT-NEXT:         let %149: i32 [synthetic] = read<i32>(%62);
// DEFAULT-NEXT:         let %150: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%149), from_bool<i32, reason=promotion>(eq<u64>(read<u64>(%60), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))));
// DEFAULT-NEXT:         write<i32>(%62, read<i32>(%150));
// DEFAULT-NEXT:         let %151: i32 [synthetic] = read<i32>(%62);
// DEFAULT-NEXT:         let %152: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%151), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%46, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%59)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%122))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%62, read<i32>(%152));
// DEFAULT-NEXT:         let %153: i32 [synthetic] = read<i32>(%62);
// DEFAULT-NEXT:         let %154: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%153), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<@type0>) -> i32>(%29, read<ptr<@type0>>(%61)), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%62, read<i32>(%154));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%43, pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%59)));
// DEFAULT-NEXT:         return read<i32>(%62);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %63 @gnu_memory_stdio() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %64 source: array<i8, 12> [storage=automatic] = code_units<array<i8, 12>>([97, 108, 112, 104, 97, 124, 98, 101, 116, 97, 10, 0]);
// DEFAULT-NEXT:         let %65 line: ptr<i8> [storage=automatic] = null<ptr<i8>>;
// DEFAULT-NEXT:         let %66 capacity: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %67 stream: ptr<@type0> [storage=automatic] = call<ptr<@type0>, signature=fn(ptr<void>, u64, ptr<const i8>) -> ptr<@type0>>(%32, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(12)>(%64)), call<u64, signature=fn(ptr<const i8>) -> u64>(%47, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(12)>(%64))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%123)));
// DEFAULT-NEXT:         let %68 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %155: i32 [synthetic] = read<i32>(%68);
// DEFAULT-NEXT:         let %156: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%155), from_bool<i32, reason=promotion>(ne<ptr<@type0>>(read<ptr<@type0>>(%67), null<ptr<@type0>>)));
// DEFAULT-NEXT:         write<i32>(%68, read<i32>(%156));
// DEFAULT-NEXT:         let %157: i32 [synthetic] = read<i32>(%68);
// DEFAULT-NEXT:         let %158: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%157), from_bool<i32, reason=promotion>(eq<i64>(call<i64, signature=fn(ptr<ptr<i8>>, ptr<u64>, i32, ptr<@type0>) -> i64>(%37, addr_of<ptr<ptr<i8>>>(%65), addr_of<ptr<u64>>(%66), const<i32>(124), read<ptr<@type0>>(%67)), widen<i64, reason=usual_arith>(const<i32>(6)))));
// DEFAULT-NEXT:         write<i32>(%68, read<i32>(%158));
// DEFAULT-NEXT:         let %159: i32 [synthetic] = read<i32>(%68);
// DEFAULT-NEXT:         let %160: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%159), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%46, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%65)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%124))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%68, read<i32>(%160));
// DEFAULT-NEXT:         let %161: i32 [synthetic] = read<i32>(%68);
// DEFAULT-NEXT:         let %162: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%161), from_bool<i32, reason=promotion>(eq<i64>(call<i64, signature=fn(ptr<ptr<i8>>, ptr<u64>, ptr<@type0>) -> i64>(%38, addr_of<ptr<ptr<i8>>>(%65), addr_of<ptr<u64>>(%66), read<ptr<@type0>>(%67)), widen<i64, reason=usual_arith>(const<i32>(5)))));
// DEFAULT-NEXT:         write<i32>(%68, read<i32>(%162));
// DEFAULT-NEXT:         let %163: i32 [synthetic] = read<i32>(%68);
// DEFAULT-NEXT:         let %164: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%163), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%46, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%65)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%125))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%68, read<i32>(%164));
// DEFAULT-NEXT:         let %165: i32 [synthetic] = read<i32>(%68);
// DEFAULT-NEXT:         let %166: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%165), from_bool<i32, reason=promotion>(ne<i32>(call<i32, signature=fn(ptr<@type0>) -> i32>(%40, read<ptr<@type0>>(%67)), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%68, read<i32>(%166));
// DEFAULT-NEXT:         let %167: i32 [synthetic] = read<i32>(%68);
// DEFAULT-NEXT:         let %168: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%167), from_bool<i32, reason=promotion>(gt<u64>(call<u64, signature=fn(ptr<@type0>) -> u64>(%39, read<ptr<@type0>>(%67)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))));
// DEFAULT-NEXT:         write<i32>(%68, read<i32>(%168));
// DEFAULT-NEXT:         let %169: i32 [synthetic] = read<i32>(%68);
// DEFAULT-NEXT:         let %170: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%169), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<@type0>) -> i32>(%29, read<ptr<@type0>>(%67)), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%68, read<i32>(%170));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%43, pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%65)));
// DEFAULT-NEXT:         return read<i32>(%68);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %69 @gnu_cookie_stdio() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %70 cookie: @type19 [storage=automatic] = aggregate<@type19, zero_fill=true>();
// DEFAULT-NEXT:         let %71 functions: @type16 [storage=automatic] = aggregate<@type16, zero_fill=true>();
// DEFAULT-NEXT:         let %72 stream: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:         let %73 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         write<ptr<fn(ptr<void>, ptr<const i8>, u64) -> i64>>(field1(%71), function_decay<ptr<fn(ptr<void>, ptr<const i8>, u64) -> i64>>(%49));
// DEFAULT-NEXT:         write<ptr<fn(ptr<void>) -> i32>>(field3(%71), function_decay<ptr<fn(ptr<void>) -> i32>>(%54));
// DEFAULT-NEXT:         write<ptr<@type0>>(%72, call<ptr<@type0>, signature=fn(ptr<void>, ptr<const i8>, @type16) -> ptr<@type0>, abi=sysv64(scalar, scalar, byval<align=8>) -> scalar>(%31, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type19>>(%70)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%126)), copy<@type16, reason=arg>(read<@type16>(%71))));
// DEFAULT-NEXT:         call<ptr<@type0>, signature=fn(ptr<void>, ptr<const i8>, @type16) -> ptr<@type0>, abi=sysv64(scalar, scalar, byval<align=8>) -> scalar>(%31, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type19>>(%70)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%126)), copy<@type16, reason=arg>(read<@type16>(%71)));
// DEFAULT-NEXT:         let %171: i32 [synthetic] = read<i32>(%73);
// DEFAULT-NEXT:         let %172: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%171), from_bool<i32, reason=promotion>(ne<ptr<@type0>>(read<ptr<@type0>>(%72), null<ptr<@type0>>)));
// DEFAULT-NEXT:         write<i32>(%73, read<i32>(%172));
// DEFAULT-NEXT:         let %173: i32 [synthetic] = read<i32>(%73);
// DEFAULT-NEXT:         let %174: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%173), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<@type0>, ptr<const i8>, ...) -> i32>(%34, read<ptr<@type0>>(%72), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%127)), array_decay<ptr<i8>, length=Some(7)>(%128), const<i32>(7)), const<i32>(8))));
// DEFAULT-NEXT:         write<i32>(%73, read<i32>(%174));
// DEFAULT-NEXT:         let %175: i32 [synthetic] = read<i32>(%73);
// DEFAULT-NEXT:         let %176: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%175), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<@type0>) -> i32>(%29, read<ptr<@type0>>(%72)), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%73, read<i32>(%176));
// DEFAULT-NEXT:         let %177: i32 [synthetic] = read<i32>(%73);
// DEFAULT-NEXT:         let %178: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%177), from_bool<i32, reason=promotion>(eq<i32>(read<i32>(field2(%70)), const<i32>(1))));
// DEFAULT-NEXT:         write<i32>(%73, read<i32>(%178));
// DEFAULT-NEXT:         let %179: i32 [synthetic] = read<i32>(%73);
// DEFAULT-NEXT:         let %180: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%179), from_bool<i32, reason=promotion>(eq<u64>(read<u64>(field1(%70)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))));
// DEFAULT-NEXT:         write<i32>(%73, read<i32>(%180));
// DEFAULT-NEXT:         let %181: i32 [synthetic] = read<i32>(%73);
// DEFAULT-NEXT:         let %182: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%181), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%45, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(32)>(field0(%70))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%129)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8)))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%73, read<i32>(%182));
// DEFAULT-NEXT:         return read<i32>(%73);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %74 @gnu_printf_introspection() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %75 types: array<i32, 4> [storage=automatic] [align=16] = aggregate<array<i32, 4>, zero_fill=true>();
// DEFAULT-NEXT:         let %76 arguments: u64 [storage=automatic] = call<u64, signature=fn(ptr<const i8>, u64, ptr<i32>) -> u64>(%3, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%130)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))), array_decay<ptr<i32>, length=Some(4)>(%75));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(logical_and<bool>(eq<u64>(read<u64>(%76), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), eq<i32>(and<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%75), const<i32>(0)))), not<i32>(const<i32>(65280))), const<i32>(3))), eq<i32>(and<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%75), const<i32>(1)))), not<i32>(const<i32>(65280))), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %77 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%35, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(13)>(%131)), call<i32, signature=fn() -> i32>(%57), call<i32, signature=fn() -> i32>(%63), call<i32, signature=fn() -> i32>(%69), call<i32, signature=fn() -> i32>(%74));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
