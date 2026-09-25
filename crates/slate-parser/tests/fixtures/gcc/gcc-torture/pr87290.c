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
// DEFAULT-NEXT:     global %0 c: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %1 @f0() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %15: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:         let %16: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%15), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%0, read<i32>(%16));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @f1(%3 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%3), const<i32>(16)), const<i32>(13)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @f2(%5 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%5), const<i32>(16)), neg<i32, overflow=ub>(const<i32>(13))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @f3(%7 x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%7), const<i32>(16)), const<i32>(13))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @f4(%9 x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%9), const<i32>(16)), neg<i32, overflow=ub>(const<i32>(13)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %11 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %12 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %13
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%11, neg<i32, overflow=ub>(const<i32>(30)));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%11), const<i32>(30))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %17: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:                 let %18: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%17), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%11, read<i32>(%18));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %19: bool [synthetic];
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i32) -> i32>(%2, add<i32, overflow=ub>(const<i32>(13), mul<i32, overflow=ub>(read<i32>(%11), const<i32>(16)))), from_bool<i32, reason=promotion>(ge<i32>(read<i32>(%11), const<i32>(0))))
// DEFAULT-NEXT:                         write<bool>(%19, const<bool>(true));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         write<bool>(%19, ne<i32>(call<i32, signature=fn(i32) -> i32>(%4, add<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(13)), mul<i32, overflow=ub>(read<i32>(%11), const<i32>(16)))), from_bool<i32, reason=promotion>(le<i32>(read<i32>(%11), const<i32>(0)))));
// DEFAULT-NEXT:                     if read<bool>(%19)
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                     call<void, signature=fn(i32) -> void>(%6, add<i32, overflow=ub>(const<i32>(13), mul<i32, overflow=ub>(read<i32>(%11), const<i32>(16))));
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%0), from_bool<i32, reason=promotion>(ge<i32>(read<i32>(%11), const<i32>(0))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                     call<void, signature=fn(i32) -> void>(%8, add<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(13)), mul<i32, overflow=ub>(read<i32>(%11), const<i32>(16))));
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%0), add<i32, overflow=ub>(const<i32>(1), from_bool<i32, reason=promotion>(eq<i32>(read<i32>(%11), const<i32>(0)))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                     for %14
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             write<i32>(%12, const<i32>(1));
// DEFAULT-NEXT:                         condition: lt<i32>(read<i32>(%12), const<i32>(16))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %20: i32 [synthetic] = read<i32>(%12);
// DEFAULT-NEXT:                             let %21: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%20), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%12, read<i32>(%21));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %22: bool [synthetic];
// DEFAULT-NEXT:                                 if ne<i32>(call<i32, signature=fn(i32) -> i32>(%2, add<i32, overflow=ub>(add<i32, overflow=ub>(const<i32>(13), mul<i32, overflow=ub>(read<i32>(%11), const<i32>(16))), read<i32>(%12))), const<i32>(0))
// DEFAULT-NEXT:                                     write<bool>(%22, const<bool>(true));
// DEFAULT-NEXT:                                 else
// DEFAULT-NEXT:                                     write<bool>(%22, ne<i32>(call<i32, signature=fn(i32) -> i32>(%4, add<i32, overflow=ub>(add<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(13)), mul<i32, overflow=ub>(read<i32>(%11), const<i32>(16))), read<i32>(%12))), const<i32>(0)));
// DEFAULT-NEXT:                                 if read<bool>(%22)
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                                 call<void, signature=fn(i32) -> void>(%6, add<i32, overflow=ub>(add<i32, overflow=ub>(const<i32>(13), mul<i32, overflow=ub>(read<i32>(%11), const<i32>(16))), read<i32>(%12)));
// DEFAULT-NEXT:                                 call<void, signature=fn(i32) -> void>(%8, add<i32, overflow=ub>(add<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(13)), mul<i32, overflow=ub>(read<i32>(%11), const<i32>(16))), read<i32>(%12)));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%0), add<i32, overflow=ub>(const<i32>(1), from_bool<i32, reason=promotion>(eq<i32>(read<i32>(%11), const<i32>(0)))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                     write<i32>(%0, const<i32>(0));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
