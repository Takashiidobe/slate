/* Test invalid initializers that are consistent with the syntax: undefined
   behavior ("shall" in Semantics not Constraints) before C2y, constraint
   violation in C2y.  Scalar cases; see bug 88642.  */
/* { dg-do compile } */
/* { dg-options "-std=c2y -pedantic-errors" } */

struct s { int a; };
union u { int a; };

int i1 = { 1, 2 }; /* { dg-error "excess elements in scalar initializer" } */
int i2 = { { 1 } }; /* { dg-error "braces around scalar initializer" } */
int i3 = { { 1, } }; /* { dg-error "braces around scalar initializer" } */
int i4 = { { 1 }, }; /* { dg-error "braces around scalar initializer" } */
int i5 = { 1, { } }; /* { dg-error "excess elements in scalar initializer" } */
/* { dg-error "braces around scalar initializer" "braces" { target *-*-* } .-1 } */
int i6 = { { } }; /* { dg-error "braces around scalar initializer" } */
int i7 = { { }, }; /* { dg-error "braces around scalar initializer" } */
int i8 = { { { 1 } } }; /* { dg-error "braces around scalar initializer" } */
struct s s1 =
  {
    { /* { dg-warning "braces around scalar initializer" } */
      { 1 } /* { dg-error "braces around scalar initializer" } */
    }
  };
union u u1 =
  {
    { /* { dg-warning "braces around scalar initializer" } */
      { 1 } /* { dg-error "braces around scalar initializer" } */
    }
  };
int a1[1] =
  {
    { /* { dg-warning "braces around scalar initializer" } */
      { 1 } /* { dg-error "braces around scalar initializer" } */
    }
  };
int *p1 = &(int) { { 1 } }; /* { dg-error "braces around scalar initializer" } */
int *p2 = &(int) { { 1, } }; /* { dg-error "braces around scalar initializer" } */

int ok1 = { 1 };
struct s ok2 = { { 1 } }; /* { dg-warning "braces around scalar initializer" } */
struct s ok3 = { { 1, } }; /* { dg-warning "braces around scalar initializer" } */
int *ok4 = &(int) { 1 };
int *ok5 = &(int) { 1, };
int ok6[1] = { { 1 } }; /* { dg-warning "braces around scalar initializer" } */
int ok7[1] = { { 1, } }; /* { dg-warning "braces around scalar initializer" } */
union u ok8 = { { 1 } }; /* { dg-warning "braces around scalar initializer" } */
union u ok9 = { { 1, } }; /* { dg-warning "braces around scalar initializer" } */

// SLATE-FILECHECK-STD DEFAULT c2y
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
// DEFAULT-NEXT:     type @type[[TYPE_s:[0-9]+]] s = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_u:[0-9]+]] u = union {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_i1:[0-9]+]] i1: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_i2:[0-9]+]] i2: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_i3:[0-9]+]] i3: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_i4:[0-9]+]] i4: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_i5:[0-9]+]] i5: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_i6:[0-9]+]] i6: i32 [storage=static] = aggregate<i32, zero_fill=true>() [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_i7:[0-9]+]] i7: i32 [storage=static] = aggregate<i32, zero_fill=true>() [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_i8:[0-9]+]] i8: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_s1:[0-9]+]] s1: @type[[TYPE_s]] [storage=static] = aggregate<@type[[TYPE_s]], zero_fill=false>(field0 = const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_u1:[0-9]+]] u1: @type[[TYPE_u]] [storage=static] = aggregate<@type[[TYPE_u]], zero_fill=false>(field0 = const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_a1:[0-9]+]] a1: array<i32, 1> [storage=static] = aggregate<array<i32, 1>, zero_fill=false>(index0 = const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_p1:[0-9]+]] p1: ptr<i32> [storage=static] = addr_of<ptr<i32>>(compound_literal %[[VALUE0:[0-9]+]] [storage=static] = const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_p2:[0-9]+]] p2: ptr<i32> [storage=static] = addr_of<ptr<i32>>(compound_literal %[[VALUE1:[0-9]+]] [storage=static] = const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ok1:[0-9]+]] ok1: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ok2:[0-9]+]] ok2: @type[[TYPE_s]] [storage=static] = aggregate<@type[[TYPE_s]], zero_fill=false>(field0 = const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ok3:[0-9]+]] ok3: @type[[TYPE_s]] [storage=static] = aggregate<@type[[TYPE_s]], zero_fill=false>(field0 = const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ok4:[0-9]+]] ok4: ptr<i32> [storage=static] = addr_of<ptr<i32>>(compound_literal %[[VALUE2:[0-9]+]] [storage=static] = const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ok5:[0-9]+]] ok5: ptr<i32> [storage=static] = addr_of<ptr<i32>>(compound_literal %[[VALUE3:[0-9]+]] [storage=static] = const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ok6:[0-9]+]] ok6: array<i32, 1> [storage=static] = aggregate<array<i32, 1>, zero_fill=false>(index0 = const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ok7:[0-9]+]] ok7: array<i32, 1> [storage=static] = aggregate<array<i32, 1>, zero_fill=false>(index0 = const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ok8:[0-9]+]] ok8: @type[[TYPE_u]] [storage=static] = aggregate<@type[[TYPE_u]], zero_fill=false>(field0 = const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ok9:[0-9]+]] ok9: @type[[TYPE_u]] [storage=static] = aggregate<@type[[TYPE_u]], zero_fill=false>(field0 = const<i32>(1)) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
