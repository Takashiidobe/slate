/* Test for C99 forms of array declarator.  Test restrict qualifiers
   properly applied to type of parameter.  */
/* Origin: Joseph Myers <joseph@codesourcery.com> */
/* { dg-do compile } */
/* { dg-options "-std=iso9899:1999 -pedantic-errors" } */

void
f0 (int a[restrict])
{
  int **b = &a; /* { dg-error "discards 'restrict' qualifier" } */
  int *restrict *c = &a;
}

void
f1 (a)
     int a[restrict];
{
  int **b = &a; /* { dg-error "discards 'restrict' qualifier" } */
  int *restrict *c = &a;
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
// DEFAULT-NEXT:     fn %0 @f0(%1 a: ptr<i32> [restrict]) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %2 b: ptr<ptr<i32>> [storage=automatic] = pointer_cast<ptr<ptr<i32>>, reason=assign>(addr_of<ptr<ptr<i32>>>(%1));
// DEFAULT-NEXT:         let %3 c: ptr<ptr<i32>> [storage=automatic] = addr_of<ptr<ptr<i32>>>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @f1(%5 a: ptr<i32> [restrict]) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %6 b: ptr<ptr<i32>> [storage=automatic] = pointer_cast<ptr<ptr<i32>>, reason=assign>(addr_of<ptr<ptr<i32>>>(%5));
// DEFAULT-NEXT:         let %7 c: ptr<ptr<i32>> [storage=automatic] = addr_of<ptr<ptr<i32>>>(%5);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
