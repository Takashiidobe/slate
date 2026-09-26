/* PR tree-optimization/49039 */
extern void abort(void);
int         cnt;

__attribute__((noinline, noclone)) void foo(unsigned int x, unsigned int y) {
  unsigned int minv, maxv;
  if (x == 1 || y == -2U)
    return;
  minv = x < y ? x : y;
  maxv = x > y ? x : y;
  if (minv == 1)
    ++cnt;
  if (maxv == -2U)
    ++cnt;
}

int main() {
  foo(-2U, 1);
  if (cnt != 2)
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
// DEFAULT-NEXT:     global %1 cnt: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @foo(%3 x: u32, %4 y: u32) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %5 minv: u32 [storage=automatic];
// DEFAULT-NEXT:         let %6 maxv: u32 [storage=automatic];
// DEFAULT-NEXT:         if logical_or<bool>(eq<u32>(read<u32>(%3), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), eq<u32>(read<u32>(%4), neg<u32, overflow=wrap>(const<u32>(2))))
// DEFAULT-NEXT:             return;
// DEFAULT-NEXT:         write<u32>(%5, conditional<u32>(lt<u32>(read<u32>(%3), read<u32>(%4)), read<u32>(%3), read<u32>(%4)));
// DEFAULT-NEXT:         write<u32>(%6, conditional<u32>(gt<u32>(read<u32>(%3), read<u32>(%4)), read<u32>(%3), read<u32>(%4)));
// DEFAULT-NEXT:         if eq<u32>(read<u32>(%5), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))
// DEFAULT-NEXT:             let %8: i32 [synthetic] = read<i32>(%1);
// DEFAULT-NEXT:             let %9: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%8), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%1, read<i32>(%9));
// DEFAULT-NEXT:         if eq<u32>(read<u32>(%6), neg<u32, overflow=wrap>(const<u32>(2)))
// DEFAULT-NEXT:             let %10: i32 [synthetic] = read<i32>(%1);
// DEFAULT-NEXT:             let %11: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%10), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%1, read<i32>(%11));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(u32, u32) -> void>(%2, neg<u32, overflow=wrap>(const<u32>(2)), reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
