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
// DEFAULT-NEXT:     fn %[[VALUE_t:[0-9]+]] @t() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_function_definition:[0-9]+]] @function_definition() -> void [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_t]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_function_declaration_both_before:[0-9]+]] @function_declaration_both_before() -> void [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_t]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_function_declaration_both_after:[0-9]+]] @function_declaration_both_after() -> void [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_t]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_function_declaration_noinline_before:[0-9]+]] @function_declaration_noinline_before() -> void [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_t]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_function_declaration_noinline_after:[0-9]+]] @function_declaration_noinline_after() -> void [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_t]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_function_declaration_inline_before:[0-9]+]] @function_declaration_inline_before() -> void [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_t]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_function_declaration_inline_noinline_before:[0-9]+]] @function_declaration_inline_noinline_before() -> void [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_t]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_function_declaration_inline_noinline_after:[0-9]+]] @function_declaration_inline_noinline_after() -> void [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_t]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_function_declaration_noinline_inline_before:[0-9]+]] @function_declaration_noinline_inline_before() -> void [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_t]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_function_definition]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_function_declaration_both_before]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_function_declaration_both_after]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_function_declaration_noinline_before]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_function_declaration_noinline_after]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_function_declaration_inline_before]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_function_declaration_inline_noinline_before]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_function_declaration_inline_noinline_after]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_function_declaration_noinline_inline_before]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
