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
// SLATE-FILECHECK-DEFINES AARCH64-LINUX-GNU-GCC

// SLATE-FILECHECK-BEGIN AARCH64-LINUX-GNU-GCC
// AARCH64-LINUX-GNU-GCC: module {
// AARCH64-LINUX-GNU-GCC-NEXT:     target "aarch64-unknown-linux-gnu" {
// AARCH64-LINUX-GNU-GCC-NEXT:         endian = little;
// AARCH64-LINUX-GNU-GCC-NEXT:         pointer [size=8, align=8];
// AARCH64-LINUX-GNU-GCC-NEXT:         stack_alignment = 16;
// AARCH64-LINUX-GNU-GCC-NEXT:         long_double = f128;
// AARCH64-LINUX-GNU-GCC-NEXT:         storage bool [size=1, align=1];
// AARCH64-LINUX-GNU-GCC-NEXT:         storage i8, u8 [size=1, align=1];
// AARCH64-LINUX-GNU-GCC-NEXT:         storage i16, u16 [size=2, align=2];
// AARCH64-LINUX-GNU-GCC-NEXT:         storage i32, u32 [size=4, align=4];
// AARCH64-LINUX-GNU-GCC-NEXT:         storage i64, u64 [size=8, align=8];
// AARCH64-LINUX-GNU-GCC-NEXT:         storage i128, u128 [size=16, align=16];
// AARCH64-LINUX-GNU-GCC-NEXT:         storage bf16 [size=2, align=2];
// AARCH64-LINUX-GNU-GCC-NEXT:         storage f16 [size=2, align=2];
// AARCH64-LINUX-GNU-GCC-NEXT:         storage f32 [size=4, align=4];
// AARCH64-LINUX-GNU-GCC-NEXT:         storage f64 [size=8, align=8];
// AARCH64-LINUX-GNU-GCC-NEXT:         storage f128 [size=16, align=16];
// AARCH64-LINUX-GNU-GCC-NEXT:         storage d32 [size=4, align=4];
// AARCH64-LINUX-GNU-GCC-NEXT:         storage d64 [size=8, align=8];
// AARCH64-LINUX-GNU-GCC-NEXT:         storage d128 [size=16, align=16];
// AARCH64-LINUX-GNU-GCC-NEXT:     }
// AARCH64-LINUX-GNU-GCC-NEXT:     type @type0 pair = struct {
// AARCH64-LINUX-GNU-GCC-NEXT:         field0 a: i32;
// AARCH64-LINUX-GNU-GCC-NEXT:         field1 b: i32;
// AARCH64-LINUX-GNU-GCC-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// AARCH64-LINUX-GNU-GCC-NEXT:     global %0 gnuc: i32 [storage=static] = const<i32>(16) [linkage=external];
// AARCH64-LINUX-GNU-GCC-NEXT:     global %1 sizeof_long: u64 [storage=static] = const<u64>(8) [linkage=external];
// AARCH64-LINUX-GNU-GCC-NEXT:     global %2 sizeof_long_double: u64 [storage=static] = const<u64>(16) [linkage=external];
// AARCH64-LINUX-GNU-GCC-NEXT:     global %3 sizeof_va_list: u64 [storage=static] = const<u64>(32) [linkage=external];
// AARCH64-LINUX-GNU-GCC-NEXT:     global %4 alignof_long_long: u64 [storage=static] = const<u64>(8) [linkage=external];
// AARCH64-LINUX-GNU-GCC-NEXT:     fn %6 @record(%7 value: @type0) -> @type0 [linkage=external] [abi=aapcs64(native_c) -> native_c] [fallthrough=ub_if_used] {
// AARCH64-LINUX-GNU-GCC-NEXT:         return copy<@type0, reason=return>(read<@type0>(%7));
// AARCH64-LINUX-GNU-GCC-NEXT:     }
// AARCH64-LINUX-GNU-GCC-NEXT: }
// SLATE-FILECHECK-END AARCH64-LINUX-GNU-GCC
