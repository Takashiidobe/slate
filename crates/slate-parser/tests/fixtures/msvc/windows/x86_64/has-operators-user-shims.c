// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

#ifndef __has_builtin
#define __has_builtin(x) 0
#endif
#ifndef __has_extension
#define __has_extension(x) 0
#endif
#ifndef __has_feature
#define __has_feature(x) 0
#endif
#ifndef __has_attribute
#define __has_attribute(x) 0
#endif
#if __has_builtin(__builtin_expect)
int builtin_true;
#else
int builtin_false;
#endif
#if __has_extension(c_atomic)
int ext_true;
#else
int ext_false;
#endif
#if __has_feature(c_atomic)
int feat_true;
#else
int feat_false;
#endif
#if __has_attribute(noreturn)
int attr_true;
#else
int attr_false;
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
// DEFAULT-NEXT:     global %[[VALUE_builtin_false:[0-9]+]] builtin_false: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ext_false:[0-9]+]] ext_false: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_feat_false:[0-9]+]] feat_false: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_attr_false:[0-9]+]] attr_false: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
