/* Test __atomic routines for existence and proper execution on 2 byte 
   values with each valid memory model.  */
/* { dg-do run } */
/* { dg-require-effective-target sync_char_short } */

/* Test the execution of the __atomic_store_n builtin for a short.  */

extern void abort(void);

short v, count;

int main() {
  v     = 0;
  count = 0;

  __atomic_store_n(&v, count + 1, __ATOMIC_RELAXED);
  if (v != ++count)
    abort();

  __atomic_store_n(&v, count + 1, __ATOMIC_RELEASE);
  if (v != ++count)
    abort();

  __atomic_store_n(&v, count + 1, __ATOMIC_SEQ_CST);
  if (v != ++count)
    abort();

  /* Now test the generic variant.  */
  count++;

  __atomic_store(&v, &count, __ATOMIC_RELAXED);
  if (v != count++)
    abort();

  __atomic_store(&v, &count, __ATOMIC_RELEASE);
  if (v != count++)
    abort();

  __atomic_store(&v, &count, __ATOMIC_SEQ_CST);
  if (v != count)
    abort();

  return 0;
}



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
// DEFAULT-NEXT:     global %1 v: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 count: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<i16>(%1, truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i16>(%2, truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i16, atomic=relaxed>(deref(addr_of<ptr<i16>>(%1)), truncate<i16, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%2)), const<i32>(1))));
// DEFAULT-NEXT:         let %4: i16 [synthetic] = read<i16>(%2);
// DEFAULT-NEXT:         let %5: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%4)), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%2, read<i16>(%5));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%1)), widen<i32, reason=promotion>(read<i16>(%5)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         write<i16, atomic=release>(deref(addr_of<ptr<i16>>(%1)), truncate<i16, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%2)), const<i32>(1))));
// DEFAULT-NEXT:         let %6: i16 [synthetic] = read<i16>(%2);
// DEFAULT-NEXT:         let %7: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%6)), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%2, read<i16>(%7));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%1)), widen<i32, reason=promotion>(read<i16>(%7)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         write<i16, atomic=seq_cst>(deref(addr_of<ptr<i16>>(%1)), truncate<i16, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%2)), const<i32>(1))));
// DEFAULT-NEXT:         let %8: i16 [synthetic] = read<i16>(%2);
// DEFAULT-NEXT:         let %9: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%8)), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%2, read<i16>(%9));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%1)), widen<i32, reason=promotion>(read<i16>(%9)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %10: i16 [synthetic] = read<i16>(%2);
// DEFAULT-NEXT:         let %11: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%10)), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%2, read<i16>(%11));
// DEFAULT-NEXT:         write<i16, atomic=relaxed>(deref(addr_of<ptr<i16>>(%1)), read<i16>(deref(addr_of<ptr<i16>>(%2))));
// DEFAULT-NEXT:         let %12: i16 [synthetic] = read<i16>(%2);
// DEFAULT-NEXT:         let %13: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%12)), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%2, read<i16>(%13));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%1)), widen<i32, reason=promotion>(read<i16>(%12)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         write<i16, atomic=release>(deref(addr_of<ptr<i16>>(%1)), read<i16>(deref(addr_of<ptr<i16>>(%2))));
// DEFAULT-NEXT:         let %14: i16 [synthetic] = read<i16>(%2);
// DEFAULT-NEXT:         let %15: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%14)), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%2, read<i16>(%15));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%1)), widen<i32, reason=promotion>(read<i16>(%14)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         write<i16, atomic=seq_cst>(deref(addr_of<ptr<i16>>(%1)), read<i16>(deref(addr_of<ptr<i16>>(%2))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%1)), widen<i32, reason=promotion>(read<i16>(%2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
