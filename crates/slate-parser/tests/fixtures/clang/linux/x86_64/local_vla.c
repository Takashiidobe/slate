#include <stdio.h>

static int sum_vla(int length, int (*values)[length]) {
  int total = 0;
  for (int index = 0; index < length; ++index) {
    total += (*values)[index];
  }
  return total;
}

int main(void) {
  int result;
  {
    int length = 4;
    int values[length];
    for (int index = 0; index < length; ++index) {
      values[index] = index + 3;
    }
    result = sum_vla(length, &values);
  }
  printf("%d\n", result + 1);
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
// DEFAULT-NEXT:     global %17 .str17: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%12 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @sum_vla(%3 length: i32, %4 values: ptr<vla<i32, %13>>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %13: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%3)));
// DEFAULT-NEXT:         let %5 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %14
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %6 index: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%6), read<i32>(%3))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %18: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:                 let %19: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%18), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%6, read<i32>(%19));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %20: i32 [synthetic] = read<i32>(%5);
// DEFAULT-NEXT:                     let %21: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%20), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(deref(read<ptr<vla<i32, %13>>>(%4))), read<i32>(%6)))));
// DEFAULT-NEXT:                     write<i32>(%5, read<i32>(%21));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<i32>(%5);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %8 result: i32 [storage=automatic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %9 length: i32 [storage=automatic] = const<i32>(4);
// DEFAULT-NEXT:             let %15: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%9)));
// DEFAULT-NEXT:             let %10 values: vla<i32, %15> [storage=automatic];
// DEFAULT-NEXT:             for %16
// DEFAULT-NEXT:                 init:
// DEFAULT-NEXT:                     let %11 index: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                 condition: lt<i32>(read<i32>(%11), read<i32>(%9))
// DEFAULT-NEXT:                 increment: {
// DEFAULT-NEXT:                     let %22: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:                     let %23: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%22), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%11, read<i32>(%23));
// DEFAULT-NEXT:                     yield void;
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:                 body:
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(%10), read<i32>(%11))), add<i32, overflow=ub>(read<i32>(%11), const<i32>(3)));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:             write<i32>(%8, call<i32, signature=fn(i32, ptr<vla<i32, *>>) -> i32>(%2, read<i32>(%9), pointer_cast<ptr<vla<i32, *>>, reason=arg>(addr_of<ptr<vla<i32, %15>>>(%10))));
// DEFAULT-NEXT:             call<i32, signature=fn(i32, ptr<vla<i32, *>>) -> i32>(%2, read<i32>(%9), pointer_cast<ptr<vla<i32, *>>, reason=arg>(addr_of<ptr<vla<i32, %15>>>(%10)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%17)), add<i32, overflow=ub>(read<i32>(%8), const<i32>(1)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
