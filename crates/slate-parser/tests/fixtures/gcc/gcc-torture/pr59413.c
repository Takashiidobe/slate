/* PR tree-optimization/59413 */

typedef unsigned int uint32_t;

uint32_t a;
int      b;

int main() {
  uint32_t c;
  for (a = 7; a <= 1; a++) {
    char d = a;
    c      = d;
    b      = a == c;
  }
  if (a != 7)
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
// DEFAULT-NEXT:     type @type0 uint32_t = u32;
// DEFAULT-NEXT:     global %1 a: u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %4 c: u32 [storage=automatic];
// DEFAULT-NEXT:         for %6
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u32>(%1, reinterpret<u32, reason=assign, fits=always>(const<i32>(7)));
// DEFAULT-NEXT:             condition: le<u32>(read<u32>(%1), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %7: u32 [synthetic] = read<u32>(%1);
// DEFAULT-NEXT:                 let %8: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%7), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<u32>(%1, read<u32>(%8));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %5 d: i8 [storage=automatic] = reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(read<u32>(%1)));
// DEFAULT-NEXT:                     write<u32>(%4, reinterpret<u32, reason=assign, fits=unknown>(widen<i32, reason=assign>(read<i8>(%5))));
// DEFAULT-NEXT:                     write<i32>(%2, from_bool<i32, reason=assign>(eq<u32>(read<u32>(%1), read<u32>(%4))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%1), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(7)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
