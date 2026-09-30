/* Test qualifier preservation of typeof and discarded for __auto_type. */
/* { dg-do compile } */
/* { dg-options "-std=c11" } */

/* Check that the qualifiers are preserved for atomic types. */

extern int i;

extern int * p;

extern int _Atomic const ci;
extern __typeof (ci) ci;

extern int _Atomic volatile vi;
extern __typeof (vi) vi;

extern int * _Atomic restrict ri;
extern __typeof (ri) ri;

/* Check that the qualifiers are discarded for atomic types. */

void f(void)
{
  __auto_type aci = ci;
  int *paci = &aci;

  __auto_type avi = vi;
  int *pavi = &avi;

  __auto_type ari = ri;
  int **pari = &ari;
}

/* Check that the qualifiers are preserved for non-atomic types. */

extern int const j;

extern int volatile k;

extern int * restrict q;

extern int const nci;
extern __typeof (nci) j;

extern int volatile nvi;
extern __typeof (nvi) k;

extern int * restrict nri;
extern __typeof (nri) q;

/* Check that the qualifiers are discarded for non-atomic types. */

void g(void)
{
  __auto_type aci = nci;
  int *paci = &aci;

  __auto_type avi = nvi;
  int *pavi = &avi;

  __auto_type ari = nri;
  int **pari = &ari;
}

// SLATE-FILECHECK-STD DEFAULT c11
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
// DEFAULT-NEXT:     extern %[[VALUE_i:[0-9]+]] i: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_p:[0-9]+]] p: ptr<i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_ci:[0-9]+]] ci: atomic i32 [storage=static] [const] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_vi:[0-9]+]] vi: volatile atomic i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_ri:[0-9]+]] ri: atomic ptr<i32> [storage=static] [restrict] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_j:[0-9]+]] j: i32 [storage=static] [const] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_k:[0-9]+]] k: volatile i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_q:[0-9]+]] q: ptr<i32> [storage=static] [restrict] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_nci:[0-9]+]] nci: i32 [storage=static] [const] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_nvi:[0-9]+]] nvi: volatile i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_nri:[0-9]+]] nri: ptr<i32> [storage=static] [restrict] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_aci:[0-9]+]] aci: i32 [storage=automatic] = read<i32, atomic=seq_cst>(%[[VALUE_ci]]);
// DEFAULT-NEXT:         let %[[VALUE_paci:[0-9]+]] paci: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(%[[VALUE_aci]]);
// DEFAULT-NEXT:         let %[[VALUE_avi:[0-9]+]] avi: i32 [storage=automatic] = read<i32, volatile, atomic=seq_cst>(%[[VALUE_vi]]);
// DEFAULT-NEXT:         let %[[VALUE_pavi:[0-9]+]] pavi: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(%[[VALUE_avi]]);
// DEFAULT-NEXT:         let %[[VALUE_ari:[0-9]+]] ari: ptr<i32> [storage=automatic] = read<ptr<i32>, atomic=seq_cst>(%[[VALUE_ri]]);
// DEFAULT-NEXT:         let %[[VALUE_pari:[0-9]+]] pari: ptr<ptr<i32>> [storage=automatic] = addr_of<ptr<ptr<i32>>>(%[[VALUE_ari]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_g:[0-9]+]] @g() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_aci_2:[0-9]+]] aci: i32 [storage=automatic] = read<i32>(%[[VALUE_nci]]);
// DEFAULT-NEXT:         let %[[VALUE_paci_2:[0-9]+]] paci: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(%[[VALUE_aci_2]]);
// DEFAULT-NEXT:         let %[[VALUE_avi_2:[0-9]+]] avi: i32 [storage=automatic] = read<i32, volatile>(%[[VALUE_nvi]]);
// DEFAULT-NEXT:         let %[[VALUE_pavi_2:[0-9]+]] pavi: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(%[[VALUE_avi_2]]);
// DEFAULT-NEXT:         let %[[VALUE_ari_2:[0-9]+]] ari: ptr<i32> [storage=automatic] = read<ptr<i32>>(%[[VALUE_nri]]);
// DEFAULT-NEXT:         let %[[VALUE_pari_2:[0-9]+]] pari: ptr<ptr<i32>> [storage=automatic] = addr_of<ptr<ptr<i32>>>(%[[VALUE_ari_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
