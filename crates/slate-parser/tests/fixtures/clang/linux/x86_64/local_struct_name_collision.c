#include <stdio.h>

static int sum_docs(void) {
  struct TestCase {
    const char *doc;
    int         expectedStatus;
  };

  const struct TestCase cases[] = {
      {"a", 1},
      {"bb", 2},
      {"ccc", 3},
  };

  int total = 0;
  for (int i = 0; i < 3; i++) {
    total += (int)cases[i].doc[0] * cases[i].expectedStatus;
  }
  return total;
}

static int sum_flags(void) {
  struct TestCase {
    int usesParameterEntities;
    int weight;
  };

  const struct TestCase cases[] = {
      {1, 10},
      {0, 20},
  };

  int total = 0;
  for (int i = 0; i < 2; i++) {
    if (cases[i].usesParameterEntities) {
      total += cases[i].weight;
    } else {
      total -= cases[i].weight;
    }
  }
  return total;
}

static int sum_movements(void) {
  struct TestCase {
    int         expectedMovementInChars;
    const char *input;
  };

  struct TestCase cases[] = {
      {1, "x"},
      {2, "yy"},
      {3, "zzz"},
  };

  int total = 0;
  for (int i = 0; i < 3; i++) {
    total += cases[i].expectedMovementInChars + (int)cases[i].input[0];
  }
  return total;
}

int main(void) {
  printf("%d %d %d\n", sum_docs(), sum_flags(), sum_movements());
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
// DEFAULT-NEXT:     type @type[[TYPE_TestCase:[0-9]+]] TestCase = struct {
// DEFAULT-NEXT:         field0 doc: ptr<const i8>;
// DEFAULT-NEXT:         field1 expectedStatus: i32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_TestCase_2:[0-9]+]] TestCase = struct {
// DEFAULT-NEXT:         field0 usesParameterEntities: i32;
// DEFAULT-NEXT:         field1 weight: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_TestCase_3:[0-9]+]] TestCase = struct {
// DEFAULT-NEXT:         field0 expectedMovementInChars: i32;
// DEFAULT-NEXT:         field1 input: ptr<const i8>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([97, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([98, 98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([99, 99, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([120, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([121, 121, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_6:[0-9]+]] .str[[VALUE_str_6]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([122, 122, 122, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_7:[0-9]+]] .str[[VALUE_str_7]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sum_docs:[0-9]+]] @sum_docs() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_cases:[0-9]+]] cases: array<@type[[TYPE_TestCase]], 3> [storage=automatic] [const] [align=16] = aggregate<array<@type[[TYPE_TestCase]], 3>, zero_fill=false>(index0 = aggregate<@type[[TYPE_TestCase]], zero_fill=false>(field0 = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str]])), field1 = const<i32>(1)), index1 = aggregate<@type[[TYPE_TestCase]], zero_fill=false>(field0 = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str_2]])), field1 = const<i32>(2)), index2 = aggregate<@type[[TYPE_TestCase]], zero_fill=false>(field0 = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_3]])), field1 = const<i32>(3)));
// DEFAULT-NEXT:         let %[[VALUE_total:[0-9]+]] total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(3))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:                     let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE3]]), mul<i32, overflow=ub>(widen<i32, reason=explicit>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(field0(deref(ptr_offset<ptr<const @type[[TYPE_TestCase]]>, subtract=false, element=@type[[TYPE_TestCase]], overflow=ub>(array_decay<ptr<const @type[[TYPE_TestCase]]>, length=Some(3)>(%[[VALUE_cases]]), read<i32>(%[[VALUE_i]]))))), const<i32>(0))))), read<i32>(field1(deref(ptr_offset<ptr<const @type[[TYPE_TestCase]]>, subtract=false, element=@type[[TYPE_TestCase]], overflow=ub>(array_decay<ptr<const @type[[TYPE_TestCase]]>, length=Some(3)>(%[[VALUE_cases]]), read<i32>(%[[VALUE_i]])))))));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_sum_flags:[0-9]+]] @sum_flags() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_cases_2:[0-9]+]] cases: array<@type[[TYPE_TestCase_2]], 2> [storage=automatic] [const] [align=16] = aggregate<array<@type[[TYPE_TestCase_2]], 2>, zero_fill=false>(index0 = aggregate<@type[[TYPE_TestCase_2]], zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(10)), index1 = aggregate<@type[[TYPE_TestCase_2]], zero_fill=false>(field0 = const<i32>(0), field1 = const<i32>(20)));
// DEFAULT-NEXT:         let %[[VALUE_total_2:[0-9]+]] total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %[[VALUE5:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i_2:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(2))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE6]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE7]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(field0(deref(ptr_offset<ptr<const @type[[TYPE_TestCase_2]]>, subtract=false, element=@type[[TYPE_TestCase_2]], overflow=ub>(array_decay<ptr<const @type[[TYPE_TestCase_2]]>, length=Some(2)>(%[[VALUE_cases_2]]), read<i32>(%[[VALUE_i_2]]))))), const<i32>(0))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             let %[[VALUE8:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_2]]);
// DEFAULT-NEXT:                             let %[[VALUE9:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE8]]), read<i32>(field1(deref(ptr_offset<ptr<const @type[[TYPE_TestCase_2]]>, subtract=false, element=@type[[TYPE_TestCase_2]], overflow=ub>(array_decay<ptr<const @type[[TYPE_TestCase_2]]>, length=Some(2)>(%[[VALUE_cases_2]]), read<i32>(%[[VALUE_i_2]]))))));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_total_2]], read<i32>(%[[VALUE9]]));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             let %[[VALUE10:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_2]]);
// DEFAULT-NEXT:                             let %[[VALUE11:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE10]]), read<i32>(field1(deref(ptr_offset<ptr<const @type[[TYPE_TestCase_2]]>, subtract=false, element=@type[[TYPE_TestCase_2]], overflow=ub>(array_decay<ptr<const @type[[TYPE_TestCase_2]]>, length=Some(2)>(%[[VALUE_cases_2]]), read<i32>(%[[VALUE_i_2]]))))));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_total_2]], read<i32>(%[[VALUE11]]));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_total_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_sum_movements:[0-9]+]] @sum_movements() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_cases_3:[0-9]+]] cases: array<@type[[TYPE_TestCase_3]], 3> [storage=automatic] [align=16] = aggregate<array<@type[[TYPE_TestCase_3]], 3>, zero_fill=false>(index0 = aggregate<@type[[TYPE_TestCase_3]], zero_fill=false>(field0 = const<i32>(1), field1 = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_4]]))), index1 = aggregate<@type[[TYPE_TestCase_3]], zero_fill=false>(field0 = const<i32>(2), field1 = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str_5]]))), index2 = aggregate<@type[[TYPE_TestCase_3]], zero_fill=false>(field0 = const<i32>(3), field1 = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_6]]))));
// DEFAULT-NEXT:         let %[[VALUE_total_3:[0-9]+]] total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %[[VALUE12:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i_3:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_3]]), const<i32>(3))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE13:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_3]]);
// DEFAULT-NEXT:                 let %[[VALUE14:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE13]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_3]], read<i32>(%[[VALUE14]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE15:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_3]]);
// DEFAULT-NEXT:                     let %[[VALUE16:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE15]]), add<i32, overflow=ub>(read<i32>(field0(deref(ptr_offset<ptr<@type[[TYPE_TestCase_3]]>, subtract=false, element=@type[[TYPE_TestCase_3]], overflow=ub>(array_decay<ptr<@type[[TYPE_TestCase_3]]>, length=Some(3)>(%[[VALUE_cases_3]]), read<i32>(%[[VALUE_i_3]]))))), widen<i32, reason=explicit>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(field1(deref(ptr_offset<ptr<@type[[TYPE_TestCase_3]]>, subtract=false, element=@type[[TYPE_TestCase_3]], overflow=ub>(array_decay<ptr<@type[[TYPE_TestCase_3]]>, length=Some(3)>(%[[VALUE_cases_3]]), read<i32>(%[[VALUE_i_3]]))))), const<i32>(0)))))));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_total_3]], read<i32>(%[[VALUE16]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_total_3]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_7]])), call<i32, signature=fn() -> i32>(%[[VALUE_sum_docs]]), call<i32, signature=fn() -> i32>(%[[VALUE_sum_flags]]), call<i32, signature=fn() -> i32>(%[[VALUE_sum_movements]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
