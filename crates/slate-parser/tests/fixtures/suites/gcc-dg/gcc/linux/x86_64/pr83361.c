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
// DEFAULT-NEXT:     global %[[VALUE_yz:[0-9]+]] yz: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_tq:[0-9]+]] @tq(%[[VALUE_z3:[0-9]+]] z3: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_n8:[0-9]+]] n8: u64 [storage=automatic] = add<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2147483647))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE_ey:[0-9]+]] ey: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(%[[VALUE_yz]]);
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%[[VALUE_yz]]), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_bc:[0-9]+]] bc: i32 [storage=automatic];
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_yz]], const<i32>(1));
// DEFAULT-NEXT:                 while %[[VALUE0:[0-9]+]] ne<i32>(read<i32>(%[[VALUE_yz]]), const<i32>(0))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE1:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(%[[VALUE_ey]]);
// DEFAULT-NEXT:                         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%[[VALUE1]])));
// DEFAULT-NEXT:                         let %[[VALUE3:[0-9]+]]: i32 [synthetic] = mul<i32, overflow=ub>(read<i32>(%[[VALUE2]]), read<i32>(%[[VALUE_bc]]));
// DEFAULT-NEXT:                         write<i32>(deref(read<ptr<i32>>(%[[VALUE1]])), read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:                         write<u64>(%[[VALUE_n8]], from_bool<u64, reason=assign>(not<bool>(not<bool>(ne<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(1), from_bool<i32, reason=promotion>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(read<i32>(%[[VALUE_yz]]))), read<u64>(%[[VALUE_n8]])))), const<i32>(0))))));
// DEFAULT-NEXT:                         write<ptr<i32>>(%[[VALUE_ey]], addr_of<ptr<i32>>(%[[VALUE_z3]]));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while %[[VALUE4:[0-9]+]] ne<i32>(read<i32>(%[[VALUE_z3]]), const<i32>(0))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<i32>(%[[VALUE_z3]], from_bool<i32, reason=assign>(logical_and<bool>(ne<u64>(read<u64>(%[[VALUE_n8]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), ne<i32>(read<i32>(deref(read<ptr<i32>>(%[[VALUE_ey]]))), const<i32>(0)))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_z3]], div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_yz]]), read<i32>(%[[VALUE_z3]])));
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%[[VALUE_z3]]), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%[[VALUE_yz]]), const<i32>(0))
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_yz]], const<i32>(0));
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_yz]]);
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: i32 [synthetic] = div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE5]]), const<i32>(0));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_yz]], read<i32>(%[[VALUE6]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
