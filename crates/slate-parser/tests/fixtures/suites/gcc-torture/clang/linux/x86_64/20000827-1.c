// SLATE-FILECHECK-DEFINES DEFAULT

/* Copyright (C) 2000  Free Software Foundation  */
/* Contributed by Alexandre Oliva <aoliva@redhat.com> */

int
foo () 
{
  while (1)
    {
      int a;
      char b;
      /* gcse should not merge these asm statements, since their
	 output operands have different modes.  */
      __asm__("":"=r" (a)); __asm__("":"=r" (b));
      if (b)
	return a;
    }
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
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         while %[[VALUE0:[0-9]+]] ne<i32>(const<i32>(1), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a:[0-9]+]] a: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_b:[0-9]+]] b: i8 [storage=automatic];
// DEFAULT-NEXT:                 asm "" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:                     lateout 0 "r" [reg] width 32 place<i32>(%[[VALUE_a]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:                 asm "" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:                     lateout 0 "r" [reg] width 8 place<i8>(%[[VALUE_b]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:                 if ne<i8>(read<i8>(%[[VALUE_b]]), const<i8>(0))
// DEFAULT-NEXT:                     return read<i32>(%[[VALUE_a]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
