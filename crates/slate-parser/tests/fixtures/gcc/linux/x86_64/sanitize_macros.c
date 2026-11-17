#ifdef __SANITIZE_ADDRESS__
int macro_address = __SANITIZE_ADDRESS__;
#else
int macro_address = -1;
#endif

#ifdef __SANITIZE_HWADDRESS__
int macro_hwaddress = __SANITIZE_HWADDRESS__;
#else
int macro_hwaddress = -1;
#endif

#ifdef __SANITIZE_THREAD__
int macro_thread = __SANITIZE_THREAD__;
#else
int macro_thread = -1;
#endif

int feature_address = __has_feature(address_sanitizer);
int feature_hwaddress = __has_feature(hwaddress_sanitizer);
int feature_memory = __has_feature(memory_sanitizer);
int feature_thread = __has_feature(thread_sanitizer);
int feature_undefined = __has_feature(undefined_behavior_sanitizer);
int feature_leak = __has_feature(leak_sanitizer);

// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-PREFIX-ARGS DEFAULT
// SLATE-FILECHECK-DEFINES ASAN_UBSAN
// SLATE-FILECHECK-PREFIX-ARGS ASAN_UBSAN -fsanitize=address -fsanitize=undefined
// SLATE-FILECHECK-DEFINES KASAN
// SLATE-FILECHECK-PREFIX-ARGS KASAN -fsanitize=kernel-address
// SLATE-FILECHECK-DEFINES HWASAN
// SLATE-FILECHECK-PREFIX-ARGS HWASAN -fsanitize=kernel-hwaddress
// SLATE-FILECHECK-DEFINES TSAN
// SLATE-FILECHECK-PREFIX-ARGS TSAN -fsanitize=thread
// SLATE-FILECHECK-DEFINES LEAK
// SLATE-FILECHECK-PREFIX-ARGS LEAK -fsanitize=leak
// SLATE-FILECHECK-DEFINES BOUNDS_STRICT
// SLATE-FILECHECK-PREFIX-ARGS BOUNDS_STRICT -fsanitize=bounds-strict
// SLATE-FILECHECK-DEFINES FLOAT_ONLY
// SLATE-FILECHECK-PREFIX-ARGS FLOAT_ONLY -fsanitize=float-cast-overflow,float-divide-by-zero
// SLATE-FILECHECK-DEFINES VPTR
// SLATE-FILECHECK-PREFIX-ARGS VPTR -fsanitize=vptr
// SLATE-FILECHECK-DEFINES POINTER_COMPARE
// SLATE-FILECHECK-PREFIX-ARGS POINTER_COMPARE -fsanitize=pointer-compare -fsanitize=kernel-address
// SLATE-FILECHECK-DEFINES NONE_LEFT
// SLATE-FILECHECK-PREFIX-ARGS NONE_LEFT -fsanitize=address,undefined -fno-sanitize=all
// SLATE-FILECHECK-DEFINES RUNTIME_OPTIONS
// SLATE-FILECHECK-PREFIX-ARGS RUNTIME_OPTIONS -fsanitize=address -fsanitize-recover=address -fno-sanitize-recover=all -fsanitize-trap=undefined -fsanitize-address-use-after-scope -fsanitize-undefined-trap-on-error

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     global %[[VALUE_macro_address:[0-9]+]] macro_address: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_macro_hwaddress:[0-9]+]] macro_hwaddress: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_macro_thread:[0-9]+]] macro_thread: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_feature_address:[0-9]+]] feature_address: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_feature_hwaddress:[0-9]+]] feature_hwaddress: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_feature_memory:[0-9]+]] feature_memory: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_feature_thread:[0-9]+]] feature_thread: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_feature_undefined:[0-9]+]] feature_undefined: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_feature_leak:[0-9]+]] feature_leak: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN ASAN_UBSAN
// ASAN_UBSAN: module {
// ASAN_UBSAN-NEXT:     target "x86_64-unknown-linux-gnu" {
// ASAN_UBSAN-NEXT:         endian = little;
// ASAN_UBSAN-NEXT:         pointer [size=8, align=8];
// ASAN_UBSAN-NEXT:         stack_alignment = 16;
// ASAN_UBSAN-NEXT:         long_double = f80;
// ASAN_UBSAN-NEXT:         storage bool [size=1, align=1];
// ASAN_UBSAN-NEXT:         storage i8, u8 [size=1, align=1];
// ASAN_UBSAN-NEXT:         storage i16, u16 [size=2, align=2];
// ASAN_UBSAN-NEXT:         storage i32, u32 [size=4, align=4];
// ASAN_UBSAN-NEXT:         storage i64, u64 [size=8, align=8];
// ASAN_UBSAN-NEXT:         storage i128, u128 [size=16, align=16];
// ASAN_UBSAN-NEXT:         storage bf16 [size=2, align=2];
// ASAN_UBSAN-NEXT:         storage f16 [size=2, align=2];
// ASAN_UBSAN-NEXT:         storage f32 [size=4, align=4];
// ASAN_UBSAN-NEXT:         storage f64 [size=8, align=8];
// ASAN_UBSAN-NEXT:         storage f80 [size=16, align=16];
// ASAN_UBSAN-NEXT:         storage f128 [size=16, align=16];
// ASAN_UBSAN-NEXT:         storage d32 [size=4, align=4];
// ASAN_UBSAN-NEXT:         storage d64 [size=8, align=8];
// ASAN_UBSAN-NEXT:         storage d128 [size=16, align=16];
// ASAN_UBSAN-NEXT:     }
// ASAN_UBSAN-NEXT:     global %[[VALUE_macro_address:[0-9]+]] macro_address: i32 [storage=static] = const<i32>(1) [linkage=external];
// ASAN_UBSAN-NEXT:     global %[[VALUE_macro_hwaddress:[0-9]+]] macro_hwaddress: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// ASAN_UBSAN-NEXT:     global %[[VALUE_macro_thread:[0-9]+]] macro_thread: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// ASAN_UBSAN-NEXT:     global %[[VALUE_feature_address:[0-9]+]] feature_address: i32 [storage=static] = const<i32>(1) [linkage=external];
// ASAN_UBSAN-NEXT:     global %[[VALUE_feature_hwaddress:[0-9]+]] feature_hwaddress: i32 [storage=static] = const<i32>(0) [linkage=external];
// ASAN_UBSAN-NEXT:     global %[[VALUE_feature_memory:[0-9]+]] feature_memory: i32 [storage=static] = const<i32>(0) [linkage=external];
// ASAN_UBSAN-NEXT:     global %[[VALUE_feature_thread:[0-9]+]] feature_thread: i32 [storage=static] = const<i32>(0) [linkage=external];
// ASAN_UBSAN-NEXT:     global %[[VALUE_feature_undefined:[0-9]+]] feature_undefined: i32 [storage=static] = const<i32>(1) [linkage=external];
// ASAN_UBSAN-NEXT:     global %[[VALUE_feature_leak:[0-9]+]] feature_leak: i32 [storage=static] = const<i32>(0) [linkage=external];
// ASAN_UBSAN-NEXT: }
// SLATE-FILECHECK-END ASAN_UBSAN
// SLATE-FILECHECK-BEGIN KASAN
// KASAN: module {
// KASAN-NEXT:     target "x86_64-unknown-linux-gnu" {
// KASAN-NEXT:         endian = little;
// KASAN-NEXT:         pointer [size=8, align=8];
// KASAN-NEXT:         stack_alignment = 16;
// KASAN-NEXT:         long_double = f80;
// KASAN-NEXT:         storage bool [size=1, align=1];
// KASAN-NEXT:         storage i8, u8 [size=1, align=1];
// KASAN-NEXT:         storage i16, u16 [size=2, align=2];
// KASAN-NEXT:         storage i32, u32 [size=4, align=4];
// KASAN-NEXT:         storage i64, u64 [size=8, align=8];
// KASAN-NEXT:         storage i128, u128 [size=16, align=16];
// KASAN-NEXT:         storage bf16 [size=2, align=2];
// KASAN-NEXT:         storage f16 [size=2, align=2];
// KASAN-NEXT:         storage f32 [size=4, align=4];
// KASAN-NEXT:         storage f64 [size=8, align=8];
// KASAN-NEXT:         storage f80 [size=16, align=16];
// KASAN-NEXT:         storage f128 [size=16, align=16];
// KASAN-NEXT:         storage d32 [size=4, align=4];
// KASAN-NEXT:         storage d64 [size=8, align=8];
// KASAN-NEXT:         storage d128 [size=16, align=16];
// KASAN-NEXT:     }
// KASAN-NEXT:     global %[[VALUE_macro_address:[0-9]+]] macro_address: i32 [storage=static] = const<i32>(1) [linkage=external];
// KASAN-NEXT:     global %[[VALUE_macro_hwaddress:[0-9]+]] macro_hwaddress: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// KASAN-NEXT:     global %[[VALUE_macro_thread:[0-9]+]] macro_thread: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// KASAN-NEXT:     global %[[VALUE_feature_address:[0-9]+]] feature_address: i32 [storage=static] = const<i32>(1) [linkage=external];
// KASAN-NEXT:     global %[[VALUE_feature_hwaddress:[0-9]+]] feature_hwaddress: i32 [storage=static] = const<i32>(0) [linkage=external];
// KASAN-NEXT:     global %[[VALUE_feature_memory:[0-9]+]] feature_memory: i32 [storage=static] = const<i32>(0) [linkage=external];
// KASAN-NEXT:     global %[[VALUE_feature_thread:[0-9]+]] feature_thread: i32 [storage=static] = const<i32>(0) [linkage=external];
// KASAN-NEXT:     global %[[VALUE_feature_undefined:[0-9]+]] feature_undefined: i32 [storage=static] = const<i32>(0) [linkage=external];
// KASAN-NEXT:     global %[[VALUE_feature_leak:[0-9]+]] feature_leak: i32 [storage=static] = const<i32>(0) [linkage=external];
// KASAN-NEXT: }
// SLATE-FILECHECK-END KASAN
// SLATE-FILECHECK-BEGIN HWASAN
// HWASAN: module {
// HWASAN-NEXT:     target "x86_64-unknown-linux-gnu" {
// HWASAN-NEXT:         endian = little;
// HWASAN-NEXT:         pointer [size=8, align=8];
// HWASAN-NEXT:         stack_alignment = 16;
// HWASAN-NEXT:         long_double = f80;
// HWASAN-NEXT:         storage bool [size=1, align=1];
// HWASAN-NEXT:         storage i8, u8 [size=1, align=1];
// HWASAN-NEXT:         storage i16, u16 [size=2, align=2];
// HWASAN-NEXT:         storage i32, u32 [size=4, align=4];
// HWASAN-NEXT:         storage i64, u64 [size=8, align=8];
// HWASAN-NEXT:         storage i128, u128 [size=16, align=16];
// HWASAN-NEXT:         storage bf16 [size=2, align=2];
// HWASAN-NEXT:         storage f16 [size=2, align=2];
// HWASAN-NEXT:         storage f32 [size=4, align=4];
// HWASAN-NEXT:         storage f64 [size=8, align=8];
// HWASAN-NEXT:         storage f80 [size=16, align=16];
// HWASAN-NEXT:         storage f128 [size=16, align=16];
// HWASAN-NEXT:         storage d32 [size=4, align=4];
// HWASAN-NEXT:         storage d64 [size=8, align=8];
// HWASAN-NEXT:         storage d128 [size=16, align=16];
// HWASAN-NEXT:     }
// HWASAN-NEXT:     global %[[VALUE_macro_address:[0-9]+]] macro_address: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// HWASAN-NEXT:     global %[[VALUE_macro_hwaddress:[0-9]+]] macro_hwaddress: i32 [storage=static] = const<i32>(1) [linkage=external];
// HWASAN-NEXT:     global %[[VALUE_macro_thread:[0-9]+]] macro_thread: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// HWASAN-NEXT:     global %[[VALUE_feature_address:[0-9]+]] feature_address: i32 [storage=static] = const<i32>(0) [linkage=external];
// HWASAN-NEXT:     global %[[VALUE_feature_hwaddress:[0-9]+]] feature_hwaddress: i32 [storage=static] = const<i32>(1) [linkage=external];
// HWASAN-NEXT:     global %[[VALUE_feature_memory:[0-9]+]] feature_memory: i32 [storage=static] = const<i32>(0) [linkage=external];
// HWASAN-NEXT:     global %[[VALUE_feature_thread:[0-9]+]] feature_thread: i32 [storage=static] = const<i32>(0) [linkage=external];
// HWASAN-NEXT:     global %[[VALUE_feature_undefined:[0-9]+]] feature_undefined: i32 [storage=static] = const<i32>(0) [linkage=external];
// HWASAN-NEXT:     global %[[VALUE_feature_leak:[0-9]+]] feature_leak: i32 [storage=static] = const<i32>(0) [linkage=external];
// HWASAN-NEXT: }
// SLATE-FILECHECK-END HWASAN
// SLATE-FILECHECK-BEGIN TSAN
// TSAN: module {
// TSAN-NEXT:     target "x86_64-unknown-linux-gnu" {
// TSAN-NEXT:         endian = little;
// TSAN-NEXT:         pointer [size=8, align=8];
// TSAN-NEXT:         stack_alignment = 16;
// TSAN-NEXT:         long_double = f80;
// TSAN-NEXT:         storage bool [size=1, align=1];
// TSAN-NEXT:         storage i8, u8 [size=1, align=1];
// TSAN-NEXT:         storage i16, u16 [size=2, align=2];
// TSAN-NEXT:         storage i32, u32 [size=4, align=4];
// TSAN-NEXT:         storage i64, u64 [size=8, align=8];
// TSAN-NEXT:         storage i128, u128 [size=16, align=16];
// TSAN-NEXT:         storage bf16 [size=2, align=2];
// TSAN-NEXT:         storage f16 [size=2, align=2];
// TSAN-NEXT:         storage f32 [size=4, align=4];
// TSAN-NEXT:         storage f64 [size=8, align=8];
// TSAN-NEXT:         storage f80 [size=16, align=16];
// TSAN-NEXT:         storage f128 [size=16, align=16];
// TSAN-NEXT:         storage d32 [size=4, align=4];
// TSAN-NEXT:         storage d64 [size=8, align=8];
// TSAN-NEXT:         storage d128 [size=16, align=16];
// TSAN-NEXT:     }
// TSAN-NEXT:     global %[[VALUE_macro_address:[0-9]+]] macro_address: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// TSAN-NEXT:     global %[[VALUE_macro_hwaddress:[0-9]+]] macro_hwaddress: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// TSAN-NEXT:     global %[[VALUE_macro_thread:[0-9]+]] macro_thread: i32 [storage=static] = const<i32>(1) [linkage=external];
// TSAN-NEXT:     global %[[VALUE_feature_address:[0-9]+]] feature_address: i32 [storage=static] = const<i32>(0) [linkage=external];
// TSAN-NEXT:     global %[[VALUE_feature_hwaddress:[0-9]+]] feature_hwaddress: i32 [storage=static] = const<i32>(0) [linkage=external];
// TSAN-NEXT:     global %[[VALUE_feature_memory:[0-9]+]] feature_memory: i32 [storage=static] = const<i32>(0) [linkage=external];
// TSAN-NEXT:     global %[[VALUE_feature_thread:[0-9]+]] feature_thread: i32 [storage=static] = const<i32>(1) [linkage=external];
// TSAN-NEXT:     global %[[VALUE_feature_undefined:[0-9]+]] feature_undefined: i32 [storage=static] = const<i32>(0) [linkage=external];
// TSAN-NEXT:     global %[[VALUE_feature_leak:[0-9]+]] feature_leak: i32 [storage=static] = const<i32>(0) [linkage=external];
// TSAN-NEXT: }
// SLATE-FILECHECK-END TSAN
// SLATE-FILECHECK-BEGIN LEAK
// LEAK: module {
// LEAK-NEXT:     target "x86_64-unknown-linux-gnu" {
// LEAK-NEXT:         endian = little;
// LEAK-NEXT:         pointer [size=8, align=8];
// LEAK-NEXT:         stack_alignment = 16;
// LEAK-NEXT:         long_double = f80;
// LEAK-NEXT:         storage bool [size=1, align=1];
// LEAK-NEXT:         storage i8, u8 [size=1, align=1];
// LEAK-NEXT:         storage i16, u16 [size=2, align=2];
// LEAK-NEXT:         storage i32, u32 [size=4, align=4];
// LEAK-NEXT:         storage i64, u64 [size=8, align=8];
// LEAK-NEXT:         storage i128, u128 [size=16, align=16];
// LEAK-NEXT:         storage bf16 [size=2, align=2];
// LEAK-NEXT:         storage f16 [size=2, align=2];
// LEAK-NEXT:         storage f32 [size=4, align=4];
// LEAK-NEXT:         storage f64 [size=8, align=8];
// LEAK-NEXT:         storage f80 [size=16, align=16];
// LEAK-NEXT:         storage f128 [size=16, align=16];
// LEAK-NEXT:         storage d32 [size=4, align=4];
// LEAK-NEXT:         storage d64 [size=8, align=8];
// LEAK-NEXT:         storage d128 [size=16, align=16];
// LEAK-NEXT:     }
// LEAK-NEXT:     global %[[VALUE_macro_address:[0-9]+]] macro_address: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// LEAK-NEXT:     global %[[VALUE_macro_hwaddress:[0-9]+]] macro_hwaddress: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// LEAK-NEXT:     global %[[VALUE_macro_thread:[0-9]+]] macro_thread: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// LEAK-NEXT:     global %[[VALUE_feature_address:[0-9]+]] feature_address: i32 [storage=static] = const<i32>(0) [linkage=external];
// LEAK-NEXT:     global %[[VALUE_feature_hwaddress:[0-9]+]] feature_hwaddress: i32 [storage=static] = const<i32>(0) [linkage=external];
// LEAK-NEXT:     global %[[VALUE_feature_memory:[0-9]+]] feature_memory: i32 [storage=static] = const<i32>(0) [linkage=external];
// LEAK-NEXT:     global %[[VALUE_feature_thread:[0-9]+]] feature_thread: i32 [storage=static] = const<i32>(0) [linkage=external];
// LEAK-NEXT:     global %[[VALUE_feature_undefined:[0-9]+]] feature_undefined: i32 [storage=static] = const<i32>(0) [linkage=external];
// LEAK-NEXT:     global %[[VALUE_feature_leak:[0-9]+]] feature_leak: i32 [storage=static] = const<i32>(1) [linkage=external];
// LEAK-NEXT: }
// SLATE-FILECHECK-END LEAK
// SLATE-FILECHECK-BEGIN BOUNDS_STRICT
// BOUNDS_STRICT: module {
// BOUNDS_STRICT-NEXT:     target "x86_64-unknown-linux-gnu" {
// BOUNDS_STRICT-NEXT:         endian = little;
// BOUNDS_STRICT-NEXT:         pointer [size=8, align=8];
// BOUNDS_STRICT-NEXT:         stack_alignment = 16;
// BOUNDS_STRICT-NEXT:         long_double = f80;
// BOUNDS_STRICT-NEXT:         storage bool [size=1, align=1];
// BOUNDS_STRICT-NEXT:         storage i8, u8 [size=1, align=1];
// BOUNDS_STRICT-NEXT:         storage i16, u16 [size=2, align=2];
// BOUNDS_STRICT-NEXT:         storage i32, u32 [size=4, align=4];
// BOUNDS_STRICT-NEXT:         storage i64, u64 [size=8, align=8];
// BOUNDS_STRICT-NEXT:         storage i128, u128 [size=16, align=16];
// BOUNDS_STRICT-NEXT:         storage bf16 [size=2, align=2];
// BOUNDS_STRICT-NEXT:         storage f16 [size=2, align=2];
// BOUNDS_STRICT-NEXT:         storage f32 [size=4, align=4];
// BOUNDS_STRICT-NEXT:         storage f64 [size=8, align=8];
// BOUNDS_STRICT-NEXT:         storage f80 [size=16, align=16];
// BOUNDS_STRICT-NEXT:         storage f128 [size=16, align=16];
// BOUNDS_STRICT-NEXT:         storage d32 [size=4, align=4];
// BOUNDS_STRICT-NEXT:         storage d64 [size=8, align=8];
// BOUNDS_STRICT-NEXT:         storage d128 [size=16, align=16];
// BOUNDS_STRICT-NEXT:     }
// BOUNDS_STRICT-NEXT:     global %[[VALUE_macro_address:[0-9]+]] macro_address: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// BOUNDS_STRICT-NEXT:     global %[[VALUE_macro_hwaddress:[0-9]+]] macro_hwaddress: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// BOUNDS_STRICT-NEXT:     global %[[VALUE_macro_thread:[0-9]+]] macro_thread: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// BOUNDS_STRICT-NEXT:     global %[[VALUE_feature_address:[0-9]+]] feature_address: i32 [storage=static] = const<i32>(0) [linkage=external];
// BOUNDS_STRICT-NEXT:     global %[[VALUE_feature_hwaddress:[0-9]+]] feature_hwaddress: i32 [storage=static] = const<i32>(0) [linkage=external];
// BOUNDS_STRICT-NEXT:     global %[[VALUE_feature_memory:[0-9]+]] feature_memory: i32 [storage=static] = const<i32>(0) [linkage=external];
// BOUNDS_STRICT-NEXT:     global %[[VALUE_feature_thread:[0-9]+]] feature_thread: i32 [storage=static] = const<i32>(0) [linkage=external];
// BOUNDS_STRICT-NEXT:     global %[[VALUE_feature_undefined:[0-9]+]] feature_undefined: i32 [storage=static] = const<i32>(1) [linkage=external];
// BOUNDS_STRICT-NEXT:     global %[[VALUE_feature_leak:[0-9]+]] feature_leak: i32 [storage=static] = const<i32>(0) [linkage=external];
// BOUNDS_STRICT-NEXT: }
// SLATE-FILECHECK-END BOUNDS_STRICT
// SLATE-FILECHECK-BEGIN FLOAT_ONLY
// FLOAT_ONLY: module {
// FLOAT_ONLY-NEXT:     target "x86_64-unknown-linux-gnu" {
// FLOAT_ONLY-NEXT:         endian = little;
// FLOAT_ONLY-NEXT:         pointer [size=8, align=8];
// FLOAT_ONLY-NEXT:         stack_alignment = 16;
// FLOAT_ONLY-NEXT:         long_double = f80;
// FLOAT_ONLY-NEXT:         storage bool [size=1, align=1];
// FLOAT_ONLY-NEXT:         storage i8, u8 [size=1, align=1];
// FLOAT_ONLY-NEXT:         storage i16, u16 [size=2, align=2];
// FLOAT_ONLY-NEXT:         storage i32, u32 [size=4, align=4];
// FLOAT_ONLY-NEXT:         storage i64, u64 [size=8, align=8];
// FLOAT_ONLY-NEXT:         storage i128, u128 [size=16, align=16];
// FLOAT_ONLY-NEXT:         storage bf16 [size=2, align=2];
// FLOAT_ONLY-NEXT:         storage f16 [size=2, align=2];
// FLOAT_ONLY-NEXT:         storage f32 [size=4, align=4];
// FLOAT_ONLY-NEXT:         storage f64 [size=8, align=8];
// FLOAT_ONLY-NEXT:         storage f80 [size=16, align=16];
// FLOAT_ONLY-NEXT:         storage f128 [size=16, align=16];
// FLOAT_ONLY-NEXT:         storage d32 [size=4, align=4];
// FLOAT_ONLY-NEXT:         storage d64 [size=8, align=8];
// FLOAT_ONLY-NEXT:         storage d128 [size=16, align=16];
// FLOAT_ONLY-NEXT:     }
// FLOAT_ONLY-NEXT:     global %[[VALUE_macro_address:[0-9]+]] macro_address: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// FLOAT_ONLY-NEXT:     global %[[VALUE_macro_hwaddress:[0-9]+]] macro_hwaddress: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// FLOAT_ONLY-NEXT:     global %[[VALUE_macro_thread:[0-9]+]] macro_thread: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// FLOAT_ONLY-NEXT:     global %[[VALUE_feature_address:[0-9]+]] feature_address: i32 [storage=static] = const<i32>(0) [linkage=external];
// FLOAT_ONLY-NEXT:     global %[[VALUE_feature_hwaddress:[0-9]+]] feature_hwaddress: i32 [storage=static] = const<i32>(0) [linkage=external];
// FLOAT_ONLY-NEXT:     global %[[VALUE_feature_memory:[0-9]+]] feature_memory: i32 [storage=static] = const<i32>(0) [linkage=external];
// FLOAT_ONLY-NEXT:     global %[[VALUE_feature_thread:[0-9]+]] feature_thread: i32 [storage=static] = const<i32>(0) [linkage=external];
// FLOAT_ONLY-NEXT:     global %[[VALUE_feature_undefined:[0-9]+]] feature_undefined: i32 [storage=static] = const<i32>(0) [linkage=external];
// FLOAT_ONLY-NEXT:     global %[[VALUE_feature_leak:[0-9]+]] feature_leak: i32 [storage=static] = const<i32>(0) [linkage=external];
// FLOAT_ONLY-NEXT: }
// SLATE-FILECHECK-END FLOAT_ONLY
// SLATE-FILECHECK-BEGIN VPTR
// VPTR: module {
// VPTR-NEXT:     target "x86_64-unknown-linux-gnu" {
// VPTR-NEXT:         endian = little;
// VPTR-NEXT:         pointer [size=8, align=8];
// VPTR-NEXT:         stack_alignment = 16;
// VPTR-NEXT:         long_double = f80;
// VPTR-NEXT:         storage bool [size=1, align=1];
// VPTR-NEXT:         storage i8, u8 [size=1, align=1];
// VPTR-NEXT:         storage i16, u16 [size=2, align=2];
// VPTR-NEXT:         storage i32, u32 [size=4, align=4];
// VPTR-NEXT:         storage i64, u64 [size=8, align=8];
// VPTR-NEXT:         storage i128, u128 [size=16, align=16];
// VPTR-NEXT:         storage bf16 [size=2, align=2];
// VPTR-NEXT:         storage f16 [size=2, align=2];
// VPTR-NEXT:         storage f32 [size=4, align=4];
// VPTR-NEXT:         storage f64 [size=8, align=8];
// VPTR-NEXT:         storage f80 [size=16, align=16];
// VPTR-NEXT:         storage f128 [size=16, align=16];
// VPTR-NEXT:         storage d32 [size=4, align=4];
// VPTR-NEXT:         storage d64 [size=8, align=8];
// VPTR-NEXT:         storage d128 [size=16, align=16];
// VPTR-NEXT:     }
// VPTR-NEXT:     global %[[VALUE_macro_address:[0-9]+]] macro_address: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// VPTR-NEXT:     global %[[VALUE_macro_hwaddress:[0-9]+]] macro_hwaddress: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// VPTR-NEXT:     global %[[VALUE_macro_thread:[0-9]+]] macro_thread: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// VPTR-NEXT:     global %[[VALUE_feature_address:[0-9]+]] feature_address: i32 [storage=static] = const<i32>(0) [linkage=external];
// VPTR-NEXT:     global %[[VALUE_feature_hwaddress:[0-9]+]] feature_hwaddress: i32 [storage=static] = const<i32>(0) [linkage=external];
// VPTR-NEXT:     global %[[VALUE_feature_memory:[0-9]+]] feature_memory: i32 [storage=static] = const<i32>(0) [linkage=external];
// VPTR-NEXT:     global %[[VALUE_feature_thread:[0-9]+]] feature_thread: i32 [storage=static] = const<i32>(0) [linkage=external];
// VPTR-NEXT:     global %[[VALUE_feature_undefined:[0-9]+]] feature_undefined: i32 [storage=static] = const<i32>(1) [linkage=external];
// VPTR-NEXT:     global %[[VALUE_feature_leak:[0-9]+]] feature_leak: i32 [storage=static] = const<i32>(0) [linkage=external];
// VPTR-NEXT: }
// SLATE-FILECHECK-END VPTR
// SLATE-FILECHECK-BEGIN POINTER_COMPARE
// POINTER_COMPARE: module {
// POINTER_COMPARE-NEXT:     target "x86_64-unknown-linux-gnu" {
// POINTER_COMPARE-NEXT:         endian = little;
// POINTER_COMPARE-NEXT:         pointer [size=8, align=8];
// POINTER_COMPARE-NEXT:         stack_alignment = 16;
// POINTER_COMPARE-NEXT:         long_double = f80;
// POINTER_COMPARE-NEXT:         storage bool [size=1, align=1];
// POINTER_COMPARE-NEXT:         storage i8, u8 [size=1, align=1];
// POINTER_COMPARE-NEXT:         storage i16, u16 [size=2, align=2];
// POINTER_COMPARE-NEXT:         storage i32, u32 [size=4, align=4];
// POINTER_COMPARE-NEXT:         storage i64, u64 [size=8, align=8];
// POINTER_COMPARE-NEXT:         storage i128, u128 [size=16, align=16];
// POINTER_COMPARE-NEXT:         storage bf16 [size=2, align=2];
// POINTER_COMPARE-NEXT:         storage f16 [size=2, align=2];
// POINTER_COMPARE-NEXT:         storage f32 [size=4, align=4];
// POINTER_COMPARE-NEXT:         storage f64 [size=8, align=8];
// POINTER_COMPARE-NEXT:         storage f80 [size=16, align=16];
// POINTER_COMPARE-NEXT:         storage f128 [size=16, align=16];
// POINTER_COMPARE-NEXT:         storage d32 [size=4, align=4];
// POINTER_COMPARE-NEXT:         storage d64 [size=8, align=8];
// POINTER_COMPARE-NEXT:         storage d128 [size=16, align=16];
// POINTER_COMPARE-NEXT:     }
// POINTER_COMPARE-NEXT:     global %[[VALUE_macro_address:[0-9]+]] macro_address: i32 [storage=static] = const<i32>(1) [linkage=external];
// POINTER_COMPARE-NEXT:     global %[[VALUE_macro_hwaddress:[0-9]+]] macro_hwaddress: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// POINTER_COMPARE-NEXT:     global %[[VALUE_macro_thread:[0-9]+]] macro_thread: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// POINTER_COMPARE-NEXT:     global %[[VALUE_feature_address:[0-9]+]] feature_address: i32 [storage=static] = const<i32>(1) [linkage=external];
// POINTER_COMPARE-NEXT:     global %[[VALUE_feature_hwaddress:[0-9]+]] feature_hwaddress: i32 [storage=static] = const<i32>(0) [linkage=external];
// POINTER_COMPARE-NEXT:     global %[[VALUE_feature_memory:[0-9]+]] feature_memory: i32 [storage=static] = const<i32>(0) [linkage=external];
// POINTER_COMPARE-NEXT:     global %[[VALUE_feature_thread:[0-9]+]] feature_thread: i32 [storage=static] = const<i32>(0) [linkage=external];
// POINTER_COMPARE-NEXT:     global %[[VALUE_feature_undefined:[0-9]+]] feature_undefined: i32 [storage=static] = const<i32>(0) [linkage=external];
// POINTER_COMPARE-NEXT:     global %[[VALUE_feature_leak:[0-9]+]] feature_leak: i32 [storage=static] = const<i32>(0) [linkage=external];
// POINTER_COMPARE-NEXT: }
// SLATE-FILECHECK-END POINTER_COMPARE
// SLATE-FILECHECK-BEGIN NONE_LEFT
// NONE_LEFT: module {
// NONE_LEFT-NEXT:     target "x86_64-unknown-linux-gnu" {
// NONE_LEFT-NEXT:         endian = little;
// NONE_LEFT-NEXT:         pointer [size=8, align=8];
// NONE_LEFT-NEXT:         stack_alignment = 16;
// NONE_LEFT-NEXT:         long_double = f80;
// NONE_LEFT-NEXT:         storage bool [size=1, align=1];
// NONE_LEFT-NEXT:         storage i8, u8 [size=1, align=1];
// NONE_LEFT-NEXT:         storage i16, u16 [size=2, align=2];
// NONE_LEFT-NEXT:         storage i32, u32 [size=4, align=4];
// NONE_LEFT-NEXT:         storage i64, u64 [size=8, align=8];
// NONE_LEFT-NEXT:         storage i128, u128 [size=16, align=16];
// NONE_LEFT-NEXT:         storage bf16 [size=2, align=2];
// NONE_LEFT-NEXT:         storage f16 [size=2, align=2];
// NONE_LEFT-NEXT:         storage f32 [size=4, align=4];
// NONE_LEFT-NEXT:         storage f64 [size=8, align=8];
// NONE_LEFT-NEXT:         storage f80 [size=16, align=16];
// NONE_LEFT-NEXT:         storage f128 [size=16, align=16];
// NONE_LEFT-NEXT:         storage d32 [size=4, align=4];
// NONE_LEFT-NEXT:         storage d64 [size=8, align=8];
// NONE_LEFT-NEXT:         storage d128 [size=16, align=16];
// NONE_LEFT-NEXT:     }
// NONE_LEFT-NEXT:     global %[[VALUE_macro_address:[0-9]+]] macro_address: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NONE_LEFT-NEXT:     global %[[VALUE_macro_hwaddress:[0-9]+]] macro_hwaddress: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NONE_LEFT-NEXT:     global %[[VALUE_macro_thread:[0-9]+]] macro_thread: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NONE_LEFT-NEXT:     global %[[VALUE_feature_address:[0-9]+]] feature_address: i32 [storage=static] = const<i32>(0) [linkage=external];
// NONE_LEFT-NEXT:     global %[[VALUE_feature_hwaddress:[0-9]+]] feature_hwaddress: i32 [storage=static] = const<i32>(0) [linkage=external];
// NONE_LEFT-NEXT:     global %[[VALUE_feature_memory:[0-9]+]] feature_memory: i32 [storage=static] = const<i32>(0) [linkage=external];
// NONE_LEFT-NEXT:     global %[[VALUE_feature_thread:[0-9]+]] feature_thread: i32 [storage=static] = const<i32>(0) [linkage=external];
// NONE_LEFT-NEXT:     global %[[VALUE_feature_undefined:[0-9]+]] feature_undefined: i32 [storage=static] = const<i32>(0) [linkage=external];
// NONE_LEFT-NEXT:     global %[[VALUE_feature_leak:[0-9]+]] feature_leak: i32 [storage=static] = const<i32>(0) [linkage=external];
// NONE_LEFT-NEXT: }
// SLATE-FILECHECK-END NONE_LEFT
// SLATE-FILECHECK-BEGIN RUNTIME_OPTIONS
// RUNTIME_OPTIONS: module {
// RUNTIME_OPTIONS-NEXT:     target "x86_64-unknown-linux-gnu" {
// RUNTIME_OPTIONS-NEXT:         endian = little;
// RUNTIME_OPTIONS-NEXT:         pointer [size=8, align=8];
// RUNTIME_OPTIONS-NEXT:         stack_alignment = 16;
// RUNTIME_OPTIONS-NEXT:         long_double = f80;
// RUNTIME_OPTIONS-NEXT:         storage bool [size=1, align=1];
// RUNTIME_OPTIONS-NEXT:         storage i8, u8 [size=1, align=1];
// RUNTIME_OPTIONS-NEXT:         storage i16, u16 [size=2, align=2];
// RUNTIME_OPTIONS-NEXT:         storage i32, u32 [size=4, align=4];
// RUNTIME_OPTIONS-NEXT:         storage i64, u64 [size=8, align=8];
// RUNTIME_OPTIONS-NEXT:         storage i128, u128 [size=16, align=16];
// RUNTIME_OPTIONS-NEXT:         storage bf16 [size=2, align=2];
// RUNTIME_OPTIONS-NEXT:         storage f16 [size=2, align=2];
// RUNTIME_OPTIONS-NEXT:         storage f32 [size=4, align=4];
// RUNTIME_OPTIONS-NEXT:         storage f64 [size=8, align=8];
// RUNTIME_OPTIONS-NEXT:         storage f80 [size=16, align=16];
// RUNTIME_OPTIONS-NEXT:         storage f128 [size=16, align=16];
// RUNTIME_OPTIONS-NEXT:         storage d32 [size=4, align=4];
// RUNTIME_OPTIONS-NEXT:         storage d64 [size=8, align=8];
// RUNTIME_OPTIONS-NEXT:         storage d128 [size=16, align=16];
// RUNTIME_OPTIONS-NEXT:     }
// RUNTIME_OPTIONS-NEXT:     global %[[VALUE_macro_address:[0-9]+]] macro_address: i32 [storage=static] = const<i32>(1) [linkage=external];
// RUNTIME_OPTIONS-NEXT:     global %[[VALUE_macro_hwaddress:[0-9]+]] macro_hwaddress: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// RUNTIME_OPTIONS-NEXT:     global %[[VALUE_macro_thread:[0-9]+]] macro_thread: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// RUNTIME_OPTIONS-NEXT:     global %[[VALUE_feature_address:[0-9]+]] feature_address: i32 [storage=static] = const<i32>(1) [linkage=external];
// RUNTIME_OPTIONS-NEXT:     global %[[VALUE_feature_hwaddress:[0-9]+]] feature_hwaddress: i32 [storage=static] = const<i32>(0) [linkage=external];
// RUNTIME_OPTIONS-NEXT:     global %[[VALUE_feature_memory:[0-9]+]] feature_memory: i32 [storage=static] = const<i32>(0) [linkage=external];
// RUNTIME_OPTIONS-NEXT:     global %[[VALUE_feature_thread:[0-9]+]] feature_thread: i32 [storage=static] = const<i32>(0) [linkage=external];
// RUNTIME_OPTIONS-NEXT:     global %[[VALUE_feature_undefined:[0-9]+]] feature_undefined: i32 [storage=static] = const<i32>(0) [linkage=external];
// RUNTIME_OPTIONS-NEXT:     global %[[VALUE_feature_leak:[0-9]+]] feature_leak: i32 [storage=static] = const<i32>(0) [linkage=external];
// RUNTIME_OPTIONS-NEXT: }
// SLATE-FILECHECK-END RUNTIME_OPTIONS
