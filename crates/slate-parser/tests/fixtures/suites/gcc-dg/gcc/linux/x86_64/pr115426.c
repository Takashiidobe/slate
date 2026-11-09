/* { dg-do compile } */
/* { dg-options "-std=gnu11" } */

_Complex int fcs (_Complex int r)
{
  __asm__("" : "=rm" (__imag__ r));
  return r;
}

_Complex int fcs2 (_Complex int r)
{
  __asm__("" : "=m" (__imag__ r));
  return r;
}

// SLATE-FILECHECK-STD DEFAULT gnu11
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
// DEFAULT-NEXT:     fn %[[VALUE_fcs:[0-9]+]] @fcs(%[[VALUE_r:[0-9]+]] r: complex<i32>) -> complex<i32> [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         asm "" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             lateout 0 "rm" [reg | mem] -> reg width 32 place<i32>(imag(%[[VALUE_r]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<complex<i32>>(%[[VALUE_r]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fcs2:[0-9]+]] @fcs2(%[[VALUE_r_2:[0-9]+]] r: complex<i32>) -> complex<i32> [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         asm "" [dialect=att] [options=nostack] {
// DEFAULT-NEXT:             lateout 0 "m" [mem] width 32 place<i32>(imag(%[[VALUE_r_2]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<complex<i32>>(%[[VALUE_r_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
