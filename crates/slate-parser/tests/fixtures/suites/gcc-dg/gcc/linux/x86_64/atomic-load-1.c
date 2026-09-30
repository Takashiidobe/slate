/* Test __atomic routines for existence and proper execution on 1 byte 
   values with each valid memory model.  */
/* { dg-do run } */
/* { dg-require-effective-target sync_char_short } */


/* Test the execution of the __atomic_load_n builtin for a char.  */

extern void abort(void);

char v, count;

int
main ()
{
  v = 0;
  count = 0;

  if (__atomic_load_n (&v, __ATOMIC_RELAXED) != count++) 
    abort(); 
  else 
    v++;

  if (__atomic_load_n (&v, __ATOMIC_ACQUIRE) != count++) 
    abort(); 
  else 
    v++;

  if (__atomic_load_n (&v, __ATOMIC_CONSUME) != count++) 
    abort(); 
  else 
    v++;

  if (__atomic_load_n (&v, __ATOMIC_SEQ_CST) != count++) 
    abort(); 
  else 
    v++;

  /* Now test the generic variants.  */

  __atomic_load (&v, &count, __ATOMIC_RELAXED);
  if (count != v)
    abort(); 
  else 
    v++;

  __atomic_load (&v, &count, __ATOMIC_ACQUIRE);
  if (count != v)
    abort(); 
  else 
    v++;

  __atomic_load (&v, &count, __ATOMIC_CONSUME);
  if (count != v)
    abort(); 
  else 
    v++;

  __atomic_load (&v, &count, __ATOMIC_SEQ_CST);
  if (count != v)
    abort(); 
  else 
    v++;

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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(unprototyped) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i8>(%[[VALUE_v]], truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_count]], truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE0]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_count]], read<i8>(%[[VALUE1]]));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8, atomic=relaxed>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])))), widen<i32, reason=promotion>(read<i8>(%[[VALUE0]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %[[VALUE2:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_v]]);
// DEFAULT-NEXT:             let %[[VALUE3:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE2]])), const<i32>(1)));
// DEFAULT-NEXT:             write<i8>(%[[VALUE_v]], read<i8>(%[[VALUE3]]));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE4]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_count]], read<i8>(%[[VALUE5]]));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8, atomic=acquire>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])))), widen<i32, reason=promotion>(read<i8>(%[[VALUE4]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %[[VALUE6:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_v]]);
// DEFAULT-NEXT:             let %[[VALUE7:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE6]])), const<i32>(1)));
// DEFAULT-NEXT:             write<i8>(%[[VALUE_v]], read<i8>(%[[VALUE7]]));
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE8]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_count]], read<i8>(%[[VALUE9]]));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8, atomic=consume>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])))), widen<i32, reason=promotion>(read<i8>(%[[VALUE8]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %[[VALUE10:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_v]]);
// DEFAULT-NEXT:             let %[[VALUE11:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE10]])), const<i32>(1)));
// DEFAULT-NEXT:             write<i8>(%[[VALUE_v]], read<i8>(%[[VALUE11]]));
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE12]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_count]], read<i8>(%[[VALUE13]]));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8, atomic=seq_cst>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])))), widen<i32, reason=promotion>(read<i8>(%[[VALUE12]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %[[VALUE14:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_v]]);
// DEFAULT-NEXT:             let %[[VALUE15:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE14]])), const<i32>(1)));
// DEFAULT-NEXT:             write<i8>(%[[VALUE_v]], read<i8>(%[[VALUE15]]));
// DEFAULT-NEXT:         write<i8>(deref(addr_of<ptr<i8>>(%[[VALUE_count]])), read<i8, atomic=relaxed>(deref(addr_of<ptr<i8>>(%[[VALUE_v]]))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_count]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_v]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %[[VALUE16:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_v]]);
// DEFAULT-NEXT:             let %[[VALUE17:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE16]])), const<i32>(1)));
// DEFAULT-NEXT:             write<i8>(%[[VALUE_v]], read<i8>(%[[VALUE17]]));
// DEFAULT-NEXT:         write<i8>(deref(addr_of<ptr<i8>>(%[[VALUE_count]])), read<i8, atomic=acquire>(deref(addr_of<ptr<i8>>(%[[VALUE_v]]))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_count]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_v]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %[[VALUE18:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_v]]);
// DEFAULT-NEXT:             let %[[VALUE19:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE18]])), const<i32>(1)));
// DEFAULT-NEXT:             write<i8>(%[[VALUE_v]], read<i8>(%[[VALUE19]]));
// DEFAULT-NEXT:         write<i8>(deref(addr_of<ptr<i8>>(%[[VALUE_count]])), read<i8, atomic=consume>(deref(addr_of<ptr<i8>>(%[[VALUE_v]]))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_count]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_v]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %[[VALUE20:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_v]]);
// DEFAULT-NEXT:             let %[[VALUE21:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE20]])), const<i32>(1)));
// DEFAULT-NEXT:             write<i8>(%[[VALUE_v]], read<i8>(%[[VALUE21]]));
// DEFAULT-NEXT:         write<i8>(deref(addr_of<ptr<i8>>(%[[VALUE_count]])), read<i8, atomic=seq_cst>(deref(addr_of<ptr<i8>>(%[[VALUE_v]]))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_count]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_v]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %[[VALUE22:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_v]]);
// DEFAULT-NEXT:             let %[[VALUE23:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE22]])), const<i32>(1)));
// DEFAULT-NEXT:             write<i8>(%[[VALUE_v]], read<i8>(%[[VALUE23]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
