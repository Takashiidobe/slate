/* Test _Float16 NaNs.  */
/* { dg-do run } */
/* { dg-options "-fsignaling-nans" } */
/* { dg-add-options float16 } */
/* { dg-add-options ieee } */
/* { dg-require-effective-target float16_runtime } */
/* { dg-require-effective-target fenv_exceptions } */

#define WIDTH 16
#define EXT   0
/* Tests for _FloatN / _FloatNx types: compile and execution tests for
   NaNs.  Before including this file, define WIDTH as the value N;
   define EXT to 1 for _FloatNx and 0 for _FloatN.  */

#define CONCATX(X, Y)       X##Y
#define CONCAT(X, Y)        CONCATX(X, Y)
#define CONCAT3(X, Y, Z)    CONCAT(CONCAT(X, Y), Z)
#define CONCAT4(W, X, Y, Z) CONCAT(CONCAT(CONCAT(W, X), Y), Z)

#if EXT
#define TYPE   CONCAT3(_Float, WIDTH, x)
#define CST(C) CONCAT4(C, f, WIDTH, x)
#define FN(F)  CONCAT4(F, f, WIDTH, x)
#else
#define TYPE   CONCAT(_Float, WIDTH)
#define CST(C) CONCAT3(C, f, WIDTH)
#define FN(F)  CONCAT3(F, f, WIDTH)
#endif

#include <fenv.h>

extern void exit(int);
extern void abort(void);

volatile TYPE nan_cst  = FN(__builtin_nan)("");
volatile TYPE nans_cst = FN(__builtin_nans)("");

int
// @lowering-fn-begin
// @rewrite-fn-begin
main(void) {
  volatile TYPE r;
  r = nan_cst + nan_cst;
  if (fetestexcept(FE_INVALID))
    abort();
  r = nans_cst + nans_cst;
  if (!fetestexcept(FE_INVALID))
    abort();
  exit(0);
}
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn main() -> std::process::ExitCode {
// LOWERING-DAG:     let mut r: f16 = 0.0f16;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(nan_cst)) };
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} as f32;
// LOWERING-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(nan_cst)) };
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} as f32;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: f16 = {{__v[0-9]+}} as f16;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f16 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{__v[0-9]+}}) };
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = unsafe { fetestexcept({{__v[0-9]+}} as i32) };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(nans_cst)) };
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} as f32;
// LOWERING-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(nans_cst)) };
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} as f32;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: f16 = {{__v[0-9]+}} as f16;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f16 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{__v[0-9]+}}) };
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = unsafe { fetestexcept({{__v[0-9]+}} as i32) };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     unsafe { exit({{__v[0-9]+}} as i32) };
// LOWERING-DAG:     return std::process::ExitCode::SUCCESS;
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn main() -> std::process::ExitCode {
// REWRITES-DAG:     let mut r: f16 = 0.0f16;
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(nan_cst)) };
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} as f32;
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(nan_cst)) };
// REWRITES-X86_64-GNU-DAG:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), ({{__v[0-9]+}} + ({{__v[0-9]+}} as f32)) as f16) };
// REWRITES-AARCH64-GNU-DAG:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{__v[0-9]+}} + {{__v[0-9]+}}) };
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = unsafe { fetestexcept(1 as i32) };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(nans_cst)) };
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} as f32;
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(nans_cst)) };
// REWRITES-X86_64-GNU-DAG:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), ({{__v[0-9]+}} + ({{__v[0-9]+}} as f32)) as f16) };
// REWRITES-AARCH64-GNU-DAG:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{__v[0-9]+}} + {{__v[0-9]+}}) };
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = unsafe { fetestexcept(1 as i32) };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = !({{__v[0-9]+}} != 0);
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe { exit(0 as i32) };
// REWRITES-DAG:     return std::process::ExitCode::SUCCESS;
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
