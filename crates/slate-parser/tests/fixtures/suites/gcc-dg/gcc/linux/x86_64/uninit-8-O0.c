/* Uninitialized variable warning tests...
   Inspired by part of optabs.c:expand_binop.
   May be the same as uninit-1.c.  */

/* { dg-do compile } */
/* { dg-options "-Wuninitialized" } */

#include <limits.h>

void
add_bignums (int *out, int *x, int *y)
{
    int p, sum;
    int carry; /* { dg-bogus "carry" "uninitialized variable warning" } */

    p = 0;
    for (; *x; x++, y++, out++, p++)
    {
	if (p)
	    sum = *x + *y + carry;
	else
	    sum = *x + *y;

	if (sum < 0)
	{
	    carry = 1;
	    sum -= INT_MAX;
	}
	else
	    carry = 0;
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
// DEFAULT-NEXT:     fn %[[VALUE_add_bignums:[0-9]+]] @add_bignums(%[[VALUE_out:[0-9]+]] out: ptr<i32>, %[[VALUE_x:[0-9]+]] x: ptr<i32>, %[[VALUE_y:[0-9]+]] y: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_sum:[0-9]+]] sum: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_carry:[0-9]+]] carry: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%[[VALUE_p]], const<i32>(0));
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: ne<i32>(read<i32>(deref(read<ptr<i32>>(%[[VALUE_x]]))), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(%[[VALUE_x]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i32>>(%[[VALUE_x]], read<ptr<i32>>(%[[VALUE2]]));
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(%[[VALUE_y]]);
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE3]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i32>>(%[[VALUE_y]], read<ptr<i32>>(%[[VALUE4]]));
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(%[[VALUE_out]]);
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE5]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i32>>(%[[VALUE_out]], read<ptr<i32>>(%[[VALUE6]]));
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_p]]);
// DEFAULT-NEXT:                 let %[[VALUE8:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE7]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_p]], read<i32>(%[[VALUE8]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%[[VALUE_p]]), const<i32>(0))
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_sum]], add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(deref(read<ptr<i32>>(%[[VALUE_x]]))), read<i32>(deref(read<ptr<i32>>(%[[VALUE_y]])))), read<i32>(%[[VALUE_carry]])));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_sum]], add<i32, overflow=ub>(read<i32>(deref(read<ptr<i32>>(%[[VALUE_x]]))), read<i32>(deref(read<ptr<i32>>(%[[VALUE_y]])))));
// DEFAULT-NEXT:                     if lt<i32>(read<i32>(%[[VALUE_sum]]), const<i32>(0))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_carry]], const<i32>(1));
// DEFAULT-NEXT:                             let %[[VALUE9:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_sum]]);
// DEFAULT-NEXT:                             let %[[VALUE10:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE9]]), const<i32>(2147483647));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_sum]], read<i32>(%[[VALUE10]]));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_carry]], const<i32>(0));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
