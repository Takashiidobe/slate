/* PR tree-optimization/49161 */

extern void abort(void);

int c;

__attribute__((noinline, noclone)) void bar(int x) {
  if (x != c++)
    abort();
}

__attribute__((noinline, noclone)) void foo(int x) {
  switch (x) {
  case 3:
    goto l1;
  case 4:
    goto l2;
  case 6:
    goto l3;
  default:
    return;
  }
l1:
  goto l4;
l2:
  goto l4;
l3:
  bar(-1);
l4:
  bar(0);
  if (x != 4)
    bar(1);
  if (x != 3)
    bar(-1);
  bar(2);
}

int main() {
  foo(3);
  if (c != 3)
    abort();
  return 0;
}


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
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_x:[0-9]+]] x: i32) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_c]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE0]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_c]], read<i32>(%[[VALUE1]]));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_x]]), read<i32>(%[[VALUE0]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x_2:[0-9]+]] x: i32) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         switch %[[VALUE2:[0-9]+]] read<i32>(%[[VALUE_x_2]])
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %[[VALUE2]] const<i32>(3):
// DEFAULT-NEXT:                     goto %[[VALUE_l1:[0-9]+]];
// DEFAULT-NEXT:                 case %[[VALUE2]] const<i32>(4):
// DEFAULT-NEXT:                     goto %[[VALUE_l2:[0-9]+]];
// DEFAULT-NEXT:                 case %[[VALUE2]] const<i32>(6):
// DEFAULT-NEXT:                     goto %[[VALUE_l3:[0-9]+]];
// DEFAULT-NEXT:                 default %[[VALUE2]]:
// DEFAULT-NEXT:                     return;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         label %[[VALUE_l1]] l1:
// DEFAULT-NEXT:             goto %[[VALUE_l4:[0-9]+]];
// DEFAULT-NEXT:         label %[[VALUE_l2]] l2:
// DEFAULT-NEXT:             goto %[[VALUE_l4]];
// DEFAULT-NEXT:         label %[[VALUE_l3]] l3:
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%[[VALUE_bar]], neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:         label %[[VALUE_l4]] l4:
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%[[VALUE_bar]], const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_x_2]]), const<i32>(4))
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%[[VALUE_bar]], const<i32>(1));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_x_2]]), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%[[VALUE_bar]], neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_bar]], const<i32>(2));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_foo]], const<i32>(3));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_c]]), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
