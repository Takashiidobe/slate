// SLATE-TRANSFORM-ARGS -fblocks
int run(int (^)(int));
int compute(int);

int goto_in_block(int a) {
  return run(^(int c) {
    if (c > a)
      goto out;
    c = compute(c);
  out:
    return c;
  });
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @goto_in_block(%{{[0-9]+}} a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     return call<i32, signature=fn(block<fn(i32) -> i32>) -> i32>(%[[V0:[0-9]+]], block_literal<block<fn(i32) -> i32>, %[[V1:[0-9]+]], captures=[%[[V_A:[0-9]+]]]>);
// CHECK-NEXT:     block_fn %[[V1]](%[[V_C:[0-9]+]] c: i32) -> i32 {
// CHECK-NEXT:         block %[[V2:[0-9]+]] {
// CHECK-NEXT:             if gt<i32>(read<i32>(%[[V_C]]), read<i32>(%[[V_A]]))
// CHECK-NEXT:                 break %[[V2]];
// CHECK-NEXT:             write<i32>(%[[V_C]], call<i32, signature=fn(i32) -> i32>(%[[V3:[0-9]+]], read<i32>(%[[V_C]])));
// CHECK-NEXT:         }
// CHECK-NEXT:         return read<i32>(%[[V_C]]);
// CHECK-NEXT:     }
// CHECK: {{^}}}
// SLATE-FILECHECK-END
