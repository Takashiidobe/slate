/* Test C23 variadic functions with no named parameters, or last named
   parameter with a declaration not allowed in C17.  Execution tests.  */
/* { dg-do run } */
/* { dg-options "-O2 -std=c23 -pedantic-errors" } */

#include <stdarg.h>

#ifdef __AVR__
/* AVR doesn't have that much stack... */
struct S {
  int a[500];
};
#else
struct S {
  int a[1024];
};
#endif

int f1(...) {
  int     r = 0;
  va_list ap;
  va_start(ap);
  r += va_arg(ap, int);
  va_end(ap);
  return r;
}

int f2(...) {
  int     r = 0;
  va_list ap;
  va_start(ap);
  r += va_arg(ap, int);
  r += va_arg(ap, int);
  va_end(ap);
  return r;
}

int f3(...) {
  int     r = 0;
  va_list ap;
  va_start(ap);
  r += va_arg(ap, int);
  r += va_arg(ap, int);
  r += va_arg(ap, int);
  va_end(ap);
  return r;
}

int f4(...) {
  int     r = 0;
  va_list ap;
  va_start(ap);
  r += va_arg(ap, int);
  r += va_arg(ap, int);
  r += va_arg(ap, int);
  r += va_arg(ap, int);
  va_end(ap);
  return r;
}

int f5(...) {
  int     r = 0;
  va_list ap;
  va_start(ap);
  r += va_arg(ap, int);
  r += va_arg(ap, int);
  r += va_arg(ap, int);
  r += va_arg(ap, int);
  r += va_arg(ap, int);
  va_end(ap);
  return r;
}

int f6(...) {
  int     r = 0;
  va_list ap;
  va_start(ap);
  r += va_arg(ap, int);
  r += va_arg(ap, int);
  r += va_arg(ap, int);
  r += va_arg(ap, int);
  r += va_arg(ap, int);
  r += va_arg(ap, int);
  va_end(ap);
  return r;
}

int f7(...) {
  int     r = 0;
  va_list ap;
  va_start(ap);
  r += va_arg(ap, int);
  r += va_arg(ap, int);
  r += va_arg(ap, int);
  r += va_arg(ap, int);
  r += va_arg(ap, int);
  r += va_arg(ap, int);
  r += va_arg(ap, int);
  va_end(ap);
  return r;
}

int f8(...) {
  int     r = 0;
  va_list ap;
  va_start(ap);
  r += va_arg(ap, int);
  r += va_arg(ap, int);
  r += va_arg(ap, int);
  r += va_arg(ap, int);
  r += va_arg(ap, int);
  r += va_arg(ap, int);
  r += va_arg(ap, int);
  r += va_arg(ap, int);
  va_end(ap);
  return r;
}

struct S s1(...) {
  int     r = 0;
  va_list ap;
  va_start(ap);
  r += va_arg(ap, int);
  va_end(ap);
  struct S s = {};
  s.a[0]     = r;
  return s;
}

struct S s2(...) {
  int     r = 0;
  va_list ap;
  va_start(ap);
  r += va_arg(ap, int);
  r += va_arg(ap, int);
  va_end(ap);
  struct S s = {};
  s.a[0]     = r;
  return s;
}

struct S s3(...) {
  int     r = 0;
  va_list ap;
  va_start(ap);
  r += va_arg(ap, int);
  r += va_arg(ap, int);
  r += va_arg(ap, int);
  va_end(ap);
  struct S s = {};
  s.a[0]     = r;
  return s;
}

struct S s4(...) {
  int     r = 0;
  va_list ap;
  va_start(ap);
  r += va_arg(ap, int);
  r += va_arg(ap, int);
  r += va_arg(ap, int);
  r += va_arg(ap, int);
  va_end(ap);
  struct S s = {};
  s.a[0]     = r;
  return s;
}

struct S s5(...) {
  int     r = 0;
  va_list ap;
  va_start(ap);
  r += va_arg(ap, int);
  r += va_arg(ap, int);
  r += va_arg(ap, int);
  r += va_arg(ap, int);
  r += va_arg(ap, int);
  va_end(ap);
  struct S s = {};
  s.a[0]     = r;
  return s;
}

struct S s6(...) {
  int     r = 0;
  va_list ap;
  va_start(ap);
  r += va_arg(ap, int);
  r += va_arg(ap, int);
  r += va_arg(ap, int);
  r += va_arg(ap, int);
  r += va_arg(ap, int);
  r += va_arg(ap, int);
  va_end(ap);
  struct S s = {};
  s.a[0]     = r;
  return s;
}

struct S s7(...) {
  int     r = 0;
  va_list ap;
  va_start(ap);
  r += va_arg(ap, int);
  r += va_arg(ap, int);
  r += va_arg(ap, int);
  r += va_arg(ap, int);
  r += va_arg(ap, int);
  r += va_arg(ap, int);
  r += va_arg(ap, int);
  va_end(ap);
  struct S s = {};
  s.a[0]     = r;
  return s;
}

struct S s8(...) {
  int     r = 0;
  va_list ap;
  va_start(ap);
  r += va_arg(ap, int);
  r += va_arg(ap, int);
  r += va_arg(ap, int);
  r += va_arg(ap, int);
  r += va_arg(ap, int);
  r += va_arg(ap, int);
  r += va_arg(ap, int);
  r += va_arg(ap, int);
  va_end(ap);
  struct S s = {};
  s.a[0]     = r;
  return s;
}

int b1(void) { return f8(1, 2, 3, 4, 5, 6, 7, 8); }

int b2(void) { return s8(1, 2, 3, 4, 5, 6, 7, 8).a[0]; }

int main() {
  if (f1(1) != 1 || f2(1, 2) != 3 || f3(1, 2, 3) != 6 || f4(1, 2, 3, 4) != 10 ||
      f5(1, 2, 3, 4, 5) != 15 || f6(1, 2, 3, 4, 5, 6) != 21 ||
      f7(1, 2, 3, 4, 5, 6, 7) != 28 || f8(1, 2, 3, 4, 5, 6, 7, 8) != 36)
    __builtin_abort();
  if (s1(1).a[0] != 1 || s2(1, 2).a[0] != 3 || s3(1, 2, 3).a[0] != 6 ||
      s4(1, 2, 3, 4).a[0] != 10 || s5(1, 2, 3, 4, 5).a[0] != 15 ||
      s6(1, 2, 3, 4, 5, 6).a[0] != 21 || s7(1, 2, 3, 4, 5, 6, 7).a[0] != 28 ||
      s8(1, 2, 3, 4, 5, 6, 7, 8).a[0] != 36)
    __builtin_abort();
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
// DEFAULT-NEXT:     type @type0 va_list = va_list;
// DEFAULT-NEXT:     type @type1 S = struct {
// DEFAULT-NEXT:         field0 a: array<i32, 1024>;
// DEFAULT-NEXT:     } [size=4096, align=4, offsets=[0]];
// DEFAULT-NEXT:     fn %2 @f1(...) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 r: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %4 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%4);
// DEFAULT-NEXT:         let %71: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:         let %72: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%71), va_arg<i32>(%4));
// DEFAULT-NEXT:         write<i32>(%3, read<i32>(%72));
// DEFAULT-NEXT:         va_end(%4);
// DEFAULT-NEXT:         return read<i32>(%3);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @f2(...) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %6 r: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %7 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%7);
// DEFAULT-NEXT:         let %73: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:         let %74: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%73), va_arg<i32>(%7));
// DEFAULT-NEXT:         write<i32>(%6, read<i32>(%74));
// DEFAULT-NEXT:         let %75: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:         let %76: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%75), va_arg<i32>(%7));
// DEFAULT-NEXT:         write<i32>(%6, read<i32>(%76));
// DEFAULT-NEXT:         va_end(%7);
// DEFAULT-NEXT:         return read<i32>(%6);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @f3(...) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %9 r: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %10 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%10);
// DEFAULT-NEXT:         let %77: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:         let %78: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%77), va_arg<i32>(%10));
// DEFAULT-NEXT:         write<i32>(%9, read<i32>(%78));
// DEFAULT-NEXT:         let %79: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:         let %80: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%79), va_arg<i32>(%10));
// DEFAULT-NEXT:         write<i32>(%9, read<i32>(%80));
// DEFAULT-NEXT:         let %81: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:         let %82: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%81), va_arg<i32>(%10));
// DEFAULT-NEXT:         write<i32>(%9, read<i32>(%82));
// DEFAULT-NEXT:         va_end(%10);
// DEFAULT-NEXT:         return read<i32>(%9);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @f4(...) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %12 r: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %13 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%13);
// DEFAULT-NEXT:         let %83: i32 [synthetic] = read<i32>(%12);
// DEFAULT-NEXT:         let %84: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%83), va_arg<i32>(%13));
// DEFAULT-NEXT:         write<i32>(%12, read<i32>(%84));
// DEFAULT-NEXT:         let %85: i32 [synthetic] = read<i32>(%12);
// DEFAULT-NEXT:         let %86: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%85), va_arg<i32>(%13));
// DEFAULT-NEXT:         write<i32>(%12, read<i32>(%86));
// DEFAULT-NEXT:         let %87: i32 [synthetic] = read<i32>(%12);
// DEFAULT-NEXT:         let %88: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%87), va_arg<i32>(%13));
// DEFAULT-NEXT:         write<i32>(%12, read<i32>(%88));
// DEFAULT-NEXT:         let %89: i32 [synthetic] = read<i32>(%12);
// DEFAULT-NEXT:         let %90: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%89), va_arg<i32>(%13));
// DEFAULT-NEXT:         write<i32>(%12, read<i32>(%90));
// DEFAULT-NEXT:         va_end(%13);
// DEFAULT-NEXT:         return read<i32>(%12);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @f5(...) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %15 r: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %16 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%16);
// DEFAULT-NEXT:         let %91: i32 [synthetic] = read<i32>(%15);
// DEFAULT-NEXT:         let %92: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%91), va_arg<i32>(%16));
// DEFAULT-NEXT:         write<i32>(%15, read<i32>(%92));
// DEFAULT-NEXT:         let %93: i32 [synthetic] = read<i32>(%15);
// DEFAULT-NEXT:         let %94: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%93), va_arg<i32>(%16));
// DEFAULT-NEXT:         write<i32>(%15, read<i32>(%94));
// DEFAULT-NEXT:         let %95: i32 [synthetic] = read<i32>(%15);
// DEFAULT-NEXT:         let %96: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%95), va_arg<i32>(%16));
// DEFAULT-NEXT:         write<i32>(%15, read<i32>(%96));
// DEFAULT-NEXT:         let %97: i32 [synthetic] = read<i32>(%15);
// DEFAULT-NEXT:         let %98: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%97), va_arg<i32>(%16));
// DEFAULT-NEXT:         write<i32>(%15, read<i32>(%98));
// DEFAULT-NEXT:         let %99: i32 [synthetic] = read<i32>(%15);
// DEFAULT-NEXT:         let %100: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%99), va_arg<i32>(%16));
// DEFAULT-NEXT:         write<i32>(%15, read<i32>(%100));
// DEFAULT-NEXT:         va_end(%16);
// DEFAULT-NEXT:         return read<i32>(%15);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @f6(...) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %18 r: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %19 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%19);
// DEFAULT-NEXT:         let %101: i32 [synthetic] = read<i32>(%18);
// DEFAULT-NEXT:         let %102: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%101), va_arg<i32>(%19));
// DEFAULT-NEXT:         write<i32>(%18, read<i32>(%102));
// DEFAULT-NEXT:         let %103: i32 [synthetic] = read<i32>(%18);
// DEFAULT-NEXT:         let %104: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%103), va_arg<i32>(%19));
// DEFAULT-NEXT:         write<i32>(%18, read<i32>(%104));
// DEFAULT-NEXT:         let %105: i32 [synthetic] = read<i32>(%18);
// DEFAULT-NEXT:         let %106: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%105), va_arg<i32>(%19));
// DEFAULT-NEXT:         write<i32>(%18, read<i32>(%106));
// DEFAULT-NEXT:         let %107: i32 [synthetic] = read<i32>(%18);
// DEFAULT-NEXT:         let %108: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%107), va_arg<i32>(%19));
// DEFAULT-NEXT:         write<i32>(%18, read<i32>(%108));
// DEFAULT-NEXT:         let %109: i32 [synthetic] = read<i32>(%18);
// DEFAULT-NEXT:         let %110: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%109), va_arg<i32>(%19));
// DEFAULT-NEXT:         write<i32>(%18, read<i32>(%110));
// DEFAULT-NEXT:         let %111: i32 [synthetic] = read<i32>(%18);
// DEFAULT-NEXT:         let %112: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%111), va_arg<i32>(%19));
// DEFAULT-NEXT:         write<i32>(%18, read<i32>(%112));
// DEFAULT-NEXT:         va_end(%19);
// DEFAULT-NEXT:         return read<i32>(%18);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @f7(...) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %21 r: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %22 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%22);
// DEFAULT-NEXT:         let %113: i32 [synthetic] = read<i32>(%21);
// DEFAULT-NEXT:         let %114: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%113), va_arg<i32>(%22));
// DEFAULT-NEXT:         write<i32>(%21, read<i32>(%114));
// DEFAULT-NEXT:         let %115: i32 [synthetic] = read<i32>(%21);
// DEFAULT-NEXT:         let %116: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%115), va_arg<i32>(%22));
// DEFAULT-NEXT:         write<i32>(%21, read<i32>(%116));
// DEFAULT-NEXT:         let %117: i32 [synthetic] = read<i32>(%21);
// DEFAULT-NEXT:         let %118: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%117), va_arg<i32>(%22));
// DEFAULT-NEXT:         write<i32>(%21, read<i32>(%118));
// DEFAULT-NEXT:         let %119: i32 [synthetic] = read<i32>(%21);
// DEFAULT-NEXT:         let %120: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%119), va_arg<i32>(%22));
// DEFAULT-NEXT:         write<i32>(%21, read<i32>(%120));
// DEFAULT-NEXT:         let %121: i32 [synthetic] = read<i32>(%21);
// DEFAULT-NEXT:         let %122: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%121), va_arg<i32>(%22));
// DEFAULT-NEXT:         write<i32>(%21, read<i32>(%122));
// DEFAULT-NEXT:         let %123: i32 [synthetic] = read<i32>(%21);
// DEFAULT-NEXT:         let %124: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%123), va_arg<i32>(%22));
// DEFAULT-NEXT:         write<i32>(%21, read<i32>(%124));
// DEFAULT-NEXT:         let %125: i32 [synthetic] = read<i32>(%21);
// DEFAULT-NEXT:         let %126: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%125), va_arg<i32>(%22));
// DEFAULT-NEXT:         write<i32>(%21, read<i32>(%126));
// DEFAULT-NEXT:         va_end(%22);
// DEFAULT-NEXT:         return read<i32>(%21);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @f8(...) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %24 r: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %25 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%25);
// DEFAULT-NEXT:         let %127: i32 [synthetic] = read<i32>(%24);
// DEFAULT-NEXT:         let %128: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%127), va_arg<i32>(%25));
// DEFAULT-NEXT:         write<i32>(%24, read<i32>(%128));
// DEFAULT-NEXT:         let %129: i32 [synthetic] = read<i32>(%24);
// DEFAULT-NEXT:         let %130: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%129), va_arg<i32>(%25));
// DEFAULT-NEXT:         write<i32>(%24, read<i32>(%130));
// DEFAULT-NEXT:         let %131: i32 [synthetic] = read<i32>(%24);
// DEFAULT-NEXT:         let %132: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%131), va_arg<i32>(%25));
// DEFAULT-NEXT:         write<i32>(%24, read<i32>(%132));
// DEFAULT-NEXT:         let %133: i32 [synthetic] = read<i32>(%24);
// DEFAULT-NEXT:         let %134: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%133), va_arg<i32>(%25));
// DEFAULT-NEXT:         write<i32>(%24, read<i32>(%134));
// DEFAULT-NEXT:         let %135: i32 [synthetic] = read<i32>(%24);
// DEFAULT-NEXT:         let %136: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%135), va_arg<i32>(%25));
// DEFAULT-NEXT:         write<i32>(%24, read<i32>(%136));
// DEFAULT-NEXT:         let %137: i32 [synthetic] = read<i32>(%24);
// DEFAULT-NEXT:         let %138: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%137), va_arg<i32>(%25));
// DEFAULT-NEXT:         write<i32>(%24, read<i32>(%138));
// DEFAULT-NEXT:         let %139: i32 [synthetic] = read<i32>(%24);
// DEFAULT-NEXT:         let %140: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%139), va_arg<i32>(%25));
// DEFAULT-NEXT:         write<i32>(%24, read<i32>(%140));
// DEFAULT-NEXT:         let %141: i32 [synthetic] = read<i32>(%24);
// DEFAULT-NEXT:         let %142: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%141), va_arg<i32>(%25));
// DEFAULT-NEXT:         write<i32>(%24, read<i32>(%142));
// DEFAULT-NEXT:         va_end(%25);
// DEFAULT-NEXT:         return read<i32>(%24);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %26 @s1(...) -> @type1 [linkage=external] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %27 r: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %28 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%28);
// DEFAULT-NEXT:         let %143: i32 [synthetic] = read<i32>(%27);
// DEFAULT-NEXT:         let %144: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%143), va_arg<i32>(%28));
// DEFAULT-NEXT:         write<i32>(%27, read<i32>(%144));
// DEFAULT-NEXT:         va_end(%28);
// DEFAULT-NEXT:         let %29 s: @type1 [storage=automatic] = aggregate<@type1, zero_fill=true>();
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1024)>(field0(%29)), const<i32>(0))), read<i32>(%27));
// DEFAULT-NEXT:         return copy<@type1, reason=return>(read<@type1>(%29));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %30 @s2(...) -> @type1 [linkage=external] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %31 r: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %32 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%32);
// DEFAULT-NEXT:         let %145: i32 [synthetic] = read<i32>(%31);
// DEFAULT-NEXT:         let %146: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%145), va_arg<i32>(%32));
// DEFAULT-NEXT:         write<i32>(%31, read<i32>(%146));
// DEFAULT-NEXT:         let %147: i32 [synthetic] = read<i32>(%31);
// DEFAULT-NEXT:         let %148: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%147), va_arg<i32>(%32));
// DEFAULT-NEXT:         write<i32>(%31, read<i32>(%148));
// DEFAULT-NEXT:         va_end(%32);
// DEFAULT-NEXT:         let %33 s: @type1 [storage=automatic] = aggregate<@type1, zero_fill=true>();
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1024)>(field0(%33)), const<i32>(0))), read<i32>(%31));
// DEFAULT-NEXT:         return copy<@type1, reason=return>(read<@type1>(%33));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %34 @s3(...) -> @type1 [linkage=external] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %35 r: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %36 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%36);
// DEFAULT-NEXT:         let %149: i32 [synthetic] = read<i32>(%35);
// DEFAULT-NEXT:         let %150: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%149), va_arg<i32>(%36));
// DEFAULT-NEXT:         write<i32>(%35, read<i32>(%150));
// DEFAULT-NEXT:         let %151: i32 [synthetic] = read<i32>(%35);
// DEFAULT-NEXT:         let %152: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%151), va_arg<i32>(%36));
// DEFAULT-NEXT:         write<i32>(%35, read<i32>(%152));
// DEFAULT-NEXT:         let %153: i32 [synthetic] = read<i32>(%35);
// DEFAULT-NEXT:         let %154: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%153), va_arg<i32>(%36));
// DEFAULT-NEXT:         write<i32>(%35, read<i32>(%154));
// DEFAULT-NEXT:         va_end(%36);
// DEFAULT-NEXT:         let %37 s: @type1 [storage=automatic] = aggregate<@type1, zero_fill=true>();
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1024)>(field0(%37)), const<i32>(0))), read<i32>(%35));
// DEFAULT-NEXT:         return copy<@type1, reason=return>(read<@type1>(%37));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %38 @s4(...) -> @type1 [linkage=external] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %39 r: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %40 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%40);
// DEFAULT-NEXT:         let %155: i32 [synthetic] = read<i32>(%39);
// DEFAULT-NEXT:         let %156: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%155), va_arg<i32>(%40));
// DEFAULT-NEXT:         write<i32>(%39, read<i32>(%156));
// DEFAULT-NEXT:         let %157: i32 [synthetic] = read<i32>(%39);
// DEFAULT-NEXT:         let %158: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%157), va_arg<i32>(%40));
// DEFAULT-NEXT:         write<i32>(%39, read<i32>(%158));
// DEFAULT-NEXT:         let %159: i32 [synthetic] = read<i32>(%39);
// DEFAULT-NEXT:         let %160: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%159), va_arg<i32>(%40));
// DEFAULT-NEXT:         write<i32>(%39, read<i32>(%160));
// DEFAULT-NEXT:         let %161: i32 [synthetic] = read<i32>(%39);
// DEFAULT-NEXT:         let %162: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%161), va_arg<i32>(%40));
// DEFAULT-NEXT:         write<i32>(%39, read<i32>(%162));
// DEFAULT-NEXT:         va_end(%40);
// DEFAULT-NEXT:         let %41 s: @type1 [storage=automatic] = aggregate<@type1, zero_fill=true>();
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1024)>(field0(%41)), const<i32>(0))), read<i32>(%39));
// DEFAULT-NEXT:         return copy<@type1, reason=return>(read<@type1>(%41));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %42 @s5(...) -> @type1 [linkage=external] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %43 r: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %44 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%44);
// DEFAULT-NEXT:         let %163: i32 [synthetic] = read<i32>(%43);
// DEFAULT-NEXT:         let %164: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%163), va_arg<i32>(%44));
// DEFAULT-NEXT:         write<i32>(%43, read<i32>(%164));
// DEFAULT-NEXT:         let %165: i32 [synthetic] = read<i32>(%43);
// DEFAULT-NEXT:         let %166: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%165), va_arg<i32>(%44));
// DEFAULT-NEXT:         write<i32>(%43, read<i32>(%166));
// DEFAULT-NEXT:         let %167: i32 [synthetic] = read<i32>(%43);
// DEFAULT-NEXT:         let %168: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%167), va_arg<i32>(%44));
// DEFAULT-NEXT:         write<i32>(%43, read<i32>(%168));
// DEFAULT-NEXT:         let %169: i32 [synthetic] = read<i32>(%43);
// DEFAULT-NEXT:         let %170: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%169), va_arg<i32>(%44));
// DEFAULT-NEXT:         write<i32>(%43, read<i32>(%170));
// DEFAULT-NEXT:         let %171: i32 [synthetic] = read<i32>(%43);
// DEFAULT-NEXT:         let %172: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%171), va_arg<i32>(%44));
// DEFAULT-NEXT:         write<i32>(%43, read<i32>(%172));
// DEFAULT-NEXT:         va_end(%44);
// DEFAULT-NEXT:         let %45 s: @type1 [storage=automatic] = aggregate<@type1, zero_fill=true>();
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1024)>(field0(%45)), const<i32>(0))), read<i32>(%43));
// DEFAULT-NEXT:         return copy<@type1, reason=return>(read<@type1>(%45));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %46 @s6(...) -> @type1 [linkage=external] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %47 r: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %48 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%48);
// DEFAULT-NEXT:         let %173: i32 [synthetic] = read<i32>(%47);
// DEFAULT-NEXT:         let %174: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%173), va_arg<i32>(%48));
// DEFAULT-NEXT:         write<i32>(%47, read<i32>(%174));
// DEFAULT-NEXT:         let %175: i32 [synthetic] = read<i32>(%47);
// DEFAULT-NEXT:         let %176: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%175), va_arg<i32>(%48));
// DEFAULT-NEXT:         write<i32>(%47, read<i32>(%176));
// DEFAULT-NEXT:         let %177: i32 [synthetic] = read<i32>(%47);
// DEFAULT-NEXT:         let %178: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%177), va_arg<i32>(%48));
// DEFAULT-NEXT:         write<i32>(%47, read<i32>(%178));
// DEFAULT-NEXT:         let %179: i32 [synthetic] = read<i32>(%47);
// DEFAULT-NEXT:         let %180: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%179), va_arg<i32>(%48));
// DEFAULT-NEXT:         write<i32>(%47, read<i32>(%180));
// DEFAULT-NEXT:         let %181: i32 [synthetic] = read<i32>(%47);
// DEFAULT-NEXT:         let %182: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%181), va_arg<i32>(%48));
// DEFAULT-NEXT:         write<i32>(%47, read<i32>(%182));
// DEFAULT-NEXT:         let %183: i32 [synthetic] = read<i32>(%47);
// DEFAULT-NEXT:         let %184: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%183), va_arg<i32>(%48));
// DEFAULT-NEXT:         write<i32>(%47, read<i32>(%184));
// DEFAULT-NEXT:         va_end(%48);
// DEFAULT-NEXT:         let %49 s: @type1 [storage=automatic] = aggregate<@type1, zero_fill=true>();
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1024)>(field0(%49)), const<i32>(0))), read<i32>(%47));
// DEFAULT-NEXT:         return copy<@type1, reason=return>(read<@type1>(%49));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %50 @s7(...) -> @type1 [linkage=external] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %51 r: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %52 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%52);
// DEFAULT-NEXT:         let %185: i32 [synthetic] = read<i32>(%51);
// DEFAULT-NEXT:         let %186: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%185), va_arg<i32>(%52));
// DEFAULT-NEXT:         write<i32>(%51, read<i32>(%186));
// DEFAULT-NEXT:         let %187: i32 [synthetic] = read<i32>(%51);
// DEFAULT-NEXT:         let %188: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%187), va_arg<i32>(%52));
// DEFAULT-NEXT:         write<i32>(%51, read<i32>(%188));
// DEFAULT-NEXT:         let %189: i32 [synthetic] = read<i32>(%51);
// DEFAULT-NEXT:         let %190: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%189), va_arg<i32>(%52));
// DEFAULT-NEXT:         write<i32>(%51, read<i32>(%190));
// DEFAULT-NEXT:         let %191: i32 [synthetic] = read<i32>(%51);
// DEFAULT-NEXT:         let %192: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%191), va_arg<i32>(%52));
// DEFAULT-NEXT:         write<i32>(%51, read<i32>(%192));
// DEFAULT-NEXT:         let %193: i32 [synthetic] = read<i32>(%51);
// DEFAULT-NEXT:         let %194: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%193), va_arg<i32>(%52));
// DEFAULT-NEXT:         write<i32>(%51, read<i32>(%194));
// DEFAULT-NEXT:         let %195: i32 [synthetic] = read<i32>(%51);
// DEFAULT-NEXT:         let %196: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%195), va_arg<i32>(%52));
// DEFAULT-NEXT:         write<i32>(%51, read<i32>(%196));
// DEFAULT-NEXT:         let %197: i32 [synthetic] = read<i32>(%51);
// DEFAULT-NEXT:         let %198: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%197), va_arg<i32>(%52));
// DEFAULT-NEXT:         write<i32>(%51, read<i32>(%198));
// DEFAULT-NEXT:         va_end(%52);
// DEFAULT-NEXT:         let %53 s: @type1 [storage=automatic] = aggregate<@type1, zero_fill=true>();
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1024)>(field0(%53)), const<i32>(0))), read<i32>(%51));
// DEFAULT-NEXT:         return copy<@type1, reason=return>(read<@type1>(%53));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %54 @s8(...) -> @type1 [linkage=external] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %55 r: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %56 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%56);
// DEFAULT-NEXT:         let %199: i32 [synthetic] = read<i32>(%55);
// DEFAULT-NEXT:         let %200: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%199), va_arg<i32>(%56));
// DEFAULT-NEXT:         write<i32>(%55, read<i32>(%200));
// DEFAULT-NEXT:         let %201: i32 [synthetic] = read<i32>(%55);
// DEFAULT-NEXT:         let %202: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%201), va_arg<i32>(%56));
// DEFAULT-NEXT:         write<i32>(%55, read<i32>(%202));
// DEFAULT-NEXT:         let %203: i32 [synthetic] = read<i32>(%55);
// DEFAULT-NEXT:         let %204: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%203), va_arg<i32>(%56));
// DEFAULT-NEXT:         write<i32>(%55, read<i32>(%204));
// DEFAULT-NEXT:         let %205: i32 [synthetic] = read<i32>(%55);
// DEFAULT-NEXT:         let %206: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%205), va_arg<i32>(%56));
// DEFAULT-NEXT:         write<i32>(%55, read<i32>(%206));
// DEFAULT-NEXT:         let %207: i32 [synthetic] = read<i32>(%55);
// DEFAULT-NEXT:         let %208: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%207), va_arg<i32>(%56));
// DEFAULT-NEXT:         write<i32>(%55, read<i32>(%208));
// DEFAULT-NEXT:         let %209: i32 [synthetic] = read<i32>(%55);
// DEFAULT-NEXT:         let %210: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%209), va_arg<i32>(%56));
// DEFAULT-NEXT:         write<i32>(%55, read<i32>(%210));
// DEFAULT-NEXT:         let %211: i32 [synthetic] = read<i32>(%55);
// DEFAULT-NEXT:         let %212: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%211), va_arg<i32>(%56));
// DEFAULT-NEXT:         write<i32>(%55, read<i32>(%212));
// DEFAULT-NEXT:         let %213: i32 [synthetic] = read<i32>(%55);
// DEFAULT-NEXT:         let %214: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%213), va_arg<i32>(%56));
// DEFAULT-NEXT:         write<i32>(%55, read<i32>(%214));
// DEFAULT-NEXT:         va_end(%56);
// DEFAULT-NEXT:         let %57 s: @type1 [storage=automatic] = aggregate<@type1, zero_fill=true>();
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1024)>(field0(%57)), const<i32>(0))), read<i32>(%55));
// DEFAULT-NEXT:         return copy<@type1, reason=return>(read<@type1>(%57));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %58 @b1() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(...) -> i32>(%23, const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %59 @b2() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1024)>(field0(temporary %61 = call<@type1, signature=fn(...) -> @type1, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar) -> native_c>(%54, const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8)))), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %62 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %60 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %215: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(...) -> i32>(%2, const<i32>(1)), const<i32>(1))
// DEFAULT-NEXT:             write<bool>(%215, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%215, ne<i32>(call<i32, signature=fn(...) -> i32>(%5, const<i32>(1), const<i32>(2)), const<i32>(3)));
// DEFAULT-NEXT:         let %216: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%215)
// DEFAULT-NEXT:             write<bool>(%216, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%216, ne<i32>(call<i32, signature=fn(...) -> i32>(%8, const<i32>(1), const<i32>(2), const<i32>(3)), const<i32>(6)));
// DEFAULT-NEXT:         let %217: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%216)
// DEFAULT-NEXT:             write<bool>(%217, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%217, ne<i32>(call<i32, signature=fn(...) -> i32>(%11, const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4)), const<i32>(10)));
// DEFAULT-NEXT:         let %218: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%217)
// DEFAULT-NEXT:             write<bool>(%218, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%218, ne<i32>(call<i32, signature=fn(...) -> i32>(%14, const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5)), const<i32>(15)));
// DEFAULT-NEXT:         let %219: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%218)
// DEFAULT-NEXT:             write<bool>(%219, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%219, ne<i32>(call<i32, signature=fn(...) -> i32>(%17, const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6)), const<i32>(21)));
// DEFAULT-NEXT:         let %220: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%219)
// DEFAULT-NEXT:             write<bool>(%220, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%220, ne<i32>(call<i32, signature=fn(...) -> i32>(%20, const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7)), const<i32>(28)));
// DEFAULT-NEXT:         let %221: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%220)
// DEFAULT-NEXT:             write<bool>(%221, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%221, ne<i32>(call<i32, signature=fn(...) -> i32>(%23, const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8)), const<i32>(36)));
// DEFAULT-NEXT:         if read<bool>(%221)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%62);
// DEFAULT-NEXT:         let %222: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1024)>(field0(temporary %63 = call<@type1, signature=fn(...) -> @type1, abi=sysv64(scalar) -> native_c>(%26, const<i32>(1)))), const<i32>(0)))), const<i32>(1))
// DEFAULT-NEXT:             write<bool>(%222, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%222, ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1024)>(field0(temporary %64 = call<@type1, signature=fn(...) -> @type1, abi=sysv64(scalar, scalar) -> native_c>(%30, const<i32>(1), const<i32>(2)))), const<i32>(0)))), const<i32>(3)));
// DEFAULT-NEXT:         let %223: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%222)
// DEFAULT-NEXT:             write<bool>(%223, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%223, ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1024)>(field0(temporary %65 = call<@type1, signature=fn(...) -> @type1, abi=sysv64(scalar, scalar, scalar) -> native_c>(%34, const<i32>(1), const<i32>(2), const<i32>(3)))), const<i32>(0)))), const<i32>(6)));
// DEFAULT-NEXT:         let %224: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%223)
// DEFAULT-NEXT:             write<bool>(%224, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%224, ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1024)>(field0(temporary %66 = call<@type1, signature=fn(...) -> @type1, abi=sysv64(scalar, scalar, scalar, scalar) -> native_c>(%38, const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4)))), const<i32>(0)))), const<i32>(10)));
// DEFAULT-NEXT:         let %225: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%224)
// DEFAULT-NEXT:             write<bool>(%225, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%225, ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1024)>(field0(temporary %67 = call<@type1, signature=fn(...) -> @type1, abi=sysv64(scalar, scalar, scalar, scalar, scalar) -> native_c>(%42, const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5)))), const<i32>(0)))), const<i32>(15)));
// DEFAULT-NEXT:         let %226: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%225)
// DEFAULT-NEXT:             write<bool>(%226, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%226, ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1024)>(field0(temporary %68 = call<@type1, signature=fn(...) -> @type1, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar) -> native_c>(%46, const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6)))), const<i32>(0)))), const<i32>(21)));
// DEFAULT-NEXT:         let %227: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%226)
// DEFAULT-NEXT:             write<bool>(%227, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%227, ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1024)>(field0(temporary %69 = call<@type1, signature=fn(...) -> @type1, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar) -> native_c>(%50, const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7)))), const<i32>(0)))), const<i32>(28)));
// DEFAULT-NEXT:         let %228: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%227)
// DEFAULT-NEXT:             write<bool>(%228, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%228, ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1024)>(field0(temporary %70 = call<@type1, signature=fn(...) -> @type1, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar) -> native_c>(%54, const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8)))), const<i32>(0)))), const<i32>(36)));
// DEFAULT-NEXT:         if read<bool>(%228)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%62);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
