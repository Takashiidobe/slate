#ifdef INLINE_IS_KEYWORD
inline int inline_function(void) { return 0; }
#else
int inline;
#endif
__inline int always_inline_function(void) { return 1; }

// SLATE-FILECHECK-DEFINES C89
// SLATE-FILECHECK-STD C89 c89
// SLATE-FILECHECK-DEFINES GNU89 INLINE_IS_KEYWORD
// SLATE-FILECHECK-STD GNU89 gnu89
// SLATE-FILECHECK-DEFINES C99 INLINE_IS_KEYWORD
// SLATE-FILECHECK-STD C99 c99
// SLATE-FILECHECK-DEFINES C23 INLINE_IS_KEYWORD
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
// C89-NEXT:     global %0 inline: i32 [storage=static] [linkage=external];
// C89-NEXT:     fn %1 @always_inline_function() -> i32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// C89-NEXT:         return const<i32>(1);
// C89-NEXT:     }
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
// GNU89-NEXT:     fn %0 @inline_function() -> i32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// GNU89-NEXT:         return const<i32>(0);
// GNU89-NEXT:     }
// GNU89-NEXT:     fn %1 @always_inline_function() -> i32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// GNU89-NEXT:         return const<i32>(1);
// GNU89-NEXT:     }
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
// C99-NEXT:     fn %0 @inline_function() -> i32 [linkage=external] [inline=hint] [definition=inline_only] [fallthrough=ub_if_used] {
// C99-NEXT:         return const<i32>(0);
// C99-NEXT:     }
// C99-NEXT:     fn %1 @always_inline_function() -> i32 [linkage=external] [inline=hint] [definition=inline_only] [fallthrough=ub_if_used] {
// C99-NEXT:         return const<i32>(1);
// C99-NEXT:     }
// C99-NEXT: }
// SLATE-FILECHECK-END C99
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
// C23-NEXT:     fn %0 @inline_function() -> i32 [linkage=external] [inline=hint] [definition=inline_only] [fallthrough=ub_if_used] {
// C23-NEXT:         return const<i32>(0);
// C23-NEXT:     }
// C23-NEXT:     fn %1 @always_inline_function() -> i32 [linkage=external] [inline=hint] [definition=inline_only] [fallthrough=ub_if_used] {
// C23-NEXT:         return const<i32>(1);
// C23-NEXT:     }
// C23-NEXT: }
// SLATE-FILECHECK-END C23
