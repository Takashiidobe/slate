/* Verify that memchr calls with a pointer to a constant character
   are folded as expected.
   { dg-do compile }
   { dg-options "-O1 -Wall -fdump-tree-gimple" } */

typedef __SIZE_TYPE__  size_t;
typedef __WCHAR_TYPE__ wchar_t;

extern void* memchr (const void*, int, size_t);
extern int printf (const char*, ...);
extern void abort (void);

#define A(expr)						\
  ((expr)						\
   ? (void)0						\
   : (printf ("assertion failed on line %i: %s\n",	\
	      __LINE__, #expr),				\
      abort ()))

const char nul = 0;
const char cha = 'a';

const char* const pnul = &nul;
const char* const pcha = &cha;

const struct
{
  char c;
} snul = { 0 },
  schb = { 'b' },
  sarr[] = {
  { 0 },
  { 'c' }
  };

const char* const psarr0c = &sarr[0].c;
const char* const psarr1c = &sarr[1].c;

void test_memchr_cst_char (void)
{
  A (&nul == memchr (&nul, 0, 1));
  A (!memchr (&nul, 'a', 1));

  A (&cha == memchr (&cha, 'a', 1));
  A (!memchr (&cha, 0, 1));

  A (&nul == memchr (pnul, 0, 1));
  A (!memchr (pnul, 'a', 1));

  A (&cha == memchr (pcha, 'a', 1));
  A (!memchr (pcha, 0, 1));

  A (&snul.c == memchr (&snul.c, 0, 1));
  A (!memchr (&snul.c, 'a', 1));

  A (&schb.c == memchr (&schb.c, 'b', 1));
  A (!memchr (&schb.c, 0, 1));

  A (&sarr[0].c == memchr (&sarr[0].c, 0, 1));
  A (!memchr (&sarr[0].c, 'a', 1));

  A (&sarr[1].c == memchr (&sarr[1].c, 'c', 1));
  A (!memchr (&sarr[1].c, 0, 1));

  A (&sarr[0].c == memchr (psarr0c, 0, 1));
  A (!memchr (psarr0c, 'a', 1));

  A (&sarr[1].c == memchr (psarr1c, 'c', 1));
  A (!memchr (psarr1c, 0, 1));
}

/* { dg-final { scan-tree-dump-not "abort" "gimple" } } */

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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_wchar_t:[0-9]+]] wchar_t = i32;
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_nul:[0-9]+]] nul: i8 [storage=static] [const] = truncate<i8, reason=assign, fits=always>(const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_cha:[0-9]+]] cha: i8 [storage=static] [const] = truncate<i8, reason=assign, fits=always>(const<i32>(97)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_pnul:[0-9]+]] pnul: ptr<const i8> [storage=static] [const] = addr_of<ptr<const i8>>(%[[VALUE_nul]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_pcha:[0-9]+]] pcha: ptr<const i8> [storage=static] [const] = addr_of<ptr<const i8>>(%[[VALUE_cha]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_snul:[0-9]+]] snul: @type[[TYPE0]] [storage=static] [const] = aggregate<@type[[TYPE0]], zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(0))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_schb:[0-9]+]] schb: @type[[TYPE0]] [storage=static] [const] = aggregate<@type[[TYPE0]], zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(98))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_sarr:[0-9]+]] sarr: array<@type[[TYPE0]], 2> [storage=static] [const] = aggregate<array<@type[[TYPE0]], 2>, zero_fill=false>(index0 = aggregate<@type[[TYPE0]], zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(0))), index1 = aggregate<@type[[TYPE0]], zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(99)))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_psarr0c:[0-9]+]] psarr0c: ptr<const i8> [storage=static] [const] = addr_of<ptr<const i8>>(field0(deref(ptr_offset<ptr<const @type[[TYPE0]]>, subtract=false, element=@type[[TYPE0]], overflow=ub>(array_decay<ptr<const @type[[TYPE0]]>, length=Some(2)>(%[[VALUE_sarr]]), const<i32>(0))))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_psarr1c:[0-9]+]] psarr1c: ptr<const i8> [storage=static] [const] = addr_of<ptr<const i8>>(field0(deref(ptr_offset<ptr<const @type[[TYPE0]]>, subtract=false, element=@type[[TYPE0]], overflow=ub>(array_decay<ptr<const @type[[TYPE0]]>, length=Some(2)>(%[[VALUE_sarr]]), const<i32>(1))))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([38, 110, 117, 108, 32, 61, 61, 32, 109, 101, 109, 99, 104, 114, 32, 40, 38, 110, 117, 108, 44, 32, 48, 44, 32, 49, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([33, 109, 101, 109, 99, 104, 114, 32, 40, 38, 110, 117, 108, 44, 32, 39, 97, 39, 44, 32, 49, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_6:[0-9]+]] .str[[VALUE_str_6]]: array<i8, 30> [storage=static] = code_units<array<i8, 30>>([38, 99, 104, 97, 32, 61, 61, 32, 109, 101, 109, 99, 104, 114, 32, 40, 38, 99, 104, 97, 44, 32, 39, 97, 39, 44, 32, 49, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_7:[0-9]+]] .str[[VALUE_str_7]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_8:[0-9]+]] .str[[VALUE_str_8]]: array<i8, 21> [storage=static] = code_units<array<i8, 21>>([33, 109, 101, 109, 99, 104, 114, 32, 40, 38, 99, 104, 97, 44, 32, 48, 44, 32, 49, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_9:[0-9]+]] .str[[VALUE_str_9]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_10:[0-9]+]] .str[[VALUE_str_10]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([38, 110, 117, 108, 32, 61, 61, 32, 109, 101, 109, 99, 104, 114, 32, 40, 112, 110, 117, 108, 44, 32, 48, 44, 32, 49, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_11:[0-9]+]] .str[[VALUE_str_11]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_12:[0-9]+]] .str[[VALUE_str_12]]: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([33, 109, 101, 109, 99, 104, 114, 32, 40, 112, 110, 117, 108, 44, 32, 39, 97, 39, 44, 32, 49, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_13:[0-9]+]] .str[[VALUE_str_13]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_14:[0-9]+]] .str[[VALUE_str_14]]: array<i8, 30> [storage=static] = code_units<array<i8, 30>>([38, 99, 104, 97, 32, 61, 61, 32, 109, 101, 109, 99, 104, 114, 32, 40, 112, 99, 104, 97, 44, 32, 39, 97, 39, 44, 32, 49, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_15:[0-9]+]] .str[[VALUE_str_15]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_16:[0-9]+]] .str[[VALUE_str_16]]: array<i8, 21> [storage=static] = code_units<array<i8, 21>>([33, 109, 101, 109, 99, 104, 114, 32, 40, 112, 99, 104, 97, 44, 32, 48, 44, 32, 49, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_17:[0-9]+]] .str[[VALUE_str_17]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_18:[0-9]+]] .str[[VALUE_str_18]]: array<i8, 34> [storage=static] = code_units<array<i8, 34>>([38, 115, 110, 117, 108, 46, 99, 32, 61, 61, 32, 109, 101, 109, 99, 104, 114, 32, 40, 38, 115, 110, 117, 108, 46, 99, 44, 32, 48, 44, 32, 49, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_19:[0-9]+]] .str[[VALUE_str_19]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_20:[0-9]+]] .str[[VALUE_str_20]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([33, 109, 101, 109, 99, 104, 114, 32, 40, 38, 115, 110, 117, 108, 46, 99, 44, 32, 39, 97, 39, 44, 32, 49, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_21:[0-9]+]] .str[[VALUE_str_21]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_22:[0-9]+]] .str[[VALUE_str_22]]: array<i8, 36> [storage=static] = code_units<array<i8, 36>>([38, 115, 99, 104, 98, 46, 99, 32, 61, 61, 32, 109, 101, 109, 99, 104, 114, 32, 40, 38, 115, 99, 104, 98, 46, 99, 44, 32, 39, 98, 39, 44, 32, 49, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_23:[0-9]+]] .str[[VALUE_str_23]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_24:[0-9]+]] .str[[VALUE_str_24]]: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([33, 109, 101, 109, 99, 104, 114, 32, 40, 38, 115, 99, 104, 98, 46, 99, 44, 32, 48, 44, 32, 49, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_25:[0-9]+]] .str[[VALUE_str_25]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_26:[0-9]+]] .str[[VALUE_str_26]]: array<i8, 40> [storage=static] = code_units<array<i8, 40>>([38, 115, 97, 114, 114, 91, 48, 93, 46, 99, 32, 61, 61, 32, 109, 101, 109, 99, 104, 114, 32, 40, 38, 115, 97, 114, 114, 91, 48, 93, 46, 99, 44, 32, 48, 44, 32, 49, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_27:[0-9]+]] .str[[VALUE_str_27]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_28:[0-9]+]] .str[[VALUE_str_28]]: array<i8, 29> [storage=static] = code_units<array<i8, 29>>([33, 109, 101, 109, 99, 104, 114, 32, 40, 38, 115, 97, 114, 114, 91, 48, 93, 46, 99, 44, 32, 39, 97, 39, 44, 32, 49, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_29:[0-9]+]] .str[[VALUE_str_29]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_30:[0-9]+]] .str[[VALUE_str_30]]: array<i8, 42> [storage=static] = code_units<array<i8, 42>>([38, 115, 97, 114, 114, 91, 49, 93, 46, 99, 32, 61, 61, 32, 109, 101, 109, 99, 104, 114, 32, 40, 38, 115, 97, 114, 114, 91, 49, 93, 46, 99, 44, 32, 39, 99, 39, 44, 32, 49, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_31:[0-9]+]] .str[[VALUE_str_31]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_32:[0-9]+]] .str[[VALUE_str_32]]: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([33, 109, 101, 109, 99, 104, 114, 32, 40, 38, 115, 97, 114, 114, 91, 49, 93, 46, 99, 44, 32, 48, 44, 32, 49, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_33:[0-9]+]] .str[[VALUE_str_33]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_34:[0-9]+]] .str[[VALUE_str_34]]: array<i8, 37> [storage=static] = code_units<array<i8, 37>>([38, 115, 97, 114, 114, 91, 48, 93, 46, 99, 32, 61, 61, 32, 109, 101, 109, 99, 104, 114, 32, 40, 112, 115, 97, 114, 114, 48, 99, 44, 32, 48, 44, 32, 49, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_35:[0-9]+]] .str[[VALUE_str_35]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_36:[0-9]+]] .str[[VALUE_str_36]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([33, 109, 101, 109, 99, 104, 114, 32, 40, 112, 115, 97, 114, 114, 48, 99, 44, 32, 39, 97, 39, 44, 32, 49, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_37:[0-9]+]] .str[[VALUE_str_37]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_38:[0-9]+]] .str[[VALUE_str_38]]: array<i8, 39> [storage=static] = code_units<array<i8, 39>>([38, 115, 97, 114, 114, 91, 49, 93, 46, 99, 32, 61, 61, 32, 109, 101, 109, 99, 104, 114, 32, 40, 112, 115, 97, 114, 114, 49, 99, 44, 32, 39, 99, 39, 44, 32, 49, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_39:[0-9]+]] .str[[VALUE_str_39]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_40:[0-9]+]] .str[[VALUE_str_40]]: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([33, 109, 101, 109, 99, 104, 114, 32, 40, 112, 115, 97, 114, 114, 49, 99, 44, 32, 48, 44, 32, 49, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_memchr:[0-9]+]] @memchr(%[[VALUE0:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE1:[0-9]+]] <unnamed>: i32, %[[VALUE2:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE3:[0-9]+]] <unnamed>: ptr<const i8>, ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_test_memchr_cst_char:[0-9]+]] @test_memchr_cst_char() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<ptr<const i8>>(addr_of<ptr<const i8>>(%[[VALUE_nul]]), pointer_cast<ptr<const i8>, reason=usual_arith>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<const i8>>(%[[VALUE_nul]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str]])), const<i32>(41), array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str_2]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<const i8>>(%[[VALUE_nul]])), const<i32>(97), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))), null<ptr<void>>))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_3]])), const<i32>(42), array_decay<ptr<i8>, length=Some(23)>(%[[VALUE_str_4]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if eq<ptr<const i8>>(addr_of<ptr<const i8>>(%[[VALUE_cha]]), pointer_cast<ptr<const i8>, reason=usual_arith>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<const i8>>(%[[VALUE_cha]])), const<i32>(97), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_5]])), const<i32>(44), array_decay<ptr<i8>, length=Some(30)>(%[[VALUE_str_6]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<const i8>>(%[[VALUE_cha]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))), null<ptr<void>>))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_7]])), const<i32>(45), array_decay<ptr<i8>, length=Some(21)>(%[[VALUE_str_8]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if eq<ptr<const i8>>(addr_of<ptr<const i8>>(%[[VALUE_nul]]), pointer_cast<ptr<const i8>, reason=usual_arith>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i8>>(%[[VALUE_pnul]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_9]])), const<i32>(47), array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str_10]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i8>>(%[[VALUE_pnul]])), const<i32>(97), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))), null<ptr<void>>))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_11]])), const<i32>(48), array_decay<ptr<i8>, length=Some(23)>(%[[VALUE_str_12]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if eq<ptr<const i8>>(addr_of<ptr<const i8>>(%[[VALUE_cha]]), pointer_cast<ptr<const i8>, reason=usual_arith>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i8>>(%[[VALUE_pcha]])), const<i32>(97), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_13]])), const<i32>(50), array_decay<ptr<i8>, length=Some(30)>(%[[VALUE_str_14]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i8>>(%[[VALUE_pcha]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))), null<ptr<void>>))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_15]])), const<i32>(51), array_decay<ptr<i8>, length=Some(21)>(%[[VALUE_str_16]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if eq<ptr<const i8>>(addr_of<ptr<const i8>>(field0(%[[VALUE_snul]])), pointer_cast<ptr<const i8>, reason=usual_arith>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<const i8>>(field0(%[[VALUE_snul]]))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_17]])), const<i32>(53), array_decay<ptr<i8>, length=Some(34)>(%[[VALUE_str_18]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<const i8>>(field0(%[[VALUE_snul]]))), const<i32>(97), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))), null<ptr<void>>))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_19]])), const<i32>(54), array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_20]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if eq<ptr<const i8>>(addr_of<ptr<const i8>>(field0(%[[VALUE_schb]])), pointer_cast<ptr<const i8>, reason=usual_arith>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<const i8>>(field0(%[[VALUE_schb]]))), const<i32>(98), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_21]])), const<i32>(56), array_decay<ptr<i8>, length=Some(36)>(%[[VALUE_str_22]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<const i8>>(field0(%[[VALUE_schb]]))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))), null<ptr<void>>))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_23]])), const<i32>(57), array_decay<ptr<i8>, length=Some(24)>(%[[VALUE_str_24]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if eq<ptr<const i8>>(addr_of<ptr<const i8>>(field0(deref(ptr_offset<ptr<const @type[[TYPE0]]>, subtract=false, element=@type[[TYPE0]], overflow=ub>(array_decay<ptr<const @type[[TYPE0]]>, length=Some(2)>(%[[VALUE_sarr]]), const<i32>(0))))), pointer_cast<ptr<const i8>, reason=usual_arith>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<const i8>>(field0(deref(ptr_offset<ptr<const @type[[TYPE0]]>, subtract=false, element=@type[[TYPE0]], overflow=ub>(array_decay<ptr<const @type[[TYPE0]]>, length=Some(2)>(%[[VALUE_sarr]]), const<i32>(0)))))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_25]])), const<i32>(59), array_decay<ptr<i8>, length=Some(40)>(%[[VALUE_str_26]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<const i8>>(field0(deref(ptr_offset<ptr<const @type[[TYPE0]]>, subtract=false, element=@type[[TYPE0]], overflow=ub>(array_decay<ptr<const @type[[TYPE0]]>, length=Some(2)>(%[[VALUE_sarr]]), const<i32>(0)))))), const<i32>(97), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))), null<ptr<void>>))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_27]])), const<i32>(60), array_decay<ptr<i8>, length=Some(29)>(%[[VALUE_str_28]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if eq<ptr<const i8>>(addr_of<ptr<const i8>>(field0(deref(ptr_offset<ptr<const @type[[TYPE0]]>, subtract=false, element=@type[[TYPE0]], overflow=ub>(array_decay<ptr<const @type[[TYPE0]]>, length=Some(2)>(%[[VALUE_sarr]]), const<i32>(1))))), pointer_cast<ptr<const i8>, reason=usual_arith>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<const i8>>(field0(deref(ptr_offset<ptr<const @type[[TYPE0]]>, subtract=false, element=@type[[TYPE0]], overflow=ub>(array_decay<ptr<const @type[[TYPE0]]>, length=Some(2)>(%[[VALUE_sarr]]), const<i32>(1)))))), const<i32>(99), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_29]])), const<i32>(62), array_decay<ptr<i8>, length=Some(42)>(%[[VALUE_str_30]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<const i8>>(field0(deref(ptr_offset<ptr<const @type[[TYPE0]]>, subtract=false, element=@type[[TYPE0]], overflow=ub>(array_decay<ptr<const @type[[TYPE0]]>, length=Some(2)>(%[[VALUE_sarr]]), const<i32>(1)))))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))), null<ptr<void>>))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_31]])), const<i32>(63), array_decay<ptr<i8>, length=Some(27)>(%[[VALUE_str_32]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if eq<ptr<const i8>>(addr_of<ptr<const i8>>(field0(deref(ptr_offset<ptr<const @type[[TYPE0]]>, subtract=false, element=@type[[TYPE0]], overflow=ub>(array_decay<ptr<const @type[[TYPE0]]>, length=Some(2)>(%[[VALUE_sarr]]), const<i32>(0))))), pointer_cast<ptr<const i8>, reason=usual_arith>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i8>>(%[[VALUE_psarr0c]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_33]])), const<i32>(65), array_decay<ptr<i8>, length=Some(37)>(%[[VALUE_str_34]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i8>>(%[[VALUE_psarr0c]])), const<i32>(97), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))), null<ptr<void>>))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_35]])), const<i32>(66), array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_36]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if eq<ptr<const i8>>(addr_of<ptr<const i8>>(field0(deref(ptr_offset<ptr<const @type[[TYPE0]]>, subtract=false, element=@type[[TYPE0]], overflow=ub>(array_decay<ptr<const @type[[TYPE0]]>, length=Some(2)>(%[[VALUE_sarr]]), const<i32>(1))))), pointer_cast<ptr<const i8>, reason=usual_arith>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i8>>(%[[VALUE_psarr1c]])), const<i32>(99), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_37]])), const<i32>(68), array_decay<ptr<i8>, length=Some(39)>(%[[VALUE_str_38]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i8>>(%[[VALUE_psarr1c]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))), null<ptr<void>>))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_39]])), const<i32>(69), array_decay<ptr<i8>, length=Some(24)>(%[[VALUE_str_40]]));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
