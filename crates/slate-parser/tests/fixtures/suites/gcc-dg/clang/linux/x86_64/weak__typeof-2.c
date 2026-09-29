/* Test typeof with __asm redirection. */
/* { dg-do compile } */
/* -mlongcall will cause us to place &baz3 in the CTR register.  */
/* { dg-skip-if "" { powerpc*-*-* } { "-mlongcall" } { "" } } */
/* { dg-require-weak "" } */
/* { dg-require-alias "" } */
/* { dg-options "-O2" } */
/* Using -mshort-calls avoids loading the function addresses in
   registers and thus getting the counts wrong.  */
/* { dg-additional-options "-mshort-calls" { target epiphany-*-* } } */

extern int foo1 (int x) __asm ("baz1");
int bar1 (int x) { return x; }
extern __typeof (bar1) foo1 __attribute ((weak, alias ("bar1")));

extern int foo2 (int x) __attribute__ ((const));
extern __typeof (foo2) foo2 __asm ("baz2");
int bar2 (int x)
{
  return foo2 (x) + foo2 (x) + foo2 (x) + foo2 (x) + foo2 (x) + foo2 (x);
}

extern int foo3 (int x);
extern __typeof (foo3) foo3 __asm ("baz3");
int bar3 (int x)
{
  return foo3 (x) + foo3 (x) + foo3 (x) + foo3 (x) + foo3 (x) + foo3 (x);
}

// { dg-final { scan-assembler-not "foo1" } }
// { dg-final { scan-assembler "baz1" } }
// { dg-final { scan-assembler-not "foo2" } }
// { dg-final { scan-assembler "baz2" } }
// { dg-final { scan-assembler-not "baz2.*baz2.*baz2.*baz2.*baz2.*baz2" } }
// { dg-final { scan-assembler-not "foo3" } }
// SH targets put the function address into a constant pool and / or register,
// so it does not appear repeated (as much as expected) in the assembler.
// { dg-final { global target_triplet } }
// { dg-final { if [string match sh-*-* $target_triplet ] {return} } }
// { dg-final { if [string match {sh[elb1-9]*-*-*} $target_triplet ] {return} } }
// Likewise for S/390 targets
// { dg-final { if [string match s390*-*-* $target_triplet ] {return} } }
// Likewise for ARM targets
// { dg-final { if [string match arm*-*-* $target_triplet ] {return} } }
// Likewise for CRIS targets.
// { dg-final { if [string match cris-*-* $target_triplet ] {return} } }
// Likewise for m68k targets.
// { dg-final { if [string match fido-*-* $target_triplet ] {return} } }
// { dg-final { if [string match m68k-*-* $target_triplet ] {return} } }
// Likewise for moxie targets.
// { dg-final { if [string match moxie-*-* $target_triplet ] {return} } }
// Likewise for msp430 targets.
// { dg-final { if [string match msp430-*-* $target_triplet ] {return} } }
// Likewise for Visium targets.
// { dg-final { if [string match visium-*-* $target_triplet ] {return} } }
// { dg-final { scan-assembler "baz3.*baz3.*baz3.*baz3.*baz3.*baz3" } }

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
// DEFAULT-NEXT:     fn %[[VALUE_foo1:[0-9]+]] @foo1(%[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=external] [asm_name="baz1"] [weak] [alias="bar1"];
// DEFAULT-NEXT:     fn %[[VALUE_bar1:[0-9]+]] @bar1(%[[VALUE_x_2:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_x_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo2:[0-9]+]] @foo2(%[[VALUE_x_3:[0-9]+]] x: i32) -> i32 [linkage=external] [asm_name="baz2"] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_bar2:[0-9]+]] @bar2(%[[VALUE_x_4:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo2]], read<i32>(%[[VALUE_x_4]])), call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo2]], read<i32>(%[[VALUE_x_4]]))), call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo2]], read<i32>(%[[VALUE_x_4]]))), call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo2]], read<i32>(%[[VALUE_x_4]]))), call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo2]], read<i32>(%[[VALUE_x_4]]))), call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo2]], read<i32>(%[[VALUE_x_4]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo3:[0-9]+]] @foo3(%[[VALUE_x_5:[0-9]+]] x: i32) -> i32 [linkage=external] [asm_name="baz3"];
// DEFAULT-NEXT:     fn %[[VALUE_bar3:[0-9]+]] @bar3(%[[VALUE_x_6:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo3]], read<i32>(%[[VALUE_x_6]])), call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo3]], read<i32>(%[[VALUE_x_6]]))), call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo3]], read<i32>(%[[VALUE_x_6]]))), call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo3]], read<i32>(%[[VALUE_x_6]]))), call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo3]], read<i32>(%[[VALUE_x_6]]))), call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo3]], read<i32>(%[[VALUE_x_6]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
