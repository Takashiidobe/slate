// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

struct flexible { int n; int tail[]; };
struct flexible_double { double d; float tail[]; };

struct flexible flexible(struct flexible value) { return value; }
struct flexible_double flexible_double(struct flexible_double value) { return value; }

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
// IR-NEXT:     type @type0 flexible = struct {
// IR-NEXT:         field0 n: i32;
// IR-NEXT:         field1 tail: array<i32, incomplete>;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type1 flexible_double = struct {
// IR-NEXT:         field0 d: f64;
// IR-NEXT:         field1 tail: array<f32, incomplete>;
// IR-NEXT:     } [size=8, align=8, offsets=[0, 8]];
// IR-NEXT:     fn %2 @flexible(%3 value: @type0) -> @type0 [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type0, reason=return>(read<@type0>(%3));
// IR-NEXT:     }
// IR-NEXT:     fn %4 @flexible_double(%5 value: @type1) -> @type1 [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type1, reason=return>(read<@type1>(%5));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
