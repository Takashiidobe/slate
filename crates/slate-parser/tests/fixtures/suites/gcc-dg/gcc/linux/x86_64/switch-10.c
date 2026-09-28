/* { dg-options "-O2 -fdump-tree-cfg" }  */
#include <stdint.h>

void foo (void);
void bar (void);

void
test (uint8_t ch)
{
  switch (ch)
   {
   case 0:
     foo ();
     break;

   case 1 ... 255:
     bar ();
     break;
   }
}

/* Switch statement is converted to GIMPLE condition.  */
/* { dg-final { scan-tree-dump-not "switch" "cfg" } }  */

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
// DEFAULT-NEXT:     type @type0 __uint8_t = u8;
// DEFAULT-NEXT:     type @type1 uint8_t = u8;
// DEFAULT-NEXT:     fn %2 @foo() -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @bar() -> void [linkage=external];
// DEFAULT-NEXT:     fn %4 @test(%5 ch: u8) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         switch %6 reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%5)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %6 const<i32>(0):
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:                 break %6;
// DEFAULT-NEXT:                 case %6 const<i32>(1) ... const<i32>(255):
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:                 break %6;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
