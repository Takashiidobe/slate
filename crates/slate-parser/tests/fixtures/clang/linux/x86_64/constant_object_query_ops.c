#include <stdio.h>

static int global_array[4];

int main(void) {
  int          local[6];
  int         *p = local;
  volatile int v = 3;

  int c_literal  = __builtin_constant_p(42);
  int c_expr     = __builtin_constant_p(7 + 5);
  int c_volatile = __builtin_constant_p(v);

  unsigned long local_whole     = __builtin_object_size(local, 0);
  unsigned long local_remaining = __builtin_object_size(&local[2], 1);
  unsigned long global_whole    = __builtin_object_size(global_array, 0);
  unsigned long unknown         = __builtin_object_size(p, 0);
  unsigned long unknown_upper   = __builtin_object_size(p, 2);

  printf("%d %d %d %lu %lu %lu %lu %lu\n", c_literal, c_expr, c_volatile,
         local_whole, local_remaining, global_whole, unknown, unknown_upper);
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
// DEFAULT-NEXT:     global %[[VALUE_global_array:[0-9]+]] global_array: array<i32, 4> [storage=static] [align=16] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 30> [storage=static] = code_units<array<i8, 30>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 108, 117, 32, 37, 108, 117, 32, 37, 108, 117, 32, 37, 108, 117, 32, 37, 108, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_object_size:[0-9]+]] @__builtin_object_size(%[[VALUE0:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE1:[0-9]+]] <unnamed>: i32) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_local:[0-9]+]] local: array<i32, 6> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<i32> [storage=automatic] = array_decay<ptr<i32>, length=Some(6)>(%[[VALUE_local]]);
// DEFAULT-NEXT:         let %[[VALUE_v:[0-9]+]] v: volatile i32 [storage=automatic] = const<i32>(3);
// DEFAULT-NEXT:         let %[[VALUE_c_literal:[0-9]+]] c_literal: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         let %[[VALUE_c_expr:[0-9]+]] c_expr: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         let %[[VALUE_c_volatile:[0-9]+]] c_volatile: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_local_whole:[0-9]+]] local_whole: u64 [storage=automatic] = call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(6)>(%[[VALUE_local]])), const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_local_remaining:[0-9]+]] local_remaining: u64 [storage=automatic] = call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(6)>(%[[VALUE_local]]), const<i32>(2))))), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE_global_whole:[0-9]+]] global_whole: u64 [storage=automatic] = call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(4)>(%[[VALUE_global_array]])), const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_unknown:[0-9]+]] unknown: u64 [storage=automatic] = call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<i32>>(%[[VALUE_p]])), const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_unknown_upper:[0-9]+]] unknown_upper: u64 [storage=automatic] = call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<i32>>(%[[VALUE_p]])), const<i32>(2));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(30)>(%[[VALUE_str]])), read<i32>(%[[VALUE_c_literal]]), read<i32>(%[[VALUE_c_expr]]), read<i32>(%[[VALUE_c_volatile]]), read<u64>(%[[VALUE_local_whole]]), read<u64>(%[[VALUE_local_remaining]]), read<u64>(%[[VALUE_global_whole]]), read<u64>(%[[VALUE_unknown]]), read<u64>(%[[VALUE_unknown_upper]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
