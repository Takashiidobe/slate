/* PR c/102989 */
/* { dg-do run } */
/* { dg-require-effective-target fenv_exceptions } */
/* { dg-options "-std=c23" } */
/* { dg-add-options ieee } */

#include <fenv.h>

#if __FLT_MANT_DIG__ == 24
#if __BITINT_MAXWIDTH__ >= 135
__attribute__((noipa)) _BitInt(135)
testflt_135 (float d)
{
  return d;
}

__attribute__((noipa)) unsigned _BitInt(135)
testfltu_135 (float d)
{
  return d;
}
#endif

#if __BITINT_MAXWIDTH__ >= 192
__attribute__((noipa)) _BitInt(192)
testflt_192 (float d)
{
  return d;
}

__attribute__((noipa)) unsigned _BitInt(192)
testfltu_192 (float d)
{
  return d;
}
#endif

#if __BITINT_MAXWIDTH__ >= 575
__attribute__((noipa)) _BitInt(575)
testflt_575 (float d)
{
  return d;
}

__attribute__((noipa)) unsigned _BitInt(575)
testfltu_575 (float d)
{
  return d;
}
#endif
#endif

#if __DBL_MANT_DIG__ == 53
#if __BITINT_MAXWIDTH__ >= 135
__attribute__((noipa)) _BitInt(135)
testdbl_135 (double d)
{
  return d;
}

__attribute__((noipa)) unsigned _BitInt(135)
testdblu_135 (double d)
{
  return d;
}
#endif

#if __BITINT_MAXWIDTH__ >= 192
__attribute__((noipa)) _BitInt(192)
testdbl_192 (double d)
{
  return d;
}

__attribute__((noipa)) unsigned _BitInt(192)
testdblu_192 (double d)
{
  return d;
}
#endif

#if __BITINT_MAXWIDTH__ >= 575
__attribute__((noipa)) _BitInt(575)
testdbl_575 (double d)
{
  return d;
}

__attribute__((noipa)) unsigned _BitInt(575)
testdblu_575 (double d)
{
  return d;
}
#endif
#endif

#if __LDBL_MANT_DIG__ == 64
#if __BITINT_MAXWIDTH__ >= 135
__attribute__((noipa)) _BitInt(135)
testldbl_135 (long double d)
{
  return d;
}

__attribute__((noipa)) unsigned _BitInt(135)
testldblu_135 (long double d)
{
  return d;
}
#endif

#if __BITINT_MAXWIDTH__ >= 192
__attribute__((noipa)) _BitInt(192)
testldbl_192 (long double d)
{
  return d;
}

__attribute__((noipa)) unsigned _BitInt(192)
testldblu_192 (long double d)
{
  return d;
}
#endif

#if __BITINT_MAXWIDTH__ >= 575
__attribute__((noipa)) _BitInt(575)
testldbl_575 (long double d)
{
  return d;
}

__attribute__((noipa)) unsigned _BitInt(575)
testldblu_575 (long double d)
{
  return d;
}
#endif
#endif

#if __FLT128_MANT_DIG__ == 113
#if __BITINT_MAXWIDTH__ >= 135
__attribute__((noipa)) _BitInt(135)
testflt128_135 (_Float128 d)
{
  return d;
}

__attribute__((noipa)) unsigned _BitInt(135)
testflt128u_135 (_Float128 d)
{
  return d;
}
#endif

#if __BITINT_MAXWIDTH__ >= 192
__attribute__((noipa)) _BitInt(192)
testflt128_192 (_Float128 d)
{
  return d;
}

__attribute__((noipa)) unsigned _BitInt(192)
testflt128u_192 (_Float128 d)
{
  return d;
}
#endif

#if __BITINT_MAXWIDTH__ >= 575
__attribute__((noipa)) _BitInt(575)
testflt128_575 (_Float128 d)
{
  return d;
}

__attribute__((noipa)) unsigned _BitInt(575)
testflt128u_575 (_Float128 d)
{
  return d;
}
#endif
#endif

__attribute__((noipa)) void
check_inexact (int test, int inex)
{
  if (!test)
    __builtin_abort ();
  if ((!fetestexcept (FE_INEXACT)) != (!inex))
    __builtin_abort ();
  feclearexcept (FE_INEXACT);
}

int
main ()
{
#if __FLT_MANT_DIG__ == 24
#if __BITINT_MAXWIDTH__ >= 135
  check_inexact (testflt_135 (-85070591730234615865843651857942052864.0f) == -85070591730234615865843651857942052864wb, 0);
  check_inexact (testflt_135 (0xffffffp+104f) == 340282346638528859811704183484516925440wb, 0);
  check_inexact (testflt_135 (-0xffffffp+104f) == -340282346638528859811704183484516925440wb, 0);
  check_inexact (testflt_135 (-0xffffffp-1f) == -8388607wb, 1);
  check_inexact (testflt_135 (-0.f) == 0wb, 0);
  check_inexact (testflt_135 (-0.f) == 0wb, 0);
  check_inexact (testflt_135 (-0.9990234375f) == 0wb, 1);
  check_inexact (testfltu_135 (0.f) == 0uwb, 0);
  check_inexact (testfltu_135 (-0.9990234375f) == 0uwb, 1);
  check_inexact (testfltu_135 (0xffffffp-1f) == 8388607uwb, 1);
  check_inexact (testfltu_135 (0xffffffp+104f) == 340282346638528859811704183484516925440uwb, 0);
#endif
#if __BITINT_MAXWIDTH__ >= 192
  check_inexact (testflt_192 (-85070591730234615865843651857942052864.0f) == -85070591730234615865843651857942052864wb, 0);
  check_inexact (testflt_192 (0xffffffp+104f) == 340282346638528859811704183484516925440wb, 0);
  check_inexact (testflt_192 (-0xffffffp+104f) == -340282346638528859811704183484516925440wb, 0);
  check_inexact (testflt_192 (-0xffffffp-3f) == -2097151wb, 1);
  check_inexact (testflt_192 (-0.f) == 0wb, 0);
  check_inexact (testflt_192 (-0.9990234375f) == 0wb, 1);
  check_inexact (testfltu_192 (0.f) == 0uwb, 0);
  check_inexact (testfltu_192 (-0.9990234375f) == 0uwb, 1);
  check_inexact (testfltu_192 (0xffffffp-3f) == 2097151uwb, 1);
  check_inexact (testfltu_192 (0xffffffp+104f) == 340282346638528859811704183484516925440uwb, 0);
#endif
#if __BITINT_MAXWIDTH__ >= 575
  check_inexact (testflt_575 (-85070591730234615865843651857942052864.0f) == -85070591730234615865843651857942052864wb, 0);
  check_inexact (testflt_575 (0xffffffp+104f) == 340282346638528859811704183484516925440wb, 0);
  check_inexact (testflt_575 (-0xffffffp+104f) == -340282346638528859811704183484516925440wb, 0);
  check_inexact (testflt_575 (-0xffffffp-5f) == -524287wb, 1);
  check_inexact (testflt_575 (0.f) == 0wb, 0);
  check_inexact (testflt_575 (-0.9990234375f) == 0wb, 1);
  check_inexact (testfltu_575 (-0.f) == 0uwb, 0);
  check_inexact (testfltu_575 (-0.9990234375f) == 0uwb, 1);
  check_inexact (testfltu_575 (0xffffffp-5f) == 524287uwb, 1);
  check_inexact (testfltu_575 (0xffffffp+104f) == 340282346638528859811704183484516925440uwb, 0);
#endif
#endif
#if __DBL_MANT_DIG__ == 53
#if __BITINT_MAXWIDTH__ >= 135
  check_inexact (testdbl_135 (-85070591730234615865843651857942052864.0) == -85070591730234615865843651857942052864wb, 0);
  check_inexact (testdbl_135 (0x1fffffffffffffp+81) == 21778071482940059243804335646374816120832wb, 0);
  check_inexact (testdbl_135 (-0x20000000000000p+81) == -21778071482940061661655974875633165533183wb - 1, 0);
  check_inexact (testdbl_135 (-0x1fffffffffffffp-1) == -4503599627370495wb, 1);
  check_inexact (testdbl_135 (-0.) == 0wb, 0);
  check_inexact (testdbl_135 (-0.9990234375) == 0wb, 1);
  check_inexact (testdblu_135 (0.) == 0uwb, 0);
  check_inexact (testdblu_135 (-0.9990234375) == 0uwb, 1);
  check_inexact (testdblu_135 (0x1fffffffffffffp-1) == 4503599627370495uwb, 1);
  check_inexact (testdblu_135 (0x1fffffffffffffp+82) == 43556142965880118487608671292749632241664uwb, 0);
#endif
#if __BITINT_MAXWIDTH__ >= 192
  check_inexact (testdbl_192 (-85070591730234615865843651857942052864.0) == -85070591730234615865843651857942052864wb, 0);
  check_inexact (testdbl_192 (0x1fffffffffffffp+138) == 3138550867693340033468750984562846621555579712101368725504wb, 0);
  check_inexact (testdbl_192 (-0x20000000000000p+138) == -3138550867693340381917894711603833208051177722232017256447wb - 1, 0);
  check_inexact (testdbl_192 (-0x1fffffffffffffp-3) == -1125899906842623wb, 1);
  check_inexact (testdbl_192 (0.) == 0wb, 0);
  check_inexact (testdbl_192 (-0.9990234375) == 0wb, 1);
  check_inexact (testdblu_192 (-0.) == 0uwb, 0);
  check_inexact (testdblu_192 (-0.9990234375) == 0uwb, 1);
  check_inexact (testdblu_192 (0x1fffffffffffffp-3) == 1125899906842623uwb, 1);
  check_inexact (testdblu_192 (0x1fffffffffffffp+139) == 6277101735386680066937501969125693243111159424202737451008uwb, 0);
#endif
#if __BITINT_MAXWIDTH__ >= 575
  check_inexact (testdbl_575 (-85070591730234615865843651857942052864.0) == -85070591730234615865843651857942052864wb, 0);
  check_inexact (testdbl_575 (0x1fffffffffffffp+521) == 61832600368276126650327970124302082526882038193909742709080463879918896882169507607035916867654709124839777195049479857541529867095829765369898539058829479405123401922117632wb, 0);
  check_inexact (testdbl_575 (-0x20000000000000p+521) == -61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783wb - 1, 0);
  check_inexact (testdbl_575 (-0x1fffffffffffffp-5) == -281474976710655wb, 1);
  check_inexact (testdbl_575 (-0.) == 0wb, 0);
  check_inexact (testdbl_575 (-0.9990234375) == 0wb, 1);
  check_inexact (testdblu_575 (0.) == 0uwb, 0);
  check_inexact (testdblu_575 (-0.9990234375) == 0uwb, 1);
  check_inexact (testdblu_575 (0x1fffffffffffffp-5) == 281474976710655uwb, 1);
  check_inexact (testdblu_575 (0x1fffffffffffffp+522) == 123665200736552253300655940248604165053764076387819485418160927759837793764339015214071833735309418249679554390098959715083059734191659530739797078117658958810246803844235264uwb, 0);
#endif
#endif
#if __LDBL_MANT_DIG__ == 64
#if __BITINT_MAXWIDTH__ >= 135
  check_inexact (testldbl_135 (-85070591730234615865843651857942052864.0L) == -85070591730234615865843651857942052864wb, 0);
  check_inexact (testldbl_135 (0xffffffffffffffffp+70L) == 21778071482940061660475383254915754229760wb, 0);
  check_inexact (testldbl_135 (-0x10000000000000000p+70L) == -21778071482940061661655974875633165533183wb - 1, 0);
  check_inexact (testldbl_135 (-0xffffffffffffffffp-1L) == -9223372036854775807wb, 1);
  check_inexact (testldbl_135 (-0.L) == 0wb, 0);
  check_inexact (testldbl_135 (-0.9990234375L) == 0wb, 1);
  check_inexact (testldblu_135 (0.L) == 0uwb, 0);
  check_inexact (testldblu_135 (-0.9990234375L) == 0uwb, 1);
  check_inexact (testldblu_135 (0xffffffffffffffffp-1L) == 9223372036854775807uwb, 1);
  check_inexact (testldblu_135 (0xffffffffffffffffp+71L) == 43556142965880123320950766509831508459520uwb, 0);
#endif
#if __BITINT_MAXWIDTH__ >= 192
  check_inexact (testldbl_192 (-85070591730234615865843651857942052864.0L) == -85070591730234615865843651857942052864wb, 0);
  check_inexact (testldbl_192 (0xffffffffffffffffp+127L) == 3138550867693340381747753528143363976319490418516133150720wb, 0);
  check_inexact (testldbl_192 (-0x10000000000000000p+127L) == -3138550867693340381917894711603833208051177722232017256447wb - 1, 0);
  check_inexact (testldbl_192 (-0xffffffffffffffffp-2L) == -4611686018427387903wb, 1);
  check_inexact (testldbl_192 (0.L) == 0wb, 0);
  check_inexact (testldbl_192 (-0.9990234375L) == 0wb, 1);
  check_inexact (testldblu_192 (-0.L) == 0uwb, 0);
  check_inexact (testldblu_192 (-0.9990234375L) == 0uwb, 1);
  check_inexact (testldblu_192 (0xffffffffffffffffp-2L) == 4611686018427387903uwb, 1);
  check_inexact (testldblu_192 (0xffffffffffffffffp+128L) == 6277101735386680763495507056286727952638980837032266301440uwb, 0);
#endif
#if __BITINT_MAXWIDTH__ >= 575
  check_inexact (testldbl_575 (-85070591730234615865843651857942052864.0L) == -85070591730234615865843651857942052864wb, 0);
  check_inexact (testldbl_575 (0xffffffffffffffffp+510L) == 61832600368276133511773678272426148233889331025751498446645922568076207932202076431648659257792374503198949281962308977915333294030066289778448068072486649492543280785653760wb, 0);
  check_inexact (testldbl_575 (-0x10000000000000000p+510L) == -61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783wb - 1, 0);
  check_inexact (testldbl_575 (-0xffffffffffffffffp-4L) == -1152921504606846975wb, 1);
  check_inexact (testldbl_575 (0.L) == 0wb, 0);
  check_inexact (testldbl_575 (-0.9990234375L) == 0wb, 1);
  check_inexact (testldblu_575 (-0.L) == 0uwb, 0);
  check_inexact (testldblu_575 (-0.9990234375L) == 0uwb, 1);
  check_inexact (testldblu_575 (0xffffffffffffffffp-4L) == 1152921504606846975uwb, 1);
  check_inexact (testldblu_575 (0xffffffffffffffffp+511L) == 123665200736552267023547356544852296467778662051502996893291845136152415864404152863297318515584749006397898563924617955830666588060132579556896136144973298985086561571307520uwb, 0);
#endif
#endif
#if __FLT128_MANT_DIG__ == 113
#if __BITINT_MAXWIDTH__ >= 135
  check_inexact (testflt128_135 (-85070591730234615865843651857942052864.0F128) == -85070591730234615865843651857942052864wb, 0);
  check_inexact (testflt128_135 (0x1ffffffffffffffffffffffffffffp+21F128) == 21778071482940061661655974875633163436032wb, 0);
  check_inexact (testflt128_135 (-0x20000000000000000000000000000p+21F128) == -21778071482940061661655974875633165533183wb - 1, 0);
  check_inexact (testflt128_135 (-0x1ffffffffffffffffffffffffffffp-1F128) == -5192296858534827628530496329220095wb, 1);
  check_inexact (testflt128_135 (-0.F128) == 0wb, 0);
  check_inexact (testflt128_135 (-0.9990234375F128) == 0wb, 1);
  check_inexact (testflt128u_135 (0.F128) == 0uwb, 0);
  check_inexact (testflt128u_135 (-0.9990234375F128) == 0uwb, 1);
  check_inexact (testflt128u_135 (0x1ffffffffffffffffffffffffffffp-1F128) == 5192296858534827628530496329220095uwb, 1);
  check_inexact (testflt128u_135 (0x1ffffffffffffffffffffffffffffp+22F128) == 43556142965880123323311949751266326872064uwb, 0);
#endif
#if __BITINT_MAXWIDTH__ >= 192
  check_inexact (testflt128_192 (-85070591730234615865843651857942052864.0F128) == -85070591730234615865843651857942052864wb, 0);
  check_inexact (testflt128_192 (0x1ffffffffffffffffffffffffffffp+78F128) == 3138550867693340381917894711603832905819722818574723579904wb, 0);
  check_inexact (testflt128_192 (-0x20000000000000000000000000000p+78F128) == -3138550867693340381917894711603833208051177722232017256447wb - 1, 0);
  check_inexact (testflt128_192 (-0x1ffffffffffffffffffffffffffffp-4F128) == -649037107316853453566312041152511wb, 1);
  check_inexact (testflt128_192 (-0.F128) == 0wb, 0);
  check_inexact (testflt128_192 (-0.9990234375F128) == 0wb, 1);
  check_inexact (testflt128u_192 (0.F128) == 0uwb, 0);
  check_inexact (testflt128u_192 (-0.9990234375F128) == 0uwb, 1);
  check_inexact (testflt128u_192 (0x1ffffffffffffffffffffffffffffp-4F128) == 649037107316853453566312041152511uwb, 1);
  check_inexact (testflt128u_192 (0x1ffffffffffffffffffffffffffffp+79F128) == 6277101735386680763835789423207665811639445637149447159808uwb, 0);
#endif
#if __BITINT_MAXWIDTH__ >= 575
  check_inexact (testflt128_575 (-85070591730234615865843651857942052864.0F128) == -85070591730234615865843651857942052864wb, 0);
  check_inexact (testflt128_575 (0x1ffffffffffffffffffffffffffffp+461F128) == 61832600368276133515125630254911791554520007845691312598455129804691160851602940042069550439343049559602369631548246946680753811425558728725309540242943660463695151425912832wb, 0);
  check_inexact (testflt128_575 (-0x20000000000000000000000000000p+461F128) == -61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783wb - 1, 0);
  check_inexact (testflt128_575 (-0x1ffffffffffffffffffffffffffffp-8F128) == -40564819207303340847894502572031wb, 1);
  check_inexact (testflt128_575 (0.F128) == 0wb, 0);
  check_inexact (testflt128_575 (-0.9990234375F128) == 0wb, 1);
  check_inexact (testflt128u_575 (-0.F128) == 0uwb, 0);
  check_inexact (testflt128u_575 (-0.9990234375F128) == 0uwb, 1);
  check_inexact (testflt128u_575 (0x1ffffffffffffffffffffffffffffp-8F128) == 40564819207303340847894502572031uwb, 1);
  check_inexact (testflt128u_575 (0x1ffffffffffffffffffffffffffffp+462F128) == 123665200736552267030251260509823583109040015691382625196910259609382321703205880084139100878686099119204739263096493893361507622851117457450619080485887320927390302851825664uwb, 0);
#endif
#endif
}

// SLATE-FILECHECK-STD DEFAULT c23
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
// DEFAULT-NEXT:     fn %0 @feclearexcept(%42 __excepts: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @fetestexcept(%43 __excepts: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @testflt_135(%3 d: f32) -> i135b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<i135b, reason=return, out_of_range=ub, exceptions=ignore>(read<f32>(%3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @testfltu_135(%5 d: f32) -> u135b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<u135b, reason=return, out_of_range=ub, exceptions=ignore>(read<f32>(%5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @testflt_192(%7 d: f32) -> i192b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<i192b, reason=return, out_of_range=ub, exceptions=ignore>(read<f32>(%7));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @testfltu_192(%9 d: f32) -> u192b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<u192b, reason=return, out_of_range=ub, exceptions=ignore>(read<f32>(%9));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @testflt_575(%11 d: f32) -> i575b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<i575b, reason=return, out_of_range=ub, exceptions=ignore>(read<f32>(%11));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @testfltu_575(%13 d: f32) -> u575b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<u575b, reason=return, out_of_range=ub, exceptions=ignore>(read<f32>(%13));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @testdbl_135(%15 d: f64) -> i135b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<i135b, reason=return, out_of_range=ub, exceptions=ignore>(read<f64>(%15));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @testdblu_135(%17 d: f64) -> u135b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<u135b, reason=return, out_of_range=ub, exceptions=ignore>(read<f64>(%17));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @testdbl_192(%19 d: f64) -> i192b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<i192b, reason=return, out_of_range=ub, exceptions=ignore>(read<f64>(%19));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @testdblu_192(%21 d: f64) -> u192b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<u192b, reason=return, out_of_range=ub, exceptions=ignore>(read<f64>(%21));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @testdbl_575(%23 d: f64) -> i575b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<i575b, reason=return, out_of_range=ub, exceptions=ignore>(read<f64>(%23));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %24 @testdblu_575(%25 d: f64) -> u575b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<u575b, reason=return, out_of_range=ub, exceptions=ignore>(read<f64>(%25));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %26 @testldbl_135(%27 d: f80) -> i135b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<i135b, reason=return, out_of_range=ub, exceptions=ignore>(read<f80>(%27));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %28 @testldblu_135(%29 d: f80) -> u135b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<u135b, reason=return, out_of_range=ub, exceptions=ignore>(read<f80>(%29));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %30 @testldbl_192(%31 d: f80) -> i192b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<i192b, reason=return, out_of_range=ub, exceptions=ignore>(read<f80>(%31));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %32 @testldblu_192(%33 d: f80) -> u192b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<u192b, reason=return, out_of_range=ub, exceptions=ignore>(read<f80>(%33));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %34 @testldbl_575(%35 d: f80) -> i575b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<i575b, reason=return, out_of_range=ub, exceptions=ignore>(read<f80>(%35));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %36 @testldblu_575(%37 d: f80) -> u575b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<u575b, reason=return, out_of_range=ub, exceptions=ignore>(read<f80>(%37));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %44 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %38 @check_inexact(%39 test: i32, %40 inex: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32>(%39), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%44);
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(not<bool>(ne<i32>(call<i32, signature=fn(i32) -> i32>(%1, const<i32>(32)), const<i32>(0)))), from_bool<i32, reason=promotion>(not<bool>(ne<i32>(read<i32>(%40), const<i32>(0)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%44);
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%0, const<i32>(32));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %41 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<i135b>(call<i135b, signature=fn(f32) -> i135b>(%2, neg<f32>(const<f32>(8.507059e37))), widen<i135b, reason=usual_arith>(neg<i128b, overflow=ub>(const<i128b>(85070591730234615865843651857942052864))))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<i135b>(call<i135b, signature=fn(f32) -> i135b>(%2, const<f32>(3.4028235e38)), widen<i135b, reason=usual_arith>(const<i129b>(340282346638528859811704183484516925440)))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<i135b>(call<i135b, signature=fn(f32) -> i135b>(%2, neg<f32>(const<f32>(3.4028235e38))), widen<i135b, reason=usual_arith>(neg<i129b, overflow=ub>(const<i129b>(340282346638528859811704183484516925440))))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<i135b>(call<i135b, signature=fn(f32) -> i135b>(%2, neg<f32>(const<f32>(8388607.5))), widen<i135b, reason=usual_arith>(neg<i24b, overflow=ub>(const<i24b>(8388607))))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<i135b>(call<i135b, signature=fn(f32) -> i135b>(%2, neg<f32>(const<f32>(0.0))), widen<i135b, reason=usual_arith>(const<i2b>(0)))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<i135b>(call<i135b, signature=fn(f32) -> i135b>(%2, neg<f32>(const<f32>(0.0))), widen<i135b, reason=usual_arith>(const<i2b>(0)))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<i135b>(call<i135b, signature=fn(f32) -> i135b>(%2, neg<f32>(const<f32>(0.99902344))), widen<i135b, reason=usual_arith>(const<i2b>(0)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<u135b>(call<u135b, signature=fn(f32) -> u135b>(%4, const<f32>(0.0)), widen<u135b, reason=usual_arith>(const<u1b>(0)))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<u135b>(call<u135b, signature=fn(f32) -> u135b>(%4, neg<f32>(const<f32>(0.99902344))), widen<u135b, reason=usual_arith>(const<u1b>(0)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<u135b>(call<u135b, signature=fn(f32) -> u135b>(%4, const<f32>(8388607.5)), widen<u135b, reason=usual_arith>(const<u23b>(8388607)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<u135b>(call<u135b, signature=fn(f32) -> u135b>(%4, const<f32>(3.4028235e38)), widen<u135b, reason=usual_arith>(const<u128b>(340282346638528859811704183484516925440)))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<i192b>(call<i192b, signature=fn(f32) -> i192b>(%6, neg<f32>(const<f32>(8.507059e37))), widen<i192b, reason=usual_arith>(neg<i128b, overflow=ub>(const<i128b>(85070591730234615865843651857942052864))))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<i192b>(call<i192b, signature=fn(f32) -> i192b>(%6, const<f32>(3.4028235e38)), widen<i192b, reason=usual_arith>(const<i129b>(340282346638528859811704183484516925440)))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<i192b>(call<i192b, signature=fn(f32) -> i192b>(%6, neg<f32>(const<f32>(3.4028235e38))), widen<i192b, reason=usual_arith>(neg<i129b, overflow=ub>(const<i129b>(340282346638528859811704183484516925440))))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<i192b>(call<i192b, signature=fn(f32) -> i192b>(%6, neg<f32>(const<f32>(2097151.9))), widen<i192b, reason=usual_arith>(neg<i22b, overflow=ub>(const<i22b>(2097151))))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<i192b>(call<i192b, signature=fn(f32) -> i192b>(%6, neg<f32>(const<f32>(0.0))), widen<i192b, reason=usual_arith>(const<i2b>(0)))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<i192b>(call<i192b, signature=fn(f32) -> i192b>(%6, neg<f32>(const<f32>(0.99902344))), widen<i192b, reason=usual_arith>(const<i2b>(0)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<u192b>(call<u192b, signature=fn(f32) -> u192b>(%8, const<f32>(0.0)), widen<u192b, reason=usual_arith>(const<u1b>(0)))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<u192b>(call<u192b, signature=fn(f32) -> u192b>(%8, neg<f32>(const<f32>(0.99902344))), widen<u192b, reason=usual_arith>(const<u1b>(0)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<u192b>(call<u192b, signature=fn(f32) -> u192b>(%8, const<f32>(2097151.9)), widen<u192b, reason=usual_arith>(const<u21b>(2097151)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<u192b>(call<u192b, signature=fn(f32) -> u192b>(%8, const<f32>(3.4028235e38)), widen<u192b, reason=usual_arith>(const<u128b>(340282346638528859811704183484516925440)))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<i575b>(call<i575b, signature=fn(f32) -> i575b>(%10, neg<f32>(const<f32>(8.507059e37))), widen<i575b, reason=usual_arith>(neg<i128b, overflow=ub>(const<i128b>(85070591730234615865843651857942052864))))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<i575b>(call<i575b, signature=fn(f32) -> i575b>(%10, const<f32>(3.4028235e38)), widen<i575b, reason=usual_arith>(const<i129b>(340282346638528859811704183484516925440)))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<i575b>(call<i575b, signature=fn(f32) -> i575b>(%10, neg<f32>(const<f32>(3.4028235e38))), widen<i575b, reason=usual_arith>(neg<i129b, overflow=ub>(const<i129b>(340282346638528859811704183484516925440))))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<i575b>(call<i575b, signature=fn(f32) -> i575b>(%10, neg<f32>(const<f32>(524287.97))), widen<i575b, reason=usual_arith>(neg<i20b, overflow=ub>(const<i20b>(524287))))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<i575b>(call<i575b, signature=fn(f32) -> i575b>(%10, const<f32>(0.0)), widen<i575b, reason=usual_arith>(const<i2b>(0)))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<i575b>(call<i575b, signature=fn(f32) -> i575b>(%10, neg<f32>(const<f32>(0.99902344))), widen<i575b, reason=usual_arith>(const<i2b>(0)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<u575b>(call<u575b, signature=fn(f32) -> u575b>(%12, neg<f32>(const<f32>(0.0))), widen<u575b, reason=usual_arith>(const<u1b>(0)))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<u575b>(call<u575b, signature=fn(f32) -> u575b>(%12, neg<f32>(const<f32>(0.99902344))), widen<u575b, reason=usual_arith>(const<u1b>(0)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<u575b>(call<u575b, signature=fn(f32) -> u575b>(%12, const<f32>(524287.97)), widen<u575b, reason=usual_arith>(const<u19b>(524287)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<u575b>(call<u575b, signature=fn(f32) -> u575b>(%12, const<f32>(3.4028235e38)), widen<u575b, reason=usual_arith>(const<u128b>(340282346638528859811704183484516925440)))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<i135b>(call<i135b, signature=fn(f64) -> i135b>(%14, neg<f64>(const<f64>(8.507059173023462e37))), widen<i135b, reason=usual_arith>(neg<i128b, overflow=ub>(const<i128b>(85070591730234615865843651857942052864))))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<i135b>(call<i135b, signature=fn(f64) -> i135b>(%14, const<f64>(2.177807148294006e40)), const<i135b>(21778071482940059243804335646374816120832))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<i135b>(call<i135b, signature=fn(f64) -> i135b>(%14, neg<f64>(const<f64>(2.1778071482940062e40))), sub<i135b, overflow=ub>(neg<i135b, overflow=ub>(const<i135b>(21778071482940061661655974875633165533183)), widen<i135b, reason=usual_arith>(const<i32>(1))))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<i135b>(call<i135b, signature=fn(f64) -> i135b>(%14, neg<f64>(const<f64>(4503599627370495.5))), widen<i135b, reason=usual_arith>(neg<i53b, overflow=ub>(const<i53b>(4503599627370495))))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<i135b>(call<i135b, signature=fn(f64) -> i135b>(%14, neg<f64>(const<f64>(0.0))), widen<i135b, reason=usual_arith>(const<i2b>(0)))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<i135b>(call<i135b, signature=fn(f64) -> i135b>(%14, neg<f64>(const<f64>(0.9990234375))), widen<i135b, reason=usual_arith>(const<i2b>(0)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<u135b>(call<u135b, signature=fn(f64) -> u135b>(%16, const<f64>(0.0)), widen<u135b, reason=usual_arith>(const<u1b>(0)))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<u135b>(call<u135b, signature=fn(f64) -> u135b>(%16, neg<f64>(const<f64>(0.9990234375))), widen<u135b, reason=usual_arith>(const<u1b>(0)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<u135b>(call<u135b, signature=fn(f64) -> u135b>(%16, const<f64>(4503599627370495.5)), widen<u135b, reason=usual_arith>(const<u52b>(4503599627370495)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<u135b>(call<u135b, signature=fn(f64) -> u135b>(%16, const<f64>(4.355614296588012e40)), const<u135b>(43556142965880118487608671292749632241664))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<i192b>(call<i192b, signature=fn(f64) -> i192b>(%18, neg<f64>(const<f64>(8.507059173023462e37))), widen<i192b, reason=usual_arith>(neg<i128b, overflow=ub>(const<i128b>(85070591730234615865843651857942052864))))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<i192b>(call<i192b, signature=fn(f64) -> i192b>(%18, const<f64>(3.13855086769334e57)), const<i192b>(3138550867693340033468750984562846621555579712101368725504))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<i192b>(call<i192b, signature=fn(f64) -> i192b>(%18, neg<f64>(const<f64>(3.1385508676933404e57))), sub<i192b, overflow=ub>(neg<i192b, overflow=ub>(const<i192b>(3138550867693340381917894711603833208051177722232017256447)), widen<i192b, reason=usual_arith>(const<i32>(1))))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<i192b>(call<i192b, signature=fn(f64) -> i192b>(%18, neg<f64>(const<f64>(1125899906842623.9))), widen<i192b, reason=usual_arith>(neg<i51b, overflow=ub>(const<i51b>(1125899906842623))))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<i192b>(call<i192b, signature=fn(f64) -> i192b>(%18, const<f64>(0.0)), widen<i192b, reason=usual_arith>(const<i2b>(0)))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<i192b>(call<i192b, signature=fn(f64) -> i192b>(%18, neg<f64>(const<f64>(0.9990234375))), widen<i192b, reason=usual_arith>(const<i2b>(0)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<u192b>(call<u192b, signature=fn(f64) -> u192b>(%20, neg<f64>(const<f64>(0.0))), widen<u192b, reason=usual_arith>(const<u1b>(0)))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<u192b>(call<u192b, signature=fn(f64) -> u192b>(%20, neg<f64>(const<f64>(0.9990234375))), widen<u192b, reason=usual_arith>(const<u1b>(0)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<u192b>(call<u192b, signature=fn(f64) -> u192b>(%20, const<f64>(1125899906842623.9)), widen<u192b, reason=usual_arith>(const<u50b>(1125899906842623)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<u192b>(call<u192b, signature=fn(f64) -> u192b>(%20, const<f64>(6.27710173538668e57)), const<u192b>(6277101735386680066937501969125693243111159424202737451008))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<i575b>(call<i575b, signature=fn(f64) -> i575b>(%22, neg<f64>(const<f64>(8.507059173023462e37))), widen<i575b, reason=usual_arith>(neg<i128b, overflow=ub>(const<i128b>(85070591730234615865843651857942052864))))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<i575b>(call<i575b, signature=fn(f64) -> i575b>(%22, const<f64>(6.183260036827613e172)), const<i575b>(61832600368276126650327970124302082526882038193909742709080463879918896882169507607035916867654709124839777195049479857541529867095829765369898539058829479405123401922117632))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<i575b>(call<i575b, signature=fn(f64) -> i575b>(%22, neg<f64>(const<f64>(6.183260036827614e172))), sub<i575b, overflow=ub>(neg<i575b, overflow=ub>(const<i575b>(61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783)), widen<i575b, reason=usual_arith>(const<i32>(1))))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<i575b>(call<i575b, signature=fn(f64) -> i575b>(%22, neg<f64>(const<f64>(281474976710655.97))), widen<i575b, reason=usual_arith>(neg<i49b, overflow=ub>(const<i49b>(281474976710655))))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<i575b>(call<i575b, signature=fn(f64) -> i575b>(%22, neg<f64>(const<f64>(0.0))), widen<i575b, reason=usual_arith>(const<i2b>(0)))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<i575b>(call<i575b, signature=fn(f64) -> i575b>(%22, neg<f64>(const<f64>(0.9990234375))), widen<i575b, reason=usual_arith>(const<i2b>(0)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<u575b>(call<u575b, signature=fn(f64) -> u575b>(%24, const<f64>(0.0)), widen<u575b, reason=usual_arith>(const<u1b>(0)))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<u575b>(call<u575b, signature=fn(f64) -> u575b>(%24, neg<f64>(const<f64>(0.9990234375))), widen<u575b, reason=usual_arith>(const<u1b>(0)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<u575b>(call<u575b, signature=fn(f64) -> u575b>(%24, const<f64>(281474976710655.97)), widen<u575b, reason=usual_arith>(const<u48b>(281474976710655)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<u575b>(call<u575b, signature=fn(f64) -> u575b>(%24, const<f64>(1.2366520073655225e173)), const<u575b>(123665200736552253300655940248604165053764076387819485418160927759837793764339015214071833735309418249679554390098959715083059734191659530739797078117658958810246803844235264))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<i135b>(call<i135b, signature=fn(f80) -> i135b>(%26, neg<f80>(const<f80>(8.50705917302346158658E+37))), widen<i135b, reason=usual_arith>(neg<i128b, overflow=ub>(const<i128b>(85070591730234615865843651857942052864))))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<i135b>(call<i135b, signature=fn(f80) -> i135b>(%26, const<f80>(2.17780714829400616605E+40)), const<i135b>(21778071482940061660475383254915754229760))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<i135b>(call<i135b, signature=fn(f80) -> i135b>(%26, neg<f80>(const<f80>(2.17780714829400616617E+40))), sub<i135b, overflow=ub>(neg<i135b, overflow=ub>(const<i135b>(21778071482940061661655974875633165533183)), widen<i135b, reason=usual_arith>(const<i32>(1))))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<i135b>(call<i135b, signature=fn(f80) -> i135b>(%26, neg<f80>(const<f80>(9223372036854775807.5))), widen<i135b, reason=usual_arith>(neg<i64b, overflow=ub>(const<i64b>(9223372036854775807))))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<i135b>(call<i135b, signature=fn(f80) -> i135b>(%26, neg<f80>(const<f80>(0))), widen<i135b, reason=usual_arith>(const<i2b>(0)))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<i135b>(call<i135b, signature=fn(f80) -> i135b>(%26, neg<f80>(const<f80>(0.9990234375))), widen<i135b, reason=usual_arith>(const<i2b>(0)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<u135b>(call<u135b, signature=fn(f80) -> u135b>(%28, const<f80>(0)), widen<u135b, reason=usual_arith>(const<u1b>(0)))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<u135b>(call<u135b, signature=fn(f80) -> u135b>(%28, neg<f80>(const<f80>(0.9990234375))), widen<u135b, reason=usual_arith>(const<u1b>(0)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<u135b>(call<u135b, signature=fn(f80) -> u135b>(%28, const<f80>(9223372036854775807.5)), widen<u135b, reason=usual_arith>(const<u63b>(9223372036854775807)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<u135b>(call<u135b, signature=fn(f80) -> u135b>(%28, const<f80>(4.3556142965880123321E+40)), const<u135b>(43556142965880123320950766509831508459520))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<i192b>(call<i192b, signature=fn(f80) -> i192b>(%30, neg<f80>(const<f80>(8.50705917302346158658E+37))), widen<i192b, reason=usual_arith>(neg<i128b, overflow=ub>(const<i128b>(85070591730234615865843651857942052864))))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<i192b>(call<i192b, signature=fn(f80) -> i192b>(%30, const<f80>(3.13855086769334038175E+57)), const<i192b>(3138550867693340381747753528143363976319490418516133150720))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<i192b>(call<i192b, signature=fn(f80) -> i192b>(%30, neg<f80>(const<f80>(3.13855086769334038192E+57))), sub<i192b, overflow=ub>(neg<i192b, overflow=ub>(const<i192b>(3138550867693340381917894711603833208051177722232017256447)), widen<i192b, reason=usual_arith>(const<i32>(1))))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<i192b>(call<i192b, signature=fn(f80) -> i192b>(%30, neg<f80>(const<f80>(4611686018427387903.75))), widen<i192b, reason=usual_arith>(neg<i63b, overflow=ub>(const<i63b>(4611686018427387903))))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<i192b>(call<i192b, signature=fn(f80) -> i192b>(%30, const<f80>(0)), widen<i192b, reason=usual_arith>(const<i2b>(0)))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<i192b>(call<i192b, signature=fn(f80) -> i192b>(%30, neg<f80>(const<f80>(0.9990234375))), widen<i192b, reason=usual_arith>(const<i2b>(0)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<u192b>(call<u192b, signature=fn(f80) -> u192b>(%32, neg<f80>(const<f80>(0))), widen<u192b, reason=usual_arith>(const<u1b>(0)))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<u192b>(call<u192b, signature=fn(f80) -> u192b>(%32, neg<f80>(const<f80>(0.9990234375))), widen<u192b, reason=usual_arith>(const<u1b>(0)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<u192b>(call<u192b, signature=fn(f80) -> u192b>(%32, const<f80>(4611686018427387903.75)), widen<u192b, reason=usual_arith>(const<u62b>(4611686018427387903)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<u192b>(call<u192b, signature=fn(f80) -> u192b>(%32, const<f80>(6.2771017353866807635E+57)), const<u192b>(6277101735386680763495507056286727952638980837032266301440))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<i575b>(call<i575b, signature=fn(f80) -> i575b>(%34, neg<f80>(const<f80>(8.50705917302346158658E+37))), widen<i575b, reason=usual_arith>(neg<i128b, overflow=ub>(const<i128b>(85070591730234615865843651857942052864))))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<i575b>(call<i575b, signature=fn(f80) -> i575b>(%34, const<f80>(6.18326003682761335118E+172)), const<i575b>(61832600368276133511773678272426148233889331025751498446645922568076207932202076431648659257792374503198949281962308977915333294030066289778448068072486649492543280785653760))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<i575b>(call<i575b, signature=fn(f80) -> i575b>(%34, neg<f80>(const<f80>(6.18326003682761335151E+172))), sub<i575b, overflow=ub>(neg<i575b, overflow=ub>(const<i575b>(61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783)), widen<i575b, reason=usual_arith>(const<i32>(1))))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<i575b>(call<i575b, signature=fn(f80) -> i575b>(%34, neg<f80>(const<f80>(1152921504606846975.94))), widen<i575b, reason=usual_arith>(neg<i61b, overflow=ub>(const<i61b>(1152921504606846975))))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<i575b>(call<i575b, signature=fn(f80) -> i575b>(%34, const<f80>(0)), widen<i575b, reason=usual_arith>(const<i2b>(0)))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<i575b>(call<i575b, signature=fn(f80) -> i575b>(%34, neg<f80>(const<f80>(0.9990234375))), widen<i575b, reason=usual_arith>(const<i2b>(0)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<u575b>(call<u575b, signature=fn(f80) -> u575b>(%36, neg<f80>(const<f80>(0))), widen<u575b, reason=usual_arith>(const<u1b>(0)))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<u575b>(call<u575b, signature=fn(f80) -> u575b>(%36, neg<f80>(const<f80>(0.9990234375))), widen<u575b, reason=usual_arith>(const<u1b>(0)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<u575b>(call<u575b, signature=fn(f80) -> u575b>(%36, const<f80>(1152921504606846975.94)), widen<u575b, reason=usual_arith>(const<u60b>(1152921504606846975)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%38, from_bool<i32, reason=arg>(eq<u575b>(call<u575b, signature=fn(f80) -> u575b>(%36, const<f80>(1.23665200736552267024E+173)), const<u575b>(123665200736552267023547356544852296467778662051502996893291845136152415864404152863297318515584749006397898563924617955830666588060132579556896136144973298985086561571307520))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
