int g;
int other;
int helper(void);

int global_name(int c) {
    {
        int g = c;
        while (g > 0) {
            if (g == 5)
                goto tail;
            g--;
        }
        return g;
    }
tail:
    other = 1;
    return -1;
}

int param_name(int c, int v) {
    {
        int v = c;
        while (v > 0) {
            if (v == 5)
                goto tail;
            v--;
        }
        return v;
    }
tail:
    other = 1;
    return -1;
}

int sibling_local(int c) {
    {
        int v = c;
        other = v;
    }
    {
        int v = c;
        while (v > 0) {
            if (v == 5)
                goto tail;
            v--;
        }
        return v;
    }
tail:
    return -1;
}

int outer_read(int c) {
    int v = 7;
    {
        int v = c;
        while (v > 0) {
            if (v == 5)
                goto tail;
            v--;
        }
        return v;
    }
tail:
    return v;
}

int global_write(int c) {
    {
        int g = c;
        while (g > 0) {
            if (g == 5)
                goto tail;
            g--;
        }
        return g;
    }
tail:
    g = 1;
    return -1;
}

int outer_call(int c) {
    {
        int helper = c;
        while (helper > 0) {
            if (helper == 5)
                goto tail;
            helper--;
        }
        return helper;
    }
tail:
    return helper();
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @global_name(%{{[0-9]+}} c: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     {
// CHECK-NEXT:         let %[[V_G:[0-9]+]] g: i32 [storage=automatic] = read<i32>(%[[V_C:[0-9]+]]);
// CHECK-NEXT:         while %[[V0:[0-9]+]] gt<i32>(read<i32>(%[[V_G]]), const<i32>(0))
// CHECK-NEXT:             {
// CHECK-NEXT:                 if eq<i32>(read<i32>(%[[V_G]]), const<i32>(5))
// CHECK-NEXT:                     write<i32>(%[[V1:[0-9]+]], const<i32>(1));
// CHECK-NEXT:                     return neg<i32, overflow=ub>(const<i32>(1));
// CHECK-NEXT:                 let %[[V2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[V_G]]);
// CHECK-NEXT:                 let %[[V3:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[V2]]), const<i32>(1));
// CHECK-NEXT:                 write<i32>(%[[V_G]], read<i32>(%[[V3]]));
// CHECK-NEXT:             }
// CHECK-NEXT:         return read<i32>(%[[V_G]]);
// CHECK-NEXT:     }
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @param_name(%{{[0-9]+}} c: i32, %{{[0-9]+}} v: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     {
// CHECK-NEXT:         let %[[V_V_2:[0-9]+]] v: i32 [storage=automatic] = read<i32>(%[[V_C:[0-9]+]]);
// CHECK-NEXT:         while %[[V0:[0-9]+]] gt<i32>(read<i32>(%[[V_V_2]]), const<i32>(0))
// CHECK-NEXT:             {
// CHECK-NEXT:                 if eq<i32>(read<i32>(%[[V_V_2]]), const<i32>(5))
// CHECK-NEXT:                     write<i32>(%[[V1:[0-9]+]], const<i32>(1));
// CHECK-NEXT:                     return neg<i32, overflow=ub>(const<i32>(1));
// CHECK-NEXT:                 let %[[V2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[V_V_2]]);
// CHECK-NEXT:                 let %[[V3:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[V2]]), const<i32>(1));
// CHECK-NEXT:                 write<i32>(%[[V_V_2]], read<i32>(%[[V3]]));
// CHECK-NEXT:             }
// CHECK-NEXT:         return read<i32>(%[[V_V_2]]);
// CHECK-NEXT:     }
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @sibling_local(%{{[0-9]+}} c: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     {
// CHECK-NEXT:         let %[[V_V:[0-9]+]] v: i32 [storage=automatic] = read<i32>(%[[V_C:[0-9]+]]);
// CHECK-NEXT:         write<i32>(%[[V0:[0-9]+]], read<i32>(%[[V_V]]));
// CHECK-NEXT:     }
// CHECK-NEXT:     {
// CHECK-NEXT:         let %[[V_V_2:[0-9]+]] v: i32 [storage=automatic] = read<i32>(%[[V_C]]);
// CHECK-NEXT:         while %[[V1:[0-9]+]] gt<i32>(read<i32>(%[[V_V_2]]), const<i32>(0))
// CHECK-NEXT:             {
// CHECK-NEXT:                 if eq<i32>(read<i32>(%[[V_V_2]]), const<i32>(5))
// CHECK-NEXT:                     return neg<i32, overflow=ub>(const<i32>(1));
// CHECK-NEXT:                 let %[[V2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[V_V_2]]);
// CHECK-NEXT:                 let %[[V3:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[V2]]), const<i32>(1));
// CHECK-NEXT:                 write<i32>(%[[V_V_2]], read<i32>(%[[V3]]));
// CHECK-NEXT:             }
// CHECK-NEXT:         return read<i32>(%[[V_V_2]]);
// CHECK-NEXT:     }
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @outer_read(%{{[0-9]+}} c: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_V:[0-9]+]] v: i32 [storage=automatic] = const<i32>(7);
// CHECK-NEXT:     block %[[V0:[0-9]+]] {
// CHECK-NEXT:         {
// CHECK-NEXT:             let %[[V_V_2:[0-9]+]] v: i32 [storage=automatic] = read<i32>(%[[V_C:[0-9]+]]);
// CHECK-NEXT:             while %[[V1:[0-9]+]] gt<i32>(read<i32>(%[[V_V_2]]), const<i32>(0))
// CHECK-NEXT:                 {
// CHECK-NEXT:                     if eq<i32>(read<i32>(%[[V_V_2]]), const<i32>(5))
// CHECK-NEXT:                         break %[[V0]];
// CHECK-NEXT:                     let %[[V2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[V_V_2]]);
// CHECK-NEXT:                     let %[[V3:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[V2]]), const<i32>(1));
// CHECK-NEXT:                     write<i32>(%[[V_V_2]], read<i32>(%[[V3]]));
// CHECK-NEXT:                 }
// CHECK-NEXT:             return read<i32>(%[[V_V_2]]);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     return read<i32>(%[[V_V]]);
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @global_write(%{{[0-9]+}} c: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     block %[[V0:[0-9]+]] {
// CHECK-NEXT:         {
// CHECK-NEXT:             let %[[V_G:[0-9]+]] g: i32 [storage=automatic] = read<i32>(%[[V_C:[0-9]+]]);
// CHECK-NEXT:             while %[[V1:[0-9]+]] gt<i32>(read<i32>(%[[V_G]]), const<i32>(0))
// CHECK-NEXT:                 {
// CHECK-NEXT:                     if eq<i32>(read<i32>(%[[V_G]]), const<i32>(5))
// CHECK-NEXT:                         break %[[V0]];
// CHECK-NEXT:                     let %[[V2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[V_G]]);
// CHECK-NEXT:                     let %[[V3:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[V2]]), const<i32>(1));
// CHECK-NEXT:                     write<i32>(%[[V_G]], read<i32>(%[[V3]]));
// CHECK-NEXT:                 }
// CHECK-NEXT:             return read<i32>(%[[V_G]]);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     write<i32>(%[[V4:[0-9]+]], const<i32>(1));
// CHECK-NEXT:     return neg<i32, overflow=ub>(const<i32>(1));
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @outer_call(%{{[0-9]+}} c: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     block %[[V0:[0-9]+]] {
// CHECK-NEXT:         {
// CHECK-NEXT:             let %[[V_HELPER:[0-9]+]] helper: i32 [storage=automatic] = read<i32>(%[[V_C:[0-9]+]]);
// CHECK-NEXT:             while %[[V1:[0-9]+]] gt<i32>(read<i32>(%[[V_HELPER]]), const<i32>(0))
// CHECK-NEXT:                 {
// CHECK-NEXT:                     if eq<i32>(read<i32>(%[[V_HELPER]]), const<i32>(5))
// CHECK-NEXT:                         break %[[V0]];
// CHECK-NEXT:                     let %[[V2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[V_HELPER]]);
// CHECK-NEXT:                     let %[[V3:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[V2]]), const<i32>(1));
// CHECK-NEXT:                     write<i32>(%[[V_HELPER]], read<i32>(%[[V3]]));
// CHECK-NEXT:                 }
// CHECK-NEXT:             return read<i32>(%[[V_HELPER]]);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     return call<i32, signature=fn() -> i32>(%[[V4:[0-9]+]]);
// CHECK: {{^}}}
// SLATE-FILECHECK-END
