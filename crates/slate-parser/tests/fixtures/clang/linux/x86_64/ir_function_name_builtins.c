// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -std=gnu99

const char *plain(void) { return __func__; }

const char *alias(void) { return __FUNCTION__; }

const char *pretty(int q, char *s) { return __PRETTY_FUNCTION__; }

const char *builtin(void) { return __builtin_FUNCTION(); }

unsigned long measured(void) { return sizeof(__func__); }

char indexed(void) { return __func__[1]; }

const char *retained(void) {
    static const char *held = __func__;
    return held;
}

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
// IR-NEXT:     global %10 .str10: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([112, 108, 97, 105, 110, 0]) [linkage=internal];
// IR-NEXT:     global %11 .str11: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([97, 108, 105, 97, 115, 0]) [linkage=internal];
// IR-NEXT:     global %12 .str12: array<i8, 32> [storage=static] = code_units<array<i8, 32>>([99, 111, 110, 115, 116, 32, 99, 104, 97, 114, 32, 42, 112, 114, 101, 116, 116, 121, 40, 105, 110, 116, 44, 32, 99, 104, 97, 114, 32, 42, 41, 0]) [linkage=internal];
// IR-NEXT:     global %13 .str13: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([98, 117, 105, 108, 116, 105, 110, 0]) [linkage=internal];
// IR-NEXT:     global %14 .str14: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([105, 110, 100, 101, 120, 101, 100, 0]) [linkage=internal];
// IR-NEXT:     global %15 .str15: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([114, 101, 116, 97, 105, 110, 101, 100, 0]) [linkage=internal];
// IR-NEXT:     global %9 held: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(9)>(%15)) [linkage=internal];
// IR-NEXT:     fn %0 @plain() -> ptr<const i8> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return pointer_cast<ptr<const i8>, reason=return>(array_decay<ptr<i8>, length=Some(6)>(%10));
// IR-NEXT:     }
// IR-NEXT:     fn %1 @alias() -> ptr<const i8> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return pointer_cast<ptr<const i8>, reason=return>(array_decay<ptr<i8>, length=Some(6)>(%11));
// IR-NEXT:     }
// IR-NEXT:     fn %2 @pretty(%3 q: i32, %4 s: ptr<i8>) -> ptr<const i8> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return pointer_cast<ptr<const i8>, reason=return>(array_decay<ptr<i8>, length=Some(32)>(%12));
// IR-NEXT:     }
// IR-NEXT:     fn %5 @builtin() -> ptr<const i8> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return pointer_cast<ptr<const i8>, reason=return>(array_decay<ptr<i8>, length=Some(8)>(%13));
// IR-NEXT:     }
// IR-NEXT:     fn %6 @measured() -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<u64>(9);
// IR-NEXT:     }
// IR-NEXT:     fn %7 @indexed() -> i8 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(8)>(%14), const<i32>(1))));
// IR-NEXT:     }
// IR-NEXT:     fn %8 @retained() -> ptr<const i8> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<ptr<const i8>>(%9);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
