typedef int Pair[2];
int *ints;
float *floats;
int **slots;
long bits;
int flag;

void *mismatched(void) { return flag ? ints : floats; }
int ordered(void) { return (ints < 1) + (0 > ints); }
unsigned long pair_size = sizeof(__builtin_bit_cast(Pair, bits));
int first_half(void) { return __builtin_bit_cast(Pair, bits)[0]; }
void floating_offset(void) { __atomic_fetch_add(slots, 1.5, 5); }

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-WARNING DEFAULT
// SLATE-FILECHECK-ARGS --dump-ir

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: -Wpointer-type-mismatch
// DEFAULT: ⚠ pointer type mismatch in conditional expression
// DEFAULT: ╭─[tests/fixtures/clang/linux/x86_64/ir_lenient_operand_rules.c:8:33]
// DEFAULT: 7 │
// DEFAULT: 8 │ void *mismatched(void) { return flag ? ints : floats; }
// DEFAULT: ·                                 ────────────────────
// DEFAULT: 9 │ int ordered(void) { return (ints < 1) + (0 > ints); }
// DEFAULT: ╰────
// DEFAULT: -Wpointer-integer-compare
// DEFAULT: ⚠ comparison between pointer and integer
// DEFAULT: ╭─[tests/fixtures/clang/linux/x86_64/ir_lenient_operand_rules.c:9:29]
// DEFAULT: 8 │ void *mismatched(void) { return flag ? ints : floats; }
// DEFAULT: 9 │ int ordered(void) { return (ints < 1) + (0 > ints); }
// DEFAULT: ·                             ────────
// DEFAULT: 10 │ unsigned long pair_size = sizeof(__builtin_bit_cast(Pair, bits));
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
// IR-DEFAULT-NEXT:     type @type[[TYPE_Pair:[0-9]+]] Pair = array<i32, 2>;
// IR-DEFAULT-NEXT:     global %[[VALUE_ints:[0-9]+]] ints: ptr<i32> [storage=static] [linkage=external];
// IR-DEFAULT-NEXT:     global %[[VALUE_floats:[0-9]+]] floats: ptr<f32> [storage=static] [linkage=external];
// IR-DEFAULT-NEXT:     global %[[VALUE_slots:[0-9]+]] slots: ptr<ptr<i32>> [storage=static] [linkage=external];
// IR-DEFAULT-NEXT:     global %[[VALUE_bits:[0-9]+]] bits: i64 [storage=static] [linkage=external];
// IR-DEFAULT-NEXT:     global %[[VALUE_flag:[0-9]+]] flag: i32 [storage=static] [linkage=external];
// IR-DEFAULT-NEXT:     global %[[VALUE_pair_size:[0-9]+]] pair_size: u64 [storage=static] = const<u64>(8) [linkage=external];
// IR-DEFAULT-NEXT:     fn %[[VALUE_mismatched:[0-9]+]] @mismatched() -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// IR-DEFAULT-NEXT:         return conditional<ptr<void>>(ne<i32>(read<i32>(%[[VALUE_flag]]), const<i32>(0)), pointer_cast<ptr<void>, reason=usual_arith>(read<ptr<i32>>(%[[VALUE_ints]])), pointer_cast<ptr<void>, reason=usual_arith>(read<ptr<f32>>(%[[VALUE_floats]])));
// IR-DEFAULT-NEXT:     }
// IR-DEFAULT-NEXT:     fn %[[VALUE_ordered:[0-9]+]] @ordered() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-DEFAULT-NEXT:         return add<i32, overflow=ub>(from_bool<i32, reason=promotion>(lt<ptr<i32>>(read<ptr<i32>>(%[[VALUE_ints]]), int_to_ptr<ptr<i32>, reason=usual_arith>(const<i32>(1)))), from_bool<i32, reason=promotion>(gt<ptr<i32>>(null<ptr<i32>>, read<ptr<i32>>(%[[VALUE_ints]]))));
// IR-DEFAULT-NEXT:     }
// IR-DEFAULT-NEXT:     fn %[[VALUE_first_half:[0-9]+]] @first_half() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-DEFAULT-NEXT:         return read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(temporary %[[VALUE0:[0-9]+]] = bit_cast<array<i32, 2>, reason=explicit>(read<i64>(%[[VALUE_bits]]))), const<i32>(0))));
// IR-DEFAULT-NEXT:     }
// IR-DEFAULT-NEXT:     fn %[[VALUE_floating_offset:[0-9]+]] @floating_offset() -> void [linkage=external] [fallthrough=ret_void] {
// IR-DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: ptr<i32> [synthetic] = update<ptr<i32>, result=old, atomic=seq_cst>(deref(read<ptr<ptr<i32>>>(%[[VALUE_slots]])), ptr_offset<ptr<i32>, subtract=false, element=u8, overflow=wrap>(old<ptr<i32>>, float_to_int<i64, reason=arg, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))));
// IR-DEFAULT-NEXT:     }
// IR-DEFAULT-NEXT: }
// SLATE-FILECHECK-END IR-DEFAULT
