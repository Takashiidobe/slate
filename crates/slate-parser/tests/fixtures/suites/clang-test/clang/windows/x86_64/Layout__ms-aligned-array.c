
// Before PR45420, we would only find the alignment on this record. Afterwards,
// we can see the alignment on the typedef through the array type.
// FIXME: What about other type sugar, like _Atomic? This would only matter in a
// packed struct context.
struct __declspec(align(16)) AlignedStruct { int x; };
struct Struct2 {
  char c[16];
};
typedef struct Struct2 __declspec(align(16)) AlignedStruct2;

#define CHECK_SIZE(X, Align) \
  _Static_assert(__alignof(struct X) == Align, "should be aligned");

#pragma pack(push, 2)

struct A {
  struct AlignedStruct a[1];
};
CHECK_SIZE(A, 16);

struct B {
  char b;
  AlignedStruct2 a[1];
};
CHECK_SIZE(B, 16);

struct C {
  char b;
  AlignedStruct2 a[];
};
CHECK_SIZE(C, 16);


#pragma pack(pop)

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT gnu17

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f64;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     type @type[[TYPE_AlignedStruct:[0-9]+]] AlignedStruct = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:     } [size=16, align=16, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_Struct2:[0-9]+]] Struct2 = struct {
// DEFAULT-NEXT:         field0 c: array<i8, 16>;
// DEFAULT-NEXT:     } [size=16, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_AlignedStruct2:[0-9]+]] AlignedStruct2 = @type[[TYPE_Struct2]];
// DEFAULT-NEXT:     type @type[[TYPE_A:[0-9]+]] A = struct {
// DEFAULT-NEXT:         field0 a: array<@type[[TYPE_AlignedStruct]], 1>;
// DEFAULT-NEXT:     } [size=16, align=16, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_B:[0-9]+]] B = struct {
// DEFAULT-NEXT:         field0 b: i8;
// DEFAULT-NEXT:         field1 a: array<@type[[TYPE_Struct2]], 1>;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 16]];
// DEFAULT-NEXT:     type @type[[TYPE_C:[0-9]+]] C = struct {
// DEFAULT-NEXT:         field0 b: i8;
// DEFAULT-NEXT:         field1 a: array<@type[[TYPE_Struct2]], incomplete>;
// DEFAULT-NEXT:     } [size=16, align=16, offsets=[0, 16]];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
