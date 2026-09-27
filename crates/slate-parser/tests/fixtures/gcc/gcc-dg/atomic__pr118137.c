/* Test that subword atomic operations only affect the subword.  */
/* { dg-do run } */
/* { dg-require-effective-target sync_char_short } */

void
foo (char *x)
{
  __sync_fetch_and_or (x, 0xff);
}

void
bar (short *y)
{
  __atomic_fetch_or (y, 0xffff, 0);
}


int
main ()
{
  char b[4] = {};
  foo(b);

  short h[2] = {};
  bar(h);

  if (b[1] || b[2] || b[3] || h[1])
    __builtin_abort();
}

// SLATE-FILECHECK-FLAVOR gcc
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
// DEFAULT-NEXT:     fn %0 @foo(%1 x: ptr<i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %8: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(read<ptr<i8>>(%1)), or<i8>(old<i8>, truncate<i8, reason=arg, fits=unknown>(const<i32>(255))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @bar(%3 y: ptr<i16>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %9: i16 [synthetic] = update<i16, result=old, atomic=relaxed>(deref(read<ptr<i16>>(%3)), or<i16>(old<i16>, truncate<i16, reason=arg, fits=unknown>(const<i32>(65535))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %5 b: array<i8, 4> [storage=automatic] = aggregate<array<i8, 4>, zero_fill=true>();
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i8>) -> void>(%0, array_decay<ptr<i8>, length=Some(4)>(%5));
// DEFAULT-NEXT:         let %6 h: array<i16, 2> [storage=automatic] = aggregate<array<i16, 2>, zero_fill=true>();
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i16>) -> void>(%2, array_decay<ptr<i16>, length=Some(2)>(%6));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i8>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(%5), const<i32>(1)))), const<i8>(0)), ne<i8>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(%5), const<i32>(2)))), const<i8>(0))), ne<i8>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(%5), const<i32>(3)))), const<i8>(0))), ne<i16>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(2)>(%6), const<i32>(1)))), const<i16>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%7);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
