#include <stdio.h>

void release(int *p) { printf("release %d\n", *p); }

int into_block(int skip) {
    int result = 1;
    if (skip)
        goto entered;
    {
        int value = result * 2;
    entered:
        value = 3;
        result += value;
    }
    return result;
}

int into_then(int a, int b) {
    int total = 0;
    if (b)
        goto inside;
    if (a > 2) {
        total += 10;
    inside:
        total += 1;
    } else {
        total += 100;
    }
    return total;
}

int into_else_from_then(int x) {
    int hits = 0;
    if (x == 1)
        goto other;
    else {
    other:
        hits += 1;
    }
    return hits;
}

int into_returning_arm(int a, int b) {
    if (b)
        goto bail;
    if (a) {
        printf("a\n");
    bail:
        return -1;
    }
    return a;
}

int into_scoped_tail(int a) {
    if (a)
        goto entered;
    {
        printf("prefix\n");
    entered:;
        int guard __attribute__((cleanup(release))) = a;
        printf("tail\n");
    }
    printf("after\n");
    return a;
}

int forwarded(int a) {
    if (a)
        goto first;
    a += 5;
    goto done;
first:
second:
    goto done;
done:
    return a;
}

int main(void) {
    printf("%d %d\n", into_block(0), into_block(1));
    printf("%d %d %d\n", into_then(3, 0), into_then(1, 0), into_then(0, 1));
    printf("%d %d\n", into_else_from_then(1), into_else_from_then(2));
    printf("%d %d %d\n", into_returning_arm(1, 0), into_returning_arm(0, 0), into_returning_arm(0, 1));
    printf("%d %d\n", into_scoped_tail(0), into_scoped_tail(4));
    printf("%d %d\n", forwarded(0), forwarded(1));
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @release(%{{[0-9]+}} p: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// CHECK-NEXT:     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[V0:[0-9]+]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(12)>(%[[V1:[0-9]+]])), read<i32>(deref(read<ptr<i32>>(%[[V_P:[0-9]+]]))));
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @into_block(%{{[0-9]+}} skip: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_RESULT:[0-9]+]] result: i32 [storage=automatic] = const<i32>(1);
// CHECK-NEXT:     let %[[V_VALUE:[0-9]+]] value: i32 [storage=automatic];
// CHECK-NEXT:     block %[[V0:[0-9]+]] {
// CHECK-NEXT:         if ne<i32>(read<i32>(%[[V_SKIP:[0-9]+]]), const<i32>(0))
// CHECK-NEXT:             break %[[V0]];
// CHECK-NEXT:         {
// CHECK-NEXT:             write<i32>(%[[V_VALUE]], mul<i32, overflow=ub>(read<i32>(%[[V_RESULT]]), const<i32>(2)));
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     write<i32>(%[[V_VALUE]], const<i32>(3));
// CHECK-NEXT:     let %[[V1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[V_RESULT]]);
// CHECK-NEXT:     let %[[V2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[V1]]), read<i32>(%[[V_VALUE]]));
// CHECK-NEXT:     write<i32>(%[[V_RESULT]], read<i32>(%[[V2]]));
// CHECK-NEXT:     return read<i32>(%[[V_RESULT]]);
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @into_then(%{{[0-9]+}} a: i32, %{{[0-9]+}} b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_TOTAL:[0-9]+]] total: i32 [storage=automatic] = const<i32>(0);
// CHECK-NEXT:     block %[[V0:[0-9]+]] {
// CHECK-NEXT:         block %[[V1:[0-9]+]] {
// CHECK-NEXT:             if ne<i32>(read<i32>(%[[V_B:[0-9]+]]), const<i32>(0))
// CHECK-NEXT:                 break %[[V1]];
// CHECK-NEXT:             if gt<i32>(read<i32>(%[[V_A:[0-9]+]]), const<i32>(2))
// CHECK-NEXT:                 {
// CHECK-NEXT:                     let %[[V2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[V_TOTAL]]);
// CHECK-NEXT:                     let %[[V3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[V2]]), const<i32>(10));
// CHECK-NEXT:                     write<i32>(%[[V_TOTAL]], read<i32>(%[[V3]]));
// CHECK-NEXT:                 }
// CHECK-NEXT:             else
// CHECK-NEXT:                 {
// CHECK-NEXT:                     let %[[V4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[V_TOTAL]]);
// CHECK-NEXT:                     let %[[V5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[V4]]), const<i32>(100));
// CHECK-NEXT:                     write<i32>(%[[V_TOTAL]], read<i32>(%[[V5]]));
// CHECK-NEXT:                 }
// CHECK-NEXT:                 break %[[V0]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let %[[V6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[V_TOTAL]]);
// CHECK-NEXT:         let %[[V7:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[V6]]), const<i32>(1));
// CHECK-NEXT:         write<i32>(%[[V_TOTAL]], read<i32>(%[[V7]]));
// CHECK-NEXT:     }
// CHECK-NEXT:     return read<i32>(%[[V_TOTAL]]);
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @into_else_from_then(%{{[0-9]+}} x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_HITS:[0-9]+]] hits: i32 [storage=automatic] = const<i32>(0);
// CHECK-NEXT:     block %[[V0:[0-9]+]] {
// CHECK-NEXT:         if eq<i32>(read<i32>(%[[V_X:[0-9]+]]), const<i32>(1))
// CHECK-NEXT:             break %[[V0]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let %[[V1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[V_HITS]]);
// CHECK-NEXT:     let %[[V2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[V1]]), const<i32>(1));
// CHECK-NEXT:     write<i32>(%[[V_HITS]], read<i32>(%[[V2]]));
// CHECK-NEXT:     return read<i32>(%[[V_HITS]]);
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @into_returning_arm(%{{[0-9]+}} a: i32, %{{[0-9]+}} b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     block %[[V0:[0-9]+]] {
// CHECK-NEXT:         block %[[V1:[0-9]+]] {
// CHECK-NEXT:             if ne<i32>(read<i32>(%[[V_B:[0-9]+]]), const<i32>(0))
// CHECK-NEXT:                 break %[[V1]];
// CHECK-NEXT:             if ne<i32>(read<i32>(%[[V_A:[0-9]+]]), const<i32>(0))
// CHECK-NEXT:                 {
// CHECK-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[V2:[0-9]+]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%[[V3:[0-9]+]])));
// CHECK-NEXT:                 }
// CHECK-NEXT:             else
// CHECK-NEXT:                 break %[[V0]];
// CHECK-NEXT:         }
// CHECK-NEXT:         return neg<i32, overflow=ub>(const<i32>(1));
// CHECK-NEXT:     }
// CHECK-NEXT:     return read<i32>(%[[V_A]]);
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @into_scoped_tail(%{{[0-9]+}} a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     block %[[V0:[0-9]+]] {
// CHECK-NEXT:         if ne<i32>(read<i32>(%[[V_A:[0-9]+]]), const<i32>(0))
// CHECK-NEXT:             break %[[V0]];
// CHECK-NEXT:         {
// CHECK-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[V1:[0-9]+]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[V2:[0-9]+]])));
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     {
// CHECK-NEXT:         ;
// CHECK-NEXT:         let %[[V_GUARD:[0-9]+]] guard: i32 [storage=automatic] [cleanup=%[[V3:[0-9]+]]] = read<i32>(%[[V_A]]);
// CHECK-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[V1]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[V4:[0-9]+]])));
// CHECK-NEXT:     }
// CHECK-NEXT:     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[V1]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[V5:[0-9]+]])));
// CHECK-NEXT:     return read<i32>(%[[V_A]]);
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @forwarded(%{{[0-9]+}} a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     block %[[V0:[0-9]+]] {
// CHECK-NEXT:         if ne<i32>(read<i32>(%[[V_A:[0-9]+]]), const<i32>(0))
// CHECK-NEXT:             break %[[V0]];
// CHECK-NEXT:         let %[[V1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[V_A]]);
// CHECK-NEXT:         let %[[V2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[V1]]), const<i32>(5));
// CHECK-NEXT:         write<i32>(%[[V_A]], read<i32>(%[[V2]]));
// CHECK-NEXT:         break %[[V0]];
// CHECK-NEXT:         break %[[V0]];
// CHECK-NEXT:     }
// CHECK-NEXT:     return read<i32>(%[[V_A]]);
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// CHECK-NEXT:     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[V0:[0-9]+]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[V1:[0-9]+]])), call<i32, signature=fn(i32) -> i32>(%[[V2:[0-9]+]], const<i32>(0)), call<i32, signature=fn(i32) -> i32>(%[[V2]], const<i32>(1)));
// CHECK-NEXT:     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[V0]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[V3:[0-9]+]])), call<i32, signature=fn(i32, i32) -> i32>(%[[V4:[0-9]+]], const<i32>(3), const<i32>(0)), call<i32, signature=fn(i32, i32) -> i32>(%[[V4]], const<i32>(1), const<i32>(0)), call<i32, signature=fn(i32, i32) -> i32>(%[[V4]], const<i32>(0), const<i32>(1)));
// CHECK-NEXT:     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[V0]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[V5:[0-9]+]])), call<i32, signature=fn(i32) -> i32>(%[[V6:[0-9]+]], const<i32>(1)), call<i32, signature=fn(i32) -> i32>(%[[V6]], const<i32>(2)));
// CHECK-NEXT:     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[V0]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[V7:[0-9]+]])), call<i32, signature=fn(i32, i32) -> i32>(%[[V8:[0-9]+]], const<i32>(1), const<i32>(0)), call<i32, signature=fn(i32, i32) -> i32>(%[[V8]], const<i32>(0), const<i32>(0)), call<i32, signature=fn(i32, i32) -> i32>(%[[V8]], const<i32>(0), const<i32>(1)));
// CHECK-NEXT:     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[V0]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[V9:[0-9]+]])), call<i32, signature=fn(i32) -> i32>(%[[V10:[0-9]+]], const<i32>(0)), call<i32, signature=fn(i32) -> i32>(%[[V10]], const<i32>(4)));
// CHECK-NEXT:     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[V0]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[V11:[0-9]+]])), call<i32, signature=fn(i32) -> i32>(%[[V12:[0-9]+]], const<i32>(0)), call<i32, signature=fn(i32) -> i32>(%[[V12]], const<i32>(1)));
// CHECK-NEXT:     return const<i32>(0);
// CHECK: {{^}}}
// SLATE-FILECHECK-END
