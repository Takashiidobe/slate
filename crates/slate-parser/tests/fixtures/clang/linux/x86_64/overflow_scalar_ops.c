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
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 100, 32, 37, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([37, 100, 32, 37, 108, 108, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_ri:[0-9]+]] ri: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_ru:[0-9]+]] ru: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_rll:[0-9]+]] rll: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_add_i:[0-9]+]] add_i: i32 [storage=automatic] = from_bool<i32, reason=assign>(overflow_add<bool>(const<i32>(2147483647), const<i32>(1), deref(addr_of<ptr<i32>>(%[[VALUE_ri]]))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str]])), read<i32>(%[[VALUE_add_i]]), read<i32>(%[[VALUE_ri]]));
// DEFAULT-NEXT:         let %[[VALUE_sub_i:[0-9]+]] sub_i: i32 [storage=automatic] = from_bool<i32, reason=assign>(overflow_sub<bool>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(1), deref(addr_of<ptr<i32>>(%[[VALUE_ri]]))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str_2]])), read<i32>(%[[VALUE_sub_i]]), read<i32>(%[[VALUE_ri]]));
// DEFAULT-NEXT:         let %[[VALUE_mul_i:[0-9]+]] mul_i: i32 [storage=automatic] = from_bool<i32, reason=assign>(overflow_mul<bool>(const<i32>(1073741824), const<i32>(2), deref(addr_of<ptr<i32>>(%[[VALUE_ri]]))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str_3]])), read<i32>(%[[VALUE_mul_i]]), read<i32>(%[[VALUE_ri]]));
// DEFAULT-NEXT:         let %[[VALUE_add_u:[0-9]+]] add_u: i32 [storage=automatic] = from_bool<i32, reason=assign>(overflow_add<bool>(const<u32>(4294967295), const<u32>(1), deref(addr_of<ptr<u32>>(%[[VALUE_ru]]))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str_4]])), read<i32>(%[[VALUE_add_u]]), read<u32>(%[[VALUE_ru]]));
// DEFAULT-NEXT:         let %[[VALUE_mul_ll:[0-9]+]] mul_ll: i32 [storage=automatic] = from_bool<i32, reason=assign>(overflow_mul<bool>(const<i64>(3037000500), const<i64>(3037000500), deref(addr_of<ptr<i64>>(%[[VALUE_rll]]))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%[[VALUE_str_5]])), read<i32>(%[[VALUE_mul_ll]]), read<i64>(%[[VALUE_rll]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
