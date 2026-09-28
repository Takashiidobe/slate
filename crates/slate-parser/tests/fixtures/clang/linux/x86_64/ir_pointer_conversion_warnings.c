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
// IR-WARN-NEXT:     fn %0 @take(%27 <unnamed>: ptr<i32>) -> void [linkage=external];
// IR-WARN-NEXT:     fn %1 @returned(%2 value: ptr<i32>) -> ptr<u32> [linkage=external] [fallthrough=ub_if_used] {
// IR-WARN-NEXT:         return pointer_cast<ptr<u32>, reason=return>(read<ptr<i32>>(%2));
// IR-WARN-NEXT:     }
// IR-WARN-NEXT:     fn %3 @passed(%4 value: ptr<u32>) -> void [linkage=external] [fallthrough=ret_void] {
// IR-WARN-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%0, pointer_cast<ptr<i32>, reason=arg>(read<ptr<u32>>(%4)));
// IR-WARN-NEXT:     }
// IR-WARN-NEXT:     fn %5 @assigned(%6 value: ptr<u32>, %7 constant: ptr<const i32>, %8 shared: ptr<volatile i32>, %9 nested: ptr<ptr<const i32>>) -> void [linkage=external] [fallthrough=ret_void] {
// IR-WARN-NEXT:         let %10 sign: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=assign>(read<ptr<u32>>(%6));
// IR-WARN-NEXT:         let %11 dropped_const: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=assign>(read<ptr<const i32>>(%7));
// IR-WARN-NEXT:         let %12 dropped_volatile: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=assign>(read<ptr<volatile i32>>(%8));
// IR-WARN-NEXT:         let %13 dropped_nested: ptr<ptr<i32>> [storage=automatic] = pointer_cast<ptr<ptr<i32>>, reason=assign>(read<ptr<ptr<const i32>>>(%9));
// IR-WARN-NEXT:         let %14 added_const: ptr<const u32> [storage=automatic] = pointer_cast<ptr<const u32>, reason=assign>(read<ptr<i32>>(%10));
// IR-WARN-NEXT:         let %15 explicit_cast: ptr<u32> [storage=automatic] = pointer_cast<ptr<u32>, reason=explicit>(read<ptr<i32>>(%10));
// IR-WARN-NEXT:         let %16 compatible: ptr<const i32> [storage=automatic] = pointer_cast<ptr<const i32>, reason=assign>(read<ptr<i32>>(%10));
// IR-WARN-NEXT:         read<ptr<i32>>(%11);
// IR-WARN-NEXT:         read<ptr<i32>>(%12);
// IR-WARN-NEXT:         read<ptr<ptr<i32>>>(%13);
// IR-WARN-NEXT:         read<ptr<const u32>>(%14);
// IR-WARN-NEXT:         read<ptr<u32>>(%15);
// IR-WARN-NEXT:         read<ptr<const i32>>(%16);
// IR-WARN-NEXT:     }
// IR-WARN-NEXT:     fn %17 @compared(%18 left: ptr<i32>, %19 right: ptr<u32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-WARN-NEXT:         return from_bool<i32, reason=return>(eq<ptr<i32>>(read<ptr<i32>>(%18), pointer_cast<ptr<i32>, reason=explicit>(read<ptr<u32>>(%19))));
// IR-WARN-NEXT:     }
// IR-WARN-NEXT:     fn %20 @initialized_from_cast(%21 value: ptr<u32>) -> ptr<i32> [linkage=external] [fallthrough=ub_if_used] {
// IR-WARN-NEXT:         let %22 result: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=assign>(read<ptr<u32>>(%21));
// IR-WARN-NEXT:         return read<ptr<i32>>(%22);
// IR-WARN-NEXT:     }
// IR-WARN-NEXT:     fn %24 @take_unsigned(%28 value: ptr<u32>) -> void [linkage=external];
// IR-WARN-NEXT:     fn %25 @passed_cast_argument(%26 value: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// IR-WARN-NEXT:         call<void, signature=fn(ptr<u32>) -> void>(%24, pointer_cast<ptr<u32>, reason=arg>(read<ptr<i32>>(%26)));
// IR-WARN-NEXT:     }
// IR-WARN-NEXT: }
// SLATE-FILECHECK-END IR-WARN
