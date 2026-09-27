/* { dg-do compile } */

/* N1150 3: Decimal floating types.
   C99 6.7.2: Type specifiers  */

/* Test for the existence of the types.  */
_Decimal32 sd1;
_Decimal64 dd2;
_Decimal128 td3;

#define ARRAY_SIZE      7

static _Decimal32 d32[ARRAY_SIZE];
static _Decimal64 d64[ARRAY_SIZE];
static _Decimal128 d128[ARRAY_SIZE];

extern _Decimal32 ext_d32[ARRAY_SIZE];
extern _Decimal64 ext_d64[ARRAY_SIZE];
extern _Decimal128 ext_d128[ARRAY_SIZE];

/* Test sizes for these types.  */
int ssize[sizeof (_Decimal32) == 4 ? 1 : -1];
int dsize[sizeof (_Decimal64) == 8 ? 1 : -1];
int tsize[sizeof (_Decimal128) == 16 ? 1 : -1];

int salign = __alignof (_Decimal32);
int dalign = __alignof (_Decimal64);
int talign = __alignof (_Decimal128);

/* sizeof operator applied on an array of DFP types is n times the
   size of a single variable of this type.  */

int d32_array_size [sizeof(d32) == ARRAY_SIZE * sizeof(sd1) ? 1 : -1];
int d64_array_size [sizeof(d64) == ARRAY_SIZE * sizeof(dd2) ? 1 : -1];
int d128_array_size [sizeof(d128) == ARRAY_SIZE * sizeof(td3)? 1 : -1];

/* Likewise for extern qualified arrays.  */

int ext_d32_array_size [sizeof(ext_d32) == ARRAY_SIZE * sizeof(sd1) ? 1 : -1];
int ext_d64_array_size [sizeof(ext_d64) == ARRAY_SIZE * sizeof(dd2) ? 1 : -1];
int ext_d128_array_size [sizeof(ext_d128) == ARRAY_SIZE * sizeof(td3)? 1 : -1];

void f()
{
  _Decimal32 d32[ARRAY_SIZE];
  _Decimal64 d64[ARRAY_SIZE];
  _Decimal128 d128[ARRAY_SIZE];

  int d32_array_size [sizeof(d32) == ARRAY_SIZE * sizeof(_Decimal32) ? 1 : -1];
  int d64_array_size [sizeof(d64) == ARRAY_SIZE * sizeof(_Decimal64) ? 1 : -1];
  int d128_array_size [sizeof(d128) == ARRAY_SIZE * sizeof(_Decimal128)? 1 : -1];
}

// SLATE-FILECHECK-FLAVOR gcc
// SLATE-FILECHECK-STD DEFAULT gnu99
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
// DEFAULT-NEXT:     global %0 sd1: d32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 dd2: d64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 td3: d128 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 d32: array<d32, 7> [storage=static] [align=16] [linkage=internal];
// DEFAULT-NEXT:     global %4 d64: array<d64, 7> [storage=static] [align=16] [linkage=internal];
// DEFAULT-NEXT:     global %5 d128: array<d128, 7> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     extern %6 ext_d32: array<d32, 7> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     extern %7 ext_d64: array<d64, 7> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     extern %8 ext_d128: array<d128, 7> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %9 ssize: array<i32, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %10 dsize: array<i32, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %11 tsize: array<i32, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %12 salign: i32 [storage=static] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(4))) [linkage=external];
// DEFAULT-NEXT:     global %13 dalign: i32 [storage=static] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(8))) [linkage=external];
// DEFAULT-NEXT:     global %14 talign: i32 [storage=static] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(16))) [linkage=external];
// DEFAULT-NEXT:     global %15 d32_array_size: array<i32, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %16 d64_array_size: array<i32, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %17 d128_array_size: array<i32, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %18 ext_d32_array_size: array<i32, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %19 ext_d64_array_size: array<i32, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %20 ext_d128_array_size: array<i32, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %21 @f(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %22 d32: array<d32, 7> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %23 d64: array<d64, 7> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %24 d128: array<d128, 7> [storage=automatic];
// DEFAULT-NEXT:         let %25 d32_array_size: array<i32, 1> [storage=automatic];
// DEFAULT-NEXT:         let %26 d64_array_size: array<i32, 1> [storage=automatic];
// DEFAULT-NEXT:         let %27 d128_array_size: array<i32, 1> [storage=automatic];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
