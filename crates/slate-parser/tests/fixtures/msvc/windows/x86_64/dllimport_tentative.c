// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES CHECK
__declspec(dllimport) unsigned short imported_tentative;
__declspec(dllimport) extern int imported_extern;
__declspec(dllexport) int exported_tentative;
int read_all(void) {
  return imported_tentative + imported_extern + exported_tentative;
}

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
// CHECK-NEXT:     extern %[[VALUE_imported_tentative:[0-9]+]] imported_tentative: u16 [storage=static] [linkage=external] [dllimport];
// CHECK-NEXT:     extern %[[VALUE_imported_extern:[0-9]+]] imported_extern: i32 [storage=static] [linkage=external] [dllimport];
// CHECK-NEXT:     global %[[VALUE_exported_tentative:[0-9]+]] exported_tentative: i32 [storage=static] [linkage=external] [dllexport];
// CHECK-NEXT:     fn %[[VALUE_read_all:[0-9]+]] @read_all() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:         return add<i32>(add<i32>(reinterpret<i32>(widen<u32>(read<u16>(%[[VALUE_imported_tentative]]))), read<i32>(%[[VALUE_imported_extern]])), read<i32>(%[[VALUE_exported_tentative]]));
// CHECK-NEXT:     }
// CHECK-NEXT: }
// SLATE-FILECHECK-END CHECK
