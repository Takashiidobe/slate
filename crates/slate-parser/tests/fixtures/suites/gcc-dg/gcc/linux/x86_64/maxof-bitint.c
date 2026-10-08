/* { dg-do compile { target bitint } } */
/* { dg-options "-std=gnu2y" } */

void
limits (void)
{
  _Static_assert (_Maxof (_BitInt (5)) == 15);
  _Static_assert (_Minof (_BitInt (5)) == -16);
  _Static_assert (_Maxof (unsigned _BitInt (5)) == 31);
  _Static_assert (_Minof (unsigned _BitInt (5)) == 0);
}

void
type (void)
{
  _Generic (_Maxof (_BitInt (5)), _BitInt (5): 0);
  _Generic (_Minof (_BitInt (5)), _BitInt (5): 0);
  _Generic (_Maxof (unsigned _BitInt (5)), unsigned _BitInt (5): 0);
  _Generic (_Minof (unsigned _BitInt (5)), unsigned _BitInt (5): 0);
}

// SLATE-FILECHECK-STD DEFAULT gnu2y
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
// DEFAULT-NEXT:     fn %[[VALUE_limits:[0-9]+]] @limits() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_type:[0-9]+]] @type() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         const<i32>(0);
// DEFAULT-NEXT:         const<i32>(0);
// DEFAULT-NEXT:         const<i32>(0);
// DEFAULT-NEXT:         const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
