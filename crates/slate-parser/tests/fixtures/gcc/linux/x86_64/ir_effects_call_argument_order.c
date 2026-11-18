// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

int plain(int *);
int pair(int, int);
int g;
int (*pick(void))(int, int);

int sum(int *p) { return plain(p) + (g = 1); }
int arguments(int *p) { return pair(plain(p), g = 2); }
int both(int *p) { return pair(plain(p), plain(p + 1)); }
int indirect(int *p) { return pick()(plain(p), plain(p + 1)); }
int i;
int unsequenced(void) { return pair(i++, i++); }

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
// IR-NEXT:     global %[[VALUE_g:[0-9]+]] g: i32 [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_i:[0-9]+]] i: i32 [storage=static] [linkage=external];
// IR-NEXT:     fn %[[VALUE_plain:[0-9]+]] @plain(%[[VALUE0:[0-9]+]] <unnamed>: ptr<i32>) -> i32 [linkage=external];
// IR-NEXT:     fn %[[VALUE_pair:[0-9]+]] @pair(%[[VALUE1:[0-9]+]] <unnamed>: i32, %[[VALUE2:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external];
// IR-NEXT:     fn %[[VALUE_pick:[0-9]+]] @pick() -> ptr<fn(i32, i32) -> i32> [linkage=external];
// IR-NEXT:     fn %[[VALUE_sum:[0-9]+]] @sum(%[[VALUE_p:[0-9]+]] p: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE3:[0-9]+]]: i32 [synthetic] = call<i32>(%[[VALUE_plain]], read<ptr<i32>>(%[[VALUE_p]]));
// IR-NEXT:         write<i32>(%[[VALUE_g]], const<i32>(1));
// IR-NEXT:         return add<i32>(read<i32>(%[[VALUE3]]), const<i32>(1));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_arguments:[0-9]+]] @arguments(%[[VALUE_p_2:[0-9]+]] p: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         write<i32>(%[[VALUE_g]], const<i32>(2));
// IR-NEXT:         return call<i32>(%[[VALUE_pair]], call<i32>(%[[VALUE_plain]], read<ptr<i32>>(%[[VALUE_p_2]])), const<i32>(2));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_both:[0-9]+]] @both(%[[VALUE_p_3:[0-9]+]] p: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE4:[0-9]+]]: i32 [synthetic] = call<i32>(%[[VALUE_plain]], ptr_offset<ptr<i32>, subtract=false>(read<ptr<i32>>(%[[VALUE_p_3]]), const<i32>(1)));
// IR-NEXT:         return call<i32>(%[[VALUE_pair]], call<i32>(%[[VALUE_plain]], read<ptr<i32>>(%[[VALUE_p_3]])), read<i32>(%[[VALUE4]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_indirect:[0-9]+]] @indirect(%[[VALUE_p_4:[0-9]+]] p: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE5:[0-9]+]]: ptr<fn(i32, i32) -> i32> [synthetic] = call<ptr<fn(i32, i32) -> i32>>(%[[VALUE_pick]]);
// IR-NEXT:         let %[[VALUE6:[0-9]+]]: i32 [synthetic] = call<i32>(%[[VALUE_plain]], ptr_offset<ptr<i32>, subtract=false>(read<ptr<i32>>(%[[VALUE_p_4]]), const<i32>(1)));
// IR-NEXT:         return call<i32>(read<ptr<fn(i32, i32) -> i32>>(%[[VALUE5]]), call<i32>(%[[VALUE_plain]], read<ptr<i32>>(%[[VALUE_p_4]])), read<i32>(%[[VALUE6]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_unsequenced:[0-9]+]] @unsequenced() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE7:[0-9]+]]: i32 [synthetic, unsequenced] = read<i32>(%[[VALUE_i]]);
// IR-NEXT:         let %[[VALUE8:[0-9]+]]: i32 [synthetic, unsequenced] = add<i32>(read<i32>(%[[VALUE7]]), const<i32>(1));
// IR-NEXT:         write<i32, unsequenced>(%[[VALUE_i]], read<i32>(%[[VALUE8]]));
// IR-NEXT:         let %[[VALUE9:[0-9]+]]: i32 [synthetic, unsequenced] = read<i32>(%[[VALUE_i]]);
// IR-NEXT:         let %[[VALUE10:[0-9]+]]: i32 [synthetic, unsequenced] = add<i32>(read<i32>(%[[VALUE9]]), const<i32>(1));
// IR-NEXT:         write<i32, unsequenced>(%[[VALUE_i]], read<i32>(%[[VALUE10]]));
// IR-NEXT:         return call<i32>(%[[VALUE_pair]], read<i32>(%[[VALUE9]]), read<i32>(%[[VALUE7]]));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
