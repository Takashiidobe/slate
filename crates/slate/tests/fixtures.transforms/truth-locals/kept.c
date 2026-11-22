int g(int);
void clear(int *);

int integer_only(int a, int b) {
  int eq = a == b;
  return eq;
}

int balanced_uses(int a, int b) {
  int lt = a < b;
  if (lt)
    return 7;
  return lt + 1;
}

int outside_range(int a) {
  int state = a > 0;
  if (a > 100)
    state = 2;
  return state != 0;
}

int call_defined(int a) {
  int r = g(a);
  if (r)
    return 1;
  return 0;
}

int incremented(int a) {
  int seen = 0;
  if (a)
    seen++;
  return seen ? 1 : 0;
}

int address_taken(int a, int b) {
  int lt = a < b;
  clear(&lt);
  return lt ? 5 : 6;
}

int assigned_in_condition(int a, int b) {
  int lt;
  if ((lt = a < b))
    return 1;
  return lt ? 2 : 3;
}

int volatile_flag(int a) {
  volatile int seen = a > 0;
  return seen ? 1 : 0;
}

int static_flag(int a) {
  static int seen = 0;
  if (a)
    seen = 1;
  return seen ? 1 : 0;
}

int uninitialized(int a) {
  int flag;
  if (a)
    flag = 1;
  return flag ? 1 : 0;
}

int depends_on_kept(int a) {
  int r = g(a);
  int copy = r;
  return copy ? 1 : 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @integer_only(%{{[0-9]+}} a: i32, %{{[0-9]+}} b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_EQ:[0-9]+]] eq: i32 [storage=automatic] = from_bool<i32, reason=assign>(eq<i32>(read<i32>(%[[V_A:[0-9]+]]), read<i32>(%[[V_B:[0-9]+]])));
// CHECK-NEXT:     return read<i32>(%[[V_EQ]]);
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @balanced_uses(%{{[0-9]+}} a: i32, %{{[0-9]+}} b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_LT:[0-9]+]] lt: i32 [storage=automatic] = from_bool<i32, reason=assign>(lt<i32>(read<i32>(%[[V_A:[0-9]+]]), read<i32>(%[[V_B:[0-9]+]])));
// CHECK-NEXT:     if ne<i32>(read<i32>(%[[V_LT]]), const<i32>(0))
// CHECK-NEXT:         return const<i32>(7);
// CHECK-NEXT:     return add<i32, overflow=ub>(read<i32>(%[[V_LT]]), const<i32>(1));
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @outside_range(%{{[0-9]+}} a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_STATE:[0-9]+]] state: i32 [storage=automatic] = from_bool<i32, reason=assign>(gt<i32>(read<i32>(%[[V_A:[0-9]+]]), const<i32>(0)));
// CHECK-NEXT:     if gt<i32>(read<i32>(%[[V_A]]), const<i32>(100))
// CHECK-NEXT:         write<i32>(%[[V_STATE]], const<i32>(2));
// CHECK-NEXT:     return from_bool<i32, reason=return>(ne<i32>(read<i32>(%[[V_STATE]]), const<i32>(0)));
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @call_defined(%{{[0-9]+}} a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_R:[0-9]+]] r: i32 [storage=automatic] = call<i32, signature=fn(i32) -> i32>(%[[V0:[0-9]+]], read<i32>(%[[V_A:[0-9]+]]));
// CHECK-NEXT:     if ne<i32>(read<i32>(%[[V_R]]), const<i32>(0))
// CHECK-NEXT:         return const<i32>(1);
// CHECK-NEXT:     return const<i32>(0);
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @incremented(%{{[0-9]+}} a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_SEEN:[0-9]+]] seen: i32 [storage=automatic] = const<i32>(0);
// CHECK-NEXT:     if ne<i32>(read<i32>(%[[V_A:[0-9]+]]), const<i32>(0))
// CHECK-NEXT:         let %[[V0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[V_SEEN]]);
// CHECK-NEXT:         let %[[V1:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[V0]]), const<i32>(1));
// CHECK-NEXT:         write<i32>(%[[V_SEEN]], read<i32>(%[[V1]]));
// CHECK-NEXT:     return conditional<i32>(ne<i32>(read<i32>(%[[V_SEEN]]), const<i32>(0)), const<i32>(1), const<i32>(0));
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @address_taken(%{{[0-9]+}} a: i32, %{{[0-9]+}} b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_LT:[0-9]+]] lt: i32 [storage=automatic] = from_bool<i32, reason=assign>(lt<i32>(read<i32>(%[[V_A:[0-9]+]]), read<i32>(%[[V_B:[0-9]+]])));
// CHECK-NEXT:     call<void, signature=fn(ptr<i32>) -> void>(%[[V0:[0-9]+]], addr_of<ptr<i32>>(%[[V_LT]]));
// CHECK-NEXT:     return conditional<i32>(ne<i32>(read<i32>(%[[V_LT]]), const<i32>(0)), const<i32>(5), const<i32>(6));
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @assigned_in_condition(%{{[0-9]+}} a: i32, %{{[0-9]+}} b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_LT:[0-9]+]] lt: i32 [storage=automatic];
// CHECK-NEXT:     let %[[V0:[0-9]+]]: i32 [synthetic] = from_bool<i32, reason=assign>(lt<i32>(read<i32>(%[[V_A:[0-9]+]]), read<i32>(%[[V_B:[0-9]+]])));
// CHECK-NEXT:     write<i32>(%[[V_LT]], read<i32>(%[[V0]]));
// CHECK-NEXT:     if ne<i32>(read<i32>(%[[V0]]), const<i32>(0))
// CHECK-NEXT:         return const<i32>(1);
// CHECK-NEXT:     return conditional<i32>(ne<i32>(read<i32>(%[[V_LT]]), const<i32>(0)), const<i32>(2), const<i32>(3));
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @volatile_flag(%{{[0-9]+}} a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_SEEN:[0-9]+]] seen: volatile i32 [storage=automatic] = from_bool<i32, reason=assign>(gt<i32>(read<i32>(%[[V_A:[0-9]+]]), const<i32>(0)));
// CHECK-NEXT:     return conditional<i32>(ne<i32>(read<i32, volatile>(%[[V_SEEN]]), const<i32>(0)), const<i32>(1), const<i32>(0));
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @static_flag(%{{[0-9]+}} a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     if ne<i32>(read<i32>(%[[V_A:[0-9]+]]), const<i32>(0))
// CHECK-NEXT:         write<i32>(%[[V0:[0-9]+]], const<i32>(1));
// CHECK-NEXT:     return conditional<i32>(ne<i32>(read<i32>(%[[V0]]), const<i32>(0)), const<i32>(1), const<i32>(0));
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @uninitialized(%{{[0-9]+}} a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_FLAG:[0-9]+]] flag: i32 [storage=automatic];
// CHECK-NEXT:     if ne<i32>(read<i32>(%[[V_A:[0-9]+]]), const<i32>(0))
// CHECK-NEXT:         write<i32>(%[[V_FLAG]], const<i32>(1));
// CHECK-NEXT:     return conditional<i32>(ne<i32>(read<i32>(%[[V_FLAG]]), const<i32>(0)), const<i32>(1), const<i32>(0));
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @depends_on_kept(%{{[0-9]+}} a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_R:[0-9]+]] r: i32 [storage=automatic] = call<i32, signature=fn(i32) -> i32>(%[[V0:[0-9]+]], read<i32>(%[[V_A:[0-9]+]]));
// CHECK-NEXT:     let %[[V_COPY:[0-9]+]] copy: i32 [storage=automatic] = read<i32>(%[[V_R]]);
// CHECK-NEXT:     return conditional<i32>(ne<i32>(read<i32>(%[[V_COPY]]), const<i32>(0)), const<i32>(1), const<i32>(0));
// CHECK: {{^}}}
// SLATE-FILECHECK-END
