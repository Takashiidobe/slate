#include <stdio.h>

int take(int);
int take(int value) { return value; }
void undo(int value) { printf("undo %d\n", value); }

int error_ladder(int a, int b) {
    int ret = 0;
    if (take(a) < 0)
        goto out_a;
    if (take(b) < 0)
        goto out_b;
    return 0;
out_b:
    undo(b);
out_a:
    undo(a);
    ret = -1;
    return ret;
}

int crossing(int a, int b) {
    if (a)
        goto first;
    if (b)
        goto second;
    printf("neither\n");
first:
    printf("first\n");
second:
    printf("second\n");
    return a + b;
}

int from_loop(int n) {
    int i;
    for (i = 0; i < n; i++) {
        while (i > 2) {
            if (i == 4)
                goto found;
            i++;
        }
    }
    return -1;
found:
    return i;
}

int from_switch(int k) {
    switch (k) {
    case 0:
        goto done;
    case 1:
        k += 10;
        break;
    default:
        k += 20;
    }
    k *= 2;
done:
    return k;
}

int unused_label(int a) {
unused:
    return a + 1;
}

int declared_between(int a) {
    if (a)
        goto skip;
    int b = a * 3;
    printf("b %d\n", b);
skip:
    return a;
}

int used_after(int a) {
    if (a)
        goto skip;
    int b;
    b = a * 3;
skip:
    b = a;
    return b;
}

void release(int *p) { printf("release %d\n", *p); }

int with_cleanup(int a) {
    {
        int guard __attribute__((cleanup(release))) = a;
        if (a > 1)
            goto out;
        printf("inside\n");
    }
    printf("after\n");
out:
    return a;
}

int backward(int n) {
    int total = 0;
again:
    total += n;
    if (--n > 0)
        goto again;
    return total;
}

int main(void) {
    printf("%d\n", error_ladder(1, 2));
    printf("%d\n", error_ladder(1, -2));
    printf("%d\n", error_ladder(-1, 2));
    printf("%d\n", crossing(0, 0));
    printf("%d\n", crossing(1, 0));
    printf("%d\n", crossing(0, 1));
    printf("%d\n", from_loop(3));
    printf("%d\n", from_loop(8));
    printf("%d %d %d\n", from_switch(0), from_switch(1), from_switch(2));
    printf("%d\n", unused_label(4));
    printf("%d %d\n", declared_between(0), declared_between(2));
    printf("%d %d\n", used_after(0), used_after(5));
    printf("%d %d\n", with_cleanup(1), with_cleanup(2));
    printf("%d\n", backward(4));
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @take(%{{[0-9]+}} value: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     return read<i32>(%[[V_VALUE:[0-9]+]]);
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @undo(%{{[0-9]+}} value: i32) -> void [linkage=external] [fallthrough=ret_void] {
// CHECK-NEXT:     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[V0:[0-9]+]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%[[V1:[0-9]+]])), read<i32>(%[[V_VALUE:[0-9]+]]));
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @error_ladder(%{{[0-9]+}} a: i32, %{{[0-9]+}} b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_RET:[0-9]+]] ret: i32 [storage=automatic] = const<i32>(0);
// CHECK-NEXT:     block %[[V0:[0-9]+]] {
// CHECK-NEXT:         if lt<i32>(call<i32, signature=fn(i32) -> i32>(%[[V1:[0-9]+]], read<i32>(%[[V_A:[0-9]+]])), const<i32>(0))
// CHECK-NEXT:             break %[[V0]];
// CHECK-NEXT:         block %[[V2:[0-9]+]] {
// CHECK-NEXT:             if lt<i32>(call<i32, signature=fn(i32) -> i32>(%[[V1]], read<i32>(%[[V_B:[0-9]+]])), const<i32>(0))
// CHECK-NEXT:                 break %[[V2]];
// CHECK-NEXT:             return const<i32>(0);
// CHECK-NEXT:         }
// CHECK-NEXT:         call<void, signature=fn(i32) -> void>(%[[V3:[0-9]+]], read<i32>(%[[V_B]]));
// CHECK-NEXT:     }
// CHECK-NEXT:     call<void, signature=fn(i32) -> void>(%[[V3]], read<i32>(%[[V_A]]));
// CHECK-NEXT:     write<i32>(%[[V_RET]], neg<i32, overflow=ub>(const<i32>(1)));
// CHECK-NEXT:     return read<i32>(%[[V_RET]]);
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @crossing(%{{[0-9]+}} a: i32, %{{[0-9]+}} b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     block %[[V0:[0-9]+]] {
// CHECK-NEXT:         block %[[V1:[0-9]+]] {
// CHECK-NEXT:             if ne<i32>(read<i32>(%[[V_A:[0-9]+]]), const<i32>(0))
// CHECK-NEXT:                 break %[[V1]];
// CHECK-NEXT:             if ne<i32>(read<i32>(%[[V_B:[0-9]+]]), const<i32>(0))
// CHECK-NEXT:                 break %[[V0]];
// CHECK-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[V2:[0-9]+]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%[[V3:[0-9]+]])));
// CHECK-NEXT:         }
// CHECK-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[V2]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[V4:[0-9]+]])));
// CHECK-NEXT:     }
// CHECK-NEXT:     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[V2]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[V5:[0-9]+]])));
// CHECK-NEXT:     return add<i32, overflow=ub>(read<i32>(%[[V_A]]), read<i32>(%[[V_B]]));
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @from_loop(%{{[0-9]+}} n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_I:[0-9]+]] i: i32 [storage=automatic];
// CHECK-NEXT:     block %[[V0:[0-9]+]] {
// CHECK-NEXT:         for %[[V1:[0-9]+]]
// CHECK-NEXT:             init:
// CHECK-NEXT:                 write<i32>(%[[V_I]], const<i32>(0));
// CHECK-NEXT:             condition: lt<i32>(read<i32>(%[[V_I]]), read<i32>(%[[V_N:[0-9]+]]))
// CHECK-NEXT:             increment: {
// CHECK-NEXT:                 let %[[V2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[V_I]]);
// CHECK-NEXT:                 let %[[V3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[V2]]), const<i32>(1));
// CHECK-NEXT:                 write<i32>(%[[V_I]], read<i32>(%[[V3]]));
// CHECK-NEXT:                 yield void;
// CHECK-NEXT:             }
// CHECK-NEXT:             body:
// CHECK-NEXT:                 {
// CHECK-NEXT:                     while %[[V4:[0-9]+]] gt<i32>(read<i32>(%[[V_I]]), const<i32>(2))
// CHECK-NEXT:                         {
// CHECK-NEXT:                             if eq<i32>(read<i32>(%[[V_I]]), const<i32>(4))
// CHECK-NEXT:                                 break %[[V0]];
// CHECK-NEXT:                             let %[[V5:[0-9]+]]: i32 [synthetic] = read<i32>(%[[V_I]]);
// CHECK-NEXT:                             let %[[V6:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[V5]]), const<i32>(1));
// CHECK-NEXT:                             write<i32>(%[[V_I]], read<i32>(%[[V6]]));
// CHECK-NEXT:                         }
// CHECK-NEXT:                 }
// CHECK-NEXT:         return neg<i32, overflow=ub>(const<i32>(1));
// CHECK-NEXT:     }
// CHECK-NEXT:     return read<i32>(%[[V_I]]);
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @from_switch(%{{[0-9]+}} k: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     block %[[V0:[0-9]+]] {
// CHECK-NEXT:         switch %[[V1:[0-9]+]] read<i32>(%[[V_K:[0-9]+]])
// CHECK-NEXT:             {
// CHECK-NEXT:                 case %[[V1]] const<i32>(0):
// CHECK-NEXT:                     break %[[V0]];
// CHECK-NEXT:                 case %[[V1]] const<i32>(1):
// CHECK-NEXT:                     let %[[V2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[V_K]]);
// CHECK-NEXT:                     let %[[V3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[V2]]), const<i32>(10));
// CHECK-NEXT:                     write<i32>(%[[V_K]], read<i32>(%[[V3]]));
// CHECK-NEXT:                 break %[[V1]];
// CHECK-NEXT:                 default %[[V1]]:
// CHECK-NEXT:                     let %[[V4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[V_K]]);
// CHECK-NEXT:                     let %[[V5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[V4]]), const<i32>(20));
// CHECK-NEXT:                     write<i32>(%[[V_K]], read<i32>(%[[V5]]));
// CHECK-NEXT:             }
// CHECK-NEXT:         let %[[V6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[V_K]]);
// CHECK-NEXT:         let %[[V7:[0-9]+]]: i32 [synthetic] = mul<i32, overflow=ub>(read<i32>(%[[V6]]), const<i32>(2));
// CHECK-NEXT:         write<i32>(%[[V_K]], read<i32>(%[[V7]]));
// CHECK-NEXT:     }
// CHECK-NEXT:     return read<i32>(%[[V_K]]);
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @unused_label(%{{[0-9]+}} a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     return add<i32, overflow=ub>(read<i32>(%[[V_A:[0-9]+]]), const<i32>(1));
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @declared_between(%{{[0-9]+}} a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     block %[[V0:[0-9]+]] {
// CHECK-NEXT:         if ne<i32>(read<i32>(%[[V_A:[0-9]+]]), const<i32>(0))
// CHECK-NEXT:             break %[[V0]];
// CHECK-NEXT:         let %[[V_B:[0-9]+]] b: i32 [storage=automatic] = mul<i32, overflow=ub>(read<i32>(%[[V_A]]), const<i32>(3));
// CHECK-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[V1:[0-9]+]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[V2:[0-9]+]])), read<i32>(%[[V_B]]));
// CHECK-NEXT:     }
// CHECK-NEXT:     return read<i32>(%[[V_A]]);
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @used_after(%{{[0-9]+}} a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     if ne<i32>(read<i32>(%[[V_A:[0-9]+]]), const<i32>(0))
// CHECK-NEXT:         goto %[[V_SKIP:[0-9]+]];
// CHECK-NEXT:     let %[[V_B:[0-9]+]] b: i32 [storage=automatic];
// CHECK-NEXT:     write<i32>(%[[V_B]], mul<i32, overflow=ub>(read<i32>(%[[V_A]]), const<i32>(3)));
// CHECK-NEXT:     label %[[V_SKIP]] skip:
// CHECK-NEXT:         write<i32>(%[[V_B]], read<i32>(%[[V_A]]));
// CHECK-NEXT:     return read<i32>(%[[V_B]]);
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @release(%{{[0-9]+}} p: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// CHECK-NEXT:     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[V0:[0-9]+]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(12)>(%[[V1:[0-9]+]])), read<i32>(deref(read<ptr<i32>>(%[[V_P:[0-9]+]]))));
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @with_cleanup(%{{[0-9]+}} a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     block %[[V0:[0-9]+]] {
// CHECK-NEXT:         {
// CHECK-NEXT:             let %[[V_GUARD:[0-9]+]] guard: i32 [storage=automatic] [cleanup=%[[V1:[0-9]+]]] = read<i32>(%[[V_A:[0-9]+]]);
// CHECK-NEXT:             if gt<i32>(read<i32>(%[[V_A]]), const<i32>(1))
// CHECK-NEXT:                 break %[[V0]];
// CHECK-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[V2:[0-9]+]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[V3:[0-9]+]])));
// CHECK-NEXT:         }
// CHECK-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[V2]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[V4:[0-9]+]])));
// CHECK-NEXT:     }
// CHECK-NEXT:     return read<i32>(%[[V_A]]);
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @backward(%{{[0-9]+}} n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
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
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// CHECK-NEXT:     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[V0:[0-9]+]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[V1:[0-9]+]])), call<i32, signature=fn(i32, i32) -> i32>(%[[V2:[0-9]+]], const<i32>(1), const<i32>(2)));
// CHECK-NEXT:     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[V0]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[V3:[0-9]+]])), call<i32, signature=fn(i32, i32) -> i32>(%[[V2]], const<i32>(1), neg<i32, overflow=ub>(const<i32>(2))));
// CHECK-NEXT:     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[V0]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[V4:[0-9]+]])), call<i32, signature=fn(i32, i32) -> i32>(%[[V2]], neg<i32, overflow=ub>(const<i32>(1)), const<i32>(2)));
// CHECK-NEXT:     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[V0]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[V5:[0-9]+]])), call<i32, signature=fn(i32, i32) -> i32>(%[[V6:[0-9]+]], const<i32>(0), const<i32>(0)));
// CHECK-NEXT:     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[V0]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[V7:[0-9]+]])), call<i32, signature=fn(i32, i32) -> i32>(%[[V6]], const<i32>(1), const<i32>(0)));
// CHECK-NEXT:     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[V0]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[V8:[0-9]+]])), call<i32, signature=fn(i32, i32) -> i32>(%[[V6]], const<i32>(0), const<i32>(1)));
// CHECK-NEXT:     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[V0]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[V9:[0-9]+]])), call<i32, signature=fn(i32) -> i32>(%[[V10:[0-9]+]], const<i32>(3)));
// CHECK-NEXT:     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[V0]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[V11:[0-9]+]])), call<i32, signature=fn(i32) -> i32>(%[[V10]], const<i32>(8)));
// CHECK-NEXT:     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[V0]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[V12:[0-9]+]])), call<i32, signature=fn(i32) -> i32>(%[[V13:[0-9]+]], const<i32>(0)), call<i32, signature=fn(i32) -> i32>(%[[V13]], const<i32>(1)), call<i32, signature=fn(i32) -> i32>(%[[V13]], const<i32>(2)));
// CHECK-NEXT:     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[V0]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[V14:[0-9]+]])), call<i32, signature=fn(i32) -> i32>(%[[V15:[0-9]+]], const<i32>(4)));
// CHECK-NEXT:     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[V0]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[V16:[0-9]+]])), call<i32, signature=fn(i32) -> i32>(%[[V17:[0-9]+]], const<i32>(0)), call<i32, signature=fn(i32) -> i32>(%[[V17]], const<i32>(2)));
// CHECK-NEXT:     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[V0]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[V18:[0-9]+]])), call<i32, signature=fn(i32) -> i32>(%[[V19:[0-9]+]], const<i32>(0)), call<i32, signature=fn(i32) -> i32>(%[[V19]], const<i32>(5)));
// CHECK-NEXT:     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[V0]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[V20:[0-9]+]])), call<i32, signature=fn(i32) -> i32>(%[[V21:[0-9]+]], const<i32>(1)), call<i32, signature=fn(i32) -> i32>(%[[V21]], const<i32>(2)));
// CHECK-NEXT:     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[V0]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[V22:[0-9]+]])), call<i32, signature=fn(i32) -> i32>(%[[V23:[0-9]+]], const<i32>(4)));
// CHECK-NEXT:     return const<i32>(0);
// CHECK: {{^}}}
// SLATE-FILECHECK-END
