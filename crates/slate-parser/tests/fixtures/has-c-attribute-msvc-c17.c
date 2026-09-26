// SLATE-FILECHECK-FLAVOR msvc
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir -std=c17 -target=x86_64-pc-windows-msvc

// cl rejects a scoped operand before C23 (C2278), so this probes only unscoped names
#ifdef __has_c_attribute
int defines_has_c_attribute;
#endif

#if !__has_c_attribute(deprecated)
int deprecated_0;
#elif __has_c_attribute(deprecated) == 1L
int deprecated_1;
#elif __has_c_attribute(deprecated) == 201904L
int deprecated_201904;
#elif __has_c_attribute(deprecated) == 201910L
int deprecated_201910;
#elif __has_c_attribute(deprecated) == 202003L
int deprecated_202003;
#elif __has_c_attribute(deprecated) == 202106L
int deprecated_202106;
#elif __has_c_attribute(deprecated) == 202202L
int deprecated_202202;
#elif __has_c_attribute(deprecated) == 202311L
int deprecated_202311;
#else
int deprecated_other;
#endif

#if !__has_c_attribute(fallthrough)
int fallthrough_0;
#elif __has_c_attribute(fallthrough) == 1L
int fallthrough_1;
#elif __has_c_attribute(fallthrough) == 201904L
int fallthrough_201904;
#elif __has_c_attribute(fallthrough) == 201910L
int fallthrough_201910;
#elif __has_c_attribute(fallthrough) == 202003L
int fallthrough_202003;
#elif __has_c_attribute(fallthrough) == 202106L
int fallthrough_202106;
#elif __has_c_attribute(fallthrough) == 202202L
int fallthrough_202202;
#elif __has_c_attribute(fallthrough) == 202311L
int fallthrough_202311;
#else
int fallthrough_other;
#endif

#if !__has_c_attribute(maybe_unused)
int maybe_unused_0;
#elif __has_c_attribute(maybe_unused) == 1L
int maybe_unused_1;
#elif __has_c_attribute(maybe_unused) == 201904L
int maybe_unused_201904;
#elif __has_c_attribute(maybe_unused) == 201910L
int maybe_unused_201910;
#elif __has_c_attribute(maybe_unused) == 202003L
int maybe_unused_202003;
#elif __has_c_attribute(maybe_unused) == 202106L
int maybe_unused_202106;
#elif __has_c_attribute(maybe_unused) == 202202L
int maybe_unused_202202;
#elif __has_c_attribute(maybe_unused) == 202311L
int maybe_unused_202311;
#else
int maybe_unused_other;
#endif

#if !__has_c_attribute(nodiscard)
int nodiscard_0;
#elif __has_c_attribute(nodiscard) == 1L
int nodiscard_1;
#elif __has_c_attribute(nodiscard) == 201904L
int nodiscard_201904;
#elif __has_c_attribute(nodiscard) == 201910L
int nodiscard_201910;
#elif __has_c_attribute(nodiscard) == 202003L
int nodiscard_202003;
#elif __has_c_attribute(nodiscard) == 202106L
int nodiscard_202106;
#elif __has_c_attribute(nodiscard) == 202202L
int nodiscard_202202;
#elif __has_c_attribute(nodiscard) == 202311L
int nodiscard_202311;
#else
int nodiscard_other;
#endif

#if !__has_c_attribute(noreturn)
int noreturn_0;
#elif __has_c_attribute(noreturn) == 1L
int noreturn_1;
#elif __has_c_attribute(noreturn) == 201904L
int noreturn_201904;
#elif __has_c_attribute(noreturn) == 201910L
int noreturn_201910;
#elif __has_c_attribute(noreturn) == 202003L
int noreturn_202003;
#elif __has_c_attribute(noreturn) == 202106L
int noreturn_202106;
#elif __has_c_attribute(noreturn) == 202202L
int noreturn_202202;
#elif __has_c_attribute(noreturn) == 202311L
int noreturn_202311;
#else
int noreturn_other;
#endif

#if !__has_c_attribute(_Noreturn)
int _Noreturn_0;
#elif __has_c_attribute(_Noreturn) == 1L
int _Noreturn_1;
#elif __has_c_attribute(_Noreturn) == 201904L
int _Noreturn_201904;
#elif __has_c_attribute(_Noreturn) == 201910L
int _Noreturn_201910;
#elif __has_c_attribute(_Noreturn) == 202003L
int _Noreturn_202003;
#elif __has_c_attribute(_Noreturn) == 202106L
int _Noreturn_202106;
#elif __has_c_attribute(_Noreturn) == 202202L
int _Noreturn_202202;
#elif __has_c_attribute(_Noreturn) == 202311L
int _Noreturn_202311;
#else
int _Noreturn_other;
#endif

#if !__has_c_attribute(unsequenced)
int unsequenced_0;
#elif __has_c_attribute(unsequenced) == 1L
int unsequenced_1;
#elif __has_c_attribute(unsequenced) == 201904L
int unsequenced_201904;
#elif __has_c_attribute(unsequenced) == 201910L
int unsequenced_201910;
#elif __has_c_attribute(unsequenced) == 202003L
int unsequenced_202003;
#elif __has_c_attribute(unsequenced) == 202106L
int unsequenced_202106;
#elif __has_c_attribute(unsequenced) == 202202L
int unsequenced_202202;
#elif __has_c_attribute(unsequenced) == 202311L
int unsequenced_202311;
#else
int unsequenced_other;
#endif

#if !__has_c_attribute(reproducible)
int reproducible_0;
#elif __has_c_attribute(reproducible) == 1L
int reproducible_1;
#elif __has_c_attribute(reproducible) == 201904L
int reproducible_201904;
#elif __has_c_attribute(reproducible) == 201910L
int reproducible_201910;
#elif __has_c_attribute(reproducible) == 202003L
int reproducible_202003;
#elif __has_c_attribute(reproducible) == 202106L
int reproducible_202106;
#elif __has_c_attribute(reproducible) == 202202L
int reproducible_202202;
#elif __has_c_attribute(reproducible) == 202311L
int reproducible_202311;
#else
int reproducible_other;
#endif

#if !__has_c_attribute(__nodiscard__)
int __nodiscard___0;
#elif __has_c_attribute(__nodiscard__) == 1L
int __nodiscard___1;
#elif __has_c_attribute(__nodiscard__) == 201904L
int __nodiscard___201904;
#elif __has_c_attribute(__nodiscard__) == 201910L
int __nodiscard___201910;
#elif __has_c_attribute(__nodiscard__) == 202003L
int __nodiscard___202003;
#elif __has_c_attribute(__nodiscard__) == 202106L
int __nodiscard___202106;
#elif __has_c_attribute(__nodiscard__) == 202202L
int __nodiscard___202202;
#elif __has_c_attribute(__nodiscard__) == 202311L
int __nodiscard___202311;
#else
int __nodiscard___other;
#endif

#if !__has_c_attribute(packed)
int packed_0;
#elif __has_c_attribute(packed) == 1L
int packed_1;
#elif __has_c_attribute(packed) == 201904L
int packed_201904;
#elif __has_c_attribute(packed) == 201910L
int packed_201910;
#elif __has_c_attribute(packed) == 202003L
int packed_202003;
#elif __has_c_attribute(packed) == 202106L
int packed_202106;
#elif __has_c_attribute(packed) == 202202L
int packed_202202;
#elif __has_c_attribute(packed) == 202311L
int packed_202311;
#else
int packed_other;
#endif

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
// DEFAULT-NEXT:     global %0 defines_has_c_attribute: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 deprecated_0: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 fallthrough_0: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 maybe_unused_0: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 nodiscard_0: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 noreturn_0: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 _Noreturn_0: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %7 unsequenced_0: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %8 reproducible_0: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %9 __nodiscard___0: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %10 packed_0: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
