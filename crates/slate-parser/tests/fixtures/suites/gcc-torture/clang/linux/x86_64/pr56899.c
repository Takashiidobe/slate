/* PR tree-optimization/56899 */

#if __SIZEOF_INT__ == 4 && __CHAR_BIT__ == 8
__attribute__((noinline, noclone)) void f1(int v) {
  int x = -214748365 * (v - 1);
  if (x != -1932735285)
    __builtin_abort();
}

__attribute__((noinline, noclone)) void f2(int v) {
  int x = 214748365 * (v + 1);
  if (x != -1932735285)
    __builtin_abort();
}

__attribute__((noinline, noclone)) void f3(unsigned int v) {
  unsigned int x = -214748365U * (v - 1);
  if (x != -1932735285U)
    __builtin_abort();
}

__attribute__((noinline, noclone)) void f4(unsigned int v) {
  unsigned int x = 214748365U * (v + 1);
  if (x != -1932735285U)
    __builtin_abort();
}
#endif

int main() {
#if __SIZEOF_INT__ == 4 && __CHAR_BIT__ == 8
  f1(10);
  f2(-10);
  f3(10);
  f4(-10U);
#endif
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
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_f1:[0-9]+]] @f1(%[[VALUE_v:[0-9]+]] v: i32) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: i32 [storage=automatic] = mul<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(214748365)), sub<i32, overflow=ub>(read<i32>(%[[VALUE_v]]), const<i32>(1)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_x]]), neg<i32, overflow=ub>(const<i32>(1932735285)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f2:[0-9]+]] @f2(%[[VALUE_v_2:[0-9]+]] v: i32) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_x_2:[0-9]+]] x: i32 [storage=automatic] = mul<i32, overflow=ub>(const<i32>(214748365), add<i32, overflow=ub>(read<i32>(%[[VALUE_v_2]]), const<i32>(1)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_x_2]]), neg<i32, overflow=ub>(const<i32>(1932735285)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f3:[0-9]+]] @f3(%[[VALUE_v_3:[0-9]+]] v: u32) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_x_3:[0-9]+]] x: u32 [storage=automatic] = mul<u32, overflow=wrap>(neg<u32, overflow=wrap>(const<u32>(214748365)), sub<u32, overflow=wrap>(read<u32>(%[[VALUE_v_3]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%[[VALUE_x_3]]), neg<u32, overflow=wrap>(const<u32>(1932735285)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f4:[0-9]+]] @f4(%[[VALUE_v_4:[0-9]+]] v: u32) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_x_4:[0-9]+]] x: u32 [storage=automatic] = mul<u32, overflow=wrap>(const<u32>(214748365), add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_4]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%[[VALUE_x_4]]), neg<u32, overflow=wrap>(const<u32>(1932735285)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_f1]], const<i32>(10));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_f2]], neg<i32, overflow=ub>(const<i32>(10)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_f3]], reinterpret<u32, reason=arg, fits=always>(const<i32>(10)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_f4]], neg<u32, overflow=wrap>(const<u32>(10)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
