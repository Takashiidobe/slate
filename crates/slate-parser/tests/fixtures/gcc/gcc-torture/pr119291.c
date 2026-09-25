/* PR rtl-optimization/119291 */

int  a;
long c;

__attribute__((noipa)) void foo(int x) {
  if (x != 0)
    __builtin_abort();
  a = 42;
}

int main() {
  int e = 1;
lab:
  if (a < 2) {
    int           b = e;
    _Bool         d = a != 0;
    _Bool         f = b != 0;
    unsigned long g = -(d & f);
    unsigned long h = c & g;
    unsigned long i = ~c;
    e               = -(i & h);
    c               = e != 0;
    a               = ~e + b;
    foo(e);
    goto lab;
  }
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
// DEFAULT-NEXT:     global %1 c: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %2 @foo(%3 x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%3), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         write<i32>(%0, const<i32>(42));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %6 e: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         label %5 lab:
// DEFAULT-NEXT:             if lt<i32>(read<i32>(%0), const<i32>(2))
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %7 b: i32 [storage=automatic] = read<i32>(%6);
// DEFAULT-NEXT:                     let %8 d: bool [storage=automatic] = ne<i32>(read<i32>(%0), const<i32>(0));
// DEFAULT-NEXT:                     let %9 f: bool [storage=automatic] = ne<i32>(read<i32>(%7), const<i32>(0));
// DEFAULT-NEXT:                     let %10 g: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(neg<i32, overflow=ub>(and<i32>(from_bool<i32, reason=promotion>(read<bool>(%8)), from_bool<i32, reason=promotion>(read<bool>(%9))))));
// DEFAULT-NEXT:                     let %11 h: u64 [storage=automatic] = and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%1)), read<u64>(%10));
// DEFAULT-NEXT:                     let %12 i: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(not<i64>(read<i64>(%1)));
// DEFAULT-NEXT:                     write<i32>(%6, reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(neg<u64, overflow=wrap>(and<u64>(read<u64>(%12), read<u64>(%11))))));
// DEFAULT-NEXT:                     write<i64>(%1, from_bool<i64, reason=assign>(ne<i32>(read<i32>(%6), const<i32>(0))));
// DEFAULT-NEXT:                     write<i32>(%0, add<i32, overflow=ub>(not<i32>(read<i32>(%6)), read<i32>(%7)));
// DEFAULT-NEXT:                     call<void, signature=fn(i32) -> void>(%2, read<i32>(%6));
// DEFAULT-NEXT:                     goto %5;
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
