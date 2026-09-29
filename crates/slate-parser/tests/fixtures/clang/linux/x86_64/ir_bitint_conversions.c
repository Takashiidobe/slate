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
// IR-NEXT:     global %[[VALUE_a8:[0-9]+]] a8: i8b [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_u8:[0-9]+]] u8: u8b [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_a16:[0-9]+]] a16: i16b [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_a32:[0-9]+]] a32: i32b [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_u32b:[0-9]+]] u32b: u32b [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_a33:[0-9]+]] a33: i33b [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_a40:[0-9]+]] a40: i40b [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_a64:[0-9]+]] a64: i64b [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_i:[0-9]+]] i: i32 [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_u:[0-9]+]] u: u32 [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_l:[0-9]+]] l: i64 [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_ul:[0-9]+]] ul: u64 [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_sh:[0-9]+]] sh: i16 [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_bo:[0-9]+]] bo: bool [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 100, 0]) [linkage=internal];
// IR-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE0:[0-9]+]] <unnamed>: ptr<const i8>, ...) -> i32 [linkage=external];
// IR-NEXT:     fn %[[VALUE_unpromoted:[0-9]+]] @unpromoted() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         add<i8b>(read<i8b>(%[[VALUE_a8]]), read<i8b>(%[[VALUE_a8]]));
// IR-NEXT:         read<i8b>(%[[VALUE_a8]]);
// IR-NEXT:         neg<i8b>(read<i8b>(%[[VALUE_a8]]));
// IR-NEXT:         not<i8b>(read<i8b>(%[[VALUE_a8]]));
// IR-NEXT:         shl<i8b>(read<i8b>(%[[VALUE_a8]]), const<i32>(1));
// IR-NEXT:         shr<i8b>(read<i8b>(%[[VALUE_a8]]), const<i32>(1));
// IR-NEXT:         eq<i8b>(read<i8b>(%[[VALUE_a8]]), read<i8b>(%[[VALUE_a8]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_mixed:[0-9]+]] @mixed() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         add<i32>(widen<i32>(read<i8b>(%[[VALUE_a8]])), read<i32>(%[[VALUE_i]]));
// IR-NEXT:         add<i32>(widen<i32>(read<i8b>(%[[VALUE_a8]])), widen<i32>(read<i16>(%[[VALUE_sh]])));
// IR-NEXT:         add<i32>(widen<i32>(read<i8b>(%[[VALUE_a8]])), from_bool<i32>(read<bool>(%[[VALUE_bo]])));
// IR-NEXT:         add<i32>(reinterpret<i32>(read<i32b>(%[[VALUE_a32]])), read<i32>(%[[VALUE_i]]));
// IR-NEXT:         add<u32>(reinterpret<u32>(read<i32b>(%[[VALUE_a32]])), read<u32>(%[[VALUE_u]]));
// IR-NEXT:         add<u32>(reinterpret<u32>(read<u32b>(%[[VALUE_u32b]])), reinterpret<u32>(read<i32>(%[[VALUE_i]])));
// IR-NEXT:         add<i64>(reinterpret<i64>(widen<u64>(read<u32b>(%[[VALUE_u32b]]))), read<i64>(%[[VALUE_l]]));
// IR-NEXT:         add<i33b>(read<i33b>(%[[VALUE_a33]]), reinterpret<i33b>(widen<u33b>(read<u32>(%[[VALUE_u]]))));
// IR-NEXT:         add<i40b>(read<i40b>(%[[VALUE_a40]]), widen<i40b>(read<i32>(%[[VALUE_i]])));
// IR-NEXT:         add<u64>(reinterpret<u64>(widen<i64>(read<i40b>(%[[VALUE_a40]]))), read<u64>(%[[VALUE_ul]]));
// IR-NEXT:         add<i64>(reinterpret<i64>(read<i64b>(%[[VALUE_a64]])), read<i64>(%[[VALUE_l]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_bit_precise_pairs:[0-9]+]] @bit_precise_pairs() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         add<u8b>(reinterpret<u8b>(read<i8b>(%[[VALUE_a8]])), read<u8b>(%[[VALUE_u8]]));
// IR-NEXT:         add<i16b>(widen<i16b>(read<i8b>(%[[VALUE_a8]])), read<i16b>(%[[VALUE_a16]]));
// IR-NEXT:         add<i16b>(reinterpret<i16b>(widen<u16b>(read<u8b>(%[[VALUE_u8]]))), read<i16b>(%[[VALUE_a16]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_contexts:[0-9]+]] @contexts() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         conditional<i8b>(read<bool>(%[[VALUE_bo]]), read<i8b>(%[[VALUE_a8]]), read<i8b>(%[[VALUE_a8]]));
// IR-NEXT:         conditional<i32>(ne<i8b>(read<i8b>(%[[VALUE_a8]]), const<i8b>(0)), read<i32>(%[[VALUE_i]]), widen<i32>(read<i8b>(%[[VALUE_a8]])));
// IR-NEXT:         truncate<i8b>(read<i32>(%[[VALUE_i]]));
// IR-NEXT:         widen<i32>(read<i8b>(%[[VALUE_a8]]));
// IR-NEXT:         call<i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str]])), read<i8b>(%[[VALUE_a8]]));
// IR-NEXT:         switch %[[VALUE1:[0-9]+]] read<i8b>(%[[VALUE_a8]])
// IR-NEXT:             {
// IR-NEXT:                 case %[[VALUE1]] const<i8b>(1):
// IR-NEXT:                     break %[[VALUE1]];
// IR-NEXT:                 default %[[VALUE1]]:
// IR-NEXT:                     break %[[VALUE1]];
// IR-NEXT:             }
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
