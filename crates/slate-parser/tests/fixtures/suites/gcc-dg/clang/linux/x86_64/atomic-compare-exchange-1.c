/* Test __atomic routines for existence and proper execution on 1 byte 
   values with each valid memory model.  */
/* { dg-do run } */
/* { dg-require-effective-target sync_char_short } */

/* Test the execution of the __atomic_compare_exchange_n builtin for a char.  */

extern void abort(void);

char v = 0;
char expected = 0;
char max = ~0;
char desired = ~0;
char zero = 0;

#define STRONG 0
#define WEAK 1

int
main ()
{

  if (!__atomic_compare_exchange_n (&v, &expected, max, STRONG , __ATOMIC_RELAXED, __ATOMIC_RELAXED)) 
    abort ();
  if (expected != 0)
    abort ();

  if (__atomic_compare_exchange_n (&v, &expected, 0, STRONG , __ATOMIC_ACQUIRE, __ATOMIC_RELAXED)) 
    abort ();
  if (expected != max)
    abort ();

  if (!__atomic_compare_exchange_n (&v, &expected, 0, STRONG , __ATOMIC_RELEASE, __ATOMIC_ACQUIRE)) 
    abort ();
  if (expected != max)
    abort ();
  if (v != 0)
    abort ();

  if (__atomic_compare_exchange_n (&v, &expected, desired, WEAK, __ATOMIC_ACQ_REL, __ATOMIC_ACQUIRE)) 
    abort ();
  if (expected != 0)
    abort ();

  if (!__atomic_compare_exchange_n (&v, &expected, desired, STRONG , __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)) 
    abort ();
  if (expected != 0)
    abort ();
  if (v != max)
    abort ();

  /* Now test the generic version.  */

  v = 0;

  if (!__atomic_compare_exchange (&v, &expected, &max, STRONG, __ATOMIC_RELAXED, __ATOMIC_RELAXED))
    abort ();
  if (expected != 0)
    abort ();

  if (__atomic_compare_exchange (&v, &expected, &zero, STRONG , __ATOMIC_ACQUIRE, __ATOMIC_RELAXED)) 
    abort ();
  if (expected != max)
    abort ();

  if (!__atomic_compare_exchange (&v, &expected, &zero, STRONG , __ATOMIC_RELEASE, __ATOMIC_ACQUIRE)) 
    abort ();
  if (expected != max)
    abort ();
  if (v != 0)
    abort ();

  if (__atomic_compare_exchange (&v, &expected, &desired, WEAK, __ATOMIC_ACQ_REL, __ATOMIC_ACQUIRE)) 
    abort ();
  if (expected != 0)
    abort ();

  if (!__atomic_compare_exchange (&v, &expected, &desired, STRONG , __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)) 
    abort ();
  if (expected != 0)
    abort ();
  if (v != max)
    abort ();

  return 0;
}

// SLATE-FILECHECK-STD DEFAULT c89
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
// DEFAULT-NEXT:     global %[[VALUE_v:[0-9]+]] v: i8 [storage=static] = truncate<i8, reason=assign, fits=always>(const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_expected:[0-9]+]] expected: i8 [storage=static] = truncate<i8, reason=assign, fits=always>(const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_max:[0-9]+]] max: i8 [storage=static] = truncate<i8, reason=assign, fits=unknown>(not<i32>(const<i32>(0))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_desired:[0-9]+]] desired: i8 [storage=static] = truncate<i8, reason=assign, fits=unknown>(not<i32>(const<i32>(0))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_zero:[0-9]+]] zero: i8 [storage=static] = truncate<i8, reason=assign, fits=always>(const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(unprototyped) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: bool [synthetic] = compare_exchange<i8, form=write_back, weak=false, success=relaxed, failure=relaxed>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), addr_of<ptr<i8>>(%[[VALUE_expected]]), read<i8>(%[[VALUE_max]]));
// DEFAULT-NEXT:         if not<bool>(read<bool>(%[[VALUE0]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_expected]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: bool [synthetic] = compare_exchange<i8, form=write_back, weak=false, success=acquire, failure=relaxed>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), addr_of<ptr<i8>>(%[[VALUE_expected]]), truncate<i8, reason=arg, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE1]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_expected]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_max]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: bool [synthetic] = compare_exchange<i8, form=write_back, weak=false, success=release, failure=acquire>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), addr_of<ptr<i8>>(%[[VALUE_expected]]), truncate<i8, reason=arg, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         if not<bool>(read<bool>(%[[VALUE2]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_expected]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_max]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_v]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: bool [synthetic] = compare_exchange<i8, form=write_back, weak=true, success=acq_rel, failure=acquire>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), addr_of<ptr<i8>>(%[[VALUE_expected]]), read<i8>(%[[VALUE_desired]]));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE3]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_expected]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: bool [synthetic] = compare_exchange<i8, form=write_back, weak=false, success=seq_cst, failure=seq_cst>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), addr_of<ptr<i8>>(%[[VALUE_expected]]), read<i8>(%[[VALUE_desired]]));
// DEFAULT-NEXT:         if not<bool>(read<bool>(%[[VALUE4]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_expected]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_v]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_max]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i8>(%[[VALUE_v]], truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: bool [synthetic] = compare_exchange<i8, form=write_back, weak=false, success=relaxed, failure=relaxed>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), addr_of<ptr<i8>>(%[[VALUE_expected]]), read<i8>(deref(addr_of<ptr<i8>>(%[[VALUE_max]]))));
// DEFAULT-NEXT:         if not<bool>(read<bool>(%[[VALUE5]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_expected]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: bool [synthetic] = compare_exchange<i8, form=write_back, weak=false, success=acquire, failure=relaxed>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), addr_of<ptr<i8>>(%[[VALUE_expected]]), read<i8>(deref(addr_of<ptr<i8>>(%[[VALUE_zero]]))));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE6]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_expected]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_max]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: bool [synthetic] = compare_exchange<i8, form=write_back, weak=false, success=release, failure=acquire>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), addr_of<ptr<i8>>(%[[VALUE_expected]]), read<i8>(deref(addr_of<ptr<i8>>(%[[VALUE_zero]]))));
// DEFAULT-NEXT:         if not<bool>(read<bool>(%[[VALUE7]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_expected]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_max]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_v]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: bool [synthetic] = compare_exchange<i8, form=write_back, weak=true, success=acq_rel, failure=acquire>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), addr_of<ptr<i8>>(%[[VALUE_expected]]), read<i8>(deref(addr_of<ptr<i8>>(%[[VALUE_desired]]))));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE8]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_expected]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: bool [synthetic] = compare_exchange<i8, form=write_back, weak=false, success=seq_cst, failure=seq_cst>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), addr_of<ptr<i8>>(%[[VALUE_expected]]), read<i8>(deref(addr_of<ptr<i8>>(%[[VALUE_desired]]))));
// DEFAULT-NEXT:         if not<bool>(read<bool>(%[[VALUE9]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_expected]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_v]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_max]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
