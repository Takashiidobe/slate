/* PR rtl-optimization/64756 */

int                   a, *tmp, **c = &tmp;
volatile int          d;
static int *volatile *e = &tmp;
unsigned int          f;

static void fn1(int *p) {
  int g;
  for (; f < 1; f++)
    for (g = 1; g >= 0; g--) {
      d || d;
      *c = p;

      if (tmp != &a)
        __builtin_abort();

      *e = 0;
    }
}

int main() {
  fn1(&a);
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
// DEFAULT-NEXT:     global %1 tmp: ptr<i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 c: ptr<ptr<i32>> [storage=static] = addr_of<ptr<ptr<i32>>>(%1) [linkage=external];
// DEFAULT-NEXT:     global %3 d: volatile i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 e: ptr<volatile ptr<i32>> [storage=static] = pointer_cast<ptr<volatile ptr<i32>>, reason=assign>(addr_of<ptr<ptr<i32>>>(%1)) [linkage=internal];
// DEFAULT-NEXT:     global %5 f: u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %6 @fn1(%7 p: ptr<i32>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %8 g: i32 [storage=automatic];
// DEFAULT-NEXT:         for %10
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: lt<u32>(read<u32>(%5), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %12: u32 [synthetic] = read<u32>(%5);
// DEFAULT-NEXT:                 let %13: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%12), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<u32>(%5, read<u32>(%13));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 for %11
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%8, const<i32>(1));
// DEFAULT-NEXT:                     condition: ge<i32>(read<i32>(%8), const<i32>(0))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %14: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:                         let %15: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%14), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%8, read<i32>(%15));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             logical_or<bool>(ne<i32>(read<i32, volatile>(%3), const<i32>(0)), ne<i32>(read<i32, volatile>(%3), const<i32>(0)));
// DEFAULT-NEXT:                             write<ptr<i32>>(deref(read<ptr<ptr<i32>>>(%2)), read<ptr<i32>>(%7));
// DEFAULT-NEXT:                             if ne<ptr<i32>>(read<ptr<i32>>(%1), addr_of<ptr<i32>>(%0))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                             write<ptr<i32>, volatile>(deref(read<ptr<volatile ptr<i32>>>(%4)), null<ptr<i32>>);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%6, addr_of<ptr<i32>>(%0));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
