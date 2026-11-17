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
int feature_safe_stack = __has_feature(safe_stack);
int feature_coverage = __has_feature(coverage_sanitizer);
int extension_address = __has_extension(address_sanitizer);

// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-PREFIX-ARGS DEFAULT
// SLATE-FILECHECK-DEFINES ASAN_UBSAN
// SLATE-FILECHECK-PREFIX-ARGS ASAN_UBSAN -fsanitize=address -fsanitize=undefined
// SLATE-FILECHECK-DEFINES KASAN
// SLATE-FILECHECK-PREFIX-ARGS KASAN -fsanitize=kernel-address
// SLATE-FILECHECK-DEFINES HWASAN
// SLATE-FILECHECK-PREFIX-ARGS HWASAN -fsanitize=hwaddress
// SLATE-FILECHECK-DEFINES TSAN
// SLATE-FILECHECK-PREFIX-ARGS TSAN -fsanitize=thread
// SLATE-FILECHECK-DEFINES MSAN
// SLATE-FILECHECK-PREFIX-ARGS MSAN -fsanitize=memory
// SLATE-FILECHECK-DEFINES LEAK_ADDRESS
// SLATE-FILECHECK-PREFIX-ARGS LEAK_ADDRESS -fsanitize=leak,address
// SLATE-FILECHECK-DEFINES SAFE_STACK
// SLATE-FILECHECK-PREFIX-ARGS SAFE_STACK -fsanitize=safe-stack
// SLATE-FILECHECK-DEFINES FUZZER
// SLATE-FILECHECK-PREFIX-ARGS FUZZER -fsanitize=fuzzer-no-link
// SLATE-FILECHECK-DEFINES LATER_WINS
// SLATE-FILECHECK-PREFIX-ARGS LATER_WINS -fsanitize=thread -fsanitize=address -fno-sanitize=thread
// SLATE-FILECHECK-DEFINES NONE_LEFT
// SLATE-FILECHECK-PREFIX-ARGS NONE_LEFT -fsanitize=address,undefined -fno-sanitize=all
// SLATE-FILECHECK-DEFINES OBJECT_SIZE_O0
// SLATE-FILECHECK-PREFIX-ARGS OBJECT_SIZE_O0 -fsanitize=object-size
// SLATE-FILECHECK-DEFINES OBJECT_SIZE_O2
// SLATE-FILECHECK-PREFIX-ARGS OBJECT_SIZE_O2 -O2 -fsanitize=object-size
// SLATE-FILECHECK-DEFINES VPTR
// SLATE-FILECHECK-PREFIX-ARGS VPTR -fsanitize=vptr
// SLATE-FILECHECK-DEFINES INTEGER_NO_UB
// SLATE-FILECHECK-PREFIX-ARGS INTEGER_NO_UB -fsanitize=integer -fno-sanitize=signed-integer-overflow,shift,integer-divide-by-zero
// SLATE-FILECHECK-DEFINES EMPTY
// SLATE-FILECHECK-PREFIX-ARGS EMPTY -fsanitize=
// SLATE-FILECHECK-DEFINES RUNTIME_OPTIONS
// SLATE-FILECHECK-PREFIX-ARGS RUNTIME_OPTIONS -fsanitize=address -fsanitize-recover=address -fno-sanitize-recover=all -fsanitize-trap=undefined -fsanitize-trap -fsanitize-address-use-after-scope -fsanitize-undefined-trap-on-error
// SLATE-FILECHECK-DEFINES MINIMAL_RUNTIME
// SLATE-FILECHECK-PREFIX-ARGS MINIMAL_RUNTIME -fsanitize=undefined -fsanitize-minimal-runtime

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
// DEFAULT-NEXT:     global %[[VALUE_feature_safe_stack:[0-9]+]] feature_safe_stack: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_feature_coverage:[0-9]+]] feature_coverage: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_extension_address:[0-9]+]] extension_address: i32 [storage=static] = const<i32>(0) [linkage=external];
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
// ASAN_UBSAN-NEXT:     global %[[VALUE_feature_safe_stack:[0-9]+]] feature_safe_stack: i32 [storage=static] = const<i32>(0) [linkage=external];
// ASAN_UBSAN-NEXT:     global %[[VALUE_feature_coverage:[0-9]+]] feature_coverage: i32 [storage=static] = const<i32>(0) [linkage=external];
// ASAN_UBSAN-NEXT:     global %[[VALUE_extension_address:[0-9]+]] extension_address: i32 [storage=static] = const<i32>(1) [linkage=external];
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
// KASAN-NEXT:     global %[[VALUE_feature_safe_stack:[0-9]+]] feature_safe_stack: i32 [storage=static] = const<i32>(0) [linkage=external];
// KASAN-NEXT:     global %[[VALUE_feature_coverage:[0-9]+]] feature_coverage: i32 [storage=static] = const<i32>(0) [linkage=external];
// KASAN-NEXT:     global %[[VALUE_extension_address:[0-9]+]] extension_address: i32 [storage=static] = const<i32>(1) [linkage=external];
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
// HWASAN-NEXT:     global %[[VALUE_feature_safe_stack:[0-9]+]] feature_safe_stack: i32 [storage=static] = const<i32>(0) [linkage=external];
// HWASAN-NEXT:     global %[[VALUE_feature_coverage:[0-9]+]] feature_coverage: i32 [storage=static] = const<i32>(0) [linkage=external];
// HWASAN-NEXT:     global %[[VALUE_extension_address:[0-9]+]] extension_address: i32 [storage=static] = const<i32>(0) [linkage=external];
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
// TSAN-NEXT:     global %[[VALUE_feature_safe_stack:[0-9]+]] feature_safe_stack: i32 [storage=static] = const<i32>(0) [linkage=external];
// TSAN-NEXT:     global %[[VALUE_feature_coverage:[0-9]+]] feature_coverage: i32 [storage=static] = const<i32>(0) [linkage=external];
// TSAN-NEXT:     global %[[VALUE_extension_address:[0-9]+]] extension_address: i32 [storage=static] = const<i32>(0) [linkage=external];
// TSAN-NEXT: }
// SLATE-FILECHECK-END TSAN
// SLATE-FILECHECK-BEGIN MSAN
// MSAN: module {
// MSAN-NEXT:     target "x86_64-unknown-linux-gnu" {
// MSAN-NEXT:         endian = little;
// MSAN-NEXT:         pointer [size=8, align=8];
// MSAN-NEXT:         stack_alignment = 16;
// MSAN-NEXT:         long_double = f80;
// MSAN-NEXT:         storage bool [size=1, align=1];
// MSAN-NEXT:         storage i8, u8 [size=1, align=1];
// MSAN-NEXT:         storage i16, u16 [size=2, align=2];
// MSAN-NEXT:         storage i32, u32 [size=4, align=4];
// MSAN-NEXT:         storage i64, u64 [size=8, align=8];
// MSAN-NEXT:         storage i128, u128 [size=16, align=16];
// MSAN-NEXT:         storage bf16 [size=2, align=2];
// MSAN-NEXT:         storage f16 [size=2, align=2];
// MSAN-NEXT:         storage f32 [size=4, align=4];
// MSAN-NEXT:         storage f64 [size=8, align=8];
// MSAN-NEXT:         storage f80 [size=16, align=16];
// MSAN-NEXT:         storage f128 [size=16, align=16];
// MSAN-NEXT:         storage d32 [size=4, align=4];
// MSAN-NEXT:         storage d64 [size=8, align=8];
// MSAN-NEXT:         storage d128 [size=16, align=16];
// MSAN-NEXT:     }
// MSAN-NEXT:     global %[[VALUE_macro_address:[0-9]+]] macro_address: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// MSAN-NEXT:     global %[[VALUE_macro_hwaddress:[0-9]+]] macro_hwaddress: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// MSAN-NEXT:     global %[[VALUE_macro_thread:[0-9]+]] macro_thread: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// MSAN-NEXT:     global %[[VALUE_feature_address:[0-9]+]] feature_address: i32 [storage=static] = const<i32>(0) [linkage=external];
// MSAN-NEXT:     global %[[VALUE_feature_hwaddress:[0-9]+]] feature_hwaddress: i32 [storage=static] = const<i32>(0) [linkage=external];
// MSAN-NEXT:     global %[[VALUE_feature_memory:[0-9]+]] feature_memory: i32 [storage=static] = const<i32>(1) [linkage=external];
// MSAN-NEXT:     global %[[VALUE_feature_thread:[0-9]+]] feature_thread: i32 [storage=static] = const<i32>(0) [linkage=external];
// MSAN-NEXT:     global %[[VALUE_feature_undefined:[0-9]+]] feature_undefined: i32 [storage=static] = const<i32>(0) [linkage=external];
// MSAN-NEXT:     global %[[VALUE_feature_leak:[0-9]+]] feature_leak: i32 [storage=static] = const<i32>(0) [linkage=external];
// MSAN-NEXT:     global %[[VALUE_feature_safe_stack:[0-9]+]] feature_safe_stack: i32 [storage=static] = const<i32>(0) [linkage=external];
// MSAN-NEXT:     global %[[VALUE_feature_coverage:[0-9]+]] feature_coverage: i32 [storage=static] = const<i32>(0) [linkage=external];
// MSAN-NEXT:     global %[[VALUE_extension_address:[0-9]+]] extension_address: i32 [storage=static] = const<i32>(0) [linkage=external];
// MSAN-NEXT: }
// SLATE-FILECHECK-END MSAN
// SLATE-FILECHECK-BEGIN LEAK_ADDRESS
// LEAK_ADDRESS: module {
// LEAK_ADDRESS-NEXT:     target "x86_64-unknown-linux-gnu" {
// LEAK_ADDRESS-NEXT:         endian = little;
// LEAK_ADDRESS-NEXT:         pointer [size=8, align=8];
// LEAK_ADDRESS-NEXT:         stack_alignment = 16;
// LEAK_ADDRESS-NEXT:         long_double = f80;
// LEAK_ADDRESS-NEXT:         storage bool [size=1, align=1];
// LEAK_ADDRESS-NEXT:         storage i8, u8 [size=1, align=1];
// LEAK_ADDRESS-NEXT:         storage i16, u16 [size=2, align=2];
// LEAK_ADDRESS-NEXT:         storage i32, u32 [size=4, align=4];
// LEAK_ADDRESS-NEXT:         storage i64, u64 [size=8, align=8];
// LEAK_ADDRESS-NEXT:         storage i128, u128 [size=16, align=16];
// LEAK_ADDRESS-NEXT:         storage bf16 [size=2, align=2];
// LEAK_ADDRESS-NEXT:         storage f16 [size=2, align=2];
// LEAK_ADDRESS-NEXT:         storage f32 [size=4, align=4];
// LEAK_ADDRESS-NEXT:         storage f64 [size=8, align=8];
// LEAK_ADDRESS-NEXT:         storage f80 [size=16, align=16];
// LEAK_ADDRESS-NEXT:         storage f128 [size=16, align=16];
// LEAK_ADDRESS-NEXT:         storage d32 [size=4, align=4];
// LEAK_ADDRESS-NEXT:         storage d64 [size=8, align=8];
// LEAK_ADDRESS-NEXT:         storage d128 [size=16, align=16];
// LEAK_ADDRESS-NEXT:     }
// LEAK_ADDRESS-NEXT:     global %[[VALUE_macro_address:[0-9]+]] macro_address: i32 [storage=static] = const<i32>(1) [linkage=external];
// LEAK_ADDRESS-NEXT:     global %[[VALUE_macro_hwaddress:[0-9]+]] macro_hwaddress: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// LEAK_ADDRESS-NEXT:     global %[[VALUE_macro_thread:[0-9]+]] macro_thread: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// LEAK_ADDRESS-NEXT:     global %[[VALUE_feature_address:[0-9]+]] feature_address: i32 [storage=static] = const<i32>(1) [linkage=external];
// LEAK_ADDRESS-NEXT:     global %[[VALUE_feature_hwaddress:[0-9]+]] feature_hwaddress: i32 [storage=static] = const<i32>(0) [linkage=external];
// LEAK_ADDRESS-NEXT:     global %[[VALUE_feature_memory:[0-9]+]] feature_memory: i32 [storage=static] = const<i32>(0) [linkage=external];
// LEAK_ADDRESS-NEXT:     global %[[VALUE_feature_thread:[0-9]+]] feature_thread: i32 [storage=static] = const<i32>(0) [linkage=external];
// LEAK_ADDRESS-NEXT:     global %[[VALUE_feature_undefined:[0-9]+]] feature_undefined: i32 [storage=static] = const<i32>(0) [linkage=external];
// LEAK_ADDRESS-NEXT:     global %[[VALUE_feature_leak:[0-9]+]] feature_leak: i32 [storage=static] = const<i32>(1) [linkage=external];
// LEAK_ADDRESS-NEXT:     global %[[VALUE_feature_safe_stack:[0-9]+]] feature_safe_stack: i32 [storage=static] = const<i32>(0) [linkage=external];
// LEAK_ADDRESS-NEXT:     global %[[VALUE_feature_coverage:[0-9]+]] feature_coverage: i32 [storage=static] = const<i32>(0) [linkage=external];
// LEAK_ADDRESS-NEXT:     global %[[VALUE_extension_address:[0-9]+]] extension_address: i32 [storage=static] = const<i32>(1) [linkage=external];
// LEAK_ADDRESS-NEXT: }
// SLATE-FILECHECK-END LEAK_ADDRESS
// SLATE-FILECHECK-BEGIN SAFE_STACK
// SAFE_STACK: module {
// SAFE_STACK-NEXT:     target "x86_64-unknown-linux-gnu" {
// SAFE_STACK-NEXT:         endian = little;
// SAFE_STACK-NEXT:         pointer [size=8, align=8];
// SAFE_STACK-NEXT:         stack_alignment = 16;
// SAFE_STACK-NEXT:         long_double = f80;
// SAFE_STACK-NEXT:         storage bool [size=1, align=1];
// SAFE_STACK-NEXT:         storage i8, u8 [size=1, align=1];
// SAFE_STACK-NEXT:         storage i16, u16 [size=2, align=2];
// SAFE_STACK-NEXT:         storage i32, u32 [size=4, align=4];
// SAFE_STACK-NEXT:         storage i64, u64 [size=8, align=8];
// SAFE_STACK-NEXT:         storage i128, u128 [size=16, align=16];
// SAFE_STACK-NEXT:         storage bf16 [size=2, align=2];
// SAFE_STACK-NEXT:         storage f16 [size=2, align=2];
// SAFE_STACK-NEXT:         storage f32 [size=4, align=4];
// SAFE_STACK-NEXT:         storage f64 [size=8, align=8];
// SAFE_STACK-NEXT:         storage f80 [size=16, align=16];
// SAFE_STACK-NEXT:         storage f128 [size=16, align=16];
// SAFE_STACK-NEXT:         storage d32 [size=4, align=4];
// SAFE_STACK-NEXT:         storage d64 [size=8, align=8];
// SAFE_STACK-NEXT:         storage d128 [size=16, align=16];
// SAFE_STACK-NEXT:     }
// SAFE_STACK-NEXT:     global %[[VALUE_macro_address:[0-9]+]] macro_address: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// SAFE_STACK-NEXT:     global %[[VALUE_macro_hwaddress:[0-9]+]] macro_hwaddress: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// SAFE_STACK-NEXT:     global %[[VALUE_macro_thread:[0-9]+]] macro_thread: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// SAFE_STACK-NEXT:     global %[[VALUE_feature_address:[0-9]+]] feature_address: i32 [storage=static] = const<i32>(0) [linkage=external];
// SAFE_STACK-NEXT:     global %[[VALUE_feature_hwaddress:[0-9]+]] feature_hwaddress: i32 [storage=static] = const<i32>(0) [linkage=external];
// SAFE_STACK-NEXT:     global %[[VALUE_feature_memory:[0-9]+]] feature_memory: i32 [storage=static] = const<i32>(0) [linkage=external];
// SAFE_STACK-NEXT:     global %[[VALUE_feature_thread:[0-9]+]] feature_thread: i32 [storage=static] = const<i32>(0) [linkage=external];
// SAFE_STACK-NEXT:     global %[[VALUE_feature_undefined:[0-9]+]] feature_undefined: i32 [storage=static] = const<i32>(0) [linkage=external];
// SAFE_STACK-NEXT:     global %[[VALUE_feature_leak:[0-9]+]] feature_leak: i32 [storage=static] = const<i32>(0) [linkage=external];
// SAFE_STACK-NEXT:     global %[[VALUE_feature_safe_stack:[0-9]+]] feature_safe_stack: i32 [storage=static] = const<i32>(1) [linkage=external];
// SAFE_STACK-NEXT:     global %[[VALUE_feature_coverage:[0-9]+]] feature_coverage: i32 [storage=static] = const<i32>(0) [linkage=external];
// SAFE_STACK-NEXT:     global %[[VALUE_extension_address:[0-9]+]] extension_address: i32 [storage=static] = const<i32>(0) [linkage=external];
// SAFE_STACK-NEXT: }
// SLATE-FILECHECK-END SAFE_STACK
// SLATE-FILECHECK-BEGIN FUZZER
// FUZZER: module {
// FUZZER-NEXT:     target "x86_64-unknown-linux-gnu" {
// FUZZER-NEXT:         endian = little;
// FUZZER-NEXT:         pointer [size=8, align=8];
// FUZZER-NEXT:         stack_alignment = 16;
// FUZZER-NEXT:         long_double = f80;
// FUZZER-NEXT:         storage bool [size=1, align=1];
// FUZZER-NEXT:         storage i8, u8 [size=1, align=1];
// FUZZER-NEXT:         storage i16, u16 [size=2, align=2];
// FUZZER-NEXT:         storage i32, u32 [size=4, align=4];
// FUZZER-NEXT:         storage i64, u64 [size=8, align=8];
// FUZZER-NEXT:         storage i128, u128 [size=16, align=16];
// FUZZER-NEXT:         storage bf16 [size=2, align=2];
// FUZZER-NEXT:         storage f16 [size=2, align=2];
// FUZZER-NEXT:         storage f32 [size=4, align=4];
// FUZZER-NEXT:         storage f64 [size=8, align=8];
// FUZZER-NEXT:         storage f80 [size=16, align=16];
// FUZZER-NEXT:         storage f128 [size=16, align=16];
// FUZZER-NEXT:         storage d32 [size=4, align=4];
// FUZZER-NEXT:         storage d64 [size=8, align=8];
// FUZZER-NEXT:         storage d128 [size=16, align=16];
// FUZZER-NEXT:     }
// FUZZER-NEXT:     global %[[VALUE_macro_address:[0-9]+]] macro_address: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// FUZZER-NEXT:     global %[[VALUE_macro_hwaddress:[0-9]+]] macro_hwaddress: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// FUZZER-NEXT:     global %[[VALUE_macro_thread:[0-9]+]] macro_thread: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// FUZZER-NEXT:     global %[[VALUE_feature_address:[0-9]+]] feature_address: i32 [storage=static] = const<i32>(0) [linkage=external];
// FUZZER-NEXT:     global %[[VALUE_feature_hwaddress:[0-9]+]] feature_hwaddress: i32 [storage=static] = const<i32>(0) [linkage=external];
// FUZZER-NEXT:     global %[[VALUE_feature_memory:[0-9]+]] feature_memory: i32 [storage=static] = const<i32>(0) [linkage=external];
// FUZZER-NEXT:     global %[[VALUE_feature_thread:[0-9]+]] feature_thread: i32 [storage=static] = const<i32>(0) [linkage=external];
// FUZZER-NEXT:     global %[[VALUE_feature_undefined:[0-9]+]] feature_undefined: i32 [storage=static] = const<i32>(0) [linkage=external];
// FUZZER-NEXT:     global %[[VALUE_feature_leak:[0-9]+]] feature_leak: i32 [storage=static] = const<i32>(0) [linkage=external];
// FUZZER-NEXT:     global %[[VALUE_feature_safe_stack:[0-9]+]] feature_safe_stack: i32 [storage=static] = const<i32>(0) [linkage=external];
// FUZZER-NEXT:     global %[[VALUE_feature_coverage:[0-9]+]] feature_coverage: i32 [storage=static] = const<i32>(1) [linkage=external];
// FUZZER-NEXT:     global %[[VALUE_extension_address:[0-9]+]] extension_address: i32 [storage=static] = const<i32>(0) [linkage=external];
// FUZZER-NEXT: }
// SLATE-FILECHECK-END FUZZER
// SLATE-FILECHECK-BEGIN LATER_WINS
// LATER_WINS: module {
// LATER_WINS-NEXT:     target "x86_64-unknown-linux-gnu" {
// LATER_WINS-NEXT:         endian = little;
// LATER_WINS-NEXT:         pointer [size=8, align=8];
// LATER_WINS-NEXT:         stack_alignment = 16;
// LATER_WINS-NEXT:         long_double = f80;
// LATER_WINS-NEXT:         storage bool [size=1, align=1];
// LATER_WINS-NEXT:         storage i8, u8 [size=1, align=1];
// LATER_WINS-NEXT:         storage i16, u16 [size=2, align=2];
// LATER_WINS-NEXT:         storage i32, u32 [size=4, align=4];
// LATER_WINS-NEXT:         storage i64, u64 [size=8, align=8];
// LATER_WINS-NEXT:         storage i128, u128 [size=16, align=16];
// LATER_WINS-NEXT:         storage bf16 [size=2, align=2];
// LATER_WINS-NEXT:         storage f16 [size=2, align=2];
// LATER_WINS-NEXT:         storage f32 [size=4, align=4];
// LATER_WINS-NEXT:         storage f64 [size=8, align=8];
// LATER_WINS-NEXT:         storage f80 [size=16, align=16];
// LATER_WINS-NEXT:         storage f128 [size=16, align=16];
// LATER_WINS-NEXT:         storage d32 [size=4, align=4];
// LATER_WINS-NEXT:         storage d64 [size=8, align=8];
// LATER_WINS-NEXT:         storage d128 [size=16, align=16];
// LATER_WINS-NEXT:     }
// LATER_WINS-NEXT:     global %[[VALUE_macro_address:[0-9]+]] macro_address: i32 [storage=static] = const<i32>(1) [linkage=external];
// LATER_WINS-NEXT:     global %[[VALUE_macro_hwaddress:[0-9]+]] macro_hwaddress: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// LATER_WINS-NEXT:     global %[[VALUE_macro_thread:[0-9]+]] macro_thread: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// LATER_WINS-NEXT:     global %[[VALUE_feature_address:[0-9]+]] feature_address: i32 [storage=static] = const<i32>(1) [linkage=external];
// LATER_WINS-NEXT:     global %[[VALUE_feature_hwaddress:[0-9]+]] feature_hwaddress: i32 [storage=static] = const<i32>(0) [linkage=external];
// LATER_WINS-NEXT:     global %[[VALUE_feature_memory:[0-9]+]] feature_memory: i32 [storage=static] = const<i32>(0) [linkage=external];
// LATER_WINS-NEXT:     global %[[VALUE_feature_thread:[0-9]+]] feature_thread: i32 [storage=static] = const<i32>(0) [linkage=external];
// LATER_WINS-NEXT:     global %[[VALUE_feature_undefined:[0-9]+]] feature_undefined: i32 [storage=static] = const<i32>(0) [linkage=external];
// LATER_WINS-NEXT:     global %[[VALUE_feature_leak:[0-9]+]] feature_leak: i32 [storage=static] = const<i32>(0) [linkage=external];
// LATER_WINS-NEXT:     global %[[VALUE_feature_safe_stack:[0-9]+]] feature_safe_stack: i32 [storage=static] = const<i32>(0) [linkage=external];
// LATER_WINS-NEXT:     global %[[VALUE_feature_coverage:[0-9]+]] feature_coverage: i32 [storage=static] = const<i32>(0) [linkage=external];
// LATER_WINS-NEXT:     global %[[VALUE_extension_address:[0-9]+]] extension_address: i32 [storage=static] = const<i32>(1) [linkage=external];
// LATER_WINS-NEXT: }
// SLATE-FILECHECK-END LATER_WINS
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
// NONE_LEFT-NEXT:     global %[[VALUE_feature_safe_stack:[0-9]+]] feature_safe_stack: i32 [storage=static] = const<i32>(0) [linkage=external];
// NONE_LEFT-NEXT:     global %[[VALUE_feature_coverage:[0-9]+]] feature_coverage: i32 [storage=static] = const<i32>(0) [linkage=external];
// NONE_LEFT-NEXT:     global %[[VALUE_extension_address:[0-9]+]] extension_address: i32 [storage=static] = const<i32>(0) [linkage=external];
// NONE_LEFT-NEXT: }
// SLATE-FILECHECK-END NONE_LEFT
// SLATE-FILECHECK-BEGIN OBJECT_SIZE_O0
// OBJECT_SIZE_O0: module {
// OBJECT_SIZE_O0-NEXT:     target "x86_64-unknown-linux-gnu" {
// OBJECT_SIZE_O0-NEXT:         endian = little;
// OBJECT_SIZE_O0-NEXT:         pointer [size=8, align=8];
// OBJECT_SIZE_O0-NEXT:         stack_alignment = 16;
// OBJECT_SIZE_O0-NEXT:         long_double = f80;
// OBJECT_SIZE_O0-NEXT:         storage bool [size=1, align=1];
// OBJECT_SIZE_O0-NEXT:         storage i8, u8 [size=1, align=1];
// OBJECT_SIZE_O0-NEXT:         storage i16, u16 [size=2, align=2];
// OBJECT_SIZE_O0-NEXT:         storage i32, u32 [size=4, align=4];
// OBJECT_SIZE_O0-NEXT:         storage i64, u64 [size=8, align=8];
// OBJECT_SIZE_O0-NEXT:         storage i128, u128 [size=16, align=16];
// OBJECT_SIZE_O0-NEXT:         storage bf16 [size=2, align=2];
// OBJECT_SIZE_O0-NEXT:         storage f16 [size=2, align=2];
// OBJECT_SIZE_O0-NEXT:         storage f32 [size=4, align=4];
// OBJECT_SIZE_O0-NEXT:         storage f64 [size=8, align=8];
// OBJECT_SIZE_O0-NEXT:         storage f80 [size=16, align=16];
// OBJECT_SIZE_O0-NEXT:         storage f128 [size=16, align=16];
// OBJECT_SIZE_O0-NEXT:         storage d32 [size=4, align=4];
// OBJECT_SIZE_O0-NEXT:         storage d64 [size=8, align=8];
// OBJECT_SIZE_O0-NEXT:         storage d128 [size=16, align=16];
// OBJECT_SIZE_O0-NEXT:     }
// OBJECT_SIZE_O0-NEXT:     global %[[VALUE_macro_address:[0-9]+]] macro_address: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// OBJECT_SIZE_O0-NEXT:     global %[[VALUE_macro_hwaddress:[0-9]+]] macro_hwaddress: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// OBJECT_SIZE_O0-NEXT:     global %[[VALUE_macro_thread:[0-9]+]] macro_thread: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// OBJECT_SIZE_O0-NEXT:     global %[[VALUE_feature_address:[0-9]+]] feature_address: i32 [storage=static] = const<i32>(0) [linkage=external];
// OBJECT_SIZE_O0-NEXT:     global %[[VALUE_feature_hwaddress:[0-9]+]] feature_hwaddress: i32 [storage=static] = const<i32>(0) [linkage=external];
// OBJECT_SIZE_O0-NEXT:     global %[[VALUE_feature_memory:[0-9]+]] feature_memory: i32 [storage=static] = const<i32>(0) [linkage=external];
// OBJECT_SIZE_O0-NEXT:     global %[[VALUE_feature_thread:[0-9]+]] feature_thread: i32 [storage=static] = const<i32>(0) [linkage=external];
// OBJECT_SIZE_O0-NEXT:     global %[[VALUE_feature_undefined:[0-9]+]] feature_undefined: i32 [storage=static] = const<i32>(0) [linkage=external];
// OBJECT_SIZE_O0-NEXT:     global %[[VALUE_feature_leak:[0-9]+]] feature_leak: i32 [storage=static] = const<i32>(0) [linkage=external];
// OBJECT_SIZE_O0-NEXT:     global %[[VALUE_feature_safe_stack:[0-9]+]] feature_safe_stack: i32 [storage=static] = const<i32>(0) [linkage=external];
// OBJECT_SIZE_O0-NEXT:     global %[[VALUE_feature_coverage:[0-9]+]] feature_coverage: i32 [storage=static] = const<i32>(0) [linkage=external];
// OBJECT_SIZE_O0-NEXT:     global %[[VALUE_extension_address:[0-9]+]] extension_address: i32 [storage=static] = const<i32>(0) [linkage=external];
// OBJECT_SIZE_O0-NEXT: }
// SLATE-FILECHECK-END OBJECT_SIZE_O0
// SLATE-FILECHECK-BEGIN OBJECT_SIZE_O2
// OBJECT_SIZE_O2: module {
// OBJECT_SIZE_O2-NEXT:     target "x86_64-unknown-linux-gnu" {
// OBJECT_SIZE_O2-NEXT:         endian = little;
// OBJECT_SIZE_O2-NEXT:         pointer [size=8, align=8];
// OBJECT_SIZE_O2-NEXT:         stack_alignment = 16;
// OBJECT_SIZE_O2-NEXT:         long_double = f80;
// OBJECT_SIZE_O2-NEXT:         storage bool [size=1, align=1];
// OBJECT_SIZE_O2-NEXT:         storage i8, u8 [size=1, align=1];
// OBJECT_SIZE_O2-NEXT:         storage i16, u16 [size=2, align=2];
// OBJECT_SIZE_O2-NEXT:         storage i32, u32 [size=4, align=4];
// OBJECT_SIZE_O2-NEXT:         storage i64, u64 [size=8, align=8];
// OBJECT_SIZE_O2-NEXT:         storage i128, u128 [size=16, align=16];
// OBJECT_SIZE_O2-NEXT:         storage bf16 [size=2, align=2];
// OBJECT_SIZE_O2-NEXT:         storage f16 [size=2, align=2];
// OBJECT_SIZE_O2-NEXT:         storage f32 [size=4, align=4];
// OBJECT_SIZE_O2-NEXT:         storage f64 [size=8, align=8];
// OBJECT_SIZE_O2-NEXT:         storage f80 [size=16, align=16];
// OBJECT_SIZE_O2-NEXT:         storage f128 [size=16, align=16];
// OBJECT_SIZE_O2-NEXT:         storage d32 [size=4, align=4];
// OBJECT_SIZE_O2-NEXT:         storage d64 [size=8, align=8];
// OBJECT_SIZE_O2-NEXT:         storage d128 [size=16, align=16];
// OBJECT_SIZE_O2-NEXT:     }
// OBJECT_SIZE_O2-NEXT:     global %[[VALUE_macro_address:[0-9]+]] macro_address: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// OBJECT_SIZE_O2-NEXT:     global %[[VALUE_macro_hwaddress:[0-9]+]] macro_hwaddress: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// OBJECT_SIZE_O2-NEXT:     global %[[VALUE_macro_thread:[0-9]+]] macro_thread: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// OBJECT_SIZE_O2-NEXT:     global %[[VALUE_feature_address:[0-9]+]] feature_address: i32 [storage=static] = const<i32>(0) [linkage=external];
// OBJECT_SIZE_O2-NEXT:     global %[[VALUE_feature_hwaddress:[0-9]+]] feature_hwaddress: i32 [storage=static] = const<i32>(0) [linkage=external];
// OBJECT_SIZE_O2-NEXT:     global %[[VALUE_feature_memory:[0-9]+]] feature_memory: i32 [storage=static] = const<i32>(0) [linkage=external];
// OBJECT_SIZE_O2-NEXT:     global %[[VALUE_feature_thread:[0-9]+]] feature_thread: i32 [storage=static] = const<i32>(0) [linkage=external];
// OBJECT_SIZE_O2-NEXT:     global %[[VALUE_feature_undefined:[0-9]+]] feature_undefined: i32 [storage=static] = const<i32>(1) [linkage=external];
// OBJECT_SIZE_O2-NEXT:     global %[[VALUE_feature_leak:[0-9]+]] feature_leak: i32 [storage=static] = const<i32>(0) [linkage=external];
// OBJECT_SIZE_O2-NEXT:     global %[[VALUE_feature_safe_stack:[0-9]+]] feature_safe_stack: i32 [storage=static] = const<i32>(0) [linkage=external];
// OBJECT_SIZE_O2-NEXT:     global %[[VALUE_feature_coverage:[0-9]+]] feature_coverage: i32 [storage=static] = const<i32>(0) [linkage=external];
// OBJECT_SIZE_O2-NEXT:     global %[[VALUE_extension_address:[0-9]+]] extension_address: i32 [storage=static] = const<i32>(0) [linkage=external];
// OBJECT_SIZE_O2-NEXT: }
// SLATE-FILECHECK-END OBJECT_SIZE_O2
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
// VPTR-NEXT:     global %[[VALUE_feature_undefined:[0-9]+]] feature_undefined: i32 [storage=static] = const<i32>(0) [linkage=external];
// VPTR-NEXT:     global %[[VALUE_feature_leak:[0-9]+]] feature_leak: i32 [storage=static] = const<i32>(0) [linkage=external];
// VPTR-NEXT:     global %[[VALUE_feature_safe_stack:[0-9]+]] feature_safe_stack: i32 [storage=static] = const<i32>(0) [linkage=external];
// VPTR-NEXT:     global %[[VALUE_feature_coverage:[0-9]+]] feature_coverage: i32 [storage=static] = const<i32>(0) [linkage=external];
// VPTR-NEXT:     global %[[VALUE_extension_address:[0-9]+]] extension_address: i32 [storage=static] = const<i32>(0) [linkage=external];
// VPTR-NEXT: }
// SLATE-FILECHECK-END VPTR
// SLATE-FILECHECK-BEGIN INTEGER_NO_UB
// INTEGER_NO_UB: module {
// INTEGER_NO_UB-NEXT:     target "x86_64-unknown-linux-gnu" {
// INTEGER_NO_UB-NEXT:         endian = little;
// INTEGER_NO_UB-NEXT:         pointer [size=8, align=8];
// INTEGER_NO_UB-NEXT:         stack_alignment = 16;
// INTEGER_NO_UB-NEXT:         long_double = f80;
// INTEGER_NO_UB-NEXT:         storage bool [size=1, align=1];
// INTEGER_NO_UB-NEXT:         storage i8, u8 [size=1, align=1];
// INTEGER_NO_UB-NEXT:         storage i16, u16 [size=2, align=2];
// INTEGER_NO_UB-NEXT:         storage i32, u32 [size=4, align=4];
// INTEGER_NO_UB-NEXT:         storage i64, u64 [size=8, align=8];
// INTEGER_NO_UB-NEXT:         storage i128, u128 [size=16, align=16];
// INTEGER_NO_UB-NEXT:         storage bf16 [size=2, align=2];
// INTEGER_NO_UB-NEXT:         storage f16 [size=2, align=2];
// INTEGER_NO_UB-NEXT:         storage f32 [size=4, align=4];
// INTEGER_NO_UB-NEXT:         storage f64 [size=8, align=8];
// INTEGER_NO_UB-NEXT:         storage f80 [size=16, align=16];
// INTEGER_NO_UB-NEXT:         storage f128 [size=16, align=16];
// INTEGER_NO_UB-NEXT:         storage d32 [size=4, align=4];
// INTEGER_NO_UB-NEXT:         storage d64 [size=8, align=8];
// INTEGER_NO_UB-NEXT:         storage d128 [size=16, align=16];
// INTEGER_NO_UB-NEXT:     }
// INTEGER_NO_UB-NEXT:     global %[[VALUE_macro_address:[0-9]+]] macro_address: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// INTEGER_NO_UB-NEXT:     global %[[VALUE_macro_hwaddress:[0-9]+]] macro_hwaddress: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// INTEGER_NO_UB-NEXT:     global %[[VALUE_macro_thread:[0-9]+]] macro_thread: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// INTEGER_NO_UB-NEXT:     global %[[VALUE_feature_address:[0-9]+]] feature_address: i32 [storage=static] = const<i32>(0) [linkage=external];
// INTEGER_NO_UB-NEXT:     global %[[VALUE_feature_hwaddress:[0-9]+]] feature_hwaddress: i32 [storage=static] = const<i32>(0) [linkage=external];
// INTEGER_NO_UB-NEXT:     global %[[VALUE_feature_memory:[0-9]+]] feature_memory: i32 [storage=static] = const<i32>(0) [linkage=external];
// INTEGER_NO_UB-NEXT:     global %[[VALUE_feature_thread:[0-9]+]] feature_thread: i32 [storage=static] = const<i32>(0) [linkage=external];
// INTEGER_NO_UB-NEXT:     global %[[VALUE_feature_undefined:[0-9]+]] feature_undefined: i32 [storage=static] = const<i32>(0) [linkage=external];
// INTEGER_NO_UB-NEXT:     global %[[VALUE_feature_leak:[0-9]+]] feature_leak: i32 [storage=static] = const<i32>(0) [linkage=external];
// INTEGER_NO_UB-NEXT:     global %[[VALUE_feature_safe_stack:[0-9]+]] feature_safe_stack: i32 [storage=static] = const<i32>(0) [linkage=external];
// INTEGER_NO_UB-NEXT:     global %[[VALUE_feature_coverage:[0-9]+]] feature_coverage: i32 [storage=static] = const<i32>(0) [linkage=external];
// INTEGER_NO_UB-NEXT:     global %[[VALUE_extension_address:[0-9]+]] extension_address: i32 [storage=static] = const<i32>(0) [linkage=external];
// INTEGER_NO_UB-NEXT: }
// SLATE-FILECHECK-END INTEGER_NO_UB
// SLATE-FILECHECK-BEGIN EMPTY
// EMPTY: module {
// EMPTY-NEXT:     target "x86_64-unknown-linux-gnu" {
// EMPTY-NEXT:         endian = little;
// EMPTY-NEXT:         pointer [size=8, align=8];
// EMPTY-NEXT:         stack_alignment = 16;
// EMPTY-NEXT:         long_double = f80;
// EMPTY-NEXT:         storage bool [size=1, align=1];
// EMPTY-NEXT:         storage i8, u8 [size=1, align=1];
// EMPTY-NEXT:         storage i16, u16 [size=2, align=2];
// EMPTY-NEXT:         storage i32, u32 [size=4, align=4];
// EMPTY-NEXT:         storage i64, u64 [size=8, align=8];
// EMPTY-NEXT:         storage i128, u128 [size=16, align=16];
// EMPTY-NEXT:         storage bf16 [size=2, align=2];
// EMPTY-NEXT:         storage f16 [size=2, align=2];
// EMPTY-NEXT:         storage f32 [size=4, align=4];
// EMPTY-NEXT:         storage f64 [size=8, align=8];
// EMPTY-NEXT:         storage f80 [size=16, align=16];
// EMPTY-NEXT:         storage f128 [size=16, align=16];
// EMPTY-NEXT:         storage d32 [size=4, align=4];
// EMPTY-NEXT:         storage d64 [size=8, align=8];
// EMPTY-NEXT:         storage d128 [size=16, align=16];
// EMPTY-NEXT:     }
// EMPTY-NEXT:     global %[[VALUE_macro_address:[0-9]+]] macro_address: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// EMPTY-NEXT:     global %[[VALUE_macro_hwaddress:[0-9]+]] macro_hwaddress: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// EMPTY-NEXT:     global %[[VALUE_macro_thread:[0-9]+]] macro_thread: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// EMPTY-NEXT:     global %[[VALUE_feature_address:[0-9]+]] feature_address: i32 [storage=static] = const<i32>(0) [linkage=external];
// EMPTY-NEXT:     global %[[VALUE_feature_hwaddress:[0-9]+]] feature_hwaddress: i32 [storage=static] = const<i32>(0) [linkage=external];
// EMPTY-NEXT:     global %[[VALUE_feature_memory:[0-9]+]] feature_memory: i32 [storage=static] = const<i32>(0) [linkage=external];
// EMPTY-NEXT:     global %[[VALUE_feature_thread:[0-9]+]] feature_thread: i32 [storage=static] = const<i32>(0) [linkage=external];
// EMPTY-NEXT:     global %[[VALUE_feature_undefined:[0-9]+]] feature_undefined: i32 [storage=static] = const<i32>(0) [linkage=external];
// EMPTY-NEXT:     global %[[VALUE_feature_leak:[0-9]+]] feature_leak: i32 [storage=static] = const<i32>(0) [linkage=external];
// EMPTY-NEXT:     global %[[VALUE_feature_safe_stack:[0-9]+]] feature_safe_stack: i32 [storage=static] = const<i32>(0) [linkage=external];
// EMPTY-NEXT:     global %[[VALUE_feature_coverage:[0-9]+]] feature_coverage: i32 [storage=static] = const<i32>(0) [linkage=external];
// EMPTY-NEXT:     global %[[VALUE_extension_address:[0-9]+]] extension_address: i32 [storage=static] = const<i32>(0) [linkage=external];
// EMPTY-NEXT: }
// SLATE-FILECHECK-END EMPTY
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
// RUNTIME_OPTIONS-NEXT:     global %[[VALUE_feature_safe_stack:[0-9]+]] feature_safe_stack: i32 [storage=static] = const<i32>(0) [linkage=external];
// RUNTIME_OPTIONS-NEXT:     global %[[VALUE_feature_coverage:[0-9]+]] feature_coverage: i32 [storage=static] = const<i32>(0) [linkage=external];
// RUNTIME_OPTIONS-NEXT:     global %[[VALUE_extension_address:[0-9]+]] extension_address: i32 [storage=static] = const<i32>(1) [linkage=external];
// RUNTIME_OPTIONS-NEXT: }
// SLATE-FILECHECK-END RUNTIME_OPTIONS
// SLATE-FILECHECK-BEGIN MINIMAL_RUNTIME
// MINIMAL_RUNTIME: module {
// MINIMAL_RUNTIME-NEXT:     target "x86_64-unknown-linux-gnu" {
// MINIMAL_RUNTIME-NEXT:         endian = little;
// MINIMAL_RUNTIME-NEXT:         pointer [size=8, align=8];
// MINIMAL_RUNTIME-NEXT:         stack_alignment = 16;
// MINIMAL_RUNTIME-NEXT:         long_double = f80;
// MINIMAL_RUNTIME-NEXT:         storage bool [size=1, align=1];
// MINIMAL_RUNTIME-NEXT:         storage i8, u8 [size=1, align=1];
// MINIMAL_RUNTIME-NEXT:         storage i16, u16 [size=2, align=2];
// MINIMAL_RUNTIME-NEXT:         storage i32, u32 [size=4, align=4];
// MINIMAL_RUNTIME-NEXT:         storage i64, u64 [size=8, align=8];
// MINIMAL_RUNTIME-NEXT:         storage i128, u128 [size=16, align=16];
// MINIMAL_RUNTIME-NEXT:         storage bf16 [size=2, align=2];
// MINIMAL_RUNTIME-NEXT:         storage f16 [size=2, align=2];
// MINIMAL_RUNTIME-NEXT:         storage f32 [size=4, align=4];
// MINIMAL_RUNTIME-NEXT:         storage f64 [size=8, align=8];
// MINIMAL_RUNTIME-NEXT:         storage f80 [size=16, align=16];
// MINIMAL_RUNTIME-NEXT:         storage f128 [size=16, align=16];
// MINIMAL_RUNTIME-NEXT:         storage d32 [size=4, align=4];
// MINIMAL_RUNTIME-NEXT:         storage d64 [size=8, align=8];
// MINIMAL_RUNTIME-NEXT:         storage d128 [size=16, align=16];
// MINIMAL_RUNTIME-NEXT:     }
// MINIMAL_RUNTIME-NEXT:     global %[[VALUE_macro_address:[0-9]+]] macro_address: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// MINIMAL_RUNTIME-NEXT:     global %[[VALUE_macro_hwaddress:[0-9]+]] macro_hwaddress: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// MINIMAL_RUNTIME-NEXT:     global %[[VALUE_macro_thread:[0-9]+]] macro_thread: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// MINIMAL_RUNTIME-NEXT:     global %[[VALUE_feature_address:[0-9]+]] feature_address: i32 [storage=static] = const<i32>(0) [linkage=external];
// MINIMAL_RUNTIME-NEXT:     global %[[VALUE_feature_hwaddress:[0-9]+]] feature_hwaddress: i32 [storage=static] = const<i32>(0) [linkage=external];
// MINIMAL_RUNTIME-NEXT:     global %[[VALUE_feature_memory:[0-9]+]] feature_memory: i32 [storage=static] = const<i32>(0) [linkage=external];
// MINIMAL_RUNTIME-NEXT:     global %[[VALUE_feature_thread:[0-9]+]] feature_thread: i32 [storage=static] = const<i32>(0) [linkage=external];
// MINIMAL_RUNTIME-NEXT:     global %[[VALUE_feature_undefined:[0-9]+]] feature_undefined: i32 [storage=static] = const<i32>(1) [linkage=external];
// MINIMAL_RUNTIME-NEXT:     global %[[VALUE_feature_leak:[0-9]+]] feature_leak: i32 [storage=static] = const<i32>(0) [linkage=external];
// MINIMAL_RUNTIME-NEXT:     global %[[VALUE_feature_safe_stack:[0-9]+]] feature_safe_stack: i32 [storage=static] = const<i32>(0) [linkage=external];
// MINIMAL_RUNTIME-NEXT:     global %[[VALUE_feature_coverage:[0-9]+]] feature_coverage: i32 [storage=static] = const<i32>(0) [linkage=external];
// MINIMAL_RUNTIME-NEXT:     global %[[VALUE_extension_address:[0-9]+]] extension_address: i32 [storage=static] = const<i32>(0) [linkage=external];
// MINIMAL_RUNTIME-NEXT: }
// SLATE-FILECHECK-END MINIMAL_RUNTIME
