// SLATE-FILECHECK-DEFINES CHECK
// SLATE-FILECHECK-ARGS --dump-ir-types

#if !defined(_WIN32) || defined(_WIN64) || _MSC_VER != 1933 || _M_ARM != 7 || _M_ARM_FP != 31 || !defined(_M_ARM_NT) || !defined(__clang__)
#error missing Clang Windows on Arm32 target predefines
#endif
#if _M_THUMB != 7 || _M_ARMT != 7 || !defined(__thumb2__) || __ARM_ARCH_ISA_THUMB != 2
#error Windows on Arm32 is Thumb-2 only
#endif
#if !defined(__ARM_PCS_VFP) || defined(__SOFTFP__) || __ARM_FP != 0xe || __ARM_NEON_FP != 0x6 || defined(__ARM_FEATURE_FMA)
#error default FPU is neon-fp16 with the hard float ABI
#endif
#if __SIZEOF_LONG__ != 4 || __SIZEOF_LONG_DOUBLE__ != 8 || __SIZEOF_POINTER__ != 4 || __SIZEOF_WCHAR_T__ != 2 || defined(__CHAR_UNSIGNED__)
#error wrong Windows on Arm32 data model
#endif
#if defined(__linux__) || defined(__ARM_EABI__) || defined(__ELF__) || defined(__SLATE_LIBC_GLIBC) || defined(_M_ARM64)
#error foreign predefines on Windows on Arm32
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
