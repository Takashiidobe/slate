/* PR middle-end/33088 */
/* Origin: Joseph S. Myers <jsm28@gcc.gnu.org> */

/* { dg-do run { target i?86-*-linux* i?86-*-gnu* x86_64-*-linux* } } */
/* { dg-options "-std=c99 -O -ffloat-store -lm" } */

#include <fenv.h>
#include <stdlib.h>

volatile int x[1024];

void __attribute__((noinline))
fill_stack (void)
{
  volatile int y[1024];
  int i;
  for (i = 0; i < 1024; i++)
    y[i] = 0x7ff00000;
  for (i = 0; i < 1024; i++)
    x[i] = y[i];
}

volatile _Complex double vc;

void __attribute__((noinline))
use_complex (_Complex double c)
{
  vc = c;
}

double t0, t1, t2, t3;

#define USE_COMPLEX(X, R, C) \
  do { __real__ X = R; __imag__ X = C; use_complex (X); } while (0)

void __attribute__((noinline))
use_stack (void)
{
  _Complex double a, b, c, d;
  USE_COMPLEX (a, t0, t1);
  USE_COMPLEX (b, t1, t2);
  USE_COMPLEX (c, t2, t3);
  USE_COMPLEX (d, t3, t0);
}

int
main (void)
{
  fill_stack ();
  feclearexcept (FE_INVALID);
  use_stack ();
  if (fetestexcept (FE_INVALID))
    abort ();
  exit (0);
}

// SLATE-FILECHECK-STD DEFAULT c99
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
// DEFAULT-NEXT:     global %4 x: volatile array<i32, 1024> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %8 vc: volatile complex<f64> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %11 t0: f64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %12 t1: f64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %13 t2: f64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %14 t3: f64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @feclearexcept(%21 __excepts: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @fetestexcept(%22 __excepts: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @exit(%23 __status: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %5 @fill_stack() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %6 y: volatile array<i32, 1024> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %7 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %24
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%7, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%7), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %30: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                 let %31: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%30), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%7, read<i32>(%31));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(1024)>(%6), read<i32>(%7))), const<i32>(2146435072));
// DEFAULT-NEXT:         for %25
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%7, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%7), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %32: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                 let %33: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%32), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%7, read<i32>(%33));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(1024)>(%4), read<i32>(%7))), read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(1024)>(%6), read<i32>(%7)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @use_complex(%10 c: complex<f64>) -> void [linkage=external] [inline=never] [definition=emitted] [abi=sysv64(coerce<f64, f64>) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<complex<f64>, volatile>(%8, read<complex<f64>>(%10));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @use_stack() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %16 a: complex<f64> [storage=automatic];
// DEFAULT-NEXT:         let %17 b: complex<f64> [storage=automatic];
// DEFAULT-NEXT:         let %18 c: complex<f64> [storage=automatic];
// DEFAULT-NEXT:         let %19 d: complex<f64> [storage=automatic];
// DEFAULT-NEXT:         do %26
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<f64>(real(%16), read<f64>(%11));
// DEFAULT-NEXT:                 write<f64>(imag(%16), read<f64>(%12));
// DEFAULT-NEXT:                 call<void, signature=fn(complex<f64>) -> void, abi=sysv64(coerce<f64, f64>) -> void>(%9, read<complex<f64>>(%16));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %27
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<f64>(real(%17), read<f64>(%12));
// DEFAULT-NEXT:                 write<f64>(imag(%17), read<f64>(%13));
// DEFAULT-NEXT:                 call<void, signature=fn(complex<f64>) -> void, abi=sysv64(coerce<f64, f64>) -> void>(%9, read<complex<f64>>(%17));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %28
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<f64>(real(%18), read<f64>(%13));
// DEFAULT-NEXT:                 write<f64>(imag(%18), read<f64>(%14));
// DEFAULT-NEXT:                 call<void, signature=fn(complex<f64>) -> void, abi=sysv64(coerce<f64, f64>) -> void>(%9, read<complex<f64>>(%18));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %29
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<f64>(real(%19), read<f64>(%14));
// DEFAULT-NEXT:                 write<f64>(imag(%19), read<f64>(%11));
// DEFAULT-NEXT:                 call<void, signature=fn(complex<f64>) -> void, abi=sysv64(coerce<f64, f64>) -> void>(%9, read<complex<f64>>(%19));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%0, const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%15);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%1, const<i32>(1)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%3, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
