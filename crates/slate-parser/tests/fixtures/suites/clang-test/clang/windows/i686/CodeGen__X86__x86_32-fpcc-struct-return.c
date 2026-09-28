
typedef struct { int a,b,c,d; } Big;
typedef struct { int i; } Small;
typedef struct { short s; } Short;
typedef struct { } ZeroSized;

Big returnBig(Big x) { return x; }

Small returnSmall(Small x) { return x; }

Short returnShort(Short x) { return x; }

ZeroSized returnZero(ZeroSized x) { return x; }

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
// DEFAULT-NEXT:     type @type0 = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:         field2 c: i32;
// DEFAULT-NEXT:         field3 d: i32;
// DEFAULT-NEXT:     } [size=16, align=4, offsets=[0, 4, 8, 12]];
// DEFAULT-NEXT:     type @type1 Big = @type0;
// DEFAULT-NEXT:     type @type2 = struct {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type3 Small = @type2;
// DEFAULT-NEXT:     type @type4 = struct {
// DEFAULT-NEXT:         field0 s: i16;
// DEFAULT-NEXT:     } [size=2, align=2, offsets=[0]];
// DEFAULT-NEXT:     type @type5 Short = @type4;
// DEFAULT-NEXT:     type @type6 = struct {
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[]];
// DEFAULT-NEXT:     type @type7 ZeroSized = @type6;
// DEFAULT-NEXT:     fn %8 @returnBig(%9 x: @type0) -> @type0 [linkage=external] [abi=x86_win32(coerce<i32, i32, i32, i32>) -> sret<align=4>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type0, reason=return>(read<@type0>(%9));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @returnSmall(%11 x: @type2) -> @type2 [linkage=external] [abi=x86_win32(coerce<i32>) -> coerce<i32>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type2, reason=return>(read<@type2>(%11));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @returnShort(%13 x: @type4) -> @type4 [linkage=external] [abi=x86_win32(coerce<i16>) -> coerce<i16>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type4, reason=return>(read<@type4>(%13));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @returnZero(%15 x: @type6) -> @type6 [linkage=external] [abi=x86_win32(coerce<>) -> sret<align=1>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type6, reason=return>(read<@type6>(%15));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
