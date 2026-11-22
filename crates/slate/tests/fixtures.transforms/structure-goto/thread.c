#include <stdio.h>

static int released;

static void release(int *p) { released += *p; }

int from_increment(int n) {
    int total = 0;
    for (int i = 0; i < n; ({ goto stop; }))
        if (0) {
        stop:
            break;
        } else {
            total += n;
            i++;
        }
    return total;
}

int behind_cleanup(int n) {
    int total = 0;
    for (int i = 0; i < n; i++) {
        int held __attribute__((cleanup(release))) = i;
        if (i == 2)
            goto stop;
        total += held;
        if (0) {
        stop:
            break;
        }
    }
    return total;
}

int main(void) {
    printf("%d %d\n", from_increment(3), from_increment(0));
    printf("%d %d\n", behind_cleanup(5), released);
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @release(%{{[0-9]+}} p: ptr<i32>) -> void [linkage=internal] [fallthrough=ret_void] {
// CHECK-NEXT:     let %[[V0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[V1:[0-9]+]]);
// CHECK-NEXT:     let %[[V2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[V0]]), read<i32>(deref(read<ptr<i32>>(%[[V_P:[0-9]+]]))));
// CHECK-NEXT:     write<i32>(%[[V1]], read<i32>(%[[V2]]));
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @from_increment(%{{[0-9]+}} n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_TOTAL:[0-9]+]] total: i32 [storage=automatic] = const<i32>(0);
// CHECK-NEXT:     for %[[V0:[0-9]+]]
// CHECK-NEXT:         init:
// CHECK-NEXT:             let %[[V_I:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// CHECK-NEXT:         condition: lt<i32>(read<i32>(%[[V_I]]), read<i32>(%[[V_N:[0-9]+]]))
// CHECK-NEXT:         increment: {
// CHECK-NEXT:             {
// CHECK-NEXT:                 break %[[V0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             yield void;
// CHECK-NEXT:         }
// CHECK-NEXT:         body:
// CHECK-NEXT:             if ne<i32>(const<i32>(0), const<i32>(0))
// CHECK-NEXT:                 {
// CHECK-NEXT:                     break %[[V0]];
// CHECK-NEXT:                 }
// CHECK-NEXT:             else
// CHECK-NEXT:                 {
// CHECK-NEXT:                     let %[[V1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[V_TOTAL]]);
// CHECK-NEXT:                     let %[[V2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[V1]]), read<i32>(%[[V_N]]));
// CHECK-NEXT:                     write<i32>(%[[V_TOTAL]], read<i32>(%[[V2]]));
// CHECK-NEXT:                     let %[[V3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[V_I]]);
// CHECK-NEXT:                     let %[[V4:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[V3]]), const<i32>(1));
// CHECK-NEXT:                     write<i32>(%[[V_I]], read<i32>(%[[V4]]));
// CHECK-NEXT:                 }
// CHECK-NEXT:     return read<i32>(%[[V_TOTAL]]);
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @behind_cleanup(%{{[0-9]+}} n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_TOTAL:[0-9]+]] total: i32 [storage=automatic] = const<i32>(0);
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
// CHECK-NEXT:             {
// CHECK-NEXT:                 let %[[V_HELD:[0-9]+]] held: i32 [storage=automatic] [cleanup=%[[V3:[0-9]+]]] = read<i32>(%[[V_I]]);
// CHECK-NEXT:                 if eq<i32>(read<i32>(%[[V_I]]), const<i32>(2))
// CHECK-NEXT:                     goto %[[V_STOP:[0-9]+]];
// CHECK-NEXT:                 let %[[V4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[V_TOTAL]]);
// CHECK-NEXT:                 let %[[V5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[V4]]), read<i32>(%[[V_HELD]]));
// CHECK-NEXT:                 write<i32>(%[[V_TOTAL]], read<i32>(%[[V5]]));
// CHECK-NEXT:                 if ne<i32>(const<i32>(0), const<i32>(0))
// CHECK-NEXT:                     {
// CHECK-NEXT:                         label %[[V_STOP]] stop:
// CHECK-NEXT:                             break %[[V0]];
// CHECK-NEXT:                     }
// CHECK-NEXT:             }
// CHECK-NEXT:     return read<i32>(%[[V_TOTAL]]);
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// CHECK-NEXT:     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[V0:[0-9]+]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[V1:[0-9]+]])), call<i32, signature=fn(i32) -> i32>(%[[V2:[0-9]+]], const<i32>(3)), call<i32, signature=fn(i32) -> i32>(%[[V2]], const<i32>(0)));
// CHECK-NEXT:     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[V0]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[V3:[0-9]+]])), call<i32, signature=fn(i32) -> i32>(%[[V4:[0-9]+]], const<i32>(5)), read<i32>(%[[V5:[0-9]+]]));
// CHECK-NEXT:     return const<i32>(0);
// CHECK: {{^}}}
// SLATE-FILECHECK-END
