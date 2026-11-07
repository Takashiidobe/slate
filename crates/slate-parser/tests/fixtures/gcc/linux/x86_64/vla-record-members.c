// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

void consume(void *);

int trailing(int n) {
    struct S { int head; char tail[n]; } x;
    x.tail[n - 1] = 1;
    return sizeof x + sizeof(struct S);
}

unsigned long nested(int i) {
    union U { int ar[1][i]; } u;
    u.ar[0][0] = 0;
    return sizeof u.ar;
}

unsigned long declared_first(int n) {
    struct D { double d[n]; };
    n++;
    struct D value;
    consume(&value);
    return sizeof value;
}

void cast(char *a, unsigned l) {
    consume(((struct { char x[l]; } *)a)->x);
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
// IR-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// IR-NEXT:         field0 head: i32;
// IR-NEXT:         field1 tail: vla<i8, %[[VALUE0:[0-9]+]]>;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type[[TYPE_U:[0-9]+]] U = union {
// IR-NEXT:         field0 ar: array<vla<i32, %[[VALUE1:[0-9]+]]>, 1>;
// IR-NEXT:     } [size=0, align=4, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_D:[0-9]+]] D = struct {
// IR-NEXT:         field0 d: vla<f64, %[[VALUE2:[0-9]+]]>;
// IR-NEXT:     } [size=0, align=8, offsets=[0]];
// IR-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// IR-NEXT:         field0 x: vla<i8, %[[VALUE3:[0-9]+]]>;
// IR-NEXT:     } [size=0, align=1, offsets=[0]];
// IR-NEXT:     fn %[[VALUE_consume:[0-9]+]] @consume(%[[VALUE4:[0-9]+]] <unnamed>: ptr<void>) -> void [linkage=external];
// IR-NEXT:     fn %[[VALUE_trailing:[0-9]+]] @trailing(%[[VALUE_n:[0-9]+]] n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE0]]: u64 [synthetic] = reinterpret<u64>(widen<i64>(read<i32>(%[[VALUE_n]]))) [c="__size_t"];
// IR-NEXT:         let %[[VALUE_x:[0-9]+]] x: @type[[TYPE_S]] [storage=automatic];
// IR-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false>(array_decay<ptr<i8>, length=None>(field1(%[[VALUE_x]])), sub<i32>(read<i32>(%[[VALUE_n]]), const<i32>(1)))), truncate<i8>(const<i32>(1)));
// IR-NEXT:         return reinterpret<i32>(truncate<u32>(add<u64>(mul<u64>(div<u64>(add<u64>(const<u64>(7) [c="__size_t"], mul<u64>(read<u64>(%[[VALUE0]]) [c="__size_t"], const<u64>(1) [c="__size_t"])), const<u64>(4) [c="__size_t"]), const<u64>(4) [c="__size_t"]) [c="__size_t"], mul<u64>(div<u64>(add<u64>(const<u64>(7) [c="__size_t"], mul<u64>(read<u64>(%[[VALUE0]]) [c="__size_t"], const<u64>(1) [c="__size_t"])), const<u64>(4) [c="__size_t"]), const<u64>(4) [c="__size_t"]) [c="__size_t"]) [c="__size_t"]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_nested:[0-9]+]] @nested(%[[VALUE_i:[0-9]+]] i: i32) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE1]]: u64 [synthetic] = reinterpret<u64>(widen<i64>(read<i32>(%[[VALUE_i]]))) [c="__size_t"];
// IR-NEXT:         let %[[VALUE_u:[0-9]+]] u: @type[[TYPE_U]] [storage=automatic];
// IR-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false>(array_decay<ptr<i32>, length=None>(deref(ptr_offset<ptr<vla<i32, %[[VALUE1]]>>, subtract=false>(array_decay<ptr<vla<i32, %[[VALUE1]]>>, length=Some(1)>(field0(%[[VALUE_u]])), const<i32>(0)))), const<i32>(0))), const<i32>(0));
// IR-NEXT:         return reinterpret<u64>(mul<u64>(const<u64>(1) [c="__size_t"], mul<u64>(read<u64>(%[[VALUE1]]) [c="__size_t"], const<u64>(4) [c="__size_t"])) [c="__size_t"]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_declared_first:[0-9]+]] @declared_first(%[[VALUE_n_2:[0-9]+]] n: i32) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE2]]: u64 [synthetic] = reinterpret<u64>(widen<i64>(read<i32>(%[[VALUE_n_2]]))) [c="__size_t"];
// IR-NEXT:         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_n_2]]);
// IR-NEXT:         let %[[VALUE6:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE5]]), const<i32>(1));
// IR-NEXT:         write<i32>(%[[VALUE_n_2]], read<i32>(%[[VALUE6]]));
// IR-NEXT:         let %[[VALUE_value:[0-9]+]] value: @type[[TYPE_D]] [storage=automatic];
// IR-NEXT:         call<void>(%[[VALUE_consume]], pointer_cast<ptr<void>>(addr_of<ptr<@type[[TYPE_D]]>>(%[[VALUE_value]])));
// IR-NEXT:         return reinterpret<u64>(mul<u64>(div<u64>(add<u64>(const<u64>(7) [c="__size_t"], mul<u64>(read<u64>(%[[VALUE2]]) [c="__size_t"], const<u64>(8) [c="__size_t"])), const<u64>(8) [c="__size_t"]), const<u64>(8) [c="__size_t"]) [c="__size_t"]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_cast:[0-9]+]] @cast(%[[VALUE_a:[0-9]+]] a: ptr<i8>, %[[VALUE_l:[0-9]+]] l: u32) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         let %[[VALUE3]]: u64 [synthetic] = widen<u64>(read<u32>(%[[VALUE_l]])) [c="__size_t"];
// IR-NEXT:         call<void>(%[[VALUE_consume]], pointer_cast<ptr<void>>(array_decay<ptr<i8>, length=None>(field0(deref(pointer_cast<ptr<@type[[TYPE0]]>>(read<ptr<i8>>(%[[VALUE_a]])))))));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
