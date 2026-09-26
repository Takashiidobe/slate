// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

// C23 6.3.1.1: bit-precise integers are exempt from the integer promotions, and
// rank at equal width is below the standard integer type. These rules are not
// gated on the standard mode: clang and gcc accept _BitInt as an extension in
// c89 through c17 and apply the same conversions there. Every result type below
// was verified against clang 22.1.8 and gcc 16.2.1, in every standard mode.

_BitInt(8) a8;
unsigned _BitInt(8) u8;
_BitInt(16) a16;
_BitInt(32) a32;
unsigned _BitInt(32) u32b;
_BitInt(33) a33;
_BitInt(40) a40;
_BitInt(64) a64;
int i;
unsigned u;
long l;
unsigned long ul;
short sh;
_Bool bo;

int printf(const char *, ...);

// no promotion: the declared bit-precise type survives
void unpromoted(void) {
    a8 + a8;
    +a8;
    -a8;
    ~a8;
    a8 << 1;
    a8 >> 1;
    a8 == a8;
}

// mixed with standard types, rank decides
void mixed(void) {
    a8 + i;
    a8 + sh;
    a8 + bo;
    a32 + i;
    a32 + u;
    u32b + i;
    u32b + l;
    a33 + u;
    a40 + i;
    a40 + ul;
    a64 + l;
}

// two bit-precise operands
void bit_precise_pairs(void) {
    a8 + u8;
    a8 + a16;
    u8 + a16;
}

void contexts(void) {
    bo ? a8 : a8;
    a8 ? i : a8;
    (_BitInt(8))i;
    (int)a8;
    printf("%d", a8);
    switch (a8) {
    case 1:
        break;
    default:
        break;
    }
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
// IR-NEXT:     global %0 a8: i8b [storage=static] [linkage=external];
// IR-NEXT:     global %1 u8: u8b [storage=static] [linkage=external];
// IR-NEXT:     global %2 a16: i16b [storage=static] [linkage=external];
// IR-NEXT:     global %3 a32: i32b [storage=static] [linkage=external];
// IR-NEXT:     global %4 u32b: u32b [storage=static] [linkage=external];
// IR-NEXT:     global %5 a33: i33b [storage=static] [linkage=external];
// IR-NEXT:     global %6 a40: i40b [storage=static] [linkage=external];
// IR-NEXT:     global %7 a64: i64b [storage=static] [linkage=external];
// IR-NEXT:     global %8 i: i32 [storage=static] [linkage=external];
// IR-NEXT:     global %9 u: u32 [storage=static] [linkage=external];
// IR-NEXT:     global %10 l: i64 [storage=static] [linkage=external];
// IR-NEXT:     global %11 ul: u64 [storage=static] [linkage=external];
// IR-NEXT:     global %12 sh: i16 [storage=static] [linkage=external];
// IR-NEXT:     global %13 bo: bool [storage=static] [linkage=external];
// IR-NEXT:     global %20 .str20: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 100, 0]) [linkage=internal];
// IR-NEXT:     fn %14 @printf(%19 <unnamed>: ptr<const i8>, ...) -> i32 [linkage=external];
// IR-NEXT:     fn %15 @unpromoted() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         add<i8b>(read<i8b>(%0), read<i8b>(%0));
// IR-NEXT:         read<i8b>(%0);
// IR-NEXT:         neg<i8b>(read<i8b>(%0));
// IR-NEXT:         not<i8b>(read<i8b>(%0));
// IR-NEXT:         shl<i8b>(read<i8b>(%0), const<i32>(1));
// IR-NEXT:         shr<i8b>(read<i8b>(%0), const<i32>(1));
// IR-NEXT:         eq<i8b>(read<i8b>(%0), read<i8b>(%0));
// IR-NEXT:     }
// IR-NEXT:     fn %16 @mixed() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         add<i32>(widen<i32>(read<i8b>(%0)), read<i32>(%8));
// IR-NEXT:         add<i32>(widen<i32>(read<i8b>(%0)), widen<i32>(read<i16>(%12)));
// IR-NEXT:         add<i32>(widen<i32>(read<i8b>(%0)), from_bool<i32>(read<bool>(%13)));
// IR-NEXT:         add<i32>(reinterpret<i32>(read<i32b>(%3)), read<i32>(%8));
// IR-NEXT:         add<u32>(reinterpret<u32>(read<i32b>(%3)), read<u32>(%9));
// IR-NEXT:         add<u32>(reinterpret<u32>(read<u32b>(%4)), reinterpret<u32>(read<i32>(%8)));
// IR-NEXT:         add<i64>(reinterpret<i64>(widen<u64>(read<u32b>(%4))), read<i64>(%10));
// IR-NEXT:         add<i33b>(read<i33b>(%5), reinterpret<i33b>(widen<u33b>(read<u32>(%9))));
// IR-NEXT:         add<i40b>(read<i40b>(%6), widen<i40b>(read<i32>(%8)));
// IR-NEXT:         add<u64>(reinterpret<u64>(widen<i64>(read<i40b>(%6))), read<u64>(%11));
// IR-NEXT:         add<i64>(reinterpret<i64>(read<i64b>(%7)), read<i64>(%10));
// IR-NEXT:     }
// IR-NEXT:     fn %17 @bit_precise_pairs() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         add<u8b>(reinterpret<u8b>(read<i8b>(%0)), read<u8b>(%1));
// IR-NEXT:         add<i16b>(widen<i16b>(read<i8b>(%0)), read<i16b>(%2));
// IR-NEXT:         add<i16b>(reinterpret<i16b>(widen<u16b>(read<u8b>(%1))), read<i16b>(%2));
// IR-NEXT:     }
// IR-NEXT:     fn %18 @contexts() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         conditional<i8b>(read<bool>(%13), read<i8b>(%0), read<i8b>(%0));
// IR-NEXT:         conditional<i32>(ne<i8b>(read<i8b>(%0), const<i8b>(0)), read<i32>(%8), widen<i32>(read<i8b>(%0)));
// IR-NEXT:         truncate<i8b>(read<i32>(%8));
// IR-NEXT:         widen<i32>(read<i8b>(%0));
// IR-NEXT:         call<i32>(%14, pointer_cast<ptr<const i8>>(array_decay<ptr<i8>, length=Some(3)>(%20)), read<i8b>(%0));
// IR-NEXT:         switch %21 read<i8b>(%0)
// IR-NEXT:             {
// IR-NEXT:                 case %21 const<i8b>(1):
// IR-NEXT:                     break %21;
// IR-NEXT:                 default %21:
// IR-NEXT:                     break %21;
// IR-NEXT:             }
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
