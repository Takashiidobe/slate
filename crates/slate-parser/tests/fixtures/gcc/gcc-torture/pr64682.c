/* PR rtl-optimization/64682 */

int a, b = 1;

__attribute__((noinline, noclone)) void foo(int x) {
  if (x != 5)
    __builtin_abort();
}

int main() {
  int i;
  for (i = 0; i < 56; i++)
    for (; a; a--)
      ;
  int *c = &b;
  if (*c)
    *c = 1 % (unsigned int)*c | 5;

  foo(b);

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
// DEFAULT-NEXT:     global %0 a: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 b: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     fn %2 @foo(%3 x: i32) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%3), const<i32>(5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %5 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %7
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%5, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%5), const<i32>(56))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %9: i32 [synthetic] = read<i32>(%5);
// DEFAULT-NEXT:                 let %10: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%9), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%5, read<i32>(%10));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 for %8
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                     condition: ne<i32>(read<i32>(%0), const<i32>(0))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %11: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                         let %12: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%11), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%0, read<i32>(%12));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         ;
// DEFAULT-NEXT:         let %6 c: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(%1);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(deref(read<ptr<i32>>(%6))), const<i32>(0))
// DEFAULT-NEXT:             write<i32>(deref(read<ptr<i32>>(%6)), reinterpret<i32, reason=assign, fits=unknown>(or<u32>(rem<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)), reinterpret<u32, reason=explicit, fits=unknown>(read<i32>(deref(read<ptr<i32>>(%6))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(5)))));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%2, read<i32>(%1));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
