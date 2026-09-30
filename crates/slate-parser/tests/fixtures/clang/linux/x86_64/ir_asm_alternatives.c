// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-STD IR c23
struct Big { long a, b, c; };

void alternatives(int x, int *p, long double f, struct Big big) {
    asm("# %0 %1" : "=r,m"(x) : "r,m"(*p));
    asm("# %0 %1" : "=r,m"(x) : "0,m"(*p));
    asm("# %0 %1" : "=f,m"(f) : "0,m"(f));
    asm("# %0 %1" : : "rm"(big), "rmi"(x + 1));
    asm("# %0 %1" : : "ri"(42), "g"(sizeof big));
    asm("# %0 %1" : : "i,r"(x), "r,m"(x));
    asm("# %0 %1" : : "l,x"(x), "r,r"(x));
    asm("# %0" : : "t"(f));
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
// IR-NEXT:     type @type[[TYPE_Big:[0-9]+]] Big = struct {
// IR-NEXT:         field0 a: i64;
// IR-NEXT:         field1 b: i64;
// IR-NEXT:         field2 c: i64;
// IR-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// IR-NEXT:     fn %[[VALUE_alternatives:[0-9]+]] @alternatives(%[[VALUE_x:[0-9]+]] x: i32, %[[VALUE_p:[0-9]+]] p: ptr<i32>, %[[VALUE_f:[0-9]+]] f: f80, %[[VALUE_big:[0-9]+]] big: @type[[TYPE_Big]]) -> void [linkage=external] [abi=sysv64(scalar, scalar, scalar, native_c) -> void] [fallthrough=ret_void] {
// IR-NEXT:         asm "# %0 %1" [dialect=att] [options=pure,nomem,nostack] [alternative=0] {
// IR-NEXT:             template: "# " %0 " " %1;
// IR-NEXT:             lateout 0 "r,m" [reg, mem] width 32 place<i32>(%[[VALUE_x]]);
// IR-NEXT:             in 1 "r,m" [reg, mem] width 32 read<i32>(deref(read<ptr<i32>>(%[[VALUE_p]])));
// IR-NEXT:         }
// IR-NEXT:         asm "# %0 %1" [dialect=att] [options=pure,nomem,nostack] [alternative=0] {
// IR-NEXT:             template: "# " %0 " " %0;
// IR-NEXT:             inlateout 0 "r,m" [reg, mem] width 32 place<i32>(%[[VALUE_x]]) from read<i32>(deref(read<ptr<i32>>(%[[VALUE_p]])));
// IR-NEXT:         }
// IR-NEXT:         asm "# %0 %1" [dialect=att] [options=nostack] [alternative=1] {
// IR-NEXT:             template: "# " %0 " " %1;
// IR-NEXT:             lateout 0 "f,m" [x87_reg, mem] width 128 place<f80>(%[[VALUE_f]]);
// IR-NEXT:             in 1 "0,m" [0, mem] width 128 place<f80>(%[[VALUE_f]]);
// IR-NEXT:             rejected: 0 (operand 0: clobber-only);
// IR-NEXT:         }
// IR-NEXT:         asm "# %0 %1" [dialect=att] [options=nostack] {
// IR-NEXT:             template: "# " %0 " " %1;
// IR-NEXT:             in 0 "rm" [reg | mem] -> mem width 192 place<@type[[TYPE_Big]]>(%[[VALUE_big]]);
// IR-NEXT:             in 1 "rmi" [reg | mem | imm | sym] -> reg width 32 add<i32, overflow=ub>(read<i32>(%[[VALUE_x]]), const<i32>(1));
// IR-NEXT:         }
// IR-NEXT:         asm "# %0 %1" [dialect=att] [options=nostack] {
// IR-NEXT:             template: "# " %0 " " %1;
// IR-NEXT:             in 0 "ri" [reg | imm | sym] -> imm width 32 const<i32>(42);
// IR-NEXT:             in 1 "g" [reg | mem | imm | sym] -> imm width 64 const<u64>(24);
// IR-NEXT:         }
// IR-NEXT:         asm "# %0 %1" [dialect=att] [options=nostack] [alternative=1] {
// IR-NEXT:             template: "# " %0 " " %1;
// IR-NEXT:             in 0 "i,r" [imm | sym, reg] width 32 read<i32>(%[[VALUE_x]]);
// IR-NEXT:             in 1 "r,m" [reg, mem] width 32 place<i32>(%[[VALUE_x]]);
// IR-NEXT:             rejected: 0 (operand 0: not-constant);
// IR-NEXT:         }
// IR-NEXT:         asm "# %0 %1" [dialect=att] [options=nostack] [alternative=1] {
// IR-NEXT:             template: "# " %0 " " %1;
// IR-NEXT:             in 0 "l,x" [unresolved("l"), xmm_reg] width 32 read<i32>(%[[VALUE_x]]);
// IR-NEXT:             in 1 "r,r" [reg, reg] width 32 read<i32>(%[[VALUE_x]]);
// IR-NEXT:             rejected: 0 (operand 0: unresolved("l"));
// IR-NEXT:         }
// IR-NEXT:         asm "# %0" [dialect=att] [options=nostack] [alternative=none] {
// IR-NEXT:             template: "# " %0;
// IR-NEXT:             in 0 "t" [{st}] width 128 read<f80>(%[[VALUE_f]]);
// IR-NEXT:             rejected: 0 (operand 0: clobber-only);
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
