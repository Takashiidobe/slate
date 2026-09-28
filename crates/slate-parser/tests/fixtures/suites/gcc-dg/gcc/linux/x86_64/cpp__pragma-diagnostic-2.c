/*
  { dg-options "-Wuninitialized -ftrack-macro-expansion=2" }
  { dg-do compile }
*/

void f (unsigned);

#define CODE_WITH_WARNING \
  int a; /* { dg-message "was declared here" } */	 \
  f (a)	 /* { dg-error "used uninitialized" } */

#pragma GCC diagnostic ignored "-Wuninitialized"

void
g (void)
{
  /* No warning expected here since the #pragma is in effect.  */
  CODE_WITH_WARNING;
}

#pragma GCC diagnostic error "-Wuninitialized"

void
h (void)
{
  CODE_WITH_WARNING; /* { dg-message "in expansion of macro 'CODE_WITH_WARNING'" } */
}

/* { dg-regexp {.*some warnings being treated as errors} } */

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
// DEFAULT-NEXT:     fn %0 @f(%5 <unnamed>: u32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @g() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %2 a: i32 [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%0, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @h() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %4 a: i32 [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%0, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%4)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
