/* Test use of sizeof with [*] in type name: should not refer to
   zero-size array.  PR 39582.  */
/* { dg-do compile } */
/* { dg-options "-std=c99 -pedantic-errors" } */

void foo11d(int x[sizeof(int *[*])]); /* { dg-warning "not in a declaration" } */

/* Although the size is not constant, it may nevertheless appear in a
   constant expression if not evaluated.  */

void foo11e(int x[1 ? 0 : sizeof(int *[*])]); /* { dg-warning "not in a declaration" } */
/* { dg-error "zero-size array" "correct zero size" { target *-*-* } .-1 } */

// SLATE-FILECHECK-STD DEFAULT c99
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
// DEFAULT-NEXT:     fn %[[VALUE_foo11d:[0-9]+]] @foo11d(%[[VALUE_x:[0-9]+]] x: ptr<i32> [array=*]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo11e:[0-9]+]] @foo11e(%[[VALUE_x_2:[0-9]+]] x: ptr<i32> [array=0]) -> void [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
