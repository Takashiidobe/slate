/* PR tree-optimization/112880 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-std=c23 -O2" } */

#if __BITINT_MAXWIDTH__ >= 1024
_BitInt(1024) a, b, c, d, e, f;

void
foo (void)
{
  __builtin_add_overflow (a, b, &a);
  __builtin_sub_overflow (c, d, &c);
  __builtin_mul_overflow (e, f, &e);
}
#endif

#if __BITINT_MAXWIDTH__ >= 512
_BitInt(512) g, h, i, j, k, l;

void
bar (void)
{
  __builtin_add_overflow (g, h, &g);
  __builtin_sub_overflow (i, j, &i);
  __builtin_mul_overflow (k, l, &k);
}
#endif

_BitInt(32) m, n, o, p, q, r;

void
baz (void)
{
  __builtin_add_overflow (m, n, &m);
  __builtin_sub_overflow (o, p, &o);
  __builtin_mul_overflow (q, r, &q);
}

// SLATE-FILECHECK-STD DEFAULT c23
// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: i1024b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: i1024b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: i1024b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: i1024b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: i1024b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: i1024b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g:[0-9]+]] g: i512b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_h:[0-9]+]] h: i512b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_i:[0-9]+]] i: i512b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_j:[0-9]+]] j: i512b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_k:[0-9]+]] k: i512b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_l:[0-9]+]] l: i512b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_m:[0-9]+]] m: i32b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_n:[0-9]+]] n: i32b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_o:[0-9]+]] o: i32b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_p:[0-9]+]] p: i32b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_q:[0-9]+]] q: i32b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_r:[0-9]+]] r: i32b [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         overflow_add<bool>(read<i1024b>(%[[VALUE_a]]), read<i1024b>(%[[VALUE_b]]), deref(addr_of<ptr<i1024b>>(%[[VALUE_a]])));
// DEFAULT-NEXT:         overflow_sub<bool>(read<i1024b>(%[[VALUE_c]]), read<i1024b>(%[[VALUE_d]]), deref(addr_of<ptr<i1024b>>(%[[VALUE_c]])));
// DEFAULT-NEXT:         overflow_mul<bool>(read<i1024b>(%[[VALUE_e]]), read<i1024b>(%[[VALUE_f]]), deref(addr_of<ptr<i1024b>>(%[[VALUE_e]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         overflow_add<bool>(read<i512b>(%[[VALUE_g]]), read<i512b>(%[[VALUE_h]]), deref(addr_of<ptr<i512b>>(%[[VALUE_g]])));
// DEFAULT-NEXT:         overflow_sub<bool>(read<i512b>(%[[VALUE_i]]), read<i512b>(%[[VALUE_j]]), deref(addr_of<ptr<i512b>>(%[[VALUE_i]])));
// DEFAULT-NEXT:         overflow_mul<bool>(read<i512b>(%[[VALUE_k]]), read<i512b>(%[[VALUE_l]]), deref(addr_of<ptr<i512b>>(%[[VALUE_k]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_baz:[0-9]+]] @baz() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         overflow_add<bool>(read<i32b>(%[[VALUE_m]]), read<i32b>(%[[VALUE_n]]), deref(addr_of<ptr<i32b>>(%[[VALUE_m]])));
// DEFAULT-NEXT:         overflow_sub<bool>(read<i32b>(%[[VALUE_o]]), read<i32b>(%[[VALUE_p]]), deref(addr_of<ptr<i32b>>(%[[VALUE_o]])));
// DEFAULT-NEXT:         overflow_mul<bool>(read<i32b>(%[[VALUE_q]]), read<i32b>(%[[VALUE_r]]), deref(addr_of<ptr<i32b>>(%[[VALUE_q]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
