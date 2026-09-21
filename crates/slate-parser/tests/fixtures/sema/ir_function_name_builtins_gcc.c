// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-FLAVOR gcc
// SLATE-FILECHECK-ARGS --dump-ir -std=gnu99

static unsigned long pretty(int q, char *s) { return sizeof(__PRETTY_FUNCTION__); }

const char *spelled(int q, char *s) { return __PRETTY_FUNCTION__; }

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
// IR-NEXT:     global %6 .str6: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([115, 112, 101, 108, 108, 101, 100, 0]) [linkage=internal];
// IR-NEXT:     fn %0 @pretty(%1 q: i32, %2 s: ptr<i8>) -> u64 [linkage=internal] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<u64>(7);
// IR-NEXT:     }
// IR-NEXT:     fn %3 @spelled(%4 q: i32, %5 s: ptr<i8>) -> ptr<const i8> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return pointer_cast<ptr<const i8>, reason=return>(array_decay<ptr<i8>, length=Some(8)>(%6));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
