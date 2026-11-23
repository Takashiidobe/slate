// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT c11
// SLATE-FILECHECK-DEFINES SIDE_EFFECT SIDE_EFFECT
// SLATE-FILECHECK-STD SIDE_EFFECT c11

int runtime(void);
#ifdef SIDE_EFFECT
int value = (runtime(), 4) + 2;
#else
int target;

int value = (3, 4) + 2;
int chain = ((void)0, (2.5, 4)) + 2;
int short_circuit = (0 && runtime(), 7);
int from_address = (&target, 8);
int *address = (3, &target);

int local(void) {
  static int values[] = {(3, 4) + 2};
  int automatic = (runtime(), 9);
  return values[0] + automatic;
}
#endif

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
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
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     global %[[VALUE_target:[0-9]+]] target: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_value:[0-9]+]] value: i32 [storage=static] = const<i32>(6) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_chain:[0-9]+]] chain: i32 [storage=static] = const<i32>(6) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_short_circuit:[0-9]+]] short_circuit: i32 [storage=static] = const<i32>(7) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_from_address:[0-9]+]] from_address: i32 [storage=static] = const<i32>(8) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_address:[0-9]+]] address: ptr<i32> [storage=static] = addr_of<ptr<i32>>(%[[VALUE_target]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_values:[0-9]+]] values: array<i32, 1> [storage=static] = aggregate<array<i32, 1>, zero_fill=false>(index0 = const<i32>(6)) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_runtime:[0-9]+]] @runtime() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_local:[0-9]+]] @local() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_automatic:[0-9]+]] automatic: i32 [storage=automatic];
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_runtime]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_automatic]], const<i32>(9));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_values]]), const<i32>(0)))), read<i32>(%[[VALUE_automatic]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN SIDE_EFFECT
// SIDE_EFFECT: module {
// SIDE_EFFECT-NEXT:     target "x86_64-unknown-linux-gnu" {
// SIDE_EFFECT-NEXT:         endian = little;
// SIDE_EFFECT-NEXT:         pointer [size=8, align=8];
// SIDE_EFFECT-NEXT:         stack_alignment = 16;
// SIDE_EFFECT-NEXT:         long_double = f80;
// SIDE_EFFECT-NEXT:         storage bool [size=1, align=1];
// SIDE_EFFECT-NEXT:         storage i8, u8 [size=1, align=1];
// SIDE_EFFECT-NEXT:         storage i16, u16 [size=2, align=2];
// SIDE_EFFECT-NEXT:         storage i32, u32 [size=4, align=4];
// SIDE_EFFECT-NEXT:         storage i64, u64 [size=8, align=8];
// SIDE_EFFECT-NEXT:         storage i128, u128 [size=16, align=16];
// SIDE_EFFECT-NEXT:         storage bf16 [size=2, align=2];
// SIDE_EFFECT-NEXT:         storage f16 [size=2, align=2];
// SIDE_EFFECT-NEXT:         storage f32 [size=4, align=4];
// SIDE_EFFECT-NEXT:         storage f64 [size=8, align=8];
// SIDE_EFFECT-NEXT:         storage f80 [size=16, align=16];
// SIDE_EFFECT-NEXT:         storage f128 [size=16, align=16];
// SIDE_EFFECT-NEXT:         storage d32 [size=4, align=4];
// SIDE_EFFECT-NEXT:         storage d64 [size=8, align=8];
// SIDE_EFFECT-NEXT:         storage d128 [size=16, align=16];
// SIDE_EFFECT-NEXT:     }
// SIDE_EFFECT-NEXT:     global %[[VALUE_value:[0-9]+]] value: i32 [storage=static] = add<i32, overflow=ub>(sequence<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_runtime:[0-9]+]]), const<i32>(4)), const<i32>(2)) [linkage=external];
// SIDE_EFFECT-NEXT:     fn %[[VALUE_runtime]] @runtime() -> i32 [linkage=external];
// SIDE_EFFECT-NEXT: }
// SLATE-FILECHECK-END SIDE_EFFECT
