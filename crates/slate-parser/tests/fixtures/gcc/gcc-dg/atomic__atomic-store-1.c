/* Test __atomic routines for existence and proper execution on 1 byte 
   values with each valid memory model.  */
/* { dg-do run } */
/* { dg-require-effective-target sync_char_short } */

/* Test the execution of the __atomic_store_n builtin for a char.  */

extern void abort(void);

char v, count;

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
// DEFAULT-NEXT:     global %1 v: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 count: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<i8>(%1, truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i8>(%2, truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i8, atomic=relaxed>(deref(addr_of<ptr<i8>>(%1)), truncate<i8, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%2)), const<i32>(1))));
// DEFAULT-NEXT:         let %4: i8 [synthetic] = read<i8>(%2);
// DEFAULT-NEXT:         let %5: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%4)), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%2, read<i8>(%5));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1)), widen<i32, reason=promotion>(read<i8>(%5)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i8, atomic=release>(deref(addr_of<ptr<i8>>(%1)), truncate<i8, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%2)), const<i32>(1))));
// DEFAULT-NEXT:         let %6: i8 [synthetic] = read<i8>(%2);
// DEFAULT-NEXT:         let %7: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%6)), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%2, read<i8>(%7));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1)), widen<i32, reason=promotion>(read<i8>(%7)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i8, atomic=seq_cst>(deref(addr_of<ptr<i8>>(%1)), truncate<i8, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%2)), const<i32>(1))));
// DEFAULT-NEXT:         let %8: i8 [synthetic] = read<i8>(%2);
// DEFAULT-NEXT:         let %9: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%8)), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%2, read<i8>(%9));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1)), widen<i32, reason=promotion>(read<i8>(%9)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %10: i8 [synthetic] = read<i8>(%2);
// DEFAULT-NEXT:         let %11: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%10)), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%2, read<i8>(%11));
// DEFAULT-NEXT:         write<i8, atomic=relaxed>(deref(addr_of<ptr<i8>>(%1)), read<i8>(deref(addr_of<ptr<i8>>(%2))));
// DEFAULT-NEXT:         let %12: i8 [synthetic] = read<i8>(%2);
// DEFAULT-NEXT:         let %13: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%12)), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%2, read<i8>(%13));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1)), widen<i32, reason=promotion>(read<i8>(%12)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i8, atomic=release>(deref(addr_of<ptr<i8>>(%1)), read<i8>(deref(addr_of<ptr<i8>>(%2))));
// DEFAULT-NEXT:         let %14: i8 [synthetic] = read<i8>(%2);
// DEFAULT-NEXT:         let %15: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%14)), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%2, read<i8>(%15));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1)), widen<i32, reason=promotion>(read<i8>(%14)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i8, atomic=seq_cst>(deref(addr_of<ptr<i8>>(%1)), read<i8>(deref(addr_of<ptr<i8>>(%2))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1)), widen<i32, reason=promotion>(read<i8>(%2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
