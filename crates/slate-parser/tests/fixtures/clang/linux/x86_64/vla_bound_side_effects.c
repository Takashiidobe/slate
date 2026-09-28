#include <stdio.h>

static int parameter_bound(int length, int values[length++]) {
  return length + values[0];
}

int main(void) {
  int           bound       = 3;
  unsigned long evaluated   = sizeof(int[bound++]);
  unsigned long unevaluated = sizeof(int (*)[bound++]);
  int           values[]    = {7, 8, 9};
  int           parameter   = parameter_bound(3, values);
  printf("%lu %lu %d %d\n", evaluated, unevaluated, bound, parameter);
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
// DEFAULT-NEXT:     global %15 .str15: array<i8, 15> [storage=static] = code_units<array<i8, 15>>([37, 108, 117, 32, 37, 108, 117, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%11 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @parameter_bound(%3 length: i32, %4 values: ptr<i32> [array=%12]) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %16: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:         let %17: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%16), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%3, read<i32>(%17));
// DEFAULT-NEXT:         let %12: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%16)));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%3), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%4), const<i32>(0)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %6 bound: i32 [storage=automatic] = const<i32>(3);
// DEFAULT-NEXT:         let %7 evaluated: u64 [storage=automatic];
// DEFAULT-NEXT:         let %18: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:         let %19: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%18), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%6, read<i32>(%19));
// DEFAULT-NEXT:         let %13: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%18)));
// DEFAULT-NEXT:         write<u64>(%7, mul<u64, overflow=wrap>(read<u64>(%13), const<u64>(4)));
// DEFAULT-NEXT:         let %8 unevaluated: u64 [storage=automatic] = const<u64>(8);
// DEFAULT-NEXT:         let %9 values: array<i32, 3> [storage=automatic] = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(7), index1 = const<i32>(8), index2 = const<i32>(9));
// DEFAULT-NEXT:         let %10 parameter: i32 [storage=automatic] = call<i32, signature=fn(i32, ptr<i32>) -> i32>(%2, const<i32>(3), array_decay<ptr<i32>, length=Some(3)>(%9));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(15)>(%15)), read<u64>(%7), read<u64>(%8), read<i32>(%6), read<i32>(%10));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
