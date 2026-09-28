#if defined(_MSC_VER)
int msc_ver = _MSC_VER;
#elif defined(__clang__)
int clang_major = __clang_major__;
#elif defined(__GNUC__)
int gnuc = __GNUC__;
#endif
unsigned long sizeof_long = sizeof(long);
unsigned long sizeof_long_double = sizeof(long double);
unsigned long sizeof_va_list = sizeof(__builtin_va_list);
unsigned long alignof_long_long = _Alignof(long long);
struct pair { int a; int b; };
struct pair record(struct pair value) { return value; }

// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES ARMV7-LINUX-GNUEABI-MSVC

// SLATE-FILECHECK-BEGIN ARMV7-LINUX-GNUEABI-MSVC
// ARMV7-LINUX-GNUEABI-MSVC: module {
// ARMV7-LINUX-GNUEABI-MSVC-NEXT:     target "armv7-unknown-linux-gnueabi" {
// ARMV7-LINUX-GNUEABI-MSVC-NEXT:         endian = little;
// ARMV7-LINUX-GNUEABI-MSVC-NEXT:         pointer [size=4, align=4];
// ARMV7-LINUX-GNUEABI-MSVC-NEXT:         stack_alignment = 8;
// ARMV7-LINUX-GNUEABI-MSVC-NEXT:         long_double = f64;
// ARMV7-LINUX-GNUEABI-MSVC-NEXT:         storage bool [size=1, align=1];
// ARMV7-LINUX-GNUEABI-MSVC-NEXT:         storage i8, u8 [size=1, align=1];
// ARMV7-LINUX-GNUEABI-MSVC-NEXT:         storage i16, u16 [size=2, align=2];
// ARMV7-LINUX-GNUEABI-MSVC-NEXT:         storage i32, u32 [size=4, align=4];
// ARMV7-LINUX-GNUEABI-MSVC-NEXT:         storage i64, u64 [size=8, align=8];
// ARMV7-LINUX-GNUEABI-MSVC-NEXT:         storage bf16 [size=2, align=2];
// ARMV7-LINUX-GNUEABI-MSVC-NEXT:         storage f16 [size=2, align=2];
// ARMV7-LINUX-GNUEABI-MSVC-NEXT:         storage f32 [size=4, align=4];
// ARMV7-LINUX-GNUEABI-MSVC-NEXT:         storage f64 [size=8, align=8];
// ARMV7-LINUX-GNUEABI-MSVC-NEXT:         storage f128 [size=16, align=16];
// ARMV7-LINUX-GNUEABI-MSVC-NEXT:         storage d32 [size=4, align=4];
// ARMV7-LINUX-GNUEABI-MSVC-NEXT:         storage d64 [size=8, align=8];
// ARMV7-LINUX-GNUEABI-MSVC-NEXT:         storage d128 [size=16, align=16];
// ARMV7-LINUX-GNUEABI-MSVC-NEXT:     }
// ARMV7-LINUX-GNUEABI-MSVC-NEXT:     type @type0 pair = struct {
// ARMV7-LINUX-GNUEABI-MSVC-NEXT:         field0 a: i32;
// ARMV7-LINUX-GNUEABI-MSVC-NEXT:         field1 b: i32;
// ARMV7-LINUX-GNUEABI-MSVC-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// ARMV7-LINUX-GNUEABI-MSVC-NEXT:     global %0 clang_major: i32 [storage=static] = const<i32>(22) [linkage=external];
// ARMV7-LINUX-GNUEABI-MSVC-NEXT:     global %1 sizeof_long: u32 [storage=static] = const<u32>(4) [linkage=external];
// ARMV7-LINUX-GNUEABI-MSVC-NEXT:     global %2 sizeof_long_double: u32 [storage=static] = const<u32>(8) [linkage=external];
// ARMV7-LINUX-GNUEABI-MSVC-NEXT:     global %3 sizeof_va_list: u32 [storage=static] = const<u32>(4) [linkage=external];
// ARMV7-LINUX-GNUEABI-MSVC-NEXT:     global %4 alignof_long_long: u32 [storage=static] = const<u32>(8) [linkage=external];
// ARMV7-LINUX-GNUEABI-MSVC-NEXT:     fn %6 @record(%7 value: @type0) -> @type0 [linkage=external] [abi=aapcs32(native_c) -> native_c] [fallthrough=ub_if_used] {
// ARMV7-LINUX-GNUEABI-MSVC-NEXT:         return copy<@type0, reason=return>(read<@type0>(%7));
// ARMV7-LINUX-GNUEABI-MSVC-NEXT:     }
// ARMV7-LINUX-GNUEABI-MSVC-NEXT: }
// SLATE-FILECHECK-END ARMV7-LINUX-GNUEABI-MSVC
