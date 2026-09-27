/* PR c/102989 */
/* { dg-do run { target bitint } } */
/* { dg-options "-std=c23 -pedantic-errors" } */

#include <stdatomic.h>

extern void abort (void);

#if __BITINT_MAXWIDTH__ >= 127
_Atomic _BitInt(127) v;
_BitInt(127) count, res;
const _BitInt(127) init = ~(_BitInt(127)) 0wb;

void
test_fetch_add ()
{
  atomic_init (&v, 13505789527944801758751150119415226784wb);
  count = -64910836855286429164283779649638556795wb;

  if (atomic_fetch_add_explicit (&v, count, memory_order_relaxed)
      != 13505789527944801758751150119415226784wb)
    abort ();

  if (atomic_fetch_add_explicit (&v, 2227507280963412295355244564739509222wb,
				 memory_order_consume)
      != -51405047327341627405532629530223330011wb)
    abort ();

  if (atomic_fetch_add_explicit (&v, count, memory_order_acquire)
      != -49177540046378215110177384965483820789wb)
    abort ();

  if (atomic_fetch_add_explicit (&v, 42245667388877614520169143618236120405wb,
				 memory_order_release)
      != 56052806558804587457226139100761728144wb)
    abort ();

  if (atomic_fetch_add_explicit (&v, count, memory_order_acq_rel)
      != -71842709512787029754292020996886257179wb)
    abort ();

  if (atomic_fetch_add_explicit (&v, 77995075987640754057679086146674392947wb,
				 memory_order_seq_cst)
      != 33387637092395772813111503069359291754wb)
    abort ();

  if (atomic_fetch_add (&v, 11810767284628435493328779846084830297wb)
      != -58758470380432704860896714499850421027wb)
    abort ();

  if (atomic_load (&v) != -46947703095804269367567934653765590730wb)
    abort ();
}

void
test_fetch_sub ()
{
  atomic_store_explicit (&v, 30796781768365552851024605388374299173wb,
			 memory_order_release);
  count = 32457177597484647488149720668185011722wb;

  if (atomic_fetch_sub_explicit (&v, count, memory_order_relaxed)
      != 30796781768365552851024605388374299173wb)
    abort ();

  if (atomic_fetch_sub_explicit (&v, 54614103079293459991417218347656369566wb,
				 memory_order_consume)
      != -1660395829119094637125115279810712549wb)
    abort ();

  if (atomic_fetch_sub_explicit (&v, count, memory_order_acquire)
      != -56274498908412554628542333627467082115wb)
    abort ();

  if (atomic_fetch_sub_explicit (&v, -44514083923735151931107302009741400482wb,
				 memory_order_release)
      != 81409506954572029614995249420232011891wb)
    abort ();

  if (atomic_fetch_sub_explicit (&v, count, memory_order_acq_rel)
      != -44217592582162050185584752285910693355wb)
    abort ();

  if (atomic_fetch_sub_explicit (&v, 30348078982452392099140613411731040827wb,
				 memory_order_seq_cst)
      != -76674770179646697673734472954095705077wb)
    abort ();

  if (atomic_fetch_sub (&v, -82224045897086857020012824788652775087wb)
      != 63118334298370141958812217350057359824wb)
    abort ();

  if (atomic_load_explicit (&v, memory_order_acquire)
      != -24798803265012232752862261577173970817wb)
    abort ();
}

void
test_fetch_and ()
{
  atomic_store (&v, init);

  if (atomic_fetch_and_explicit (&v, 0, memory_order_relaxed) != init)
    abort ();

  if (atomic_fetch_and_explicit (&v, init, memory_order_consume) != 0)
    abort ();

  if (atomic_fetch_and_explicit (&v, 0, memory_order_acquire) != 0)
    abort ();

  v = ~v;
  if (atomic_fetch_and_explicit (&v, init, memory_order_release) != init)
    abort ();

  if (atomic_fetch_and_explicit (&v, 0, memory_order_acq_rel) != init)
    abort ();

  if (atomic_fetch_and_explicit (&v, 0, memory_order_seq_cst) != 0)
    abort ();

  if (atomic_fetch_and (&v, 0) != 0)
    abort ();
}

void
test_fetch_xor ()
{
  v = init;
  count = 0;

  if (atomic_fetch_xor_explicit (&v, count, memory_order_relaxed) != init)
    abort ();

  if (atomic_fetch_xor_explicit (&v, ~count, memory_order_consume) != init)
    abort ();

  if (atomic_fetch_xor_explicit (&v, 0, memory_order_acquire) != 0)
    abort ();

  if (atomic_fetch_xor_explicit (&v, ~count, memory_order_release) != 0)
    abort ();

  if (atomic_fetch_xor_explicit (&v, 0, memory_order_acq_rel) != init)
    abort ();

  if (atomic_fetch_xor_explicit (&v, ~count, memory_order_seq_cst) != init)
    abort ();

  if (atomic_fetch_xor (&v, ~count) != 0)
    abort ();
}

void
test_fetch_or ()
{
  v = 0;
  count = 17592186044416wb;

  if (atomic_fetch_or_explicit (&v, count, memory_order_relaxed) != 0)
    abort ();

  count *= 2;
  if (atomic_fetch_or_explicit (&v, 35184372088832wb, memory_order_consume)
      != 17592186044416wb)
    abort ();

  count *= 2;
  if (atomic_fetch_or_explicit (&v, count, memory_order_acquire)
      != 52776558133248wb)
    abort ();

  count *= 2;
  if (atomic_fetch_or_explicit (&v, 140737488355328wb, memory_order_release)
      != 123145302310912wb)
    abort ();

  count *= 2;
  if (atomic_fetch_or_explicit (&v, count, memory_order_acq_rel)
      != 263882790666240wb)
    abort ();

  count *= 2;
  if (atomic_fetch_or_explicit (&v, count, memory_order_seq_cst)
      != 545357767376896wb)
    abort ();

  count *= 2;
  if (atomic_fetch_or (&v, count) != 1108307720798208wb)
    abort ();
}


/* Test the OP routines with a result which isn't used.  */

void
test_add ()
{
  v = 0;
  count = 4722366482869645213696wb;

  atomic_fetch_add (&v, count);
  if (v != 4722366482869645213696wb)
    abort ();

  atomic_fetch_add_explicit (&v, count, memory_order_consume);
  if (v != 9444732965739290427392wb)
    abort ();

  atomic_fetch_add (&v, 4722366482869645213696wb);
  if (v != 14167099448608935641088wb)
    abort ();

  atomic_fetch_add_explicit (&v, 4722366482869645213696wb,
			     memory_order_release);
  if (v != 18889465931478580854784wb)
    abort ();

  atomic_fetch_add (&v, 4722366482869645213696wb);
  if (v != 23611832414348226068480wb)
    abort ();

  atomic_fetch_add_explicit (&v, count, memory_order_seq_cst);
  if (v != 28334198897217871282176wb)
    abort ();
}

void
test_sub ()
{
  v = res = -3638804536836293398783417724445294828wb;
  count = 0;

  atomic_fetch_sub (&v, count + 1);
  if (v != --res)
    abort ();

  atomic_fetch_sub_explicit (&v, count + 1, memory_order_consume);
  if (v != --res)
    abort ();

  atomic_fetch_sub (&v, 1);
  if (v != --res)
    abort ();

  atomic_fetch_sub_explicit (&v, 1, memory_order_release);
  if (v != --res)
    abort ();

  atomic_fetch_sub (&v, count + 1);
  if (v != --res)
    abort ();

  atomic_fetch_sub_explicit (&v, count + 1, memory_order_seq_cst);
  if (v != --res)
    abort ();
}

void
test_and ()
{
  v = init;

  atomic_fetch_and (&v, 0);
  if (v != 0)
    abort ();

  v = init;
  atomic_fetch_and_explicit (&v, init, memory_order_consume);
  if (v != init)
    abort ();

  atomic_fetch_and (&v, 0);
  if (v != 0)
    abort ();

  v = ~v;
  atomic_fetch_and_explicit (&v, init, memory_order_release);
  if (v != init)
    abort ();

  atomic_fetch_and (&v, 0);
  if (v != 0)
    abort ();

  v = ~v;
  atomic_fetch_and_explicit (&v, 0, memory_order_seq_cst);
  if (v != 0)
    abort ();
}

void
test_xor ()
{
  v = init;
  count = 0;

  atomic_fetch_xor (&v, count);
  if (v != init)
    abort ();

  atomic_fetch_xor_explicit (&v, ~count, memory_order_consume);
  if (v != 0)
    abort ();

  atomic_fetch_xor (&v, 0);
  if (v != 0)
    abort ();

  atomic_fetch_xor_explicit (&v, ~count, memory_order_release);
  if (v != init)
    abort ();

  atomic_fetch_xor_explicit (&v, 0, memory_order_acq_rel);
  if (v != init)
    abort ();

  atomic_fetch_xor (&v, ~count);
  if (v != 0)
    abort ();
}

void
test_or ()
{
  v = 0;
  count = 19342813113834066795298816wb;

  atomic_fetch_or (&v, count);
  if (v != 19342813113834066795298816wb)
    abort ();

  count *= 2;
  atomic_fetch_or_explicit (&v, count, memory_order_consume);
  if (v != 58028439341502200385896448wb)
    abort ();

  count *= 2;
  atomic_fetch_or (&v, 77371252455336267181195264wb);
  if (v != 135399691796838467567091712wb)
    abort ();

  count *= 2;
  atomic_fetch_or_explicit (&v, 154742504910672534362390528wb,
			    memory_order_release);
  if (v != 290142196707511001929482240wb)
    abort ();

  count *= 2;
  atomic_fetch_or (&v, count);
  if (v != 599627206528856070654263296wb)
    abort ();

  count *= 2;
  atomic_fetch_or_explicit (&v, count, memory_order_seq_cst);
  if (v != 1218597226171546208103825408wb)
    abort ();
}

void
test_exchange (void)
{
  atomic_store (&v, 15794812138349191682564933935017390008wb);
  if (atomic_exchange (&v, -2166613183393424891717146518563613668wb)
      != 15794812138349191682564933935017390008wb
      || atomic_load (&v) != -2166613183393424891717146518563613668wb)
    abort ();
  if (atomic_exchange_explicit (&v, 61251098386268815852902382804483910638wb,
			        memory_order_relaxed)
      != -2166613183393424891717146518563613668wb
      || (atomic_load_explicit (&v, memory_order_acquire)
	  != 61251098386268815852902382804483910638wb))
    abort ();

  count = -2166613183393424891717146518563613668wb;
  if (atomic_compare_exchange_strong (&v, &count,
				      -36677332297536901313774263310237646448wb))
    abort ();
  if (count != 61251098386268815852902382804483910638wb
      || atomic_load (&v) != 61251098386268815852902382804483910638wb)
    abort ();
  if (!atomic_compare_exchange_strong (&v, &count,
				       -36677332297536901313774263310237646448wb))
    abort ();
  if (count != 61251098386268815852902382804483910638wb
      || atomic_load (&v) != -36677332297536901313774263310237646448wb)
    abort ();

  count = -2166613183393424891717146518563613668wb;
  if (atomic_compare_exchange_strong_explicit (&v, &count,
					       73949932022761409003352953944661689416wb,
					       memory_order_seq_cst,
					       memory_order_relaxed))
    abort ();
  if (count != -36677332297536901313774263310237646448wb
      || atomic_load (&v) != -36677332297536901313774263310237646448wb)
    abort ();
  if (!atomic_compare_exchange_strong_explicit (&v, &count,
						73949932022761409003352953944661689416wb,
						memory_order_seq_cst,
						memory_order_seq_cst))
    abort ();
  if (count != -36677332297536901313774263310237646448wb
      || atomic_load (&v) != 73949932022761409003352953944661689416wb)
    abort ();

  count = atomic_load (&v);
  do
    res = count + -82256758205518164043596305502815392646wb;
  while (!atomic_compare_exchange_weak (&v, &count, res));
  if (atomic_load (&v) != -8306826182756755040243351558153703230wb)
    abort ();

  count = atomic_load_explicit (&v, memory_order_acquire);
  do
    res = count + 48855144829609538366772317026461909818wb;
  while (!atomic_compare_exchange_weak_explicit (&v, &count, res,
						 memory_order_relaxed,
						 memory_order_relaxed));
  if (atomic_load (&v) != 40548318646852783326528965468308206588wb)
    abort ();
}
#endif

int
main ()
{
#if __BITINT_MAXWIDTH__ >= 127
  test_fetch_add ();
  test_fetch_sub ();
  test_fetch_and ();
  test_fetch_xor ();
  test_fetch_or ();
  test_add ();
  test_sub ();
  test_and ();
  test_xor ();
  test_or ();
  test_exchange ();
#endif
  return 0;
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
// DEFAULT-NEXT:     type @type0 = enum : u32 {
// DEFAULT-NEXT:         %0 memory_order_relaxed = const<i32>(0);
// DEFAULT-NEXT:         %1 memory_order_consume = const<i32>(1);
// DEFAULT-NEXT:         %2 memory_order_acquire = const<i32>(2);
// DEFAULT-NEXT:         %3 memory_order_release = const<i32>(3);
// DEFAULT-NEXT:         %4 memory_order_acq_rel = const<i32>(4);
// DEFAULT-NEXT:         %5 memory_order_seq_cst = const<i32>(5);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type1 memory_order = @type0;
// DEFAULT-NEXT:     global %9 v: atomic i127b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %10 count: i127b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %11 res: i127b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %12 init: i127b [storage=static] [const] = not<i127b>(widen<i127b, reason=explicit>(const<i2b>(0))) [linkage=external];
// DEFAULT-NEXT:     fn %8 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %13 @test_fetch_add() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %14 __atomic_store_ptr: ptr<atomic i127b> [storage=automatic] = addr_of<ptr<atomic i127b>>(%9);
// DEFAULT-NEXT:             let %15 __atomic_store_tmp: i127b [storage=automatic] = widen<i127b, reason=assign>(const<i125b>(13505789527944801758751150119415226784));
// DEFAULT-NEXT:             write<i127b, atomic=relaxed>(deref(read<ptr<atomic i127b>>(%14)), read<i127b>(deref(addr_of<ptr<i127b>>(%15))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<i127b>(%10, neg<i127b, overflow=ub>(const<i127b>(64910836855286429164283779649638556795)));
// DEFAULT-NEXT:         let %77: i127b [synthetic] = update<i127b, result=old, atomic=relaxed>(deref(addr_of<ptr<atomic i127b>>(%9)), add<i127b, overflow=wrap>(old<i127b>, read<i127b>(%10)));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%77), widen<i127b, reason=usual_arith>(const<i125b>(13505789527944801758751150119415226784)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %78: i127b [synthetic] = update<i127b, result=old, atomic=consume>(deref(addr_of<ptr<atomic i127b>>(%9)), add<i127b, overflow=wrap>(old<i127b>, widen<i127b, reason=arg>(const<i122b>(2227507280963412295355244564739509222))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%78), neg<i127b, overflow=ub>(const<i127b>(51405047327341627405532629530223330011)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %79: i127b [synthetic] = update<i127b, result=old, atomic=acquire>(deref(addr_of<ptr<atomic i127b>>(%9)), add<i127b, overflow=wrap>(old<i127b>, read<i127b>(%10)));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%79), neg<i127b, overflow=ub>(const<i127b>(49177540046378215110177384965483820789)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %80: i127b [synthetic] = update<i127b, result=old, atomic=release>(deref(addr_of<ptr<atomic i127b>>(%9)), add<i127b, overflow=wrap>(old<i127b>, widen<i127b, reason=arg>(const<i126b>(42245667388877614520169143618236120405))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%80), const<i127b>(56052806558804587457226139100761728144))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %81: i127b [synthetic] = update<i127b, result=old, atomic=acq_rel>(deref(addr_of<ptr<atomic i127b>>(%9)), add<i127b, overflow=wrap>(old<i127b>, read<i127b>(%10)));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%81), neg<i127b, overflow=ub>(const<i127b>(71842709512787029754292020996886257179)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %82: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%9)), add<i127b, overflow=wrap>(old<i127b>, const<i127b>(77995075987640754057679086146674392947)));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%82), widen<i127b, reason=usual_arith>(const<i126b>(33387637092395772813111503069359291754)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %83: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%9)), add<i127b, overflow=wrap>(old<i127b>, widen<i127b, reason=arg>(const<i125b>(11810767284628435493328779846084830297))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%83), neg<i127b, overflow=ub>(const<i127b>(58758470380432704860896714499850421027)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %84: i127b [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %16 __atomic_load_ptr: ptr<atomic i127b> [storage=automatic] = addr_of<ptr<atomic i127b>>(%9);
// DEFAULT-NEXT:             let %17 __atomic_load_tmp: i127b [storage=automatic];
// DEFAULT-NEXT:             write<i127b>(deref(addr_of<ptr<i127b>>(%17)), read<i127b, atomic=seq_cst>(deref(read<ptr<atomic i127b>>(%16))));
// DEFAULT-NEXT:             write<i127b>(%84, read<i127b>(%17));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%84), neg<i127b, overflow=ub>(const<i127b>(46947703095804269367567934653765590730)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @test_fetch_sub() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %19 __atomic_store_ptr: ptr<atomic i127b> [storage=automatic] = addr_of<ptr<atomic i127b>>(%9);
// DEFAULT-NEXT:             let %20 __atomic_store_tmp: i127b [storage=automatic] = widen<i127b, reason=assign>(const<i126b>(30796781768365552851024605388374299173));
// DEFAULT-NEXT:             write<i127b, atomic=release>(deref(read<ptr<atomic i127b>>(%19)), read<i127b>(deref(addr_of<ptr<i127b>>(%20))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<i127b>(%10, widen<i127b, reason=assign>(const<i126b>(32457177597484647488149720668185011722)));
// DEFAULT-NEXT:         let %85: i127b [synthetic] = update<i127b, result=old, atomic=relaxed>(deref(addr_of<ptr<atomic i127b>>(%9)), sub<i127b, overflow=wrap>(old<i127b>, read<i127b>(%10)));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%85), widen<i127b, reason=usual_arith>(const<i126b>(30796781768365552851024605388374299173)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %86: i127b [synthetic] = update<i127b, result=old, atomic=consume>(deref(addr_of<ptr<atomic i127b>>(%9)), sub<i127b, overflow=wrap>(old<i127b>, const<i127b>(54614103079293459991417218347656369566)));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%86), widen<i127b, reason=usual_arith>(neg<i122b, overflow=ub>(const<i122b>(1660395829119094637125115279810712549))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %87: i127b [synthetic] = update<i127b, result=old, atomic=acquire>(deref(addr_of<ptr<atomic i127b>>(%9)), sub<i127b, overflow=wrap>(old<i127b>, read<i127b>(%10)));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%87), neg<i127b, overflow=ub>(const<i127b>(56274498908412554628542333627467082115)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %88: i127b [synthetic] = update<i127b, result=old, atomic=release>(deref(addr_of<ptr<atomic i127b>>(%9)), sub<i127b, overflow=wrap>(old<i127b>, neg<i127b, overflow=ub>(const<i127b>(44514083923735151931107302009741400482))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%88), const<i127b>(81409506954572029614995249420232011891))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %89: i127b [synthetic] = update<i127b, result=old, atomic=acq_rel>(deref(addr_of<ptr<atomic i127b>>(%9)), sub<i127b, overflow=wrap>(old<i127b>, read<i127b>(%10)));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%89), neg<i127b, overflow=ub>(const<i127b>(44217592582162050185584752285910693355)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %90: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%9)), sub<i127b, overflow=wrap>(old<i127b>, widen<i127b, reason=arg>(const<i126b>(30348078982452392099140613411731040827))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%90), neg<i127b, overflow=ub>(const<i127b>(76674770179646697673734472954095705077)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %91: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%9)), sub<i127b, overflow=wrap>(old<i127b>, neg<i127b, overflow=ub>(const<i127b>(82224045897086857020012824788652775087))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%91), const<i127b>(63118334298370141958812217350057359824))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %92: i127b [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %21 __atomic_load_ptr: ptr<atomic i127b> [storage=automatic] = addr_of<ptr<atomic i127b>>(%9);
// DEFAULT-NEXT:             let %22 __atomic_load_tmp: i127b [storage=automatic];
// DEFAULT-NEXT:             write<i127b>(deref(addr_of<ptr<i127b>>(%22)), read<i127b, atomic=acquire>(deref(read<ptr<atomic i127b>>(%21))));
// DEFAULT-NEXT:             write<i127b>(%92, read<i127b>(%22));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%92), widen<i127b, reason=usual_arith>(neg<i126b, overflow=ub>(const<i126b>(24798803265012232752862261577173970817))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @test_fetch_and() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %24 __atomic_store_ptr: ptr<atomic i127b> [storage=automatic] = addr_of<ptr<atomic i127b>>(%9);
// DEFAULT-NEXT:             let %25 __atomic_store_tmp: i127b [storage=automatic] = read<i127b>(%12);
// DEFAULT-NEXT:             write<i127b, atomic=seq_cst>(deref(read<ptr<atomic i127b>>(%24)), read<i127b>(deref(addr_of<ptr<i127b>>(%25))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %93: i127b [synthetic] = update<i127b, result=old, atomic=relaxed>(deref(addr_of<ptr<atomic i127b>>(%9)), and<i127b>(old<i127b>, widen<i127b, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%93), read<i127b>(%12))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %94: i127b [synthetic] = update<i127b, result=old, atomic=consume>(deref(addr_of<ptr<atomic i127b>>(%9)), and<i127b>(old<i127b>, read<i127b>(%12)));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%94), widen<i127b, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %95: i127b [synthetic] = update<i127b, result=old, atomic=acquire>(deref(addr_of<ptr<atomic i127b>>(%9)), and<i127b>(old<i127b>, widen<i127b, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%95), widen<i127b, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         write<i127b, atomic=seq_cst>(%9, not<i127b>(read<i127b, atomic=seq_cst>(%9)));
// DEFAULT-NEXT:         let %96: i127b [synthetic] = update<i127b, result=old, atomic=release>(deref(addr_of<ptr<atomic i127b>>(%9)), and<i127b>(old<i127b>, read<i127b>(%12)));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%96), read<i127b>(%12))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %97: i127b [synthetic] = update<i127b, result=old, atomic=acq_rel>(deref(addr_of<ptr<atomic i127b>>(%9)), and<i127b>(old<i127b>, widen<i127b, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%97), read<i127b>(%12))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %98: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%9)), and<i127b>(old<i127b>, widen<i127b, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%98), widen<i127b, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %99: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%9)), and<i127b>(old<i127b>, widen<i127b, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%99), widen<i127b, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %26 @test_fetch_xor() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i127b, atomic=seq_cst>(%9, read<i127b>(%12));
// DEFAULT-NEXT:         write<i127b>(%10, widen<i127b, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %100: i127b [synthetic] = update<i127b, result=old, atomic=relaxed>(deref(addr_of<ptr<atomic i127b>>(%9)), xor<i127b>(old<i127b>, read<i127b>(%10)));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%100), read<i127b>(%12))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %101: i127b [synthetic] = update<i127b, result=old, atomic=consume>(deref(addr_of<ptr<atomic i127b>>(%9)), xor<i127b>(old<i127b>, not<i127b>(read<i127b>(%10))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%101), read<i127b>(%12))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %102: i127b [synthetic] = update<i127b, result=old, atomic=acquire>(deref(addr_of<ptr<atomic i127b>>(%9)), xor<i127b>(old<i127b>, widen<i127b, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%102), widen<i127b, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %103: i127b [synthetic] = update<i127b, result=old, atomic=release>(deref(addr_of<ptr<atomic i127b>>(%9)), xor<i127b>(old<i127b>, not<i127b>(read<i127b>(%10))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%103), widen<i127b, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %104: i127b [synthetic] = update<i127b, result=old, atomic=acq_rel>(deref(addr_of<ptr<atomic i127b>>(%9)), xor<i127b>(old<i127b>, widen<i127b, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%104), read<i127b>(%12))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %105: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%9)), xor<i127b>(old<i127b>, not<i127b>(read<i127b>(%10))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%105), read<i127b>(%12))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %106: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%9)), xor<i127b>(old<i127b>, not<i127b>(read<i127b>(%10))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%106), widen<i127b, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %27 @test_fetch_or() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i127b, atomic=seq_cst>(%9, widen<i127b, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         write<i127b>(%10, widen<i127b, reason=assign>(const<i46b>(17592186044416)));
// DEFAULT-NEXT:         let %107: i127b [synthetic] = update<i127b, result=old, atomic=relaxed>(deref(addr_of<ptr<atomic i127b>>(%9)), or<i127b>(old<i127b>, read<i127b>(%10)));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%107), widen<i127b, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %108: i127b [synthetic] = read<i127b>(%10);
// DEFAULT-NEXT:         let %109: i127b [synthetic] = mul<i127b, overflow=ub>(read<i127b>(%108), widen<i127b, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i127b>(%10, read<i127b>(%109));
// DEFAULT-NEXT:         let %110: i127b [synthetic] = update<i127b, result=old, atomic=consume>(deref(addr_of<ptr<atomic i127b>>(%9)), or<i127b>(old<i127b>, widen<i127b, reason=arg>(const<i47b>(35184372088832))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%110), widen<i127b, reason=usual_arith>(const<i46b>(17592186044416)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %111: i127b [synthetic] = read<i127b>(%10);
// DEFAULT-NEXT:         let %112: i127b [synthetic] = mul<i127b, overflow=ub>(read<i127b>(%111), widen<i127b, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i127b>(%10, read<i127b>(%112));
// DEFAULT-NEXT:         let %113: i127b [synthetic] = update<i127b, result=old, atomic=acquire>(deref(addr_of<ptr<atomic i127b>>(%9)), or<i127b>(old<i127b>, read<i127b>(%10)));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%113), widen<i127b, reason=usual_arith>(const<i47b>(52776558133248)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %114: i127b [synthetic] = read<i127b>(%10);
// DEFAULT-NEXT:         let %115: i127b [synthetic] = mul<i127b, overflow=ub>(read<i127b>(%114), widen<i127b, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i127b>(%10, read<i127b>(%115));
// DEFAULT-NEXT:         let %116: i127b [synthetic] = update<i127b, result=old, atomic=release>(deref(addr_of<ptr<atomic i127b>>(%9)), or<i127b>(old<i127b>, widen<i127b, reason=arg>(const<i49b>(140737488355328))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%116), widen<i127b, reason=usual_arith>(const<i48b>(123145302310912)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %117: i127b [synthetic] = read<i127b>(%10);
// DEFAULT-NEXT:         let %118: i127b [synthetic] = mul<i127b, overflow=ub>(read<i127b>(%117), widen<i127b, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i127b>(%10, read<i127b>(%118));
// DEFAULT-NEXT:         let %119: i127b [synthetic] = update<i127b, result=old, atomic=acq_rel>(deref(addr_of<ptr<atomic i127b>>(%9)), or<i127b>(old<i127b>, read<i127b>(%10)));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%119), widen<i127b, reason=usual_arith>(const<i49b>(263882790666240)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %120: i127b [synthetic] = read<i127b>(%10);
// DEFAULT-NEXT:         let %121: i127b [synthetic] = mul<i127b, overflow=ub>(read<i127b>(%120), widen<i127b, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i127b>(%10, read<i127b>(%121));
// DEFAULT-NEXT:         let %122: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%9)), or<i127b>(old<i127b>, read<i127b>(%10)));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%122), widen<i127b, reason=usual_arith>(const<i50b>(545357767376896)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %123: i127b [synthetic] = read<i127b>(%10);
// DEFAULT-NEXT:         let %124: i127b [synthetic] = mul<i127b, overflow=ub>(read<i127b>(%123), widen<i127b, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i127b>(%10, read<i127b>(%124));
// DEFAULT-NEXT:         let %125: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%9)), or<i127b>(old<i127b>, read<i127b>(%10)));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%125), widen<i127b, reason=usual_arith>(const<i51b>(1108307720798208)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %28 @test_add() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i127b, atomic=seq_cst>(%9, widen<i127b, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         write<i127b>(%10, widen<i127b, reason=assign>(const<i74b>(4722366482869645213696)));
// DEFAULT-NEXT:         let %126: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%9)), add<i127b, overflow=wrap>(old<i127b>, read<i127b>(%10)));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%9), widen<i127b, reason=usual_arith>(const<i74b>(4722366482869645213696)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %127: i127b [synthetic] = update<i127b, result=old, atomic=consume>(deref(addr_of<ptr<atomic i127b>>(%9)), add<i127b, overflow=wrap>(old<i127b>, read<i127b>(%10)));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%9), widen<i127b, reason=usual_arith>(const<i75b>(9444732965739290427392)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %128: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%9)), add<i127b, overflow=wrap>(old<i127b>, widen<i127b, reason=arg>(const<i74b>(4722366482869645213696))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%9), widen<i127b, reason=usual_arith>(const<i75b>(14167099448608935641088)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %129: i127b [synthetic] = update<i127b, result=old, atomic=release>(deref(addr_of<ptr<atomic i127b>>(%9)), add<i127b, overflow=wrap>(old<i127b>, widen<i127b, reason=arg>(const<i74b>(4722366482869645213696))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%9), widen<i127b, reason=usual_arith>(const<i76b>(18889465931478580854784)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %130: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%9)), add<i127b, overflow=wrap>(old<i127b>, widen<i127b, reason=arg>(const<i74b>(4722366482869645213696))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%9), widen<i127b, reason=usual_arith>(const<i76b>(23611832414348226068480)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %131: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%9)), add<i127b, overflow=wrap>(old<i127b>, read<i127b>(%10)));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%9), widen<i127b, reason=usual_arith>(const<i76b>(28334198897217871282176)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %29 @test_sub() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i127b>(%11, widen<i127b, reason=assign>(neg<i123b, overflow=ub>(const<i123b>(3638804536836293398783417724445294828))));
// DEFAULT-NEXT:         write<i127b, atomic=seq_cst>(%9, widen<i127b, reason=assign>(neg<i123b, overflow=ub>(const<i123b>(3638804536836293398783417724445294828))));
// DEFAULT-NEXT:         write<i127b>(%10, widen<i127b, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %132: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%9)), sub<i127b, overflow=wrap>(old<i127b>, add<i127b, overflow=ub>(read<i127b>(%10), widen<i127b, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         let %133: i127b [synthetic] = read<i127b>(%11);
// DEFAULT-NEXT:         let %134: i127b [synthetic] = sub<i127b, overflow=ub>(read<i127b>(%133), widen<i127b, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i127b>(%11, read<i127b>(%134));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%9), read<i127b>(%134))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %135: i127b [synthetic] = update<i127b, result=old, atomic=consume>(deref(addr_of<ptr<atomic i127b>>(%9)), sub<i127b, overflow=wrap>(old<i127b>, add<i127b, overflow=ub>(read<i127b>(%10), widen<i127b, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         let %136: i127b [synthetic] = read<i127b>(%11);
// DEFAULT-NEXT:         let %137: i127b [synthetic] = sub<i127b, overflow=ub>(read<i127b>(%136), widen<i127b, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i127b>(%11, read<i127b>(%137));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%9), read<i127b>(%137))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %138: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%9)), sub<i127b, overflow=wrap>(old<i127b>, widen<i127b, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         let %139: i127b [synthetic] = read<i127b>(%11);
// DEFAULT-NEXT:         let %140: i127b [synthetic] = sub<i127b, overflow=ub>(read<i127b>(%139), widen<i127b, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i127b>(%11, read<i127b>(%140));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%9), read<i127b>(%140))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %141: i127b [synthetic] = update<i127b, result=old, atomic=release>(deref(addr_of<ptr<atomic i127b>>(%9)), sub<i127b, overflow=wrap>(old<i127b>, widen<i127b, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         let %142: i127b [synthetic] = read<i127b>(%11);
// DEFAULT-NEXT:         let %143: i127b [synthetic] = sub<i127b, overflow=ub>(read<i127b>(%142), widen<i127b, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i127b>(%11, read<i127b>(%143));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%9), read<i127b>(%143))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %144: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%9)), sub<i127b, overflow=wrap>(old<i127b>, add<i127b, overflow=ub>(read<i127b>(%10), widen<i127b, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         let %145: i127b [synthetic] = read<i127b>(%11);
// DEFAULT-NEXT:         let %146: i127b [synthetic] = sub<i127b, overflow=ub>(read<i127b>(%145), widen<i127b, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i127b>(%11, read<i127b>(%146));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%9), read<i127b>(%146))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %147: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%9)), sub<i127b, overflow=wrap>(old<i127b>, add<i127b, overflow=ub>(read<i127b>(%10), widen<i127b, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         let %148: i127b [synthetic] = read<i127b>(%11);
// DEFAULT-NEXT:         let %149: i127b [synthetic] = sub<i127b, overflow=ub>(read<i127b>(%148), widen<i127b, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i127b>(%11, read<i127b>(%149));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%9), read<i127b>(%149))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %30 @test_and() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i127b, atomic=seq_cst>(%9, read<i127b>(%12));
// DEFAULT-NEXT:         let %150: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%9)), and<i127b>(old<i127b>, widen<i127b, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%9), widen<i127b, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         write<i127b, atomic=seq_cst>(%9, read<i127b>(%12));
// DEFAULT-NEXT:         let %151: i127b [synthetic] = update<i127b, result=old, atomic=consume>(deref(addr_of<ptr<atomic i127b>>(%9)), and<i127b>(old<i127b>, read<i127b>(%12)));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%9), read<i127b>(%12))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %152: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%9)), and<i127b>(old<i127b>, widen<i127b, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%9), widen<i127b, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         write<i127b, atomic=seq_cst>(%9, not<i127b>(read<i127b, atomic=seq_cst>(%9)));
// DEFAULT-NEXT:         let %153: i127b [synthetic] = update<i127b, result=old, atomic=release>(deref(addr_of<ptr<atomic i127b>>(%9)), and<i127b>(old<i127b>, read<i127b>(%12)));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%9), read<i127b>(%12))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %154: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%9)), and<i127b>(old<i127b>, widen<i127b, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%9), widen<i127b, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         write<i127b, atomic=seq_cst>(%9, not<i127b>(read<i127b, atomic=seq_cst>(%9)));
// DEFAULT-NEXT:         let %155: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%9)), and<i127b>(old<i127b>, widen<i127b, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%9), widen<i127b, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %31 @test_xor() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i127b, atomic=seq_cst>(%9, read<i127b>(%12));
// DEFAULT-NEXT:         write<i127b>(%10, widen<i127b, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %156: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%9)), xor<i127b>(old<i127b>, read<i127b>(%10)));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%9), read<i127b>(%12))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %157: i127b [synthetic] = update<i127b, result=old, atomic=consume>(deref(addr_of<ptr<atomic i127b>>(%9)), xor<i127b>(old<i127b>, not<i127b>(read<i127b>(%10))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%9), widen<i127b, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %158: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%9)), xor<i127b>(old<i127b>, widen<i127b, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%9), widen<i127b, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %159: i127b [synthetic] = update<i127b, result=old, atomic=release>(deref(addr_of<ptr<atomic i127b>>(%9)), xor<i127b>(old<i127b>, not<i127b>(read<i127b>(%10))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%9), read<i127b>(%12))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %160: i127b [synthetic] = update<i127b, result=old, atomic=acq_rel>(deref(addr_of<ptr<atomic i127b>>(%9)), xor<i127b>(old<i127b>, widen<i127b, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%9), read<i127b>(%12))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %161: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%9)), xor<i127b>(old<i127b>, not<i127b>(read<i127b>(%10))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%9), widen<i127b, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %32 @test_or() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i127b, atomic=seq_cst>(%9, widen<i127b, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         write<i127b>(%10, widen<i127b, reason=assign>(const<i86b>(19342813113834066795298816)));
// DEFAULT-NEXT:         let %162: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%9)), or<i127b>(old<i127b>, read<i127b>(%10)));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%9), widen<i127b, reason=usual_arith>(const<i86b>(19342813113834066795298816)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %163: i127b [synthetic] = read<i127b>(%10);
// DEFAULT-NEXT:         let %164: i127b [synthetic] = mul<i127b, overflow=ub>(read<i127b>(%163), widen<i127b, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i127b>(%10, read<i127b>(%164));
// DEFAULT-NEXT:         let %165: i127b [synthetic] = update<i127b, result=old, atomic=consume>(deref(addr_of<ptr<atomic i127b>>(%9)), or<i127b>(old<i127b>, read<i127b>(%10)));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%9), widen<i127b, reason=usual_arith>(const<i87b>(58028439341502200385896448)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %166: i127b [synthetic] = read<i127b>(%10);
// DEFAULT-NEXT:         let %167: i127b [synthetic] = mul<i127b, overflow=ub>(read<i127b>(%166), widen<i127b, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i127b>(%10, read<i127b>(%167));
// DEFAULT-NEXT:         let %168: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%9)), or<i127b>(old<i127b>, widen<i127b, reason=arg>(const<i88b>(77371252455336267181195264))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%9), widen<i127b, reason=usual_arith>(const<i88b>(135399691796838467567091712)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %169: i127b [synthetic] = read<i127b>(%10);
// DEFAULT-NEXT:         let %170: i127b [synthetic] = mul<i127b, overflow=ub>(read<i127b>(%169), widen<i127b, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i127b>(%10, read<i127b>(%170));
// DEFAULT-NEXT:         let %171: i127b [synthetic] = update<i127b, result=old, atomic=release>(deref(addr_of<ptr<atomic i127b>>(%9)), or<i127b>(old<i127b>, widen<i127b, reason=arg>(const<i89b>(154742504910672534362390528))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%9), widen<i127b, reason=usual_arith>(const<i89b>(290142196707511001929482240)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %172: i127b [synthetic] = read<i127b>(%10);
// DEFAULT-NEXT:         let %173: i127b [synthetic] = mul<i127b, overflow=ub>(read<i127b>(%172), widen<i127b, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i127b>(%10, read<i127b>(%173));
// DEFAULT-NEXT:         let %174: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%9)), or<i127b>(old<i127b>, read<i127b>(%10)));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%9), widen<i127b, reason=usual_arith>(const<i90b>(599627206528856070654263296)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %175: i127b [synthetic] = read<i127b>(%10);
// DEFAULT-NEXT:         let %176: i127b [synthetic] = mul<i127b, overflow=ub>(read<i127b>(%175), widen<i127b, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i127b>(%10, read<i127b>(%176));
// DEFAULT-NEXT:         let %177: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%9)), or<i127b>(old<i127b>, read<i127b>(%10)));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%9), widen<i127b, reason=usual_arith>(const<i91b>(1218597226171546208103825408)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %33 @test_exchange() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %34 __atomic_store_ptr: ptr<atomic i127b> [storage=automatic] = addr_of<ptr<atomic i127b>>(%9);
// DEFAULT-NEXT:             let %35 __atomic_store_tmp: i127b [storage=automatic] = widen<i127b, reason=assign>(const<i125b>(15794812138349191682564933935017390008));
// DEFAULT-NEXT:             write<i127b, atomic=seq_cst>(deref(read<ptr<atomic i127b>>(%34)), read<i127b>(deref(addr_of<ptr<i127b>>(%35))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %178: i127b [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %36 __atomic_exchange_ptr: ptr<atomic i127b> [storage=automatic] = addr_of<ptr<atomic i127b>>(%9);
// DEFAULT-NEXT:             let %37 __atomic_exchange_val: i127b [storage=automatic] = widen<i127b, reason=assign>(neg<i122b, overflow=ub>(const<i122b>(2166613183393424891717146518563613668)));
// DEFAULT-NEXT:             let %38 __atomic_exchange_tmp: i127b [storage=automatic];
// DEFAULT-NEXT:             let %179: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(read<ptr<atomic i127b>>(%36)), read<i127b>(deref(addr_of<ptr<i127b>>(%37))));
// DEFAULT-NEXT:             write<i127b>(deref(addr_of<ptr<i127b>>(%38)), read<i127b>(%179));
// DEFAULT-NEXT:             write<i127b>(%178, read<i127b>(%38));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %180: bool [synthetic];
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%178), widen<i127b, reason=usual_arith>(const<i125b>(15794812138349191682564933935017390008)))
// DEFAULT-NEXT:             write<bool>(%180, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %181: i127b [synthetic];
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %39 __atomic_load_ptr: ptr<atomic i127b> [storage=automatic] = addr_of<ptr<atomic i127b>>(%9);
// DEFAULT-NEXT:                 let %40 __atomic_load_tmp: i127b [storage=automatic];
// DEFAULT-NEXT:                 write<i127b>(deref(addr_of<ptr<i127b>>(%40)), read<i127b, atomic=seq_cst>(deref(read<ptr<atomic i127b>>(%39))));
// DEFAULT-NEXT:                 write<i127b>(%181, read<i127b>(%40));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             write<bool>(%180, ne<i127b>(read<i127b>(%181), widen<i127b, reason=usual_arith>(neg<i122b, overflow=ub>(const<i122b>(2166613183393424891717146518563613668)))));
// DEFAULT-NEXT:         if read<bool>(%180)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %182: i127b [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %41 __atomic_exchange_ptr: ptr<atomic i127b> [storage=automatic] = addr_of<ptr<atomic i127b>>(%9);
// DEFAULT-NEXT:             let %42 __atomic_exchange_val: i127b [storage=automatic] = const<i127b>(61251098386268815852902382804483910638);
// DEFAULT-NEXT:             let %43 __atomic_exchange_tmp: i127b [storage=automatic];
// DEFAULT-NEXT:             let %183: i127b [synthetic] = update<i127b, result=old, atomic=relaxed>(deref(read<ptr<atomic i127b>>(%41)), read<i127b>(deref(addr_of<ptr<i127b>>(%42))));
// DEFAULT-NEXT:             write<i127b>(deref(addr_of<ptr<i127b>>(%43)), read<i127b>(%183));
// DEFAULT-NEXT:             write<i127b>(%182, read<i127b>(%43));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %184: bool [synthetic];
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%182), widen<i127b, reason=usual_arith>(neg<i122b, overflow=ub>(const<i122b>(2166613183393424891717146518563613668))))
// DEFAULT-NEXT:             write<bool>(%184, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %185: i127b [synthetic];
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %44 __atomic_load_ptr: ptr<atomic i127b> [storage=automatic] = addr_of<ptr<atomic i127b>>(%9);
// DEFAULT-NEXT:                 let %45 __atomic_load_tmp: i127b [storage=automatic];
// DEFAULT-NEXT:                 write<i127b>(deref(addr_of<ptr<i127b>>(%45)), read<i127b, atomic=acquire>(deref(read<ptr<atomic i127b>>(%44))));
// DEFAULT-NEXT:                 write<i127b>(%185, read<i127b>(%45));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             write<bool>(%184, ne<i127b>(read<i127b>(%185), const<i127b>(61251098386268815852902382804483910638)));
// DEFAULT-NEXT:         if read<bool>(%184)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         write<i127b>(%10, widen<i127b, reason=assign>(neg<i122b, overflow=ub>(const<i122b>(2166613183393424891717146518563613668))));
// DEFAULT-NEXT:         let %186: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %46 __atomic_compare_exchange_ptr: ptr<atomic i127b> [storage=automatic] = addr_of<ptr<atomic i127b>>(%9);
// DEFAULT-NEXT:             let %47 __atomic_compare_exchange_tmp: i127b [storage=automatic] = widen<i127b, reason=assign>(neg<i126b, overflow=ub>(const<i126b>(36677332297536901313774263310237646448)));
// DEFAULT-NEXT:             let %187: bool [synthetic] = compare_exchange<i127b, form=write_back, weak=false, success=seq_cst, failure=seq_cst>(deref(read<ptr<atomic i127b>>(%46)), addr_of<ptr<i127b>>(%10), read<i127b>(deref(addr_of<ptr<i127b>>(%47))));
// DEFAULT-NEXT:             write<bool>(%186, read<bool>(%187));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if read<bool>(%186)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %188: bool [synthetic];
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%10), const<i127b>(61251098386268815852902382804483910638))
// DEFAULT-NEXT:             write<bool>(%188, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %189: i127b [synthetic];
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %48 __atomic_load_ptr: ptr<atomic i127b> [storage=automatic] = addr_of<ptr<atomic i127b>>(%9);
// DEFAULT-NEXT:                 let %49 __atomic_load_tmp: i127b [storage=automatic];
// DEFAULT-NEXT:                 write<i127b>(deref(addr_of<ptr<i127b>>(%49)), read<i127b, atomic=seq_cst>(deref(read<ptr<atomic i127b>>(%48))));
// DEFAULT-NEXT:                 write<i127b>(%189, read<i127b>(%49));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             write<bool>(%188, ne<i127b>(read<i127b>(%189), const<i127b>(61251098386268815852902382804483910638)));
// DEFAULT-NEXT:         if read<bool>(%188)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %190: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %50 __atomic_compare_exchange_ptr: ptr<atomic i127b> [storage=automatic] = addr_of<ptr<atomic i127b>>(%9);
// DEFAULT-NEXT:             let %51 __atomic_compare_exchange_tmp: i127b [storage=automatic] = widen<i127b, reason=assign>(neg<i126b, overflow=ub>(const<i126b>(36677332297536901313774263310237646448)));
// DEFAULT-NEXT:             let %191: bool [synthetic] = compare_exchange<i127b, form=write_back, weak=false, success=seq_cst, failure=seq_cst>(deref(read<ptr<atomic i127b>>(%50)), addr_of<ptr<i127b>>(%10), read<i127b>(deref(addr_of<ptr<i127b>>(%51))));
// DEFAULT-NEXT:             write<bool>(%190, read<bool>(%191));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if not<bool>(read<bool>(%190))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %192: bool [synthetic];
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%10), const<i127b>(61251098386268815852902382804483910638))
// DEFAULT-NEXT:             write<bool>(%192, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %193: i127b [synthetic];
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %52 __atomic_load_ptr: ptr<atomic i127b> [storage=automatic] = addr_of<ptr<atomic i127b>>(%9);
// DEFAULT-NEXT:                 let %53 __atomic_load_tmp: i127b [storage=automatic];
// DEFAULT-NEXT:                 write<i127b>(deref(addr_of<ptr<i127b>>(%53)), read<i127b, atomic=seq_cst>(deref(read<ptr<atomic i127b>>(%52))));
// DEFAULT-NEXT:                 write<i127b>(%193, read<i127b>(%53));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             write<bool>(%192, ne<i127b>(read<i127b>(%193), widen<i127b, reason=usual_arith>(neg<i126b, overflow=ub>(const<i126b>(36677332297536901313774263310237646448)))));
// DEFAULT-NEXT:         if read<bool>(%192)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         write<i127b>(%10, widen<i127b, reason=assign>(neg<i122b, overflow=ub>(const<i122b>(2166613183393424891717146518563613668))));
// DEFAULT-NEXT:         let %194: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %54 __atomic_compare_exchange_ptr: ptr<atomic i127b> [storage=automatic] = addr_of<ptr<atomic i127b>>(%9);
// DEFAULT-NEXT:             let %55 __atomic_compare_exchange_tmp: i127b [storage=automatic] = const<i127b>(73949932022761409003352953944661689416);
// DEFAULT-NEXT:             let %195: bool [synthetic] = compare_exchange<i127b, form=write_back, weak=false, success=seq_cst, failure=relaxed>(deref(read<ptr<atomic i127b>>(%54)), addr_of<ptr<i127b>>(%10), read<i127b>(deref(addr_of<ptr<i127b>>(%55))));
// DEFAULT-NEXT:             write<bool>(%194, read<bool>(%195));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if read<bool>(%194)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %196: bool [synthetic];
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%10), widen<i127b, reason=usual_arith>(neg<i126b, overflow=ub>(const<i126b>(36677332297536901313774263310237646448))))
// DEFAULT-NEXT:             write<bool>(%196, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %197: i127b [synthetic];
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %56 __atomic_load_ptr: ptr<atomic i127b> [storage=automatic] = addr_of<ptr<atomic i127b>>(%9);
// DEFAULT-NEXT:                 let %57 __atomic_load_tmp: i127b [storage=automatic];
// DEFAULT-NEXT:                 write<i127b>(deref(addr_of<ptr<i127b>>(%57)), read<i127b, atomic=seq_cst>(deref(read<ptr<atomic i127b>>(%56))));
// DEFAULT-NEXT:                 write<i127b>(%197, read<i127b>(%57));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             write<bool>(%196, ne<i127b>(read<i127b>(%197), widen<i127b, reason=usual_arith>(neg<i126b, overflow=ub>(const<i126b>(36677332297536901313774263310237646448)))));
// DEFAULT-NEXT:         if read<bool>(%196)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %198: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %58 __atomic_compare_exchange_ptr: ptr<atomic i127b> [storage=automatic] = addr_of<ptr<atomic i127b>>(%9);
// DEFAULT-NEXT:             let %59 __atomic_compare_exchange_tmp: i127b [storage=automatic] = const<i127b>(73949932022761409003352953944661689416);
// DEFAULT-NEXT:             let %199: bool [synthetic] = compare_exchange<i127b, form=write_back, weak=false, success=seq_cst, failure=seq_cst>(deref(read<ptr<atomic i127b>>(%58)), addr_of<ptr<i127b>>(%10), read<i127b>(deref(addr_of<ptr<i127b>>(%59))));
// DEFAULT-NEXT:             write<bool>(%198, read<bool>(%199));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if not<bool>(read<bool>(%198))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %200: bool [synthetic];
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%10), widen<i127b, reason=usual_arith>(neg<i126b, overflow=ub>(const<i126b>(36677332297536901313774263310237646448))))
// DEFAULT-NEXT:             write<bool>(%200, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %201: i127b [synthetic];
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %60 __atomic_load_ptr: ptr<atomic i127b> [storage=automatic] = addr_of<ptr<atomic i127b>>(%9);
// DEFAULT-NEXT:                 let %61 __atomic_load_tmp: i127b [storage=automatic];
// DEFAULT-NEXT:                 write<i127b>(deref(addr_of<ptr<i127b>>(%61)), read<i127b, atomic=seq_cst>(deref(read<ptr<atomic i127b>>(%60))));
// DEFAULT-NEXT:                 write<i127b>(%201, read<i127b>(%61));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             write<bool>(%200, ne<i127b>(read<i127b>(%201), const<i127b>(73949932022761409003352953944661689416)));
// DEFAULT-NEXT:         if read<bool>(%200)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %202: i127b [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %62 __atomic_load_ptr: ptr<atomic i127b> [storage=automatic] = addr_of<ptr<atomic i127b>>(%9);
// DEFAULT-NEXT:             let %63 __atomic_load_tmp: i127b [storage=automatic];
// DEFAULT-NEXT:             write<i127b>(deref(addr_of<ptr<i127b>>(%63)), read<i127b, atomic=seq_cst>(deref(read<ptr<atomic i127b>>(%62))));
// DEFAULT-NEXT:             write<i127b>(%202, read<i127b>(%63));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<i127b>(%10, read<i127b>(%202));
// DEFAULT-NEXT:         do %75
// DEFAULT-NEXT:             write<i127b>(%11, add<i127b, overflow=ub>(read<i127b>(%10), neg<i127b, overflow=ub>(const<i127b>(82256758205518164043596305502815392646))));
// DEFAULT-NEXT:         while {
// DEFAULT-NEXT:             let %203: bool [synthetic];
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %64 __atomic_compare_exchange_ptr: ptr<atomic i127b> [storage=automatic] = addr_of<ptr<atomic i127b>>(%9);
// DEFAULT-NEXT:                 let %65 __atomic_compare_exchange_tmp: i127b [storage=automatic] = read<i127b>(%11);
// DEFAULT-NEXT:                 let %204: bool [synthetic] = compare_exchange<i127b, form=write_back, weak=true, success=seq_cst, failure=seq_cst>(deref(read<ptr<atomic i127b>>(%64)), addr_of<ptr<i127b>>(%10), read<i127b>(deref(addr_of<ptr<i127b>>(%65))));
// DEFAULT-NEXT:                 write<bool>(%203, read<bool>(%204));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             yield not<bool>(read<bool>(%203));
// DEFAULT-NEXT:         };
// DEFAULT-NEXT:         let %205: i127b [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %66 __atomic_load_ptr: ptr<atomic i127b> [storage=automatic] = addr_of<ptr<atomic i127b>>(%9);
// DEFAULT-NEXT:             let %67 __atomic_load_tmp: i127b [storage=automatic];
// DEFAULT-NEXT:             write<i127b>(deref(addr_of<ptr<i127b>>(%67)), read<i127b, atomic=seq_cst>(deref(read<ptr<atomic i127b>>(%66))));
// DEFAULT-NEXT:             write<i127b>(%205, read<i127b>(%67));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%205), widen<i127b, reason=usual_arith>(neg<i124b, overflow=ub>(const<i124b>(8306826182756755040243351558153703230))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %206: i127b [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %68 __atomic_load_ptr: ptr<atomic i127b> [storage=automatic] = addr_of<ptr<atomic i127b>>(%9);
// DEFAULT-NEXT:             let %69 __atomic_load_tmp: i127b [storage=automatic];
// DEFAULT-NEXT:             write<i127b>(deref(addr_of<ptr<i127b>>(%69)), read<i127b, atomic=acquire>(deref(read<ptr<atomic i127b>>(%68))));
// DEFAULT-NEXT:             write<i127b>(%206, read<i127b>(%69));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<i127b>(%10, read<i127b>(%206));
// DEFAULT-NEXT:         do %76
// DEFAULT-NEXT:             write<i127b>(%11, add<i127b, overflow=ub>(read<i127b>(%10), const<i127b>(48855144829609538366772317026461909818)));
// DEFAULT-NEXT:         while {
// DEFAULT-NEXT:             let %207: bool [synthetic];
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %70 __atomic_compare_exchange_ptr: ptr<atomic i127b> [storage=automatic] = addr_of<ptr<atomic i127b>>(%9);
// DEFAULT-NEXT:                 let %71 __atomic_compare_exchange_tmp: i127b [storage=automatic] = read<i127b>(%11);
// DEFAULT-NEXT:                 let %208: bool [synthetic] = compare_exchange<i127b, form=write_back, weak=true, success=relaxed, failure=relaxed>(deref(read<ptr<atomic i127b>>(%70)), addr_of<ptr<i127b>>(%10), read<i127b>(deref(addr_of<ptr<i127b>>(%71))));
// DEFAULT-NEXT:                 write<bool>(%207, read<bool>(%208));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             yield not<bool>(read<bool>(%207));
// DEFAULT-NEXT:         };
// DEFAULT-NEXT:         let %209: i127b [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %72 __atomic_load_ptr: ptr<atomic i127b> [storage=automatic] = addr_of<ptr<atomic i127b>>(%9);
// DEFAULT-NEXT:             let %73 __atomic_load_tmp: i127b [storage=automatic];
// DEFAULT-NEXT:             write<i127b>(deref(addr_of<ptr<i127b>>(%73)), read<i127b, atomic=seq_cst>(deref(read<ptr<atomic i127b>>(%72))));
// DEFAULT-NEXT:             write<i127b>(%209, read<i127b>(%73));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%209), widen<i127b, reason=usual_arith>(const<i126b>(40548318646852783326528965468308206588)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %74 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%13);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%18);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%23);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%26);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%27);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%29);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%30);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%31);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%32);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%33);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
