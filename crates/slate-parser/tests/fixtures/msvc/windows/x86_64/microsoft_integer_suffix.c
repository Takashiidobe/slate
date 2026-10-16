// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir -std=c17

_Static_assert(_Generic(1i8, char: 1, default: 0), "i8 is char");
_Static_assert(_Generic(1ui8, unsigned char: 1, default: 0), "ui8 is unsigned char");
_Static_assert(_Generic(1i16, short: 1, default: 0), "i16 is short");
_Static_assert(_Generic(1i32, int: 1, default: 0), "i32 is int");
_Static_assert(_Generic(1i64, long long: 1, default: 0), "i64 is long long");
_Static_assert(_Generic(1ui64, unsigned long long: 1, default: 0), "ui64");
_Static_assert(1I64 == 1 && 1Ui64 == 1 && 1uI64 == 1, "suffix case");
_Static_assert(-1ui16 < 0, "ui16 promotes to int");
_Static_assert(0xFFi8 < 0, "i8 wraps");
_Static_assert(300i8 == 44, "i8 wraps decimal");
_Static_assert(sizeof(4294967296i32) == 4, "i32 keeps its type");
_Static_assert(18446744073709551615i64 == -1, "i64 wraps");

#if 0xFFi8 >= 0 || -1ui8 > 0 || 0x100000000i32 != 0 || !(-1ui32 > 0)
#error preprocessor wraps and promotes like cl.exe
#endif

long long max = 9223372036854775807i64;
long long min = (-9223372036854775807i64 - 1);
unsigned long long all = 0xffffffffffffffffui64;
long long remainder = 10i64 % 3;
int wrapped = 0x80000000i32;
char narrow = 300i8;

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-pc-windows-msvc" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f64;
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
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     global %[[VALUE_max:[0-9]+]] max: i64 [storage=static] = const<i64>(9223372036854775807) [linkage=external];
// IR-NEXT:     global %[[VALUE_min:[0-9]+]] min: i64 [storage=static] = sub<i64>(neg<i64>(const<i64>(9223372036854775807)), widen<i64>(const<i32>(1))) [linkage=external];
// IR-NEXT:     global %[[VALUE_all:[0-9]+]] all: u64 [storage=static] = const<u64>(18446744073709551615) [linkage=external];
// IR-NEXT:     global %[[VALUE_remainder:[0-9]+]] remainder: i64 [storage=static] = rem<i64>(const<i64>(10), widen<i64>(const<i32>(3))) [linkage=external];
// IR-NEXT:     global %[[VALUE_wrapped:[0-9]+]] wrapped: i32 [storage=static] = const<i32>(-2147483648) [linkage=external];
// IR-NEXT:     global %[[VALUE_narrow:[0-9]+]] narrow: i8 [storage=static] = const<i8>(44) [linkage=external];
// IR-NEXT: }
// SLATE-FILECHECK-END IR
