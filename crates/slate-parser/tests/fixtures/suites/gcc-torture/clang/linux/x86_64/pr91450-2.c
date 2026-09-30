/* PR middle-end/91450 */

__attribute__((noipa)) void foo(int a, int b) {
  unsigned long long r;
  if (__builtin_mul_overflow(a, b, &r))
    __builtin_abort();
  if (r != 0)
    __builtin_abort();
}

__attribute__((noipa)) void bar(int a, int b) {
  unsigned long long r;
  if (a >= 0)
    return;
  if (__builtin_mul_overflow(a, b, &r))
    __builtin_abort();
  if (r != 0)
    __builtin_abort();
}

__attribute__((noipa)) void baz(int a, int b) {
  unsigned long long r;
  if (b >= 0)
    return;
  if (__builtin_mul_overflow(a, b, &r))
    __builtin_abort();
  if (r != 0)
    __builtin_abort();
}

__attribute__((noipa)) void qux(int a, int b) {
  unsigned long long r;
  if (a >= 0)
    return;
  if (b < 0)
    return;
  if (__builtin_mul_overflow(a, b, &r))
    __builtin_abort();
  if (r != 0)
    __builtin_abort();
}

__attribute__((noipa)) void quux(int a, int b) {
  unsigned long long r;
  if (a < 0)
    return;
  if (b >= 0)
    return;
  if (__builtin_mul_overflow(a, b, &r))
    __builtin_abort();
  if (r != 0)
    __builtin_abort();
}

int main() {
  foo(-4, 0);
  foo(0, -4);
  foo(0, 0);
  bar(-4, 0);
  baz(0, -4);
  qux(-4, 0);
  quux(0, -4);
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
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_a:[0-9]+]] a: i32, %[[VALUE_b:[0-9]+]] b: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: u64 [storage=automatic];
// DEFAULT-NEXT:         if overflow_mul<bool>(read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]]), deref(addr_of<ptr<u64>>(%[[VALUE_r]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<u64>(read<u64>(%[[VALUE_r]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_a_2:[0-9]+]] a: i32, %[[VALUE_b_2:[0-9]+]] b: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_r_2:[0-9]+]] r: u64 [storage=automatic];
// DEFAULT-NEXT:         if ge<i32>(read<i32>(%[[VALUE_a_2]]), const<i32>(0))
// DEFAULT-NEXT:             return;
// DEFAULT-NEXT:         if overflow_mul<bool>(read<i32>(%[[VALUE_a_2]]), read<i32>(%[[VALUE_b_2]]), deref(addr_of<ptr<u64>>(%[[VALUE_r_2]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<u64>(read<u64>(%[[VALUE_r_2]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_baz:[0-9]+]] @baz(%[[VALUE_a_3:[0-9]+]] a: i32, %[[VALUE_b_3:[0-9]+]] b: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_r_3:[0-9]+]] r: u64 [storage=automatic];
// DEFAULT-NEXT:         if ge<i32>(read<i32>(%[[VALUE_b_3]]), const<i32>(0))
// DEFAULT-NEXT:             return;
// DEFAULT-NEXT:         if overflow_mul<bool>(read<i32>(%[[VALUE_a_3]]), read<i32>(%[[VALUE_b_3]]), deref(addr_of<ptr<u64>>(%[[VALUE_r_3]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<u64>(read<u64>(%[[VALUE_r_3]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_qux:[0-9]+]] @qux(%[[VALUE_a_4:[0-9]+]] a: i32, %[[VALUE_b_4:[0-9]+]] b: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_r_4:[0-9]+]] r: u64 [storage=automatic];
// DEFAULT-NEXT:         if ge<i32>(read<i32>(%[[VALUE_a_4]]), const<i32>(0))
// DEFAULT-NEXT:             return;
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%[[VALUE_b_4]]), const<i32>(0))
// DEFAULT-NEXT:             return;
// DEFAULT-NEXT:         if overflow_mul<bool>(read<i32>(%[[VALUE_a_4]]), read<i32>(%[[VALUE_b_4]]), deref(addr_of<ptr<u64>>(%[[VALUE_r_4]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<u64>(read<u64>(%[[VALUE_r_4]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_quux:[0-9]+]] @quux(%[[VALUE_a_5:[0-9]+]] a: i32, %[[VALUE_b_5:[0-9]+]] b: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_r_5:[0-9]+]] r: u64 [storage=automatic];
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%[[VALUE_a_5]]), const<i32>(0))
// DEFAULT-NEXT:             return;
// DEFAULT-NEXT:         if ge<i32>(read<i32>(%[[VALUE_b_5]]), const<i32>(0))
// DEFAULT-NEXT:             return;
// DEFAULT-NEXT:         if overflow_mul<bool>(read<i32>(%[[VALUE_a_5]]), read<i32>(%[[VALUE_b_5]]), deref(addr_of<ptr<u64>>(%[[VALUE_r_5]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<u64>(read<u64>(%[[VALUE_r_5]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_foo]], neg<i32, overflow=ub>(const<i32>(4)), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_foo]], const<i32>(0), neg<i32, overflow=ub>(const<i32>(4)));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_foo]], const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_bar]], neg<i32, overflow=ub>(const<i32>(4)), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_baz]], const<i32>(0), neg<i32, overflow=ub>(const<i32>(4)));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_qux]], neg<i32, overflow=ub>(const<i32>(4)), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_quux]], const<i32>(0), neg<i32, overflow=ub>(const<i32>(4)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
