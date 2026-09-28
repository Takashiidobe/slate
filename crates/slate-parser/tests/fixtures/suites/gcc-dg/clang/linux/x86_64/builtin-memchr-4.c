/* PR tree-optimization/89772
   Verify that memchr calls with a pointer to a constant character
   are folded as expected.
   { dg-do compile }
   { dg-options "-O1 -Wall -fdump-tree-release_ssa" } */

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

const char a[8] = {'a',0,'b'};
const char b[3] = {'a','b'};
const char c[8] = {'a','b','c'};

void test_memchr_cst_char (void)
{
  A (!memchr (a, 'c', 2));
  A (!memchr (a, 'c', 5));
  A (!memchr (a, 'c', sizeof a));
  A (&a[1] == memchr (a, 0, sizeof a));

  A (!memchr (b, 0, 2));
  A (&b[2] == memchr (b, 0, sizeof b));

  A (!memchr (c, 0, 2));
  A (&c[3] == memchr (c, 0, 4));
  A (&c[3] == memchr (c, 0, sizeof a));
}

/* { dg-final { scan-tree-dump-not "abort" "release_ssa" } } */

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
// DEFAULT-NEXT:     global %5 a: array<i8, 8> [storage=static] [const] = aggregate<array<i8, 8>, zero_fill=true>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(97)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(0)), index2 = truncate<i8, reason=assign, fits=always>(const<i32>(98))) [linkage=external];
// DEFAULT-NEXT:     global %6 b: array<i8, 3> [storage=static] [const] = aggregate<array<i8, 3>, zero_fill=true>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(97)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(98))) [linkage=external];
// DEFAULT-NEXT:     global %7 c: array<i8, 8> [storage=static] [const] = aggregate<array<i8, 8>, zero_fill=true>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(97)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(98)), index2 = truncate<i8, reason=assign, fits=always>(const<i32>(99))) [linkage=external];
// DEFAULT-NEXT:     global %13 .str13: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %14 .str14: array<i8, 20> [storage=static] = code_units<array<i8, 20>>([33, 109, 101, 109, 99, 104, 114, 32, 40, 97, 44, 32, 39, 99, 39, 44, 32, 50, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %15 .str15: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %16 .str16: array<i8, 20> [storage=static] = code_units<array<i8, 20>>([33, 109, 101, 109, 99, 104, 114, 32, 40, 97, 44, 32, 39, 99, 39, 44, 32, 53, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %17 .str17: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %18 .str18: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([33, 109, 101, 109, 99, 104, 114, 32, 40, 97, 44, 32, 39, 99, 39, 44, 32, 115, 105, 122, 101, 111, 102, 32, 97, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %19 .str19: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %20 .str20: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([38, 97, 91, 49, 93, 32, 61, 61, 32, 109, 101, 109, 99, 104, 114, 32, 40, 97, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 97, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %21 .str21: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %22 .str22: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([33, 109, 101, 109, 99, 104, 114, 32, 40, 98, 44, 32, 48, 44, 32, 50, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %23 .str23: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %24 .str24: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([38, 98, 91, 50, 93, 32, 61, 61, 32, 109, 101, 109, 99, 104, 114, 32, 40, 98, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 98, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %25 .str25: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %26 .str26: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([33, 109, 101, 109, 99, 104, 114, 32, 40, 99, 44, 32, 48, 44, 32, 50, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %27 .str27: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %28 .str28: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([38, 99, 91, 51, 93, 32, 61, 61, 32, 109, 101, 109, 99, 104, 114, 32, 40, 99, 44, 32, 48, 44, 32, 52, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %29 .str29: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([97, 115, 115, 101, 114, 116, 105, 111, 110, 32, 102, 97, 105, 108, 101, 100, 32, 111, 110, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %30 .str30: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([38, 99, 91, 51, 93, 32, 61, 61, 32, 109, 101, 109, 99, 104, 114, 32, 40, 99, 44, 32, 48, 44, 32, 115, 105, 122, 101, 111, 102, 32, 97, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %2 @memchr(%9 <unnamed>: ptr<const void>, %10 <unnamed>: i32, %11 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %3 @printf(%12 <unnamed>: ptr<const i8>, ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %4 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %8 @test_memchr_cst_char() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if not<bool>(ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<const i8>, length=Some(8)>(%5)), const<i32>(99), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2)))), null<ptr<void>>))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%13)), const<i32>(27), array_decay<ptr<i8>, length=Some(20)>(%14));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if not<bool>(ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<const i8>, length=Some(8)>(%5)), const<i32>(99), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))), null<ptr<void>>))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%15)), const<i32>(28), array_decay<ptr<i8>, length=Some(20)>(%16));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if not<bool>(ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<const i8>, length=Some(8)>(%5)), const<i32>(99), const<u64>(8)), null<ptr<void>>))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%17)), const<i32>(29), array_decay<ptr<i8>, length=Some(27)>(%18));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if eq<ptr<const i8>>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(8)>(%5), const<i32>(1)))), pointer_cast<ptr<const i8>, reason=usual_arith>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<const i8>, length=Some(8)>(%5)), const<i32>(0), const<u64>(8))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%19)), const<i32>(30), array_decay<ptr<i8>, length=Some(33)>(%20));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if not<bool>(ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<const i8>, length=Some(3)>(%6)), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2)))), null<ptr<void>>))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%21)), const<i32>(32), array_decay<ptr<i8>, length=Some(18)>(%22));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if eq<ptr<const i8>>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(%6), const<i32>(2)))), pointer_cast<ptr<const i8>, reason=usual_arith>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<const i8>, length=Some(3)>(%6)), const<i32>(0), const<u64>(3))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%23)), const<i32>(33), array_decay<ptr<i8>, length=Some(33)>(%24));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if not<bool>(ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<const i8>, length=Some(8)>(%7)), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2)))), null<ptr<void>>))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%25)), const<i32>(35), array_decay<ptr<i8>, length=Some(18)>(%26));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if eq<ptr<const i8>>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(8)>(%7), const<i32>(3)))), pointer_cast<ptr<const i8>, reason=usual_arith>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<const i8>, length=Some(8)>(%7)), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%27)), const<i32>(36), array_decay<ptr<i8>, length=Some(26)>(%28));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if eq<ptr<const i8>>(addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(8)>(%7), const<i32>(3)))), pointer_cast<ptr<const i8>, reason=usual_arith>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<const i8>, length=Some(8)>(%7)), const<i32>(0), const<u64>(8))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%29)), const<i32>(37), array_decay<ptr<i8>, length=Some(33)>(%30));
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
