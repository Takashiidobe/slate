// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ARGS --dump-ir --show-metadata

typedef unsigned long size_t;
typedef unsigned int uint32_t;
typedef uint32_t word;
typedef const char *cstring;
typedef int vector[4];
typedef int callback(int value);
typedef int (*fnptr)(int value);
typedef char plain_char;
typedef signed char signed_char;
typedef unsigned char unsigned_char;

word value(void) { return 7; }
size_t count(void) { return 1; }

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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64 [c="unsigned long"];
// DEFAULT-NEXT:     type @type[[TYPE_uint32_t:[0-9]+]] uint32_t = u32 [c="unsigned int"];
// DEFAULT-NEXT:     type @type[[TYPE_word:[0-9]+]] word = u32 [c="uint32_t"] [c_canon="unsigned int"] [typedef_chain="uint32_t"];
// DEFAULT-NEXT:     type @type[[TYPE_cstring:[0-9]+]] cstring = ptr<const i8> [c="const char *"];
// DEFAULT-NEXT:     type @type[[TYPE_vector:[0-9]+]] vector = array<i32, 4> [c="int[4]"];
// DEFAULT-NEXT:     type @type[[TYPE_callback:[0-9]+]] callback = fn(i32) -> i32 [c="int(int)"];
// DEFAULT-NEXT:     type @type[[TYPE_fnptr:[0-9]+]] fnptr = ptr<fn(i32) -> i32> [c="int (*)(int)"];
// DEFAULT-NEXT:     type @type[[TYPE_plain_char:[0-9]+]] plain_char = i8 [c="char"];
// DEFAULT-NEXT:     type @type[[TYPE_signed_char:[0-9]+]] signed_char = i8 [c="signed char"];
// DEFAULT-NEXT:     type @type[[TYPE_unsigned_char:[0-9]+]] unsigned_char = u8 [c="unsigned char"];
// DEFAULT-NEXT:     fn %[[VALUE_value:[0-9]+]] @value() -> u32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="word"] [c="word(void)"] [c_canon="unsigned int(void)"] [typedef_chain="word -> uint32_t"] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=always>(const<i32>(7));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_count:[0-9]+]] @count() -> u64 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="size_t"] [c="size_t(void)"] [c_canon="unsigned long(void)"] [typedef_chain="size_t"] {
// DEFAULT-NEXT:         return reinterpret<u64, reason=return, fits=unknown>(widen<i64, reason=return>(const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
