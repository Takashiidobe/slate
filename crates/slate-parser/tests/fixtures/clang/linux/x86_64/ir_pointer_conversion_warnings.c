// SLATE-FILECHECK-DEFINES WARN
// SLATE-FILECHECK-WARNING WARN
// SLATE-FILECHECK-ARGS --dump-ir -std=c23

void take(int *);
unsigned *returned(int *value) { return value; }
void passed(unsigned *value) { take(value); }
void assigned(unsigned *value, const int *constant, volatile int *shared, const int **nested) {
    int *sign = value;
    int *dropped_const = constant;
    int *dropped_volatile = shared;
    int **dropped_nested = nested;
    const unsigned *added_const = sign;
    unsigned *explicit_cast = (unsigned *)sign;
    const int *compatible = sign;
    (void)dropped_const, (void)dropped_volatile, (void)dropped_nested;
    (void)added_const, (void)explicit_cast, (void)compatible;
}
int compared(int *left, unsigned *right) { return left == (int *)right; }
int *initialized_from_cast(unsigned *value) {
    int *result = (unsigned *)value;
    return result;
}
void take_unsigned(unsigned *value);
void passed_cast_argument(int *value) { take_unsigned((int *)value); }

// SLATE-FILECHECK-BEGIN WARN
// WARN: -Wpointer-sign
// WARN: ⚠ conversion between pointers to integer types with different sign
// WARN: ╭─[tests/fixtures/clang/linux/x86_64/ir_pointer_conversion_warnings.c:3:41]
// WARN: 2 │ void take(int *);
// WARN: 3 │ unsigned *returned(int *value) { return value; }
// WARN: ·                                         ─────
// WARN: 4 │ void passed(unsigned *value) { take(value); }
// WARN: ╰────
// WARN: -Wpointer-sign
// WARN: ⚠ conversion between pointers to integer types with different sign
// WARN: ╭─[tests/fixtures/clang/linux/x86_64/ir_pointer_conversion_warnings.c:4:37]
// WARN: 3 │ unsigned *returned(int *value) { return value; }
// WARN: 4 │ void passed(unsigned *value) { take(value); }
// WARN: ·                                     ─────
// WARN: 5 │ void assigned(unsigned *value, const int *constant, volatile int *shared, const int **nested) {
// WARN: ╰────
// WARN: -Wpointer-sign
// WARN: ⚠ conversion between pointers to integer types with different sign
// WARN: ╭─[tests/fixtures/clang/linux/x86_64/ir_pointer_conversion_warnings.c:6:17]
// WARN: 5 │ void assigned(unsigned *value, const int *constant, volatile int *shared, const int **nested) {
// WARN: 6 │     int *sign = value;
// WARN: ·                 ─────
// WARN: 7 │     int *dropped_const = constant;
// WARN: ╰────
// WARN: -Wincompatible-pointer-types-discards-qualifiers
// WARN: ⚠ pointer conversion discards qualifiers
// WARN: ╭─[tests/fixtures/clang/linux/x86_64/ir_pointer_conversion_warnings.c:7:26]
// WARN: 6 │     int *sign = value;
// WARN: 7 │     int *dropped_const = constant;
// WARN: ·                          ────────
// WARN: 8 │     int *dropped_volatile = shared;
// WARN: ╰────
// WARN: -Wincompatible-pointer-types-discards-qualifiers
// WARN: ⚠ pointer conversion discards qualifiers
// WARN: ╭─[tests/fixtures/clang/linux/x86_64/ir_pointer_conversion_warnings.c:8:29]
// WARN: 7 │     int *dropped_const = constant;
// WARN: 8 │     int *dropped_volatile = shared;
// WARN: ·                             ──────
// WARN: 9 │     int **dropped_nested = nested;
// WARN: ╰────
// WARN: -Wincompatible-pointer-types-discards-qualifiers
// WARN: ⚠ pointer conversion discards qualifiers in nested pointer types
// WARN: ╭─[tests/fixtures/clang/linux/x86_64/ir_pointer_conversion_warnings.c:9:28]
// WARN: 8 │     int *dropped_volatile = shared;
// WARN: 9 │     int **dropped_nested = nested;
// WARN: ·                            ──────
// WARN: 10 │     const unsigned *added_const = sign;
// WARN: ╰────
// WARN: -Wpointer-sign
// WARN: ⚠ conversion between pointers to integer types with different sign
// WARN: ╭─[tests/fixtures/clang/linux/x86_64/ir_pointer_conversion_warnings.c:10:35]
// WARN: 9 │     int **dropped_nested = nested;
// WARN: 10 │     const unsigned *added_const = sign;
// WARN: ·                                   ────
// WARN: 11 │     unsigned *explicit_cast = (unsigned *)sign;
// WARN: ╰────
// WARN: -Wpointer-sign
// WARN: ⚠ conversion between pointers to integer types with different sign
// WARN: ╭─[tests/fixtures/clang/linux/x86_64/ir_pointer_conversion_warnings.c:18:19]
// WARN: 17 │ int *initialized_from_cast(unsigned *value) {
// WARN: 18 │     int *result = (unsigned *)value;
// WARN: ·                   ─────────────────
// WARN: 19 │     return result;
// WARN: ╰────
// WARN: -Wpointer-sign
// WARN: ⚠ conversion between pointers to integer types with different sign
// WARN: ╭─[tests/fixtures/clang/linux/x86_64/ir_pointer_conversion_warnings.c:22:55]
// WARN: 21 │ void take_unsigned(unsigned *value);
// WARN: 22 │ void passed_cast_argument(int *value) { take_unsigned((int *)value); }
// WARN: ·                                                       ────────────
// WARN: 23 │
// WARN: ╰────
// SLATE-FILECHECK-END WARN
// SLATE-FILECHECK-BEGIN IR-WARN
// IR-WARN: module {
// IR-WARN-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-WARN-NEXT:         endian = little;
// IR-WARN-NEXT:         pointer [size=8, align=8];
// IR-WARN-NEXT:         stack_alignment = 16;
// IR-WARN-NEXT:         long_double = f80;
// IR-WARN-NEXT:         storage bool [size=1, align=1];
// IR-WARN-NEXT:         storage i8, u8 [size=1, align=1];
// IR-WARN-NEXT:         storage i16, u16 [size=2, align=2];
// IR-WARN-NEXT:         storage i32, u32 [size=4, align=4];
// IR-WARN-NEXT:         storage i64, u64 [size=8, align=8];
// IR-WARN-NEXT:         storage i128, u128 [size=16, align=16];
// IR-WARN-NEXT:         storage bf16 [size=2, align=2];
// IR-WARN-NEXT:         storage f16 [size=2, align=2];
// IR-WARN-NEXT:         storage f32 [size=4, align=4];
// IR-WARN-NEXT:         storage f64 [size=8, align=8];
// IR-WARN-NEXT:         storage f80 [size=16, align=16];
// IR-WARN-NEXT:         storage f128 [size=16, align=16];
// IR-WARN-NEXT:         storage d32 [size=4, align=4];
// IR-WARN-NEXT:         storage d64 [size=8, align=8];
// IR-WARN-NEXT:         storage d128 [size=16, align=16];
// IR-WARN-NEXT:     }
// IR-WARN-NEXT:     fn %[[VALUE_take:[0-9]+]] @take(%[[VALUE0:[0-9]+]] <unnamed>: ptr<i32>) -> void [linkage=external];
// IR-WARN-NEXT:     fn %[[VALUE_returned:[0-9]+]] @returned(%[[VALUE_value:[0-9]+]] value: ptr<i32>) -> ptr<u32> [linkage=external] [fallthrough=ub_if_used] {
// IR-WARN-NEXT:         return pointer_cast<ptr<u32>, reason=return>(read<ptr<i32>>(%[[VALUE_value]]));
// IR-WARN-NEXT:     }
// IR-WARN-NEXT:     fn %[[VALUE_passed:[0-9]+]] @passed(%[[VALUE_value_2:[0-9]+]] value: ptr<u32>) -> void [linkage=external] [fallthrough=ret_void] {
// IR-WARN-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%[[VALUE_take]], pointer_cast<ptr<i32>, reason=arg>(read<ptr<u32>>(%[[VALUE_value_2]])));
// IR-WARN-NEXT:     }
// IR-WARN-NEXT:     fn %[[VALUE_assigned:[0-9]+]] @assigned(%[[VALUE_value_3:[0-9]+]] value: ptr<u32>, %[[VALUE_constant:[0-9]+]] constant: ptr<const i32>, %[[VALUE_shared:[0-9]+]] shared: ptr<volatile i32>, %[[VALUE_nested:[0-9]+]] nested: ptr<ptr<const i32>>) -> void [linkage=external] [fallthrough=ret_void] {
// IR-WARN-NEXT:         let %[[VALUE_sign:[0-9]+]] sign: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=assign>(read<ptr<u32>>(%[[VALUE_value_3]]));
// IR-WARN-NEXT:         let %[[VALUE_dropped_const:[0-9]+]] dropped_const: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=assign>(read<ptr<const i32>>(%[[VALUE_constant]]));
// IR-WARN-NEXT:         let %[[VALUE_dropped_volatile:[0-9]+]] dropped_volatile: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=assign>(read<ptr<volatile i32>>(%[[VALUE_shared]]));
// IR-WARN-NEXT:         let %[[VALUE_dropped_nested:[0-9]+]] dropped_nested: ptr<ptr<i32>> [storage=automatic] = pointer_cast<ptr<ptr<i32>>, reason=assign>(read<ptr<ptr<const i32>>>(%[[VALUE_nested]]));
// IR-WARN-NEXT:         let %[[VALUE_added_const:[0-9]+]] added_const: ptr<const u32> [storage=automatic] = pointer_cast<ptr<const u32>, reason=assign>(read<ptr<i32>>(%[[VALUE_sign]]));
// IR-WARN-NEXT:         let %[[VALUE_explicit_cast:[0-9]+]] explicit_cast: ptr<u32> [storage=automatic] = pointer_cast<ptr<u32>, reason=explicit>(read<ptr<i32>>(%[[VALUE_sign]]));
// IR-WARN-NEXT:         let %[[VALUE_compatible:[0-9]+]] compatible: ptr<const i32> [storage=automatic] = pointer_cast<ptr<const i32>, reason=assign>(read<ptr<i32>>(%[[VALUE_sign]]));
// IR-WARN-NEXT:         read<ptr<i32>>(%[[VALUE_dropped_const]]);
// IR-WARN-NEXT:         read<ptr<i32>>(%[[VALUE_dropped_volatile]]);
// IR-WARN-NEXT:         read<ptr<ptr<i32>>>(%[[VALUE_dropped_nested]]);
// IR-WARN-NEXT:         read<ptr<const u32>>(%[[VALUE_added_const]]);
// IR-WARN-NEXT:         read<ptr<u32>>(%[[VALUE_explicit_cast]]);
// IR-WARN-NEXT:         read<ptr<const i32>>(%[[VALUE_compatible]]);
// IR-WARN-NEXT:     }
// IR-WARN-NEXT:     fn %[[VALUE_compared:[0-9]+]] @compared(%[[VALUE_left:[0-9]+]] left: ptr<i32>, %[[VALUE_right:[0-9]+]] right: ptr<u32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-WARN-NEXT:         return from_bool<i32, reason=return>(eq<ptr<i32>>(read<ptr<i32>>(%[[VALUE_left]]), pointer_cast<ptr<i32>, reason=explicit>(read<ptr<u32>>(%[[VALUE_right]]))));
// IR-WARN-NEXT:     }
// IR-WARN-NEXT:     fn %[[VALUE_initialized_from_cast:[0-9]+]] @initialized_from_cast(%[[VALUE_value_4:[0-9]+]] value: ptr<u32>) -> ptr<i32> [linkage=external] [fallthrough=ub_if_used] {
// IR-WARN-NEXT:         let %[[VALUE_result:[0-9]+]] result: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=assign>(read<ptr<u32>>(%[[VALUE_value_4]]));
// IR-WARN-NEXT:         return read<ptr<i32>>(%[[VALUE_result]]);
// IR-WARN-NEXT:     }
// IR-WARN-NEXT:     fn %[[VALUE_take_unsigned:[0-9]+]] @take_unsigned(%[[VALUE_value_5:[0-9]+]] value: ptr<u32>) -> void [linkage=external];
// IR-WARN-NEXT:     fn %[[VALUE_passed_cast_argument:[0-9]+]] @passed_cast_argument(%[[VALUE_value_6:[0-9]+]] value: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// IR-WARN-NEXT:         call<void, signature=fn(ptr<u32>) -> void>(%[[VALUE_take_unsigned]], pointer_cast<ptr<u32>, reason=arg>(read<ptr<i32>>(%[[VALUE_value_6]])));
// IR-WARN-NEXT:     }
// IR-WARN-NEXT: }
// SLATE-FILECHECK-END IR-WARN
