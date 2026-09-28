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
// DEFAULT-NEXT:     extern %0 i: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %1 p: ptr<i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %2 ci: atomic i32 [storage=static] [const] [linkage=external];
// DEFAULT-NEXT:     extern %3 vi: volatile atomic i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %4 ri: atomic ptr<i32> [storage=static] [restrict] [linkage=external];
// DEFAULT-NEXT:     extern %12 j: i32 [storage=static] [const] [linkage=external];
// DEFAULT-NEXT:     extern %13 k: volatile i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %14 q: ptr<i32> [storage=static] [restrict] [linkage=external];
// DEFAULT-NEXT:     extern %15 nci: i32 [storage=static] [const] [linkage=external];
// DEFAULT-NEXT:     extern %16 nvi: volatile i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %17 nri: ptr<i32> [storage=static] [restrict] [linkage=external];
// DEFAULT-NEXT:     fn %5 @f() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %6 aci: i32 [storage=automatic] = read<i32, atomic=seq_cst>(%2);
// DEFAULT-NEXT:         let %7 paci: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(%6);
// DEFAULT-NEXT:         let %8 avi: i32 [storage=automatic] = read<i32, volatile, atomic=seq_cst>(%3);
// DEFAULT-NEXT:         let %9 pavi: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(%8);
// DEFAULT-NEXT:         let %10 ari: ptr<i32> [storage=automatic] = read<ptr<i32>, atomic=seq_cst>(%4);
// DEFAULT-NEXT:         let %11 pari: ptr<ptr<i32>> [storage=automatic] = addr_of<ptr<ptr<i32>>>(%10);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @g() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %19 aci: i32 [storage=automatic] = read<i32>(%15);
// DEFAULT-NEXT:         let %20 paci: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(%19);
// DEFAULT-NEXT:         let %21 avi: i32 [storage=automatic] = read<i32, volatile>(%16);
// DEFAULT-NEXT:         let %22 pavi: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(%21);
// DEFAULT-NEXT:         let %23 ari: ptr<i32> [storage=automatic] = read<ptr<i32>>(%17);
// DEFAULT-NEXT:         let %24 pari: ptr<ptr<i32>> [storage=automatic] = addr_of<ptr<ptr<i32>>>(%23);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
