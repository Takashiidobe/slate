// SLATE-FILECHECK-DEFINES DEFAULT

/* Red Hat bugzilla #68395
   PR middle-end/7245
   This testcase ICEd on IA-32 because shift & compare patterns
   predicates allowed any immediate, but constraints allowed only
   numbers from 1 to 31.  */

void foo (int *x, unsigned int y)
{
  int a = y >> -13;
  if (a)
    *x = a;
}

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
// DEFAULT-NEXT:     fn %0 @foo(%1 x: ptr<i32>, %2 y: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %3 a: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%2), neg<i32, overflow=ub>(const<i32>(13))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%3), const<i32>(0))
// DEFAULT-NEXT:             write<i32>(deref(read<ptr<i32>>(%1)), read<i32>(%3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
