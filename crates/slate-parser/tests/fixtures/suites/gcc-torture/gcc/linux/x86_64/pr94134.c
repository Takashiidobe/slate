/* PR target/94134 */

static volatile int a = 0;
static volatile int b = 1;

int main() {
  a++;
  b++;
  if (a != 1 || b != 2)
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
// DEFAULT-NEXT:     global %0 a: volatile i32 [storage=static] = const<i32>(0) [linkage=internal];
// DEFAULT-NEXT:     global %1 b: volatile i32 [storage=static] = const<i32>(1) [linkage=internal];
// DEFAULT-NEXT:     fn %3 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %4: i32 [synthetic] = read<i32, volatile>(%0);
// DEFAULT-NEXT:         let %5: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%4), const<i32>(1));
// DEFAULT-NEXT:         write<i32, volatile>(%0, read<i32>(%5));
// DEFAULT-NEXT:         let %6: i32 [synthetic] = read<i32, volatile>(%1);
// DEFAULT-NEXT:         let %7: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%6), const<i32>(1));
// DEFAULT-NEXT:         write<i32, volatile>(%1, read<i32>(%7));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32, volatile>(%0), const<i32>(1)), ne<i32>(read<i32, volatile>(%1), const<i32>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
