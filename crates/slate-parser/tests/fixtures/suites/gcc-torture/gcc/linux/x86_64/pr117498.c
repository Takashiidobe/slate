/* PR middle-end/117498 */

int          a, d, f;
char         g;
volatile int c = 1;

int foo() {
  if (c == 0)
    return -1;
  return 1;
}

void bar(int h, int i, char *k, char *m) {
  for (; d < i; d += 2)
    for (int j = 0; j < h; j++)
      m[j] = k[4 * j];
}

void baz(long h) {
  char n = 0;
  bar(h, 4, &n, &g);
}

int main() {
  f = foo();
  baz((unsigned char)f - 4);
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
// DEFAULT-NEXT:     global %0 a: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 d: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 f: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 g: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 c: volatile i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     fn %5 @foo() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if eq<i32>(read<i32, volatile>(%4), const<i32>(0))
// DEFAULT-NEXT:             return neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @bar(%7 h: i32, %8 i: i32, %9 k: ptr<i8>, %10 m: ptr<i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         for %16
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%1), read<i32>(%8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %18: i32 [synthetic] = read<i32>(%1);
// DEFAULT-NEXT:                 let %19: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%18), const<i32>(2));
// DEFAULT-NEXT:                 write<i32>(%1, read<i32>(%19));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 for %17
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         let %11 j: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%11), read<i32>(%7))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %20: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:                         let %21: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%20), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%11, read<i32>(%21));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%10), read<i32>(%11))), read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%9), mul<i32, overflow=ub>(const<i32>(4), read<i32>(%11))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @baz(%13 h: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %14 n: i8 [storage=automatic] = truncate<i8, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, ptr<i8>, ptr<i8>) -> void>(%6, truncate<i32, reason=arg, fits=unknown>(read<i64>(%13)), const<i32>(4), addr_of<ptr<i8>>(%14), addr_of<ptr<i8>>(%3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<i32>(%2, call<i32, signature=fn() -> i32>(%5));
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%5);
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%12, widen<i64, reason=arg>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(read<i32>(%2))))), const<i32>(4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
