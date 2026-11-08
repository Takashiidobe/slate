// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-STD IR gnu23
int flags(int a, int b) {
    int eq, ne, cs, hs, cc, lo, mi, pl, vs, vc, hi, ls, ge, lt, gt, le;
    asm("cmp %[a], %[b]"
        : "=@cceq"(eq), "=@ccne"(ne), "=@cccs"(cs), "=@cchs"(hs), "=@cccc"(cc), "=@cclo"(lo),
          "=@ccmi"(mi), "=@ccpl"(pl), "=@ccvs"(vs), "=@ccvc"(vc), "=@cchi"(hi), "=@ccls"(ls),
          "=@ccge"(ge), "=@cclt"(lt), "=@ccgt"(gt), "=@ccle"(le)
        : [a] "r"(a), [b] "r"(b));
    return eq + ne + cs + hs + cc + lo + mi + pl + vs + vc + hi + ls + ge + lt + gt + le;
}

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "armv7-unknown-linux-gnueabihf" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=4, align=4];
// IR-NEXT:         stack_alignment = 8;
// IR-NEXT:         long_double = f64;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_flags:[0-9]+]] @flags(%[[VALUE_a:[0-9]+]] a: i32, %[[VALUE_b:[0-9]+]] b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_eq:[0-9]+]] eq: i32 [storage=automatic];
// IR-NEXT:         let %[[VALUE_ne:[0-9]+]] ne: i32 [storage=automatic];
// IR-NEXT:         let %[[VALUE_cs:[0-9]+]] cs: i32 [storage=automatic];
// IR-NEXT:         let %[[VALUE_hs:[0-9]+]] hs: i32 [storage=automatic];
// IR-NEXT:         let %[[VALUE_cc:[0-9]+]] cc: i32 [storage=automatic];
// IR-NEXT:         let %[[VALUE_lo:[0-9]+]] lo: i32 [storage=automatic];
// IR-NEXT:         let %[[VALUE_mi:[0-9]+]] mi: i32 [storage=automatic];
// IR-NEXT:         let %[[VALUE_pl:[0-9]+]] pl: i32 [storage=automatic];
// IR-NEXT:         let %[[VALUE_vs:[0-9]+]] vs: i32 [storage=automatic];
// IR-NEXT:         let %[[VALUE_vc:[0-9]+]] vc: i32 [storage=automatic];
// IR-NEXT:         let %[[VALUE_hi:[0-9]+]] hi: i32 [storage=automatic];
// IR-NEXT:         let %[[VALUE_ls:[0-9]+]] ls: i32 [storage=automatic];
// IR-NEXT:         let %[[VALUE_ge:[0-9]+]] ge: i32 [storage=automatic];
// IR-NEXT:         let %[[VALUE_lt:[0-9]+]] lt: i32 [storage=automatic];
// IR-NEXT:         let %[[VALUE_gt:[0-9]+]] gt: i32 [storage=automatic];
// IR-NEXT:         let %[[VALUE_le:[0-9]+]] le: i32 [storage=automatic];
// IR-NEXT:         asm "cmp %[a], %[b]" [options=pure,nomem,nostack,preserves_flags] {
// IR-NEXT:             template: "cmp " %16 ", " %17;
// IR-NEXT:             lateout 0 "@cceq" [cc(eq)] width 32 place<i32>(%[[VALUE_eq]]);
// IR-NEXT:             lateout 1 "@ccne" [cc(ne)] width 32 place<i32>(%[[VALUE_ne]]);
// IR-NEXT:             lateout 2 "@cccs" [cc(cs)] width 32 place<i32>(%[[VALUE_cs]]);
// IR-NEXT:             lateout 3 "@cchs" [cc(hs)] width 32 place<i32>(%[[VALUE_hs]]);
// IR-NEXT:             lateout 4 "@cccc" [cc(cc)] width 32 place<i32>(%[[VALUE_cc]]);
// IR-NEXT:             lateout 5 "@cclo" [cc(lo)] width 32 place<i32>(%[[VALUE_lo]]);
// IR-NEXT:             lateout 6 "@ccmi" [cc(mi)] width 32 place<i32>(%[[VALUE_mi]]);
// IR-NEXT:             lateout 7 "@ccpl" [cc(pl)] width 32 place<i32>(%[[VALUE_pl]]);
// IR-NEXT:             lateout 8 "@ccvs" [cc(vs)] width 32 place<i32>(%[[VALUE_vs]]);
// IR-NEXT:             lateout 9 "@ccvc" [cc(vc)] width 32 place<i32>(%[[VALUE_vc]]);
// IR-NEXT:             lateout 10 "@cchi" [cc(hi)] width 32 place<i32>(%[[VALUE_hi]]);
// IR-NEXT:             lateout 11 "@ccls" [cc(ls)] width 32 place<i32>(%[[VALUE_ls]]);
// IR-NEXT:             lateout 12 "@ccge" [cc(ge)] width 32 place<i32>(%[[VALUE_ge]]);
// IR-NEXT:             lateout 13 "@cclt" [cc(lt)] width 32 place<i32>(%[[VALUE_lt]]);
// IR-NEXT:             lateout 14 "@ccgt" [cc(gt)] width 32 place<i32>(%[[VALUE_gt]]);
// IR-NEXT:             lateout 15 "@ccle" [cc(le)] width 32 place<i32>(%[[VALUE_le]]);
// IR-NEXT:             in 16 [a] "r" [reg] width 32 read<i32>(%[[VALUE_a]]);
// IR-NEXT:             in 17 [b] "r" [reg] width 32 read<i32>(%[[VALUE_b]]);
// IR-NEXT:         }
// IR-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(%[[VALUE_eq]]), read<i32>(%[[VALUE_ne]])), read<i32>(%[[VALUE_cs]])), read<i32>(%[[VALUE_hs]])), read<i32>(%[[VALUE_cc]])), read<i32>(%[[VALUE_lo]])), read<i32>(%[[VALUE_mi]])), read<i32>(%[[VALUE_pl]])), read<i32>(%[[VALUE_vs]])), read<i32>(%[[VALUE_vc]])), read<i32>(%[[VALUE_hi]])), read<i32>(%[[VALUE_ls]])), read<i32>(%[[VALUE_ge]])), read<i32>(%[[VALUE_lt]])), read<i32>(%[[VALUE_gt]])), read<i32>(%[[VALUE_le]]));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
