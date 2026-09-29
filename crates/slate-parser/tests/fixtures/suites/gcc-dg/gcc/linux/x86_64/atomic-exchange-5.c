/* Test __atomic routines for existence and proper execution on 16 byte 
   values with each valid memory model.  */
/* { dg-do run } */
/* { dg-require-effective-target sync_int_128_runtime } */
/* { dg-options "-mcx16" { target { i?86-*-* x86_64-*-* } } } */
/* { dg-options "-mlsx -mscq" { target { loongarch64-*-* } } } */

/* Test the execution of the __atomic_X builtin for a 16 byte value.  */

extern void abort(void);

__int128_t v, count, ret;

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

// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     global %[[VALUE_v:[0-9]+]] v: i128 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_count:[0-9]+]] count: i128 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ret:[0-9]+]] ret: i128 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<i128>(%[[VALUE_v]], widen<i128, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_count]], widen<i128, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=relaxed>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), add<i128, overflow=ub>(read<i128>(%[[VALUE_count]]), widen<i128, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE0]]), read<i128>(%[[VALUE_count]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i128 [synthetic] = read<i128>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i128 [synthetic] = add<i128, overflow=ub>(read<i128>(%[[VALUE1]]), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_count]], read<i128>(%[[VALUE2]]));
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=acquire>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), add<i128, overflow=ub>(read<i128>(%[[VALUE_count]]), widen<i128, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE3]]), read<i128>(%[[VALUE_count]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: i128 [synthetic] = read<i128>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: i128 [synthetic] = add<i128, overflow=ub>(read<i128>(%[[VALUE4]]), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_count]], read<i128>(%[[VALUE5]]));
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=release>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), add<i128, overflow=ub>(read<i128>(%[[VALUE_count]]), widen<i128, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE6]]), read<i128>(%[[VALUE_count]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: i128 [synthetic] = read<i128>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: i128 [synthetic] = add<i128, overflow=ub>(read<i128>(%[[VALUE7]]), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_count]], read<i128>(%[[VALUE8]]));
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=acq_rel>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), add<i128, overflow=ub>(read<i128>(%[[VALUE_count]]), widen<i128, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE9]]), read<i128>(%[[VALUE_count]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: i128 [synthetic] = read<i128>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: i128 [synthetic] = add<i128, overflow=ub>(read<i128>(%[[VALUE10]]), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_count]], read<i128>(%[[VALUE11]]));
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=seq_cst>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), add<i128, overflow=ub>(read<i128>(%[[VALUE_count]]), widen<i128, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE12]]), read<i128>(%[[VALUE_count]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: i128 [synthetic] = read<i128>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE14:[0-9]+]]: i128 [synthetic] = add<i128, overflow=ub>(read<i128>(%[[VALUE13]]), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_count]], read<i128>(%[[VALUE14]]));
// DEFAULT-NEXT:         let %[[VALUE15:[0-9]+]]: i128 [synthetic] = read<i128>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE16:[0-9]+]]: i128 [synthetic] = add<i128, overflow=ub>(read<i128>(%[[VALUE15]]), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_count]], read<i128>(%[[VALUE16]]));
// DEFAULT-NEXT:         let %[[VALUE17:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=relaxed>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), read<i128>(deref(addr_of<ptr<i128>>(%[[VALUE_count]]))));
// DEFAULT-NEXT:         write<i128>(deref(addr_of<ptr<i128>>(%[[VALUE_ret]])), read<i128>(%[[VALUE17]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i128>(read<i128>(%[[VALUE_ret]]), sub<i128, overflow=ub>(read<i128>(%[[VALUE_count]]), widen<i128, reason=usual_arith>(const<i32>(1)))), ne<i128>(read<i128>(%[[VALUE_v]]), read<i128>(%[[VALUE_count]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE18:[0-9]+]]: i128 [synthetic] = read<i128>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE19:[0-9]+]]: i128 [synthetic] = add<i128, overflow=ub>(read<i128>(%[[VALUE18]]), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_count]], read<i128>(%[[VALUE19]]));
// DEFAULT-NEXT:         let %[[VALUE20:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=acquire>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), read<i128>(deref(addr_of<ptr<i128>>(%[[VALUE_count]]))));
// DEFAULT-NEXT:         write<i128>(deref(addr_of<ptr<i128>>(%[[VALUE_ret]])), read<i128>(%[[VALUE20]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i128>(read<i128>(%[[VALUE_ret]]), sub<i128, overflow=ub>(read<i128>(%[[VALUE_count]]), widen<i128, reason=usual_arith>(const<i32>(1)))), ne<i128>(read<i128>(%[[VALUE_v]]), read<i128>(%[[VALUE_count]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE21:[0-9]+]]: i128 [synthetic] = read<i128>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE22:[0-9]+]]: i128 [synthetic] = add<i128, overflow=ub>(read<i128>(%[[VALUE21]]), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_count]], read<i128>(%[[VALUE22]]));
// DEFAULT-NEXT:         let %[[VALUE23:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=release>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), read<i128>(deref(addr_of<ptr<i128>>(%[[VALUE_count]]))));
// DEFAULT-NEXT:         write<i128>(deref(addr_of<ptr<i128>>(%[[VALUE_ret]])), read<i128>(%[[VALUE23]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i128>(read<i128>(%[[VALUE_ret]]), sub<i128, overflow=ub>(read<i128>(%[[VALUE_count]]), widen<i128, reason=usual_arith>(const<i32>(1)))), ne<i128>(read<i128>(%[[VALUE_v]]), read<i128>(%[[VALUE_count]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE24:[0-9]+]]: i128 [synthetic] = read<i128>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE25:[0-9]+]]: i128 [synthetic] = add<i128, overflow=ub>(read<i128>(%[[VALUE24]]), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_count]], read<i128>(%[[VALUE25]]));
// DEFAULT-NEXT:         let %[[VALUE26:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=acq_rel>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), read<i128>(deref(addr_of<ptr<i128>>(%[[VALUE_count]]))));
// DEFAULT-NEXT:         write<i128>(deref(addr_of<ptr<i128>>(%[[VALUE_ret]])), read<i128>(%[[VALUE26]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i128>(read<i128>(%[[VALUE_ret]]), sub<i128, overflow=ub>(read<i128>(%[[VALUE_count]]), widen<i128, reason=usual_arith>(const<i32>(1)))), ne<i128>(read<i128>(%[[VALUE_v]]), read<i128>(%[[VALUE_count]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE27:[0-9]+]]: i128 [synthetic] = read<i128>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE28:[0-9]+]]: i128 [synthetic] = add<i128, overflow=ub>(read<i128>(%[[VALUE27]]), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_count]], read<i128>(%[[VALUE28]]));
// DEFAULT-NEXT:         let %[[VALUE29:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=seq_cst>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), read<i128>(deref(addr_of<ptr<i128>>(%[[VALUE_count]]))));
// DEFAULT-NEXT:         write<i128>(deref(addr_of<ptr<i128>>(%[[VALUE_ret]])), read<i128>(%[[VALUE29]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i128>(read<i128>(%[[VALUE_ret]]), sub<i128, overflow=ub>(read<i128>(%[[VALUE_count]]), widen<i128, reason=usual_arith>(const<i32>(1)))), ne<i128>(read<i128>(%[[VALUE_v]]), read<i128>(%[[VALUE_count]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE30:[0-9]+]]: i128 [synthetic] = read<i128>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE31:[0-9]+]]: i128 [synthetic] = add<i128, overflow=ub>(read<i128>(%[[VALUE30]]), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_count]], read<i128>(%[[VALUE31]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
