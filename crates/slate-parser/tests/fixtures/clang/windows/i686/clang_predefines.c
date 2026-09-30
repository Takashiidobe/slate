// SLATE-FILECHECK-DEFINES CHECK
// SLATE-FILECHECK-ARGS --dump-ir-types

#if !defined(_WIN32) || defined(_WIN64) || !defined(_MSC_VER) || _M_IX86 != 600 || _M_IX86_FP != 2 || !defined(__clang__)
#error missing Clang Win32 target predefines
#endif
#if !defined(__i386__) || !defined(__SSE2__) || !defined(__pentium4__)
#error missing x86 ISA predefines
#endif
#if __SIZEOF_LONG__ != 4 || __SIZEOF_LONG_DOUBLE__ != 8 || __SIZEOF_POINTER__ != 4 || __SIZEOF_WCHAR_T__ != 2
#error wrong Win32 data model
#endif
#if defined(__linux__) || defined(__SLATE_LIBC_GLIBC) || defined(_M_X64)
#error foreign predefines on Win32 target
#endif
#if !defined(i386)
#error gnu modes define i386
#endif

struct Scalars { long value; long double real; int *pointer; };

// SLATE-FILECHECK-BEGIN CHECK
// CHECK: module {
// CHECK-NEXT:     target "i686-pc-windows-msvc" {
// CHECK-NEXT:         endian = little;
// CHECK-NEXT:         pointer [size=4, align=4];
// CHECK-NEXT:         stack_alignment = 4;
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
// CHECK-NEXT:     type @type[[TYPE_Scalars:[0-9]+]] Scalars = struct {
// CHECK-NEXT:         field0 value: i32;
// CHECK-NEXT:         field1 real: f64;
// CHECK-NEXT:         field2 pointer: ptr<i32>;
// CHECK-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// CHECK-NEXT: }
// SLATE-FILECHECK-END CHECK
