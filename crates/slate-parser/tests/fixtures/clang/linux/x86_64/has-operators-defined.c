// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

// __has_declspec_attribute, __has_warning and __is_identifier are clang operators slate does not evaluate yet, so they stay undefined
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
// DEFAULT-NEXT:     global %0 defines__has_include: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 defines__has_include_next: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 defines__has_embed: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 defines__has_attribute: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 defines__has_c_attribute: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 defines__has_builtin: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 defines__has_feature: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %7 defines__has_extension: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %8 defines__building_module: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
