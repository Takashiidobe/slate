/* Copyright (C) 2003  Free Software Foundation.

   Test that switch statements suitable using case bit tests are
   implemented correctly.

   Written by Roger Sayle, 01/25/2001.  */

extern void abort(void);

int foo(int x) {
  switch (x) {
  case 4:
  case 6:
  case 9:
  case 11:
    return 30;
  }
  return 31;
}

int main() {
  int i, r;

  for (i = -1; i < 66; i++) {
    r = foo(i);
    if (i == 4) {
      if (r != 30)
        abort();
    } else if (i == 6) {
      if (r != 30)
        abort();
    } else if (i == 9) {
      if (r != 30)
        abort();
    } else if (i == 11) {
      if (r != 30)
        abort();
    } else if (r != 31)
      abort();
  }
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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         switch %[[VALUE0:[0-9]+]] read<i32>(%[[VALUE_x]])
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(4):
// DEFAULT-NEXT:                     case %[[VALUE0]] const<i32>(6):
// DEFAULT-NEXT:                         case %[[VALUE0]] const<i32>(9):
// DEFAULT-NEXT:                             case %[[VALUE0]] const<i32>(11):
// DEFAULT-NEXT:                                 return const<i32>(30);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return const<i32>(31);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(66))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_r]], call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo]], read<i32>(%[[VALUE_i]])));
// DEFAULT-NEXT:                     call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo]], read<i32>(%[[VALUE_i]]));
// DEFAULT-NEXT:                     if eq<i32>(read<i32>(%[[VALUE_i]]), const<i32>(4))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(read<i32>(%[[VALUE_r]]), const<i32>(30))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         if eq<i32>(read<i32>(%[[VALUE_i]]), const<i32>(6))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%[[VALUE_r]]), const<i32>(30))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             if eq<i32>(read<i32>(%[[VALUE_i]]), const<i32>(9))
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(read<i32>(%[[VALUE_r]]), const<i32>(30))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             else
// DEFAULT-NEXT:                                 if eq<i32>(read<i32>(%[[VALUE_i]]), const<i32>(11))
// DEFAULT-NEXT:                                     {
// DEFAULT-NEXT:                                         if ne<i32>(read<i32>(%[[VALUE_r]]), const<i32>(30))
// DEFAULT-NEXT:                                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                                 else
// DEFAULT-NEXT:                                     if ne<i32>(read<i32>(%[[VALUE_r]]), const<i32>(31))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
