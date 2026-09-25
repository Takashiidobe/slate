#include <stdio.h>
enum FuzzEnum {
  FuzzZero,
  FuzzOne,
  FuzzFive = 5,
  FuzzSix,
  FuzzNegative = -2,
  FuzzNegativeNext
};
union FuzzPair {
  int left;
  int right;
};
static int helper(int a, int b) {
  int c = a + b;
  return c;
}
static int loop_sum(int n) {
  int total = 0;
  for (int i = 1; i <= n; i++) {
    total += i;
  }
  return total;
}
static int array_pick(int index) {
  int values[3];
  values[0] = 6;
  values[1] = 1;
  values[2] = values[0] + values[1];
  return values[index];
}
static int union_pick(int a, int b) {
  union FuzzPair p;
  p.left  = a;
  p.right = b;
  return p.left;
}
int main(void) {
  printf("%d\n", helper(7, 2));
  printf("%d\n", loop_sum(2));
  printf("%d\n", array_pick(1));
  printf("%d\n", union_pick(8, 9));
  printf("%d\n", FuzzSix);
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
// DEFAULT-NEXT:     type @type0 FuzzEnum = enum : i32 {
// DEFAULT-NEXT:         %0 FuzzZero = const<i32>(0);
// DEFAULT-NEXT:         %1 FuzzOne = const<i32>(1);
// DEFAULT-NEXT:         %2 FuzzFive = const<i32>(5);
// DEFAULT-NEXT:         %3 FuzzSix = const<i32>(6);
// DEFAULT-NEXT:         %4 FuzzNegative = const<i32>(-2);
// DEFAULT-NEXT:         %5 FuzzNegativeNext = const<i32>(-1);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type1 FuzzPair = union {
// DEFAULT-NEXT:         field0 left: i32;
// DEFAULT-NEXT:         field1 right: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     global %27 .str27: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %28 .str28: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %29 .str29: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %30 .str30: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %31 .str31: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%25 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %9 @helper(%10 a: i32, %11 b: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %12 c: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%10), read<i32>(%11));
// DEFAULT-NEXT:         return read<i32>(%12);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @loop_sum(%14 n: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %15 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %26
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %16 i: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%16), read<i32>(%14))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %32: i32 [synthetic] = read<i32>(%16);
// DEFAULT-NEXT:                 let %33: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%32), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%16, read<i32>(%33));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %34: i32 [synthetic] = read<i32>(%15);
// DEFAULT-NEXT:                     let %35: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%34), read<i32>(%16));
// DEFAULT-NEXT:                     write<i32>(%15, read<i32>(%35));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<i32>(%15);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @array_pick(%18 index: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %19 values: array<i32, 3> [storage=automatic];
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(%19), const<i32>(0))), const<i32>(6));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(%19), const<i32>(1))), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(%19), const<i32>(2))), add<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(%19), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(%19), const<i32>(1))))));
// DEFAULT-NEXT:         return read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(%19), read<i32>(%18))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @union_pick(%21 a: i32, %22 b: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %23 p: @type1 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(field0(%23), read<i32>(%21));
// DEFAULT-NEXT:         write<i32>(field1(%23), read<i32>(%22));
// DEFAULT-NEXT:         return read<i32>(field0(%23));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %24 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%27)), call<i32, signature=fn(i32, i32) -> i32>(%9, const<i32>(7), const<i32>(2)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%28)), call<i32, signature=fn(i32) -> i32>(%13, const<i32>(2)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%29)), call<i32, signature=fn(i32) -> i32>(%17, const<i32>(1)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%30)), call<i32, signature=fn(i32, i32) -> i32>(%20, const<i32>(8), const<i32>(9)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%31)), const<i32>(6));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
