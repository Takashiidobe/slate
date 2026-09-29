/* PR c/102989 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-std=gnu23" } */

#define expr_has_type(e, t) _Generic (e, default : 0, t : 1)

void
foo (_Complex int ci, _Complex long long cl)
{
  _BitInt(__SIZEOF_INT__ * __CHAR_BIT__ - 1) bi = 0wb;
  _BitInt(__SIZEOF_LONG_LONG__ * __CHAR_BIT__ - 1) bl = 0wb;
  static_assert (expr_has_type (ci + bi, _Complex int));
  static_assert (expr_has_type (cl + bl, _Complex long long));
  static_assert (expr_has_type (bi + ci, _Complex int));
  static_assert (expr_has_type (bl + cl, _Complex long long));
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
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_ci:[0-9]+]] ci: complex<i32>, %[[VALUE_cl:[0-9]+]] cl: complex<i64>) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_bi:[0-9]+]] bi: i31b [storage=automatic] = widen<i31b, reason=assign>(const<i2b>(0));
// DEFAULT-NEXT:         let %[[VALUE_bl:[0-9]+]] bl: i63b [storage=automatic] = widen<i63b, reason=assign>(const<i2b>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
