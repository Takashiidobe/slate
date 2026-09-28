/* PR tree-optimization/46909 */

extern void abort();

int __attribute__((__noinline__)) foo(unsigned int x) {
  if (!(x == 4 || x == 6) || (x == 2 || x == 6))
    return 1;
  return -1;
}

int main() {
  int i;
  for (i = -10; i < 10; i++)
    if (foo(i) != 1 - 2 * (i == 4))
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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @foo(%2 x: u32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_or<bool>(not<bool>(logical_or<bool>(eq<u32>(read<u32>(%2), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(4))), eq<u32>(read<u32>(%2), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(6))))), logical_or<bool>(eq<u32>(read<u32>(%2), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))), eq<u32>(read<u32>(%2), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(6)))))
// DEFAULT-NEXT:             return const<i32>(1);
// DEFAULT-NEXT:         return neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %4 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %5
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%4, neg<i32, overflow=ub>(const<i32>(10)));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%4), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %6: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:                 let %7: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%6), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%4, read<i32>(%7));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(u32) -> i32>(%1, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%4))), sub<i32, overflow=ub>(const<i32>(1), mul<i32, overflow=ub>(const<i32>(2), from_bool<i32, reason=promotion>(eq<i32>(read<i32>(%4), const<i32>(4))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
