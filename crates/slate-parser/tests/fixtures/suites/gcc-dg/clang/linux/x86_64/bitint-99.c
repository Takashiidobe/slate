/* PR tree-optimization/114278 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-O2 -fno-tree-dce -fno-tree-dse -fno-tree-ccp" } */
/* { dg-additional-options "-mavx2" { target i?86-*-* x86_64-*-* } } */

void
foo (void *p)
{
  _BitInt(64) b = *(_BitInt(64) *) __builtin_memmove (&b, p, sizeof (_BitInt(64)));
}

#if __BITINT_MAXWIDTH__ >= 128
void
bar (void *p)
{
  _BitInt(128) b = *(_BitInt(128) *) __builtin_memmove (&b, p, sizeof (_BitInt(128)));
}
#endif

#if __BITINT_MAXWIDTH__ >= 256
void
baz (void *p)
{
  _BitInt(256) b = *(_BitInt(256) *) __builtin_memmove (&b, p, sizeof (_BitInt(256)));
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
// DEFAULT-NEXT:     fn %12 @__builtin_memmove(%9 <unnamed>: ptr<void>, %10 <unnamed>: ptr<const void>, %11 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %0 @foo(%1 p: ptr<void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %2 b: i64b [storage=automatic] = read<i64b>(deref(pointer_cast<ptr<i64b>, reason=explicit>(call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i64b>>(%2)), pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%1)), const<u64>(8)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @bar(%4 p: ptr<void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %5 b: i128b [storage=automatic] = read<i128b>(deref(pointer_cast<ptr<i128b>, reason=explicit>(call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i128b>>(%5)), pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%4)), const<u64>(16)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @baz(%7 p: ptr<void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %8 b: i256b [storage=automatic] = read<i256b>(deref(pointer_cast<ptr<i256b>, reason=explicit>(call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%12, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i256b>>(%8)), pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%7)), const<u64>(32)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
