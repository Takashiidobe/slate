// PR c/102989
// { dg-do compile { target bitint } }
// { dg-options "-std=c11 -pedantic-errors" }

_BitInt(63) a;					/* { dg-error "ISO C does not support '_BitInt\\\(63\\\)' before C23" } */
signed _BitInt(15) b;				/* { dg-error "ISO C does not support 'signed _BitInt\\\(15\\\)' before C23" } */
unsigned _BitInt(31) c;				/* { dg-error "ISO C does not support 'unsigned _BitInt\\\(31\\\)' before C23" } */
int d = 21wb;					/* { dg-error "ISO C does not support literal 'wb' suffixes before C23" } */
long long e = 60594869054uwb;			/* { dg-error "ISO C does not support literal 'wb' suffixes before C23" } */
__extension__ _BitInt(63) f;
__extension__ _BitInt(15) g;
__extension__ unsigned _BitInt(31) h;
int i = __extension__ 21wb;
long long j = __extension__ 60594869054uwb;
#if 0wb == 0					/* { dg-error "ISO C does not support literal 'wb' suffixes before C23" } */
#endif
#if 0uwb == 0					/* { dg-error "ISO C does not support literal 'wb' suffixes before C23" } */
#endif

// SLATE-FILECHECK-STD DEFAULT c11
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
// DEFAULT-NEXT:     global %0 a: i63b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 b: i15b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 c: u31b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 d: i32 [storage=static] = widen<i32, reason=assign>(const<i6b>(21)) [linkage=external];
// DEFAULT-NEXT:     global %4 e: i64 [storage=static] = reinterpret<i64, reason=assign, fits=unknown>(widen<u64, reason=assign>(const<u36b>(60594869054))) [linkage=external];
// DEFAULT-NEXT:     global %5 f: i63b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 g: i15b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %7 h: u31b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %8 i: i32 [storage=static] = widen<i32, reason=assign>(const<i6b>(21)) [linkage=external];
// DEFAULT-NEXT:     global %9 j: i64 [storage=static] = reinterpret<i64, reason=assign, fits=unknown>(widen<u64, reason=assign>(const<u36b>(60594869054))) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
