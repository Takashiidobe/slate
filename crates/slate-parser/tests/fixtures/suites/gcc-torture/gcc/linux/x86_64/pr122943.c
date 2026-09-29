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
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_val:[0-9]+]] val: i64) -> u8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_result:[0-9]+]] result: u8 [storage=automatic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         switch %[[VALUE0:[0-9]+]] read<i64>(%[[VALUE_val]])
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i64>(0):
// DEFAULT-NEXT:                     write<u8>(%[[VALUE_result]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i64>(1):
// DEFAULT-NEXT:                     write<u8>(%[[VALUE_result]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i64>(2):
// DEFAULT-NEXT:                     write<u8>(%[[VALUE_result]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(3))));
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 default %[[VALUE0]]:
// DEFAULT-NEXT:                     break %[[VALUE0]];
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<u8>(%[[VALUE_result]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_val_2:[0-9]+]] val: i64) -> u8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_result_2:[0-9]+]] result: u8 [storage=automatic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         switch %[[VALUE1:[0-9]+]] read<i64>(%[[VALUE_val_2]])
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i64>(0):
// DEFAULT-NEXT:                     write<u8>(%[[VALUE_result_2]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(8))));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i64>(1):
// DEFAULT-NEXT:                     write<u8>(%[[VALUE_result_2]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(31))));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i64>(2):
// DEFAULT-NEXT:                     write<u8>(%[[VALUE_result_2]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(72))));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 default %[[VALUE1]]:
// DEFAULT-NEXT:                     break %[[VALUE1]];
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<u8>(%[[VALUE_result_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_baz:[0-9]+]] @baz(%[[VALUE_val_3:[0-9]+]] val: i128) -> u8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_result_3:[0-9]+]] result: u8 [storage=automatic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         switch %[[VALUE2:[0-9]+]] read<i128>(%[[VALUE_val_3]])
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %[[VALUE2]] const<i128>(0):
// DEFAULT-NEXT:                     write<u8>(%[[VALUE_result_3]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                 break %[[VALUE2]];
// DEFAULT-NEXT:                 case %[[VALUE2]] const<i128>(1):
// DEFAULT-NEXT:                     write<u8>(%[[VALUE_result_3]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:                 break %[[VALUE2]];
// DEFAULT-NEXT:                 case %[[VALUE2]] const<i128>(2):
// DEFAULT-NEXT:                     write<u8>(%[[VALUE_result_3]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(3))));
// DEFAULT-NEXT:                 break %[[VALUE2]];
// DEFAULT-NEXT:                 default %[[VALUE2]]:
// DEFAULT-NEXT:                     break %[[VALUE2]];
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<u8>(%[[VALUE_result_3]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_qux:[0-9]+]] @qux(%[[VALUE_val_4:[0-9]+]] val: i128) -> u8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_result_4:[0-9]+]] result: u8 [storage=automatic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         switch %[[VALUE3:[0-9]+]] read<i128>(%[[VALUE_val_4]])
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %[[VALUE3]] const<i128>(0):
// DEFAULT-NEXT:                     write<u8>(%[[VALUE_result_4]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(8))));
// DEFAULT-NEXT:                 break %[[VALUE3]];
// DEFAULT-NEXT:                 case %[[VALUE3]] const<i128>(1):
// DEFAULT-NEXT:                     write<u8>(%[[VALUE_result_4]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(31))));
// DEFAULT-NEXT:                 break %[[VALUE3]];
// DEFAULT-NEXT:                 case %[[VALUE3]] const<i128>(2):
// DEFAULT-NEXT:                     write<u8>(%[[VALUE_result_4]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(72))));
// DEFAULT-NEXT:                 break %[[VALUE3]];
// DEFAULT-NEXT:                 default %[[VALUE3]]:
// DEFAULT-NEXT:                     break %[[VALUE3]];
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<u8>(%[[VALUE_result_4]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i64) -> u8>(%[[VALUE_foo]], widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1)))))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i64) -> u8>(%[[VALUE_foo]], widen<i64, reason=arg>(const<i32>(0))))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i64) -> u8>(%[[VALUE_foo]], widen<i64, reason=arg>(const<i32>(1))))), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i64) -> u8>(%[[VALUE_foo]], widen<i64, reason=arg>(const<i32>(2))))), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i64) -> u8>(%[[VALUE_foo]], widen<i64, reason=arg>(const<i32>(3))))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i64) -> u8>(%[[VALUE_foo]], sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1)))))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i64) -> u8>(%[[VALUE_foo]], neg<i64, overflow=ub>(const<i64>(9223372036854775807))))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i64) -> u8>(%[[VALUE_foo]], add<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1)))))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i64) -> u8>(%[[VALUE_bar]], widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1)))))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i64) -> u8>(%[[VALUE_bar]], widen<i64, reason=arg>(const<i32>(0))))), const<i32>(8))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i64) -> u8>(%[[VALUE_bar]], widen<i64, reason=arg>(const<i32>(1))))), const<i32>(31))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i64) -> u8>(%[[VALUE_bar]], widen<i64, reason=arg>(const<i32>(2))))), const<i32>(72))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i64) -> u8>(%[[VALUE_bar]], widen<i64, reason=arg>(const<i32>(3))))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i64) -> u8>(%[[VALUE_bar]], sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1)))))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i64) -> u8>(%[[VALUE_bar]], neg<i64, overflow=ub>(const<i64>(9223372036854775807))))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i64) -> u8>(%[[VALUE_bar]], add<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1)))))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i128) -> u8>(%[[VALUE_baz]], widen<i128, reason=arg>(neg<i32, overflow=ub>(const<i32>(1)))))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i128) -> u8>(%[[VALUE_baz]], widen<i128, reason=arg>(const<i32>(0))))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i128) -> u8>(%[[VALUE_baz]], widen<i128, reason=arg>(const<i32>(1))))), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i128) -> u8>(%[[VALUE_baz]], widen<i128, reason=arg>(const<i32>(2))))), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i128) -> u8>(%[[VALUE_baz]], widen<i128, reason=arg>(const<i32>(3))))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i128) -> u8>(%[[VALUE_baz]], shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i128, reason=explicit>(const<i32>(1)), const<i32>(64))))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i128) -> u8>(%[[VALUE_baz]], add<i128, overflow=ub>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i128, reason=explicit>(const<i32>(1)), const<i32>(64)), widen<i128, reason=usual_arith>(const<i32>(1)))))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i128) -> u8>(%[[VALUE_baz]], add<i128, overflow=ub>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i128, reason=explicit>(const<i32>(1)), const<i32>(64)), widen<i128, reason=usual_arith>(const<i32>(2)))))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i128) -> u8>(%[[VALUE_qux]], widen<i128, reason=arg>(neg<i32, overflow=ub>(const<i32>(1)))))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i128) -> u8>(%[[VALUE_qux]], widen<i128, reason=arg>(const<i32>(0))))), const<i32>(8))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i128) -> u8>(%[[VALUE_qux]], widen<i128, reason=arg>(const<i32>(1))))), const<i32>(31))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i128) -> u8>(%[[VALUE_qux]], widen<i128, reason=arg>(const<i32>(2))))), const<i32>(72))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i128) -> u8>(%[[VALUE_qux]], widen<i128, reason=arg>(const<i32>(3))))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i128) -> u8>(%[[VALUE_qux]], shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i128, reason=explicit>(const<i32>(1)), const<i32>(64))))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i128) -> u8>(%[[VALUE_qux]], add<i128, overflow=ub>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i128, reason=explicit>(const<i32>(1)), const<i32>(64)), widen<i128, reason=usual_arith>(const<i32>(1)))))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(i128) -> u8>(%[[VALUE_qux]], add<i128, overflow=ub>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i128, reason=explicit>(const<i32>(1)), const<i32>(64)), widen<i128, reason=usual_arith>(const<i32>(2)))))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
