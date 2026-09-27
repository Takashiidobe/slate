/* Test for flexible array members.  Test for where structures with
   such members may not occur.  */
/* Origin: Joseph Myers <jsm28@cam.ac.uk> */
/* { dg-do compile } */
/* { dg-options "-std=iso9899:1999 -pedantic-errors" } */

struct flex { int a; int b[]; };
union rf1 { struct flex a; int b; };
union rf2 { int a; struct flex b; };
union rf3 { int a; union rf1 b; };
union rf4 { union rf2 a; int b; };

/* The above structure and unions may not be members of structures or
   elements of arrays (6.7.2.1#2).  */

struct t0 { struct flex a; }; /* { dg-bogus "warning" "warning in place of error" } */
/* { dg-error "invalid use of structure" "struct in struct" { target *-*-* } .-1 } */
struct t1 { union rf1 a; }; /* { dg-bogus "warning" "warning in place of error" } */
/* { dg-error "invalid use of structure" "union in struct" { target *-*-* } .-1 } */
struct t2 { union rf2 a; }; /* { dg-bogus "warning" "warning in place of error" } */
/* { dg-error "invalid use of structure" "union in struct" { target *-*-* } .-1 } */
struct t3 { union rf3 a; }; /* { dg-bogus "warning" "warning in place of error" } */
/* { dg-error "invalid use of structure" "recursive union in struct" { target *-*-* } .-1 } */
struct t4 { union rf4 a; }; /* { dg-bogus "warning" "warning in place of error" } */
/* { dg-error "invalid use of structure" "recursive union in struct" { target *-*-* } .-1 } */

void f0 (struct flex[]); /* { dg-bogus "warning" "warning in place of error" } */
/* { dg-error "invalid use of structure" "struct in array" { target *-*-* } .-1 } */
void f1 (union rf1[]); /* { dg-bogus "warning" "warning in place of error" } */
/* { dg-error "invalid use of structure" "union in array" { target *-*-* } .-1 } */
void f2 (union rf2[]); /* { dg-bogus "warning" "warning in place of error" } */
/* { dg-error "invalid use of structure" "union in array" { target *-*-* } .-1 } */
void f3 (union rf3[]); /* { dg-bogus "warning" "warning in place of error" } */
/* { dg-error "invalid use of structure" "recursive union in array" { target *-*-* } .-1 } */
void f4 (union rf4[]); /* { dg-bogus "warning" "warning in place of error" } */
/* { dg-error "invalid use of structure" "recursive union in array" { target *-*-* } .-1 } */

struct flex a0[1]; /* { dg-bogus "warning" "warning in place of error" } */
/* { dg-error "invalid use of structure" "struct in array" { target *-*-* } .-1 } */
union rf1 a1[1]; /* { dg-bogus "warning" "warning in place of error" } */
/* { dg-error "invalid use of structure" "union in array" { target *-*-* } .-1 } */
union rf2 a2[1]; /* { dg-bogus "warning" "warning in place of error" } */
/* { dg-error "invalid use of structure" "union in array" { target *-*-* } .-1 } */
union rf3 a3[1]; /* { dg-bogus "warning" "warning in place of error" } */
/* { dg-error "invalid use of structure" "recursive union in array" { target *-*-* } .-1 } */
union rf4 a4[1]; /* { dg-bogus "warning" "warning in place of error" } */
/* { dg-error "invalid use of structure" "recursive union in array" { target *-*-* } .-1 } */

// SLATE-FILECHECK-FLAVOR gcc
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
// DEFAULT-NEXT:     type @type0 flex = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: array<i32, incomplete>;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type1 rf1 = union {
// DEFAULT-NEXT:         field0 a: @type0;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type2 rf2 = union {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: @type0;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type3 rf3 = union {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: @type1;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type4 rf4 = union {
// DEFAULT-NEXT:         field0 a: @type2;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type5 t0 = struct {
// DEFAULT-NEXT:         field0 a: @type0;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type6 t1 = struct {
// DEFAULT-NEXT:         field0 a: @type1;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type7 t2 = struct {
// DEFAULT-NEXT:         field0 a: @type2;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type8 t3 = struct {
// DEFAULT-NEXT:         field0 a: @type3;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type9 t4 = struct {
// DEFAULT-NEXT:         field0 a: @type4;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     global %15 a0: array<@type0, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %16 a1: array<@type1, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %17 a2: array<@type2, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %18 a3: array<@type3, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %19 a4: array<@type4, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %10 @f0(%20 <unnamed>: ptr<@type0>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %11 @f1(%21 <unnamed>: ptr<@type1>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %12 @f2(%22 <unnamed>: ptr<@type2>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %13 @f3(%23 <unnamed>: ptr<@type3>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %14 @f4(%24 <unnamed>: ptr<@type4>) -> void [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
