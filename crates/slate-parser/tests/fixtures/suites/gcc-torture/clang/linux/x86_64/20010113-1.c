// SLATE-FILECHECK-DEFINES DEFAULT

/* Origin: PR c/364 from and@genesyslab.com, very much reduced to a
   testcase by Joseph Myers <jsm28@cam.ac.uk>.

   The initializer of z is a valid address constant, and GCC 2.95.2
   accepts it as such.  CVS GCC as of 2001-01-13 rejects it, but accepts
   it if y is changed to x in the initializer.  */

struct {
  struct {
    int x;
    int y;
  } p;
} v;

int *z = &((&(v.p))->y);

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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 p: @type[[TYPE1:[0-9]+]];
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE1]] = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 y: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     global %[[VALUE_v:[0-9]+]] v: @type[[TYPE0]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_z:[0-9]+]] z: ptr<i32> [storage=static] = addr_of<ptr<i32>>(field1(deref(addr_of<ptr<@type[[TYPE1]]>>(field0(%[[VALUE_v]]))))) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
