/* PR c/77767 */

void foo(int a, int b[a++], int c, int d[c++]) {
  if (a != 2 || c != 2)
    __builtin_abort();
}

int main() {
  int e[10];
  foo(1, e, 1, e);
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
// DEFAULT-NEXT:     fn %9 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %0 @foo(%1 a: i32, %2 b: ptr<i32> [array=%7], %3 c: i32, %4 d: ptr<i32> [array=%8]) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %10: i32 [synthetic] = read<i32>(%1);
// DEFAULT-NEXT:         let %11: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%10), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%1, read<i32>(%11));
// DEFAULT-NEXT:         let %7: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%10)));
// DEFAULT-NEXT:         let %12: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:         let %13: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%12), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%3, read<i32>(%13));
// DEFAULT-NEXT:         let %8: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%12)));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(%1), const<i32>(2)), ne<i32>(read<i32>(%3), const<i32>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %6 e: array<i32, 10> [storage=automatic] [align=16];
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i32>, i32, ptr<i32>) -> void>(%0, const<i32>(1), array_decay<ptr<i32>, length=Some(10)>(%6), const<i32>(1), array_decay<ptr<i32>, length=Some(10)>(%6));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
