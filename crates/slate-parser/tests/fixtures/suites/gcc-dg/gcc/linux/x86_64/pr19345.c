/* We shouldn't crash trying to produce the inlined structure type die debug info.  */
/* { dg-do compile } */
/* { dg-options "-O1 -g" } */
inline void bar(char a[], unsigned int l)
{
  asm volatile ("" :: "m" ( *(struct {char x[l]; } *)a));
}

void foo(void)
{
  bar (0, 0);
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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 x: vla<i8, %[[VALUE0:[0-9]+]]>;
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[0]];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_a:[0-9]+]] a: ptr<i8>, %[[VALUE_l:[0-9]+]] l: u32) -> void [linkage=external] [inline=hint] [definition=inline_only] [fallthrough=ret_void] [c="void(char[], unsigned int)"] {
// DEFAULT-NEXT:         let %[[VALUE0]]: u64 [synthetic] = widen<u64, reason=assign>(read<u32>(%[[VALUE_l]])) [c="__size_t"];
// DEFAULT-NEXT:         asm volatile "" [dialect=att] [options=readonly,nostack] {
// DEFAULT-NEXT:             in 0 "m" [mem] width 0 place<@type[[TYPE0]]>(deref(pointer_cast<ptr<@type[[TYPE0]]>, reason=explicit>(read<ptr<i8>>(%[[VALUE_a]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i8>, u32) -> void>(%[[VALUE_bar]], null<ptr<i8>>, reinterpret<u32, reason=arg, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
