void abort(void);

int movegt(int x, int y, long long a) {
  int i;
  int ret = 0;
  for (i = 0; i < y; i++) {
    if (a >= (long long)0xf000000000000000LL)
      ret = x;
    else
      ret = y;
  }
  return ret;
}

struct test {
  long long val;
  int       ret;
} tests[] = {
    {0xf000000000000000LL, -1}, {0xefffffffffffffffLL, 1},
    {0xf000000000000001LL, -1}, {0x0000000000000000LL, -1},
    {0x8000000000000000LL, 1},
};

int main() {
  int i;
  for (i = 0; i < sizeof(tests) / sizeof(tests[0]); i++) {
    if (movegt(-1, 1, tests[i].val) != tests[i].ret)
      abort();
  }
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
// DEFAULT-NEXT:     type @type[[TYPE_test:[0-9]+]] test = struct {
// DEFAULT-NEXT:         field0 val: i64;
// DEFAULT-NEXT:         field1 ret: i32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %[[VALUE_tests:[0-9]+]] tests:
// DEFAULT-SAME: array<@type[[TYPE_test]], 5> [storage=static] [align=16] =
// DEFAULT-SAME: aggregate<array<@type[[TYPE_test]], 5>, zero_fill=false>(index0 =
// DEFAULT-SAME: aggregate<@type[[TYPE_test]], zero_fill=false>(field0 = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(17293822569102704640)), field1 = neg<i32,
// DEFAULT-SAME: overflow=ub>(const<i32>(1))), index1 = aggregate<@type[[TYPE_test]], zero_fill=false>(field0 = reinterpret<i64, reason=assign,
// DEFAULT-SAME: fits=unknown>(const<u64>(17293822569102704639)), field1 = const<i32>(1)), index2 = aggregate<@type[[TYPE_test]], zero_fill=false>(field0 = reinterpret<i64,
// DEFAULT-SAME: reason=assign, fits=unknown>(const<u64>(17293822569102704641)), field1 = neg<i32, overflow=ub>(const<i32>(1))), index3 = aggregate<@type[[TYPE_test]],
// DEFAULT-SAME: zero_fill=false>(field0 = const<i64>(0), field1 = neg<i32, overflow=ub>(const<i32>(1))), index4 = aggregate<@type[[TYPE_test]], zero_fill=false>(field0 =
// DEFAULT-SAME: reinterpret<i64, reason=assign, fits=unknown>(const<u64>(9223372036854775808)), field1 = const<i32>(1))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_movegt:[0-9]+]] @movegt(%[[VALUE_x:[0-9]+]] x: i32, %[[VALUE_y:[0-9]+]] y: i32, %[[VALUE_a:[0-9]+]] a: i64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ret:[0-9]+]] ret: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_y]]))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ge<i64>(read<i64>(%[[VALUE_a]]), reinterpret<i64, reason=explicit, fits=unknown>(const<u64>(17293822569102704640)))
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_ret]], read<i32>(%[[VALUE_x]]));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_ret]], read<i32>(%[[VALUE_y]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_ret]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_i_2:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i_2]]))), div<u64, by_zero=ub>(const<u64>(80), const<u64>(16)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i32, i32, i64) -> i32>(%[[VALUE_movegt]], neg<i32, overflow=ub>(const<i32>(1)), const<i32>(1), read<i64>(field0(deref(ptr_offset<ptr<@type[[TYPE_test]]>, subtract=false, element=@type[[TYPE_test]], overflow=ub>(array_decay<ptr<@type[[TYPE_test]]>, length=Some(5)>(%[[VALUE_tests]]), read<i32>(%[[VALUE_i_2]])))))), read<i32>(field1(deref(ptr_offset<ptr<@type[[TYPE_test]]>, subtract=false, element=@type[[TYPE_test]], overflow=ub>(array_decay<ptr<@type[[TYPE_test]]>, length=Some(5)>(%[[VALUE_tests]]), read<i32>(%[[VALUE_i_2]]))))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
