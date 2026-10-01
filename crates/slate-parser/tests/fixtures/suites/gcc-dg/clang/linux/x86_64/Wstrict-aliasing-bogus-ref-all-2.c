/* { dg-do compile } */
/* { dg-options "-O2 -Wall" } */
/* { dg-options "-O2 -Wall -mabi=altivec" { target { { powerpc*-*-linux* } && ilp32 } } } */
/* { dg-options "-O2 -Wall -msse2" { target { i?86-*-* x86_64-*-* } } } */

typedef long long __m128i __attribute__ ((__vector_size__ (16), __may_alias__));

extern __inline __m128i __attribute__((__gnu_inline__, __always_inline__, __artificial__))
_mm_load_si128 (__m128i const *__P)
{
  return *__P;
}

static const short __attribute__((__aligned__(16))) tbl[8] =
{ 1, 2, 3, 4, 5, 6, 7, 8};


__m128i get_vec(void)
{
  __m128i ret;

  ret = _mm_load_si128((__m128i *)tbl); /* { dg-bogus "type-punning" } */

  return ret;
}

/* Ignore a warning that is irrelevant to the purpose of this test.  */
/* { dg-prune-output ".*GCC vector returned by reference.*" } */

// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     type @type[[TYPE___m128i:[0-9]+]] __m128i = vector<i64, 2>;
// DEFAULT-NEXT:     global %[[VALUE_tbl:[0-9]+]] tbl: array<i16, 8> [storage=static] [const] [align=16] = aggregate<array<i16, 8>, zero_fill=false>(index0 = truncate<i16, reason=assign, fits=always>(const<i32>(1)), index1 = truncate<i16, reason=assign, fits=always>(const<i32>(2)), index2 = truncate<i16, reason=assign, fits=always>(const<i32>(3)), index3 = truncate<i16, reason=assign, fits=always>(const<i32>(4)), index4 = truncate<i16, reason=assign, fits=always>(const<i32>(5)), index5 = truncate<i16, reason=assign, fits=always>(const<i32>(6)), index6 = truncate<i16, reason=assign, fits=always>(const<i32>(7)), index7 = truncate<i16, reason=assign, fits=always>(const<i32>(8))) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE__mm_load_si128:[0-9]+]] @_mm_load_si128(%[[VALUE___P:[0-9]+]] __P: ptr<const vector<i64, 2>>) -> vector<i64, 2> [linkage=external] [inline=always] [definition=inline_only] [abi=sysv64(scalar) -> direct] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<vector<i64, 2>>(deref(read<ptr<const vector<i64, 2>>>(%[[VALUE___P]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_get_vec:[0-9]+]] @get_vec() -> vector<i64, 2> [linkage=external] [abi=sysv64() -> direct] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_ret:[0-9]+]] ret: vector<i64, 2> [storage=automatic];
// DEFAULT-NEXT:         write<vector<i64, 2>>(%[[VALUE_ret]], call<vector<i64, 2>, signature=fn(ptr<const vector<i64, 2>>) -> vector<i64, 2>, abi=sysv64(scalar) -> direct>(%[[VALUE__mm_load_si128]], pointer_cast<ptr<const vector<i64, 2>>, reason=arg>(pointer_cast<ptr<vector<i64, 2>>, reason=explicit>(array_decay<ptr<const i16>, length=Some(8)>(%[[VALUE_tbl]])))));
// DEFAULT-NEXT:         return read<vector<i64, 2>>(%[[VALUE_ret]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
