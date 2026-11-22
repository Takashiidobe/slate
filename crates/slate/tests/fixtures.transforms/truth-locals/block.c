// SLATE-TRANSFORM-ARGS -fblocks
int run(int (^)(int));
void go(void (^)(void));

int inside_block(int b) {
  return run(^(int c) {
    int lt = c < b;
    if (lt)
      return 7;
    return lt ? 1 : 2;
  });
}

int byref_flag(int a) {
  __block int seen = 0;
  go(^{ seen = a > 0; });
  return seen ? 1 : 0;
}

int captured_by_value(int a, int b) {
  int lt = a < b;
  if (lt)
    return 3;
  return run(^(int c) { return lt + c; });
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @inside_block(%{{[0-9]+}} b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     return call<i32, signature=fn(block<fn(i32) -> i32>) -> i32>(%[[V0:[0-9]+]], block_literal<block<fn(i32) -> i32>, %[[V1:[0-9]+]], captures=[%[[V_B:[0-9]+]]]>);
// CHECK-NEXT:     block_fn %[[V1]](%[[V_C:[0-9]+]] c: i32) -> i32 {
// CHECK-NEXT:         let %[[V_LT:[0-9]+]] lt: bool [storage=automatic] = lt<i32>(read<i32>(%[[V_C]]), read<i32>(%[[V_B]]));
// CHECK-NEXT:         if read<bool>(%[[V_LT]])
// CHECK-NEXT:             return const<i32>(7);
// CHECK-NEXT:         return conditional<i32>(read<bool>(%[[V_LT]]), const<i32>(1), const<i32>(2));
// CHECK-NEXT:     }
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @byref_flag(%{{[0-9]+}} a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_SEEN:[0-9]+]] seen: i32 [storage=automatic] [byref] = const<i32>(0);
// CHECK-NEXT:     call<void, signature=fn(block<fn() -> void>) -> void>(%[[V0:[0-9]+]], block_literal<block<fn() -> void>, %[[V1:[0-9]+]], captures=[%[[V_SEEN]] byref, %[[V_A:[0-9]+]]]>);
// CHECK-NEXT:     block_fn %[[V1]]() -> void {
// CHECK-NEXT:         write<i32>(%[[V_SEEN]], from_bool<i32, reason=assign>(gt<i32>(read<i32>(%[[V_A]]), const<i32>(0))));
// CHECK-NEXT:     }
// CHECK-NEXT:     return conditional<i32>(ne<i32>(read<i32>(%[[V_SEEN]]), const<i32>(0)), const<i32>(1), const<i32>(0));
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @captured_by_value(%{{[0-9]+}} a: i32, %{{[0-9]+}} b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_LT:[0-9]+]] lt: i32 [storage=automatic] = from_bool<i32, reason=assign>(lt<i32>(read<i32>(%[[V_A:[0-9]+]]), read<i32>(%[[V_B:[0-9]+]])));
// CHECK-NEXT:     if ne<i32>(read<i32>(%[[V_LT]]), const<i32>(0))
// CHECK-NEXT:         return const<i32>(3);
// CHECK-NEXT:     return call<i32, signature=fn(block<fn(i32) -> i32>) -> i32>(%[[V0:[0-9]+]], block_literal<block<fn(i32) -> i32>, %[[V1:[0-9]+]], captures=[%[[V_LT]]]>);
// CHECK-NEXT:     block_fn %[[V1]](%[[V_C:[0-9]+]] c: i32) -> i32 {
// CHECK-NEXT:         return add<i32, overflow=ub>(read<i32>(%[[V_LT]]), read<i32>(%[[V_C]]));
// CHECK-NEXT:     }
// CHECK: {{^}}}
// SLATE-FILECHECK-END
