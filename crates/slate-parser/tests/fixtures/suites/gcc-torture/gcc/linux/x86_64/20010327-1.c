// SLATE-FILECHECK-DEFINES DEFAULT

/* { dg-do compile { target { ptr32plus && { ! llp64 } } } } */

/* This testcase tests whether GCC can produce static initialized data
   that references addresses of size 'unsigned long', even if that's not
   the same as __SIZE_TYPE__.  (See 20011114-1.c for the same test of
   size __SIZE_TYPE__.)  

   Some rare environments might not have the required relocs to support
   this; they should have this test disabled in the .x file.  */

extern void _text;
static unsigned long x = (unsigned long) &_text - 0x10000000L - 1;

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
// DEFAULT-NEXT:     extern %[[VALUE__text:[0-9]+]] _text: void [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_x:[0-9]+]] x: u64 [storage=static] = sub<u64, overflow=wrap>(sub<u64, overflow=wrap>(ptr_to_int<u64, reason=explicit>(addr_of<ptr<void>>(%[[VALUE__text]])), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(268435456))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
