#define ATTR(name) __attribute__((name))
ATTR(preserve_most) void most(void);
ATTR(preserve_all) void all(void);
ATTR(preserve_none) void none(void);
ATTR(address_space(1 + 2)) int *device_memory;
ATTR(overloadable) int overloaded(int);
ATTR(overloadable) int overloaded(double);
ATTR(optnone) int unoptimized(int);
ATTR(annotate("marker")) int annotated;
ATTR(availability(macos, introduced=12.0)) int platform_api(void);
ATTR(cpu_dispatch(generic, haswell)) int dispatch(void);
ATTR(cpu_specific(haswell)) int specialized(void);
typedef int int2 ATTR(ext_vector_type(2));
ATTR(weak_import) int weak_platform;
ATTR(noinline) void noinline_fn(void);
ATTR(always_inline) inline void inline_fn(void);
// SLATE-FILECHECK-DEFINES GNU
// SLATE-FILECHECK-STD GNU c23
// SLATE-FILECHECK-FLAVOR clang

// SLATE-FILECHECK-BEGIN GNU
// GNU: module {
// GNU-NEXT:     target "x86_64-unknown-linux-gnu" {
// GNU-NEXT:         endian = little;
// GNU-NEXT:         pointer [size=8, align=8];
// GNU-NEXT:         stack_alignment = 16;
// GNU-NEXT:         long_double = f80;
// GNU-NEXT:         storage bool [size=1, align=1];
// GNU-NEXT:         storage i8, u8 [size=1, align=1];
// GNU-NEXT:         storage i16, u16 [size=2, align=2];
// GNU-NEXT:         storage i32, u32 [size=4, align=4];
// GNU-NEXT:         storage i64, u64 [size=8, align=8];
// GNU-NEXT:         storage i128, u128 [size=16, align=16];
// GNU-NEXT:         storage bf16 [size=2, align=2];
// GNU-NEXT:         storage f16 [size=2, align=2];
// GNU-NEXT:         storage f32 [size=4, align=4];
// GNU-NEXT:         storage f64 [size=8, align=8];
// GNU-NEXT:         storage f80 [size=16, align=16];
// GNU-NEXT:         storage f128 [size=16, align=16];
// GNU-NEXT:         storage d32 [size=4, align=4];
// GNU-NEXT:         storage d64 [size=8, align=8];
// GNU-NEXT:         storage d128 [size=16, align=16];
// GNU-NEXT:     }
// GNU-NEXT:     type @type0 int2 = vector<i32, 2>;
// GNU-NEXT:     global %3 device_memory: ptr<i32> [storage=static] [linkage=external];
// GNU-NEXT:     global %6 annotated: i32 [storage=static] [linkage=external];
// GNU-NEXT:     global %11 weak_platform: i32 [storage=static] [linkage=external];
// GNU-NEXT:     fn %0 @most() -> void [linkage=external];
// GNU-NEXT:     fn %1 @all() -> void [linkage=external];
// GNU-NEXT:     fn %2 @none() -> void [linkage=external];
// GNU-NEXT:     fn %4 @overloaded(%14 <unnamed>: i32) -> i32 [linkage=external];
// GNU-NEXT:     fn %5 @unoptimized(%16 <unnamed>: i32) -> i32 [linkage=external];
// GNU-NEXT:     fn %7 @platform_api() -> i32 [linkage=external];
// GNU-NEXT:     fn %8 @dispatch() -> i32 [linkage=external];
// GNU-NEXT:     fn %9 @specialized() -> i32 [linkage=external];
// GNU-NEXT:     fn %12 @noinline_fn() -> void [linkage=external] [inline=never];
// GNU-NEXT:     fn %13 @inline_fn() -> void [linkage=external] [inline=always];
// GNU-NEXT: }
// SLATE-FILECHECK-END GNU
