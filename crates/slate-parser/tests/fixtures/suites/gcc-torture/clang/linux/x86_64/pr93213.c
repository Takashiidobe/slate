/* PR tree-optimization/93213 - wrong code on a multibyte store with
   -Og -foptimize-strlen
   { dg-require-effective-target int128 }
   { dg-additional-options "-Og -foptimize-strlen" } */

typedef unsigned __INT16_TYPE__ u16;
typedef unsigned __INT32_TYPE__ u32;
typedef unsigned __int128       u128;

static inline u128 foo(u16 u16_1, u32 u32_1, u128 u128_1) {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  u128 u128_0  = 0;
  u128_1      -= __builtin_mul_overflow(u32_1, u16_1, &u32_1);
  __builtin_memmove(&u16_1, &u128_0, 2);
  __builtin_memmove(&u16_1, &u128_1, 1);
  return u16_1;
#else
  return 0xff;
#endif
}

__attribute__((noipa)) void bar(void) {
  char       a[] = {1, 2};
  const char b[] = {0, 0};
  const char c[] = {2};
  __builtin_memcpy(a, b, 2);
  // The above is transformed into
  //   MEM <short unsigned int> [(char * {ref-all})&a] = 0;
  // which was then dropped because of the non-nul store below.
  __builtin_memcpy(a, c, 1);

  volatile char *p = a;
  if (p[0] != 2 || p[1] != 0)
    __builtin_abort();
}

int main(void) {
  u16 x = foo(-1, -1, 0);
  if (x != 0xff)
    __builtin_abort();

  bar();
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
// DEFAULT-NEXT:     type @type[[TYPE_u16:[0-9]+]] u16 = u16;
// DEFAULT-NEXT:     type @type[[TYPE_u32:[0-9]+]] u32 = u32;
// DEFAULT-NEXT:     type @type[[TYPE_u128:[0-9]+]] u128 = u128;
// DEFAULT-NEXT:     fn %[[VALUE___builtin_memmove:[0-9]+]] @__builtin_memmove(%[[VALUE0:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE2:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_u16_1:[0-9]+]] u16_1: u16, %[[VALUE_u32_1:[0-9]+]] u32_1: u32, %[[VALUE_u128_1:[0-9]+]] u128_1: u128) -> u128 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_u128_0:[0-9]+]] u128_0: u128 [storage=automatic] = reinterpret<u128, reason=assign, fits=unknown>(widen<i128, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: u128 [synthetic] = read<u128>(%[[VALUE_u128_1]]);
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: u128 [synthetic] = sub<u128, overflow=wrap>(read<u128>(%[[VALUE3]]), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(from_bool<i32, reason=promotion>(overflow_mul<bool>(read<u32>(%[[VALUE_u32_1]]), read<u16>(%[[VALUE_u16_1]]), deref(addr_of<ptr<u32>>(%[[VALUE_u32_1]])))))));
// DEFAULT-NEXT:         write<u128>(%[[VALUE_u128_1]], read<u128>(%[[VALUE4]]));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE___builtin_memmove]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<u16>>(%[[VALUE_u16_1]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<u128>>(%[[VALUE_u128_0]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE___builtin_memmove]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<u16>>(%[[VALUE_u16_1]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<u128>>(%[[VALUE_u128_1]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         return widen<u128, reason=return>(read<u16>(%[[VALUE_u16_1]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_memcpy:[0-9]+]] @__builtin_memcpy(%[[VALUE5:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE6:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE7:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: array<i8, 2> [storage=automatic] = aggregate<array<i8, 2>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(2)));
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: array<i8, 2> [storage=automatic] [const] = aggregate<array<i8, 2>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(0)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: array<i8, 1> [storage=automatic] [const] = aggregate<array<i8, 1>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(2)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE___builtin_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<const i8>, length=Some(2)>(%[[VALUE_b]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE___builtin_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<const i8>, length=Some(1)>(%[[VALUE_c]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<volatile i8> [storage=automatic] = pointer_cast<ptr<volatile i8>, reason=assign>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_a]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8, volatile>(deref(ptr_offset<ptr<volatile i8>, subtract=false, element=i8, overflow=ub>(read<ptr<volatile i8>>(%[[VALUE_p]]), const<i32>(0))))), const<i32>(2)), ne<i32>(widen<i32, reason=promotion>(read<i8, volatile>(deref(ptr_offset<ptr<volatile i8>, subtract=false, element=i8, overflow=ub>(read<ptr<volatile i8>>(%[[VALUE_p]]), const<i32>(1))))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: u16 [storage=automatic] = truncate<u16, reason=assign, fits=unknown>(call<u128, signature=fn(u16, u32, u128) -> u128>(%[[VALUE_foo]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<u32, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))), reinterpret<u128, reason=arg, fits=unknown>(widen<i128, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_x]]))), const<i32>(255))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_bar]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
