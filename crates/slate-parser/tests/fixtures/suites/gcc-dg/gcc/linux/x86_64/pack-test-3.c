/* { dg-do compile } */

/* Copyright (C) 2003 Free Software Foundation, Inc.
   Contributed by Nathan Sidwell 15 Jul 2003 <nathan@codesourcery.com> */

/* you should not be able to pack a typedef to a struct, only the
   underlying struct can be packed.  */

/* ok */
struct u1
{
  char field1;
  short field2;
  int field3;
};

/* ok */
typedef struct p1 {
   char  field1;
   short field2;
   int field3;
} __attribute__ ((packed)) p1_t1;

/* ok */
typedef struct __attribute__ ((packed)) p2 {
   char  field1;
   short field2;
   int field3;
} p2_t1;

int ary1[sizeof (struct p1) == sizeof (p1_t1) ? 1 : -1];
int ary2[sizeof (struct p2) == sizeof (p2_t1) ? 1 : -1];
int ary3[sizeof (struct p1) == sizeof (struct p2) ? 1 : -1];

/* not ok */
typedef struct u1 __attribute__ ((packed)) u1_t1; /* { dg-warning "attribute ignored" }*/
typedef struct u1 u1_t2 __attribute__ ((packed)); /* { dg-warning "attribute ignored" }*/

typedef struct p3 {
   char  field1;
   short field2;
   int field3;
} p3_t1 __attribute__ ((packed)); /* { dg-warning "attribute ignored" }*/


// SLATE-FILECHECK-STD DEFAULT c89
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
// DEFAULT-NEXT:     type @type[[TYPE_u1:[0-9]+]] u1 = struct {
// DEFAULT-NEXT:         field0 field1: i8;
// DEFAULT-NEXT:         field1 field2: i16;
// DEFAULT-NEXT:         field2 field3: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 2, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_p1:[0-9]+]] p1 = struct {
// DEFAULT-NEXT:         field0 field1: i8;
// DEFAULT-NEXT:         field1 field2: i16;
// DEFAULT-NEXT:         field2 field3: i32;
// DEFAULT-NEXT:     } [size=7, align=1, offsets=[0, 1, 3]];
// DEFAULT-NEXT:     type @type[[TYPE_p1_t1:[0-9]+]] p1_t1 = @type[[TYPE_p1]];
// DEFAULT-NEXT:     type @type[[TYPE_p2:[0-9]+]] p2 = struct {
// DEFAULT-NEXT:         field0 field1: i8;
// DEFAULT-NEXT:         field1 field2: i16;
// DEFAULT-NEXT:         field2 field3: i32;
// DEFAULT-NEXT:     } [size=7, align=1, offsets=[0, 1, 3]];
// DEFAULT-NEXT:     type @type[[TYPE_p2_t1:[0-9]+]] p2_t1 = @type[[TYPE_p2]];
// DEFAULT-NEXT:     type @type[[TYPE_u1_t1:[0-9]+]] u1_t1 = @type[[TYPE_u1]];
// DEFAULT-NEXT:     type @type[[TYPE_u1_t2:[0-9]+]] u1_t2 = @type[[TYPE_u1]];
// DEFAULT-NEXT:     type @type[[TYPE_p3:[0-9]+]] p3 = struct {
// DEFAULT-NEXT:         field0 field1: i8;
// DEFAULT-NEXT:         field1 field2: i16;
// DEFAULT-NEXT:         field2 field3: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 2, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_p3_t1:[0-9]+]] p3_t1 = @type[[TYPE_p3]];
// DEFAULT-NEXT:     global %[[VALUE_ary1:[0-9]+]] ary1: array<i32, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ary2:[0-9]+]] ary2: array<i32, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ary3:[0-9]+]] ary3: array<i32, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
