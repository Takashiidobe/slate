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
// SLATE-FILECHECK-DEFINES AARCH64-WINDOWS-MSVC-CLANG

// SLATE-FILECHECK-BEGIN AARCH64-WINDOWS-MSVC-CLANG
// AARCH64-WINDOWS-MSVC-CLANG: module {
// AARCH64-WINDOWS-MSVC-CLANG-NEXT:     target "aarch64-pc-windows-msvc" {
// AARCH64-WINDOWS-MSVC-CLANG-NEXT:         endian = little;
// AARCH64-WINDOWS-MSVC-CLANG-NEXT:         pointer [size=8, align=8];
// AARCH64-WINDOWS-MSVC-CLANG-NEXT:         stack_alignment = 16;
// AARCH64-WINDOWS-MSVC-CLANG-NEXT:         long_double = f64;
// AARCH64-WINDOWS-MSVC-CLANG-NEXT:         storage bool [size=1, align=1];
// AARCH64-WINDOWS-MSVC-CLANG-NEXT:         storage i8, u8 [size=1, align=1];
// AARCH64-WINDOWS-MSVC-CLANG-NEXT:         storage i16, u16 [size=2, align=2];
// AARCH64-WINDOWS-MSVC-CLANG-NEXT:         storage i32, u32 [size=4, align=4];
// AARCH64-WINDOWS-MSVC-CLANG-NEXT:         storage i64, u64 [size=8, align=8];
// AARCH64-WINDOWS-MSVC-CLANG-NEXT:         storage i128, u128 [size=16, align=16];
// AARCH64-WINDOWS-MSVC-CLANG-NEXT:         storage bf16 [size=2, align=2];
// AARCH64-WINDOWS-MSVC-CLANG-NEXT:         storage f16 [size=2, align=2];
// AARCH64-WINDOWS-MSVC-CLANG-NEXT:         storage f32 [size=4, align=4];
// AARCH64-WINDOWS-MSVC-CLANG-NEXT:         storage f64 [size=8, align=8];
// AARCH64-WINDOWS-MSVC-CLANG-NEXT:         storage f128 [size=16, align=16];
// AARCH64-WINDOWS-MSVC-CLANG-NEXT:         storage d32 [size=4, align=4];
// AARCH64-WINDOWS-MSVC-CLANG-NEXT:         storage d64 [size=8, align=8];
// AARCH64-WINDOWS-MSVC-CLANG-NEXT:         storage d128 [size=16, align=16];
// AARCH64-WINDOWS-MSVC-CLANG-NEXT:     }
// AARCH64-WINDOWS-MSVC-CLANG-NEXT:     type @type0 pair = struct {
// AARCH64-WINDOWS-MSVC-CLANG-NEXT:         field0 a: i32;
// AARCH64-WINDOWS-MSVC-CLANG-NEXT:         field1 b: i32;
// AARCH64-WINDOWS-MSVC-CLANG-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// AARCH64-WINDOWS-MSVC-CLANG-NEXT:     global %0 msc_ver: i32 [storage=static] = const<i32>(1933) [linkage=external];
// AARCH64-WINDOWS-MSVC-CLANG-NEXT:     global %1 sizeof_long: u32 [storage=static] = truncate<u32>(const<u64>(4)) [linkage=external];
// AARCH64-WINDOWS-MSVC-CLANG-NEXT:     global %2 sizeof_long_double: u32 [storage=static] = truncate<u32>(const<u64>(8)) [linkage=external];
// AARCH64-WINDOWS-MSVC-CLANG-NEXT:     global %3 sizeof_va_list: u32 [storage=static] = truncate<u32>(const<u64>(8)) [linkage=external];
// AARCH64-WINDOWS-MSVC-CLANG-NEXT:     global %4 alignof_long_long: u32 [storage=static] = truncate<u32>(const<u64>(8)) [linkage=external];
// AARCH64-WINDOWS-MSVC-CLANG-NEXT:     fn %6 @record(%7 value: @type0) -> @type0 [linkage=external] [abi=win_arm64(native_c) -> native_c] [fallthrough=ub_if_used] {
// AARCH64-WINDOWS-MSVC-CLANG-NEXT:         return copy<@type0, reason=return>(read<@type0>(%7));
// AARCH64-WINDOWS-MSVC-CLANG-NEXT:     }
// AARCH64-WINDOWS-MSVC-CLANG-NEXT: }
// SLATE-FILECHECK-END AARCH64-WINDOWS-MSVC-CLANG
