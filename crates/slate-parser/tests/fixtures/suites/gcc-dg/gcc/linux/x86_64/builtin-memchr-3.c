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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     type @type1 wchar_t = i32;
// DEFAULT-NEXT:     type @type2 = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     global %5 nul: i8 [storage=static] [const] = truncate<i8, reason=assign, fits=always>(const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     global %6 cha: i8 [storage=static] [const] = truncate<i8, reason=assign, fits=always>(const<i32>(97)) [linkage=external];
// DEFAULT-NEXT:     global %7 pnul: ptr<const i8> [storage=static] [const] = addr_of<ptr<const i8>>(%5) [linkage=external];
// DEFAULT-NEXT:     global %8 pcha: ptr<const i8> [storage=static] [const] = addr_of<ptr<const i8>>(%6) [linkage=external];
// DEFAULT-NEXT:     global %10 snul: @type2 [storage=static] [const] = aggregate<@type2, zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(0))) [linkage=external];
// DEFAULT-NEXT:     global %11 schb: @type2 [storage=static] [const] = aggregate<@type2, zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(98))) [linkage=external];
// DEFAULT-NEXT:     global %12 sarr: array<@type2, 2> [storage=static] [const] = aggregate<array<@type2, 2>, zero_fill=false>(index0 = aggregate<@type2, zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(0))), index1 = aggregate<@type2, zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(99)))) [linkage=external];
// DEFAULT-NEXT:     global %13 psarr0c: ptr<const i8> [storage=static] [const] = addr_of<ptr<const i8>>(field0(deref(ptr_offset<ptr<const @type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<const @type2>, length=Some(2)>(%12), const<i32>(0))))) [linkage=external];
// DEFAULT-NEXT:     global %14 psarr1c: ptr<const i8> [storage=static] [const] = addr_of<ptr<const i8>>(field0(deref(ptr_offset<ptr<const @type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<const @type2>, length=Some(2)>(%12), const<i32>(1))))) [linkage=external];
// DEFAULT-NEXT:     global %20 .str20: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %21 .str21: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([38, 110, 117, 108, 32, 61, 61, 32, 109, 101, 109, 99, 104, 114, 32, 40, 38, 110, 117, 108, 44, 32, 48, 44, 32, 49, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %22 .str22: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %23 .str23: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([33, 109, 101, 109, 99, 104, 114, 32, 40, 38, 110, 117, 108, 44, 32, 39, 97, 39, 44, 32, 49, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %24 .str24: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %25 .str25: array<i8, 30> [storage=static] = code_units<array<i8, 30>>([38, 99, 104, 97, 32, 61, 61, 32, 109, 101, 109, 99, 104, 114, 32, 40, 38, 99, 104, 97, 44, 32, 39, 97, 39, 44, 32, 49, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %26 .str26: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %27 .str27: array<i8, 21> [storage=static] = code_units<array<i8, 21>>([33, 109, 101, 109, 99, 104, 114, 32, 40, 38, 99, 104, 97, 44, 32, 48, 44, 32, 49, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %28 .str28: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %29 .str29: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([38, 110, 117, 108, 32, 61, 61, 32, 109, 101, 109, 99, 104, 114, 32, 40, 112, 110, 117, 108, 44, 32, 48, 44, 32, 49, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %30 .str30: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %31 .str31: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([33, 109, 101, 109, 99, 104, 114, 32, 40, 112, 110, 117, 108, 44, 32, 39, 97, 39, 44, 32, 49, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %32 .str32: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %33 .str33: array<i8, 30> [storage=static] = code_units<array<i8, 30>>([38, 99, 104, 97, 32, 61, 61, 32, 109, 101, 109, 99, 104, 114, 32, 40, 112, 99, 104, 97, 44, 32, 39, 97, 39, 44, 32, 49, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %34 .str34: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %35 .str35: array<i8, 21> [storage=static] = code_units<array<i8, 21>>([33, 109, 101, 109, 99, 104, 114, 32, 40, 112, 99, 104, 97, 44, 32, 48, 44, 32, 49, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %36 .str36: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %37 .str37: array<i8, 34> [storage=static] = code_units<array<i8, 34>>([38, 115, 110, 117, 108, 46, 99, 32, 61, 61, 32, 109, 101, 109, 99, 104, 114, 32, 40, 38, 115, 110, 117, 108, 46, 99, 44, 32, 48, 44, 32, 49, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %38 .str38: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %39 .str39: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([33, 109, 101, 109, 99, 104, 114, 32, 40, 38, 115, 110, 117, 108, 46, 99, 44, 32, 39, 97, 39, 44, 32, 49, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %40 .str40: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %41 .str41: array<i8, 36> [storage=static] = code_units<array<i8, 36>>([38, 115, 99, 104, 98, 46, 99, 32, 61, 61, 32, 109, 101, 109, 99, 104, 114, 32, 40, 38, 115, 99, 104, 98, 46, 99, 44, 32, 39, 98, 39, 44, 32, 49, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %42 .str42: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %43 .str43: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([33, 109, 101, 109, 99, 104, 114, 32, 40, 38, 115, 99, 104, 98, 46, 99, 44, 32, 48, 44, 32, 49, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %44 .str44: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %45 .str45: array<i8, 40> [storage=static] = code_units<array<i8, 40>>([38, 115, 97, 114, 114, 91, 48, 93, 46, 99, 32, 61, 61, 32, 109, 101, 109, 99, 104, 114, 32, 40, 38, 115, 97, 114, 114, 91, 48, 93, 46, 99, 44, 32, 48, 44, 32, 49, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %46 .str46: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %47 .str47: array<i8, 29> [storage=static] = code_units<array<i8, 29>>([33, 109, 101, 109, 99, 104, 114, 32, 40, 38, 115, 97, 114, 114, 91, 48, 93, 46, 99, 44, 32, 39, 97, 39, 44, 32, 49, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %48 .str48: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %49 .str49: array<i8, 42> [storage=static] = code_units<array<i8, 42>>([38, 115, 97, 114, 114, 91, 49, 93, 46, 99, 32, 61, 61, 32, 109, 101, 109, 99, 104, 114, 32, 40, 38, 115, 97, 114, 114, 91, 49, 93, 46, 99, 44, 32, 39, 99, 39, 44, 32, 49, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %50 .str50: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %51 .str51: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([33, 109, 101, 109, 99, 104, 114, 32, 40, 38, 115, 97, 114, 114, 91, 49, 93, 46, 99, 44, 32, 48, 44, 32, 49, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %52 .str52: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %53 .str53: array<i8, 37> [storage=static] = code_units<array<i8, 37>>([38, 115, 97, 114, 114, 91, 48, 93, 46, 99, 32, 61, 61, 32, 109, 101, 109, 99, 104, 114, 32, 40, 112, 115, 97, 114, 114, 48, 99, 44, 32, 48, 44, 32, 49, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %54 .str54: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %55 .str55: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([33, 109, 101, 109, 99, 104, 114, 32, 40, 112, 115, 97, 114, 114, 48, 99, 44, 32, 39, 97, 39, 44, 32, 49, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %56 .str56: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %57 .str57: array<i8, 39> [storage=static] = code_units<array<i8, 39>>([38, 115, 97, 114, 114, 91, 49, 93, 46, 99, 32, 61, 61, 32, 109, 101, 109, 99, 104, 114, 32, 40, 112, 115, 97, 114, 114, 49, 99, 44, 32, 39, 99, 39, 44, 32, 49, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %58 .str58: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %59 .str59: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([33, 109, 101, 109, 99, 104, 114, 32, 40, 112, 115, 97, 114, 114, 49, 99, 44, 32, 48, 44, 32, 49, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %2 @memchr(%16 <unnamed>: ptr<const void>, %17 <unnamed>: i32, %18 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %3 @printf(%19 <unnamed>: ptr<const i8>, ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %4 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %15 @test_memchr_cst_char() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<ptr<const i8>>(addr_of<ptr<const i8>>(%5), pointer_cast<ptr<const i8>, reason=usual_arith>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<const i8>>(%5)), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%20)), const<i32>(41), array_decay<ptr<i8>, length=Some(28)>(%21));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if not<bool>(ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<const i8>>(%5)), const<i32>(97), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))), null<ptr<void>>))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%22)), const<i32>(42), array_decay<ptr<i8>, length=Some(23)>(%23));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if eq<ptr<const i8>>(addr_of<ptr<const i8>>(%6), pointer_cast<ptr<const i8>, reason=usual_arith>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<const i8>>(%6)), const<i32>(97), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%24)), const<i32>(44), array_decay<ptr<i8>, length=Some(30)>(%25));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if not<bool>(ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<const i8>>(%6)), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))), null<ptr<void>>))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%26)), const<i32>(45), array_decay<ptr<i8>, length=Some(21)>(%27));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if eq<ptr<const i8>>(addr_of<ptr<const i8>>(%5), pointer_cast<ptr<const i8>, reason=usual_arith>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i8>>(%7)), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%28)), const<i32>(47), array_decay<ptr<i8>, length=Some(28)>(%29));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if not<bool>(ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i8>>(%7)), const<i32>(97), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))), null<ptr<void>>))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%30)), const<i32>(48), array_decay<ptr<i8>, length=Some(23)>(%31));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if eq<ptr<const i8>>(addr_of<ptr<const i8>>(%6), pointer_cast<ptr<const i8>, reason=usual_arith>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i8>>(%8)), const<i32>(97), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%32)), const<i32>(50), array_decay<ptr<i8>, length=Some(30)>(%33));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if not<bool>(ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i8>>(%8)), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))), null<ptr<void>>))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%34)), const<i32>(51), array_decay<ptr<i8>, length=Some(21)>(%35));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if eq<ptr<const i8>>(addr_of<ptr<const i8>>(field0(%10)), pointer_cast<ptr<const i8>, reason=usual_arith>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<const i8>>(field0(%10))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%36)), const<i32>(53), array_decay<ptr<i8>, length=Some(34)>(%37));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if not<bool>(ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<const i8>>(field0(%10))), const<i32>(97), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))), null<ptr<void>>))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%38)), const<i32>(54), array_decay<ptr<i8>, length=Some(26)>(%39));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if eq<ptr<const i8>>(addr_of<ptr<const i8>>(field0(%11)), pointer_cast<ptr<const i8>, reason=usual_arith>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<const i8>>(field0(%11))), const<i32>(98), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%40)), const<i32>(56), array_decay<ptr<i8>, length=Some(36)>(%41));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if not<bool>(ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<const i8>>(field0(%11))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))), null<ptr<void>>))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%42)), const<i32>(57), array_decay<ptr<i8>, length=Some(24)>(%43));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if eq<ptr<const i8>>(addr_of<ptr<const i8>>(field0(deref(ptr_offset<ptr<const @type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<const @type2>, length=Some(2)>(%12), const<i32>(0))))), pointer_cast<ptr<const i8>, reason=usual_arith>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<const i8>>(field0(deref(ptr_offset<ptr<const @type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<const @type2>, length=Some(2)>(%12), const<i32>(0)))))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%44)), const<i32>(59), array_decay<ptr<i8>, length=Some(40)>(%45));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if not<bool>(ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<const i8>>(field0(deref(ptr_offset<ptr<const @type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<const @type2>, length=Some(2)>(%12), const<i32>(0)))))), const<i32>(97), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))), null<ptr<void>>))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%46)), const<i32>(60), array_decay<ptr<i8>, length=Some(29)>(%47));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if eq<ptr<const i8>>(addr_of<ptr<const i8>>(field0(deref(ptr_offset<ptr<const @type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<const @type2>, length=Some(2)>(%12), const<i32>(1))))), pointer_cast<ptr<const i8>, reason=usual_arith>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<const i8>>(field0(deref(ptr_offset<ptr<const @type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<const @type2>, length=Some(2)>(%12), const<i32>(1)))))), const<i32>(99), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%48)), const<i32>(62), array_decay<ptr<i8>, length=Some(42)>(%49));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if not<bool>(ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<const i8>>(field0(deref(ptr_offset<ptr<const @type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<const @type2>, length=Some(2)>(%12), const<i32>(1)))))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))), null<ptr<void>>))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%50)), const<i32>(63), array_decay<ptr<i8>, length=Some(27)>(%51));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if eq<ptr<const i8>>(addr_of<ptr<const i8>>(field0(deref(ptr_offset<ptr<const @type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<const @type2>, length=Some(2)>(%12), const<i32>(0))))), pointer_cast<ptr<const i8>, reason=usual_arith>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i8>>(%13)), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%52)), const<i32>(65), array_decay<ptr<i8>, length=Some(37)>(%53));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if not<bool>(ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i8>>(%13)), const<i32>(97), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))), null<ptr<void>>))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%54)), const<i32>(66), array_decay<ptr<i8>, length=Some(26)>(%55));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if eq<ptr<const i8>>(addr_of<ptr<const i8>>(field0(deref(ptr_offset<ptr<const @type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<const @type2>, length=Some(2)>(%12), const<i32>(1))))), pointer_cast<ptr<const i8>, reason=usual_arith>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i8>>(%14)), const<i32>(99), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%56)), const<i32>(68), array_decay<ptr<i8>, length=Some(39)>(%57));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if not<bool>(ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i8>>(%14)), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))), null<ptr<void>>))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%58)), const<i32>(69), array_decay<ptr<i8>, length=Some(24)>(%59));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
