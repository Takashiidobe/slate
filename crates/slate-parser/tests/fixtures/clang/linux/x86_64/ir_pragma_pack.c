// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-STD IR c23
#pragma pack(1)
#pragma pack(3)
struct AfterIgnoredPack { char a; int b; };
#pragma pack(0)
struct AfterZeroPack { char a; int b; };

#pragma pack(push, 1)
struct FieldAligned { char a; int b __attribute__((aligned(4))); char c; };
struct __attribute__((aligned(16))) PackedButAligned { char a; int b; char c; };
#pragma pack(pop)

#pragma pack(push, lbl, 2)
struct NamedPushed { char a; int b; };
#pragma pack(pop, lbl)
struct AfterNamedPop { char a; int b; };
#pragma pack(pop)

#pragma pack(2)
struct FieldPacked { char a; int b __attribute__((packed)); };
struct Bitfields { char a; unsigned b : 20; unsigned c : 20; };
struct ZeroWidth { char a; unsigned : 0; char b; };
struct Straddle { unsigned a : 30; unsigned b : 20; };
union Plain { char a; int b; };
union Bits { unsigned a : 20; char c; };
#pragma pack()
struct Reset { char a; int b; };

void local(void) {
#pragma pack(push, 1)
  struct InFunction { char a; int b; } value;
#pragma pack(pop)
  (void)value;
}

struct AfterIgnoredPack v0;
struct AfterZeroPack v1;
struct FieldAligned v2;
struct PackedButAligned v3;
struct NamedPushed v4;
struct AfterNamedPop v5;
struct FieldPacked v6;
struct Bitfields v7;
struct ZeroWidth v8;
struct Straddle v9;
union Plain v10;
union Bits v11;
struct Reset v12;

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
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
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type0 AfterIgnoredPack = struct {
// IR-NEXT:         field0 a: i8;
// IR-NEXT:         field1 b: i32;
// IR-NEXT:     } [size=5, align=1, offsets=[0, 1]];
// IR-NEXT:     type @type1 AfterZeroPack = struct {
// IR-NEXT:         field0 a: i8;
// IR-NEXT:         field1 b: i32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type2 FieldAligned = struct {
// IR-NEXT:         field0 a: i8;
// IR-NEXT:         field1 b: i32;
// IR-NEXT:         field2 c: i8;
// IR-NEXT:     } [size=6, align=1, offsets=[0, 1, 5]];
// IR-NEXT:     type @type3 PackedButAligned = struct {
// IR-NEXT:         field0 a: i8;
// IR-NEXT:         field1 b: i32;
// IR-NEXT:         field2 c: i8;
// IR-NEXT:     } [size=16, align=16, offsets=[0, 1, 5]];
// IR-NEXT:     type @type4 NamedPushed = struct {
// IR-NEXT:         field0 a: i8;
// IR-NEXT:         field1 b: i32;
// IR-NEXT:     } [size=6, align=2, offsets=[0, 2]];
// IR-NEXT:     type @type5 AfterNamedPop = struct {
// IR-NEXT:         field0 a: i8;
// IR-NEXT:         field1 b: i32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type6 FieldPacked = struct {
// IR-NEXT:         field0 a: i8;
// IR-NEXT:         field1 b: i32;
// IR-NEXT:     } [size=5, align=1, offsets=[0, 1]];
// IR-NEXT:     type @type7 Bitfields = struct {
// IR-NEXT:         field0 a: i8;
// IR-NEXT:         field1 b: u32 : 20;
// IR-NEXT:         field2 c: u32 : 20;
// IR-NEXT:     } [size=6, align=2, offsets=[0, 1, 3], bit_offsets=[None, Some(8), Some(28)], bit_units=[(1, 5)], field_units=[None, Some(0), Some(0)]];
// IR-NEXT:     type @type8 ZeroWidth = struct {
// IR-NEXT:         field0 a: i8;
// IR-NEXT:         field1 <anonymous>: u32 : 0;
// IR-NEXT:         field2 b: i8;
// IR-NEXT:     } [size=5, align=1, offsets=[0, 4, 4], bit_offsets=[None, Some(32), None]];
// IR-NEXT:     type @type9 Straddle = struct {
// IR-NEXT:         field0 a: u32 : 30;
// IR-NEXT:         field1 b: u32 : 20;
// IR-NEXT:     } [size=8, align=2, offsets=[0, 3], bit_offsets=[Some(0), Some(30)], bit_units=[(0, 7)], field_units=[Some(0), Some(0)]];
// IR-NEXT:     type @type10 Plain = union {
// IR-NEXT:         field0 a: i8;
// IR-NEXT:         field1 b: i32;
// IR-NEXT:     } [size=4, align=2, offsets=[0, 0]];
// IR-NEXT:     type @type11 Bits = union {
// IR-NEXT:         field0 a: u32 : 20;
// IR-NEXT:         field1 c: i8;
// IR-NEXT:     } [size=4, align=2, offsets=[0, 0], bit_offsets=[Some(0), None], bit_units=[(0, 3)], field_units=[Some(0), None]];
// IR-NEXT:     type @type12 Reset = struct {
// IR-NEXT:         field0 a: i8;
// IR-NEXT:         field1 b: i32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type13 InFunction = struct {
// IR-NEXT:         field0 a: i8;
// IR-NEXT:         field1 b: i32;
// IR-NEXT:     } [size=5, align=1, offsets=[0, 1]];
// IR-NEXT:     global %16 v0: @type0 [storage=static] [linkage=external];
// IR-NEXT:     global %17 v1: @type1 [storage=static] [linkage=external];
// IR-NEXT:     global %18 v2: @type2 [storage=static] [linkage=external];
// IR-NEXT:     global %19 v3: @type3 [storage=static] [linkage=external];
// IR-NEXT:     global %20 v4: @type4 [storage=static] [linkage=external];
// IR-NEXT:     global %21 v5: @type5 [storage=static] [linkage=external];
// IR-NEXT:     global %22 v6: @type6 [storage=static] [linkage=external];
// IR-NEXT:     global %23 v7: @type7 [storage=static] [linkage=external];
// IR-NEXT:     global %24 v8: @type8 [storage=static] [linkage=external];
// IR-NEXT:     global %25 v9: @type9 [storage=static] [linkage=external];
// IR-NEXT:     global %26 v10: @type10 [storage=static] [linkage=external];
// IR-NEXT:     global %27 v11: @type11 [storage=static] [linkage=external];
// IR-NEXT:     global %28 v12: @type12 [storage=static] [linkage=external];
// IR-NEXT:     fn %13 @local() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         let %15 value: @type13 [storage=automatic];
// IR-NEXT:         read<@type13>(%15);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
