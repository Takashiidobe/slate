/* Test obsolete forms of designated initializers.  Test with
   -pedantic.  */
/* Origin: Joseph Myers <jsm@polyomino.org.uk> */
/* { dg-do compile } */
/* { dg-options "-std=gnu99 -pedantic" } */
struct s { int a; };
struct s s0 = { .a = 1 };
struct s s1 = { a: 1 }; /* { dg-warning "obsolete use of designated initializer with ':'" } */

int x0[] = { [0] = 1 };
int x1[] = { [0] 1 }; /* { dg-warning "obsolete use of designated initializer without '='" } */

// SLATE-FILECHECK-STD DEFAULT gnu99
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
// DEFAULT-NEXT:     global %[[VALUE_s0:[0-9]+]] s0: @type[[TYPE_s]] [storage=static] = aggregate<@type[[TYPE_s]], zero_fill=false>(field0 = const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_s1:[0-9]+]] s1: @type[[TYPE_s]] [storage=static] = aggregate<@type[[TYPE_s]], zero_fill=false>(field0 = const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_x0:[0-9]+]] x0: array<i32, 1> [storage=static] = aggregate<array<i32, 1>, zero_fill=false>(index0 = const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_x1:[0-9]+]] x1: array<i32, 1> [storage=static] = aggregate<array<i32, 1>, zero_fill=false>(index0 = const<i32>(1)) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
