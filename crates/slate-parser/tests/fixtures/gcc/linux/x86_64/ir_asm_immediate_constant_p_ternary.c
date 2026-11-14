// SLATE-FILECHECK-DEFINES O0
// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-PREFIX-ARGS O0 -O0
static inline __attribute__((const)) int __ilog2_u32(unsigned n) { return 31 - __builtin_clz(n); }
static inline __attribute__((const)) int __ilog2_u64(unsigned long long n) { return 63 - __builtin_clzll(n); }
#define ilog2(n) (__builtin_constant_p(n) ? ((n) < 2 ? 0 : 63 - __builtin_clzll(n)) : (sizeof(n) <= 4) ? __ilog2_u32(n) : __ilog2_u64(n))
static inline __attribute__((const)) int __order_base_2(unsigned long n) { return n > 1 ? ilog2(n - 1) + 1 : 0; }
#define order_base_2(n) (__builtin_constant_p(n) ? (((n) == 0 || (n) == 1) ? 0 : ilog2((n) - 1) + 1) : __order_base_2(n))

int nr_cpus_bits(void) {
    int v;
    __asm__("mov %1, %0" : "=r"(v) : "i"(order_base_2(64)));
    return v;
}

int popcount_operand(unsigned x) {
    int v;
    __asm__("mov %1, %0" : "=r"(v) : "ir"(__builtin_popcount(x)), "ir"(__builtin_popcount(0xf0u)));
    return v;
}

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
// O0-NEXT:     fn %[[VALUE___builtin_clz:[0-9]+]] @__builtin_clz(%[[VALUE0:[0-9]+]] <unnamed>: u32) -> i32 [linkage=external] [memory=none];
// O0-NEXT:     fn %[[VALUE___ilog2_u32:[0-9]+]] @__ilog2_u32(%[[VALUE_n:[0-9]+]] n: u32) -> i32 [linkage=internal] [inline=hint] [definition=emitted] [memory=none] [fallthrough=ub_if_used] {
// O0-NEXT:         return sub<i32, overflow=ub>(const<i32>(31), call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_clz]], read<u32>(%[[VALUE_n]])));
// O0-NEXT:     }
// O0-NEXT:     fn %[[VALUE___builtin_clzll:[0-9]+]] @__builtin_clzll(%[[VALUE1:[0-9]+]] <unnamed>: u64) -> i32 [linkage=external] [memory=none];
// O0-NEXT:     fn %[[VALUE___ilog2_u64:[0-9]+]] @__ilog2_u64(%[[VALUE_n_2:[0-9]+]] n: u64) -> i32 [linkage=internal] [inline=hint] [definition=emitted] [memory=none] [fallthrough=ub_if_used] {
// O0-NEXT:         return sub<i32, overflow=ub>(const<i32>(63), call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_clzll]], read<u64>(%[[VALUE_n_2]])));
// O0-NEXT:     }
// O0-NEXT:     fn %[[VALUE___order_base_2:[0-9]+]] @__order_base_2(%[[VALUE_n_3:[0-9]+]] n: u64) -> i32 [linkage=internal] [inline=hint] [definition=emitted] [memory=none] [fallthrough=ub_if_used] {
// O0-NEXT:         return conditional<i32>(gt<u64>(read<u64>(%[[VALUE_n_3]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), add<i32, overflow=ub>(conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), conditional<i32>(lt<u64>(sub<u64, overflow=wrap>(read<u64>(%[[VALUE_n_3]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), const<i32>(0), sub<i32, overflow=ub>(const<i32>(63), call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_clzll]], reinterpret<u64, reason=arg>(sub<u64, overflow=wrap>(read<u64>(%[[VALUE_n_3]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))))), conditional<i32>(le<u64>(size_of<u64, u64>(8) [c="__size_t"], reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))) [c="__size_t"]), call<i32, signature=fn(u32) -> i32>(%[[VALUE___ilog2_u32]], truncate<u32, reason=arg, fits=unknown>(sub<u64, overflow=wrap>(read<u64>(%[[VALUE_n_3]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))), call<i32, signature=fn(u64) -> i32>(%[[VALUE___ilog2_u64]], reinterpret<u64, reason=arg>(sub<u64, overflow=wrap>(read<u64>(%[[VALUE_n_3]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))))), const<i32>(1)), const<i32>(0));
// O0-NEXT:     }
// O0-NEXT:     fn %[[VALUE_nr_cpus_bits:[0-9]+]] @nr_cpus_bits() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// O0-NEXT:         let %[[VALUE_v:[0-9]+]] v: i32 [storage=automatic];
// O0-NEXT:         asm "mov %1, %0" [dialect=att] [options=pure,nomem,nostack] {
// O0-NEXT:             template: "mov " %1 ", " %0;
// O0-NEXT:             lateout 0 "r" [reg] width 32 place<i32>(%[[VALUE_v]]);
// O0-NEXT:             in 1 "i" [imm | sym] -> imm width 32 const<i32>(6);
// O0-NEXT:         }
// O0-NEXT:         return read<i32>(%[[VALUE_v]]);
// O0-NEXT:     }
// O0-NEXT:     fn %[[VALUE___builtin_popcount:[0-9]+]] @__builtin_popcount(%[[VALUE2:[0-9]+]] <unnamed>: u32) -> i32 [linkage=external] [memory=none];
// O0-NEXT:     fn %[[VALUE_popcount_operand:[0-9]+]] @popcount_operand(%[[VALUE_x:[0-9]+]] x: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// O0-NEXT:         let %[[VALUE_v_2:[0-9]+]] v: i32 [storage=automatic];
// O0-NEXT:         asm "mov %1, %0" [dialect=att] [options=pure,nomem,nostack] {
// O0-NEXT:             template: "mov " %1 ", " %0;
// O0-NEXT:             lateout 0 "r" [reg] width 32 place<i32>(%[[VALUE_v_2]]);
// O0-NEXT:             in 1 "ir" [imm | sym | reg] -> reg width 32 call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_popcount]], read<u32>(%[[VALUE_x]]));
// O0-NEXT:             in 2 "ir" [imm | sym | reg] -> imm width 32 const<i32>(4);
// O0-NEXT:         }
// O0-NEXT:         return read<i32>(%[[VALUE_v_2]]);
// O0-NEXT:     }
// O0-NEXT: }
// SLATE-FILECHECK-END O0
