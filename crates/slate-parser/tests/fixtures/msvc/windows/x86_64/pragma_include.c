// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES CHECK
int kept_fn(void) { return 7; }
int kept_global = 5;
int unkept_fn(void) { return 1; }
#pragma comment(linker, "/include:kept_fn /include:kept_global")

// SLATE-FILECHECK-BEGIN CHECK
// CHECK: module {
// CHECK-NEXT:     target "x86_64-pc-windows-msvc" {
// CHECK-NEXT:         endian = little;
// CHECK-NEXT:         pointer [size=8, align=8];
// CHECK-NEXT:         stack_alignment = 16;
// CHECK-NEXT:         long_double = f64;
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
// CHECK-NEXT:         storage f128 [size=16, align=16];
// CHECK-NEXT:         storage d32 [size=4, align=4];
// CHECK-NEXT:         storage d64 [size=8, align=8];
// CHECK-NEXT:         storage d128 [size=16, align=16];
// CHECK-NEXT:     }
// CHECK-NEXT:     global %[[VALUE_kept_global:[0-9]+]] kept_global: i32 [storage=static] = const<i32>(5) [linkage=external] [include];
// CHECK-NEXT:     fn %[[VALUE_kept_fn:[0-9]+]] @kept_fn() -> i32 [linkage=external] [include] [fallthrough=ub_if_used] {
// CHECK-NEXT:         return const<i32>(7);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn %[[VALUE_unkept_fn:[0-9]+]] @unkept_fn() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:         return const<i32>(1);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// SLATE-FILECHECK-END CHECK
