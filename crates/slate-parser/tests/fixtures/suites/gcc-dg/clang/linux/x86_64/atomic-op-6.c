/* Test we don't generate bogus warnings.  */
/* PR c/69407 */
/* { dg-do compile } */
/* { dg-options "-Wall -Wextra" } */

void
foo (int *p, int a)
{
  __atomic_fetch_add (&p, a, 0); /* { dg-bogus "value computed is not used" } */
  __atomic_add_fetch (&p, a, 0); /* { dg-bogus "value computed is not used" } */
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
// DEFAULT-NEXT:     fn %0 @foo(%1 p: ptr<i32>, %2 a: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %3: ptr<i32> [synthetic] = update<ptr<i32>, result=old, atomic=relaxed>(deref(addr_of<ptr<ptr<i32>>>(%1)), ptr_offset<ptr<i32>, subtract=false, element=u8, overflow=wrap>(old<ptr<i32>>, read<i32>(%2)));
// DEFAULT-NEXT:         let %4: ptr<i32> [synthetic] = update<ptr<i32>, result=new, atomic=relaxed>(deref(addr_of<ptr<ptr<i32>>>(%1)), ptr_offset<ptr<i32>, subtract=false, element=u8, overflow=wrap>(old<ptr<i32>>, read<i32>(%2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
