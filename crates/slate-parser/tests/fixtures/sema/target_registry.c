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
// SLATE-FILECHECK-DEFINES X86-64-LINUX-GNU-GCC
// SLATE-FILECHECK-PREFIX-ARGS X86-64-LINUX-GNU-GCC --target=x86_64-unknown-linux-gnu --flavor=gcc
// SLATE-FILECHECK-DEFINES X86-64-LINUX-GNU-CLANG
// SLATE-FILECHECK-PREFIX-ARGS X86-64-LINUX-GNU-CLANG --target=x86_64-unknown-linux-gnu --flavor=clang
// SLATE-FILECHECK-DEFINES X86-64-LINUX-GNU-MSVC
// SLATE-FILECHECK-PREFIX-ARGS X86-64-LINUX-GNU-MSVC --target=x86_64-unknown-linux-gnu --flavor=msvc
// SLATE-FILECHECK-DEFINES I386-LINUX-GNU-GCC
// SLATE-FILECHECK-PREFIX-ARGS I386-LINUX-GNU-GCC --target=i386-unknown-linux-gnu --flavor=gcc
// SLATE-FILECHECK-DEFINES I386-LINUX-GNU-CLANG
// SLATE-FILECHECK-PREFIX-ARGS I386-LINUX-GNU-CLANG --target=i386-unknown-linux-gnu --flavor=clang
// SLATE-FILECHECK-DEFINES I386-LINUX-GNU-MSVC
// SLATE-FILECHECK-PREFIX-ARGS I386-LINUX-GNU-MSVC --target=i386-unknown-linux-gnu --flavor=msvc
// SLATE-FILECHECK-DEFINES I686-LINUX-GNU-GCC
// SLATE-FILECHECK-PREFIX-ARGS I686-LINUX-GNU-GCC --target=i686-unknown-linux-gnu --flavor=gcc
// SLATE-FILECHECK-DEFINES I686-LINUX-GNU-CLANG
// SLATE-FILECHECK-PREFIX-ARGS I686-LINUX-GNU-CLANG --target=i686-unknown-linux-gnu --flavor=clang
// SLATE-FILECHECK-DEFINES I686-LINUX-GNU-MSVC
// SLATE-FILECHECK-PREFIX-ARGS I686-LINUX-GNU-MSVC --target=i686-unknown-linux-gnu --flavor=msvc
// SLATE-FILECHECK-DEFINES AARCH64-LINUX-GNU-GCC
// SLATE-FILECHECK-PREFIX-ARGS AARCH64-LINUX-GNU-GCC --target=aarch64-unknown-linux-gnu --flavor=gcc
// SLATE-FILECHECK-DEFINES AARCH64-LINUX-GNU-CLANG
// SLATE-FILECHECK-PREFIX-ARGS AARCH64-LINUX-GNU-CLANG --target=aarch64-unknown-linux-gnu --flavor=clang
// SLATE-FILECHECK-DEFINES AARCH64-LINUX-GNU-MSVC
// SLATE-FILECHECK-PREFIX-ARGS AARCH64-LINUX-GNU-MSVC --target=aarch64-unknown-linux-gnu --flavor=msvc
// SLATE-FILECHECK-DEFINES ARMV7-LINUX-GNUEABI-GCC
// SLATE-FILECHECK-PREFIX-ARGS ARMV7-LINUX-GNUEABI-GCC --target=armv7-unknown-linux-gnueabi --flavor=gcc
// SLATE-FILECHECK-DEFINES ARMV7-LINUX-GNUEABI-CLANG
// SLATE-FILECHECK-PREFIX-ARGS ARMV7-LINUX-GNUEABI-CLANG --target=armv7-unknown-linux-gnueabi --flavor=clang
// SLATE-FILECHECK-DEFINES ARMV7-LINUX-GNUEABI-MSVC
// SLATE-FILECHECK-PREFIX-ARGS ARMV7-LINUX-GNUEABI-MSVC --target=armv7-unknown-linux-gnueabi --flavor=msvc
// SLATE-FILECHECK-DEFINES ARMV7-LINUX-GNUEABIHF-GCC
// SLATE-FILECHECK-PREFIX-ARGS ARMV7-LINUX-GNUEABIHF-GCC --target=armv7-unknown-linux-gnueabihf --flavor=gcc
// SLATE-FILECHECK-DEFINES ARMV7-LINUX-GNUEABIHF-CLANG
// SLATE-FILECHECK-PREFIX-ARGS ARMV7-LINUX-GNUEABIHF-CLANG --target=armv7-unknown-linux-gnueabihf --flavor=clang
// SLATE-FILECHECK-DEFINES ARMV7-LINUX-GNUEABIHF-MSVC
// SLATE-FILECHECK-PREFIX-ARGS ARMV7-LINUX-GNUEABIHF-MSVC --target=armv7-unknown-linux-gnueabihf --flavor=msvc
// SLATE-FILECHECK-DEFINES AARCH64-DARWIN-CLANG
// SLATE-FILECHECK-PREFIX-ARGS AARCH64-DARWIN-CLANG --target=aarch64-apple-darwin --flavor=clang
// SLATE-FILECHECK-DEFINES X86-64-DARWIN-CLANG
// SLATE-FILECHECK-PREFIX-ARGS X86-64-DARWIN-CLANG --target=x86_64-apple-darwin --flavor=clang
// SLATE-FILECHECK-DEFINES AARCH64-ANDROID-CLANG
// SLATE-FILECHECK-PREFIX-ARGS AARCH64-ANDROID-CLANG --target=aarch64-linux-android --flavor=clang
// SLATE-FILECHECK-DEFINES X86-64-ANDROID-CLANG
// SLATE-FILECHECK-PREFIX-ARGS X86-64-ANDROID-CLANG --target=x86_64-linux-android --flavor=clang
// SLATE-FILECHECK-DEFINES AARCH64-FREEBSD-CLANG
// SLATE-FILECHECK-PREFIX-ARGS AARCH64-FREEBSD-CLANG --target=aarch64-unknown-freebsd --flavor=clang
// SLATE-FILECHECK-DEFINES X86-64-FREEBSD-CLANG
// SLATE-FILECHECK-PREFIX-ARGS X86-64-FREEBSD-CLANG --target=x86_64-unknown-freebsd --flavor=clang
// SLATE-FILECHECK-DEFINES X86-64-WINDOWS-MSVC-CLANG
// SLATE-FILECHECK-PREFIX-ARGS X86-64-WINDOWS-MSVC-CLANG --target=x86_64-pc-windows-msvc --flavor=clang
// SLATE-FILECHECK-DEFINES X86-64-WINDOWS-MSVC-MSVC
// SLATE-FILECHECK-PREFIX-ARGS X86-64-WINDOWS-MSVC-MSVC --target=x86_64-pc-windows-msvc --flavor=msvc
// SLATE-FILECHECK-DEFINES AARCH64-WINDOWS-MSVC-CLANG
// SLATE-FILECHECK-PREFIX-ARGS AARCH64-WINDOWS-MSVC-CLANG --target=aarch64-pc-windows-msvc --flavor=clang
// SLATE-FILECHECK-DEFINES AARCH64-WINDOWS-MSVC-MSVC
// SLATE-FILECHECK-PREFIX-ARGS AARCH64-WINDOWS-MSVC-MSVC --target=aarch64-pc-windows-msvc --flavor=msvc

// SLATE-FILECHECK-BEGIN X86-64-LINUX-GNU-GCC
// X86-64-LINUX-GNU-GCC: module {
// X86-64-LINUX-GNU-GCC-NEXT:     target "x86_64-unknown-linux-gnu" {
// X86-64-LINUX-GNU-GCC-NEXT:         endian = little;
// X86-64-LINUX-GNU-GCC-NEXT:         pointer [size=8, align=8];
// X86-64-LINUX-GNU-GCC-NEXT:         stack_alignment = 16;
// X86-64-LINUX-GNU-GCC-NEXT:         long_double = f80;
// X86-64-LINUX-GNU-GCC-NEXT:         storage bool [size=1, align=1];
// X86-64-LINUX-GNU-GCC-NEXT:         storage i8, u8 [size=1, align=1];
// X86-64-LINUX-GNU-GCC-NEXT:         storage i16, u16 [size=2, align=2];
// X86-64-LINUX-GNU-GCC-NEXT:         storage i32, u32 [size=4, align=4];
// X86-64-LINUX-GNU-GCC-NEXT:         storage i64, u64 [size=8, align=8];
// X86-64-LINUX-GNU-GCC-NEXT:         storage i128, u128 [size=16, align=16];
// X86-64-LINUX-GNU-GCC-NEXT:         storage bf16 [size=2, align=2];
// X86-64-LINUX-GNU-GCC-NEXT:         storage f16 [size=2, align=2];
// X86-64-LINUX-GNU-GCC-NEXT:         storage f32 [size=4, align=4];
// X86-64-LINUX-GNU-GCC-NEXT:         storage f64 [size=8, align=8];
// X86-64-LINUX-GNU-GCC-NEXT:         storage f80 [size=16, align=16];
// X86-64-LINUX-GNU-GCC-NEXT:         storage f128 [size=16, align=16];
// X86-64-LINUX-GNU-GCC-NEXT:         storage d32 [size=4, align=4];
// X86-64-LINUX-GNU-GCC-NEXT:         storage d64 [size=8, align=8];
// X86-64-LINUX-GNU-GCC-NEXT:         storage d128 [size=16, align=16];
// X86-64-LINUX-GNU-GCC-NEXT:     }
// X86-64-LINUX-GNU-GCC-NEXT:     type @type0 pair = struct {
// X86-64-LINUX-GNU-GCC-NEXT:         field0 a: i32;
// X86-64-LINUX-GNU-GCC-NEXT:         field1 b: i32;
// X86-64-LINUX-GNU-GCC-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// X86-64-LINUX-GNU-GCC-NEXT:     global %0 gnuc: i32 [storage=static] = const<i32>(16) [linkage=external];
// X86-64-LINUX-GNU-GCC-NEXT:     global %1 sizeof_long: u64 [storage=static] = const<u64>(8) [linkage=external];
// X86-64-LINUX-GNU-GCC-NEXT:     global %2 sizeof_long_double: u64 [storage=static] = const<u64>(16) [linkage=external];
// X86-64-LINUX-GNU-GCC-NEXT:     global %3 sizeof_va_list: u64 [storage=static] = const<u64>(24) [linkage=external];
// X86-64-LINUX-GNU-GCC-NEXT:     global %4 alignof_long_long: u64 [storage=static] = const<u64>(8) [linkage=external];
// X86-64-LINUX-GNU-GCC-NEXT:     fn %6 @record(%7 value: @type0) -> @type0 [linkage=external] [abi=sysv64(coerce<i64>) -> coerce<i64>] [fallthrough=ub_if_used] {
// X86-64-LINUX-GNU-GCC-NEXT:         return copy<@type0, reason=return>(read<@type0>(%7));
// X86-64-LINUX-GNU-GCC-NEXT:     }
// X86-64-LINUX-GNU-GCC-NEXT: }
// SLATE-FILECHECK-END X86-64-LINUX-GNU-GCC
// SLATE-FILECHECK-BEGIN X86-64-LINUX-GNU-CLANG
// X86-64-LINUX-GNU-CLANG: module {
// X86-64-LINUX-GNU-CLANG-NEXT:     target "x86_64-unknown-linux-gnu" {
// X86-64-LINUX-GNU-CLANG-NEXT:         endian = little;
// X86-64-LINUX-GNU-CLANG-NEXT:         pointer [size=8, align=8];
// X86-64-LINUX-GNU-CLANG-NEXT:         stack_alignment = 16;
// X86-64-LINUX-GNU-CLANG-NEXT:         long_double = f80;
// X86-64-LINUX-GNU-CLANG-NEXT:         storage bool [size=1, align=1];
// X86-64-LINUX-GNU-CLANG-NEXT:         storage i8, u8 [size=1, align=1];
// X86-64-LINUX-GNU-CLANG-NEXT:         storage i16, u16 [size=2, align=2];
// X86-64-LINUX-GNU-CLANG-NEXT:         storage i32, u32 [size=4, align=4];
// X86-64-LINUX-GNU-CLANG-NEXT:         storage i64, u64 [size=8, align=8];
// X86-64-LINUX-GNU-CLANG-NEXT:         storage i128, u128 [size=16, align=16];
// X86-64-LINUX-GNU-CLANG-NEXT:         storage bf16 [size=2, align=2];
// X86-64-LINUX-GNU-CLANG-NEXT:         storage f16 [size=2, align=2];
// X86-64-LINUX-GNU-CLANG-NEXT:         storage f32 [size=4, align=4];
// X86-64-LINUX-GNU-CLANG-NEXT:         storage f64 [size=8, align=8];
// X86-64-LINUX-GNU-CLANG-NEXT:         storage f80 [size=16, align=16];
// X86-64-LINUX-GNU-CLANG-NEXT:         storage f128 [size=16, align=16];
// X86-64-LINUX-GNU-CLANG-NEXT:         storage d32 [size=4, align=4];
// X86-64-LINUX-GNU-CLANG-NEXT:         storage d64 [size=8, align=8];
// X86-64-LINUX-GNU-CLANG-NEXT:         storage d128 [size=16, align=16];
// X86-64-LINUX-GNU-CLANG-NEXT:     }
// X86-64-LINUX-GNU-CLANG-NEXT:     type @type0 pair = struct {
// X86-64-LINUX-GNU-CLANG-NEXT:         field0 a: i32;
// X86-64-LINUX-GNU-CLANG-NEXT:         field1 b: i32;
// X86-64-LINUX-GNU-CLANG-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// X86-64-LINUX-GNU-CLANG-NEXT:     global %0 clang_major: i32 [storage=static] = const<i32>(22) [linkage=external];
// X86-64-LINUX-GNU-CLANG-NEXT:     global %1 sizeof_long: u64 [storage=static] = const<u64>(8) [linkage=external];
// X86-64-LINUX-GNU-CLANG-NEXT:     global %2 sizeof_long_double: u64 [storage=static] = const<u64>(16) [linkage=external];
// X86-64-LINUX-GNU-CLANG-NEXT:     global %3 sizeof_va_list: u64 [storage=static] = const<u64>(24) [linkage=external];
// X86-64-LINUX-GNU-CLANG-NEXT:     global %4 alignof_long_long: u64 [storage=static] = const<u64>(8) [linkage=external];
// X86-64-LINUX-GNU-CLANG-NEXT:     fn %6 @record(%7 value: @type0) -> @type0 [linkage=external] [abi=sysv64(coerce<i64>) -> coerce<i64>] [fallthrough=ub_if_used] {
// X86-64-LINUX-GNU-CLANG-NEXT:         return copy<@type0, reason=return>(read<@type0>(%7));
// X86-64-LINUX-GNU-CLANG-NEXT:     }
// X86-64-LINUX-GNU-CLANG-NEXT: }
// SLATE-FILECHECK-END X86-64-LINUX-GNU-CLANG
// SLATE-FILECHECK-BEGIN X86-64-LINUX-GNU-MSVC
// X86-64-LINUX-GNU-MSVC: module {
// X86-64-LINUX-GNU-MSVC-NEXT:     target "x86_64-unknown-linux-gnu" {
// X86-64-LINUX-GNU-MSVC-NEXT:         endian = little;
// X86-64-LINUX-GNU-MSVC-NEXT:         pointer [size=8, align=8];
// X86-64-LINUX-GNU-MSVC-NEXT:         stack_alignment = 16;
// X86-64-LINUX-GNU-MSVC-NEXT:         long_double = f80;
// X86-64-LINUX-GNU-MSVC-NEXT:         storage bool [size=1, align=1];
// X86-64-LINUX-GNU-MSVC-NEXT:         storage i8, u8 [size=1, align=1];
// X86-64-LINUX-GNU-MSVC-NEXT:         storage i16, u16 [size=2, align=2];
// X86-64-LINUX-GNU-MSVC-NEXT:         storage i32, u32 [size=4, align=4];
// X86-64-LINUX-GNU-MSVC-NEXT:         storage i64, u64 [size=8, align=8];
// X86-64-LINUX-GNU-MSVC-NEXT:         storage i128, u128 [size=16, align=16];
// X86-64-LINUX-GNU-MSVC-NEXT:         storage bf16 [size=2, align=2];
// X86-64-LINUX-GNU-MSVC-NEXT:         storage f16 [size=2, align=2];
// X86-64-LINUX-GNU-MSVC-NEXT:         storage f32 [size=4, align=4];
// X86-64-LINUX-GNU-MSVC-NEXT:         storage f64 [size=8, align=8];
// X86-64-LINUX-GNU-MSVC-NEXT:         storage f80 [size=16, align=16];
// X86-64-LINUX-GNU-MSVC-NEXT:         storage f128 [size=16, align=16];
// X86-64-LINUX-GNU-MSVC-NEXT:         storage d32 [size=4, align=4];
// X86-64-LINUX-GNU-MSVC-NEXT:         storage d64 [size=8, align=8];
// X86-64-LINUX-GNU-MSVC-NEXT:         storage d128 [size=16, align=16];
// X86-64-LINUX-GNU-MSVC-NEXT:     }
// X86-64-LINUX-GNU-MSVC-NEXT:     type @type0 pair = struct {
// X86-64-LINUX-GNU-MSVC-NEXT:         field0 a: i32;
// X86-64-LINUX-GNU-MSVC-NEXT:         field1 b: i32;
// X86-64-LINUX-GNU-MSVC-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// X86-64-LINUX-GNU-MSVC-NEXT:     global %0 clang_major: i32 [storage=static] = const<i32>(22) [linkage=external];
// X86-64-LINUX-GNU-MSVC-NEXT:     global %1 sizeof_long: u64 [storage=static] = const<u64>(8) [linkage=external];
// X86-64-LINUX-GNU-MSVC-NEXT:     global %2 sizeof_long_double: u64 [storage=static] = const<u64>(16) [linkage=external];
// X86-64-LINUX-GNU-MSVC-NEXT:     global %3 sizeof_va_list: u64 [storage=static] = const<u64>(24) [linkage=external];
// X86-64-LINUX-GNU-MSVC-NEXT:     global %4 alignof_long_long: u64 [storage=static] = const<u64>(8) [linkage=external];
// X86-64-LINUX-GNU-MSVC-NEXT:     fn %6 @record(%7 value: @type0) -> @type0 [linkage=external] [abi=sysv64(coerce<i64>) -> coerce<i64>] [fallthrough=ub_if_used] {
// X86-64-LINUX-GNU-MSVC-NEXT:         return copy<@type0, reason=return>(read<@type0>(%7));
// X86-64-LINUX-GNU-MSVC-NEXT:     }
// X86-64-LINUX-GNU-MSVC-NEXT: }
// SLATE-FILECHECK-END X86-64-LINUX-GNU-MSVC
// SLATE-FILECHECK-BEGIN I386-LINUX-GNU-GCC
// I386-LINUX-GNU-GCC: module {
// I386-LINUX-GNU-GCC-NEXT:     target "i386-unknown-linux-gnu" {
// I386-LINUX-GNU-GCC-NEXT:         endian = little;
// I386-LINUX-GNU-GCC-NEXT:         pointer [size=4, align=4];
// I386-LINUX-GNU-GCC-NEXT:         stack_alignment = 16;
// I386-LINUX-GNU-GCC-NEXT:         long_double = f80;
// I386-LINUX-GNU-GCC-NEXT:         storage bool [size=1, align=1];
// I386-LINUX-GNU-GCC-NEXT:         storage i8, u8 [size=1, align=1];
// I386-LINUX-GNU-GCC-NEXT:         storage i16, u16 [size=2, align=2];
// I386-LINUX-GNU-GCC-NEXT:         storage i32, u32 [size=4, align=4];
// I386-LINUX-GNU-GCC-NEXT:         storage i64, u64 [size=8, align=4];
// I386-LINUX-GNU-GCC-NEXT:         storage i128, u128 [size=16, align=16];
// I386-LINUX-GNU-GCC-NEXT:         storage bf16 [size=2, align=2];
// I386-LINUX-GNU-GCC-NEXT:         storage f16 [size=2, align=2];
// I386-LINUX-GNU-GCC-NEXT:         storage f32 [size=4, align=4];
// I386-LINUX-GNU-GCC-NEXT:         storage f64 [size=8, align=4];
// I386-LINUX-GNU-GCC-NEXT:         storage f80 [size=12, align=4];
// I386-LINUX-GNU-GCC-NEXT:         storage f128 [size=16, align=16];
// I386-LINUX-GNU-GCC-NEXT:         storage d32 [size=4, align=4];
// I386-LINUX-GNU-GCC-NEXT:         storage d64 [size=8, align=8];
// I386-LINUX-GNU-GCC-NEXT:         storage d128 [size=16, align=16];
// I386-LINUX-GNU-GCC-NEXT:     }
// I386-LINUX-GNU-GCC-NEXT:     type @type0 pair = struct {
// I386-LINUX-GNU-GCC-NEXT:         field0 a: i32;
// I386-LINUX-GNU-GCC-NEXT:         field1 b: i32;
// I386-LINUX-GNU-GCC-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// I386-LINUX-GNU-GCC-NEXT:     global %0 gnuc: i32 [storage=static] = const<i32>(16) [linkage=external];
// I386-LINUX-GNU-GCC-NEXT:     global %1 sizeof_long: u32 [storage=static] = const<u32>(4) [linkage=external];
// I386-LINUX-GNU-GCC-NEXT:     global %2 sizeof_long_double: u32 [storage=static] = const<u32>(12) [linkage=external];
// I386-LINUX-GNU-GCC-NEXT:     global %3 sizeof_va_list: u32 [storage=static] = const<u32>(4) [linkage=external];
// I386-LINUX-GNU-GCC-NEXT:     global %4 alignof_long_long: u32 [storage=static] = const<u32>(4) [linkage=external];
// I386-LINUX-GNU-GCC-NEXT:     fn %6 @record(%7 value: @type0) -> @type0 [linkage=external] [abi=x86_cdecl(coerce<i32, i32>) -> sret<align=4>] [fallthrough=ub_if_used] {
// I386-LINUX-GNU-GCC-NEXT:         return copy<@type0, reason=return>(read<@type0>(%7));
// I386-LINUX-GNU-GCC-NEXT:     }
// I386-LINUX-GNU-GCC-NEXT: }
// SLATE-FILECHECK-END I386-LINUX-GNU-GCC
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
// SLATE-FILECHECK-BEGIN I386-LINUX-GNU-MSVC
// I386-LINUX-GNU-MSVC: module {
// I386-LINUX-GNU-MSVC-NEXT:     target "i386-unknown-linux-gnu" {
// I386-LINUX-GNU-MSVC-NEXT:         endian = little;
// I386-LINUX-GNU-MSVC-NEXT:         pointer [size=4, align=4];
// I386-LINUX-GNU-MSVC-NEXT:         stack_alignment = 16;
// I386-LINUX-GNU-MSVC-NEXT:         long_double = f80;
// I386-LINUX-GNU-MSVC-NEXT:         storage bool [size=1, align=1];
// I386-LINUX-GNU-MSVC-NEXT:         storage i8, u8 [size=1, align=1];
// I386-LINUX-GNU-MSVC-NEXT:         storage i16, u16 [size=2, align=2];
// I386-LINUX-GNU-MSVC-NEXT:         storage i32, u32 [size=4, align=4];
// I386-LINUX-GNU-MSVC-NEXT:         storage i64, u64 [size=8, align=4];
// I386-LINUX-GNU-MSVC-NEXT:         storage i128, u128 [size=16, align=16];
// I386-LINUX-GNU-MSVC-NEXT:         storage bf16 [size=2, align=2];
// I386-LINUX-GNU-MSVC-NEXT:         storage f16 [size=2, align=2];
// I386-LINUX-GNU-MSVC-NEXT:         storage f32 [size=4, align=4];
// I386-LINUX-GNU-MSVC-NEXT:         storage f64 [size=8, align=4];
// I386-LINUX-GNU-MSVC-NEXT:         storage f80 [size=12, align=4];
// I386-LINUX-GNU-MSVC-NEXT:         storage f128 [size=16, align=16];
// I386-LINUX-GNU-MSVC-NEXT:         storage d32 [size=4, align=4];
// I386-LINUX-GNU-MSVC-NEXT:         storage d64 [size=8, align=8];
// I386-LINUX-GNU-MSVC-NEXT:         storage d128 [size=16, align=16];
// I386-LINUX-GNU-MSVC-NEXT:     }
// I386-LINUX-GNU-MSVC-NEXT:     type @type0 pair = struct {
// I386-LINUX-GNU-MSVC-NEXT:         field0 a: i32;
// I386-LINUX-GNU-MSVC-NEXT:         field1 b: i32;
// I386-LINUX-GNU-MSVC-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// I386-LINUX-GNU-MSVC-NEXT:     global %0 clang_major: i32 [storage=static] = const<i32>(22) [linkage=external];
// I386-LINUX-GNU-MSVC-NEXT:     global %1 sizeof_long: u32 [storage=static] = const<u32>(4) [linkage=external];
// I386-LINUX-GNU-MSVC-NEXT:     global %2 sizeof_long_double: u32 [storage=static] = const<u32>(12) [linkage=external];
// I386-LINUX-GNU-MSVC-NEXT:     global %3 sizeof_va_list: u32 [storage=static] = const<u32>(4) [linkage=external];
// I386-LINUX-GNU-MSVC-NEXT:     global %4 alignof_long_long: u32 [storage=static] = const<u32>(4) [linkage=external];
// I386-LINUX-GNU-MSVC-NEXT:     fn %6 @record(%7 value: @type0) -> @type0 [linkage=external] [abi=x86_cdecl(coerce<i32, i32>) -> sret<align=4>] [fallthrough=ub_if_used] {
// I386-LINUX-GNU-MSVC-NEXT:         return copy<@type0, reason=return>(read<@type0>(%7));
// I386-LINUX-GNU-MSVC-NEXT:     }
// I386-LINUX-GNU-MSVC-NEXT: }
// SLATE-FILECHECK-END I386-LINUX-GNU-MSVC
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
// SLATE-FILECHECK-BEGIN I686-LINUX-GNU-CLANG
// I686-LINUX-GNU-CLANG: module {
// I686-LINUX-GNU-CLANG-NEXT:     target "i686-unknown-linux-gnu" {
// I686-LINUX-GNU-CLANG-NEXT:         endian = little;
// I686-LINUX-GNU-CLANG-NEXT:         pointer [size=4, align=4];
// I686-LINUX-GNU-CLANG-NEXT:         stack_alignment = 16;
// I686-LINUX-GNU-CLANG-NEXT:         long_double = f80;
// I686-LINUX-GNU-CLANG-NEXT:         storage bool [size=1, align=1];
// I686-LINUX-GNU-CLANG-NEXT:         storage i8, u8 [size=1, align=1];
// I686-LINUX-GNU-CLANG-NEXT:         storage i16, u16 [size=2, align=2];
// I686-LINUX-GNU-CLANG-NEXT:         storage i32, u32 [size=4, align=4];
// I686-LINUX-GNU-CLANG-NEXT:         storage i64, u64 [size=8, align=4];
// I686-LINUX-GNU-CLANG-NEXT:         storage i128, u128 [size=16, align=16];
// I686-LINUX-GNU-CLANG-NEXT:         storage bf16 [size=2, align=2];
// I686-LINUX-GNU-CLANG-NEXT:         storage f16 [size=2, align=2];
// I686-LINUX-GNU-CLANG-NEXT:         storage f32 [size=4, align=4];
// I686-LINUX-GNU-CLANG-NEXT:         storage f64 [size=8, align=4];
// I686-LINUX-GNU-CLANG-NEXT:         storage f80 [size=12, align=4];
// I686-LINUX-GNU-CLANG-NEXT:         storage f128 [size=16, align=16];
// I686-LINUX-GNU-CLANG-NEXT:         storage d32 [size=4, align=4];
// I686-LINUX-GNU-CLANG-NEXT:         storage d64 [size=8, align=8];
// I686-LINUX-GNU-CLANG-NEXT:         storage d128 [size=16, align=16];
// I686-LINUX-GNU-CLANG-NEXT:     }
// I686-LINUX-GNU-CLANG-NEXT:     type @type0 pair = struct {
// I686-LINUX-GNU-CLANG-NEXT:         field0 a: i32;
// I686-LINUX-GNU-CLANG-NEXT:         field1 b: i32;
// I686-LINUX-GNU-CLANG-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// I686-LINUX-GNU-CLANG-NEXT:     global %0 clang_major: i32 [storage=static] = const<i32>(22) [linkage=external];
// I686-LINUX-GNU-CLANG-NEXT:     global %1 sizeof_long: u32 [storage=static] = const<u32>(4) [linkage=external];
// I686-LINUX-GNU-CLANG-NEXT:     global %2 sizeof_long_double: u32 [storage=static] = const<u32>(12) [linkage=external];
// I686-LINUX-GNU-CLANG-NEXT:     global %3 sizeof_va_list: u32 [storage=static] = const<u32>(4) [linkage=external];
// I686-LINUX-GNU-CLANG-NEXT:     global %4 alignof_long_long: u32 [storage=static] = const<u32>(4) [linkage=external];
// I686-LINUX-GNU-CLANG-NEXT:     fn %6 @record(%7 value: @type0) -> @type0 [linkage=external] [abi=x86_cdecl(coerce<i32, i32>) -> sret<align=4>] [fallthrough=ub_if_used] {
// I686-LINUX-GNU-CLANG-NEXT:         return copy<@type0, reason=return>(read<@type0>(%7));
// I686-LINUX-GNU-CLANG-NEXT:     }
// I686-LINUX-GNU-CLANG-NEXT: }
// SLATE-FILECHECK-END I686-LINUX-GNU-CLANG
// SLATE-FILECHECK-BEGIN I686-LINUX-GNU-MSVC
// I686-LINUX-GNU-MSVC: module {
// I686-LINUX-GNU-MSVC-NEXT:     target "i686-unknown-linux-gnu" {
// I686-LINUX-GNU-MSVC-NEXT:         endian = little;
// I686-LINUX-GNU-MSVC-NEXT:         pointer [size=4, align=4];
// I686-LINUX-GNU-MSVC-NEXT:         stack_alignment = 16;
// I686-LINUX-GNU-MSVC-NEXT:         long_double = f80;
// I686-LINUX-GNU-MSVC-NEXT:         storage bool [size=1, align=1];
// I686-LINUX-GNU-MSVC-NEXT:         storage i8, u8 [size=1, align=1];
// I686-LINUX-GNU-MSVC-NEXT:         storage i16, u16 [size=2, align=2];
// I686-LINUX-GNU-MSVC-NEXT:         storage i32, u32 [size=4, align=4];
// I686-LINUX-GNU-MSVC-NEXT:         storage i64, u64 [size=8, align=4];
// I686-LINUX-GNU-MSVC-NEXT:         storage i128, u128 [size=16, align=16];
// I686-LINUX-GNU-MSVC-NEXT:         storage bf16 [size=2, align=2];
// I686-LINUX-GNU-MSVC-NEXT:         storage f16 [size=2, align=2];
// I686-LINUX-GNU-MSVC-NEXT:         storage f32 [size=4, align=4];
// I686-LINUX-GNU-MSVC-NEXT:         storage f64 [size=8, align=4];
// I686-LINUX-GNU-MSVC-NEXT:         storage f80 [size=12, align=4];
// I686-LINUX-GNU-MSVC-NEXT:         storage f128 [size=16, align=16];
// I686-LINUX-GNU-MSVC-NEXT:         storage d32 [size=4, align=4];
// I686-LINUX-GNU-MSVC-NEXT:         storage d64 [size=8, align=8];
// I686-LINUX-GNU-MSVC-NEXT:         storage d128 [size=16, align=16];
// I686-LINUX-GNU-MSVC-NEXT:     }
// I686-LINUX-GNU-MSVC-NEXT:     type @type0 pair = struct {
// I686-LINUX-GNU-MSVC-NEXT:         field0 a: i32;
// I686-LINUX-GNU-MSVC-NEXT:         field1 b: i32;
// I686-LINUX-GNU-MSVC-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// I686-LINUX-GNU-MSVC-NEXT:     global %0 clang_major: i32 [storage=static] = const<i32>(22) [linkage=external];
// I686-LINUX-GNU-MSVC-NEXT:     global %1 sizeof_long: u32 [storage=static] = const<u32>(4) [linkage=external];
// I686-LINUX-GNU-MSVC-NEXT:     global %2 sizeof_long_double: u32 [storage=static] = const<u32>(12) [linkage=external];
// I686-LINUX-GNU-MSVC-NEXT:     global %3 sizeof_va_list: u32 [storage=static] = const<u32>(4) [linkage=external];
// I686-LINUX-GNU-MSVC-NEXT:     global %4 alignof_long_long: u32 [storage=static] = const<u32>(4) [linkage=external];
// I686-LINUX-GNU-MSVC-NEXT:     fn %6 @record(%7 value: @type0) -> @type0 [linkage=external] [abi=x86_cdecl(coerce<i32, i32>) -> sret<align=4>] [fallthrough=ub_if_used] {
// I686-LINUX-GNU-MSVC-NEXT:         return copy<@type0, reason=return>(read<@type0>(%7));
// I686-LINUX-GNU-MSVC-NEXT:     }
// I686-LINUX-GNU-MSVC-NEXT: }
// SLATE-FILECHECK-END I686-LINUX-GNU-MSVC
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
// AARCH64-LINUX-GNU-GCC-NEXT:     fn %6 @record(%7 value: @type0) -> @type0 [linkage=external] [abi=aapcs64(coerce<i64>) -> coerce<i64>] [fallthrough=ub_if_used] {
// AARCH64-LINUX-GNU-GCC-NEXT:         return copy<@type0, reason=return>(read<@type0>(%7));
// AARCH64-LINUX-GNU-GCC-NEXT:     }
// AARCH64-LINUX-GNU-GCC-NEXT: }
// SLATE-FILECHECK-END AARCH64-LINUX-GNU-GCC
// SLATE-FILECHECK-BEGIN AARCH64-LINUX-GNU-CLANG
// AARCH64-LINUX-GNU-CLANG: module {
// AARCH64-LINUX-GNU-CLANG-NEXT:     target "aarch64-unknown-linux-gnu" {
// AARCH64-LINUX-GNU-CLANG-NEXT:         endian = little;
// AARCH64-LINUX-GNU-CLANG-NEXT:         pointer [size=8, align=8];
// AARCH64-LINUX-GNU-CLANG-NEXT:         stack_alignment = 16;
// AARCH64-LINUX-GNU-CLANG-NEXT:         long_double = f128;
// AARCH64-LINUX-GNU-CLANG-NEXT:         storage bool [size=1, align=1];
// AARCH64-LINUX-GNU-CLANG-NEXT:         storage i8, u8 [size=1, align=1];
// AARCH64-LINUX-GNU-CLANG-NEXT:         storage i16, u16 [size=2, align=2];
// AARCH64-LINUX-GNU-CLANG-NEXT:         storage i32, u32 [size=4, align=4];
// AARCH64-LINUX-GNU-CLANG-NEXT:         storage i64, u64 [size=8, align=8];
// AARCH64-LINUX-GNU-CLANG-NEXT:         storage i128, u128 [size=16, align=16];
// AARCH64-LINUX-GNU-CLANG-NEXT:         storage bf16 [size=2, align=2];
// AARCH64-LINUX-GNU-CLANG-NEXT:         storage f16 [size=2, align=2];
// AARCH64-LINUX-GNU-CLANG-NEXT:         storage f32 [size=4, align=4];
// AARCH64-LINUX-GNU-CLANG-NEXT:         storage f64 [size=8, align=8];
// AARCH64-LINUX-GNU-CLANG-NEXT:         storage f128 [size=16, align=16];
// AARCH64-LINUX-GNU-CLANG-NEXT:         storage d32 [size=4, align=4];
// AARCH64-LINUX-GNU-CLANG-NEXT:         storage d64 [size=8, align=8];
// AARCH64-LINUX-GNU-CLANG-NEXT:         storage d128 [size=16, align=16];
// AARCH64-LINUX-GNU-CLANG-NEXT:     }
// AARCH64-LINUX-GNU-CLANG-NEXT:     type @type0 pair = struct {
// AARCH64-LINUX-GNU-CLANG-NEXT:         field0 a: i32;
// AARCH64-LINUX-GNU-CLANG-NEXT:         field1 b: i32;
// AARCH64-LINUX-GNU-CLANG-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// AARCH64-LINUX-GNU-CLANG-NEXT:     global %0 clang_major: i32 [storage=static] = const<i32>(22) [linkage=external];
// AARCH64-LINUX-GNU-CLANG-NEXT:     global %1 sizeof_long: u64 [storage=static] = const<u64>(8) [linkage=external];
// AARCH64-LINUX-GNU-CLANG-NEXT:     global %2 sizeof_long_double: u64 [storage=static] = const<u64>(16) [linkage=external];
// AARCH64-LINUX-GNU-CLANG-NEXT:     global %3 sizeof_va_list: u64 [storage=static] = const<u64>(32) [linkage=external];
// AARCH64-LINUX-GNU-CLANG-NEXT:     global %4 alignof_long_long: u64 [storage=static] = const<u64>(8) [linkage=external];
// AARCH64-LINUX-GNU-CLANG-NEXT:     fn %6 @record(%7 value: @type0) -> @type0 [linkage=external] [abi=aapcs64(coerce<i64>) -> coerce<i64>] [fallthrough=ub_if_used] {
// AARCH64-LINUX-GNU-CLANG-NEXT:         return copy<@type0, reason=return>(read<@type0>(%7));
// AARCH64-LINUX-GNU-CLANG-NEXT:     }
// AARCH64-LINUX-GNU-CLANG-NEXT: }
// SLATE-FILECHECK-END AARCH64-LINUX-GNU-CLANG
// SLATE-FILECHECK-BEGIN AARCH64-LINUX-GNU-MSVC
// AARCH64-LINUX-GNU-MSVC: module {
// AARCH64-LINUX-GNU-MSVC-NEXT:     target "aarch64-unknown-linux-gnu" {
// AARCH64-LINUX-GNU-MSVC-NEXT:         endian = little;
// AARCH64-LINUX-GNU-MSVC-NEXT:         pointer [size=8, align=8];
// AARCH64-LINUX-GNU-MSVC-NEXT:         stack_alignment = 16;
// AARCH64-LINUX-GNU-MSVC-NEXT:         long_double = f128;
// AARCH64-LINUX-GNU-MSVC-NEXT:         storage bool [size=1, align=1];
// AARCH64-LINUX-GNU-MSVC-NEXT:         storage i8, u8 [size=1, align=1];
// AARCH64-LINUX-GNU-MSVC-NEXT:         storage i16, u16 [size=2, align=2];
// AARCH64-LINUX-GNU-MSVC-NEXT:         storage i32, u32 [size=4, align=4];
// AARCH64-LINUX-GNU-MSVC-NEXT:         storage i64, u64 [size=8, align=8];
// AARCH64-LINUX-GNU-MSVC-NEXT:         storage i128, u128 [size=16, align=16];
// AARCH64-LINUX-GNU-MSVC-NEXT:         storage bf16 [size=2, align=2];
// AARCH64-LINUX-GNU-MSVC-NEXT:         storage f16 [size=2, align=2];
// AARCH64-LINUX-GNU-MSVC-NEXT:         storage f32 [size=4, align=4];
// AARCH64-LINUX-GNU-MSVC-NEXT:         storage f64 [size=8, align=8];
// AARCH64-LINUX-GNU-MSVC-NEXT:         storage f128 [size=16, align=16];
// AARCH64-LINUX-GNU-MSVC-NEXT:         storage d32 [size=4, align=4];
// AARCH64-LINUX-GNU-MSVC-NEXT:         storage d64 [size=8, align=8];
// AARCH64-LINUX-GNU-MSVC-NEXT:         storage d128 [size=16, align=16];
// AARCH64-LINUX-GNU-MSVC-NEXT:     }
// AARCH64-LINUX-GNU-MSVC-NEXT:     type @type0 pair = struct {
// AARCH64-LINUX-GNU-MSVC-NEXT:         field0 a: i32;
// AARCH64-LINUX-GNU-MSVC-NEXT:         field1 b: i32;
// AARCH64-LINUX-GNU-MSVC-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// AARCH64-LINUX-GNU-MSVC-NEXT:     global %0 clang_major: i32 [storage=static] = const<i32>(22) [linkage=external];
// AARCH64-LINUX-GNU-MSVC-NEXT:     global %1 sizeof_long: u64 [storage=static] = const<u64>(8) [linkage=external];
// AARCH64-LINUX-GNU-MSVC-NEXT:     global %2 sizeof_long_double: u64 [storage=static] = const<u64>(16) [linkage=external];
// AARCH64-LINUX-GNU-MSVC-NEXT:     global %3 sizeof_va_list: u64 [storage=static] = const<u64>(32) [linkage=external];
// AARCH64-LINUX-GNU-MSVC-NEXT:     global %4 alignof_long_long: u64 [storage=static] = const<u64>(8) [linkage=external];
// AARCH64-LINUX-GNU-MSVC-NEXT:     fn %6 @record(%7 value: @type0) -> @type0 [linkage=external] [abi=aapcs64(coerce<i64>) -> coerce<i64>] [fallthrough=ub_if_used] {
// AARCH64-LINUX-GNU-MSVC-NEXT:         return copy<@type0, reason=return>(read<@type0>(%7));
// AARCH64-LINUX-GNU-MSVC-NEXT:     }
// AARCH64-LINUX-GNU-MSVC-NEXT: }
// SLATE-FILECHECK-END AARCH64-LINUX-GNU-MSVC
// SLATE-FILECHECK-BEGIN ARMV7-LINUX-GNUEABI-GCC
// ARMV7-LINUX-GNUEABI-GCC: module {
// ARMV7-LINUX-GNUEABI-GCC-NEXT:     target "armv7-unknown-linux-gnueabi" {
// ARMV7-LINUX-GNUEABI-GCC-NEXT:         endian = little;
// ARMV7-LINUX-GNUEABI-GCC-NEXT:         pointer [size=4, align=4];
// ARMV7-LINUX-GNUEABI-GCC-NEXT:         stack_alignment = 8;
// ARMV7-LINUX-GNUEABI-GCC-NEXT:         long_double = f64;
// ARMV7-LINUX-GNUEABI-GCC-NEXT:         storage bool [size=1, align=1];
// ARMV7-LINUX-GNUEABI-GCC-NEXT:         storage i8, u8 [size=1, align=1];
// ARMV7-LINUX-GNUEABI-GCC-NEXT:         storage i16, u16 [size=2, align=2];
// ARMV7-LINUX-GNUEABI-GCC-NEXT:         storage i32, u32 [size=4, align=4];
// ARMV7-LINUX-GNUEABI-GCC-NEXT:         storage i64, u64 [size=8, align=8];
// ARMV7-LINUX-GNUEABI-GCC-NEXT:         storage bf16 [size=2, align=2];
// ARMV7-LINUX-GNUEABI-GCC-NEXT:         storage f16 [size=2, align=2];
// ARMV7-LINUX-GNUEABI-GCC-NEXT:         storage f32 [size=4, align=4];
// ARMV7-LINUX-GNUEABI-GCC-NEXT:         storage f64 [size=8, align=8];
// ARMV7-LINUX-GNUEABI-GCC-NEXT:         storage f128 [size=16, align=16];
// ARMV7-LINUX-GNUEABI-GCC-NEXT:         storage d32 [size=4, align=4];
// ARMV7-LINUX-GNUEABI-GCC-NEXT:         storage d64 [size=8, align=8];
// ARMV7-LINUX-GNUEABI-GCC-NEXT:         storage d128 [size=16, align=16];
// ARMV7-LINUX-GNUEABI-GCC-NEXT:     }
// ARMV7-LINUX-GNUEABI-GCC-NEXT:     type @type0 pair = struct {
// ARMV7-LINUX-GNUEABI-GCC-NEXT:         field0 a: i32;
// ARMV7-LINUX-GNUEABI-GCC-NEXT:         field1 b: i32;
// ARMV7-LINUX-GNUEABI-GCC-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// ARMV7-LINUX-GNUEABI-GCC-NEXT:     global %0 gnuc: i32 [storage=static] = const<i32>(15) [linkage=external];
// ARMV7-LINUX-GNUEABI-GCC-NEXT:     global %1 sizeof_long: u32 [storage=static] = const<u32>(4) [linkage=external];
// ARMV7-LINUX-GNUEABI-GCC-NEXT:     global %2 sizeof_long_double: u32 [storage=static] = const<u32>(8) [linkage=external];
// ARMV7-LINUX-GNUEABI-GCC-NEXT:     global %3 sizeof_va_list: u32 [storage=static] = const<u32>(4) [linkage=external];
// ARMV7-LINUX-GNUEABI-GCC-NEXT:     global %4 alignof_long_long: u32 [storage=static] = const<u32>(8) [linkage=external];
// ARMV7-LINUX-GNUEABI-GCC-NEXT:     fn %6 @record(%7 value: @type0) -> @type0 [linkage=external] [abi=aapcs32(coerce<i32, i32>) -> sret<align=4>] [fallthrough=ub_if_used] {
// ARMV7-LINUX-GNUEABI-GCC-NEXT:         return copy<@type0, reason=return>(read<@type0>(%7));
// ARMV7-LINUX-GNUEABI-GCC-NEXT:     }
// ARMV7-LINUX-GNUEABI-GCC-NEXT: }
// SLATE-FILECHECK-END ARMV7-LINUX-GNUEABI-GCC
// SLATE-FILECHECK-BEGIN ARMV7-LINUX-GNUEABI-CLANG
// ARMV7-LINUX-GNUEABI-CLANG: module {
// ARMV7-LINUX-GNUEABI-CLANG-NEXT:     target "armv7-unknown-linux-gnueabi" {
// ARMV7-LINUX-GNUEABI-CLANG-NEXT:         endian = little;
// ARMV7-LINUX-GNUEABI-CLANG-NEXT:         pointer [size=4, align=4];
// ARMV7-LINUX-GNUEABI-CLANG-NEXT:         stack_alignment = 8;
// ARMV7-LINUX-GNUEABI-CLANG-NEXT:         long_double = f64;
// ARMV7-LINUX-GNUEABI-CLANG-NEXT:         storage bool [size=1, align=1];
// ARMV7-LINUX-GNUEABI-CLANG-NEXT:         storage i8, u8 [size=1, align=1];
// ARMV7-LINUX-GNUEABI-CLANG-NEXT:         storage i16, u16 [size=2, align=2];
// ARMV7-LINUX-GNUEABI-CLANG-NEXT:         storage i32, u32 [size=4, align=4];
// ARMV7-LINUX-GNUEABI-CLANG-NEXT:         storage i64, u64 [size=8, align=8];
// ARMV7-LINUX-GNUEABI-CLANG-NEXT:         storage bf16 [size=2, align=2];
// ARMV7-LINUX-GNUEABI-CLANG-NEXT:         storage f16 [size=2, align=2];
// ARMV7-LINUX-GNUEABI-CLANG-NEXT:         storage f32 [size=4, align=4];
// ARMV7-LINUX-GNUEABI-CLANG-NEXT:         storage f64 [size=8, align=8];
// ARMV7-LINUX-GNUEABI-CLANG-NEXT:         storage f128 [size=16, align=16];
// ARMV7-LINUX-GNUEABI-CLANG-NEXT:         storage d32 [size=4, align=4];
// ARMV7-LINUX-GNUEABI-CLANG-NEXT:         storage d64 [size=8, align=8];
// ARMV7-LINUX-GNUEABI-CLANG-NEXT:         storage d128 [size=16, align=16];
// ARMV7-LINUX-GNUEABI-CLANG-NEXT:     }
// ARMV7-LINUX-GNUEABI-CLANG-NEXT:     type @type0 pair = struct {
// ARMV7-LINUX-GNUEABI-CLANG-NEXT:         field0 a: i32;
// ARMV7-LINUX-GNUEABI-CLANG-NEXT:         field1 b: i32;
// ARMV7-LINUX-GNUEABI-CLANG-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// ARMV7-LINUX-GNUEABI-CLANG-NEXT:     global %0 clang_major: i32 [storage=static] = const<i32>(22) [linkage=external];
// ARMV7-LINUX-GNUEABI-CLANG-NEXT:     global %1 sizeof_long: u32 [storage=static] = const<u32>(4) [linkage=external];
// ARMV7-LINUX-GNUEABI-CLANG-NEXT:     global %2 sizeof_long_double: u32 [storage=static] = const<u32>(8) [linkage=external];
// ARMV7-LINUX-GNUEABI-CLANG-NEXT:     global %3 sizeof_va_list: u32 [storage=static] = const<u32>(4) [linkage=external];
// ARMV7-LINUX-GNUEABI-CLANG-NEXT:     global %4 alignof_long_long: u32 [storage=static] = const<u32>(8) [linkage=external];
// ARMV7-LINUX-GNUEABI-CLANG-NEXT:     fn %6 @record(%7 value: @type0) -> @type0 [linkage=external] [abi=aapcs32(coerce<i32, i32>) -> sret<align=4>] [fallthrough=ub_if_used] {
// ARMV7-LINUX-GNUEABI-CLANG-NEXT:         return copy<@type0, reason=return>(read<@type0>(%7));
// ARMV7-LINUX-GNUEABI-CLANG-NEXT:     }
// ARMV7-LINUX-GNUEABI-CLANG-NEXT: }
// SLATE-FILECHECK-END ARMV7-LINUX-GNUEABI-CLANG
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
// ARMV7-LINUX-GNUEABI-MSVC-NEXT:     fn %6 @record(%7 value: @type0) -> @type0 [linkage=external] [abi=aapcs32(coerce<i32, i32>) -> sret<align=4>] [fallthrough=ub_if_used] {
// ARMV7-LINUX-GNUEABI-MSVC-NEXT:         return copy<@type0, reason=return>(read<@type0>(%7));
// ARMV7-LINUX-GNUEABI-MSVC-NEXT:     }
// ARMV7-LINUX-GNUEABI-MSVC-NEXT: }
// SLATE-FILECHECK-END ARMV7-LINUX-GNUEABI-MSVC
// SLATE-FILECHECK-BEGIN ARMV7-LINUX-GNUEABIHF-GCC
// ARMV7-LINUX-GNUEABIHF-GCC: module {
// ARMV7-LINUX-GNUEABIHF-GCC-NEXT:     target "armv7-unknown-linux-gnueabihf" {
// ARMV7-LINUX-GNUEABIHF-GCC-NEXT:         endian = little;
// ARMV7-LINUX-GNUEABIHF-GCC-NEXT:         pointer [size=4, align=4];
// ARMV7-LINUX-GNUEABIHF-GCC-NEXT:         stack_alignment = 8;
// ARMV7-LINUX-GNUEABIHF-GCC-NEXT:         long_double = f64;
// ARMV7-LINUX-GNUEABIHF-GCC-NEXT:         storage bool [size=1, align=1];
// ARMV7-LINUX-GNUEABIHF-GCC-NEXT:         storage i8, u8 [size=1, align=1];
// ARMV7-LINUX-GNUEABIHF-GCC-NEXT:         storage i16, u16 [size=2, align=2];
// ARMV7-LINUX-GNUEABIHF-GCC-NEXT:         storage i32, u32 [size=4, align=4];
// ARMV7-LINUX-GNUEABIHF-GCC-NEXT:         storage i64, u64 [size=8, align=8];
// ARMV7-LINUX-GNUEABIHF-GCC-NEXT:         storage bf16 [size=2, align=2];
// ARMV7-LINUX-GNUEABIHF-GCC-NEXT:         storage f16 [size=2, align=2];
// ARMV7-LINUX-GNUEABIHF-GCC-NEXT:         storage f32 [size=4, align=4];
// ARMV7-LINUX-GNUEABIHF-GCC-NEXT:         storage f64 [size=8, align=8];
// ARMV7-LINUX-GNUEABIHF-GCC-NEXT:         storage f128 [size=16, align=16];
// ARMV7-LINUX-GNUEABIHF-GCC-NEXT:         storage d32 [size=4, align=4];
// ARMV7-LINUX-GNUEABIHF-GCC-NEXT:         storage d64 [size=8, align=8];
// ARMV7-LINUX-GNUEABIHF-GCC-NEXT:         storage d128 [size=16, align=16];
// ARMV7-LINUX-GNUEABIHF-GCC-NEXT:     }
// ARMV7-LINUX-GNUEABIHF-GCC-NEXT:     type @type0 pair = struct {
// ARMV7-LINUX-GNUEABIHF-GCC-NEXT:         field0 a: i32;
// ARMV7-LINUX-GNUEABIHF-GCC-NEXT:         field1 b: i32;
// ARMV7-LINUX-GNUEABIHF-GCC-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// ARMV7-LINUX-GNUEABIHF-GCC-NEXT:     global %0 gnuc: i32 [storage=static] = const<i32>(15) [linkage=external];
// ARMV7-LINUX-GNUEABIHF-GCC-NEXT:     global %1 sizeof_long: u32 [storage=static] = const<u32>(4) [linkage=external];
// ARMV7-LINUX-GNUEABIHF-GCC-NEXT:     global %2 sizeof_long_double: u32 [storage=static] = const<u32>(8) [linkage=external];
// ARMV7-LINUX-GNUEABIHF-GCC-NEXT:     global %3 sizeof_va_list: u32 [storage=static] = const<u32>(4) [linkage=external];
// ARMV7-LINUX-GNUEABIHF-GCC-NEXT:     global %4 alignof_long_long: u32 [storage=static] = const<u32>(8) [linkage=external];
// ARMV7-LINUX-GNUEABIHF-GCC-NEXT:     fn %6 @record(%7 value: @type0) -> @type0 [linkage=external] [abi=aapcs32_hard_float(coerce<i32, i32>) -> sret<align=4>] [fallthrough=ub_if_used] {
// ARMV7-LINUX-GNUEABIHF-GCC-NEXT:         return copy<@type0, reason=return>(read<@type0>(%7));
// ARMV7-LINUX-GNUEABIHF-GCC-NEXT:     }
// ARMV7-LINUX-GNUEABIHF-GCC-NEXT: }
// SLATE-FILECHECK-END ARMV7-LINUX-GNUEABIHF-GCC
// SLATE-FILECHECK-BEGIN ARMV7-LINUX-GNUEABIHF-CLANG
// ARMV7-LINUX-GNUEABIHF-CLANG: module {
// ARMV7-LINUX-GNUEABIHF-CLANG-NEXT:     target "armv7-unknown-linux-gnueabihf" {
// ARMV7-LINUX-GNUEABIHF-CLANG-NEXT:         endian = little;
// ARMV7-LINUX-GNUEABIHF-CLANG-NEXT:         pointer [size=4, align=4];
// ARMV7-LINUX-GNUEABIHF-CLANG-NEXT:         stack_alignment = 8;
// ARMV7-LINUX-GNUEABIHF-CLANG-NEXT:         long_double = f64;
// ARMV7-LINUX-GNUEABIHF-CLANG-NEXT:         storage bool [size=1, align=1];
// ARMV7-LINUX-GNUEABIHF-CLANG-NEXT:         storage i8, u8 [size=1, align=1];
// ARMV7-LINUX-GNUEABIHF-CLANG-NEXT:         storage i16, u16 [size=2, align=2];
// ARMV7-LINUX-GNUEABIHF-CLANG-NEXT:         storage i32, u32 [size=4, align=4];
// ARMV7-LINUX-GNUEABIHF-CLANG-NEXT:         storage i64, u64 [size=8, align=8];
// ARMV7-LINUX-GNUEABIHF-CLANG-NEXT:         storage bf16 [size=2, align=2];
// ARMV7-LINUX-GNUEABIHF-CLANG-NEXT:         storage f16 [size=2, align=2];
// ARMV7-LINUX-GNUEABIHF-CLANG-NEXT:         storage f32 [size=4, align=4];
// ARMV7-LINUX-GNUEABIHF-CLANG-NEXT:         storage f64 [size=8, align=8];
// ARMV7-LINUX-GNUEABIHF-CLANG-NEXT:         storage f128 [size=16, align=16];
// ARMV7-LINUX-GNUEABIHF-CLANG-NEXT:         storage d32 [size=4, align=4];
// ARMV7-LINUX-GNUEABIHF-CLANG-NEXT:         storage d64 [size=8, align=8];
// ARMV7-LINUX-GNUEABIHF-CLANG-NEXT:         storage d128 [size=16, align=16];
// ARMV7-LINUX-GNUEABIHF-CLANG-NEXT:     }
// ARMV7-LINUX-GNUEABIHF-CLANG-NEXT:     type @type0 pair = struct {
// ARMV7-LINUX-GNUEABIHF-CLANG-NEXT:         field0 a: i32;
// ARMV7-LINUX-GNUEABIHF-CLANG-NEXT:         field1 b: i32;
// ARMV7-LINUX-GNUEABIHF-CLANG-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// ARMV7-LINUX-GNUEABIHF-CLANG-NEXT:     global %0 clang_major: i32 [storage=static] = const<i32>(22) [linkage=external];
// ARMV7-LINUX-GNUEABIHF-CLANG-NEXT:     global %1 sizeof_long: u32 [storage=static] = const<u32>(4) [linkage=external];
// ARMV7-LINUX-GNUEABIHF-CLANG-NEXT:     global %2 sizeof_long_double: u32 [storage=static] = const<u32>(8) [linkage=external];
// ARMV7-LINUX-GNUEABIHF-CLANG-NEXT:     global %3 sizeof_va_list: u32 [storage=static] = const<u32>(4) [linkage=external];
// ARMV7-LINUX-GNUEABIHF-CLANG-NEXT:     global %4 alignof_long_long: u32 [storage=static] = const<u32>(8) [linkage=external];
// ARMV7-LINUX-GNUEABIHF-CLANG-NEXT:     fn %6 @record(%7 value: @type0) -> @type0 [linkage=external] [abi=aapcs32_hard_float(coerce<i32, i32>) -> sret<align=4>] [fallthrough=ub_if_used] {
// ARMV7-LINUX-GNUEABIHF-CLANG-NEXT:         return copy<@type0, reason=return>(read<@type0>(%7));
// ARMV7-LINUX-GNUEABIHF-CLANG-NEXT:     }
// ARMV7-LINUX-GNUEABIHF-CLANG-NEXT: }
// SLATE-FILECHECK-END ARMV7-LINUX-GNUEABIHF-CLANG
// SLATE-FILECHECK-BEGIN ARMV7-LINUX-GNUEABIHF-MSVC
// ARMV7-LINUX-GNUEABIHF-MSVC: module {
// ARMV7-LINUX-GNUEABIHF-MSVC-NEXT:     target "armv7-unknown-linux-gnueabihf" {
// ARMV7-LINUX-GNUEABIHF-MSVC-NEXT:         endian = little;
// ARMV7-LINUX-GNUEABIHF-MSVC-NEXT:         pointer [size=4, align=4];
// ARMV7-LINUX-GNUEABIHF-MSVC-NEXT:         stack_alignment = 8;
// ARMV7-LINUX-GNUEABIHF-MSVC-NEXT:         long_double = f64;
// ARMV7-LINUX-GNUEABIHF-MSVC-NEXT:         storage bool [size=1, align=1];
// ARMV7-LINUX-GNUEABIHF-MSVC-NEXT:         storage i8, u8 [size=1, align=1];
// ARMV7-LINUX-GNUEABIHF-MSVC-NEXT:         storage i16, u16 [size=2, align=2];
// ARMV7-LINUX-GNUEABIHF-MSVC-NEXT:         storage i32, u32 [size=4, align=4];
// ARMV7-LINUX-GNUEABIHF-MSVC-NEXT:         storage i64, u64 [size=8, align=8];
// ARMV7-LINUX-GNUEABIHF-MSVC-NEXT:         storage bf16 [size=2, align=2];
// ARMV7-LINUX-GNUEABIHF-MSVC-NEXT:         storage f16 [size=2, align=2];
// ARMV7-LINUX-GNUEABIHF-MSVC-NEXT:         storage f32 [size=4, align=4];
// ARMV7-LINUX-GNUEABIHF-MSVC-NEXT:         storage f64 [size=8, align=8];
// ARMV7-LINUX-GNUEABIHF-MSVC-NEXT:         storage f128 [size=16, align=16];
// ARMV7-LINUX-GNUEABIHF-MSVC-NEXT:         storage d32 [size=4, align=4];
// ARMV7-LINUX-GNUEABIHF-MSVC-NEXT:         storage d64 [size=8, align=8];
// ARMV7-LINUX-GNUEABIHF-MSVC-NEXT:         storage d128 [size=16, align=16];
// ARMV7-LINUX-GNUEABIHF-MSVC-NEXT:     }
// ARMV7-LINUX-GNUEABIHF-MSVC-NEXT:     type @type0 pair = struct {
// ARMV7-LINUX-GNUEABIHF-MSVC-NEXT:         field0 a: i32;
// ARMV7-LINUX-GNUEABIHF-MSVC-NEXT:         field1 b: i32;
// ARMV7-LINUX-GNUEABIHF-MSVC-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// ARMV7-LINUX-GNUEABIHF-MSVC-NEXT:     global %0 clang_major: i32 [storage=static] = const<i32>(22) [linkage=external];
// ARMV7-LINUX-GNUEABIHF-MSVC-NEXT:     global %1 sizeof_long: u32 [storage=static] = const<u32>(4) [linkage=external];
// ARMV7-LINUX-GNUEABIHF-MSVC-NEXT:     global %2 sizeof_long_double: u32 [storage=static] = const<u32>(8) [linkage=external];
// ARMV7-LINUX-GNUEABIHF-MSVC-NEXT:     global %3 sizeof_va_list: u32 [storage=static] = const<u32>(4) [linkage=external];
// ARMV7-LINUX-GNUEABIHF-MSVC-NEXT:     global %4 alignof_long_long: u32 [storage=static] = const<u32>(8) [linkage=external];
// ARMV7-LINUX-GNUEABIHF-MSVC-NEXT:     fn %6 @record(%7 value: @type0) -> @type0 [linkage=external] [abi=aapcs32_hard_float(coerce<i32, i32>) -> sret<align=4>] [fallthrough=ub_if_used] {
// ARMV7-LINUX-GNUEABIHF-MSVC-NEXT:         return copy<@type0, reason=return>(read<@type0>(%7));
// ARMV7-LINUX-GNUEABIHF-MSVC-NEXT:     }
// ARMV7-LINUX-GNUEABIHF-MSVC-NEXT: }
// SLATE-FILECHECK-END ARMV7-LINUX-GNUEABIHF-MSVC
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
// AARCH64-DARWIN-CLANG-NEXT:     type @type0 pair = struct {
// AARCH64-DARWIN-CLANG-NEXT:         field0 a: i32;
// AARCH64-DARWIN-CLANG-NEXT:         field1 b: i32;
// AARCH64-DARWIN-CLANG-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// AARCH64-DARWIN-CLANG-NEXT:     global %0 clang_major: i32 [storage=static] = const<i32>(22) [linkage=external];
// AARCH64-DARWIN-CLANG-NEXT:     global %1 sizeof_long: u64 [storage=static] = const<u64>(8) [linkage=external];
// AARCH64-DARWIN-CLANG-NEXT:     global %2 sizeof_long_double: u64 [storage=static] = const<u64>(8) [linkage=external];
// AARCH64-DARWIN-CLANG-NEXT:     global %3 sizeof_va_list: u64 [storage=static] = const<u64>(8) [linkage=external];
// AARCH64-DARWIN-CLANG-NEXT:     global %4 alignof_long_long: u64 [storage=static] = const<u64>(8) [linkage=external];
// AARCH64-DARWIN-CLANG-NEXT:     fn %6 @record(%7 value: @type0) -> @type0 [linkage=external] [abi=aapcs64(coerce<i64>) -> coerce<i64>] [fallthrough=ub_if_used] {
// AARCH64-DARWIN-CLANG-NEXT:         return copy<@type0, reason=return>(read<@type0>(%7));
// AARCH64-DARWIN-CLANG-NEXT:     }
// AARCH64-DARWIN-CLANG-NEXT: }
// SLATE-FILECHECK-END AARCH64-DARWIN-CLANG
// SLATE-FILECHECK-BEGIN X86-64-DARWIN-CLANG
// X86-64-DARWIN-CLANG: module {
// X86-64-DARWIN-CLANG-NEXT:     target "x86_64-apple-darwin" {
// X86-64-DARWIN-CLANG-NEXT:         endian = little;
// X86-64-DARWIN-CLANG-NEXT:         pointer [size=8, align=8];
// X86-64-DARWIN-CLANG-NEXT:         stack_alignment = 16;
// X86-64-DARWIN-CLANG-NEXT:         long_double = f80;
// X86-64-DARWIN-CLANG-NEXT:         storage bool [size=1, align=1];
// X86-64-DARWIN-CLANG-NEXT:         storage i8, u8 [size=1, align=1];
// X86-64-DARWIN-CLANG-NEXT:         storage i16, u16 [size=2, align=2];
// X86-64-DARWIN-CLANG-NEXT:         storage i32, u32 [size=4, align=4];
// X86-64-DARWIN-CLANG-NEXT:         storage i64, u64 [size=8, align=8];
// X86-64-DARWIN-CLANG-NEXT:         storage i128, u128 [size=16, align=16];
// X86-64-DARWIN-CLANG-NEXT:         storage bf16 [size=2, align=2];
// X86-64-DARWIN-CLANG-NEXT:         storage f16 [size=2, align=2];
// X86-64-DARWIN-CLANG-NEXT:         storage f32 [size=4, align=4];
// X86-64-DARWIN-CLANG-NEXT:         storage f64 [size=8, align=8];
// X86-64-DARWIN-CLANG-NEXT:         storage f80 [size=16, align=16];
// X86-64-DARWIN-CLANG-NEXT:         storage f128 [size=16, align=16];
// X86-64-DARWIN-CLANG-NEXT:         storage d32 [size=4, align=4];
// X86-64-DARWIN-CLANG-NEXT:         storage d64 [size=8, align=8];
// X86-64-DARWIN-CLANG-NEXT:         storage d128 [size=16, align=16];
// X86-64-DARWIN-CLANG-NEXT:     }
// X86-64-DARWIN-CLANG-NEXT:     type @type0 pair = struct {
// X86-64-DARWIN-CLANG-NEXT:         field0 a: i32;
// X86-64-DARWIN-CLANG-NEXT:         field1 b: i32;
// X86-64-DARWIN-CLANG-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// X86-64-DARWIN-CLANG-NEXT:     global %0 clang_major: i32 [storage=static] = const<i32>(22) [linkage=external];
// X86-64-DARWIN-CLANG-NEXT:     global %1 sizeof_long: u64 [storage=static] = const<u64>(8) [linkage=external];
// X86-64-DARWIN-CLANG-NEXT:     global %2 sizeof_long_double: u64 [storage=static] = const<u64>(16) [linkage=external];
// X86-64-DARWIN-CLANG-NEXT:     global %3 sizeof_va_list: u64 [storage=static] = const<u64>(24) [linkage=external];
// X86-64-DARWIN-CLANG-NEXT:     global %4 alignof_long_long: u64 [storage=static] = const<u64>(8) [linkage=external];
// X86-64-DARWIN-CLANG-NEXT:     fn %6 @record(%7 value: @type0) -> @type0 [linkage=external] [abi=sysv64(coerce<i64>) -> coerce<i64>] [fallthrough=ub_if_used] {
// X86-64-DARWIN-CLANG-NEXT:         return copy<@type0, reason=return>(read<@type0>(%7));
// X86-64-DARWIN-CLANG-NEXT:     }
// X86-64-DARWIN-CLANG-NEXT: }
// SLATE-FILECHECK-END X86-64-DARWIN-CLANG
// SLATE-FILECHECK-BEGIN AARCH64-ANDROID-CLANG
// AARCH64-ANDROID-CLANG: module {
// AARCH64-ANDROID-CLANG-NEXT:     target "aarch64-linux-android" {
// AARCH64-ANDROID-CLANG-NEXT:         endian = little;
// AARCH64-ANDROID-CLANG-NEXT:         pointer [size=8, align=8];
// AARCH64-ANDROID-CLANG-NEXT:         stack_alignment = 16;
// AARCH64-ANDROID-CLANG-NEXT:         long_double = f128;
// AARCH64-ANDROID-CLANG-NEXT:         storage bool [size=1, align=1];
// AARCH64-ANDROID-CLANG-NEXT:         storage i8, u8 [size=1, align=1];
// AARCH64-ANDROID-CLANG-NEXT:         storage i16, u16 [size=2, align=2];
// AARCH64-ANDROID-CLANG-NEXT:         storage i32, u32 [size=4, align=4];
// AARCH64-ANDROID-CLANG-NEXT:         storage i64, u64 [size=8, align=8];
// AARCH64-ANDROID-CLANG-NEXT:         storage i128, u128 [size=16, align=16];
// AARCH64-ANDROID-CLANG-NEXT:         storage bf16 [size=2, align=2];
// AARCH64-ANDROID-CLANG-NEXT:         storage f16 [size=2, align=2];
// AARCH64-ANDROID-CLANG-NEXT:         storage f32 [size=4, align=4];
// AARCH64-ANDROID-CLANG-NEXT:         storage f64 [size=8, align=8];
// AARCH64-ANDROID-CLANG-NEXT:         storage f128 [size=16, align=16];
// AARCH64-ANDROID-CLANG-NEXT:         storage d32 [size=4, align=4];
// AARCH64-ANDROID-CLANG-NEXT:         storage d64 [size=8, align=8];
// AARCH64-ANDROID-CLANG-NEXT:         storage d128 [size=16, align=16];
// AARCH64-ANDROID-CLANG-NEXT:     }
// AARCH64-ANDROID-CLANG-NEXT:     type @type0 pair = struct {
// AARCH64-ANDROID-CLANG-NEXT:         field0 a: i32;
// AARCH64-ANDROID-CLANG-NEXT:         field1 b: i32;
// AARCH64-ANDROID-CLANG-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// AARCH64-ANDROID-CLANG-NEXT:     global %0 clang_major: i32 [storage=static] = const<i32>(22) [linkage=external];
// AARCH64-ANDROID-CLANG-NEXT:     global %1 sizeof_long: u64 [storage=static] = const<u64>(8) [linkage=external];
// AARCH64-ANDROID-CLANG-NEXT:     global %2 sizeof_long_double: u64 [storage=static] = const<u64>(16) [linkage=external];
// AARCH64-ANDROID-CLANG-NEXT:     global %3 sizeof_va_list: u64 [storage=static] = const<u64>(32) [linkage=external];
// AARCH64-ANDROID-CLANG-NEXT:     global %4 alignof_long_long: u64 [storage=static] = const<u64>(8) [linkage=external];
// AARCH64-ANDROID-CLANG-NEXT:     fn %6 @record(%7 value: @type0) -> @type0 [linkage=external] [abi=aapcs64(coerce<i64>) -> coerce<i64>] [fallthrough=ub_if_used] {
// AARCH64-ANDROID-CLANG-NEXT:         return copy<@type0, reason=return>(read<@type0>(%7));
// AARCH64-ANDROID-CLANG-NEXT:     }
// AARCH64-ANDROID-CLANG-NEXT: }
// SLATE-FILECHECK-END AARCH64-ANDROID-CLANG
// SLATE-FILECHECK-BEGIN X86-64-ANDROID-CLANG
// X86-64-ANDROID-CLANG: module {
// X86-64-ANDROID-CLANG-NEXT:     target "x86_64-linux-android" {
// X86-64-ANDROID-CLANG-NEXT:         endian = little;
// X86-64-ANDROID-CLANG-NEXT:         pointer [size=8, align=8];
// X86-64-ANDROID-CLANG-NEXT:         stack_alignment = 16;
// X86-64-ANDROID-CLANG-NEXT:         long_double = f128;
// X86-64-ANDROID-CLANG-NEXT:         storage bool [size=1, align=1];
// X86-64-ANDROID-CLANG-NEXT:         storage i8, u8 [size=1, align=1];
// X86-64-ANDROID-CLANG-NEXT:         storage i16, u16 [size=2, align=2];
// X86-64-ANDROID-CLANG-NEXT:         storage i32, u32 [size=4, align=4];
// X86-64-ANDROID-CLANG-NEXT:         storage i64, u64 [size=8, align=8];
// X86-64-ANDROID-CLANG-NEXT:         storage i128, u128 [size=16, align=16];
// X86-64-ANDROID-CLANG-NEXT:         storage bf16 [size=2, align=2];
// X86-64-ANDROID-CLANG-NEXT:         storage f16 [size=2, align=2];
// X86-64-ANDROID-CLANG-NEXT:         storage f32 [size=4, align=4];
// X86-64-ANDROID-CLANG-NEXT:         storage f64 [size=8, align=8];
// X86-64-ANDROID-CLANG-NEXT:         storage f80 [size=16, align=16];
// X86-64-ANDROID-CLANG-NEXT:         storage f128 [size=16, align=16];
// X86-64-ANDROID-CLANG-NEXT:         storage d32 [size=4, align=4];
// X86-64-ANDROID-CLANG-NEXT:         storage d64 [size=8, align=8];
// X86-64-ANDROID-CLANG-NEXT:         storage d128 [size=16, align=16];
// X86-64-ANDROID-CLANG-NEXT:     }
// X86-64-ANDROID-CLANG-NEXT:     type @type0 pair = struct {
// X86-64-ANDROID-CLANG-NEXT:         field0 a: i32;
// X86-64-ANDROID-CLANG-NEXT:         field1 b: i32;
// X86-64-ANDROID-CLANG-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// X86-64-ANDROID-CLANG-NEXT:     global %0 clang_major: i32 [storage=static] = const<i32>(22) [linkage=external];
// X86-64-ANDROID-CLANG-NEXT:     global %1 sizeof_long: u64 [storage=static] = const<u64>(8) [linkage=external];
// X86-64-ANDROID-CLANG-NEXT:     global %2 sizeof_long_double: u64 [storage=static] = const<u64>(16) [linkage=external];
// X86-64-ANDROID-CLANG-NEXT:     global %3 sizeof_va_list: u64 [storage=static] = const<u64>(24) [linkage=external];
// X86-64-ANDROID-CLANG-NEXT:     global %4 alignof_long_long: u64 [storage=static] = const<u64>(8) [linkage=external];
// X86-64-ANDROID-CLANG-NEXT:     fn %6 @record(%7 value: @type0) -> @type0 [linkage=external] [abi=sysv64(coerce<i64>) -> coerce<i64>] [fallthrough=ub_if_used] {
// X86-64-ANDROID-CLANG-NEXT:         return copy<@type0, reason=return>(read<@type0>(%7));
// X86-64-ANDROID-CLANG-NEXT:     }
// X86-64-ANDROID-CLANG-NEXT: }
// SLATE-FILECHECK-END X86-64-ANDROID-CLANG
// SLATE-FILECHECK-BEGIN AARCH64-FREEBSD-CLANG
// AARCH64-FREEBSD-CLANG: module {
// AARCH64-FREEBSD-CLANG-NEXT:     target "aarch64-unknown-freebsd" {
// AARCH64-FREEBSD-CLANG-NEXT:         endian = little;
// AARCH64-FREEBSD-CLANG-NEXT:         pointer [size=8, align=8];
// AARCH64-FREEBSD-CLANG-NEXT:         stack_alignment = 16;
// AARCH64-FREEBSD-CLANG-NEXT:         long_double = f128;
// AARCH64-FREEBSD-CLANG-NEXT:         storage bool [size=1, align=1];
// AARCH64-FREEBSD-CLANG-NEXT:         storage i8, u8 [size=1, align=1];
// AARCH64-FREEBSD-CLANG-NEXT:         storage i16, u16 [size=2, align=2];
// AARCH64-FREEBSD-CLANG-NEXT:         storage i32, u32 [size=4, align=4];
// AARCH64-FREEBSD-CLANG-NEXT:         storage i64, u64 [size=8, align=8];
// AARCH64-FREEBSD-CLANG-NEXT:         storage i128, u128 [size=16, align=16];
// AARCH64-FREEBSD-CLANG-NEXT:         storage bf16 [size=2, align=2];
// AARCH64-FREEBSD-CLANG-NEXT:         storage f16 [size=2, align=2];
// AARCH64-FREEBSD-CLANG-NEXT:         storage f32 [size=4, align=4];
// AARCH64-FREEBSD-CLANG-NEXT:         storage f64 [size=8, align=8];
// AARCH64-FREEBSD-CLANG-NEXT:         storage f128 [size=16, align=16];
// AARCH64-FREEBSD-CLANG-NEXT:         storage d32 [size=4, align=4];
// AARCH64-FREEBSD-CLANG-NEXT:         storage d64 [size=8, align=8];
// AARCH64-FREEBSD-CLANG-NEXT:         storage d128 [size=16, align=16];
// AARCH64-FREEBSD-CLANG-NEXT:     }
// AARCH64-FREEBSD-CLANG-NEXT:     type @type0 pair = struct {
// AARCH64-FREEBSD-CLANG-NEXT:         field0 a: i32;
// AARCH64-FREEBSD-CLANG-NEXT:         field1 b: i32;
// AARCH64-FREEBSD-CLANG-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// AARCH64-FREEBSD-CLANG-NEXT:     global %0 clang_major: i32 [storage=static] = const<i32>(22) [linkage=external];
// AARCH64-FREEBSD-CLANG-NEXT:     global %1 sizeof_long: u64 [storage=static] = const<u64>(8) [linkage=external];
// AARCH64-FREEBSD-CLANG-NEXT:     global %2 sizeof_long_double: u64 [storage=static] = const<u64>(16) [linkage=external];
// AARCH64-FREEBSD-CLANG-NEXT:     global %3 sizeof_va_list: u64 [storage=static] = const<u64>(32) [linkage=external];
// AARCH64-FREEBSD-CLANG-NEXT:     global %4 alignof_long_long: u64 [storage=static] = const<u64>(8) [linkage=external];
// AARCH64-FREEBSD-CLANG-NEXT:     fn %6 @record(%7 value: @type0) -> @type0 [linkage=external] [abi=aapcs64(coerce<i64>) -> coerce<i64>] [fallthrough=ub_if_used] {
// AARCH64-FREEBSD-CLANG-NEXT:         return copy<@type0, reason=return>(read<@type0>(%7));
// AARCH64-FREEBSD-CLANG-NEXT:     }
// AARCH64-FREEBSD-CLANG-NEXT: }
// SLATE-FILECHECK-END AARCH64-FREEBSD-CLANG
// SLATE-FILECHECK-BEGIN X86-64-FREEBSD-CLANG
// X86-64-FREEBSD-CLANG: module {
// X86-64-FREEBSD-CLANG-NEXT:     target "x86_64-unknown-freebsd" {
// X86-64-FREEBSD-CLANG-NEXT:         endian = little;
// X86-64-FREEBSD-CLANG-NEXT:         pointer [size=8, align=8];
// X86-64-FREEBSD-CLANG-NEXT:         stack_alignment = 16;
// X86-64-FREEBSD-CLANG-NEXT:         long_double = f80;
// X86-64-FREEBSD-CLANG-NEXT:         storage bool [size=1, align=1];
// X86-64-FREEBSD-CLANG-NEXT:         storage i8, u8 [size=1, align=1];
// X86-64-FREEBSD-CLANG-NEXT:         storage i16, u16 [size=2, align=2];
// X86-64-FREEBSD-CLANG-NEXT:         storage i32, u32 [size=4, align=4];
// X86-64-FREEBSD-CLANG-NEXT:         storage i64, u64 [size=8, align=8];
// X86-64-FREEBSD-CLANG-NEXT:         storage i128, u128 [size=16, align=16];
// X86-64-FREEBSD-CLANG-NEXT:         storage bf16 [size=2, align=2];
// X86-64-FREEBSD-CLANG-NEXT:         storage f16 [size=2, align=2];
// X86-64-FREEBSD-CLANG-NEXT:         storage f32 [size=4, align=4];
// X86-64-FREEBSD-CLANG-NEXT:         storage f64 [size=8, align=8];
// X86-64-FREEBSD-CLANG-NEXT:         storage f80 [size=16, align=16];
// X86-64-FREEBSD-CLANG-NEXT:         storage f128 [size=16, align=16];
// X86-64-FREEBSD-CLANG-NEXT:         storage d32 [size=4, align=4];
// X86-64-FREEBSD-CLANG-NEXT:         storage d64 [size=8, align=8];
// X86-64-FREEBSD-CLANG-NEXT:         storage d128 [size=16, align=16];
// X86-64-FREEBSD-CLANG-NEXT:     }
// X86-64-FREEBSD-CLANG-NEXT:     type @type0 pair = struct {
// X86-64-FREEBSD-CLANG-NEXT:         field0 a: i32;
// X86-64-FREEBSD-CLANG-NEXT:         field1 b: i32;
// X86-64-FREEBSD-CLANG-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// X86-64-FREEBSD-CLANG-NEXT:     global %0 clang_major: i32 [storage=static] = const<i32>(22) [linkage=external];
// X86-64-FREEBSD-CLANG-NEXT:     global %1 sizeof_long: u64 [storage=static] = const<u64>(8) [linkage=external];
// X86-64-FREEBSD-CLANG-NEXT:     global %2 sizeof_long_double: u64 [storage=static] = const<u64>(16) [linkage=external];
// X86-64-FREEBSD-CLANG-NEXT:     global %3 sizeof_va_list: u64 [storage=static] = const<u64>(24) [linkage=external];
// X86-64-FREEBSD-CLANG-NEXT:     global %4 alignof_long_long: u64 [storage=static] = const<u64>(8) [linkage=external];
// X86-64-FREEBSD-CLANG-NEXT:     fn %6 @record(%7 value: @type0) -> @type0 [linkage=external] [abi=sysv64(coerce<i64>) -> coerce<i64>] [fallthrough=ub_if_used] {
// X86-64-FREEBSD-CLANG-NEXT:         return copy<@type0, reason=return>(read<@type0>(%7));
// X86-64-FREEBSD-CLANG-NEXT:     }
// X86-64-FREEBSD-CLANG-NEXT: }
// SLATE-FILECHECK-END X86-64-FREEBSD-CLANG
// SLATE-FILECHECK-BEGIN X86-64-WINDOWS-MSVC-CLANG
// X86-64-WINDOWS-MSVC-CLANG: module {
// X86-64-WINDOWS-MSVC-CLANG-NEXT:     target "x86_64-pc-windows-msvc" {
// X86-64-WINDOWS-MSVC-CLANG-NEXT:         endian = little;
// X86-64-WINDOWS-MSVC-CLANG-NEXT:         pointer [size=8, align=8];
// X86-64-WINDOWS-MSVC-CLANG-NEXT:         stack_alignment = 16;
// X86-64-WINDOWS-MSVC-CLANG-NEXT:         long_double = f64;
// X86-64-WINDOWS-MSVC-CLANG-NEXT:         storage bool [size=1, align=1];
// X86-64-WINDOWS-MSVC-CLANG-NEXT:         storage i8, u8 [size=1, align=1];
// X86-64-WINDOWS-MSVC-CLANG-NEXT:         storage i16, u16 [size=2, align=2];
// X86-64-WINDOWS-MSVC-CLANG-NEXT:         storage i32, u32 [size=4, align=4];
// X86-64-WINDOWS-MSVC-CLANG-NEXT:         storage i64, u64 [size=8, align=8];
// X86-64-WINDOWS-MSVC-CLANG-NEXT:         storage i128, u128 [size=16, align=16];
// X86-64-WINDOWS-MSVC-CLANG-NEXT:         storage bf16 [size=2, align=2];
// X86-64-WINDOWS-MSVC-CLANG-NEXT:         storage f16 [size=2, align=2];
// X86-64-WINDOWS-MSVC-CLANG-NEXT:         storage f32 [size=4, align=4];
// X86-64-WINDOWS-MSVC-CLANG-NEXT:         storage f64 [size=8, align=8];
// X86-64-WINDOWS-MSVC-CLANG-NEXT:         storage f128 [size=16, align=16];
// X86-64-WINDOWS-MSVC-CLANG-NEXT:         storage d32 [size=4, align=4];
// X86-64-WINDOWS-MSVC-CLANG-NEXT:         storage d64 [size=8, align=8];
// X86-64-WINDOWS-MSVC-CLANG-NEXT:         storage d128 [size=16, align=16];
// X86-64-WINDOWS-MSVC-CLANG-NEXT:     }
// X86-64-WINDOWS-MSVC-CLANG-NEXT:     type @type0 pair = struct {
// X86-64-WINDOWS-MSVC-CLANG-NEXT:         field0 a: i32;
// X86-64-WINDOWS-MSVC-CLANG-NEXT:         field1 b: i32;
// X86-64-WINDOWS-MSVC-CLANG-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// X86-64-WINDOWS-MSVC-CLANG-NEXT:     global %0 msc_ver: i32 [storage=static] = const<i32>(1940) [linkage=external];
// X86-64-WINDOWS-MSVC-CLANG-NEXT:     global %1 sizeof_long: u32 [storage=static] = truncate<u32>(const<u64>(4)) [linkage=external];
// X86-64-WINDOWS-MSVC-CLANG-NEXT:     global %2 sizeof_long_double: u32 [storage=static] = truncate<u32>(const<u64>(8)) [linkage=external];
// X86-64-WINDOWS-MSVC-CLANG-NEXT:     global %3 sizeof_va_list: u32 [storage=static] = truncate<u32>(const<u64>(8)) [linkage=external];
// X86-64-WINDOWS-MSVC-CLANG-NEXT:     global %4 alignof_long_long: u32 [storage=static] = truncate<u32>(const<u64>(8)) [linkage=external];
// X86-64-WINDOWS-MSVC-CLANG-NEXT:     fn %6 @record(%7 value: @type0) -> @type0 [linkage=external] [abi=win64(coerce<i64>) -> coerce<i64>] [fallthrough=ub_if_used] {
// X86-64-WINDOWS-MSVC-CLANG-NEXT:         return copy<@type0, reason=return>(read<@type0>(%7));
// X86-64-WINDOWS-MSVC-CLANG-NEXT:     }
// X86-64-WINDOWS-MSVC-CLANG-NEXT: }
// SLATE-FILECHECK-END X86-64-WINDOWS-MSVC-CLANG
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
// X86-64-WINDOWS-MSVC-MSVC-NEXT:     fn %6 @record(%7 value: @type0) -> @type0 [linkage=external] [abi=win64(coerce<i64>) -> coerce<i64>] [fallthrough=ub_if_used] {
// X86-64-WINDOWS-MSVC-MSVC-NEXT:         return copy<@type0, reason=return>(read<@type0>(%7));
// X86-64-WINDOWS-MSVC-MSVC-NEXT:     }
// X86-64-WINDOWS-MSVC-MSVC-NEXT: }
// SLATE-FILECHECK-END X86-64-WINDOWS-MSVC-MSVC
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
// AARCH64-WINDOWS-MSVC-CLANG-NEXT:     fn %6 @record(%7 value: @type0) -> @type0 [linkage=external] [abi=win_arm64(coerce<i64>) -> coerce<i64>] [fallthrough=ub_if_used] {
// AARCH64-WINDOWS-MSVC-CLANG-NEXT:         return copy<@type0, reason=return>(read<@type0>(%7));
// AARCH64-WINDOWS-MSVC-CLANG-NEXT:     }
// AARCH64-WINDOWS-MSVC-CLANG-NEXT: }
// SLATE-FILECHECK-END AARCH64-WINDOWS-MSVC-CLANG
// SLATE-FILECHECK-BEGIN AARCH64-WINDOWS-MSVC-MSVC
// AARCH64-WINDOWS-MSVC-MSVC: module {
// AARCH64-WINDOWS-MSVC-MSVC-NEXT:     target "aarch64-pc-windows-msvc" {
// AARCH64-WINDOWS-MSVC-MSVC-NEXT:         endian = little;
// AARCH64-WINDOWS-MSVC-MSVC-NEXT:         pointer [size=8, align=8];
// AARCH64-WINDOWS-MSVC-MSVC-NEXT:         stack_alignment = 16;
// AARCH64-WINDOWS-MSVC-MSVC-NEXT:         long_double = f64;
// AARCH64-WINDOWS-MSVC-MSVC-NEXT:         storage bool [size=1, align=1];
// AARCH64-WINDOWS-MSVC-MSVC-NEXT:         storage i8, u8 [size=1, align=1];
// AARCH64-WINDOWS-MSVC-MSVC-NEXT:         storage i16, u16 [size=2, align=2];
// AARCH64-WINDOWS-MSVC-MSVC-NEXT:         storage i32, u32 [size=4, align=4];
// AARCH64-WINDOWS-MSVC-MSVC-NEXT:         storage i64, u64 [size=8, align=8];
// AARCH64-WINDOWS-MSVC-MSVC-NEXT:         storage i128, u128 [size=16, align=16];
// AARCH64-WINDOWS-MSVC-MSVC-NEXT:         storage bf16 [size=2, align=2];
// AARCH64-WINDOWS-MSVC-MSVC-NEXT:         storage f16 [size=2, align=2];
// AARCH64-WINDOWS-MSVC-MSVC-NEXT:         storage f32 [size=4, align=4];
// AARCH64-WINDOWS-MSVC-MSVC-NEXT:         storage f64 [size=8, align=8];
// AARCH64-WINDOWS-MSVC-MSVC-NEXT:         storage f128 [size=16, align=16];
// AARCH64-WINDOWS-MSVC-MSVC-NEXT:         storage d32 [size=4, align=4];
// AARCH64-WINDOWS-MSVC-MSVC-NEXT:         storage d64 [size=8, align=8];
// AARCH64-WINDOWS-MSVC-MSVC-NEXT:         storage d128 [size=16, align=16];
// AARCH64-WINDOWS-MSVC-MSVC-NEXT:     }
// AARCH64-WINDOWS-MSVC-MSVC-NEXT:     type @type0 pair = struct {
// AARCH64-WINDOWS-MSVC-MSVC-NEXT:         field0 a: i32;
// AARCH64-WINDOWS-MSVC-MSVC-NEXT:         field1 b: i32;
// AARCH64-WINDOWS-MSVC-MSVC-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// AARCH64-WINDOWS-MSVC-MSVC-NEXT:     global %0 msc_ver: i32 [storage=static] = const<i32>(1951) [linkage=external];
// AARCH64-WINDOWS-MSVC-MSVC-NEXT:     global %1 sizeof_long: u32 [storage=static] = truncate<u32>(const<u64>(4)) [linkage=external];
// AARCH64-WINDOWS-MSVC-MSVC-NEXT:     global %2 sizeof_long_double: u32 [storage=static] = truncate<u32>(const<u64>(8)) [linkage=external];
// AARCH64-WINDOWS-MSVC-MSVC-NEXT:     global %3 sizeof_va_list: u32 [storage=static] = truncate<u32>(const<u64>(8)) [linkage=external];
// AARCH64-WINDOWS-MSVC-MSVC-NEXT:     global %4 alignof_long_long: u32 [storage=static] = truncate<u32>(const<u64>(8)) [linkage=external];
// AARCH64-WINDOWS-MSVC-MSVC-NEXT:     fn %6 @record(%7 value: @type0) -> @type0 [linkage=external] [abi=win_arm64(coerce<i64>) -> coerce<i64>] [fallthrough=ub_if_used] {
// AARCH64-WINDOWS-MSVC-MSVC-NEXT:         return copy<@type0, reason=return>(read<@type0>(%7));
// AARCH64-WINDOWS-MSVC-MSVC-NEXT:     }
// AARCH64-WINDOWS-MSVC-MSVC-NEXT: }
// SLATE-FILECHECK-END AARCH64-WINDOWS-MSVC-MSVC
