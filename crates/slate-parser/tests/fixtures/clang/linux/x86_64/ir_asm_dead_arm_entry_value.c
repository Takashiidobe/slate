// SLATE-FILECHECK-DEFINES O2
// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-PREFIX-ARGS O2 -O2
struct key { int enabled; };
struct key keys[2][3];
unsigned long word;
char table[32];
void handler(void);

static inline __attribute__((always_inline)) int branch(struct key *key) {
    asm goto("# %c0" : : "i"(key) : : yes);
    return 0;
yes:
    return 1;
}

static inline int hook(unsigned char pf, unsigned int nr) {
    if (__builtin_constant_p(pf) && __builtin_constant_p(nr) && !branch(&keys[pf][nr]))
        return 1;
    return 2;
}

static inline __attribute__((always_inline)) int constant_bit(long nr) {
    int out;
    asm("# %1" : "=r"(out) : "i"(1 << (nr & 7)));
    return out;
}

static inline __attribute__((always_inline)) int test_bit(long nr) {
    return __builtin_constant_p(nr) ? constant_bit(nr) : (int)(word >> nr);
}

static inline __attribute__((always_inline)) void *rip_rel(void *p) {
    asm("leaq %c1(%%rip), %0" : "=r"(p) : "i"(p));
    return p;
}

int f(unsigned char pf, unsigned int nr, long bit) {
    void *h = 0;
    if (0)
        h = rip_rel(handler);
    return hook(pf, nr) + test_bit(bit) + (h != 0);
}

void *g(void) { return rip_rel((void *)(unsigned long)&table[1]); }

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
// O2-NEXT:     global %[[VALUE_keys:[0-9]+]] keys: array<array<@type[[TYPE_key]], 3>, 2> [storage=static] [align=16] [linkage=external];
// O2-NEXT:     global %[[VALUE_word:[0-9]+]] word: u64 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_table:[0-9]+]] table: array<i8, 32> [storage=static] [align=16] [linkage=external];
// O2-NEXT:     fn %[[VALUE_handler:[0-9]+]] @handler() -> void [linkage=external];
// O2-NEXT:     fn %[[VALUE_hook:[0-9]+]] @hook(%[[VALUE_pf:[0-9]+]] pf: u8, %[[VALUE_nr:[0-9]+]] nr: u32) -> i32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// O2-NEXT:         {
// O2-NEXT:         }
// O2-NEXT:         return const<i32>(2);
// O2-NEXT:     }
// O2-NEXT:     fn %[[VALUE_test_bit:[0-9]+]] @test_bit(%[[VALUE_nr_2:[0-9]+]] nr: i64) -> i32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// O2-NEXT:         return reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%[[VALUE_word]]), read<i64>(%[[VALUE_nr_2]]))));
// O2-NEXT:     }
// O2-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_pf_2:[0-9]+]] pf: u8, %[[VALUE_nr_3:[0-9]+]] nr: u32, %[[VALUE_bit:[0-9]+]] bit: i64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// O2-NEXT:         let %[[VALUE_h:[0-9]+]] h: ptr<void> [storage=automatic] = null<ptr<void>>;
// O2-NEXT:         {
// O2-NEXT:         }
// O2-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(call<i32, signature=fn(u8, u32) -> i32>(%[[VALUE_hook]], read<u8>(%[[VALUE_pf_2]]), read<u32>(%[[VALUE_nr_3]])), call<i32, signature=fn(i64) -> i32>(%[[VALUE_test_bit]], read<i64>(%[[VALUE_bit]]))), from_bool<i32, reason=promotion>(ne<ptr<void>>(read<ptr<void>>(%[[VALUE_h]]), null<ptr<void>>)));
// O2-NEXT:     }
// O2-NEXT:     fn %[[VALUE_g:[0-9]+]] @g() -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// O2-NEXT:         return call<ptr<void>, signature=fn(ptr<void>) -> ptr<void>>(%[[VALUE_rip_rel__const1:[0-9]+]], int_to_ptr<ptr<void>, reason=explicit>(ptr_to_int<u64, reason=explicit>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(32)>(%[[VALUE_table]]), const<i32>(1)))))));
// O2-NEXT:     }
// O2-NEXT:     fn %[[VALUE_rip_rel__const1]] @rip_rel__const1(%[[VALUE_p:[0-9]+]] p: ptr<void>) -> ptr<void> [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// O2-NEXT:         asm "leaq %c1(%%rip), %0" [dialect=att] [options=pure,nomem,nostack] {
// O2-NEXT:             template: "leaq " %c1 "(" %% "rip), " %0;
// O2-NEXT:             lateout 0 "r" [reg] width 64 place<ptr<void>>(%[[VALUE_p]]);
// O2-NEXT:             in 1 "i" [imm | sym] -> sym width 64 sym<offset=1>(%[[VALUE_table]]);
// O2-NEXT:         }
// O2-NEXT:         return read<ptr<void>>(%[[VALUE_p]]);
// O2-NEXT:     }
// O2-NEXT: }
// SLATE-FILECHECK-END O2
