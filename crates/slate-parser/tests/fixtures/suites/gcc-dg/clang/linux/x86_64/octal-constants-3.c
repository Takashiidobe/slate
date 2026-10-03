/* Test for octal integer constants: -pedantic-errors.  */

/* Origin: Joerg Wunsch <j.gnu@uriah.heep.sax.de>.  */
/* { dg-do compile } */
/* { dg-options "-std=iso9899:1999 -pedantic-errors -ftrack-macro-expansion=0" } */

#define FOO 0o1307

int
foo (void)
{
#if FOO /* { dg-error "'0o' prefixed constants are a C2Y feature or GCC extension" } */
  return 23;
#endif
  return 0o1307; /* { dg-error "'0o' prefixed constants are a C2Y feature or GCC extension" } */
}

// SLATE-FILECHECK-STD DEFAULT iso9899:1999
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
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(23);
// DEFAULT-NEXT:         return const<i32>(711);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
