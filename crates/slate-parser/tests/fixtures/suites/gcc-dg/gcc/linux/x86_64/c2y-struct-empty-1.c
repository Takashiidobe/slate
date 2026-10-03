/* Test structures and unions without named members: implementation-defined in
   C2y, undefined behavior previously.  GCC has an extension here, but does not
   allow it in pedantic mode.  */
/* { dg-do compile } */
/* { dg-options "-std=c2y -pedantic-errors" } */

struct s1 { }; /* { dg-error "struct has no members" } */
union u1 { }; /* { dg-error "union has no members" } */
struct s2 { struct { }; }; /* { dg-error "struct has no members" } */
struct s3 { int : 3; int : 4; }; /* { dg-error "struct has no named members" } */

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
// DEFAULT-NEXT:     type @type[[TYPE_s1:[0-9]+]] s1 = struct {
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[]];
// DEFAULT-NEXT:     type @type[[TYPE_u1:[0-9]+]] u1 = union {
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[]];
// DEFAULT-NEXT:     type @type[[TYPE_s2:[0-9]+]] s2 = struct {
// DEFAULT-NEXT:         field0 <anonymous>: @type[[TYPE0:[0-9]+]];
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE0]] = struct {
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[]];
// DEFAULT-NEXT:     type @type[[TYPE_s3:[0-9]+]] s3 = struct {
// DEFAULT-NEXT:         field0 <anonymous>: i32 : 3;
// DEFAULT-NEXT:         field1 <anonymous>: i32 : 4;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0], bit_offsets=[Some(0), Some(3)], bit_units=[(0, 1)], field_units=[Some(0), Some(0)]];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
