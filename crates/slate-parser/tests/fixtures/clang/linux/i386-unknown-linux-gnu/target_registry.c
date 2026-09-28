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
// SLATE-FILECHECK-DEFINES I386-LINUX-GNU-CLANG

// SLATE-FILECHECK-BEGIN I386-LINUX-GNU-CLANG
// I386-LINUX-GNU-CLANG: module {
// I386-LINUX-GNU-CLANG-NEXT:     target "i386-unknown-linux-gnu" {
// I386-LINUX-GNU-CLANG-NEXT:         endian = little;
// I386-LINUX-GNU-CLANG-NEXT:         pointer [size=4, align=4];
// I386-LINUX-GNU-CLANG-NEXT:         stack_alignment = 16;
// I386-LINUX-GNU-CLANG-NEXT:         long_double = f80;
// I386-LINUX-GNU-CLANG-NEXT:         storage bool [size=1, align=1];
// I386-LINUX-GNU-CLANG-NEXT:         storage i8, u8 [size=1, align=1];
// I386-LINUX-GNU-CLANG-NEXT:         storage i16, u16 [size=2, align=2];
// I386-LINUX-GNU-CLANG-NEXT:         storage i32, u32 [size=4, align=4];
// I386-LINUX-GNU-CLANG-NEXT:         storage i64, u64 [size=8, align=4];
// I386-LINUX-GNU-CLANG-NEXT:         storage i128, u128 [size=16, align=16];
// I386-LINUX-GNU-CLANG-NEXT:         storage bf16 [size=2, align=2];
// I386-LINUX-GNU-CLANG-NEXT:         storage f16 [size=2, align=2];
// I386-LINUX-GNU-CLANG-NEXT:         storage f32 [size=4, align=4];
// I386-LINUX-GNU-CLANG-NEXT:         storage f64 [size=8, align=4];
// I386-LINUX-GNU-CLANG-NEXT:         storage f80 [size=12, align=4];
// I386-LINUX-GNU-CLANG-NEXT:         storage f128 [size=16, align=16];
// I386-LINUX-GNU-CLANG-NEXT:         storage d32 [size=4, align=4];
// I386-LINUX-GNU-CLANG-NEXT:         storage d64 [size=8, align=8];
// I386-LINUX-GNU-CLANG-NEXT:         storage d128 [size=16, align=16];
// I386-LINUX-GNU-CLANG-NEXT:     }
// I386-LINUX-GNU-CLANG-NEXT:     type @type0 pair = struct {
// I386-LINUX-GNU-CLANG-NEXT:         field0 a: i32;
// I386-LINUX-GNU-CLANG-NEXT:         field1 b: i32;
// I386-LINUX-GNU-CLANG-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// I386-LINUX-GNU-CLANG-NEXT:     global %0 clang_major: i32 [storage=static] = const<i32>(22) [linkage=external];
// I386-LINUX-GNU-CLANG-NEXT:     global %1 sizeof_long: u32 [storage=static] = const<u32>(4) [linkage=external];
// I386-LINUX-GNU-CLANG-NEXT:     global %2 sizeof_long_double: u32 [storage=static] = const<u32>(12) [linkage=external];
// I386-LINUX-GNU-CLANG-NEXT:     global %3 sizeof_va_list: u32 [storage=static] = const<u32>(4) [linkage=external];
// I386-LINUX-GNU-CLANG-NEXT:     global %4 alignof_long_long: u32 [storage=static] = const<u32>(4) [linkage=external];
// I386-LINUX-GNU-CLANG-NEXT:     fn %6 @record(%7 value: @type0) -> @type0 [linkage=external] [abi=x86_cdecl(coerce<i32, i32>) -> sret<align=4>] [fallthrough=ub_if_used] {
// I386-LINUX-GNU-CLANG-NEXT:         return copy<@type0, reason=return>(read<@type0>(%7));
// I386-LINUX-GNU-CLANG-NEXT:     }
// I386-LINUX-GNU-CLANG-NEXT: }
// SLATE-FILECHECK-END I386-LINUX-GNU-CLANG
