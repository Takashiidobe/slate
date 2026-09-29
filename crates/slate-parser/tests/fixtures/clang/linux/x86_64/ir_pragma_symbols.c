// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-STD IR c23
#pragma weak weak_name
int weak_name(int v) { return v; }

int weak_target(int v) { return v + 1; }
#pragma weak weak_alias = weak_target
extern int weak_alias(int);

#pragma redefine_extname renamed actual
int renamed(int v) { return v * 3; }

#pragma GCC visibility push(hidden)
int hidden_fn(void) { return 1; }
int hidden_var;
int __attribute__((visibility("default"))) explicit_default;
#pragma GCC visibility push(protected)
int protected_var;
#pragma GCC visibility pop
int still_hidden_var;
#pragma GCC visibility pop
int plain_fn(void) { return 2; }
int plain_var;

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
// IR-NEXT:     global %[[VALUE_hidden_var:[0-9]+]] hidden_var: i32 [storage=static] [linkage=external] [visibility=hidden];
// IR-NEXT:     global %[[VALUE_explicit_default:[0-9]+]] explicit_default: i32 [storage=static] [linkage=external] [visibility=default];
// IR-NEXT:     global %[[VALUE_protected_var:[0-9]+]] protected_var: i32 [storage=static] [linkage=external] [visibility=protected];
// IR-NEXT:     global %[[VALUE_still_hidden_var:[0-9]+]] still_hidden_var: i32 [storage=static] [linkage=external] [visibility=hidden];
// IR-NEXT:     global %[[VALUE_plain_var:[0-9]+]] plain_var: i32 [storage=static] [linkage=external];
// IR-NEXT:     fn %[[VALUE_weak_name:[0-9]+]] @weak_name(%[[VALUE_v:[0-9]+]] v: i32) -> i32 [linkage=external] [weak] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<i32>(%[[VALUE_v]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_weak_target:[0-9]+]] @weak_target(%[[VALUE_v_2:[0-9]+]] v: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<i32, overflow=ub>(read<i32>(%[[VALUE_v_2]]), const<i32>(1));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_weak_alias:[0-9]+]] @weak_alias(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external] [weak] [alias="weak_target"];
// IR-NEXT:     fn %[[VALUE_renamed:[0-9]+]] @renamed(%[[VALUE_v_3:[0-9]+]] v: i32) -> i32 [linkage=external] [asm_name="actual"] [fallthrough=ub_if_used] {
// IR-NEXT:         return mul<i32, overflow=ub>(read<i32>(%[[VALUE_v_3]]), const<i32>(3));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_hidden_fn:[0-9]+]] @hidden_fn() -> i32 [linkage=external] [visibility=hidden] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<i32>(1);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_plain_fn:[0-9]+]] @plain_fn() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<i32>(2);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
