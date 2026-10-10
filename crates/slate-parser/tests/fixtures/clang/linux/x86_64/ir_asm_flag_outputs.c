// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-STD IR c23
void conditions(int x, int y, int *r) {
    asm("cmpl %11, %10" : "=@cca"(r[0]), "=@ccae"(r[1]), "=@ccb"(r[2]), "=@ccbe"(r[3]), "=@ccc"(r[4]), "=@cce"(r[5]), "=@ccg"(r[6]), "=@ccge"(r[7]), "=@ccl"(r[8]), "=@ccle"(r[9]) : "r"(x), "r"(y));
    asm("cmpl %11, %10" : "=@ccna"(r[0]), "=@ccnae"(r[1]), "=@ccnb"(r[2]), "=@ccnbe"(r[3]), "=@ccnc"(r[4]), "=@ccne"(r[5]), "=@ccng"(r[6]), "=@ccnge"(r[7]), "=@ccnl"(r[8]), "=@ccnle"(r[9]) : "r"(x), "r"(y));
    asm("cmpl %9, %8" : "=@ccno"(r[0]), "=@ccnp"(r[1]), "=@ccns"(r[2]), "=@ccnz"(r[3]), "=@cco"(r[4]), "=@ccp"(r[5]), "=@ccs"(r[6]), "=@ccz"(r[7]) : "r"(x), "r"(y));
}

void widths(int x, _Bool *b, long *l, char *c) {
    asm("testl %3, %3" : "=@ccz"(*b), "=@ccs"(*l), "=&@ccne"(*c) : "r"(x));
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
// IR-NEXT:     fn %[[VALUE_conditions:[0-9]+]] @conditions(%[[VALUE_x:[0-9]+]] x: i32, %[[VALUE_y:[0-9]+]] y: i32, %[[VALUE_r:[0-9]+]] r: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         asm "cmpl %11, %10" [dialect=att] [options=pure,nomem,nostack] {
// IR-NEXT:             template: "cmpl " %11 ", " %10;
// IR-NEXT:             lateout 0 "@cca" [cc(a)] width 32 place<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_r]]), const<i32>(0))));
// IR-NEXT:             lateout 1 "@ccae" [cc(ae)] width 32 place<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_r]]), const<i32>(1))));
// IR-NEXT:             lateout 2 "@ccb" [cc(b)] width 32 place<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_r]]), const<i32>(2))));
// IR-NEXT:             lateout 3 "@ccbe" [cc(be)] width 32 place<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_r]]), const<i32>(3))));
// IR-NEXT:             lateout 4 "@ccc" [cc(c)] width 32 place<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_r]]), const<i32>(4))));
// IR-NEXT:             lateout 5 "@cce" [cc(e)] width 32 place<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_r]]), const<i32>(5))));
// IR-NEXT:             lateout 6 "@ccg" [cc(g)] width 32 place<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_r]]), const<i32>(6))));
// IR-NEXT:             lateout 7 "@ccge" [cc(ge)] width 32 place<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_r]]), const<i32>(7))));
// IR-NEXT:             lateout 8 "@ccl" [cc(l)] width 32 place<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_r]]), const<i32>(8))));
// IR-NEXT:             lateout 9 "@ccle" [cc(le)] width 32 place<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_r]]), const<i32>(9))));
// IR-NEXT:             in 10 "r" [reg] width 32 read<i32>(%[[VALUE_x]]);
// IR-NEXT:             in 11 "r" [reg] width 32 read<i32>(%[[VALUE_y]]);
// IR-NEXT:         }
// IR-NEXT:         asm "cmpl %11, %10" [dialect=att] [options=pure,nomem,nostack] {
// IR-NEXT:             template: "cmpl " %11 ", " %10;
// IR-NEXT:             lateout 0 "@ccna" [cc(na)] width 32 place<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_r]]), const<i32>(0))));
// IR-NEXT:             lateout 1 "@ccnae" [cc(nae)] width 32 place<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_r]]), const<i32>(1))));
// IR-NEXT:             lateout 2 "@ccnb" [cc(nb)] width 32 place<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_r]]), const<i32>(2))));
// IR-NEXT:             lateout 3 "@ccnbe" [cc(nbe)] width 32 place<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_r]]), const<i32>(3))));
// IR-NEXT:             lateout 4 "@ccnc" [cc(nc)] width 32 place<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_r]]), const<i32>(4))));
// IR-NEXT:             lateout 5 "@ccne" [cc(ne)] width 32 place<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_r]]), const<i32>(5))));
// IR-NEXT:             lateout 6 "@ccng" [cc(ng)] width 32 place<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_r]]), const<i32>(6))));
// IR-NEXT:             lateout 7 "@ccnge" [cc(nge)] width 32 place<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_r]]), const<i32>(7))));
// IR-NEXT:             lateout 8 "@ccnl" [cc(nl)] width 32 place<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_r]]), const<i32>(8))));
// IR-NEXT:             lateout 9 "@ccnle" [cc(nle)] width 32 place<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_r]]), const<i32>(9))));
// IR-NEXT:             in 10 "r" [reg] width 32 read<i32>(%[[VALUE_x]]);
// IR-NEXT:             in 11 "r" [reg] width 32 read<i32>(%[[VALUE_y]]);
// IR-NEXT:         }
// IR-NEXT:         asm "cmpl %9, %8" [dialect=att] [options=pure,nomem,nostack] {
// IR-NEXT:             template: "cmpl " %9 ", " %8;
// IR-NEXT:             lateout 0 "@ccno" [cc(no)] width 32 place<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_r]]), const<i32>(0))));
// IR-NEXT:             lateout 1 "@ccnp" [cc(np)] width 32 place<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_r]]), const<i32>(1))));
// IR-NEXT:             lateout 2 "@ccns" [cc(ns)] width 32 place<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_r]]), const<i32>(2))));
// IR-NEXT:             lateout 3 "@ccnz" [cc(nz)] width 32 place<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_r]]), const<i32>(3))));
// IR-NEXT:             lateout 4 "@cco" [cc(o)] width 32 place<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_r]]), const<i32>(4))));
// IR-NEXT:             lateout 5 "@ccp" [cc(p)] width 32 place<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_r]]), const<i32>(5))));
// IR-NEXT:             lateout 6 "@ccs" [cc(s)] width 32 place<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_r]]), const<i32>(6))));
// IR-NEXT:             lateout 7 "@ccz" [cc(z)] width 32 place<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_r]]), const<i32>(7))));
// IR-NEXT:             in 8 "r" [reg] width 32 read<i32>(%[[VALUE_x]]);
// IR-NEXT:             in 9 "r" [reg] width 32 read<i32>(%[[VALUE_y]]);
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_widths:[0-9]+]] @widths(%[[VALUE_x_2:[0-9]+]] x: i32, %[[VALUE_b:[0-9]+]] b: ptr<bool>, %[[VALUE_l:[0-9]+]] l: ptr<i64>, %[[VALUE_c:[0-9]+]] c: ptr<i8>) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         asm "testl %3, %3" [dialect=att] [options=pure,nomem,nostack] {
// IR-NEXT:             template: "testl " %3 ", " %3;
// IR-NEXT:             lateout 0 "@ccz" [cc(z)] width 8 place<bool>(deref(read<ptr<bool>>(%[[VALUE_b]])));
// IR-NEXT:             lateout 1 "@ccs" [cc(s)] width 64 place<i64>(deref(read<ptr<i64>>(%[[VALUE_l]])));
// IR-NEXT:             out 2 "@ccne" [cc(ne)] width 8 place<i8>(deref(read<ptr<i8>>(%[[VALUE_c]])));
// IR-NEXT:             in 3 "r" [reg] width 32 read<i32>(%[[VALUE_x_2]]);
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
