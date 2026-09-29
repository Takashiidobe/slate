int *ints;
float *floats;
int flag;

int ordered(void) { return (ints < 1) + (0 > ints); }
int *masked(void) { return __atomic_fetch_and(&ints, 3, 5); }
int *inverted(void) { return __atomic_nand_fetch(&ints, 1, 5); }
void toggled(void) { __sync_fetch_and_xor(&ints, 1); }
void *mismatched(void) { return flag ? ints : floats; }

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT c89
// SLATE-FILECHECK-WARNING DEFAULT
// SLATE-FILECHECK-ARGS --dump-ir

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: -Wpointer-integer-compare
// DEFAULT: ⚠ comparison between pointer and integer
// DEFAULT: ╭─[tests/fixtures/gcc/linux/x86_64/ir_lenient_operand_rules.c:5:29]
// DEFAULT: 4 │
// DEFAULT: 5 │ int ordered(void) { return (ints < 1) + (0 > ints); }
// DEFAULT: ·                             ────────
// DEFAULT: 6 │ int *masked(void) { return __atomic_fetch_and(&ints, 3, 5); }
// DEFAULT: ╰────
// DEFAULT: -Wincompatible-pointer-types
// DEFAULT: ⚠ pointer type mismatch in conditional expression
// DEFAULT: ╭─[tests/fixtures/gcc/linux/x86_64/ir_lenient_operand_rules.c:9:33]
// DEFAULT: 8 │ void toggled(void) { __sync_fetch_and_xor(&ints, 1); }
// DEFAULT: 9 │ void *mismatched(void) { return flag ? ints : floats; }
// DEFAULT: ·                                 ────────────────────
// DEFAULT: 10 │
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN IR-DEFAULT
// IR-DEFAULT: module {
// IR-DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-DEFAULT-NEXT:         endian = little;
// IR-DEFAULT-NEXT:         pointer [size=8, align=8];
// IR-DEFAULT-NEXT:         stack_alignment = 16;
// IR-DEFAULT-NEXT:         long_double = f80;
// IR-DEFAULT-NEXT:         storage bool [size=1, align=1];
// IR-DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// IR-DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// IR-DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// IR-DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// IR-DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// IR-DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// IR-DEFAULT-NEXT:         storage f16 [size=2, align=2];
// IR-DEFAULT-NEXT:         storage f32 [size=4, align=4];
// IR-DEFAULT-NEXT:         storage f64 [size=8, align=8];
// IR-DEFAULT-NEXT:         storage f80 [size=16, align=16];
// IR-DEFAULT-NEXT:         storage f128 [size=16, align=16];
// IR-DEFAULT-NEXT:         storage d32 [size=4, align=4];
// IR-DEFAULT-NEXT:         storage d64 [size=8, align=8];
// IR-DEFAULT-NEXT:         storage d128 [size=16, align=16];
// IR-DEFAULT-NEXT:     }
// IR-DEFAULT-NEXT:     global %[[VALUE_ints:[0-9]+]] ints: ptr<i32> [storage=static] [linkage=external];
// IR-DEFAULT-NEXT:     global %[[VALUE_floats:[0-9]+]] floats: ptr<f32> [storage=static] [linkage=external];
// IR-DEFAULT-NEXT:     global %[[VALUE_flag:[0-9]+]] flag: i32 [storage=static] [linkage=external];
// IR-DEFAULT-NEXT:     fn %[[VALUE_ordered:[0-9]+]] @ordered() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-DEFAULT-NEXT:         return add<i32, overflow=ub>(from_bool<i32, reason=promotion>(lt<ptr<i32>>(read<ptr<i32>>(%[[VALUE_ints]]), int_to_ptr<ptr<i32>, reason=usual_arith>(const<i32>(1)))), from_bool<i32, reason=promotion>(gt<ptr<i32>>(null<ptr<i32>>, read<ptr<i32>>(%[[VALUE_ints]]))));
// IR-DEFAULT-NEXT:     }
// IR-DEFAULT-NEXT:     fn %[[VALUE_masked:[0-9]+]] @masked() -> ptr<i32> [linkage=external] [fallthrough=ub_if_used] {
// IR-DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: ptr<i32> [synthetic] = update<ptr<i32>, result=old, atomic=seq_cst>(deref(addr_of<ptr<ptr<i32>>>(%[[VALUE_ints]])), int_to_ptr<ptr<i32>, reason=explicit>(and<u64>(ptr_to_int<u64, reason=explicit>(old<ptr<i32>>), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))))));
// IR-DEFAULT-NEXT:         return read<ptr<i32>>(%[[VALUE0]]);
// IR-DEFAULT-NEXT:     }
// IR-DEFAULT-NEXT:     fn %[[VALUE_inverted:[0-9]+]] @inverted() -> ptr<i32> [linkage=external] [fallthrough=ub_if_used] {
// IR-DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: ptr<i32> [synthetic] = update<ptr<i32>, result=new, atomic=seq_cst>(deref(addr_of<ptr<ptr<i32>>>(%[[VALUE_ints]])), int_to_ptr<ptr<i32>, reason=explicit>(not<u64>(and<u64>(ptr_to_int<u64, reason=explicit>(old<ptr<i32>>), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))))));
// IR-DEFAULT-NEXT:         return read<ptr<i32>>(%[[VALUE1]]);
// IR-DEFAULT-NEXT:     }
// IR-DEFAULT-NEXT:     fn %[[VALUE_toggled:[0-9]+]] @toggled() -> void [linkage=external] [fallthrough=ret_void] {
// IR-DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: ptr<i32> [synthetic] = update<ptr<i32>, result=old, atomic=seq_cst>(deref(addr_of<ptr<ptr<i32>>>(%[[VALUE_ints]])), int_to_ptr<ptr<i32>, reason=explicit>(xor<u64>(ptr_to_int<u64, reason=explicit>(old<ptr<i32>>), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))))));
// IR-DEFAULT-NEXT:     }
// IR-DEFAULT-NEXT:     fn %[[VALUE_mismatched:[0-9]+]] @mismatched() -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// IR-DEFAULT-NEXT:         return conditional<ptr<void>>(ne<i32>(read<i32>(%[[VALUE_flag]]), const<i32>(0)), pointer_cast<ptr<void>, reason=usual_arith>(read<ptr<i32>>(%[[VALUE_ints]])), pointer_cast<ptr<void>, reason=usual_arith>(read<ptr<f32>>(%[[VALUE_floats]])));
// IR-DEFAULT-NEXT:     }
// IR-DEFAULT-NEXT: }
// SLATE-FILECHECK-END IR-DEFAULT
