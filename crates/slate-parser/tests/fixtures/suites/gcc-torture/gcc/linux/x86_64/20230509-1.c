int __attribute__((noipa)) f(unsigned a, int b) {
  if (a < 0)
    __builtin_unreachable();
  if (a > 30)
    __builtin_unreachable();
  int t = a;
  if (b)
    t = 100;
  else if (a != 0)
    t = a;
  else
    t = 1;
  return t;
}

int main(void) {
  if (f(0, 0) != 1)
    __builtin_abort();
  if (f(1, 0) != 1)
    __builtin_abort();
  if (f(0, 1) != 100)
    __builtin_abort();
  if (f(1, 0) != 1)
    __builtin_abort();
  if (f(30, 0) != 30)
    __builtin_abort();
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
// DEFAULT-NEXT:     fn %[[VALUE___builtin_unreachable:[0-9]+]] @__builtin_unreachable() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_a:[0-9]+]] a: u32, %[[VALUE_b:[0-9]+]] b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if lt<u32>(read<u32>(%[[VALUE_a]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_unreachable]]);
// DEFAULT-NEXT:         if gt<u32>(read<u32>(%[[VALUE_a]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(30)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_unreachable]]);
// DEFAULT-NEXT:         let %[[VALUE_t:[0-9]+]] t: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(read<u32>(%[[VALUE_a]]));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_b]]), const<i32>(0))
// DEFAULT-NEXT:             write<i32>(%[[VALUE_t]], const<i32>(100));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<u32>(read<u32>(%[[VALUE_a]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_t]], reinterpret<i32, reason=assign, fits=unknown>(read<u32>(%[[VALUE_a]])));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_t]], const<i32>(1));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_t]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32, i32) -> i32>(%[[VALUE_f]], reinterpret<u32, reason=arg, fits=always>(const<i32>(0)), const<i32>(0)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32, i32) -> i32>(%[[VALUE_f]], reinterpret<u32, reason=arg, fits=always>(const<i32>(1)), const<i32>(0)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32, i32) -> i32>(%[[VALUE_f]], reinterpret<u32, reason=arg, fits=always>(const<i32>(0)), const<i32>(1)), const<i32>(100))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32, i32) -> i32>(%[[VALUE_f]], reinterpret<u32, reason=arg, fits=always>(const<i32>(1)), const<i32>(0)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32, i32) -> i32>(%[[VALUE_f]], reinterpret<u32, reason=arg, fits=always>(const<i32>(30)), const<i32>(0)), const<i32>(30))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
