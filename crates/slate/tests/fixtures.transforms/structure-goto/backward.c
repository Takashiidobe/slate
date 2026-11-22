#include <stdio.h>

int retry(int n) {
    int total = 0;
again:
    total += n;
    if (--n > 0)
        goto again;
    return total;
}

int spin(int n) {
    int steps = 0;
top:
    steps++;
    if (n-- <= 0)
        return steps;
    goto top;
}

int restart(int *values, int count) {
    int i, swaps = 0;
restart:
    for (i = 0; i + 1 < count; i++) {
        if (values[i] > values[i + 1]) {
            int t = values[i];
            values[i] = values[i + 1];
            values[i + 1] = t;
            swaps++;
            goto restart;
        }
    }
    return swaps;
}

int two_edges(int n) {
    int odd = 0, even = 0;
loop:
    if (n <= 0)
        return odd * 100 + even;
    if (n % 2) {
        odd++;
        n--;
        goto loop;
    }
    even++;
    n--;
    goto loop;
}

int entered(int n, int skip) {
    int total = 0;
    if (skip)
        goto body;
    total = 100;
body:
    total += n;
    if (--n > 0)
        goto body;
    return total;
}

int scoped(int n) {
    int sum = 0;
again:
    {
        int square = n * n;
        sum += square;
    }
    if (--n > 0)
        goto again;
    return sum;
}

int main(void) {
    int values[] = {4, 1, 3, 2};
    printf("%d %d\n", retry(4), retry(1));
    printf("%d %d\n", spin(3), spin(0));
    printf("%d\n", restart(values, 4));
    printf("%d %d %d %d\n", values[0], values[1], values[2], values[3]);
    printf("%d\n", two_edges(7));
    printf("%d %d\n", entered(3, 0), entered(3, 1));
    printf("%d\n", scoped(4));
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @retry(%{{[0-9]+}} n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_TOTAL:[0-9]+]] total: i32 [storage=automatic] = const<i32>(0);
// CHECK-NEXT:     do %[[V0:[0-9]+]]
// CHECK-NEXT:         let %[[V1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[V_TOTAL]]);
// CHECK-NEXT:         let %[[V2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[V1]]), read<i32>(%[[V_N:[0-9]+]]));
// CHECK-NEXT:         write<i32>(%[[V_TOTAL]], read<i32>(%[[V2]]));
// CHECK-NEXT:     while {
// CHECK-NEXT:         let %[[V3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[V_N]]);
// CHECK-NEXT:         let %[[V4:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[V3]]), const<i32>(1));
// CHECK-NEXT:         write<i32>(%[[V_N]], read<i32>(%[[V4]]));
// CHECK-NEXT:         yield gt<i32>(read<i32>(%[[V4]]), const<i32>(0));
// CHECK-NEXT:     };
// CHECK-NEXT:     return read<i32>(%[[V_TOTAL]]);
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @spin(%{{[0-9]+}} n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_STEPS:[0-9]+]] steps: i32 [storage=automatic] = const<i32>(0);
// CHECK-NEXT:     for %[[V0:[0-9]+]]
// CHECK-NEXT:         init:
// CHECK-NEXT:         condition: omitted
// CHECK-NEXT:         increment: omitted
// CHECK-NEXT:         body:
// CHECK-NEXT:             let %[[V1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[V_STEPS]]);
// CHECK-NEXT:             let %[[V2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[V1]]), const<i32>(1));
// CHECK-NEXT:             write<i32>(%[[V_STEPS]], read<i32>(%[[V2]]));
// CHECK-NEXT:             let %[[V3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[V_N:[0-9]+]]);
// CHECK-NEXT:             let %[[V4:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[V3]]), const<i32>(1));
// CHECK-NEXT:             write<i32>(%[[V_N]], read<i32>(%[[V4]]));
// CHECK-NEXT:             if le<i32>(read<i32>(%[[V3]]), const<i32>(0))
// CHECK-NEXT:                 return read<i32>(%[[V_STEPS]]);
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @restart(%{{[0-9]+}} values: ptr<i32>, %{{[0-9]+}} count: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_I:[0-9]+]] i: i32 [storage=automatic];
// CHECK-NEXT:     let %[[V_SWAPS:[0-9]+]] swaps: i32 [storage=automatic] = const<i32>(0);
// CHECK-NEXT:     for %[[V0:[0-9]+]]
// CHECK-NEXT:         init:
// CHECK-NEXT:         condition: omitted
// CHECK-NEXT:         increment: omitted
// CHECK-NEXT:         body:
// CHECK-NEXT:             for %[[V1:[0-9]+]]
// CHECK-NEXT:                 init:
// CHECK-NEXT:                     write<i32>(%[[V_I]], const<i32>(0));
// CHECK-NEXT:                 condition: lt<i32>(add<i32, overflow=ub>(read<i32>(%[[V_I]]), const<i32>(1)), read<i32>(%[[V_COUNT:[0-9]+]]))
// CHECK-NEXT:                 increment: {
// CHECK-NEXT:                     let %[[V2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[V_I]]);
// CHECK-NEXT:                     let %[[V3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[V2]]), const<i32>(1));
// CHECK-NEXT:                     write<i32>(%[[V_I]], read<i32>(%[[V3]]));
// CHECK-NEXT:                     yield void;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 body:
// CHECK-NEXT:                     {
// CHECK-NEXT:                         if gt<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[V_VALUES:[0-9]+]]), read<i32>(%[[V_I]])))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[V_VALUES]]), add<i32, overflow=ub>(read<i32>(%[[V_I]]), const<i32>(1))))))
// CHECK-NEXT:                             {
// CHECK-NEXT:                                 let %[[V_T:[0-9]+]] t: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[V_VALUES]]), read<i32>(%[[V_I]]))));
// CHECK-NEXT:                                 write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[V_VALUES]]), read<i32>(%[[V_I]]))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[V_VALUES]]), add<i32, overflow=ub>(read<i32>(%[[V_I]]), const<i32>(1))))));
// CHECK-NEXT:                                 write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[V_VALUES]]), add<i32, overflow=ub>(read<i32>(%[[V_I]]), const<i32>(1)))), read<i32>(%[[V_T]]));
// CHECK-NEXT:                                 let %[[V4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[V_SWAPS]]);
// CHECK-NEXT:                                 let %[[V5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[V4]]), const<i32>(1));
// CHECK-NEXT:                                 write<i32>(%[[V_SWAPS]], read<i32>(%[[V5]]));
// CHECK-NEXT:                                 continue %[[V0]];
// CHECK-NEXT:                             }
// CHECK-NEXT:                     }
// CHECK-NEXT:             break %[[V0]];
// CHECK-NEXT:     return read<i32>(%[[V_SWAPS]]);
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @two_edges(%{{[0-9]+}} n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_ODD:[0-9]+]] odd: i32 [storage=automatic] = const<i32>(0);
// CHECK-NEXT:     let %[[V_EVEN:[0-9]+]] even: i32 [storage=automatic] = const<i32>(0);
// CHECK-NEXT:     for %[[V0:[0-9]+]]
// CHECK-NEXT:         init:
// CHECK-NEXT:         condition: omitted
// CHECK-NEXT:         increment: omitted
// CHECK-NEXT:         body:
// CHECK-NEXT:             if le<i32>(read<i32>(%[[V_N:[0-9]+]]), const<i32>(0))
// CHECK-NEXT:                 return add<i32, overflow=ub>(mul<i32, overflow=ub>(read<i32>(%[[V_ODD]]), const<i32>(100)), read<i32>(%[[V_EVEN]]));
// CHECK-NEXT:             if ne<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[V_N]]), const<i32>(2)), const<i32>(0))
// CHECK-NEXT:                 {
// CHECK-NEXT:                     let %[[V1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[V_ODD]]);
// CHECK-NEXT:                     let %[[V2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[V1]]), const<i32>(1));
// CHECK-NEXT:                     write<i32>(%[[V_ODD]], read<i32>(%[[V2]]));
// CHECK-NEXT:                     let %[[V3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[V_N]]);
// CHECK-NEXT:                     let %[[V4:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[V3]]), const<i32>(1));
// CHECK-NEXT:                     write<i32>(%[[V_N]], read<i32>(%[[V4]]));
// CHECK-NEXT:                     continue %[[V0]];
// CHECK-NEXT:                 }
// CHECK-NEXT:             let %[[V5:[0-9]+]]: i32 [synthetic] = read<i32>(%[[V_EVEN]]);
// CHECK-NEXT:             let %[[V6:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[V5]]), const<i32>(1));
// CHECK-NEXT:             write<i32>(%[[V_EVEN]], read<i32>(%[[V6]]));
// CHECK-NEXT:             let %[[V7:[0-9]+]]: i32 [synthetic] = read<i32>(%[[V_N]]);
// CHECK-NEXT:             let %[[V8:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[V7]]), const<i32>(1));
// CHECK-NEXT:             write<i32>(%[[V_N]], read<i32>(%[[V8]]));
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @entered(%{{[0-9]+}} n: i32, %{{[0-9]+}} skip: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_TOTAL:[0-9]+]] total: i32 [storage=automatic] = const<i32>(0);
// CHECK-NEXT:     block %[[V0:[0-9]+]] {
// CHECK-NEXT:         if ne<i32>(read<i32>(%[[V_SKIP:[0-9]+]]), const<i32>(0))
// CHECK-NEXT:             break %[[V0]];
// CHECK-NEXT:         write<i32>(%[[V_TOTAL]], const<i32>(100));
// CHECK-NEXT:     }
// CHECK-NEXT:     do %[[V1:[0-9]+]]
// CHECK-NEXT:         let %[[V2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[V_TOTAL]]);
// CHECK-NEXT:         let %[[V3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[V2]]), read<i32>(%[[V_N:[0-9]+]]));
// CHECK-NEXT:         write<i32>(%[[V_TOTAL]], read<i32>(%[[V3]]));
// CHECK-NEXT:     while {
// CHECK-NEXT:         let %[[V4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[V_N]]);
// CHECK-NEXT:         let %[[V5:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[V4]]), const<i32>(1));
// CHECK-NEXT:         write<i32>(%[[V_N]], read<i32>(%[[V5]]));
// CHECK-NEXT:         yield gt<i32>(read<i32>(%[[V5]]), const<i32>(0));
// CHECK-NEXT:     };
// CHECK-NEXT:     return read<i32>(%[[V_TOTAL]]);
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @scoped(%{{[0-9]+}} n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_SUM:[0-9]+]] sum: i32 [storage=automatic] = const<i32>(0);
// CHECK-NEXT:     do %[[V0:[0-9]+]]
// CHECK-NEXT:         {
// CHECK-NEXT:             let %[[V_SQUARE:[0-9]+]] square: i32 [storage=automatic] = mul<i32, overflow=ub>(read<i32>(%[[V_N:[0-9]+]]), read<i32>(%[[V_N]]));
// CHECK-NEXT:             let %[[V1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[V_SUM]]);
// CHECK-NEXT:             let %[[V2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[V1]]), read<i32>(%[[V_SQUARE]]));
// CHECK-NEXT:             write<i32>(%[[V_SUM]], read<i32>(%[[V2]]));
// CHECK-NEXT:         }
// CHECK-NEXT:     while {
// CHECK-NEXT:         let %[[V3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[V_N]]);
// CHECK-NEXT:         let %[[V4:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[V3]]), const<i32>(1));
// CHECK-NEXT:         write<i32>(%[[V_N]], read<i32>(%[[V4]]));
// CHECK-NEXT:         yield gt<i32>(read<i32>(%[[V4]]), const<i32>(0));
// CHECK-NEXT:     };
// CHECK-NEXT:     return read<i32>(%[[V_SUM]]);
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// CHECK-NEXT:     let %[[V_VALUES:[0-9]+]] values: array<i32, 4> [storage=automatic] [align=16] = aggregate<array<i32, 4>, zero_fill=false>(index0 = const<i32>(4), index1 = const<i32>(1), index2 = const<i32>(3), index3 = const<i32>(2));
// CHECK-NEXT:     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[V0:[0-9]+]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[V1:[0-9]+]])), call<i32, signature=fn(i32) -> i32>(%[[V2:[0-9]+]], const<i32>(4)), call<i32, signature=fn(i32) -> i32>(%[[V2]], const<i32>(1)));
// CHECK-NEXT:     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[V0]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[V3:[0-9]+]])), call<i32, signature=fn(i32) -> i32>(%[[V4:[0-9]+]], const<i32>(3)), call<i32, signature=fn(i32) -> i32>(%[[V4]], const<i32>(0)));
// CHECK-NEXT:     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[V0]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[V5:[0-9]+]])), call<i32, signature=fn(ptr<i32>, i32) -> i32>(%[[V6:[0-9]+]], array_decay<ptr<i32>, length=Some(4)>(%[[V_VALUES]]), const<i32>(4)));
// CHECK-NEXT:     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[V0]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(13)>(%[[V7:[0-9]+]])), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%[[V_VALUES]]), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%[[V_VALUES]]), const<i32>(1)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%[[V_VALUES]]), const<i32>(2)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%[[V_VALUES]]), const<i32>(3)))));
// CHECK-NEXT:     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[V0]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[V8:[0-9]+]])), call<i32, signature=fn(i32) -> i32>(%[[V9:[0-9]+]], const<i32>(7)));
// CHECK-NEXT:     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[V0]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[V10:[0-9]+]])), call<i32, signature=fn(i32, i32) -> i32>(%[[V11:[0-9]+]], const<i32>(3), const<i32>(0)), call<i32, signature=fn(i32, i32) -> i32>(%[[V11]], const<i32>(3), const<i32>(1)));
// CHECK-NEXT:     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[V0]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[V12:[0-9]+]])), call<i32, signature=fn(i32) -> i32>(%[[V13:[0-9]+]], const<i32>(4)));
// CHECK-NEXT:     return const<i32>(0);
// CHECK: {{^}}}
// SLATE-FILECHECK-END
