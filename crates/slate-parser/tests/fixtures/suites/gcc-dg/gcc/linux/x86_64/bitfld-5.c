/* { dg-options "" } */
#include <limits.h>

struct s
{
  int i1 : sizeof (int) * CHAR_BIT;
  int i2 : sizeof (int) * CHAR_BIT;
  int i3 : sizeof (int) * CHAR_BIT;
  int i4 : sizeof (int) * CHAR_BIT;
};

int f[sizeof (struct s) != sizeof (int) * 4 ? -1 : 1];

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
// DEFAULT-NEXT:     type @type[[TYPE_s:[0-9]+]] s = struct {
// DEFAULT-NEXT:         field0 i1: i32 : 32;
// DEFAULT-NEXT:         field1 i2: i32 : 32;
// DEFAULT-NEXT:         field2 i3: i32 : 32;
// DEFAULT-NEXT:         field3 i4: i32 : 32;
// DEFAULT-NEXT:     } [size=16, align=4, offsets=[0, 4, 8, 12], bit_offsets=[Some(0), Some(32), Some(64), Some(96)], bit_units=[(0, 16)], field_units=[Some(0), Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: array<i32, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
