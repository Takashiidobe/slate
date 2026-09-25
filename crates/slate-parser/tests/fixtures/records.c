struct Point {
  int x;
  int y;
};

union Value {
  int i;
  char c;
};

struct {
  int z;
};

enum Color {
  RED,
  GREEN = 3,
  BLUE,
};

struct Forward;
struct Point point;
enum Color color;
typedef struct Point PointAlias;
PointAlias alias;

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
// DEFAULT-NEXT:     type @type0 Point = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 y: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type1 Value = union {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:         field1 c: i8;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type2 = struct {
// DEFAULT-NEXT:         field0 z: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type3 Color = enum : u32 {
// DEFAULT-NEXT:         %0 RED = const<i32>(0);
// DEFAULT-NEXT:         %1 GREEN = const<i32>(3);
// DEFAULT-NEXT:         %2 BLUE = const<i32>(4);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type4 Forward = struct incomplete;
// DEFAULT-NEXT:     type @type5 PointAlias = @type0;
// DEFAULT-NEXT:     global %8 point: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %9 color: @type3 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %11 alias: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
