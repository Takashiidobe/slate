/* { dg-options "-O2" } */
/* { dg-do compile } */
/* { dg-require-effective-target ilp32 } */

/* The problem was we were not striping the long cast here.
   Note this really should be invalid code but not for the
   current release (4.0) as we have people using it.  */

void blockCopy_MMX2(int*);
void postProcess_MMX2()
{
  int c, x,y, width;
  asm( "" :: "m" ((long)x));
  blockCopy_MMX2(&c);
}

// SLATE-FILECHECK-STD DEFAULT gnu23
// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "i686-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=4, align=4];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=4];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=4];
// DEFAULT-NEXT:         storage f80 [size=12, align=4];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_blockCopy_MMX2:[0-9]+]] @blockCopy_MMX2(%[[VALUE0:[0-9]+]] <unnamed>: ptr<i32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_postProcess_MMX2:[0-9]+]] @postProcess_MMX2() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_y:[0-9]+]] y: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_width:[0-9]+]] width: i32 [storage=automatic];
// DEFAULT-NEXT:         asm "" [dialect=att] [options=readonly,nostack] {
// DEFAULT-NEXT:             in 0 "m" [mem] width 32 read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%[[VALUE_blockCopy_MMX2]], addr_of<ptr<i32>>(%[[VALUE_c]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
