#include <stdio.h>
#include <string.h>

static int cmp_texts(const char *a, int alen, const char *b, int blen) {
  int sa = 0, sb = 0;
  for (int i = 0; i < alen; i++)
    sa += a[i];
  for (int i = 0; i < blen; i++)
    sb += b[i];
  int order = strcmp(a, b);
  int sign  = (order > 0) - (order < 0);
  int eq    = strcmp(a, b) == 0;
  return sign * 1000 + eq * 100 + (sa - sb);
}

int main(void) {
  const char x[] = "abc";
  const char y[] = "abd";
  printf("%d %d %d\n", cmp_texts(x, 3, y, 3), cmp_texts(y, 3, x, 3),
         cmp_texts(x, 3, x, 3));
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
// DEFAULT-NEXT:     global %25 .str25: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%20 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %4 @strcmp(%21 __s1: ptr<const i8>, %22 __s2: ptr<const i8>) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %5 @cmp_texts(%6 a: ptr<const i8>, %7 alen: i32, %8 b: ptr<const i8>, %9 blen: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %10 sa: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %11 sb: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %23
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %12 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%12), read<i32>(%7))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %26: i32 [synthetic] = read<i32>(%12);
// DEFAULT-NEXT:                 let %27: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%26), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%12, read<i32>(%27));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %28: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                 let %29: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%28), widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%6), read<i32>(%12))))));
// DEFAULT-NEXT:                 write<i32>(%10, read<i32>(%29));
// DEFAULT-NEXT:         for %24
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %13 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%13), read<i32>(%9))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %30: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:                 let %31: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%30), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%13, read<i32>(%31));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %32: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:                 let %33: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%32), widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%8), read<i32>(%13))))));
// DEFAULT-NEXT:                 write<i32>(%11, read<i32>(%33));
// DEFAULT-NEXT:         let %14 order: i32 [storage=automatic] = call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%4, read<ptr<const i8>>(%6), read<ptr<const i8>>(%8));
// DEFAULT-NEXT:         let %15 sign: i32 [storage=automatic] = sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(gt<i32>(read<i32>(%14), const<i32>(0))), from_bool<i32, reason=promotion>(lt<i32>(read<i32>(%14), const<i32>(0))));
// DEFAULT-NEXT:         let %16 eq: i32 [storage=automatic] = from_bool<i32, reason=assign>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%4, read<ptr<const i8>>(%6), read<ptr<const i8>>(%8)), const<i32>(0)));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(mul<i32, overflow=ub>(read<i32>(%15), const<i32>(1000)), mul<i32, overflow=ub>(read<i32>(%16), const<i32>(100))), sub<i32, overflow=ub>(read<i32>(%10), read<i32>(%11)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %18 x: array<i8, 4> [storage=automatic] [const] = code_units<array<i8, 4>>([97, 98, 99, 0]);
// DEFAULT-NEXT:         let %19 y: array<i8, 4> [storage=automatic] [const] = code_units<array<i8, 4>>([97, 98, 100, 0]);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%25)), call<i32, signature=fn(ptr<const i8>, i32, ptr<const i8>, i32) -> i32>(%5, array_decay<ptr<const i8>, length=Some(4)>(%18), const<i32>(3), array_decay<ptr<const i8>, length=Some(4)>(%19), const<i32>(3)), call<i32, signature=fn(ptr<const i8>, i32, ptr<const i8>, i32) -> i32>(%5, array_decay<ptr<const i8>, length=Some(4)>(%19), const<i32>(3), array_decay<ptr<const i8>, length=Some(4)>(%18), const<i32>(3)), call<i32, signature=fn(ptr<const i8>, i32, ptr<const i8>, i32) -> i32>(%5, array_decay<ptr<const i8>, length=Some(4)>(%18), const<i32>(3), array_decay<ptr<const i8>, length=Some(4)>(%18), const<i32>(3)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
