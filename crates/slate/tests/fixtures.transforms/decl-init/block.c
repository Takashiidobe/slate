// SLATE-TRANSFORM-ARGS -fblocks
int run(int (^)(int));
void go(void (^)(void));
int compute(void);

int inside_block(void) {
  return run(^(int c) {
    int result;
    result = compute() + c;
    return result;
  });
}

int byref_written_in_block(void) {
  __block int result;
  go(^{ result = 1; });
  result = compute();
  return result;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @inside_block() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     return call<i32, signature=fn(block<fn(i32) -> i32>) -> i32>(%[[V0:[0-9]+]], block_literal<block<fn(i32) -> i32>, %[[V1:[0-9]+]], captures=[]>);
// CHECK-NEXT:     block_fn %[[V1]](%[[V_C:[0-9]+]] c: i32) -> i32 {
// CHECK-NEXT:         let %[[V_RESULT:[0-9]+]] result: i32 [storage=automatic] = add<i32, overflow=ub>(call<i32, signature=fn() -> i32>(%[[V2:[0-9]+]]), read<i32>(%[[V_C]]));
// CHECK-NEXT:         return read<i32>(%[[V_RESULT]]);
// CHECK-NEXT:     }
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @byref_written_in_block() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_RESULT:[0-9]+]] result: i32 [storage=automatic] [byref];
// CHECK-NEXT:     call<void, signature=fn(block<fn() -> void>) -> void>(%[[V0:[0-9]+]], block_literal<block<fn() -> void>, %[[V1:[0-9]+]], captures=[%[[V_RESULT]] byref]>);
// CHECK-NEXT:     block_fn %[[V1]]() -> void {
// CHECK-NEXT:         write<i32>(%[[V_RESULT]], const<i32>(1));
// CHECK-NEXT:     }
// CHECK-NEXT:     write<i32>(%[[V_RESULT]], call<i32, signature=fn() -> i32>(%[[V2:[0-9]+]]));
// CHECK-NEXT:     return read<i32>(%[[V_RESULT]]);
// CHECK: {{^}}}
// SLATE-FILECHECK-END
