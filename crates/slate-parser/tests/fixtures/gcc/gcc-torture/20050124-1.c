/* PR rtl-optimization/19579 */

extern void abort(void);

int foo(int i, int j) {
  int k = i + 1;

  if (j) {
    if (k > 0)
      k++;
    else if (k < 0)
      k--;
  }

  return k;
}

int main(void) {
  if (foo(-2, 0) != -1)
    abort();
  if (foo(-1, 0) != 0)
    abort();
  if (foo(0, 0) != 1)
    abort();
  if (foo(1, 0) != 2)
    abort();
  if (foo(-2, 1) != -2)
    abort();
  if (foo(-1, 1) != 0)
    abort();
  if (foo(0, 1) != 2)
    abort();
  if (foo(1, 1) != 3)
    abort();
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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @foo(%2 i: i32, %3 j: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %4 k: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%2), const<i32>(1));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%3), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if gt<i32>(read<i32>(%4), const<i32>(0))
// DEFAULT-NEXT:                     let %6: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:                     let %7: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%6), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%4, read<i32>(%7));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     if lt<i32>(read<i32>(%4), const<i32>(0))
// DEFAULT-NEXT:                         let %8: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:                         let %9: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%8), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%4, read<i32>(%9));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<i32>(%4);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%1, neg<i32, overflow=ub>(const<i32>(2)), const<i32>(0)), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%1, neg<i32, overflow=ub>(const<i32>(1)), const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%1, const<i32>(0), const<i32>(0)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%1, const<i32>(1), const<i32>(0)), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%1, neg<i32, overflow=ub>(const<i32>(2)), const<i32>(1)), neg<i32, overflow=ub>(const<i32>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%1, neg<i32, overflow=ub>(const<i32>(1)), const<i32>(1)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%1, const<i32>(0), const<i32>(1)), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%1, const<i32>(1), const<i32>(1)), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
