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
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_v:[0-9]+]] v: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f1:[0-9]+]] @f1() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_v]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE0]]), from_bool<i32, reason=promotion>(ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_c]])), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_v]], read<i32>(%[[VALUE1]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f2:[0-9]+]] @f2() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_v]]);
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE2]]), from_bool<i32, reason=promotion>(ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_c]])), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_v]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f3:[0-9]+]] @f3() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_v]]);
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), from_bool<i32, reason=promotion>(eq<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_c]])), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_v]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f4:[0-9]+]] @f4() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_v]]);
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE6]]), from_bool<i32, reason=promotion>(eq<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_c]])), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_v]], read<i32>(%[[VALUE7]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f5:[0-9]+]] @f5() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_v]]);
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE8]]), sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_c]])), const<i32>(0))), const<i32>(26)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_v]], read<i32>(%[[VALUE9]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f6:[0-9]+]] @f6() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_v]]);
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE10]]), sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_c]])), const<i32>(0))), const<i32>(26)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_v]], read<i32>(%[[VALUE11]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f7:[0-9]+]] @f7() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_v]]);
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE12]]), sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(eq<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_c]])), const<i32>(0))), const<i32>(26)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_v]], read<i32>(%[[VALUE13]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f8:[0-9]+]] @f8() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE14:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_v]]);
// DEFAULT-NEXT:         let %[[VALUE15:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE14]]), sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(eq<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_c]])), const<i32>(0))), const<i32>(26)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_v]], read<i32>(%[[VALUE15]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f9:[0-9]+]] @f9() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE16:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_v]]);
// DEFAULT-NEXT:         let %[[VALUE17:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE16]]), add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_c]])), const<i32>(0))), const<i32>(42)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_v]], read<i32>(%[[VALUE17]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f10:[0-9]+]] @f10() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE18:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_v]]);
// DEFAULT-NEXT:         let %[[VALUE19:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE18]]), add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_c]])), const<i32>(0))), const<i32>(42)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_v]], read<i32>(%[[VALUE19]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f11:[0-9]+]] @f11() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE20:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_v]]);
// DEFAULT-NEXT:         let %[[VALUE21:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE20]]), add<i32, overflow=ub>(from_bool<i32, reason=promotion>(eq<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_c]])), const<i32>(0))), const<i32>(42)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_v]], read<i32>(%[[VALUE21]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f12:[0-9]+]] @f12() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE22:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_v]]);
// DEFAULT-NEXT:         let %[[VALUE23:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE22]]), add<i32, overflow=ub>(from_bool<i32, reason=promotion>(eq<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_c]])), const<i32>(0))), const<i32>(42)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_v]], read<i32>(%[[VALUE23]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f13:[0-9]+]] @f13(%[[VALUE_z:[0-9]+]] z: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE24:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_v]]);
// DEFAULT-NEXT:         let %[[VALUE25:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE24]]), add<i32, overflow=ub>(from_bool<i32, reason=promotion>(eq<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_c]])), const<i32>(0))), read<i32>(%[[VALUE_z]])));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_v]], read<i32>(%[[VALUE25]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f14:[0-9]+]] @f14(%[[VALUE_z_2:[0-9]+]] z: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE26:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_v]]);
// DEFAULT-NEXT:         let %[[VALUE27:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE26]]), add<i32, overflow=ub>(from_bool<i32, reason=promotion>(eq<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_c]])), const<i32>(0))), read<i32>(%[[VALUE_z_2]])));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_v]], read<i32>(%[[VALUE27]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f15:[0-9]+]] @f15(%[[VALUE_n:[0-9]+]] n: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(conditional<i32>(ne<u32>(read<u32>(%[[VALUE_n]]), const<u32>(0)), const<i32>(2), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE28:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(2))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE29:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE30:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE29]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE30]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_v]], const<i32>(15));
// DEFAULT-NEXT:                     if eq<i32>(read<i32>(%[[VALUE_i]]), const<i32>(1))
// DEFAULT-NEXT:                         write<i8>(%[[VALUE_c]], truncate<i8, reason=assign, fits=always>(const<i32>(37)));
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_f1]]);
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%[[VALUE_v]]), add<i32, overflow=ub>(const<i32>(15), read<i32>(%[[VALUE_i]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_f2]]);
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%[[VALUE_v]]), const<i32>(15))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_f3]]);
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%[[VALUE_v]]), sub<i32, overflow=ub>(const<i32>(16), read<i32>(%[[VALUE_i]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_f4]]);
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%[[VALUE_v]]), const<i32>(15))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_f5]]);
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%[[VALUE_v]]), sub<i32, overflow=ub>(add<i32, overflow=ub>(const<i32>(15), read<i32>(%[[VALUE_i]])), const<i32>(26)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_f6]]);
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%[[VALUE_v]]), const<i32>(15))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_f7]]);
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%[[VALUE_v]]), sub<i32, overflow=ub>(sub<i32, overflow=ub>(const<i32>(16), read<i32>(%[[VALUE_i]])), const<i32>(26)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_f8]]);
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%[[VALUE_v]]), const<i32>(15))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_f9]]);
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%[[VALUE_v]]), add<i32, overflow=ub>(add<i32, overflow=ub>(const<i32>(15), read<i32>(%[[VALUE_i]])), const<i32>(42)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_f10]]);
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%[[VALUE_v]]), const<i32>(15))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_f11]]);
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%[[VALUE_v]]), add<i32, overflow=ub>(sub<i32, overflow=ub>(const<i32>(16), read<i32>(%[[VALUE_i]])), const<i32>(42)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_f12]]);
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%[[VALUE_v]]), const<i32>(15))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32) -> void>(%[[VALUE_f13]], const<i32>(173));
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%[[VALUE_v]]), add<i32, overflow=ub>(sub<i32, overflow=ub>(const<i32>(16), read<i32>(%[[VALUE_i]])), const<i32>(173)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32) -> void>(%[[VALUE_f14]], const<i32>(173));
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%[[VALUE_v]]), const<i32>(15))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32) -> void>(%[[VALUE_f13]], neg<i32, overflow=ub>(const<i32>(35)));
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%[[VALUE_v]]), sub<i32, overflow=ub>(sub<i32, overflow=ub>(const<i32>(16), read<i32>(%[[VALUE_i]])), const<i32>(35)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32) -> void>(%[[VALUE_f14]], neg<i32, overflow=ub>(const<i32>(35)));
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%[[VALUE_v]]), const<i32>(15))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         let %[[VALUE31:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_f15]], reinterpret<u32, reason=arg, fits=always>(const<i32>(0))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))
// DEFAULT-NEXT:             write<bool>(%[[VALUE31]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE31]], ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_f15]], reinterpret<u32, reason=arg, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:         let %[[VALUE32:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE31]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE32]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE32]], ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_f15]], reinterpret<u32, reason=arg, fits=always>(const<i32>(371))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE32]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
