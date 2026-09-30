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
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_i:[0-9]+]] i: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g:[0-9]+]] g: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_h:[0-9]+]] h: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_j:[0-9]+]] j: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_k:[0-9]+]] k: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_aa:[0-9]+]] aa: ptr<i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_error:[0-9]+]] @error() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort:[0-9]+]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_see_me_here:[0-9]+]] @see_me_here() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_aaa:[0-9]+]] @aaa() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_a:[0-9]+]] @a() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_am:[0-9]+]] am: i64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ao:[0-9]+]] ao: i64 [storage=automatic];
// DEFAULT-NEXT:         if eq<ptr<i32>>(read<ptr<i32>>(%[[VALUE_aa]]), null<ptr<i32>>)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_aaa]]);
// DEFAULT-NEXT:                 if ne<i8>(read<i8>(%[[VALUE_j]]), const<i8>(0))
// DEFAULT-NEXT:                     goto %[[VALUE_ay:[0-9]+]];
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return;
// DEFAULT-NEXT:         label %[[VALUE_ay]] ay:
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_aaa]]);
// DEFAULT-NEXT:         if ne<i8>(read<i8>(%[[VALUE_k]]), const<i8>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_aaa]]);
// DEFAULT-NEXT:                 goto %[[VALUE_az:[0-9]+]];
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return;
// DEFAULT-NEXT:         label %[[VALUE_az]] az:
// DEFAULT-NEXT:             if ne<i64>(read<i64>(%[[VALUE_i]]), const<i64>(0))
// DEFAULT-NEXT:                 if ne<i8>(read<i8>(%[[VALUE_g]]), const<i8>(0))
// DEFAULT-NEXT:                     if ne<i8>(read<i8>(%[[VALUE_h]]), const<i8>(0))
// DEFAULT-NEXT:                         if ne<i64>(read<i64>(%[[VALUE_e]]), const<i64>(0))
// DEFAULT-NEXT:                             goto %[[VALUE_bd:[0-9]+]];
// DEFAULT-NEXT:         return;
// DEFAULT-NEXT:         label %[[VALUE_bd]] bd:
// DEFAULT-NEXT:             write<i64>(%[[VALUE_am]], widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         while %[[VALUE0:[0-9]+]] lt<i64>(read<i64>(%[[VALUE_am]]), read<i64>(%[[VALUE_e]]))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 switch %[[VALUE1:[0-9]+]] read<i64>(%[[VALUE_c]])
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         case %[[VALUE1]] const<i64>(8):
// DEFAULT-NEXT:                             goto %[[VALUE_bh:[0-9]+]];
// DEFAULT-NEXT:                         case %[[VALUE1]] const<i64>(4):
// DEFAULT-NEXT:                             return;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 label %[[VALUE_bh]] bh:
// DEFAULT-NEXT:                     if ge<i64>(read<i64>(%[[VALUE_am]]), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                         write<i64>(%[[VALUE_b]], neg<i64, overflow=ub>(read<i64>(%[[VALUE_am]])));
// DEFAULT-NEXT:                 write<i64>(%[[VALUE_ao]], add<i64, overflow=ub>(read<i64>(%[[VALUE_am]]), read<i64>(%[[VALUE_b]])));
// DEFAULT-NEXT:                 write<i64>(%[[VALUE_f]], and<i64>(read<i64>(%[[VALUE_ao]]), widen<i64, reason=usual_arith>(const<i32>(7))));
// DEFAULT-NEXT:                 if eq<i64>(read<i64>(%[[VALUE_f]]), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_see_me_here]]);
// DEFAULT-NEXT:                 if ge<i64>(read<i64>(%[[VALUE_ao]]), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                     let %[[VALUE2:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_am]]);
// DEFAULT-NEXT:                     let %[[VALUE3:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE2]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                     write<i64>(%[[VALUE_am]], read<i64>(%[[VALUE3]]));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_error]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_j]]);
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE4]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_j]], read<i8>(%[[VALUE5]]));
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_k]]);
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE6]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_k]], read<i8>(%[[VALUE7]]));
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_i]]);
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE8]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(%[[VALUE_i]], read<i64>(%[[VALUE9]]));
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_g]]);
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE10]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_g]], read<i8>(%[[VALUE11]]));
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_h]]);
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE12]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_h]], read<i8>(%[[VALUE13]]));
// DEFAULT-NEXT:         write<i64>(%[[VALUE_e]], widen<i64, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_a]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
