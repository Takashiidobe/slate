#include <stdio.h>

int main(void) {
  float _Complex f       = __builtin_complex(1.0f, 2.0f);
  double _Complex d      = __builtin_complex(3.0, 4.0);
  long double _Complex l = __builtin_complex(5.0L, 6.0L);

  printf("%d %d\n", f == __builtin_complex(1.0f, 2.0f),
         f != __builtin_complex(1.0f, 3.0f));
  printf("%d %d\n", d == __builtin_complex(3.0, 4.0),
         d != __builtin_complex(3.0, 5.0));
  printf("%d %d\n", l == __builtin_complex(5.0L, 6.0L),
         l != __builtin_complex(5.0L, 7.0L));
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
// DEFAULT-NEXT:     global %7 .str7: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %8 .str8: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %9 .str9: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%6 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %3 f: complex<f32> [storage=automatic] = aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(1.0), index1 = const<f32>(2.0));
// DEFAULT-NEXT:         let %4 d: complex<f64> [storage=automatic] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(3.0), index1 = const<f64>(4.0));
// DEFAULT-NEXT:         let %5 l: complex<f80> [storage=automatic] = aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(5), index1 = const<f80>(6));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%7)), from_bool<i32, reason=vararg>(eq<complex<f32>, exceptions=ignore>(read<complex<f32>>(%3), aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(1.0), index1 = const<f32>(2.0)))), from_bool<i32, reason=vararg>(ne<complex<f32>, exceptions=ignore>(read<complex<f32>>(%3), aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(1.0), index1 = const<f32>(3.0)))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%8)), from_bool<i32, reason=vararg>(eq<complex<f64>, exceptions=ignore>(read<complex<f64>>(%4), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(3.0), index1 = const<f64>(4.0)))), from_bool<i32, reason=vararg>(ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(%4), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(3.0), index1 = const<f64>(5.0)))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%9)), from_bool<i32, reason=vararg>(eq<complex<f80>, exceptions=ignore>(read<complex<f80>>(%5), aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(5), index1 = const<f80>(6)))), from_bool<i32, reason=vararg>(ne<complex<f80>, exceptions=ignore>(read<complex<f80>>(%5), aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(5), index1 = const<f80>(7)))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
