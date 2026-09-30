
struct EmptyIntMemb {
  int FlexArrayMemb[0];
};

struct EmptyLongLongMemb {
  long long FlexArrayMemb[0];
};

struct EmptyAligned2LongLongMemb {
  long long __declspec(align(2)) FlexArrayMemb[0];
};


struct EmptyAligned8LongLongMemb {
  long long __declspec(align(8)) FlexArrayMemb[0];
};


#pragma pack(1)
struct __declspec(align(4)) EmptyPackedAligned4LongLongMemb {
  long long FlexArrayMemb[0];
};
#pragma pack()


#pragma pack(1)
struct EmptyPackedAligned8LongLongMemb {
  long long __declspec(align(8)) FlexArrayMemb[0];
};
#pragma pack()



int a[
sizeof(struct EmptyIntMemb)+
sizeof(struct EmptyLongLongMemb)+
sizeof(struct EmptyAligned2LongLongMemb)+
sizeof(struct EmptyAligned8LongLongMemb)+
sizeof(struct EmptyPackedAligned4LongLongMemb)+
sizeof(struct EmptyPackedAligned8LongLongMemb)+
0];

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT gnu17

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "i686-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=4, align=4];
// DEFAULT-NEXT:         stack_alignment = 4;
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
// DEFAULT-NEXT:     type @type[[TYPE_EmptyIntMemb:[0-9]+]] EmptyIntMemb = struct {
// DEFAULT-NEXT:         field0 FlexArrayMemb: array<i32, 0>;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_EmptyLongLongMemb:[0-9]+]] EmptyLongLongMemb = struct {
// DEFAULT-NEXT:         field0 FlexArrayMemb: array<i64, 0>;
// DEFAULT-NEXT:     } [size=4, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_EmptyAligned2LongLongMemb:[0-9]+]] EmptyAligned2LongLongMemb = struct {
// DEFAULT-NEXT:         field0 FlexArrayMemb: array<i64, 0>;
// DEFAULT-NEXT:     } [size=4, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_EmptyAligned8LongLongMemb:[0-9]+]] EmptyAligned8LongLongMemb = struct {
// DEFAULT-NEXT:         field0 FlexArrayMemb: array<i64, 0>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_EmptyPackedAligned4LongLongMemb:[0-9]+]] EmptyPackedAligned4LongLongMemb = struct {
// DEFAULT-NEXT:         field0 FlexArrayMemb: array<i64, 0>;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_EmptyPackedAligned8LongLongMemb:[0-9]+]] EmptyPackedAligned8LongLongMemb = struct {
// DEFAULT-NEXT:         field0 FlexArrayMemb: array<i64, 0>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: array<i32, 32> [storage=static] [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
