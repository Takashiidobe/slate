// SLATE-FILECHECK-DEFINES O2
// SLATE-FILECHECK-DEFINES O0
// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-PREFIX-ARGS O2 -O2
// SLATE-FILECHECK-PREFIX-ARGS O0 -O0
struct key { int enabled; };
struct key k1, k2;

static inline __attribute__((always_inline)) int branch(struct key *key, _Bool likely) {
    asm goto("# %c0 %c1" : : "i"(&((char *)key)[likely]), "i"(likely) : : yes);
    return 0;
yes:
    return 1;
}

static inline __attribute__((always_inline)) int bit(int nr, int *p) {
    int out;
    asm("# %1 %2" : "=r"(out) : "i"(1 << (nr & 7)), "r"(p));
    return out;
}

static inline __attribute__((always_inline)) int outer(struct key *key) {
    return branch(key, 0);
}

void locals(int runtime) {
    int flags = 0x100 | 3;
    __auto_type wide = 1L << 40;
    int maybe = 1;
    if (runtime)
        maybe = 2;
    asm("# %0 %1" : : "i"(flags), "i"(wide));
    asm("# %0" : : "i"(maybe));
}

int calls(int runtime, struct key *dynamic) {
    int x = 0;
    return branch(&k1, 1) + branch(&k2, 0) + branch(&k1, 1) + bit(9, &x) + outer(&k2)
        + branch(dynamic, 1) + bit(runtime, &x);
}

// SLATE-FILECHECK-BEGIN O2
// O2: module {
// O2-NEXT:     target "x86_64-unknown-linux-gnu" {
// O2-NEXT:         endian = little;
// O2-NEXT:         pointer [size=8, align=8];
// O2-NEXT:         stack_alignment = 16;
// O2-NEXT:         long_double = f80;
// O2-NEXT:         storage bool [size=1, align=1];
// O2-NEXT:         storage i8, u8 [size=1, align=1];
// O2-NEXT:         storage i16, u16 [size=2, align=2];
// O2-NEXT:         storage i32, u32 [size=4, align=4];
// O2-NEXT:         storage i64, u64 [size=8, align=8];
// O2-NEXT:         storage i128, u128 [size=16, align=16];
// O2-NEXT:         storage bf16 [size=2, align=2];
// O2-NEXT:         storage f16 [size=2, align=2];
// O2-NEXT:         storage f32 [size=4, align=4];
// O2-NEXT:         storage f64 [size=8, align=8];
// O2-NEXT:         storage f80 [size=16, align=16];
// O2-NEXT:         storage f128 [size=16, align=16];
// O2-NEXT:         storage d32 [size=4, align=4];
// O2-NEXT:         storage d64 [size=8, align=8];
// O2-NEXT:         storage d128 [size=16, align=16];
// O2-NEXT:     }
// O2-NEXT:     type @type[[TYPE_key:[0-9]+]] key = struct {
// O2-NEXT:         field0 enabled: i32;
// O2-NEXT:     } [size=4, align=4, offsets=[0]];
// O2-NEXT:     global %[[VALUE_k1:[0-9]+]] k1: @type[[TYPE_key]] [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_k2:[0-9]+]] k2: @type[[TYPE_key]] [storage=static] [linkage=external];
// O2-NEXT:     fn %[[VALUE_branch:[0-9]+]] @branch(%[[VALUE_key:[0-9]+]] key: ptr<@type[[TYPE_key]]>, %[[VALUE_likely:[0-9]+]] likely: bool) -> i32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// O2-NEXT:         asm goto "# %c0 %c1" [dialect=att] [options=nostack] [alternative=none] {
// O2-NEXT:             template: "# " %c0 " " %c1;
// O2-NEXT:             in 0 "i" [imm | sym] width 64 addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(pointer_cast<ptr<i8>, reason=explicit>(read<ptr<@type[[TYPE_key]]>>(%[[VALUE_key]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_likely]])))));
// O2-NEXT:             in 1 "i" [imm | sym] width 8 read<bool>(%[[VALUE_likely]]);
// O2-NEXT:             rejected: 0 (operand 0: not-constant);
// O2-NEXT:             labels: %[[VALUE_yes:[0-9]+]];
// O2-NEXT:         }
// O2-NEXT:         return const<i32>(0);
// O2-NEXT:         label %[[VALUE_yes]] yes:
// O2-NEXT:             return const<i32>(1);
// O2-NEXT:     }
// O2-NEXT:     fn %[[VALUE_bit:[0-9]+]] @bit(%[[VALUE_nr:[0-9]+]] nr: i32, %[[VALUE_p:[0-9]+]] p: ptr<i32>) -> i32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// O2-NEXT:         let %[[VALUE_out:[0-9]+]] out: i32 [storage=automatic];
// O2-NEXT:         asm "# %1 %2" [dialect=att] [options=pure,nomem,nostack] [alternative=none] {
// O2-NEXT:             template: "# " %1 " " %2;
// O2-NEXT:             lateout 0 "r" [reg] width 32 place<i32>(%[[VALUE_out]]);
// O2-NEXT:             in 1 "i" [imm | sym] width 32 shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), and<i32>(read<i32>(%[[VALUE_nr]]), const<i32>(7)));
// O2-NEXT:             in 2 "r" [reg] width 64 read<ptr<i32>>(%[[VALUE_p]]);
// O2-NEXT:             rejected: 0 (operand 1: not-constant);
// O2-NEXT:         }
// O2-NEXT:         return read<i32>(%[[VALUE_out]]);
// O2-NEXT:     }
// O2-NEXT:     fn %[[VALUE_locals:[0-9]+]] @locals(%[[VALUE_runtime:[0-9]+]] runtime: i32) -> void [linkage=external] [fallthrough=ret_void] {
// O2-NEXT:         let %[[VALUE_flags:[0-9]+]] flags: i32 [storage=automatic] = or<i32>(const<i32>(256), const<i32>(3));
// O2-NEXT:         let %[[VALUE_wide:[0-9]+]] wide: i64 [storage=automatic] = shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i64>(1), const<i32>(40));
// O2-NEXT:         let %[[VALUE_maybe:[0-9]+]] maybe: i32 [storage=automatic] = const<i32>(1);
// O2-NEXT:         if ne<i32>(read<i32>(%[[VALUE_runtime]]), const<i32>(0))
// O2-NEXT:             write<i32>(%[[VALUE_maybe]], const<i32>(2));
// O2-NEXT:         asm "# %0 %1" [dialect=att] [options=nostack] {
// O2-NEXT:             template: "# " %0 " " %1;
// O2-NEXT:             in 0 "i" [imm | sym] -> imm width 32 const<i32>(259);
// O2-NEXT:             in 1 "i" [imm | sym] -> imm width 64 const<i64>(1099511627776);
// O2-NEXT:         }
// O2-NEXT:         asm "# %0" [dialect=att] [options=nostack] [alternative=none] {
// O2-NEXT:             template: "# " %0;
// O2-NEXT:             in 0 "i" [imm | sym] width 32 read<i32>(%[[VALUE_maybe]]);
// O2-NEXT:             rejected: 0 (operand 0: not-constant);
// O2-NEXT:         }
// O2-NEXT:     }
// O2-NEXT:     fn %[[VALUE_calls:[0-9]+]] @calls(%[[VALUE_runtime_2:[0-9]+]] runtime: i32, %[[VALUE_dynamic:[0-9]+]] dynamic: ptr<@type[[TYPE_key]]>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// O2-NEXT:         let %[[VALUE_x:[0-9]+]] x: i32 [storage=automatic] = const<i32>(0);
// O2-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(call<i32, signature=fn(ptr<@type[[TYPE_key]]>, bool) -> i32>(%[[VALUE_branch__const1:[0-9]+]], addr_of<ptr<@type[[TYPE_key]]>>(%[[VALUE_k1]]), ne<i32, reason=arg>(const<i32>(1), const<i32>(0))), call<i32, signature=fn(ptr<@type[[TYPE_key]]>, bool) -> i32>(%[[VALUE_branch__const2:[0-9]+]], addr_of<ptr<@type[[TYPE_key]]>>(%[[VALUE_k2]]), ne<i32, reason=arg>(const<i32>(0), const<i32>(0)))), call<i32, signature=fn(ptr<@type[[TYPE_key]]>, bool) -> i32>(%[[VALUE_branch__const1]], addr_of<ptr<@type[[TYPE_key]]>>(%[[VALUE_k1]]), ne<i32, reason=arg>(const<i32>(1), const<i32>(0)))), call<i32, signature=fn(i32, ptr<i32>) -> i32>(%[[VALUE_bit__const3:[0-9]+]], const<i32>(9), addr_of<ptr<i32>>(%[[VALUE_x]]))), call<i32, signature=fn(ptr<@type[[TYPE_key]]>) -> i32>(%[[VALUE_outer__const4:[0-9]+]], addr_of<ptr<@type[[TYPE_key]]>>(%[[VALUE_k2]]))), call<i32, signature=fn(ptr<@type[[TYPE_key]]>, bool) -> i32>(%[[VALUE_branch]], read<ptr<@type[[TYPE_key]]>>(%[[VALUE_dynamic]]), ne<i32, reason=arg>(const<i32>(1), const<i32>(0)))), call<i32, signature=fn(i32, ptr<i32>) -> i32>(%[[VALUE_bit]], read<i32>(%[[VALUE_runtime_2]]), addr_of<ptr<i32>>(%[[VALUE_x]])));
// O2-NEXT:     }
// O2-NEXT:     fn %[[VALUE_branch__const1]] @branch__const1(%[[VALUE_key]] key: ptr<@type[[TYPE_key]]>, %[[VALUE_likely]] likely: bool) -> i32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// O2-NEXT:         asm goto "# %c0 %c1" [dialect=att] [options=nostack] {
// O2-NEXT:             template: "# " %c0 " " %c1;
// O2-NEXT:             in 0 "i" [imm | sym] -> sym width 64 sym<offset=1>(%[[VALUE_k1]]);
// O2-NEXT:             in 1 "i" [imm | sym] -> imm width 8 const<bool>(true);
// O2-NEXT:             labels: %[[VALUE_yes]];
// O2-NEXT:         }
// O2-NEXT:         return const<i32>(0);
// O2-NEXT:         label %[[VALUE_yes]] yes:
// O2-NEXT:             return const<i32>(1);
// O2-NEXT:     }
// O2-NEXT:     fn %[[VALUE_branch__const2]] @branch__const2(%[[VALUE_key]] key: ptr<@type[[TYPE_key]]>, %[[VALUE_likely]] likely: bool) -> i32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// O2-NEXT:         asm goto "# %c0 %c1" [dialect=att] [options=nostack] {
// O2-NEXT:             template: "# " %c0 " " %c1;
// O2-NEXT:             in 0 "i" [imm | sym] -> sym width 64 sym<offset=0>(%[[VALUE_k2]]);
// O2-NEXT:             in 1 "i" [imm | sym] -> imm width 8 const<bool>(false);
// O2-NEXT:             labels: %[[VALUE_yes]];
// O2-NEXT:         }
// O2-NEXT:         return const<i32>(0);
// O2-NEXT:         label %[[VALUE_yes]] yes:
// O2-NEXT:             return const<i32>(1);
// O2-NEXT:     }
// O2-NEXT:     fn %[[VALUE_bit__const3]] @bit__const3(%[[VALUE_nr]] nr: i32, %[[VALUE_p]] p: ptr<i32>) -> i32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// O2-NEXT:         let %[[VALUE_out]] out: i32 [storage=automatic];
// O2-NEXT:         asm "# %1 %2" [dialect=att] [options=pure,nomem,nostack] {
// O2-NEXT:             template: "# " %1 " " %2;
// O2-NEXT:             lateout 0 "r" [reg] width 32 place<i32>(%[[VALUE_out]]);
// O2-NEXT:             in 1 "i" [imm | sym] -> imm width 32 const<i32>(2);
// O2-NEXT:             in 2 "r" [reg] width 64 read<ptr<i32>>(%[[VALUE_p]]);
// O2-NEXT:         }
// O2-NEXT:         return read<i32>(%[[VALUE_out]]);
// O2-NEXT:     }
// O2-NEXT:     fn %[[VALUE_outer__const4]] @outer__const4(%[[VALUE_key_2:[0-9]+]] key: ptr<@type[[TYPE_key]]>) -> i32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// O2-NEXT:         return call<i32, signature=fn(ptr<@type[[TYPE_key]]>, bool) -> i32>(%[[VALUE_branch__const2]], read<ptr<@type[[TYPE_key]]>>(%[[VALUE_key_2]]), ne<i32, reason=arg>(const<i32>(0), const<i32>(0)));
// O2-NEXT:     }
// O2-NEXT: }
// SLATE-FILECHECK-END O2
// SLATE-FILECHECK-BEGIN O0
// O0: module {
// O0-NEXT:     target "x86_64-unknown-linux-gnu" {
// O0-NEXT:         endian = little;
// O0-NEXT:         pointer [size=8, align=8];
// O0-NEXT:         stack_alignment = 16;
// O0-NEXT:         long_double = f80;
// O0-NEXT:         storage bool [size=1, align=1];
// O0-NEXT:         storage i8, u8 [size=1, align=1];
// O0-NEXT:         storage i16, u16 [size=2, align=2];
// O0-NEXT:         storage i32, u32 [size=4, align=4];
// O0-NEXT:         storage i64, u64 [size=8, align=8];
// O0-NEXT:         storage i128, u128 [size=16, align=16];
// O0-NEXT:         storage bf16 [size=2, align=2];
// O0-NEXT:         storage f16 [size=2, align=2];
// O0-NEXT:         storage f32 [size=4, align=4];
// O0-NEXT:         storage f64 [size=8, align=8];
// O0-NEXT:         storage f80 [size=16, align=16];
// O0-NEXT:         storage f128 [size=16, align=16];
// O0-NEXT:         storage d32 [size=4, align=4];
// O0-NEXT:         storage d64 [size=8, align=8];
// O0-NEXT:         storage d128 [size=16, align=16];
// O0-NEXT:     }
// O0-NEXT:     type @type[[TYPE_key:[0-9]+]] key = struct {
// O0-NEXT:         field0 enabled: i32;
// O0-NEXT:     } [size=4, align=4, offsets=[0]];
// O0-NEXT:     global %[[VALUE_k1:[0-9]+]] k1: @type[[TYPE_key]] [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_k2:[0-9]+]] k2: @type[[TYPE_key]] [storage=static] [linkage=external];
// O0-NEXT:     fn %[[VALUE_branch:[0-9]+]] @branch(%[[VALUE_key:[0-9]+]] key: ptr<@type[[TYPE_key]]>, %[[VALUE_likely:[0-9]+]] likely: bool) -> i32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// O0-NEXT:         asm goto "# %c0 %c1" [dialect=att] [options=nostack] [alternative=none] {
// O0-NEXT:             template: "# " %c0 " " %c1;
// O0-NEXT:             in 0 "i" [imm | sym] width 64 addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(pointer_cast<ptr<i8>, reason=explicit>(read<ptr<@type[[TYPE_key]]>>(%[[VALUE_key]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_likely]])))));
// O0-NEXT:             in 1 "i" [imm | sym] width 8 read<bool>(%[[VALUE_likely]]);
// O0-NEXT:             rejected: 0 (operand 0: not-constant);
// O0-NEXT:             labels: %[[VALUE_yes:[0-9]+]];
// O0-NEXT:         }
// O0-NEXT:         return const<i32>(0);
// O0-NEXT:         label %[[VALUE_yes]] yes:
// O0-NEXT:             return const<i32>(1);
// O0-NEXT:     }
// O0-NEXT:     fn %[[VALUE_bit:[0-9]+]] @bit(%[[VALUE_nr:[0-9]+]] nr: i32, %[[VALUE_p:[0-9]+]] p: ptr<i32>) -> i32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// O0-NEXT:         let %[[VALUE_out:[0-9]+]] out: i32 [storage=automatic];
// O0-NEXT:         asm "# %1 %2" [dialect=att] [options=pure,nomem,nostack] [alternative=none] {
// O0-NEXT:             template: "# " %1 " " %2;
// O0-NEXT:             lateout 0 "r" [reg] width 32 place<i32>(%[[VALUE_out]]);
// O0-NEXT:             in 1 "i" [imm | sym] width 32 shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), and<i32>(read<i32>(%[[VALUE_nr]]), const<i32>(7)));
// O0-NEXT:             in 2 "r" [reg] width 64 read<ptr<i32>>(%[[VALUE_p]]);
// O0-NEXT:             rejected: 0 (operand 1: not-constant);
// O0-NEXT:         }
// O0-NEXT:         return read<i32>(%[[VALUE_out]]);
// O0-NEXT:     }
// O0-NEXT:     fn %[[VALUE_outer:[0-9]+]] @outer(%[[VALUE_key_2:[0-9]+]] key: ptr<@type[[TYPE_key]]>) -> i32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// O0-NEXT:         return call<i32, signature=fn(ptr<@type[[TYPE_key]]>, bool) -> i32>(%[[VALUE_branch]], read<ptr<@type[[TYPE_key]]>>(%[[VALUE_key_2]]), ne<i32, reason=arg>(const<i32>(0), const<i32>(0)));
// O0-NEXT:     }
// O0-NEXT:     fn %[[VALUE_locals:[0-9]+]] @locals(%[[VALUE_runtime:[0-9]+]] runtime: i32) -> void [linkage=external] [fallthrough=ret_void] {
// O0-NEXT:         let %[[VALUE_flags:[0-9]+]] flags: i32 [storage=automatic] = or<i32>(const<i32>(256), const<i32>(3));
// O0-NEXT:         let %[[VALUE_wide:[0-9]+]] wide: i64 [storage=automatic] = shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i64>(1), const<i32>(40));
// O0-NEXT:         let %[[VALUE_maybe:[0-9]+]] maybe: i32 [storage=automatic] = const<i32>(1);
// O0-NEXT:         if ne<i32>(read<i32>(%[[VALUE_runtime]]), const<i32>(0))
// O0-NEXT:             write<i32>(%[[VALUE_maybe]], const<i32>(2));
// O0-NEXT:         asm "# %0 %1" [dialect=att] [options=nostack] [alternative=none] {
// O0-NEXT:             template: "# " %0 " " %1;
// O0-NEXT:             in 0 "i" [imm | sym] width 32 read<i32>(%[[VALUE_flags]]);
// O0-NEXT:             in 1 "i" [imm | sym] width 64 read<i64>(%[[VALUE_wide]]);
// O0-NEXT:             rejected: 0 (operand 0: not-constant);
// O0-NEXT:         }
// O0-NEXT:         asm "# %0" [dialect=att] [options=nostack] [alternative=none] {
// O0-NEXT:             template: "# " %0;
// O0-NEXT:             in 0 "i" [imm | sym] width 32 read<i32>(%[[VALUE_maybe]]);
// O0-NEXT:             rejected: 0 (operand 0: not-constant);
// O0-NEXT:         }
// O0-NEXT:     }
// O0-NEXT:     fn %[[VALUE_calls:[0-9]+]] @calls(%[[VALUE_runtime_2:[0-9]+]] runtime: i32, %[[VALUE_dynamic:[0-9]+]] dynamic: ptr<@type[[TYPE_key]]>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// O0-NEXT:         let %[[VALUE_x:[0-9]+]] x: i32 [storage=automatic] = const<i32>(0);
// O0-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(call<i32, signature=fn(ptr<@type[[TYPE_key]]>, bool) -> i32>(%[[VALUE_branch]], addr_of<ptr<@type[[TYPE_key]]>>(%[[VALUE_k1]]), ne<i32, reason=arg>(const<i32>(1), const<i32>(0))), call<i32, signature=fn(ptr<@type[[TYPE_key]]>, bool) -> i32>(%[[VALUE_branch]], addr_of<ptr<@type[[TYPE_key]]>>(%[[VALUE_k2]]), ne<i32, reason=arg>(const<i32>(0), const<i32>(0)))), call<i32, signature=fn(ptr<@type[[TYPE_key]]>, bool) -> i32>(%[[VALUE_branch]], addr_of<ptr<@type[[TYPE_key]]>>(%[[VALUE_k1]]), ne<i32, reason=arg>(const<i32>(1), const<i32>(0)))), call<i32, signature=fn(i32, ptr<i32>) -> i32>(%[[VALUE_bit]], const<i32>(9), addr_of<ptr<i32>>(%[[VALUE_x]]))), call<i32, signature=fn(ptr<@type[[TYPE_key]]>) -> i32>(%[[VALUE_outer]], addr_of<ptr<@type[[TYPE_key]]>>(%[[VALUE_k2]]))), call<i32, signature=fn(ptr<@type[[TYPE_key]]>, bool) -> i32>(%[[VALUE_branch]], read<ptr<@type[[TYPE_key]]>>(%[[VALUE_dynamic]]), ne<i32, reason=arg>(const<i32>(1), const<i32>(0)))), call<i32, signature=fn(i32, ptr<i32>) -> i32>(%[[VALUE_bit]], read<i32>(%[[VALUE_runtime_2]]), addr_of<ptr<i32>>(%[[VALUE_x]])));
// O0-NEXT:     }
// O0-NEXT: }
// SLATE-FILECHECK-END O0
