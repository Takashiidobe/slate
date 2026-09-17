int one(void) {
  return 1;
}

// SLATE-FILECHECK-DEFINES CHECK
// SLATE-FILECHECK-FLAVOR gcc
// SLATE-FILECHECK-ARGS --dump-ir -mpreferred-stack-boundary=4

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
// CHECK-NEXT:         storage f16 [size=2, align=2];
// CHECK-NEXT:         storage f32 [size=4, align=4];
// CHECK-NEXT:         storage f64 [size=8, align=8];
// CHECK-NEXT:         storage f80 [size=16, align=16];
// CHECK-NEXT:         storage f128 [size=16, align=16];
// CHECK-NEXT:     }
// CHECK-NEXT:     fn %0 @one() -> i32 [linkage=external] {
// CHECK-NEXT:         return const<i32>(1);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// SLATE-FILECHECK-END CHECK
