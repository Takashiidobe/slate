/* Test for ICEs on invalid transparent unions (empty or no named
   members).  Bug 21213.  */
/* Origin: Joseph Myers <joseph@codesourcery.com> */
/* { dg-do compile } */
/* { dg-options "" } */

enum e { A };

union __attribute__((__transparent_union__)) ue1 { enum e; }; /* { dg-warning "declaration does not declare anything" "not anything" } */
/* { dg-warning "union cannot be made transparent" "cannot" { target *-*-* } .-1 } */
union ue2 { enum e; } __attribute__((__transparent_union__)); /* { dg-warning "declaration does not declare anything" "not anything" } */
/* { dg-warning "union cannot be made transparent" "cannot" { target *-*-* } .-1 } */

union __attribute__((__transparent_union__)) ui1 { int; }; /* { dg-warning "declaration does not declare anything" "not anything" } */
/* { dg-warning "union cannot be made transparent" "cannot" { target *-*-* } .-1 } */
union ui2 { int; } __attribute__((__transparent_union__)); /* { dg-warning "declaration does not declare anything" "no anything" } */
/* { dg-warning "union cannot be made transparent" "cannot" { target *-*-* } .-1 } */

union __attribute__((__transparent_union__)) u1 { };
/* { dg-warning "union cannot be made transparent" "" { target *-*-* } .-1 } */
union u2 { } __attribute__((__transparent_union__));
/* { dg-warning "union cannot be made transparent" "" { target *-*-* } .-1 } */

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
// DEFAULT-NEXT:     type @type0 e = enum : u32 {
// DEFAULT-NEXT:         %0 A = const<i32>(0);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type1 ue1 = union {
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[]];
// DEFAULT-NEXT:     type @type2 ue2 = union {
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[]];
// DEFAULT-NEXT:     type @type3 ui1 = union {
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[]];
// DEFAULT-NEXT:     type @type4 ui2 = union {
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[]];
// DEFAULT-NEXT:     type @type5 u1 = union {
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[]];
// DEFAULT-NEXT:     type @type6 u2 = union {
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[]];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
