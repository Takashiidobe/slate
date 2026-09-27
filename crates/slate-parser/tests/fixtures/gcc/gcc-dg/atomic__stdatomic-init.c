/* Test the atomic_init generic function.  Verify that __atomic_store_N
   is called with the last argument of memory_order_relaxed (i.e., 0)
   for each invocation of the atomic_init() macro in the test and that
   there are no calls to __atomic_store_N with a non-zero last argument.  */
/* { dg-do compile } */
/* { dg-options "-fdump-tree-gimple -std=c11 -pedantic-errors" } */
/* { dg-final { scan-tree-dump-times "__atomic_store_. \\(\[^\n\r]*, 0\\)" 54 "gimple" } } */
/* { dg-final { scan-tree-dump-not "__atomic_store_. \\(\[^\n\r]*, \[1-5\]\\)" "gimple" } } */

#include <stdatomic.h>

struct Atomic {
  /* Volatile to prevent re-initialization from being optimized away.  */
  volatile atomic_bool   b;
  volatile atomic_char   c;
  volatile atomic_schar  sc;
  volatile atomic_uchar  uc;
  volatile atomic_short  ss;
  volatile atomic_ushort us;
  volatile atomic_int    si;
  volatile atomic_uint   ui;
  volatile atomic_long   sl;
  volatile atomic_ulong  ul;
  volatile atomic_llong  sll;
  volatile atomic_ullong ull;
  volatile atomic_size_t sz;
};

struct Value {
  _Bool              b;
  char               c;
  signed char        sc;
  unsigned char      uc;
  short              ss;
  unsigned short     us;
  int                si;
  unsigned int       ui;
  long               sl;
  unsigned long      ul;
  long long          sll;
  unsigned long long ull;
  __SIZE_TYPE__      sz;
};

/* Exercise the atomic_init() macro with a literal argument.  */

void atomic_init_lit (struct Atomic *pa)
{
  atomic_init (&pa->b, 0);
  atomic_init (&pa->b, 1);

  atomic_init (&pa->c, 'x');
  atomic_init (&pa->c, 0);
  atomic_init (&pa->c, 1);
  atomic_init (&pa->c, 255);
  
  atomic_init (&pa->sc, (signed char)'x');
  atomic_init (&pa->sc, (signed char)0);
  atomic_init (&pa->sc, (signed char)1);
  atomic_init (&pa->sc, (signed char)__SCHAR_MAX__);

  atomic_init (&pa->uc, (unsigned char)'x');
  atomic_init (&pa->uc, (unsigned char)0);
  atomic_init (&pa->uc, (unsigned char)1);
  atomic_init (&pa->sc, (unsigned char)__SCHAR_MAX__);

  atomic_init (&pa->ss, (signed short)0);
  atomic_init (&pa->ss, (signed short)1);
  atomic_init (&pa->ss, (signed short)__SHRT_MAX__);

  atomic_init (&pa->us, (unsigned short)0);
  atomic_init (&pa->us, (unsigned short)1);
  atomic_init (&pa->us, (unsigned short)__SHRT_MAX__);

  atomic_init (&pa->si, (signed int)0);
  atomic_init (&pa->si, (signed int)1);
  atomic_init (&pa->si, (signed int)__INT_MAX__);

  atomic_init (&pa->ui, (unsigned int)0);
  atomic_init (&pa->ui, (unsigned int)1);
  atomic_init (&pa->ui, (unsigned int)__INT_MAX__);
  
  atomic_init (&pa->sl, (signed long)0);
  atomic_init (&pa->sl, (signed long)1);
  atomic_init (&pa->sl, (signed long)__LONG_MAX__);

  atomic_init (&pa->ul, (unsigned long)0);
  atomic_init (&pa->ul, (unsigned long)1);
  atomic_init (&pa->ul, (unsigned long)__LONG_MAX__);

  atomic_init (&pa->sll, (signed long long)0);
  atomic_init (&pa->sll, (signed long long)1);
  atomic_init (&pa->sll, (signed long long)__LONG_LONG_MAX__);

  atomic_init (&pa->ull, (unsigned long long)0);
  atomic_init (&pa->ull, (unsigned long long)1);
  atomic_init (&pa->ull, (unsigned long long)__LONG_LONG_MAX__); 

  atomic_init (&pa->sz, 0);
  atomic_init (&pa->sz, 1);
  atomic_init (&pa->sz, __SIZE_MAX__); 
}

/* Exercise the atomic_init() macro with an lvalue argument.  */

void atomic_init_lval (struct Atomic *pa, const struct Value *pv)
{
  atomic_init (&pa->b, pv->b);
  atomic_init (&pa->c, pv->c);
  atomic_init (&pa->sc, pv->sc);
  atomic_init (&pa->uc, pv->uc);
  atomic_init (&pa->ss, pv->ss);
  atomic_init (&pa->us, pv->us);
  atomic_init (&pa->si, pv->si);
  atomic_init (&pa->ui, pv->ui); 
  atomic_init (&pa->sl, pv->sl);
  atomic_init (&pa->ul, pv->ul);
  atomic_init (&pa->sll, pv->sll);
  atomic_init (&pa->ull, pv->ull);
  atomic_init (&pa->sz, pv->sz);
}

// SLATE-FILECHECK-FLAVOR gcc
// SLATE-FILECHECK-STD DEFAULT c11
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
// DEFAULT-NEXT:     type @type0 atomic_bool = bool;
// DEFAULT-NEXT:     type @type1 atomic_char = i8;
// DEFAULT-NEXT:     type @type2 atomic_schar = i8;
// DEFAULT-NEXT:     type @type3 atomic_uchar = u8;
// DEFAULT-NEXT:     type @type4 atomic_short = i16;
// DEFAULT-NEXT:     type @type5 atomic_ushort = u16;
// DEFAULT-NEXT:     type @type6 atomic_int = i32;
// DEFAULT-NEXT:     type @type7 atomic_uint = u32;
// DEFAULT-NEXT:     type @type8 atomic_long = i64;
// DEFAULT-NEXT:     type @type9 atomic_ulong = u64;
// DEFAULT-NEXT:     type @type10 atomic_llong = i64;
// DEFAULT-NEXT:     type @type11 atomic_ullong = u64;
// DEFAULT-NEXT:     type @type12 atomic_size_t = u64;
// DEFAULT-NEXT:     type @type13 Atomic = struct {
// DEFAULT-NEXT:         field0 b: volatile atomic bool;
// DEFAULT-NEXT:         field1 c: volatile atomic i8;
// DEFAULT-NEXT:         field2 sc: volatile atomic i8;
// DEFAULT-NEXT:         field3 uc: volatile atomic u8;
// DEFAULT-NEXT:         field4 ss: volatile atomic i16;
// DEFAULT-NEXT:         field5 us: volatile atomic u16;
// DEFAULT-NEXT:         field6 si: volatile atomic i32;
// DEFAULT-NEXT:         field7 ui: volatile atomic u32;
// DEFAULT-NEXT:         field8 sl: volatile atomic i64;
// DEFAULT-NEXT:         field9 ul: volatile atomic u64;
// DEFAULT-NEXT:         field10 sll: volatile atomic i64;
// DEFAULT-NEXT:         field11 ull: volatile atomic u64;
// DEFAULT-NEXT:         field12 sz: volatile atomic u64;
// DEFAULT-NEXT:     } [size=56, align=8, offsets=[0, 1, 2, 3, 4, 6, 8, 12, 16, 24, 32, 40, 48]];
// DEFAULT-NEXT:     type @type14 Value = struct {
// DEFAULT-NEXT:         field0 b: bool;
// DEFAULT-NEXT:         field1 c: i8;
// DEFAULT-NEXT:         field2 sc: i8;
// DEFAULT-NEXT:         field3 uc: u8;
// DEFAULT-NEXT:         field4 ss: i16;
// DEFAULT-NEXT:         field5 us: u16;
// DEFAULT-NEXT:         field6 si: i32;
// DEFAULT-NEXT:         field7 ui: u32;
// DEFAULT-NEXT:         field8 sl: i64;
// DEFAULT-NEXT:         field9 ul: u64;
// DEFAULT-NEXT:         field10 sll: i64;
// DEFAULT-NEXT:         field11 ull: u64;
// DEFAULT-NEXT:         field12 sz: u64;
// DEFAULT-NEXT:     } [size=56, align=8, offsets=[0, 1, 2, 3, 4, 6, 8, 12, 16, 24, 32, 40, 48]];
// DEFAULT-NEXT:     fn %15 @atomic_init_lit(%16 pa: ptr<@type13>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %17 __atomic_store_ptr: ptr<volatile atomic bool> [storage=automatic] = addr_of<ptr<volatile atomic bool>>(field0(deref(read<ptr<@type13>>(%16))));
// DEFAULT-NEXT:             let %18 __atomic_store_tmp: bool [storage=automatic] = ne<i32, reason=assign>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             write<bool, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic bool>>(%17)), read<bool>(deref(addr_of<ptr<bool>>(%18))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %19 __atomic_store_ptr: ptr<volatile atomic bool> [storage=automatic] = addr_of<ptr<volatile atomic bool>>(field0(deref(read<ptr<@type13>>(%16))));
// DEFAULT-NEXT:             let %20 __atomic_store_tmp: bool [storage=automatic] = ne<i32, reason=assign>(const<i32>(1), const<i32>(0));
// DEFAULT-NEXT:             write<bool, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic bool>>(%19)), read<bool>(deref(addr_of<ptr<bool>>(%20))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %21 __atomic_store_ptr: ptr<volatile atomic i8> [storage=automatic] = addr_of<ptr<volatile atomic i8>>(field1(deref(read<ptr<@type13>>(%16))));
// DEFAULT-NEXT:             let %22 __atomic_store_tmp: i8 [storage=automatic] = truncate<i8, reason=assign, fits=always>(const<i32>(120));
// DEFAULT-NEXT:             write<i8, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic i8>>(%21)), read<i8>(deref(addr_of<ptr<i8>>(%22))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %23 __atomic_store_ptr: ptr<volatile atomic i8> [storage=automatic] = addr_of<ptr<volatile atomic i8>>(field1(deref(read<ptr<@type13>>(%16))));
// DEFAULT-NEXT:             let %24 __atomic_store_tmp: i8 [storage=automatic] = truncate<i8, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:             write<i8, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic i8>>(%23)), read<i8>(deref(addr_of<ptr<i8>>(%24))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %25 __atomic_store_ptr: ptr<volatile atomic i8> [storage=automatic] = addr_of<ptr<volatile atomic i8>>(field1(deref(read<ptr<@type13>>(%16))));
// DEFAULT-NEXT:             let %26 __atomic_store_tmp: i8 [storage=automatic] = truncate<i8, reason=assign, fits=always>(const<i32>(1));
// DEFAULT-NEXT:             write<i8, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic i8>>(%25)), read<i8>(deref(addr_of<ptr<i8>>(%26))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %27 __atomic_store_ptr: ptr<volatile atomic i8> [storage=automatic] = addr_of<ptr<volatile atomic i8>>(field1(deref(read<ptr<@type13>>(%16))));
// DEFAULT-NEXT:             let %28 __atomic_store_tmp: i8 [storage=automatic] = truncate<i8, reason=assign, fits=unknown>(const<i32>(255));
// DEFAULT-NEXT:             write<i8, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic i8>>(%27)), read<i8>(deref(addr_of<ptr<i8>>(%28))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %29 __atomic_store_ptr: ptr<volatile atomic i8> [storage=automatic] = addr_of<ptr<volatile atomic i8>>(field2(deref(read<ptr<@type13>>(%16))));
// DEFAULT-NEXT:             let %30 __atomic_store_tmp: i8 [storage=automatic] = truncate<i8, reason=explicit, fits=always>(const<i32>(120));
// DEFAULT-NEXT:             write<i8, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic i8>>(%29)), read<i8>(deref(addr_of<ptr<i8>>(%30))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %31 __atomic_store_ptr: ptr<volatile atomic i8> [storage=automatic] = addr_of<ptr<volatile atomic i8>>(field2(deref(read<ptr<@type13>>(%16))));
// DEFAULT-NEXT:             let %32 __atomic_store_tmp: i8 [storage=automatic] = truncate<i8, reason=explicit, fits=always>(const<i32>(0));
// DEFAULT-NEXT:             write<i8, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic i8>>(%31)), read<i8>(deref(addr_of<ptr<i8>>(%32))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %33 __atomic_store_ptr: ptr<volatile atomic i8> [storage=automatic] = addr_of<ptr<volatile atomic i8>>(field2(deref(read<ptr<@type13>>(%16))));
// DEFAULT-NEXT:             let %34 __atomic_store_tmp: i8 [storage=automatic] = truncate<i8, reason=explicit, fits=always>(const<i32>(1));
// DEFAULT-NEXT:             write<i8, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic i8>>(%33)), read<i8>(deref(addr_of<ptr<i8>>(%34))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %35 __atomic_store_ptr: ptr<volatile atomic i8> [storage=automatic] = addr_of<ptr<volatile atomic i8>>(field2(deref(read<ptr<@type13>>(%16))));
// DEFAULT-NEXT:             let %36 __atomic_store_tmp: i8 [storage=automatic] = truncate<i8, reason=explicit, fits=always>(const<i32>(127));
// DEFAULT-NEXT:             write<i8, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic i8>>(%35)), read<i8>(deref(addr_of<ptr<i8>>(%36))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %37 __atomic_store_ptr: ptr<volatile atomic u8> [storage=automatic] = addr_of<ptr<volatile atomic u8>>(field3(deref(read<ptr<@type13>>(%16))));
// DEFAULT-NEXT:             let %38 __atomic_store_tmp: u8 [storage=automatic] = reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(120)));
// DEFAULT-NEXT:             write<u8, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic u8>>(%37)), read<u8>(deref(addr_of<ptr<u8>>(%38))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %39 __atomic_store_ptr: ptr<volatile atomic u8> [storage=automatic] = addr_of<ptr<volatile atomic u8>>(field3(deref(read<ptr<@type13>>(%16))));
// DEFAULT-NEXT:             let %40 __atomic_store_tmp: u8 [storage=automatic] = reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             write<u8, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic u8>>(%39)), read<u8>(deref(addr_of<ptr<u8>>(%40))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %41 __atomic_store_ptr: ptr<volatile atomic u8> [storage=automatic] = addr_of<ptr<volatile atomic u8>>(field3(deref(read<ptr<@type13>>(%16))));
// DEFAULT-NEXT:             let %42 __atomic_store_tmp: u8 [storage=automatic] = reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:             write<u8, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic u8>>(%41)), read<u8>(deref(addr_of<ptr<u8>>(%42))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %43 __atomic_store_ptr: ptr<volatile atomic i8> [storage=automatic] = addr_of<ptr<volatile atomic i8>>(field2(deref(read<ptr<@type13>>(%16))));
// DEFAULT-NEXT:             let %44 __atomic_store_tmp: i8 [storage=automatic] = reinterpret<i8, reason=assign, fits=unknown>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(127))));
// DEFAULT-NEXT:             write<i8, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic i8>>(%43)), read<i8>(deref(addr_of<ptr<i8>>(%44))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %45 __atomic_store_ptr: ptr<volatile atomic i16> [storage=automatic] = addr_of<ptr<volatile atomic i16>>(field4(deref(read<ptr<@type13>>(%16))));
// DEFAULT-NEXT:             let %46 __atomic_store_tmp: i16 [storage=automatic] = truncate<i16, reason=explicit, fits=always>(const<i32>(0));
// DEFAULT-NEXT:             write<i16, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic i16>>(%45)), read<i16>(deref(addr_of<ptr<i16>>(%46))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %47 __atomic_store_ptr: ptr<volatile atomic i16> [storage=automatic] = addr_of<ptr<volatile atomic i16>>(field4(deref(read<ptr<@type13>>(%16))));
// DEFAULT-NEXT:             let %48 __atomic_store_tmp: i16 [storage=automatic] = truncate<i16, reason=explicit, fits=always>(const<i32>(1));
// DEFAULT-NEXT:             write<i16, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic i16>>(%47)), read<i16>(deref(addr_of<ptr<i16>>(%48))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %49 __atomic_store_ptr: ptr<volatile atomic i16> [storage=automatic] = addr_of<ptr<volatile atomic i16>>(field4(deref(read<ptr<@type13>>(%16))));
// DEFAULT-NEXT:             let %50 __atomic_store_tmp: i16 [storage=automatic] = truncate<i16, reason=explicit, fits=always>(const<i32>(32767));
// DEFAULT-NEXT:             write<i16, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic i16>>(%49)), read<i16>(deref(addr_of<ptr<i16>>(%50))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %51 __atomic_store_ptr: ptr<volatile atomic u16> [storage=automatic] = addr_of<ptr<volatile atomic u16>>(field5(deref(read<ptr<@type13>>(%16))));
// DEFAULT-NEXT:             let %52 __atomic_store_tmp: u16 [storage=automatic] = reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             write<u16, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic u16>>(%51)), read<u16>(deref(addr_of<ptr<u16>>(%52))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %53 __atomic_store_ptr: ptr<volatile atomic u16> [storage=automatic] = addr_of<ptr<volatile atomic u16>>(field5(deref(read<ptr<@type13>>(%16))));
// DEFAULT-NEXT:             let %54 __atomic_store_tmp: u16 [storage=automatic] = reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:             write<u16, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic u16>>(%53)), read<u16>(deref(addr_of<ptr<u16>>(%54))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %55 __atomic_store_ptr: ptr<volatile atomic u16> [storage=automatic] = addr_of<ptr<volatile atomic u16>>(field5(deref(read<ptr<@type13>>(%16))));
// DEFAULT-NEXT:             let %56 __atomic_store_tmp: u16 [storage=automatic] = reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(32767)));
// DEFAULT-NEXT:             write<u16, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic u16>>(%55)), read<u16>(deref(addr_of<ptr<u16>>(%56))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %57 __atomic_store_ptr: ptr<volatile atomic i32> [storage=automatic] = addr_of<ptr<volatile atomic i32>>(field6(deref(read<ptr<@type13>>(%16))));
// DEFAULT-NEXT:             let %58 __atomic_store_tmp: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             write<i32, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic i32>>(%57)), read<i32>(deref(addr_of<ptr<i32>>(%58))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %59 __atomic_store_ptr: ptr<volatile atomic i32> [storage=automatic] = addr_of<ptr<volatile atomic i32>>(field6(deref(read<ptr<@type13>>(%16))));
// DEFAULT-NEXT:             let %60 __atomic_store_tmp: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:             write<i32, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic i32>>(%59)), read<i32>(deref(addr_of<ptr<i32>>(%60))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %61 __atomic_store_ptr: ptr<volatile atomic i32> [storage=automatic] = addr_of<ptr<volatile atomic i32>>(field6(deref(read<ptr<@type13>>(%16))));
// DEFAULT-NEXT:             let %62 __atomic_store_tmp: i32 [storage=automatic] = const<i32>(2147483647);
// DEFAULT-NEXT:             write<i32, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic i32>>(%61)), read<i32>(deref(addr_of<ptr<i32>>(%62))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %63 __atomic_store_ptr: ptr<volatile atomic u32> [storage=automatic] = addr_of<ptr<volatile atomic u32>>(field7(deref(read<ptr<@type13>>(%16))));
// DEFAULT-NEXT:             let %64 __atomic_store_tmp: u32 [storage=automatic] = reinterpret<u32, reason=explicit, fits=always>(const<i32>(0));
// DEFAULT-NEXT:             write<u32, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic u32>>(%63)), read<u32>(deref(addr_of<ptr<u32>>(%64))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %65 __atomic_store_ptr: ptr<volatile atomic u32> [storage=automatic] = addr_of<ptr<volatile atomic u32>>(field7(deref(read<ptr<@type13>>(%16))));
// DEFAULT-NEXT:             let %66 __atomic_store_tmp: u32 [storage=automatic] = reinterpret<u32, reason=explicit, fits=always>(const<i32>(1));
// DEFAULT-NEXT:             write<u32, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic u32>>(%65)), read<u32>(deref(addr_of<ptr<u32>>(%66))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %67 __atomic_store_ptr: ptr<volatile atomic u32> [storage=automatic] = addr_of<ptr<volatile atomic u32>>(field7(deref(read<ptr<@type13>>(%16))));
// DEFAULT-NEXT:             let %68 __atomic_store_tmp: u32 [storage=automatic] = reinterpret<u32, reason=explicit, fits=always>(const<i32>(2147483647));
// DEFAULT-NEXT:             write<u32, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic u32>>(%67)), read<u32>(deref(addr_of<ptr<u32>>(%68))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %69 __atomic_store_ptr: ptr<volatile atomic i64> [storage=automatic] = addr_of<ptr<volatile atomic i64>>(field8(deref(read<ptr<@type13>>(%16))));
// DEFAULT-NEXT:             let %70 __atomic_store_tmp: i64 [storage=automatic] = widen<i64, reason=explicit>(const<i32>(0));
// DEFAULT-NEXT:             write<i64, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic i64>>(%69)), read<i64>(deref(addr_of<ptr<i64>>(%70))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %71 __atomic_store_ptr: ptr<volatile atomic i64> [storage=automatic] = addr_of<ptr<volatile atomic i64>>(field8(deref(read<ptr<@type13>>(%16))));
// DEFAULT-NEXT:             let %72 __atomic_store_tmp: i64 [storage=automatic] = widen<i64, reason=explicit>(const<i32>(1));
// DEFAULT-NEXT:             write<i64, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic i64>>(%71)), read<i64>(deref(addr_of<ptr<i64>>(%72))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %73 __atomic_store_ptr: ptr<volatile atomic i64> [storage=automatic] = addr_of<ptr<volatile atomic i64>>(field8(deref(read<ptr<@type13>>(%16))));
// DEFAULT-NEXT:             let %74 __atomic_store_tmp: i64 [storage=automatic] = const<i64>(9223372036854775807);
// DEFAULT-NEXT:             write<i64, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic i64>>(%73)), read<i64>(deref(addr_of<ptr<i64>>(%74))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %75 __atomic_store_ptr: ptr<volatile atomic u64> [storage=automatic] = addr_of<ptr<volatile atomic u64>>(field9(deref(read<ptr<@type13>>(%16))));
// DEFAULT-NEXT:             let %76 __atomic_store_tmp: u64 [storage=automatic] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)));
// DEFAULT-NEXT:             write<u64, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic u64>>(%75)), read<u64>(deref(addr_of<ptr<u64>>(%76))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %77 __atomic_store_ptr: ptr<volatile atomic u64> [storage=automatic] = addr_of<ptr<volatile atomic u64>>(field9(deref(read<ptr<@type13>>(%16))));
// DEFAULT-NEXT:             let %78 __atomic_store_tmp: u64 [storage=automatic] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)));
// DEFAULT-NEXT:             write<u64, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic u64>>(%77)), read<u64>(deref(addr_of<ptr<u64>>(%78))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %79 __atomic_store_ptr: ptr<volatile atomic u64> [storage=automatic] = addr_of<ptr<volatile atomic u64>>(field9(deref(read<ptr<@type13>>(%16))));
// DEFAULT-NEXT:             let %80 __atomic_store_tmp: u64 [storage=automatic] = reinterpret<u64, reason=explicit, fits=always>(const<i64>(9223372036854775807));
// DEFAULT-NEXT:             write<u64, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic u64>>(%79)), read<u64>(deref(addr_of<ptr<u64>>(%80))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %81 __atomic_store_ptr: ptr<volatile atomic i64> [storage=automatic] = addr_of<ptr<volatile atomic i64>>(field10(deref(read<ptr<@type13>>(%16))));
// DEFAULT-NEXT:             let %82 __atomic_store_tmp: i64 [storage=automatic] = widen<i64, reason=explicit>(const<i32>(0));
// DEFAULT-NEXT:             write<i64, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic i64>>(%81)), read<i64>(deref(addr_of<ptr<i64>>(%82))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %83 __atomic_store_ptr: ptr<volatile atomic i64> [storage=automatic] = addr_of<ptr<volatile atomic i64>>(field10(deref(read<ptr<@type13>>(%16))));
// DEFAULT-NEXT:             let %84 __atomic_store_tmp: i64 [storage=automatic] = widen<i64, reason=explicit>(const<i32>(1));
// DEFAULT-NEXT:             write<i64, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic i64>>(%83)), read<i64>(deref(addr_of<ptr<i64>>(%84))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %85 __atomic_store_ptr: ptr<volatile atomic i64> [storage=automatic] = addr_of<ptr<volatile atomic i64>>(field10(deref(read<ptr<@type13>>(%16))));
// DEFAULT-NEXT:             let %86 __atomic_store_tmp: i64 [storage=automatic] = const<i64>(9223372036854775807);
// DEFAULT-NEXT:             write<i64, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic i64>>(%85)), read<i64>(deref(addr_of<ptr<i64>>(%86))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %87 __atomic_store_ptr: ptr<volatile atomic u64> [storage=automatic] = addr_of<ptr<volatile atomic u64>>(field11(deref(read<ptr<@type13>>(%16))));
// DEFAULT-NEXT:             let %88 __atomic_store_tmp: u64 [storage=automatic] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)));
// DEFAULT-NEXT:             write<u64, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic u64>>(%87)), read<u64>(deref(addr_of<ptr<u64>>(%88))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %89 __atomic_store_ptr: ptr<volatile atomic u64> [storage=automatic] = addr_of<ptr<volatile atomic u64>>(field11(deref(read<ptr<@type13>>(%16))));
// DEFAULT-NEXT:             let %90 __atomic_store_tmp: u64 [storage=automatic] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)));
// DEFAULT-NEXT:             write<u64, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic u64>>(%89)), read<u64>(deref(addr_of<ptr<u64>>(%90))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %91 __atomic_store_ptr: ptr<volatile atomic u64> [storage=automatic] = addr_of<ptr<volatile atomic u64>>(field11(deref(read<ptr<@type13>>(%16))));
// DEFAULT-NEXT:             let %92 __atomic_store_tmp: u64 [storage=automatic] = reinterpret<u64, reason=explicit, fits=always>(const<i64>(9223372036854775807));
// DEFAULT-NEXT:             write<u64, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic u64>>(%91)), read<u64>(deref(addr_of<ptr<u64>>(%92))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %93 __atomic_store_ptr: ptr<volatile atomic u64> [storage=automatic] = addr_of<ptr<volatile atomic u64>>(field12(deref(read<ptr<@type13>>(%16))));
// DEFAULT-NEXT:             let %94 __atomic_store_tmp: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:             write<u64, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic u64>>(%93)), read<u64>(deref(addr_of<ptr<u64>>(%94))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %95 __atomic_store_ptr: ptr<volatile atomic u64> [storage=automatic] = addr_of<ptr<volatile atomic u64>>(field12(deref(read<ptr<@type13>>(%16))));
// DEFAULT-NEXT:             let %96 __atomic_store_tmp: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:             write<u64, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic u64>>(%95)), read<u64>(deref(addr_of<ptr<u64>>(%96))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %97 __atomic_store_ptr: ptr<volatile atomic u64> [storage=automatic] = addr_of<ptr<volatile atomic u64>>(field12(deref(read<ptr<@type13>>(%16))));
// DEFAULT-NEXT:             let %98 __atomic_store_tmp: u64 [storage=automatic] = const<u64>(18446744073709551615);
// DEFAULT-NEXT:             write<u64, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic u64>>(%97)), read<u64>(deref(addr_of<ptr<u64>>(%98))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %99 @atomic_init_lval(%100 pa: ptr<@type13>, %101 pv: ptr<const @type14>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %102 __atomic_store_ptr: ptr<volatile atomic bool> [storage=automatic] = addr_of<ptr<volatile atomic bool>>(field0(deref(read<ptr<@type13>>(%100))));
// DEFAULT-NEXT:             let %103 __atomic_store_tmp: bool [storage=automatic] = read<bool>(field0(deref(read<ptr<const @type14>>(%101))));
// DEFAULT-NEXT:             write<bool, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic bool>>(%102)), read<bool>(deref(addr_of<ptr<bool>>(%103))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %104 __atomic_store_ptr: ptr<volatile atomic i8> [storage=automatic] = addr_of<ptr<volatile atomic i8>>(field1(deref(read<ptr<@type13>>(%100))));
// DEFAULT-NEXT:             let %105 __atomic_store_tmp: i8 [storage=automatic] = read<i8>(field1(deref(read<ptr<const @type14>>(%101))));
// DEFAULT-NEXT:             write<i8, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic i8>>(%104)), read<i8>(deref(addr_of<ptr<i8>>(%105))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %106 __atomic_store_ptr: ptr<volatile atomic i8> [storage=automatic] = addr_of<ptr<volatile atomic i8>>(field2(deref(read<ptr<@type13>>(%100))));
// DEFAULT-NEXT:             let %107 __atomic_store_tmp: i8 [storage=automatic] = read<i8>(field2(deref(read<ptr<const @type14>>(%101))));
// DEFAULT-NEXT:             write<i8, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic i8>>(%106)), read<i8>(deref(addr_of<ptr<i8>>(%107))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %108 __atomic_store_ptr: ptr<volatile atomic u8> [storage=automatic] = addr_of<ptr<volatile atomic u8>>(field3(deref(read<ptr<@type13>>(%100))));
// DEFAULT-NEXT:             let %109 __atomic_store_tmp: u8 [storage=automatic] = read<u8>(field3(deref(read<ptr<const @type14>>(%101))));
// DEFAULT-NEXT:             write<u8, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic u8>>(%108)), read<u8>(deref(addr_of<ptr<u8>>(%109))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %110 __atomic_store_ptr: ptr<volatile atomic i16> [storage=automatic] = addr_of<ptr<volatile atomic i16>>(field4(deref(read<ptr<@type13>>(%100))));
// DEFAULT-NEXT:             let %111 __atomic_store_tmp: i16 [storage=automatic] = read<i16>(field4(deref(read<ptr<const @type14>>(%101))));
// DEFAULT-NEXT:             write<i16, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic i16>>(%110)), read<i16>(deref(addr_of<ptr<i16>>(%111))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %112 __atomic_store_ptr: ptr<volatile atomic u16> [storage=automatic] = addr_of<ptr<volatile atomic u16>>(field5(deref(read<ptr<@type13>>(%100))));
// DEFAULT-NEXT:             let %113 __atomic_store_tmp: u16 [storage=automatic] = read<u16>(field5(deref(read<ptr<const @type14>>(%101))));
// DEFAULT-NEXT:             write<u16, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic u16>>(%112)), read<u16>(deref(addr_of<ptr<u16>>(%113))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %114 __atomic_store_ptr: ptr<volatile atomic i32> [storage=automatic] = addr_of<ptr<volatile atomic i32>>(field6(deref(read<ptr<@type13>>(%100))));
// DEFAULT-NEXT:             let %115 __atomic_store_tmp: i32 [storage=automatic] = read<i32>(field6(deref(read<ptr<const @type14>>(%101))));
// DEFAULT-NEXT:             write<i32, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic i32>>(%114)), read<i32>(deref(addr_of<ptr<i32>>(%115))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %116 __atomic_store_ptr: ptr<volatile atomic u32> [storage=automatic] = addr_of<ptr<volatile atomic u32>>(field7(deref(read<ptr<@type13>>(%100))));
// DEFAULT-NEXT:             let %117 __atomic_store_tmp: u32 [storage=automatic] = read<u32>(field7(deref(read<ptr<const @type14>>(%101))));
// DEFAULT-NEXT:             write<u32, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic u32>>(%116)), read<u32>(deref(addr_of<ptr<u32>>(%117))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %118 __atomic_store_ptr: ptr<volatile atomic i64> [storage=automatic] = addr_of<ptr<volatile atomic i64>>(field8(deref(read<ptr<@type13>>(%100))));
// DEFAULT-NEXT:             let %119 __atomic_store_tmp: i64 [storage=automatic] = read<i64>(field8(deref(read<ptr<const @type14>>(%101))));
// DEFAULT-NEXT:             write<i64, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic i64>>(%118)), read<i64>(deref(addr_of<ptr<i64>>(%119))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %120 __atomic_store_ptr: ptr<volatile atomic u64> [storage=automatic] = addr_of<ptr<volatile atomic u64>>(field9(deref(read<ptr<@type13>>(%100))));
// DEFAULT-NEXT:             let %121 __atomic_store_tmp: u64 [storage=automatic] = read<u64>(field9(deref(read<ptr<const @type14>>(%101))));
// DEFAULT-NEXT:             write<u64, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic u64>>(%120)), read<u64>(deref(addr_of<ptr<u64>>(%121))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %122 __atomic_store_ptr: ptr<volatile atomic i64> [storage=automatic] = addr_of<ptr<volatile atomic i64>>(field10(deref(read<ptr<@type13>>(%100))));
// DEFAULT-NEXT:             let %123 __atomic_store_tmp: i64 [storage=automatic] = read<i64>(field10(deref(read<ptr<const @type14>>(%101))));
// DEFAULT-NEXT:             write<i64, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic i64>>(%122)), read<i64>(deref(addr_of<ptr<i64>>(%123))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %124 __atomic_store_ptr: ptr<volatile atomic u64> [storage=automatic] = addr_of<ptr<volatile atomic u64>>(field11(deref(read<ptr<@type13>>(%100))));
// DEFAULT-NEXT:             let %125 __atomic_store_tmp: u64 [storage=automatic] = read<u64>(field11(deref(read<ptr<const @type14>>(%101))));
// DEFAULT-NEXT:             write<u64, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic u64>>(%124)), read<u64>(deref(addr_of<ptr<u64>>(%125))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %126 __atomic_store_ptr: ptr<volatile atomic u64> [storage=automatic] = addr_of<ptr<volatile atomic u64>>(field12(deref(read<ptr<@type13>>(%100))));
// DEFAULT-NEXT:             let %127 __atomic_store_tmp: u64 [storage=automatic] = read<u64>(field12(deref(read<ptr<const @type14>>(%101))));
// DEFAULT-NEXT:             write<u64, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic u64>>(%126)), read<u64>(deref(addr_of<ptr<u64>>(%127))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
