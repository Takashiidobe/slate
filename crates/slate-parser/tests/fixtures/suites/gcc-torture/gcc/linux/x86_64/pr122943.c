/* PR tree-optimization/122943 */

__attribute__((noipa)) unsigned char foo(long long val) {
  unsigned char result = 0;
  switch (val) {
  case 0:
    result = 1;
    break;
  case 1:
    result = 2;
    break;
  case 2:
    result = 3;
    break;
  default:
    break;
  }
  return result;
}

__attribute__((noipa)) unsigned char bar(long long val) {
  unsigned char result = 1;
  switch (val) {
  case 0:
    result = 8;
    break;
  case 1:
    result = 31;
    break;
  case 2:
    result = 72;
    break;
  default:
    break;
  }
  return result;
}

#ifdef __SIZEOF_INT128__
__attribute__((noipa)) unsigned char baz(__int128 val) {
  unsigned char result = 0;
  switch (val) {
  case 0:
    result = 1;
    break;
  case 1:
    result = 2;
    break;
  case 2:
    result = 3;
    break;
  default:
    break;
  }
  return result;
}

__attribute__((noipa)) unsigned char qux(__int128 val) {
  unsigned char result = 1;
  switch (val) {
  case 0:
    result = 8;
    break;
  case 1:
    result = 31;
    break;
  case 2:
    result = 72;
    break;
  default:
    break;
  }
  return result;
}
#endif

int main() {
  if (foo(-1) != 0)
    __builtin_abort();
  if (foo(0) != 1)
    __builtin_abort();
  if (foo(1) != 2)
    __builtin_abort();
  if (foo(2) != 3)
    __builtin_abort();
  if (foo(3) != 0)
    __builtin_abort();
  if (foo(-__LONG_LONG_MAX__ - 1) != 0)
    __builtin_abort();
  if (foo(-__LONG_LONG_MAX__) != 0)
    __builtin_abort();
  if (foo(-__LONG_LONG_MAX__ + 1) != 0)
    __builtin_abort();
  if (bar(-1) != 1)
    __builtin_abort();
  if (bar(0) != 8)
    __builtin_abort();
  if (bar(1) != 31)
    __builtin_abort();
  if (bar(2) != 72)
    __builtin_abort();
  if (bar(3) != 1)
    __builtin_abort();
  if (bar(-__LONG_LONG_MAX__ - 1) != 1)
    __builtin_abort();
  if (bar(-__LONG_LONG_MAX__) != 1)
    __builtin_abort();
  if (bar(-__LONG_LONG_MAX__ + 1) != 1)
    __builtin_abort();
#ifdef __SIZEOF_INT128__
  if (baz(-1) != 0)
    __builtin_abort();
  if (baz(0) != 1)
    __builtin_abort();
  if (baz(1) != 2)
    __builtin_abort();
  if (baz(2) != 3)
    __builtin_abort();
  if (baz(3) != 0)
    __builtin_abort();
  if (baz(((__int128)1) << 64) != 0)
    __builtin_abort();
  if (baz((((__int128)1) << 64) + 1) != 0)
    __builtin_abort();
  if (baz((((__int128)1) << 64) + 2) != 0)
    __builtin_abort();
  if (qux(-1) != 1)
    __builtin_abort();
  if (qux(0) != 8)
    __builtin_abort();
  if (qux(1) != 31)
    __builtin_abort();
  if (qux(2) != 72)
    __builtin_abort();
  if (qux(3) != 1)
    __builtin_abort();
  if (qux(((__int128)1) << 64) != 1)
    __builtin_abort();
  if (qux((((__int128)1) << 64) + 1) != 1)
    __builtin_abort();
  if (qux((((__int128)1) << 64) + 2) != 1)
    __builtin_abort();
#endif
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
// DEFAULT-NEXT:     fn %0 @foo(%1 val: i64) -> u8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %2 result: u8 [storage=automatic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         switch %13 read<i64>(%1)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %13 const<i64>(0):
// DEFAULT-NEXT:                     write<u8>(%2, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                 break %13;
// DEFAULT-NEXT:                 case %13 const<i64>(1):
// DEFAULT-NEXT:                     write<u8>(%2, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:                 break %13;
// DEFAULT-NEXT:                 case %13 const<i64>(2):
// DEFAULT-NEXT:                     write<u8>(%2, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(3))));
// DEFAULT-NEXT:                 break %13;
// DEFAULT-NEXT:                 default %13:
// DEFAULT-NEXT:                     break %13;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<u8>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @bar(%4 val: i64) -> u8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %5 result: u8 [storage=automatic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         switch %14 read<i64>(%4)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %14 const<i64>(0):
// DEFAULT-NEXT:                     write<u8>(%5, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(8))));
// DEFAULT-NEXT:                 break %14;
// DEFAULT-NEXT:                 case %14 const<i64>(1):
// DEFAULT-NEXT:                     write<u8>(%5, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(31))));
// DEFAULT-NEXT:                 break %14;
// DEFAULT-NEXT:                 case %14 const<i64>(2):
// DEFAULT-NEXT:                     write<u8>(%5, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(72))));
// DEFAULT-NEXT:                 break %14;
// DEFAULT-NEXT:                 default %14:
// DEFAULT-NEXT:                     break %14;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<u8>(%5);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @baz(%7 val: i128) -> u8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %8 result: u8 [storage=automatic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         switch %15 read<i128>(%7)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %15 const<i128>(0):
// DEFAULT-NEXT:                     write<u8>(%8, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                 break %15;
// DEFAULT-NEXT:                 case %15 const<i128>(1):
// DEFAULT-NEXT:                     write<u8>(%8, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:                 break %15;
// DEFAULT-NEXT:                 case %15 const<i128>(2):
// DEFAULT-NEXT:                     write<u8>(%8, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(3))));
// DEFAULT-NEXT:                 break %15;
// DEFAULT-NEXT:                 default %15:
// DEFAULT-NEXT:                     break %15;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<u8>(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @qux(%10 val: i128) -> u8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %11 result: u8 [storage=automatic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         switch %16 read<i128>(%10)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %16 const<i128>(0):
// DEFAULT-NEXT:                     write<u8>(%11, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(8))));
// DEFAULT-NEXT:                 break %16;
// DEFAULT-NEXT:                 case %16 const<i128>(1):
// DEFAULT-NEXT:                     write<u8>(%11, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(31))));
// DEFAULT-NEXT:                 break %16;
// DEFAULT-NEXT:                 case %16 const<i128>(2):
// DEFAULT-NEXT:                     write<u8>(%11, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(72))));
// DEFAULT-NEXT:                 break %16;
// DEFAULT-NEXT:                 default %16:
// DEFAULT-NEXT:                     break %16;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<u8>(%11);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %12 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i64) -> u8>(%0, widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1)))))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%17);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i64) -> u8>(%0, widen<i64, reason=arg>(const<i32>(0))))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%17);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i64) -> u8>(%0, widen<i64, reason=arg>(const<i32>(1))))), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%17);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i64) -> u8>(%0, widen<i64, reason=arg>(const<i32>(2))))), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%17);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i64) -> u8>(%0, widen<i64, reason=arg>(const<i32>(3))))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%17);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i64) -> u8>(%0, sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1)))))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%17);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i64) -> u8>(%0, neg<i64, overflow=ub>(const<i64>(9223372036854775807))))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%17);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i64) -> u8>(%0, add<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1)))))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%17);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i64) -> u8>(%3, widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1)))))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%17);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i64) -> u8>(%3, widen<i64, reason=arg>(const<i32>(0))))), const<i32>(8))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%17);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i64) -> u8>(%3, widen<i64, reason=arg>(const<i32>(1))))), const<i32>(31))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%17);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i64) -> u8>(%3, widen<i64, reason=arg>(const<i32>(2))))), const<i32>(72))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%17);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i64) -> u8>(%3, widen<i64, reason=arg>(const<i32>(3))))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%17);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i64) -> u8>(%3, sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1)))))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%17);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i64) -> u8>(%3, neg<i64, overflow=ub>(const<i64>(9223372036854775807))))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%17);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i64) -> u8>(%3, add<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1)))))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%17);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i128) -> u8>(%6, widen<i128, reason=arg>(neg<i32, overflow=ub>(const<i32>(1)))))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%17);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i128) -> u8>(%6, widen<i128, reason=arg>(const<i32>(0))))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%17);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i128) -> u8>(%6, widen<i128, reason=arg>(const<i32>(1))))), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%17);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i128) -> u8>(%6, widen<i128, reason=arg>(const<i32>(2))))), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%17);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i128) -> u8>(%6, widen<i128, reason=arg>(const<i32>(3))))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%17);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i128) -> u8>(%6, shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i128, reason=explicit>(const<i32>(1)), const<i32>(64))))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%17);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i128) -> u8>(%6, add<i128, overflow=ub>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i128, reason=explicit>(const<i32>(1)), const<i32>(64)), widen<i128, reason=usual_arith>(const<i32>(1)))))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%17);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i128) -> u8>(%6, add<i128, overflow=ub>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i128, reason=explicit>(const<i32>(1)), const<i32>(64)), widen<i128, reason=usual_arith>(const<i32>(2)))))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%17);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i128) -> u8>(%9, widen<i128, reason=arg>(neg<i32, overflow=ub>(const<i32>(1)))))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%17);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i128) -> u8>(%9, widen<i128, reason=arg>(const<i32>(0))))), const<i32>(8))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%17);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i128) -> u8>(%9, widen<i128, reason=arg>(const<i32>(1))))), const<i32>(31))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%17);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i128) -> u8>(%9, widen<i128, reason=arg>(const<i32>(2))))), const<i32>(72))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%17);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i128) -> u8>(%9, widen<i128, reason=arg>(const<i32>(3))))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%17);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i128) -> u8>(%9, shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i128, reason=explicit>(const<i32>(1)), const<i32>(64))))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%17);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i128) -> u8>(%9, add<i128, overflow=ub>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i128, reason=explicit>(const<i32>(1)), const<i32>(64)), widen<i128, reason=usual_arith>(const<i32>(1)))))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%17);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i128) -> u8>(%9, add<i128, overflow=ub>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i128, reason=explicit>(const<i32>(1)), const<i32>(64)), widen<i128, reason=usual_arith>(const<i32>(2)))))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%17);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
