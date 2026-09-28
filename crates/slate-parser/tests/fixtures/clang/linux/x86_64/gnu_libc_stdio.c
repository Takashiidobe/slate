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
// DEFAULT-NEXT:     global %166 .str166: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([37, 115, 58, 37, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %167 .str167: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([103, 110, 117, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %168 .str168: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([103, 110, 117, 58, 50, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %169 .str169: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([37, 115, 45, 37, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %170 .str170: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([115, 108, 97, 116, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %171 .str171: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([115, 108, 97, 116, 101, 45, 50, 52, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %172 .str172: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %173 .str173: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([97, 108, 112, 104, 97, 124, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %174 .str174: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([98, 101, 116, 97, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %175 .str175: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([119, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %176 .str176: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([37, 115, 58, 37, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %177 .str177: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([99, 111, 111, 107, 105, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %178 .str178: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([99, 111, 111, 107, 105, 101, 58, 55, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %179 .str179: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([37, 50, 36, 100, 32, 37, 49, 36, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %180 .str180: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %6 @parse_printf_format(%127 __fmt: ptr<const i8> [restrict], %128 __n: u64, %129 __argtypes: ptr<i32> [restrict]) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %43 @fclose(%130 __stream: ptr<@type0>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %45 @fflush(%131 __stream: ptr<@type0>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %49 @fopencookie(%132 __magic_cookie: ptr<void> [restrict], %133 __modes: ptr<const i8> [restrict], %134 __io_funcs: @type16) -> ptr<@type0> [linkage=external] [abi=sysv64(scalar, scalar, native_c) -> scalar];
// DEFAULT-NEXT:     fn %53 @fmemopen(%135 __s: ptr<void>, %136 __len: u64, %137 __modes: ptr<const i8>) -> ptr<@type0> [linkage=external];
// DEFAULT-NEXT:     fn %56 @open_memstream(%138 __bufloc: ptr<ptr<i8>>, %139 __sizeloc: ptr<u64>) -> ptr<@type0> [linkage=external];
// DEFAULT-NEXT:     fn %59 @fprintf(%140 __stream: ptr<@type0> [restrict], %141 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %61 @printf(%142 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %64 @asprintf(%143 __ptr: ptr<ptr<i8>> [restrict], %144 __fmt: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %69 @getdelim(%145 __lineptr: ptr<ptr<i8>> [restrict], %146 __n: ptr<u64> [restrict], %147 __delimiter: i32, %148 __stream: ptr<@type0> [restrict]) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %73 @getline(%149 __lineptr: ptr<ptr<i8>> [restrict], %150 __n: ptr<u64> [restrict], %151 __stream: ptr<@type0> [restrict]) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %75 @__fbufsize(%152 __fp: ptr<@type0>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %77 @__freading(%153 __fp: ptr<@type0>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %79 @__fwriting(%154 __fp: ptr<@type0>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %81 @__fpending(%155 __fp: ptr<@type0>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %83 @free(%156 __ptr: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %87 @memcpy(%157 __dest: ptr<void> [restrict], %158 __src: ptr<const void> [restrict], %159 __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %91 @memcmp(%160 __s1: ptr<const void>, %161 __s2: ptr<const void>, %162 __n: u64) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %94 @strcmp(%163 __s1: ptr<const i8>, %164 __s2: ptr<const i8>) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %96 @strlen(%165 __s: ptr<const i8>) -> u64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %98 @gnu_cookie_write(%99 state: ptr<void>, %100 buffer: ptr<const i8>, %101 size: u64) -> i64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %102 cookie: ptr<@type19> [storage=automatic] = pointer_cast<ptr<@type19>, reason=assign>(read<ptr<void>>(%99));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%87, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(32)>(field0(deref(read<ptr<@type19>>(%102)))), read<u64>(field1(deref(read<ptr<@type19>>(%102)))))), pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i8>>(%100)), read<u64>(%101));
// DEFAULT-NEXT:         let %181: ptr<@type19> [synthetic] = read<ptr<@type19>>(%102);
// DEFAULT-NEXT:         let %182: u64 [synthetic] = read<u64>(field1(deref(read<ptr<@type19>>(%181))));
// DEFAULT-NEXT:         let %183: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%182), read<u64>(%101));
// DEFAULT-NEXT:         write<u64>(field1(deref(read<ptr<@type19>>(%181))), read<u64>(%183));
// DEFAULT-NEXT:         return reinterpret<i64, reason=explicit, fits=unknown>(read<u64>(%101));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %103 @gnu_cookie_close(%104 state: ptr<void>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %105 cookie: ptr<@type19> [storage=automatic] = pointer_cast<ptr<@type19>, reason=assign>(read<ptr<void>>(%104));
// DEFAULT-NEXT:         write<i32>(field2(deref(read<ptr<@type19>>(%105))), const<i32>(1));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %106 @gnu_allocating_stdio() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %107 formatted: ptr<i8> [storage=automatic] = null<ptr<i8>>;
// DEFAULT-NEXT:         let %108 stream_data: ptr<i8> [storage=automatic] = null<ptr<i8>>;
// DEFAULT-NEXT:         let %109 stream_size: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %110 stream: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:         let %111 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %184: i32 [synthetic] = read<i32>(%111);
// DEFAULT-NEXT:         let %185: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%184), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<ptr<i8>>, ptr<const i8>, ...) -> i32>(%64, addr_of<ptr<ptr<i8>>>(%107), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%166)), array_decay<ptr<i8>, length=Some(4)>(%167), const<i32>(23)), const<i32>(6))));
// DEFAULT-NEXT:         write<i32>(%111, read<i32>(%185));
// DEFAULT-NEXT:         let %186: i32 [synthetic] = read<i32>(%111);
// DEFAULT-NEXT:         let %187: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%186), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%94, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%107)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%168))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%111, read<i32>(%187));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%83, pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%107)));
// DEFAULT-NEXT:         write<ptr<@type0>>(%110, call<ptr<@type0>, signature=fn(ptr<ptr<i8>>, ptr<u64>) -> ptr<@type0>>(%56, addr_of<ptr<ptr<i8>>>(%108), addr_of<ptr<u64>>(%109)));
// DEFAULT-NEXT:         call<ptr<@type0>, signature=fn(ptr<ptr<i8>>, ptr<u64>) -> ptr<@type0>>(%56, addr_of<ptr<ptr<i8>>>(%108), addr_of<ptr<u64>>(%109));
// DEFAULT-NEXT:         let %188: i32 [synthetic] = read<i32>(%111);
// DEFAULT-NEXT:         let %189: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%188), from_bool<i32, reason=promotion>(ne<ptr<@type0>>(read<ptr<@type0>>(%110), null<ptr<@type0>>)));
// DEFAULT-NEXT:         write<i32>(%111, read<i32>(%189));
// DEFAULT-NEXT:         let %190: i32 [synthetic] = read<i32>(%111);
// DEFAULT-NEXT:         let %191: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%190), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<@type0>, ptr<const i8>, ...) -> i32>(%59, read<ptr<@type0>>(%110), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%169)), array_decay<ptr<i8>, length=Some(6)>(%170), const<i32>(24)), const<i32>(8))));
// DEFAULT-NEXT:         write<i32>(%111, read<i32>(%191));
// DEFAULT-NEXT:         let %192: i32 [synthetic] = read<i32>(%111);
// DEFAULT-NEXT:         let %193: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%192), from_bool<i32, reason=promotion>(ne<i32>(call<i32, signature=fn(ptr<@type0>) -> i32>(%79, read<ptr<@type0>>(%110)), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%111, read<i32>(%193));
// DEFAULT-NEXT:         let %194: i32 [synthetic] = read<i32>(%111);
// DEFAULT-NEXT:         let %195: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%194), from_bool<i32, reason=promotion>(gt<u64>(call<u64, signature=fn(ptr<@type0>) -> u64>(%81, read<ptr<@type0>>(%110)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))));
// DEFAULT-NEXT:         write<i32>(%111, read<i32>(%195));
// DEFAULT-NEXT:         let %196: i32 [synthetic] = read<i32>(%111);
// DEFAULT-NEXT:         let %197: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%196), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<@type0>) -> i32>(%45, read<ptr<@type0>>(%110)), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%111, read<i32>(%197));
// DEFAULT-NEXT:         let %198: i32 [synthetic] = read<i32>(%111);
// DEFAULT-NEXT:         let %199: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%198), from_bool<i32, reason=promotion>(eq<u64>(read<u64>(%109), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))));
// DEFAULT-NEXT:         write<i32>(%111, read<i32>(%199));
// DEFAULT-NEXT:         let %200: i32 [synthetic] = read<i32>(%111);
// DEFAULT-NEXT:         let %201: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%200), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%94, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%108)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%171))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%111, read<i32>(%201));
// DEFAULT-NEXT:         let %202: i32 [synthetic] = read<i32>(%111);
// DEFAULT-NEXT:         let %203: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%202), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<@type0>) -> i32>(%43, read<ptr<@type0>>(%110)), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%111, read<i32>(%203));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%83, pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%108)));
// DEFAULT-NEXT:         return read<i32>(%111);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %112 @gnu_memory_stdio() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %113 source: array<i8, 12> [storage=automatic] = code_units<array<i8, 12>>([97, 108, 112, 104, 97, 124, 98, 101, 116, 97, 10, 0]);
// DEFAULT-NEXT:         let %114 line: ptr<i8> [storage=automatic] = null<ptr<i8>>;
// DEFAULT-NEXT:         let %115 capacity: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %116 stream: ptr<@type0> [storage=automatic] = call<ptr<@type0>, signature=fn(ptr<void>, u64, ptr<const i8>) -> ptr<@type0>>(%53, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(12)>(%113)), call<u64, signature=fn(ptr<const i8>) -> u64>(%96, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(12)>(%113))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%172)));
// DEFAULT-NEXT:         let %117 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %204: i32 [synthetic] = read<i32>(%117);
// DEFAULT-NEXT:         let %205: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%204), from_bool<i32, reason=promotion>(ne<ptr<@type0>>(read<ptr<@type0>>(%116), null<ptr<@type0>>)));
// DEFAULT-NEXT:         write<i32>(%117, read<i32>(%205));
// DEFAULT-NEXT:         let %206: i32 [synthetic] = read<i32>(%117);
// DEFAULT-NEXT:         let %207: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%206), from_bool<i32, reason=promotion>(eq<i64>(call<i64, signature=fn(ptr<ptr<i8>>, ptr<u64>, i32, ptr<@type0>) -> i64>(%69, addr_of<ptr<ptr<i8>>>(%114), addr_of<ptr<u64>>(%115), const<i32>(124), read<ptr<@type0>>(%116)), widen<i64, reason=usual_arith>(const<i32>(6)))));
// DEFAULT-NEXT:         write<i32>(%117, read<i32>(%207));
// DEFAULT-NEXT:         let %208: i32 [synthetic] = read<i32>(%117);
// DEFAULT-NEXT:         let %209: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%208), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%94, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%114)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%173))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%117, read<i32>(%209));
// DEFAULT-NEXT:         let %210: i32 [synthetic] = read<i32>(%117);
// DEFAULT-NEXT:         let %211: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%210), from_bool<i32, reason=promotion>(eq<i64>(call<i64, signature=fn(ptr<ptr<i8>>, ptr<u64>, ptr<@type0>) -> i64>(%73, addr_of<ptr<ptr<i8>>>(%114), addr_of<ptr<u64>>(%115), read<ptr<@type0>>(%116)), widen<i64, reason=usual_arith>(const<i32>(5)))));
// DEFAULT-NEXT:         write<i32>(%117, read<i32>(%211));
// DEFAULT-NEXT:         let %212: i32 [synthetic] = read<i32>(%117);
// DEFAULT-NEXT:         let %213: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%212), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%94, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%114)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%174))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%117, read<i32>(%213));
// DEFAULT-NEXT:         let %214: i32 [synthetic] = read<i32>(%117);
// DEFAULT-NEXT:         let %215: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%214), from_bool<i32, reason=promotion>(ne<i32>(call<i32, signature=fn(ptr<@type0>) -> i32>(%77, read<ptr<@type0>>(%116)), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%117, read<i32>(%215));
// DEFAULT-NEXT:         let %216: i32 [synthetic] = read<i32>(%117);
// DEFAULT-NEXT:         let %217: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%216), from_bool<i32, reason=promotion>(gt<u64>(call<u64, signature=fn(ptr<@type0>) -> u64>(%75, read<ptr<@type0>>(%116)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))));
// DEFAULT-NEXT:         write<i32>(%117, read<i32>(%217));
// DEFAULT-NEXT:         let %218: i32 [synthetic] = read<i32>(%117);
// DEFAULT-NEXT:         let %219: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%218), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<@type0>) -> i32>(%43, read<ptr<@type0>>(%116)), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%117, read<i32>(%219));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%83, pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%114)));
// DEFAULT-NEXT:         return read<i32>(%117);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %118 @gnu_cookie_stdio() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %119 cookie: @type19 [storage=automatic] = aggregate<@type19, zero_fill=true>();
// DEFAULT-NEXT:         let %120 functions: @type16 [storage=automatic] = aggregate<@type16, zero_fill=true>();
// DEFAULT-NEXT:         let %121 stream: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:         let %122 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         write<ptr<fn(ptr<void>, ptr<const i8>, u64) -> i64>>(field1(%120), function_decay<ptr<fn(ptr<void>, ptr<const i8>, u64) -> i64>>(%98));
// DEFAULT-NEXT:         write<ptr<fn(ptr<void>) -> i32>>(field3(%120), function_decay<ptr<fn(ptr<void>) -> i32>>(%103));
// DEFAULT-NEXT:         write<ptr<@type0>>(%121, call<ptr<@type0>, signature=fn(ptr<void>, ptr<const i8>, @type16) -> ptr<@type0>, abi=sysv64(scalar, scalar, native_c) -> scalar>(%49, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type19>>(%119)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%175)), copy<@type16, reason=arg>(read<@type16>(%120))));
// DEFAULT-NEXT:         call<ptr<@type0>, signature=fn(ptr<void>, ptr<const i8>, @type16) -> ptr<@type0>, abi=sysv64(scalar, scalar, native_c) -> scalar>(%49, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type19>>(%119)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%175)), copy<@type16, reason=arg>(read<@type16>(%120)));
// DEFAULT-NEXT:         let %220: i32 [synthetic] = read<i32>(%122);
// DEFAULT-NEXT:         let %221: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%220), from_bool<i32, reason=promotion>(ne<ptr<@type0>>(read<ptr<@type0>>(%121), null<ptr<@type0>>)));
// DEFAULT-NEXT:         write<i32>(%122, read<i32>(%221));
// DEFAULT-NEXT:         let %222: i32 [synthetic] = read<i32>(%122);
// DEFAULT-NEXT:         let %223: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%222), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<@type0>, ptr<const i8>, ...) -> i32>(%59, read<ptr<@type0>>(%121), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%176)), array_decay<ptr<i8>, length=Some(7)>(%177), const<i32>(7)), const<i32>(8))));
// DEFAULT-NEXT:         write<i32>(%122, read<i32>(%223));
// DEFAULT-NEXT:         let %224: i32 [synthetic] = read<i32>(%122);
// DEFAULT-NEXT:         let %225: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%224), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<@type0>) -> i32>(%43, read<ptr<@type0>>(%121)), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%122, read<i32>(%225));
// DEFAULT-NEXT:         let %226: i32 [synthetic] = read<i32>(%122);
// DEFAULT-NEXT:         let %227: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%226), from_bool<i32, reason=promotion>(eq<i32>(read<i32>(field2(%119)), const<i32>(1))));
// DEFAULT-NEXT:         write<i32>(%122, read<i32>(%227));
// DEFAULT-NEXT:         let %228: i32 [synthetic] = read<i32>(%122);
// DEFAULT-NEXT:         let %229: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%228), from_bool<i32, reason=promotion>(eq<u64>(read<u64>(field1(%119)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))));
// DEFAULT-NEXT:         write<i32>(%122, read<i32>(%229));
// DEFAULT-NEXT:         let %230: i32 [synthetic] = read<i32>(%122);
// DEFAULT-NEXT:         let %231: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%230), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%91, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(32)>(field0(%119))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%178)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8)))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%122, read<i32>(%231));
// DEFAULT-NEXT:         return read<i32>(%122);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %123 @gnu_printf_introspection() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %124 types: array<i32, 4> [storage=automatic] [align=16] = aggregate<array<i32, 4>, zero_fill=true>();
// DEFAULT-NEXT:         let %125 arguments: u64 [storage=automatic] = call<u64, signature=fn(ptr<const i8>, u64, ptr<i32>) -> u64>(%6, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%179)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))), array_decay<ptr<i32>, length=Some(4)>(%124));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(logical_and<bool>(eq<u64>(read<u64>(%125), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), eq<i32>(and<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%124), const<i32>(0)))), not<i32>(const<i32>(65280))), const<i32>(3))), eq<i32>(and<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%124), const<i32>(1)))), not<i32>(const<i32>(65280))), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %126 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%61, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(13)>(%180)), call<i32, signature=fn() -> i32>(%106), call<i32, signature=fn() -> i32>(%112), call<i32, signature=fn() -> i32>(%118), call<i32, signature=fn() -> i32>(%123));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
