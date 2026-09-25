/* PR middle-end/123635 */
/* { dg-do run { target bitint } } */
/* { dg-options "-std=c23 -pedantic-errors" } */
/* { dg-skip-if "" { ! run_expensive_tests }  { "*" } { "-O0" "-O2" } } */
/* { dg-skip-if "" { ! run_expensive_tests } { "-flto" } { "" } } */

#if __BITINT_MAXWIDTH__ >= 513
_BitInt(513) a, b, c, d;
unsigned _BitInt(513) e, f, g, h;
_BitInt(513) i, j, k;
unsigned _BitInt(513) l, m, n;
#endif

[[gnu::noipa]] void do_copy(void *p, const void *q, __SIZE_TYPE__ r) {
  __builtin_memcpy(p, q, r);
}

/* Obtain the value of N from a _BitInt(N)-typed expression X
   at compile time.  */
#define S(x)                                                                   \
  ((typeof(x))-1 < 0 ? __builtin_clrsbg(__builtin_choose_expr(                 \
                           (typeof(x))-1 < 0, (typeof(x))-1, -1)) +            \
                           1                                                   \
                     : __builtin_popcountg(__builtin_choose_expr(              \
                           (typeof(x))-1 < 0, 0U, (typeof(x))-1)))

#define CEIL(x, y) (((x) + (y) - 1) / (y))

/* Promote a _BitInt type to include its padding bits.  */
#if defined(__s390x__) || defined(__arm__) || defined(__riscv)
#define PROMOTED_SIZE(x) sizeof(x)
#elif defined(__loongarch__)
#define PROMOTED_SIZE(x) (sizeof(x) > 8 ? CEIL(S(x), 64) * 8 : sizeof(x))
#endif

/* Macro to test whether (on targets where psABI requires it) _BitInt
   with padding bits have those filled with sign or zero extension.  */
#if defined(__s390x__) || defined(__arm__) || defined(__loongarch__) ||        \
    defined(__riscv)
#define BEXTC1(x, uns)                                                         \
  do {                                                                         \
    uns _BitInt(PROMOTED_SIZE(x) * __CHAR_BIT__) __x;                          \
    do_copy(&__x, &(x), sizeof(__x));                                          \
    if (__x != (typeof(x))__x)                                                 \
      __builtin_abort();                                                       \
  } while (0)

#define BEXTC(x)                                                               \
  do {                                                                         \
    if ((typeof(x))-1 < 0)                                                     \
      BEXTC1((x), signed);                                                     \
    else                                                                       \
      BEXTC1((x), unsigned);                                                   \
  } while (0)
#else
#define BEXTC(x)                                                               \
  do {                                                                         \
    (void)(x);                                                                 \
  } while (0)
#endif

#if __BITINT_MAXWIDTH__ >= 513
[[gnu::noipa]] void f1(_BitInt(513) q, _BitInt(513) r, _BitInt(513) s,
                       unsigned _BitInt(513) t, unsigned _BitInt(513) u,
                       unsigned _BitInt(513) v) {
  a = q * r;
  BEXTC(a);
  b = r * s;
  BEXTC(b);
  c = q / r;
  BEXTC(c);
  d = q / s;
  BEXTC(d);
  e = t * u;
  BEXTC(e);
  f = u * v;
  BEXTC(f);
  g = t / u;
  BEXTC(g);
  h = t / v;
  BEXTC(h);
}

[[gnu::noipa]] void f2(float q, double r, long double s, float t, double u,
                       long double v) {
  i = q;
  BEXTC(i);
  j = r;
  BEXTC(j);
  k = s;
  BEXTC(k);
  l = t;
  BEXTC(l);
  m = u;
  BEXTC(m);
  n = v;
  BEXTC(n);
}
#endif

int
main() {
#if __BITINT_MAXWIDTH__ >= 513
  __builtin_memset(&a, 0x55, sizeof(a));
  __builtin_memset(&b, 0xaa, sizeof(b));
  __builtin_memset(&c, 0x55, sizeof(c));
  __builtin_memset(&d, 0xaa, sizeof(d));
  __builtin_memset(&e, 0x55, sizeof(e));
  __builtin_memset(&f, 0xaa, sizeof(f));
  __builtin_memset(&g, 0x55, sizeof(g));
  __builtin_memset(&h, 0xaa, sizeof(h));
  f1(-53323980256963787505256507743137477556434962931963225515943461794698643113423wb,
     -10076482373458251489901780456236592759327822657780415144730546867053397315531wb,
     9430367348600775477158545473775377451258484445522540280907903691748059121081wb,
     15046745594550617619205422464231805109110883864578289024439083517871820578179553927615539526791313634437787081814763432804808038115388367331529035246240655uwb,
     20633637828717837096174917874088391607464281656818868213468970773994599068609617673436080725569780340050358299419252926775025136778754971701553110169281uwb,
     5136122090451895036220764749952166060863831396714271041799786889820341177646647112770727950839029515643589984981094121423221389337601736282556974uwb);
  __builtin_memset(&i, 0x55, sizeof(i));
  __builtin_memset(&j, 0xaa, sizeof(j));
  __builtin_memset(&k, 0x55, sizeof(k));
  __builtin_memset(&l, 0xaa, sizeof(l));
  __builtin_memset(&m, 0x55, sizeof(m));
  __builtin_memset(&n, 0xaa, sizeof(n));
  f2(12345678.5f, -234567891234567.125, 123465987893275.53244532L, 12345678.5f,
     234567891234567.125, 123465987893275.53244532L);
#endif
}



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
// DEFAULT-NEXT:     global %0 a: i513b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 b: i513b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 c: i513b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 d: i513b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 e: u513b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 f: u513b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 g: u513b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %7 h: u513b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %8 i: i513b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %9 j: i513b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %10 k: i513b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %11 l: u513b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %12 m: u513b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %13 n: u513b [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %14 @do_copy(%15 p: ptr<void>, %16 q: ptr<const void>, %17 r: u64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(__builtin_memcpy, read<ptr<void>>(%15), read<ptr<const void>>(%16), read<u64>(%17));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @f1(%19 q: i513b, %20 r: i513b, %21 s: i513b, %22 t: u513b, %23 u: u513b, %24 v: u513b) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i513b>(%0, mul<i513b, overflow=ub>(read<i513b>(%19), read<i513b>(%20)));
// DEFAULT-NEXT:         do %33
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 read<i513b>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<i513b>(%1, mul<i513b, overflow=ub>(read<i513b>(%20), read<i513b>(%21)));
// DEFAULT-NEXT:         do %34
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 read<i513b>(%1);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<i513b>(%2, div<i513b, by_zero=ub, min_by_neg_one=ub>(read<i513b>(%19), read<i513b>(%20)));
// DEFAULT-NEXT:         do %35
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 read<i513b>(%2);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<i513b>(%3, div<i513b, by_zero=ub, min_by_neg_one=ub>(read<i513b>(%19), read<i513b>(%21)));
// DEFAULT-NEXT:         do %36
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 read<i513b>(%3);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<u513b>(%4, mul<u513b, overflow=wrap>(read<u513b>(%22), read<u513b>(%23)));
// DEFAULT-NEXT:         do %37
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 read<u513b>(%4);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<u513b>(%5, mul<u513b, overflow=wrap>(read<u513b>(%23), read<u513b>(%24)));
// DEFAULT-NEXT:         do %38
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 read<u513b>(%5);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<u513b>(%6, div<u513b, by_zero=ub>(read<u513b>(%22), read<u513b>(%23)));
// DEFAULT-NEXT:         do %39
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 read<u513b>(%6);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<u513b>(%7, div<u513b, by_zero=ub>(read<u513b>(%22), read<u513b>(%24)));
// DEFAULT-NEXT:         do %40
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 read<u513b>(%7);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %25 @f2(%26 q: f32, %27 r: f64, %28 s: f80, %29 t: f32, %30 u: f64, %31 v: f80) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i513b>(%8, float_to_int<i513b, reason=assign, out_of_range=ub, exceptions=ignore>(read<f32>(%26)));
// DEFAULT-NEXT:         do %41
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 read<i513b>(%8);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<i513b>(%9, float_to_int<i513b, reason=assign, out_of_range=ub, exceptions=ignore>(read<f64>(%27)));
// DEFAULT-NEXT:         do %42
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 read<i513b>(%9);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<i513b>(%10, float_to_int<i513b, reason=assign, out_of_range=ub, exceptions=ignore>(read<f80>(%28)));
// DEFAULT-NEXT:         do %43
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 read<i513b>(%10);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<u513b>(%11, float_to_int<u513b, reason=assign, out_of_range=ub, exceptions=ignore>(read<f32>(%29)));
// DEFAULT-NEXT:         do %44
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 read<u513b>(%11);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<u513b>(%12, float_to_int<u513b, reason=assign, out_of_range=ub, exceptions=ignore>(read<f64>(%30)));
// DEFAULT-NEXT:         do %45
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 read<u513b>(%12);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<u513b>(%13, float_to_int<u513b, reason=assign, out_of_range=ub, exceptions=ignore>(read<f80>(%31)));
// DEFAULT-NEXT:         do %46
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 read<u513b>(%13);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %32 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(__builtin_memset, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i513b>>(%0)), const<i32>(85), const<u64>(72));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(__builtin_memset, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i513b>>(%1)), const<i32>(170), const<u64>(72));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(__builtin_memset, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i513b>>(%2)), const<i32>(85), const<u64>(72));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(__builtin_memset, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i513b>>(%3)), const<i32>(170), const<u64>(72));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(__builtin_memset, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<u513b>>(%4)), const<i32>(85), const<u64>(72));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(__builtin_memset, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<u513b>>(%5)), const<i32>(170), const<u64>(72));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(__builtin_memset, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<u513b>>(%6)), const<i32>(85), const<u64>(72));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(__builtin_memset, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<u513b>>(%7)), const<i32>(170), const<u64>(72));
// DEFAULT-NEXT:         call<void, signature=fn(i513b, i513b, i513b, u513b, u513b, u513b) -> void>(%18, widen<i513b, reason=arg>(neg<i256b, overflow=ub>(const<i256b>(53323980256963787505256507743137477556434962931963225515943461794698643113423))), widen<i513b, reason=arg>(neg<i254b, overflow=ub>(const<i254b>(10076482373458251489901780456236592759327822657780415144730546867053397315531))), widen<i513b, reason=arg>(const<i254b>(9430367348600775477158545473775377451258484445522540280907903691748059121081)), const<u513b>(15046745594550617619205422464231805109110883864578289024439083517871820578179553927615539526791313634437787081814763432804808038115388367331529035246240655), widen<u513b, reason=arg>(const<u503b>(20633637828717837096174917874088391607464281656818868213468970773994599068609617673436080725569780340050358299419252926775025136778754971701553110169281)), widen<u513b, reason=arg>(const<u481b>(5136122090451895036220764749952166060863831396714271041799786889820341177646647112770727950839029515643589984981094121423221389337601736282556974)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(__builtin_memset, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i513b>>(%8)), const<i32>(85), const<u64>(72));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(__builtin_memset, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i513b>>(%9)), const<i32>(170), const<u64>(72));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(__builtin_memset, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i513b>>(%10)), const<i32>(85), const<u64>(72));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(__builtin_memset, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<u513b>>(%11)), const<i32>(170), const<u64>(72));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(__builtin_memset, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<u513b>>(%12)), const<i32>(85), const<u64>(72));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(__builtin_memset, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<u513b>>(%13)), const<i32>(170), const<u64>(72));
// DEFAULT-NEXT:         call<void, signature=fn(f32, f64, f80, f32, f64, f80) -> void>(%25, const<f32>(12345678.0), neg<f64>(const<f64>(234567891234567.13)), const<f80>(123465987893275.532448), const<f32>(12345678.0), const<f64>(234567891234567.13), const<f80>(123465987893275.532448));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
