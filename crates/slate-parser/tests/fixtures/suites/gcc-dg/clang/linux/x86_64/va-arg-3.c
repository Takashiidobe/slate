/* { dg-do compile } */
/* { dg-options "-O3" } */
/* PR 14498.  */
#include <stdarg.h>
extern void baz(char *);
void bar (va_list *args)
{
  if (args)
    {
      char *s = va_arg(*args, char *);
      baz(s);
    }
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
// DEFAULT-NEXT:     type @type0 va_list = va_list;
// DEFAULT-NEXT:     fn %1 @baz(%5 <unnamed>: ptr<i8>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @bar(%3 args: ptr<va_list>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<ptr<va_list>>(read<ptr<va_list>>(%3), null<ptr<va_list>>)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %4 s: ptr<i8> [storage=automatic] = va_arg<ptr<i8>>(deref(read<ptr<va_list>>(%3)));
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<i8>) -> void>(%1, read<ptr<i8>>(%4));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
