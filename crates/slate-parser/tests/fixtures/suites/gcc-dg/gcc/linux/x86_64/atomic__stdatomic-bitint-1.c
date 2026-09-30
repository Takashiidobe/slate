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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_memory_order_relaxed:[0-9]+]] memory_order_relaxed = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_memory_order_consume:[0-9]+]] memory_order_consume = const<i32>(1);
// DEFAULT-NEXT:         %[[VALUE_memory_order_acquire:[0-9]+]] memory_order_acquire = const<i32>(2);
// DEFAULT-NEXT:         %[[VALUE_memory_order_release:[0-9]+]] memory_order_release = const<i32>(3);
// DEFAULT-NEXT:         %[[VALUE_memory_order_acq_rel:[0-9]+]] memory_order_acq_rel = const<i32>(4);
// DEFAULT-NEXT:         %[[VALUE_memory_order_seq_cst:[0-9]+]] memory_order_seq_cst = const<i32>(5);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_memory_order:[0-9]+]] memory_order = @type[[TYPE0]];
// DEFAULT-NEXT:     global %[[VALUE_v:[0-9]+]] v: atomic i127b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_count:[0-9]+]] count: i127b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_res:[0-9]+]] res: i127b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_init:[0-9]+]] init: i127b [storage=static] [const] = not<i127b>(widen<i127b, reason=explicit>(const<i2b>(0))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_test_fetch_add:[0-9]+]] @test_fetch_add() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr:[0-9]+]] __atomic_store_ptr: ptr<atomic i127b> [storage=automatic] = addr_of<ptr<atomic i127b>>(%[[VALUE_v]]);
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp:[0-9]+]] __atomic_store_tmp: i127b [storage=automatic] = widen<i127b, reason=assign>(const<i125b>(13505789527944801758751150119415226784));
// DEFAULT-NEXT:             write<i127b, atomic=relaxed>(deref(read<ptr<atomic i127b>>(%[[VALUE___atomic_store_ptr]])), read<i127b>(deref(addr_of<ptr<i127b>>(%[[VALUE___atomic_store_tmp]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<i127b>(%[[VALUE_count]], neg<i127b, overflow=ub>(const<i127b>(64910836855286429164283779649638556795)));
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=relaxed>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), add<i127b, overflow=wrap>(old<i127b>, read<i127b>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%[[VALUE0]]), widen<i127b, reason=usual_arith>(const<i125b>(13505789527944801758751150119415226784)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=consume>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), add<i127b, overflow=wrap>(old<i127b>, widen<i127b, reason=arg>(const<i122b>(2227507280963412295355244564739509222))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%[[VALUE1]]), neg<i127b, overflow=ub>(const<i127b>(51405047327341627405532629530223330011)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=acquire>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), add<i127b, overflow=wrap>(old<i127b>, read<i127b>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%[[VALUE2]]), neg<i127b, overflow=ub>(const<i127b>(49177540046378215110177384965483820789)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=release>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), add<i127b, overflow=wrap>(old<i127b>, widen<i127b, reason=arg>(const<i126b>(42245667388877614520169143618236120405))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%[[VALUE3]]), const<i127b>(56052806558804587457226139100761728144))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=acq_rel>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), add<i127b, overflow=wrap>(old<i127b>, read<i127b>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%[[VALUE4]]), neg<i127b, overflow=ub>(const<i127b>(71842709512787029754292020996886257179)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), add<i127b, overflow=wrap>(old<i127b>, const<i127b>(77995075987640754057679086146674392947)));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%[[VALUE5]]), widen<i127b, reason=usual_arith>(const<i126b>(33387637092395772813111503069359291754)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), add<i127b, overflow=wrap>(old<i127b>, widen<i127b, reason=arg>(const<i125b>(11810767284628435493328779846084830297))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%[[VALUE6]]), neg<i127b, overflow=ub>(const<i127b>(58758470380432704860896714499850421027)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: i127b [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_load_ptr:[0-9]+]] __atomic_load_ptr: ptr<atomic i127b> [storage=automatic] = addr_of<ptr<atomic i127b>>(%[[VALUE_v]]);
// DEFAULT-NEXT:             let %[[VALUE___atomic_load_tmp:[0-9]+]] __atomic_load_tmp: i127b [storage=automatic];
// DEFAULT-NEXT:             write<i127b>(deref(addr_of<ptr<i127b>>(%[[VALUE___atomic_load_tmp]])), read<i127b, atomic=seq_cst>(deref(read<ptr<atomic i127b>>(%[[VALUE___atomic_load_ptr]]))));
// DEFAULT-NEXT:             write<i127b>(%[[VALUE7]], read<i127b>(%[[VALUE___atomic_load_tmp]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%[[VALUE7]]), neg<i127b, overflow=ub>(const<i127b>(46947703095804269367567934653765590730)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_fetch_sub:[0-9]+]] @test_fetch_sub() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_2:[0-9]+]] __atomic_store_ptr: ptr<atomic i127b> [storage=automatic] = addr_of<ptr<atomic i127b>>(%[[VALUE_v]]);
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_2:[0-9]+]] __atomic_store_tmp: i127b [storage=automatic] = widen<i127b, reason=assign>(const<i126b>(30796781768365552851024605388374299173));
// DEFAULT-NEXT:             write<i127b, atomic=release>(deref(read<ptr<atomic i127b>>(%[[VALUE___atomic_store_ptr_2]])), read<i127b>(deref(addr_of<ptr<i127b>>(%[[VALUE___atomic_store_tmp_2]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<i127b>(%[[VALUE_count]], widen<i127b, reason=assign>(const<i126b>(32457177597484647488149720668185011722)));
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=relaxed>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), sub<i127b, overflow=wrap>(old<i127b>, read<i127b>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%[[VALUE8]]), widen<i127b, reason=usual_arith>(const<i126b>(30796781768365552851024605388374299173)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=consume>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), sub<i127b, overflow=wrap>(old<i127b>, const<i127b>(54614103079293459991417218347656369566)));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%[[VALUE9]]), widen<i127b, reason=usual_arith>(neg<i122b, overflow=ub>(const<i122b>(1660395829119094637125115279810712549))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=acquire>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), sub<i127b, overflow=wrap>(old<i127b>, read<i127b>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%[[VALUE10]]), neg<i127b, overflow=ub>(const<i127b>(56274498908412554628542333627467082115)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=release>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), sub<i127b, overflow=wrap>(old<i127b>, neg<i127b, overflow=ub>(const<i127b>(44514083923735151931107302009741400482))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%[[VALUE11]]), const<i127b>(81409506954572029614995249420232011891))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=acq_rel>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), sub<i127b, overflow=wrap>(old<i127b>, read<i127b>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%[[VALUE12]]), neg<i127b, overflow=ub>(const<i127b>(44217592582162050185584752285910693355)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), sub<i127b, overflow=wrap>(old<i127b>, widen<i127b, reason=arg>(const<i126b>(30348078982452392099140613411731040827))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%[[VALUE13]]), neg<i127b, overflow=ub>(const<i127b>(76674770179646697673734472954095705077)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE14:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), sub<i127b, overflow=wrap>(old<i127b>, neg<i127b, overflow=ub>(const<i127b>(82224045897086857020012824788652775087))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%[[VALUE14]]), const<i127b>(63118334298370141958812217350057359824))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE15:[0-9]+]]: i127b [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_load_ptr_2:[0-9]+]] __atomic_load_ptr: ptr<atomic i127b> [storage=automatic] = addr_of<ptr<atomic i127b>>(%[[VALUE_v]]);
// DEFAULT-NEXT:             let %[[VALUE___atomic_load_tmp_2:[0-9]+]] __atomic_load_tmp: i127b [storage=automatic];
// DEFAULT-NEXT:             write<i127b>(deref(addr_of<ptr<i127b>>(%[[VALUE___atomic_load_tmp_2]])), read<i127b, atomic=acquire>(deref(read<ptr<atomic i127b>>(%[[VALUE___atomic_load_ptr_2]]))));
// DEFAULT-NEXT:             write<i127b>(%[[VALUE15]], read<i127b>(%[[VALUE___atomic_load_tmp_2]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%[[VALUE15]]), widen<i127b, reason=usual_arith>(neg<i126b, overflow=ub>(const<i126b>(24798803265012232752862261577173970817))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_fetch_and:[0-9]+]] @test_fetch_and() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_3:[0-9]+]] __atomic_store_ptr: ptr<atomic i127b> [storage=automatic] = addr_of<ptr<atomic i127b>>(%[[VALUE_v]]);
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_3:[0-9]+]] __atomic_store_tmp: i127b [storage=automatic] = read<i127b>(%[[VALUE_init]]);
// DEFAULT-NEXT:             write<i127b, atomic=seq_cst>(deref(read<ptr<atomic i127b>>(%[[VALUE___atomic_store_ptr_3]])), read<i127b>(deref(addr_of<ptr<i127b>>(%[[VALUE___atomic_store_tmp_3]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %[[VALUE16:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=relaxed>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), and<i127b>(old<i127b>, widen<i127b, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%[[VALUE16]]), read<i127b>(%[[VALUE_init]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE17:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=consume>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), and<i127b>(old<i127b>, read<i127b>(%[[VALUE_init]])));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%[[VALUE17]]), widen<i127b, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE18:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=acquire>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), and<i127b>(old<i127b>, widen<i127b, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%[[VALUE18]]), widen<i127b, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i127b, atomic=seq_cst>(%[[VALUE_v]], not<i127b>(read<i127b, atomic=seq_cst>(%[[VALUE_v]])));
// DEFAULT-NEXT:         let %[[VALUE19:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=release>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), and<i127b>(old<i127b>, read<i127b>(%[[VALUE_init]])));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%[[VALUE19]]), read<i127b>(%[[VALUE_init]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE20:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=acq_rel>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), and<i127b>(old<i127b>, widen<i127b, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%[[VALUE20]]), read<i127b>(%[[VALUE_init]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE21:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), and<i127b>(old<i127b>, widen<i127b, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%[[VALUE21]]), widen<i127b, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE22:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), and<i127b>(old<i127b>, widen<i127b, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%[[VALUE22]]), widen<i127b, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_fetch_xor:[0-9]+]] @test_fetch_xor() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i127b, atomic=seq_cst>(%[[VALUE_v]], read<i127b>(%[[VALUE_init]]));
// DEFAULT-NEXT:         write<i127b>(%[[VALUE_count]], widen<i127b, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE23:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=relaxed>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), xor<i127b>(old<i127b>, read<i127b>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%[[VALUE23]]), read<i127b>(%[[VALUE_init]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE24:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=consume>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), xor<i127b>(old<i127b>, not<i127b>(read<i127b>(%[[VALUE_count]]))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%[[VALUE24]]), read<i127b>(%[[VALUE_init]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE25:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=acquire>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), xor<i127b>(old<i127b>, widen<i127b, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%[[VALUE25]]), widen<i127b, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE26:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=release>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), xor<i127b>(old<i127b>, not<i127b>(read<i127b>(%[[VALUE_count]]))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%[[VALUE26]]), widen<i127b, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE27:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=acq_rel>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), xor<i127b>(old<i127b>, widen<i127b, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%[[VALUE27]]), read<i127b>(%[[VALUE_init]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE28:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), xor<i127b>(old<i127b>, not<i127b>(read<i127b>(%[[VALUE_count]]))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%[[VALUE28]]), read<i127b>(%[[VALUE_init]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE29:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), xor<i127b>(old<i127b>, not<i127b>(read<i127b>(%[[VALUE_count]]))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%[[VALUE29]]), widen<i127b, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_fetch_or:[0-9]+]] @test_fetch_or() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i127b, atomic=seq_cst>(%[[VALUE_v]], widen<i127b, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         write<i127b>(%[[VALUE_count]], widen<i127b, reason=assign>(const<i46b>(17592186044416)));
// DEFAULT-NEXT:         let %[[VALUE30:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=relaxed>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), or<i127b>(old<i127b>, read<i127b>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%[[VALUE30]]), widen<i127b, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE31:[0-9]+]]: i127b [synthetic] = read<i127b>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE32:[0-9]+]]: i127b [synthetic] = mul<i127b, overflow=ub>(read<i127b>(%[[VALUE31]]), widen<i127b, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i127b>(%[[VALUE_count]], read<i127b>(%[[VALUE32]]));
// DEFAULT-NEXT:         let %[[VALUE33:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=consume>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), or<i127b>(old<i127b>, widen<i127b, reason=arg>(const<i47b>(35184372088832))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%[[VALUE33]]), widen<i127b, reason=usual_arith>(const<i46b>(17592186044416)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE34:[0-9]+]]: i127b [synthetic] = read<i127b>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE35:[0-9]+]]: i127b [synthetic] = mul<i127b, overflow=ub>(read<i127b>(%[[VALUE34]]), widen<i127b, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i127b>(%[[VALUE_count]], read<i127b>(%[[VALUE35]]));
// DEFAULT-NEXT:         let %[[VALUE36:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=acquire>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), or<i127b>(old<i127b>, read<i127b>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%[[VALUE36]]), widen<i127b, reason=usual_arith>(const<i47b>(52776558133248)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE37:[0-9]+]]: i127b [synthetic] = read<i127b>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE38:[0-9]+]]: i127b [synthetic] = mul<i127b, overflow=ub>(read<i127b>(%[[VALUE37]]), widen<i127b, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i127b>(%[[VALUE_count]], read<i127b>(%[[VALUE38]]));
// DEFAULT-NEXT:         let %[[VALUE39:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=release>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), or<i127b>(old<i127b>, widen<i127b, reason=arg>(const<i49b>(140737488355328))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%[[VALUE39]]), widen<i127b, reason=usual_arith>(const<i48b>(123145302310912)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE40:[0-9]+]]: i127b [synthetic] = read<i127b>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE41:[0-9]+]]: i127b [synthetic] = mul<i127b, overflow=ub>(read<i127b>(%[[VALUE40]]), widen<i127b, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i127b>(%[[VALUE_count]], read<i127b>(%[[VALUE41]]));
// DEFAULT-NEXT:         let %[[VALUE42:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=acq_rel>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), or<i127b>(old<i127b>, read<i127b>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%[[VALUE42]]), widen<i127b, reason=usual_arith>(const<i49b>(263882790666240)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE43:[0-9]+]]: i127b [synthetic] = read<i127b>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE44:[0-9]+]]: i127b [synthetic] = mul<i127b, overflow=ub>(read<i127b>(%[[VALUE43]]), widen<i127b, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i127b>(%[[VALUE_count]], read<i127b>(%[[VALUE44]]));
// DEFAULT-NEXT:         let %[[VALUE45:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), or<i127b>(old<i127b>, read<i127b>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%[[VALUE45]]), widen<i127b, reason=usual_arith>(const<i50b>(545357767376896)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE46:[0-9]+]]: i127b [synthetic] = read<i127b>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE47:[0-9]+]]: i127b [synthetic] = mul<i127b, overflow=ub>(read<i127b>(%[[VALUE46]]), widen<i127b, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i127b>(%[[VALUE_count]], read<i127b>(%[[VALUE47]]));
// DEFAULT-NEXT:         let %[[VALUE48:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), or<i127b>(old<i127b>, read<i127b>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%[[VALUE48]]), widen<i127b, reason=usual_arith>(const<i51b>(1108307720798208)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_add:[0-9]+]] @test_add() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i127b, atomic=seq_cst>(%[[VALUE_v]], widen<i127b, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         write<i127b>(%[[VALUE_count]], widen<i127b, reason=assign>(const<i74b>(4722366482869645213696)));
// DEFAULT-NEXT:         let %[[VALUE49:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), add<i127b, overflow=wrap>(old<i127b>, read<i127b>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%[[VALUE_v]]), widen<i127b, reason=usual_arith>(const<i74b>(4722366482869645213696)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE50:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=consume>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), add<i127b, overflow=wrap>(old<i127b>, read<i127b>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%[[VALUE_v]]), widen<i127b, reason=usual_arith>(const<i75b>(9444732965739290427392)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE51:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), add<i127b, overflow=wrap>(old<i127b>, widen<i127b, reason=arg>(const<i74b>(4722366482869645213696))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%[[VALUE_v]]), widen<i127b, reason=usual_arith>(const<i75b>(14167099448608935641088)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE52:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=release>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), add<i127b, overflow=wrap>(old<i127b>, widen<i127b, reason=arg>(const<i74b>(4722366482869645213696))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%[[VALUE_v]]), widen<i127b, reason=usual_arith>(const<i76b>(18889465931478580854784)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE53:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), add<i127b, overflow=wrap>(old<i127b>, widen<i127b, reason=arg>(const<i74b>(4722366482869645213696))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%[[VALUE_v]]), widen<i127b, reason=usual_arith>(const<i76b>(23611832414348226068480)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE54:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), add<i127b, overflow=wrap>(old<i127b>, read<i127b>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%[[VALUE_v]]), widen<i127b, reason=usual_arith>(const<i76b>(28334198897217871282176)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_sub:[0-9]+]] @test_sub() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE55:[0-9]+]]: i127b [synthetic] = widen<i127b, reason=assign>(neg<i123b, overflow=ub>(const<i123b>(3638804536836293398783417724445294828)));
// DEFAULT-NEXT:         write<i127b>(%[[VALUE_res]], read<i127b>(%[[VALUE55]]));
// DEFAULT-NEXT:         write<i127b, atomic=seq_cst>(%[[VALUE_v]], read<i127b>(%[[VALUE55]]));
// DEFAULT-NEXT:         write<i127b>(%[[VALUE_count]], widen<i127b, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE56:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), sub<i127b, overflow=wrap>(old<i127b>, add<i127b, overflow=ub>(read<i127b>(%[[VALUE_count]]), widen<i127b, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         let %[[VALUE57:[0-9]+]]: i127b [synthetic] = read<i127b>(%[[VALUE_res]]);
// DEFAULT-NEXT:         let %[[VALUE58:[0-9]+]]: i127b [synthetic] = sub<i127b, overflow=ub>(read<i127b>(%[[VALUE57]]), widen<i127b, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i127b>(%[[VALUE_res]], read<i127b>(%[[VALUE58]]));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%[[VALUE_v]]), read<i127b>(%[[VALUE58]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE59:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=consume>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), sub<i127b, overflow=wrap>(old<i127b>, add<i127b, overflow=ub>(read<i127b>(%[[VALUE_count]]), widen<i127b, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         let %[[VALUE60:[0-9]+]]: i127b [synthetic] = read<i127b>(%[[VALUE_res]]);
// DEFAULT-NEXT:         let %[[VALUE61:[0-9]+]]: i127b [synthetic] = sub<i127b, overflow=ub>(read<i127b>(%[[VALUE60]]), widen<i127b, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i127b>(%[[VALUE_res]], read<i127b>(%[[VALUE61]]));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%[[VALUE_v]]), read<i127b>(%[[VALUE61]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE62:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), sub<i127b, overflow=wrap>(old<i127b>, widen<i127b, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE63:[0-9]+]]: i127b [synthetic] = read<i127b>(%[[VALUE_res]]);
// DEFAULT-NEXT:         let %[[VALUE64:[0-9]+]]: i127b [synthetic] = sub<i127b, overflow=ub>(read<i127b>(%[[VALUE63]]), widen<i127b, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i127b>(%[[VALUE_res]], read<i127b>(%[[VALUE64]]));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%[[VALUE_v]]), read<i127b>(%[[VALUE64]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE65:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=release>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), sub<i127b, overflow=wrap>(old<i127b>, widen<i127b, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE66:[0-9]+]]: i127b [synthetic] = read<i127b>(%[[VALUE_res]]);
// DEFAULT-NEXT:         let %[[VALUE67:[0-9]+]]: i127b [synthetic] = sub<i127b, overflow=ub>(read<i127b>(%[[VALUE66]]), widen<i127b, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i127b>(%[[VALUE_res]], read<i127b>(%[[VALUE67]]));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%[[VALUE_v]]), read<i127b>(%[[VALUE67]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE68:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), sub<i127b, overflow=wrap>(old<i127b>, add<i127b, overflow=ub>(read<i127b>(%[[VALUE_count]]), widen<i127b, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         let %[[VALUE69:[0-9]+]]: i127b [synthetic] = read<i127b>(%[[VALUE_res]]);
// DEFAULT-NEXT:         let %[[VALUE70:[0-9]+]]: i127b [synthetic] = sub<i127b, overflow=ub>(read<i127b>(%[[VALUE69]]), widen<i127b, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i127b>(%[[VALUE_res]], read<i127b>(%[[VALUE70]]));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%[[VALUE_v]]), read<i127b>(%[[VALUE70]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE71:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), sub<i127b, overflow=wrap>(old<i127b>, add<i127b, overflow=ub>(read<i127b>(%[[VALUE_count]]), widen<i127b, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         let %[[VALUE72:[0-9]+]]: i127b [synthetic] = read<i127b>(%[[VALUE_res]]);
// DEFAULT-NEXT:         let %[[VALUE73:[0-9]+]]: i127b [synthetic] = sub<i127b, overflow=ub>(read<i127b>(%[[VALUE72]]), widen<i127b, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i127b>(%[[VALUE_res]], read<i127b>(%[[VALUE73]]));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%[[VALUE_v]]), read<i127b>(%[[VALUE73]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_and:[0-9]+]] @test_and() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i127b, atomic=seq_cst>(%[[VALUE_v]], read<i127b>(%[[VALUE_init]]));
// DEFAULT-NEXT:         let %[[VALUE74:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), and<i127b>(old<i127b>, widen<i127b, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%[[VALUE_v]]), widen<i127b, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i127b, atomic=seq_cst>(%[[VALUE_v]], read<i127b>(%[[VALUE_init]]));
// DEFAULT-NEXT:         let %[[VALUE75:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=consume>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), and<i127b>(old<i127b>, read<i127b>(%[[VALUE_init]])));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%[[VALUE_v]]), read<i127b>(%[[VALUE_init]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE76:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), and<i127b>(old<i127b>, widen<i127b, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%[[VALUE_v]]), widen<i127b, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i127b, atomic=seq_cst>(%[[VALUE_v]], not<i127b>(read<i127b, atomic=seq_cst>(%[[VALUE_v]])));
// DEFAULT-NEXT:         let %[[VALUE77:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=release>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), and<i127b>(old<i127b>, read<i127b>(%[[VALUE_init]])));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%[[VALUE_v]]), read<i127b>(%[[VALUE_init]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE78:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), and<i127b>(old<i127b>, widen<i127b, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%[[VALUE_v]]), widen<i127b, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i127b, atomic=seq_cst>(%[[VALUE_v]], not<i127b>(read<i127b, atomic=seq_cst>(%[[VALUE_v]])));
// DEFAULT-NEXT:         let %[[VALUE79:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), and<i127b>(old<i127b>, widen<i127b, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%[[VALUE_v]]), widen<i127b, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_xor:[0-9]+]] @test_xor() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i127b, atomic=seq_cst>(%[[VALUE_v]], read<i127b>(%[[VALUE_init]]));
// DEFAULT-NEXT:         write<i127b>(%[[VALUE_count]], widen<i127b, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE80:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), xor<i127b>(old<i127b>, read<i127b>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%[[VALUE_v]]), read<i127b>(%[[VALUE_init]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE81:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=consume>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), xor<i127b>(old<i127b>, not<i127b>(read<i127b>(%[[VALUE_count]]))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%[[VALUE_v]]), widen<i127b, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE82:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), xor<i127b>(old<i127b>, widen<i127b, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%[[VALUE_v]]), widen<i127b, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE83:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=release>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), xor<i127b>(old<i127b>, not<i127b>(read<i127b>(%[[VALUE_count]]))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%[[VALUE_v]]), read<i127b>(%[[VALUE_init]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE84:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=acq_rel>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), xor<i127b>(old<i127b>, widen<i127b, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%[[VALUE_v]]), read<i127b>(%[[VALUE_init]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE85:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), xor<i127b>(old<i127b>, not<i127b>(read<i127b>(%[[VALUE_count]]))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%[[VALUE_v]]), widen<i127b, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_or:[0-9]+]] @test_or() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i127b, atomic=seq_cst>(%[[VALUE_v]], widen<i127b, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         write<i127b>(%[[VALUE_count]], widen<i127b, reason=assign>(const<i86b>(19342813113834066795298816)));
// DEFAULT-NEXT:         let %[[VALUE86:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), or<i127b>(old<i127b>, read<i127b>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%[[VALUE_v]]), widen<i127b, reason=usual_arith>(const<i86b>(19342813113834066795298816)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE87:[0-9]+]]: i127b [synthetic] = read<i127b>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE88:[0-9]+]]: i127b [synthetic] = mul<i127b, overflow=ub>(read<i127b>(%[[VALUE87]]), widen<i127b, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i127b>(%[[VALUE_count]], read<i127b>(%[[VALUE88]]));
// DEFAULT-NEXT:         let %[[VALUE89:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=consume>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), or<i127b>(old<i127b>, read<i127b>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%[[VALUE_v]]), widen<i127b, reason=usual_arith>(const<i87b>(58028439341502200385896448)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE90:[0-9]+]]: i127b [synthetic] = read<i127b>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE91:[0-9]+]]: i127b [synthetic] = mul<i127b, overflow=ub>(read<i127b>(%[[VALUE90]]), widen<i127b, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i127b>(%[[VALUE_count]], read<i127b>(%[[VALUE91]]));
// DEFAULT-NEXT:         let %[[VALUE92:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), or<i127b>(old<i127b>, widen<i127b, reason=arg>(const<i88b>(77371252455336267181195264))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%[[VALUE_v]]), widen<i127b, reason=usual_arith>(const<i88b>(135399691796838467567091712)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE93:[0-9]+]]: i127b [synthetic] = read<i127b>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE94:[0-9]+]]: i127b [synthetic] = mul<i127b, overflow=ub>(read<i127b>(%[[VALUE93]]), widen<i127b, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i127b>(%[[VALUE_count]], read<i127b>(%[[VALUE94]]));
// DEFAULT-NEXT:         let %[[VALUE95:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=release>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), or<i127b>(old<i127b>, widen<i127b, reason=arg>(const<i89b>(154742504910672534362390528))));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%[[VALUE_v]]), widen<i127b, reason=usual_arith>(const<i89b>(290142196707511001929482240)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE96:[0-9]+]]: i127b [synthetic] = read<i127b>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE97:[0-9]+]]: i127b [synthetic] = mul<i127b, overflow=ub>(read<i127b>(%[[VALUE96]]), widen<i127b, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i127b>(%[[VALUE_count]], read<i127b>(%[[VALUE97]]));
// DEFAULT-NEXT:         let %[[VALUE98:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), or<i127b>(old<i127b>, read<i127b>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%[[VALUE_v]]), widen<i127b, reason=usual_arith>(const<i90b>(599627206528856070654263296)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE99:[0-9]+]]: i127b [synthetic] = read<i127b>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE100:[0-9]+]]: i127b [synthetic] = mul<i127b, overflow=ub>(read<i127b>(%[[VALUE99]]), widen<i127b, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i127b>(%[[VALUE_count]], read<i127b>(%[[VALUE100]]));
// DEFAULT-NEXT:         let %[[VALUE101:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i127b>>(%[[VALUE_v]])), or<i127b>(old<i127b>, read<i127b>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i127b>(read<i127b, atomic=seq_cst>(%[[VALUE_v]]), widen<i127b, reason=usual_arith>(const<i91b>(1218597226171546208103825408)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_exchange:[0-9]+]] @test_exchange() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_4:[0-9]+]] __atomic_store_ptr: ptr<atomic i127b> [storage=automatic] = addr_of<ptr<atomic i127b>>(%[[VALUE_v]]);
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_4:[0-9]+]] __atomic_store_tmp: i127b [storage=automatic] = widen<i127b, reason=assign>(const<i125b>(15794812138349191682564933935017390008));
// DEFAULT-NEXT:             write<i127b, atomic=seq_cst>(deref(read<ptr<atomic i127b>>(%[[VALUE___atomic_store_ptr_4]])), read<i127b>(deref(addr_of<ptr<i127b>>(%[[VALUE___atomic_store_tmp_4]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %[[VALUE102:[0-9]+]]: i127b [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_exchange_ptr:[0-9]+]] __atomic_exchange_ptr: ptr<atomic i127b> [storage=automatic] = addr_of<ptr<atomic i127b>>(%[[VALUE_v]]);
// DEFAULT-NEXT:             let %[[VALUE___atomic_exchange_val:[0-9]+]] __atomic_exchange_val: i127b [storage=automatic] = widen<i127b, reason=assign>(neg<i122b, overflow=ub>(const<i122b>(2166613183393424891717146518563613668)));
// DEFAULT-NEXT:             let %[[VALUE___atomic_exchange_tmp:[0-9]+]] __atomic_exchange_tmp: i127b [storage=automatic];
// DEFAULT-NEXT:             let %[[VALUE103:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=seq_cst>(deref(read<ptr<atomic i127b>>(%[[VALUE___atomic_exchange_ptr]])), read<i127b>(deref(addr_of<ptr<i127b>>(%[[VALUE___atomic_exchange_val]]))));
// DEFAULT-NEXT:             write<i127b>(deref(addr_of<ptr<i127b>>(%[[VALUE___atomic_exchange_tmp]])), read<i127b>(%[[VALUE103]]));
// DEFAULT-NEXT:             write<i127b>(%[[VALUE102]], read<i127b>(%[[VALUE___atomic_exchange_tmp]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %[[VALUE104:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%[[VALUE102]]), widen<i127b, reason=usual_arith>(const<i125b>(15794812138349191682564933935017390008)))
// DEFAULT-NEXT:             write<bool>(%[[VALUE104]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %[[VALUE105:[0-9]+]]: i127b [synthetic];
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___atomic_load_ptr_3:[0-9]+]] __atomic_load_ptr: ptr<atomic i127b> [storage=automatic] = addr_of<ptr<atomic i127b>>(%[[VALUE_v]]);
// DEFAULT-NEXT:                 let %[[VALUE___atomic_load_tmp_3:[0-9]+]] __atomic_load_tmp: i127b [storage=automatic];
// DEFAULT-NEXT:                 write<i127b>(deref(addr_of<ptr<i127b>>(%[[VALUE___atomic_load_tmp_3]])), read<i127b, atomic=seq_cst>(deref(read<ptr<atomic i127b>>(%[[VALUE___atomic_load_ptr_3]]))));
// DEFAULT-NEXT:                 write<i127b>(%[[VALUE105]], read<i127b>(%[[VALUE___atomic_load_tmp_3]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             write<bool>(%[[VALUE104]], ne<i127b>(read<i127b>(%[[VALUE105]]), widen<i127b, reason=usual_arith>(neg<i122b, overflow=ub>(const<i122b>(2166613183393424891717146518563613668)))));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE104]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE106:[0-9]+]]: i127b [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_exchange_ptr_2:[0-9]+]] __atomic_exchange_ptr: ptr<atomic i127b> [storage=automatic] = addr_of<ptr<atomic i127b>>(%[[VALUE_v]]);
// DEFAULT-NEXT:             let %[[VALUE___atomic_exchange_val_2:[0-9]+]] __atomic_exchange_val: i127b [storage=automatic] = const<i127b>(61251098386268815852902382804483910638);
// DEFAULT-NEXT:             let %[[VALUE___atomic_exchange_tmp_2:[0-9]+]] __atomic_exchange_tmp: i127b [storage=automatic];
// DEFAULT-NEXT:             let %[[VALUE107:[0-9]+]]: i127b [synthetic] = update<i127b, result=old, atomic=relaxed>(deref(read<ptr<atomic i127b>>(%[[VALUE___atomic_exchange_ptr_2]])), read<i127b>(deref(addr_of<ptr<i127b>>(%[[VALUE___atomic_exchange_val_2]]))));
// DEFAULT-NEXT:             write<i127b>(deref(addr_of<ptr<i127b>>(%[[VALUE___atomic_exchange_tmp_2]])), read<i127b>(%[[VALUE107]]));
// DEFAULT-NEXT:             write<i127b>(%[[VALUE106]], read<i127b>(%[[VALUE___atomic_exchange_tmp_2]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %[[VALUE108:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%[[VALUE106]]), widen<i127b, reason=usual_arith>(neg<i122b, overflow=ub>(const<i122b>(2166613183393424891717146518563613668))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE108]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %[[VALUE109:[0-9]+]]: i127b [synthetic];
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___atomic_load_ptr_4:[0-9]+]] __atomic_load_ptr: ptr<atomic i127b> [storage=automatic] = addr_of<ptr<atomic i127b>>(%[[VALUE_v]]);
// DEFAULT-NEXT:                 let %[[VALUE___atomic_load_tmp_4:[0-9]+]] __atomic_load_tmp: i127b [storage=automatic];
// DEFAULT-NEXT:                 write<i127b>(deref(addr_of<ptr<i127b>>(%[[VALUE___atomic_load_tmp_4]])), read<i127b, atomic=acquire>(deref(read<ptr<atomic i127b>>(%[[VALUE___atomic_load_ptr_4]]))));
// DEFAULT-NEXT:                 write<i127b>(%[[VALUE109]], read<i127b>(%[[VALUE___atomic_load_tmp_4]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             write<bool>(%[[VALUE108]], ne<i127b>(read<i127b>(%[[VALUE109]]), const<i127b>(61251098386268815852902382804483910638)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE108]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i127b>(%[[VALUE_count]], widen<i127b, reason=assign>(neg<i122b, overflow=ub>(const<i122b>(2166613183393424891717146518563613668))));
// DEFAULT-NEXT:         let %[[VALUE110:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_compare_exchange_ptr:[0-9]+]] __atomic_compare_exchange_ptr: ptr<atomic i127b> [storage=automatic] = addr_of<ptr<atomic i127b>>(%[[VALUE_v]]);
// DEFAULT-NEXT:             let %[[VALUE___atomic_compare_exchange_tmp:[0-9]+]] __atomic_compare_exchange_tmp: i127b [storage=automatic] = widen<i127b, reason=assign>(neg<i126b, overflow=ub>(const<i126b>(36677332297536901313774263310237646448)));
// DEFAULT-NEXT:             let %[[VALUE111:[0-9]+]]: bool [synthetic] = compare_exchange<i127b, form=write_back, weak=false, success=seq_cst, failure=seq_cst>(deref(read<ptr<atomic i127b>>(%[[VALUE___atomic_compare_exchange_ptr]])), addr_of<ptr<i127b>>(%[[VALUE_count]]), read<i127b>(deref(addr_of<ptr<i127b>>(%[[VALUE___atomic_compare_exchange_tmp]]))));
// DEFAULT-NEXT:             write<bool>(%[[VALUE110]], read<bool>(%[[VALUE111]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if read<bool>(%[[VALUE110]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE112:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%[[VALUE_count]]), const<i127b>(61251098386268815852902382804483910638))
// DEFAULT-NEXT:             write<bool>(%[[VALUE112]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %[[VALUE113:[0-9]+]]: i127b [synthetic];
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___atomic_load_ptr_5:[0-9]+]] __atomic_load_ptr: ptr<atomic i127b> [storage=automatic] = addr_of<ptr<atomic i127b>>(%[[VALUE_v]]);
// DEFAULT-NEXT:                 let %[[VALUE___atomic_load_tmp_5:[0-9]+]] __atomic_load_tmp: i127b [storage=automatic];
// DEFAULT-NEXT:                 write<i127b>(deref(addr_of<ptr<i127b>>(%[[VALUE___atomic_load_tmp_5]])), read<i127b, atomic=seq_cst>(deref(read<ptr<atomic i127b>>(%[[VALUE___atomic_load_ptr_5]]))));
// DEFAULT-NEXT:                 write<i127b>(%[[VALUE113]], read<i127b>(%[[VALUE___atomic_load_tmp_5]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             write<bool>(%[[VALUE112]], ne<i127b>(read<i127b>(%[[VALUE113]]), const<i127b>(61251098386268815852902382804483910638)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE112]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE114:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_compare_exchange_ptr_2:[0-9]+]] __atomic_compare_exchange_ptr: ptr<atomic i127b> [storage=automatic] = addr_of<ptr<atomic i127b>>(%[[VALUE_v]]);
// DEFAULT-NEXT:             let %[[VALUE___atomic_compare_exchange_tmp_2:[0-9]+]] __atomic_compare_exchange_tmp: i127b [storage=automatic] = widen<i127b, reason=assign>(neg<i126b, overflow=ub>(const<i126b>(36677332297536901313774263310237646448)));
// DEFAULT-NEXT:             let %[[VALUE115:[0-9]+]]: bool [synthetic] = compare_exchange<i127b, form=write_back, weak=false, success=seq_cst, failure=seq_cst>(deref(read<ptr<atomic i127b>>(%[[VALUE___atomic_compare_exchange_ptr_2]])), addr_of<ptr<i127b>>(%[[VALUE_count]]), read<i127b>(deref(addr_of<ptr<i127b>>(%[[VALUE___atomic_compare_exchange_tmp_2]]))));
// DEFAULT-NEXT:             write<bool>(%[[VALUE114]], read<bool>(%[[VALUE115]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if not<bool>(read<bool>(%[[VALUE114]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE116:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%[[VALUE_count]]), const<i127b>(61251098386268815852902382804483910638))
// DEFAULT-NEXT:             write<bool>(%[[VALUE116]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %[[VALUE117:[0-9]+]]: i127b [synthetic];
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___atomic_load_ptr_6:[0-9]+]] __atomic_load_ptr: ptr<atomic i127b> [storage=automatic] = addr_of<ptr<atomic i127b>>(%[[VALUE_v]]);
// DEFAULT-NEXT:                 let %[[VALUE___atomic_load_tmp_6:[0-9]+]] __atomic_load_tmp: i127b [storage=automatic];
// DEFAULT-NEXT:                 write<i127b>(deref(addr_of<ptr<i127b>>(%[[VALUE___atomic_load_tmp_6]])), read<i127b, atomic=seq_cst>(deref(read<ptr<atomic i127b>>(%[[VALUE___atomic_load_ptr_6]]))));
// DEFAULT-NEXT:                 write<i127b>(%[[VALUE117]], read<i127b>(%[[VALUE___atomic_load_tmp_6]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             write<bool>(%[[VALUE116]], ne<i127b>(read<i127b>(%[[VALUE117]]), widen<i127b, reason=usual_arith>(neg<i126b, overflow=ub>(const<i126b>(36677332297536901313774263310237646448)))));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE116]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i127b>(%[[VALUE_count]], widen<i127b, reason=assign>(neg<i122b, overflow=ub>(const<i122b>(2166613183393424891717146518563613668))));
// DEFAULT-NEXT:         let %[[VALUE118:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_compare_exchange_ptr_3:[0-9]+]] __atomic_compare_exchange_ptr: ptr<atomic i127b> [storage=automatic] = addr_of<ptr<atomic i127b>>(%[[VALUE_v]]);
// DEFAULT-NEXT:             let %[[VALUE___atomic_compare_exchange_tmp_3:[0-9]+]] __atomic_compare_exchange_tmp: i127b [storage=automatic] = const<i127b>(73949932022761409003352953944661689416);
// DEFAULT-NEXT:             let %[[VALUE119:[0-9]+]]: bool [synthetic] = compare_exchange<i127b, form=write_back, weak=false, success=seq_cst, failure=relaxed>(deref(read<ptr<atomic i127b>>(%[[VALUE___atomic_compare_exchange_ptr_3]])), addr_of<ptr<i127b>>(%[[VALUE_count]]), read<i127b>(deref(addr_of<ptr<i127b>>(%[[VALUE___atomic_compare_exchange_tmp_3]]))));
// DEFAULT-NEXT:             write<bool>(%[[VALUE118]], read<bool>(%[[VALUE119]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if read<bool>(%[[VALUE118]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE120:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%[[VALUE_count]]), widen<i127b, reason=usual_arith>(neg<i126b, overflow=ub>(const<i126b>(36677332297536901313774263310237646448))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE120]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %[[VALUE121:[0-9]+]]: i127b [synthetic];
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___atomic_load_ptr_7:[0-9]+]] __atomic_load_ptr: ptr<atomic i127b> [storage=automatic] = addr_of<ptr<atomic i127b>>(%[[VALUE_v]]);
// DEFAULT-NEXT:                 let %[[VALUE___atomic_load_tmp_7:[0-9]+]] __atomic_load_tmp: i127b [storage=automatic];
// DEFAULT-NEXT:                 write<i127b>(deref(addr_of<ptr<i127b>>(%[[VALUE___atomic_load_tmp_7]])), read<i127b, atomic=seq_cst>(deref(read<ptr<atomic i127b>>(%[[VALUE___atomic_load_ptr_7]]))));
// DEFAULT-NEXT:                 write<i127b>(%[[VALUE121]], read<i127b>(%[[VALUE___atomic_load_tmp_7]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             write<bool>(%[[VALUE120]], ne<i127b>(read<i127b>(%[[VALUE121]]), widen<i127b, reason=usual_arith>(neg<i126b, overflow=ub>(const<i126b>(36677332297536901313774263310237646448)))));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE120]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE122:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_compare_exchange_ptr_4:[0-9]+]] __atomic_compare_exchange_ptr: ptr<atomic i127b> [storage=automatic] = addr_of<ptr<atomic i127b>>(%[[VALUE_v]]);
// DEFAULT-NEXT:             let %[[VALUE___atomic_compare_exchange_tmp_4:[0-9]+]] __atomic_compare_exchange_tmp: i127b [storage=automatic] = const<i127b>(73949932022761409003352953944661689416);
// DEFAULT-NEXT:             let %[[VALUE123:[0-9]+]]: bool [synthetic] = compare_exchange<i127b, form=write_back, weak=false, success=seq_cst, failure=seq_cst>(deref(read<ptr<atomic i127b>>(%[[VALUE___atomic_compare_exchange_ptr_4]])), addr_of<ptr<i127b>>(%[[VALUE_count]]), read<i127b>(deref(addr_of<ptr<i127b>>(%[[VALUE___atomic_compare_exchange_tmp_4]]))));
// DEFAULT-NEXT:             write<bool>(%[[VALUE122]], read<bool>(%[[VALUE123]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if not<bool>(read<bool>(%[[VALUE122]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE124:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%[[VALUE_count]]), widen<i127b, reason=usual_arith>(neg<i126b, overflow=ub>(const<i126b>(36677332297536901313774263310237646448))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE124]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %[[VALUE125:[0-9]+]]: i127b [synthetic];
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___atomic_load_ptr_8:[0-9]+]] __atomic_load_ptr: ptr<atomic i127b> [storage=automatic] = addr_of<ptr<atomic i127b>>(%[[VALUE_v]]);
// DEFAULT-NEXT:                 let %[[VALUE___atomic_load_tmp_8:[0-9]+]] __atomic_load_tmp: i127b [storage=automatic];
// DEFAULT-NEXT:                 write<i127b>(deref(addr_of<ptr<i127b>>(%[[VALUE___atomic_load_tmp_8]])), read<i127b, atomic=seq_cst>(deref(read<ptr<atomic i127b>>(%[[VALUE___atomic_load_ptr_8]]))));
// DEFAULT-NEXT:                 write<i127b>(%[[VALUE125]], read<i127b>(%[[VALUE___atomic_load_tmp_8]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             write<bool>(%[[VALUE124]], ne<i127b>(read<i127b>(%[[VALUE125]]), const<i127b>(73949932022761409003352953944661689416)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE124]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE126:[0-9]+]]: i127b [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_load_ptr_9:[0-9]+]] __atomic_load_ptr: ptr<atomic i127b> [storage=automatic] = addr_of<ptr<atomic i127b>>(%[[VALUE_v]]);
// DEFAULT-NEXT:             let %[[VALUE___atomic_load_tmp_9:[0-9]+]] __atomic_load_tmp: i127b [storage=automatic];
// DEFAULT-NEXT:             write<i127b>(deref(addr_of<ptr<i127b>>(%[[VALUE___atomic_load_tmp_9]])), read<i127b, atomic=seq_cst>(deref(read<ptr<atomic i127b>>(%[[VALUE___atomic_load_ptr_9]]))));
// DEFAULT-NEXT:             write<i127b>(%[[VALUE126]], read<i127b>(%[[VALUE___atomic_load_tmp_9]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<i127b>(%[[VALUE_count]], read<i127b>(%[[VALUE126]]));
// DEFAULT-NEXT:         do %[[VALUE127:[0-9]+]]
// DEFAULT-NEXT:             write<i127b>(%[[VALUE_res]], add<i127b, overflow=ub>(read<i127b>(%[[VALUE_count]]), neg<i127b, overflow=ub>(const<i127b>(82256758205518164043596305502815392646))));
// DEFAULT-NEXT:         while {
// DEFAULT-NEXT:             let %[[VALUE128:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___atomic_compare_exchange_ptr_5:[0-9]+]] __atomic_compare_exchange_ptr: ptr<atomic i127b> [storage=automatic] = addr_of<ptr<atomic i127b>>(%[[VALUE_v]]);
// DEFAULT-NEXT:                 let %[[VALUE___atomic_compare_exchange_tmp_5:[0-9]+]] __atomic_compare_exchange_tmp: i127b [storage=automatic] = read<i127b>(%[[VALUE_res]]);
// DEFAULT-NEXT:                 let %[[VALUE129:[0-9]+]]: bool [synthetic] = compare_exchange<i127b, form=write_back, weak=true, success=seq_cst, failure=seq_cst>(deref(read<ptr<atomic i127b>>(%[[VALUE___atomic_compare_exchange_ptr_5]])), addr_of<ptr<i127b>>(%[[VALUE_count]]), read<i127b>(deref(addr_of<ptr<i127b>>(%[[VALUE___atomic_compare_exchange_tmp_5]]))));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE128]], read<bool>(%[[VALUE129]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             yield not<bool>(read<bool>(%[[VALUE128]]));
// DEFAULT-NEXT:         };
// DEFAULT-NEXT:         let %[[VALUE130:[0-9]+]]: i127b [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_load_ptr_10:[0-9]+]] __atomic_load_ptr: ptr<atomic i127b> [storage=automatic] = addr_of<ptr<atomic i127b>>(%[[VALUE_v]]);
// DEFAULT-NEXT:             let %[[VALUE___atomic_load_tmp_10:[0-9]+]] __atomic_load_tmp: i127b [storage=automatic];
// DEFAULT-NEXT:             write<i127b>(deref(addr_of<ptr<i127b>>(%[[VALUE___atomic_load_tmp_10]])), read<i127b, atomic=seq_cst>(deref(read<ptr<atomic i127b>>(%[[VALUE___atomic_load_ptr_10]]))));
// DEFAULT-NEXT:             write<i127b>(%[[VALUE130]], read<i127b>(%[[VALUE___atomic_load_tmp_10]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%[[VALUE130]]), widen<i127b, reason=usual_arith>(neg<i124b, overflow=ub>(const<i124b>(8306826182756755040243351558153703230))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE131:[0-9]+]]: i127b [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_load_ptr_11:[0-9]+]] __atomic_load_ptr: ptr<atomic i127b> [storage=automatic] = addr_of<ptr<atomic i127b>>(%[[VALUE_v]]);
// DEFAULT-NEXT:             let %[[VALUE___atomic_load_tmp_11:[0-9]+]] __atomic_load_tmp: i127b [storage=automatic];
// DEFAULT-NEXT:             write<i127b>(deref(addr_of<ptr<i127b>>(%[[VALUE___atomic_load_tmp_11]])), read<i127b, atomic=acquire>(deref(read<ptr<atomic i127b>>(%[[VALUE___atomic_load_ptr_11]]))));
// DEFAULT-NEXT:             write<i127b>(%[[VALUE131]], read<i127b>(%[[VALUE___atomic_load_tmp_11]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<i127b>(%[[VALUE_count]], read<i127b>(%[[VALUE131]]));
// DEFAULT-NEXT:         do %[[VALUE132:[0-9]+]]
// DEFAULT-NEXT:             write<i127b>(%[[VALUE_res]], add<i127b, overflow=ub>(read<i127b>(%[[VALUE_count]]), const<i127b>(48855144829609538366772317026461909818)));
// DEFAULT-NEXT:         while {
// DEFAULT-NEXT:             let %[[VALUE133:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___atomic_compare_exchange_ptr_6:[0-9]+]] __atomic_compare_exchange_ptr: ptr<atomic i127b> [storage=automatic] = addr_of<ptr<atomic i127b>>(%[[VALUE_v]]);
// DEFAULT-NEXT:                 let %[[VALUE___atomic_compare_exchange_tmp_6:[0-9]+]] __atomic_compare_exchange_tmp: i127b [storage=automatic] = read<i127b>(%[[VALUE_res]]);
// DEFAULT-NEXT:                 let %[[VALUE134:[0-9]+]]: bool [synthetic] = compare_exchange<i127b, form=write_back, weak=true, success=relaxed, failure=relaxed>(deref(read<ptr<atomic i127b>>(%[[VALUE___atomic_compare_exchange_ptr_6]])), addr_of<ptr<i127b>>(%[[VALUE_count]]), read<i127b>(deref(addr_of<ptr<i127b>>(%[[VALUE___atomic_compare_exchange_tmp_6]]))));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE133]], read<bool>(%[[VALUE134]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             yield not<bool>(read<bool>(%[[VALUE133]]));
// DEFAULT-NEXT:         };
// DEFAULT-NEXT:         let %[[VALUE135:[0-9]+]]: i127b [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_load_ptr_12:[0-9]+]] __atomic_load_ptr: ptr<atomic i127b> [storage=automatic] = addr_of<ptr<atomic i127b>>(%[[VALUE_v]]);
// DEFAULT-NEXT:             let %[[VALUE___atomic_load_tmp_12:[0-9]+]] __atomic_load_tmp: i127b [storage=automatic];
// DEFAULT-NEXT:             write<i127b>(deref(addr_of<ptr<i127b>>(%[[VALUE___atomic_load_tmp_12]])), read<i127b, atomic=seq_cst>(deref(read<ptr<atomic i127b>>(%[[VALUE___atomic_load_ptr_12]]))));
// DEFAULT-NEXT:             write<i127b>(%[[VALUE135]], read<i127b>(%[[VALUE___atomic_load_tmp_12]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if ne<i127b>(read<i127b>(%[[VALUE135]]), widen<i127b, reason=usual_arith>(const<i126b>(40548318646852783326528965468308206588)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test_fetch_add]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test_fetch_sub]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test_fetch_and]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test_fetch_xor]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test_fetch_or]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test_add]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test_sub]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test_and]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test_xor]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test_or]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test_exchange]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
