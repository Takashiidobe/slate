// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

#define PACKED_NAME packed
#define BSWAP_NAME __builtin_bswap16
#define ATOMIC_NAME c_atomic
#define HAS(x) __has_attribute(x)

int text_attribute = __has_attribute(packed);
int text_missing_attribute = __has_attribute(no_such_attribute);
int text_builtin = __has_builtin(__builtin_bswap16);
int text_feature = __has_feature(c_atomic);
int text_from_macro = HAS(packed);
int text_macro_attribute = __has_attribute(PACKED_NAME);
int text_macro_builtin = __has_builtin(BSWAP_NAME);
int text_macro_feature = __has_feature(ATOMIC_NAME);

int checked(int x) {
  if (__has_attribute(packed) && __has_builtin(__builtin_bswap16))
    return x;
  return 0;
}

#if __has_attribute(PACKED_NAME)
int if_macro_attribute;
#endif
#if __has_builtin(BSWAP_NAME)
int if_macro_builtin;
#endif
#if __has_feature(ATOMIC_NAME)
int if_macro_feature;
#endif

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
// DEFAULT-NEXT:     global %[[VALUE_text_attribute:[0-9]+]] text_attribute: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_text_missing_attribute:[0-9]+]] text_missing_attribute: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_text_builtin:[0-9]+]] text_builtin: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_text_feature:[0-9]+]] text_feature: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_text_from_macro:[0-9]+]] text_from_macro: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_text_macro_attribute:[0-9]+]] text_macro_attribute: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_text_macro_builtin:[0-9]+]] text_macro_builtin: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_text_macro_feature:[0-9]+]] text_macro_feature: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_if_macro_attribute:[0-9]+]] if_macro_attribute: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_checked:[0-9]+]] @checked(%[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(ne<i32>(const<i32>(1), const<i32>(0)), ne<i32>(const<i32>(1), const<i32>(0)))
// DEFAULT-NEXT:             return read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
