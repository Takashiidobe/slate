#include <stdio.h>

int main(void) {
  int          ri  = 0;
  unsigned int ru  = 0;
  long long    rll = 0;

  int add_i = __builtin_add_overflow(2147483647, 1, &ri);
  printf("%d %d\n", add_i, ri);

  int sub_i = __builtin_sub_overflow((-2147483647 - 1), 1, &ri);
  printf("%d %d\n", sub_i, ri);

  int mul_i = __builtin_mul_overflow(1073741824, 2, &ri);
  printf("%d %d\n", mul_i, ri);

  int add_u = __builtin_add_overflow(4294967295u, 1u, &ru);
  printf("%d %u\n", add_u, ru);

  int mul_ll = __builtin_mul_overflow(3037000500LL, 3037000500LL, &rll);
  printf("%d %lld\n", mul_ll, rll);

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
// DEFAULT-NEXT:     global %11 .str11: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %12 .str12: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %13 .str13: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %14 .str14: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 100, 32, 37, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %15 .str15: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([37, 100, 32, 37, 108, 108, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%10 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %2 ri: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %3 ru: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:         let %4 rll: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:         let %5 add_i: i32 [storage=automatic] = from_bool<i32, reason=assign>(overflow_add<bool>(const<i32>(2147483647), const<i32>(1), deref(addr_of<ptr<i32>>(%2))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%11)), read<i32>(%5), read<i32>(%2));
// DEFAULT-NEXT:         let %6 sub_i: i32 [storage=automatic] = from_bool<i32, reason=assign>(overflow_sub<bool>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(1), deref(addr_of<ptr<i32>>(%2))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%12)), read<i32>(%6), read<i32>(%2));
// DEFAULT-NEXT:         let %7 mul_i: i32 [storage=automatic] = from_bool<i32, reason=assign>(overflow_mul<bool>(const<i32>(1073741824), const<i32>(2), deref(addr_of<ptr<i32>>(%2))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%13)), read<i32>(%7), read<i32>(%2));
// DEFAULT-NEXT:         let %8 add_u: i32 [storage=automatic] = from_bool<i32, reason=assign>(overflow_add<bool>(const<u32>(4294967295), const<u32>(1), deref(addr_of<ptr<u32>>(%3))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%14)), read<i32>(%8), read<u32>(%3));
// DEFAULT-NEXT:         let %9 mul_ll: i32 [storage=automatic] = from_bool<i32, reason=assign>(overflow_mul<bool>(const<i64>(3037000500), const<i64>(3037000500), deref(addr_of<ptr<i64>>(%4))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%15)), read<i32>(%9), read<i64>(%4));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
