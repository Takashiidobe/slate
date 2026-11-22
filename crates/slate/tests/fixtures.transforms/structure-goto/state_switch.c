#include <stdio.h>

enum mode { HEAD = 16180, FLAGS, TIME, DONE, BAD };

struct state {
    enum mode mode;
    int have;
    int out;
};

#define NEED(n)                                                                \
    do {                                                                       \
        if (s->have < (n))                                                     \
            goto leave;                                                        \
        s->have -= (n);                                                        \
    } while (0)

int run(struct state *s) {
    int ret = 0;
    for (;;)
        switch (s->mode) {
        case HEAD:
            NEED(2);
            s->out += 1;
            s->mode = FLAGS;
        case FLAGS:
            NEED(1);
            s->out += 10;
            s->mode = TIME;
        case TIME:
            NEED(3);
            s->out += 100;
            s->mode = DONE;
            break;
        case DONE:
            ret = 1;
            goto leave;
        case BAD:
            ret = -3;
            goto leave;
        default:
            return -2;
        }
leave:
    s->out *= 2;
    return ret;
}

int main(void) {
    struct state a = {HEAD, 10, 0};
    struct state b = {HEAD, 2, 0};
    struct state c = {BAD, 0, 0};
    int ra = run(&a);
    int rb = run(&b);
    int rc = run(&c);
    printf("%d %d %d %d %d %d %d\n", ra, a.out, a.mode, rb, b.out, b.mode, rc);
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @run(%{{[0-9]+}} s: ptr<@type{{[0-9]+}}>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_RET:[0-9]+]] ret: i32 [storage=automatic] = const<i32>(0);
// CHECK-NEXT:     block %[[V0:[0-9]+]] {
// CHECK-NEXT:         for %[[V1:[0-9]+]]
// CHECK-NEXT:             init:
// CHECK-NEXT:             condition: omitted
// CHECK-NEXT:             increment: omitted
// CHECK-NEXT:             body:
// CHECK-NEXT:                 switch %[[V4:[0-9]+]] enum_to_int<u32, reason=promotion>(read<@type[[T2:[0-9]+]]>(field0(deref(read<ptr<@type[[T3:[0-9]+]]>>(%[[V_S:[0-9]+]])))))
// CHECK-NEXT:                     {
// CHECK-NEXT:                         case %[[V4]] const<u32>(16180):
// CHECK-NEXT:                             do %[[V5:[0-9]+]]
// CHECK-NEXT:                                 {
// CHECK-NEXT:                                     if lt<i32>(read<i32>(field1(deref(read<ptr<@type[[T3]]>>(%[[V_S]])))), const<i32>(2))
// CHECK-NEXT:                                         break %[[V0]];
// CHECK-NEXT:                                     let %[[V6:[0-9]+]]: ptr<@type[[T3]]> [synthetic] = read<ptr<@type[[T3]]>>(%[[V_S]]);
// CHECK-NEXT:                                     let %[[V7:[0-9]+]]: i32 [synthetic] = read<i32>(field1(deref(read<ptr<@type[[T3]]>>(%[[V6]]))));
// CHECK-NEXT:                                     let %[[V8:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[V7]]), const<i32>(2));
// CHECK-NEXT:                                     write<i32>(field1(deref(read<ptr<@type[[T3]]>>(%[[V6]]))), read<i32>(%[[V8]]));
// CHECK-NEXT:                                 }
// CHECK-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// CHECK-NEXT:                         let %[[V9:[0-9]+]]: ptr<@type[[T3]]> [synthetic] = read<ptr<@type[[T3]]>>(%[[V_S]]);
// CHECK-NEXT:                         let %[[V10:[0-9]+]]: i32 [synthetic] = read<i32>(field2(deref(read<ptr<@type[[T3]]>>(%[[V9]]))));
// CHECK-NEXT:                         let %[[V11:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[V10]]), const<i32>(1));
// CHECK-NEXT:                         write<i32>(field2(deref(read<ptr<@type[[T3]]>>(%[[V9]]))), read<i32>(%[[V11]]));
// CHECK-NEXT:                         write<@type[[T2]]>(field0(deref(read<ptr<@type[[T3]]>>(%[[V_S]]))), int_to_enum<@type[[T2]], reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(16181))));
// CHECK-NEXT:                         case %[[V4]] const<u32>(16181):
// CHECK-NEXT:                             do %[[V12:[0-9]+]]
// CHECK-NEXT:                                 {
// CHECK-NEXT:                                     if lt<i32>(read<i32>(field1(deref(read<ptr<@type[[T3]]>>(%[[V_S]])))), const<i32>(1))
// CHECK-NEXT:                                         break %[[V0]];
// CHECK-NEXT:                                     let %[[V13:[0-9]+]]: ptr<@type[[T3]]> [synthetic] = read<ptr<@type[[T3]]>>(%[[V_S]]);
// CHECK-NEXT:                                     let %[[V14:[0-9]+]]: i32 [synthetic] = read<i32>(field1(deref(read<ptr<@type[[T3]]>>(%[[V13]]))));
// CHECK-NEXT:                                     let %[[V15:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[V14]]), const<i32>(1));
// CHECK-NEXT:                                     write<i32>(field1(deref(read<ptr<@type[[T3]]>>(%[[V13]]))), read<i32>(%[[V15]]));
// CHECK-NEXT:                                 }
// CHECK-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// CHECK-NEXT:                         let %[[V16:[0-9]+]]: ptr<@type[[T3]]> [synthetic] = read<ptr<@type[[T3]]>>(%[[V_S]]);
// CHECK-NEXT:                         let %[[V17:[0-9]+]]: i32 [synthetic] = read<i32>(field2(deref(read<ptr<@type[[T3]]>>(%[[V16]]))));
// CHECK-NEXT:                         let %[[V18:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[V17]]), const<i32>(10));
// CHECK-NEXT:                         write<i32>(field2(deref(read<ptr<@type[[T3]]>>(%[[V16]]))), read<i32>(%[[V18]]));
// CHECK-NEXT:                         write<@type[[T2]]>(field0(deref(read<ptr<@type[[T3]]>>(%[[V_S]]))), int_to_enum<@type[[T2]], reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(16182))));
// CHECK-NEXT:                         case %[[V4]] const<u32>(16182):
// CHECK-NEXT:                             do %[[V19:[0-9]+]]
// CHECK-NEXT:                                 {
// CHECK-NEXT:                                     if lt<i32>(read<i32>(field1(deref(read<ptr<@type[[T3]]>>(%[[V_S]])))), const<i32>(3))
// CHECK-NEXT:                                         break %[[V0]];
// CHECK-NEXT:                                     let %[[V20:[0-9]+]]: ptr<@type[[T3]]> [synthetic] = read<ptr<@type[[T3]]>>(%[[V_S]]);
// CHECK-NEXT:                                     let %[[V21:[0-9]+]]: i32 [synthetic] = read<i32>(field1(deref(read<ptr<@type[[T3]]>>(%[[V20]]))));
// CHECK-NEXT:                                     let %[[V22:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[V21]]), const<i32>(3));
// CHECK-NEXT:                                     write<i32>(field1(deref(read<ptr<@type[[T3]]>>(%[[V20]]))), read<i32>(%[[V22]]));
// CHECK-NEXT:                                 }
// CHECK-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// CHECK-NEXT:                         let %[[V23:[0-9]+]]: ptr<@type[[T3]]> [synthetic] = read<ptr<@type[[T3]]>>(%[[V_S]]);
// CHECK-NEXT:                         let %[[V24:[0-9]+]]: i32 [synthetic] = read<i32>(field2(deref(read<ptr<@type[[T3]]>>(%[[V23]]))));
// CHECK-NEXT:                         let %[[V25:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[V24]]), const<i32>(100));
// CHECK-NEXT:                         write<i32>(field2(deref(read<ptr<@type[[T3]]>>(%[[V23]]))), read<i32>(%[[V25]]));
// CHECK-NEXT:                         write<@type[[T2]]>(field0(deref(read<ptr<@type[[T3]]>>(%[[V_S]]))), int_to_enum<@type[[T2]], reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(16183))));
// CHECK-NEXT:                         break %[[V4]];
// CHECK-NEXT:                         case %[[V4]] const<u32>(16183):
// CHECK-NEXT:                             write<i32>(%[[V_RET]], const<i32>(1));
// CHECK-NEXT:                         break %[[V0]];
// CHECK-NEXT:                         case %[[V4]] const<u32>(16184):
// CHECK-NEXT:                             write<i32>(%[[V_RET]], neg<i32, overflow=ub>(const<i32>(3)));
// CHECK-NEXT:                         break %[[V0]];
// CHECK-NEXT:                         default %[[V4]]:
// CHECK-NEXT:                             return neg<i32, overflow=ub>(const<i32>(2));
// CHECK-NEXT:                     }
// CHECK-NEXT:     }
// CHECK-NEXT:     let %[[V26:[0-9]+]]: ptr<@type[[T3]]> [synthetic] = read<ptr<@type[[T3]]>>(%[[V_S]]);
// CHECK-NEXT:     let %[[V27:[0-9]+]]: i32 [synthetic] = read<i32>(field2(deref(read<ptr<@type[[T3]]>>(%[[V26]]))));
// CHECK-NEXT:     let %[[V28:[0-9]+]]: i32 [synthetic] = mul<i32, overflow=ub>(read<i32>(%[[V27]]), const<i32>(2));
// CHECK-NEXT:     write<i32>(field2(deref(read<ptr<@type[[T3]]>>(%[[V26]]))), read<i32>(%[[V28]]));
// CHECK-NEXT:     return read<i32>(%[[V_RET]]);
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// CHECK-NEXT:     let %[[V_A:[0-9]+]] a: @type[[T0:[0-9]+]] [storage=automatic] = aggregate<@type[[T0]], zero_fill=false>(field0 = int_to_enum<@type[[T1:[0-9]+]], reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(16180))), field1 = const<i32>(10), field2 = const<i32>(0));
// CHECK-NEXT:     let %[[V_B:[0-9]+]] b: @type[[T0]] [storage=automatic] = aggregate<@type[[T0]], zero_fill=false>(field0 = int_to_enum<@type[[T1]], reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(16180))), field1 = const<i32>(2), field2 = const<i32>(0));
// CHECK-NEXT:     let %[[V_C:[0-9]+]] c: @type[[T0]] [storage=automatic] = aggregate<@type[[T0]], zero_fill=false>(field0 = int_to_enum<@type[[T1]], reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(16184))), field1 = const<i32>(0), field2 = const<i32>(0));
// CHECK-NEXT:     let %[[V_RA:[0-9]+]] ra: i32 [storage=automatic] = call<i32, signature=fn(ptr<@type[[T0]]>) -> i32>(%[[V2:[0-9]+]], addr_of<ptr<@type[[T0]]>>(%[[V_A]]));
// CHECK-NEXT:     let %[[V_RB:[0-9]+]] rb: i32 [storage=automatic] = call<i32, signature=fn(ptr<@type[[T0]]>) -> i32>(%[[V2]], addr_of<ptr<@type[[T0]]>>(%[[V_B]]));
// CHECK-NEXT:     let %[[V_RC:[0-9]+]] rc: i32 [storage=automatic] = call<i32, signature=fn(ptr<@type[[T0]]>) -> i32>(%[[V2]], addr_of<ptr<@type[[T0]]>>(%[[V_C]]));
// CHECK-NEXT:     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[V3:[0-9]+]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(22)>(%[[V4:[0-9]+]])), read<i32>(%[[V_RA]]), read<i32>(field2(%[[V_A]])), enum_to_int<u32, reason=promotion>(read<@type[[T1]]>(field0(%[[V_A]]))), read<i32>(%[[V_RB]]), read<i32>(field2(%[[V_B]])), enum_to_int<u32, reason=promotion>(read<@type[[T1]]>(field0(%[[V_B]]))), read<i32>(%[[V_RC]]));
// CHECK-NEXT:     return const<i32>(0);
// CHECK: {{^}}}
// SLATE-FILECHECK-END
