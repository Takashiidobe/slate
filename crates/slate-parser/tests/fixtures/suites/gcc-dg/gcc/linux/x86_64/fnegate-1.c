/* Copyright (C) 2002 Free Software Foundation.
  
   Test floating point negation produces the expected results.
  
   Written by Roger Sayle, 21st May 2002.  */

/* { dg-do run } */
/* { dg-options "-O2 -ffast-math" } */

extern void abort ();


double
dneg (double x)
{
  return -x;
}

double
dmult (double x)
{
  return -1.0 * x;
}

double
ddiv (double x)
{
  return x / -1.0;
}


float
fneg (float x)
{
  return -x;
}

float
fmult (float x)
{
  return -1.0f * x;
}

float
fdiv (float x)
{
  return x / -1.0f;
}


void
ftest(float src, float dst)
{
  if (fneg (src) != dst)
    abort ();

  if (src != fneg (dst))
    abort ();

  if (fmult (src) != dst)
    abort ();

  if (src != fmult (dst))
    abort ();

  if (fdiv (src) != dst)
    abort ();

  if (src != fdiv(dst))
    abort ();
}

void
dtest(double src, double dst)
{
  if (dneg (src) != dst)
    abort ();

  if (src != dneg (dst))
    abort ();

  if (dmult (src) != dst)
    abort ();

  if (src != dmult (dst))
    abort ();

  if (ddiv (src) != dst)
    abort ();

  if (src != ddiv(dst))
    abort ();
}


int
main ()
{
  ftest (1.0f, -1.0f);
  ftest (2.0f, -2.0f);
  ftest (-3.0f, 3.0f);
  ftest (0.0f, -0.0f);
  ftest (-0.0f, 0.0f);

  dtest (1.0, -1.0);
  dtest (2.0, -2.0);
  dtest (-3.0, 3.0);
  dtest (0.0, -0.0);
  dtest (-0.0, 0.0);

  return 0;
}


// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_dneg:[0-9]+]] @dneg(%[[VALUE_x:[0-9]+]] x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return neg<f64>(read<f64>(%[[VALUE_x]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_dmult:[0-9]+]] @dmult(%[[VALUE_x_2:[0-9]+]] x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(neg<f64>(const<f64>(1.0)), read<f64>(%[[VALUE_x_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ddiv:[0-9]+]] @ddiv(%[[VALUE_x_3:[0-9]+]] x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return div<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE_x_3]]), neg<f64>(const<f64>(1.0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fneg:[0-9]+]] @fneg(%[[VALUE_x_4:[0-9]+]] x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return neg<f32>(read<f32>(%[[VALUE_x_4]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fmult:[0-9]+]] @fmult(%[[VALUE_x_5:[0-9]+]] x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return mul<f32, rounding=nearest_even, exceptions=observable, contract=fast>(neg<f32>(const<f32>(1.0)), read<f32>(%[[VALUE_x_5]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fdiv:[0-9]+]] @fdiv(%[[VALUE_x_6:[0-9]+]] x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return div<f32, rounding=nearest_even, exceptions=observable, contract=fast>(read<f32>(%[[VALUE_x_6]]), neg<f32>(const<f32>(1.0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ftest:[0-9]+]] @ftest(%[[VALUE_src:[0-9]+]] src: f32, %[[VALUE_dst:[0-9]+]] dst: f32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32) -> f32>(%[[VALUE_fneg]], read<f32>(%[[VALUE_src]])), read<f32>(%[[VALUE_dst]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(read<f32>(%[[VALUE_src]]), call<f32, signature=fn(f32) -> f32>(%[[VALUE_fneg]], read<f32>(%[[VALUE_dst]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32) -> f32>(%[[VALUE_fmult]], read<f32>(%[[VALUE_src]])), read<f32>(%[[VALUE_dst]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(read<f32>(%[[VALUE_src]]), call<f32, signature=fn(f32) -> f32>(%[[VALUE_fmult]], read<f32>(%[[VALUE_dst]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32) -> f32>(%[[VALUE_fdiv]], read<f32>(%[[VALUE_src]])), read<f32>(%[[VALUE_dst]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(read<f32>(%[[VALUE_src]]), call<f32, signature=fn(f32) -> f32>(%[[VALUE_fdiv]], read<f32>(%[[VALUE_dst]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_dtest:[0-9]+]] @dtest(%[[VALUE_src_2:[0-9]+]] src: f64, %[[VALUE_dst_2:[0-9]+]] dst: f64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_dneg]], read<f64>(%[[VALUE_src_2]])), read<f64>(%[[VALUE_dst_2]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(read<f64>(%[[VALUE_src_2]]), call<f64, signature=fn(f64) -> f64>(%[[VALUE_dneg]], read<f64>(%[[VALUE_dst_2]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_dmult]], read<f64>(%[[VALUE_src_2]])), read<f64>(%[[VALUE_dst_2]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(read<f64>(%[[VALUE_src_2]]), call<f64, signature=fn(f64) -> f64>(%[[VALUE_dmult]], read<f64>(%[[VALUE_dst_2]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_ddiv]], read<f64>(%[[VALUE_src_2]])), read<f64>(%[[VALUE_dst_2]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(read<f64>(%[[VALUE_src_2]]), call<f64, signature=fn(f64) -> f64>(%[[VALUE_ddiv]], read<f64>(%[[VALUE_dst_2]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(f32, f32) -> void>(%[[VALUE_ftest]], const<f32>(1.0), neg<f32>(const<f32>(1.0)));
// DEFAULT-NEXT:         call<void, signature=fn(f32, f32) -> void>(%[[VALUE_ftest]], const<f32>(2.0), neg<f32>(const<f32>(2.0)));
// DEFAULT-NEXT:         call<void, signature=fn(f32, f32) -> void>(%[[VALUE_ftest]], neg<f32>(const<f32>(3.0)), const<f32>(3.0));
// DEFAULT-NEXT:         call<void, signature=fn(f32, f32) -> void>(%[[VALUE_ftest]], const<f32>(0.0), neg<f32>(const<f32>(0.0)));
// DEFAULT-NEXT:         call<void, signature=fn(f32, f32) -> void>(%[[VALUE_ftest]], neg<f32>(const<f32>(0.0)), const<f32>(0.0));
// DEFAULT-NEXT:         call<void, signature=fn(f64, f64) -> void>(%[[VALUE_dtest]], const<f64>(1.0), neg<f64>(const<f64>(1.0)));
// DEFAULT-NEXT:         call<void, signature=fn(f64, f64) -> void>(%[[VALUE_dtest]], const<f64>(2.0), neg<f64>(const<f64>(2.0)));
// DEFAULT-NEXT:         call<void, signature=fn(f64, f64) -> void>(%[[VALUE_dtest]], neg<f64>(const<f64>(3.0)), const<f64>(3.0));
// DEFAULT-NEXT:         call<void, signature=fn(f64, f64) -> void>(%[[VALUE_dtest]], const<f64>(0.0), neg<f64>(const<f64>(0.0)));
// DEFAULT-NEXT:         call<void, signature=fn(f64, f64) -> void>(%[[VALUE_dtest]], neg<f64>(const<f64>(0.0)), const<f64>(0.0));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
