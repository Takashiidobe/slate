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
// SLATE-FILECHECK-DEFINES AARCH64-DARWIN-CLANG

// SLATE-FILECHECK-BEGIN AARCH64-DARWIN-CLANG
// AARCH64-DARWIN-CLANG: module {
// AARCH64-DARWIN-CLANG-NEXT:     target "aarch64-apple-darwin" {
// AARCH64-DARWIN-CLANG-NEXT:         endian = little;
// AARCH64-DARWIN-CLANG-NEXT:         pointer [size=8, align=8];
// AARCH64-DARWIN-CLANG-NEXT:         stack_alignment = 16;
// AARCH64-DARWIN-CLANG-NEXT:         long_double = f64;
// AARCH64-DARWIN-CLANG-NEXT:         storage bool [size=1, align=1];
// AARCH64-DARWIN-CLANG-NEXT:         storage i8, u8 [size=1, align=1];
// AARCH64-DARWIN-CLANG-NEXT:         storage i16, u16 [size=2, align=2];
// AARCH64-DARWIN-CLANG-NEXT:         storage i32, u32 [size=4, align=4];
// AARCH64-DARWIN-CLANG-NEXT:         storage i64, u64 [size=8, align=8];
// AARCH64-DARWIN-CLANG-NEXT:         storage i128, u128 [size=16, align=16];
// AARCH64-DARWIN-CLANG-NEXT:         storage bf16 [size=2, align=2];
// AARCH64-DARWIN-CLANG-NEXT:         storage f16 [size=2, align=2];
// AARCH64-DARWIN-CLANG-NEXT:         storage f32 [size=4, align=4];
// AARCH64-DARWIN-CLANG-NEXT:         storage f64 [size=8, align=8];
// AARCH64-DARWIN-CLANG-NEXT:         storage f128 [size=16, align=16];
// AARCH64-DARWIN-CLANG-NEXT:         storage d32 [size=4, align=4];
// AARCH64-DARWIN-CLANG-NEXT:         storage d64 [size=8, align=8];
// AARCH64-DARWIN-CLANG-NEXT:         storage d128 [size=16, align=16];
// AARCH64-DARWIN-CLANG-NEXT:     }
// AARCH64-DARWIN-CLANG-NEXT:     type @type[[TYPE_pair:[0-9]+]] pair = struct {
// AARCH64-DARWIN-CLANG-NEXT:         field0 a: i32;
// AARCH64-DARWIN-CLANG-NEXT:         field1 b: i32;
// AARCH64-DARWIN-CLANG-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// AARCH64-DARWIN-CLANG-NEXT:     global %[[VALUE_clang_major:[0-9]+]] clang_major: i32 [storage=static] = const<i32>(22) [linkage=external];
// AARCH64-DARWIN-CLANG-NEXT:     global %[[VALUE_sizeof_long:[0-9]+]] sizeof_long: u64 [storage=static] = const<u64>(8) [linkage=external];
// AARCH64-DARWIN-CLANG-NEXT:     global %[[VALUE_sizeof_long_double:[0-9]+]] sizeof_long_double: u64 [storage=static] = const<u64>(8) [linkage=external];
// AARCH64-DARWIN-CLANG-NEXT:     global %[[VALUE_sizeof_va_list:[0-9]+]] sizeof_va_list: u64 [storage=static] = const<u64>(8) [linkage=external];
// AARCH64-DARWIN-CLANG-NEXT:     global %[[VALUE_alignof_long_long:[0-9]+]] alignof_long_long: u64 [storage=static] = const<u64>(8) [linkage=external];
// AARCH64-DARWIN-CLANG-NEXT:     fn %[[VALUE_record:[0-9]+]] @record(%[[VALUE_value:[0-9]+]] value: @type[[TYPE_pair]]) -> @type[[TYPE_pair]] [linkage=external] [abi=aapcs64(native_c) -> native_c] [fallthrough=ub_if_used] {
// AARCH64-DARWIN-CLANG-NEXT:         return copy<@type[[TYPE_pair]], reason=return>(read<@type[[TYPE_pair]]>(%[[VALUE_value]]));
// AARCH64-DARWIN-CLANG-NEXT:     }
// AARCH64-DARWIN-CLANG-NEXT: }
// SLATE-FILECHECK-END AARCH64-DARWIN-CLANG
