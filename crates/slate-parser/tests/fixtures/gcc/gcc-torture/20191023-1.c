/* PR tree-optimization/92131 */
/* Testcase by Armin Rigo <arigo@tunes.org> */

long b, c, d, e, f, i;
char g, h, j, k;
int *aa;

static void error(void) __attribute__((noipa));
static void error(void) { __builtin_abort(); }

static void see_me_here(void) __attribute__((noipa));
static void see_me_here(void) {}

static void aaa(void) __attribute__((noipa));
static void aaa(void) {}

static void a(void) __attribute__((noipa));
static void a(void) {
  long am, ao;
  if (aa == 0) {
    aaa();
    if (j)
      goto ay;
  }
  return;
ay:
  aaa();
  if (k) {
    aaa();
    goto az;
  }
  return;
az:
  if (i)
    if (g)
      if (h)
        if (e)
          goto bd;
  return;
bd:
  am = 0;
  while (am < e) {
    switch (c) {
    case 8:
      goto bh;
    case 4:
      return;
    }
  bh:
    if (am >= 0)
      b = -am;
    ao = am + b;
    f  = ao & 7;
    if (f == 0)
      see_me_here();
    if (ao >= 0)
      am++;
    else
      error();
  }
}

int main(void) {
  j++;
  k++;
  i++;
  g++;
  h++;
  e = 1;
  a();
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
// DEFAULT-NEXT:     global %0 b: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 c: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 d: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 e: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 f: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 i: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 g: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %7 h: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %8 j: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %9 k: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %10 aa: ptr<i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %11 @error() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @see_me_here() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @aaa() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @a() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %19 am: i64 [storage=automatic];
// DEFAULT-NEXT:         let %20 ao: i64 [storage=automatic];
// DEFAULT-NEXT:         if eq<ptr<i32>>(read<ptr<i32>>(%10), null<ptr<i32>>)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%13);
// DEFAULT-NEXT:                 if ne<i8>(read<i8>(%8), const<i8>(0))
// DEFAULT-NEXT:                     goto %15;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return;
// DEFAULT-NEXT:         label %15 ay:
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%13);
// DEFAULT-NEXT:         if ne<i8>(read<i8>(%9), const<i8>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%13);
// DEFAULT-NEXT:                 goto %16;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return;
// DEFAULT-NEXT:         label %16 az:
// DEFAULT-NEXT:             if ne<i64>(read<i64>(%5), const<i64>(0))
// DEFAULT-NEXT:                 if ne<i8>(read<i8>(%6), const<i8>(0))
// DEFAULT-NEXT:                     if ne<i8>(read<i8>(%7), const<i8>(0))
// DEFAULT-NEXT:                         if ne<i64>(read<i64>(%3), const<i64>(0))
// DEFAULT-NEXT:                             goto %17;
// DEFAULT-NEXT:         return;
// DEFAULT-NEXT:         label %17 bd:
// DEFAULT-NEXT:             write<i64>(%19, widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         while %22 lt<i64>(read<i64>(%19), read<i64>(%3))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 switch %23 read<i64>(%1)
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         case %23 const<i64>(8):
// DEFAULT-NEXT:                             goto %18;
// DEFAULT-NEXT:                         case %23 const<i64>(4):
// DEFAULT-NEXT:                             return;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 label %18 bh:
// DEFAULT-NEXT:                     if ge<i64>(read<i64>(%19), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                         write<i64>(%0, neg<i64, overflow=ub>(read<i64>(%19)));
// DEFAULT-NEXT:                 write<i64>(%20, add<i64, overflow=ub>(read<i64>(%19), read<i64>(%0)));
// DEFAULT-NEXT:                 write<i64>(%4, and<i64>(read<i64>(%20), widen<i64, reason=usual_arith>(const<i32>(7))));
// DEFAULT-NEXT:                 if eq<i64>(read<i64>(%4), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%12);
// DEFAULT-NEXT:                 if ge<i64>(read<i64>(%20), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                     let %24: i64 [synthetic] = read<i64>(%19);
// DEFAULT-NEXT:                     let %25: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%24), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                     write<i64>(%19, read<i64>(%25));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%11);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %26: i8 [synthetic] = read<i8>(%8);
// DEFAULT-NEXT:         let %27: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%26)), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%8, read<i8>(%27));
// DEFAULT-NEXT:         let %28: i8 [synthetic] = read<i8>(%9);
// DEFAULT-NEXT:         let %29: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%28)), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%9, read<i8>(%29));
// DEFAULT-NEXT:         let %30: i64 [synthetic] = read<i64>(%5);
// DEFAULT-NEXT:         let %31: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%30), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(%5, read<i64>(%31));
// DEFAULT-NEXT:         let %32: i8 [synthetic] = read<i8>(%6);
// DEFAULT-NEXT:         let %33: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%32)), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%6, read<i8>(%33));
// DEFAULT-NEXT:         let %34: i8 [synthetic] = read<i8>(%7);
// DEFAULT-NEXT:         let %35: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%34)), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%7, read<i8>(%35));
// DEFAULT-NEXT:         write<i64>(%3, widen<i64, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%14);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
