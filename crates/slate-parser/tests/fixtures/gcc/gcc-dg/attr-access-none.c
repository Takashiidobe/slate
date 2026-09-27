/* Test to verify the handling of attribute access (none).
   { dg-do compile }
   { dg-options "-O -Wall -ftrack-macro-expansion=0" } */

int __attribute__ ((access (none, 1)))
fnone_pv1 (void*);

void nowarn_fnone_pv1 (void)
{
  int x;
  fnone_pv1 (&x);
}


int __attribute__ ((access (none, 1)))
fnone_pcv1 (const void*);

void nowarn_fnone_pcv1 (void)
{
  char a[2];
  fnone_pcv1 (a);
}


int __attribute__ ((access (none, 1, 2)))
fnone_pcv1_2 (const void*, int);  // { dg-message "in a call to function 'fnone_pcv1_2' declared with attribute 'access \\\(none, 1, 2\\\)'" "note" }

void nowarn_fnone_pcv1_2 (void)
{
  char a[2];
  fnone_pcv1_2 (a, 2);
}

void warn_fnone_pcv1_2 (void)
{
  char a[3];
  fnone_pcv1_2 (a, 4);        // { dg-warning "expecting 4 bytes in a region of size 3" }
}

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
// DEFAULT-NEXT:     fn %0 @fnone_pv1(%11 <unnamed>: ptr<void>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @nowarn_fnone_pv1() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %2 x: i32 [storage=automatic];
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<void>) -> i32>(%0, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i32>>(%2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @fnone_pcv1(%12 <unnamed>: ptr<const void>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %4 @nowarn_fnone_pcv1() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %5 a: array<i8, 2> [storage=automatic];
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const void>) -> i32>(%3, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%5)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @fnone_pcv1_2(%13 <unnamed>: ptr<const void>, %14 <unnamed>: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %7 @nowarn_fnone_pcv1_2() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %8 a: array<i8, 2> [storage=automatic];
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const void>, i32) -> i32>(%6, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%8)), const<i32>(2));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @warn_fnone_pcv1_2() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %10 a: array<i8, 3> [storage=automatic];
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const void>, i32) -> i32>(%6, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%10)), const<i32>(4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
