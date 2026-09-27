/* PR middle-end/81824 - Warn for missing attributes with function aliases
   Verify that attributes always_inline, gnu_inline, and noinline aren't
   copied.  Also verify that copying attribute tls_model to a non-thread
   variable triggers a warning.
   { dg-do compile }
   { dg-require-alias "" }
   { dg-options "-Wall" }
   { dg-require-effective-target tls } */

#define ATTR(...)   __attribute__ ((__VA_ARGS__))

ATTR (always_inline, gnu_inline, noreturn) inline int
finline_noret (void)
{
  __builtin_abort ();
  /* Expect no -Wreturn-type.  */
}

int call_finline_noret (void)
{
  finline_noret ();
  /* Expect no -Wreturn-type.  */
}


ATTR (copy (finline_noret)) int
fnoret (void);

int call_fnoret (void)
{
  fnoret ();
  /* Expect no -Wreturn-type.  */
}


/* Verify that attribute always_inline on an alias target doesn't
   get copied and interfere with attribute noinline on the alias
   (trigger a warning due to a conflict).  */

ATTR (always_inline) static inline int
finline (void) { return 0; }

ATTR (alias ("finline"), noinline) int
fnoinline (void);

ATTR (copy (finline)) int
fnoinline (void);


ATTR (tls_model ("global-dynamic")) __thread int
  tls_target;

ATTR (alias ("tls_target"), copy (tls_target)) extern __thread int
  thread_alias;


ATTR (alias ("tls_target"), copy (tls_target)) extern int
  alias;            /* { dg-warning ".tls_model. attribute ignored because .alias. does not have thread storage duration" } */

// SLATE-FILECHECK-FLAVOR gcc
// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     global %6 tls_target: i32 [storage=thread] [linkage=external] [tls_model=global-dynamic];
// DEFAULT-NEXT:     global %7 thread_alias: i32 [storage=thread] [linkage=external] [alias="tls_target"];
// DEFAULT-NEXT:     global %8 alias: i32 [storage=static] [linkage=external] [alias="tls_target"];
// DEFAULT-NEXT:     fn %9 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %0 @finline_noret() -> i32 [linkage=external] [inline=always] [definition=emitted] [noreturn] [fallthrough=ub] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1 @call_finline_noret() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @fnoret() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %3 @call_fnoret() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @finline() -> i32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @fnoinline() -> i32 [linkage=external] [alias="finline"] [inline=never];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
