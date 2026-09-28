/* For PR tree-optimization/14784  */

/* { dg-do compile } */
/* { dg-options "-O2 -funswitch-loops -fdump-tree-unswitch-details" } */

typedef struct bitmap_element_def
{
  unsigned int indx;
} bitmap_element;

typedef struct bitmap_head_def {
    bitmap_element *first;
    int using_obstack;
} bitmap_head;
typedef struct bitmap_head_def *bitmap;

bitmap_element *bitmap_free;

void foo (bitmap head, bitmap_element *elt)
{
  while (1)
    {
      /* Alias analysis problems used to prevent us from recognizing
	 that this condition is invariant.  */
      if (head->using_obstack)
	bitmap_free = elt;
    }
}


/* { dg-final { scan-tree-dump-times "unswitching" 1 "unswitch"} } */

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
// DEFAULT-NEXT:     type @type0 bitmap_element_def = struct {
// DEFAULT-NEXT:         field0 indx: u32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type1 bitmap_element = @type0;
// DEFAULT-NEXT:     type @type2 bitmap_head_def = struct {
// DEFAULT-NEXT:         field0 first: ptr<@type0>;
// DEFAULT-NEXT:         field1 using_obstack: i32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type3 bitmap_head = @type2;
// DEFAULT-NEXT:     type @type4 bitmap = ptr<@type2>;
// DEFAULT-NEXT:     global %5 bitmap_free: ptr<@type0> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %6 @foo(%7 head: ptr<@type2>, %8 elt: ptr<@type0>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         while %9 ne<i32>(const<i32>(1), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(field1(deref(read<ptr<@type2>>(%7)))), const<i32>(0))
// DEFAULT-NEXT:                     write<ptr<@type0>>(%5, read<ptr<@type0>>(%8));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
