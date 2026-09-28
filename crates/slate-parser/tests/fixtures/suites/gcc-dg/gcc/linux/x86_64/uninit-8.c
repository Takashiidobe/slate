/* Uninitialized variable warning tests...
   Inspired by part of optabs.c:expand_binop.
   May be the same as uninit-1.c.  */

/* { dg-do compile } */
/* { dg-options "-O -Wuninitialized" } */

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
// DEFAULT-NEXT:     fn %0 @add_bignums(%1 out: ptr<i32>, %2 x: ptr<i32>, %3 y: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %4 p: i32 [storage=automatic];
// DEFAULT-NEXT:         let %5 sum: i32 [storage=automatic];
// DEFAULT-NEXT:         let %6 carry: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%4, const<i32>(0));
// DEFAULT-NEXT:         for %7
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: ne<i32>(read<i32>(deref(read<ptr<i32>>(%2))), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %8: ptr<i32> [synthetic] = read<ptr<i32>>(%2);
// DEFAULT-NEXT:                 let %9: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%8), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i32>>(%2, read<ptr<i32>>(%9));
// DEFAULT-NEXT:                 let %10: ptr<i32> [synthetic] = read<ptr<i32>>(%3);
// DEFAULT-NEXT:                 let %11: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%10), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i32>>(%3, read<ptr<i32>>(%11));
// DEFAULT-NEXT:                 let %12: ptr<i32> [synthetic] = read<ptr<i32>>(%1);
// DEFAULT-NEXT:                 let %13: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%12), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i32>>(%1, read<ptr<i32>>(%13));
// DEFAULT-NEXT:                 let %14: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:                 let %15: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%14), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%4, read<i32>(%15));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%4), const<i32>(0))
// DEFAULT-NEXT:                         write<i32>(%5, add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(deref(read<ptr<i32>>(%2))), read<i32>(deref(read<ptr<i32>>(%3)))), read<i32>(%6)));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         write<i32>(%5, add<i32, overflow=ub>(read<i32>(deref(read<ptr<i32>>(%2))), read<i32>(deref(read<ptr<i32>>(%3)))));
// DEFAULT-NEXT:                     if lt<i32>(read<i32>(%5), const<i32>(0))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             write<i32>(%6, const<i32>(1));
// DEFAULT-NEXT:                             let %16: i32 [synthetic] = read<i32>(%5);
// DEFAULT-NEXT:                             let %17: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%16), const<i32>(2147483647));
// DEFAULT-NEXT:                             write<i32>(%5, read<i32>(%17));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         write<i32>(%6, const<i32>(0));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
