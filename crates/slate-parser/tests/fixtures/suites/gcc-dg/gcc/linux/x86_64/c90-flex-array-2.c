/* [] does not indicate a flexible array member unless it is the field
   itself being declared as an incomplete array type rather than a
   pointer or other type derived from such a type.  PR 36432.  */
/* { dg-do compile } */
/* { dg-options "-std=iso9899:1990 -pedantic-errors" } */

void
f (void)
{
  int a[3];
  int (*p)[];
  struct { int (*p)[]; } s;
  p = &a;
  s.p = &a;
}

// SLATE-FILECHECK-STD DEFAULT iso9899:1990
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
// DEFAULT-NEXT:     type @type0 = struct {
// DEFAULT-NEXT:         field0 p: ptr<array<i32, incomplete>>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     fn %0 @f() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %1 a: array<i32, 3> [storage=automatic];
// DEFAULT-NEXT:         let %2 p: ptr<array<i32, incomplete>> [storage=automatic];
// DEFAULT-NEXT:         let %4 s: @type0 [storage=automatic];
// DEFAULT-NEXT:         write<ptr<array<i32, incomplete>>>(%2, pointer_cast<ptr<array<i32, incomplete>>, reason=assign>(addr_of<ptr<array<i32, 3>>>(%1)));
// DEFAULT-NEXT:         write<ptr<array<i32, incomplete>>>(field0(%4), pointer_cast<ptr<array<i32, incomplete>>, reason=assign>(addr_of<ptr<array<i32, 3>>>(%1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
