_Bool bool_value;
__bf16 bfloat16_value;
char char_value;
signed char signed_char_value;
unsigned char unsigned_char_value;
short short_value;
unsigned short unsigned_short_value;
signed signed_value;
int int_value;
unsigned unsigned_value;
unsigned int unsigned_int_value;
long long_value;
long int long_int_value;
unsigned long unsigned_long_value;
long long long_long_value;
long long int long_long_int_value;
signed long signed_long_value;
signed long long signed_long_long_value;
unsigned long long unsigned_long_long_value;
unsigned long long int unsigned_long_long_int_value;
unsigned long int unsigned_long_int_value;
float float_value;
_Float16 float16_value;
__fp16 fp16_value;
double double_value;
long double long_double_value;
float _Complex float_complex_value;
double _Complex double_complex_value;
long double _Complex long_double_complex_value;
__int128 int128_value;
unsigned __int128 unsigned_int128_value;
__float128 float128_extension_value;
_BitInt(17) bit_int_value;
unsigned _BitInt(33) unsigned_bit_int_value;

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     global %0 bool_value: bool [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 bfloat16_value: bf16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 char_value: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 signed_char_value: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 unsigned_char_value: u8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 short_value: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 unsigned_short_value: u16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %7 signed_value: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %8 int_value: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %9 unsigned_value: u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %10 unsigned_int_value: u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %11 long_value: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %12 long_int_value: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %13 unsigned_long_value: u64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %14 long_long_value: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %15 long_long_int_value: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %16 signed_long_value: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %17 signed_long_long_value: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %18 unsigned_long_long_value: u64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %19 unsigned_long_long_int_value: u64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %20 unsigned_long_int_value: u64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %21 float_value: f32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %22 float16_value: f16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %23 fp16_value: f16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %24 double_value: f64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %25 long_double_value: f80 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %26 float_complex_value: complex<f32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %27 double_complex_value: complex<f64> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %28 long_double_complex_value: complex<f80> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %29 int128_value: i128 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %30 unsigned_int128_value: u128 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %31 float128_extension_value: f128 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %32 bit_int_value: i17b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %33 unsigned_bit_int_value: u33b [storage=static] [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
