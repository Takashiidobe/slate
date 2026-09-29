/* Test __atomic routines for existence and proper execution on 8 byte 
   values with each valid memory model.  */
/* { dg-do run } */
/* { dg-require-effective-target sync_long_long_runtime } */
/* { dg-options "" } */
/* { dg-options "-march=pentium" { target { { i?86-*-* x86_64-*-* } && ia32 } } } */

/* Test the execution of the __atomic_store_n builtin for a long long.  */

extern void abort(void);

long long v, count;

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
// DEFAULT-NEXT:     global %[[VALUE_v:[0-9]+]] v: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_count:[0-9]+]] count: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<i64>(%[[VALUE_v]], widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         write<i64>(%[[VALUE_count]], widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         write<i64, atomic=relaxed>(deref(addr_of<ptr<i64>>(%[[VALUE_v]])), add<i64, overflow=ub>(read<i64>(%[[VALUE_count]]), widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE0]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(%[[VALUE_count]], read<i64>(%[[VALUE1]]));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%[[VALUE_v]]), read<i64>(%[[VALUE1]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i64, atomic=release>(deref(addr_of<ptr<i64>>(%[[VALUE_v]])), add<i64, overflow=ub>(read<i64>(%[[VALUE_count]]), widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE2]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(%[[VALUE_count]], read<i64>(%[[VALUE3]]));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%[[VALUE_v]]), read<i64>(%[[VALUE3]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i64, atomic=seq_cst>(deref(addr_of<ptr<i64>>(%[[VALUE_v]])), add<i64, overflow=ub>(read<i64>(%[[VALUE_count]]), widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE4]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(%[[VALUE_count]], read<i64>(%[[VALUE5]]));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%[[VALUE_v]]), read<i64>(%[[VALUE5]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE6]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(%[[VALUE_count]], read<i64>(%[[VALUE7]]));
// DEFAULT-NEXT:         write<i64, atomic=relaxed>(deref(addr_of<ptr<i64>>(%[[VALUE_v]])), read<i64>(deref(addr_of<ptr<i64>>(%[[VALUE_count]]))));
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE8]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(%[[VALUE_count]], read<i64>(%[[VALUE9]]));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%[[VALUE_v]]), read<i64>(%[[VALUE8]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i64, atomic=release>(deref(addr_of<ptr<i64>>(%[[VALUE_v]])), read<i64>(deref(addr_of<ptr<i64>>(%[[VALUE_count]]))));
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE10]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(%[[VALUE_count]], read<i64>(%[[VALUE11]]));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%[[VALUE_v]]), read<i64>(%[[VALUE10]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i64, atomic=seq_cst>(deref(addr_of<ptr<i64>>(%[[VALUE_v]])), read<i64>(deref(addr_of<ptr<i64>>(%[[VALUE_count]]))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%[[VALUE_v]]), read<i64>(%[[VALUE_count]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
