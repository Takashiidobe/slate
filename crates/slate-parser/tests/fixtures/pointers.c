#include <stdio.h>

static int bump_through_pointer(int value) {
  int  local = value;
  int *ptr   = &local;
  *ptr       = *ptr + 3;
  return local;
}

static int add_into_pointer(int *slot, int amount) {
  *slot = *slot + amount;
  return *slot;
}

static int pick_with_pointer_arithmetic(int index) {
  int  values[4];
  int *ptr  = values;
  values[0] = 4;
  values[1] = 8;
  values[2] = 12;
  values[3] = 16;
  return *(ptr + index);
}

int main(void) {
  int total = 10;
  printf("%d\n", bump_through_pointer(5));
  printf("%d\n", add_into_pointer(&total, 7));
  printf("%d\n", total);
  printf("%d\n", pick_with_pointer_arithmetic(2));
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
// DEFAULT-NEXT:     global %16 .str16: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %17 .str17: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %18 .str18: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%14 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @bump_through_pointer(%2 value: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 local: i32 [storage=automatic] = read<i32>(%2);
// DEFAULT-NEXT:         let %4 ptr: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(%3);
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%4)), add<i32, overflow=ub>(read<i32>(deref(read<ptr<i32>>(%4))), const<i32>(3)));
// DEFAULT-NEXT:         return read<i32>(%3);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @add_into_pointer(%6 slot: ptr<i32>, %7 amount: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%6)), add<i32, overflow=ub>(read<i32>(deref(read<ptr<i32>>(%6))), read<i32>(%7)));
// DEFAULT-NEXT:         return read<i32>(deref(read<ptr<i32>>(%6)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @pick_with_pointer_arithmetic(%9 index: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %10 values: array<i32, 4> [storage=automatic];
// DEFAULT-NEXT:         let %11 ptr: ptr<i32> [storage=automatic] = array_decay<ptr<i32>, length=Some(4)>(%10);
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%10), const<i32>(0))), const<i32>(4));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%10), const<i32>(1))), const<i32>(8));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%10), const<i32>(2))), const<i32>(12));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%10), const<i32>(3))), const<i32>(16));
// DEFAULT-NEXT:         return read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%11), read<i32>(%9))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %13 total: i32 [storage=automatic] = const<i32>(10);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%15)), call<i32, signature=fn(i32) -> i32>(%1, const<i32>(5)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%16)), call<i32, signature=fn(ptr<i32>, i32) -> i32>(%5, addr_of<ptr<i32>>(%13), const<i32>(7)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%17)), read<i32>(%13));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%18)), call<i32, signature=fn(i32) -> i32>(%8, const<i32>(2)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
