/* { dg-options "-std=gnu89" } */
/* In GNU C mode, we recognize the anonymous struct/union extension,
   but not Microsoft extensions.  */

struct A { char a; };

/* MS extension.  */
struct B {
  struct A;			/* { dg-warning "does not declare anything" } */
  char b;
};
char testB[sizeof(struct B) == sizeof(struct A) ? 1 : -1];

/* MS extension.  */
struct C {
  struct D { char d; };		/* { dg-warning "does not declare anything" } */
  char c;
};
char testC[sizeof(struct C) == sizeof(struct A) ? 1 : -1];
char testD[sizeof(struct D) == sizeof(struct A) ? 1 : -1];

/* GNU extension.  */
struct E {
  struct { char z; };
  char e;
};
char testE[sizeof(struct E) == 2 * sizeof(struct A) ? 1 : -1];

/* MS extension.  */
typedef struct A typedef_A;
struct F {
  typedef_A;			/* { dg-warning "does not declare anything" } */
  char f;
};
char testF[sizeof(struct F) == sizeof(struct A) ? 1 : -1];

/* Test that __extension__ does the right thing coming _from_ GNU C mode.  */
__extension__ struct G {
  struct { char z; };
  char g;
};
char testG[sizeof(struct G) == 2 * sizeof(struct A) ? 1 : -1];

struct H {
  struct { char z; };
  char h;
};
char testH[sizeof(struct H) == 2 * sizeof(struct A) ? 1 : -1];

// SLATE-FILECHECK-STD DEFAULT gnu89
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
// DEFAULT-NEXT:     type @type0 A = struct {
// DEFAULT-NEXT:         field0 a: i8;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type1 B = struct {
// DEFAULT-NEXT:         field0 b: i8;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type2 C = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type3 D = struct {
// DEFAULT-NEXT:         field0 d: i8;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type4 E = struct {
// DEFAULT-NEXT:         field0 <anonymous>: @type5;
// DEFAULT-NEXT:         field1 e: i8;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     type @type5 = struct {
// DEFAULT-NEXT:         field0 z: i8;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type6 typedef_A = @type0;
// DEFAULT-NEXT:     type @type7 F = struct {
// DEFAULT-NEXT:         field0 f: i8;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type8 G = struct {
// DEFAULT-NEXT:         field0 <anonymous>: @type9;
// DEFAULT-NEXT:         field1 g: i8;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     type @type9 = struct {
// DEFAULT-NEXT:         field0 z: i8;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type10 H = struct {
// DEFAULT-NEXT:         field0 <anonymous>: @type11;
// DEFAULT-NEXT:         field1 h: i8;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     type @type11 = struct {
// DEFAULT-NEXT:         field0 z: i8;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     global %2 testB: array<i8, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 testC: array<i8, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 testD: array<i8, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %9 testE: array<i8, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %12 testF: array<i8, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %15 testG: array<i8, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %18 testH: array<i8, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
