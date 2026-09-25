#include <stdio.h>

static int sum(int *items, int len) {
  int total = 0;
  for (int i = 0; i < len; i++) {
    total += items[i];
  }
  return total;
}

static void bump(int *items, int len) {
  for (int i = 0; i < len; i++) {
    items[i] += 1;
  }
}

int main(void) {
  int values[4] = {2, 4, 6, 8};
  printf("%d\n", sum(values, 4));
  bump(values, 4);
  printf("%d %d\n", values[0], values[3]);
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
// DEFAULT-NEXT:     global %15 .str15: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %16 .str16: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%12 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @sum(%2 items: ptr<i32>, %3 len: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %4 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %13
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %5 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%5), read<i32>(%3))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %17: i32 [synthetic] = read<i32>(%5);
// DEFAULT-NEXT:                 let %18: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%17), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%5, read<i32>(%18));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %19: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:                     let %20: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%19), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%2), read<i32>(%5)))));
// DEFAULT-NEXT:                     write<i32>(%4, read<i32>(%20));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<i32>(%4);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @bump(%7 items: ptr<i32>, %8 len: i32) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         for %14
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %9 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%9), read<i32>(%8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %21: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:                 let %22: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%21), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%9, read<i32>(%22));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %23: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%7), read<i32>(%9));
// DEFAULT-NEXT:                     let %24: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%23)));
// DEFAULT-NEXT:                     let %25: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%24), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(deref(read<ptr<i32>>(%23)), read<i32>(%25));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %11 values: array<i32, 4> [storage=automatic] = aggregate<array<i32, 4>, zero_fill=false>(index0 = const<i32>(2), index1 = const<i32>(4), index2 = const<i32>(6), index3 = const<i32>(8));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%15)), call<i32, signature=fn(ptr<i32>, i32) -> i32>(%1, array_decay<ptr<i32>, length=Some(4)>(%11), const<i32>(4)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>, i32) -> void>(%6, array_decay<ptr<i32>, length=Some(4)>(%11), const<i32>(4));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%16)), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%11), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%11), const<i32>(3)))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
