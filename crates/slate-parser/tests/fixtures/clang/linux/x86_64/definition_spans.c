// SLATE-FILECHECK-DEFINES CHECK
// SLATE-FILECHECK-ARGS --dump-ir --show-spans

int prototyped(void);
extern int declared;
int tentative;

int prototyped(void) { return declared + tentative; }
int declared = 1;
int tentative = 2;

int defined_first(void) { return 3; }
int defined_first(void);

// SLATE-FILECHECK-BEGIN CHECK
// CHECK: module {
// CHECK-NEXT:     target "x86_64-unknown-linux-gnu" {
// CHECK-NEXT:         endian = little;
// CHECK-NEXT:         pointer [size=8, align=8];
// CHECK-NEXT:         stack_alignment = 16;
// CHECK-NEXT:         long_double = f80;
// CHECK-NEXT:         storage bool [size=1, align=1];
// CHECK-NEXT:         storage i8, u8 [size=1, align=1];
// CHECK-NEXT:         storage i16, u16 [size=2, align=2];
// CHECK-NEXT:         storage i32, u32 [size=4, align=4];
// CHECK-NEXT:         storage i64, u64 [size=8, align=8];
// CHECK-NEXT:         storage i128, u128 [size=16, align=16];
// CHECK-NEXT:         storage bf16 [size=2, align=2];
// CHECK-NEXT:         storage f16 [size=2, align=2];
// CHECK-NEXT:         storage f32 [size=4, align=4];
// CHECK-NEXT:         storage f64 [size=8, align=8];
// CHECK-NEXT:         storage f80 [size=16, align=16];
// CHECK-NEXT:         storage f128 [size=16, align=16];
// CHECK-NEXT:         storage d32 [size=4, align=4];
// CHECK-NEXT:         storage d64 [size=8, align=8];
// CHECK-NEXT:         storage d128 [size=16, align=16];
// CHECK-NEXT:     }
// CHECK-NEXT:     global %[[VALUE_declared:[0-9]+]] declared: i32 [storage=static] = const<i32>(1) [linkage=external] [spelling=[[#FILE0:]]:118+12, expansion={{[0-9]+}}:118+12];
// CHECK-NEXT:     global %[[VALUE_tentative:[0-9]+]] tentative: i32 [storage=static] = const<i32>(2) [linkage=external] [spelling=[[#FILE0]]:136+13, expansion=[[#FILE0]]:136+13];
// CHECK-NEXT:     fn %[[VALUE_prototyped:[0-9]+]] @prototyped() -> i32 [linkage=external] [fallthrough=ub_if_used] [spelling=[[#FILE0]]:60+53, expansion=[[#FILE0]]:60+53] {
// CHECK-NEXT:         return add<i32, overflow=ub>(read<i32>(%[[VALUE_declared]]), read<i32>(%[[VALUE_tentative]]));
// CHECK-NEXT:     }
// CHECK-NEXT:     fn %[[VALUE_defined_first:[0-9]+]] @defined_first() -> i32 [linkage=external] [fallthrough=ub_if_used] [spelling=[[#FILE0]]:152+37, expansion=[[#FILE0]]:152+37] {
// CHECK-NEXT:         return const<i32>(3);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// SLATE-FILECHECK-END CHECK
