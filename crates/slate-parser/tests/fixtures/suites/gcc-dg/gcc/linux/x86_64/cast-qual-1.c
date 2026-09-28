/* Incorrect `cast discards `const'' warnings.  There should be warnings
   in bad_cast and bad_assign; bad_assign gets the correct warning, but
   good_cast may get the warning instead of bad_cast.
   gcc 2.7.2.3 passes, egcs-1.1.2 and egcs-ss-19990428 fail.
   http://gcc.gnu.org/ml/gcc-bugs/1998-08/msg00645.html */
/* { dg-do compile } */
/* { dg-options "-Wcast-qual" } */
void
good_cast(const void *bar)
{
  (char *const *)bar; /* { dg-bogus "cast discards" "discarding `const' warning" } */
}

void
bad_cast(const void *bar)
{
  (const char **)bar; /* { dg-warning "cast discards" "discarding `const' warning" } */
}

void
good_assign(const void *bar)
{
  char *const *foo = bar; /* { dg-bogus "initialization discards" "discarding `const' warning" } */
}

void
bad_assign(const void *bar)
{
  const char **foo = bar; /* { dg-warning "initialization discards" "discarding `const' warning" } */
}

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
// DEFAULT-NEXT:     fn %0 @good_cast(%1 bar: ptr<const void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         pointer_cast<ptr<const ptr<i8>>, reason=explicit>(read<ptr<const void>>(%1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @bad_cast(%3 bar: ptr<const void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         pointer_cast<ptr<ptr<const i8>>, reason=explicit>(read<ptr<const void>>(%3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @good_assign(%5 bar: ptr<const void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %6 foo: ptr<const ptr<i8>> [storage=automatic] = pointer_cast<ptr<const ptr<i8>>, reason=assign>(read<ptr<const void>>(%5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @bad_assign(%8 bar: ptr<const void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %9 foo: ptr<ptr<const i8>> [storage=automatic] = pointer_cast<ptr<ptr<const i8>>, reason=assign>(read<ptr<const void>>(%8));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
