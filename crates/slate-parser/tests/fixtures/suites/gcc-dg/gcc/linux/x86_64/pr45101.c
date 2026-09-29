/* PR rtl-optimization/45101 */
/* { dg-do compile } */
/* { dg-options "-O2 -fgcse -fgcse-las" } */

struct
{
  int i;
} *s;

extern void bar (void);

void foo ()
{
  !s ? s->i++ : bar ();
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
// DEFAULT-NEXT:     type @type0 = struct {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     global %1 s: ptr<@type0> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %2 @bar() -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if not<bool>(ne<ptr<@type0>>(read<ptr<@type0>>(%1), null<ptr<@type0>>))
// DEFAULT-NEXT:             let %4: ptr<@type0> [synthetic] = read<ptr<@type0>>(%1);
// DEFAULT-NEXT:             let %5: i32 [synthetic] = read<i32>(field0(deref(read<ptr<@type0>>(%4))));
// DEFAULT-NEXT:             let %6: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%5), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(field0(deref(read<ptr<@type0>>(%4))), read<i32>(%6));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
