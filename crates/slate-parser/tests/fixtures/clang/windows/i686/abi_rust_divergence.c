// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

struct pair { int a; int b; };
struct odd_array { char a[3]; char b; };
struct bit_precise { _BitInt(24) a; float f; };
struct flexible { int n; int tail[]; };
struct zero_tail { float f; int tail[0]; };
struct aligned { int a; } __attribute__((aligned(16)));

struct pair pair(struct pair value) { return value; }
struct odd_array odd_array(struct odd_array value) { return value; }
struct bit_precise bit_precise(struct bit_precise value) { return value; }
struct flexible flexible(struct flexible value) { return value; }
struct zero_tail zero_tail(struct zero_tail value) { return value; }
struct aligned aligned(struct aligned value) { return value; }

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "i686-pc-windows-msvc" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=4, align=4];
// IR-NEXT:         stack_alignment = 4;
// IR-NEXT:         long_double = f64;
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
// IR-NEXT:     type @type0 pair = struct {
// IR-NEXT:         field0 a: i32;
// IR-NEXT:         field1 b: i32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type1 odd_array = struct {
// IR-NEXT:         field0 a: array<i8, 3>;
// IR-NEXT:         field1 b: i8;
// IR-NEXT:     } [size=4, align=1, offsets=[0, 3]];
// IR-NEXT:     type @type2 bit_precise = struct {
// IR-NEXT:         field0 a: i24b;
// IR-NEXT:         field1 f: f32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type3 flexible = struct {
// IR-NEXT:         field0 n: i32;
// IR-NEXT:         field1 tail: array<i32, incomplete>;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type4 zero_tail = struct {
// IR-NEXT:         field0 f: f32;
// IR-NEXT:         field1 tail: array<i32, 0>;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type5 aligned = struct {
// IR-NEXT:         field0 a: i32;
// IR-NEXT:     } [size=16, align=16, offsets=[0]];
// IR-NEXT:     fn %6 @pair(%7 value: @type0) -> @type0 [linkage=external] [abi=x86_win32(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type0, reason=return>(read<@type0>(%7));
// IR-NEXT:     }
// IR-NEXT:     fn %8 @odd_array(%9 value: @type1) -> @type1 [linkage=external] [abi=x86_win32(native_c) -> sret<align=1>] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type1, reason=return>(read<@type1>(%9));
// IR-NEXT:     }
// IR-NEXT:     fn %10 @bit_precise(%11 value: @type2) -> @type2 [linkage=external] [abi=x86_win32(native_c) -> sret<align=4>] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type2, reason=return>(read<@type2>(%11));
// IR-NEXT:     }
// IR-NEXT:     fn %12 @flexible(%13 value: @type3) -> @type3 [linkage=external] [abi=x86_win32(native_c) -> sret<align=4>] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type3, reason=return>(read<@type3>(%13));
// IR-NEXT:     }
// IR-NEXT:     fn %14 @zero_tail(%15 value: @type4) -> @type4 [linkage=external] [abi=x86_win32(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type4, reason=return>(read<@type4>(%15));
// IR-NEXT:     }
// IR-NEXT:     fn %16 @aligned(%17 value: @type5) -> @type5 [linkage=external] [abi=x86_win32(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type5, reason=return>(read<@type5>(%17));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
