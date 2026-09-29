/* PR middle-end/117354 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-O2" } */
/* { dg-additional-options "-mavx2" { target x86_64-*-* i?86-*-* } } */

#if __BITINT_MAXWIDTH__ >= 256
#define N 256
#else
#define N 64
#endif

struct S {
  unsigned char y;
  _BitInt(N) x;
} s;

__attribute__((noipa)) static void
foo (const char *, _BitInt(N))
{
}

__attribute__((noipa)) static void
bar (_BitInt(N))
{
}

static void
baz (void *p)
{
  foo ("bazbazbazb", s.x);
  __builtin_memcpy (p, &s.x, sizeof s.x);
}

int
main ()
{
  void *ptr = &s.x;
  baz (&s.x);
  bar (*(_BitInt(N) *) ptr);
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
// DEFAULT-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// DEFAULT-NEXT:         field0 y: u8;
// DEFAULT-NEXT:         field1 x: i256b;
// DEFAULT-NEXT:     } [size=40, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %[[VALUE_s:[0-9]+]] s: @type[[TYPE_S]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([98, 97, 122, 98, 97, 122, 98, 97, 122, 98, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE0:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE1:[0-9]+]] <unnamed>: i256b) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE2:[0-9]+]] <unnamed>: i256b) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_memcpy:[0-9]+]] @__builtin_memcpy(%[[VALUE3:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE4:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE5:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_baz:[0-9]+]] @baz(%[[VALUE_p:[0-9]+]] p: ptr<void>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, i256b) -> void>(%[[VALUE_foo]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str]])), read<i256b>(field1(%[[VALUE_s]])));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE___builtin_memcpy]], read<ptr<void>>(%[[VALUE_p]]), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i256b>>(field1(%[[VALUE_s]]))), const<u64>(32));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_ptr:[0-9]+]] ptr: ptr<void> [storage=automatic] = pointer_cast<ptr<void>, reason=assign>(addr_of<ptr<i256b>>(field1(%[[VALUE_s]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_baz]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i256b>>(field1(%[[VALUE_s]]))));
// DEFAULT-NEXT:         call<void, signature=fn(i256b) -> void>(%[[VALUE_bar]], read<i256b>(deref(pointer_cast<ptr<i256b>, reason=explicit>(read<ptr<void>>(%[[VALUE_ptr]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
