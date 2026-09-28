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
// DEFAULT-NEXT:     global %7 x: volatile array<i32, 1024> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %11 vc: volatile complex<f64> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %14 t0: f64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %15 t1: f64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %16 t2: f64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %17 t3: f64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %1 @feclearexcept(%24 __excepts: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %3 @fetestexcept(%25 __excepts: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %4 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %6 @exit(%26 __status: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %8 @fill_stack() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %9 y: volatile array<i32, 1024> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %10 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %27
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%10, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%10), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %33: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                 let %34: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%33), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%10, read<i32>(%34));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(1024)>(%9), read<i32>(%10))), const<i32>(2146435072));
// DEFAULT-NEXT:         for %28
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%10, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%10), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %35: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                 let %36: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%35), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%10, read<i32>(%36));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(1024)>(%7), read<i32>(%10))), read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(1024)>(%9), read<i32>(%10)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @use_complex(%13 c: complex<f64>) -> void [linkage=external] [inline=never] [definition=emitted] [abi=sysv64(native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<complex<f64>, volatile>(%11, read<complex<f64>>(%13));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @use_stack() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %19 a: complex<f64> [storage=automatic];
// DEFAULT-NEXT:         let %20 b: complex<f64> [storage=automatic];
// DEFAULT-NEXT:         let %21 c: complex<f64> [storage=automatic];
// DEFAULT-NEXT:         let %22 d: complex<f64> [storage=automatic];
// DEFAULT-NEXT:         do %29
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<f64>(real(%19), read<f64>(%14));
// DEFAULT-NEXT:                 write<f64>(imag(%19), read<f64>(%15));
// DEFAULT-NEXT:                 call<void, signature=fn(complex<f64>) -> void, abi=sysv64(native_c) -> void>(%12, read<complex<f64>>(%19));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %30
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<f64>(real(%20), read<f64>(%15));
// DEFAULT-NEXT:                 write<f64>(imag(%20), read<f64>(%16));
// DEFAULT-NEXT:                 call<void, signature=fn(complex<f64>) -> void, abi=sysv64(native_c) -> void>(%12, read<complex<f64>>(%20));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %31
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<f64>(real(%21), read<f64>(%16));
// DEFAULT-NEXT:                 write<f64>(imag(%21), read<f64>(%17));
// DEFAULT-NEXT:                 call<void, signature=fn(complex<f64>) -> void, abi=sysv64(native_c) -> void>(%12, read<complex<f64>>(%21));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %32
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<f64>(real(%22), read<f64>(%17));
// DEFAULT-NEXT:                 write<f64>(imag(%22), read<f64>(%14));
// DEFAULT-NEXT:                 call<void, signature=fn(complex<f64>) -> void, abi=sysv64(native_c) -> void>(%12, read<complex<f64>>(%22));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%1, const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%18);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%3, const<i32>(1)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%6, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
