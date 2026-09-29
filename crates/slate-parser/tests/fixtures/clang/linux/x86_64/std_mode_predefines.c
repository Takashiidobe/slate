#ifdef __STRICT_ANSI__
int strict_ansi;
#endif
#ifdef linux
int gnu_namespace_linux;
#endif
#ifdef unix
int gnu_namespace_unix;
#endif
#ifdef __GNUC_STDC_INLINE__
int stdc_inline_semantics;
#endif
#ifdef __GNUC_GNU_INLINE__
int gnu_inline_semantics;
#endif
#ifdef __CHAR8_TYPE__
__CHAR8_TYPE__ char8_unit;
#endif
#if defined(__GCC_ATOMIC_CHAR8_T_LOCK_FREE) && __GCC_ATOMIC_CHAR8_T_LOCK_FREE == 2
int char8_lock_free;
#endif
#ifdef __UINT64_FMTb__
const char *uint64_binary_format = __UINT64_FMTb__;
#endif

// SLATE-FILECHECK-DEFINES C89
// SLATE-FILECHECK-STD C89 c89
// SLATE-FILECHECK-DEFINES GNU89
// SLATE-FILECHECK-STD GNU89 gnu89
// SLATE-FILECHECK-DEFINES C99
// SLATE-FILECHECK-STD C99 c99
// SLATE-FILECHECK-DEFINES C17
// SLATE-FILECHECK-STD C17 c17
// SLATE-FILECHECK-DEFINES GNU17
// SLATE-FILECHECK-STD GNU17 gnu17
// SLATE-FILECHECK-DEFINES C23
// SLATE-FILECHECK-STD C23 c23

// SLATE-FILECHECK-BEGIN C89
// C89: module {
// C89-NEXT:     target "x86_64-unknown-linux-gnu" {
// C89-NEXT:         endian = little;
// C89-NEXT:         pointer [size=8, align=8];
// C89-NEXT:         stack_alignment = 16;
// C89-NEXT:         long_double = f80;
// C89-NEXT:         storage bool [size=1, align=1];
// C89-NEXT:         storage i8, u8 [size=1, align=1];
// C89-NEXT:         storage i16, u16 [size=2, align=2];
// C89-NEXT:         storage i32, u32 [size=4, align=4];
// C89-NEXT:         storage i64, u64 [size=8, align=8];
// C89-NEXT:         storage i128, u128 [size=16, align=16];
// C89-NEXT:         storage bf16 [size=2, align=2];
// C89-NEXT:         storage f16 [size=2, align=2];
// C89-NEXT:         storage f32 [size=4, align=4];
// C89-NEXT:         storage f64 [size=8, align=8];
// C89-NEXT:         storage f80 [size=16, align=16];
// C89-NEXT:         storage f128 [size=16, align=16];
// C89-NEXT:         storage d32 [size=4, align=4];
// C89-NEXT:         storage d64 [size=8, align=8];
// C89-NEXT:         storage d128 [size=16, align=16];
// C89-NEXT:     }
// C89-NEXT:     global %[[VALUE_strict_ansi:[0-9]+]] strict_ansi: i32 [storage=static] [linkage=external];
// C89-NEXT:     global %[[VALUE_gnu_inline_semantics:[0-9]+]] gnu_inline_semantics: i32 [storage=static] [linkage=external];
// C89-NEXT: }
// SLATE-FILECHECK-END C89
// SLATE-FILECHECK-BEGIN GNU89
// GNU89: module {
// GNU89-NEXT:     target "x86_64-unknown-linux-gnu" {
// GNU89-NEXT:         endian = little;
// GNU89-NEXT:         pointer [size=8, align=8];
// GNU89-NEXT:         stack_alignment = 16;
// GNU89-NEXT:         long_double = f80;
// GNU89-NEXT:         storage bool [size=1, align=1];
// GNU89-NEXT:         storage i8, u8 [size=1, align=1];
// GNU89-NEXT:         storage i16, u16 [size=2, align=2];
// GNU89-NEXT:         storage i32, u32 [size=4, align=4];
// GNU89-NEXT:         storage i64, u64 [size=8, align=8];
// GNU89-NEXT:         storage i128, u128 [size=16, align=16];
// GNU89-NEXT:         storage bf16 [size=2, align=2];
// GNU89-NEXT:         storage f16 [size=2, align=2];
// GNU89-NEXT:         storage f32 [size=4, align=4];
// GNU89-NEXT:         storage f64 [size=8, align=8];
// GNU89-NEXT:         storage f80 [size=16, align=16];
// GNU89-NEXT:         storage f128 [size=16, align=16];
// GNU89-NEXT:         storage d32 [size=4, align=4];
// GNU89-NEXT:         storage d64 [size=8, align=8];
// GNU89-NEXT:         storage d128 [size=16, align=16];
// GNU89-NEXT:     }
// GNU89-NEXT:     global %[[VALUE_gnu_namespace_linux:[0-9]+]] gnu_namespace_linux: i32 [storage=static] [linkage=external];
// GNU89-NEXT:     global %[[VALUE_gnu_namespace_unix:[0-9]+]] gnu_namespace_unix: i32 [storage=static] [linkage=external];
// GNU89-NEXT:     global %[[VALUE_gnu_inline_semantics:[0-9]+]] gnu_inline_semantics: i32 [storage=static] [linkage=external];
// GNU89-NEXT: }
// SLATE-FILECHECK-END GNU89
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
// C99-NEXT:     global %[[VALUE_strict_ansi:[0-9]+]] strict_ansi: i32 [storage=static] [linkage=external];
// C99-NEXT:     global %[[VALUE_stdc_inline_semantics:[0-9]+]] stdc_inline_semantics: i32 [storage=static] [linkage=external];
// C99-NEXT: }
// SLATE-FILECHECK-END C99
// SLATE-FILECHECK-BEGIN C17
// C17: module {
// C17-NEXT:     target "x86_64-unknown-linux-gnu" {
// C17-NEXT:         endian = little;
// C17-NEXT:         pointer [size=8, align=8];
// C17-NEXT:         stack_alignment = 16;
// C17-NEXT:         long_double = f80;
// C17-NEXT:         storage bool [size=1, align=1];
// C17-NEXT:         storage i8, u8 [size=1, align=1];
// C17-NEXT:         storage i16, u16 [size=2, align=2];
// C17-NEXT:         storage i32, u32 [size=4, align=4];
// C17-NEXT:         storage i64, u64 [size=8, align=8];
// C17-NEXT:         storage i128, u128 [size=16, align=16];
// C17-NEXT:         storage bf16 [size=2, align=2];
// C17-NEXT:         storage f16 [size=2, align=2];
// C17-NEXT:         storage f32 [size=4, align=4];
// C17-NEXT:         storage f64 [size=8, align=8];
// C17-NEXT:         storage f80 [size=16, align=16];
// C17-NEXT:         storage f128 [size=16, align=16];
// C17-NEXT:         storage d32 [size=4, align=4];
// C17-NEXT:         storage d64 [size=8, align=8];
// C17-NEXT:         storage d128 [size=16, align=16];
// C17-NEXT:     }
// C17-NEXT:     global %[[VALUE_strict_ansi:[0-9]+]] strict_ansi: i32 [storage=static] [linkage=external];
// C17-NEXT:     global %[[VALUE_stdc_inline_semantics:[0-9]+]] stdc_inline_semantics: i32 [storage=static] [linkage=external];
// C17-NEXT: }
// SLATE-FILECHECK-END C17
// SLATE-FILECHECK-BEGIN GNU17
// GNU17: module {
// GNU17-NEXT:     target "x86_64-unknown-linux-gnu" {
// GNU17-NEXT:         endian = little;
// GNU17-NEXT:         pointer [size=8, align=8];
// GNU17-NEXT:         stack_alignment = 16;
// GNU17-NEXT:         long_double = f80;
// GNU17-NEXT:         storage bool [size=1, align=1];
// GNU17-NEXT:         storage i8, u8 [size=1, align=1];
// GNU17-NEXT:         storage i16, u16 [size=2, align=2];
// GNU17-NEXT:         storage i32, u32 [size=4, align=4];
// GNU17-NEXT:         storage i64, u64 [size=8, align=8];
// GNU17-NEXT:         storage i128, u128 [size=16, align=16];
// GNU17-NEXT:         storage bf16 [size=2, align=2];
// GNU17-NEXT:         storage f16 [size=2, align=2];
// GNU17-NEXT:         storage f32 [size=4, align=4];
// GNU17-NEXT:         storage f64 [size=8, align=8];
// GNU17-NEXT:         storage f80 [size=16, align=16];
// GNU17-NEXT:         storage f128 [size=16, align=16];
// GNU17-NEXT:         storage d32 [size=4, align=4];
// GNU17-NEXT:         storage d64 [size=8, align=8];
// GNU17-NEXT:         storage d128 [size=16, align=16];
// GNU17-NEXT:     }
// GNU17-NEXT:     global %[[VALUE_gnu_namespace_linux:[0-9]+]] gnu_namespace_linux: i32 [storage=static] [linkage=external];
// GNU17-NEXT:     global %[[VALUE_gnu_namespace_unix:[0-9]+]] gnu_namespace_unix: i32 [storage=static] [linkage=external];
// GNU17-NEXT:     global %[[VALUE_stdc_inline_semantics:[0-9]+]] stdc_inline_semantics: i32 [storage=static] [linkage=external];
// GNU17-NEXT: }
// SLATE-FILECHECK-END GNU17
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
// C23-NEXT:     global %[[VALUE_strict_ansi:[0-9]+]] strict_ansi: i32 [storage=static] [linkage=external];
// C23-NEXT:     global %[[VALUE_stdc_inline_semantics:[0-9]+]] stdc_inline_semantics: i32 [storage=static] [linkage=external];
// C23-NEXT:     global %[[VALUE_char8_unit:[0-9]+]] char8_unit: u8 [storage=static] [linkage=external];
// C23-NEXT:     global %[[VALUE_char8_lock_free:[0-9]+]] char8_lock_free: i32 [storage=static] [linkage=external];
// C23-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([108, 98, 0]) [linkage=internal];
// C23-NEXT:     global %[[VALUE_uint64_binary_format:[0-9]+]] uint64_binary_format: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str]])) [linkage=external];
// C23-NEXT: }
// SLATE-FILECHECK-END C23
