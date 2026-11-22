#define TRUE 1

int g(int);

int macro_literal(int a) {
  int flag = 0;
  if (a)
    flag = TRUE;
  return flag ? 1 : 0;
}

int literal_flag(int n) {
  int found = 0;
  for (int i = 0; i < n; i++)
    if (g(i))
      found = 1;
  return found ? 2 : 3;
}

int comparison_chain(int a, int b, int c) {
  int lo = a < b;
  int hi = b <= c;
  int both = lo && hi;
  if (!both)
    return -1;
  return 1;
}

int copied(int a, int b) {
  int lt = a < b;
  int same = lt;
  if (same)
    return 4;
  return 5;
}

int conditional_definition(int a, int b) {
  int flag = a ? b > 2 : 0;
  while (flag)
    flag = g(a) > 0;
  return 0;
}

int mostly_truth_uses(int a, int b) {
  int lt = a < b;
  if (lt)
    return 7;
  while (!lt)
    lt = g(a) > b;
  return lt + 1;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @macro_literal(%{{[0-9]+}} a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_FLAG:[0-9]+]] flag: bool [storage=automatic] = const<bool>(false);
// CHECK-NEXT:     if ne<i32>(read<i32>(%[[V_A:[0-9]+]]), const<i32>(0))
// CHECK-NEXT:         write<bool>(%[[V_FLAG]], const<bool>(true));
// CHECK-NEXT:     return conditional<i32>(read<bool>(%[[V_FLAG]]), const<i32>(1), const<i32>(0));
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @literal_flag(%{{[0-9]+}} n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_FOUND:[0-9]+]] found: bool [storage=automatic] = const<bool>(false);
// CHECK-NEXT:     for %[[V0:[0-9]+]]
// CHECK-NEXT:         init:
// CHECK-NEXT:             let %[[V_I:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// CHECK-NEXT:         condition: lt<i32>(read<i32>(%[[V_I]]), read<i32>(%[[V_N:[0-9]+]]))
// CHECK-NEXT:         increment: {
// CHECK-NEXT:             let %[[V1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[V_I]]);
// CHECK-NEXT:             let %[[V2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[V1]]), const<i32>(1));
// CHECK-NEXT:             write<i32>(%[[V_I]], read<i32>(%[[V2]]));
// CHECK-NEXT:             yield void;
// CHECK-NEXT:         }
// CHECK-NEXT:         body:
// CHECK-NEXT:             if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[V3:[0-9]+]], read<i32>(%[[V_I]])), const<i32>(0))
// CHECK-NEXT:                 write<bool>(%[[V_FOUND]], const<bool>(true));
// CHECK-NEXT:     return conditional<i32>(read<bool>(%[[V_FOUND]]), const<i32>(2), const<i32>(3));
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @comparison_chain(%{{[0-9]+}} a: i32, %{{[0-9]+}} b: i32, %{{[0-9]+}} c: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_LO:[0-9]+]] lo: bool [storage=automatic] = lt<i32>(read<i32>(%[[V_A:[0-9]+]]), read<i32>(%[[V_B:[0-9]+]]));
// CHECK-NEXT:     let %[[V_HI:[0-9]+]] hi: bool [storage=automatic] = le<i32>(read<i32>(%[[V_B]]), read<i32>(%[[V_C:[0-9]+]]));
// CHECK-NEXT:     let %[[V_BOTH:[0-9]+]] both: bool [storage=automatic] = logical_and<bool>(read<bool>(%[[V_LO]]), read<bool>(%[[V_HI]]));
// CHECK-NEXT:     if not<bool>(read<bool>(%[[V_BOTH]]))
// CHECK-NEXT:         return neg<i32, overflow=ub>(const<i32>(1));
// CHECK-NEXT:     return const<i32>(1);
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @copied(%{{[0-9]+}} a: i32, %{{[0-9]+}} b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_LT:[0-9]+]] lt: bool [storage=automatic] = lt<i32>(read<i32>(%[[V_A:[0-9]+]]), read<i32>(%[[V_B:[0-9]+]]));
// CHECK-NEXT:     let %[[V_SAME:[0-9]+]] same: bool [storage=automatic] = read<bool>(%[[V_LT]]);
// CHECK-NEXT:     if read<bool>(%[[V_SAME]])
// CHECK-NEXT:         return const<i32>(4);
// CHECK-NEXT:     return const<i32>(5);
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @conditional_definition(%{{[0-9]+}} a: i32, %{{[0-9]+}} b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_FLAG:[0-9]+]] flag: bool [storage=automatic] = conditional<bool>(ne<i32>(read<i32>(%[[V_A:[0-9]+]]), const<i32>(0)), gt<i32>(read<i32>(%[[V_B:[0-9]+]]), const<i32>(2)), const<bool>(false));
// CHECK-NEXT:     while %[[V0:[0-9]+]] read<bool>(%[[V_FLAG]])
// CHECK-NEXT:         write<bool>(%[[V_FLAG]], gt<i32>(call<i32, signature=fn(i32) -> i32>(%[[V1:[0-9]+]], read<i32>(%[[V_A]])), const<i32>(0)));
// CHECK-NEXT:     return const<i32>(0);
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @mostly_truth_uses(%{{[0-9]+}} a: i32, %{{[0-9]+}} b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_LT:[0-9]+]] lt: bool [storage=automatic] = lt<i32>(read<i32>(%[[V_A:[0-9]+]]), read<i32>(%[[V_B:[0-9]+]]));
// CHECK-NEXT:     if read<bool>(%[[V_LT]])
// CHECK-NEXT:         return const<i32>(7);
// CHECK-NEXT:     while %[[V0:[0-9]+]] not<bool>(read<bool>(%[[V_LT]]))
// CHECK-NEXT:         write<bool>(%[[V_LT]], gt<i32>(call<i32, signature=fn(i32) -> i32>(%[[V1:[0-9]+]], read<i32>(%[[V_A]])), read<i32>(%[[V_B]])));
// CHECK-NEXT:     return add<i32, overflow=ub>(from_bool<i32, reason=assign>(read<bool>(%[[V_LT]])), const<i32>(1));
// CHECK: {{^}}}
// SLATE-FILECHECK-END
