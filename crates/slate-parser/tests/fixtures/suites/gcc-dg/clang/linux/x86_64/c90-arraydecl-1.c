/* Test for C99 forms of array declarator: rejected in C90.  */
/* Origin: Joseph Myers <jsm28@cam.ac.uk> */
/* { dg-do compile } */
/* { dg-options "-std=iso9899:1990 -pedantic-errors" } */

/* Use of [*] (possibly with type qualifiers) in an array declarator with
   function prototype scope is a C99 feature.  GCC does not yet implement
   it correctly, so gives a warning about this. so we can't yet test here
   that we get just one error and no warnings.  */

void foo0 (int a, int b[*]); /* { dg-error "ISO C90" "\[*\] not in C90" } */
void foo1 (int, int [*]); /* { dg-error "ISO C90" "\[*\] not in C90" } */


/* Use of static and type qualifiers (not allowed with abstract declarators)
   is a C99 feature.  */

void bar0 (int a[const]); /* { dg-bogus "warning" "warning in place of error" } */
/* { dg-error "ISO C90" "\[quals\] not in C90" { target *-*-* } .-1 } */
void bar1 (int a[const 2]); /* { dg-bogus "warning" "warning in place of error" } */
/* { dg-error "ISO C90" "\[quals expr\] not in C90" { target *-*-* } .-1 } */
void bar2 (int a[static 2]); /* { dg-bogus "warning" "warning in place of error" } */
/* { dg-error "ISO C90" "\[static expr\] not in C90" { target *-*-* } .-1 } */
void bar3 (int a[static const 2]); /* { dg-bogus "warning" "warning in place of error" } */
/* { dg-error "ISO C90" "\[static quals expr\] not in C90" { target *-*-* } .-1 } */
void bar4 (int a[const static 2]); /* { dg-bogus "warning" "warning in place of error" } */
/* { dg-error "ISO C90" "\[quals static expr\] not in C90" { target *-*-* } .-1 } */

/* Because [*] isn't properly implemented and so warns, we don't test here
   for [const *] yet.  */

// SLATE-FILECHECK-STD DEFAULT iso9899:1990
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
// DEFAULT-NEXT:     fn %2 @foo0(%14 a: i32, %15 b: ptr<i32> [array=*]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @foo1(%16 <unnamed>: i32, %17 <unnamed>: ptr<i32> [array=*]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %5 @bar0(%18 a: ptr<i32> [const]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %7 @bar1(%19 a: ptr<i32> [const] [array=2]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %9 @bar2(%20 a: ptr<i32> [array=static 2]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %11 @bar3(%21 a: ptr<i32> [const] [array=static 2]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %13 @bar4(%22 a: ptr<i32> [const] [array=static 2]) -> void [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
