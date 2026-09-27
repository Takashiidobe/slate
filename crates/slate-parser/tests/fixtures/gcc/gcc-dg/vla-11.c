/* Further tests of [*] being rejected other that in declarations, as
   per the consensus in DR#341 that the second example there should be
   invalid (but warnings because the final wording appears to allow
   these cases).  */
/* Origin: Joseph Myers <joseph@codesourcery.com> */
/* { dg-do compile } */
/* { dg-options "-std=c99 -pedantic-errors" } */

void foo11a(int x[sizeof(int *(*)[*])]);	/* { dg-warning "not in a declaration" } */
void foo11b(__SIZE_TYPE__ x, int y[(__UINTPTR_TYPE__)(int (*)[*])x]);	/* { dg-warning "not in a declaration" } */
void foo11c(struct s { int (*x)[*]; } *y);	/* { dg-error "a member of a structure or union cannot have a variably modified type" "variably modified" } */
/* { dg-warning "'struct s' declared inside parameter list" "struct decl" { target *-*-* } .-1 } */

// SLATE-FILECHECK-FLAVOR gcc
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
// DEFAULT-NEXT:     type @type0 s = struct {
// DEFAULT-NEXT:         field0 x: ptr<vla<i32, *>>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     fn %0 @foo11a(%4 x: ptr<i32> [array=8]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @foo11b(%5 x: u64, %6 y: ptr<i32> [array=*]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @foo11c(%7 y: ptr<@type0>) -> void [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
