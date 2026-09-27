/* Test inline functions declared in inner scopes.  Bug 93072.  */
/* { dg-do compile } */
/* { dg-options "-fgnu89-inline" } */

void
inline_1 (void)
{
}

void
inline_2 (void)
{
}

static void
inline_static_1 (void)
{
}

static void
inline_static_2 (void)
{
}

static void
test (void)
{
  inline void inline_1 (void);
  if (inline_1 == 0) ;
  extern inline void inline_2 (void);
  if (inline_2 == 0) ;
  inline void inline_3 (void);
  if (inline_3 == 0) ;
  extern inline void inline_4 (void);
  if (inline_4 == 0) ;
  inline void inline_static_1 (void);
  if (inline_static_1 == 0) ;
  extern inline void inline_static_2 (void);
  if (inline_static_2 == 0) ;
}

void
inline_3 (void)
{
}

void
inline_4 (void)
{
}

// SLATE-FILECHECK-FLAVOR gcc
// SLATE-FILECHECK-STD DEFAULT gnu23
// SLATE-FILECHECK-ARGS -fgnu89-inline
// SLATE-FILECHECK-DEFINES DEFAULT

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
// DEFAULT-NEXT:     fn %0 @inline_1() -> void [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1 @inline_2() -> void [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @inline_static_1() -> void [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @inline_static_2() -> void [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @inline_3() -> void [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @inline_4() -> void [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @test() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<ptr<fn() -> void>>(function_decay<ptr<fn() -> void>>(%0), null<ptr<fn() -> void>>)
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         if eq<ptr<fn() -> void>>(function_decay<ptr<fn() -> void>>(%1), null<ptr<fn() -> void>>)
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         if eq<ptr<fn() -> void>>(function_decay<ptr<fn() -> void>>(%5), null<ptr<fn() -> void>>)
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         if eq<ptr<fn() -> void>>(function_decay<ptr<fn() -> void>>(%6), null<ptr<fn() -> void>>)
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         if eq<ptr<fn() -> void>>(function_decay<ptr<fn() -> void>>(%2), null<ptr<fn() -> void>>)
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         if eq<ptr<fn() -> void>>(function_decay<ptr<fn() -> void>>(%3), null<ptr<fn() -> void>>)
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
