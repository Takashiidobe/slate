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
// DEFAULT-NEXT:     fn %0 @foo(%1 a: i32, %2 b: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %3 r: u64 [storage=automatic];
// DEFAULT-NEXT:         if overflow_mul<bool>(read<i32>(%1), read<i32>(%2), deref(addr_of<ptr<u64>>(%3)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if ne<u64>(read<u64>(%3), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @bar(%5 a: i32, %6 b: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %7 r: u64 [storage=automatic];
// DEFAULT-NEXT:         if ge<i32>(read<i32>(%5), const<i32>(0))
// DEFAULT-NEXT:             return;
// DEFAULT-NEXT:         if overflow_mul<bool>(read<i32>(%5), read<i32>(%6), deref(addr_of<ptr<u64>>(%7)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if ne<u64>(read<u64>(%7), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @baz(%9 a: i32, %10 b: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %11 r: u64 [storage=automatic];
// DEFAULT-NEXT:         if ge<i32>(read<i32>(%10), const<i32>(0))
// DEFAULT-NEXT:             return;
// DEFAULT-NEXT:         if overflow_mul<bool>(read<i32>(%9), read<i32>(%10), deref(addr_of<ptr<u64>>(%11)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if ne<u64>(read<u64>(%11), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @qux(%13 a: i32, %14 b: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %15 r: u64 [storage=automatic];
// DEFAULT-NEXT:         if ge<i32>(read<i32>(%13), const<i32>(0))
// DEFAULT-NEXT:             return;
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%14), const<i32>(0))
// DEFAULT-NEXT:             return;
// DEFAULT-NEXT:         if overflow_mul<bool>(read<i32>(%13), read<i32>(%14), deref(addr_of<ptr<u64>>(%15)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if ne<u64>(read<u64>(%15), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @quux(%17 a: i32, %18 b: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %19 r: u64 [storage=automatic];
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%17), const<i32>(0))
// DEFAULT-NEXT:             return;
// DEFAULT-NEXT:         if ge<i32>(read<i32>(%18), const<i32>(0))
// DEFAULT-NEXT:             return;
// DEFAULT-NEXT:         if overflow_mul<bool>(read<i32>(%17), read<i32>(%18), deref(addr_of<ptr<u64>>(%19)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if ne<u64>(read<u64>(%19), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%0, neg<i32, overflow=ub>(const<i32>(4)), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%0, const<i32>(0), neg<i32, overflow=ub>(const<i32>(4)));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%0, const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%4, neg<i32, overflow=ub>(const<i32>(4)), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%8, const<i32>(0), neg<i32, overflow=ub>(const<i32>(4)));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%12, neg<i32, overflow=ub>(const<i32>(4)), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%16, const<i32>(0), neg<i32, overflow=ub>(const<i32>(4)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
