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
// SLATE-FILECHECK-DEFINES NO-COMPAT
// SLATE-FILECHECK-PREFIX-ARGS NO-COMPAT -fno-ms-compatibility
// SLATE-FILECHECK-DEFINES NO-EXT
// SLATE-FILECHECK-PREFIX-ARGS NO-EXT -fno-ms-extensions
// SLATE-FILECHECK-DEFINES COMPAT-NO-EXT
// SLATE-FILECHECK-PREFIX-ARGS COMPAT-NO-EXT -fms-compatibility -fno-ms-extensions

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f64;
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
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     global %[[VALUE__MSC_VER_value:[0-9]+]] _MSC_VER_value: i32 [storage=static] = const<i32>(1933) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE__MSC_FULL_VER_value:[0-9]+]] _MSC_FULL_VER_value: i32 [storage=static] = const<i32>(193300000) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE__MSC_BUILD_value:[0-9]+]] _MSC_BUILD_value: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE__MSC_EXTENSIONS_value:[0-9]+]] _MSC_EXTENSIONS_value: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE__MSVC_CONSTEXPR_ATTRIBUTE_value:[0-9]+]] _MSVC_CONSTEXPR_ATTRIBUTE_value: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE__CRT_USE_BUILTIN_OFFSETOF_value:[0-9]+]] _CRT_USE_BUILTIN_OFFSETOF_value: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN NO-COMPAT
// NO-COMPAT: module {
// NO-COMPAT-NEXT:     target "x86_64-pc-windows-msvc" {
// NO-COMPAT-NEXT:         endian = little;
// NO-COMPAT-NEXT:         pointer [size=8, align=8];
// NO-COMPAT-NEXT:         stack_alignment = 16;
// NO-COMPAT-NEXT:         long_double = f64;
// NO-COMPAT-NEXT:         storage bool [size=1, align=1];
// NO-COMPAT-NEXT:         storage i8, u8 [size=1, align=1];
// NO-COMPAT-NEXT:         storage i16, u16 [size=2, align=2];
// NO-COMPAT-NEXT:         storage i32, u32 [size=4, align=4];
// NO-COMPAT-NEXT:         storage i64, u64 [size=8, align=8];
// NO-COMPAT-NEXT:         storage i128, u128 [size=16, align=16];
// NO-COMPAT-NEXT:         storage bf16 [size=2, align=2];
// NO-COMPAT-NEXT:         storage f16 [size=2, align=2];
// NO-COMPAT-NEXT:         storage f32 [size=4, align=4];
// NO-COMPAT-NEXT:         storage f64 [size=8, align=8];
// NO-COMPAT-NEXT:         storage f128 [size=16, align=16];
// NO-COMPAT-NEXT:         storage d32 [size=4, align=4];
// NO-COMPAT-NEXT:         storage d64 [size=8, align=8];
// NO-COMPAT-NEXT:         storage d128 [size=16, align=16];
// NO-COMPAT-NEXT:     }
// NO-COMPAT-NEXT:     global %[[VALUE___GNUC___value:[0-9]+]] __GNUC___value: i32 [storage=static] = const<i32>(4) [linkage=external];
// NO-COMPAT-NEXT:     global %[[VALUE___GNUC_MINOR___value:[0-9]+]] __GNUC_MINOR___value: i32 [storage=static] = const<i32>(2) [linkage=external];
// NO-COMPAT-NEXT:     global %[[VALUE___GNUC_PATCHLEVEL___value:[0-9]+]] __GNUC_PATCHLEVEL___value: i32 [storage=static] = const<i32>(1) [linkage=external];
// NO-COMPAT-NEXT:     global %[[VALUE___GNUC_STDC_INLINE___value:[0-9]+]] __GNUC_STDC_INLINE___value: i32 [storage=static] = const<i32>(1) [linkage=external];
// NO-COMPAT-NEXT:     global %[[VALUE___GXX_ABI_VERSION_value:[0-9]+]] __GXX_ABI_VERSION_value: i32 [storage=static] = const<i32>(1002) [linkage=external];
// NO-COMPAT-NEXT:     global %[[VALUE___STDC___value:[0-9]+]] __STDC___value: i32 [storage=static] = const<i32>(1) [linkage=external];
// NO-COMPAT-NEXT:     global %[[VALUE___GCC_ATOMIC_INT_LOCK_FREE_value:[0-9]+]] __GCC_ATOMIC_INT_LOCK_FREE_value: i32 [storage=static] = const<i32>(2) [linkage=external];
// NO-COMPAT-NEXT:     global %[[VALUE___GCC_ATOMIC_TEST_AND_SET_TRUEVAL_value:[0-9]+]] __GCC_ATOMIC_TEST_AND_SET_TRUEVAL_value: i32 [storage=static] = const<i32>(1) [linkage=external];
// NO-COMPAT-NEXT:     global %[[VALUE__MSC_VER_value:[0-9]+]] _MSC_VER_value: i32 [storage=static] = const<i32>(1933) [linkage=external];
// NO-COMPAT-NEXT:     global %[[VALUE__MSC_FULL_VER_value:[0-9]+]] _MSC_FULL_VER_value: i32 [storage=static] = const<i32>(193300000) [linkage=external];
// NO-COMPAT-NEXT:     global %[[VALUE__MSC_BUILD_value:[0-9]+]] _MSC_BUILD_value: i32 [storage=static] = const<i32>(1) [linkage=external];
// NO-COMPAT-NEXT:     global %[[VALUE__MSC_EXTENSIONS_value:[0-9]+]] _MSC_EXTENSIONS_value: i32 [storage=static] = const<i32>(1) [linkage=external];
// NO-COMPAT-NEXT:     global %[[VALUE__MSVC_CONSTEXPR_ATTRIBUTE_value:[0-9]+]] _MSVC_CONSTEXPR_ATTRIBUTE_value: i32 [storage=static] = const<i32>(1) [linkage=external];
// NO-COMPAT-NEXT:     global %[[VALUE__CRT_USE_BUILTIN_OFFSETOF_value:[0-9]+]] _CRT_USE_BUILTIN_OFFSETOF_value: i32 [storage=static] = const<i32>(1) [linkage=external];
// NO-COMPAT-NEXT: }
// SLATE-FILECHECK-END NO-COMPAT
// SLATE-FILECHECK-BEGIN NO-EXT
// NO-EXT: module {
// NO-EXT-NEXT:     target "x86_64-pc-windows-msvc" {
// NO-EXT-NEXT:         endian = little;
// NO-EXT-NEXT:         pointer [size=8, align=8];
// NO-EXT-NEXT:         stack_alignment = 16;
// NO-EXT-NEXT:         long_double = f64;
// NO-EXT-NEXT:         storage bool [size=1, align=1];
// NO-EXT-NEXT:         storage i8, u8 [size=1, align=1];
// NO-EXT-NEXT:         storage i16, u16 [size=2, align=2];
// NO-EXT-NEXT:         storage i32, u32 [size=4, align=4];
// NO-EXT-NEXT:         storage i64, u64 [size=8, align=8];
// NO-EXT-NEXT:         storage i128, u128 [size=16, align=16];
// NO-EXT-NEXT:         storage bf16 [size=2, align=2];
// NO-EXT-NEXT:         storage f16 [size=2, align=2];
// NO-EXT-NEXT:         storage f32 [size=4, align=4];
// NO-EXT-NEXT:         storage f64 [size=8, align=8];
// NO-EXT-NEXT:         storage f128 [size=16, align=16];
// NO-EXT-NEXT:         storage d32 [size=4, align=4];
// NO-EXT-NEXT:         storage d64 [size=8, align=8];
// NO-EXT-NEXT:         storage d128 [size=16, align=16];
// NO-EXT-NEXT:     }
// NO-EXT-NEXT:     global %[[VALUE___GNUC___value:[0-9]+]] __GNUC___value: i32 [storage=static] = const<i32>(4) [linkage=external];
// NO-EXT-NEXT:     global %[[VALUE___GNUC_MINOR___value:[0-9]+]] __GNUC_MINOR___value: i32 [storage=static] = const<i32>(2) [linkage=external];
// NO-EXT-NEXT:     global %[[VALUE___GNUC_PATCHLEVEL___value:[0-9]+]] __GNUC_PATCHLEVEL___value: i32 [storage=static] = const<i32>(1) [linkage=external];
// NO-EXT-NEXT:     global %[[VALUE___GNUC_STDC_INLINE___value:[0-9]+]] __GNUC_STDC_INLINE___value: i32 [storage=static] = const<i32>(1) [linkage=external];
// NO-EXT-NEXT:     global %[[VALUE___GXX_ABI_VERSION_value:[0-9]+]] __GXX_ABI_VERSION_value: i32 [storage=static] = const<i32>(1002) [linkage=external];
// NO-EXT-NEXT:     global %[[VALUE___STDC___value:[0-9]+]] __STDC___value: i32 [storage=static] = const<i32>(1) [linkage=external];
// NO-EXT-NEXT:     global %[[VALUE___GCC_ATOMIC_INT_LOCK_FREE_value:[0-9]+]] __GCC_ATOMIC_INT_LOCK_FREE_value: i32 [storage=static] = const<i32>(2) [linkage=external];
// NO-EXT-NEXT:     global %[[VALUE___GCC_ATOMIC_TEST_AND_SET_TRUEVAL_value:[0-9]+]] __GCC_ATOMIC_TEST_AND_SET_TRUEVAL_value: i32 [storage=static] = const<i32>(1) [linkage=external];
// NO-EXT-NEXT: }
// SLATE-FILECHECK-END NO-EXT
// SLATE-FILECHECK-BEGIN COMPAT-NO-EXT
// COMPAT-NO-EXT: module {
// COMPAT-NO-EXT-NEXT:     target "x86_64-pc-windows-msvc" {
// COMPAT-NO-EXT-NEXT:         endian = little;
// COMPAT-NO-EXT-NEXT:         pointer [size=8, align=8];
// COMPAT-NO-EXT-NEXT:         stack_alignment = 16;
// COMPAT-NO-EXT-NEXT:         long_double = f64;
// COMPAT-NO-EXT-NEXT:         storage bool [size=1, align=1];
// COMPAT-NO-EXT-NEXT:         storage i8, u8 [size=1, align=1];
// COMPAT-NO-EXT-NEXT:         storage i16, u16 [size=2, align=2];
// COMPAT-NO-EXT-NEXT:         storage i32, u32 [size=4, align=4];
// COMPAT-NO-EXT-NEXT:         storage i64, u64 [size=8, align=8];
// COMPAT-NO-EXT-NEXT:         storage i128, u128 [size=16, align=16];
// COMPAT-NO-EXT-NEXT:         storage bf16 [size=2, align=2];
// COMPAT-NO-EXT-NEXT:         storage f16 [size=2, align=2];
// COMPAT-NO-EXT-NEXT:         storage f32 [size=4, align=4];
// COMPAT-NO-EXT-NEXT:         storage f64 [size=8, align=8];
// COMPAT-NO-EXT-NEXT:         storage f128 [size=16, align=16];
// COMPAT-NO-EXT-NEXT:         storage d32 [size=4, align=4];
// COMPAT-NO-EXT-NEXT:         storage d64 [size=8, align=8];
// COMPAT-NO-EXT-NEXT:         storage d128 [size=16, align=16];
// COMPAT-NO-EXT-NEXT:     }
// COMPAT-NO-EXT-NEXT:     global %[[VALUE__MSC_EXTENSIONS_value:[0-9]+]] _MSC_EXTENSIONS_value: i32 [storage=static] = const<i32>(1) [linkage=external];
// COMPAT-NO-EXT-NEXT: }
// SLATE-FILECHECK-END COMPAT-NO-EXT
