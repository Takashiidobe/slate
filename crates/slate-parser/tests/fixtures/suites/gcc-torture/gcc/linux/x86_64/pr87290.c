/* PR middle-end/87290 */

int c;

__attribute__((noipa)) void f0(void) { c++; }

__attribute__((noipa)) int f1(int x) { return x % 16 == 13; }

__attribute__((noipa)) int f2(int x) { return x % 16 == -13; }

__attribute__((noipa)) void f3(int x) {
  if (x % 16 == 13)
    f0();
}

__attribute__((noipa)) void f4(int x) {
  if (x % 16 == -13)
    f0();
}

int main() {
  int i, j;
  for (i = -30; i < 30; i++) {
    if (f1(13 + i * 16) != (i >= 0) || f2(-13 + i * 16) != (i <= 0))
      __builtin_abort();
    f3(13 + i * 16);
    if (c != (i >= 0))
      __builtin_abort();
    f4(-13 + i * 16);
    if (c != 1 + (i == 0))
      __builtin_abort();
    for (j = 1; j < 16; j++) {
      if (f1(13 + i * 16 + j) || f2(-13 + i * 16 + j))
        __builtin_abort();
      f3(13 + i * 16 + j);
      f4(-13 + i * 16 + j);
    }
    if (c != 1 + (i == 0))
      __builtin_abort();
    c = 0;
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
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f0:[0-9]+]] @f0() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_c]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE0]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_c]], read<i32>(%[[VALUE1]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f1:[0-9]+]] @f1(%[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_x]]), const<i32>(16)), const<i32>(13)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f2:[0-9]+]] @f2(%[[VALUE_x_2:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_x_2]]), const<i32>(16)), neg<i32, overflow=ub>(const<i32>(13))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f3:[0-9]+]] @f3(%[[VALUE_x_3:[0-9]+]] x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_x_3]]), const<i32>(16)), const<i32>(13))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_f0]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f4:[0-9]+]] @f4(%[[VALUE_x_4:[0-9]+]] x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_x_4]]), const<i32>(16)), neg<i32, overflow=ub>(const<i32>(13)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_f0]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE2:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], neg<i32, overflow=ub>(const<i32>(30)));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(30))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE3]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE5:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_f1]], add<i32, overflow=ub>(const<i32>(13), mul<i32, overflow=ub>(read<i32>(%[[VALUE_i]]), const<i32>(16)))), from_bool<i32, reason=promotion>(ge<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0))))
// DEFAULT-NEXT:                         write<bool>(%[[VALUE5]], const<bool>(true));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         write<bool>(%[[VALUE5]], ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_f2]], add<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(13)), mul<i32, overflow=ub>(read<i32>(%[[VALUE_i]]), const<i32>(16)))), from_bool<i32, reason=promotion>(le<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0)))));
// DEFAULT-NEXT:                     if read<bool>(%[[VALUE5]])
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32) -> void>(%[[VALUE_f3]], add<i32, overflow=ub>(const<i32>(13), mul<i32, overflow=ub>(read<i32>(%[[VALUE_i]]), const<i32>(16))));
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%[[VALUE_c]]), from_bool<i32, reason=promotion>(ge<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32) -> void>(%[[VALUE_f4]], add<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(13)), mul<i32, overflow=ub>(read<i32>(%[[VALUE_i]]), const<i32>(16))));
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%[[VALUE_c]]), add<i32, overflow=ub>(const<i32>(1), from_bool<i32, reason=promotion>(eq<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0)))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                     for %[[VALUE6:[0-9]+]]
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_j]], const<i32>(1));
// DEFAULT-NEXT:                         condition: lt<i32>(read<i32>(%[[VALUE_j]]), const<i32>(16))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %[[VALUE7:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j]]);
// DEFAULT-NEXT:                             let %[[VALUE8:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE7]]), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_j]], read<i32>(%[[VALUE8]]));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE9:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                                 if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_f1]], add<i32, overflow=ub>(add<i32, overflow=ub>(const<i32>(13), mul<i32, overflow=ub>(read<i32>(%[[VALUE_i]]), const<i32>(16))), read<i32>(%[[VALUE_j]]))), const<i32>(0))
// DEFAULT-NEXT:                                     write<bool>(%[[VALUE9]], const<bool>(true));
// DEFAULT-NEXT:                                 else
// DEFAULT-NEXT:                                     write<bool>(%[[VALUE9]], ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_f2]], add<i32, overflow=ub>(add<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(13)), mul<i32, overflow=ub>(read<i32>(%[[VALUE_i]]), const<i32>(16))), read<i32>(%[[VALUE_j]]))), const<i32>(0)));
// DEFAULT-NEXT:                                 if read<bool>(%[[VALUE9]])
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 call<void, signature=fn(i32) -> void>(%[[VALUE_f3]], add<i32, overflow=ub>(add<i32, overflow=ub>(const<i32>(13), mul<i32, overflow=ub>(read<i32>(%[[VALUE_i]]), const<i32>(16))), read<i32>(%[[VALUE_j]])));
// DEFAULT-NEXT:                                 call<void, signature=fn(i32) -> void>(%[[VALUE_f4]], add<i32, overflow=ub>(add<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(13)), mul<i32, overflow=ub>(read<i32>(%[[VALUE_i]]), const<i32>(16))), read<i32>(%[[VALUE_j]])));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%[[VALUE_c]]), add<i32, overflow=ub>(const<i32>(1), from_bool<i32, reason=promotion>(eq<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0)))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_c]], const<i32>(0));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
