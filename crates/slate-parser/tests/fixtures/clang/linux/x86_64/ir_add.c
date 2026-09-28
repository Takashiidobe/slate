// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-ISYSTEM tests/fixtures/inputs
#include "add.c"
int exercise_add(void) { return main(); }

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
// IR-NEXT:     global %9 .str9: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// IR-NEXT:     fn %1 @printf(%8 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// IR-NEXT:     fn %2 @add(%3 a: i32, %4 b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %5 c: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%3), read<i32>(%4));
// IR-NEXT:         return read<i32>(%5);
// IR-NEXT:     }
// IR-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// IR-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%9)), call<i32, signature=fn(i32, i32) -> i32>(%2, const<i32>(2), const<i32>(3)));
// IR-NEXT:     }
// IR-NEXT:     fn %7 @exercise_add() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<i32, signature=fn() -> i32>(%6);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
