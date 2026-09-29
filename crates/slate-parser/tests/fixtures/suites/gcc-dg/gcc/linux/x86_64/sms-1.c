/* The same test as loop-3c.c.  It failed on ia64
   due to not handling of subreg in the lhs that is fixed.  */
/* { dg-do run } */
/* { dg-options "-O2 -fmodulo-sched -fmodulo-sched-allow-regmoves -fdump-rtl-sms" } */


#include <limits.h>
extern void abort (void);

void * a[255];

__attribute__ ((noinline))
void
f (int m)
{
  int i;
  int sh = 0x100;
  i = m;
  do
    {
      a[sh >>= 1] = ((unsigned)i << 3)  + (char*)a;
      i += 4;
    }
  while (i < INT_MAX/2 + 1 + 4 * 4);
}

int
main ()
{
  a[0x10] = 0;
  a[0x08] = 0;
  f (INT_MAX/2 + INT_MAX/4 + 2);
  if (a[0x10] || a[0x08])
    abort ();
  a[0x10] = 0;
  a[0x08] = 0;
  f (INT_MAX/2 + 1);
  if (! a[0x10] || a[0x08])
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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: array<ptr<void>, 255> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_m:[0-9]+]] m: i32) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_sh:[0-9]+]] sh: i32 [storage=automatic] = const<i32>(256);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE_m]]));
// DEFAULT-NEXT:         do %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_sh]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_sh]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 write<ptr<void>>(deref(ptr_offset<ptr<ptr<void>>, subtract=false, element=ptr<void>, overflow=ub>(array_decay<ptr<ptr<void>>, length=Some(255)>(%[[VALUE_a]]), read<i32>(%[[VALUE2]]))), pointer_cast<ptr<void>, reason=assign>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(pointer_cast<ptr<i8>, reason=explicit>(array_decay<ptr<ptr<void>>, length=Some(255)>(%[[VALUE_a]])), shl<u32, overflow=wrap, amount_out_of_range=ub>(reinterpret<u32, reason=explicit, fits=unknown>(read<i32>(%[[VALUE_i]])), const<i32>(3)))));
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE3]]), const<i32>(4));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while lt<i32>(read<i32>(%[[VALUE_i]]), add<i32, overflow=ub>(add<i32, overflow=ub>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(2147483647), const<i32>(2)), const<i32>(1)), mul<i32, overflow=ub>(const<i32>(4), const<i32>(4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<ptr<void>>(deref(ptr_offset<ptr<ptr<void>>, subtract=false, element=ptr<void>, overflow=ub>(array_decay<ptr<ptr<void>>, length=Some(255)>(%[[VALUE_a]]), const<i32>(16))), null<ptr<void>>);
// DEFAULT-NEXT:         write<ptr<void>>(deref(ptr_offset<ptr<ptr<void>>, subtract=false, element=ptr<void>, overflow=ub>(array_decay<ptr<ptr<void>>, length=Some(255)>(%[[VALUE_a]]), const<i32>(8))), null<ptr<void>>);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_f]], add<i32, overflow=ub>(add<i32, overflow=ub>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(2147483647), const<i32>(2)), div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(2147483647), const<i32>(4))), const<i32>(2)));
// DEFAULT-NEXT:         if logical_or<bool>(ne<ptr<void>>(read<ptr<void>>(deref(ptr_offset<ptr<ptr<void>>, subtract=false, element=ptr<void>, overflow=ub>(array_decay<ptr<ptr<void>>, length=Some(255)>(%[[VALUE_a]]), const<i32>(16)))), null<ptr<void>>), ne<ptr<void>>(read<ptr<void>>(deref(ptr_offset<ptr<ptr<void>>, subtract=false, element=ptr<void>, overflow=ub>(array_decay<ptr<ptr<void>>, length=Some(255)>(%[[VALUE_a]]), const<i32>(8)))), null<ptr<void>>))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<ptr<void>>(deref(ptr_offset<ptr<ptr<void>>, subtract=false, element=ptr<void>, overflow=ub>(array_decay<ptr<ptr<void>>, length=Some(255)>(%[[VALUE_a]]), const<i32>(16))), null<ptr<void>>);
// DEFAULT-NEXT:         write<ptr<void>>(deref(ptr_offset<ptr<ptr<void>>, subtract=false, element=ptr<void>, overflow=ub>(array_decay<ptr<ptr<void>>, length=Some(255)>(%[[VALUE_a]]), const<i32>(8))), null<ptr<void>>);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_f]], add<i32, overflow=ub>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(2147483647), const<i32>(2)), const<i32>(1)));
// DEFAULT-NEXT:         if logical_or<bool>(not<bool>(ne<ptr<void>>(read<ptr<void>>(deref(ptr_offset<ptr<ptr<void>>, subtract=false, element=ptr<void>, overflow=ub>(array_decay<ptr<ptr<void>>, length=Some(255)>(%[[VALUE_a]]), const<i32>(16)))), null<ptr<void>>)), ne<ptr<void>>(read<ptr<void>>(deref(ptr_offset<ptr<ptr<void>>, subtract=false, element=ptr<void>, overflow=ub>(array_decay<ptr<ptr<void>>, length=Some(255)>(%[[VALUE_a]]), const<i32>(8)))), null<ptr<void>>))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
