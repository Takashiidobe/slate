/* { dg-do compile } */
/* { dg-options "-O1" } */
/* Using -mshort-calls avoids loading the function addresses in
   registers and thus getting the counts wrong.  */
/* { dg-additional-options "-mshort-calls" { target epiphany-*-* } } */
/* { dg-require-effective-target tls_emulated } */
/* { dg-additional-options "-fdump-rtl-final-slim" { target nvptx-*-* } }*/

/* Test that we only get one call to emutls_get_address when CSE is
   active.  Note that the var _must_ be initialized for the scan asm
   to work, since otherwise there will be an initializer which will,
   correctly, call emutls_get_address.  */
int foo (int b, int c, int d)
{
  static __thread int a=1;
  a += b;
  a -= c;
  a += d;
  return a;
}

/* { dg-final { scan-assembler-not "emutls_get_address.*emutls_get_address.*" { target { ! { "*-wrs-vxworks"  "*-*-darwin8"  "hppa*-*-hpux*" "i?86-*-mingw*" "x86_64-*-mingw*" visium-*-* nvptx-*-* } } } } } */
/* { dg-final { scan-assembler-not "call\tL___emutls_get_address.stub.*call\tL___emutls_get_address.stub.*" { target "*-*-darwin8" } } } */
/* { dg-final { scan-assembler-not "(b,l|bl) __emutls_get_address.*(b,l|bl) __emutls_get_address.*" { target "hppa*-*-hpux*" } } } */
/* { dg-final { scan-assembler-not "tls_lookup.*tls_lookup.*" { target *-wrs-vxworks } } } */
/* { dg-final { scan-assembler-not "call\t___emutls_get_address.*call\t___emutls_get_address" { target "i?86-*-mingw*" } } } */
/* { dg-final { scan-assembler-not "call\t__emutls_get_address.*call\t__emutls_get_address" { target "x86_64-*-mingw*" } } } */
/* { dg-final { scan-assembler-not "%l __emutls_get_address.*%l __emutls_get_address" { target visium-*-* } } } */

/* { dg-final { scan-rtl-dump-times "emutls_get_address" 1 "final" { target nvptx-*-* } } } */

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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: i32 [storage=thread] = const<i32>(1) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_b:[0-9]+]] b: i32, %[[VALUE_c:[0-9]+]] c: i32, %[[VALUE_d:[0-9]+]] d: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE0]]), read<i32>(%[[VALUE_b]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE1]]));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE2]]), read<i32>(%[[VALUE_c]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), read<i32>(%[[VALUE_d]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_a]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
