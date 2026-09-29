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
check_invalid (int test, int inv)
{
  if (!test)
    __builtin_abort ();
  if ((!fetestexcept (FE_INVALID)) != (!inv))
    __builtin_abort ();
  feclearexcept (FE_INVALID);
}

int
main ()
{
#if __FLT_MANT_DIG__ == 24
#if __BITINT_MAXWIDTH__ >= 135
  check_invalid (testflt_135 (-85070591730234615865843651857942052864.0f) == -85070591730234615865843651857942052864wb, 0);
  check_invalid (testflt_135 (__builtin_inff ()) == 21778071482940061661655974875633165533183wb, 1);
  check_invalid (testflt_135 (-__builtin_inff ()) == -21778071482940061661655974875633165533183wb - 1wb, 1);
  check_invalid (testflt_135 (__builtin_nanf ("")) == 21778071482940061661655974875633165533183wb, 1);
  check_invalid (testflt_135 (0xffffffp+104f) == 340282346638528859811704183484516925440wb, 0);
  check_invalid (testflt_135 (-0xffffffp+104f) == -340282346638528859811704183484516925440wb, 0);
  check_invalid (testfltu_135 (-0.9990234375f) == 0uwb, 0);
  check_invalid (testfltu_135 (-1.f) == 0uwb, 1);
  check_invalid (testfltu_135 (__builtin_inff ()) == 43556142965880123323311949751266331066367uwb, 1);
  check_invalid (testfltu_135 (-__builtin_inff ()) == 0uwb, 1);
  check_invalid (testfltu_135 (__builtin_nanf ("")) == 43556142965880123323311949751266331066367uwb, 1);
  check_invalid (testfltu_135 (0xffffffp+104f) == 340282346638528859811704183484516925440uwb, 0);
#endif
#if __BITINT_MAXWIDTH__ >= 192
  check_invalid (testflt_192 (-85070591730234615865843651857942052864.0f) == -85070591730234615865843651857942052864wb, 0);
  check_invalid (testflt_192 (__builtin_inff ()) == 3138550867693340381917894711603833208051177722232017256447wb, 1);
  check_invalid (testflt_192 (-__builtin_inff ()) == -3138550867693340381917894711603833208051177722232017256447wb - 1wb, 1);
  check_invalid (testflt_192 (__builtin_nanf ("")) == 3138550867693340381917894711603833208051177722232017256447wb, 1);
  check_invalid (testflt_192 (0xffffffp+104f) == 340282346638528859811704183484516925440wb, 0);
  check_invalid (testflt_192 (-0xffffffp+104f) == -340282346638528859811704183484516925440wb, 0);
  check_invalid (testfltu_192 (-0.9990234375f) == 0uwb, 0);
  check_invalid (testfltu_192 (-1.f) == 0uwb, 1);
  check_invalid (testfltu_192 (__builtin_inff ()) == 6277101735386680763835789423207666416102355444464034512895uwb, 1);
  check_invalid (testfltu_192 (-__builtin_inff ()) == 0uwb, 1);
  check_invalid (testfltu_192 (__builtin_nanf ("")) == 6277101735386680763835789423207666416102355444464034512895uwb, 1);
  check_invalid (testfltu_192 (0xffffffp+104f) == 340282346638528859811704183484516925440uwb, 0);
#endif
#if __BITINT_MAXWIDTH__ >= 575
  check_invalid (testflt_575 (-85070591730234615865843651857942052864.0f) == -85070591730234615865843651857942052864wb, 0);
  check_invalid (testflt_575 (__builtin_inff ()) == 61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783wb, 1);
  check_invalid (testflt_575 (-__builtin_inff ()) == -61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783wb - 1wb, 1);
  check_invalid (testflt_575 (__builtin_nanf ("")) == 61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783wb, 1);
  check_invalid (testflt_575 (0xffffffp+104f) == 340282346638528859811704183484516925440wb, 0);
  check_invalid (testflt_575 (-0xffffffp+104f) == -340282346638528859811704183484516925440wb, 0);
  check_invalid (testfltu_575 (-0.9990234375f) == 0uwb, 0);
  check_invalid (testfltu_575 (-1.f) == 0uwb, 1);
  check_invalid (testfltu_575 (__builtin_inff ()) == 123665200736552267030251260509823595017565674550605919957031528046448612553265933585158200530621522494798835713008069669675682517153375604983773077550946583958303386074349567uwb, 1);
  check_invalid (testfltu_575 (-__builtin_inff ()) == 0uwb, 1);
  check_invalid (testfltu_575 (__builtin_nanf ("")) == 123665200736552267030251260509823595017565674550605919957031528046448612553265933585158200530621522494798835713008069669675682517153375604983773077550946583958303386074349567uwb, 1);
  check_invalid (testfltu_575 (0xffffffp+104f) == 340282346638528859811704183484516925440uwb, 0);
#endif
#endif
#if __DBL_MANT_DIG__ == 53
#if __BITINT_MAXWIDTH__ >= 135
  check_invalid (testdbl_135 (-85070591730234615865843651857942052864.0) == -85070591730234615865843651857942052864wb, 0);
  check_invalid (testdbl_135 (__builtin_inf ()) == 21778071482940061661655974875633165533183wb, 1);
  check_invalid (testdbl_135 (-__builtin_inf ()) == -21778071482940061661655974875633165533183wb - 1wb, 1);
  check_invalid (testdbl_135 (__builtin_nan ("")) == 21778071482940061661655974875633165533183wb, 1);
  check_invalid (testdbl_135 (0x1fffffffffffffp+81) == 21778071482940059243804335646374816120832wb, 0);
  check_invalid (testdbl_135 (0x20000000000000p+81) == 21778071482940061661655974875633165533183wb, 1);
  check_invalid (testdbl_135 (-0x20000000000000p+81) == -21778071482940061661655974875633165533183wb - 1, 0);
  check_invalid (testdbl_135 (-0x20000000000002p+81) == -21778071482940061661655974875633165533183wb - 1, 1);
  check_invalid (testdblu_135 (-0.9990234375) == 0uwb, 0);
  check_invalid (testdblu_135 (-1.) == 0uwb, 1);
  check_invalid (testdblu_135 (__builtin_inf ()) == 43556142965880123323311949751266331066367uwb, 1);
  check_invalid (testdblu_135 (-__builtin_inf ()) == 0uwb, 1);
  check_invalid (testdblu_135 (__builtin_nan ("")) == 43556142965880123323311949751266331066367uwb, 1);
  check_invalid (testdblu_135 (0x1fffffffffffffp+82) == 43556142965880118487608671292749632241664uwb, 0);
  check_invalid (testdblu_135 (0x20000000000000p+82) == 43556142965880123323311949751266331066367uwb, 1);
#endif
#if __BITINT_MAXWIDTH__ >= 192
  check_invalid (testdbl_192 (-85070591730234615865843651857942052864.0) == -85070591730234615865843651857942052864wb, 0);
  check_invalid (testdbl_192 (__builtin_inf ()) == 3138550867693340381917894711603833208051177722232017256447wb, 1);
  check_invalid (testdbl_192 (-__builtin_inf ()) == -3138550867693340381917894711603833208051177722232017256447wb - 1wb, 1);
  check_invalid (testdbl_192 (__builtin_nan ("")) == 3138550867693340381917894711603833208051177722232017256447wb, 1);
  check_invalid (testdbl_192 (0x1fffffffffffffp+138) == 3138550867693340033468750984562846621555579712101368725504wb, 0);
  check_invalid (testdbl_192 (0x20000000000000p+138) == 3138550867693340381917894711603833208051177722232017256447wb, 1);
  check_invalid (testdbl_192 (-0x20000000000000p+138) == -3138550867693340381917894711603833208051177722232017256447wb - 1, 0);
  check_invalid (testdbl_192 (-0x20000000000002p+138) == -3138550867693340381917894711603833208051177722232017256447wb - 1, 1);
  check_invalid (testdblu_192 (-0.9990234375) == 0uwb, 0);
  check_invalid (testdblu_192 (-1.) == 0uwb, 1);
  check_invalid (testdblu_192 (__builtin_inf ()) == 6277101735386680763835789423207666416102355444464034512895uwb, 1);
  check_invalid (testdblu_192 (-__builtin_inf ()) == 0uwb, 1);
  check_invalid (testdblu_192 (__builtin_nan ("")) == 6277101735386680763835789423207666416102355444464034512895uwb, 1);
  check_invalid (testdblu_192 (0x1fffffffffffffp+139) == 6277101735386680066937501969125693243111159424202737451008uwb, 0);
  check_invalid (testdblu_192 (0x20000000000000p+139) == 6277101735386680763835789423207666416102355444464034512895uwb, 1);
#endif
#if __BITINT_MAXWIDTH__ >= 575
  check_invalid (testdbl_575 (-85070591730234615865843651857942052864.0) == -85070591730234615865843651857942052864wb, 0);
  check_invalid (testdbl_575 (__builtin_inf ()) == 61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783wb, 1);
  check_invalid (testdbl_575 (-__builtin_inf ()) == -61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783wb - 1wb, 1);
  check_invalid (testdbl_575 (__builtin_nan ("")) == 61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783wb, 1);
  check_invalid (testdbl_575 (0x1fffffffffffffp+521) == 61832600368276126650327970124302082526882038193909742709080463879918896882169507607035916867654709124839777195049479857541529867095829765369898539058829479405123401922117632wb, 0);
  check_invalid (testdbl_575 (0x20000000000000p+521) == 61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783wb, 1);
  check_invalid (testdbl_575 (-0x20000000000000p+521) == -61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783wb - 1, 0);
  check_invalid (testdbl_575 (-0x20000000000002p+521) == -61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783wb - 1, 1);
  check_invalid (testdblu_575 (-0.9990234375) == 0uwb, 0);
  check_invalid (testdblu_575 (-1.) == 0uwb, 1);
  check_invalid (testdblu_575 (__builtin_inf ()) == 123665200736552267030251260509823595017565674550605919957031528046448612553265933585158200530621522494798835713008069669675682517153375604983773077550946583958303386074349567uwb, 1);
  check_invalid (testdblu_575 (-__builtin_inf ()) == 0uwb, 1);
  check_invalid (testdblu_575 (__builtin_nan ("")) == 123665200736552267030251260509823595017565674550605919957031528046448612553265933585158200530621522494798835713008069669675682517153375604983773077550946583958303386074349567uwb, 1);
  check_invalid (testdblu_575 (0x1fffffffffffffp+522) == 123665200736552253300655940248604165053764076387819485418160927759837793764339015214071833735309418249679554390098959715083059734191659530739797078117658958810246803844235264uwb, 0);
  check_invalid (testdblu_575 (0x20000000000000p+522) == 123665200736552267030251260509823595017565674550605919957031528046448612553265933585158200530621522494798835713008069669675682517153375604983773077550946583958303386074349567uwb, 1);
#endif
#endif
#if __LDBL_MANT_DIG__ == 64
#if __BITINT_MAXWIDTH__ >= 135
  check_invalid (testldbl_135 (-85070591730234615865843651857942052864.0L) == -85070591730234615865843651857942052864wb, 0);
  check_invalid (testldbl_135 (__builtin_infl ()) == 21778071482940061661655974875633165533183wb, 1);
  check_invalid (testldbl_135 (-__builtin_infl ()) == -21778071482940061661655974875633165533183wb - 1wb, 1);
  check_invalid (testldbl_135 (__builtin_nanl ("")) == 21778071482940061661655974875633165533183wb, 1);
  check_invalid (testldbl_135 (0xffffffffffffffffp+70L) == 21778071482940061660475383254915754229760wb, 0);
  check_invalid (testldbl_135 (0x10000000000000000p+70L) == 21778071482940061661655974875633165533183wb, 1);
  check_invalid (testldbl_135 (-0x10000000000000000p+70L) == -21778071482940061661655974875633165533183wb - 1, 0);
  check_invalid (testldbl_135 (-0x10000000000000002p+70L) == -21778071482940061661655974875633165533183wb - 1, 1);
  check_invalid (testldblu_135 (-0.9990234375L) == 0uwb, 0);
  check_invalid (testldblu_135 (-1.L) == 0uwb, 1);
  check_invalid (testldblu_135 (__builtin_infl ()) == 43556142965880123323311949751266331066367uwb, 1);
  check_invalid (testldblu_135 (-__builtin_infl ()) == 0uwb, 1);
  check_invalid (testldblu_135 (__builtin_nanl ("")) == 43556142965880123323311949751266331066367uwb, 1);
  check_invalid (testldblu_135 (0xffffffffffffffffp+71L) == 43556142965880123320950766509831508459520uwb, 0);
  check_invalid (testldblu_135 (0x10000000000000000p+71L) == 43556142965880123323311949751266331066367uwb, 1);
#endif
#if __BITINT_MAXWIDTH__ >= 192
  check_invalid (testldbl_192 (-85070591730234615865843651857942052864.0L) == -85070591730234615865843651857942052864wb, 0);
  check_invalid (testldbl_192 (__builtin_infl ()) == 3138550867693340381917894711603833208051177722232017256447wb, 1);
  check_invalid (testldbl_192 (-__builtin_infl ()) == -3138550867693340381917894711603833208051177722232017256447wb - 1wb, 1);
  check_invalid (testldbl_192 (__builtin_nanl ("")) == 3138550867693340381917894711603833208051177722232017256447wb, 1);
  check_invalid (testldbl_192 (0xffffffffffffffffp+127L) == 3138550867693340381747753528143363976319490418516133150720wb, 0);
  check_invalid (testldbl_192 (0x10000000000000000p+127L) == 3138550867693340381917894711603833208051177722232017256447wb, 1);
  check_invalid (testldbl_192 (-0x10000000000000000p+127L) == -3138550867693340381917894711603833208051177722232017256447wb - 1, 0);
  check_invalid (testldbl_192 (-0x10000000000000002p+127L) == -3138550867693340381917894711603833208051177722232017256447wb - 1, 1);
  check_invalid (testldblu_192 (-0.9990234375L) == 0uwb, 0);
  check_invalid (testldblu_192 (-1.L) == 0uwb, 1);
  check_invalid (testldblu_192 (__builtin_infl ()) == 6277101735386680763835789423207666416102355444464034512895uwb, 1);
  check_invalid (testldblu_192 (-__builtin_infl ()) == 0uwb, 1);
  check_invalid (testldblu_192 (__builtin_nanl ("")) == 6277101735386680763835789423207666416102355444464034512895uwb, 1);
  check_invalid (testldblu_192 (0xffffffffffffffffp+128L) == 6277101735386680763495507056286727952638980837032266301440uwb, 0);
  check_invalid (testldblu_192 (0x10000000000000000p+128L) == 6277101735386680763835789423207666416102355444464034512895uwb, 1);
#endif
#if __BITINT_MAXWIDTH__ >= 575
  check_invalid (testldbl_575 (-85070591730234615865843651857942052864.0L) == -85070591730234615865843651857942052864wb, 0);
  check_invalid (testldbl_575 (__builtin_infl ()) == 61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783wb, 1);
  check_invalid (testldbl_575 (-__builtin_infl ()) == -61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783wb - 1wb, 1);
  check_invalid (testldbl_575 (__builtin_nanl ("")) == 61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783wb, 1);
  check_invalid (testldbl_575 (0xffffffffffffffffp+510L) == 61832600368276133511773678272426148233889331025751498446645922568076207932202076431648659257792374503198949281962308977915333294030066289778448068072486649492543280785653760wb, 0);
  check_invalid (testldbl_575 (0x10000000000000000p+510L) == 61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783wb, 1);
  check_invalid (testldbl_575 (-0x10000000000000000p+510L) == -61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783wb - 1, 0);
  check_invalid (testldbl_575 (-0x10000000000000002p+510L) == -61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783wb - 1, 1);
  check_invalid (testldblu_575 (-0.9990234375L) == 0uwb, 0);
  check_invalid (testldblu_575 (-1.L) == 0uwb, 1);
  check_invalid (testldblu_575 (__builtin_infl ()) == 123665200736552267030251260509823595017565674550605919957031528046448612553265933585158200530621522494798835713008069669675682517153375604983773077550946583958303386074349567uwb, 1);
  check_invalid (testldblu_575 (-__builtin_infl ()) == 0uwb, 1);
  check_invalid (testldblu_575 (__builtin_nanl ("")) == 123665200736552267030251260509823595017565674550605919957031528046448612553265933585158200530621522494798835713008069669675682517153375604983773077550946583958303386074349567uwb, 1);
  check_invalid (testldblu_575 (0xffffffffffffffffp+511L) == 123665200736552267023547356544852296467778662051502996893291845136152415864404152863297318515584749006397898563924617955830666588060132579556896136144973298985086561571307520uwb, 0);
  check_invalid (testldblu_575 (0x10000000000000000p+511L) == 123665200736552267030251260509823595017565674550605919957031528046448612553265933585158200530621522494798835713008069669675682517153375604983773077550946583958303386074349567uwb, 1);
#endif
#endif
#if __FLT128_MANT_DIG__ == 113
#if __BITINT_MAXWIDTH__ >= 135
  check_invalid (testflt128_135 (-85070591730234615865843651857942052864.0F128) == -85070591730234615865843651857942052864wb, 0);
  check_invalid (testflt128_135 (__builtin_inff128 ()) == 21778071482940061661655974875633165533183wb, 1);
  check_invalid (testflt128_135 (-__builtin_inff128 ()) == -21778071482940061661655974875633165533183wb - 1wb, 1);
  check_invalid (testflt128_135 (__builtin_nanf128 ("")) == 21778071482940061661655974875633165533183wb, 1);
  check_invalid (testflt128_135 (0x1ffffffffffffffffffffffffffffp+21F128) == 21778071482940061661655974875633163436032wb, 0);
  check_invalid (testflt128_135 (0x20000000000000000000000000000p+21F128) == 21778071482940061661655974875633165533183wb, 1);
  check_invalid (testflt128_135 (-0x20000000000000000000000000000p+21F128) == -21778071482940061661655974875633165533183wb - 1, 0);
  check_invalid (testflt128_135 (-0x20000000000000000000000000002p+21F128) == -21778071482940061661655974875633165533183wb - 1, 1);
  check_invalid (testflt128u_135 (-0.9990234375F128) == 0uwb, 0);
  check_invalid (testflt128u_135 (-1.F128) == 0uwb, 1);
  check_invalid (testflt128u_135 (__builtin_inff128 ()) == 43556142965880123323311949751266331066367uwb, 1);
  check_invalid (testflt128u_135 (-__builtin_inff128 ()) == 0uwb, 1);
  check_invalid (testflt128u_135 (__builtin_nanf128 ("")) == 43556142965880123323311949751266331066367uwb, 1);
  check_invalid (testflt128u_135 (0x1ffffffffffffffffffffffffffffp+22F128) == 43556142965880123323311949751266326872064uwb, 0);
  check_invalid (testflt128u_135 (0x20000000000000000000000000000p+22F128) == 43556142965880123323311949751266331066367uwb, 1);
#endif
#if __BITINT_MAXWIDTH__ >= 192
  check_invalid (testflt128_192 (-85070591730234615865843651857942052864.0F128) == -85070591730234615865843651857942052864wb, 0);
  check_invalid (testflt128_192 (__builtin_inff128 ()) == 3138550867693340381917894711603833208051177722232017256447wb, 1);
  check_invalid (testflt128_192 (-__builtin_inff128 ()) == -3138550867693340381917894711603833208051177722232017256447wb - 1wb, 1);
  check_invalid (testflt128_192 (__builtin_nanf128 ("")) == 3138550867693340381917894711603833208051177722232017256447wb, 1);
  check_invalid (testflt128_192 (0x1ffffffffffffffffffffffffffffp+78F128) == 3138550867693340381917894711603832905819722818574723579904wb, 0);
  check_invalid (testflt128_192 (0x20000000000000000000000000000p+78F128) == 3138550867693340381917894711603833208051177722232017256447wb, 1);
  check_invalid (testflt128_192 (-0x20000000000000000000000000000p+78F128) == -3138550867693340381917894711603833208051177722232017256447wb - 1, 0);
  check_invalid (testflt128_192 (-0x20000000000000000000000000002p+78F128) == -3138550867693340381917894711603833208051177722232017256447wb - 1, 1);
  check_invalid (testflt128u_192 (-0.9990234375F128) == 0uwb, 0);
  check_invalid (testflt128u_192 (-1.F128) == 0uwb, 1);
  check_invalid (testflt128u_192 (__builtin_inff128 ()) == 6277101735386680763835789423207666416102355444464034512895uwb, 1);
  check_invalid (testflt128u_192 (-__builtin_inff128 ()) == 0uwb, 1);
  check_invalid (testflt128u_192 (__builtin_nanf128 ("")) == 6277101735386680763835789423207666416102355444464034512895uwb, 1);
  check_invalid (testflt128u_192 (0x1ffffffffffffffffffffffffffffp+79F128) == 6277101735386680763835789423207665811639445637149447159808uwb, 0);
  check_invalid (testflt128u_192 (0x20000000000000000000000000000p+79F128) == 6277101735386680763835789423207666416102355444464034512895uwb, 1);
#endif
#if __BITINT_MAXWIDTH__ >= 575
  check_invalid (testflt128_575 (-85070591730234615865843651857942052864.0F128) == -85070591730234615865843651857942052864wb, 0);
  check_invalid (testflt128_575 (__builtin_inff128 ()) == 61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783wb, 1);
  check_invalid (testflt128_575 (-__builtin_inff128 ()) == -61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783wb - 1wb, 1);
  check_invalid (testflt128_575 (__builtin_nanf128 ("")) == 61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783wb, 1);
  check_invalid (testflt128_575 (0x1ffffffffffffffffffffffffffffp+461F128) == 61832600368276133515125630254911791554520007845691312598455129804691160851602940042069550439343049559602369631548246946680753811425558728725309540242943660463695151425912832wb, 0);
  check_invalid (testflt128_575 (0x20000000000000000000000000000p+461F128) == 61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783wb, 1);
  check_invalid (testflt128_575 (-0x20000000000000000000000000000p+461F128) == -61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783wb - 1, 0);
  check_invalid (testflt128_575 (-0x20000000000000000000000000002p+461F128) == -61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783wb - 1, 1);
  check_invalid (testflt128u_575 (-0.9990234375F128) == 0uwb, 0);
  check_invalid (testflt128u_575 (-1.F128) == 0uwb, 1);
  check_invalid (testflt128u_575 (__builtin_inff128 ()) == 123665200736552267030251260509823595017565674550605919957031528046448612553265933585158200530621522494798835713008069669675682517153375604983773077550946583958303386074349567uwb, 1);
  check_invalid (testflt128u_575 (-__builtin_inff128 ()) == 0uwb, 1);
  check_invalid (testflt128u_575 (__builtin_nanf128 ("")) == 123665200736552267030251260509823595017565674550605919957031528046448612553265933585158200530621522494798835713008069669675682517153375604983773077550946583958303386074349567uwb, 1);
  check_invalid (testflt128u_575 (0x1ffffffffffffffffffffffffffffp+462F128) == 123665200736552267030251260509823583109040015691382625196910259609382321703205880084139100878686099119204739263096493893361507622851117457450619080485887320927390302851825664uwb, 0);
  check_invalid (testflt128u_575 (0x20000000000000000000000000000p+462F128) == 123665200736552267030251260509823595017565674550605919957031528046448612553265933585158200530621522494798835713008069669675682517153375604983773077550946583958303386074349567uwb, 1);
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
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_6:[0-9]+]] .str[[VALUE_str_6]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_7:[0-9]+]] .str[[VALUE_str_7]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_8:[0-9]+]] .str[[VALUE_str_8]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_9:[0-9]+]] .str[[VALUE_str_9]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_10:[0-9]+]] .str[[VALUE_str_10]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_11:[0-9]+]] .str[[VALUE_str_11]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_12:[0-9]+]] .str[[VALUE_str_12]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_13:[0-9]+]] .str[[VALUE_str_13]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_14:[0-9]+]] .str[[VALUE_str_14]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_15:[0-9]+]] .str[[VALUE_str_15]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_16:[0-9]+]] .str[[VALUE_str_16]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_17:[0-9]+]] .str[[VALUE_str_17]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_18:[0-9]+]] .str[[VALUE_str_18]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_feclearexcept:[0-9]+]] @feclearexcept(%[[VALUE___excepts:[0-9]+]] __excepts: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fetestexcept:[0-9]+]] @fetestexcept(%[[VALUE___excepts_2:[0-9]+]] __excepts: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_testflt_135:[0-9]+]] @testflt_135(%[[VALUE_d:[0-9]+]] d: f32) -> i135b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<i135b, reason=return, out_of_range=ub, exceptions=ignore>(read<f32>(%[[VALUE_d]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testfltu_135:[0-9]+]] @testfltu_135(%[[VALUE_d_2:[0-9]+]] d: f32) -> u135b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<u135b, reason=return, out_of_range=ub, exceptions=ignore>(read<f32>(%[[VALUE_d_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testflt_192:[0-9]+]] @testflt_192(%[[VALUE_d_3:[0-9]+]] d: f32) -> i192b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<i192b, reason=return, out_of_range=ub, exceptions=ignore>(read<f32>(%[[VALUE_d_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testfltu_192:[0-9]+]] @testfltu_192(%[[VALUE_d_4:[0-9]+]] d: f32) -> u192b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<u192b, reason=return, out_of_range=ub, exceptions=ignore>(read<f32>(%[[VALUE_d_4]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testflt_575:[0-9]+]] @testflt_575(%[[VALUE_d_5:[0-9]+]] d: f32) -> i575b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<i575b, reason=return, out_of_range=ub, exceptions=ignore>(read<f32>(%[[VALUE_d_5]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testfltu_575:[0-9]+]] @testfltu_575(%[[VALUE_d_6:[0-9]+]] d: f32) -> u575b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<u575b, reason=return, out_of_range=ub, exceptions=ignore>(read<f32>(%[[VALUE_d_6]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testdbl_135:[0-9]+]] @testdbl_135(%[[VALUE_d_7:[0-9]+]] d: f64) -> i135b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<i135b, reason=return, out_of_range=ub, exceptions=ignore>(read<f64>(%[[VALUE_d_7]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testdblu_135:[0-9]+]] @testdblu_135(%[[VALUE_d_8:[0-9]+]] d: f64) -> u135b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<u135b, reason=return, out_of_range=ub, exceptions=ignore>(read<f64>(%[[VALUE_d_8]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testdbl_192:[0-9]+]] @testdbl_192(%[[VALUE_d_9:[0-9]+]] d: f64) -> i192b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<i192b, reason=return, out_of_range=ub, exceptions=ignore>(read<f64>(%[[VALUE_d_9]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testdblu_192:[0-9]+]] @testdblu_192(%[[VALUE_d_10:[0-9]+]] d: f64) -> u192b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<u192b, reason=return, out_of_range=ub, exceptions=ignore>(read<f64>(%[[VALUE_d_10]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testdbl_575:[0-9]+]] @testdbl_575(%[[VALUE_d_11:[0-9]+]] d: f64) -> i575b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<i575b, reason=return, out_of_range=ub, exceptions=ignore>(read<f64>(%[[VALUE_d_11]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testdblu_575:[0-9]+]] @testdblu_575(%[[VALUE_d_12:[0-9]+]] d: f64) -> u575b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<u575b, reason=return, out_of_range=ub, exceptions=ignore>(read<f64>(%[[VALUE_d_12]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testldbl_135:[0-9]+]] @testldbl_135(%[[VALUE_d_13:[0-9]+]] d: f80) -> i135b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<i135b, reason=return, out_of_range=ub, exceptions=ignore>(read<f80>(%[[VALUE_d_13]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testldblu_135:[0-9]+]] @testldblu_135(%[[VALUE_d_14:[0-9]+]] d: f80) -> u135b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<u135b, reason=return, out_of_range=ub, exceptions=ignore>(read<f80>(%[[VALUE_d_14]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testldbl_192:[0-9]+]] @testldbl_192(%[[VALUE_d_15:[0-9]+]] d: f80) -> i192b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<i192b, reason=return, out_of_range=ub, exceptions=ignore>(read<f80>(%[[VALUE_d_15]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testldblu_192:[0-9]+]] @testldblu_192(%[[VALUE_d_16:[0-9]+]] d: f80) -> u192b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<u192b, reason=return, out_of_range=ub, exceptions=ignore>(read<f80>(%[[VALUE_d_16]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testldbl_575:[0-9]+]] @testldbl_575(%[[VALUE_d_17:[0-9]+]] d: f80) -> i575b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<i575b, reason=return, out_of_range=ub, exceptions=ignore>(read<f80>(%[[VALUE_d_17]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testldblu_575:[0-9]+]] @testldblu_575(%[[VALUE_d_18:[0-9]+]] d: f80) -> u575b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<u575b, reason=return, out_of_range=ub, exceptions=ignore>(read<f80>(%[[VALUE_d_18]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_check_invalid:[0-9]+]] @check_invalid(%[[VALUE_test:[0-9]+]] test: i32, %[[VALUE_inv:[0-9]+]] inv: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32>(%[[VALUE_test]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(not<bool>(ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_fetestexcept]], const<i32>(1)), const<i32>(0)))), from_bool<i32, reason=promotion>(not<bool>(ne<i32>(read<i32>(%[[VALUE_inv]]), const<i32>(0)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%[[VALUE_feclearexcept]], const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_inff:[0-9]+]] @__builtin_inff() -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nanf:[0-9]+]] @__builtin_nanf(%[[VALUE0:[0-9]+]] <unnamed>: ptr<const i8>) -> f32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_inf:[0-9]+]] @__builtin_inf() -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nan:[0-9]+]] @__builtin_nan(%[[VALUE1:[0-9]+]] <unnamed>: ptr<const i8>) -> f64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_infl:[0-9]+]] @__builtin_infl() -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nanl:[0-9]+]] @__builtin_nanl(%[[VALUE2:[0-9]+]] <unnamed>: ptr<const i8>) -> f80 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i135b>(call<i135b, signature=fn(f32) -> i135b>(%[[VALUE_testflt_135]], neg<f32>(const<f32>(8.507059e37))), widen<i135b, reason=usual_arith>(neg<i128b, overflow=ub>(const<i128b>(85070591730234615865843651857942052864))))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i135b>(call<i135b, signature=fn(f32) -> i135b>(%[[VALUE_testflt_135]], call<f32, signature=fn() -> f32>(%[[VALUE___builtin_inff]])), const<i135b>(21778071482940061661655974875633165533183))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i135b>(call<i135b, signature=fn(f32) -> i135b>(%[[VALUE_testflt_135]], neg<f32>(call<f32, signature=fn() -> f32>(%[[VALUE___builtin_inff]]))), sub<i135b, overflow=ub>(neg<i135b, overflow=ub>(const<i135b>(21778071482940061661655974875633165533183)), widen<i135b, reason=usual_arith>(const<i2b>(1))))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i135b>(call<i135b, signature=fn(f32) -> i135b>(%[[VALUE_testflt_135]], call<f32, signature=fn(ptr<const i8>) -> f32>(%[[VALUE___builtin_nanf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str]])))), const<i135b>(21778071482940061661655974875633165533183))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i135b>(call<i135b, signature=fn(f32) -> i135b>(%[[VALUE_testflt_135]], const<f32>(3.4028235e38)), widen<i135b, reason=usual_arith>(const<i129b>(340282346638528859811704183484516925440)))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i135b>(call<i135b, signature=fn(f32) -> i135b>(%[[VALUE_testflt_135]], neg<f32>(const<f32>(3.4028235e38))), widen<i135b, reason=usual_arith>(neg<i129b, overflow=ub>(const<i129b>(340282346638528859811704183484516925440))))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u135b>(call<u135b, signature=fn(f32) -> u135b>(%[[VALUE_testfltu_135]], neg<f32>(const<f32>(0.99902344))), widen<u135b, reason=usual_arith>(const<u1b>(0)))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u135b>(call<u135b, signature=fn(f32) -> u135b>(%[[VALUE_testfltu_135]], neg<f32>(const<f32>(1.0))), widen<u135b, reason=usual_arith>(const<u1b>(0)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u135b>(call<u135b, signature=fn(f32) -> u135b>(%[[VALUE_testfltu_135]], call<f32, signature=fn() -> f32>(%[[VALUE___builtin_inff]])), const<u135b>(43556142965880123323311949751266331066367))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u135b>(call<u135b, signature=fn(f32) -> u135b>(%[[VALUE_testfltu_135]], neg<f32>(call<f32, signature=fn() -> f32>(%[[VALUE___builtin_inff]]))), widen<u135b, reason=usual_arith>(const<u1b>(0)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u135b>(call<u135b, signature=fn(f32) -> u135b>(%[[VALUE_testfltu_135]], call<f32, signature=fn(ptr<const i8>) -> f32>(%[[VALUE___builtin_nanf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_2]])))), const<u135b>(43556142965880123323311949751266331066367))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u135b>(call<u135b, signature=fn(f32) -> u135b>(%[[VALUE_testfltu_135]], const<f32>(3.4028235e38)), widen<u135b, reason=usual_arith>(const<u128b>(340282346638528859811704183484516925440)))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i192b>(call<i192b, signature=fn(f32) -> i192b>(%[[VALUE_testflt_192]], neg<f32>(const<f32>(8.507059e37))), widen<i192b, reason=usual_arith>(neg<i128b, overflow=ub>(const<i128b>(85070591730234615865843651857942052864))))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i192b>(call<i192b, signature=fn(f32) -> i192b>(%[[VALUE_testflt_192]], call<f32, signature=fn() -> f32>(%[[VALUE___builtin_inff]])), const<i192b>(3138550867693340381917894711603833208051177722232017256447))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i192b>(call<i192b, signature=fn(f32) -> i192b>(%[[VALUE_testflt_192]], neg<f32>(call<f32, signature=fn() -> f32>(%[[VALUE___builtin_inff]]))), sub<i192b, overflow=ub>(neg<i192b, overflow=ub>(const<i192b>(3138550867693340381917894711603833208051177722232017256447)), widen<i192b, reason=usual_arith>(const<i2b>(1))))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i192b>(call<i192b, signature=fn(f32) -> i192b>(%[[VALUE_testflt_192]], call<f32, signature=fn(ptr<const i8>) -> f32>(%[[VALUE___builtin_nanf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_3]])))), const<i192b>(3138550867693340381917894711603833208051177722232017256447))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i192b>(call<i192b, signature=fn(f32) -> i192b>(%[[VALUE_testflt_192]], const<f32>(3.4028235e38)), widen<i192b, reason=usual_arith>(const<i129b>(340282346638528859811704183484516925440)))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i192b>(call<i192b, signature=fn(f32) -> i192b>(%[[VALUE_testflt_192]], neg<f32>(const<f32>(3.4028235e38))), widen<i192b, reason=usual_arith>(neg<i129b, overflow=ub>(const<i129b>(340282346638528859811704183484516925440))))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u192b>(call<u192b, signature=fn(f32) -> u192b>(%[[VALUE_testfltu_192]], neg<f32>(const<f32>(0.99902344))), widen<u192b, reason=usual_arith>(const<u1b>(0)))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u192b>(call<u192b, signature=fn(f32) -> u192b>(%[[VALUE_testfltu_192]], neg<f32>(const<f32>(1.0))), widen<u192b, reason=usual_arith>(const<u1b>(0)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u192b>(call<u192b, signature=fn(f32) -> u192b>(%[[VALUE_testfltu_192]], call<f32, signature=fn() -> f32>(%[[VALUE___builtin_inff]])), const<u192b>(6277101735386680763835789423207666416102355444464034512895))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u192b>(call<u192b, signature=fn(f32) -> u192b>(%[[VALUE_testfltu_192]], neg<f32>(call<f32, signature=fn() -> f32>(%[[VALUE___builtin_inff]]))), widen<u192b, reason=usual_arith>(const<u1b>(0)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u192b>(call<u192b, signature=fn(f32) -> u192b>(%[[VALUE_testfltu_192]], call<f32, signature=fn(ptr<const i8>) -> f32>(%[[VALUE___builtin_nanf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_4]])))), const<u192b>(6277101735386680763835789423207666416102355444464034512895))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u192b>(call<u192b, signature=fn(f32) -> u192b>(%[[VALUE_testfltu_192]], const<f32>(3.4028235e38)), widen<u192b, reason=usual_arith>(const<u128b>(340282346638528859811704183484516925440)))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i575b>(call<i575b, signature=fn(f32) -> i575b>(%[[VALUE_testflt_575]], neg<f32>(const<f32>(8.507059e37))), widen<i575b, reason=usual_arith>(neg<i128b, overflow=ub>(const<i128b>(85070591730234615865843651857942052864))))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i575b>(call<i575b, signature=fn(f32) -> i575b>(%[[VALUE_testflt_575]], call<f32, signature=fn() -> f32>(%[[VALUE___builtin_inff]])), const<i575b>(61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i575b>(call<i575b, signature=fn(f32) -> i575b>(%[[VALUE_testflt_575]], neg<f32>(call<f32, signature=fn() -> f32>(%[[VALUE___builtin_inff]]))), sub<i575b, overflow=ub>(neg<i575b, overflow=ub>(const<i575b>(61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783)), widen<i575b, reason=usual_arith>(const<i2b>(1))))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i575b>(call<i575b, signature=fn(f32) -> i575b>(%[[VALUE_testflt_575]], call<f32, signature=fn(ptr<const i8>) -> f32>(%[[VALUE___builtin_nanf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_5]])))), const<i575b>(61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i575b>(call<i575b, signature=fn(f32) -> i575b>(%[[VALUE_testflt_575]], const<f32>(3.4028235e38)), widen<i575b, reason=usual_arith>(const<i129b>(340282346638528859811704183484516925440)))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i575b>(call<i575b, signature=fn(f32) -> i575b>(%[[VALUE_testflt_575]], neg<f32>(const<f32>(3.4028235e38))), widen<i575b, reason=usual_arith>(neg<i129b, overflow=ub>(const<i129b>(340282346638528859811704183484516925440))))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u575b>(call<u575b, signature=fn(f32) -> u575b>(%[[VALUE_testfltu_575]], neg<f32>(const<f32>(0.99902344))), widen<u575b, reason=usual_arith>(const<u1b>(0)))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u575b>(call<u575b, signature=fn(f32) -> u575b>(%[[VALUE_testfltu_575]], neg<f32>(const<f32>(1.0))), widen<u575b, reason=usual_arith>(const<u1b>(0)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u575b>(call<u575b, signature=fn(f32) -> u575b>(%[[VALUE_testfltu_575]], call<f32, signature=fn() -> f32>(%[[VALUE___builtin_inff]])), const<u575b>(123665200736552267030251260509823595017565674550605919957031528046448612553265933585158200530621522494798835713008069669675682517153375604983773077550946583958303386074349567))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u575b>(call<u575b, signature=fn(f32) -> u575b>(%[[VALUE_testfltu_575]], neg<f32>(call<f32, signature=fn() -> f32>(%[[VALUE___builtin_inff]]))), widen<u575b, reason=usual_arith>(const<u1b>(0)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u575b>(call<u575b, signature=fn(f32) -> u575b>(%[[VALUE_testfltu_575]], call<f32, signature=fn(ptr<const i8>) -> f32>(%[[VALUE___builtin_nanf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_6]])))), const<u575b>(123665200736552267030251260509823595017565674550605919957031528046448612553265933585158200530621522494798835713008069669675682517153375604983773077550946583958303386074349567))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u575b>(call<u575b, signature=fn(f32) -> u575b>(%[[VALUE_testfltu_575]], const<f32>(3.4028235e38)), widen<u575b, reason=usual_arith>(const<u128b>(340282346638528859811704183484516925440)))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i135b>(call<i135b, signature=fn(f64) -> i135b>(%[[VALUE_testdbl_135]], neg<f64>(const<f64>(8.507059173023462e37))), widen<i135b, reason=usual_arith>(neg<i128b, overflow=ub>(const<i128b>(85070591730234615865843651857942052864))))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i135b>(call<i135b, signature=fn(f64) -> i135b>(%[[VALUE_testdbl_135]], call<f64, signature=fn() -> f64>(%[[VALUE___builtin_inf]])), const<i135b>(21778071482940061661655974875633165533183))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i135b>(call<i135b, signature=fn(f64) -> i135b>(%[[VALUE_testdbl_135]], neg<f64>(call<f64, signature=fn() -> f64>(%[[VALUE___builtin_inf]]))), sub<i135b, overflow=ub>(neg<i135b, overflow=ub>(const<i135b>(21778071482940061661655974875633165533183)), widen<i135b, reason=usual_arith>(const<i2b>(1))))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i135b>(call<i135b, signature=fn(f64) -> i135b>(%[[VALUE_testdbl_135]], call<f64, signature=fn(ptr<const i8>) -> f64>(%[[VALUE___builtin_nan]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_7]])))), const<i135b>(21778071482940061661655974875633165533183))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i135b>(call<i135b, signature=fn(f64) -> i135b>(%[[VALUE_testdbl_135]], const<f64>(2.177807148294006e40)), const<i135b>(21778071482940059243804335646374816120832))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i135b>(call<i135b, signature=fn(f64) -> i135b>(%[[VALUE_testdbl_135]], const<f64>(2.1778071482940062e40)), const<i135b>(21778071482940061661655974875633165533183))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i135b>(call<i135b, signature=fn(f64) -> i135b>(%[[VALUE_testdbl_135]], neg<f64>(const<f64>(2.1778071482940062e40))), sub<i135b, overflow=ub>(neg<i135b, overflow=ub>(const<i135b>(21778071482940061661655974875633165533183)), widen<i135b, reason=usual_arith>(const<i32>(1))))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i135b>(call<i135b, signature=fn(f64) -> i135b>(%[[VALUE_testdbl_135]], neg<f64>(const<f64>(2.1778071482940066e40))), sub<i135b, overflow=ub>(neg<i135b, overflow=ub>(const<i135b>(21778071482940061661655974875633165533183)), widen<i135b, reason=usual_arith>(const<i32>(1))))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u135b>(call<u135b, signature=fn(f64) -> u135b>(%[[VALUE_testdblu_135]], neg<f64>(const<f64>(0.9990234375))), widen<u135b, reason=usual_arith>(const<u1b>(0)))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u135b>(call<u135b, signature=fn(f64) -> u135b>(%[[VALUE_testdblu_135]], neg<f64>(const<f64>(1.0))), widen<u135b, reason=usual_arith>(const<u1b>(0)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u135b>(call<u135b, signature=fn(f64) -> u135b>(%[[VALUE_testdblu_135]], call<f64, signature=fn() -> f64>(%[[VALUE___builtin_inf]])), const<u135b>(43556142965880123323311949751266331066367))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u135b>(call<u135b, signature=fn(f64) -> u135b>(%[[VALUE_testdblu_135]], neg<f64>(call<f64, signature=fn() -> f64>(%[[VALUE___builtin_inf]]))), widen<u135b, reason=usual_arith>(const<u1b>(0)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u135b>(call<u135b, signature=fn(f64) -> u135b>(%[[VALUE_testdblu_135]], call<f64, signature=fn(ptr<const i8>) -> f64>(%[[VALUE___builtin_nan]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_8]])))), const<u135b>(43556142965880123323311949751266331066367))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u135b>(call<u135b, signature=fn(f64) -> u135b>(%[[VALUE_testdblu_135]], const<f64>(4.355614296588012e40)), const<u135b>(43556142965880118487608671292749632241664))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u135b>(call<u135b, signature=fn(f64) -> u135b>(%[[VALUE_testdblu_135]], const<f64>(4.3556142965880123e40)), const<u135b>(43556142965880123323311949751266331066367))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i192b>(call<i192b, signature=fn(f64) -> i192b>(%[[VALUE_testdbl_192]], neg<f64>(const<f64>(8.507059173023462e37))), widen<i192b, reason=usual_arith>(neg<i128b, overflow=ub>(const<i128b>(85070591730234615865843651857942052864))))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i192b>(call<i192b, signature=fn(f64) -> i192b>(%[[VALUE_testdbl_192]], call<f64, signature=fn() -> f64>(%[[VALUE___builtin_inf]])), const<i192b>(3138550867693340381917894711603833208051177722232017256447))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i192b>(call<i192b, signature=fn(f64) -> i192b>(%[[VALUE_testdbl_192]], neg<f64>(call<f64, signature=fn() -> f64>(%[[VALUE___builtin_inf]]))), sub<i192b, overflow=ub>(neg<i192b, overflow=ub>(const<i192b>(3138550867693340381917894711603833208051177722232017256447)), widen<i192b, reason=usual_arith>(const<i2b>(1))))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i192b>(call<i192b, signature=fn(f64) -> i192b>(%[[VALUE_testdbl_192]], call<f64, signature=fn(ptr<const i8>) -> f64>(%[[VALUE___builtin_nan]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_9]])))), const<i192b>(3138550867693340381917894711603833208051177722232017256447))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i192b>(call<i192b, signature=fn(f64) -> i192b>(%[[VALUE_testdbl_192]], const<f64>(3.13855086769334e57)), const<i192b>(3138550867693340033468750984562846621555579712101368725504))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i192b>(call<i192b, signature=fn(f64) -> i192b>(%[[VALUE_testdbl_192]], const<f64>(3.1385508676933404e57)), const<i192b>(3138550867693340381917894711603833208051177722232017256447))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i192b>(call<i192b, signature=fn(f64) -> i192b>(%[[VALUE_testdbl_192]], neg<f64>(const<f64>(3.1385508676933404e57))), sub<i192b, overflow=ub>(neg<i192b, overflow=ub>(const<i192b>(3138550867693340381917894711603833208051177722232017256447)), widen<i192b, reason=usual_arith>(const<i32>(1))))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i192b>(call<i192b, signature=fn(f64) -> i192b>(%[[VALUE_testdbl_192]], neg<f64>(const<f64>(3.138550867693341e57))), sub<i192b, overflow=ub>(neg<i192b, overflow=ub>(const<i192b>(3138550867693340381917894711603833208051177722232017256447)), widen<i192b, reason=usual_arith>(const<i32>(1))))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u192b>(call<u192b, signature=fn(f64) -> u192b>(%[[VALUE_testdblu_192]], neg<f64>(const<f64>(0.9990234375))), widen<u192b, reason=usual_arith>(const<u1b>(0)))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u192b>(call<u192b, signature=fn(f64) -> u192b>(%[[VALUE_testdblu_192]], neg<f64>(const<f64>(1.0))), widen<u192b, reason=usual_arith>(const<u1b>(0)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u192b>(call<u192b, signature=fn(f64) -> u192b>(%[[VALUE_testdblu_192]], call<f64, signature=fn() -> f64>(%[[VALUE___builtin_inf]])), const<u192b>(6277101735386680763835789423207666416102355444464034512895))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u192b>(call<u192b, signature=fn(f64) -> u192b>(%[[VALUE_testdblu_192]], neg<f64>(call<f64, signature=fn() -> f64>(%[[VALUE___builtin_inf]]))), widen<u192b, reason=usual_arith>(const<u1b>(0)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u192b>(call<u192b, signature=fn(f64) -> u192b>(%[[VALUE_testdblu_192]], call<f64, signature=fn(ptr<const i8>) -> f64>(%[[VALUE___builtin_nan]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_10]])))), const<u192b>(6277101735386680763835789423207666416102355444464034512895))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u192b>(call<u192b, signature=fn(f64) -> u192b>(%[[VALUE_testdblu_192]], const<f64>(6.27710173538668e57)), const<u192b>(6277101735386680066937501969125693243111159424202737451008))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u192b>(call<u192b, signature=fn(f64) -> u192b>(%[[VALUE_testdblu_192]], const<f64>(6.277101735386681e57)), const<u192b>(6277101735386680763835789423207666416102355444464034512895))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i575b>(call<i575b, signature=fn(f64) -> i575b>(%[[VALUE_testdbl_575]], neg<f64>(const<f64>(8.507059173023462e37))), widen<i575b, reason=usual_arith>(neg<i128b, overflow=ub>(const<i128b>(85070591730234615865843651857942052864))))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i575b>(call<i575b, signature=fn(f64) -> i575b>(%[[VALUE_testdbl_575]], call<f64, signature=fn() -> f64>(%[[VALUE___builtin_inf]])), const<i575b>(61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i575b>(call<i575b, signature=fn(f64) -> i575b>(%[[VALUE_testdbl_575]], neg<f64>(call<f64, signature=fn() -> f64>(%[[VALUE___builtin_inf]]))), sub<i575b, overflow=ub>(neg<i575b, overflow=ub>(const<i575b>(61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783)), widen<i575b, reason=usual_arith>(const<i2b>(1))))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i575b>(call<i575b, signature=fn(f64) -> i575b>(%[[VALUE_testdbl_575]], call<f64, signature=fn(ptr<const i8>) -> f64>(%[[VALUE___builtin_nan]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_11]])))), const<i575b>(61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i575b>(call<i575b, signature=fn(f64) -> i575b>(%[[VALUE_testdbl_575]], const<f64>(6.183260036827613e172)), const<i575b>(61832600368276126650327970124302082526882038193909742709080463879918896882169507607035916867654709124839777195049479857541529867095829765369898539058829479405123401922117632))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i575b>(call<i575b, signature=fn(f64) -> i575b>(%[[VALUE_testdbl_575]], const<f64>(6.183260036827614e172)), const<i575b>(61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i575b>(call<i575b, signature=fn(f64) -> i575b>(%[[VALUE_testdbl_575]], neg<f64>(const<f64>(6.183260036827614e172))), sub<i575b, overflow=ub>(neg<i575b, overflow=ub>(const<i575b>(61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783)), widen<i575b, reason=usual_arith>(const<i32>(1))))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i575b>(call<i575b, signature=fn(f64) -> i575b>(%[[VALUE_testdbl_575]], neg<f64>(const<f64>(6.183260036827615e172))), sub<i575b, overflow=ub>(neg<i575b, overflow=ub>(const<i575b>(61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783)), widen<i575b, reason=usual_arith>(const<i32>(1))))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u575b>(call<u575b, signature=fn(f64) -> u575b>(%[[VALUE_testdblu_575]], neg<f64>(const<f64>(0.9990234375))), widen<u575b, reason=usual_arith>(const<u1b>(0)))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u575b>(call<u575b, signature=fn(f64) -> u575b>(%[[VALUE_testdblu_575]], neg<f64>(const<f64>(1.0))), widen<u575b, reason=usual_arith>(const<u1b>(0)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u575b>(call<u575b, signature=fn(f64) -> u575b>(%[[VALUE_testdblu_575]], call<f64, signature=fn() -> f64>(%[[VALUE___builtin_inf]])), const<u575b>(123665200736552267030251260509823595017565674550605919957031528046448612553265933585158200530621522494798835713008069669675682517153375604983773077550946583958303386074349567))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u575b>(call<u575b, signature=fn(f64) -> u575b>(%[[VALUE_testdblu_575]], neg<f64>(call<f64, signature=fn() -> f64>(%[[VALUE___builtin_inf]]))), widen<u575b, reason=usual_arith>(const<u1b>(0)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u575b>(call<u575b, signature=fn(f64) -> u575b>(%[[VALUE_testdblu_575]], call<f64, signature=fn(ptr<const i8>) -> f64>(%[[VALUE___builtin_nan]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_12]])))), const<u575b>(123665200736552267030251260509823595017565674550605919957031528046448612553265933585158200530621522494798835713008069669675682517153375604983773077550946583958303386074349567))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u575b>(call<u575b, signature=fn(f64) -> u575b>(%[[VALUE_testdblu_575]], const<f64>(1.2366520073655225e173)), const<u575b>(123665200736552253300655940248604165053764076387819485418160927759837793764339015214071833735309418249679554390098959715083059734191659530739797078117658958810246803844235264))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u575b>(call<u575b, signature=fn(f64) -> u575b>(%[[VALUE_testdblu_575]], const<f64>(1.2366520073655227e173)), const<u575b>(123665200736552267030251260509823595017565674550605919957031528046448612553265933585158200530621522494798835713008069669675682517153375604983773077550946583958303386074349567))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i135b>(call<i135b, signature=fn(f80) -> i135b>(%[[VALUE_testldbl_135]], neg<f80>(const<f80>(8.50705917302346158658E+37))), widen<i135b, reason=usual_arith>(neg<i128b, overflow=ub>(const<i128b>(85070591730234615865843651857942052864))))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i135b>(call<i135b, signature=fn(f80) -> i135b>(%[[VALUE_testldbl_135]], call<f80, signature=fn() -> f80>(%[[VALUE___builtin_infl]])), const<i135b>(21778071482940061661655974875633165533183))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i135b>(call<i135b, signature=fn(f80) -> i135b>(%[[VALUE_testldbl_135]], neg<f80>(call<f80, signature=fn() -> f80>(%[[VALUE___builtin_infl]]))), sub<i135b, overflow=ub>(neg<i135b, overflow=ub>(const<i135b>(21778071482940061661655974875633165533183)), widen<i135b, reason=usual_arith>(const<i2b>(1))))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i135b>(call<i135b, signature=fn(f80) -> i135b>(%[[VALUE_testldbl_135]], call<f80, signature=fn(ptr<const i8>) -> f80>(%[[VALUE___builtin_nanl]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_13]])))), const<i135b>(21778071482940061661655974875633165533183))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i135b>(call<i135b, signature=fn(f80) -> i135b>(%[[VALUE_testldbl_135]], const<f80>(2.17780714829400616605E+40)), const<i135b>(21778071482940061660475383254915754229760))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i135b>(call<i135b, signature=fn(f80) -> i135b>(%[[VALUE_testldbl_135]], const<f80>(2.17780714829400616617E+40)), const<i135b>(21778071482940061661655974875633165533183))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i135b>(call<i135b, signature=fn(f80) -> i135b>(%[[VALUE_testldbl_135]], neg<f80>(const<f80>(2.17780714829400616617E+40))), sub<i135b, overflow=ub>(neg<i135b, overflow=ub>(const<i135b>(21778071482940061661655974875633165533183)), widen<i135b, reason=usual_arith>(const<i32>(1))))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i135b>(call<i135b, signature=fn(f80) -> i135b>(%[[VALUE_testldbl_135]], neg<f80>(const<f80>(2.1778071482940061664E+40))), sub<i135b, overflow=ub>(neg<i135b, overflow=ub>(const<i135b>(21778071482940061661655974875633165533183)), widen<i135b, reason=usual_arith>(const<i32>(1))))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u135b>(call<u135b, signature=fn(f80) -> u135b>(%[[VALUE_testldblu_135]], neg<f80>(const<f80>(0.9990234375))), widen<u135b, reason=usual_arith>(const<u1b>(0)))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u135b>(call<u135b, signature=fn(f80) -> u135b>(%[[VALUE_testldblu_135]], neg<f80>(const<f80>(1))), widen<u135b, reason=usual_arith>(const<u1b>(0)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u135b>(call<u135b, signature=fn(f80) -> u135b>(%[[VALUE_testldblu_135]], call<f80, signature=fn() -> f80>(%[[VALUE___builtin_infl]])), const<u135b>(43556142965880123323311949751266331066367))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u135b>(call<u135b, signature=fn(f80) -> u135b>(%[[VALUE_testldblu_135]], neg<f80>(call<f80, signature=fn() -> f80>(%[[VALUE___builtin_infl]]))), widen<u135b, reason=usual_arith>(const<u1b>(0)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u135b>(call<u135b, signature=fn(f80) -> u135b>(%[[VALUE_testldblu_135]], call<f80, signature=fn(ptr<const i8>) -> f80>(%[[VALUE___builtin_nanl]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_14]])))), const<u135b>(43556142965880123323311949751266331066367))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u135b>(call<u135b, signature=fn(f80) -> u135b>(%[[VALUE_testldblu_135]], const<f80>(4.3556142965880123321E+40)), const<u135b>(43556142965880123320950766509831508459520))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u135b>(call<u135b, signature=fn(f80) -> u135b>(%[[VALUE_testldblu_135]], const<f80>(4.35561429658801233233E+40)), const<u135b>(43556142965880123323311949751266331066367))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i192b>(call<i192b, signature=fn(f80) -> i192b>(%[[VALUE_testldbl_192]], neg<f80>(const<f80>(8.50705917302346158658E+37))), widen<i192b, reason=usual_arith>(neg<i128b, overflow=ub>(const<i128b>(85070591730234615865843651857942052864))))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i192b>(call<i192b, signature=fn(f80) -> i192b>(%[[VALUE_testldbl_192]], call<f80, signature=fn() -> f80>(%[[VALUE___builtin_infl]])), const<i192b>(3138550867693340381917894711603833208051177722232017256447))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i192b>(call<i192b, signature=fn(f80) -> i192b>(%[[VALUE_testldbl_192]], neg<f80>(call<f80, signature=fn() -> f80>(%[[VALUE___builtin_infl]]))), sub<i192b, overflow=ub>(neg<i192b, overflow=ub>(const<i192b>(3138550867693340381917894711603833208051177722232017256447)), widen<i192b, reason=usual_arith>(const<i2b>(1))))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i192b>(call<i192b, signature=fn(f80) -> i192b>(%[[VALUE_testldbl_192]], call<f80, signature=fn(ptr<const i8>) -> f80>(%[[VALUE___builtin_nanl]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_15]])))), const<i192b>(3138550867693340381917894711603833208051177722232017256447))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i192b>(call<i192b, signature=fn(f80) -> i192b>(%[[VALUE_testldbl_192]], const<f80>(3.13855086769334038175E+57)), const<i192b>(3138550867693340381747753528143363976319490418516133150720))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i192b>(call<i192b, signature=fn(f80) -> i192b>(%[[VALUE_testldbl_192]], const<f80>(3.13855086769334038192E+57)), const<i192b>(3138550867693340381917894711603833208051177722232017256447))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i192b>(call<i192b, signature=fn(f80) -> i192b>(%[[VALUE_testldbl_192]], neg<f80>(const<f80>(3.13855086769334038192E+57))), sub<i192b, overflow=ub>(neg<i192b, overflow=ub>(const<i192b>(3138550867693340381917894711603833208051177722232017256447)), widen<i192b, reason=usual_arith>(const<i32>(1))))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i192b>(call<i192b, signature=fn(f80) -> i192b>(%[[VALUE_testldbl_192]], neg<f80>(const<f80>(3.13855086769334038226E+57))), sub<i192b, overflow=ub>(neg<i192b, overflow=ub>(const<i192b>(3138550867693340381917894711603833208051177722232017256447)), widen<i192b, reason=usual_arith>(const<i32>(1))))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u192b>(call<u192b, signature=fn(f80) -> u192b>(%[[VALUE_testldblu_192]], neg<f80>(const<f80>(0.9990234375))), widen<u192b, reason=usual_arith>(const<u1b>(0)))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u192b>(call<u192b, signature=fn(f80) -> u192b>(%[[VALUE_testldblu_192]], neg<f80>(const<f80>(1))), widen<u192b, reason=usual_arith>(const<u1b>(0)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u192b>(call<u192b, signature=fn(f80) -> u192b>(%[[VALUE_testldblu_192]], call<f80, signature=fn() -> f80>(%[[VALUE___builtin_infl]])), const<u192b>(6277101735386680763835789423207666416102355444464034512895))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u192b>(call<u192b, signature=fn(f80) -> u192b>(%[[VALUE_testldblu_192]], neg<f80>(call<f80, signature=fn() -> f80>(%[[VALUE___builtin_infl]]))), widen<u192b, reason=usual_arith>(const<u1b>(0)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u192b>(call<u192b, signature=fn(f80) -> u192b>(%[[VALUE_testldblu_192]], call<f80, signature=fn(ptr<const i8>) -> f80>(%[[VALUE___builtin_nanl]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_16]])))), const<u192b>(6277101735386680763835789423207666416102355444464034512895))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u192b>(call<u192b, signature=fn(f80) -> u192b>(%[[VALUE_testldblu_192]], const<f80>(6.2771017353866807635E+57)), const<u192b>(6277101735386680763495507056286727952638980837032266301440))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u192b>(call<u192b, signature=fn(f80) -> u192b>(%[[VALUE_testldblu_192]], const<f80>(6.27710173538668076383E+57)), const<u192b>(6277101735386680763835789423207666416102355444464034512895))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i575b>(call<i575b, signature=fn(f80) -> i575b>(%[[VALUE_testldbl_575]], neg<f80>(const<f80>(8.50705917302346158658E+37))), widen<i575b, reason=usual_arith>(neg<i128b, overflow=ub>(const<i128b>(85070591730234615865843651857942052864))))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i575b>(call<i575b, signature=fn(f80) -> i575b>(%[[VALUE_testldbl_575]], call<f80, signature=fn() -> f80>(%[[VALUE___builtin_infl]])), const<i575b>(61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i575b>(call<i575b, signature=fn(f80) -> i575b>(%[[VALUE_testldbl_575]], neg<f80>(call<f80, signature=fn() -> f80>(%[[VALUE___builtin_infl]]))), sub<i575b, overflow=ub>(neg<i575b, overflow=ub>(const<i575b>(61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783)), widen<i575b, reason=usual_arith>(const<i2b>(1))))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i575b>(call<i575b, signature=fn(f80) -> i575b>(%[[VALUE_testldbl_575]], call<f80, signature=fn(ptr<const i8>) -> f80>(%[[VALUE___builtin_nanl]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_17]])))), const<i575b>(61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i575b>(call<i575b, signature=fn(f80) -> i575b>(%[[VALUE_testldbl_575]], const<f80>(6.18326003682761335118E+172)), const<i575b>(61832600368276133511773678272426148233889331025751498446645922568076207932202076431648659257792374503198949281962308977915333294030066289778448068072486649492543280785653760))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i575b>(call<i575b, signature=fn(f80) -> i575b>(%[[VALUE_testldbl_575]], const<f80>(6.18326003682761335151E+172)), const<i575b>(61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i575b>(call<i575b, signature=fn(f80) -> i575b>(%[[VALUE_testldbl_575]], neg<f80>(const<f80>(6.18326003682761335151E+172))), sub<i575b, overflow=ub>(neg<i575b, overflow=ub>(const<i575b>(61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783)), widen<i575b, reason=usual_arith>(const<i32>(1))))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<i575b>(call<i575b, signature=fn(f80) -> i575b>(%[[VALUE_testldbl_575]], neg<f80>(const<f80>(6.18326003682761335218E+172))), sub<i575b, overflow=ub>(neg<i575b, overflow=ub>(const<i575b>(61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783)), widen<i575b, reason=usual_arith>(const<i32>(1))))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u575b>(call<u575b, signature=fn(f80) -> u575b>(%[[VALUE_testldblu_575]], neg<f80>(const<f80>(0.9990234375))), widen<u575b, reason=usual_arith>(const<u1b>(0)))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u575b>(call<u575b, signature=fn(f80) -> u575b>(%[[VALUE_testldblu_575]], neg<f80>(const<f80>(1))), widen<u575b, reason=usual_arith>(const<u1b>(0)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u575b>(call<u575b, signature=fn(f80) -> u575b>(%[[VALUE_testldblu_575]], call<f80, signature=fn() -> f80>(%[[VALUE___builtin_infl]])), const<u575b>(123665200736552267030251260509823595017565674550605919957031528046448612553265933585158200530621522494798835713008069669675682517153375604983773077550946583958303386074349567))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u575b>(call<u575b, signature=fn(f80) -> u575b>(%[[VALUE_testldblu_575]], neg<f80>(call<f80, signature=fn() -> f80>(%[[VALUE___builtin_infl]]))), widen<u575b, reason=usual_arith>(const<u1b>(0)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u575b>(call<u575b, signature=fn(f80) -> u575b>(%[[VALUE_testldblu_575]], call<f80, signature=fn(ptr<const i8>) -> f80>(%[[VALUE___builtin_nanl]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_18]])))), const<u575b>(123665200736552267030251260509823595017565674550605919957031528046448612553265933585158200530621522494798835713008069669675682517153375604983773077550946583958303386074349567))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u575b>(call<u575b, signature=fn(f80) -> u575b>(%[[VALUE_testldblu_575]], const<f80>(1.23665200736552267024E+173)), const<u575b>(123665200736552267023547356544852296467778662051502996893291845136152415864404152863297318515584749006397898563924617955830666588060132579556896136144973298985086561571307520))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_check_invalid]], from_bool<i32, reason=arg>(eq<u575b>(call<u575b, signature=fn(f80) -> u575b>(%[[VALUE_testldblu_575]], const<f80>(1.2366520073655226703E+173)), const<u575b>(123665200736552267030251260509823595017565674550605919957031528046448612553265933585158200530621522494798835713008069669675682517153375604983773077550946583958303386074349567))), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
