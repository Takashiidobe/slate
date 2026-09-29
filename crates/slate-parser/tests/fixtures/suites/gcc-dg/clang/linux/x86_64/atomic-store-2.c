/* Test __atomic routines for existence and proper execution on 2 byte 
   values with each valid memory model.  */
/* { dg-do run } */
/* { dg-require-effective-target sync_char_short } */

/* Test the execution of the __atomic_store_n builtin for a short.  */

extern void abort(void);

short v, count;

int
main ()
{
  v = 0;
  count = 0;

  __atomic_store_n (&v, count + 1, __ATOMIC_RELAXED);
  if (v != ++count)
    abort ();

  __atomic_store_n (&v, count + 1, __ATOMIC_RELEASE);
  if (v != ++count)
    abort ();

  __atomic_store_n (&v, count + 1, __ATOMIC_SEQ_CST);
  if (v != ++count)
    abort ();

  /* Now test the generic variant.  */
  count++;

  __atomic_store (&v, &count, __ATOMIC_RELAXED);
  if (v != count++)
    abort ();

  __atomic_store (&v, &count, __ATOMIC_RELEASE);
  if (v != count++)
    abort ();

  __atomic_store (&v, &count, __ATOMIC_SEQ_CST);
  if (v != count)
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
// DEFAULT-NEXT:     global %[[VALUE_v:[0-9]+]] v: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_count:[0-9]+]] count: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(unprototyped) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i16>(%[[VALUE_v]], truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i16>(%[[VALUE_count]], truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i16, atomic=relaxed>(deref(addr_of<ptr<i16>>(%[[VALUE_v]])), truncate<i16, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_count]])), const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i16 [synthetic] = read<i16>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE0]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%[[VALUE_count]], read<i16>(%[[VALUE1]]));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_v]])), widen<i32, reason=promotion>(read<i16>(%[[VALUE1]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i16, atomic=release>(deref(addr_of<ptr<i16>>(%[[VALUE_v]])), truncate<i16, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_count]])), const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i16 [synthetic] = read<i16>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE2]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%[[VALUE_count]], read<i16>(%[[VALUE3]]));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_v]])), widen<i32, reason=promotion>(read<i16>(%[[VALUE3]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i16, atomic=seq_cst>(deref(addr_of<ptr<i16>>(%[[VALUE_v]])), truncate<i16, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_count]])), const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: i16 [synthetic] = read<i16>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE4]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%[[VALUE_count]], read<i16>(%[[VALUE5]]));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_v]])), widen<i32, reason=promotion>(read<i16>(%[[VALUE5]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: i16 [synthetic] = read<i16>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE6]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%[[VALUE_count]], read<i16>(%[[VALUE7]]));
// DEFAULT-NEXT:         write<i16, atomic=relaxed>(deref(addr_of<ptr<i16>>(%[[VALUE_v]])), read<i16>(deref(addr_of<ptr<i16>>(%[[VALUE_count]]))));
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: i16 [synthetic] = read<i16>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE8]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%[[VALUE_count]], read<i16>(%[[VALUE9]]));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_v]])), widen<i32, reason=promotion>(read<i16>(%[[VALUE8]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i16, atomic=release>(deref(addr_of<ptr<i16>>(%[[VALUE_v]])), read<i16>(deref(addr_of<ptr<i16>>(%[[VALUE_count]]))));
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: i16 [synthetic] = read<i16>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE10]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%[[VALUE_count]], read<i16>(%[[VALUE11]]));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_v]])), widen<i32, reason=promotion>(read<i16>(%[[VALUE10]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i16, atomic=seq_cst>(deref(addr_of<ptr<i16>>(%[[VALUE_v]])), read<i16>(deref(addr_of<ptr<i16>>(%[[VALUE_count]]))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_v]])), widen<i32, reason=promotion>(read<i16>(%[[VALUE_count]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
