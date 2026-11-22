int compute(void);
void cleanup(int *);

int across_call(int *flags) {
    int result;
    int old = *flags;
    result = compute();
    return old + result;
}

int explicit_zero(void) {
    int result = 0;
    result = compute();
    return result;
}

int with_guard(void) {
    int result;
    int guard __attribute__((cleanup(cleanup))) = compute();
    result = compute();
    return result;
}

void check(int *);
int addressed(void) {
    int result;
    check(&result);
    result = compute();
    return result;
}

int with_label(int c) {
    int result;
    if (c) goto init;
init:
    result = compute();
    return result;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @across_call(%{{[0-9]+}} flags: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_OLD:[0-9]+]] old: i32 [storage=automatic] = read<i32>(deref(read<ptr<i32>>(%[[V_FLAGS:[0-9]+]])));
// CHECK-NEXT:     let %[[V_RESULT:[0-9]+]] result: i32 [storage=automatic] = call<i32, signature=fn() -> i32>(%[[V0:[0-9]+]]);
// CHECK-NEXT:     return add<i32, overflow=ub>(read<i32>(%[[V_OLD]]), read<i32>(%[[V_RESULT]]));
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @explicit_zero() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_RESULT:[0-9]+]] result: i32 [storage=automatic] = const<i32>(0);
// CHECK-NEXT:     write<i32>(%[[V_RESULT]], call<i32, signature=fn() -> i32>(%[[V0:[0-9]+]]));
// CHECK-NEXT:     return read<i32>(%[[V_RESULT]]);
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @with_guard() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_RESULT:[0-9]+]] result: i32 [storage=automatic];
// CHECK-NEXT:     let %[[V_GUARD:[0-9]+]] guard: i32 [storage=automatic] [cleanup=%[[V0:[0-9]+]]] = call<i32, signature=fn() -> i32>(%[[V1:[0-9]+]]);
// CHECK-NEXT:     write<i32>(%[[V_RESULT]], call<i32, signature=fn() -> i32>(%[[V1]]));
// CHECK-NEXT:     return read<i32>(%[[V_RESULT]]);
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @addressed() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_RESULT:[0-9]+]] result: i32 [storage=automatic];
// CHECK-NEXT:     call<void, signature=fn(ptr<i32>) -> void>(%[[V0:[0-9]+]], addr_of<ptr<i32>>(%[[V_RESULT]]));
// CHECK-NEXT:     write<i32>(%[[V_RESULT]], call<i32, signature=fn() -> i32>(%[[V1:[0-9]+]]));
// CHECK-NEXT:     return read<i32>(%[[V_RESULT]]);
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @with_label(%{{[0-9]+}} c: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_RESULT:[0-9]+]] result: i32 [storage=automatic];
// CHECK-NEXT:     if ne<i32>(read<i32>(%[[V_C:[0-9]+]]), const<i32>(0))
// CHECK-NEXT:         goto %[[V_INIT:[0-9]+]];
// CHECK-NEXT:     label %[[V_INIT]] init:
// CHECK-NEXT:         write<i32>(%[[V_RESULT]], call<i32, signature=fn() -> i32>(%[[V0:[0-9]+]]));
// CHECK-NEXT:     return read<i32>(%[[V_RESULT]]);
// CHECK: {{^}}}
// SLATE-FILECHECK-END
