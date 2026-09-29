// SLATE-FILECHECK-DEFINES DEFAULT

/* PR bootstrap/4192
   This testcase caused infinite loop in flow (several places),
   because flow assumes gen_jump generates simple_jump_p.  */

/* { dg-require-effective-target indirect_calls } */

typedef void (*T) (void);
extern T x[];

void
foo (void)
{
  static T *p = x;
  static _Bool a;
  T f;

  if (__builtin_expect (a, 0))
    return;

  while ((f = *p))
    {
      p++;
      f ();
    }
  a = 1;
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
// DEFAULT-NEXT:     type @type[[TYPE_T:[0-9]+]] T = ptr<fn() -> void>;
// DEFAULT-NEXT:     extern %[[VALUE_x:[0-9]+]] x: array<ptr<fn() -> void>, incomplete> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_p:[0-9]+]] p: ptr<ptr<fn() -> void>> [storage=static] = array_decay<ptr<ptr<fn() -> void>>, length=None>(%[[VALUE_x]]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: bool [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_expect:[0-9]+]] @__builtin_expect(%[[VALUE0:[0-9]+]] <unnamed>: i64, %[[VALUE1:[0-9]+]] <unnamed>: i64) -> i64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_f:[0-9]+]] f: ptr<fn() -> void> [storage=automatic];
// DEFAULT-NEXT:         if ne<i64>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE___builtin_expect]], from_bool<i64, reason=arg>(read<bool>(%[[VALUE_a]])), widen<i64, reason=arg>(const<i32>(0))), const<i64>(0))
// DEFAULT-NEXT:             return;
// DEFAULT-NEXT:         while %[[VALUE2:[0-9]+]] {
// DEFAULT-NEXT:             write<ptr<fn() -> void>>(%[[VALUE_f]], read<ptr<fn() -> void>>(deref(read<ptr<ptr<fn() -> void>>>(%[[VALUE_p]]))));
// DEFAULT-NEXT:             yield ne<ptr<fn() -> void>>(read<ptr<fn() -> void>>(deref(read<ptr<ptr<fn() -> void>>>(%[[VALUE_p]]))), null<ptr<fn() -> void>>);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: ptr<ptr<fn() -> void>> [synthetic] = read<ptr<ptr<fn() -> void>>>(%[[VALUE_p]]);
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: ptr<ptr<fn() -> void>> [synthetic] = ptr_offset<ptr<ptr<fn() -> void>>, subtract=false, element=ptr<fn() -> void>, overflow=ub>(read<ptr<ptr<fn() -> void>>>(%[[VALUE3]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<ptr<fn() -> void>>>(%[[VALUE_p]], read<ptr<ptr<fn() -> void>>>(%[[VALUE4]]));
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(read<ptr<fn() -> void>>(%[[VALUE_f]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<bool>(%[[VALUE_a]], ne<i32, reason=assign>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
