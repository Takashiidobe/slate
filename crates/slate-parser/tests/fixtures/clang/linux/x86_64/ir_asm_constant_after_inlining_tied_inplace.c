// SLATE-FILECHECK-DEFINES O2
// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-PREFIX-ARGS O2 -O2
static inline __attribute__((always_inline)) unsigned add_imm(unsigned x, int n) {
    unsigned out;
    __asm__("addl %2, %0" : "=r"(out) : "0"(x), "i"(n));
    return out;
}
static inline __attribute__((always_inline)) unsigned add_inplace(unsigned x, int n) {
    __asm__("addl %1, %0" : "+r"(x) : "i"(n));
    return x;
}
static inline __attribute__((always_inline)) int counted(int n) {
    static int calls;
    int out;
    calls++;
    __asm__("movl %1, %0" : "=r"(out) : "i"(n));
    return out + calls;
}
static inline __attribute__((always_inline)) unsigned load_g(unsigned *p, int n) {
    unsigned out;
    __asm__("movl %1, %0\n\taddl %2, %0" : "=r"(out) : "g"(*p), "i"(n));
    return out;
}
unsigned use(unsigned *p) { return add_imm(40, 2) + add_inplace(1, 5) + counted(3) + counted(4) + load_g(p, 3); }

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
// O2-NEXT:     global %[[VALUE_calls:[0-9]+]] calls: i32 [storage=static] [linkage=internal];
// O2-NEXT:     fn %[[VALUE_use:[0-9]+]] @use(%[[VALUE_p:[0-9]+]] p: ptr<u32>) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// O2-NEXT:         return add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(call<u32, signature=fn(u32, i32) -> u32>(%[[VALUE_add_imm__const1:[0-9]+]], reinterpret<u32, reason=arg, fits=always>(const<i32>(40)), const<i32>(2)), call<u32, signature=fn(u32, i32) -> u32>(%[[VALUE_add_inplace__const2:[0-9]+]], reinterpret<u32, reason=arg, fits=always>(const<i32>(1)), const<i32>(5))), reinterpret<u32, reason=usual_arith, fits=unknown>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_counted__const3:[0-9]+]], const<i32>(3)))), reinterpret<u32, reason=usual_arith, fits=unknown>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_counted__const4:[0-9]+]], const<i32>(4)))), call<u32, signature=fn(ptr<u32>, i32) -> u32>(%[[VALUE_load_g__const5:[0-9]+]], read<ptr<u32>>(%[[VALUE_p]]), const<i32>(3)));
// O2-NEXT:     }
// O2-NEXT:     fn %[[VALUE_add_imm__const1]] @add_imm__const1(%[[VALUE_x:[0-9]+]] x: u32, %[[VALUE_n:[0-9]+]] n: i32) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// O2-NEXT:         let %[[VALUE_out:[0-9]+]] out: u32 [storage=automatic];
// O2-NEXT:         asm "addl %2, %0" [dialect=att] [options=pure,nomem,nostack] {
// O2-NEXT:             template: "addl " %1 ", " %0;
// O2-NEXT:             inlateout 0 "r" [reg] width 32 place<u32>(%[[VALUE_out]]) from read<u32>(%[[VALUE_x]]);
// O2-NEXT:             in 1 "i" [imm | sym] -> imm width 32 const<i32>(2);
// O2-NEXT:         }
// O2-NEXT:         return read<u32>(%[[VALUE_out]]);
// O2-NEXT:     }
// O2-NEXT:     fn %[[VALUE_add_inplace__const2]] @add_inplace__const2(%[[VALUE_x_2:[0-9]+]] x: u32, %[[VALUE_n_2:[0-9]+]] n: i32) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// O2-NEXT:         asm "addl %1, %0" [dialect=att] [options=pure,nomem,nostack] {
// O2-NEXT:             template: "addl " %1 ", " %0;
// O2-NEXT:             inlateout 0 "r" [reg] width 32 place<u32>(%[[VALUE_x_2]]);
// O2-NEXT:             in 1 "i" [imm | sym] -> imm width 32 const<i32>(5);
// O2-NEXT:         }
// O2-NEXT:         return read<u32>(%[[VALUE_x_2]]);
// O2-NEXT:     }
// O2-NEXT:     fn %[[VALUE_counted__const3]] @counted__const3(%[[VALUE_n_3:[0-9]+]] n: i32) -> i32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// O2-NEXT:         let %[[VALUE_out_2:[0-9]+]] out: i32 [storage=automatic];
// O2-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_calls]]);
// O2-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE0]]), const<i32>(1));
// O2-NEXT:         write<i32>(%[[VALUE_calls]], read<i32>(%[[VALUE1]]));
// O2-NEXT:         asm "movl %1, %0" [dialect=att] [options=pure,nomem,nostack] {
// O2-NEXT:             template: "movl " %1 ", " %0;
// O2-NEXT:             lateout 0 "r" [reg] width 32 place<i32>(%[[VALUE_out_2]]);
// O2-NEXT:             in 1 "i" [imm | sym] -> imm width 32 const<i32>(3);
// O2-NEXT:         }
// O2-NEXT:         return add<i32, overflow=ub>(read<i32>(%[[VALUE_out_2]]), read<i32>(%[[VALUE_calls]]));
// O2-NEXT:     }
// O2-NEXT:     fn %[[VALUE_counted__const4]] @counted__const4(%[[VALUE_n_3]] n: i32) -> i32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// O2-NEXT:         let %[[VALUE_out_2]] out: i32 [storage=automatic];
// O2-NEXT:         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_calls]]);
// O2-NEXT:         let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// O2-NEXT:         write<i32>(%[[VALUE_calls]], read<i32>(%[[VALUE3]]));
// O2-NEXT:         asm "movl %1, %0" [dialect=att] [options=pure,nomem,nostack] {
// O2-NEXT:             template: "movl " %1 ", " %0;
// O2-NEXT:             lateout 0 "r" [reg] width 32 place<i32>(%[[VALUE_out_2]]);
// O2-NEXT:             in 1 "i" [imm | sym] -> imm width 32 const<i32>(4);
// O2-NEXT:         }
// O2-NEXT:         return add<i32, overflow=ub>(read<i32>(%[[VALUE_out_2]]), read<i32>(%[[VALUE_calls]]));
// O2-NEXT:     }
// O2-NEXT:     fn %[[VALUE_load_g__const5]] @load_g__const5(%[[VALUE_p_2:[0-9]+]] p: ptr<u32>, %[[VALUE_n_4:[0-9]+]] n: i32) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// O2-NEXT:         let %[[VALUE_out_3:[0-9]+]] out: u32 [storage=automatic];
// O2-NEXT:         asm "movl %1, %0\n\taddl %2, %0" [dialect=att] [options=pure,readonly,nostack] {
// O2-NEXT:             template: "movl " %1 ", " %0 "\n\taddl " %2 ", " %0;
// O2-NEXT:             lateout 0 "r" [reg] width 32 place<u32>(%[[VALUE_out_3]]);
// O2-NEXT:             in 1 "g" [reg | mem | imm | sym] -> mem width 32 place<u32>(deref(read<ptr<u32>>(%[[VALUE_p_2]])));
// O2-NEXT:             in 2 "i" [imm | sym] -> imm width 32 const<i32>(3);
// O2-NEXT:         }
// O2-NEXT:         return read<u32>(%[[VALUE_out_3]]);
// O2-NEXT:     }
// O2-NEXT: }
// SLATE-FILECHECK-END O2
