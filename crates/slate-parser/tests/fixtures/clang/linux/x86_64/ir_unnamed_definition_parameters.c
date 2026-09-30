// SLATE-FILECHECK-DEFINES C23
// SLATE-FILECHECK-DEFINES GNU23
// SLATE-FILECHECK-STD C23 c23
// SLATE-FILECHECK-STD GNU23 gnu23
// SLATE-FILECHECK-ARGS --dump-ir

int first(int, int b) { return b; }
void pointer(char *, long) {}
int callback(int (*)(int), int x) { return x; }

// SLATE-FILECHECK-BEGIN C23
// C23: module {
// C23-NEXT:     target "x86_64-unknown-linux-gnu" {
// C23-NEXT:         endian = little;
// C23-NEXT:         pointer [size=8, align=8];
// C23-NEXT:         stack_alignment = 16;
// C23-NEXT:         long_double = f80;
// C23-NEXT:         storage bool [size=1, align=1];
// C23-NEXT:         storage i8, u8 [size=1, align=1];
// C23-NEXT:         storage i16, u16 [size=2, align=2];
// C23-NEXT:         storage i32, u32 [size=4, align=4];
// C23-NEXT:         storage i64, u64 [size=8, align=8];
// C23-NEXT:         storage i128, u128 [size=16, align=16];
// C23-NEXT:         storage bf16 [size=2, align=2];
// C23-NEXT:         storage f16 [size=2, align=2];
// C23-NEXT:         storage f32 [size=4, align=4];
// C23-NEXT:         storage f64 [size=8, align=8];
// C23-NEXT:         storage f80 [size=16, align=16];
// C23-NEXT:         storage f128 [size=16, align=16];
// C23-NEXT:         storage d32 [size=4, align=4];
// C23-NEXT:         storage d64 [size=8, align=8];
// C23-NEXT:         storage d128 [size=16, align=16];
// C23-NEXT:     }
// C23-NEXT:     fn %[[VALUE_first:[0-9]+]] @first(%[[VALUE0:[0-9]+]] <unnamed>: i32, %[[VALUE_b:[0-9]+]] b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// C23-NEXT:         return read<i32>(%[[VALUE_b]]);
// C23-NEXT:     }
// C23-NEXT:     fn %[[VALUE_pointer:[0-9]+]] @pointer(%[[VALUE1:[0-9]+]] <unnamed>: ptr<i8>, %[[VALUE2:[0-9]+]] <unnamed>: i64) -> void [linkage=external] [fallthrough=ret_void] {
// C23-NEXT:     }
// C23-NEXT:     fn %[[VALUE_callback:[0-9]+]] @callback(%[[VALUE3:[0-9]+]] <unnamed>: ptr<fn(i32) -> i32>, %[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// C23-NEXT:         return read<i32>(%[[VALUE_x]]);
// C23-NEXT:     }
// C23-NEXT: }
// SLATE-FILECHECK-END C23
// SLATE-FILECHECK-BEGIN GNU23
// GNU23: module {
// GNU23-NEXT:     target "x86_64-unknown-linux-gnu" {
// GNU23-NEXT:         endian = little;
// GNU23-NEXT:         pointer [size=8, align=8];
// GNU23-NEXT:         stack_alignment = 16;
// GNU23-NEXT:         long_double = f80;
// GNU23-NEXT:         storage bool [size=1, align=1];
// GNU23-NEXT:         storage i8, u8 [size=1, align=1];
// GNU23-NEXT:         storage i16, u16 [size=2, align=2];
// GNU23-NEXT:         storage i32, u32 [size=4, align=4];
// GNU23-NEXT:         storage i64, u64 [size=8, align=8];
// GNU23-NEXT:         storage i128, u128 [size=16, align=16];
// GNU23-NEXT:         storage bf16 [size=2, align=2];
// GNU23-NEXT:         storage f16 [size=2, align=2];
// GNU23-NEXT:         storage f32 [size=4, align=4];
// GNU23-NEXT:         storage f64 [size=8, align=8];
// GNU23-NEXT:         storage f80 [size=16, align=16];
// GNU23-NEXT:         storage f128 [size=16, align=16];
// GNU23-NEXT:         storage d32 [size=4, align=4];
// GNU23-NEXT:         storage d64 [size=8, align=8];
// GNU23-NEXT:         storage d128 [size=16, align=16];
// GNU23-NEXT:     }
// GNU23-NEXT:     fn %[[VALUE_first:[0-9]+]] @first(%[[VALUE0:[0-9]+]] <unnamed>: i32, %[[VALUE_b:[0-9]+]] b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// GNU23-NEXT:         return read<i32>(%[[VALUE_b]]);
// GNU23-NEXT:     }
// GNU23-NEXT:     fn %[[VALUE_pointer:[0-9]+]] @pointer(%[[VALUE1:[0-9]+]] <unnamed>: ptr<i8>, %[[VALUE2:[0-9]+]] <unnamed>: i64) -> void [linkage=external] [fallthrough=ret_void] {
// GNU23-NEXT:     }
// GNU23-NEXT:     fn %[[VALUE_callback:[0-9]+]] @callback(%[[VALUE3:[0-9]+]] <unnamed>: ptr<fn(i32) -> i32>, %[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// GNU23-NEXT:         return read<i32>(%[[VALUE_x]]);
// GNU23-NEXT:     }
// GNU23-NEXT: }
// SLATE-FILECHECK-END GNU23
