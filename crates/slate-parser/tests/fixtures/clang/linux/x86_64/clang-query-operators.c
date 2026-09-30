#if __has_declspec_attribute(noreturn)
int declspec_noreturn;
#endif
#if __has_warning("-Wshadow")
int warning_shadow;
#endif
#if __has_warning("-Wbogus")
int warning_bogus;
#endif
#if __is_identifier(foo)
int identifier_foo;
#endif
#if __is_identifier(__FILE__)
int identifier_file;
#endif
#if __is_identifier(__builtin_va_arg)
int identifier_builtin_va_arg;
#endif
#if __is_identifier(asm)
int identifier_asm;
#endif
#if __is_identifier(typeof)
int identifier_typeof;
#endif
#if __is_identifier(bool)
int identifier_bool;
#endif
#if __is_identifier(__int64)
int identifier_int64;
#endif

// SLATE-FILECHECK-DEFINES GNU
// SLATE-FILECHECK-STD GNU gnu17
// SLATE-FILECHECK-DEFINES ISO
// SLATE-FILECHECK-STD ISO c17
// SLATE-FILECHECK-DEFINES C23
// SLATE-FILECHECK-STD C23 c23

// SLATE-FILECHECK-BEGIN GNU
// GNU: module {
// GNU-NEXT:     target "x86_64-unknown-linux-gnu" {
// GNU-NEXT:         endian = little;
// GNU-NEXT:         pointer [size=8, align=8];
// GNU-NEXT:         stack_alignment = 16;
// GNU-NEXT:         long_double = f80;
// GNU-NEXT:         storage bool [size=1, align=1];
// GNU-NEXT:         storage i8, u8 [size=1, align=1];
// GNU-NEXT:         storage i16, u16 [size=2, align=2];
// GNU-NEXT:         storage i32, u32 [size=4, align=4];
// GNU-NEXT:         storage i64, u64 [size=8, align=8];
// GNU-NEXT:         storage i128, u128 [size=16, align=16];
// GNU-NEXT:         storage bf16 [size=2, align=2];
// GNU-NEXT:         storage f16 [size=2, align=2];
// GNU-NEXT:         storage f32 [size=4, align=4];
// GNU-NEXT:         storage f64 [size=8, align=8];
// GNU-NEXT:         storage f80 [size=16, align=16];
// GNU-NEXT:         storage f128 [size=16, align=16];
// GNU-NEXT:         storage d32 [size=4, align=4];
// GNU-NEXT:         storage d64 [size=8, align=8];
// GNU-NEXT:         storage d128 [size=16, align=16];
// GNU-NEXT:     }
// GNU-NEXT:     global %[[VALUE_warning_shadow:[0-9]+]] warning_shadow: i32 [storage=static] [linkage=external];
// GNU-NEXT:     global %[[VALUE_identifier_foo:[0-9]+]] identifier_foo: i32 [storage=static] [linkage=external];
// GNU-NEXT:     global %[[VALUE_identifier_file:[0-9]+]] identifier_file: i32 [storage=static] [linkage=external];
// GNU-NEXT:     global %[[VALUE_identifier_bool:[0-9]+]] identifier_bool: i32 [storage=static] [linkage=external];
// GNU-NEXT:     global %[[VALUE_identifier_int64:[0-9]+]] identifier_int64: i32 [storage=static] [linkage=external];
// GNU-NEXT: }
// SLATE-FILECHECK-END GNU
// SLATE-FILECHECK-BEGIN ISO
// ISO: module {
// ISO-NEXT:     target "x86_64-unknown-linux-gnu" {
// ISO-NEXT:         endian = little;
// ISO-NEXT:         pointer [size=8, align=8];
// ISO-NEXT:         stack_alignment = 16;
// ISO-NEXT:         long_double = f80;
// ISO-NEXT:         storage bool [size=1, align=1];
// ISO-NEXT:         storage i8, u8 [size=1, align=1];
// ISO-NEXT:         storage i16, u16 [size=2, align=2];
// ISO-NEXT:         storage i32, u32 [size=4, align=4];
// ISO-NEXT:         storage i64, u64 [size=8, align=8];
// ISO-NEXT:         storage i128, u128 [size=16, align=16];
// ISO-NEXT:         storage bf16 [size=2, align=2];
// ISO-NEXT:         storage f16 [size=2, align=2];
// ISO-NEXT:         storage f32 [size=4, align=4];
// ISO-NEXT:         storage f64 [size=8, align=8];
// ISO-NEXT:         storage f80 [size=16, align=16];
// ISO-NEXT:         storage f128 [size=16, align=16];
// ISO-NEXT:         storage d32 [size=4, align=4];
// ISO-NEXT:         storage d64 [size=8, align=8];
// ISO-NEXT:         storage d128 [size=16, align=16];
// ISO-NEXT:     }
// ISO-NEXT:     global %[[VALUE_warning_shadow:[0-9]+]] warning_shadow: i32 [storage=static] [linkage=external];
// ISO-NEXT:     global %[[VALUE_identifier_foo:[0-9]+]] identifier_foo: i32 [storage=static] [linkage=external];
// ISO-NEXT:     global %[[VALUE_identifier_file:[0-9]+]] identifier_file: i32 [storage=static] [linkage=external];
// ISO-NEXT:     global %[[VALUE_identifier_asm:[0-9]+]] identifier_asm: i32 [storage=static] [linkage=external];
// ISO-NEXT:     global %[[VALUE_identifier_typeof:[0-9]+]] identifier_typeof: i32 [storage=static] [linkage=external];
// ISO-NEXT:     global %[[VALUE_identifier_bool:[0-9]+]] identifier_bool: i32 [storage=static] [linkage=external];
// ISO-NEXT:     global %[[VALUE_identifier_int64:[0-9]+]] identifier_int64: i32 [storage=static] [linkage=external];
// ISO-NEXT: }
// SLATE-FILECHECK-END ISO
// SLATE-FILECHECK-BEGIN C23
// C23: module {
// C23-NEXT:     target "x86_64-unknown-linux-gnu" {
// C23-NEXT:         endian = little;
// C23-NEXT:         pointer [size=8, align=8];
// C23-NEXT:         stack_alignment = 16;
// C23-NEXT:         long_double = f80;
// C23-NEXT:         storage bool [size=1, align=1];
// C23-NEXT:         storage i8, u8 [size=1, align=1];
// C23-NEXT:         storage i16, u16 [size=2, align=2];
// C23-NEXT:         storage i32, u32 [size=4, align=4];
// C23-NEXT:         storage i64, u64 [size=8, align=8];
// C23-NEXT:         storage i128, u128 [size=16, align=16];
// C23-NEXT:         storage bf16 [size=2, align=2];
// C23-NEXT:         storage f16 [size=2, align=2];
// C23-NEXT:         storage f32 [size=4, align=4];
// C23-NEXT:         storage f64 [size=8, align=8];
// C23-NEXT:         storage f80 [size=16, align=16];
// C23-NEXT:         storage f128 [size=16, align=16];
// C23-NEXT:         storage d32 [size=4, align=4];
// C23-NEXT:         storage d64 [size=8, align=8];
// C23-NEXT:         storage d128 [size=16, align=16];
// C23-NEXT:     }
// C23-NEXT:     global %[[VALUE_warning_shadow:[0-9]+]] warning_shadow: i32 [storage=static] [linkage=external];
// C23-NEXT:     global %[[VALUE_identifier_foo:[0-9]+]] identifier_foo: i32 [storage=static] [linkage=external];
// C23-NEXT:     global %[[VALUE_identifier_file:[0-9]+]] identifier_file: i32 [storage=static] [linkage=external];
// C23-NEXT:     global %[[VALUE_identifier_asm:[0-9]+]] identifier_asm: i32 [storage=static] [linkage=external];
// C23-NEXT:     global %[[VALUE_identifier_int64:[0-9]+]] identifier_int64: i32 [storage=static] [linkage=external];
// C23-NEXT: }
// SLATE-FILECHECK-END C23
