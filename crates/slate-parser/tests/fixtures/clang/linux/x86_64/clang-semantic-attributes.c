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
// GNU-NEXT:     type @type[[TYPE_int2:[0-9]+]] int2 = vector<i32, 2>;
// GNU-NEXT:     global %[[VALUE_device_memory:[0-9]+]] device_memory: ptr<i32> [storage=static] [linkage=external];
// GNU-NEXT:     global %[[VALUE_annotated:[0-9]+]] annotated: i32 [storage=static] [linkage=external];
// GNU-NEXT:     global %[[VALUE_weak_platform:[0-9]+]] weak_platform: i32 [storage=static] [linkage=external];
// GNU-NEXT:     fn %[[VALUE_most:[0-9]+]] @most() -> void [linkage=external];
// GNU-NEXT:     fn %[[VALUE_all:[0-9]+]] @all() -> void [linkage=external];
// GNU-NEXT:     fn %[[VALUE_none:[0-9]+]] @none() -> void [linkage=external];
// GNU-NEXT:     fn %[[VALUE_overloaded:[0-9]+]] @overloaded(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external];
// GNU-NEXT:     fn %[[VALUE_unoptimized:[0-9]+]] @unoptimized(%[[VALUE1:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external];
// GNU-NEXT:     fn %[[VALUE_platform_api:[0-9]+]] @platform_api() -> i32 [linkage=external];
// GNU-NEXT:     fn %[[VALUE_dispatch:[0-9]+]] @dispatch() -> i32 [linkage=external];
// GNU-NEXT:     fn %[[VALUE_specialized:[0-9]+]] @specialized() -> i32 [linkage=external];
// GNU-NEXT:     fn %[[VALUE_noinline_fn:[0-9]+]] @noinline_fn() -> void [linkage=external] [inline=never];
// GNU-NEXT:     fn %[[VALUE_inline_fn:[0-9]+]] @inline_fn() -> void [linkage=external] [inline=always];
// GNU-NEXT: }
// SLATE-FILECHECK-END GNU
