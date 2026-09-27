/* PR c/102989 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-std=c23 -pedantic-errors" } */

_Atomic _BitInt(15) a;
_Atomic(_BitInt(15)) b;
_Atomic _BitInt(115) c;
_Atomic _BitInt(192) d;
_Atomic _BitInt(575) e;
unsigned _BitInt(575) f;

__attribute__((noipa)) _BitInt(575)
foo (_BitInt(575) x)
{
  return x;
}

__attribute__((noipa)) int
bar (int x)
{
  return x;
}

__attribute__((noipa)) _Atomic _BitInt(575) *
baz (_Atomic _BitInt(575) *x)
{
  return x;
}

int
main ()
{
  a += 1wb;
  b -= 2wb;
  c *= 3wb;
  d /= 4wb;
  e -= 5wb;
  f = __atomic_fetch_add (&e, 54342985743985743985743895743834298574985734895743895734895wb, __ATOMIC_SEQ_CST);
  f += __atomic_sub_fetch (&e, 13110356772307144130089534440127211568864891923061809853784155727841516341877716905506658630804426134644404380556711020290072702485839594283061059349912463486203837251238365wb, __ATOMIC_SEQ_CST);
  f += __atomic_fetch_and (&e, -33740418462630594385361724744395454079240140931656245750192534103967695265126850678980088699287669565365078793986191778469857714756111026776864987769580622009237241167211461wb, __ATOMIC_RELAXED);
  f += __atomic_xor_fetch (&e, 30799001892772360282132495459823194445423296347702377756575214695893559890977912003055702776548378201752339680602420936304294728688029412276600086349055079523071860836114234wb, __ATOMIC_SEQ_CST);
  f += __atomic_fetch_or (baz (&e), foo (-6581969867283727911005990155704642154324773504588160884865628865547696324844988049982401783508268917375066790729408659617189350524019843499435572226770089390885472550659255wb), bar (__ATOMIC_RELAXED));
  f += __atomic_nand_fetch (&e, 55047840194947228224723671648125013926111290688378416557548660662319034233151051252215595447712248992759177463741832904590457754423713378627482465906620631734790561114905369wb, __ATOMIC_ACQ_REL);
}

// SLATE-FILECHECK-FLAVOR gcc
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
// DEFAULT-NEXT:     global %0 a: atomic i15b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 b: atomic i15b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 c: atomic i115b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 d: atomic i192b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 e: atomic i575b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 f: u575b [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %6 @foo(%7 x: i575b) -> i575b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i575b>(%7);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @bar(%9 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%9);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @baz(%11 x: ptr<atomic i575b>) -> ptr<atomic i575b> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<ptr<atomic i575b>>(%11);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %13: i15b [synthetic] = update<i15b, result=new, atomic=seq_cst>(%0, add<i15b, overflow=ub>(old<i15b>, widen<i15b, reason=usual_arith>(const<i2b>(1))));
// DEFAULT-NEXT:         let %14: i15b [synthetic] = update<i15b, result=new, atomic=seq_cst>(%1, sub<i15b, overflow=ub>(old<i15b>, widen<i15b, reason=usual_arith>(const<i3b>(2))));
// DEFAULT-NEXT:         let %15: i115b [synthetic] = update<i115b, result=new, atomic=seq_cst>(%2, mul<i115b, overflow=ub>(old<i115b>, widen<i115b, reason=usual_arith>(const<i3b>(3))));
// DEFAULT-NEXT:         let %16: i192b [synthetic] = update<i192b, result=new, atomic=seq_cst>(%3, div<i192b, by_zero=ub, min_by_neg_one=ub>(old<i192b>, widen<i192b, reason=usual_arith>(const<i4b>(4))));
// DEFAULT-NEXT:         let %17: i575b [synthetic] = update<i575b, result=new, atomic=seq_cst>(%4, sub<i575b, overflow=ub>(old<i575b>, widen<i575b, reason=usual_arith>(const<i4b>(5))));
// DEFAULT-NEXT:         let %18: i575b [synthetic] = update<i575b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i575b>>(%4)), add<i575b, overflow=wrap>(old<i575b>, widen<i575b, reason=arg>(const<i197b>(54342985743985743985743895743834298574985734895743895734895))));
// DEFAULT-NEXT:         write<u575b>(%5, reinterpret<u575b, reason=assign, fits=unknown>(read<i575b>(%18)));
// DEFAULT-NEXT:         let %19: u575b [synthetic] = read<u575b>(%5);
// DEFAULT-NEXT:         let %20: i575b [synthetic] = update<i575b, result=new, atomic=seq_cst>(deref(addr_of<ptr<atomic i575b>>(%4)), sub<i575b, overflow=wrap>(old<i575b>, widen<i575b, reason=arg>(const<i573b>(13110356772307144130089534440127211568864891923061809853784155727841516341877716905506658630804426134644404380556711020290072702485839594283061059349912463486203837251238365))));
// DEFAULT-NEXT:         let %21: u575b [synthetic] = add<u575b, overflow=wrap>(read<u575b>(%19), reinterpret<u575b, reason=usual_arith, fits=unknown>(read<i575b>(%20)));
// DEFAULT-NEXT:         write<u575b>(%5, read<u575b>(%21));
// DEFAULT-NEXT:         let %22: u575b [synthetic] = read<u575b>(%5);
// DEFAULT-NEXT:         let %23: i575b [synthetic] = update<i575b, result=old, atomic=relaxed>(deref(addr_of<ptr<atomic i575b>>(%4)), and<i575b>(old<i575b>, neg<i575b, overflow=ub>(const<i575b>(33740418462630594385361724744395454079240140931656245750192534103967695265126850678980088699287669565365078793986191778469857714756111026776864987769580622009237241167211461))));
// DEFAULT-NEXT:         let %24: u575b [synthetic] = add<u575b, overflow=wrap>(read<u575b>(%22), reinterpret<u575b, reason=usual_arith, fits=unknown>(read<i575b>(%23)));
// DEFAULT-NEXT:         write<u575b>(%5, read<u575b>(%24));
// DEFAULT-NEXT:         let %25: u575b [synthetic] = read<u575b>(%5);
// DEFAULT-NEXT:         let %26: i575b [synthetic] = update<i575b, result=new, atomic=seq_cst>(deref(addr_of<ptr<atomic i575b>>(%4)), xor<i575b>(old<i575b>, widen<i575b, reason=arg>(const<i574b>(30799001892772360282132495459823194445423296347702377756575214695893559890977912003055702776548378201752339680602420936304294728688029412276600086349055079523071860836114234))));
// DEFAULT-NEXT:         let %27: u575b [synthetic] = add<u575b, overflow=wrap>(read<u575b>(%25), reinterpret<u575b, reason=usual_arith, fits=unknown>(read<i575b>(%26)));
// DEFAULT-NEXT:         write<u575b>(%5, read<u575b>(%27));
// DEFAULT-NEXT:         let %28: u575b [synthetic] = read<u575b>(%5);
// DEFAULT-NEXT:         let %29: i575b [synthetic] = update<i575b, result=old, atomic=dynamic(call<i32, signature=fn(i32) -> i32>(%8, const<i32>(0)))>(deref(call<ptr<atomic i575b>, signature=fn(ptr<atomic i575b>) -> ptr<atomic i575b>>(%10, addr_of<ptr<atomic i575b>>(%4))), or<i575b>(old<i575b>, call<i575b, signature=fn(i575b) -> i575b>(%6, widen<i575b, reason=arg>(neg<i572b, overflow=ub>(const<i572b>(6581969867283727911005990155704642154324773504588160884865628865547696324844988049982401783508268917375066790729408659617189350524019843499435572226770089390885472550659255))))));
// DEFAULT-NEXT:         let %30: u575b [synthetic] = add<u575b, overflow=wrap>(read<u575b>(%28), reinterpret<u575b, reason=usual_arith, fits=unknown>(read<i575b>(%29)));
// DEFAULT-NEXT:         write<u575b>(%5, read<u575b>(%30));
// DEFAULT-NEXT:         let %31: u575b [synthetic] = read<u575b>(%5);
// DEFAULT-NEXT:         let %32: i575b [synthetic] = update<i575b, result=new, atomic=acq_rel>(deref(addr_of<ptr<atomic i575b>>(%4)), not<i575b>(and<i575b>(old<i575b>, const<i575b>(55047840194947228224723671648125013926111290688378416557548660662319034233151051252215595447712248992759177463741832904590457754423713378627482465906620631734790561114905369))));
// DEFAULT-NEXT:         let %33: u575b [synthetic] = add<u575b, overflow=wrap>(read<u575b>(%31), reinterpret<u575b, reason=usual_arith, fits=unknown>(read<i575b>(%32)));
// DEFAULT-NEXT:         write<u575b>(%5, read<u575b>(%33));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
