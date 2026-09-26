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
// DEFAULT-NEXT:     global %19 .str19: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 117, 32, 37, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %20 .str20: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 117, 32, 37, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %21 .str21: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 117, 32, 37, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %22 .str22: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 117, 32, 37, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%18 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @add_with_carry(%2 a: u32, %3 b: u32, %4 carry_in: u32, %5 carry_out: ptr<u32>) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<u32, signature=fn(u32, u32, u32, ptr<u32>) -> u32>(__builtin_addc, read<u32>(%2), read<u32>(%3), read<u32>(%4), read<ptr<u32>>(%5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @sub_with_borrow(%7 a: u32, %8 b: u32, %9 borrow_in: u32, %10 borrow_out: ptr<u32>) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<u32, signature=fn(u32, u32, u32, ptr<u32>) -> u32>(__builtin_subc, read<u32>(%7), read<u32>(%8), read<u32>(%9), read<ptr<u32>>(%10));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %12 carry: u32 [storage=automatic];
// DEFAULT-NEXT:         let %13 borrow: u32 [storage=automatic];
// DEFAULT-NEXT:         let %14 sum: u32 [storage=automatic] = call<u32, signature=fn(u32, u32, u32, ptr<u32>) -> u32>(%1, const<u32>(4294967295), const<u32>(1), const<u32>(0), addr_of<ptr<u32>>(%12));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%19)), read<u32>(%14), read<u32>(%12));
// DEFAULT-NEXT:         let %15 sum2: u32 [storage=automatic] = call<u32, signature=fn(u32, u32, u32, ptr<u32>) -> u32>(%1, const<u32>(1), const<u32>(1), const<u32>(0), addr_of<ptr<u32>>(%12));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%20)), read<u32>(%15), read<u32>(%12));
// DEFAULT-NEXT:         let %16 diff: u32 [storage=automatic] = call<u32, signature=fn(u32, u32, u32, ptr<u32>) -> u32>(%6, const<u32>(0), const<u32>(1), const<u32>(0), addr_of<ptr<u32>>(%13));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%21)), read<u32>(%16), read<u32>(%13));
// DEFAULT-NEXT:         let %17 diff2: u32 [storage=automatic] = call<u32, signature=fn(u32, u32, u32, ptr<u32>) -> u32>(%6, const<u32>(5), const<u32>(3), const<u32>(0), addr_of<ptr<u32>>(%13));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%22)), read<u32>(%17), read<u32>(%13));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
