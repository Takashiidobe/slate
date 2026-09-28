/* PR c/69104.  Test atomic routines for invalid memory model errors.  This
   only needs to be tested on a single size.  */
/* { dg-do compile } */
/* { dg-require-effective-target sync_int_long } */

#include <stdatomic.h>

/* atomic_store_explicit():
   The order argument shall not be memory_order_acquire,
   memory_order_consume, nor memory_order_acq_rel.  */

void
store (atomic_int *i)
{
  atomic_store_explicit (i, 0, memory_order_consume); /* { dg-warning "invalid memory model" } */
  atomic_store_explicit (i, 0, memory_order_acquire); /* { dg-warning "invalid memory model" } */
  atomic_store_explicit (i, 0, memory_order_acq_rel); /* { dg-warning "invalid memory model" } */
}

/* atomic_load_explicit():
   The order argument shall not be memory_order_release nor
   memory_order_acq_rel.  */

void
load (atomic_int *i)
{
  atomic_int j = atomic_load_explicit (i, memory_order_release); /* { dg-warning "invalid memory model" } */
  atomic_int k = atomic_load_explicit (i, memory_order_acq_rel); /* { dg-warning "invalid memory model" } */
}

/* atomic_compare_exchange():
   The failure argument shall not be memory_order_release nor
   memory_order_acq_rel.  The failure argument shall be no stronger than the
   success argument.  */

void
exchange (atomic_int *i)
{
  int r;

  atomic_compare_exchange_strong_explicit (i, &r, 0, memory_order_seq_cst, memory_order_release); /* { dg-warning "invalid failure memory model 'memory_order_release'" } */
  atomic_compare_exchange_strong_explicit (i, &r, 0, memory_order_seq_cst, memory_order_acq_rel); /* { dg-warning "invalid failure memory model 'memory_order_acq_rel'" } */
  atomic_compare_exchange_strong_explicit (i, &r, 0, memory_order_relaxed, memory_order_consume); /* { dg-warning "failure memory model 'memory_order_consume' cannot be stronger than success memory model 'memory_order_relaxed'" } */

  atomic_compare_exchange_weak_explicit (i, &r, 0, memory_order_seq_cst, memory_order_release); /* { dg-warning "invalid failure memory model 'memory_order_release'" } */
  atomic_compare_exchange_weak_explicit (i, &r, 0, memory_order_seq_cst, memory_order_acq_rel); /* { dg-warning "invalid failure memory model 'memory_order_acq_rel'" } */
  atomic_compare_exchange_weak_explicit (i, &r, 0, memory_order_relaxed, memory_order_consume); /* { dg-warning "failure memory model 'memory_order_consume' cannot be stronger than success memory model 'memory_order_relaxed'" } */
}

/* atomic_flag_clear():
   The order argument shall not be memory_order_acquire nor
   memory_order_acq_rel.  */

void
clear (atomic_int *i)
{
  atomic_flag_clear_explicit (i, memory_order_acquire); /* { dg-warning "invalid memory model" } */
  atomic_flag_clear_explicit (i, memory_order_acq_rel); /* { dg-warning "invalid memory model" } */
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
// DEFAULT-NEXT:     type @type0 = enum : u32 {
// DEFAULT-NEXT:         %0 memory_order_relaxed = const<i32>(0);
// DEFAULT-NEXT:         %1 memory_order_consume = const<i32>(1);
// DEFAULT-NEXT:         %2 memory_order_acquire = const<i32>(2);
// DEFAULT-NEXT:         %3 memory_order_release = const<i32>(3);
// DEFAULT-NEXT:         %4 memory_order_acq_rel = const<i32>(4);
// DEFAULT-NEXT:         %5 memory_order_seq_cst = const<i32>(5);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type1 memory_order = @type0;
// DEFAULT-NEXT:     type @type2 atomic_int = i32;
// DEFAULT-NEXT:     fn %9 @store(%10 i: ptr<atomic i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %11 __atomic_store_ptr: ptr<atomic i32> [storage=automatic] = read<ptr<atomic i32>>(%10);
// DEFAULT-NEXT:             let %12 __atomic_store_tmp: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             write<i32, atomic=consume>(deref(read<ptr<atomic i32>>(%11)), read<i32>(deref(addr_of<ptr<i32>>(%12))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %13 __atomic_store_ptr: ptr<atomic i32> [storage=automatic] = read<ptr<atomic i32>>(%10);
// DEFAULT-NEXT:             let %14 __atomic_store_tmp: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             write<i32, atomic=acquire>(deref(read<ptr<atomic i32>>(%13)), read<i32>(deref(addr_of<ptr<i32>>(%14))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %15 __atomic_store_ptr: ptr<atomic i32> [storage=automatic] = read<ptr<atomic i32>>(%10);
// DEFAULT-NEXT:             let %16 __atomic_store_tmp: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             write<i32, atomic=acq_rel>(deref(read<ptr<atomic i32>>(%15)), read<i32>(deref(addr_of<ptr<i32>>(%16))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @load(%18 i: ptr<atomic i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %19 j: atomic i32 [storage=automatic];
// DEFAULT-NEXT:         let %42: i32 [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %20 __atomic_load_ptr: ptr<atomic i32> [storage=automatic] = read<ptr<atomic i32>>(%18);
// DEFAULT-NEXT:             let %21 __atomic_load_tmp: i32 [storage=automatic];
// DEFAULT-NEXT:             write<i32>(deref(addr_of<ptr<i32>>(%21)), read<i32, atomic=release>(deref(read<ptr<atomic i32>>(%20))));
// DEFAULT-NEXT:             write<i32>(%42, read<i32>(%21));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<i32, atomic=seq_cst>(%19, read<i32>(%42));
// DEFAULT-NEXT:         let %22 k: atomic i32 [storage=automatic];
// DEFAULT-NEXT:         let %43: i32 [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %23 __atomic_load_ptr: ptr<atomic i32> [storage=automatic] = read<ptr<atomic i32>>(%18);
// DEFAULT-NEXT:             let %24 __atomic_load_tmp: i32 [storage=automatic];
// DEFAULT-NEXT:             write<i32>(deref(addr_of<ptr<i32>>(%24)), read<i32, atomic=acq_rel>(deref(read<ptr<atomic i32>>(%23))));
// DEFAULT-NEXT:             write<i32>(%43, read<i32>(%24));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<i32, atomic=seq_cst>(%22, read<i32>(%43));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %25 @exchange(%26 i: ptr<atomic i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %27 r: i32 [storage=automatic];
// DEFAULT-NEXT:         let %44: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %28 __atomic_compare_exchange_ptr: ptr<atomic i32> [storage=automatic] = read<ptr<atomic i32>>(%26);
// DEFAULT-NEXT:             let %29 __atomic_compare_exchange_tmp: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             let %45: bool [synthetic] = compare_exchange<i32, form=write_back, weak=false, success=seq_cst, failure=release>(deref(read<ptr<atomic i32>>(%28)), addr_of<ptr<i32>>(%27), read<i32>(deref(addr_of<ptr<i32>>(%29))));
// DEFAULT-NEXT:             write<bool>(%44, read<bool>(%45));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %46: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %30 __atomic_compare_exchange_ptr: ptr<atomic i32> [storage=automatic] = read<ptr<atomic i32>>(%26);
// DEFAULT-NEXT:             let %31 __atomic_compare_exchange_tmp: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             let %47: bool [synthetic] = compare_exchange<i32, form=write_back, weak=false, success=seq_cst, failure=acq_rel>(deref(read<ptr<atomic i32>>(%30)), addr_of<ptr<i32>>(%27), read<i32>(deref(addr_of<ptr<i32>>(%31))));
// DEFAULT-NEXT:             write<bool>(%46, read<bool>(%47));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %48: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %32 __atomic_compare_exchange_ptr: ptr<atomic i32> [storage=automatic] = read<ptr<atomic i32>>(%26);
// DEFAULT-NEXT:             let %33 __atomic_compare_exchange_tmp: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             let %49: bool [synthetic] = compare_exchange<i32, form=write_back, weak=false, success=relaxed, failure=consume>(deref(read<ptr<atomic i32>>(%32)), addr_of<ptr<i32>>(%27), read<i32>(deref(addr_of<ptr<i32>>(%33))));
// DEFAULT-NEXT:             write<bool>(%48, read<bool>(%49));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %50: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %34 __atomic_compare_exchange_ptr: ptr<atomic i32> [storage=automatic] = read<ptr<atomic i32>>(%26);
// DEFAULT-NEXT:             let %35 __atomic_compare_exchange_tmp: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             let %51: bool [synthetic] = compare_exchange<i32, form=write_back, weak=true, success=seq_cst, failure=release>(deref(read<ptr<atomic i32>>(%34)), addr_of<ptr<i32>>(%27), read<i32>(deref(addr_of<ptr<i32>>(%35))));
// DEFAULT-NEXT:             write<bool>(%50, read<bool>(%51));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %52: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %36 __atomic_compare_exchange_ptr: ptr<atomic i32> [storage=automatic] = read<ptr<atomic i32>>(%26);
// DEFAULT-NEXT:             let %37 __atomic_compare_exchange_tmp: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             let %53: bool [synthetic] = compare_exchange<i32, form=write_back, weak=true, success=seq_cst, failure=acq_rel>(deref(read<ptr<atomic i32>>(%36)), addr_of<ptr<i32>>(%27), read<i32>(deref(addr_of<ptr<i32>>(%37))));
// DEFAULT-NEXT:             write<bool>(%52, read<bool>(%53));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %54: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %38 __atomic_compare_exchange_ptr: ptr<atomic i32> [storage=automatic] = read<ptr<atomic i32>>(%26);
// DEFAULT-NEXT:             let %39 __atomic_compare_exchange_tmp: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             let %55: bool [synthetic] = compare_exchange<i32, form=write_back, weak=true, success=relaxed, failure=consume>(deref(read<ptr<atomic i32>>(%38)), addr_of<ptr<i32>>(%27), read<i32>(deref(addr_of<ptr<i32>>(%39))));
// DEFAULT-NEXT:             write<bool>(%54, read<bool>(%55));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %40 @clear(%41 i: ptr<atomic i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32, atomic=acquire>(deref(read<ptr<atomic i32>>(%41)), const<i32>(0));
// DEFAULT-NEXT:         write<i32, atomic=acq_rel>(deref(read<ptr<atomic i32>>(%41)), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
