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
// SLATE-FILECHECK-DEFINES X86-64-WINDOWS-MSVC-MSVC

// SLATE-FILECHECK-BEGIN X86-64-WINDOWS-MSVC-MSVC
// X86-64-WINDOWS-MSVC-MSVC: module {
// X86-64-WINDOWS-MSVC-MSVC-NEXT:     target "x86_64-pc-windows-msvc" {
// X86-64-WINDOWS-MSVC-MSVC-NEXT:         endian = little;
// X86-64-WINDOWS-MSVC-MSVC-NEXT:         pointer [size=8, align=8];
// X86-64-WINDOWS-MSVC-MSVC-NEXT:         stack_alignment = 16;
// X86-64-WINDOWS-MSVC-MSVC-NEXT:         long_double = f64;
// X86-64-WINDOWS-MSVC-MSVC-NEXT:         storage bool [size=1, align=1];
// X86-64-WINDOWS-MSVC-MSVC-NEXT:         storage i8, u8 [size=1, align=1];
// X86-64-WINDOWS-MSVC-MSVC-NEXT:         storage i16, u16 [size=2, align=2];
// X86-64-WINDOWS-MSVC-MSVC-NEXT:         storage i32, u32 [size=4, align=4];
// X86-64-WINDOWS-MSVC-MSVC-NEXT:         storage i64, u64 [size=8, align=8];
// X86-64-WINDOWS-MSVC-MSVC-NEXT:         storage i128, u128 [size=16, align=16];
// X86-64-WINDOWS-MSVC-MSVC-NEXT:         storage bf16 [size=2, align=2];
// X86-64-WINDOWS-MSVC-MSVC-NEXT:         storage f16 [size=2, align=2];
// X86-64-WINDOWS-MSVC-MSVC-NEXT:         storage f32 [size=4, align=4];
// X86-64-WINDOWS-MSVC-MSVC-NEXT:         storage f64 [size=8, align=8];
// X86-64-WINDOWS-MSVC-MSVC-NEXT:         storage f128 [size=16, align=16];
// X86-64-WINDOWS-MSVC-MSVC-NEXT:         storage d32 [size=4, align=4];
// X86-64-WINDOWS-MSVC-MSVC-NEXT:         storage d64 [size=8, align=8];
// X86-64-WINDOWS-MSVC-MSVC-NEXT:         storage d128 [size=16, align=16];
// X86-64-WINDOWS-MSVC-MSVC-NEXT:     }
// X86-64-WINDOWS-MSVC-MSVC-NEXT:     type @type0 pair = struct {
// X86-64-WINDOWS-MSVC-MSVC-NEXT:         field0 a: i32;
// X86-64-WINDOWS-MSVC-MSVC-NEXT:         field1 b: i32;
// X86-64-WINDOWS-MSVC-MSVC-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// X86-64-WINDOWS-MSVC-MSVC-NEXT:     global %0 msc_ver: i32 [storage=static] = const<i32>(1951) [linkage=external];
// X86-64-WINDOWS-MSVC-MSVC-NEXT:     global %1 sizeof_long: u32 [storage=static] = truncate<u32>(const<u64>(4)) [linkage=external];
// X86-64-WINDOWS-MSVC-MSVC-NEXT:     global %2 sizeof_long_double: u32 [storage=static] = truncate<u32>(const<u64>(8)) [linkage=external];
// X86-64-WINDOWS-MSVC-MSVC-NEXT:     global %3 sizeof_va_list: u32 [storage=static] = truncate<u32>(const<u64>(8)) [linkage=external];
// X86-64-WINDOWS-MSVC-MSVC-NEXT:     global %4 alignof_long_long: u32 [storage=static] = truncate<u32>(const<u64>(8)) [linkage=external];
// X86-64-WINDOWS-MSVC-MSVC-NEXT:     fn %6 @record(%7 value: @type0) -> @type0 [linkage=external] [abi=win64(native_c) -> native_c] [fallthrough=ub_if_used] {
// X86-64-WINDOWS-MSVC-MSVC-NEXT:         return copy<@type0, reason=return>(read<@type0>(%7));
// X86-64-WINDOWS-MSVC-MSVC-NEXT:     }
// X86-64-WINDOWS-MSVC-MSVC-NEXT: }
// SLATE-FILECHECK-END X86-64-WINDOWS-MSVC-MSVC
