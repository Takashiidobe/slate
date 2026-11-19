// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES SVE
typedef svint32_t (*unary_fn)(svint32_t);
typedef svbool_t (predicate_fn)(svint32_t, svint32_t);
svint32_t (*hook)(svint32_t);
void prototype_only(svint32_t, svbool_t);
void takes_callback(svint32_t (*callback)(int));
typedef __builtin_va_list (*list_fn)(int);
typedef __int128_t (*wide_fn)(__int128_t);
void unnamed_list(__builtin_va_list);

// SLATE-FILECHECK-BEGIN SVE
// SVE: module {
// SVE-NEXT:     target "aarch64-unknown-linux-gnu" {
// SVE-NEXT:         endian = little;
// SVE-NEXT:         pointer [size=8, align=8];
// SVE-NEXT:         stack_alignment = 16;
// SVE-NEXT:         long_double = f128;
// SVE-NEXT:         storage bool [size=1, align=1];
// SVE-NEXT:         storage i8, u8 [size=1, align=1];
// SVE-NEXT:         storage i16, u16 [size=2, align=2];
// SVE-NEXT:         storage i32, u32 [size=4, align=4];
// SVE-NEXT:         storage i64, u64 [size=8, align=8];
// SVE-NEXT:         storage i128, u128 [size=16, align=16];
// SVE-NEXT:         storage bf16 [size=2, align=2];
// SVE-NEXT:         storage f16 [size=2, align=2];
// SVE-NEXT:         storage f32 [size=4, align=4];
// SVE-NEXT:         storage f64 [size=8, align=8];
// SVE-NEXT:         storage f128 [size=16, align=16];
// SVE-NEXT:         storage d32 [size=4, align=4];
// SVE-NEXT:         storage d64 [size=8, align=8];
// SVE-NEXT:         storage d128 [size=16, align=16];
// SVE-NEXT:     }
// SVE-NEXT:     type @type[[TYPE_unary_fn:[0-9]+]] unary_fn = ptr<fn(sizeless<svint32_t>) -> sizeless<svint32_t>>;
// SVE-NEXT:     type @type[[TYPE_predicate_fn:[0-9]+]] predicate_fn = fn(sizeless<svint32_t>, sizeless<svint32_t>) -> sizeless<svbool_t>;
// SVE-NEXT:     type @type[[TYPE_list_fn:[0-9]+]] list_fn = ptr<fn(i32) -> va_list>;
// SVE-NEXT:     type @type[[TYPE_wide_fn:[0-9]+]] wide_fn = ptr<fn(i128) -> i128>;
// SVE-NEXT:     global %[[VALUE_hook:[0-9]+]] hook: ptr<fn(sizeless<svint32_t>) -> sizeless<svint32_t>> [storage=static] [linkage=external];
// SVE-NEXT:     fn %[[VALUE_prototype_only:[0-9]+]] @prototype_only(%[[VALUE0:[0-9]+]] <unnamed>: sizeless<svint32_t>, %[[VALUE1:[0-9]+]] <unnamed>: sizeless<svbool_t>) -> void [linkage=external];
// SVE-NEXT:     fn %[[VALUE_takes_callback:[0-9]+]] @takes_callback(%[[VALUE_callback:[0-9]+]] callback: ptr<fn(i32) -> sizeless<svint32_t>>) -> void [linkage=external];
// SVE-NEXT:     fn %[[VALUE_unnamed_list:[0-9]+]] @unnamed_list(%[[VALUE2:[0-9]+]] <unnamed>: va_list) -> void [linkage=external];
// SVE-NEXT: }
// SLATE-FILECHECK-END SVE
