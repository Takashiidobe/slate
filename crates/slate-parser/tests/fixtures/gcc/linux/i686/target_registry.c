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
// SLATE-FILECHECK-DEFINES I686-LINUX-GNU-GCC

// SLATE-FILECHECK-BEGIN I686-LINUX-GNU-GCC
// I686-LINUX-GNU-GCC: module {
// I686-LINUX-GNU-GCC-NEXT:     target "i686-unknown-linux-gnu" {
// I686-LINUX-GNU-GCC-NEXT:         endian = little;
// I686-LINUX-GNU-GCC-NEXT:         pointer [size=4, align=4];
// I686-LINUX-GNU-GCC-NEXT:         stack_alignment = 16;
// I686-LINUX-GNU-GCC-NEXT:         long_double = f80;
// I686-LINUX-GNU-GCC-NEXT:         storage bool [size=1, align=1];
// I686-LINUX-GNU-GCC-NEXT:         storage i8, u8 [size=1, align=1];
// I686-LINUX-GNU-GCC-NEXT:         storage i16, u16 [size=2, align=2];
// I686-LINUX-GNU-GCC-NEXT:         storage i32, u32 [size=4, align=4];
// I686-LINUX-GNU-GCC-NEXT:         storage i64, u64 [size=8, align=4];
// I686-LINUX-GNU-GCC-NEXT:         storage i128, u128 [size=16, align=16];
// I686-LINUX-GNU-GCC-NEXT:         storage bf16 [size=2, align=2];
// I686-LINUX-GNU-GCC-NEXT:         storage f16 [size=2, align=2];
// I686-LINUX-GNU-GCC-NEXT:         storage f32 [size=4, align=4];
// I686-LINUX-GNU-GCC-NEXT:         storage f64 [size=8, align=4];
// I686-LINUX-GNU-GCC-NEXT:         storage f80 [size=12, align=4];
// I686-LINUX-GNU-GCC-NEXT:         storage f128 [size=16, align=16];
// I686-LINUX-GNU-GCC-NEXT:         storage d32 [size=4, align=4];
// I686-LINUX-GNU-GCC-NEXT:         storage d64 [size=8, align=8];
// I686-LINUX-GNU-GCC-NEXT:         storage d128 [size=16, align=16];
// I686-LINUX-GNU-GCC-NEXT:     }
// I686-LINUX-GNU-GCC-NEXT:     type @type0 pair = struct {
// I686-LINUX-GNU-GCC-NEXT:         field0 a: i32;
// I686-LINUX-GNU-GCC-NEXT:         field1 b: i32;
// I686-LINUX-GNU-GCC-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// I686-LINUX-GNU-GCC-NEXT:     global %0 gnuc: i32 [storage=static] = const<i32>(16) [linkage=external];
// I686-LINUX-GNU-GCC-NEXT:     global %1 sizeof_long: u32 [storage=static] = const<u32>(4) [linkage=external];
// I686-LINUX-GNU-GCC-NEXT:     global %2 sizeof_long_double: u32 [storage=static] = const<u32>(12) [linkage=external];
// I686-LINUX-GNU-GCC-NEXT:     global %3 sizeof_va_list: u32 [storage=static] = const<u32>(4) [linkage=external];
// I686-LINUX-GNU-GCC-NEXT:     global %4 alignof_long_long: u32 [storage=static] = const<u32>(4) [linkage=external];
// I686-LINUX-GNU-GCC-NEXT:     fn %6 @record(%7 value: @type0) -> @type0 [linkage=external] [abi=x86_cdecl(coerce<i32, i32>) -> sret<align=4>] [fallthrough=ub_if_used] {
// I686-LINUX-GNU-GCC-NEXT:         return copy<@type0, reason=return>(read<@type0>(%7));
// I686-LINUX-GNU-GCC-NEXT:     }
// I686-LINUX-GNU-GCC-NEXT: }
// SLATE-FILECHECK-END I686-LINUX-GNU-GCC
