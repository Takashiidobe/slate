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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: atomic i15b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: atomic i15b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: atomic i115b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: atomic i192b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: atomic i575b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: u575b [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: i575b) -> i575b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i575b>(%[[VALUE_x]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_x_2:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_x_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_baz:[0-9]+]] @baz(%[[VALUE_x_3:[0-9]+]] x: ptr<atomic i575b>) -> ptr<atomic i575b> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<ptr<atomic i575b>>(%[[VALUE_x_3]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i15b [synthetic] = update<i15b, result=new, atomic=seq_cst>(%[[VALUE_a]], add<i15b, overflow=ub>(old<i15b>, widen<i15b, reason=usual_arith>(const<i2b>(1))));
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i15b [synthetic] = update<i15b, result=new, atomic=seq_cst>(%[[VALUE_b]], sub<i15b, overflow=ub>(old<i15b>, widen<i15b, reason=usual_arith>(const<i3b>(2))));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i115b [synthetic] = update<i115b, result=new, atomic=seq_cst>(%[[VALUE_c]], mul<i115b, overflow=ub>(old<i115b>, widen<i115b, reason=usual_arith>(const<i3b>(3))));
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: i192b [synthetic] = update<i192b, result=new, atomic=seq_cst>(%[[VALUE_d]], div<i192b, by_zero=ub, min_by_neg_one=ub>(old<i192b>, widen<i192b, reason=usual_arith>(const<i4b>(4))));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: i575b [synthetic] = update<i575b, result=new, atomic=seq_cst>(%[[VALUE_e]], sub<i575b, overflow=ub>(old<i575b>, widen<i575b, reason=usual_arith>(const<i4b>(5))));
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: i575b [synthetic] = update<i575b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i575b>>(%[[VALUE_e]])), add<i575b, overflow=wrap>(old<i575b>, widen<i575b, reason=arg>(const<i197b>(54342985743985743985743895743834298574985734895743895734895))));
// DEFAULT-NEXT:         write<u575b>(%[[VALUE_f]], reinterpret<u575b, reason=assign, fits=unknown>(read<i575b>(%[[VALUE5]])));
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: u575b [synthetic] = read<u575b>(%[[VALUE_f]]);
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: i575b [synthetic] = update<i575b, result=new, atomic=seq_cst>(deref(addr_of<ptr<atomic i575b>>(%[[VALUE_e]])), sub<i575b, overflow=wrap>(old<i575b>, widen<i575b, reason=arg>(const<i573b>(13110356772307144130089534440127211568864891923061809853784155727841516341877716905506658630804426134644404380556711020290072702485839594283061059349912463486203837251238365))));
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: u575b [synthetic] = add<u575b, overflow=wrap>(read<u575b>(%[[VALUE6]]), reinterpret<u575b, reason=usual_arith, fits=unknown>(read<i575b>(%[[VALUE7]])));
// DEFAULT-NEXT:         write<u575b>(%[[VALUE_f]], read<u575b>(%[[VALUE8]]));
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: u575b [synthetic] = read<u575b>(%[[VALUE_f]]);
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: i575b [synthetic] = update<i575b, result=old, atomic=relaxed>(deref(addr_of<ptr<atomic i575b>>(%[[VALUE_e]])), and<i575b>(old<i575b>, neg<i575b, overflow=ub>(const<i575b>(33740418462630594385361724744395454079240140931656245750192534103967695265126850678980088699287669565365078793986191778469857714756111026776864987769580622009237241167211461))));
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: u575b [synthetic] = add<u575b, overflow=wrap>(read<u575b>(%[[VALUE9]]), reinterpret<u575b, reason=usual_arith, fits=unknown>(read<i575b>(%[[VALUE10]])));
// DEFAULT-NEXT:         write<u575b>(%[[VALUE_f]], read<u575b>(%[[VALUE11]]));
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: u575b [synthetic] = read<u575b>(%[[VALUE_f]]);
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: i575b [synthetic] = update<i575b, result=new, atomic=seq_cst>(deref(addr_of<ptr<atomic i575b>>(%[[VALUE_e]])), xor<i575b>(old<i575b>, widen<i575b, reason=arg>(const<i574b>(30799001892772360282132495459823194445423296347702377756575214695893559890977912003055702776548378201752339680602420936304294728688029412276600086349055079523071860836114234))));
// DEFAULT-NEXT:         let %[[VALUE14:[0-9]+]]: u575b [synthetic] = add<u575b, overflow=wrap>(read<u575b>(%[[VALUE12]]), reinterpret<u575b, reason=usual_arith, fits=unknown>(read<i575b>(%[[VALUE13]])));
// DEFAULT-NEXT:         write<u575b>(%[[VALUE_f]], read<u575b>(%[[VALUE14]]));
// DEFAULT-NEXT:         let %[[VALUE15:[0-9]+]]: u575b [synthetic] = read<u575b>(%[[VALUE_f]]);
// DEFAULT-NEXT:         let %[[VALUE16:[0-9]+]]: i575b [synthetic] = update<i575b, result=old, atomic=dynamic(call<i32, signature=fn(i32) -> i32>(%[[VALUE_bar]], const<i32>(0)))>(deref(call<ptr<atomic i575b>, signature=fn(ptr<atomic i575b>) -> ptr<atomic i575b>>(%[[VALUE_baz]], addr_of<ptr<atomic i575b>>(%[[VALUE_e]]))), or<i575b>(old<i575b>, call<i575b, signature=fn(i575b) -> i575b>(%[[VALUE_foo]], widen<i575b, reason=arg>(neg<i572b, overflow=ub>(const<i572b>(6581969867283727911005990155704642154324773504588160884865628865547696324844988049982401783508268917375066790729408659617189350524019843499435572226770089390885472550659255))))));
// DEFAULT-NEXT:         let %[[VALUE17:[0-9]+]]: u575b [synthetic] = add<u575b, overflow=wrap>(read<u575b>(%[[VALUE15]]), reinterpret<u575b, reason=usual_arith, fits=unknown>(read<i575b>(%[[VALUE16]])));
// DEFAULT-NEXT:         write<u575b>(%[[VALUE_f]], read<u575b>(%[[VALUE17]]));
// DEFAULT-NEXT:         let %[[VALUE18:[0-9]+]]: u575b [synthetic] = read<u575b>(%[[VALUE_f]]);
// DEFAULT-NEXT:         let %[[VALUE19:[0-9]+]]: i575b [synthetic] = update<i575b, result=new, atomic=acq_rel>(deref(addr_of<ptr<atomic i575b>>(%[[VALUE_e]])), not<i575b>(and<i575b>(old<i575b>, const<i575b>(55047840194947228224723671648125013926111290688378416557548660662319034233151051252215595447712248992759177463741832904590457754423713378627482465906620631734790561114905369))));
// DEFAULT-NEXT:         let %[[VALUE20:[0-9]+]]: u575b [synthetic] = add<u575b, overflow=wrap>(read<u575b>(%[[VALUE18]]), reinterpret<u575b, reason=usual_arith, fits=unknown>(read<i575b>(%[[VALUE19]])));
// DEFAULT-NEXT:         write<u575b>(%[[VALUE_f]], read<u575b>(%[[VALUE20]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
