#define LIMIT 1 + 1
#if __has_embed("pp-has-embed-parameters.bin" limit(LIMIT) prefix(1,) suffix(,2) if_empty(0)) == 1
int found_with_parameters;
#endif
#if __has_embed("pp-has-embed-parameters.bin" __limit__(0) __if_empty__(0)) == 2
int empty_after_limit;
#endif
#if __has_embed("pp-has-embed-parameters.bin" clang::offset(3)) == 2
int empty_after_offset;
#endif
#if __has_embed("pp-has-embed-parameters.bin" gnu::offset(1)) == 0
int foreign_vendor_parameter;
#endif
#if __has_embed("pp-has-embed-parameters.bin" unknown(1)) == 0
int unknown_parameter;
#endif
#if __has_embed("missing.bin" limit(1)) == 0
int missing_resource;
#endif
int window[] = {
#embed "pp-has-embed-parameters.bin" limit(1) clang::offset(1) prefix(0,) suffix(,9)
};

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT c23

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
// DEFAULT-NEXT:     global %[[VALUE_found_with_parameters:[0-9]+]] found_with_parameters: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_empty_after_limit:[0-9]+]] empty_after_limit: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_empty_after_offset:[0-9]+]] empty_after_offset: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_foreign_vendor_parameter:[0-9]+]] foreign_vendor_parameter: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_unknown_parameter:[0-9]+]] unknown_parameter: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_missing_resource:[0-9]+]] missing_resource: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_window:[0-9]+]] window: array<i32, 3> [storage=static] = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(98), index2 = const<i32>(9)) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
