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
// DEFAULT-NEXT:     global %[[VALUE_x:[0-9]+]] x: volatile array<i32, 1024> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_vc:[0-9]+]] vc: volatile complex<f64> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_t0:[0-9]+]] t0: f64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_t1:[0-9]+]] t1: f64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_t2:[0-9]+]] t2: f64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_t3:[0-9]+]] t3: f64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_feclearexcept:[0-9]+]] @feclearexcept(%[[VALUE___excepts:[0-9]+]] __excepts: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fetestexcept:[0-9]+]] @fetestexcept(%[[VALUE___excepts_2:[0-9]+]] __excepts: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE___status:[0-9]+]] __status: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_fill_stack:[0-9]+]] @fill_stack() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_y:[0-9]+]] y: volatile array<i32, 1024> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(1024)>(%[[VALUE_y]]), read<i32>(%[[VALUE_i]]))), const<i32>(2146435072));
// DEFAULT-NEXT:         for %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(1024)>(%[[VALUE_x]]), read<i32>(%[[VALUE_i]]))), read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(1024)>(%[[VALUE_y]]), read<i32>(%[[VALUE_i]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_use_complex:[0-9]+]] @use_complex(%[[VALUE_c:[0-9]+]] c: complex<f64>) -> void [linkage=external] [inline=never] [definition=emitted] [abi=sysv64(native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<complex<f64>, volatile>(%[[VALUE_vc]], read<complex<f64>>(%[[VALUE_c]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_use_stack:[0-9]+]] @use_stack() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: complex<f64> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: complex<f64> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_c_2:[0-9]+]] c: complex<f64> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_d:[0-9]+]] d: complex<f64> [storage=automatic];
// DEFAULT-NEXT:         do %[[VALUE6:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<f64>(real(%[[VALUE_a]]), read<f64>(%[[VALUE_t0]]));
// DEFAULT-NEXT:                 write<f64>(imag(%[[VALUE_a]]), read<f64>(%[[VALUE_t1]]));
// DEFAULT-NEXT:                 call<void, signature=fn(complex<f64>) -> void, abi=sysv64(native_c) -> void>(%[[VALUE_use_complex]], read<complex<f64>>(%[[VALUE_a]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE7:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<f64>(real(%[[VALUE_b]]), read<f64>(%[[VALUE_t1]]));
// DEFAULT-NEXT:                 write<f64>(imag(%[[VALUE_b]]), read<f64>(%[[VALUE_t2]]));
// DEFAULT-NEXT:                 call<void, signature=fn(complex<f64>) -> void, abi=sysv64(native_c) -> void>(%[[VALUE_use_complex]], read<complex<f64>>(%[[VALUE_b]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE8:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<f64>(real(%[[VALUE_c_2]]), read<f64>(%[[VALUE_t2]]));
// DEFAULT-NEXT:                 write<f64>(imag(%[[VALUE_c_2]]), read<f64>(%[[VALUE_t3]]));
// DEFAULT-NEXT:                 call<void, signature=fn(complex<f64>) -> void, abi=sysv64(native_c) -> void>(%[[VALUE_use_complex]], read<complex<f64>>(%[[VALUE_c_2]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE9:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<f64>(real(%[[VALUE_d]]), read<f64>(%[[VALUE_t3]]));
// DEFAULT-NEXT:                 write<f64>(imag(%[[VALUE_d]]), read<f64>(%[[VALUE_t0]]));
// DEFAULT-NEXT:                 call<void, signature=fn(complex<f64>) -> void, abi=sysv64(native_c) -> void>(%[[VALUE_use_complex]], read<complex<f64>>(%[[VALUE_d]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_fill_stack]]);
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%[[VALUE_feclearexcept]], const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_use_stack]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_fetestexcept]], const<i32>(1)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
