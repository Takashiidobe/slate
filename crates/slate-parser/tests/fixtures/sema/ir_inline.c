// SLATE-FILECHECK-DEFINES GNU
// SLATE-FILECHECK-STD GNU gnu89
// SLATE-FILECHECK-DEFINES ISO
// SLATE-FILECHECK-STD ISO c99
// SLATE-FILECHECK-ARGS --dump-ir

inline int plain(void) { return 1; }
extern inline int external(void) { return 2; }
static inline int local(void) { return 3; }
extern int before(void);
inline int before(void) { return 4; }
inline int after(void) { return 5; }
extern int after(void);
extern inline __attribute__((gnu_inline)) int pinned(void) { return 6; }
inline __attribute__((always_inline)) int always(void) { return 7; }
__attribute__((noinline)) int never(void) { return 8; }

#ifdef __GNUC_GNU_INLINE__
int macro_mode(void) { return 1; }
#else
int macro_mode(void) { return 2; }
#endif

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
// GNU-NEXT:     fn %0 @plain() -> i32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// GNU-NEXT:         return const<i32>(1);
// GNU-NEXT:     }
// GNU-NEXT:     fn %1 @external() -> i32 [linkage=external] [inline=hint] [definition=inline_only] [fallthrough=ub_if_used] {
// GNU-NEXT:         return const<i32>(2);
// GNU-NEXT:     }
// GNU-NEXT:     fn %2 @local() -> i32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// GNU-NEXT:         return const<i32>(3);
// GNU-NEXT:     }
// GNU-NEXT:     fn %3 @before() -> i32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// GNU-NEXT:         return const<i32>(4);
// GNU-NEXT:     }
// GNU-NEXT:     fn %4 @after() -> i32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// GNU-NEXT:         return const<i32>(5);
// GNU-NEXT:     }
// GNU-NEXT:     fn %5 @pinned() -> i32 [linkage=external] [inline=hint] [definition=inline_only] [fallthrough=ub_if_used] {
// GNU-NEXT:         return const<i32>(6);
// GNU-NEXT:     }
// GNU-NEXT:     fn %6 @always() -> i32 [linkage=external] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// GNU-NEXT:         return const<i32>(7);
// GNU-NEXT:     }
// GNU-NEXT:     fn %7 @never() -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// GNU-NEXT:         return const<i32>(8);
// GNU-NEXT:     }
// GNU-NEXT:     fn %8 @macro_mode() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// GNU-NEXT:         return const<i32>(1);
// GNU-NEXT:     }
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
// ISO-NEXT:     fn %0 @plain() -> i32 [linkage=external] [inline=hint] [definition=inline_only] [fallthrough=ub_if_used] {
// ISO-NEXT:         return const<i32>(1);
// ISO-NEXT:     }
// ISO-NEXT:     fn %1 @external() -> i32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// ISO-NEXT:         return const<i32>(2);
// ISO-NEXT:     }
// ISO-NEXT:     fn %2 @local() -> i32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// ISO-NEXT:         return const<i32>(3);
// ISO-NEXT:     }
// ISO-NEXT:     fn %3 @before() -> i32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// ISO-NEXT:         return const<i32>(4);
// ISO-NEXT:     }
// ISO-NEXT:     fn %4 @after() -> i32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// ISO-NEXT:         return const<i32>(5);
// ISO-NEXT:     }
// ISO-NEXT:     fn %5 @pinned() -> i32 [linkage=external] [inline=hint] [definition=inline_only] [fallthrough=ub_if_used] {
// ISO-NEXT:         return const<i32>(6);
// ISO-NEXT:     }
// ISO-NEXT:     fn %6 @always() -> i32 [linkage=external] [inline=always] [definition=inline_only] [fallthrough=ub_if_used] {
// ISO-NEXT:         return const<i32>(7);
// ISO-NEXT:     }
// ISO-NEXT:     fn %7 @never() -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// ISO-NEXT:         return const<i32>(8);
// ISO-NEXT:     }
// ISO-NEXT:     fn %8 @macro_mode() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// ISO-NEXT:         return const<i32>(2);
// ISO-NEXT:     }
// ISO-NEXT: }
// SLATE-FILECHECK-END ISO
