void report(int value);
void release(int *p);

int nested(int a, int b) {
    int r;
    if (a > 0) {
        if (b > 0)
            goto both;
        else
            goto first;
    } else {
        goto neither;
    }
both:
    r = 1;
    goto done;
first:
    r = 2;
    goto done;
neither:
    r = 3;
done:
    return r;
}

int cases(int x) {
    int r = 0;
    switch (x) {
    case 1:
        goto one;
    case 2:
        goto two;
    default:
        goto other;
    }
one:
    r = 10;
    goto done;
two:
    r = 20;
    goto done;
other:
    r = 30;
done:
    report(r);
    return r;
}

int skip(int c) {
    int r = 0;
    if (c)
        goto out;
    r = 5;
out:
    return r;
}

int exits(int n) {
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += i;
        if (sum > 100)
            goto overflow;
    }
    report(sum);
    return 0;
overflow:
    report(-1);
    return 1;
}

int shadowed(int c) {
    int x = 1;
    {
        int x = 2;
        if (c)
            goto tail;
        report(x);
        return x;
    }
tail:
    return x;
}

int cleanup(int c) {
    int r = 0;
    {
        __attribute__((cleanup(release))) int held = 1;
        if (c)
            goto tail;
        r = held;
        goto done;
    }
tail:
    r = -1;
done:
    return r;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @nested(%{{[0-9]+}} a: i32, %{{[0-9]+}} b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_R:[0-9]+]] r: i32 [storage=automatic];
// CHECK-NEXT:     if gt<i32>(read<i32>(%[[V_A:[0-9]+]]), const<i32>(0))
// CHECK-NEXT:         {
// CHECK-NEXT:             if gt<i32>(read<i32>(%[[V_B:[0-9]+]]), const<i32>(0))
// CHECK-NEXT:                 write<i32>(%[[V_R]], const<i32>(1));
// CHECK-NEXT:             else
// CHECK-NEXT:                 write<i32>(%[[V_R]], const<i32>(2));
// CHECK-NEXT:         }
// CHECK-NEXT:     else
// CHECK-NEXT:         {
// CHECK-NEXT:             write<i32>(%[[V_R]], const<i32>(3));
// CHECK-NEXT:         }
// CHECK-NEXT:     return read<i32>(%[[V_R]]);
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @cases(%{{[0-9]+}} x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_R:[0-9]+]] r: i32 [storage=automatic] = const<i32>(0);
// CHECK-NEXT:     switch %[[V0:[0-9]+]] read<i32>(%[[V_X:[0-9]+]])
// CHECK-NEXT:         {
// CHECK-NEXT:             case %[[V0]] const<i32>(1):
// CHECK-NEXT:                 write<i32>(%[[V_R]], const<i32>(10));
// CHECK-NEXT:                 break %[[V0]];
// CHECK-NEXT:             case %[[V0]] const<i32>(2):
// CHECK-NEXT:                 write<i32>(%[[V_R]], const<i32>(20));
// CHECK-NEXT:                 break %[[V0]];
// CHECK-NEXT:             default %[[V0]]:
// CHECK-NEXT:                 write<i32>(%[[V_R]], const<i32>(30));
// CHECK-NEXT:                 break %[[V0]];
// CHECK-NEXT:         }
// CHECK-NEXT:     call<void, signature=fn(i32) -> void>(%[[V1:[0-9]+]], read<i32>(%[[V_R]]));
// CHECK-NEXT:     return read<i32>(%[[V_R]]);
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @skip(%{{[0-9]+}} c: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_R:[0-9]+]] r: i32 [storage=automatic] = const<i32>(0);
// CHECK-NEXT:     if eq<i32>(read<i32>(%[[V_C:[0-9]+]]), const<i32>(0))
// CHECK-NEXT:         write<i32>(%[[V_R]], const<i32>(5));
// CHECK-NEXT:     return read<i32>(%[[V_R]]);
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @exits(%{{[0-9]+}} n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_SUM:[0-9]+]] sum: i32 [storage=automatic] = const<i32>(0);
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
// CHECK-NEXT:                 let %[[V3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[V_SUM]]);
// CHECK-NEXT:                 let %[[V4:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[V3]]), read<i32>(%[[V_I]]));
// CHECK-NEXT:                 write<i32>(%[[V_SUM]], read<i32>(%[[V4]]));
// CHECK-NEXT:                 if gt<i32>(read<i32>(%[[V_SUM]]), const<i32>(100))
// CHECK-NEXT:                     call<void, signature=fn(i32) -> void>(%[[V5:[0-9]+]], neg<i32, overflow=ub>(const<i32>(1)));
// CHECK-NEXT:                     return const<i32>(1);
// CHECK-NEXT:             }
// CHECK-NEXT:     call<void, signature=fn(i32) -> void>(%[[V5]], read<i32>(%[[V_SUM]]));
// CHECK-NEXT:     return const<i32>(0);
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @shadowed(%{{[0-9]+}} c: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_X:[0-9]+]] x: i32 [storage=automatic] = const<i32>(1);
// CHECK-NEXT:     {
// CHECK-NEXT:         let %[[V_X_2:[0-9]+]] x: i32 [storage=automatic] = const<i32>(2);
// CHECK-NEXT:         if eq<i32>(read<i32>(%[[V_C:[0-9]+]]), const<i32>(0))
// CHECK-NEXT:             call<void, signature=fn(i32) -> void>(%[[V0:[0-9]+]], read<i32>(%[[V_X_2]]));
// CHECK-NEXT:             return read<i32>(%[[V_X_2]]);
// CHECK-NEXT:     }
// CHECK-NEXT:     return read<i32>(%[[V_X]]);
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @cleanup(%{{[0-9]+}} c: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_R:[0-9]+]] r: i32 [storage=automatic] = const<i32>(0);
// CHECK-NEXT:     block %[[V0:[0-9]+]] {
// CHECK-NEXT:         {
// CHECK-NEXT:             let %[[V_HELD:[0-9]+]] held: i32 [storage=automatic] [cleanup=%[[V1:[0-9]+]]] = const<i32>(1);
// CHECK-NEXT:             if eq<i32>(read<i32>(%[[V_C:[0-9]+]]), const<i32>(0))
// CHECK-NEXT:                 write<i32>(%[[V_R]], read<i32>(%[[V_HELD]]));
// CHECK-NEXT:                 break %[[V0]];
// CHECK-NEXT:         }
// CHECK-NEXT:         write<i32>(%[[V_R]], neg<i32, overflow=ub>(const<i32>(1)));
// CHECK-NEXT:     }
// CHECK-NEXT:     return read<i32>(%[[V_R]]);
// CHECK: {{^}}}
// SLATE-FILECHECK-END
