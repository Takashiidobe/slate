int first(void);
int second(void);

void discarded_choice(int c) {
    int unused = 1 + 2;
    c ? first() : second();
}

int used_choice(int c) {
    return c ? first() : second();
}

void captured_extent(int n) {
    int array[n];
    (void)array;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @discarded_choice(%{{[0-9]+}} c: i32) -> void [linkage=external] [fallthrough=ret_void] {
// CHECK-NEXT:     let %[[V_UNUSED:[0-9]+]] unused: i32 [storage=automatic] = add<i32, overflow=ub>(const<i32>(1), const<i32>(2));
// CHECK-NEXT:     if ne<i32>(read<i32>(%[[V_C:[0-9]+]]), const<i32>(0))
// CHECK-NEXT:         call<i32, signature=fn() -> i32>(%[[V0:[0-9]+]]);
// CHECK-NEXT:     else
// CHECK-NEXT:         call<i32, signature=fn() -> i32>(%[[V1:[0-9]+]]);
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @used_choice(%{{[0-9]+}} c: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V0:[0-9]+]]: i32 [synthetic];
// CHECK-NEXT:     if ne<i32>(read<i32>(%[[V_C:[0-9]+]]), const<i32>(0))
// CHECK-NEXT:         write<i32>(%[[V0]], call<i32, signature=fn() -> i32>(%[[V1:[0-9]+]]));
// CHECK-NEXT:     else
// CHECK-NEXT:         write<i32>(%[[V0]], call<i32, signature=fn() -> i32>(%[[V2:[0-9]+]]));
// CHECK-NEXT:     return read<i32>(%[[V0]]);
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @captured_extent(%{{[0-9]+}} n: i32) -> void [linkage=external] [fallthrough=ret_void] {
// CHECK-NEXT:     let %[[V0:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[V_N:[0-9]+]]))) [c="__size_t"];
// CHECK-NEXT:     let %[[V_ARRAY:[0-9]+]] array: vla<i32, %[[V0]]> [storage=automatic];
// CHECK-NEXT:     array_decay<ptr<i32>, length=None>(%[[V_ARRAY]]);
// CHECK: {{^}}}
// SLATE-FILECHECK-END
