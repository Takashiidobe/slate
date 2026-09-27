// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-STD IR c23
struct S { int a; int b[3]; } s;
_Thread_local int t;
int g, arr[4];
void fn(void);

void symbols(void) {
    static int local;
    asm("# %0 %1 %2 %3 %4" : : "i"(&g), "i"(fn), "i"(&arr[2]), "i"((char *)&g + 3), "i"(&local));
    asm("# %0 %1 %2 %3" : : "i"(&g - 1), "i"(&s.b[1]), "i"("abc"), "i"((long)&g));
    asm("# %0 %1" : : "ri"(&g), "g"(&g));
}

void rejected(int i) {
    int automatic;
    asm("# %0" : : "i"(&automatic));
    asm("# %0" : : "i"(&arr[i]));
    asm("# %0" : : "i"((int)(long)&g));
    asm("# %0" : : "i"(&t));
    asm("# %0" : : "rn"(&g));
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
// IR-NEXT:     type @type0 S = struct {
// IR-NEXT:         field0 a: i32;
// IR-NEXT:         field1 b: array<i32, 3>;
// IR-NEXT:     } [size=16, align=4, offsets=[0, 4]];
// IR-NEXT:     global %1 s: @type0 [storage=static] [linkage=external];
// IR-NEXT:     global %2 t: i32 [storage=thread] [linkage=external];
// IR-NEXT:     global %3 g: i32 [storage=static] [linkage=external];
// IR-NEXT:     global %4 arr: array<i32, 4> [storage=static] [align=16] [linkage=external];
// IR-NEXT:     global %7 local: i32 [storage=static] [linkage=internal];
// IR-NEXT:     global %11 .str11: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([97, 98, 99, 0]) [linkage=internal];
// IR-NEXT:     fn %5 @fn() -> void [linkage=external];
// IR-NEXT:     fn %6 @symbols() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         asm "# %0 %1 %2 %3 %4" [dialect=att] [options=nostack] {
// IR-NEXT:             template: "# " %0 " " %1 " " %2 " " %3 " " %4;
// IR-NEXT:             in 0 "i" [imm | sym] -> sym width 64 sym<offset=0>(%3);
// IR-NEXT:             in 1 "i" [imm | sym] -> sym width 64 sym<offset=0>(%5);
// IR-NEXT:             in 2 "i" [imm | sym] -> sym width 64 sym<offset=8>(%4);
// IR-NEXT:             in 3 "i" [imm | sym] -> sym width 64 sym<offset=3>(%3);
// IR-NEXT:             in 4 "i" [imm | sym] -> sym width 64 sym<offset=0>(%7);
// IR-NEXT:         }
// IR-NEXT:         asm "# %0 %1 %2 %3" [dialect=att] [options=nostack] {
// IR-NEXT:             template: "# " %0 " " %1 " " %2 " " %3;
// IR-NEXT:             in 0 "i" [imm | sym] -> sym width 64 sym<offset=-4>(%3);
// IR-NEXT:             in 1 "i" [imm | sym] -> sym width 64 sym<offset=8>(%1);
// IR-NEXT:             in 2 "i" [imm | sym] -> sym width 64 sym<offset=0>(%11);
// IR-NEXT:             in 3 "i" [imm | sym] -> sym width 64 sym<offset=0>(%3);
// IR-NEXT:         }
// IR-NEXT:         asm "# %0 %1" [dialect=att] [options=nostack] {
// IR-NEXT:             template: "# " %0 " " %1;
// IR-NEXT:             in 0 "ri" [reg | imm | sym] -> sym width 64 sym<offset=0>(%3);
// IR-NEXT:             in 1 "g" [reg | mem | imm | sym] -> sym width 64 sym<offset=0>(%3);
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT:     fn %8 @rejected(%9 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         let %10 automatic: i32 [storage=automatic];
// IR-NEXT:         asm "# %0" [dialect=att] [options=nostack] [alternative=none] {
// IR-NEXT:             template: "# " %0;
// IR-NEXT:             in 0 "i" [imm | sym] width 64 addr_of<ptr<i32>>(%10);
// IR-NEXT:             rejected: 0 (operand 0: not-constant);
// IR-NEXT:         }
// IR-NEXT:         asm "# %0" [dialect=att] [options=nostack] [alternative=none] {
// IR-NEXT:             template: "# " %0;
// IR-NEXT:             in 0 "i" [imm | sym] width 64 addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%4), read<i32>(%9))));
// IR-NEXT:             rejected: 0 (operand 0: not-constant);
// IR-NEXT:         }
// IR-NEXT:         asm "# %0" [dialect=att] [options=nostack] [alternative=none] {
// IR-NEXT:             template: "# " %0;
// IR-NEXT:             in 0 "i" [imm | sym] width 32 truncate<i32, reason=explicit, fits=unknown>(ptr_to_int<i64, reason=explicit>(addr_of<ptr<i32>>(%3)));
// IR-NEXT:             rejected: 0 (operand 0: not-constant);
// IR-NEXT:         }
// IR-NEXT:         asm "# %0" [dialect=att] [options=nostack] [alternative=none] {
// IR-NEXT:             template: "# " %0;
// IR-NEXT:             in 0 "i" [imm | sym] width 64 addr_of<ptr<i32>>(%2);
// IR-NEXT:             rejected: 0 (operand 0: not-constant);
// IR-NEXT:         }
// IR-NEXT:         asm "# %0" [dialect=att] [options=nostack] {
// IR-NEXT:             template: "# " %0;
// IR-NEXT:             in 0 "rn" [reg | imm] -> reg width 64 addr_of<ptr<i32>>(%3);
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
