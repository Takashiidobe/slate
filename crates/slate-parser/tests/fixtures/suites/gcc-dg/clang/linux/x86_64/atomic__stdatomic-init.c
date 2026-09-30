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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
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
// DEFAULT-NEXT:         write<bool, volatile>(deref(addr_of<ptr<volatile atomic bool>>(field0(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))))), ne<i32, reason=arg>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:         write<bool, volatile>(deref(addr_of<ptr<volatile atomic bool>>(field0(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))))), ne<i32, reason=arg>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:         write<i8, volatile>(deref(addr_of<ptr<volatile atomic i8>>(field1(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))))), truncate<i8, reason=arg, fits=always>(const<i32>(120)));
// DEFAULT-NEXT:         write<i8, volatile>(deref(addr_of<ptr<volatile atomic i8>>(field1(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))))), truncate<i8, reason=arg, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i8, volatile>(deref(addr_of<ptr<volatile atomic i8>>(field1(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))))), truncate<i8, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<i8, volatile>(deref(addr_of<ptr<volatile atomic i8>>(field1(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))))), truncate<i8, reason=arg, fits=unknown>(const<i32>(255)));
// DEFAULT-NEXT:         write<i8, volatile>(deref(addr_of<ptr<volatile atomic i8>>(field2(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))))), truncate<i8, reason=explicit, fits=always>(const<i32>(120)));
// DEFAULT-NEXT:         write<i8, volatile>(deref(addr_of<ptr<volatile atomic i8>>(field2(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))))), truncate<i8, reason=explicit, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i8, volatile>(deref(addr_of<ptr<volatile atomic i8>>(field2(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))))), truncate<i8, reason=explicit, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<i8, volatile>(deref(addr_of<ptr<volatile atomic i8>>(field2(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))))), truncate<i8, reason=explicit, fits=always>(const<i32>(127)));
// DEFAULT-NEXT:         write<u8, volatile>(deref(addr_of<ptr<volatile atomic u8>>(field3(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))))), reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(120))));
// DEFAULT-NEXT:         write<u8, volatile>(deref(addr_of<ptr<volatile atomic u8>>(field3(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))))), reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         write<u8, volatile>(deref(addr_of<ptr<volatile atomic u8>>(field3(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))))), reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         write<i8, volatile>(deref(addr_of<ptr<volatile atomic i8>>(field2(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))))), reinterpret<i8, reason=arg, fits=unknown>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(127)))));
// DEFAULT-NEXT:         write<i16, volatile>(deref(addr_of<ptr<volatile atomic i16>>(field4(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))))), truncate<i16, reason=explicit, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i16, volatile>(deref(addr_of<ptr<volatile atomic i16>>(field4(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))))), truncate<i16, reason=explicit, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<i16, volatile>(deref(addr_of<ptr<volatile atomic i16>>(field4(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))))), truncate<i16, reason=explicit, fits=always>(const<i32>(32767)));
// DEFAULT-NEXT:         write<u16, volatile>(deref(addr_of<ptr<volatile atomic u16>>(field5(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))))), reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         write<u16, volatile>(deref(addr_of<ptr<volatile atomic u16>>(field5(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))))), reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         write<u16, volatile>(deref(addr_of<ptr<volatile atomic u16>>(field5(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))))), reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(32767))));
// DEFAULT-NEXT:         write<i32, volatile>(deref(addr_of<ptr<volatile atomic i32>>(field6(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))))), const<i32>(0));
// DEFAULT-NEXT:         write<i32, volatile>(deref(addr_of<ptr<volatile atomic i32>>(field6(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))))), const<i32>(1));
// DEFAULT-NEXT:         write<i32, volatile>(deref(addr_of<ptr<volatile atomic i32>>(field6(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))))), const<i32>(2147483647));
// DEFAULT-NEXT:         write<u32, volatile>(deref(addr_of<ptr<volatile atomic u32>>(field7(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))))), reinterpret<u32, reason=explicit, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<u32, volatile>(deref(addr_of<ptr<volatile atomic u32>>(field7(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))))), reinterpret<u32, reason=explicit, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32, volatile>(deref(addr_of<ptr<volatile atomic u32>>(field7(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))))), reinterpret<u32, reason=explicit, fits=always>(const<i32>(2147483647)));
// DEFAULT-NEXT:         write<i64, volatile>(deref(addr_of<ptr<volatile atomic i64>>(field8(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))))), widen<i64, reason=explicit>(const<i32>(0)));
// DEFAULT-NEXT:         write<i64, volatile>(deref(addr_of<ptr<volatile atomic i64>>(field8(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))))), widen<i64, reason=explicit>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64, volatile>(deref(addr_of<ptr<volatile atomic i64>>(field8(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))))), const<i64>(9223372036854775807));
// DEFAULT-NEXT:         write<u64, volatile>(deref(addr_of<ptr<volatile atomic u64>>(field9(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))));
// DEFAULT-NEXT:         write<u64, volatile>(deref(addr_of<ptr<volatile atomic u64>>(field9(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))));
// DEFAULT-NEXT:         write<u64, volatile>(deref(addr_of<ptr<volatile atomic u64>>(field9(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))))), reinterpret<u64, reason=explicit, fits=always>(const<i64>(9223372036854775807)));
// DEFAULT-NEXT:         write<i64, volatile>(deref(addr_of<ptr<volatile atomic i64>>(field10(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))))), widen<i64, reason=explicit>(const<i32>(0)));
// DEFAULT-NEXT:         write<i64, volatile>(deref(addr_of<ptr<volatile atomic i64>>(field10(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))))), widen<i64, reason=explicit>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64, volatile>(deref(addr_of<ptr<volatile atomic i64>>(field10(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))))), const<i64>(9223372036854775807));
// DEFAULT-NEXT:         write<u64, volatile>(deref(addr_of<ptr<volatile atomic u64>>(field11(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))));
// DEFAULT-NEXT:         write<u64, volatile>(deref(addr_of<ptr<volatile atomic u64>>(field11(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))));
// DEFAULT-NEXT:         write<u64, volatile>(deref(addr_of<ptr<volatile atomic u64>>(field11(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))))), reinterpret<u64, reason=explicit, fits=always>(const<i64>(9223372036854775807)));
// DEFAULT-NEXT:         write<u64, volatile>(deref(addr_of<ptr<volatile atomic u64>>(field12(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         write<u64, volatile>(deref(addr_of<ptr<volatile atomic u64>>(field12(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         write<u64, volatile>(deref(addr_of<ptr<volatile atomic u64>>(field12(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa]]))))), const<u64>(18446744073709551615));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_atomic_init_lval:[0-9]+]] @atomic_init_lval(%[[VALUE_pa_2:[0-9]+]] pa: ptr<@type[[TYPE_Atomic]]>, %[[VALUE_pv:[0-9]+]] pv: ptr<const @type[[TYPE_Value]]>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<bool, volatile>(deref(addr_of<ptr<volatile atomic bool>>(field0(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa_2]]))))), read<bool>(field0(deref(read<ptr<const @type[[TYPE_Value]]>>(%[[VALUE_pv]])))));
// DEFAULT-NEXT:         write<i8, volatile>(deref(addr_of<ptr<volatile atomic i8>>(field1(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa_2]]))))), read<i8>(field1(deref(read<ptr<const @type[[TYPE_Value]]>>(%[[VALUE_pv]])))));
// DEFAULT-NEXT:         write<i8, volatile>(deref(addr_of<ptr<volatile atomic i8>>(field2(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa_2]]))))), read<i8>(field2(deref(read<ptr<const @type[[TYPE_Value]]>>(%[[VALUE_pv]])))));
// DEFAULT-NEXT:         write<u8, volatile>(deref(addr_of<ptr<volatile atomic u8>>(field3(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa_2]]))))), read<u8>(field3(deref(read<ptr<const @type[[TYPE_Value]]>>(%[[VALUE_pv]])))));
// DEFAULT-NEXT:         write<i16, volatile>(deref(addr_of<ptr<volatile atomic i16>>(field4(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa_2]]))))), read<i16>(field4(deref(read<ptr<const @type[[TYPE_Value]]>>(%[[VALUE_pv]])))));
// DEFAULT-NEXT:         write<u16, volatile>(deref(addr_of<ptr<volatile atomic u16>>(field5(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa_2]]))))), read<u16>(field5(deref(read<ptr<const @type[[TYPE_Value]]>>(%[[VALUE_pv]])))));
// DEFAULT-NEXT:         write<i32, volatile>(deref(addr_of<ptr<volatile atomic i32>>(field6(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa_2]]))))), read<i32>(field6(deref(read<ptr<const @type[[TYPE_Value]]>>(%[[VALUE_pv]])))));
// DEFAULT-NEXT:         write<u32, volatile>(deref(addr_of<ptr<volatile atomic u32>>(field7(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa_2]]))))), read<u32>(field7(deref(read<ptr<const @type[[TYPE_Value]]>>(%[[VALUE_pv]])))));
// DEFAULT-NEXT:         write<i64, volatile>(deref(addr_of<ptr<volatile atomic i64>>(field8(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa_2]]))))), read<i64>(field8(deref(read<ptr<const @type[[TYPE_Value]]>>(%[[VALUE_pv]])))));
// DEFAULT-NEXT:         write<u64, volatile>(deref(addr_of<ptr<volatile atomic u64>>(field9(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa_2]]))))), read<u64>(field9(deref(read<ptr<const @type[[TYPE_Value]]>>(%[[VALUE_pv]])))));
// DEFAULT-NEXT:         write<i64, volatile>(deref(addr_of<ptr<volatile atomic i64>>(field10(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa_2]]))))), read<i64>(field10(deref(read<ptr<const @type[[TYPE_Value]]>>(%[[VALUE_pv]])))));
// DEFAULT-NEXT:         write<u64, volatile>(deref(addr_of<ptr<volatile atomic u64>>(field11(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa_2]]))))), read<u64>(field11(deref(read<ptr<const @type[[TYPE_Value]]>>(%[[VALUE_pv]])))));
// DEFAULT-NEXT:         write<u64, volatile>(deref(addr_of<ptr<volatile atomic u64>>(field12(deref(read<ptr<@type[[TYPE_Atomic]]>>(%[[VALUE_pa_2]]))))), read<u64>(field12(deref(read<ptr<const @type[[TYPE_Value]]>>(%[[VALUE_pv]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
