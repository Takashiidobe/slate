/* Test __atomic routines for invalid memory model errors. This only needs
   to be tested on a single size.  */
/* { dg-do compile } */
/* { dg-require-effective-target sync_int_long } */

#include <stddef.h>
#include <stdbool.h>

int i, e, b;
size_t s;
bool x;

int
main ()
{
  __atomic_compare_exchange_n (&i, &e, 1, 0, __ATOMIC_RELAXED, __ATOMIC_SEQ_CST); /* { dg-warning "failure memory model 'memory_order_seq_cst' cannot be stronger" } */
  __atomic_compare_exchange_n (&i, &e, 1, 0, __ATOMIC_SEQ_CST, __ATOMIC_RELEASE); /* { dg-warning "invalid failure memory" } */
  __atomic_compare_exchange_n (&i, &e, 1, 1, __ATOMIC_SEQ_CST, __ATOMIC_ACQ_REL); /* { dg-warning "invalid failure memory" } */

  __atomic_load_n (&i, __ATOMIC_RELEASE); /* { dg-warning "invalid memory model" } */
  __atomic_load_n (&i, __ATOMIC_ACQ_REL); /* { dg-warning "invalid memory model" } */

  __atomic_store_n (&i, 1, __ATOMIC_ACQUIRE); /* { dg-warning "invalid memory model" } */
  __atomic_store_n (&i, 1, __ATOMIC_CONSUME); /* { dg-warning "invalid memory model" } */
  __atomic_store_n (&i, 1, __ATOMIC_ACQ_REL); /* { dg-warning "invalid memory model" } */

  i = __atomic_always_lock_free (s, NULL); /* { dg-error "non-constant argument" } */

  __atomic_load_n (&i, 44); /* { dg-warning "invalid memory model" } */

  __atomic_clear (&x, __ATOMIC_CONSUME); /* { dg-warning "invalid memory model" } */
  __atomic_clear (&x, __ATOMIC_ACQUIRE); /* { dg-warning "invalid memory model" } */

  __atomic_clear (&x, __ATOMIC_ACQ_REL); /* { dg-warning "invalid memory model" } */

}

// SLATE-FILECHECK-FLAVOR gcc
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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     global %1 i: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 e: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 s: u64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 x: bool [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %6 @main(unprototyped) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %7: bool [synthetic] = compare_exchange<i32, form=write_back, weak=false, success=relaxed, failure=seq_cst>(deref(addr_of<ptr<i32>>(%1)), addr_of<ptr<i32>>(%2), const<i32>(1));
// DEFAULT-NEXT:         let %8: bool [synthetic] = compare_exchange<i32, form=write_back, weak=false, success=seq_cst, failure=release>(deref(addr_of<ptr<i32>>(%1)), addr_of<ptr<i32>>(%2), const<i32>(1));
// DEFAULT-NEXT:         let %9: bool [synthetic] = compare_exchange<i32, form=write_back, weak=true, success=seq_cst, failure=acq_rel>(deref(addr_of<ptr<i32>>(%1)), addr_of<ptr<i32>>(%2), const<i32>(1));
// DEFAULT-NEXT:         read<i32, atomic=release>(deref(addr_of<ptr<i32>>(%1)));
// DEFAULT-NEXT:         read<i32, atomic=acq_rel>(deref(addr_of<ptr<i32>>(%1)));
// DEFAULT-NEXT:         write<i32, atomic=acquire>(deref(addr_of<ptr<i32>>(%1)), const<i32>(1));
// DEFAULT-NEXT:         write<i32, atomic=consume>(deref(addr_of<ptr<i32>>(%1)), const<i32>(1));
// DEFAULT-NEXT:         write<i32, atomic=acq_rel>(deref(addr_of<ptr<i32>>(%1)), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%1, from_bool<i32, reason=assign>(const<bool>(false)));
// DEFAULT-NEXT:         read<i32, atomic=dynamic(const<i32>(44))>(deref(addr_of<ptr<i32>>(%1)));
// DEFAULT-NEXT:         write<bool, atomic=consume>(deref(addr_of<ptr<bool>>(%5)), const<bool>(false));
// DEFAULT-NEXT:         write<bool, atomic=acquire>(deref(addr_of<ptr<bool>>(%5)), const<bool>(false));
// DEFAULT-NEXT:         write<bool, atomic=acq_rel>(deref(addr_of<ptr<bool>>(%5)), const<bool>(false));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
