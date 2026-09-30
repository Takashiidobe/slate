#include <stdio.h>

unsigned add_with_carry(unsigned a, unsigned b, unsigned carry_in,
                        unsigned *carry_out) {
  return __builtin_addc(a, b, carry_in, carry_out);
}

unsigned sub_with_borrow(unsigned a, unsigned b, unsigned borrow_in,
                         unsigned *borrow_out) {
  return __builtin_subc(a, b, borrow_in, borrow_out);
}

int main(void) {
  unsigned carry, borrow;

  unsigned sum = add_with_carry(0xFFFFFFFFu, 1u, 0u, &carry);
  printf("%u %u\n", sum, carry);

  unsigned sum2 = add_with_carry(1u, 1u, 0u, &carry);
  printf("%u %u\n", sum2, carry);

  unsigned diff = sub_with_borrow(0u, 1u, 0u, &borrow);
  printf("%u %u\n", diff, borrow);

  unsigned diff2 = sub_with_borrow(5u, 3u, 0u, &borrow);
  printf("%u %u\n", diff2, borrow);

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
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 117, 32, 37, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 117, 32, 37, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 117, 32, 37, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 117, 32, 37, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_addc:[0-9]+]] @__builtin_addc(%[[VALUE0:[0-9]+]] <unnamed>: u32, %[[VALUE1:[0-9]+]] <unnamed>: u32, %[[VALUE2:[0-9]+]] <unnamed>: u32, %[[VALUE3:[0-9]+]] <unnamed>: ptr<u32>) -> u32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_add_with_carry:[0-9]+]] @add_with_carry(%[[VALUE_a:[0-9]+]] a: u32, %[[VALUE_b:[0-9]+]] b: u32, %[[VALUE_carry_in:[0-9]+]] carry_in: u32, %[[VALUE_carry_out:[0-9]+]] carry_out: ptr<u32>) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<u32, signature=fn(u32, u32, u32, ptr<u32>) -> u32>(%[[VALUE___builtin_addc]], read<u32>(%[[VALUE_a]]), read<u32>(%[[VALUE_b]]), read<u32>(%[[VALUE_carry_in]]), read<ptr<u32>>(%[[VALUE_carry_out]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_subc:[0-9]+]] @__builtin_subc(%[[VALUE4:[0-9]+]] <unnamed>: u32, %[[VALUE5:[0-9]+]] <unnamed>: u32, %[[VALUE6:[0-9]+]] <unnamed>: u32, %[[VALUE7:[0-9]+]] <unnamed>: ptr<u32>) -> u32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sub_with_borrow:[0-9]+]] @sub_with_borrow(%[[VALUE_a_2:[0-9]+]] a: u32, %[[VALUE_b_2:[0-9]+]] b: u32, %[[VALUE_borrow_in:[0-9]+]] borrow_in: u32, %[[VALUE_borrow_out:[0-9]+]] borrow_out: ptr<u32>) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<u32, signature=fn(u32, u32, u32, ptr<u32>) -> u32>(%[[VALUE___builtin_subc]], read<u32>(%[[VALUE_a_2]]), read<u32>(%[[VALUE_b_2]]), read<u32>(%[[VALUE_borrow_in]]), read<ptr<u32>>(%[[VALUE_borrow_out]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_carry:[0-9]+]] carry: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_borrow:[0-9]+]] borrow: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_sum:[0-9]+]] sum: u32 [storage=automatic] = call<u32, signature=fn(u32, u32, u32, ptr<u32>) -> u32>(%[[VALUE_add_with_carry]], const<u32>(4294967295), const<u32>(1), const<u32>(0), addr_of<ptr<u32>>(%[[VALUE_carry]]));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str]])), read<u32>(%[[VALUE_sum]]), read<u32>(%[[VALUE_carry]]));
// DEFAULT-NEXT:         let %[[VALUE_sum2:[0-9]+]] sum2: u32 [storage=automatic] = call<u32, signature=fn(u32, u32, u32, ptr<u32>) -> u32>(%[[VALUE_add_with_carry]], const<u32>(1), const<u32>(1), const<u32>(0), addr_of<ptr<u32>>(%[[VALUE_carry]]));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str_2]])), read<u32>(%[[VALUE_sum2]]), read<u32>(%[[VALUE_carry]]));
// DEFAULT-NEXT:         let %[[VALUE_diff:[0-9]+]] diff: u32 [storage=automatic] = call<u32, signature=fn(u32, u32, u32, ptr<u32>) -> u32>(%[[VALUE_sub_with_borrow]], const<u32>(0), const<u32>(1), const<u32>(0), addr_of<ptr<u32>>(%[[VALUE_borrow]]));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str_3]])), read<u32>(%[[VALUE_diff]]), read<u32>(%[[VALUE_borrow]]));
// DEFAULT-NEXT:         let %[[VALUE_diff2:[0-9]+]] diff2: u32 [storage=automatic] = call<u32, signature=fn(u32, u32, u32, ptr<u32>) -> u32>(%[[VALUE_sub_with_borrow]], const<u32>(5), const<u32>(3), const<u32>(0), addr_of<ptr<u32>>(%[[VALUE_borrow]]));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str_4]])), read<u32>(%[[VALUE_diff2]]), read<u32>(%[[VALUE_borrow]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
