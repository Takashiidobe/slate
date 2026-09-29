/* Test for various cases of excess initializers for empty objects:
   bug 21873.  Various versions of GCC ICE, hang or loop repeating
   diagnostics on various of these tests.  */
/* Origin: Joseph Myers <joseph@codesourcery.com> */
/* { dg-do compile } */
/* { dg-options "" } */

struct s0 { };
struct s1 { int a; };
struct s2 { int a; int b; };

int a0[0] = { 1, 2 }; /* { dg-warning "excess elements|near init" } */
int a1[0][0] = { 1, 2 }; /* { dg-warning "excess elements|near init" } */
int a2[0][1] = { 1, 2 }; /* { dg-warning "excess elements|near init" } */
int a3[1][0] = { 1, 2 }; /* { dg-warning "excess elements|near init" } */
int a4[][0] = { 1, 2 }; /* { dg-warning "excess elements|near init" } */
int a5[][0][0] = { 1, 2 }; /* { dg-warning "excess elements|near init" } */
int a6[][0][1] = { 1, 2 }; /* { dg-warning "excess elements|near init" } */
int a7[][1][0] = { 1, 2 }; /* { dg-warning "excess elements|near init" } */

struct s0 b0[0] = { 1, 2 }; /* { dg-warning "excess elements|near init" } */
struct s0 b1[0][0] = { 1, 2 }; /* { dg-warning "excess elements|near init" } */
struct s0 b2[0][1] = { 1, 2 }; /* { dg-warning "excess elements|near init" } */
struct s0 b3[1][0] = { 1, 2 }; /* { dg-warning "excess elements|near init" } */
struct s0 b4[][0] = { 1, 2 }; /* { dg-warning "excess elements|near init" } */
struct s0 b5[][0][0] = { 1, 2 }; /* { dg-warning "excess elements|near init" } */
struct s0 b6[][0][1] = { 1, 2 }; /* { dg-warning "excess elements|near init" } */
struct s0 b7[][1][0] = { 1, 2 }; /* { dg-warning "excess elements|near init" } */
struct s0 b8[1] = { 1, 2 }; /* { dg-warning "excess elements|near init" } */
struct s0 b9[] = { 1, 2 }; /* { dg-warning "excess elements|near init" } */

struct s1 c0[0] = { 1, 2 }; /* { dg-warning "excess elements|near init" } */
struct s1 c1[0][0] = { 1, 2 }; /* { dg-warning "excess elements|near init" } */
struct s1 c2[0][1] = { 1, 2 }; /* { dg-warning "excess elements|near init" } */
struct s1 c3[1][0] = { 1, 2 }; /* { dg-warning "excess elements|near init" } */
struct s1 c4[][0] = { 1, 2 }; /* { dg-warning "excess elements|near init" } */
struct s1 c5[][0][0] = { 1, 2 }; /* { dg-warning "excess elements|near init" } */
struct s1 c6[][0][1] = { 1, 2 }; /* { dg-warning "excess elements|near init" } */
struct s1 c7[][1][0] = { 1, 2 }; /* { dg-warning "excess elements|near init" } */

struct s2 d0[0] = { 1, 2 }; /* { dg-warning "excess elements|near init" } */
struct s2 d1[0][0] = { 1, 2 }; /* { dg-warning "excess elements|near init" } */
struct s2 d2[0][1] = { 1, 2 }; /* { dg-warning "excess elements|near init" } */
struct s2 d3[1][0] = { 1, 2 }; /* { dg-warning "excess elements|near init" } */
struct s2 d4[][0] = { 1, 2 }; /* { dg-warning "excess elements|near init" } */
struct s2 d5[][0][0] = { 1, 2 }; /* { dg-warning "excess elements|near init" } */
struct s2 d6[][0][1] = { 1, 2 }; /* { dg-warning "excess elements|near init" } */
struct s2 d7[][1][0] = { 1, 2 }; /* { dg-warning "excess elements|near init" } */

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
// DEFAULT-NEXT:     type @type[[TYPE_s0:[0-9]+]] s0 = struct {
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[]];
// DEFAULT-NEXT:     type @type[[TYPE_s1:[0-9]+]] s1 = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_s2:[0-9]+]] s2 = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     global %[[VALUE_a0:[0-9]+]] a0: array<i32, 0> [storage=static] = aggregate<array<i32, 0>, zero_fill=false>() [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_a1:[0-9]+]] a1: array<array<i32, 0>, 0> [storage=static] = aggregate<array<array<i32, 0>, 0>, zero_fill=false>() [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_a2:[0-9]+]] a2: array<array<i32, 1>, 0> [storage=static] = aggregate<array<array<i32, 1>, 0>, zero_fill=false>() [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_a3:[0-9]+]] a3: array<array<i32, 0>, 1> [storage=static] = aggregate<array<array<i32, 0>, 1>, zero_fill=false>(index0 = aggregate<array<i32, 0>, zero_fill=false>()) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_a4:[0-9]+]] a4: array<array<i32, 0>, 2> [storage=static] = aggregate<array<array<i32, 0>, 2>, zero_fill=false>(index0 = aggregate<array<i32, 0>, zero_fill=false>(), index1 = aggregate<array<i32, 0>, zero_fill=false>()) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_a5:[0-9]+]] a5: array<array<array<i32, 0>, 0>, 2> [storage=static] = aggregate<array<array<array<i32, 0>, 0>, 2>, zero_fill=false>(index0 = aggregate<array<array<i32, 0>, 0>, zero_fill=false>(), index1 = aggregate<array<array<i32, 0>, 0>, zero_fill=false>()) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_a6:[0-9]+]] a6: array<array<array<i32, 1>, 0>, 2> [storage=static] = aggregate<array<array<array<i32, 1>, 0>, 2>, zero_fill=false>(index0 = aggregate<array<array<i32, 1>, 0>, zero_fill=false>(), index1 = aggregate<array<array<i32, 1>, 0>, zero_fill=false>()) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_a7:[0-9]+]] a7: array<array<array<i32, 0>, 1>, 2> [storage=static] = aggregate<array<array<array<i32, 0>, 1>, 2>, zero_fill=false>(index0 = aggregate<array<array<i32, 0>, 1>, zero_fill=false>(index0 = aggregate<array<i32, 0>, zero_fill=false>()), index1 = aggregate<array<array<i32, 0>, 1>, zero_fill=false>(index0 = aggregate<array<i32, 0>, zero_fill=false>())) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b0:[0-9]+]] b0: array<@type[[TYPE_s0]], 0> [storage=static] = aggregate<array<@type[[TYPE_s0]], 0>, zero_fill=false>() [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b1:[0-9]+]] b1: array<array<@type[[TYPE_s0]], 0>, 0> [storage=static] = aggregate<array<array<@type[[TYPE_s0]], 0>, 0>, zero_fill=false>() [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b2:[0-9]+]] b2: array<array<@type[[TYPE_s0]], 1>, 0> [storage=static] = aggregate<array<array<@type[[TYPE_s0]], 1>, 0>, zero_fill=false>() [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b3:[0-9]+]] b3: array<array<@type[[TYPE_s0]], 0>, 1> [storage=static] = aggregate<array<array<@type[[TYPE_s0]], 0>, 1>, zero_fill=false>(index0 = aggregate<array<@type[[TYPE_s0]], 0>, zero_fill=false>()) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b4:[0-9]+]] b4: array<array<@type[[TYPE_s0]], 0>, 2> [storage=static] = aggregate<array<array<@type[[TYPE_s0]], 0>, 2>, zero_fill=false>(index0 = aggregate<array<@type[[TYPE_s0]], 0>, zero_fill=false>(), index1 = aggregate<array<@type[[TYPE_s0]], 0>, zero_fill=false>()) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b5:[0-9]+]] b5: array<array<array<@type[[TYPE_s0]], 0>, 0>, 2> [storage=static] = aggregate<array<array<array<@type[[TYPE_s0]], 0>, 0>, 2>, zero_fill=false>(index0 = aggregate<array<array<@type[[TYPE_s0]], 0>, 0>, zero_fill=false>(), index1 = aggregate<array<array<@type[[TYPE_s0]], 0>, 0>, zero_fill=false>()) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b6:[0-9]+]] b6: array<array<array<@type[[TYPE_s0]], 1>, 0>, 2> [storage=static] = aggregate<array<array<array<@type[[TYPE_s0]], 1>, 0>, 2>, zero_fill=false>(index0 = aggregate<array<array<@type[[TYPE_s0]], 1>, 0>, zero_fill=false>(), index1 = aggregate<array<array<@type[[TYPE_s0]], 1>, 0>, zero_fill=false>()) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b7:[0-9]+]] b7: array<array<array<@type[[TYPE_s0]], 0>, 1>, 2> [storage=static] = aggregate<array<array<array<@type[[TYPE_s0]], 0>, 1>, 2>, zero_fill=false>(index0 = aggregate<array<array<@type[[TYPE_s0]], 0>, 1>, zero_fill=false>(index0 = aggregate<array<@type[[TYPE_s0]], 0>, zero_fill=false>()), index1 = aggregate<array<array<@type[[TYPE_s0]], 0>, 1>, zero_fill=false>(index0 = aggregate<array<@type[[TYPE_s0]], 0>, zero_fill=false>())) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b8:[0-9]+]] b8: array<@type[[TYPE_s0]], 1> [storage=static] = aggregate<array<@type[[TYPE_s0]], 1>, zero_fill=false>(index0 = aggregate<@type[[TYPE_s0]], zero_fill=false>()) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b9:[0-9]+]] b9: array<@type[[TYPE_s0]], 2> [storage=static] = aggregate<array<@type[[TYPE_s0]], 2>, zero_fill=false>(index0 = aggregate<@type[[TYPE_s0]], zero_fill=false>(), index1 = aggregate<@type[[TYPE_s0]], zero_fill=false>()) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c0:[0-9]+]] c0: array<@type[[TYPE_s1]], 0> [storage=static] = aggregate<array<@type[[TYPE_s1]], 0>, zero_fill=false>() [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c1:[0-9]+]] c1: array<array<@type[[TYPE_s1]], 0>, 0> [storage=static] = aggregate<array<array<@type[[TYPE_s1]], 0>, 0>, zero_fill=false>() [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c2:[0-9]+]] c2: array<array<@type[[TYPE_s1]], 1>, 0> [storage=static] = aggregate<array<array<@type[[TYPE_s1]], 1>, 0>, zero_fill=false>() [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c3:[0-9]+]] c3: array<array<@type[[TYPE_s1]], 0>, 1> [storage=static] = aggregate<array<array<@type[[TYPE_s1]], 0>, 1>, zero_fill=false>(index0 = aggregate<array<@type[[TYPE_s1]], 0>, zero_fill=false>()) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c4:[0-9]+]] c4: array<array<@type[[TYPE_s1]], 0>, 2> [storage=static] = aggregate<array<array<@type[[TYPE_s1]], 0>, 2>, zero_fill=false>(index0 = aggregate<array<@type[[TYPE_s1]], 0>, zero_fill=false>(), index1 = aggregate<array<@type[[TYPE_s1]], 0>, zero_fill=false>()) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c5:[0-9]+]] c5: array<array<array<@type[[TYPE_s1]], 0>, 0>, 2> [storage=static] = aggregate<array<array<array<@type[[TYPE_s1]], 0>, 0>, 2>, zero_fill=false>(index0 = aggregate<array<array<@type[[TYPE_s1]], 0>, 0>, zero_fill=false>(), index1 = aggregate<array<array<@type[[TYPE_s1]], 0>, 0>, zero_fill=false>()) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c6:[0-9]+]] c6: array<array<array<@type[[TYPE_s1]], 1>, 0>, 2> [storage=static] = aggregate<array<array<array<@type[[TYPE_s1]], 1>, 0>, 2>, zero_fill=false>(index0 = aggregate<array<array<@type[[TYPE_s1]], 1>, 0>, zero_fill=false>(), index1 = aggregate<array<array<@type[[TYPE_s1]], 1>, 0>, zero_fill=false>()) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c7:[0-9]+]] c7: array<array<array<@type[[TYPE_s1]], 0>, 1>, 2> [storage=static] = aggregate<array<array<array<@type[[TYPE_s1]], 0>, 1>, 2>, zero_fill=false>(index0 = aggregate<array<array<@type[[TYPE_s1]], 0>, 1>, zero_fill=false>(index0 = aggregate<array<@type[[TYPE_s1]], 0>, zero_fill=false>()), index1 = aggregate<array<array<@type[[TYPE_s1]], 0>, 1>, zero_fill=false>(index0 = aggregate<array<@type[[TYPE_s1]], 0>, zero_fill=false>())) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d0:[0-9]+]] d0: array<@type[[TYPE_s2]], 0> [storage=static] = aggregate<array<@type[[TYPE_s2]], 0>, zero_fill=false>() [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d1:[0-9]+]] d1: array<array<@type[[TYPE_s2]], 0>, 0> [storage=static] = aggregate<array<array<@type[[TYPE_s2]], 0>, 0>, zero_fill=false>() [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d2:[0-9]+]] d2: array<array<@type[[TYPE_s2]], 1>, 0> [storage=static] = aggregate<array<array<@type[[TYPE_s2]], 1>, 0>, zero_fill=false>() [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d3:[0-9]+]] d3: array<array<@type[[TYPE_s2]], 0>, 1> [storage=static] = aggregate<array<array<@type[[TYPE_s2]], 0>, 1>, zero_fill=false>(index0 = aggregate<array<@type[[TYPE_s2]], 0>, zero_fill=false>()) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d4:[0-9]+]] d4: array<array<@type[[TYPE_s2]], 0>, 2> [storage=static] = aggregate<array<array<@type[[TYPE_s2]], 0>, 2>, zero_fill=false>(index0 = aggregate<array<@type[[TYPE_s2]], 0>, zero_fill=false>(), index1 = aggregate<array<@type[[TYPE_s2]], 0>, zero_fill=false>()) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d5:[0-9]+]] d5: array<array<array<@type[[TYPE_s2]], 0>, 0>, 2> [storage=static] = aggregate<array<array<array<@type[[TYPE_s2]], 0>, 0>, 2>, zero_fill=false>(index0 = aggregate<array<array<@type[[TYPE_s2]], 0>, 0>, zero_fill=false>(), index1 = aggregate<array<array<@type[[TYPE_s2]], 0>, 0>, zero_fill=false>()) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d6:[0-9]+]] d6: array<array<array<@type[[TYPE_s2]], 1>, 0>, 2> [storage=static] = aggregate<array<array<array<@type[[TYPE_s2]], 1>, 0>, 2>, zero_fill=false>(index0 = aggregate<array<array<@type[[TYPE_s2]], 1>, 0>, zero_fill=false>(), index1 = aggregate<array<array<@type[[TYPE_s2]], 1>, 0>, zero_fill=false>()) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d7:[0-9]+]] d7: array<array<array<@type[[TYPE_s2]], 0>, 1>, 2> [storage=static] = aggregate<array<array<array<@type[[TYPE_s2]], 0>, 1>, 2>, zero_fill=false>(index0 = aggregate<array<array<@type[[TYPE_s2]], 0>, 1>, zero_fill=false>(index0 = aggregate<array<@type[[TYPE_s2]], 0>, zero_fill=false>()), index1 = aggregate<array<array<@type[[TYPE_s2]], 0>, 1>, zero_fill=false>(index0 = aggregate<array<@type[[TYPE_s2]], 0>, zero_fill=false>())) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
