#ifdef __GNUC__
int __GNUC___value = __GNUC__;
#endif
#ifdef __GNUC_MINOR__
int __GNUC_MINOR___value = __GNUC_MINOR__;
#endif
#ifdef __GNUC_PATCHLEVEL__
int __GNUC_PATCHLEVEL___value = __GNUC_PATCHLEVEL__;
#endif
#ifdef __GNUC_STDC_INLINE__
int __GNUC_STDC_INLINE___value = __GNUC_STDC_INLINE__;
#endif
#ifdef __GXX_ABI_VERSION
int __GXX_ABI_VERSION_value = __GXX_ABI_VERSION;
#endif
#ifdef __STDC__
int __STDC___value = __STDC__;
#endif
#ifdef __GCC_ATOMIC_INT_LOCK_FREE
int __GCC_ATOMIC_INT_LOCK_FREE_value = __GCC_ATOMIC_INT_LOCK_FREE;
#endif
#ifdef __GCC_ATOMIC_TEST_AND_SET_TRUEVAL
int __GCC_ATOMIC_TEST_AND_SET_TRUEVAL_value = __GCC_ATOMIC_TEST_AND_SET_TRUEVAL;
#endif
#ifdef _MSC_VER
int _MSC_VER_value = _MSC_VER;
#endif
#ifdef _MSC_FULL_VER
int _MSC_FULL_VER_value = _MSC_FULL_VER;
#endif
#ifdef _MSC_BUILD
int _MSC_BUILD_value = _MSC_BUILD;
#endif
#ifdef _MSC_EXTENSIONS
int _MSC_EXTENSIONS_value = _MSC_EXTENSIONS;
#endif
#ifdef _MSVC_CONSTEXPR_ATTRIBUTE
int _MSVC_CONSTEXPR_ATTRIBUTE_value = _MSVC_CONSTEXPR_ATTRIBUTE;
#endif
#ifdef _CRT_USE_BUILTIN_OFFSETOF
int _CRT_USE_BUILTIN_OFFSETOF_value = _CRT_USE_BUILTIN_OFFSETOF;
#endif
#ifdef _M_IX86_FP
int _M_IX86_FP_value = _M_IX86_FP;
#endif

// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-DEFINES EXT
// SLATE-FILECHECK-PREFIX-ARGS EXT -fms-extensions
// SLATE-FILECHECK-DEFINES COMPAT
// SLATE-FILECHECK-PREFIX-ARGS COMPAT -fms-compatibility

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "i686-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=4, align=4];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=4];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=4];
// DEFAULT-NEXT:         storage f80 [size=12, align=4];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     global %[[VALUE___GNUC___value:[0-9]+]] __GNUC___value: i32 [storage=static] = const<i32>(4) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE___GNUC_MINOR___value:[0-9]+]] __GNUC_MINOR___value: i32 [storage=static] = const<i32>(2) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE___GNUC_PATCHLEVEL___value:[0-9]+]] __GNUC_PATCHLEVEL___value: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE___GNUC_STDC_INLINE___value:[0-9]+]] __GNUC_STDC_INLINE___value: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE___GXX_ABI_VERSION_value:[0-9]+]] __GXX_ABI_VERSION_value: i32 [storage=static] = const<i32>(1002) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE___STDC___value:[0-9]+]] __STDC___value: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE___GCC_ATOMIC_INT_LOCK_FREE_value:[0-9]+]] __GCC_ATOMIC_INT_LOCK_FREE_value: i32 [storage=static] = const<i32>(2) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE___GCC_ATOMIC_TEST_AND_SET_TRUEVAL_value:[0-9]+]] __GCC_ATOMIC_TEST_AND_SET_TRUEVAL_value: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN EXT
// EXT: module {
// EXT-NEXT:     target "i686-unknown-linux-gnu" {
// EXT-NEXT:         endian = little;
// EXT-NEXT:         pointer [size=4, align=4];
// EXT-NEXT:         stack_alignment = 16;
// EXT-NEXT:         long_double = f80;
// EXT-NEXT:         storage bool [size=1, align=1];
// EXT-NEXT:         storage i8, u8 [size=1, align=1];
// EXT-NEXT:         storage i16, u16 [size=2, align=2];
// EXT-NEXT:         storage i32, u32 [size=4, align=4];
// EXT-NEXT:         storage i64, u64 [size=8, align=4];
// EXT-NEXT:         storage i128, u128 [size=16, align=16];
// EXT-NEXT:         storage bf16 [size=2, align=2];
// EXT-NEXT:         storage f16 [size=2, align=2];
// EXT-NEXT:         storage f32 [size=4, align=4];
// EXT-NEXT:         storage f64 [size=8, align=4];
// EXT-NEXT:         storage f80 [size=12, align=4];
// EXT-NEXT:         storage f128 [size=16, align=16];
// EXT-NEXT:         storage d32 [size=4, align=4];
// EXT-NEXT:         storage d64 [size=8, align=8];
// EXT-NEXT:         storage d128 [size=16, align=16];
// EXT-NEXT:     }
// EXT-NEXT:     global %[[VALUE___GNUC___value:[0-9]+]] __GNUC___value: i32 [storage=static] = const<i32>(4) [linkage=external];
// EXT-NEXT:     global %[[VALUE___GNUC_MINOR___value:[0-9]+]] __GNUC_MINOR___value: i32 [storage=static] = const<i32>(2) [linkage=external];
// EXT-NEXT:     global %[[VALUE___GNUC_PATCHLEVEL___value:[0-9]+]] __GNUC_PATCHLEVEL___value: i32 [storage=static] = const<i32>(1) [linkage=external];
// EXT-NEXT:     global %[[VALUE___GNUC_STDC_INLINE___value:[0-9]+]] __GNUC_STDC_INLINE___value: i32 [storage=static] = const<i32>(1) [linkage=external];
// EXT-NEXT:     global %[[VALUE___GXX_ABI_VERSION_value:[0-9]+]] __GXX_ABI_VERSION_value: i32 [storage=static] = const<i32>(1002) [linkage=external];
// EXT-NEXT:     global %[[VALUE___STDC___value:[0-9]+]] __STDC___value: i32 [storage=static] = const<i32>(1) [linkage=external];
// EXT-NEXT:     global %[[VALUE___GCC_ATOMIC_INT_LOCK_FREE_value:[0-9]+]] __GCC_ATOMIC_INT_LOCK_FREE_value: i32 [storage=static] = const<i32>(2) [linkage=external];
// EXT-NEXT:     global %[[VALUE___GCC_ATOMIC_TEST_AND_SET_TRUEVAL_value:[0-9]+]] __GCC_ATOMIC_TEST_AND_SET_TRUEVAL_value: i32 [storage=static] = const<i32>(1) [linkage=external];
// EXT-NEXT:     global %[[VALUE__M_IX86_FP_value:[0-9]+]] _M_IX86_FP_value: i32 [storage=static] = const<i32>(2) [linkage=external];
// EXT-NEXT: }
// SLATE-FILECHECK-END EXT
// SLATE-FILECHECK-BEGIN COMPAT
// COMPAT: module {
// COMPAT-NEXT:     target "i686-unknown-linux-gnu" {
// COMPAT-NEXT:         endian = little;
// COMPAT-NEXT:         pointer [size=4, align=4];
// COMPAT-NEXT:         stack_alignment = 16;
// COMPAT-NEXT:         long_double = f80;
// COMPAT-NEXT:         storage bool [size=1, align=1];
// COMPAT-NEXT:         storage i8, u8 [size=1, align=1];
// COMPAT-NEXT:         storage i16, u16 [size=2, align=2];
// COMPAT-NEXT:         storage i32, u32 [size=4, align=4];
// COMPAT-NEXT:         storage i64, u64 [size=8, align=4];
// COMPAT-NEXT:         storage i128, u128 [size=16, align=16];
// COMPAT-NEXT:         storage bf16 [size=2, align=2];
// COMPAT-NEXT:         storage f16 [size=2, align=2];
// COMPAT-NEXT:         storage f32 [size=4, align=4];
// COMPAT-NEXT:         storage f64 [size=8, align=4];
// COMPAT-NEXT:         storage f80 [size=12, align=4];
// COMPAT-NEXT:         storage f128 [size=16, align=16];
// COMPAT-NEXT:         storage d32 [size=4, align=4];
// COMPAT-NEXT:         storage d64 [size=8, align=8];
// COMPAT-NEXT:         storage d128 [size=16, align=16];
// COMPAT-NEXT:     }
// COMPAT-NEXT:     global %[[VALUE__M_IX86_FP_value:[0-9]+]] _M_IX86_FP_value: i32 [storage=static] = const<i32>(2) [linkage=external];
// COMPAT-NEXT: }
// SLATE-FILECHECK-END COMPAT
