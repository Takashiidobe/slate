/* { dg-do run }
   { dg-require-effective-target c99_runtime } */

/*
  Program to test complex divide for correct results on selected values.
  Checking known failure points.
*/

#include <float.h>

extern void abort(void);
extern void exit(int);

extern int ilogbl(long double);
int        match(long double _Complex, long double _Complex);

#define SMALL  LDBL_MIN
#define MAXBIT LDBL_MANT_DIG
#define ERRLIM 6

/*
  Compare c (computed value) with z (expected value).
  Return 0 if within allowed range.  Return 1 if not.
*/
int match(long double _Complex c, long double _Complex z) {
  long double rz, iz, rc, ic;
  long double rerr, ierr, rmax;
  int         biterr;
  rz = __real__ z;
  iz = __imag__ z;
  rc = __real__ c;
  ic = __imag__ c;

  if (__builtin_fabsl(rz) > SMALL) {
    rerr = __builtin_fabsl(rz - rc) / __builtin_fabsl(rz);
  } else if (__builtin_fabsl(rz) == 0.0) {
    rerr = __builtin_fabsl(rc);
  } else {
    rerr = __builtin_fabsl(rz - rc) / SMALL;
  }

  if (__builtin_fabsl(iz) > SMALL) {
    ierr = __builtin_fabsl(iz - ic) / __builtin_fabsl(iz);
  } else if (__builtin_fabsl(iz) == 0.0) {
    ierr = __builtin_fabsl(ic);
  } else {
    ierr = __builtin_fabsl(iz - ic) / SMALL;
  }
  rmax   = __builtin_fmaxl(rerr, ierr);
  biterr = 0;
  if (rmax != 0.0) {
    biterr = ilogbl(rmax) + MAXBIT + 1;
  }

  if (biterr >= ERRLIM)
    return 0;
  else
    return 1;
}

int main(int argc, char **argv) {
  long double _Complex a, b, c, z;
  long double xr[4], xi[4], yr[4], yi[4], zr[4], zi[4];
  long double cr, ci;
  int         i;
  int         ok = 1;

#if (LDBL_MAX_EXP < 2048)
  /*
    Test values when mantissa is 11 or fewer bits.  Either LDBL is
    using DBL on this platform or we are using IBM extended double
    precision. Test values will be automatically truncated when
    the available precision is smaller than the explicit precision.
  */
  xr[0] = -0x1.16e7fad79e45ep+651;
  xi[0] = -0x1.f7f75b94c6c6ap-860;
  yr[0] = -0x1.2f40d8ff7e55ep+245;
  yi[0] = -0x0.0000000004ebcp-968;
  zr[0] = 0x1.d6e4b0e2828694570ba839070beep+405L;
  zi[0] = -0x1.e9095e311e70498db810196259b7p-846L;

  xr[1] = -0x1.21ff587f953d3p-310;
  xi[1] = -0x1.5a526dcc59960p+837;
  yr[1] = 0x1.b88b8b552eaadp+735;
  yi[1] = -0x1.873e2d6544d92p-327;
  zr[1] = 0x1.65734a88b2ddff699c482ee8eef6p-961L;
  zi[1] = -0x1.927e85b8b576f94a797a1bcb733dp+101L;

  xr[2] = 0x1.4612e41aa8080p-846;
  xi[2] = -0x0.0000000613e07p-968;
  yr[2] = 0x1.df9cd0d58caafp-820;
  yi[2] = -0x1.e47051a9036dbp-584;
  zr[2] = 0x1.9b194f3aaadea545174c5372d8p-415L;
  zi[2] = 0x1.58a00ab740a6ad3249002f2b79p-263L;

  xr[3] = 0x1.cb27eece7c585p-355;
  xi[3] = 0x0.000000223b8a8p-968;
  yr[3] = -0x1.74e7ed2b9189fp-22;
  yi[3] = 0x1.3d80439e9a119p-731;
  zr[3] = -0x1.3b35ed806ae5a2a8cc1c9a96931dp-333L;
  zi[3] = -0x1.7802c17c774895bd541adeb200p-974L;
#else
  /*
    Test values intended for either IEEE128 or Intel80 formats.  In
    either case, 15 bits of exponent are available.  Test values will
    be automatically truncated when the available precision is smaller
    than the explicit precision.
  */
  xr[0] = -0x9.c793985b7d029d90p-8480L;
  xi[0] = 0x8.018745ffa61a8fe0p+16329L;
  yr[0] = -0xe.d5bee9c523a35ad0p-15599L;
  yi[0] = -0xa.8c93c5a4f94128f0p+869L;
  zr[0] = -0x1.849178451c035b95d16311d0efdap+15459L;
  zi[0] = -0x1.11375ed2c1f58b9d047ab64aed97p-1008L;

  xr[1] = 0xb.68e44bc6d0b91a30p+16026L;
  xi[1] = 0xb.ab10f5453e972f30p-14239L;
  yr[1] = 0x8.8cbd470705428ff0p-16350L;
  yi[1] = -0xa.0c1cbeae4e4b69f0p+347L;
  zr[1] = 0x1.eec40848785e500d9f0945ab58d3p-1019L;
  zi[1] = 0x1.22b6b579927a3f238b772bb6dc95p+15679L;

  xr[2] = -0x9.e8c093a43b546a90p+15983L;
  xi[2] = 0xc.95b18274208311e0p-2840L;
  yr[2] = -0x8.dedb729b5c1b2ec0p+8L;
  yi[2] = 0xa.a49fb81b24738370p-16385L;
  zr[2] = 0x1.1df99ee89bb118f3201369e06576p+15975L;
  zi[2] = 0x1.571e7ef904d6b6eee7acb0dcf098p-418L;

  xr[3] = 0xc.4687f251c0f48bd0p-3940L;
  xi[3] = -0xe.a3f2138992d85fa0p+15598L;
  yr[3] = 0xe.4b0c25c3d5ebb830p-16344L;
  yi[3] = -0xa.6cbf1ba80f7b97a0p+78L;
  zr[3] = 0x1.6785ba23bfb744cee97b4142348bp+15520L;
  zi[3] = -0x1.ecee7b8c7bdd36237eb538324289p-902L;
#endif

  for (i = 0; i < 4; i++) {
    __real__ a = xr[i];
    __imag__ a = xi[i];
    __real__ b = yr[i];
    __imag__ b = yi[i];
    __real__ z = zr[i];
    __imag__ z = zi[i];
    c          = a / b;
    cr         = __real__ c;
    ci         = __imag__ c;

    if (!match(c, z)) {
      ok = 0;
    }
  }
  if (!ok)
    abort();
  exit(0);
}



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
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_ilogbl:[0-9]+]] @ilogbl(%[[VALUE1:[0-9]+]] <unnamed>: f80) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_match:[0-9]+]] @match(%[[VALUE_c:[0-9]+]] c: complex<f80>, %[[VALUE_z:[0-9]+]] z: complex<f80>) -> i32 [linkage=external] [abi=sysv64(byval<align=16>, byval<align=16>) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_rz:[0-9]+]] rz: f80 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_iz:[0-9]+]] iz: f80 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_rc:[0-9]+]] rc: f80 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ic:[0-9]+]] ic: f80 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_rerr:[0-9]+]] rerr: f80 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ierr:[0-9]+]] ierr: f80 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_rmax:[0-9]+]] rmax: f80 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_biterr:[0-9]+]] biterr: i32 [storage=automatic];
// DEFAULT-NEXT:         write<f80>(%[[VALUE_rz]], read<f80>(real(%[[VALUE_z]])));
// DEFAULT-NEXT:         write<f80>(%[[VALUE_iz]], read<f80>(imag(%[[VALUE_z]])));
// DEFAULT-NEXT:         write<f80>(%[[VALUE_rc]], read<f80>(real(%[[VALUE_c]])));
// DEFAULT-NEXT:         write<f80>(%[[VALUE_ic]], read<f80>(imag(%[[VALUE_c]])));
// DEFAULT-NEXT:         if gt<f80, exceptions=ignore>(call<f80, signature=fn(f80) -> f80>(%[[VALUE___builtin_fabsl:[0-9]+]], read<f80>(%[[VALUE_rz]])), const<f80>(3.36210314311209350626E-4932))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<f80>(%[[VALUE_rerr]], div<f80, rounding=nearest_even, exceptions=ignore, contract=on>(call<f80, signature=fn(f80) -> f80>(%[[VALUE___builtin_fabsl]], sub<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%[[VALUE_rz]]), read<f80>(%[[VALUE_rc]]))), call<f80, signature=fn(f80) -> f80>(%[[VALUE___builtin_fabsl]], read<f80>(%[[VALUE_rz]]))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if eq<f80, exceptions=ignore>(call<f80, signature=fn(f80) -> f80>(%[[VALUE___builtin_fabsl]], read<f80>(%[[VALUE_rz]])), float_widen<f80, reason=usual_arith>(const<f64>(0.0)))
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f80>(%[[VALUE_rerr]], call<f80, signature=fn(f80) -> f80>(%[[VALUE___builtin_fabsl]], read<f80>(%[[VALUE_rc]])));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f80>(%[[VALUE_rerr]], div<f80, rounding=nearest_even, exceptions=ignore, contract=on>(call<f80, signature=fn(f80) -> f80>(%[[VALUE___builtin_fabsl]], sub<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%[[VALUE_rz]]), read<f80>(%[[VALUE_rc]]))), const<f80>(3.36210314311209350626E-4932)));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         if gt<f80, exceptions=ignore>(call<f80, signature=fn(f80) -> f80>(%[[VALUE___builtin_fabsl]], read<f80>(%[[VALUE_iz]])), const<f80>(3.36210314311209350626E-4932))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<f80>(%[[VALUE_ierr]], div<f80, rounding=nearest_even, exceptions=ignore, contract=on>(call<f80, signature=fn(f80) -> f80>(%[[VALUE___builtin_fabsl]], sub<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%[[VALUE_iz]]), read<f80>(%[[VALUE_ic]]))), call<f80, signature=fn(f80) -> f80>(%[[VALUE___builtin_fabsl]], read<f80>(%[[VALUE_iz]]))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if eq<f80, exceptions=ignore>(call<f80, signature=fn(f80) -> f80>(%[[VALUE___builtin_fabsl]], read<f80>(%[[VALUE_iz]])), float_widen<f80, reason=usual_arith>(const<f64>(0.0)))
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f80>(%[[VALUE_ierr]], call<f80, signature=fn(f80) -> f80>(%[[VALUE___builtin_fabsl]], read<f80>(%[[VALUE_ic]])));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f80>(%[[VALUE_ierr]], div<f80, rounding=nearest_even, exceptions=ignore, contract=on>(call<f80, signature=fn(f80) -> f80>(%[[VALUE___builtin_fabsl]], sub<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%[[VALUE_iz]]), read<f80>(%[[VALUE_ic]]))), const<f80>(3.36210314311209350626E-4932)));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         write<f80>(%[[VALUE_rmax]], call<f80, signature=fn(f80, f80) -> f80>(%[[VALUE___builtin_fmaxl:[0-9]+]], read<f80>(%[[VALUE_rerr]]), read<f80>(%[[VALUE_ierr]])));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_biterr]], const<i32>(0));
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(read<f80>(%[[VALUE_rmax]]), float_widen<f80, reason=usual_arith>(const<f64>(0.0)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_biterr]], add<i32, overflow=ub>(add<i32, overflow=ub>(call<i32, signature=fn(f80) -> i32>(%[[VALUE_ilogbl]], read<f80>(%[[VALUE_rmax]])), const<i32>(64)), const<i32>(1)));
// DEFAULT-NEXT:                 add<i32, overflow=ub>(add<i32, overflow=ub>(call<i32, signature=fn(f80) -> i32>(%[[VALUE_ilogbl]], read<f80>(%[[VALUE_rmax]])), const<i32>(64)), const<i32>(1));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if ge<i32>(read<i32>(%[[VALUE_biterr]]), const<i32>(6))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_fabsl]] @__builtin_fabsl(%[[VALUE2:[0-9]+]] <unnamed>: f80) -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_fmaxl]] @__builtin_fmaxl(%[[VALUE3:[0-9]+]] <unnamed>: f80, %[[VALUE4:[0-9]+]] <unnamed>: f80) -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(%[[VALUE_argc:[0-9]+]] argc: i32, %[[VALUE_argv:[0-9]+]] argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: complex<f80> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: complex<f80> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_c_2:[0-9]+]] c: complex<f80> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_z_2:[0-9]+]] z: complex<f80> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_xr:[0-9]+]] xr: array<f80, 4> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_xi:[0-9]+]] xi: array<f80, 4> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_yr:[0-9]+]] yr: array<f80, 4> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_yi:[0-9]+]] yi: array<f80, 4> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_zr:[0-9]+]] zr: array<f80, 4> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_zi:[0-9]+]] zi: array<f80, 4> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_cr:[0-9]+]] cr: f80 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ci:[0-9]+]] ci: f80 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ok:[0-9]+]] ok: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         write<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(4)>(%[[VALUE_xr]]), const<i32>(0))), neg<f80>(const<f80>(1.80284204551487856924E-2552)));
// DEFAULT-NEXT:         write<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(4)>(%[[VALUE_xi]]), const<i32>(0))), const<f80>(2.64370611144962632511E+4916));
// DEFAULT-NEXT:         write<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(4)>(%[[VALUE_yr]]), const<i32>(0))), neg<f80>(const<f80>(2.53736858833639415543E-4695)));
// DEFAULT-NEXT:         write<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(4)>(%[[VALUE_yi]]), const<i32>(0))), neg<f80>(const<f80>(4.15224402656888868824E+262)));
// DEFAULT-NEXT:         write<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(4)>(%[[VALUE_zr]]), const<i32>(0))), neg<f80>(const<f80>(6.3669333847755379659E+4653)));
// DEFAULT-NEXT:         write<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(4)>(%[[VALUE_zi]]), const<i32>(0))), neg<f80>(const<f80>(3.89072912651265618456E-304)));
// DEFAULT-NEXT:         write<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(4)>(%[[VALUE_xr]]), const<i32>(1))), const<f80>(2.31199032499427573017E+4825));
// DEFAULT-NEXT:         write<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(4)>(%[[VALUE_xi]]), const<i32>(1))), const<f80>(5.02223035999083336654E-4286));
// DEFAULT-NEXT:         write<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(4)>(%[[VALUE_yr]]), const<i32>(1))), const<f80>(1.23459631818840338799E-4921));
// DEFAULT-NEXT:         write<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(4)>(%[[VALUE_yi]]), const<i32>(1))), neg<f80>(const<f80>(2.88043748281638274857E+105)));
// DEFAULT-NEXT:         write<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(4)>(%[[VALUE_zr]]), const<i32>(1))), const<f80>(3.44028231094461896128E-307));
// DEFAULT-NEXT:         write<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(4)>(%[[VALUE_zi]]), const<i32>(1))), const<f80>(8.0265249247267088655E+4719));
// DEFAULT-NEXT:         write<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(4)>(%[[VALUE_xr]]), const<i32>(2))), neg<f80>(const<f80>(2.282752288459431994E+4812)));
// DEFAULT-NEXT:         write<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(4)>(%[[VALUE_xi]]), const<i32>(2))), const<f80>(1.49505288698142573183E-854));
// DEFAULT-NEXT:         write<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(4)>(%[[VALUE_yr]]), const<i32>(2))), neg<f80>(const<f80>(2270.85721751211820685)));
// DEFAULT-NEXT:         write<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(4)>(%[[VALUE_yi]]), const<i32>(2))), const<f80>(4.47288407618369521217E-4932));
// DEFAULT-NEXT:         write<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(4)>(%[[VALUE_zr]]), const<i32>(2))), const<f80>(1.00523814128672768937E+4809));
// DEFAULT-NEXT:         write<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(4)>(%[[VALUE_zi]]), const<i32>(2))), const<f80>(1.98000721501104493297E-126));
// DEFAULT-NEXT:         write<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(4)>(%[[VALUE_xr]]), const<i32>(3))), const<f80>(1.07363508404356840197E-1185));
// DEFAULT-NEXT:         write<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(4)>(%[[VALUE_xi]]), const<i32>(3))), neg<f80>(const<f80>(4.27982174853080997604E+4696)));
// DEFAULT-NEXT:         write<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(4)>(%[[VALUE_yr]]), const<i32>(3))), const<f80>(1.32092738562856559194E-4919));
// DEFAULT-NEXT:         write<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(4)>(%[[VALUE_yi]]), const<i32>(3))), neg<f80>(const<f80>(3.1506997743185376413E+24)));
// DEFAULT-NEXT:         write<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(4)>(%[[VALUE_zr]]), const<i32>(3))), const<f80>(1.35837180788083473681E+4672));
// DEFAULT-NEXT:         write<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(4)>(%[[VALUE_zi]]), const<i32>(3))), neg<f80>(const<f80>(5.69495873748767192994E-272)));
// DEFAULT-NEXT:         for %[[VALUE5:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE6]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE7]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f80>(real(%[[VALUE_a]]), read<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(4)>(%[[VALUE_xr]]), read<i32>(%[[VALUE_i]])))));
// DEFAULT-NEXT:                     write<f80>(imag(%[[VALUE_a]]), read<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(4)>(%[[VALUE_xi]]), read<i32>(%[[VALUE_i]])))));
// DEFAULT-NEXT:                     write<f80>(real(%[[VALUE_b]]), read<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(4)>(%[[VALUE_yr]]), read<i32>(%[[VALUE_i]])))));
// DEFAULT-NEXT:                     write<f80>(imag(%[[VALUE_b]]), read<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(4)>(%[[VALUE_yi]]), read<i32>(%[[VALUE_i]])))));
// DEFAULT-NEXT:                     write<f80>(real(%[[VALUE_z_2]]), read<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(4)>(%[[VALUE_zr]]), read<i32>(%[[VALUE_i]])))));
// DEFAULT-NEXT:                     write<f80>(imag(%[[VALUE_z_2]]), read<f80>(deref(ptr_offset<ptr<f80>, subtract=false, element=f80, overflow=ub>(array_decay<ptr<f80>, length=Some(4)>(%[[VALUE_zi]]), read<i32>(%[[VALUE_i]])))));
// DEFAULT-NEXT:                     write<complex<f80>>(%[[VALUE_c_2]], div<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%[[VALUE_a]]), read<complex<f80>>(%[[VALUE_b]])));
// DEFAULT-NEXT:                     write<f80>(%[[VALUE_cr]], read<f80>(real(%[[VALUE_c_2]])));
// DEFAULT-NEXT:                     write<f80>(%[[VALUE_ci]], read<f80>(imag(%[[VALUE_c_2]])));
// DEFAULT-NEXT:                     if not<bool>(ne<i32>(call<i32, signature=fn(complex<f80>, complex<f80>) -> i32, abi=sysv64(byval<align=16>, byval<align=16>) -> scalar>(%[[VALUE_match]], read<complex<f80>>(%[[VALUE_c_2]]), read<complex<f80>>(%[[VALUE_z_2]])), const<i32>(0)))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_ok]], const<i32>(0));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32>(%[[VALUE_ok]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
