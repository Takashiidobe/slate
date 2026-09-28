// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -std=gnu99

typedef double T;

int prototype(enum { T = 2 } value, int a[sizeof(T)]);
T after_prototype;

int defined(enum { U = 3 } value) { return U; }
T after_definition;

int tagged(struct hidden { int x; } *p);

extern __inline int probe(double value) { return 0; }
extern __typeof(probe) probe __asm__("__GI_probe");

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
// IR-NEXT:     type @type0 T = f64;
// IR-NEXT:     type @type1 = enum : u32 {
// IR-NEXT:         %0 T = const<i32>(2);
// IR-NEXT:     } [size=4, align=4];
// IR-NEXT:     type @type2 = enum : u32 {
// IR-NEXT:         %0 U = const<i32>(3);
// IR-NEXT:     } [size=4, align=4];
// IR-NEXT:     type @type3 hidden = struct {
// IR-NEXT:         field0 x: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     global %4 after_prototype: f64 [storage=static] [linkage=external];
// IR-NEXT:     global %9 after_definition: f64 [storage=static] [linkage=external];
// IR-NEXT:     fn %3 @prototype(%14 value: @type1, %15 a: ptr<i32> [array=4]) -> i32 [linkage=external];
// IR-NEXT:     fn %5 @defined(%8 value: @type2) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<i32>(3);
// IR-NEXT:     }
// IR-NEXT:     fn %11 @tagged(%16 p: ptr<@type3>) -> i32 [linkage=external];
// IR-NEXT:     fn %12 @probe(%13 value: f64) -> i32 [linkage=external] [asm_name="__GI_probe"] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<i32>(0);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
