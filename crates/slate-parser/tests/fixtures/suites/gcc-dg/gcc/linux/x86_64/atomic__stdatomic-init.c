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
// DEFAULT-NEXT:     type @type[[TYPE_atomic_bool:[0-9]+]] atomic_bool = bool;
// DEFAULT-NEXT:     type @type[[TYPE_atomic_char:[0-9]+]] atomic_char = i8;
// DEFAULT-NEXT:     type @type[[TYPE_atomic_schar:[0-9]+]] atomic_schar = i8;
// DEFAULT-NEXT:     type @type[[TYPE_atomic_uchar:[0-9]+]] atomic_uchar = u8;
// DEFAULT-NEXT:     type @type[[TYPE_atomic_short:[0-9]+]] atomic_short = i16;
// DEFAULT-NEXT:     type @type[[TYPE_atomic_ushort:[0-9]+]] atomic_ushort = u16;
// DEFAULT-NEXT:     type @type[[TYPE_atomic_int:[0-9]+]] atomic_int = i32;
// DEFAULT-NEXT:     type @type[[TYPE_atomic_uint:[0-9]+]] atomic_uint = u32;
// DEFAULT-NEXT:     type @type[[TYPE_atomic_long:[0-9]+]] atomic_long = i64;
// DEFAULT-NEXT:     type @type[[TYPE_atomic_ulong:[0-9]+]] atomic_ulong = u64;
// DEFAULT-NEXT:     type @type[[TYPE_atomic_llong:[0-9]+]] atomic_llong = i64;
// DEFAULT-NEXT:     type @type[[TYPE_atomic_ullong:[0-9]+]] atomic_ullong = u64;
// DEFAULT-NEXT:     type @type[[TYPE_atomic_size_t:[0-9]+]] atomic_size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_Atomic:[0-9]+]] Atomic = struct {
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
// DEFAULT-NEXT:     type @type[[TYPE_Value:[0-9]+]] Value = struct {
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
// DEFAULT-NEXT:     fn %[[VALUE_atomic_init_lit:[0-9]+]] @atomic_init_lit(%[[VALUE_pa:[0-9]+]] pa: ptr<@type[[TYPE_Atomic]]>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr:[0-9]+]] __atomic_store_ptr: ptr<volatile atomic bool> [storage=automatic] = addr_of<ptr<volatile atomic bool>>(field0(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))));
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp:[0-9]+]] __atomic_store_tmp: bool [storage=automatic] = ne<i32, reason=assign>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             write<bool, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic bool>>(%[[VALUE___atomic_store_ptr]])), read<bool>(deref(addr_of<ptr<bool>>(%[[VALUE___atomic_store_tmp]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_2:[0-9]+]] __atomic_store_ptr: ptr<volatile atomic bool> [storage=automatic] = addr_of<ptr<volatile atomic bool>>(field0(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))));
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_2:[0-9]+]] __atomic_store_tmp: bool [storage=automatic] = ne<i32, reason=assign>(const<i32>(1), const<i32>(0));
// DEFAULT-NEXT:             write<bool, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic bool>>(%[[VALUE___atomic_store_ptr_2]])), read<bool>(deref(addr_of<ptr<bool>>(%[[VALUE___atomic_store_tmp_2]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_3:[0-9]+]] __atomic_store_ptr: ptr<volatile atomic i8> [storage=automatic] = addr_of<ptr<volatile atomic i8>>(field1(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))));
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_3:[0-9]+]] __atomic_store_tmp: i8 [storage=automatic] = truncate<i8, reason=assign, fits=always>(const<i32>(120));
// DEFAULT-NEXT:             write<i8, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic i8>>(%[[VALUE___atomic_store_ptr_3]])), read<i8>(deref(addr_of<ptr<i8>>(%[[VALUE___atomic_store_tmp_3]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_4:[0-9]+]] __atomic_store_ptr: ptr<volatile atomic i8> [storage=automatic] = addr_of<ptr<volatile atomic i8>>(field1(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))));
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_4:[0-9]+]] __atomic_store_tmp: i8 [storage=automatic] = truncate<i8, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:             write<i8, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic i8>>(%[[VALUE___atomic_store_ptr_4]])), read<i8>(deref(addr_of<ptr<i8>>(%[[VALUE___atomic_store_tmp_4]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_5:[0-9]+]] __atomic_store_ptr: ptr<volatile atomic i8> [storage=automatic] = addr_of<ptr<volatile atomic i8>>(field1(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))));
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_5:[0-9]+]] __atomic_store_tmp: i8 [storage=automatic] = truncate<i8, reason=assign, fits=always>(const<i32>(1));
// DEFAULT-NEXT:             write<i8, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic i8>>(%[[VALUE___atomic_store_ptr_5]])), read<i8>(deref(addr_of<ptr<i8>>(%[[VALUE___atomic_store_tmp_5]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_6:[0-9]+]] __atomic_store_ptr: ptr<volatile atomic i8> [storage=automatic] = addr_of<ptr<volatile atomic i8>>(field1(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))));
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_6:[0-9]+]] __atomic_store_tmp: i8 [storage=automatic] = truncate<i8, reason=assign, fits=unknown>(const<i32>(255));
// DEFAULT-NEXT:             write<i8, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic i8>>(%[[VALUE___atomic_store_ptr_6]])), read<i8>(deref(addr_of<ptr<i8>>(%[[VALUE___atomic_store_tmp_6]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_7:[0-9]+]] __atomic_store_ptr: ptr<volatile atomic i8> [storage=automatic] = addr_of<ptr<volatile atomic i8>>(field2(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))));
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_7:[0-9]+]] __atomic_store_tmp: i8 [storage=automatic] = truncate<i8, reason=explicit, fits=always>(const<i32>(120));
// DEFAULT-NEXT:             write<i8, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic i8>>(%[[VALUE___atomic_store_ptr_7]])), read<i8>(deref(addr_of<ptr<i8>>(%[[VALUE___atomic_store_tmp_7]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_8:[0-9]+]] __atomic_store_ptr: ptr<volatile atomic i8> [storage=automatic] = addr_of<ptr<volatile atomic i8>>(field2(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))));
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_8:[0-9]+]] __atomic_store_tmp: i8 [storage=automatic] = truncate<i8, reason=explicit, fits=always>(const<i32>(0));
// DEFAULT-NEXT:             write<i8, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic i8>>(%[[VALUE___atomic_store_ptr_8]])), read<i8>(deref(addr_of<ptr<i8>>(%[[VALUE___atomic_store_tmp_8]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_9:[0-9]+]] __atomic_store_ptr: ptr<volatile atomic i8> [storage=automatic] = addr_of<ptr<volatile atomic i8>>(field2(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))));
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_9:[0-9]+]] __atomic_store_tmp: i8 [storage=automatic] = truncate<i8, reason=explicit, fits=always>(const<i32>(1));
// DEFAULT-NEXT:             write<i8, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic i8>>(%[[VALUE___atomic_store_ptr_9]])), read<i8>(deref(addr_of<ptr<i8>>(%[[VALUE___atomic_store_tmp_9]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_10:[0-9]+]] __atomic_store_ptr: ptr<volatile atomic i8> [storage=automatic] = addr_of<ptr<volatile atomic i8>>(field2(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))));
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_10:[0-9]+]] __atomic_store_tmp: i8 [storage=automatic] = truncate<i8, reason=explicit, fits=always>(const<i32>(127));
// DEFAULT-NEXT:             write<i8, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic i8>>(%[[VALUE___atomic_store_ptr_10]])), read<i8>(deref(addr_of<ptr<i8>>(%[[VALUE___atomic_store_tmp_10]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_11:[0-9]+]] __atomic_store_ptr: ptr<volatile atomic u8> [storage=automatic] = addr_of<ptr<volatile atomic u8>>(field3(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))));
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_11:[0-9]+]] __atomic_store_tmp: u8 [storage=automatic] = reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(120)));
// DEFAULT-NEXT:             write<u8, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic u8>>(%[[VALUE___atomic_store_ptr_11]])), read<u8>(deref(addr_of<ptr<u8>>(%[[VALUE___atomic_store_tmp_11]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_12:[0-9]+]] __atomic_store_ptr: ptr<volatile atomic u8> [storage=automatic] = addr_of<ptr<volatile atomic u8>>(field3(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))));
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_12:[0-9]+]] __atomic_store_tmp: u8 [storage=automatic] = reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             write<u8, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic u8>>(%[[VALUE___atomic_store_ptr_12]])), read<u8>(deref(addr_of<ptr<u8>>(%[[VALUE___atomic_store_tmp_12]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_13:[0-9]+]] __atomic_store_ptr: ptr<volatile atomic u8> [storage=automatic] = addr_of<ptr<volatile atomic u8>>(field3(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))));
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_13:[0-9]+]] __atomic_store_tmp: u8 [storage=automatic] = reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:             write<u8, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic u8>>(%[[VALUE___atomic_store_ptr_13]])), read<u8>(deref(addr_of<ptr<u8>>(%[[VALUE___atomic_store_tmp_13]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_14:[0-9]+]] __atomic_store_ptr: ptr<volatile atomic i8> [storage=automatic] = addr_of<ptr<volatile atomic i8>>(field2(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))));
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_14:[0-9]+]] __atomic_store_tmp: i8 [storage=automatic] = reinterpret<i8, reason=assign, fits=unknown>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(127))));
// DEFAULT-NEXT:             write<i8, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic i8>>(%[[VALUE___atomic_store_ptr_14]])), read<i8>(deref(addr_of<ptr<i8>>(%[[VALUE___atomic_store_tmp_14]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_15:[0-9]+]] __atomic_store_ptr: ptr<volatile atomic i16> [storage=automatic] = addr_of<ptr<volatile atomic i16>>(field4(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))));
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_15:[0-9]+]] __atomic_store_tmp: i16 [storage=automatic] = truncate<i16, reason=explicit, fits=always>(const<i32>(0));
// DEFAULT-NEXT:             write<i16, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic i16>>(%[[VALUE___atomic_store_ptr_15]])), read<i16>(deref(addr_of<ptr<i16>>(%[[VALUE___atomic_store_tmp_15]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_16:[0-9]+]] __atomic_store_ptr: ptr<volatile atomic i16> [storage=automatic] = addr_of<ptr<volatile atomic i16>>(field4(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))));
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_16:[0-9]+]] __atomic_store_tmp: i16 [storage=automatic] = truncate<i16, reason=explicit, fits=always>(const<i32>(1));
// DEFAULT-NEXT:             write<i16, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic i16>>(%[[VALUE___atomic_store_ptr_16]])), read<i16>(deref(addr_of<ptr<i16>>(%[[VALUE___atomic_store_tmp_16]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_17:[0-9]+]] __atomic_store_ptr: ptr<volatile atomic i16> [storage=automatic] = addr_of<ptr<volatile atomic i16>>(field4(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))));
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_17:[0-9]+]] __atomic_store_tmp: i16 [storage=automatic] = truncate<i16, reason=explicit, fits=always>(const<i32>(32767));
// DEFAULT-NEXT:             write<i16, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic i16>>(%[[VALUE___atomic_store_ptr_17]])), read<i16>(deref(addr_of<ptr<i16>>(%[[VALUE___atomic_store_tmp_17]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_18:[0-9]+]] __atomic_store_ptr: ptr<volatile atomic u16> [storage=automatic] = addr_of<ptr<volatile atomic u16>>(field5(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))));
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_18:[0-9]+]] __atomic_store_tmp: u16 [storage=automatic] = reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             write<u16, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic u16>>(%[[VALUE___atomic_store_ptr_18]])), read<u16>(deref(addr_of<ptr<u16>>(%[[VALUE___atomic_store_tmp_18]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_19:[0-9]+]] __atomic_store_ptr: ptr<volatile atomic u16> [storage=automatic] = addr_of<ptr<volatile atomic u16>>(field5(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))));
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_19:[0-9]+]] __atomic_store_tmp: u16 [storage=automatic] = reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:             write<u16, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic u16>>(%[[VALUE___atomic_store_ptr_19]])), read<u16>(deref(addr_of<ptr<u16>>(%[[VALUE___atomic_store_tmp_19]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_20:[0-9]+]] __atomic_store_ptr: ptr<volatile atomic u16> [storage=automatic] = addr_of<ptr<volatile atomic u16>>(field5(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))));
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_20:[0-9]+]] __atomic_store_tmp: u16 [storage=automatic] = reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(32767)));
// DEFAULT-NEXT:             write<u16, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic u16>>(%[[VALUE___atomic_store_ptr_20]])), read<u16>(deref(addr_of<ptr<u16>>(%[[VALUE___atomic_store_tmp_20]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_21:[0-9]+]] __atomic_store_ptr: ptr<volatile atomic i32> [storage=automatic] = addr_of<ptr<volatile atomic i32>>(field6(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))));
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_21:[0-9]+]] __atomic_store_tmp: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             write<i32, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic i32>>(%[[VALUE___atomic_store_ptr_21]])), read<i32>(deref(addr_of<ptr<i32>>(%[[VALUE___atomic_store_tmp_21]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_22:[0-9]+]] __atomic_store_ptr: ptr<volatile atomic i32> [storage=automatic] = addr_of<ptr<volatile atomic i32>>(field6(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))));
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_22:[0-9]+]] __atomic_store_tmp: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:             write<i32, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic i32>>(%[[VALUE___atomic_store_ptr_22]])), read<i32>(deref(addr_of<ptr<i32>>(%[[VALUE___atomic_store_tmp_22]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_23:[0-9]+]] __atomic_store_ptr: ptr<volatile atomic i32> [storage=automatic] = addr_of<ptr<volatile atomic i32>>(field6(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))));
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_23:[0-9]+]] __atomic_store_tmp: i32 [storage=automatic] = const<i32>(2147483647);
// DEFAULT-NEXT:             write<i32, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic i32>>(%[[VALUE___atomic_store_ptr_23]])), read<i32>(deref(addr_of<ptr<i32>>(%[[VALUE___atomic_store_tmp_23]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_24:[0-9]+]] __atomic_store_ptr: ptr<volatile atomic u32> [storage=automatic] = addr_of<ptr<volatile atomic u32>>(field7(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))));
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_24:[0-9]+]] __atomic_store_tmp: u32 [storage=automatic] = reinterpret<u32, reason=explicit, fits=always>(const<i32>(0));
// DEFAULT-NEXT:             write<u32, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic u32>>(%[[VALUE___atomic_store_ptr_24]])), read<u32>(deref(addr_of<ptr<u32>>(%[[VALUE___atomic_store_tmp_24]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_25:[0-9]+]] __atomic_store_ptr: ptr<volatile atomic u32> [storage=automatic] = addr_of<ptr<volatile atomic u32>>(field7(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))));
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_25:[0-9]+]] __atomic_store_tmp: u32 [storage=automatic] = reinterpret<u32, reason=explicit, fits=always>(const<i32>(1));
// DEFAULT-NEXT:             write<u32, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic u32>>(%[[VALUE___atomic_store_ptr_25]])), read<u32>(deref(addr_of<ptr<u32>>(%[[VALUE___atomic_store_tmp_25]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_26:[0-9]+]] __atomic_store_ptr: ptr<volatile atomic u32> [storage=automatic] = addr_of<ptr<volatile atomic u32>>(field7(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))));
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_26:[0-9]+]] __atomic_store_tmp: u32 [storage=automatic] = reinterpret<u32, reason=explicit, fits=always>(const<i32>(2147483647));
// DEFAULT-NEXT:             write<u32, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic u32>>(%[[VALUE___atomic_store_ptr_26]])), read<u32>(deref(addr_of<ptr<u32>>(%[[VALUE___atomic_store_tmp_26]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_27:[0-9]+]] __atomic_store_ptr: ptr<volatile atomic i64> [storage=automatic] = addr_of<ptr<volatile atomic i64>>(field8(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))));
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_27:[0-9]+]] __atomic_store_tmp: i64 [storage=automatic] = widen<i64, reason=explicit>(const<i32>(0));
// DEFAULT-NEXT:             write<i64, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic i64>>(%[[VALUE___atomic_store_ptr_27]])), read<i64>(deref(addr_of<ptr<i64>>(%[[VALUE___atomic_store_tmp_27]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_28:[0-9]+]] __atomic_store_ptr: ptr<volatile atomic i64> [storage=automatic] = addr_of<ptr<volatile atomic i64>>(field8(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))));
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_28:[0-9]+]] __atomic_store_tmp: i64 [storage=automatic] = widen<i64, reason=explicit>(const<i32>(1));
// DEFAULT-NEXT:             write<i64, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic i64>>(%[[VALUE___atomic_store_ptr_28]])), read<i64>(deref(addr_of<ptr<i64>>(%[[VALUE___atomic_store_tmp_28]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_29:[0-9]+]] __atomic_store_ptr: ptr<volatile atomic i64> [storage=automatic] = addr_of<ptr<volatile atomic i64>>(field8(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))));
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_29:[0-9]+]] __atomic_store_tmp: i64 [storage=automatic] = const<i64>(9223372036854775807);
// DEFAULT-NEXT:             write<i64, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic i64>>(%[[VALUE___atomic_store_ptr_29]])), read<i64>(deref(addr_of<ptr<i64>>(%[[VALUE___atomic_store_tmp_29]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_30:[0-9]+]] __atomic_store_ptr: ptr<volatile atomic u64> [storage=automatic] = addr_of<ptr<volatile atomic u64>>(field9(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))));
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_30:[0-9]+]] __atomic_store_tmp: u64 [storage=automatic] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)));
// DEFAULT-NEXT:             write<u64, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic u64>>(%[[VALUE___atomic_store_ptr_30]])), read<u64>(deref(addr_of<ptr<u64>>(%[[VALUE___atomic_store_tmp_30]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_31:[0-9]+]] __atomic_store_ptr: ptr<volatile atomic u64> [storage=automatic] = addr_of<ptr<volatile atomic u64>>(field9(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))));
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_31:[0-9]+]] __atomic_store_tmp: u64 [storage=automatic] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)));
// DEFAULT-NEXT:             write<u64, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic u64>>(%[[VALUE___atomic_store_ptr_31]])), read<u64>(deref(addr_of<ptr<u64>>(%[[VALUE___atomic_store_tmp_31]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_32:[0-9]+]] __atomic_store_ptr: ptr<volatile atomic u64> [storage=automatic] = addr_of<ptr<volatile atomic u64>>(field9(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))));
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_32:[0-9]+]] __atomic_store_tmp: u64 [storage=automatic] = reinterpret<u64, reason=explicit, fits=always>(const<i64>(9223372036854775807));
// DEFAULT-NEXT:             write<u64, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic u64>>(%[[VALUE___atomic_store_ptr_32]])), read<u64>(deref(addr_of<ptr<u64>>(%[[VALUE___atomic_store_tmp_32]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_33:[0-9]+]] __atomic_store_ptr: ptr<volatile atomic i64> [storage=automatic] = addr_of<ptr<volatile atomic i64>>(field10(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))));
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_33:[0-9]+]] __atomic_store_tmp: i64 [storage=automatic] = widen<i64, reason=explicit>(const<i32>(0));
// DEFAULT-NEXT:             write<i64, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic i64>>(%[[VALUE___atomic_store_ptr_33]])), read<i64>(deref(addr_of<ptr<i64>>(%[[VALUE___atomic_store_tmp_33]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_34:[0-9]+]] __atomic_store_ptr: ptr<volatile atomic i64> [storage=automatic] = addr_of<ptr<volatile atomic i64>>(field10(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))));
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_34:[0-9]+]] __atomic_store_tmp: i64 [storage=automatic] = widen<i64, reason=explicit>(const<i32>(1));
// DEFAULT-NEXT:             write<i64, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic i64>>(%[[VALUE___atomic_store_ptr_34]])), read<i64>(deref(addr_of<ptr<i64>>(%[[VALUE___atomic_store_tmp_34]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_35:[0-9]+]] __atomic_store_ptr: ptr<volatile atomic i64> [storage=automatic] = addr_of<ptr<volatile atomic i64>>(field10(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))));
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_35:[0-9]+]] __atomic_store_tmp: i64 [storage=automatic] = const<i64>(9223372036854775807);
// DEFAULT-NEXT:             write<i64, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic i64>>(%[[VALUE___atomic_store_ptr_35]])), read<i64>(deref(addr_of<ptr<i64>>(%[[VALUE___atomic_store_tmp_35]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_36:[0-9]+]] __atomic_store_ptr: ptr<volatile atomic u64> [storage=automatic] = addr_of<ptr<volatile atomic u64>>(field11(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))));
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_36:[0-9]+]] __atomic_store_tmp: u64 [storage=automatic] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)));
// DEFAULT-NEXT:             write<u64, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic u64>>(%[[VALUE___atomic_store_ptr_36]])), read<u64>(deref(addr_of<ptr<u64>>(%[[VALUE___atomic_store_tmp_36]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_37:[0-9]+]] __atomic_store_ptr: ptr<volatile atomic u64> [storage=automatic] = addr_of<ptr<volatile atomic u64>>(field11(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))));
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_37:[0-9]+]] __atomic_store_tmp: u64 [storage=automatic] = reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)));
// DEFAULT-NEXT:             write<u64, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic u64>>(%[[VALUE___atomic_store_ptr_37]])), read<u64>(deref(addr_of<ptr<u64>>(%[[VALUE___atomic_store_tmp_37]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_38:[0-9]+]] __atomic_store_ptr: ptr<volatile atomic u64> [storage=automatic] = addr_of<ptr<volatile atomic u64>>(field11(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))));
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_38:[0-9]+]] __atomic_store_tmp: u64 [storage=automatic] = reinterpret<u64, reason=explicit, fits=always>(const<i64>(9223372036854775807));
// DEFAULT-NEXT:             write<u64, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic u64>>(%[[VALUE___atomic_store_ptr_38]])), read<u64>(deref(addr_of<ptr<u64>>(%[[VALUE___atomic_store_tmp_38]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_39:[0-9]+]] __atomic_store_ptr: ptr<volatile atomic u64> [storage=automatic] = addr_of<ptr<volatile atomic u64>>(field12(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))));
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_39:[0-9]+]] __atomic_store_tmp: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:             write<u64, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic u64>>(%[[VALUE___atomic_store_ptr_39]])), read<u64>(deref(addr_of<ptr<u64>>(%[[VALUE___atomic_store_tmp_39]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_40:[0-9]+]] __atomic_store_ptr: ptr<volatile atomic u64> [storage=automatic] = addr_of<ptr<volatile atomic u64>>(field12(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))));
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_40:[0-9]+]] __atomic_store_tmp: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:             write<u64, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic u64>>(%[[VALUE___atomic_store_ptr_40]])), read<u64>(deref(addr_of<ptr<u64>>(%[[VALUE___atomic_store_tmp_40]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_41:[0-9]+]] __atomic_store_ptr: ptr<volatile atomic u64> [storage=automatic] = addr_of<ptr<volatile atomic u64>>(field12(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))));
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_41:[0-9]+]] __atomic_store_tmp: u64 [storage=automatic] = const<u64>(18446744073709551615);
// DEFAULT-NEXT:             write<u64, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic u64>>(%[[VALUE___atomic_store_ptr_41]])), read<u64>(deref(addr_of<ptr<u64>>(%[[VALUE___atomic_store_tmp_41]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_atomic_init_lval:[0-9]+]] @atomic_init_lval(%[[VALUE_pa_2:[0-9]+]] pa: ptr<@type[[TYPE_Atomic]]>, %[[VALUE_pv:[0-9]+]] pv: ptr<const @type[[TYPE_Value]]>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_42:[0-9]+]] __atomic_store_ptr: ptr<volatile atomic bool> [storage=automatic] = addr_of<ptr<volatile atomic bool>>(field0(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa_2]]))));
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_42:[0-9]+]] __atomic_store_tmp: bool [storage=automatic] = read<bool>(field0(deref(read<ptr<const @type[[TYPE_Value]]>>(%[[VALUE_pv]]))));
// DEFAULT-NEXT:             write<bool, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic bool>>(%[[VALUE___atomic_store_ptr_42]])), read<bool>(deref(addr_of<ptr<bool>>(%[[VALUE___atomic_store_tmp_42]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_43:[0-9]+]] __atomic_store_ptr: ptr<volatile atomic i8> [storage=automatic] = addr_of<ptr<volatile atomic i8>>(field1(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa_2]]))));
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_43:[0-9]+]] __atomic_store_tmp: i8 [storage=automatic] = read<i8>(field1(deref(read<ptr<const @type[[TYPE_Value]]>>(%[[VALUE_pv]]))));
// DEFAULT-NEXT:             write<i8, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic i8>>(%[[VALUE___atomic_store_ptr_43]])), read<i8>(deref(addr_of<ptr<i8>>(%[[VALUE___atomic_store_tmp_43]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_44:[0-9]+]] __atomic_store_ptr: ptr<volatile atomic i8> [storage=automatic] = addr_of<ptr<volatile atomic i8>>(field2(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa_2]]))));
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_44:[0-9]+]] __atomic_store_tmp: i8 [storage=automatic] = read<i8>(field2(deref(read<ptr<const @type[[TYPE_Value]]>>(%[[VALUE_pv]]))));
// DEFAULT-NEXT:             write<i8, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic i8>>(%[[VALUE___atomic_store_ptr_44]])), read<i8>(deref(addr_of<ptr<i8>>(%[[VALUE___atomic_store_tmp_44]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_45:[0-9]+]] __atomic_store_ptr: ptr<volatile atomic u8> [storage=automatic] = addr_of<ptr<volatile atomic u8>>(field3(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa_2]]))));
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_45:[0-9]+]] __atomic_store_tmp: u8 [storage=automatic] = read<u8>(field3(deref(read<ptr<const @type[[TYPE_Value]]>>(%[[VALUE_pv]]))));
// DEFAULT-NEXT:             write<u8, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic u8>>(%[[VALUE___atomic_store_ptr_45]])), read<u8>(deref(addr_of<ptr<u8>>(%[[VALUE___atomic_store_tmp_45]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_46:[0-9]+]] __atomic_store_ptr: ptr<volatile atomic i16> [storage=automatic] = addr_of<ptr<volatile atomic i16>>(field4(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa_2]]))));
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_46:[0-9]+]] __atomic_store_tmp: i16 [storage=automatic] = read<i16>(field4(deref(read<ptr<const @type[[TYPE_Value]]>>(%[[VALUE_pv]]))));
// DEFAULT-NEXT:             write<i16, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic i16>>(%[[VALUE___atomic_store_ptr_46]])), read<i16>(deref(addr_of<ptr<i16>>(%[[VALUE___atomic_store_tmp_46]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_47:[0-9]+]] __atomic_store_ptr: ptr<volatile atomic u16> [storage=automatic] = addr_of<ptr<volatile atomic u16>>(field5(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa_2]]))));
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_47:[0-9]+]] __atomic_store_tmp: u16 [storage=automatic] = read<u16>(field5(deref(read<ptr<const @type[[TYPE_Value]]>>(%[[VALUE_pv]]))));
// DEFAULT-NEXT:             write<u16, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic u16>>(%[[VALUE___atomic_store_ptr_47]])), read<u16>(deref(addr_of<ptr<u16>>(%[[VALUE___atomic_store_tmp_47]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_48:[0-9]+]] __atomic_store_ptr: ptr<volatile atomic i32> [storage=automatic] = addr_of<ptr<volatile atomic i32>>(field6(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa_2]]))));
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_48:[0-9]+]] __atomic_store_tmp: i32 [storage=automatic] = read<i32>(field6(deref(read<ptr<const @type[[TYPE_Value]]>>(%[[VALUE_pv]]))));
// DEFAULT-NEXT:             write<i32, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic i32>>(%[[VALUE___atomic_store_ptr_48]])), read<i32>(deref(addr_of<ptr<i32>>(%[[VALUE___atomic_store_tmp_48]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_49:[0-9]+]] __atomic_store_ptr: ptr<volatile atomic u32> [storage=automatic] = addr_of<ptr<volatile atomic u32>>(field7(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa_2]]))));
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_49:[0-9]+]] __atomic_store_tmp: u32 [storage=automatic] = read<u32>(field7(deref(read<ptr<const @type[[TYPE_Value]]>>(%[[VALUE_pv]]))));
// DEFAULT-NEXT:             write<u32, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic u32>>(%[[VALUE___atomic_store_ptr_49]])), read<u32>(deref(addr_of<ptr<u32>>(%[[VALUE___atomic_store_tmp_49]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_50:[0-9]+]] __atomic_store_ptr: ptr<volatile atomic i64> [storage=automatic] = addr_of<ptr<volatile atomic i64>>(field8(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa_2]]))));
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_50:[0-9]+]] __atomic_store_tmp: i64 [storage=automatic] = read<i64>(field8(deref(read<ptr<const @type[[TYPE_Value]]>>(%[[VALUE_pv]]))));
// DEFAULT-NEXT:             write<i64, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic i64>>(%[[VALUE___atomic_store_ptr_50]])), read<i64>(deref(addr_of<ptr<i64>>(%[[VALUE___atomic_store_tmp_50]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_51:[0-9]+]] __atomic_store_ptr: ptr<volatile atomic u64> [storage=automatic] = addr_of<ptr<volatile atomic u64>>(field9(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa_2]]))));
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_51:[0-9]+]] __atomic_store_tmp: u64 [storage=automatic] = read<u64>(field9(deref(read<ptr<const @type[[TYPE_Value]]>>(%[[VALUE_pv]]))));
// DEFAULT-NEXT:             write<u64, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic u64>>(%[[VALUE___atomic_store_ptr_51]])), read<u64>(deref(addr_of<ptr<u64>>(%[[VALUE___atomic_store_tmp_51]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_52:[0-9]+]] __atomic_store_ptr: ptr<volatile atomic i64> [storage=automatic] = addr_of<ptr<volatile atomic i64>>(field10(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa_2]]))));
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_52:[0-9]+]] __atomic_store_tmp: i64 [storage=automatic] = read<i64>(field10(deref(read<ptr<const @type[[TYPE_Value]]>>(%[[VALUE_pv]]))));
// DEFAULT-NEXT:             write<i64, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic i64>>(%[[VALUE___atomic_store_ptr_52]])), read<i64>(deref(addr_of<ptr<i64>>(%[[VALUE___atomic_store_tmp_52]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_53:[0-9]+]] __atomic_store_ptr: ptr<volatile atomic u64> [storage=automatic] = addr_of<ptr<volatile atomic u64>>(field11(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa_2]]))));
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_53:[0-9]+]] __atomic_store_tmp: u64 [storage=automatic] = read<u64>(field11(deref(read<ptr<const @type[[TYPE_Value]]>>(%[[VALUE_pv]]))));
// DEFAULT-NEXT:             write<u64, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic u64>>(%[[VALUE___atomic_store_ptr_53]])), read<u64>(deref(addr_of<ptr<u64>>(%[[VALUE___atomic_store_tmp_53]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_54:[0-9]+]] __atomic_store_ptr: ptr<volatile atomic u64> [storage=automatic] = addr_of<ptr<volatile atomic u64>>(field12(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa_2]]))));
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_54:[0-9]+]] __atomic_store_tmp: u64 [storage=automatic] = read<u64>(field12(deref(read<ptr<const @type[[TYPE_Value]]>>(%[[VALUE_pv]]))));
// DEFAULT-NEXT:             write<u64, volatile, atomic=relaxed>(deref(read<ptr<volatile atomic u64>>(%[[VALUE___atomic_store_ptr_54]])), read<u64>(deref(addr_of<ptr<u64>>(%[[VALUE___atomic_store_tmp_54]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
