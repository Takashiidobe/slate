/* ICE arising from bug computing composite type of zero-length array
   types: PR 35433.  */
/* { dg-do compile } */
/* { dg-options "" } */

typedef int* X;
typedef int* Y;

X (*p)[][0];
Y (*q)[][0];

typeof(*(0 ? p : q)) x = { 0 }; /* { dg-warning "excess elements in array initializer|near initialization" } */

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
// DEFAULT-NEXT:     type @type[[TYPE_X:[0-9]+]] X = ptr<i32>;
// DEFAULT-NEXT:     type @type[[TYPE_Y:[0-9]+]] Y = ptr<i32>;
// DEFAULT-NEXT:     global %[[VALUE_p:[0-9]+]] p: ptr<array<array<ptr<i32>, 0>, incomplete>> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_q:[0-9]+]] q: ptr<array<array<ptr<i32>, 0>, incomplete>> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_x:[0-9]+]] x: array<array<ptr<i32>, 0>, 1> [storage=static] = aggregate<array<array<ptr<i32>, 0>, 1>, zero_fill=false>(index0 = aggregate<array<ptr<i32>, 0>, zero_fill=false>()) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
