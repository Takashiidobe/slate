/* { dg-options "-Wmissing-braces -fdiagnostics-show-caret" } */

struct sf2 { int i; int j; };
struct sf3 { int i; int j; int k; };
struct sa2 { int arr[2]; };
struct sa3 { int arr[3]; };

int arr_12[12] = \
  { 0, 1, 2, 3, 4, 5,
    6, 7, 8, 9, 10, 11};

int arr_12_1[12][1] = \
  { 0, 1, 2, 3, 4, 5, /* { dg-warning "missing braces around initializer" } */
    6, 7, 8, 9, 10, 11};
  /* { dg-begin-multiline-output "" }
   { 0, 1, 2, 3, 4, 5,
   ^
     {} {} {} {} {} {}
     6, 7, 8, 9, 10, 11};
     {} {} {} {} { } { }
     { dg-end-multiline-output "" } */

int arr_1_12[1][12] = \
  { 0, 1, 2, 3, 4, 5, /* { dg-warning "missing braces around initializer" } */
    6, 7, 8, 9, 10, 11};
  /* { dg-begin-multiline-output "" }
   { 0, 1, 2, 3, 4, 5,
   ^
     {
     6, 7, 8, 9, 10, 11};
                       }
     { dg-end-multiline-output "" } */

int arr_2_6[2][6] = \
  { 0, 1, 2, 3, 4, 5, /* { dg-warning "missing braces around initializer" } */
    6, 7, 8, 9, 10, 11};
  /* { dg-begin-multiline-output "" }
   { 0, 1, 2, 3, 4, 5,
   ^
     {               }
     6, 7, 8, 9, 10, 11};
     {                 }
     { dg-end-multiline-output "" } */

int arr_2_2_3[2][2][3] = \
  { 0, 1, 2, 3, 4, 5, /* { dg-warning "missing braces around initializer" } */
    6, 7, 8, 9, 10, 11};
  /* { dg-begin-multiline-output "" }
   { 0, 1, 2, 3, 4, 5,
   ^
     {{     } {      }}
     6, 7, 8, 9, 10, 11};
     {{     } {        }}
     { dg-end-multiline-output "" } */

int arr_2_3_2[2][3][2] = \
  { 0, 1, 2, 3, 4, 5, /* { dg-warning "missing braces around initializer" } */
    6, 7, 8, 9, 10, 11};
  /* { dg-begin-multiline-output "" }
   { 0, 1, 2, 3, 4, 5,
   ^
     {{  } {   } {   }}
     6, 7, 8, 9, 10, 11};
     {{  } {   } {     }}
     { dg-end-multiline-output "" } */

int arr_6_2[6][2] = \
  { 0, 1, 2, 3, 4, 5, /* { dg-warning "missing braces around initializer" } */
    6, 7, 8, 9, 10, 11};
  /* { dg-begin-multiline-output "" }
   { 0, 1, 2, 3, 4, 5,
   ^
     {   } {   } {   }
     6, 7, 8, 9, 10, 11};
     {   } {   } {     }
     { dg-end-multiline-output "" } */

int arr_3_2_2[3][2][2] = \
  { 0, 1, 2, 3, 4, 5, /* { dg-warning "missing braces around initializer" } */
    6, 7, 8, 9, 10, 11};
  /* { dg-begin-multiline-output "" }
   { 0, 1, 2, 3, 4, 5,
   ^
     {{  } {   }}{{  }
     6, 7, 8, 9, 10, 11};
     {   }}{{  } {     }}
     { dg-end-multiline-output "" } */

int arr_3_4[3][4] = \
  { 0, 1, 2, 3, 4, 5, /* { dg-warning "missing braces around initializer" } */
    6, 7, 8, 9, 10, 11};
  /* { dg-begin-multiline-output "" }
   { 0, 1, 2, 3, 4, 5,
   ^
     {         } {
     6, 7, 8, 9, 10, 11};
         } {           }
     { dg-end-multiline-output "" } */

int arr_4_3[4][3] = \
  { 0, 1, 2, 3, 4, 5, /* { dg-warning "missing braces around initializer" } */
    6, 7, 8, 9, 10, 11};
  /* { dg-begin-multiline-output "" }
   { 0, 1, 2, 3, 4, 5,
   ^
     {      } {      }
     6, 7, 8, 9, 10, 11};
     {      } {        }
     { dg-end-multiline-output "" } */

int arr_2_1_6[2][1][6] = \
  { 0, 1, 2, 3, 4, 5, /* { dg-warning "missing braces around initializer" } */
    6, 7, 8, 9, 10, 11};
  /* { dg-begin-multiline-output "" }
   { 0, 1, 2, 3, 4, 5,
   ^
     {{              }}
     6, 7, 8, 9, 10, 11};
     {{                }}
     { dg-end-multiline-output "" } */

struct sf2 arr_6_sf2[6] = \
  { 0, 1, 2, 3, 4, 5, /* { dg-warning "missing braces around initializer" } */
    6, 7, 8, 9, 10, 11};
  /* { dg-begin-multiline-output "" }
   { 0, 1, 2, 3, 4, 5,
   ^
     {   } {   } {   }
     6, 7, 8, 9, 10, 11};
     {   } {   } {     }
     { dg-end-multiline-output "" } */

struct sf3 arr_4_sf3[4] = \
  { 0, 1, 2, 3, 4, 5, /* { dg-warning "missing braces around initializer" } */
    6, 7, 8, 9, 10, 11};
  /* { dg-begin-multiline-output "" }
   { 0, 1, 2, 3, 4, 5,
   ^
     {      } {      }
     6, 7, 8, 9, 10, 11};
     {      } {        }
     { dg-end-multiline-output "" } */

struct sa2 arr_6_sa2[6] = \
  { 0, 1, 2, 3, 4, 5, /* { dg-warning "missing braces around initializer" } */
    6, 7, 8, 9, 10, 11};
  /* { dg-begin-multiline-output "" }
   { 0, 1, 2, 3, 4, 5,
   ^
     {{  }}{{  }}{{  }}
     6, 7, 8, 9, 10, 11};
     {{  }}{{  }}{{    }}
     { dg-end-multiline-output "" } */

struct sa3 arr_4_sa3[4] = \
  { 0, 1, 2, 3, 4, 5, /* { dg-warning "missing braces around initializer" } */
    6, 7, 8, 9, 10, 11};
  /* { dg-begin-multiline-output "" }
   { 0, 1, 2, 3, 4, 5,
   ^
     {{     }}{{     }}
     6, 7, 8, 9, 10, 11};
     {{     }}{{       }}
     { dg-end-multiline-output "" } */

/* PR c/81405.  */
int a5[][0][0] = { 1, 2 }; /* { dg-line pr_81405 } */

  /* { dg-warning "missing braces around initializer" "" { target c } pr_81405 } */
  /* { dg-begin-multiline-output "" }
 int a5[][0][0] = { 1, 2 };
                  ^
 {                  -----
                    {{1}}}}, {{{2 }}
     { dg-end-multiline-output "" } */

  /* { dg-warning "excess elements" "" { target c } pr_81405 } */
  /* { dg-begin-multiline-output "" }
 int a5[][0][0] = { 1, 2 };
                    ^
     { dg-end-multiline-output "" } */
  /* { dg-begin-multiline-output "" }
 int a5[][0][0] = { 1, 2 };
                       ^
     { dg-end-multiline-output "" } */
  /* { dg-begin-multiline-output "" }
 int a5[][0][0] = { 1, 2 };
 ^~~
     { dg-end-multiline-output "" } */

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
// DEFAULT-NEXT:     type @type[[TYPE_sf2:[0-9]+]] sf2 = struct {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:         field1 j: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_sf3:[0-9]+]] sf3 = struct {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:         field1 j: i32;
// DEFAULT-NEXT:         field2 k: i32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_sa2:[0-9]+]] sa2 = struct {
// DEFAULT-NEXT:         field0 arr: array<i32, 2>;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_sa3:[0-9]+]] sa3 = struct {
// DEFAULT-NEXT:         field0 arr: array<i32, 3>;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_arr_12:[0-9]+]] arr_12: array<i32, 12> [storage=static] [align=16] = aggregate<array<i32, 12>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1), index2 = const<i32>(2), index3 = const<i32>(3), index4 = const<i32>(4), index5 = const<i32>(5), index6 = const<i32>(6), index7 = const<i32>(7), index8 = const<i32>(8), index9 = const<i32>(9), index10 = const<i32>(10), index11 = const<i32>(11)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_arr_12_1:[0-9]+]] arr_12_1: array<array<i32, 1>, 12> [storage=static] [align=16] = aggregate<array<array<i32, 1>, 12>, zero_fill=false>(index0 = aggregate<array<i32, 1>, zero_fill=false>(index0 = const<i32>(0)), index1 = aggregate<array<i32, 1>, zero_fill=false>(index0 = const<i32>(1)), index2 = aggregate<array<i32, 1>, zero_fill=false>(index0 = const<i32>(2)), index3 = aggregate<array<i32, 1>, zero_fill=false>(index0 = const<i32>(3)), index4 = aggregate<array<i32, 1>, zero_fill=false>(index0 = const<i32>(4)), index5 = aggregate<array<i32, 1>, zero_fill=false>(index0 = const<i32>(5)), index6 = aggregate<array<i32, 1>, zero_fill=false>(index0 = const<i32>(6)), index7 = aggregate<array<i32, 1>, zero_fill=false>(index0 = const<i32>(7)), index8 = aggregate<array<i32, 1>, zero_fill=false>(index0 = const<i32>(8)), index9 = aggregate<array<i32, 1>, zero_fill=false>(index0 = const<i32>(9)), index10 = aggregate<array<i32, 1>, zero_fill=false>(index0 = const<i32>(10)), index11 = aggregate<array<i32, 1>, zero_fill=false>(index0 = const<i32>(11))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_arr_1_12:[0-9]+]] arr_1_12: array<array<i32, 12>, 1> [storage=static] [align=16] = aggregate<array<array<i32, 12>, 1>, zero_fill=false>(index0 = aggregate<array<i32, 12>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1), index2 = const<i32>(2), index3 = const<i32>(3), index4 = const<i32>(4), index5 = const<i32>(5), index6 = const<i32>(6), index7 = const<i32>(7), index8 = const<i32>(8), index9 = const<i32>(9), index10 = const<i32>(10), index11 = const<i32>(11))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_arr_2_6:[0-9]+]] arr_2_6: array<array<i32, 6>, 2> [storage=static] [align=16] = aggregate<array<array<i32, 6>, 2>, zero_fill=false>(index0 = aggregate<array<i32, 6>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1), index2 = const<i32>(2), index3 = const<i32>(3), index4 = const<i32>(4), index5 = const<i32>(5)), index1 = aggregate<array<i32, 6>, zero_fill=false>(index0 = const<i32>(6), index1 = const<i32>(7), index2 = const<i32>(8), index3 = const<i32>(9), index4 = const<i32>(10), index5 = const<i32>(11))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_arr_2_2_3:[0-9]+]] arr_2_2_3: array<array<array<i32, 3>, 2>, 2> [storage=static] [align=16] = aggregate<array<array<array<i32, 3>, 2>, 2>, zero_fill=false>(index0 = aggregate<array<array<i32, 3>, 2>, zero_fill=false>(index0 = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1), index2 = const<i32>(2)), index1 = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(3), index1 = const<i32>(4), index2 = const<i32>(5))), index1 = aggregate<array<array<i32, 3>, 2>, zero_fill=false>(index0 = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(6), index1 = const<i32>(7), index2 = const<i32>(8)), index1 = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(9), index1 = const<i32>(10), index2 = const<i32>(11)))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_arr_2_3_2:[0-9]+]] arr_2_3_2: array<array<array<i32, 2>, 3>, 2> [storage=static] [align=16] = aggregate<array<array<array<i32, 2>, 3>, 2>, zero_fill=false>(index0 = aggregate<array<array<i32, 2>, 3>, zero_fill=false>(index0 = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)), index1 = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(2), index1 = const<i32>(3)), index2 = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(4), index1 = const<i32>(5))), index1 = aggregate<array<array<i32, 2>, 3>, zero_fill=false>(index0 = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(6), index1 = const<i32>(7)), index1 = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(8), index1 = const<i32>(9)), index2 = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(10), index1 = const<i32>(11)))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_arr_6_2:[0-9]+]] arr_6_2: array<array<i32, 2>, 6> [storage=static] [align=16] = aggregate<array<array<i32, 2>, 6>, zero_fill=false>(index0 = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)), index1 = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(2), index1 = const<i32>(3)), index2 = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(4), index1 = const<i32>(5)), index3 = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(6), index1 = const<i32>(7)), index4 = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(8), index1 = const<i32>(9)), index5 = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(10), index1 = const<i32>(11))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_arr_3_2_2:[0-9]+]] arr_3_2_2: array<array<array<i32, 2>, 2>, 3> [storage=static] [align=16] = aggregate<array<array<array<i32, 2>, 2>, 3>, zero_fill=false>(index0 = aggregate<array<array<i32, 2>, 2>, zero_fill=false>(index0 = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)), index1 = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(2), index1 = const<i32>(3))), index1 = aggregate<array<array<i32, 2>, 2>, zero_fill=false>(index0 = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(4), index1 = const<i32>(5)), index1 = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(6), index1 = const<i32>(7))), index2 = aggregate<array<array<i32, 2>, 2>, zero_fill=false>(index0 = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(8), index1 = const<i32>(9)), index1 = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(10), index1 = const<i32>(11)))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_arr_3_4:[0-9]+]] arr_3_4: array<array<i32, 4>, 3> [storage=static] [align=16] = aggregate<array<array<i32, 4>, 3>, zero_fill=false>(index0 = aggregate<array<i32, 4>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1), index2 = const<i32>(2), index3 = const<i32>(3)), index1 = aggregate<array<i32, 4>, zero_fill=false>(index0 = const<i32>(4), index1 = const<i32>(5), index2 = const<i32>(6), index3 = const<i32>(7)), index2 = aggregate<array<i32, 4>, zero_fill=false>(index0 = const<i32>(8), index1 = const<i32>(9), index2 = const<i32>(10), index3 = const<i32>(11))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_arr_4_3:[0-9]+]] arr_4_3: array<array<i32, 3>, 4> [storage=static] [align=16] = aggregate<array<array<i32, 3>, 4>, zero_fill=false>(index0 = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1), index2 = const<i32>(2)), index1 = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(3), index1 = const<i32>(4), index2 = const<i32>(5)), index2 = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(6), index1 = const<i32>(7), index2 = const<i32>(8)), index3 = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(9), index1 = const<i32>(10), index2 = const<i32>(11))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_arr_2_1_6:[0-9]+]] arr_2_1_6: array<array<array<i32, 6>, 1>, 2> [storage=static] [align=16] = aggregate<array<array<array<i32, 6>, 1>, 2>, zero_fill=false>(index0 = aggregate<array<array<i32, 6>, 1>, zero_fill=false>(index0 = aggregate<array<i32, 6>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1), index2 = const<i32>(2), index3 = const<i32>(3), index4 = const<i32>(4), index5 = const<i32>(5))), index1 = aggregate<array<array<i32, 6>, 1>, zero_fill=false>(index0 = aggregate<array<i32, 6>, zero_fill=false>(index0 = const<i32>(6), index1 = const<i32>(7), index2 = const<i32>(8), index3 = const<i32>(9), index4 = const<i32>(10), index5 = const<i32>(11)))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_arr_6_sf2:[0-9]+]] arr_6_sf2: array<@type[[TYPE_sf2]], 6> [storage=static] [align=16] = aggregate<array<@type[[TYPE_sf2]], 6>, zero_fill=false>(index0 = aggregate<@type[[TYPE_sf2]], zero_fill=false>(field0 = const<i32>(0), field1 = const<i32>(1)), index1 = aggregate<@type[[TYPE_sf2]], zero_fill=false>(field0 = const<i32>(2), field1 = const<i32>(3)), index2 = aggregate<@type[[TYPE_sf2]], zero_fill=false>(field0 = const<i32>(4), field1 = const<i32>(5)), index3 = aggregate<@type[[TYPE_sf2]], zero_fill=false>(field0 = const<i32>(6), field1 = const<i32>(7)), index4 = aggregate<@type[[TYPE_sf2]], zero_fill=false>(field0 = const<i32>(8), field1 = const<i32>(9)), index5 = aggregate<@type[[TYPE_sf2]], zero_fill=false>(field0 = const<i32>(10), field1 = const<i32>(11))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_arr_4_sf3:[0-9]+]] arr_4_sf3: array<@type[[TYPE_sf3]], 4> [storage=static] [align=16] = aggregate<array<@type[[TYPE_sf3]], 4>, zero_fill=false>(index0 = aggregate<@type[[TYPE_sf3]], zero_fill=false>(field0 = const<i32>(0), field1 = const<i32>(1), field2 = const<i32>(2)), index1 = aggregate<@type[[TYPE_sf3]], zero_fill=false>(field0 = const<i32>(3), field1 = const<i32>(4), field2 = const<i32>(5)), index2 = aggregate<@type[[TYPE_sf3]], zero_fill=false>(field0 = const<i32>(6), field1 = const<i32>(7), field2 = const<i32>(8)), index3 = aggregate<@type[[TYPE_sf3]], zero_fill=false>(field0 = const<i32>(9), field1 = const<i32>(10), field2 = const<i32>(11))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_arr_6_sa2:[0-9]+]] arr_6_sa2: array<@type[[TYPE_sa2]], 6> [storage=static] [align=16] = aggregate<array<@type[[TYPE_sa2]], 6>, zero_fill=false>(index0 = aggregate<@type[[TYPE_sa2]], zero_fill=false>(field0 = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1))), index1 = aggregate<@type[[TYPE_sa2]], zero_fill=false>(field0 = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(2), index1 = const<i32>(3))), index2 = aggregate<@type[[TYPE_sa2]], zero_fill=false>(field0 = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(4), index1 = const<i32>(5))), index3 = aggregate<@type[[TYPE_sa2]], zero_fill=false>(field0 = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(6), index1 = const<i32>(7))), index4 = aggregate<@type[[TYPE_sa2]], zero_fill=false>(field0 = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(8), index1 = const<i32>(9))), index5 = aggregate<@type[[TYPE_sa2]], zero_fill=false>(field0 = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(10), index1 = const<i32>(11)))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_arr_4_sa3:[0-9]+]] arr_4_sa3: array<@type[[TYPE_sa3]], 4> [storage=static] [align=16] = aggregate<array<@type[[TYPE_sa3]], 4>, zero_fill=false>(index0 = aggregate<@type[[TYPE_sa3]], zero_fill=false>(field0 = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1), index2 = const<i32>(2))), index1 = aggregate<@type[[TYPE_sa3]], zero_fill=false>(field0 = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(3), index1 = const<i32>(4), index2 = const<i32>(5))), index2 = aggregate<@type[[TYPE_sa3]], zero_fill=false>(field0 = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(6), index1 = const<i32>(7), index2 = const<i32>(8))), index3 = aggregate<@type[[TYPE_sa3]], zero_fill=false>(field0 = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(9), index1 = const<i32>(10), index2 = const<i32>(11)))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_a5:[0-9]+]] a5: array<array<array<i32, 0>, 0>, 2> [storage=static] = aggregate<array<array<array<i32, 0>, 0>, 2>, zero_fill=false>(index0 = aggregate<array<array<i32, 0>, 0>, zero_fill=false>(), index1 = aggregate<array<array<i32, 0>, 0>, zero_fill=false>()) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
