// SLATE-FILECHECK-DEFINES O2
// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-PREFIX-ARGS O2 -O2
struct key { int enabled; };
struct key k1;

static inline __attribute__((always_inline)) int branch(struct key *key) {
    asm goto("# %c0" : : "i"(key) : : yes);
    return 0;
yes:
    return 1;
}

static long do_fork(void) { return branch(&k1); }
long sys_fork(void) __attribute__((alias("do_fork")));

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
// O2-NEXT:     fn %[[VALUE_do_fork:[0-9]+]] @do_fork() -> i64 [linkage=internal] [fallthrough=ub_if_used] {
// O2-NEXT:         return widen<i64, reason=return>(call<i32, signature=fn(ptr<@type[[TYPE_key]]>) -> i32>(%[[VALUE_branch__const1:[0-9]+]], addr_of<ptr<@type[[TYPE_key]]>>(%[[VALUE_k1]])));
// O2-NEXT:     }
// O2-NEXT:     fn %[[VALUE_sys_fork:[0-9]+]] @sys_fork() -> i64 [linkage=external] [alias="do_fork"];
// O2-NEXT:     fn %[[VALUE_branch__const1]] @branch__const1(%[[VALUE_key:[0-9]+]] key: ptr<@type[[TYPE_key]]>) -> i32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// O2-NEXT:         asm goto "# %c0" [dialect=att] [options=nostack] {
// O2-NEXT:             template: "# " %c0;
// O2-NEXT:             in 0 "i" [imm | sym] -> sym width 64 sym<offset=0>(%[[VALUE_k1]]);
// O2-NEXT:             labels: %[[VALUE_yes:[0-9]+]];
// O2-NEXT:         }
// O2-NEXT:         return const<i32>(0);
// O2-NEXT:         label %[[VALUE_yes]] yes:
// O2-NEXT:             return const<i32>(1);
// O2-NEXT:     }
// O2-NEXT: }
// SLATE-FILECHECK-END O2
