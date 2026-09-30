/* Test for modifying and taking addresses of compound literals whose
   variably modified types involve typeof.  */
/* Origin: Joseph Myers <joseph@codesourcery.com> */
/* { dg-do run } */
/* { dg-options "-std=gnu99" } */

#include <stdarg.h>

extern void exit (int);
extern void abort (void);

int a[1];

void
f1 (void)
{
  int i = 0;
  int (**p)[1] = &(typeof (++i, (int (*)[i])a)){&a};
  if (*p != &a)
    abort ();
  if (i != 1)
    abort ();
}

void
f2 (void)
{
  int i = 0;
  (typeof (++i, (int (*)[i])a)){&a} = 0;
  if (i != 1)
    abort ();
}

void
f3 (void)
{
  int i = 0;
  (typeof (++i, (int (*)[i])a)){&a} += 1;
  if (i != 1)
    abort ();
}

void
f4 (void)
{
  int i = 0;
  --(typeof (++i, (int (*)[i])a)){&a + 1};
  if (i != 1)
    abort ();
}

void
f5 (void)
{
  int i = 0;
  (typeof (++i, (int (*)[i])a)){&a}++;
  if (i != 1)
    abort ();
}

int
main (void)
{
  f1 ();
  f2 ();
  f3 ();
  f4 ();
  f5 ();
  exit (0);
}

// SLATE-FILECHECK-STD DEFAULT gnu99
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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: array<i32, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_f1:[0-9]+]] @f1() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<ptr<array<i32, 1>>> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_i]])));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: ptr<vla<i32, %[[VALUE3]]>> [synthetic] = pointer_cast<ptr<vla<i32, %[[VALUE3]]>>, reason=explicit>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_a]]));
// DEFAULT-NEXT:         write<ptr<ptr<array<i32, 1>>>>(%[[VALUE_p]], pointer_cast<ptr<ptr<array<i32, 1>>>, reason=assign>(addr_of<ptr<ptr<vla<i32, %[[VALUE3]]>>>>(compound_literal %[[VALUE5:[0-9]+]] [storage=automatic] = pointer_cast<ptr<vla<i32, %[[VALUE3]]>>, reason=assign>(addr_of<ptr<array<i32, 1>>>(%[[VALUE_a]])))));
// DEFAULT-NEXT:         if ne<ptr<array<i32, 1>>>(read<ptr<array<i32, 1>>>(deref(read<ptr<ptr<array<i32, 1>>>>(%[[VALUE_p]]))), addr_of<ptr<array<i32, 1>>>(%[[VALUE_a]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_i]]), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f2:[0-9]+]] @f2() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i_2:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE6]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE7]]));
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_i_2]])));
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: ptr<vla<i32, %[[VALUE8]]>> [synthetic] = pointer_cast<ptr<vla<i32, %[[VALUE8]]>>, reason=explicit>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_a]]));
// DEFAULT-NEXT:         write<ptr<vla<i32, %[[VALUE8]]>>>(compound_literal %[[VALUE10:[0-9]+]] [storage=automatic] = pointer_cast<ptr<vla<i32, %[[VALUE8]]>>, reason=assign>(addr_of<ptr<array<i32, 1>>>(%[[VALUE_a]])), null<ptr<vla<i32, %[[VALUE8]]>>>);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f3:[0-9]+]] @f3() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i_3:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_3]]);
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE11]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i_3]], read<i32>(%[[VALUE12]]));
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_i_3]])));
// DEFAULT-NEXT:         let %[[VALUE14:[0-9]+]]: ptr<vla<i32, %[[VALUE13]]>> [synthetic] = pointer_cast<ptr<vla<i32, %[[VALUE13]]>>, reason=explicit>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_a]]));
// DEFAULT-NEXT:         let %[[VALUE15:[0-9]+]]: ptr<vla<i32, %[[VALUE13]]>> [synthetic] = read<ptr<vla<i32, %[[VALUE13]]>>>(compound_literal %[[VALUE16:[0-9]+]] [storage=automatic] = pointer_cast<ptr<vla<i32, %[[VALUE13]]>>, reason=assign>(addr_of<ptr<array<i32, 1>>>(%[[VALUE_a]])));
// DEFAULT-NEXT:         let %[[VALUE17:[0-9]+]]: ptr<vla<i32, %[[VALUE13]]>> [synthetic] = ptr_offset<ptr<vla<i32, %[[VALUE13]]>>, subtract=false, element=vla<i32, %[[VALUE13]]>, overflow=ub>(read<ptr<vla<i32, %[[VALUE13]]>>>(%[[VALUE15]]), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<vla<i32, %[[VALUE13]]>>>(compound_literal %[[VALUE16]] [storage=automatic] = pointer_cast<ptr<vla<i32, %[[VALUE13]]>>, reason=assign>(addr_of<ptr<array<i32, 1>>>(%[[VALUE_a]])), read<ptr<vla<i32, %[[VALUE13]]>>>(%[[VALUE17]]));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_i_3]]), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f4:[0-9]+]] @f4() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i_4:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE18:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_4]]);
// DEFAULT-NEXT:         let %[[VALUE19:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE18]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i_4]], read<i32>(%[[VALUE19]]));
// DEFAULT-NEXT:         let %[[VALUE20:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_i_4]])));
// DEFAULT-NEXT:         let %[[VALUE21:[0-9]+]]: ptr<vla<i32, %[[VALUE20]]>> [synthetic] = pointer_cast<ptr<vla<i32, %[[VALUE20]]>>, reason=explicit>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_a]]));
// DEFAULT-NEXT:         let %[[VALUE22:[0-9]+]]: ptr<vla<i32, %[[VALUE20]]>> [synthetic] = read<ptr<vla<i32, %[[VALUE20]]>>>(compound_literal %[[VALUE23:[0-9]+]] [storage=automatic] = pointer_cast<ptr<vla<i32, %[[VALUE20]]>>, reason=assign>(ptr_offset<ptr<array<i32, 1>>, subtract=false, element=array<i32, 1>, overflow=ub>(addr_of<ptr<array<i32, 1>>>(%[[VALUE_a]]), const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE24:[0-9]+]]: ptr<vla<i32, %[[VALUE20]]>> [synthetic] = ptr_offset<ptr<vla<i32, %[[VALUE20]]>>, subtract=true, element=vla<i32, %[[VALUE20]]>, overflow=ub>(read<ptr<vla<i32, %[[VALUE20]]>>>(%[[VALUE22]]), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<vla<i32, %[[VALUE20]]>>>(compound_literal %[[VALUE23]] [storage=automatic] = pointer_cast<ptr<vla<i32, %[[VALUE20]]>>, reason=assign>(ptr_offset<ptr<array<i32, 1>>, subtract=false, element=array<i32, 1>, overflow=ub>(addr_of<ptr<array<i32, 1>>>(%[[VALUE_a]]), const<i32>(1))), read<ptr<vla<i32, %[[VALUE20]]>>>(%[[VALUE24]]));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_i_4]]), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f5:[0-9]+]] @f5() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i_5:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE25:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_5]]);
// DEFAULT-NEXT:         let %[[VALUE26:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE25]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i_5]], read<i32>(%[[VALUE26]]));
// DEFAULT-NEXT:         let %[[VALUE27:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_i_5]])));
// DEFAULT-NEXT:         let %[[VALUE28:[0-9]+]]: ptr<vla<i32, %[[VALUE27]]>> [synthetic] = pointer_cast<ptr<vla<i32, %[[VALUE27]]>>, reason=explicit>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_a]]));
// DEFAULT-NEXT:         let %[[VALUE29:[0-9]+]]: ptr<vla<i32, %[[VALUE27]]>> [synthetic] = read<ptr<vla<i32, %[[VALUE27]]>>>(compound_literal %[[VALUE30:[0-9]+]] [storage=automatic] = pointer_cast<ptr<vla<i32, %[[VALUE27]]>>, reason=assign>(addr_of<ptr<array<i32, 1>>>(%[[VALUE_a]])));
// DEFAULT-NEXT:         let %[[VALUE31:[0-9]+]]: ptr<vla<i32, %[[VALUE27]]>> [synthetic] = ptr_offset<ptr<vla<i32, %[[VALUE27]]>>, subtract=false, element=vla<i32, %[[VALUE27]]>, overflow=ub>(read<ptr<vla<i32, %[[VALUE27]]>>>(%[[VALUE29]]), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<vla<i32, %[[VALUE27]]>>>(compound_literal %[[VALUE30]] [storage=automatic] = pointer_cast<ptr<vla<i32, %[[VALUE27]]>>, reason=assign>(addr_of<ptr<array<i32, 1>>>(%[[VALUE_a]])), read<ptr<vla<i32, %[[VALUE27]]>>>(%[[VALUE31]]));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_i_5]]), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_f1]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_f2]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_f3]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_f4]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_f5]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
