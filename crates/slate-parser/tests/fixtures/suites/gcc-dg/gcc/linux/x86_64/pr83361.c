/* PR rtl-optimization/83361 */
/* { dg-do compile } */
/* { dg-options "-O2 -freorder-blocks-and-partition -Wno-div-by-zero" } */

#include <limits.h>

int yz;

void
tq (int z3)
{
  unsigned long long int n8 = (unsigned long long int)INT_MAX + 1;
  int *ey = &yz;

  if (yz == 0)
    {
      int bc;

      yz = 1;
      while (yz != 0)
        {
          *ey *= bc;
          n8 = !!(1 / ((unsigned long long int)yz == n8));
          ey = &z3;
        }

      while (z3 != 0)
        {
        }
    }

  z3 = (n8 != 0) && (*ey != 0);
  z3 = yz / z3;
  if (z3 < 0)
    {
      if (yz != 0)
        yz = 0;
      yz /= 0;
    }
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
// DEFAULT-NEXT:     global %0 yz: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %1 @tq(%2 z3: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %3 n8: u64 [storage=automatic] = add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2147483647))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         let %4 ey: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(%0);
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%0), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %5 bc: i32 [storage=automatic];
// DEFAULT-NEXT:                 write<i32>(%0, const<i32>(1));
// DEFAULT-NEXT:                 while %6 ne<i32>(read<i32>(%0), const<i32>(0))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %8: ptr<i32> [synthetic] = read<ptr<i32>>(%4);
// DEFAULT-NEXT:                         let %9: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%8)));
// DEFAULT-NEXT:                         let %10: i32 [synthetic] = mul<i32, overflow=ub>(read<i32>(%9), read<i32>(%5));
// DEFAULT-NEXT:                         write<i32>(deref(read<ptr<i32>>(%8)), read<i32>(%10));
// DEFAULT-NEXT:                         write<u64>(%3, from_bool<u64, reason=assign>(not<bool>(not<bool>(ne<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(1), from_bool<i32, reason=promotion>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(read<i32>(%0))), read<u64>(%3)))), const<i32>(0))))));
// DEFAULT-NEXT:                         write<ptr<i32>>(%4, addr_of<ptr<i32>>(%2));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while %7 ne<i32>(read<i32>(%2), const<i32>(0))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<i32>(%2, from_bool<i32, reason=assign>(logical_and<bool>(ne<u64>(read<u64>(%3), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), ne<i32>(read<i32>(deref(read<ptr<i32>>(%4))), const<i32>(0)))));
// DEFAULT-NEXT:         write<i32>(%2, div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%0), read<i32>(%2)));
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%2), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%0), const<i32>(0))
// DEFAULT-NEXT:                     write<i32>(%0, const<i32>(0));
// DEFAULT-NEXT:                 let %11: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                 let %12: i32 [synthetic] = div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%11), const<i32>(0));
// DEFAULT-NEXT:                 write<i32>(%0, read<i32>(%12));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
