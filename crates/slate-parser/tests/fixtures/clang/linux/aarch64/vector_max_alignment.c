// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

typedef unsigned char v8u8 __attribute__((vector_size(8)));
typedef unsigned char v32u8 __attribute__((vector_size(32)));
typedef float v64f __attribute__((vector_size(256)));

struct holder {
  char tag;
  v32u8 lanes;
};

int sizes[] = {sizeof(v8u8), _Alignof(v8u8), sizeof(v32u8), _Alignof(v32u8),
               sizeof(v64f), _Alignof(v64f), sizeof(struct holder), _Alignof(struct holder)};

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "aarch64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f128;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type[[TYPE_v8u8:[0-9]+]] v8u8 = vector<u8, 8>;
// IR-NEXT:     type @type[[TYPE_v32u8:[0-9]+]] v32u8 = vector<u8, 32>;
// IR-NEXT:     type @type[[TYPE_v64f:[0-9]+]] v64f = vector<f32, 64>;
// IR-NEXT:     type @type[[TYPE_holder:[0-9]+]] holder = struct {
// IR-NEXT:         field0 tag: u8;
// IR-NEXT:         field1 lanes: vector<u8, 32>;
// IR-NEXT:     } [size=48, align=16, offsets=[0, 16]];
// IR-NEXT:     global %[[VALUE_sizes:[0-9]+]] sizes: array<i32, 8> [storage=static] = aggregate<array<i32, 8>, zero_fill=false>(index0 = reinterpret<i32>(truncate<u32>(const<u64>(8))), index1 = reinterpret<i32>(truncate<u32>(const<u64>(8))), index2 = reinterpret<i32>(truncate<u32>(const<u64>(32))), index3 = reinterpret<i32>(truncate<u32>(const<u64>(16))), index4 = reinterpret<i32>(truncate<u32>(const<u64>(256))), index5 = reinterpret<i32>(truncate<u32>(const<u64>(16))), index6 = reinterpret<i32>(truncate<u32>(const<u64>(48))), index7 = reinterpret<i32>(truncate<u32>(const<u64>(16)))) [linkage=external];
// IR-NEXT: }
// SLATE-FILECHECK-END IR
