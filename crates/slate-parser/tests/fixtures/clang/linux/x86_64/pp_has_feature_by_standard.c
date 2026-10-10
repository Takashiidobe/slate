// SLATE-FILECHECK-DEFINES C99
// SLATE-FILECHECK-STD C99 c99
// SLATE-FILECHECK-DEFINES C11
// SLATE-FILECHECK-STD C11 c11
// SLATE-FILECHECK-DEFINES C23
// SLATE-FILECHECK-STD C23 c23

#if __has_feature(c_alignas)
int feature_c_alignas;
#endif
#if __has_extension(c_alignas)
int extension_c_alignas;
#endif
#if __has_feature(__c_static_assert__)
int feature_c_static_assert;
#endif
#if __has_extension(__c_static_assert__)
int extension_c_static_assert;
#endif
#if __has_feature(c_thread_local)
int feature_c_thread_local;
#endif
#if __has_extension(c_thread_local)
int extension_c_thread_local;
#endif
#if __has_feature(objc_c_static_assert)
int feature_objc_c_static_assert;
#endif
#if __has_extension(objc_c_static_assert)
int extension_objc_c_static_assert;
#endif
#if __has_feature(c_fixed_enum)
int feature_c_fixed_enum;
#endif
#if __has_extension(c_fixed_enum)
int extension_c_fixed_enum;
#endif
#if __has_feature(cxx_binary_literals)
int feature_cxx_binary_literals;
#endif
#if __has_extension(cxx_binary_literals)
int extension_cxx_binary_literals;
#endif
#if __has_feature(tls)
int feature_tls;
#endif
#if __has_extension(tls)
int extension_tls;
#endif
#if __has_feature(enumerator_attributes)
int feature_enumerator_attributes;
#endif
#if __has_extension(enumerator_attributes)
int extension_enumerator_attributes;
#endif
#if __has_feature(nullability)
int feature_nullability;
#endif
#if __has_extension(nullability)
int extension_nullability;
#endif
#if __has_feature(c_attributes)
int feature_c_attributes;
#endif
#if __has_extension(c_attributes)
int extension_c_attributes;
#endif
#if __has_feature(gnu_asm_goto_with_outputs)
int feature_gnu_asm_goto_with_outputs;
#endif
#if __has_extension(gnu_asm_goto_with_outputs)
int extension_gnu_asm_goto_with_outputs;
#endif
#if __has_feature(attribute_availability)
int feature_attribute_availability;
#endif
#if __has_extension(attribute_availability)
int extension_attribute_availability;
#endif

// SLATE-FILECHECK-BEGIN C99
// C99: module {
// C99-NEXT:     target "x86_64-unknown-linux-gnu" {
// C99-NEXT:         endian = little;
// C99-NEXT:         pointer [size=8, align=8];
// C99-NEXT:         stack_alignment = 16;
// C99-NEXT:         long_double = f80;
// C99-NEXT:         storage bool [size=1, align=1];
// C99-NEXT:         storage i8, u8 [size=1, align=1];
// C99-NEXT:         storage i16, u16 [size=2, align=2];
// C99-NEXT:         storage i32, u32 [size=4, align=4];
// C99-NEXT:         storage i64, u64 [size=8, align=8];
// C99-NEXT:         storage i128, u128 [size=16, align=16];
// C99-NEXT:         storage bf16 [size=2, align=2];
// C99-NEXT:         storage f16 [size=2, align=2];
// C99-NEXT:         storage f32 [size=4, align=4];
// C99-NEXT:         storage f64 [size=8, align=8];
// C99-NEXT:         storage f80 [size=16, align=16];
// C99-NEXT:         storage f128 [size=16, align=16];
// C99-NEXT:         storage d32 [size=4, align=4];
// C99-NEXT:         storage d64 [size=8, align=8];
// C99-NEXT:         storage d128 [size=16, align=16];
// C99-NEXT:     }
// C99-NEXT:     global %[[VALUE_extension_c_alignas:[0-9]+]] extension_c_alignas: i32 [storage=static] [linkage=external];
// C99-NEXT:     global %[[VALUE_extension_c_static_assert:[0-9]+]] extension_c_static_assert: i32 [storage=static] [linkage=external];
// C99-NEXT:     global %[[VALUE_extension_c_thread_local:[0-9]+]] extension_c_thread_local: i32 [storage=static] [linkage=external];
// C99-NEXT:     global %[[VALUE_extension_objc_c_static_assert:[0-9]+]] extension_objc_c_static_assert: i32 [storage=static] [linkage=external];
// C99-NEXT:     global %[[VALUE_extension_c_fixed_enum:[0-9]+]] extension_c_fixed_enum: i32 [storage=static] [linkage=external];
// C99-NEXT:     global %[[VALUE_extension_cxx_binary_literals:[0-9]+]] extension_cxx_binary_literals: i32 [storage=static] [linkage=external];
// C99-NEXT:     global %[[VALUE_feature_tls:[0-9]+]] feature_tls: i32 [storage=static] [linkage=external];
// C99-NEXT:     global %[[VALUE_extension_tls:[0-9]+]] extension_tls: i32 [storage=static] [linkage=external];
// C99-NEXT:     global %[[VALUE_feature_enumerator_attributes:[0-9]+]] feature_enumerator_attributes: i32 [storage=static] [linkage=external];
// C99-NEXT:     global %[[VALUE_extension_enumerator_attributes:[0-9]+]] extension_enumerator_attributes: i32 [storage=static] [linkage=external];
// C99-NEXT:     global %[[VALUE_feature_nullability:[0-9]+]] feature_nullability: i32 [storage=static] [linkage=external];
// C99-NEXT:     global %[[VALUE_extension_nullability:[0-9]+]] extension_nullability: i32 [storage=static] [linkage=external];
// C99-NEXT:     global %[[VALUE_extension_c_attributes:[0-9]+]] extension_c_attributes: i32 [storage=static] [linkage=external];
// C99-NEXT:     global %[[VALUE_extension_gnu_asm_goto_with_outputs:[0-9]+]] extension_gnu_asm_goto_with_outputs: i32 [storage=static] [linkage=external];
// C99-NEXT:     global %[[VALUE_feature_attribute_availability:[0-9]+]] feature_attribute_availability: i32 [storage=static] [linkage=external];
// C99-NEXT:     global %[[VALUE_extension_attribute_availability:[0-9]+]] extension_attribute_availability: i32 [storage=static] [linkage=external];
// C99-NEXT: }
// SLATE-FILECHECK-END C99
// SLATE-FILECHECK-BEGIN C11
// C11: module {
// C11-NEXT:     target "x86_64-unknown-linux-gnu" {
// C11-NEXT:         endian = little;
// C11-NEXT:         pointer [size=8, align=8];
// C11-NEXT:         stack_alignment = 16;
// C11-NEXT:         long_double = f80;
// C11-NEXT:         storage bool [size=1, align=1];
// C11-NEXT:         storage i8, u8 [size=1, align=1];
// C11-NEXT:         storage i16, u16 [size=2, align=2];
// C11-NEXT:         storage i32, u32 [size=4, align=4];
// C11-NEXT:         storage i64, u64 [size=8, align=8];
// C11-NEXT:         storage i128, u128 [size=16, align=16];
// C11-NEXT:         storage bf16 [size=2, align=2];
// C11-NEXT:         storage f16 [size=2, align=2];
// C11-NEXT:         storage f32 [size=4, align=4];
// C11-NEXT:         storage f64 [size=8, align=8];
// C11-NEXT:         storage f80 [size=16, align=16];
// C11-NEXT:         storage f128 [size=16, align=16];
// C11-NEXT:         storage d32 [size=4, align=4];
// C11-NEXT:         storage d64 [size=8, align=8];
// C11-NEXT:         storage d128 [size=16, align=16];
// C11-NEXT:     }
// C11-NEXT:     global %[[VALUE_feature_c_alignas:[0-9]+]] feature_c_alignas: i32 [storage=static] [linkage=external];
// C11-NEXT:     global %[[VALUE_extension_c_alignas:[0-9]+]] extension_c_alignas: i32 [storage=static] [linkage=external];
// C11-NEXT:     global %[[VALUE_feature_c_static_assert:[0-9]+]] feature_c_static_assert: i32 [storage=static] [linkage=external];
// C11-NEXT:     global %[[VALUE_extension_c_static_assert:[0-9]+]] extension_c_static_assert: i32 [storage=static] [linkage=external];
// C11-NEXT:     global %[[VALUE_feature_c_thread_local:[0-9]+]] feature_c_thread_local: i32 [storage=static] [linkage=external];
// C11-NEXT:     global %[[VALUE_extension_c_thread_local:[0-9]+]] extension_c_thread_local: i32 [storage=static] [linkage=external];
// C11-NEXT:     global %[[VALUE_feature_objc_c_static_assert:[0-9]+]] feature_objc_c_static_assert: i32 [storage=static] [linkage=external];
// C11-NEXT:     global %[[VALUE_extension_objc_c_static_assert:[0-9]+]] extension_objc_c_static_assert: i32 [storage=static] [linkage=external];
// C11-NEXT:     global %[[VALUE_extension_c_fixed_enum:[0-9]+]] extension_c_fixed_enum: i32 [storage=static] [linkage=external];
// C11-NEXT:     global %[[VALUE_extension_cxx_binary_literals:[0-9]+]] extension_cxx_binary_literals: i32 [storage=static] [linkage=external];
// C11-NEXT:     global %[[VALUE_feature_tls:[0-9]+]] feature_tls: i32 [storage=static] [linkage=external];
// C11-NEXT:     global %[[VALUE_extension_tls:[0-9]+]] extension_tls: i32 [storage=static] [linkage=external];
// C11-NEXT:     global %[[VALUE_feature_enumerator_attributes:[0-9]+]] feature_enumerator_attributes: i32 [storage=static] [linkage=external];
// C11-NEXT:     global %[[VALUE_extension_enumerator_attributes:[0-9]+]] extension_enumerator_attributes: i32 [storage=static] [linkage=external];
// C11-NEXT:     global %[[VALUE_feature_nullability:[0-9]+]] feature_nullability: i32 [storage=static] [linkage=external];
// C11-NEXT:     global %[[VALUE_extension_nullability:[0-9]+]] extension_nullability: i32 [storage=static] [linkage=external];
// C11-NEXT:     global %[[VALUE_extension_c_attributes:[0-9]+]] extension_c_attributes: i32 [storage=static] [linkage=external];
// C11-NEXT:     global %[[VALUE_extension_gnu_asm_goto_with_outputs:[0-9]+]] extension_gnu_asm_goto_with_outputs: i32 [storage=static] [linkage=external];
// C11-NEXT:     global %[[VALUE_feature_attribute_availability:[0-9]+]] feature_attribute_availability: i32 [storage=static] [linkage=external];
// C11-NEXT:     global %[[VALUE_extension_attribute_availability:[0-9]+]] extension_attribute_availability: i32 [storage=static] [linkage=external];
// C11-NEXT: }
// SLATE-FILECHECK-END C11
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
// C23-NEXT:     global %[[VALUE_feature_c_alignas:[0-9]+]] feature_c_alignas: i32 [storage=static] [linkage=external];
// C23-NEXT:     global %[[VALUE_extension_c_alignas:[0-9]+]] extension_c_alignas: i32 [storage=static] [linkage=external];
// C23-NEXT:     global %[[VALUE_feature_c_static_assert:[0-9]+]] feature_c_static_assert: i32 [storage=static] [linkage=external];
// C23-NEXT:     global %[[VALUE_extension_c_static_assert:[0-9]+]] extension_c_static_assert: i32 [storage=static] [linkage=external];
// C23-NEXT:     global %[[VALUE_feature_c_thread_local:[0-9]+]] feature_c_thread_local: i32 [storage=static] [linkage=external];
// C23-NEXT:     global %[[VALUE_extension_c_thread_local:[0-9]+]] extension_c_thread_local: i32 [storage=static] [linkage=external];
// C23-NEXT:     global %[[VALUE_feature_objc_c_static_assert:[0-9]+]] feature_objc_c_static_assert: i32 [storage=static] [linkage=external];
// C23-NEXT:     global %[[VALUE_extension_objc_c_static_assert:[0-9]+]] extension_objc_c_static_assert: i32 [storage=static] [linkage=external];
// C23-NEXT:     global %[[VALUE_feature_c_fixed_enum:[0-9]+]] feature_c_fixed_enum: i32 [storage=static] [linkage=external];
// C23-NEXT:     global %[[VALUE_extension_c_fixed_enum:[0-9]+]] extension_c_fixed_enum: i32 [storage=static] [linkage=external];
// C23-NEXT:     global %[[VALUE_extension_cxx_binary_literals:[0-9]+]] extension_cxx_binary_literals: i32 [storage=static] [linkage=external];
// C23-NEXT:     global %[[VALUE_feature_tls:[0-9]+]] feature_tls: i32 [storage=static] [linkage=external];
// C23-NEXT:     global %[[VALUE_extension_tls:[0-9]+]] extension_tls: i32 [storage=static] [linkage=external];
// C23-NEXT:     global %[[VALUE_feature_enumerator_attributes:[0-9]+]] feature_enumerator_attributes: i32 [storage=static] [linkage=external];
// C23-NEXT:     global %[[VALUE_extension_enumerator_attributes:[0-9]+]] extension_enumerator_attributes: i32 [storage=static] [linkage=external];
// C23-NEXT:     global %[[VALUE_feature_nullability:[0-9]+]] feature_nullability: i32 [storage=static] [linkage=external];
// C23-NEXT:     global %[[VALUE_extension_nullability:[0-9]+]] extension_nullability: i32 [storage=static] [linkage=external];
// C23-NEXT:     global %[[VALUE_extension_c_attributes:[0-9]+]] extension_c_attributes: i32 [storage=static] [linkage=external];
// C23-NEXT:     global %[[VALUE_extension_gnu_asm_goto_with_outputs:[0-9]+]] extension_gnu_asm_goto_with_outputs: i32 [storage=static] [linkage=external];
// C23-NEXT:     global %[[VALUE_feature_attribute_availability:[0-9]+]] feature_attribute_availability: i32 [storage=static] [linkage=external];
// C23-NEXT:     global %[[VALUE_extension_attribute_availability:[0-9]+]] extension_attribute_availability: i32 [storage=static] [linkage=external];
// C23-NEXT: }
// SLATE-FILECHECK-END C23
