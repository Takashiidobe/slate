// SLATE-FILECHECK-DEFINES TARGET
// SLATE-FILECHECK-ARGS --dump-ir-types

#if __SIZEOF_POINTER__ != 4 || __SIZEOF_LONG__ != 4 || __SIZEOF_LONG_DOUBLE__ != 8 || __SIZEOF_WCHAR_T__ != 2
#error wrong Win32 data model
#endif

_Static_assert(_Alignof(long long) == 8 && _Alignof(double) == 8, "Win32 aligns 64-bit scalars to 8");
_Static_assert(sizeof(long double) == 8 && _Alignof(long double) == 8, "long double is double");
_Static_assert(sizeof(void *) == 4 && _Alignof(void *) == 4, "32-bit pointers");
_Static_assert((__WCHAR_TYPE__)-1 > 0 && sizeof(__WCHAR_TYPE__) == 2, "wchar_t is unsigned short");

struct Scalars { char first; long long wide; double real; };
struct Pointers { char first; int *pointer; char last; };
struct Arrays { char first; double values[2]; char last; };
union Choice { long double real; char bytes[3]; };
struct Aligned { _Alignas(8) int value; };
struct NestedAligned { char first; struct Aligned inner; };

_Static_assert(sizeof(struct Scalars) == 24, "clang: 24 on Win32, 20 on i386 linux");

// SLATE-FILECHECK-BEGIN TARGET
// TARGET: module {
// TARGET-NEXT:     target "i686-pc-windows-msvc" {
// TARGET-NEXT:         endian = little;
// TARGET-NEXT:         pointer [size=4, align=4];
// TARGET-NEXT:         stack_alignment = 4;
// TARGET-NEXT:         long_double = f64;
// TARGET-NEXT:         storage bool [size=1, align=1];
// TARGET-NEXT:         storage i8, u8 [size=1, align=1];
// TARGET-NEXT:         storage i16, u16 [size=2, align=2];
// TARGET-NEXT:         storage i32, u32 [size=4, align=4];
// TARGET-NEXT:         storage i64, u64 [size=8, align=8];
// TARGET-NEXT:         storage i128, u128 [size=16, align=16];
// TARGET-NEXT:         storage bf16 [size=2, align=2];
// TARGET-NEXT:         storage f16 [size=2, align=2];
// TARGET-NEXT:         storage f32 [size=4, align=4];
// TARGET-NEXT:         storage f64 [size=8, align=8];
// TARGET-NEXT:         storage f128 [size=16, align=16];
// TARGET-NEXT:         storage d32 [size=4, align=4];
// TARGET-NEXT:         storage d64 [size=8, align=8];
// TARGET-NEXT:         storage d128 [size=16, align=16];
// TARGET-NEXT:     }
// TARGET-NEXT:     type @type0 Scalars = struct {
// TARGET-NEXT:         field0 first: i8;
// TARGET-NEXT:         field1 wide: i64;
// TARGET-NEXT:         field2 real: f64;
// TARGET-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// TARGET-NEXT:     type @type1 Pointers = struct {
// TARGET-NEXT:         field0 first: i8;
// TARGET-NEXT:         field1 pointer: ptr<i32>;
// TARGET-NEXT:         field2 last: i8;
// TARGET-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// TARGET-NEXT:     type @type2 Arrays = struct {
// TARGET-NEXT:         field0 first: i8;
// TARGET-NEXT:         field1 values: array<f64, 2>;
// TARGET-NEXT:         field2 last: i8;
// TARGET-NEXT:     } [size=32, align=8, offsets=[0, 8, 24]];
// TARGET-NEXT:     type @type3 Choice = union {
// TARGET-NEXT:         field0 real: f64;
// TARGET-NEXT:         field1 bytes: array<i8, 3>;
// TARGET-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// TARGET-NEXT:     type @type4 Aligned = struct {
// TARGET-NEXT:         field0 value: i32;
// TARGET-NEXT:     } [size=8, align=8, offsets=[0]];
// TARGET-NEXT:     type @type5 NestedAligned = struct {
// TARGET-NEXT:         field0 first: i8;
// TARGET-NEXT:         field1 inner: @type4;
// TARGET-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// TARGET-NEXT: }
// SLATE-FILECHECK-END TARGET
