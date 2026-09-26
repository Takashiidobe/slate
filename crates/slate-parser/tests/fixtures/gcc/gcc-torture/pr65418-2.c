/* PR tree-optimization/65418 */

__attribute__((noinline, noclone)) int foo(int x) {
  if (x == -216 || x == -211 || x == -218 || x == -205 || x == -223)
    return 1;
  return 0;
}

int main() {
  volatile int i;
  for (i = -230; i < -200; i++)
    if (foo(i) !=
        (i == -216 || i == -211 || i == -218 || i == -205 || i == -223))
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
// DEFAULT-NEXT:     fn %0 @foo(%1 x: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(eq<i32>(read<i32>(%1), neg<i32, overflow=ub>(const<i32>(216))), eq<i32>(read<i32>(%1), neg<i32, overflow=ub>(const<i32>(211)))), eq<i32>(read<i32>(%1), neg<i32, overflow=ub>(const<i32>(218)))), eq<i32>(read<i32>(%1), neg<i32, overflow=ub>(const<i32>(205)))), eq<i32>(read<i32>(%1), neg<i32, overflow=ub>(const<i32>(223))))
// DEFAULT-NEXT:             return const<i32>(1);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %3 i: volatile i32 [storage=automatic];
// DEFAULT-NEXT:         for %4
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32, volatile>(%3, neg<i32, overflow=ub>(const<i32>(230)));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32, volatile>(%3), neg<i32, overflow=ub>(const<i32>(200)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %6: i32 [synthetic] = read<i32, volatile>(%3);
// DEFAULT-NEXT:                 let %7: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%6), const<i32>(1));
// DEFAULT-NEXT:                 write<i32, volatile>(%3, read<i32>(%7));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(i32) -> i32>(%0, read<i32, volatile>(%3)), from_bool<i32, reason=promotion>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(eq<i32>(read<i32, volatile>(%3), neg<i32, overflow=ub>(const<i32>(216))), eq<i32>(read<i32, volatile>(%3), neg<i32, overflow=ub>(const<i32>(211)))), eq<i32>(read<i32, volatile>(%3), neg<i32, overflow=ub>(const<i32>(218)))), eq<i32>(read<i32, volatile>(%3), neg<i32, overflow=ub>(const<i32>(205)))), eq<i32>(read<i32, volatile>(%3), neg<i32, overflow=ub>(const<i32>(223))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
