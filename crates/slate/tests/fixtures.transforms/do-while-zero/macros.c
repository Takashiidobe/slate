#include <stdio.h>

#define SWAP(a, b)                                                             \
    do {                                                                       \
        int t = (a);                                                           \
        (a) = (b);                                                             \
        (b) = t;                                                               \
    } while (0)

#define NEED(n)                                                                \
    do {                                                                       \
        if (have < (n))                                                        \
            goto leave;                                                        \
        have -= (n);                                                           \
    } while (0)

#define CLAMP(x)                                                               \
    do {                                                                       \
        if ((x) < 0) {                                                         \
            (x) = 0;                                                           \
            break;                                                             \
        }                                                                      \
        if ((x) > 9)                                                           \
            (x) = 9;                                                           \
    } while (0)

int swapped(int a, int b) {
    SWAP(a, b);
    return a * 10 + b;
}

int need(int have) {
    int out = 0;
    NEED(2);
    out += 1;
    NEED(3);
    out += 10;
leave:
    return out * 100 + have;
}

int clamp(int x) {
    CLAMP(x);
    return x;
}

int skip(int x) {
    do {
        if (x > 5)
            continue;
        x += 100;
    } while (0);
    return x;
}

int nested(int x) {
    do {
        do {
            if (x == 1)
                break;
            x += 2;
        } while (0);
        x *= 3;
    } while (0);
    return x;
}

int repeat(int x) {
    int n = 0;
    do {
        n++;
    } while (x-- > 0);
    return n;
}

int main(void) {
    printf("%d\n", swapped(1, 2));
    printf("%d %d %d\n", need(1), need(3), need(9));
    printf("%d %d %d\n", clamp(-4), clamp(4), clamp(40));
    printf("%d %d\n", skip(3), skip(7));
    printf("%d %d\n", nested(1), nested(2));
    printf("%d\n", repeat(3));
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @swapped(%{{[0-9]+}} a: i32, %{{[0-9]+}} b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     macro_body #0 {
// CHECK-NEXT:         {
// CHECK-NEXT:             {
// CHECK-NEXT:                 let %[[V_T:[0-9]+]] t: i32 [storage=automatic] = read<i32>(%[[V_A:[0-9]+]]);
// CHECK-NEXT:                 write<i32>(%[[V_A]], read<i32>(%[[V_B:[0-9]+]]));
// CHECK-NEXT:                 write<i32>(%[[V_B]], read<i32>(%[[V_T]]));
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     return add<i32, overflow=ub>(mul<i32, overflow=ub>(read<i32>(%[[V_A]]), const<i32>(10)), read<i32>(%[[V_B]]));
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @need(%{{[0-9]+}} have: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_OUT:[0-9]+]] out: i32 [storage=automatic] = const<i32>(0);
// CHECK-NEXT:     {
// CHECK-NEXT:         {
// CHECK-NEXT:             if lt<i32>(read<i32>(%[[V_HAVE:[0-9]+]]), const<i32>(2))
// CHECK-NEXT:                 goto %[[V_LEAVE:[0-9]+]];
// CHECK-NEXT:             let %[[V0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[V_HAVE]]);
// CHECK-NEXT:             let %[[V1:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[V0]]), const<i32>(2));
// CHECK-NEXT:             write<i32>(%[[V_HAVE]], read<i32>(%[[V1]]));
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let %[[V2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[V_OUT]]);
// CHECK-NEXT:     let %[[V3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[V2]]), const<i32>(1));
// CHECK-NEXT:     write<i32>(%[[V_OUT]], read<i32>(%[[V3]]));
// CHECK-NEXT:     {
// CHECK-NEXT:         {
// CHECK-NEXT:             if lt<i32>(read<i32>(%[[V_HAVE]]), const<i32>(3))
// CHECK-NEXT:                 goto %[[V_LEAVE]];
// CHECK-NEXT:             let %[[V4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[V_HAVE]]);
// CHECK-NEXT:             let %[[V5:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[V4]]), const<i32>(3));
// CHECK-NEXT:             write<i32>(%[[V_HAVE]], read<i32>(%[[V5]]));
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let %[[V6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[V_OUT]]);
// CHECK-NEXT:     let %[[V7:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[V6]]), const<i32>(10));
// CHECK-NEXT:     write<i32>(%[[V_OUT]], read<i32>(%[[V7]]));
// CHECK-NEXT:     label %[[V_LEAVE]] leave:
// CHECK-NEXT:         return add<i32, overflow=ub>(mul<i32, overflow=ub>(read<i32>(%[[V_OUT]]), const<i32>(100)), read<i32>(%[[V_HAVE]]));
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @clamp(%{{[0-9]+}} x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     macro_body #3 {
// CHECK-NEXT:         block %[[V0:[0-9]+]] {
// CHECK-NEXT:             {
// CHECK-NEXT:                 if lt<i32>(read<i32>(%[[V_X:[0-9]+]]), const<i32>(0))
// CHECK-NEXT:                     {
// CHECK-NEXT:                         write<i32>(%[[V_X]], const<i32>(0));
// CHECK-NEXT:                         break %[[V0]];
// CHECK-NEXT:                     }
// CHECK-NEXT:                 if gt<i32>(read<i32>(%[[V_X]]), const<i32>(9))
// CHECK-NEXT:                     write<i32>(%[[V_X]], const<i32>(9));
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     return read<i32>(%[[V_X]]);
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @skip(%{{[0-9]+}} x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     block %[[V0:[0-9]+]] {
// CHECK-NEXT:         {
// CHECK-NEXT:             if gt<i32>(read<i32>(%[[V_X:[0-9]+]]), const<i32>(5))
// CHECK-NEXT:                 break %[[V0]];
// CHECK-NEXT:             let %[[V1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[V_X]]);
// CHECK-NEXT:             let %[[V2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[V1]]), const<i32>(100));
// CHECK-NEXT:             write<i32>(%[[V_X]], read<i32>(%[[V2]]));
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     return read<i32>(%[[V_X]]);
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @nested(%{{[0-9]+}} x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     {
// CHECK-NEXT:         {
// CHECK-NEXT:             block %[[V0:[0-9]+]] {
// CHECK-NEXT:                 {
// CHECK-NEXT:                     if eq<i32>(read<i32>(%[[V_X:[0-9]+]]), const<i32>(1))
// CHECK-NEXT:                         break %[[V0]];
// CHECK-NEXT:                     let %[[V1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[V_X]]);
// CHECK-NEXT:                     let %[[V2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[V1]]), const<i32>(2));
// CHECK-NEXT:                     write<i32>(%[[V_X]], read<i32>(%[[V2]]));
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             let %[[V3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[V_X]]);
// CHECK-NEXT:             let %[[V4:[0-9]+]]: i32 [synthetic] = mul<i32, overflow=ub>(read<i32>(%[[V3]]), const<i32>(3));
// CHECK-NEXT:             write<i32>(%[[V_X]], read<i32>(%[[V4]]));
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     return read<i32>(%[[V_X]]);
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @repeat(%{{[0-9]+}} x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_N:[0-9]+]] n: i32 [storage=automatic] = const<i32>(0);
// CHECK-NEXT:     do %[[V0:[0-9]+]]
// CHECK-NEXT:         {
// CHECK-NEXT:             let %[[V1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[V_N]]);
// CHECK-NEXT:             let %[[V2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[V1]]), const<i32>(1));
// CHECK-NEXT:             write<i32>(%[[V_N]], read<i32>(%[[V2]]));
// CHECK-NEXT:         }
// CHECK-NEXT:     while {
// CHECK-NEXT:         let %[[V3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[V_X:[0-9]+]]);
// CHECK-NEXT:         let %[[V4:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[V3]]), const<i32>(1));
// CHECK-NEXT:         write<i32>(%[[V_X]], read<i32>(%[[V4]]));
// CHECK-NEXT:         yield gt<i32>(read<i32>(%[[V3]]), const<i32>(0));
// CHECK-NEXT:     };
// CHECK-NEXT:     return read<i32>(%[[V_N]]);
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// CHECK-NEXT:     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[V0:[0-9]+]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[V1:[0-9]+]])), call<i32, signature=fn(i32, i32) -> i32>(%[[V2:[0-9]+]], const<i32>(1), const<i32>(2)));
// CHECK-NEXT:     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[V0]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[V3:[0-9]+]])), call<i32, signature=fn(i32) -> i32>(%[[V4:[0-9]+]], const<i32>(1)), call<i32, signature=fn(i32) -> i32>(%[[V4]], const<i32>(3)), call<i32, signature=fn(i32) -> i32>(%[[V4]], const<i32>(9)));
// CHECK-NEXT:     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[V0]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[V5:[0-9]+]])), call<i32, signature=fn(i32) -> i32>(%[[V6:[0-9]+]], neg<i32, overflow=ub>(const<i32>(4))), call<i32, signature=fn(i32) -> i32>(%[[V6]], const<i32>(4)), call<i32, signature=fn(i32) -> i32>(%[[V6]], const<i32>(40)));
// CHECK-NEXT:     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[V0]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[V7:[0-9]+]])), call<i32, signature=fn(i32) -> i32>(%[[V8:[0-9]+]], const<i32>(3)), call<i32, signature=fn(i32) -> i32>(%[[V8]], const<i32>(7)));
// CHECK-NEXT:     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[V0]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[V9:[0-9]+]])), call<i32, signature=fn(i32) -> i32>(%[[V10:[0-9]+]], const<i32>(1)), call<i32, signature=fn(i32) -> i32>(%[[V10]], const<i32>(2)));
// CHECK-NEXT:     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[V0]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[V11:[0-9]+]])), call<i32, signature=fn(i32) -> i32>(%[[V12:[0-9]+]], const<i32>(3)));
// CHECK-NEXT:     return const<i32>(0);
// CHECK: {{^}}}
// SLATE-FILECHECK-END
