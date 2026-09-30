/* { dg-options "-std=iso9899:1990 -pedantic" } */
/* In strict ISO C mode, we don't recognize the anonymous struct/union
   extension or any Microsoft extensions.  */

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
  struct { char z; };		/* { dg-warning "unnamed structs" } */
  char e;
};


/* MS extension.  */
typedef struct A typedef_A;
struct F {
  typedef_A;			/* { dg-warning "does not declare anything" } */
  char f;
};
char testF[sizeof(struct F) == sizeof(struct A) ? 1 : -1];

/* __extension__ enables GNU C mode for the duration of the declaration.  */
__extension__ struct G {
  struct { char z; };
  char g;
};
char testG[sizeof(struct G) == 2 * sizeof(struct A) ? 1 : -1];

struct H {
  __extension__ struct { char z; };
  char h;
};
char testH[sizeof(struct H) == 2 * sizeof(struct A) ? 1 : -1];

/* Make sure __extension__ gets turned back off.  */
struct I {
  struct { char z; };		/* { dg-warning "unnamed structs" } */
  char i;
};
char testI[sizeof(struct I) == sizeof(struct E) ? 1 : -1];

// SLATE-FILECHECK-STD DEFAULT iso9899:1990
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
// DEFAULT-NEXT:     type @type[[TYPE_A:[0-9]+]] A = struct {
// DEFAULT-NEXT:         field0 a: i8;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_B:[0-9]+]] B = struct {
// DEFAULT-NEXT:         field0 b: i8;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_C:[0-9]+]] C = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_D:[0-9]+]] D = struct {
// DEFAULT-NEXT:         field0 d: i8;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_E:[0-9]+]] E = struct {
// DEFAULT-NEXT:         field0 <anonymous>: @type[[TYPE0:[0-9]+]];
// DEFAULT-NEXT:         field1 e: i8;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     type @type[[TYPE0]] = struct {
// DEFAULT-NEXT:         field0 z: i8;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_typedef_A:[0-9]+]] typedef_A = @type[[TYPE_A]];
// DEFAULT-NEXT:     type @type[[TYPE_F:[0-9]+]] F = struct {
// DEFAULT-NEXT:         field0 f: i8;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_G:[0-9]+]] G = struct {
// DEFAULT-NEXT:         field0 <anonymous>: @type[[TYPE1:[0-9]+]];
// DEFAULT-NEXT:         field1 g: i8;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     type @type[[TYPE1]] = struct {
// DEFAULT-NEXT:         field0 z: i8;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_H:[0-9]+]] H = struct {
// DEFAULT-NEXT:         field0 <anonymous>: @type[[TYPE2:[0-9]+]];
// DEFAULT-NEXT:         field1 h: i8;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     type @type[[TYPE2]] = struct {
// DEFAULT-NEXT:         field0 z: i8;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_I:[0-9]+]] I = struct {
// DEFAULT-NEXT:         field0 <anonymous>: @type[[TYPE3:[0-9]+]];
// DEFAULT-NEXT:         field1 i: i8;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     type @type[[TYPE3]] = struct {
// DEFAULT-NEXT:         field0 z: i8;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_testB:[0-9]+]] testB: array<i8, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_testC:[0-9]+]] testC: array<i8, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_testD:[0-9]+]] testD: array<i8, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_testF:[0-9]+]] testF: array<i8, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_testG:[0-9]+]] testG: array<i8, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_testH:[0-9]+]] testH: array<i8, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_testI:[0-9]+]] testI: array<i8, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
