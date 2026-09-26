#include <stdio.h>

int main(void) {
  int a  = 20;
  a     -= 5;
  printf("%d\n", a);
  a *= 3;
  printf("%d\n", a);
  a /= 5;
  printf("%d\n", a);
  a %= 7;
  printf("%d\n", a);
  a <<= 3;
  printf("%d\n", a);
  a >>= 2;
  printf("%d\n", a);
  a &= 6;
  printf("%d\n", a);
  a ^= 3;
  printf("%d\n", a);
  a |= 8;
  printf("%d\n", a);
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
// DEFAULT-NEXT:     global %4 .str4: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %5 .str5: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %6 .str6: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %7 .str7: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %8 .str8: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %9 .str9: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %10 .str10: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %11 .str11: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %12 .str12: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%3 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %2 a: i32 [storage=automatic] = const<i32>(20);
// DEFAULT-NEXT:         let %13: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %14: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%13), const<i32>(5));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%14));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%4)), read<i32>(%2));
// DEFAULT-NEXT:         let %15: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %16: i32 [synthetic] = mul<i32, overflow=ub>(read<i32>(%15), const<i32>(3));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%16));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%5)), read<i32>(%2));
// DEFAULT-NEXT:         let %17: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %18: i32 [synthetic] = div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%17), const<i32>(5));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%18));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%6)), read<i32>(%2));
// DEFAULT-NEXT:         let %19: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %20: i32 [synthetic] = rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%19), const<i32>(7));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%20));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%7)), read<i32>(%2));
// DEFAULT-NEXT:         let %21: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %22: i32 [synthetic] = shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i32>(%21), const<i32>(3));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%22));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%8)), read<i32>(%2));
// DEFAULT-NEXT:         let %23: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %24: i32 [synthetic] = shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%23), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%24));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%9)), read<i32>(%2));
// DEFAULT-NEXT:         let %25: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %26: i32 [synthetic] = and<i32>(read<i32>(%25), const<i32>(6));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%26));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%10)), read<i32>(%2));
// DEFAULT-NEXT:         let %27: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %28: i32 [synthetic] = xor<i32>(read<i32>(%27), const<i32>(3));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%28));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%11)), read<i32>(%2));
// DEFAULT-NEXT:         let %29: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %30: i32 [synthetic] = or<i32>(read<i32>(%29), const<i32>(8));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%30));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%12)), read<i32>(%2));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
