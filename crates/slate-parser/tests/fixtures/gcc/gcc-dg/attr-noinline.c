/* { dg-do compile } */
/* { dg-options "-O2 -finline-functions -fno-ipa-icf" } */

extern int t();

static inline void __attribute__((__noinline__)) function_definition(void) {t();} /* { dg-warning "inline function \[^\n\]* given attribute 'noinline'" } */

static inline void __attribute__((__noinline__)) function_declaration_both_before(void); /* { dg-warning "inline function \[^\n\]* given attribute 'noinline'" } */

static void function_declaration_both_before(void) {t();}

static void function_declaration_both_after(void);

static inline void __attribute__((__noinline__)) function_declaration_both_after(void); /* { dg-warning "(inline function \[^\n\]* given attribute .noinline.|declared inline after its definition)" } */

static void function_declaration_both_after(void) {t();}

static void function_declaration_noinline_before(void) __attribute__((__noinline__)); /* { dg-message "note: previous declaration" } */

static inline void function_declaration_noinline_before(void) {t();} /* { dg-warning "follows declaration with attribute .noinline." } */

static inline void function_declaration_noinline_after(void) {t();} /* { dg-message "note: previous definition" } */

static void function_declaration_noinline_after(void) __attribute__((__noinline__)); /* { dg-warning "follows inline declaration" } */

static inline void function_declaration_inline_before(void); /* { dg-message "note: previous declaration" } */

static void __attribute__((__noinline__)) function_declaration_inline_before(void) {t();} /* { dg-warning "follows inline declaration" } */

static inline void function_declaration_inline_noinline_before(void); /* { dg-message "note: previous declaration" } */

static void function_declaration_inline_noinline_before(void) __attribute__((__noinline__)); /* { dg-warning "follows inline declaration" } */

static void function_declaration_inline_noinline_before(void) {t();}

static inline void function_declaration_inline_noinline_after(void);

static void function_declaration_inline_noinline_after(void) {t();} /* { dg-message "note: previous definition" } */

static void function_declaration_inline_noinline_after(void) __attribute__((__noinline__)); /* { dg-warning "follows inline declaration" } */

static void function_declaration_noinline_inline_before(void) __attribute__((__noinline__)); /* { dg-message "note: previous declaration" } */

static inline void function_declaration_noinline_inline_before(void); /* { dg-warning "follows declaration with attribute .noinline." } */

static void function_declaration_noinline_inline_before(void) {t();}

void f () {
  function_definition ();
  function_declaration_both_before ();
  function_declaration_both_after ();
  function_declaration_noinline_before ();
  function_declaration_noinline_after ();
  function_declaration_inline_before ();
  function_declaration_inline_noinline_before ();
  function_declaration_inline_noinline_after ();
  function_declaration_noinline_inline_before ();
}

/* { dg-final { scan-assembler "function_definition" } } */
/* { dg-final { scan-assembler "function_declaration_both_before" } } */
/* { dg-final { scan-assembler "function_declaration_both_after" } } */
/* { dg-final { scan-assembler "function_declaration_noinline_before" } } */
/* { dg-final { scan-assembler "function_declaration_noinline_after" } } */
/* { dg-final { scan-assembler "function_declaration_inline_before" } } */
/* { dg-final { scan-assembler "function_declaration_inline_noinline_before" } } */
/* { dg-final { scan-assembler "function_declaration_inline_noinline_after" } } */
/* { dg-final { scan-assembler "function_declaration_noinline_inline_before" } } */

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
// DEFAULT-NEXT:     fn %0 @t() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @function_definition() -> void [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @function_declaration_both_before() -> void [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @function_declaration_both_after() -> void [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @function_declaration_noinline_before() -> void [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @function_declaration_noinline_after() -> void [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @function_declaration_inline_before() -> void [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @function_declaration_inline_noinline_before() -> void [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @function_declaration_inline_noinline_after() -> void [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @function_declaration_noinline_inline_before() -> void [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @f() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%7);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
