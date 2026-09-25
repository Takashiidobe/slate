/* PR rtl-optimization/106032 */

__attribute__((noipa)) int foo(int x, int *y) {
  int a = 0;
  if (x < 0)
    a = *y;
  return a;
}

int main() {
  int a = 42;
  if (foo(0, 0) != 0 || foo(1, 0) != 0)
    __builtin_abort();
  if (foo(-1, &a) != 42 || foo(-42, &a) != 42)
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
// DEFAULT-NEXT:     fn %0 @foo(%1 x: i32, %2 y: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 a: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%1), const<i32>(0))
// DEFAULT-NEXT:             write<i32>(%3, read<i32>(deref(read<ptr<i32>>(%2))));
// DEFAULT-NEXT:         return read<i32>(%3);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %5 a: i32 [storage=automatic] = const<i32>(42);
// DEFAULT-NEXT:         let %6: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, ptr<i32>) -> i32>(%0, const<i32>(0), null<ptr<i32>>), const<i32>(0))
// DEFAULT-NEXT:             write<bool>(%6, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%6, ne<i32>(call<i32, signature=fn(i32, ptr<i32>) -> i32>(%0, const<i32>(1), null<ptr<i32>>), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%6)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         let %7: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, ptr<i32>) -> i32>(%0, neg<i32, overflow=ub>(const<i32>(1)), addr_of<ptr<i32>>(%5)), const<i32>(42))
// DEFAULT-NEXT:             write<bool>(%7, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%7, ne<i32>(call<i32, signature=fn(i32, ptr<i32>) -> i32>(%0, neg<i32, overflow=ub>(const<i32>(42)), addr_of<ptr<i32>>(%5)), const<i32>(42)));
// DEFAULT-NEXT:         if read<bool>(%7)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
