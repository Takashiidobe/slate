int g(void);
int h(void);

int either(void) {
  if (g() || h())
    return 1;
  return 0;
}

int both(void) {
  if (g() && h())
    return 1;
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @either() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V0:[0-9]+]]: bool [synthetic] = logical_or<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%[[V1:[0-9]+]]), const<i32>(0)), ne<i32>(call<i32, signature=fn() -> i32>(%[[V2:[0-9]+]]), const<i32>(0)));
// CHECK-NEXT:     if read<bool>(%[[V0]])
// CHECK-NEXT:         return const<i32>(1);
// CHECK-NEXT:     return const<i32>(0);
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @both() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V0:[0-9]+]]: bool [synthetic] = logical_and<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%[[V1:[0-9]+]]), const<i32>(0)), ne<i32>(call<i32, signature=fn() -> i32>(%[[V2:[0-9]+]]), const<i32>(0)));
// CHECK-NEXT:     if read<bool>(%[[V0]])
// CHECK-NEXT:         return const<i32>(1);
// CHECK-NEXT:     return const<i32>(0);
// CHECK: {{^}}}
// SLATE-FILECHECK-END
