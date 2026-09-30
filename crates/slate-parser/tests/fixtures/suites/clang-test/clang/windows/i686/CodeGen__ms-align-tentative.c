
char __declspec(align(8192)) x;

typedef char __declspec(align(8192)) T;
T y;

T __declspec(align(8192)) z;

int __declspec(align(16)) redef;
int __declspec(align(32)) redef = 8;

struct __declspec(align(64)) S {
  char fd;
} s;

struct Wrap {
  struct S x;
} w;

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT gnu17

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "i686-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=4, align=4];
// DEFAULT-NEXT:         stack_alignment = 4;
// DEFAULT-NEXT:         long_double = f64;
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
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     type @type[[TYPE_T:[0-9]+]] T = i8;
// DEFAULT-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// DEFAULT-NEXT:         field0 fd: i8;
// DEFAULT-NEXT:     } [size=64, align=64, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_Wrap:[0-9]+]] Wrap = struct {
// DEFAULT-NEXT:         field0 x: @type[[TYPE_S]];
// DEFAULT-NEXT:     } [size=64, align=64, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_x:[0-9]+]] x: i8 [storage=static] [align=8192] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_y:[0-9]+]] y: i8 [storage=static] [align=8192] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_z:[0-9]+]] z: i8 [storage=static] [align=8192] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_redef:[0-9]+]] redef: i32 [storage=static] [align=32] = const<i32>(8) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_s:[0-9]+]] s: @type[[TYPE_S]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_w:[0-9]+]] w: @type[[TYPE_Wrap]] [storage=static] [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
