/* { dg-options "" } */
#include <limits.h>

struct s
{
  int i1 : sizeof (int) * CHAR_BIT;
  int i2 : sizeof (int) * CHAR_BIT;
  int i3 : sizeof (int) * CHAR_BIT;
  int i4 : sizeof (int) * CHAR_BIT;
  int i5 : sizeof (int) * CHAR_BIT;
  int i6 : sizeof (int) * CHAR_BIT;
  int i7 : sizeof (int) * CHAR_BIT;
  int i8 : sizeof (int) * CHAR_BIT;
};

int f[sizeof (struct s) != sizeof (int) * 8 ? -1 : 1];

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
// DEFAULT-NEXT:     type @type0 s = struct {
// DEFAULT-NEXT:         field0 i1: i32 : 32;
// DEFAULT-NEXT:         field1 i2: i32 : 32;
// DEFAULT-NEXT:         field2 i3: i32 : 32;
// DEFAULT-NEXT:         field3 i4: i32 : 32;
// DEFAULT-NEXT:         field4 i5: i32 : 32;
// DEFAULT-NEXT:         field5 i6: i32 : 32;
// DEFAULT-NEXT:         field6 i7: i32 : 32;
// DEFAULT-NEXT:         field7 i8: i32 : 32;
// DEFAULT-NEXT:     } [size=32, align=4, offsets=[0, 4, 8, 12, 16, 20, 24, 28], bit_offsets=[Some(0), Some(32), Some(64), Some(96), Some(128), Some(160), Some(192), Some(224)], bit_units=[(0, 32)], field_units=[Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     global %1 f: array<i32, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
