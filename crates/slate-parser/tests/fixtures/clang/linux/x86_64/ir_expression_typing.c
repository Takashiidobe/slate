// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

short inc(short s) { return s + 1; }
extern int consume(short x, ...);
int decisions(short s, unsigned long wide, float f) {
    unsigned char byte = wide;
    _Bool truth = f;
    s = byte;
    consume(wide, s, f, truth);
    return truth ? (s, s + byte) : -s;
}
int sequencing(int *p, int i) {
    p[i++] += 2;
    return (i = 0, p[i] && (i = 3)) || (i ? ++i : i--);
}
struct Pair { short x; int y; };
int fields(struct Pair *p, struct Pair q) {
    p->x = q.y;
    return q.x + p->y;
}
int pointers(int *p) {
    int a[3];
    int *q = 0;
    q = a;
    return p != 0 ? q[1] : !p;
}

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type[[TYPE_Pair:[0-9]+]] Pair = struct {
// IR-NEXT:         field0 x: i16;
// IR-NEXT:         field1 y: i32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     fn %[[VALUE_inc:[0-9]+]] @inc(%[[VALUE_s:[0-9]+]] s: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return truncate<i16, reason=return, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_s]])), const<i32>(1)));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_consume:[0-9]+]] @consume(%[[VALUE_x:[0-9]+]] x: i16, ...) -> i32 [linkage=external];
// IR-NEXT:     fn %[[VALUE_decisions:[0-9]+]] @decisions(%[[VALUE_s_2:[0-9]+]] s: i16, %[[VALUE_wide:[0-9]+]] wide: u64, %[[VALUE_f:[0-9]+]] f: f32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_byte:[0-9]+]] byte: u8 [storage=automatic] = truncate<u8, reason=assign, fits=unknown>(read<u64>(%[[VALUE_wide]]));
// IR-NEXT:         let %[[VALUE_truth:[0-9]+]] truth: bool [storage=automatic] = ne<f32, reason=assign, exceptions=ignore>(read<f32>(%[[VALUE_f]]), const<f32>(0.0));
// IR-NEXT:         write<i16>(%[[VALUE_s_2]], reinterpret<i16, reason=assign, fits=unknown>(widen<u16, reason=assign>(read<u8>(%[[VALUE_byte]]))));
// IR-NEXT:         call<i32, signature=fn(i16, ...) -> i32>(%[[VALUE_consume]], reinterpret<i16, reason=arg, fits=unknown>(truncate<u16, reason=arg, fits=unknown>(read<u64>(%[[VALUE_wide]]))), widen<i32, reason=vararg>(read<i16>(%[[VALUE_s_2]])), float_widen<f64, reason=vararg>(read<f32>(%[[VALUE_f]])), from_bool<i32, reason=vararg>(read<bool>(%[[VALUE_truth]])));
// IR-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic];
// IR-NEXT:         if read<bool>(%[[VALUE_truth]])
// IR-NEXT:             read<i16>(%[[VALUE_s_2]]);
// IR-NEXT:             write<i32>(%[[VALUE0]], add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_s_2]])), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_byte]])))));
// IR-NEXT:         else
// IR-NEXT:             write<i32>(%[[VALUE0]], neg<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_s_2]]))));
// IR-NEXT:         return read<i32>(%[[VALUE0]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_sequencing:[0-9]+]] @sequencing(%[[VALUE_p:[0-9]+]] p: ptr<i32>, %[[VALUE_i:[0-9]+]] i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// IR-NEXT:         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// IR-NEXT:         write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// IR-NEXT:         let %[[VALUE3:[0-9]+]]: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_p]]), read<i32>(%[[VALUE1]]));
// IR-NEXT:         let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%[[VALUE3]])));
// IR-NEXT:         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(2));
// IR-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE3]])), read<i32>(%[[VALUE5]]));
// IR-NEXT:         write<i32>(%[[VALUE_i]], const<i32>(0));
// IR-NEXT:         let %[[VALUE6:[0-9]+]]: bool [synthetic];
// IR-NEXT:         if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_p]]), read<i32>(%[[VALUE_i]])))), const<i32>(0))
// IR-NEXT:             write<i32>(%[[VALUE_i]], const<i32>(3));
// IR-NEXT:             write<bool>(%[[VALUE6]], ne<i32>(const<i32>(3), const<i32>(0)));
// IR-NEXT:         else
// IR-NEXT:             write<bool>(%[[VALUE6]], const<bool>(false));
// IR-NEXT:         let %[[VALUE7:[0-9]+]]: bool [synthetic];
// IR-NEXT:         if read<bool>(%[[VALUE6]])
// IR-NEXT:             write<bool>(%[[VALUE7]], const<bool>(true));
// IR-NEXT:         else
// IR-NEXT:             let %[[VALUE8:[0-9]+]]: i32 [synthetic];
// IR-NEXT:             if ne<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0))
// IR-NEXT:                 let %[[VALUE9:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// IR-NEXT:                 let %[[VALUE10:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE9]]), const<i32>(1));
// IR-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE10]]));
// IR-NEXT:                 write<i32>(%[[VALUE8]], read<i32>(%[[VALUE10]]));
// IR-NEXT:             else
// IR-NEXT:                 let %[[VALUE11:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// IR-NEXT:                 let %[[VALUE12:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE11]]), const<i32>(1));
// IR-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE12]]));
// IR-NEXT:                 write<i32>(%[[VALUE8]], read<i32>(%[[VALUE11]]));
// IR-NEXT:             write<bool>(%[[VALUE7]], ne<i32>(read<i32>(%[[VALUE8]]), const<i32>(0)));
// IR-NEXT:         return from_bool<i32, reason=return>(read<bool>(%[[VALUE7]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_fields:[0-9]+]] @fields(%[[VALUE_p_2:[0-9]+]] p: ptr<@type[[TYPE_Pair]]>, %[[VALUE_q:[0-9]+]] q: @type[[TYPE_Pair]]) -> i32 [linkage=external] [abi=sysv64(scalar, native_c) -> scalar] [fallthrough=ub_if_used] {
// IR-NEXT:         write<i16>(field0(deref(read<ptr<@type[[TYPE_Pair]]>>(%[[VALUE_p_2]]))), truncate<i16, reason=assign, fits=unknown>(read<i32>(field1(%[[VALUE_q]]))));
// IR-NEXT:         return add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(field0(%[[VALUE_q]]))), read<i32>(field1(deref(read<ptr<@type[[TYPE_Pair]]>>(%[[VALUE_p_2]])))));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_pointers:[0-9]+]] @pointers(%[[VALUE_p_3:[0-9]+]] p: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_a:[0-9]+]] a: array<i32, 3> [storage=automatic];
// IR-NEXT:         let %[[VALUE_q_2:[0-9]+]] q: ptr<i32> [storage=automatic] = null<ptr<i32>>;
// IR-NEXT:         write<ptr<i32>>(%[[VALUE_q_2]], array_decay<ptr<i32>, length=Some(3)>(%[[VALUE_a]]));
// IR-NEXT:         return conditional<i32>(ne<ptr<i32>>(read<ptr<i32>>(%[[VALUE_p_3]]), null<ptr<i32>>), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_q_2]]), const<i32>(1)))), from_bool<i32, reason=promotion>(not<bool>(ne<ptr<i32>>(read<ptr<i32>>(%[[VALUE_p_3]]), null<ptr<i32>>))));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
