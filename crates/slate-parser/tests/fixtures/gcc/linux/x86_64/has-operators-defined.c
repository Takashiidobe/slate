// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

#ifdef __has_include
int defines__has_include;
#endif
#ifdef __has_include_next
int defines__has_include_next;
#endif
#ifdef __has_embed
int defines__has_embed;
#endif
#ifdef __has_attribute
int defines__has_attribute;
#endif
#ifdef __has_c_attribute
int defines__has_c_attribute;
#endif
#ifdef __has_cpp_attribute
int defines__has_cpp_attribute;
#endif
#ifdef __has_builtin
int defines__has_builtin;
#endif
#ifdef __has_feature
int defines__has_feature;
#endif
#ifdef __has_extension
int defines__has_extension;
#endif
#ifdef __has_declspec_attribute
int defines__has_declspec_attribute;
#endif
#ifdef __has_warning
int defines__has_warning;
#endif
#ifdef __is_identifier
int defines__is_identifier;
#endif
#ifdef __building_module
int defines__building_module;
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
// DEFAULT-NEXT:     global %[[VALUE_defines__has_include:[0-9]+]] defines__has_include: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_defines__has_include_next:[0-9]+]] defines__has_include_next: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_defines__has_embed:[0-9]+]] defines__has_embed: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_defines__has_attribute:[0-9]+]] defines__has_attribute: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_defines__has_c_attribute:[0-9]+]] defines__has_c_attribute: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_defines__has_cpp_attribute:[0-9]+]] defines__has_cpp_attribute: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_defines__has_builtin:[0-9]+]] defines__has_builtin: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_defines__has_feature:[0-9]+]] defines__has_feature: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_defines__has_extension:[0-9]+]] defines__has_extension: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
