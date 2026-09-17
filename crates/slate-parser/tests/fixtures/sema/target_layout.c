unsigned long object_size(void) {
  return sizeof(long double);
}

unsigned long object_alignment(void) {
  return _Alignof(long double);
}

unsigned long integer_size(void) {
  return sizeof(unsigned long);
}

long double one(void) {
  return 1.0L;
}

// SLATE-FILECHECK-DEFINES CHECK
// SLATE-FILECHECK-ARGS --dump-ir --show-metadata

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
// CHECK-NEXT:     fn %0 @object_size() -> u64 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="unsigned long"] [c="unsigned long(void)"] {
// CHECK-NEXT:         return const<u64>(16) [size_of="f80"];
// CHECK-NEXT:     }
// CHECK-NEXT:     fn %1 @object_alignment() -> u64 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="unsigned long"] [c="unsigned long(void)"] {
// CHECK-NEXT:         return const<u64>(16) [align_of="f80"];
// CHECK-NEXT:     }
// CHECK-NEXT:     fn %2 @integer_size() -> u64 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="unsigned long"] [c="unsigned long(void)"] {
// CHECK-NEXT:         return const<u64>(8) [size_of="u64"];
// CHECK-NEXT:     }
// CHECK-NEXT:     fn %3 @one() -> f80 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="long double"] [c="long double(void)"] {
// CHECK-NEXT:         return const<f80>(1);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// SLATE-FILECHECK-END CHECK
