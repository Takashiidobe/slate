/* { dg-do run } */
/* This test is too big for small targets.  */
/* { dg-require-effective-target size32plus } */

#include <stdlib.h>

#define N 1024
signed char        sc[N];
short              ss[N];
int                si[N];
long long          sl[N];
unsigned char      uc[N];
unsigned short     us[N];
unsigned int       ui[N];
unsigned long long ul[N];
float              f[N];
double             d[N];

#define FN1(from, to)                                                          \
  __attribute__((noinline, noclone)) void from##2##to(void) {                  \
    int i;                                                                     \
    for (i = 0; i < N; i++)                                                    \
      to[i] = from[i];                                                         \
  }
#define FN(intt, fltt) FN1(intt, fltt) FN1(fltt, intt)

FN(sc, f)
FN(ss, f)
FN(si, f)
FN(sl, f)
FN(uc, f)
FN(us, f)
FN(ui, f)
FN(ul, f)
FN(sc, d)
FN(ss, d)
FN(si, d)
FN(sl, d)
FN(uc, d)
FN(us, d)
FN(ui, d)
FN(ul, d)

#define FLTTEST(min, max, intt)                                                \
  __attribute__((noinline, noclone)) void flttointtest##intt(void) {           \
    int             i;                                                         \
    volatile float  fltmin, fltmax, vf, vf2;                                   \
    volatile double dblmin, dblmax, vd, vd2;                                   \
    if (min == 0)                                                              \
      fltmin = 0.0f;                                                           \
    else {                                                                     \
      vf2 = fltmin = min - 1.0f;                                               \
      for (vf = 1.0f; fltmin = vf2 + vf, fltmin == vf2; vf = vf * 2.0f)        \
        ;                                                                      \
    }                                                                          \
    vf2 = fltmax = max + 1.0f;                                                 \
    for (vf = 1.0f; fltmax = vf2 - vf, fltmax == vf2; vf = vf * 2.0f)          \
      ;                                                                        \
    if (min == 0)                                                              \
      dblmin = 0.0;                                                            \
    else {                                                                     \
      vd2 = dblmin = min - 1.0;                                                \
      for (vd = 1.0; dblmin = vd2 + vd, dblmin == vd2; vd = vd * 2.0)          \
        ;                                                                      \
    }                                                                          \
    vd2 = dblmax = max + 1.0;                                                  \
    for (vd = 1.0; dblmax = vd2 - vd, dblmax == vd2; vd = vd * 2.0)            \
      ;                                                                        \
    for (i = 0; i < N; i++) {                                                  \
      asm("");                                                                 \
      if (i == 0)                                                              \
        f[i] = fltmin;                                                         \
      else if (i < N / 4)                                                      \
        f[i] = fltmin + i + 0.25f;                                             \
      else if (i < 3 * N / 4)                                                  \
        f[i] = (fltmax + fltmin) / 2.0 - N * 8 + 16.0f * i;                    \
      else                                                                     \
        f[i] = fltmax - N + 1 + i;                                             \
      if (f[i] < fltmin)                                                       \
        f[i] = fltmin;                                                         \
      if (f[i] > fltmax)                                                       \
        f[i] = fltmax;                                                         \
      if (i == 0)                                                              \
        d[i] = dblmin;                                                         \
      else if (i < N / 4)                                                      \
        d[i] = dblmin + i + 0.25f;                                             \
      else if (i < 3 * N / 4)                                                  \
        d[i] = (dblmax + dblmin) / 2.0 - N * 8 + 16.0f * i;                    \
      else                                                                     \
        d[i] = dblmax - N + 1 + i;                                             \
      if (d[i] < dblmin)                                                       \
        d[i] = dblmin;                                                         \
      if (d[i] > dblmax)                                                       \
        d[i] = dblmax;                                                         \
    }                                                                          \
    f2##intt();                                                                \
    for (i = 0; i < N; i++)                                                    \
      if (intt[i] != (__typeof(intt[0]))f[i])                                  \
        abort();                                                               \
    d2##intt();                                                                \
    for (i = 0; i < N; i++)                                                    \
      if (intt[i] != (__typeof(intt[0]))d[i])                                  \
        abort();                                                               \
    for (i = 0; i < N; i++) {                                                  \
      unsigned long long r = rand();                                           \
      r                    = (r << 21) ^ (unsigned)rand();                     \
      r                    = (r << 21) ^ (unsigned)rand();                     \
      asm("");                                                                 \
      f[i] = (r >> 59) / 32.0f + (__typeof(intt[0]))r;                         \
      if (f[i] < fltmin)                                                       \
        f[i] = fltmin;                                                         \
      if (f[i] > fltmax)                                                       \
        f[i] = fltmax;                                                         \
      d[i] = (r >> 59) / 32.0 + (__typeof(intt[0]))r;                          \
      if (d[i] < dblmin)                                                       \
        f[i] = dblmin;                                                         \
      if (d[i] > dblmax)                                                       \
        f[i] = dblmax;                                                         \
    }                                                                          \
    f2##intt();                                                                \
    for (i = 0; i < N; i++)                                                    \
      if (intt[i] != (__typeof(intt[0]))f[i])                                  \
        abort();                                                               \
    d2##intt();                                                                \
    for (i = 0; i < N; i++)                                                    \
      if (intt[i] != (__typeof(intt[0]))d[i])                                  \
        abort();                                                               \
  }                                                                            \
                                                                               \
  __attribute__((noinline, noclone)) void inttoflttest##intt(void) {           \
    int             i;                                                         \
    volatile float  vf;                                                        \
    volatile double vd;                                                        \
    for (i = 0; i < N; i++) {                                                  \
      asm("");                                                                 \
      if (i < N / 4)                                                           \
        intt[i] = min + i;                                                     \
      else if (i < 3 * N / 4)                                                  \
        intt[i] = (max + min) / 2 - N * 8 + 16 * i;                            \
      else                                                                     \
        intt[i] = max - N + 1 + i;                                             \
    }                                                                          \
    intt##2f();                                                                \
    for (i = 0; i < N; i++) {                                                  \
      vf = intt[i];                                                            \
      if (f[i] != vf)                                                          \
        abort();                                                               \
    }                                                                          \
    intt##2d();                                                                \
    for (i = 0; i < N; i++) {                                                  \
      vd = intt[i];                                                            \
      if (d[i] != vd)                                                          \
        abort();                                                               \
    }                                                                          \
    for (i = 0; i < N; i++) {                                                  \
      unsigned long long r = rand();                                           \
      r                    = (r << 21) ^ (unsigned)rand();                     \
      r                    = (r << 21) ^ (unsigned)rand();                     \
      asm("");                                                                 \
      intt[i] = r;                                                             \
    }                                                                          \
    intt##2f();                                                                \
    for (i = 0; i < N; i++) {                                                  \
      vf = intt[i];                                                            \
      if (f[i] != vf)                                                          \
        abort();                                                               \
    }                                                                          \
    intt##2d();                                                                \
    for (i = 0; i < N; i++) {                                                  \
      vd = intt[i];                                                            \
      if (d[i] != vd)                                                          \
        abort();                                                               \
    }                                                                          \
  }

FLTTEST(-__SCHAR_MAX__ - 1, __SCHAR_MAX__, sc)
FLTTEST(-__SHRT_MAX__ - 1, __SHRT_MAX__, ss)
FLTTEST(-__INT_MAX__ - 1, __INT_MAX__, si)
FLTTEST(-__LONG_LONG_MAX__ - 1LL, __LONG_LONG_MAX__, sl)
FLTTEST(0, 2U * __SCHAR_MAX__ + 1, uc)
FLTTEST(0, 2U * __SHRT_MAX__ + 1, us)
FLTTEST(0, 2U * __INT_MAX__ + 1, ui)
FLTTEST(0, 2ULL * __LONG_LONG_MAX__ + 1, ul)

int
main() {
  flttointtestsc();
  flttointtestss();
  flttointtestsi();
  flttointtestsl();
  flttointtestuc();
  flttointtestus();
  flttointtestui();
  flttointtestul();
  inttoflttestsc();
  inttoflttestss();
  inttoflttestsi();
  inttoflttestsl();
  inttoflttestuc();
  inttoflttestus();
  inttoflttestui();
  inttoflttestul();
  return 0;
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
// DEFAULT-NEXT:     global %2 sc: array<i8, 1024> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 ss: array<i16, 1024> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 si: array<i32, 1024> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 sl: array<i64, 1024> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 uc: array<u8, 1024> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %7 us: array<u16, 1024> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %8 ui: array<u32, 1024> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %9 ul: array<u64, 1024> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %10 f: array<f32, 1024> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %11 d: array<f64, 1024> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @rand() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %12 @sc2f() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %13 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %205
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%13, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%13), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %365: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:                 let %366: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%365), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%13, read<i32>(%366));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%13))), int_to_float<f32, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1024)>(%2), read<i32>(%13))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @f2sc() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %15 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %206
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%15, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%15), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %367: i32 [synthetic] = read<i32>(%15);
// DEFAULT-NEXT:                 let %368: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%367), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%15, read<i32>(%368));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1024)>(%2), read<i32>(%15))), float_to_int<i8, reason=assign, out_of_range=ub, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%15))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @ss2f() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %17 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %207
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%17, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%17), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %369: i32 [synthetic] = read<i32>(%17);
// DEFAULT-NEXT:                 let %370: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%369), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%17, read<i32>(%370));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%17))), int_to_float<f32, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(1024)>(%3), read<i32>(%17))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @f2ss() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %19 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %208
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%19, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%19), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %371: i32 [synthetic] = read<i32>(%19);
// DEFAULT-NEXT:                 let %372: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%371), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%19, read<i32>(%372));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(1024)>(%3), read<i32>(%19))), float_to_int<i16, reason=assign, out_of_range=ub, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%19))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @si2f() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %21 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %209
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%21, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%21), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %373: i32 [synthetic] = read<i32>(%21);
// DEFAULT-NEXT:                 let %374: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%373), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%21, read<i32>(%374));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%21))), int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1024)>(%4), read<i32>(%21))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @f2si() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %23 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %210
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%23, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%23), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %375: i32 [synthetic] = read<i32>(%23);
// DEFAULT-NEXT:                 let %376: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%375), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%23, read<i32>(%376));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1024)>(%4), read<i32>(%23))), float_to_int<i32, reason=assign, out_of_range=ub, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%23))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %24 @sl2f() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %25 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %211
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%25, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%25), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %377: i32 [synthetic] = read<i32>(%25);
// DEFAULT-NEXT:                 let %378: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%377), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%25, read<i32>(%378));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%25))), int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(1024)>(%5), read<i32>(%25))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %26 @f2sl() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %27 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %212
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%27, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%27), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %379: i32 [synthetic] = read<i32>(%27);
// DEFAULT-NEXT:                 let %380: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%379), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%27, read<i32>(%380));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(1024)>(%5), read<i32>(%27))), float_to_int<i64, reason=assign, out_of_range=ub, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%27))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %28 @uc2f() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %29 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %213
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%29, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%29), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %381: i32 [synthetic] = read<i32>(%29);
// DEFAULT-NEXT:                 let %382: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%381), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%29, read<i32>(%382));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%29))), int_to_float<f32, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(1024)>(%6), read<i32>(%29))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %30 @f2uc() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %31 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %214
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%31, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%31), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %383: i32 [synthetic] = read<i32>(%31);
// DEFAULT-NEXT:                 let %384: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%383), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%31, read<i32>(%384));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(1024)>(%6), read<i32>(%31))), float_to_int<u8, reason=assign, out_of_range=ub, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%31))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %32 @us2f() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %33 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %215
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%33, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%33), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %385: i32 [synthetic] = read<i32>(%33);
// DEFAULT-NEXT:                 let %386: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%385), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%33, read<i32>(%386));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%33))), int_to_float<f32, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(read<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<u16>, length=Some(1024)>(%7), read<i32>(%33))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %34 @f2us() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %35 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %216
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%35, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%35), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %387: i32 [synthetic] = read<i32>(%35);
// DEFAULT-NEXT:                 let %388: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%387), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%35, read<i32>(%388));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<u16>, length=Some(1024)>(%7), read<i32>(%35))), float_to_int<u16, reason=assign, out_of_range=ub, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%35))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %36 @ui2f() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %37 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %217
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%37, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%37), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %389: i32 [synthetic] = read<i32>(%37);
// DEFAULT-NEXT:                 let %390: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%389), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%37, read<i32>(%390));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%37))), int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(1024)>(%8), read<i32>(%37))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %38 @f2ui() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %39 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %218
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%39, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%39), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %391: i32 [synthetic] = read<i32>(%39);
// DEFAULT-NEXT:                 let %392: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%391), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%39, read<i32>(%392));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(1024)>(%8), read<i32>(%39))), float_to_int<u32, reason=assign, out_of_range=ub, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%39))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %40 @ul2f() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %41 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %219
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%41, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%41), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %393: i32 [synthetic] = read<i32>(%41);
// DEFAULT-NEXT:                 let %394: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%393), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%41, read<i32>(%394));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%41))), int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(1024)>(%9), read<i32>(%41))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %42 @f2ul() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %43 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %220
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%43, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%43), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %395: i32 [synthetic] = read<i32>(%43);
// DEFAULT-NEXT:                 let %396: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%395), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%43, read<i32>(%396));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(1024)>(%9), read<i32>(%43))), float_to_int<u64, reason=assign, out_of_range=ub, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%43))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %44 @sc2d() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %45 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %221
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%45, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%45), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %397: i32 [synthetic] = read<i32>(%45);
// DEFAULT-NEXT:                 let %398: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%397), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%45, read<i32>(%398));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%45))), int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1024)>(%2), read<i32>(%45))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %46 @d2sc() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %47 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %222
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%47, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%47), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %399: i32 [synthetic] = read<i32>(%47);
// DEFAULT-NEXT:                 let %400: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%399), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%47, read<i32>(%400));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1024)>(%2), read<i32>(%47))), float_to_int<i8, reason=assign, out_of_range=ub, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%47))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %48 @ss2d() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %49 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %223
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%49, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%49), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %401: i32 [synthetic] = read<i32>(%49);
// DEFAULT-NEXT:                 let %402: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%401), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%49, read<i32>(%402));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%49))), int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(1024)>(%3), read<i32>(%49))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %50 @d2ss() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %51 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %224
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%51, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%51), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %403: i32 [synthetic] = read<i32>(%51);
// DEFAULT-NEXT:                 let %404: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%403), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%51, read<i32>(%404));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(1024)>(%3), read<i32>(%51))), float_to_int<i16, reason=assign, out_of_range=ub, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%51))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %52 @si2d() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %53 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %225
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%53, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%53), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %405: i32 [synthetic] = read<i32>(%53);
// DEFAULT-NEXT:                 let %406: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%405), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%53, read<i32>(%406));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%53))), int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1024)>(%4), read<i32>(%53))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %54 @d2si() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %55 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %226
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%55, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%55), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %407: i32 [synthetic] = read<i32>(%55);
// DEFAULT-NEXT:                 let %408: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%407), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%55, read<i32>(%408));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1024)>(%4), read<i32>(%55))), float_to_int<i32, reason=assign, out_of_range=ub, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%55))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %56 @sl2d() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %57 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %227
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%57, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%57), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %409: i32 [synthetic] = read<i32>(%57);
// DEFAULT-NEXT:                 let %410: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%409), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%57, read<i32>(%410));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%57))), int_to_float<f64, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(1024)>(%5), read<i32>(%57))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %58 @d2sl() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %59 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %228
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%59, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%59), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %411: i32 [synthetic] = read<i32>(%59);
// DEFAULT-NEXT:                 let %412: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%411), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%59, read<i32>(%412));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(1024)>(%5), read<i32>(%59))), float_to_int<i64, reason=assign, out_of_range=ub, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%59))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %60 @uc2d() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %61 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %229
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%61, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%61), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %413: i32 [synthetic] = read<i32>(%61);
// DEFAULT-NEXT:                 let %414: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%413), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%61, read<i32>(%414));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%61))), int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(1024)>(%6), read<i32>(%61))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %62 @d2uc() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %63 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %230
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%63, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%63), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %415: i32 [synthetic] = read<i32>(%63);
// DEFAULT-NEXT:                 let %416: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%415), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%63, read<i32>(%416));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(1024)>(%6), read<i32>(%63))), float_to_int<u8, reason=assign, out_of_range=ub, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%63))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %64 @us2d() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %65 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %231
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%65, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%65), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %417: i32 [synthetic] = read<i32>(%65);
// DEFAULT-NEXT:                 let %418: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%417), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%65, read<i32>(%418));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%65))), int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(read<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<u16>, length=Some(1024)>(%7), read<i32>(%65))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %66 @d2us() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %67 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %232
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%67, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%67), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %419: i32 [synthetic] = read<i32>(%67);
// DEFAULT-NEXT:                 let %420: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%419), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%67, read<i32>(%420));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<u16>, length=Some(1024)>(%7), read<i32>(%67))), float_to_int<u16, reason=assign, out_of_range=ub, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%67))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %68 @ui2d() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %69 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %233
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%69, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%69), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %421: i32 [synthetic] = read<i32>(%69);
// DEFAULT-NEXT:                 let %422: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%421), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%69, read<i32>(%422));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%69))), int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(1024)>(%8), read<i32>(%69))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %70 @d2ui() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %71 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %234
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%71, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%71), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %423: i32 [synthetic] = read<i32>(%71);
// DEFAULT-NEXT:                 let %424: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%423), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%71, read<i32>(%424));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(1024)>(%8), read<i32>(%71))), float_to_int<u32, reason=assign, out_of_range=ub, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%71))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %72 @ul2d() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %73 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %235
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%73, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%73), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %425: i32 [synthetic] = read<i32>(%73);
// DEFAULT-NEXT:                 let %426: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%425), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%73, read<i32>(%426));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%73))), int_to_float<f64, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(1024)>(%9), read<i32>(%73))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %74 @d2ul() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %75 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %236
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%75, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%75), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %427: i32 [synthetic] = read<i32>(%75);
// DEFAULT-NEXT:                 let %428: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%427), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%75, read<i32>(%428));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(1024)>(%9), read<i32>(%75))), float_to_int<u64, reason=assign, out_of_range=ub, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%75))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %76 @flttointtestsc() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %77 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %78 fltmin: volatile f32 [storage=automatic];
// DEFAULT-NEXT:         let %79 fltmax: volatile f32 [storage=automatic];
// DEFAULT-NEXT:         let %80 vf: volatile f32 [storage=automatic];
// DEFAULT-NEXT:         let %81 vf2: volatile f32 [storage=automatic];
// DEFAULT-NEXT:         let %82 dblmin: volatile f64 [storage=automatic];
// DEFAULT-NEXT:         let %83 dblmax: volatile f64 [storage=automatic];
// DEFAULT-NEXT:         let %84 vd: volatile f64 [storage=automatic];
// DEFAULT-NEXT:         let %85 vd2: volatile f64 [storage=automatic];
// DEFAULT-NEXT:         if eq<i32>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(127)), const<i32>(1)), const<i32>(0))
// DEFAULT-NEXT:             write<f32, volatile>(%78, const<f32>(0.0));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<f32, volatile>(%78, sub<f32, rounding=nearest_even, exceptions=ignore>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(127)), const<i32>(1))), const<f32>(1.0)));
// DEFAULT-NEXT:                 write<f32, volatile>(%81, sub<f32, rounding=nearest_even, exceptions=ignore>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(127)), const<i32>(1))), const<f32>(1.0)));
// DEFAULT-NEXT:                 for %237
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<f32, volatile>(%80, const<f32>(1.0));
// DEFAULT-NEXT:                     condition: {
// DEFAULT-NEXT:                         write<f32, volatile>(%78, add<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%81), read<f32, volatile>(%80)));
// DEFAULT-NEXT:                         yield eq<f32, exceptions=ignore>(read<f32, volatile>(%78), read<f32, volatile>(%81));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         write<f32, volatile>(%80, mul<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%80), const<f32>(2.0)));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         ;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<f32, volatile>(%79, add<f32, rounding=nearest_even, exceptions=ignore>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(127)), const<f32>(1.0)));
// DEFAULT-NEXT:         write<f32, volatile>(%81, add<f32, rounding=nearest_even, exceptions=ignore>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(127)), const<f32>(1.0)));
// DEFAULT-NEXT:         for %238
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<f32, volatile>(%80, const<f32>(1.0));
// DEFAULT-NEXT:             condition: {
// DEFAULT-NEXT:                 write<f32, volatile>(%79, sub<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%81), read<f32, volatile>(%80)));
// DEFAULT-NEXT:                 yield eq<f32, exceptions=ignore>(read<f32, volatile>(%79), read<f32, volatile>(%81));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 write<f32, volatile>(%80, mul<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%80), const<f32>(2.0)));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:         if eq<i32>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(127)), const<i32>(1)), const<i32>(0))
// DEFAULT-NEXT:             write<f64, volatile>(%82, const<f64>(0.0));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<f64, volatile>(%82, sub<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(127)), const<i32>(1))), const<f64>(1.0)));
// DEFAULT-NEXT:                 write<f64, volatile>(%85, sub<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(127)), const<i32>(1))), const<f64>(1.0)));
// DEFAULT-NEXT:                 for %239
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<f64, volatile>(%84, const<f64>(1.0));
// DEFAULT-NEXT:                     condition: {
// DEFAULT-NEXT:                         write<f64, volatile>(%82, add<f64, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%85), read<f64, volatile>(%84)));
// DEFAULT-NEXT:                         yield eq<f64, exceptions=ignore>(read<f64, volatile>(%82), read<f64, volatile>(%85));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         write<f64, volatile>(%84, mul<f64, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%84), const<f64>(2.0)));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         ;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<f64, volatile>(%83, add<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(127)), const<f64>(1.0)));
// DEFAULT-NEXT:         write<f64, volatile>(%85, add<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(127)), const<f64>(1.0)));
// DEFAULT-NEXT:         for %240
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<f64, volatile>(%84, const<f64>(1.0));
// DEFAULT-NEXT:             condition: {
// DEFAULT-NEXT:                 write<f64, volatile>(%83, sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%85), read<f64, volatile>(%84)));
// DEFAULT-NEXT:                 yield eq<f64, exceptions=ignore>(read<f64, volatile>(%83), read<f64, volatile>(%85));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 write<f64, volatile>(%84, mul<f64, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%84), const<f64>(2.0)));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:         for %241
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%77, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%77), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %429: i32 [synthetic] = read<i32>(%77);
// DEFAULT-NEXT:                 let %430: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%429), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%77, read<i32>(%430));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     asm "";
// DEFAULT-NEXT:                     if eq<i32>(read<i32>(%77), const<i32>(0))
// DEFAULT-NEXT:                         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%77))), read<f32, volatile>(%78));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         if lt<i32>(read<i32>(%77), div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(1024), const<i32>(4)))
// DEFAULT-NEXT:                             write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%77))), add<f32, rounding=nearest_even, exceptions=ignore>(add<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%78), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%77))), const<f32>(0.25)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             if lt<i32>(read<i32>(%77), div<i32, by_zero=ub, min_by_neg_one=ub>(mul<i32, overflow=ub>(const<i32>(3), const<i32>(1024)), const<i32>(4)))
// DEFAULT-NEXT:                                 write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%77))), float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore>(sub<f64, rounding=nearest_even, exceptions=ignore>(div<f64, rounding=nearest_even, exceptions=ignore>(float_widen<f64, reason=usual_arith>(add<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%79), read<f32, volatile>(%78))), const<f64>(2.0)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(mul<i32, overflow=ub>(const<i32>(1024), const<i32>(8)))), float_widen<f64, reason=usual_arith>(mul<f32, rounding=nearest_even, exceptions=ignore>(const<f32>(16.0), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%77)))))));
// DEFAULT-NEXT:                             else
// DEFAULT-NEXT:                                 write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%77))), add<f32, rounding=nearest_even, exceptions=ignore>(add<f32, rounding=nearest_even, exceptions=ignore>(sub<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%79), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1024))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%77))));
// DEFAULT-NEXT:                     if lt<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%77)))), read<f32, volatile>(%78))
// DEFAULT-NEXT:                         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%77))), read<f32, volatile>(%78));
// DEFAULT-NEXT:                     if gt<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%77)))), read<f32, volatile>(%79))
// DEFAULT-NEXT:                         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%77))), read<f32, volatile>(%79));
// DEFAULT-NEXT:                     if eq<i32>(read<i32>(%77), const<i32>(0))
// DEFAULT-NEXT:                         write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%77))), read<f64, volatile>(%82));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         if lt<i32>(read<i32>(%77), div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(1024), const<i32>(4)))
// DEFAULT-NEXT:                             write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%77))), add<f64, rounding=nearest_even, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%82), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(read<i32>(%77))), float_widen<f64, reason=usual_arith>(const<f32>(0.25))));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             if lt<i32>(read<i32>(%77), div<i32, by_zero=ub, min_by_neg_one=ub>(mul<i32, overflow=ub>(const<i32>(3), const<i32>(1024)), const<i32>(4)))
// DEFAULT-NEXT:                                 write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%77))), add<f64, rounding=nearest_even, exceptions=ignore>(sub<f64, rounding=nearest_even, exceptions=ignore>(div<f64, rounding=nearest_even, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%83), read<f64, volatile>(%82)), const<f64>(2.0)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(mul<i32, overflow=ub>(const<i32>(1024), const<i32>(8)))), float_widen<f64, reason=usual_arith>(mul<f32, rounding=nearest_even, exceptions=ignore>(const<f32>(16.0), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%77))))));
// DEFAULT-NEXT:                             else
// DEFAULT-NEXT:                                 write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%77))), add<f64, rounding=nearest_even, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore>(sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%83), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1024))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(read<i32>(%77))));
// DEFAULT-NEXT:                     if lt<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%77)))), read<f64, volatile>(%82))
// DEFAULT-NEXT:                         write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%77))), read<f64, volatile>(%82));
// DEFAULT-NEXT:                     if gt<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%77)))), read<f64, volatile>(%83))
// DEFAULT-NEXT:                         write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%77))), read<f64, volatile>(%83));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%14);
// DEFAULT-NEXT:         for %242
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%77, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%77), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %431: i32 [synthetic] = read<i32>(%77);
// DEFAULT-NEXT:                 let %432: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%431), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%77, read<i32>(%432));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1024)>(%2), read<i32>(%77))))), widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%77)))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%46);
// DEFAULT-NEXT:         for %243
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%77, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%77), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %433: i32 [synthetic] = read<i32>(%77);
// DEFAULT-NEXT:                 let %434: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%433), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%77, read<i32>(%434));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1024)>(%2), read<i32>(%77))))), widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%77)))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         for %244
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%77, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%77), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %435: i32 [synthetic] = read<i32>(%77);
// DEFAULT-NEXT:                 let %436: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%435), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%77, read<i32>(%436));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %86 r: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(call<i32, signature=fn() -> i32>(%0)));
// DEFAULT-NEXT:                     write<u64>(%86, xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%86), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0)))));
// DEFAULT-NEXT:                     xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%86), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0))));
// DEFAULT-NEXT:                     write<u64>(%86, xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%86), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0)))));
// DEFAULT-NEXT:                     xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%86), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0))));
// DEFAULT-NEXT:                     asm "";
// DEFAULT-NEXT:                     write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%77))), add<f32, rounding=nearest_even, exceptions=ignore>(div<f32, rounding=nearest_even, exceptions=ignore>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%86), const<i32>(59))), const<f32>(32.0)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(read<u64>(%86)))))));
// DEFAULT-NEXT:                     if lt<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%77)))), read<f32, volatile>(%78))
// DEFAULT-NEXT:                         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%77))), read<f32, volatile>(%78));
// DEFAULT-NEXT:                     if gt<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%77)))), read<f32, volatile>(%79))
// DEFAULT-NEXT:                         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%77))), read<f32, volatile>(%79));
// DEFAULT-NEXT:                     write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%77))), add<f64, rounding=nearest_even, exceptions=ignore>(div<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%86), const<i32>(59))), const<f64>(32.0)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(read<u64>(%86)))))));
// DEFAULT-NEXT:                     if lt<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%77)))), read<f64, volatile>(%82))
// DEFAULT-NEXT:                         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%77))), float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%82)));
// DEFAULT-NEXT:                     if gt<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%77)))), read<f64, volatile>(%83))
// DEFAULT-NEXT:                         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%77))), float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%83)));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%14);
// DEFAULT-NEXT:         for %245
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%77, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%77), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %437: i32 [synthetic] = read<i32>(%77);
// DEFAULT-NEXT:                 let %438: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%437), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%77, read<i32>(%438));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1024)>(%2), read<i32>(%77))))), widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%77)))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%46);
// DEFAULT-NEXT:         for %246
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%77, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%77), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %439: i32 [synthetic] = read<i32>(%77);
// DEFAULT-NEXT:                 let %440: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%439), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%77, read<i32>(%440));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1024)>(%2), read<i32>(%77))))), widen<i32, reason=promotion>(float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%77)))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %87 @inttoflttestsc() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %88 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %89 vf: volatile f32 [storage=automatic];
// DEFAULT-NEXT:         let %90 vd: volatile f64 [storage=automatic];
// DEFAULT-NEXT:         for %247
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%88, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%88), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %441: i32 [synthetic] = read<i32>(%88);
// DEFAULT-NEXT:                 let %442: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%441), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%88, read<i32>(%442));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     asm "";
// DEFAULT-NEXT:                     if lt<i32>(read<i32>(%88), div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(1024), const<i32>(4)))
// DEFAULT-NEXT:                         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1024)>(%2), read<i32>(%88))), truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(127)), const<i32>(1)), read<i32>(%88))));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         if lt<i32>(read<i32>(%88), div<i32, by_zero=ub, min_by_neg_one=ub>(mul<i32, overflow=ub>(const<i32>(3), const<i32>(1024)), const<i32>(4)))
// DEFAULT-NEXT:                             write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1024)>(%2), read<i32>(%88))), truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(sub<i32, overflow=ub>(div<i32, by_zero=ub, min_by_neg_one=ub>(sub<i32, overflow=ub>(add<i32, overflow=ub>(const<i32>(127), neg<i32, overflow=ub>(const<i32>(127))), const<i32>(1)), const<i32>(2)), mul<i32, overflow=ub>(const<i32>(1024), const<i32>(8))), mul<i32, overflow=ub>(const<i32>(16), read<i32>(%88)))));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1024)>(%2), read<i32>(%88))), truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(add<i32, overflow=ub>(sub<i32, overflow=ub>(const<i32>(127), const<i32>(1024)), const<i32>(1)), read<i32>(%88))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%12);
// DEFAULT-NEXT:         for %248
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%88, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%88), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %443: i32 [synthetic] = read<i32>(%88);
// DEFAULT-NEXT:                 let %444: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%443), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%88, read<i32>(%444));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f32, volatile>(%89, int_to_float<f32, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1024)>(%2), read<i32>(%88))))));
// DEFAULT-NEXT:                     if ne<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%88)))), read<f32, volatile>(%89))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%44);
// DEFAULT-NEXT:         for %249
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%88, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%88), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %445: i32 [synthetic] = read<i32>(%88);
// DEFAULT-NEXT:                 let %446: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%445), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%88, read<i32>(%446));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f64, volatile>(%90, int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1024)>(%2), read<i32>(%88))))));
// DEFAULT-NEXT:                     if ne<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%88)))), read<f64, volatile>(%90))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %250
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%88, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%88), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %447: i32 [synthetic] = read<i32>(%88);
// DEFAULT-NEXT:                 let %448: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%447), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%88, read<i32>(%448));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %91 r: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(call<i32, signature=fn() -> i32>(%0)));
// DEFAULT-NEXT:                     write<u64>(%91, xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%91), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0)))));
// DEFAULT-NEXT:                     xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%91), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0))));
// DEFAULT-NEXT:                     write<u64>(%91, xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%91), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0)))));
// DEFAULT-NEXT:                     xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%91), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0))));
// DEFAULT-NEXT:                     asm "";
// DEFAULT-NEXT:                     write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1024)>(%2), read<i32>(%88))), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(read<u64>(%91))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%12);
// DEFAULT-NEXT:         for %251
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%88, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%88), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %449: i32 [synthetic] = read<i32>(%88);
// DEFAULT-NEXT:                 let %450: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%449), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%88, read<i32>(%450));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f32, volatile>(%89, int_to_float<f32, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1024)>(%2), read<i32>(%88))))));
// DEFAULT-NEXT:                     if ne<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%88)))), read<f32, volatile>(%89))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%44);
// DEFAULT-NEXT:         for %252
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%88, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%88), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %451: i32 [synthetic] = read<i32>(%88);
// DEFAULT-NEXT:                 let %452: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%451), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%88, read<i32>(%452));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f64, volatile>(%90, int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1024)>(%2), read<i32>(%88))))));
// DEFAULT-NEXT:                     if ne<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%88)))), read<f64, volatile>(%90))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %92 @flttointtestss() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %93 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %94 fltmin: volatile f32 [storage=automatic];
// DEFAULT-NEXT:         let %95 fltmax: volatile f32 [storage=automatic];
// DEFAULT-NEXT:         let %96 vf: volatile f32 [storage=automatic];
// DEFAULT-NEXT:         let %97 vf2: volatile f32 [storage=automatic];
// DEFAULT-NEXT:         let %98 dblmin: volatile f64 [storage=automatic];
// DEFAULT-NEXT:         let %99 dblmax: volatile f64 [storage=automatic];
// DEFAULT-NEXT:         let %100 vd: volatile f64 [storage=automatic];
// DEFAULT-NEXT:         let %101 vd2: volatile f64 [storage=automatic];
// DEFAULT-NEXT:         if eq<i32>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(32767)), const<i32>(1)), const<i32>(0))
// DEFAULT-NEXT:             write<f32, volatile>(%94, const<f32>(0.0));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<f32, volatile>(%94, sub<f32, rounding=nearest_even, exceptions=ignore>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(32767)), const<i32>(1))), const<f32>(1.0)));
// DEFAULT-NEXT:                 write<f32, volatile>(%97, sub<f32, rounding=nearest_even, exceptions=ignore>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(32767)), const<i32>(1))), const<f32>(1.0)));
// DEFAULT-NEXT:                 for %253
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<f32, volatile>(%96, const<f32>(1.0));
// DEFAULT-NEXT:                     condition: {
// DEFAULT-NEXT:                         write<f32, volatile>(%94, add<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%97), read<f32, volatile>(%96)));
// DEFAULT-NEXT:                         yield eq<f32, exceptions=ignore>(read<f32, volatile>(%94), read<f32, volatile>(%97));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         write<f32, volatile>(%96, mul<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%96), const<f32>(2.0)));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         ;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<f32, volatile>(%95, add<f32, rounding=nearest_even, exceptions=ignore>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(32767)), const<f32>(1.0)));
// DEFAULT-NEXT:         write<f32, volatile>(%97, add<f32, rounding=nearest_even, exceptions=ignore>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(32767)), const<f32>(1.0)));
// DEFAULT-NEXT:         for %254
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<f32, volatile>(%96, const<f32>(1.0));
// DEFAULT-NEXT:             condition: {
// DEFAULT-NEXT:                 write<f32, volatile>(%95, sub<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%97), read<f32, volatile>(%96)));
// DEFAULT-NEXT:                 yield eq<f32, exceptions=ignore>(read<f32, volatile>(%95), read<f32, volatile>(%97));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 write<f32, volatile>(%96, mul<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%96), const<f32>(2.0)));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:         if eq<i32>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(32767)), const<i32>(1)), const<i32>(0))
// DEFAULT-NEXT:             write<f64, volatile>(%98, const<f64>(0.0));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<f64, volatile>(%98, sub<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(32767)), const<i32>(1))), const<f64>(1.0)));
// DEFAULT-NEXT:                 write<f64, volatile>(%101, sub<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(32767)), const<i32>(1))), const<f64>(1.0)));
// DEFAULT-NEXT:                 for %255
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<f64, volatile>(%100, const<f64>(1.0));
// DEFAULT-NEXT:                     condition: {
// DEFAULT-NEXT:                         write<f64, volatile>(%98, add<f64, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%101), read<f64, volatile>(%100)));
// DEFAULT-NEXT:                         yield eq<f64, exceptions=ignore>(read<f64, volatile>(%98), read<f64, volatile>(%101));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         write<f64, volatile>(%100, mul<f64, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%100), const<f64>(2.0)));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         ;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<f64, volatile>(%99, add<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(32767)), const<f64>(1.0)));
// DEFAULT-NEXT:         write<f64, volatile>(%101, add<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(32767)), const<f64>(1.0)));
// DEFAULT-NEXT:         for %256
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<f64, volatile>(%100, const<f64>(1.0));
// DEFAULT-NEXT:             condition: {
// DEFAULT-NEXT:                 write<f64, volatile>(%99, sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%101), read<f64, volatile>(%100)));
// DEFAULT-NEXT:                 yield eq<f64, exceptions=ignore>(read<f64, volatile>(%99), read<f64, volatile>(%101));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 write<f64, volatile>(%100, mul<f64, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%100), const<f64>(2.0)));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:         for %257
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%93, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%93), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %453: i32 [synthetic] = read<i32>(%93);
// DEFAULT-NEXT:                 let %454: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%453), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%93, read<i32>(%454));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     asm "";
// DEFAULT-NEXT:                     if eq<i32>(read<i32>(%93), const<i32>(0))
// DEFAULT-NEXT:                         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%93))), read<f32, volatile>(%94));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         if lt<i32>(read<i32>(%93), div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(1024), const<i32>(4)))
// DEFAULT-NEXT:                             write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%93))), add<f32, rounding=nearest_even, exceptions=ignore>(add<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%94), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%93))), const<f32>(0.25)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             if lt<i32>(read<i32>(%93), div<i32, by_zero=ub, min_by_neg_one=ub>(mul<i32, overflow=ub>(const<i32>(3), const<i32>(1024)), const<i32>(4)))
// DEFAULT-NEXT:                                 write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%93))), float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore>(sub<f64, rounding=nearest_even, exceptions=ignore>(div<f64, rounding=nearest_even, exceptions=ignore>(float_widen<f64, reason=usual_arith>(add<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%95), read<f32, volatile>(%94))), const<f64>(2.0)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(mul<i32, overflow=ub>(const<i32>(1024), const<i32>(8)))), float_widen<f64, reason=usual_arith>(mul<f32, rounding=nearest_even, exceptions=ignore>(const<f32>(16.0), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%93)))))));
// DEFAULT-NEXT:                             else
// DEFAULT-NEXT:                                 write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%93))), add<f32, rounding=nearest_even, exceptions=ignore>(add<f32, rounding=nearest_even, exceptions=ignore>(sub<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%95), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1024))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%93))));
// DEFAULT-NEXT:                     if lt<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%93)))), read<f32, volatile>(%94))
// DEFAULT-NEXT:                         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%93))), read<f32, volatile>(%94));
// DEFAULT-NEXT:                     if gt<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%93)))), read<f32, volatile>(%95))
// DEFAULT-NEXT:                         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%93))), read<f32, volatile>(%95));
// DEFAULT-NEXT:                     if eq<i32>(read<i32>(%93), const<i32>(0))
// DEFAULT-NEXT:                         write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%93))), read<f64, volatile>(%98));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         if lt<i32>(read<i32>(%93), div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(1024), const<i32>(4)))
// DEFAULT-NEXT:                             write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%93))), add<f64, rounding=nearest_even, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%98), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(read<i32>(%93))), float_widen<f64, reason=usual_arith>(const<f32>(0.25))));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             if lt<i32>(read<i32>(%93), div<i32, by_zero=ub, min_by_neg_one=ub>(mul<i32, overflow=ub>(const<i32>(3), const<i32>(1024)), const<i32>(4)))
// DEFAULT-NEXT:                                 write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%93))), add<f64, rounding=nearest_even, exceptions=ignore>(sub<f64, rounding=nearest_even, exceptions=ignore>(div<f64, rounding=nearest_even, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%99), read<f64, volatile>(%98)), const<f64>(2.0)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(mul<i32, overflow=ub>(const<i32>(1024), const<i32>(8)))), float_widen<f64, reason=usual_arith>(mul<f32, rounding=nearest_even, exceptions=ignore>(const<f32>(16.0), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%93))))));
// DEFAULT-NEXT:                             else
// DEFAULT-NEXT:                                 write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%93))), add<f64, rounding=nearest_even, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore>(sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%99), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1024))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(read<i32>(%93))));
// DEFAULT-NEXT:                     if lt<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%93)))), read<f64, volatile>(%98))
// DEFAULT-NEXT:                         write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%93))), read<f64, volatile>(%98));
// DEFAULT-NEXT:                     if gt<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%93)))), read<f64, volatile>(%99))
// DEFAULT-NEXT:                         write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%93))), read<f64, volatile>(%99));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%18);
// DEFAULT-NEXT:         for %258
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%93, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%93), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %455: i32 [synthetic] = read<i32>(%93);
// DEFAULT-NEXT:                 let %456: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%455), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%93, read<i32>(%456));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(1024)>(%3), read<i32>(%93))))), widen<i32, reason=promotion>(float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%93)))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%50);
// DEFAULT-NEXT:         for %259
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%93, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%93), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %457: i32 [synthetic] = read<i32>(%93);
// DEFAULT-NEXT:                 let %458: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%457), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%93, read<i32>(%458));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(1024)>(%3), read<i32>(%93))))), widen<i32, reason=promotion>(float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%93)))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         for %260
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%93, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%93), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %459: i32 [synthetic] = read<i32>(%93);
// DEFAULT-NEXT:                 let %460: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%459), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%93, read<i32>(%460));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %102 r: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(call<i32, signature=fn() -> i32>(%0)));
// DEFAULT-NEXT:                     write<u64>(%102, xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%102), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0)))));
// DEFAULT-NEXT:                     xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%102), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0))));
// DEFAULT-NEXT:                     write<u64>(%102, xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%102), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0)))));
// DEFAULT-NEXT:                     xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%102), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0))));
// DEFAULT-NEXT:                     asm "";
// DEFAULT-NEXT:                     write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%93))), add<f32, rounding=nearest_even, exceptions=ignore>(div<f32, rounding=nearest_even, exceptions=ignore>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%102), const<i32>(59))), const<f32>(32.0)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(truncate<u16, reason=explicit, fits=unknown>(read<u64>(%102)))))));
// DEFAULT-NEXT:                     if lt<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%93)))), read<f32, volatile>(%94))
// DEFAULT-NEXT:                         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%93))), read<f32, volatile>(%94));
// DEFAULT-NEXT:                     if gt<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%93)))), read<f32, volatile>(%95))
// DEFAULT-NEXT:                         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%93))), read<f32, volatile>(%95));
// DEFAULT-NEXT:                     write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%93))), add<f64, rounding=nearest_even, exceptions=ignore>(div<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%102), const<i32>(59))), const<f64>(32.0)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(truncate<u16, reason=explicit, fits=unknown>(read<u64>(%102)))))));
// DEFAULT-NEXT:                     if lt<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%93)))), read<f64, volatile>(%98))
// DEFAULT-NEXT:                         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%93))), float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%98)));
// DEFAULT-NEXT:                     if gt<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%93)))), read<f64, volatile>(%99))
// DEFAULT-NEXT:                         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%93))), float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%99)));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%18);
// DEFAULT-NEXT:         for %261
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%93, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%93), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %461: i32 [synthetic] = read<i32>(%93);
// DEFAULT-NEXT:                 let %462: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%461), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%93, read<i32>(%462));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(1024)>(%3), read<i32>(%93))))), widen<i32, reason=promotion>(float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%93)))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%50);
// DEFAULT-NEXT:         for %262
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%93, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%93), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %463: i32 [synthetic] = read<i32>(%93);
// DEFAULT-NEXT:                 let %464: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%463), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%93, read<i32>(%464));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(1024)>(%3), read<i32>(%93))))), widen<i32, reason=promotion>(float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%93)))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %103 @inttoflttestss() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %104 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %105 vf: volatile f32 [storage=automatic];
// DEFAULT-NEXT:         let %106 vd: volatile f64 [storage=automatic];
// DEFAULT-NEXT:         for %263
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%104, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%104), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %465: i32 [synthetic] = read<i32>(%104);
// DEFAULT-NEXT:                 let %466: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%465), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%104, read<i32>(%466));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     asm "";
// DEFAULT-NEXT:                     if lt<i32>(read<i32>(%104), div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(1024), const<i32>(4)))
// DEFAULT-NEXT:                         write<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(1024)>(%3), read<i32>(%104))), truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(32767)), const<i32>(1)), read<i32>(%104))));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         if lt<i32>(read<i32>(%104), div<i32, by_zero=ub, min_by_neg_one=ub>(mul<i32, overflow=ub>(const<i32>(3), const<i32>(1024)), const<i32>(4)))
// DEFAULT-NEXT:                             write<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(1024)>(%3), read<i32>(%104))), truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(sub<i32, overflow=ub>(div<i32, by_zero=ub, min_by_neg_one=ub>(sub<i32, overflow=ub>(add<i32, overflow=ub>(const<i32>(32767), neg<i32, overflow=ub>(const<i32>(32767))), const<i32>(1)), const<i32>(2)), mul<i32, overflow=ub>(const<i32>(1024), const<i32>(8))), mul<i32, overflow=ub>(const<i32>(16), read<i32>(%104)))));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(1024)>(%3), read<i32>(%104))), truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(add<i32, overflow=ub>(sub<i32, overflow=ub>(const<i32>(32767), const<i32>(1024)), const<i32>(1)), read<i32>(%104))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:         for %264
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%104, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%104), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %467: i32 [synthetic] = read<i32>(%104);
// DEFAULT-NEXT:                 let %468: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%467), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%104, read<i32>(%468));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f32, volatile>(%105, int_to_float<f32, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(1024)>(%3), read<i32>(%104))))));
// DEFAULT-NEXT:                     if ne<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%104)))), read<f32, volatile>(%105))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%48);
// DEFAULT-NEXT:         for %265
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%104, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%104), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %469: i32 [synthetic] = read<i32>(%104);
// DEFAULT-NEXT:                 let %470: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%469), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%104, read<i32>(%470));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f64, volatile>(%106, int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(1024)>(%3), read<i32>(%104))))));
// DEFAULT-NEXT:                     if ne<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%104)))), read<f64, volatile>(%106))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %266
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%104, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%104), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %471: i32 [synthetic] = read<i32>(%104);
// DEFAULT-NEXT:                 let %472: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%471), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%104, read<i32>(%472));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %107 r: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(call<i32, signature=fn() -> i32>(%0)));
// DEFAULT-NEXT:                     write<u64>(%107, xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%107), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0)))));
// DEFAULT-NEXT:                     xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%107), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0))));
// DEFAULT-NEXT:                     write<u64>(%107, xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%107), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0)))));
// DEFAULT-NEXT:                     xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%107), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0))));
// DEFAULT-NEXT:                     asm "";
// DEFAULT-NEXT:                     write<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(1024)>(%3), read<i32>(%104))), reinterpret<i16, reason=assign, fits=unknown>(truncate<u16, reason=assign, fits=unknown>(read<u64>(%107))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:         for %267
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%104, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%104), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %473: i32 [synthetic] = read<i32>(%104);
// DEFAULT-NEXT:                 let %474: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%473), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%104, read<i32>(%474));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f32, volatile>(%105, int_to_float<f32, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(1024)>(%3), read<i32>(%104))))));
// DEFAULT-NEXT:                     if ne<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%104)))), read<f32, volatile>(%105))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%48);
// DEFAULT-NEXT:         for %268
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%104, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%104), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %475: i32 [synthetic] = read<i32>(%104);
// DEFAULT-NEXT:                 let %476: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%475), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%104, read<i32>(%476));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f64, volatile>(%106, int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(1024)>(%3), read<i32>(%104))))));
// DEFAULT-NEXT:                     if ne<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%104)))), read<f64, volatile>(%106))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %108 @flttointtestsi() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %109 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %110 fltmin: volatile f32 [storage=automatic];
// DEFAULT-NEXT:         let %111 fltmax: volatile f32 [storage=automatic];
// DEFAULT-NEXT:         let %112 vf: volatile f32 [storage=automatic];
// DEFAULT-NEXT:         let %113 vf2: volatile f32 [storage=automatic];
// DEFAULT-NEXT:         let %114 dblmin: volatile f64 [storage=automatic];
// DEFAULT-NEXT:         let %115 dblmax: volatile f64 [storage=automatic];
// DEFAULT-NEXT:         let %116 vd: volatile f64 [storage=automatic];
// DEFAULT-NEXT:         let %117 vd2: volatile f64 [storage=automatic];
// DEFAULT-NEXT:         if eq<i32>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(0))
// DEFAULT-NEXT:             write<f32, volatile>(%110, const<f32>(0.0));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<f32, volatile>(%110, sub<f32, rounding=nearest_even, exceptions=ignore>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1))), const<f32>(1.0)));
// DEFAULT-NEXT:                 write<f32, volatile>(%113, sub<f32, rounding=nearest_even, exceptions=ignore>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1))), const<f32>(1.0)));
// DEFAULT-NEXT:                 for %269
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<f32, volatile>(%112, const<f32>(1.0));
// DEFAULT-NEXT:                     condition: {
// DEFAULT-NEXT:                         write<f32, volatile>(%110, add<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%113), read<f32, volatile>(%112)));
// DEFAULT-NEXT:                         yield eq<f32, exceptions=ignore>(read<f32, volatile>(%110), read<f32, volatile>(%113));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         write<f32, volatile>(%112, mul<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%112), const<f32>(2.0)));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         ;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<f32, volatile>(%111, add<f32, rounding=nearest_even, exceptions=ignore>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2147483647)), const<f32>(1.0)));
// DEFAULT-NEXT:         write<f32, volatile>(%113, add<f32, rounding=nearest_even, exceptions=ignore>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2147483647)), const<f32>(1.0)));
// DEFAULT-NEXT:         for %270
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<f32, volatile>(%112, const<f32>(1.0));
// DEFAULT-NEXT:             condition: {
// DEFAULT-NEXT:                 write<f32, volatile>(%111, sub<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%113), read<f32, volatile>(%112)));
// DEFAULT-NEXT:                 yield eq<f32, exceptions=ignore>(read<f32, volatile>(%111), read<f32, volatile>(%113));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 write<f32, volatile>(%112, mul<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%112), const<f32>(2.0)));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:         if eq<i32>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(0))
// DEFAULT-NEXT:             write<f64, volatile>(%114, const<f64>(0.0));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<f64, volatile>(%114, sub<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1))), const<f64>(1.0)));
// DEFAULT-NEXT:                 write<f64, volatile>(%117, sub<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1))), const<f64>(1.0)));
// DEFAULT-NEXT:                 for %271
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<f64, volatile>(%116, const<f64>(1.0));
// DEFAULT-NEXT:                     condition: {
// DEFAULT-NEXT:                         write<f64, volatile>(%114, add<f64, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%117), read<f64, volatile>(%116)));
// DEFAULT-NEXT:                         yield eq<f64, exceptions=ignore>(read<f64, volatile>(%114), read<f64, volatile>(%117));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         write<f64, volatile>(%116, mul<f64, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%116), const<f64>(2.0)));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         ;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<f64, volatile>(%115, add<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2147483647)), const<f64>(1.0)));
// DEFAULT-NEXT:         write<f64, volatile>(%117, add<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2147483647)), const<f64>(1.0)));
// DEFAULT-NEXT:         for %272
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<f64, volatile>(%116, const<f64>(1.0));
// DEFAULT-NEXT:             condition: {
// DEFAULT-NEXT:                 write<f64, volatile>(%115, sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%117), read<f64, volatile>(%116)));
// DEFAULT-NEXT:                 yield eq<f64, exceptions=ignore>(read<f64, volatile>(%115), read<f64, volatile>(%117));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 write<f64, volatile>(%116, mul<f64, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%116), const<f64>(2.0)));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:         for %273
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%109, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%109), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %477: i32 [synthetic] = read<i32>(%109);
// DEFAULT-NEXT:                 let %478: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%477), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%109, read<i32>(%478));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     asm "";
// DEFAULT-NEXT:                     if eq<i32>(read<i32>(%109), const<i32>(0))
// DEFAULT-NEXT:                         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%109))), read<f32, volatile>(%110));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         if lt<i32>(read<i32>(%109), div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(1024), const<i32>(4)))
// DEFAULT-NEXT:                             write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%109))), add<f32, rounding=nearest_even, exceptions=ignore>(add<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%110), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%109))), const<f32>(0.25)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             if lt<i32>(read<i32>(%109), div<i32, by_zero=ub, min_by_neg_one=ub>(mul<i32, overflow=ub>(const<i32>(3), const<i32>(1024)), const<i32>(4)))
// DEFAULT-NEXT:                                 write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%109))), float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore>(sub<f64, rounding=nearest_even, exceptions=ignore>(div<f64, rounding=nearest_even, exceptions=ignore>(float_widen<f64, reason=usual_arith>(add<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%111), read<f32, volatile>(%110))), const<f64>(2.0)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(mul<i32, overflow=ub>(const<i32>(1024), const<i32>(8)))), float_widen<f64, reason=usual_arith>(mul<f32, rounding=nearest_even, exceptions=ignore>(const<f32>(16.0), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%109)))))));
// DEFAULT-NEXT:                             else
// DEFAULT-NEXT:                                 write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%109))), add<f32, rounding=nearest_even, exceptions=ignore>(add<f32, rounding=nearest_even, exceptions=ignore>(sub<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%111), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1024))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%109))));
// DEFAULT-NEXT:                     if lt<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%109)))), read<f32, volatile>(%110))
// DEFAULT-NEXT:                         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%109))), read<f32, volatile>(%110));
// DEFAULT-NEXT:                     if gt<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%109)))), read<f32, volatile>(%111))
// DEFAULT-NEXT:                         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%109))), read<f32, volatile>(%111));
// DEFAULT-NEXT:                     if eq<i32>(read<i32>(%109), const<i32>(0))
// DEFAULT-NEXT:                         write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%109))), read<f64, volatile>(%114));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         if lt<i32>(read<i32>(%109), div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(1024), const<i32>(4)))
// DEFAULT-NEXT:                             write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%109))), add<f64, rounding=nearest_even, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%114), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(read<i32>(%109))), float_widen<f64, reason=usual_arith>(const<f32>(0.25))));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             if lt<i32>(read<i32>(%109), div<i32, by_zero=ub, min_by_neg_one=ub>(mul<i32, overflow=ub>(const<i32>(3), const<i32>(1024)), const<i32>(4)))
// DEFAULT-NEXT:                                 write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%109))), add<f64, rounding=nearest_even, exceptions=ignore>(sub<f64, rounding=nearest_even, exceptions=ignore>(div<f64, rounding=nearest_even, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%115), read<f64, volatile>(%114)), const<f64>(2.0)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(mul<i32, overflow=ub>(const<i32>(1024), const<i32>(8)))), float_widen<f64, reason=usual_arith>(mul<f32, rounding=nearest_even, exceptions=ignore>(const<f32>(16.0), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%109))))));
// DEFAULT-NEXT:                             else
// DEFAULT-NEXT:                                 write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%109))), add<f64, rounding=nearest_even, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore>(sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%115), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1024))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(read<i32>(%109))));
// DEFAULT-NEXT:                     if lt<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%109)))), read<f64, volatile>(%114))
// DEFAULT-NEXT:                         write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%109))), read<f64, volatile>(%114));
// DEFAULT-NEXT:                     if gt<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%109)))), read<f64, volatile>(%115))
// DEFAULT-NEXT:                         write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%109))), read<f64, volatile>(%115));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%22);
// DEFAULT-NEXT:         for %274
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%109, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%109), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %479: i32 [synthetic] = read<i32>(%109);
// DEFAULT-NEXT:                 let %480: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%479), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%109, read<i32>(%480));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1024)>(%4), read<i32>(%109)))), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%109))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%54);
// DEFAULT-NEXT:         for %275
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%109, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%109), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %481: i32 [synthetic] = read<i32>(%109);
// DEFAULT-NEXT:                 let %482: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%481), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%109, read<i32>(%482));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1024)>(%4), read<i32>(%109)))), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%109))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         for %276
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%109, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%109), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %483: i32 [synthetic] = read<i32>(%109);
// DEFAULT-NEXT:                 let %484: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%483), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%109, read<i32>(%484));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %118 r: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(call<i32, signature=fn() -> i32>(%0)));
// DEFAULT-NEXT:                     write<u64>(%118, xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%118), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0)))));
// DEFAULT-NEXT:                     xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%118), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0))));
// DEFAULT-NEXT:                     write<u64>(%118, xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%118), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0)))));
// DEFAULT-NEXT:                     xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%118), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0))));
// DEFAULT-NEXT:                     asm "";
// DEFAULT-NEXT:                     write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%109))), add<f32, rounding=nearest_even, exceptions=ignore>(div<f32, rounding=nearest_even, exceptions=ignore>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%118), const<i32>(59))), const<f32>(32.0)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(read<u64>(%118))))));
// DEFAULT-NEXT:                     if lt<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%109)))), read<f32, volatile>(%110))
// DEFAULT-NEXT:                         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%109))), read<f32, volatile>(%110));
// DEFAULT-NEXT:                     if gt<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%109)))), read<f32, volatile>(%111))
// DEFAULT-NEXT:                         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%109))), read<f32, volatile>(%111));
// DEFAULT-NEXT:                     write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%109))), add<f64, rounding=nearest_even, exceptions=ignore>(div<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%118), const<i32>(59))), const<f64>(32.0)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(read<u64>(%118))))));
// DEFAULT-NEXT:                     if lt<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%109)))), read<f64, volatile>(%114))
// DEFAULT-NEXT:                         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%109))), float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%114)));
// DEFAULT-NEXT:                     if gt<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%109)))), read<f64, volatile>(%115))
// DEFAULT-NEXT:                         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%109))), float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%115)));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%22);
// DEFAULT-NEXT:         for %277
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%109, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%109), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %485: i32 [synthetic] = read<i32>(%109);
// DEFAULT-NEXT:                 let %486: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%485), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%109, read<i32>(%486));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1024)>(%4), read<i32>(%109)))), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%109))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%54);
// DEFAULT-NEXT:         for %278
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%109, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%109), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %487: i32 [synthetic] = read<i32>(%109);
// DEFAULT-NEXT:                 let %488: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%487), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%109, read<i32>(%488));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1024)>(%4), read<i32>(%109)))), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%109))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %119 @inttoflttestsi() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %120 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %121 vf: volatile f32 [storage=automatic];
// DEFAULT-NEXT:         let %122 vd: volatile f64 [storage=automatic];
// DEFAULT-NEXT:         for %279
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%120, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%120), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %489: i32 [synthetic] = read<i32>(%120);
// DEFAULT-NEXT:                 let %490: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%489), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%120, read<i32>(%490));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     asm "";
// DEFAULT-NEXT:                     if lt<i32>(read<i32>(%120), div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(1024), const<i32>(4)))
// DEFAULT-NEXT:                         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1024)>(%4), read<i32>(%120))), add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), read<i32>(%120)));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         if lt<i32>(read<i32>(%120), div<i32, by_zero=ub, min_by_neg_one=ub>(mul<i32, overflow=ub>(const<i32>(3), const<i32>(1024)), const<i32>(4)))
// DEFAULT-NEXT:                             write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1024)>(%4), read<i32>(%120))), add<i32, overflow=ub>(sub<i32, overflow=ub>(div<i32, by_zero=ub, min_by_neg_one=ub>(sub<i32, overflow=ub>(add<i32, overflow=ub>(const<i32>(2147483647), neg<i32, overflow=ub>(const<i32>(2147483647))), const<i32>(1)), const<i32>(2)), mul<i32, overflow=ub>(const<i32>(1024), const<i32>(8))), mul<i32, overflow=ub>(const<i32>(16), read<i32>(%120))));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1024)>(%4), read<i32>(%120))), add<i32, overflow=ub>(add<i32, overflow=ub>(sub<i32, overflow=ub>(const<i32>(2147483647), const<i32>(1024)), const<i32>(1)), read<i32>(%120)));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%20);
// DEFAULT-NEXT:         for %280
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%120, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%120), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %491: i32 [synthetic] = read<i32>(%120);
// DEFAULT-NEXT:                 let %492: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%491), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%120, read<i32>(%492));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f32, volatile>(%121, int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1024)>(%4), read<i32>(%120))))));
// DEFAULT-NEXT:                     if ne<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%120)))), read<f32, volatile>(%121))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%52);
// DEFAULT-NEXT:         for %281
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%120, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%120), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %493: i32 [synthetic] = read<i32>(%120);
// DEFAULT-NEXT:                 let %494: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%493), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%120, read<i32>(%494));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f64, volatile>(%122, int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1024)>(%4), read<i32>(%120))))));
// DEFAULT-NEXT:                     if ne<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%120)))), read<f64, volatile>(%122))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %282
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%120, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%120), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %495: i32 [synthetic] = read<i32>(%120);
// DEFAULT-NEXT:                 let %496: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%495), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%120, read<i32>(%496));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %123 r: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(call<i32, signature=fn() -> i32>(%0)));
// DEFAULT-NEXT:                     write<u64>(%123, xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%123), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0)))));
// DEFAULT-NEXT:                     xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%123), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0))));
// DEFAULT-NEXT:                     write<u64>(%123, xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%123), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0)))));
// DEFAULT-NEXT:                     xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%123), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0))));
// DEFAULT-NEXT:                     asm "";
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1024)>(%4), read<i32>(%120))), reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(read<u64>(%123))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%20);
// DEFAULT-NEXT:         for %283
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%120, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%120), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %497: i32 [synthetic] = read<i32>(%120);
// DEFAULT-NEXT:                 let %498: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%497), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%120, read<i32>(%498));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f32, volatile>(%121, int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1024)>(%4), read<i32>(%120))))));
// DEFAULT-NEXT:                     if ne<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%120)))), read<f32, volatile>(%121))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%52);
// DEFAULT-NEXT:         for %284
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%120, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%120), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %499: i32 [synthetic] = read<i32>(%120);
// DEFAULT-NEXT:                 let %500: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%499), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%120, read<i32>(%500));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f64, volatile>(%122, int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1024)>(%4), read<i32>(%120))))));
// DEFAULT-NEXT:                     if ne<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%120)))), read<f64, volatile>(%122))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %124 @flttointtestsl() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %125 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %126 fltmin: volatile f32 [storage=automatic];
// DEFAULT-NEXT:         let %127 fltmax: volatile f32 [storage=automatic];
// DEFAULT-NEXT:         let %128 vf: volatile f32 [storage=automatic];
// DEFAULT-NEXT:         let %129 vf2: volatile f32 [storage=automatic];
// DEFAULT-NEXT:         let %130 dblmin: volatile f64 [storage=automatic];
// DEFAULT-NEXT:         let %131 dblmax: volatile f64 [storage=automatic];
// DEFAULT-NEXT:         let %132 vd: volatile f64 [storage=automatic];
// DEFAULT-NEXT:         let %133 vd2: volatile f64 [storage=automatic];
// DEFAULT-NEXT:         if eq<i64>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), const<i64>(1)), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             write<f32, volatile>(%126, const<f32>(0.0));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<f32, volatile>(%126, sub<f32, rounding=nearest_even, exceptions=ignore>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), const<i64>(1))), const<f32>(1.0)));
// DEFAULT-NEXT:                 write<f32, volatile>(%129, sub<f32, rounding=nearest_even, exceptions=ignore>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), const<i64>(1))), const<f32>(1.0)));
// DEFAULT-NEXT:                 for %285
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<f32, volatile>(%128, const<f32>(1.0));
// DEFAULT-NEXT:                     condition: {
// DEFAULT-NEXT:                         write<f32, volatile>(%126, add<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%129), read<f32, volatile>(%128)));
// DEFAULT-NEXT:                         yield eq<f32, exceptions=ignore>(read<f32, volatile>(%126), read<f32, volatile>(%129));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         write<f32, volatile>(%128, mul<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%128), const<f32>(2.0)));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         ;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<f32, volatile>(%127, add<f32, rounding=nearest_even, exceptions=ignore>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i64>(9223372036854775807)), const<f32>(1.0)));
// DEFAULT-NEXT:         write<f32, volatile>(%129, add<f32, rounding=nearest_even, exceptions=ignore>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i64>(9223372036854775807)), const<f32>(1.0)));
// DEFAULT-NEXT:         for %286
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<f32, volatile>(%128, const<f32>(1.0));
// DEFAULT-NEXT:             condition: {
// DEFAULT-NEXT:                 write<f32, volatile>(%127, sub<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%129), read<f32, volatile>(%128)));
// DEFAULT-NEXT:                 yield eq<f32, exceptions=ignore>(read<f32, volatile>(%127), read<f32, volatile>(%129));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 write<f32, volatile>(%128, mul<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%128), const<f32>(2.0)));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:         if eq<i64>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), const<i64>(1)), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             write<f64, volatile>(%130, const<f64>(0.0));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<f64, volatile>(%130, sub<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), const<i64>(1))), const<f64>(1.0)));
// DEFAULT-NEXT:                 write<f64, volatile>(%133, sub<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), const<i64>(1))), const<f64>(1.0)));
// DEFAULT-NEXT:                 for %287
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<f64, volatile>(%132, const<f64>(1.0));
// DEFAULT-NEXT:                     condition: {
// DEFAULT-NEXT:                         write<f64, volatile>(%130, add<f64, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%133), read<f64, volatile>(%132)));
// DEFAULT-NEXT:                         yield eq<f64, exceptions=ignore>(read<f64, volatile>(%130), read<f64, volatile>(%133));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         write<f64, volatile>(%132, mul<f64, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%132), const<f64>(2.0)));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         ;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<f64, volatile>(%131, add<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i64>(9223372036854775807)), const<f64>(1.0)));
// DEFAULT-NEXT:         write<f64, volatile>(%133, add<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i64>(9223372036854775807)), const<f64>(1.0)));
// DEFAULT-NEXT:         for %288
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<f64, volatile>(%132, const<f64>(1.0));
// DEFAULT-NEXT:             condition: {
// DEFAULT-NEXT:                 write<f64, volatile>(%131, sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%133), read<f64, volatile>(%132)));
// DEFAULT-NEXT:                 yield eq<f64, exceptions=ignore>(read<f64, volatile>(%131), read<f64, volatile>(%133));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 write<f64, volatile>(%132, mul<f64, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%132), const<f64>(2.0)));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:         for %289
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%125, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%125), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %501: i32 [synthetic] = read<i32>(%125);
// DEFAULT-NEXT:                 let %502: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%501), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%125, read<i32>(%502));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     asm "";
// DEFAULT-NEXT:                     if eq<i32>(read<i32>(%125), const<i32>(0))
// DEFAULT-NEXT:                         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%125))), read<f32, volatile>(%126));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         if lt<i32>(read<i32>(%125), div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(1024), const<i32>(4)))
// DEFAULT-NEXT:                             write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%125))), add<f32, rounding=nearest_even, exceptions=ignore>(add<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%126), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%125))), const<f32>(0.25)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             if lt<i32>(read<i32>(%125), div<i32, by_zero=ub, min_by_neg_one=ub>(mul<i32, overflow=ub>(const<i32>(3), const<i32>(1024)), const<i32>(4)))
// DEFAULT-NEXT:                                 write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%125))), float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore>(sub<f64, rounding=nearest_even, exceptions=ignore>(div<f64, rounding=nearest_even, exceptions=ignore>(float_widen<f64, reason=usual_arith>(add<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%127), read<f32, volatile>(%126))), const<f64>(2.0)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(mul<i32, overflow=ub>(const<i32>(1024), const<i32>(8)))), float_widen<f64, reason=usual_arith>(mul<f32, rounding=nearest_even, exceptions=ignore>(const<f32>(16.0), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%125)))))));
// DEFAULT-NEXT:                             else
// DEFAULT-NEXT:                                 write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%125))), add<f32, rounding=nearest_even, exceptions=ignore>(add<f32, rounding=nearest_even, exceptions=ignore>(sub<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%127), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1024))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%125))));
// DEFAULT-NEXT:                     if lt<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%125)))), read<f32, volatile>(%126))
// DEFAULT-NEXT:                         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%125))), read<f32, volatile>(%126));
// DEFAULT-NEXT:                     if gt<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%125)))), read<f32, volatile>(%127))
// DEFAULT-NEXT:                         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%125))), read<f32, volatile>(%127));
// DEFAULT-NEXT:                     if eq<i32>(read<i32>(%125), const<i32>(0))
// DEFAULT-NEXT:                         write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%125))), read<f64, volatile>(%130));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         if lt<i32>(read<i32>(%125), div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(1024), const<i32>(4)))
// DEFAULT-NEXT:                             write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%125))), add<f64, rounding=nearest_even, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%130), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(read<i32>(%125))), float_widen<f64, reason=usual_arith>(const<f32>(0.25))));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             if lt<i32>(read<i32>(%125), div<i32, by_zero=ub, min_by_neg_one=ub>(mul<i32, overflow=ub>(const<i32>(3), const<i32>(1024)), const<i32>(4)))
// DEFAULT-NEXT:                                 write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%125))), add<f64, rounding=nearest_even, exceptions=ignore>(sub<f64, rounding=nearest_even, exceptions=ignore>(div<f64, rounding=nearest_even, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%131), read<f64, volatile>(%130)), const<f64>(2.0)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(mul<i32, overflow=ub>(const<i32>(1024), const<i32>(8)))), float_widen<f64, reason=usual_arith>(mul<f32, rounding=nearest_even, exceptions=ignore>(const<f32>(16.0), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%125))))));
// DEFAULT-NEXT:                             else
// DEFAULT-NEXT:                                 write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%125))), add<f64, rounding=nearest_even, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore>(sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%131), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1024))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(read<i32>(%125))));
// DEFAULT-NEXT:                     if lt<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%125)))), read<f64, volatile>(%130))
// DEFAULT-NEXT:                         write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%125))), read<f64, volatile>(%130));
// DEFAULT-NEXT:                     if gt<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%125)))), read<f64, volatile>(%131))
// DEFAULT-NEXT:                         write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%125))), read<f64, volatile>(%131));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%26);
// DEFAULT-NEXT:         for %290
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%125, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%125), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %503: i32 [synthetic] = read<i32>(%125);
// DEFAULT-NEXT:                 let %504: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%503), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%125, read<i32>(%504));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i64>(read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(1024)>(%5), read<i32>(%125)))), float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%125))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%58);
// DEFAULT-NEXT:         for %291
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%125, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%125), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %505: i32 [synthetic] = read<i32>(%125);
// DEFAULT-NEXT:                 let %506: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%505), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%125, read<i32>(%506));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i64>(read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(1024)>(%5), read<i32>(%125)))), float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%125))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         for %292
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%125, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%125), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %507: i32 [synthetic] = read<i32>(%125);
// DEFAULT-NEXT:                 let %508: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%507), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%125, read<i32>(%508));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %134 r: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(call<i32, signature=fn() -> i32>(%0)));
// DEFAULT-NEXT:                     write<u64>(%134, xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%134), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0)))));
// DEFAULT-NEXT:                     xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%134), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0))));
// DEFAULT-NEXT:                     write<u64>(%134, xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%134), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0)))));
// DEFAULT-NEXT:                     xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%134), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0))));
// DEFAULT-NEXT:                     asm "";
// DEFAULT-NEXT:                     write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%125))), add<f32, rounding=nearest_even, exceptions=ignore>(div<f32, rounding=nearest_even, exceptions=ignore>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%134), const<i32>(59))), const<f32>(32.0)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(reinterpret<i64, reason=explicit, fits=unknown>(read<u64>(%134)))));
// DEFAULT-NEXT:                     if lt<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%125)))), read<f32, volatile>(%126))
// DEFAULT-NEXT:                         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%125))), read<f32, volatile>(%126));
// DEFAULT-NEXT:                     if gt<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%125)))), read<f32, volatile>(%127))
// DEFAULT-NEXT:                         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%125))), read<f32, volatile>(%127));
// DEFAULT-NEXT:                     write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%125))), add<f64, rounding=nearest_even, exceptions=ignore>(div<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%134), const<i32>(59))), const<f64>(32.0)), int_to_float<f64, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(reinterpret<i64, reason=explicit, fits=unknown>(read<u64>(%134)))));
// DEFAULT-NEXT:                     if lt<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%125)))), read<f64, volatile>(%130))
// DEFAULT-NEXT:                         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%125))), float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%130)));
// DEFAULT-NEXT:                     if gt<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%125)))), read<f64, volatile>(%131))
// DEFAULT-NEXT:                         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%125))), float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%131)));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%26);
// DEFAULT-NEXT:         for %293
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%125, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%125), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %509: i32 [synthetic] = read<i32>(%125);
// DEFAULT-NEXT:                 let %510: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%509), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%125, read<i32>(%510));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i64>(read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(1024)>(%5), read<i32>(%125)))), float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%125))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%58);
// DEFAULT-NEXT:         for %294
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%125, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%125), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %511: i32 [synthetic] = read<i32>(%125);
// DEFAULT-NEXT:                 let %512: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%511), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%125, read<i32>(%512));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i64>(read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(1024)>(%5), read<i32>(%125)))), float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%125))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %135 @inttoflttestsl() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %136 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %137 vf: volatile f32 [storage=automatic];
// DEFAULT-NEXT:         let %138 vd: volatile f64 [storage=automatic];
// DEFAULT-NEXT:         for %295
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%136, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%136), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %513: i32 [synthetic] = read<i32>(%136);
// DEFAULT-NEXT:                 let %514: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%513), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%136, read<i32>(%514));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     asm "";
// DEFAULT-NEXT:                     if lt<i32>(read<i32>(%136), div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(1024), const<i32>(4)))
// DEFAULT-NEXT:                         write<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(1024)>(%5), read<i32>(%136))), add<i64, overflow=ub>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), const<i64>(1)), widen<i64, reason=usual_arith>(read<i32>(%136))));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         if lt<i32>(read<i32>(%136), div<i32, by_zero=ub, min_by_neg_one=ub>(mul<i32, overflow=ub>(const<i32>(3), const<i32>(1024)), const<i32>(4)))
// DEFAULT-NEXT:                             write<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(1024)>(%5), read<i32>(%136))), add<i64, overflow=ub>(sub<i64, overflow=ub>(div<i64, by_zero=ub, min_by_neg_one=ub>(sub<i64, overflow=ub>(add<i64, overflow=ub>(const<i64>(9223372036854775807), neg<i64, overflow=ub>(const<i64>(9223372036854775807))), const<i64>(1)), widen<i64, reason=usual_arith>(const<i32>(2))), widen<i64, reason=usual_arith>(mul<i32, overflow=ub>(const<i32>(1024), const<i32>(8)))), widen<i64, reason=usual_arith>(mul<i32, overflow=ub>(const<i32>(16), read<i32>(%136)))));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(1024)>(%5), read<i32>(%136))), add<i64, overflow=ub>(add<i64, overflow=ub>(sub<i64, overflow=ub>(const<i64>(9223372036854775807), widen<i64, reason=usual_arith>(const<i32>(1024))), widen<i64, reason=usual_arith>(const<i32>(1))), widen<i64, reason=usual_arith>(read<i32>(%136))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%24);
// DEFAULT-NEXT:         for %296
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%136, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%136), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %515: i32 [synthetic] = read<i32>(%136);
// DEFAULT-NEXT:                 let %516: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%515), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%136, read<i32>(%516));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f32, volatile>(%137, int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(1024)>(%5), read<i32>(%136))))));
// DEFAULT-NEXT:                     if ne<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%136)))), read<f32, volatile>(%137))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%56);
// DEFAULT-NEXT:         for %297
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%136, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%136), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %517: i32 [synthetic] = read<i32>(%136);
// DEFAULT-NEXT:                 let %518: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%517), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%136, read<i32>(%518));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f64, volatile>(%138, int_to_float<f64, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(1024)>(%5), read<i32>(%136))))));
// DEFAULT-NEXT:                     if ne<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%136)))), read<f64, volatile>(%138))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %298
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%136, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%136), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %519: i32 [synthetic] = read<i32>(%136);
// DEFAULT-NEXT:                 let %520: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%519), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%136, read<i32>(%520));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %139 r: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(call<i32, signature=fn() -> i32>(%0)));
// DEFAULT-NEXT:                     write<u64>(%139, xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%139), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0)))));
// DEFAULT-NEXT:                     xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%139), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0))));
// DEFAULT-NEXT:                     write<u64>(%139, xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%139), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0)))));
// DEFAULT-NEXT:                     xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%139), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0))));
// DEFAULT-NEXT:                     asm "";
// DEFAULT-NEXT:                     write<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(1024)>(%5), read<i32>(%136))), reinterpret<i64, reason=assign, fits=unknown>(read<u64>(%139)));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%24);
// DEFAULT-NEXT:         for %299
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%136, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%136), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %521: i32 [synthetic] = read<i32>(%136);
// DEFAULT-NEXT:                 let %522: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%521), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%136, read<i32>(%522));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f32, volatile>(%137, int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(1024)>(%5), read<i32>(%136))))));
// DEFAULT-NEXT:                     if ne<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%136)))), read<f32, volatile>(%137))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%56);
// DEFAULT-NEXT:         for %300
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%136, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%136), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %523: i32 [synthetic] = read<i32>(%136);
// DEFAULT-NEXT:                 let %524: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%523), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%136, read<i32>(%524));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f64, volatile>(%138, int_to_float<f64, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(1024)>(%5), read<i32>(%136))))));
// DEFAULT-NEXT:                     if ne<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%136)))), read<f64, volatile>(%138))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %140 @flttointtestuc() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %141 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %142 fltmin: volatile f32 [storage=automatic];
// DEFAULT-NEXT:         let %143 fltmax: volatile f32 [storage=automatic];
// DEFAULT-NEXT:         let %144 vf: volatile f32 [storage=automatic];
// DEFAULT-NEXT:         let %145 vf2: volatile f32 [storage=automatic];
// DEFAULT-NEXT:         let %146 dblmin: volatile f64 [storage=automatic];
// DEFAULT-NEXT:         let %147 dblmax: volatile f64 [storage=automatic];
// DEFAULT-NEXT:         let %148 vd: volatile f64 [storage=automatic];
// DEFAULT-NEXT:         let %149 vd2: volatile f64 [storage=automatic];
// DEFAULT-NEXT:         if eq<i32>(const<i32>(0), const<i32>(0))
// DEFAULT-NEXT:             write<f32, volatile>(%142, const<f32>(0.0));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<f32, volatile>(%142, sub<f32, rounding=nearest_even, exceptions=ignore>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), const<f32>(1.0)));
// DEFAULT-NEXT:                 write<f32, volatile>(%145, sub<f32, rounding=nearest_even, exceptions=ignore>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), const<f32>(1.0)));
// DEFAULT-NEXT:                 for %301
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<f32, volatile>(%144, const<f32>(1.0));
// DEFAULT-NEXT:                     condition: {
// DEFAULT-NEXT:                         write<f32, volatile>(%142, add<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%145), read<f32, volatile>(%144)));
// DEFAULT-NEXT:                         yield eq<f32, exceptions=ignore>(read<f32, volatile>(%142), read<f32, volatile>(%145));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         write<f32, volatile>(%144, mul<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%144), const<f32>(2.0)));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         ;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<f32, volatile>(%143, add<f32, rounding=nearest_even, exceptions=ignore>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(const<u32>(2), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(127))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))), const<f32>(1.0)));
// DEFAULT-NEXT:         write<f32, volatile>(%145, add<f32, rounding=nearest_even, exceptions=ignore>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(const<u32>(2), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(127))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))), const<f32>(1.0)));
// DEFAULT-NEXT:         for %302
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<f32, volatile>(%144, const<f32>(1.0));
// DEFAULT-NEXT:             condition: {
// DEFAULT-NEXT:                 write<f32, volatile>(%143, sub<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%145), read<f32, volatile>(%144)));
// DEFAULT-NEXT:                 yield eq<f32, exceptions=ignore>(read<f32, volatile>(%143), read<f32, volatile>(%145));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 write<f32, volatile>(%144, mul<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%144), const<f32>(2.0)));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:         if eq<i32>(const<i32>(0), const<i32>(0))
// DEFAULT-NEXT:             write<f64, volatile>(%146, const<f64>(0.0));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<f64, volatile>(%146, sub<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), const<f64>(1.0)));
// DEFAULT-NEXT:                 write<f64, volatile>(%149, sub<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), const<f64>(1.0)));
// DEFAULT-NEXT:                 for %303
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<f64, volatile>(%148, const<f64>(1.0));
// DEFAULT-NEXT:                     condition: {
// DEFAULT-NEXT:                         write<f64, volatile>(%146, add<f64, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%149), read<f64, volatile>(%148)));
// DEFAULT-NEXT:                         yield eq<f64, exceptions=ignore>(read<f64, volatile>(%146), read<f64, volatile>(%149));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         write<f64, volatile>(%148, mul<f64, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%148), const<f64>(2.0)));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         ;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<f64, volatile>(%147, add<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(const<u32>(2), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(127))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))), const<f64>(1.0)));
// DEFAULT-NEXT:         write<f64, volatile>(%149, add<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(const<u32>(2), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(127))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))), const<f64>(1.0)));
// DEFAULT-NEXT:         for %304
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<f64, volatile>(%148, const<f64>(1.0));
// DEFAULT-NEXT:             condition: {
// DEFAULT-NEXT:                 write<f64, volatile>(%147, sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%149), read<f64, volatile>(%148)));
// DEFAULT-NEXT:                 yield eq<f64, exceptions=ignore>(read<f64, volatile>(%147), read<f64, volatile>(%149));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 write<f64, volatile>(%148, mul<f64, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%148), const<f64>(2.0)));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:         for %305
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%141, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%141), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %525: i32 [synthetic] = read<i32>(%141);
// DEFAULT-NEXT:                 let %526: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%525), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%141, read<i32>(%526));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     asm "";
// DEFAULT-NEXT:                     if eq<i32>(read<i32>(%141), const<i32>(0))
// DEFAULT-NEXT:                         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%141))), read<f32, volatile>(%142));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         if lt<i32>(read<i32>(%141), div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(1024), const<i32>(4)))
// DEFAULT-NEXT:                             write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%141))), add<f32, rounding=nearest_even, exceptions=ignore>(add<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%142), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%141))), const<f32>(0.25)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             if lt<i32>(read<i32>(%141), div<i32, by_zero=ub, min_by_neg_one=ub>(mul<i32, overflow=ub>(const<i32>(3), const<i32>(1024)), const<i32>(4)))
// DEFAULT-NEXT:                                 write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%141))), float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore>(sub<f64, rounding=nearest_even, exceptions=ignore>(div<f64, rounding=nearest_even, exceptions=ignore>(float_widen<f64, reason=usual_arith>(add<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%143), read<f32, volatile>(%142))), const<f64>(2.0)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(mul<i32, overflow=ub>(const<i32>(1024), const<i32>(8)))), float_widen<f64, reason=usual_arith>(mul<f32, rounding=nearest_even, exceptions=ignore>(const<f32>(16.0), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%141)))))));
// DEFAULT-NEXT:                             else
// DEFAULT-NEXT:                                 write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%141))), add<f32, rounding=nearest_even, exceptions=ignore>(add<f32, rounding=nearest_even, exceptions=ignore>(sub<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%143), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1024))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%141))));
// DEFAULT-NEXT:                     if lt<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%141)))), read<f32, volatile>(%142))
// DEFAULT-NEXT:                         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%141))), read<f32, volatile>(%142));
// DEFAULT-NEXT:                     if gt<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%141)))), read<f32, volatile>(%143))
// DEFAULT-NEXT:                         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%141))), read<f32, volatile>(%143));
// DEFAULT-NEXT:                     if eq<i32>(read<i32>(%141), const<i32>(0))
// DEFAULT-NEXT:                         write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%141))), read<f64, volatile>(%146));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         if lt<i32>(read<i32>(%141), div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(1024), const<i32>(4)))
// DEFAULT-NEXT:                             write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%141))), add<f64, rounding=nearest_even, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%146), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(read<i32>(%141))), float_widen<f64, reason=usual_arith>(const<f32>(0.25))));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             if lt<i32>(read<i32>(%141), div<i32, by_zero=ub, min_by_neg_one=ub>(mul<i32, overflow=ub>(const<i32>(3), const<i32>(1024)), const<i32>(4)))
// DEFAULT-NEXT:                                 write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%141))), add<f64, rounding=nearest_even, exceptions=ignore>(sub<f64, rounding=nearest_even, exceptions=ignore>(div<f64, rounding=nearest_even, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%147), read<f64, volatile>(%146)), const<f64>(2.0)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(mul<i32, overflow=ub>(const<i32>(1024), const<i32>(8)))), float_widen<f64, reason=usual_arith>(mul<f32, rounding=nearest_even, exceptions=ignore>(const<f32>(16.0), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%141))))));
// DEFAULT-NEXT:                             else
// DEFAULT-NEXT:                                 write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%141))), add<f64, rounding=nearest_even, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore>(sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%147), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1024))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(read<i32>(%141))));
// DEFAULT-NEXT:                     if lt<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%141)))), read<f64, volatile>(%146))
// DEFAULT-NEXT:                         write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%141))), read<f64, volatile>(%146));
// DEFAULT-NEXT:                     if gt<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%141)))), read<f64, volatile>(%147))
// DEFAULT-NEXT:                         write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%141))), read<f64, volatile>(%147));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%30);
// DEFAULT-NEXT:         for %306
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%141, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%141), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %527: i32 [synthetic] = read<i32>(%141);
// DEFAULT-NEXT:                 let %528: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%527), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%141, read<i32>(%528));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(1024)>(%6), read<i32>(%141)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%141))))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%62);
// DEFAULT-NEXT:         for %307
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%141, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%141), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %529: i32 [synthetic] = read<i32>(%141);
// DEFAULT-NEXT:                 let %530: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%529), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%141, read<i32>(%530));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(1024)>(%6), read<i32>(%141)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%141))))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         for %308
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%141, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%141), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %531: i32 [synthetic] = read<i32>(%141);
// DEFAULT-NEXT:                 let %532: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%531), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%141, read<i32>(%532));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %150 r: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(call<i32, signature=fn() -> i32>(%0)));
// DEFAULT-NEXT:                     write<u64>(%150, xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%150), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0)))));
// DEFAULT-NEXT:                     xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%150), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0))));
// DEFAULT-NEXT:                     write<u64>(%150, xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%150), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0)))));
// DEFAULT-NEXT:                     xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%150), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0))));
// DEFAULT-NEXT:                     asm "";
// DEFAULT-NEXT:                     write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%141))), add<f32, rounding=nearest_even, exceptions=ignore>(div<f32, rounding=nearest_even, exceptions=ignore>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%150), const<i32>(59))), const<f32>(32.0)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(read<u64>(%150)))))));
// DEFAULT-NEXT:                     if lt<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%141)))), read<f32, volatile>(%142))
// DEFAULT-NEXT:                         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%141))), read<f32, volatile>(%142));
// DEFAULT-NEXT:                     if gt<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%141)))), read<f32, volatile>(%143))
// DEFAULT-NEXT:                         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%141))), read<f32, volatile>(%143));
// DEFAULT-NEXT:                     write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%141))), add<f64, rounding=nearest_even, exceptions=ignore>(div<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%150), const<i32>(59))), const<f64>(32.0)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(read<u64>(%150)))))));
// DEFAULT-NEXT:                     if lt<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%141)))), read<f64, volatile>(%146))
// DEFAULT-NEXT:                         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%141))), float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%146)));
// DEFAULT-NEXT:                     if gt<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%141)))), read<f64, volatile>(%147))
// DEFAULT-NEXT:                         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%141))), float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%147)));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%30);
// DEFAULT-NEXT:         for %309
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%141, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%141), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %533: i32 [synthetic] = read<i32>(%141);
// DEFAULT-NEXT:                 let %534: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%533), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%141, read<i32>(%534));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(1024)>(%6), read<i32>(%141)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%141))))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%62);
// DEFAULT-NEXT:         for %310
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%141, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%141), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %535: i32 [synthetic] = read<i32>(%141);
// DEFAULT-NEXT:                 let %536: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%535), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%141, read<i32>(%536));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(1024)>(%6), read<i32>(%141)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%141))))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %151 @inttoflttestuc() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %152 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %153 vf: volatile f32 [storage=automatic];
// DEFAULT-NEXT:         let %154 vd: volatile f64 [storage=automatic];
// DEFAULT-NEXT:         for %311
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%152, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%152), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %537: i32 [synthetic] = read<i32>(%152);
// DEFAULT-NEXT:                 let %538: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%537), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%152, read<i32>(%538));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     asm "";
// DEFAULT-NEXT:                     if lt<i32>(read<i32>(%152), div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(1024), const<i32>(4)))
// DEFAULT-NEXT:                         write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(1024)>(%6), read<i32>(%152))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(const<i32>(0), read<i32>(%152)))));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         if lt<i32>(read<i32>(%152), div<i32, by_zero=ub, min_by_neg_one=ub>(mul<i32, overflow=ub>(const<i32>(3), const<i32>(1024)), const<i32>(4)))
// DEFAULT-NEXT:                             write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(1024)>(%6), read<i32>(%152))), truncate<u8, reason=assign, fits=unknown>(add<u32, overflow=wrap>(sub<u32, overflow=wrap>(div<u32, by_zero=ub>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(const<u32>(2), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(127))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))), reinterpret<u32, reason=usual_arith, fits=unknown>(mul<i32, overflow=ub>(const<i32>(1024), const<i32>(8)))), reinterpret<u32, reason=usual_arith, fits=unknown>(mul<i32, overflow=ub>(const<i32>(16), read<i32>(%152))))));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(1024)>(%6), read<i32>(%152))), truncate<u8, reason=assign, fits=unknown>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(sub<u32, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(const<u32>(2), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(127))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1024))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%152)))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         for %312
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%152, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%152), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %539: i32 [synthetic] = read<i32>(%152);
// DEFAULT-NEXT:                 let %540: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%539), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%152, read<i32>(%540));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f32, volatile>(%153, int_to_float<f32, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(1024)>(%6), read<i32>(%152))))));
// DEFAULT-NEXT:                     if ne<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%152)))), read<f32, volatile>(%153))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%60);
// DEFAULT-NEXT:         for %313
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%152, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%152), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %541: i32 [synthetic] = read<i32>(%152);
// DEFAULT-NEXT:                 let %542: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%541), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%152, read<i32>(%542));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f64, volatile>(%154, int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(1024)>(%6), read<i32>(%152))))));
// DEFAULT-NEXT:                     if ne<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%152)))), read<f64, volatile>(%154))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %314
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%152, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%152), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %543: i32 [synthetic] = read<i32>(%152);
// DEFAULT-NEXT:                 let %544: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%543), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%152, read<i32>(%544));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %155 r: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(call<i32, signature=fn() -> i32>(%0)));
// DEFAULT-NEXT:                     write<u64>(%155, xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%155), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0)))));
// DEFAULT-NEXT:                     xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%155), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0))));
// DEFAULT-NEXT:                     write<u64>(%155, xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%155), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0)))));
// DEFAULT-NEXT:                     xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%155), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0))));
// DEFAULT-NEXT:                     asm "";
// DEFAULT-NEXT:                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(1024)>(%6), read<i32>(%152))), truncate<u8, reason=assign, fits=unknown>(read<u64>(%155)));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         for %315
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%152, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%152), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %545: i32 [synthetic] = read<i32>(%152);
// DEFAULT-NEXT:                 let %546: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%545), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%152, read<i32>(%546));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f32, volatile>(%153, int_to_float<f32, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(1024)>(%6), read<i32>(%152))))));
// DEFAULT-NEXT:                     if ne<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%152)))), read<f32, volatile>(%153))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%60);
// DEFAULT-NEXT:         for %316
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%152, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%152), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %547: i32 [synthetic] = read<i32>(%152);
// DEFAULT-NEXT:                 let %548: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%547), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%152, read<i32>(%548));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f64, volatile>(%154, int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(1024)>(%6), read<i32>(%152))))));
// DEFAULT-NEXT:                     if ne<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%152)))), read<f64, volatile>(%154))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %156 @flttointtestus() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %157 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %158 fltmin: volatile f32 [storage=automatic];
// DEFAULT-NEXT:         let %159 fltmax: volatile f32 [storage=automatic];
// DEFAULT-NEXT:         let %160 vf: volatile f32 [storage=automatic];
// DEFAULT-NEXT:         let %161 vf2: volatile f32 [storage=automatic];
// DEFAULT-NEXT:         let %162 dblmin: volatile f64 [storage=automatic];
// DEFAULT-NEXT:         let %163 dblmax: volatile f64 [storage=automatic];
// DEFAULT-NEXT:         let %164 vd: volatile f64 [storage=automatic];
// DEFAULT-NEXT:         let %165 vd2: volatile f64 [storage=automatic];
// DEFAULT-NEXT:         if eq<i32>(const<i32>(0), const<i32>(0))
// DEFAULT-NEXT:             write<f32, volatile>(%158, const<f32>(0.0));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<f32, volatile>(%158, sub<f32, rounding=nearest_even, exceptions=ignore>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), const<f32>(1.0)));
// DEFAULT-NEXT:                 write<f32, volatile>(%161, sub<f32, rounding=nearest_even, exceptions=ignore>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), const<f32>(1.0)));
// DEFAULT-NEXT:                 for %317
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<f32, volatile>(%160, const<f32>(1.0));
// DEFAULT-NEXT:                     condition: {
// DEFAULT-NEXT:                         write<f32, volatile>(%158, add<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%161), read<f32, volatile>(%160)));
// DEFAULT-NEXT:                         yield eq<f32, exceptions=ignore>(read<f32, volatile>(%158), read<f32, volatile>(%161));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         write<f32, volatile>(%160, mul<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%160), const<f32>(2.0)));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         ;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<f32, volatile>(%159, add<f32, rounding=nearest_even, exceptions=ignore>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(const<u32>(2), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(32767))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))), const<f32>(1.0)));
// DEFAULT-NEXT:         write<f32, volatile>(%161, add<f32, rounding=nearest_even, exceptions=ignore>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(const<u32>(2), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(32767))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))), const<f32>(1.0)));
// DEFAULT-NEXT:         for %318
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<f32, volatile>(%160, const<f32>(1.0));
// DEFAULT-NEXT:             condition: {
// DEFAULT-NEXT:                 write<f32, volatile>(%159, sub<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%161), read<f32, volatile>(%160)));
// DEFAULT-NEXT:                 yield eq<f32, exceptions=ignore>(read<f32, volatile>(%159), read<f32, volatile>(%161));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 write<f32, volatile>(%160, mul<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%160), const<f32>(2.0)));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:         if eq<i32>(const<i32>(0), const<i32>(0))
// DEFAULT-NEXT:             write<f64, volatile>(%162, const<f64>(0.0));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<f64, volatile>(%162, sub<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), const<f64>(1.0)));
// DEFAULT-NEXT:                 write<f64, volatile>(%165, sub<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), const<f64>(1.0)));
// DEFAULT-NEXT:                 for %319
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<f64, volatile>(%164, const<f64>(1.0));
// DEFAULT-NEXT:                     condition: {
// DEFAULT-NEXT:                         write<f64, volatile>(%162, add<f64, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%165), read<f64, volatile>(%164)));
// DEFAULT-NEXT:                         yield eq<f64, exceptions=ignore>(read<f64, volatile>(%162), read<f64, volatile>(%165));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         write<f64, volatile>(%164, mul<f64, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%164), const<f64>(2.0)));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         ;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<f64, volatile>(%163, add<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(const<u32>(2), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(32767))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))), const<f64>(1.0)));
// DEFAULT-NEXT:         write<f64, volatile>(%165, add<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(const<u32>(2), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(32767))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))), const<f64>(1.0)));
// DEFAULT-NEXT:         for %320
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<f64, volatile>(%164, const<f64>(1.0));
// DEFAULT-NEXT:             condition: {
// DEFAULT-NEXT:                 write<f64, volatile>(%163, sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%165), read<f64, volatile>(%164)));
// DEFAULT-NEXT:                 yield eq<f64, exceptions=ignore>(read<f64, volatile>(%163), read<f64, volatile>(%165));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 write<f64, volatile>(%164, mul<f64, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%164), const<f64>(2.0)));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:         for %321
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%157, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%157), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %549: i32 [synthetic] = read<i32>(%157);
// DEFAULT-NEXT:                 let %550: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%549), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%157, read<i32>(%550));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     asm "";
// DEFAULT-NEXT:                     if eq<i32>(read<i32>(%157), const<i32>(0))
// DEFAULT-NEXT:                         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%157))), read<f32, volatile>(%158));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         if lt<i32>(read<i32>(%157), div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(1024), const<i32>(4)))
// DEFAULT-NEXT:                             write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%157))), add<f32, rounding=nearest_even, exceptions=ignore>(add<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%158), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%157))), const<f32>(0.25)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             if lt<i32>(read<i32>(%157), div<i32, by_zero=ub, min_by_neg_one=ub>(mul<i32, overflow=ub>(const<i32>(3), const<i32>(1024)), const<i32>(4)))
// DEFAULT-NEXT:                                 write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%157))), float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore>(sub<f64, rounding=nearest_even, exceptions=ignore>(div<f64, rounding=nearest_even, exceptions=ignore>(float_widen<f64, reason=usual_arith>(add<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%159), read<f32, volatile>(%158))), const<f64>(2.0)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(mul<i32, overflow=ub>(const<i32>(1024), const<i32>(8)))), float_widen<f64, reason=usual_arith>(mul<f32, rounding=nearest_even, exceptions=ignore>(const<f32>(16.0), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%157)))))));
// DEFAULT-NEXT:                             else
// DEFAULT-NEXT:                                 write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%157))), add<f32, rounding=nearest_even, exceptions=ignore>(add<f32, rounding=nearest_even, exceptions=ignore>(sub<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%159), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1024))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%157))));
// DEFAULT-NEXT:                     if lt<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%157)))), read<f32, volatile>(%158))
// DEFAULT-NEXT:                         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%157))), read<f32, volatile>(%158));
// DEFAULT-NEXT:                     if gt<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%157)))), read<f32, volatile>(%159))
// DEFAULT-NEXT:                         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%157))), read<f32, volatile>(%159));
// DEFAULT-NEXT:                     if eq<i32>(read<i32>(%157), const<i32>(0))
// DEFAULT-NEXT:                         write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%157))), read<f64, volatile>(%162));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         if lt<i32>(read<i32>(%157), div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(1024), const<i32>(4)))
// DEFAULT-NEXT:                             write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%157))), add<f64, rounding=nearest_even, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%162), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(read<i32>(%157))), float_widen<f64, reason=usual_arith>(const<f32>(0.25))));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             if lt<i32>(read<i32>(%157), div<i32, by_zero=ub, min_by_neg_one=ub>(mul<i32, overflow=ub>(const<i32>(3), const<i32>(1024)), const<i32>(4)))
// DEFAULT-NEXT:                                 write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%157))), add<f64, rounding=nearest_even, exceptions=ignore>(sub<f64, rounding=nearest_even, exceptions=ignore>(div<f64, rounding=nearest_even, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%163), read<f64, volatile>(%162)), const<f64>(2.0)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(mul<i32, overflow=ub>(const<i32>(1024), const<i32>(8)))), float_widen<f64, reason=usual_arith>(mul<f32, rounding=nearest_even, exceptions=ignore>(const<f32>(16.0), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%157))))));
// DEFAULT-NEXT:                             else
// DEFAULT-NEXT:                                 write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%157))), add<f64, rounding=nearest_even, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore>(sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%163), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1024))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(read<i32>(%157))));
// DEFAULT-NEXT:                     if lt<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%157)))), read<f64, volatile>(%162))
// DEFAULT-NEXT:                         write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%157))), read<f64, volatile>(%162));
// DEFAULT-NEXT:                     if gt<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%157)))), read<f64, volatile>(%163))
// DEFAULT-NEXT:                         write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%157))), read<f64, volatile>(%163));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%34);
// DEFAULT-NEXT:         for %322
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%157, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%157), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %551: i32 [synthetic] = read<i32>(%157);
// DEFAULT-NEXT:                 let %552: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%551), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%157, read<i32>(%552));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<u16>, length=Some(1024)>(%7), read<i32>(%157)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%157))))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%66);
// DEFAULT-NEXT:         for %323
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%157, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%157), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %553: i32 [synthetic] = read<i32>(%157);
// DEFAULT-NEXT:                 let %554: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%553), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%157, read<i32>(%554));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<u16>, length=Some(1024)>(%7), read<i32>(%157)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%157))))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         for %324
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%157, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%157), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %555: i32 [synthetic] = read<i32>(%157);
// DEFAULT-NEXT:                 let %556: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%555), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%157, read<i32>(%556));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %166 r: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(call<i32, signature=fn() -> i32>(%0)));
// DEFAULT-NEXT:                     write<u64>(%166, xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%166), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0)))));
// DEFAULT-NEXT:                     xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%166), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0))));
// DEFAULT-NEXT:                     write<u64>(%166, xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%166), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0)))));
// DEFAULT-NEXT:                     xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%166), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0))));
// DEFAULT-NEXT:                     asm "";
// DEFAULT-NEXT:                     write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%157))), add<f32, rounding=nearest_even, exceptions=ignore>(div<f32, rounding=nearest_even, exceptions=ignore>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%166), const<i32>(59))), const<f32>(32.0)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u16, reason=explicit, fits=unknown>(read<u64>(%166)))))));
// DEFAULT-NEXT:                     if lt<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%157)))), read<f32, volatile>(%158))
// DEFAULT-NEXT:                         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%157))), read<f32, volatile>(%158));
// DEFAULT-NEXT:                     if gt<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%157)))), read<f32, volatile>(%159))
// DEFAULT-NEXT:                         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%157))), read<f32, volatile>(%159));
// DEFAULT-NEXT:                     write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%157))), add<f64, rounding=nearest_even, exceptions=ignore>(div<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%166), const<i32>(59))), const<f64>(32.0)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u16, reason=explicit, fits=unknown>(read<u64>(%166)))))));
// DEFAULT-NEXT:                     if lt<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%157)))), read<f64, volatile>(%162))
// DEFAULT-NEXT:                         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%157))), float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%162)));
// DEFAULT-NEXT:                     if gt<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%157)))), read<f64, volatile>(%163))
// DEFAULT-NEXT:                         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%157))), float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%163)));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%34);
// DEFAULT-NEXT:         for %325
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%157, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%157), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %557: i32 [synthetic] = read<i32>(%157);
// DEFAULT-NEXT:                 let %558: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%557), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%157, read<i32>(%558));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<u16>, length=Some(1024)>(%7), read<i32>(%157)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%157))))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%66);
// DEFAULT-NEXT:         for %326
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%157, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%157), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %559: i32 [synthetic] = read<i32>(%157);
// DEFAULT-NEXT:                 let %560: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%559), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%157, read<i32>(%560));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<u16>, length=Some(1024)>(%7), read<i32>(%157)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%157))))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %167 @inttoflttestus() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %168 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %169 vf: volatile f32 [storage=automatic];
// DEFAULT-NEXT:         let %170 vd: volatile f64 [storage=automatic];
// DEFAULT-NEXT:         for %327
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%168, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%168), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %561: i32 [synthetic] = read<i32>(%168);
// DEFAULT-NEXT:                 let %562: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%561), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%168, read<i32>(%562));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     asm "";
// DEFAULT-NEXT:                     if lt<i32>(read<i32>(%168), div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(1024), const<i32>(4)))
// DEFAULT-NEXT:                         write<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<u16>, length=Some(1024)>(%7), read<i32>(%168))), reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(const<i32>(0), read<i32>(%168)))));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         if lt<i32>(read<i32>(%168), div<i32, by_zero=ub, min_by_neg_one=ub>(mul<i32, overflow=ub>(const<i32>(3), const<i32>(1024)), const<i32>(4)))
// DEFAULT-NEXT:                             write<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<u16>, length=Some(1024)>(%7), read<i32>(%168))), truncate<u16, reason=assign, fits=unknown>(add<u32, overflow=wrap>(sub<u32, overflow=wrap>(div<u32, by_zero=ub>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(const<u32>(2), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(32767))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))), reinterpret<u32, reason=usual_arith, fits=unknown>(mul<i32, overflow=ub>(const<i32>(1024), const<i32>(8)))), reinterpret<u32, reason=usual_arith, fits=unknown>(mul<i32, overflow=ub>(const<i32>(16), read<i32>(%168))))));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<u16>, length=Some(1024)>(%7), read<i32>(%168))), truncate<u16, reason=assign, fits=unknown>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(sub<u32, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(const<u32>(2), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(32767))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1024))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%168)))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%32);
// DEFAULT-NEXT:         for %328
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%168, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%168), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %563: i32 [synthetic] = read<i32>(%168);
// DEFAULT-NEXT:                 let %564: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%563), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%168, read<i32>(%564));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f32, volatile>(%169, int_to_float<f32, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(read<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<u16>, length=Some(1024)>(%7), read<i32>(%168))))));
// DEFAULT-NEXT:                     if ne<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%168)))), read<f32, volatile>(%169))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%64);
// DEFAULT-NEXT:         for %329
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%168, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%168), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %565: i32 [synthetic] = read<i32>(%168);
// DEFAULT-NEXT:                 let %566: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%565), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%168, read<i32>(%566));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f64, volatile>(%170, int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(read<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<u16>, length=Some(1024)>(%7), read<i32>(%168))))));
// DEFAULT-NEXT:                     if ne<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%168)))), read<f64, volatile>(%170))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %330
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%168, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%168), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %567: i32 [synthetic] = read<i32>(%168);
// DEFAULT-NEXT:                 let %568: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%567), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%168, read<i32>(%568));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %171 r: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(call<i32, signature=fn() -> i32>(%0)));
// DEFAULT-NEXT:                     write<u64>(%171, xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%171), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0)))));
// DEFAULT-NEXT:                     xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%171), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0))));
// DEFAULT-NEXT:                     write<u64>(%171, xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%171), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0)))));
// DEFAULT-NEXT:                     xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%171), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0))));
// DEFAULT-NEXT:                     asm "";
// DEFAULT-NEXT:                     write<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<u16>, length=Some(1024)>(%7), read<i32>(%168))), truncate<u16, reason=assign, fits=unknown>(read<u64>(%171)));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%32);
// DEFAULT-NEXT:         for %331
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%168, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%168), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %569: i32 [synthetic] = read<i32>(%168);
// DEFAULT-NEXT:                 let %570: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%569), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%168, read<i32>(%570));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f32, volatile>(%169, int_to_float<f32, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(read<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<u16>, length=Some(1024)>(%7), read<i32>(%168))))));
// DEFAULT-NEXT:                     if ne<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%168)))), read<f32, volatile>(%169))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%64);
// DEFAULT-NEXT:         for %332
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%168, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%168), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %571: i32 [synthetic] = read<i32>(%168);
// DEFAULT-NEXT:                 let %572: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%571), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%168, read<i32>(%572));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f64, volatile>(%170, int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(read<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<u16>, length=Some(1024)>(%7), read<i32>(%168))))));
// DEFAULT-NEXT:                     if ne<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%168)))), read<f64, volatile>(%170))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %172 @flttointtestui() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %173 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %174 fltmin: volatile f32 [storage=automatic];
// DEFAULT-NEXT:         let %175 fltmax: volatile f32 [storage=automatic];
// DEFAULT-NEXT:         let %176 vf: volatile f32 [storage=automatic];
// DEFAULT-NEXT:         let %177 vf2: volatile f32 [storage=automatic];
// DEFAULT-NEXT:         let %178 dblmin: volatile f64 [storage=automatic];
// DEFAULT-NEXT:         let %179 dblmax: volatile f64 [storage=automatic];
// DEFAULT-NEXT:         let %180 vd: volatile f64 [storage=automatic];
// DEFAULT-NEXT:         let %181 vd2: volatile f64 [storage=automatic];
// DEFAULT-NEXT:         if eq<i32>(const<i32>(0), const<i32>(0))
// DEFAULT-NEXT:             write<f32, volatile>(%174, const<f32>(0.0));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<f32, volatile>(%174, sub<f32, rounding=nearest_even, exceptions=ignore>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), const<f32>(1.0)));
// DEFAULT-NEXT:                 write<f32, volatile>(%177, sub<f32, rounding=nearest_even, exceptions=ignore>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), const<f32>(1.0)));
// DEFAULT-NEXT:                 for %333
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<f32, volatile>(%176, const<f32>(1.0));
// DEFAULT-NEXT:                     condition: {
// DEFAULT-NEXT:                         write<f32, volatile>(%174, add<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%177), read<f32, volatile>(%176)));
// DEFAULT-NEXT:                         yield eq<f32, exceptions=ignore>(read<f32, volatile>(%174), read<f32, volatile>(%177));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         write<f32, volatile>(%176, mul<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%176), const<f32>(2.0)));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         ;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<f32, volatile>(%175, add<f32, rounding=nearest_even, exceptions=ignore>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(const<u32>(2), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))), const<f32>(1.0)));
// DEFAULT-NEXT:         write<f32, volatile>(%177, add<f32, rounding=nearest_even, exceptions=ignore>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(const<u32>(2), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))), const<f32>(1.0)));
// DEFAULT-NEXT:         for %334
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<f32, volatile>(%176, const<f32>(1.0));
// DEFAULT-NEXT:             condition: {
// DEFAULT-NEXT:                 write<f32, volatile>(%175, sub<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%177), read<f32, volatile>(%176)));
// DEFAULT-NEXT:                 yield eq<f32, exceptions=ignore>(read<f32, volatile>(%175), read<f32, volatile>(%177));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 write<f32, volatile>(%176, mul<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%176), const<f32>(2.0)));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:         if eq<i32>(const<i32>(0), const<i32>(0))
// DEFAULT-NEXT:             write<f64, volatile>(%178, const<f64>(0.0));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<f64, volatile>(%178, sub<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), const<f64>(1.0)));
// DEFAULT-NEXT:                 write<f64, volatile>(%181, sub<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), const<f64>(1.0)));
// DEFAULT-NEXT:                 for %335
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<f64, volatile>(%180, const<f64>(1.0));
// DEFAULT-NEXT:                     condition: {
// DEFAULT-NEXT:                         write<f64, volatile>(%178, add<f64, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%181), read<f64, volatile>(%180)));
// DEFAULT-NEXT:                         yield eq<f64, exceptions=ignore>(read<f64, volatile>(%178), read<f64, volatile>(%181));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         write<f64, volatile>(%180, mul<f64, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%180), const<f64>(2.0)));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         ;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<f64, volatile>(%179, add<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(const<u32>(2), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))), const<f64>(1.0)));
// DEFAULT-NEXT:         write<f64, volatile>(%181, add<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(const<u32>(2), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))), const<f64>(1.0)));
// DEFAULT-NEXT:         for %336
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<f64, volatile>(%180, const<f64>(1.0));
// DEFAULT-NEXT:             condition: {
// DEFAULT-NEXT:                 write<f64, volatile>(%179, sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%181), read<f64, volatile>(%180)));
// DEFAULT-NEXT:                 yield eq<f64, exceptions=ignore>(read<f64, volatile>(%179), read<f64, volatile>(%181));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 write<f64, volatile>(%180, mul<f64, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%180), const<f64>(2.0)));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:         for %337
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%173, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%173), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %573: i32 [synthetic] = read<i32>(%173);
// DEFAULT-NEXT:                 let %574: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%573), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%173, read<i32>(%574));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     asm "";
// DEFAULT-NEXT:                     if eq<i32>(read<i32>(%173), const<i32>(0))
// DEFAULT-NEXT:                         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%173))), read<f32, volatile>(%174));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         if lt<i32>(read<i32>(%173), div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(1024), const<i32>(4)))
// DEFAULT-NEXT:                             write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%173))), add<f32, rounding=nearest_even, exceptions=ignore>(add<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%174), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%173))), const<f32>(0.25)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             if lt<i32>(read<i32>(%173), div<i32, by_zero=ub, min_by_neg_one=ub>(mul<i32, overflow=ub>(const<i32>(3), const<i32>(1024)), const<i32>(4)))
// DEFAULT-NEXT:                                 write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%173))), float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore>(sub<f64, rounding=nearest_even, exceptions=ignore>(div<f64, rounding=nearest_even, exceptions=ignore>(float_widen<f64, reason=usual_arith>(add<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%175), read<f32, volatile>(%174))), const<f64>(2.0)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(mul<i32, overflow=ub>(const<i32>(1024), const<i32>(8)))), float_widen<f64, reason=usual_arith>(mul<f32, rounding=nearest_even, exceptions=ignore>(const<f32>(16.0), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%173)))))));
// DEFAULT-NEXT:                             else
// DEFAULT-NEXT:                                 write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%173))), add<f32, rounding=nearest_even, exceptions=ignore>(add<f32, rounding=nearest_even, exceptions=ignore>(sub<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%175), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1024))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%173))));
// DEFAULT-NEXT:                     if lt<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%173)))), read<f32, volatile>(%174))
// DEFAULT-NEXT:                         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%173))), read<f32, volatile>(%174));
// DEFAULT-NEXT:                     if gt<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%173)))), read<f32, volatile>(%175))
// DEFAULT-NEXT:                         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%173))), read<f32, volatile>(%175));
// DEFAULT-NEXT:                     if eq<i32>(read<i32>(%173), const<i32>(0))
// DEFAULT-NEXT:                         write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%173))), read<f64, volatile>(%178));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         if lt<i32>(read<i32>(%173), div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(1024), const<i32>(4)))
// DEFAULT-NEXT:                             write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%173))), add<f64, rounding=nearest_even, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%178), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(read<i32>(%173))), float_widen<f64, reason=usual_arith>(const<f32>(0.25))));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             if lt<i32>(read<i32>(%173), div<i32, by_zero=ub, min_by_neg_one=ub>(mul<i32, overflow=ub>(const<i32>(3), const<i32>(1024)), const<i32>(4)))
// DEFAULT-NEXT:                                 write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%173))), add<f64, rounding=nearest_even, exceptions=ignore>(sub<f64, rounding=nearest_even, exceptions=ignore>(div<f64, rounding=nearest_even, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%179), read<f64, volatile>(%178)), const<f64>(2.0)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(mul<i32, overflow=ub>(const<i32>(1024), const<i32>(8)))), float_widen<f64, reason=usual_arith>(mul<f32, rounding=nearest_even, exceptions=ignore>(const<f32>(16.0), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%173))))));
// DEFAULT-NEXT:                             else
// DEFAULT-NEXT:                                 write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%173))), add<f64, rounding=nearest_even, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore>(sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%179), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1024))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(read<i32>(%173))));
// DEFAULT-NEXT:                     if lt<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%173)))), read<f64, volatile>(%178))
// DEFAULT-NEXT:                         write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%173))), read<f64, volatile>(%178));
// DEFAULT-NEXT:                     if gt<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%173)))), read<f64, volatile>(%179))
// DEFAULT-NEXT:                         write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%173))), read<f64, volatile>(%179));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:         for %338
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%173, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%173), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %575: i32 [synthetic] = read<i32>(%173);
// DEFAULT-NEXT:                 let %576: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%575), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%173, read<i32>(%576));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(1024)>(%8), read<i32>(%173)))), float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%173))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%70);
// DEFAULT-NEXT:         for %339
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%173, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%173), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %577: i32 [synthetic] = read<i32>(%173);
// DEFAULT-NEXT:                 let %578: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%577), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%173, read<i32>(%578));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(1024)>(%8), read<i32>(%173)))), float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%173))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         for %340
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%173, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%173), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %579: i32 [synthetic] = read<i32>(%173);
// DEFAULT-NEXT:                 let %580: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%579), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%173, read<i32>(%580));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %182 r: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(call<i32, signature=fn() -> i32>(%0)));
// DEFAULT-NEXT:                     write<u64>(%182, xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%182), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0)))));
// DEFAULT-NEXT:                     xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%182), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0))));
// DEFAULT-NEXT:                     write<u64>(%182, xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%182), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0)))));
// DEFAULT-NEXT:                     xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%182), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0))));
// DEFAULT-NEXT:                     asm "";
// DEFAULT-NEXT:                     write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%173))), add<f32, rounding=nearest_even, exceptions=ignore>(div<f32, rounding=nearest_even, exceptions=ignore>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%182), const<i32>(59))), const<f32>(32.0)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(truncate<u32, reason=explicit, fits=unknown>(read<u64>(%182)))));
// DEFAULT-NEXT:                     if lt<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%173)))), read<f32, volatile>(%174))
// DEFAULT-NEXT:                         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%173))), read<f32, volatile>(%174));
// DEFAULT-NEXT:                     if gt<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%173)))), read<f32, volatile>(%175))
// DEFAULT-NEXT:                         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%173))), read<f32, volatile>(%175));
// DEFAULT-NEXT:                     write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%173))), add<f64, rounding=nearest_even, exceptions=ignore>(div<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%182), const<i32>(59))), const<f64>(32.0)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(truncate<u32, reason=explicit, fits=unknown>(read<u64>(%182)))));
// DEFAULT-NEXT:                     if lt<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%173)))), read<f64, volatile>(%178))
// DEFAULT-NEXT:                         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%173))), float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%178)));
// DEFAULT-NEXT:                     if gt<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%173)))), read<f64, volatile>(%179))
// DEFAULT-NEXT:                         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%173))), float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%179)));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:         for %341
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%173, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%173), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %581: i32 [synthetic] = read<i32>(%173);
// DEFAULT-NEXT:                 let %582: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%581), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%173, read<i32>(%582));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(1024)>(%8), read<i32>(%173)))), float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%173))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%70);
// DEFAULT-NEXT:         for %342
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%173, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%173), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %583: i32 [synthetic] = read<i32>(%173);
// DEFAULT-NEXT:                 let %584: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%583), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%173, read<i32>(%584));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(1024)>(%8), read<i32>(%173)))), float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%173))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %183 @inttoflttestui() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %184 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %185 vf: volatile f32 [storage=automatic];
// DEFAULT-NEXT:         let %186 vd: volatile f64 [storage=automatic];
// DEFAULT-NEXT:         for %343
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%184, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%184), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %585: i32 [synthetic] = read<i32>(%184);
// DEFAULT-NEXT:                 let %586: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%585), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%184, read<i32>(%586));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     asm "";
// DEFAULT-NEXT:                     if lt<i32>(read<i32>(%184), div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(1024), const<i32>(4)))
// DEFAULT-NEXT:                         write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(1024)>(%8), read<i32>(%184))), reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(const<i32>(0), read<i32>(%184))));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         if lt<i32>(read<i32>(%184), div<i32, by_zero=ub, min_by_neg_one=ub>(mul<i32, overflow=ub>(const<i32>(3), const<i32>(1024)), const<i32>(4)))
// DEFAULT-NEXT:                             write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(1024)>(%8), read<i32>(%184))), add<u32, overflow=wrap>(sub<u32, overflow=wrap>(div<u32, by_zero=ub>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(const<u32>(2), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))), reinterpret<u32, reason=usual_arith, fits=unknown>(mul<i32, overflow=ub>(const<i32>(1024), const<i32>(8)))), reinterpret<u32, reason=usual_arith, fits=unknown>(mul<i32, overflow=ub>(const<i32>(16), read<i32>(%184)))));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(1024)>(%8), read<i32>(%184))), add<u32, overflow=wrap>(add<u32, overflow=wrap>(sub<u32, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(const<u32>(2), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1024))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%184))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%36);
// DEFAULT-NEXT:         for %344
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%184, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%184), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %587: i32 [synthetic] = read<i32>(%184);
// DEFAULT-NEXT:                 let %588: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%587), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%184, read<i32>(%588));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f32, volatile>(%185, int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(1024)>(%8), read<i32>(%184))))));
// DEFAULT-NEXT:                     if ne<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%184)))), read<f32, volatile>(%185))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%68);
// DEFAULT-NEXT:         for %345
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%184, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%184), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %589: i32 [synthetic] = read<i32>(%184);
// DEFAULT-NEXT:                 let %590: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%589), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%184, read<i32>(%590));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f64, volatile>(%186, int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(1024)>(%8), read<i32>(%184))))));
// DEFAULT-NEXT:                     if ne<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%184)))), read<f64, volatile>(%186))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %346
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%184, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%184), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %591: i32 [synthetic] = read<i32>(%184);
// DEFAULT-NEXT:                 let %592: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%591), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%184, read<i32>(%592));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %187 r: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(call<i32, signature=fn() -> i32>(%0)));
// DEFAULT-NEXT:                     write<u64>(%187, xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%187), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0)))));
// DEFAULT-NEXT:                     xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%187), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0))));
// DEFAULT-NEXT:                     write<u64>(%187, xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%187), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0)))));
// DEFAULT-NEXT:                     xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%187), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0))));
// DEFAULT-NEXT:                     asm "";
// DEFAULT-NEXT:                     write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(1024)>(%8), read<i32>(%184))), truncate<u32, reason=assign, fits=unknown>(read<u64>(%187)));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%36);
// DEFAULT-NEXT:         for %347
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%184, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%184), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %593: i32 [synthetic] = read<i32>(%184);
// DEFAULT-NEXT:                 let %594: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%593), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%184, read<i32>(%594));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f32, volatile>(%185, int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(1024)>(%8), read<i32>(%184))))));
// DEFAULT-NEXT:                     if ne<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%184)))), read<f32, volatile>(%185))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%68);
// DEFAULT-NEXT:         for %348
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%184, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%184), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %595: i32 [synthetic] = read<i32>(%184);
// DEFAULT-NEXT:                 let %596: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%595), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%184, read<i32>(%596));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f64, volatile>(%186, int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(1024)>(%8), read<i32>(%184))))));
// DEFAULT-NEXT:                     if ne<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%184)))), read<f64, volatile>(%186))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %188 @flttointtestul() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %189 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %190 fltmin: volatile f32 [storage=automatic];
// DEFAULT-NEXT:         let %191 fltmax: volatile f32 [storage=automatic];
// DEFAULT-NEXT:         let %192 vf: volatile f32 [storage=automatic];
// DEFAULT-NEXT:         let %193 vf2: volatile f32 [storage=automatic];
// DEFAULT-NEXT:         let %194 dblmin: volatile f64 [storage=automatic];
// DEFAULT-NEXT:         let %195 dblmax: volatile f64 [storage=automatic];
// DEFAULT-NEXT:         let %196 vd: volatile f64 [storage=automatic];
// DEFAULT-NEXT:         let %197 vd2: volatile f64 [storage=automatic];
// DEFAULT-NEXT:         if eq<i32>(const<i32>(0), const<i32>(0))
// DEFAULT-NEXT:             write<f32, volatile>(%190, const<f32>(0.0));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<f32, volatile>(%190, sub<f32, rounding=nearest_even, exceptions=ignore>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), const<f32>(1.0)));
// DEFAULT-NEXT:                 write<f32, volatile>(%193, sub<f32, rounding=nearest_even, exceptions=ignore>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), const<f32>(1.0)));
// DEFAULT-NEXT:                 for %349
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<f32, volatile>(%192, const<f32>(1.0));
// DEFAULT-NEXT:                     condition: {
// DEFAULT-NEXT:                         write<f32, volatile>(%190, add<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%193), read<f32, volatile>(%192)));
// DEFAULT-NEXT:                         yield eq<f32, exceptions=ignore>(read<f32, volatile>(%190), read<f32, volatile>(%193));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         write<f32, volatile>(%192, mul<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%192), const<f32>(2.0)));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         ;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<f32, volatile>(%191, add<f32, rounding=nearest_even, exceptions=ignore>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(add<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), const<f32>(1.0)));
// DEFAULT-NEXT:         write<f32, volatile>(%193, add<f32, rounding=nearest_even, exceptions=ignore>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(add<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), const<f32>(1.0)));
// DEFAULT-NEXT:         for %350
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<f32, volatile>(%192, const<f32>(1.0));
// DEFAULT-NEXT:             condition: {
// DEFAULT-NEXT:                 write<f32, volatile>(%191, sub<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%193), read<f32, volatile>(%192)));
// DEFAULT-NEXT:                 yield eq<f32, exceptions=ignore>(read<f32, volatile>(%191), read<f32, volatile>(%193));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 write<f32, volatile>(%192, mul<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%192), const<f32>(2.0)));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:         if eq<i32>(const<i32>(0), const<i32>(0))
// DEFAULT-NEXT:             write<f64, volatile>(%194, const<f64>(0.0));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<f64, volatile>(%194, sub<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), const<f64>(1.0)));
// DEFAULT-NEXT:                 write<f64, volatile>(%197, sub<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), const<f64>(1.0)));
// DEFAULT-NEXT:                 for %351
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<f64, volatile>(%196, const<f64>(1.0));
// DEFAULT-NEXT:                     condition: {
// DEFAULT-NEXT:                         write<f64, volatile>(%194, add<f64, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%197), read<f64, volatile>(%196)));
// DEFAULT-NEXT:                         yield eq<f64, exceptions=ignore>(read<f64, volatile>(%194), read<f64, volatile>(%197));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         write<f64, volatile>(%196, mul<f64, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%196), const<f64>(2.0)));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         ;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<f64, volatile>(%195, add<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(add<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), const<f64>(1.0)));
// DEFAULT-NEXT:         write<f64, volatile>(%197, add<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(add<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), const<f64>(1.0)));
// DEFAULT-NEXT:         for %352
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<f64, volatile>(%196, const<f64>(1.0));
// DEFAULT-NEXT:             condition: {
// DEFAULT-NEXT:                 write<f64, volatile>(%195, sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%197), read<f64, volatile>(%196)));
// DEFAULT-NEXT:                 yield eq<f64, exceptions=ignore>(read<f64, volatile>(%195), read<f64, volatile>(%197));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 write<f64, volatile>(%196, mul<f64, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%196), const<f64>(2.0)));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:         for %353
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%189, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%189), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %597: i32 [synthetic] = read<i32>(%189);
// DEFAULT-NEXT:                 let %598: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%597), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%189, read<i32>(%598));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     asm "";
// DEFAULT-NEXT:                     if eq<i32>(read<i32>(%189), const<i32>(0))
// DEFAULT-NEXT:                         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%189))), read<f32, volatile>(%190));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         if lt<i32>(read<i32>(%189), div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(1024), const<i32>(4)))
// DEFAULT-NEXT:                             write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%189))), add<f32, rounding=nearest_even, exceptions=ignore>(add<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%190), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%189))), const<f32>(0.25)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             if lt<i32>(read<i32>(%189), div<i32, by_zero=ub, min_by_neg_one=ub>(mul<i32, overflow=ub>(const<i32>(3), const<i32>(1024)), const<i32>(4)))
// DEFAULT-NEXT:                                 write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%189))), float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore>(sub<f64, rounding=nearest_even, exceptions=ignore>(div<f64, rounding=nearest_even, exceptions=ignore>(float_widen<f64, reason=usual_arith>(add<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%191), read<f32, volatile>(%190))), const<f64>(2.0)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(mul<i32, overflow=ub>(const<i32>(1024), const<i32>(8)))), float_widen<f64, reason=usual_arith>(mul<f32, rounding=nearest_even, exceptions=ignore>(const<f32>(16.0), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%189)))))));
// DEFAULT-NEXT:                             else
// DEFAULT-NEXT:                                 write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%189))), add<f32, rounding=nearest_even, exceptions=ignore>(add<f32, rounding=nearest_even, exceptions=ignore>(sub<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%191), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1024))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%189))));
// DEFAULT-NEXT:                     if lt<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%189)))), read<f32, volatile>(%190))
// DEFAULT-NEXT:                         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%189))), read<f32, volatile>(%190));
// DEFAULT-NEXT:                     if gt<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%189)))), read<f32, volatile>(%191))
// DEFAULT-NEXT:                         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%189))), read<f32, volatile>(%191));
// DEFAULT-NEXT:                     if eq<i32>(read<i32>(%189), const<i32>(0))
// DEFAULT-NEXT:                         write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%189))), read<f64, volatile>(%194));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         if lt<i32>(read<i32>(%189), div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(1024), const<i32>(4)))
// DEFAULT-NEXT:                             write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%189))), add<f64, rounding=nearest_even, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%194), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(read<i32>(%189))), float_widen<f64, reason=usual_arith>(const<f32>(0.25))));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             if lt<i32>(read<i32>(%189), div<i32, by_zero=ub, min_by_neg_one=ub>(mul<i32, overflow=ub>(const<i32>(3), const<i32>(1024)), const<i32>(4)))
// DEFAULT-NEXT:                                 write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%189))), add<f64, rounding=nearest_even, exceptions=ignore>(sub<f64, rounding=nearest_even, exceptions=ignore>(div<f64, rounding=nearest_even, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%195), read<f64, volatile>(%194)), const<f64>(2.0)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(mul<i32, overflow=ub>(const<i32>(1024), const<i32>(8)))), float_widen<f64, reason=usual_arith>(mul<f32, rounding=nearest_even, exceptions=ignore>(const<f32>(16.0), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%189))))));
// DEFAULT-NEXT:                             else
// DEFAULT-NEXT:                                 write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%189))), add<f64, rounding=nearest_even, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore>(sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%195), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1024))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(read<i32>(%189))));
// DEFAULT-NEXT:                     if lt<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%189)))), read<f64, volatile>(%194))
// DEFAULT-NEXT:                         write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%189))), read<f64, volatile>(%194));
// DEFAULT-NEXT:                     if gt<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%189)))), read<f64, volatile>(%195))
// DEFAULT-NEXT:                         write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%189))), read<f64, volatile>(%195));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%42);
// DEFAULT-NEXT:         for %354
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%189, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%189), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %599: i32 [synthetic] = read<i32>(%189);
// DEFAULT-NEXT:                 let %600: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%599), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%189, read<i32>(%600));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(1024)>(%9), read<i32>(%189)))), float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%189))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%74);
// DEFAULT-NEXT:         for %355
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%189, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%189), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %601: i32 [synthetic] = read<i32>(%189);
// DEFAULT-NEXT:                 let %602: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%601), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%189, read<i32>(%602));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(1024)>(%9), read<i32>(%189)))), float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%189))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         for %356
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%189, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%189), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %603: i32 [synthetic] = read<i32>(%189);
// DEFAULT-NEXT:                 let %604: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%603), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%189, read<i32>(%604));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %198 r: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(call<i32, signature=fn() -> i32>(%0)));
// DEFAULT-NEXT:                     write<u64>(%198, xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%198), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0)))));
// DEFAULT-NEXT:                     xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%198), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0))));
// DEFAULT-NEXT:                     write<u64>(%198, xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%198), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0)))));
// DEFAULT-NEXT:                     xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%198), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0))));
// DEFAULT-NEXT:                     asm "";
// DEFAULT-NEXT:                     write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%189))), add<f32, rounding=nearest_even, exceptions=ignore>(div<f32, rounding=nearest_even, exceptions=ignore>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%198), const<i32>(59))), const<f32>(32.0)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(read<u64>(%198))));
// DEFAULT-NEXT:                     if lt<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%189)))), read<f32, volatile>(%190))
// DEFAULT-NEXT:                         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%189))), read<f32, volatile>(%190));
// DEFAULT-NEXT:                     if gt<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%189)))), read<f32, volatile>(%191))
// DEFAULT-NEXT:                         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%189))), read<f32, volatile>(%191));
// DEFAULT-NEXT:                     write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%189))), add<f64, rounding=nearest_even, exceptions=ignore>(div<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%198), const<i32>(59))), const<f64>(32.0)), int_to_float<f64, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(read<u64>(%198))));
// DEFAULT-NEXT:                     if lt<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%189)))), read<f64, volatile>(%194))
// DEFAULT-NEXT:                         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%189))), float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%194)));
// DEFAULT-NEXT:                     if gt<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%189)))), read<f64, volatile>(%195))
// DEFAULT-NEXT:                         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%189))), float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(read<f64, volatile>(%195)));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%42);
// DEFAULT-NEXT:         for %357
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%189, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%189), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %605: i32 [synthetic] = read<i32>(%189);
// DEFAULT-NEXT:                 let %606: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%605), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%189, read<i32>(%606));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(1024)>(%9), read<i32>(%189)))), float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%189))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%74);
// DEFAULT-NEXT:         for %358
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%189, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%189), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %607: i32 [synthetic] = read<i32>(%189);
// DEFAULT-NEXT:                 let %608: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%607), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%189, read<i32>(%608));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(1024)>(%9), read<i32>(%189)))), float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%189))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %199 @inttoflttestul() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %200 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %201 vf: volatile f32 [storage=automatic];
// DEFAULT-NEXT:         let %202 vd: volatile f64 [storage=automatic];
// DEFAULT-NEXT:         for %359
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%200, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%200), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %609: i32 [synthetic] = read<i32>(%200);
// DEFAULT-NEXT:                 let %610: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%609), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%200, read<i32>(%610));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     asm "";
// DEFAULT-NEXT:                     if lt<i32>(read<i32>(%200), div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(1024), const<i32>(4)))
// DEFAULT-NEXT:                         write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(1024)>(%9), read<i32>(%200))), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(add<i32, overflow=ub>(const<i32>(0), read<i32>(%200)))));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         if lt<i32>(read<i32>(%200), div<i32, by_zero=ub, min_by_neg_one=ub>(mul<i32, overflow=ub>(const<i32>(3), const<i32>(1024)), const<i32>(4)))
// DEFAULT-NEXT:                             write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(1024)>(%9), read<i32>(%200))), add<u64, overflow=wrap>(sub<u64, overflow=wrap>(div<u64, by_zero=ub>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(mul<i32, overflow=ub>(const<i32>(1024), const<i32>(8))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(mul<i32, overflow=ub>(const<i32>(16), read<i32>(%200))))));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(1024)>(%9), read<i32>(%200))), add<u64, overflow=wrap>(add<u64, overflow=wrap>(sub<u64, overflow=wrap>(add<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1024)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%200)))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%40);
// DEFAULT-NEXT:         for %360
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%200, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%200), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %611: i32 [synthetic] = read<i32>(%200);
// DEFAULT-NEXT:                 let %612: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%611), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%200, read<i32>(%612));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f32, volatile>(%201, int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(1024)>(%9), read<i32>(%200))))));
// DEFAULT-NEXT:                     if ne<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%200)))), read<f32, volatile>(%201))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%72);
// DEFAULT-NEXT:         for %361
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%200, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%200), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %613: i32 [synthetic] = read<i32>(%200);
// DEFAULT-NEXT:                 let %614: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%613), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%200, read<i32>(%614));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f64, volatile>(%202, int_to_float<f64, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(1024)>(%9), read<i32>(%200))))));
// DEFAULT-NEXT:                     if ne<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%200)))), read<f64, volatile>(%202))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %362
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%200, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%200), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %615: i32 [synthetic] = read<i32>(%200);
// DEFAULT-NEXT:                 let %616: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%615), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%200, read<i32>(%616));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %203 r: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(call<i32, signature=fn() -> i32>(%0)));
// DEFAULT-NEXT:                     write<u64>(%203, xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%203), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0)))));
// DEFAULT-NEXT:                     xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%203), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0))));
// DEFAULT-NEXT:                     write<u64>(%203, xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%203), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0)))));
// DEFAULT-NEXT:                     xor<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%203), const<i32>(21)), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn() -> i32>(%0))));
// DEFAULT-NEXT:                     asm "";
// DEFAULT-NEXT:                     write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(1024)>(%9), read<i32>(%200))), read<u64>(%203));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%40);
// DEFAULT-NEXT:         for %363
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%200, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%200), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %617: i32 [synthetic] = read<i32>(%200);
// DEFAULT-NEXT:                 let %618: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%617), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%200, read<i32>(%618));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f32, volatile>(%201, int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(1024)>(%9), read<i32>(%200))))));
// DEFAULT-NEXT:                     if ne<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(1024)>(%10), read<i32>(%200)))), read<f32, volatile>(%201))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%72);
// DEFAULT-NEXT:         for %364
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%200, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%200), const<i32>(1024))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %619: i32 [synthetic] = read<i32>(%200);
// DEFAULT-NEXT:                 let %620: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%619), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%200, read<i32>(%620));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f64, volatile>(%202, int_to_float<f64, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(1024)>(%9), read<i32>(%200))))));
// DEFAULT-NEXT:                     if ne<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(1024)>(%11), read<i32>(%200)))), read<f64, volatile>(%202))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %204 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%76);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%92);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%108);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%124);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%140);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%156);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%172);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%188);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%87);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%103);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%119);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%135);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%151);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%167);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%183);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%199);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
