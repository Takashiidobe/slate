// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

struct Pair { short lo, hi; };

unsigned bits(float x) { return __builtin_bit_cast(unsigned, x); }

double from_bits(unsigned long long x) { return __builtin_bit_cast(double, x); }

struct Pair split(int x) { return __builtin_bit_cast(struct Pair, x); }

int join(struct Pair p) { return __builtin_bit_cast(int, p); }

unsigned long address(int *p) { return __builtin_bit_cast(unsigned long, p); }

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
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type0 Pair = struct {
// IR-NEXT:         field0 lo: i16;
// IR-NEXT:         field1 hi: i16;
// IR-NEXT:     } [size=4, align=2, offsets=[0, 2]];
// IR-NEXT:     fn %1 @bits(%2 x: f32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return bit_cast<u32, reason=explicit>(read<f32>(%2));
// IR-NEXT:     }
// IR-NEXT:     fn %3 @from_bits(%4 x: u64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return bit_cast<f64, reason=explicit>(read<u64>(%4));
// IR-NEXT:     }
// IR-NEXT:     fn %5 @split(%6 x: i32) -> @type0 [linkage=external] [abi=sysv64(scalar) -> coerce<i32>] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type0, reason=return>(bit_cast<@type0, reason=explicit>(read<i32>(%6)));
// IR-NEXT:     }
// IR-NEXT:     fn %7 @join(%8 p: @type0) -> i32 [linkage=external] [abi=sysv64(coerce<i32>) -> scalar] [fallthrough=ub_if_used] {
// IR-NEXT:         return bit_cast<i32, reason=explicit>(read<@type0>(%8));
// IR-NEXT:     }
// IR-NEXT:     fn %9 @address(%10 p: ptr<i32>) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return bit_cast<u64, reason=explicit>(read<ptr<i32>>(%10));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
