/* Test __atomic routines for existence and proper execution on 1 byte 
   values with each valid memory model.  */
/* { dg-do run } */
/* { dg-require-effective-target sync_char_short } */

/* Test the execution of the __atomic_exchange_n builtin for a char.  */

extern void abort(void);

char v, count, ret;

int
main ()
{
  v = 0;
  count = 0;

  if (__atomic_exchange_n (&v, count + 1, __ATOMIC_RELAXED) != count)
    abort ();
  count++;

  if (__atomic_exchange_n (&v, count + 1, __ATOMIC_ACQUIRE) != count)
    abort ();
  count++;

  if (__atomic_exchange_n (&v, count + 1, __ATOMIC_RELEASE) != count)
    abort ();
  count++;

  if (__atomic_exchange_n (&v, count + 1, __ATOMIC_ACQ_REL) != count)
    abort ();
  count++;

  if (__atomic_exchange_n (&v, count + 1, __ATOMIC_SEQ_CST) != count)
    abort ();
  count++;

  /* Now test the generic version.  */

  count++;

  __atomic_exchange (&v, &count, &ret, __ATOMIC_RELAXED);
  if (ret != count - 1 || v != count)
    abort ();
  count++;

  __atomic_exchange (&v, &count, &ret, __ATOMIC_ACQUIRE);
  if (ret != count - 1 || v != count)
    abort ();
  count++;

  __atomic_exchange (&v, &count, &ret, __ATOMIC_RELEASE);
  if (ret != count - 1 || v != count)
    abort ();
  count++;

  __atomic_exchange (&v, &count, &ret, __ATOMIC_ACQ_REL);
  if (ret != count - 1 || v != count)
    abort ();
  count++;

  __atomic_exchange (&v, &count, &ret, __ATOMIC_SEQ_CST);
  if (ret != count - 1 || v != count)
    abort ();
  count++;

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
// DEFAULT-NEXT:     global %[[VALUE_v:[0-9]+]] v: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_count:[0-9]+]] count: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ret:[0-9]+]] ret: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(unprototyped) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i8>(%[[VALUE_v]], truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_count]], truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=relaxed>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), truncate<i8, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_count]])), const<i32>(1))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE0]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_count]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE1]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_count]], read<i8>(%[[VALUE2]]));
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=acquire>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), truncate<i8, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_count]])), const<i32>(1))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE3]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_count]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE4]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_count]], read<i8>(%[[VALUE5]]));
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=release>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), truncate<i8, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_count]])), const<i32>(1))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE6]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_count]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE7]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_count]], read<i8>(%[[VALUE8]]));
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=acq_rel>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), truncate<i8, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_count]])), const<i32>(1))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE9]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_count]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE10]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_count]], read<i8>(%[[VALUE11]]));
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), truncate<i8, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_count]])), const<i32>(1))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE12]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_count]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE14:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE13]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_count]], read<i8>(%[[VALUE14]]));
// DEFAULT-NEXT:         let %[[VALUE15:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE16:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE15]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_count]], read<i8>(%[[VALUE16]]));
// DEFAULT-NEXT:         let %[[VALUE17:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=relaxed>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), read<i8>(deref(addr_of<ptr<i8>>(%[[VALUE_count]]))));
// DEFAULT-NEXT:         write<i8>(deref(addr_of<ptr<i8>>(%[[VALUE_ret]])), read<i8>(%[[VALUE17]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_ret]])), sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_count]])), const<i32>(1))), ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_v]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_count]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE18:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE19:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE18]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_count]], read<i8>(%[[VALUE19]]));
// DEFAULT-NEXT:         let %[[VALUE20:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=acquire>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), read<i8>(deref(addr_of<ptr<i8>>(%[[VALUE_count]]))));
// DEFAULT-NEXT:         write<i8>(deref(addr_of<ptr<i8>>(%[[VALUE_ret]])), read<i8>(%[[VALUE20]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_ret]])), sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_count]])), const<i32>(1))), ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_v]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_count]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE21:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE22:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE21]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_count]], read<i8>(%[[VALUE22]]));
// DEFAULT-NEXT:         let %[[VALUE23:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=release>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), read<i8>(deref(addr_of<ptr<i8>>(%[[VALUE_count]]))));
// DEFAULT-NEXT:         write<i8>(deref(addr_of<ptr<i8>>(%[[VALUE_ret]])), read<i8>(%[[VALUE23]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_ret]])), sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_count]])), const<i32>(1))), ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_v]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_count]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE24:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE25:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE24]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_count]], read<i8>(%[[VALUE25]]));
// DEFAULT-NEXT:         let %[[VALUE26:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=acq_rel>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), read<i8>(deref(addr_of<ptr<i8>>(%[[VALUE_count]]))));
// DEFAULT-NEXT:         write<i8>(deref(addr_of<ptr<i8>>(%[[VALUE_ret]])), read<i8>(%[[VALUE26]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_ret]])), sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_count]])), const<i32>(1))), ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_v]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_count]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE27:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE28:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE27]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_count]], read<i8>(%[[VALUE28]]));
// DEFAULT-NEXT:         let %[[VALUE29:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), read<i8>(deref(addr_of<ptr<i8>>(%[[VALUE_count]]))));
// DEFAULT-NEXT:         write<i8>(deref(addr_of<ptr<i8>>(%[[VALUE_ret]])), read<i8>(%[[VALUE29]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_ret]])), sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_count]])), const<i32>(1))), ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_v]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_count]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE30:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE31:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE30]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_count]], read<i8>(%[[VALUE31]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
