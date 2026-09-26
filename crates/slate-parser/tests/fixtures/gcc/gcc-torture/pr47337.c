/* PR rtl-optimization/47337 */

static unsigned int a[256], b = 0;
static char         c = 0;
static int          d = 0, *f = &d;
static long long    e = 0;

static short foo(long long x, long long y) { return x / y; }

static char bar(char x, char y) { return x - y; }

static int baz(int x, int y) {
  *f = (y != (short)(y * 3));
  for (c = 0; c < 2; c++) {
  lab:
    if (d) {
      if (e)
        e = 1;
      else
        return x;
    } else {
      d = 1;
      goto lab;
    }
    f = &d;
  }
  return x;
}

static void fnx(unsigned long long x, int y) {
  if (!y) {
    b = a[b & 1];
    b = a[b & 1];
    b = a[(b ^ (x & 1)) & 1];
    b = a[(b ^ (x & 1)) & 1];
  }
}

char *volatile w = "2";

int main() {
  int          h = 0;
  unsigned int k = 0;
  int          l[8];
  int          i, j;

  if (__builtin_strcmp(w, "1") == 0)
    h = 1;

  for (i = 0; i < 256; i++) {
    for (j = 8; j > 0; j--)
      k = 1;
    a[i] = k;
  }
  for (i = 0; i < 8; i++)
    l[i] = 0;

  d = bar(c, c);
  d = baz(c, 1 | foo(l[0], 10));
  fnx(d, h);
  fnx(e, h);

  if (d != 0)
    __builtin_abort();
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
// DEFAULT-NEXT:     global %0 a: array<u32, 256> [storage=static] [align=16] [linkage=internal];
// DEFAULT-NEXT:     global %1 b: u32 [storage=static] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %2 c: i8 [storage=static] = truncate<i8, reason=assign, fits=always>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %3 d: i32 [storage=static] = const<i32>(0) [linkage=internal];
// DEFAULT-NEXT:     global %4 f: ptr<i32> [storage=static] = addr_of<ptr<i32>>(%3) [linkage=internal];
// DEFAULT-NEXT:     global %5 e: i64 [storage=static] = widen<i64, reason=assign>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %27 .str27: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %19 w: volatile ptr<i8> [storage=static] = array_decay<ptr<i8>, length=Some(2)>(%27) [linkage=external];
// DEFAULT-NEXT:     global %31 .str31: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([49, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %6 @foo(%7 x: i64, %8 y: i64) -> i16 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(div<i64, by_zero=ub, min_by_neg_one=ub>(read<i64>(%7), read<i64>(%8)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @bar(%10 x: i8, %11 y: i8) -> i8 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i8, reason=return, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%10)), widen<i32, reason=promotion>(read<i8>(%11))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @baz(%14 x: i32, %15 y: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%4)), from_bool<i32, reason=assign>(ne<i32>(read<i32>(%15), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(mul<i32, overflow=ub>(read<i32>(%15), const<i32>(3)))))));
// DEFAULT-NEXT:         for %26
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i8>(%2, truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             condition: lt<i32>(widen<i32, reason=promotion>(read<i8>(%2)), const<i32>(2))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %36: i8 [synthetic] = read<i8>(%2);
// DEFAULT-NEXT:                 let %37: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%36)), const<i32>(1)));
// DEFAULT-NEXT:                 write<i8>(%2, read<i8>(%37));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     label %13 lab:
// DEFAULT-NEXT:                         if ne<i32>(read<i32>(%3), const<i32>(0))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 if ne<i64>(read<i64>(%5), const<i64>(0))
// DEFAULT-NEXT:                                     write<i64>(%5, widen<i64, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:                                 else
// DEFAULT-NEXT:                                     return read<i32>(%14);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%3, const<i32>(1));
// DEFAULT-NEXT:                                 goto %13;
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     write<ptr<i32>>(%4, addr_of<ptr<i32>>(%3));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<i32>(%14);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @fnx(%17 x: u64, %18 y: i32) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32>(%18), const<i32>(0)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<u32>(%1, read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(256)>(%0), and<u32>(read<u32>(%1), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))))));
// DEFAULT-NEXT:                 write<u32>(%1, read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(256)>(%0), and<u32>(read<u32>(%1), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))))));
// DEFAULT-NEXT:                 write<u32>(%1, read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(256)>(%0), and<u64>(xor<u64>(widen<u64, reason=usual_arith>(read<u32>(%1)), and<u64>(read<u64>(%17), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))))));
// DEFAULT-NEXT:                 write<u32>(%1, read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(256)>(%0), and<u64>(xor<u64>(widen<u64, reason=usual_arith>(read<u32>(%1)), and<u64>(read<u64>(%17), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %30 @__builtin_strcmp(%28 <unnamed>: ptr<const i8>, %29 <unnamed>: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %35 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %20 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %21 h: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %22 k: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:         let %23 l: array<i32, 8> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %24 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %25 j: i32 [storage=automatic];
// DEFAULT-NEXT:         if eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%30, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>, volatile>(%19)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%31))), const<i32>(0))
// DEFAULT-NEXT:             write<i32>(%21, const<i32>(1));
// DEFAULT-NEXT:         for %32
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%24, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%24), const<i32>(256))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %38: i32 [synthetic] = read<i32>(%24);
// DEFAULT-NEXT:                 let %39: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%38), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%24, read<i32>(%39));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     for %33
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             write<i32>(%25, const<i32>(8));
// DEFAULT-NEXT:                         condition: gt<i32>(read<i32>(%25), const<i32>(0))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %40: i32 [synthetic] = read<i32>(%25);
// DEFAULT-NEXT:                             let %41: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%40), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%25, read<i32>(%41));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             write<u32>(%22, reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(256)>(%0), read<i32>(%24))), read<u32>(%22));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %34
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%24, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%24), const<i32>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %42: i32 [synthetic] = read<i32>(%24);
// DEFAULT-NEXT:                 let %43: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%42), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%24, read<i32>(%43));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(8)>(%23), read<i32>(%24))), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%3, widen<i32, reason=assign>(call<i8, signature=fn(i8, i8) -> i8>(%9, read<i8>(%2), read<i8>(%2))));
// DEFAULT-NEXT:         widen<i32, reason=assign>(call<i8, signature=fn(i8, i8) -> i8>(%9, read<i8>(%2), read<i8>(%2)));
// DEFAULT-NEXT:         write<i32>(%3, call<i32, signature=fn(i32, i32) -> i32>(%12, widen<i32, reason=arg>(read<i8>(%2)), or<i32>(const<i32>(1), widen<i32, reason=promotion>(call<i16, signature=fn(i64, i64) -> i16>(%6, widen<i64, reason=arg>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(8)>(%23), const<i32>(0))))), widen<i64, reason=arg>(const<i32>(10)))))));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, i32) -> i32>(%12, widen<i32, reason=arg>(read<i8>(%2)), or<i32>(const<i32>(1), widen<i32, reason=promotion>(call<i16, signature=fn(i64, i64) -> i16>(%6, widen<i64, reason=arg>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(8)>(%23), const<i32>(0))))), widen<i64, reason=arg>(const<i32>(10))))));
// DEFAULT-NEXT:         call<void, signature=fn(u64, i32) -> void>(%16, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%3))), read<i32>(%21));
// DEFAULT-NEXT:         call<void, signature=fn(u64, i32) -> void>(%16, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%5)), read<i32>(%21));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%3), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%35);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
