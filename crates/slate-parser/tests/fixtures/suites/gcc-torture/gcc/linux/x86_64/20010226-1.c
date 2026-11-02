// SLATE-FILECHECK-DEFINES DEFAULT

/* { dg-require-effective-target trampolines } */

void f1 (void *);
void f3 (void *, void (*)(void *));
void f2 (void *);

int foo (void *a, int b)
{
  if (!b)
    {
      f1 (a);
      return 1;
    }
  if (b)
    {
      void bar (void *c)
      {
	if (c == a)
	  f2 (c);
      }
      f3 (a, bar);
    }
  return 0;
}

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
// DEFAULT-NEXT:     fn %[[VALUE_f1:[0-9]+]] @f1(%[[VALUE0:[0-9]+]] <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f3:[0-9]+]] @f3(%[[VALUE1:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE2:[0-9]+]] <unnamed>: ptr<fn(ptr<void>) -> void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f2:[0-9]+]] @f2(%[[VALUE3:[0-9]+]] <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_a:[0-9]+]] a: ptr<void>, %[[VALUE_b:[0-9]+]] b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32>(%[[VALUE_b]]), const<i32>(0)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_f1]], read<ptr<void>>(%[[VALUE_a]]));
// DEFAULT-NEXT:                 return const<i32>(1);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_b]]), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 nested_fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_c:[0-9]+]] c: ptr<void>) -> void [captures=%[[VALUE_a]]: ptr<void>] {
// DEFAULT-NEXT:                     if eq<ptr<void>>(read<ptr<void>>(%[[VALUE_c]]), read<ptr<void>>(%[[VALUE_a]]))
// DEFAULT-NEXT:                         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_f2]], read<ptr<void>>(%[[VALUE_c]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<void>, ptr<fn(ptr<void>) -> void>) -> void>(%[[VALUE_f3]], read<ptr<void>>(%[[VALUE_a]]), function_decay<ptr<fn(ptr<void>) -> void>>(%[[VALUE_bar]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
