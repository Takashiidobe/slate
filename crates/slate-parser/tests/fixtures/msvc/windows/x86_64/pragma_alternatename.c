// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES CHECK
int missing_defined(void);
int missing_other(int);
int present(void);
int fallback_defined(void) { return 7; }
int present(void) { return 1; }
#pragma comment(linker, "/alternatename:missing_defined=fallback_defined /alternatename:missing_other=elsewhere")
#pragma comment(linker, "/alternatename:present=fallback_defined")
#pragma comment(linker, "/include:keep_me")

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
// CHECK-NEXT:     fn %[[VALUE_missing_defined:[0-9]+]] @missing_defined() -> i32 [linkage=external] [fallback="fallback_defined"];
// CHECK-NEXT:     fn %[[VALUE_missing_other:[0-9]+]] @missing_other(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external] [fallback="elsewhere"];
// CHECK-NEXT:     fn %[[VALUE_present:[0-9]+]] @present() -> i32 [linkage=external] [fallback="fallback_defined"] [fallthrough=ub_if_used] {
// CHECK-NEXT:         return const<i32>(1);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn %[[VALUE_fallback_defined:[0-9]+]] @fallback_defined() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:         return const<i32>(7);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// SLATE-FILECHECK-END CHECK
