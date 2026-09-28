/* { dg-do run { target *-*-solaris2.* } } */
/* { dg-options "-std=gnu99" } */

void abort (void);

#pragma align 1(x1)
#pragma align 2(x2)
#pragma align 4(x4)
#pragma align 8(x8,y8,z8)
#pragma align 16(x16)
#pragma align 32(x32)
#pragma align 64(x64)
#pragma align 128(x128)

#define MACRO 128
#define MACRO2(A) A

#pragma align MACRO(y128)
#pragma align MACRO2(MACRO) (z128)

#pragma align 8(not_defined)

#pragma align 9(odd_align)	/* { dg-warning "invalid alignment" } */
#pragma align 256(high_align)	/* { dg-warning "invalid alignment" } */
#pragma align -1(neg_align)	/* { dg-warning "malformed" } */
#pragma align bad_align		/* { dg-warning "malformed" } */
#pragma align 1(bad_align	/* { dg-warning "malformed" } */

int x, x1, x2, x4, x8, y8, z8, x16, x32, x64, x128, y128, z128;

#pragma align 16(x)		/* { dg-warning "must appear before" } */

int
main ()
{
  if (__alignof__ (x4) < 4)
    abort ();

  if (__alignof__ (x8) < 8)
    abort ();

  if (__alignof__ (y8) < 8)
    abort ();

  if (__alignof__ (z8) < 8)
    abort ();

  if (__alignof__ (x16) < 16)
    abort ();

  if (__alignof__ (x32) < 32)
    abort ();

  if (__alignof__ (x64) < 64)
    abort ();

  if (__alignof__ (x128) < 128)
    abort ();

  if (__alignof__ (y128) < 128)
    abort ();

  if (__alignof__ (z128) < 128)
    abort (); 

  return 0;
}

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
// DEFAULT-NEXT:     global %1 x: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 x1: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 x2: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 x4: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 x8: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 y8: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %7 z8: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %8 x16: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %9 x32: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %10 x64: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %11 x128: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %12 y128: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %13 z128: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %14 @main(unprototyped) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if lt<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if lt<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if lt<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if lt<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if lt<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(16))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if lt<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(32))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if lt<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(64))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if lt<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(128))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if lt<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(128))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if lt<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(128))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
