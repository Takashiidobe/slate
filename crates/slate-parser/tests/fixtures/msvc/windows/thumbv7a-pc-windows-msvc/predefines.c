// SLATE-FILECHECK-DEFINES CHECK
// SLATE-FILECHECK-ARGS --dump-ir-types

#if !defined(_WIN32) || defined(_WIN64) || _MSC_VER != 1944 || _M_ARM != 7 || _M_ARM_FP != 31 || _M_THUMB != 7 || !defined(_M_ARM_NT)
#error missing native MSVC Windows on Arm32 target predefines
#endif
#if defined(__linux__) || defined(__clang__) || defined(__ARM_ARCH) || defined(__LDBL_MANT_DIG__) || defined(_M_ARM64)
#error foreign predefines on native MSVC target
#endif

struct Scalars { long value; long double real; int *pointer; };

// SLATE-FILECHECK-BEGIN CHECK
// CHECK: module {
// CHECK-NEXT:     target "thumbv7a-pc-windows-msvc" {
// CHECK-NEXT:         endian = little;
// CHECK-NEXT:         pointer [size=4, align=4];
// CHECK-NEXT:         stack_alignment = 8;
// CHECK-NEXT:         long_double = f64;
// CHECK-NEXT:         storage bool [size=1, align=1];
// CHECK-NEXT:         storage i8, u8 [size=1, align=1];
// CHECK-NEXT:         storage i16, u16 [size=2, align=2];
// CHECK-NEXT:         storage i32, u32 [size=4, align=4];
// CHECK-NEXT:         storage i64, u64 [size=8, align=8];
// CHECK-NEXT:         storage bf16 [size=2, align=2];
// CHECK-NEXT:         storage f16 [size=2, align=2];
// CHECK-NEXT:         storage f32 [size=4, align=4];
// CHECK-NEXT:         storage f64 [size=8, align=8];
// CHECK-NEXT:         storage f128 [size=16, align=16];
// CHECK-NEXT:         storage d32 [size=4, align=4];
// CHECK-NEXT:         storage d64 [size=8, align=8];
// CHECK-NEXT:         storage d128 [size=16, align=16];
// CHECK-NEXT:     }
// CHECK-NEXT:     type @type0 Scalars = struct {
// CHECK-NEXT:         field0 value: i32;
// CHECK-NEXT:         field1 real: f64;
// CHECK-NEXT:         field2 pointer: ptr<i32>;
// CHECK-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// CHECK-NEXT: }
// SLATE-FILECHECK-END CHECK
