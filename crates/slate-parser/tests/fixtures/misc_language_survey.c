#include <stdio.h>

void scale(int n, int arr[*]);

void scale(int n, int arr[n]) {
  for (int i = 0; i < n; ++i) {
    arr[i] *= 2;
  }
}

int add(int, int);
int add(int a, int b) { return a + b; }

int main(void) {
  int values[3] = {1, 2, 3};
  scale(3, values);

  double separated      = 1'000.5;
  char   escape_e       = '\e';
  char   escape_unknown = '\%';

  printf("%d %d %d\n", values[0], values[1], values[2]);
  printf("%f\n", separated);
  printf("%d %d\n", (int)escape_e, (int)escape_unknown);
  printf("%zu\n", sizeof(add));
  printf("%d\n", add(3, 4));
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
// DEFAULT-NEXT:     global %20 .str20: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %21 .str21: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 102, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %22 .str22: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %23 .str23: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %24 .str24: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%13 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @scale(%2 n: i32, %3 arr: ptr<i32> [array=%16]) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %16: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%2)));
// DEFAULT-NEXT:         for %17
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %4 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%4), read<i32>(%2))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %25: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:                 let %26: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%25), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%4, read<i32>(%26));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %27: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%3), read<i32>(%4));
// DEFAULT-NEXT:                     let %28: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%27)));
// DEFAULT-NEXT:                     let %29: i32 [synthetic] = mul<i32, overflow=ub>(read<i32>(%28), const<i32>(2));
// DEFAULT-NEXT:                     write<i32>(deref(read<ptr<i32>>(%27)), read<i32>(%29));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @add(%6 a: i32, %7 b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%6), read<i32>(%7));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %9 values: array<i32, 3> [storage=automatic] = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2), index2 = const<i32>(3));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i32>) -> void>(%1, const<i32>(3), array_decay<ptr<i32>, length=Some(3)>(%9));
// DEFAULT-NEXT:         let %10 separated: f64 [storage=automatic] = const<f64>(1000.5);
// DEFAULT-NEXT:         let %11 escape_e: i8 [storage=automatic] = truncate<i8, reason=assign, fits=always>(const<i32>(27));
// DEFAULT-NEXT:         let %12 escape_unknown: i8 [storage=automatic] = truncate<i8, reason=assign, fits=always>(const<i32>(37));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%20)), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(%9), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(%9), const<i32>(1)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(%9), const<i32>(2)))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%21)), read<f64>(%10));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%22)), widen<i32, reason=explicit>(read<i8>(%11)), widen<i32, reason=explicit>(read<i8>(%12)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%23)), const<u64>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%24)), call<i32, signature=fn(i32, i32) -> i32>(%5, const<i32>(3), const<i32>(4)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
