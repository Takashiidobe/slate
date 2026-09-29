/* Test we're able use __atomic_fetch_* where possible and verify
   we generate correct code.  */
/* { dg-do run } */
/* { dg-options "-std=c11 -pedantic-errors -fdump-tree-original" } */
/* { dg-xfail-run-if "PR97444: stack atomics" { nvptx*-*-* } }*/

#include <stdatomic.h>
#include <limits.h>

extern void abort (void);

#define TEST_TYPE(TYPE, MAX)				\
  do							\
    {							\
      struct S { char a[(MAX) + 1]; };			\
      TYPE t = 1;					\
      struct S a[2][2];					\
      struct S (*_Atomic p)[2] = &a[0];			\
      p += t;						\
      if (p != &a[1])					\
	abort ();					\
      p -= t;						\
      if (p != &a[0])					\
	abort ();					\
    }							\
  while (0)

int
main (void)
{
  TEST_TYPE (signed char, UCHAR_MAX);
  TEST_TYPE (signed short, USHRT_MAX);
}

/* { dg-final { scan-tree-dump-not "__atomic_compare_exchange" "original" } } */

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
// DEFAULT-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// DEFAULT-NEXT:         field0 a: array<i8, 256>;
// DEFAULT-NEXT:     } [size=256, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S_2:[0-9]+]] S = struct {
// DEFAULT-NEXT:         field0 a: array<i8, 65536>;
// DEFAULT-NEXT:     } [size=65536, align=1, offsets=[0]];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         do %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_t:[0-9]+]] t: i8 [storage=automatic] = truncate<i8, reason=assign, fits=always>(const<i32>(1));
// DEFAULT-NEXT:                 let %[[VALUE_a:[0-9]+]] a: array<array<@type[[TYPE_S]], 2>, 2> [storage=automatic] [align=16];
// DEFAULT-NEXT:                 let %[[VALUE_p:[0-9]+]] p: atomic ptr<array<@type[[TYPE_S]], 2>> [storage=automatic] = addr_of<ptr<array<@type[[TYPE_S]], 2>>>(deref(ptr_offset<ptr<array<@type[[TYPE_S]], 2>>, subtract=false, element=array<@type[[TYPE_S]], 2>, overflow=ub>(array_decay<ptr<array<@type[[TYPE_S]], 2>>, length=Some(2)>(%[[VALUE_a]]), const<i32>(0))));
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: ptr<array<@type[[TYPE_S]], 2>> [synthetic] = update<ptr<array<@type[[TYPE_S]], 2>>, result=new, atomic=seq_cst>(%[[VALUE_p]], ptr_offset<ptr<array<@type[[TYPE_S]], 2>>, subtract=false, element=array<@type[[TYPE_S]], 2>, overflow=ub>(old<ptr<array<@type[[TYPE_S]], 2>>>, widen<i32, reason=promotion>(read<i8>(%[[VALUE_t]]))));
// DEFAULT-NEXT:                 if ne<ptr<array<@type[[TYPE_S]], 2>>>(read<ptr<array<@type[[TYPE_S]], 2>>, atomic=seq_cst>(%[[VALUE_p]]), addr_of<ptr<array<@type[[TYPE_S]], 2>>>(deref(ptr_offset<ptr<array<@type[[TYPE_S]], 2>>, subtract=false, element=array<@type[[TYPE_S]], 2>, overflow=ub>(array_decay<ptr<array<@type[[TYPE_S]], 2>>, length=Some(2)>(%[[VALUE_a]]), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: ptr<array<@type[[TYPE_S]], 2>> [synthetic] = update<ptr<array<@type[[TYPE_S]], 2>>, result=new, atomic=seq_cst>(%[[VALUE_p]], ptr_offset<ptr<array<@type[[TYPE_S]], 2>>, subtract=true, element=array<@type[[TYPE_S]], 2>, overflow=ub>(old<ptr<array<@type[[TYPE_S]], 2>>>, widen<i32, reason=promotion>(read<i8>(%[[VALUE_t]]))));
// DEFAULT-NEXT:                 if ne<ptr<array<@type[[TYPE_S]], 2>>>(read<ptr<array<@type[[TYPE_S]], 2>>, atomic=seq_cst>(%[[VALUE_p]]), addr_of<ptr<array<@type[[TYPE_S]], 2>>>(deref(ptr_offset<ptr<array<@type[[TYPE_S]], 2>>, subtract=false, element=array<@type[[TYPE_S]], 2>, overflow=ub>(array_decay<ptr<array<@type[[TYPE_S]], 2>>, length=Some(2)>(%[[VALUE_a]]), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_t_2:[0-9]+]] t: i16 [storage=automatic] = truncate<i16, reason=assign, fits=always>(const<i32>(1));
// DEFAULT-NEXT:                 let %[[VALUE_a_2:[0-9]+]] a: array<array<@type[[TYPE_S_2]], 2>, 2> [storage=automatic] [align=16];
// DEFAULT-NEXT:                 let %[[VALUE_p_2:[0-9]+]] p: atomic ptr<array<@type[[TYPE_S_2]], 2>> [storage=automatic] = addr_of<ptr<array<@type[[TYPE_S_2]], 2>>>(deref(ptr_offset<ptr<array<@type[[TYPE_S_2]], 2>>, subtract=false, element=array<@type[[TYPE_S_2]], 2>, overflow=ub>(array_decay<ptr<array<@type[[TYPE_S_2]], 2>>, length=Some(2)>(%[[VALUE_a_2]]), const<i32>(0))));
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: ptr<array<@type[[TYPE_S_2]], 2>> [synthetic] = update<ptr<array<@type[[TYPE_S_2]], 2>>, result=new, atomic=seq_cst>(%[[VALUE_p_2]], ptr_offset<ptr<array<@type[[TYPE_S_2]], 2>>, subtract=false, element=array<@type[[TYPE_S_2]], 2>, overflow=ub>(old<ptr<array<@type[[TYPE_S_2]], 2>>>, widen<i32, reason=promotion>(read<i16>(%[[VALUE_t_2]]))));
// DEFAULT-NEXT:                 if ne<ptr<array<@type[[TYPE_S_2]], 2>>>(read<ptr<array<@type[[TYPE_S_2]], 2>>, atomic=seq_cst>(%[[VALUE_p_2]]), addr_of<ptr<array<@type[[TYPE_S_2]], 2>>>(deref(ptr_offset<ptr<array<@type[[TYPE_S_2]], 2>>, subtract=false, element=array<@type[[TYPE_S_2]], 2>, overflow=ub>(array_decay<ptr<array<@type[[TYPE_S_2]], 2>>, length=Some(2)>(%[[VALUE_a_2]]), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: ptr<array<@type[[TYPE_S_2]], 2>> [synthetic] = update<ptr<array<@type[[TYPE_S_2]], 2>>, result=new, atomic=seq_cst>(%[[VALUE_p_2]], ptr_offset<ptr<array<@type[[TYPE_S_2]], 2>>, subtract=true, element=array<@type[[TYPE_S_2]], 2>, overflow=ub>(old<ptr<array<@type[[TYPE_S_2]], 2>>>, widen<i32, reason=promotion>(read<i16>(%[[VALUE_t_2]]))));
// DEFAULT-NEXT:                 if ne<ptr<array<@type[[TYPE_S_2]], 2>>>(read<ptr<array<@type[[TYPE_S_2]], 2>>, atomic=seq_cst>(%[[VALUE_p_2]]), addr_of<ptr<array<@type[[TYPE_S_2]], 2>>>(deref(ptr_offset<ptr<array<@type[[TYPE_S_2]], 2>>, subtract=false, element=array<@type[[TYPE_S_2]], 2>, overflow=ub>(array_decay<ptr<array<@type[[TYPE_S_2]], 2>>, length=Some(2)>(%[[VALUE_a_2]]), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
