// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir
#define PREFIX "data."

__attribute__((deprecated("use " "new_api"))) int old_api(void);
__attribute__((section(PREFIX "custom"))) int placed = 1;
__attribute__((section(u8".wide" "r"))) int prefixed = 2;
__attribute__((visibility("hid" "den"))) int hidden = 3;

int use(void) { return old_api() + placed + prefixed + hidden; }

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
// IR-NEXT:     global %[[VALUE_placed:[0-9]+]] placed: i32 [storage=static] = const<i32>(1) [linkage=external] [section="data.custom"];
// IR-NEXT:     global %[[VALUE_prefixed:[0-9]+]] prefixed: i32 [storage=static] = const<i32>(2) [linkage=external] [section=".wider"];
// IR-NEXT:     global %[[VALUE_hidden:[0-9]+]] hidden: i32 [storage=static] = const<i32>(3) [linkage=external] [visibility=hidden];
// IR-NEXT:     fn %[[VALUE_old_api:[0-9]+]] @old_api() -> i32 [linkage=external];
// IR-NEXT:     fn %[[VALUE_use:[0-9]+]] @use() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(call<i32, signature=fn() -> i32>(%[[VALUE_old_api]]), read<i32>(%[[VALUE_placed]])), read<i32>(%[[VALUE_prefixed]])), read<i32>(%[[VALUE_hidden]]));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
