/* PR target/92140 */

char c;
int  v;

__attribute__((noipa)) void         f1(void) { v += c != 0; }
__attribute__((noipa)) void         f2(void) { v -= c != 0; }
__attribute__((noipa)) void         f3(void) { v += c == 0; }
__attribute__((noipa)) void         f4(void) { v -= c == 0; }
__attribute__((noipa)) void         f5(void) { v += (c != 0) - 26; }
__attribute__((noipa)) void         f6(void) { v -= (c != 0) - 26; }
__attribute__((noipa)) void         f7(void) { v += (c == 0) - 26; }
__attribute__((noipa)) void         f8(void) { v -= (c == 0) - 26; }
__attribute__((noipa)) void         f9(void) { v += (c != 0) + 42; }
__attribute__((noipa)) void         f10(void) { v -= (c != 0) + 42; }
__attribute__((noipa)) void         f11(void) { v += (c == 0) + 42; }
__attribute__((noipa)) void         f12(void) { v -= (c == 0) + 42; }
__attribute__((noipa)) void         f13(int z) { v += (c == 0) + z; }
__attribute__((noipa)) void         f14(int z) { v -= (c == 0) + z; }
__attribute__((noipa)) unsigned int f15(unsigned int n) { return n ? 2 : 1; }

int main() {
  int i;
  for (i = 0; i < 2; i++) {
    v = 15;
    if (i == 1)
      c = 37;
    f1();
    if (v != 15 + i)
      __builtin_abort();
    f2();
    if (v != 15)
      __builtin_abort();
    f3();
    if (v != 16 - i)
      __builtin_abort();
    f4();
    if (v != 15)
      __builtin_abort();
    f5();
    if (v != 15 + i - 26)
      __builtin_abort();
    f6();
    if (v != 15)
      __builtin_abort();
    f7();
    if (v != 16 - i - 26)
      __builtin_abort();
    f8();
    if (v != 15)
      __builtin_abort();
    f9();
    if (v != 15 + i + 42)
      __builtin_abort();
    f10();
    if (v != 15)
      __builtin_abort();
    f11();
    if (v != 16 - i + 42)
      __builtin_abort();
    f12();
    if (v != 15)
      __builtin_abort();
    f13(173);
    if (v != 16 - i + 173)
      __builtin_abort();
    f14(173);
    if (v != 15)
      __builtin_abort();
    f13(-35);
    if (v != 16 - i - 35)
      __builtin_abort();
    f14(-35);
    if (v != 15)
      __builtin_abort();
  }
  if (f15(0) != 1 || f15(1) != 2 || f15(371) != 2)
    __builtin_abort();
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
// DEFAULT-NEXT:     global %0 c: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 v: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %2 @f1() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %24: i32 [synthetic] = read<i32>(%1);
// DEFAULT-NEXT:         let %25: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%24), from_bool<i32, reason=promotion>(ne<i32>(widen<i32, reason=promotion>(read<i8>(%0)), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%1, read<i32>(%25));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @f2() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %26: i32 [synthetic] = read<i32>(%1);
// DEFAULT-NEXT:         let %27: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%26), from_bool<i32, reason=promotion>(ne<i32>(widen<i32, reason=promotion>(read<i8>(%0)), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%1, read<i32>(%27));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @f3() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %28: i32 [synthetic] = read<i32>(%1);
// DEFAULT-NEXT:         let %29: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%28), from_bool<i32, reason=promotion>(eq<i32>(widen<i32, reason=promotion>(read<i8>(%0)), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%1, read<i32>(%29));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @f4() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %30: i32 [synthetic] = read<i32>(%1);
// DEFAULT-NEXT:         let %31: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%30), from_bool<i32, reason=promotion>(eq<i32>(widen<i32, reason=promotion>(read<i8>(%0)), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%1, read<i32>(%31));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @f5() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %32: i32 [synthetic] = read<i32>(%1);
// DEFAULT-NEXT:         let %33: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%32), sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32>(widen<i32, reason=promotion>(read<i8>(%0)), const<i32>(0))), const<i32>(26)));
// DEFAULT-NEXT:         write<i32>(%1, read<i32>(%33));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @f6() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %34: i32 [synthetic] = read<i32>(%1);
// DEFAULT-NEXT:         let %35: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%34), sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32>(widen<i32, reason=promotion>(read<i8>(%0)), const<i32>(0))), const<i32>(26)));
// DEFAULT-NEXT:         write<i32>(%1, read<i32>(%35));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @f7() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %36: i32 [synthetic] = read<i32>(%1);
// DEFAULT-NEXT:         let %37: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%36), sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(eq<i32>(widen<i32, reason=promotion>(read<i8>(%0)), const<i32>(0))), const<i32>(26)));
// DEFAULT-NEXT:         write<i32>(%1, read<i32>(%37));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @f8() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %38: i32 [synthetic] = read<i32>(%1);
// DEFAULT-NEXT:         let %39: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%38), sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(eq<i32>(widen<i32, reason=promotion>(read<i8>(%0)), const<i32>(0))), const<i32>(26)));
// DEFAULT-NEXT:         write<i32>(%1, read<i32>(%39));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @f9() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %40: i32 [synthetic] = read<i32>(%1);
// DEFAULT-NEXT:         let %41: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%40), add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32>(widen<i32, reason=promotion>(read<i8>(%0)), const<i32>(0))), const<i32>(42)));
// DEFAULT-NEXT:         write<i32>(%1, read<i32>(%41));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @f10() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %42: i32 [synthetic] = read<i32>(%1);
// DEFAULT-NEXT:         let %43: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%42), add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32>(widen<i32, reason=promotion>(read<i8>(%0)), const<i32>(0))), const<i32>(42)));
// DEFAULT-NEXT:         write<i32>(%1, read<i32>(%43));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @f11() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %44: i32 [synthetic] = read<i32>(%1);
// DEFAULT-NEXT:         let %45: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%44), add<i32, overflow=ub>(from_bool<i32, reason=promotion>(eq<i32>(widen<i32, reason=promotion>(read<i8>(%0)), const<i32>(0))), const<i32>(42)));
// DEFAULT-NEXT:         write<i32>(%1, read<i32>(%45));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @f12() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %46: i32 [synthetic] = read<i32>(%1);
// DEFAULT-NEXT:         let %47: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%46), add<i32, overflow=ub>(from_bool<i32, reason=promotion>(eq<i32>(widen<i32, reason=promotion>(read<i8>(%0)), const<i32>(0))), const<i32>(42)));
// DEFAULT-NEXT:         write<i32>(%1, read<i32>(%47));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @f13(%15 z: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %48: i32 [synthetic] = read<i32>(%1);
// DEFAULT-NEXT:         let %49: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%48), add<i32, overflow=ub>(from_bool<i32, reason=promotion>(eq<i32>(widen<i32, reason=promotion>(read<i8>(%0)), const<i32>(0))), read<i32>(%15)));
// DEFAULT-NEXT:         write<i32>(%1, read<i32>(%49));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @f14(%17 z: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %50: i32 [synthetic] = read<i32>(%1);
// DEFAULT-NEXT:         let %51: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%50), add<i32, overflow=ub>(from_bool<i32, reason=promotion>(eq<i32>(widen<i32, reason=promotion>(read<i8>(%0)), const<i32>(0))), read<i32>(%17)));
// DEFAULT-NEXT:         write<i32>(%1, read<i32>(%51));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @f15(%19 n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(conditional<i32>(ne<u32>(read<u32>(%19), const<u32>(0)), const<i32>(2), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %20 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %21 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %22
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%21, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%21), const<i32>(2))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %52: i32 [synthetic] = read<i32>(%21);
// DEFAULT-NEXT:                 let %53: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%52), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%21, read<i32>(%53));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(%1, const<i32>(15));
// DEFAULT-NEXT:                     if eq<i32>(read<i32>(%21), const<i32>(1))
// DEFAULT-NEXT:                         write<i8>(%0, truncate<i8, reason=assign, fits=always>(const<i32>(37)));
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%1), add<i32, overflow=ub>(const<i32>(15), read<i32>(%21)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%23);
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%1), const<i32>(15))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%23);
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%1), sub<i32, overflow=ub>(const<i32>(16), read<i32>(%21)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%23);
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%1), const<i32>(15))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%23);
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%1), sub<i32, overflow=ub>(add<i32, overflow=ub>(const<i32>(15), read<i32>(%21)), const<i32>(26)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%23);
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%7);
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%1), const<i32>(15))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%23);
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%1), sub<i32, overflow=ub>(sub<i32, overflow=ub>(const<i32>(16), read<i32>(%21)), const<i32>(26)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%23);
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%1), const<i32>(15))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%23);
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%10);
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%1), add<i32, overflow=ub>(add<i32, overflow=ub>(const<i32>(15), read<i32>(%21)), const<i32>(42)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%23);
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%11);
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%1), const<i32>(15))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%23);
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%12);
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%1), add<i32, overflow=ub>(sub<i32, overflow=ub>(const<i32>(16), read<i32>(%21)), const<i32>(42)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%23);
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%13);
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%1), const<i32>(15))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%23);
// DEFAULT-NEXT:                     call<void, signature=fn(i32) -> void>(%14, const<i32>(173));
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%1), add<i32, overflow=ub>(sub<i32, overflow=ub>(const<i32>(16), read<i32>(%21)), const<i32>(173)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%23);
// DEFAULT-NEXT:                     call<void, signature=fn(i32) -> void>(%16, const<i32>(173));
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%1), const<i32>(15))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%23);
// DEFAULT-NEXT:                     call<void, signature=fn(i32) -> void>(%14, neg<i32, overflow=ub>(const<i32>(35)));
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%1), sub<i32, overflow=ub>(sub<i32, overflow=ub>(const<i32>(16), read<i32>(%21)), const<i32>(35)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%23);
// DEFAULT-NEXT:                     call<void, signature=fn(i32) -> void>(%16, neg<i32, overflow=ub>(const<i32>(35)));
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%1), const<i32>(15))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%23);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         let %54: bool [synthetic];
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%18, reinterpret<u32, reason=arg, fits=always>(const<i32>(0))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))
// DEFAULT-NEXT:             write<bool>(%54, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%54, ne<u32>(call<u32, signature=fn(u32) -> u32>(%18, reinterpret<u32, reason=arg, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:         let %55: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%54)
// DEFAULT-NEXT:             write<bool>(%55, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%55, ne<u32>(call<u32, signature=fn(u32) -> u32>(%18, reinterpret<u32, reason=arg, fits=always>(const<i32>(371))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:         if read<bool>(%55)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%23);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
