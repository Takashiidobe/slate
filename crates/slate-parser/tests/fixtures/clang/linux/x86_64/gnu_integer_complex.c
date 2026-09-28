#include <stdio.h>

static __complex__ int global_value = 17 + 19i;

int main(void) {
  __complex__ int first     = 5 + 7i;
  __complex__ int second    = -3 + 11i;
  __complex__ int imaginary = 13i;
  printf("%d %d %d %d %d %d %d %d\n", __real__ first, __imag__ first,
         __real__ second, __imag__ second, __real__ imaginary,
         __imag__ imaginary, __real__ global_value, __imag__ global_value);
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
// DEFAULT-NEXT:     global %2 global_value: complex<i32> [storage=static] = add<complex<i32>, complex=true, overflow=ub>(const<i32>(17), aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(19))) [linkage=internal];
// DEFAULT-NEXT:     global %8 .str8: array<i8, 25> [storage=static] = code_units<array<i8, 25>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%7 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %4 first: complex<i32> [storage=automatic] = add<complex<i32>, complex=true, overflow=ub>(const<i32>(5), aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(7)));
// DEFAULT-NEXT:         let %5 second: complex<i32> [storage=automatic] = add<complex<i32>, complex=true, overflow=ub>(neg<i32, overflow=ub>(const<i32>(3)), aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(11)));
// DEFAULT-NEXT:         let %6 imaginary: complex<i32> [storage=automatic] = aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(13));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(25)>(%8)), read<i32>(real(%4)), read<i32>(imag(%4)), read<i32>(real(%5)), read<i32>(imag(%5)), read<i32>(real(%6)), read<i32>(imag(%6)), read<i32>(real(%2)), read<i32>(imag(%2)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
