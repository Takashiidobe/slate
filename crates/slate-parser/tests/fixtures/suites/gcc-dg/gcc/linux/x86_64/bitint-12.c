/* PR c/102989 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-O2 -std=gnu23 -w" } */

_BitInt(37)
foo (_BitInt(37) x, _BitInt(37) y)
{
  _BitInt(37) w;
  __asm ("# %0 %1 %2 %3" : "=r" (w) : "r" (x), "r" (x + y), "g" (68719476735wb));
  return w;
}

#if __BITINT_MAXWIDTH__ >= 125
_BitInt(125)
bar (_BitInt(125) x, _BitInt(125) y)
{
  _BitInt(125) w;
  __asm ("# %0 %1 %2 %3" : "=g" (w) : "g" (x), "g" (x + y), "g" (21267647932558653966460912964485513215wb));
  return w;
}
#endif

#if __BITINT_MAXWIDTH__ >= 575
_BitInt(575)
baz (_BitInt(575) x, _BitInt(575) y)
{
  _BitInt(575) w;
  __asm ("# %0 %1 %2 %3" : "=g" (w) : "g" (x), "g" (x + y), "g" (61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783wb));
  return w;
}
#endif

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
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: i37b, %[[VALUE_y:[0-9]+]] y: i37b) -> i37b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_w:[0-9]+]] w: i37b [storage=automatic];
// DEFAULT-NEXT:         asm "# %0 %1 %2 %3" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             template: "# " %0 " " %1 " " %2 " " %3;
// DEFAULT-NEXT:             lateout 0 "r" [reg] width 64 place<i37b>(%[[VALUE_w]]);
// DEFAULT-NEXT:             in 1 "r" [reg] width 64 read<i37b>(%[[VALUE_x]]);
// DEFAULT-NEXT:             in 2 "r" [reg] width 64 add<i37b, overflow=ub>(read<i37b>(%[[VALUE_x]]), read<i37b>(%[[VALUE_y]]));
// DEFAULT-NEXT:             in 3 "g" [reg | mem | imm | sym] -> imm width 64 const<i37b>(68719476735);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<i37b>(%[[VALUE_w]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_x_2:[0-9]+]] x: i125b, %[[VALUE_y_2:[0-9]+]] y: i125b) -> i125b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_w_2:[0-9]+]] w: i125b [storage=automatic];
// DEFAULT-NEXT:         asm "# %0 %1 %2 %3" [dialect=att] [options=nostack] {
// DEFAULT-NEXT:             template: "# " %0 " " %1 " " %2 " " %3;
// DEFAULT-NEXT:             lateout 0 "g" [reg | mem | imm | sym] -> mem width 128 place<i125b>(%[[VALUE_w_2]]);
// DEFAULT-NEXT:             in 1 "g" [reg | mem | imm | sym] -> mem width 128 place<i125b>(%[[VALUE_x_2]]);
// DEFAULT-NEXT:             in 2 "g" [reg | mem | imm | sym] -> mem width 128 add<i125b, overflow=ub>(read<i125b>(%[[VALUE_x_2]]), read<i125b>(%[[VALUE_y_2]]));
// DEFAULT-NEXT:             in 3 "g" [reg | mem | imm | sym] -> imm width 128 const<i125b>(21267647932558653966460912964485513215);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<i125b>(%[[VALUE_w_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_baz:[0-9]+]] @baz(%[[VALUE_x_3:[0-9]+]] x: i575b, %[[VALUE_y_3:[0-9]+]] y: i575b) -> i575b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_w_3:[0-9]+]] w: i575b [storage=automatic];
// DEFAULT-NEXT:         asm "# %0 %1 %2 %3" [dialect=att] [options=nostack] {
// DEFAULT-NEXT:             template: "# " %0 " " %1 " " %2 " " %3;
// DEFAULT-NEXT:             lateout 0 "g" [reg | mem | imm | sym] -> mem width 576 place<i575b>(%[[VALUE_w_3]]);
// DEFAULT-NEXT:             in 1 "g" [reg | mem | imm | sym] -> mem width 576 place<i575b>(%[[VALUE_x_3]]);
// DEFAULT-NEXT:             in 2 "g" [reg | mem | imm | sym] -> mem width 576 add<i575b, overflow=ub>(read<i575b>(%[[VALUE_x_3]]), read<i575b>(%[[VALUE_y_3]]));
// DEFAULT-NEXT:             in 3 "g" [reg | mem | imm | sym] -> imm width 576 const<i575b>(61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<i575b>(%[[VALUE_w_3]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
