/* The purpose of this code is to test argument passing of a tuple of
   11 integers, with the break point between named and unnamed arguments
   at every possible position.	*/

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

static int errors = 0;

static void verify(const char *tcase, int n[11]) {
  int i;
  for (i = 0; i <= 10; i++)
    if (n[i] != i) {
      printf(" %s: n[%d] = %d expected %d\n", tcase, i, n[i], i);
      errors++;
    }
}

#define STR(x) #x

#define p(i) int q##i,
#define P(i) n[i] = q##i;

#define p0 p(0)
#define p1 p(1)
#define p2 p(2)
#define p3 p(3)
#define p4 p(4)
#define p5 p(5)
#define p6 p(6)
#define p7 p(7)
#define p8 p(8)
#define p9 p(9)

#define P0 P(0)
#define P1 P(1)
#define P2 P(2)
#define P3 P(3)
#define P4 P(4)
#define P5 P(5)
#define P6 P(6)
#define P7 P(7)
#define P8 P(8)
#define P9 P(9)

#define TCASE(x, params, vecinit)                                              \
  static void varargs##x(params...) {                                          \
    va_list ap;                                                                \
    int     n[11];                                                             \
    int     i;                                                                 \
                                                                               \
    va_start(ap, q##x);                                                        \
    vecinit for (i = x + 1; i <= 10; i++) n[i] = va_arg(ap, int);              \
    va_end(ap);                                                                \
                                                                               \
    verify(STR(varargs##x), n);                                                \
  }

#define TEST(x) varargs##x(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10)

TCASE(0, p0, P0)
TCASE(1, p0 p1, P0 P1)
TCASE(2, p0 p1 p2, P0 P1 P2)
TCASE(3, p0 p1 p2 p3, P0 P1 P2 P3)
TCASE(4, p0 p1 p2 p3 p4, P0 P1 P2 P3 P4)
TCASE(5, p0 p1 p2 p3 p4 p5, P0 P1 P2 P3 P4 P5)
TCASE(6, p0 p1 p2 p3 p4 p5 p6, P0 P1 P2 P3 P4 P5 P6)
TCASE(7, p0 p1 p2 p3 p4 p5 p6 p7, P0 P1 P2 P3 P4 P5 P6 P7)
TCASE(8, p0 p1 p2 p3 p4 p5 p6 p7 p8, P0 P1 P2 P3 P4 P5 P6 P7 P8)
TCASE(9, p0 p1 p2 p3 p4 p5 p6 p7 p8 p9, P0 P1 P2 P3 P4 P5 P6 P7 P8 P9)

int main(void) {
  TEST(0);
  TEST(1);
  TEST(2);
  TEST(3);
  TEST(4);
  TEST(5);
  TEST(6);
  TEST(7);
  TEST(8);
  TEST(9);

  if (errors)
    abort();
  exit(0);
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
// DEFAULT-NEXT:     type @type0 __gnuc_va_list = va_list;
// DEFAULT-NEXT:     type @type1 va_list = va_list;
// DEFAULT-NEXT:     global %7 errors: i32 [storage=static] = const<i32>(0) [linkage=internal];
// DEFAULT-NEXT:     global %111 .str111: array<i8, 29> [storage=static] = code_units<array<i8, 29>>([32, 37, 115, 58, 32, 110, 91, 37, 100, 93, 32, 61, 32, 37, 100, 32, 101, 120, 112, 101, 99, 116, 101, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %113 .str113: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([118, 97, 114, 97, 114, 103, 115, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %115 .str115: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([118, 97, 114, 97, 114, 103, 115, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %117 .str117: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([118, 97, 114, 97, 114, 103, 115, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %119 .str119: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([118, 97, 114, 97, 114, 103, 115, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %121 .str121: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([118, 97, 114, 97, 114, 103, 115, 52, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %123 .str123: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([118, 97, 114, 97, 114, 103, 115, 53, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %125 .str125: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([118, 97, 114, 97, 114, 103, 115, 54, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %127 .str127: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([118, 97, 114, 97, 114, 103, 115, 55, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %129 .str129: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([118, 97, 114, 97, 114, 103, 115, 56, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %131 .str131: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([118, 97, 114, 97, 114, 103, 115, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %3 @printf(%108 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %4 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %6 @exit(%109 __status: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %8 @verify(%9 tcase: ptr<const i8>, %10 n: ptr<i32> [array=11]) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %11 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %110
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%11, const<i32>(0));
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%11), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %132: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:                 let %133: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%132), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%11, read<i32>(%133));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%10), read<i32>(%11)))), read<i32>(%11))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(29)>(%111)), read<ptr<const i8>>(%9), read<i32>(%11), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%10), read<i32>(%11)))), read<i32>(%11));
// DEFAULT-NEXT:                         let %134: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                         let %135: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%134), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%7, read<i32>(%135));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @varargs0(%13 q0: i32, ...) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %14 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %15 n: array<i32, 11> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %16 i: i32 [storage=automatic];
// DEFAULT-NEXT:         va_start(%14);
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%15), const<i32>(0))), read<i32>(%13));
// DEFAULT-NEXT:         for %112
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%16, add<i32, overflow=ub>(const<i32>(0), const<i32>(1)));
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%16), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %136: i32 [synthetic] = read<i32>(%16);
// DEFAULT-NEXT:                 let %137: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%136), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%16, read<i32>(%137));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%15), read<i32>(%16))), va_arg<i32>(%14));
// DEFAULT-NEXT:                 va_arg<i32>(%14);
// DEFAULT-NEXT:         va_end(%14);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, ptr<i32>) -> void>(%8, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%113)), array_decay<ptr<i32>, length=Some(11)>(%15));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @varargs1(%18 q0: i32, %19 q1: i32, ...) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %20 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %21 n: array<i32, 11> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %22 i: i32 [storage=automatic];
// DEFAULT-NEXT:         va_start(%20);
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%21), const<i32>(0))), read<i32>(%18));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%21), const<i32>(1))), read<i32>(%19));
// DEFAULT-NEXT:         for %114
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%22, add<i32, overflow=ub>(const<i32>(1), const<i32>(1)));
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%22), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %138: i32 [synthetic] = read<i32>(%22);
// DEFAULT-NEXT:                 let %139: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%138), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%22, read<i32>(%139));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%21), read<i32>(%22))), va_arg<i32>(%20));
// DEFAULT-NEXT:                 va_arg<i32>(%20);
// DEFAULT-NEXT:         va_end(%20);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, ptr<i32>) -> void>(%8, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%115)), array_decay<ptr<i32>, length=Some(11)>(%21));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @varargs2(%24 q0: i32, %25 q1: i32, %26 q2: i32, ...) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %27 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %28 n: array<i32, 11> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %29 i: i32 [storage=automatic];
// DEFAULT-NEXT:         va_start(%27);
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%28), const<i32>(0))), read<i32>(%24));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%28), const<i32>(1))), read<i32>(%25));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%28), const<i32>(2))), read<i32>(%26));
// DEFAULT-NEXT:         for %116
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%29, add<i32, overflow=ub>(const<i32>(2), const<i32>(1)));
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%29), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %140: i32 [synthetic] = read<i32>(%29);
// DEFAULT-NEXT:                 let %141: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%140), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%29, read<i32>(%141));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%28), read<i32>(%29))), va_arg<i32>(%27));
// DEFAULT-NEXT:                 va_arg<i32>(%27);
// DEFAULT-NEXT:         va_end(%27);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, ptr<i32>) -> void>(%8, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%117)), array_decay<ptr<i32>, length=Some(11)>(%28));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %30 @varargs3(%31 q0: i32, %32 q1: i32, %33 q2: i32, %34 q3: i32, ...) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %35 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %36 n: array<i32, 11> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %37 i: i32 [storage=automatic];
// DEFAULT-NEXT:         va_start(%35);
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%36), const<i32>(0))), read<i32>(%31));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%36), const<i32>(1))), read<i32>(%32));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%36), const<i32>(2))), read<i32>(%33));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%36), const<i32>(3))), read<i32>(%34));
// DEFAULT-NEXT:         for %118
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%37, add<i32, overflow=ub>(const<i32>(3), const<i32>(1)));
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%37), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %142: i32 [synthetic] = read<i32>(%37);
// DEFAULT-NEXT:                 let %143: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%142), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%37, read<i32>(%143));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%36), read<i32>(%37))), va_arg<i32>(%35));
// DEFAULT-NEXT:                 va_arg<i32>(%35);
// DEFAULT-NEXT:         va_end(%35);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, ptr<i32>) -> void>(%8, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%119)), array_decay<ptr<i32>, length=Some(11)>(%36));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %38 @varargs4(%39 q0: i32, %40 q1: i32, %41 q2: i32, %42 q3: i32, %43 q4: i32, ...) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %44 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %45 n: array<i32, 11> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %46 i: i32 [storage=automatic];
// DEFAULT-NEXT:         va_start(%44);
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%45), const<i32>(0))), read<i32>(%39));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%45), const<i32>(1))), read<i32>(%40));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%45), const<i32>(2))), read<i32>(%41));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%45), const<i32>(3))), read<i32>(%42));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%45), const<i32>(4))), read<i32>(%43));
// DEFAULT-NEXT:         for %120
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%46, add<i32, overflow=ub>(const<i32>(4), const<i32>(1)));
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%46), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %144: i32 [synthetic] = read<i32>(%46);
// DEFAULT-NEXT:                 let %145: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%144), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%46, read<i32>(%145));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%45), read<i32>(%46))), va_arg<i32>(%44));
// DEFAULT-NEXT:                 va_arg<i32>(%44);
// DEFAULT-NEXT:         va_end(%44);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, ptr<i32>) -> void>(%8, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%121)), array_decay<ptr<i32>, length=Some(11)>(%45));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %47 @varargs5(%48 q0: i32, %49 q1: i32, %50 q2: i32, %51 q3: i32, %52 q4: i32, %53 q5: i32, ...) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %54 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %55 n: array<i32, 11> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %56 i: i32 [storage=automatic];
// DEFAULT-NEXT:         va_start(%54);
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%55), const<i32>(0))), read<i32>(%48));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%55), const<i32>(1))), read<i32>(%49));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%55), const<i32>(2))), read<i32>(%50));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%55), const<i32>(3))), read<i32>(%51));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%55), const<i32>(4))), read<i32>(%52));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%55), const<i32>(5))), read<i32>(%53));
// DEFAULT-NEXT:         for %122
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%56, add<i32, overflow=ub>(const<i32>(5), const<i32>(1)));
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%56), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %146: i32 [synthetic] = read<i32>(%56);
// DEFAULT-NEXT:                 let %147: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%146), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%56, read<i32>(%147));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%55), read<i32>(%56))), va_arg<i32>(%54));
// DEFAULT-NEXT:                 va_arg<i32>(%54);
// DEFAULT-NEXT:         va_end(%54);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, ptr<i32>) -> void>(%8, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%123)), array_decay<ptr<i32>, length=Some(11)>(%55));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %57 @varargs6(%58 q0: i32, %59 q1: i32, %60 q2: i32, %61 q3: i32, %62 q4: i32, %63 q5: i32, %64 q6: i32, ...) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %65 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %66 n: array<i32, 11> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %67 i: i32 [storage=automatic];
// DEFAULT-NEXT:         va_start(%65);
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%66), const<i32>(0))), read<i32>(%58));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%66), const<i32>(1))), read<i32>(%59));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%66), const<i32>(2))), read<i32>(%60));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%66), const<i32>(3))), read<i32>(%61));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%66), const<i32>(4))), read<i32>(%62));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%66), const<i32>(5))), read<i32>(%63));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%66), const<i32>(6))), read<i32>(%64));
// DEFAULT-NEXT:         for %124
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%67, add<i32, overflow=ub>(const<i32>(6), const<i32>(1)));
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%67), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %148: i32 [synthetic] = read<i32>(%67);
// DEFAULT-NEXT:                 let %149: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%148), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%67, read<i32>(%149));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%66), read<i32>(%67))), va_arg<i32>(%65));
// DEFAULT-NEXT:                 va_arg<i32>(%65);
// DEFAULT-NEXT:         va_end(%65);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, ptr<i32>) -> void>(%8, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%125)), array_decay<ptr<i32>, length=Some(11)>(%66));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %68 @varargs7(%69 q0: i32, %70 q1: i32, %71 q2: i32, %72 q3: i32, %73 q4: i32, %74 q5: i32, %75 q6: i32, %76 q7: i32, ...) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %77 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %78 n: array<i32, 11> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %79 i: i32 [storage=automatic];
// DEFAULT-NEXT:         va_start(%77);
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%78), const<i32>(0))), read<i32>(%69));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%78), const<i32>(1))), read<i32>(%70));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%78), const<i32>(2))), read<i32>(%71));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%78), const<i32>(3))), read<i32>(%72));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%78), const<i32>(4))), read<i32>(%73));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%78), const<i32>(5))), read<i32>(%74));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%78), const<i32>(6))), read<i32>(%75));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%78), const<i32>(7))), read<i32>(%76));
// DEFAULT-NEXT:         for %126
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%79, add<i32, overflow=ub>(const<i32>(7), const<i32>(1)));
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%79), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %150: i32 [synthetic] = read<i32>(%79);
// DEFAULT-NEXT:                 let %151: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%150), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%79, read<i32>(%151));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%78), read<i32>(%79))), va_arg<i32>(%77));
// DEFAULT-NEXT:                 va_arg<i32>(%77);
// DEFAULT-NEXT:         va_end(%77);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, ptr<i32>) -> void>(%8, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%127)), array_decay<ptr<i32>, length=Some(11)>(%78));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %80 @varargs8(%81 q0: i32, %82 q1: i32, %83 q2: i32, %84 q3: i32, %85 q4: i32, %86 q5: i32, %87 q6: i32, %88 q7: i32, %89 q8: i32, ...) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %90 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %91 n: array<i32, 11> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %92 i: i32 [storage=automatic];
// DEFAULT-NEXT:         va_start(%90);
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%91), const<i32>(0))), read<i32>(%81));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%91), const<i32>(1))), read<i32>(%82));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%91), const<i32>(2))), read<i32>(%83));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%91), const<i32>(3))), read<i32>(%84));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%91), const<i32>(4))), read<i32>(%85));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%91), const<i32>(5))), read<i32>(%86));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%91), const<i32>(6))), read<i32>(%87));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%91), const<i32>(7))), read<i32>(%88));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%91), const<i32>(8))), read<i32>(%89));
// DEFAULT-NEXT:         for %128
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%92, add<i32, overflow=ub>(const<i32>(8), const<i32>(1)));
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%92), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %152: i32 [synthetic] = read<i32>(%92);
// DEFAULT-NEXT:                 let %153: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%152), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%92, read<i32>(%153));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%91), read<i32>(%92))), va_arg<i32>(%90));
// DEFAULT-NEXT:                 va_arg<i32>(%90);
// DEFAULT-NEXT:         va_end(%90);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, ptr<i32>) -> void>(%8, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%129)), array_decay<ptr<i32>, length=Some(11)>(%91));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %93 @varargs9(%94 q0: i32, %95 q1: i32, %96 q2: i32, %97 q3: i32, %98 q4: i32, %99 q5: i32, %100 q6: i32, %101 q7: i32, %102 q8: i32, %103 q9: i32, ...) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %104 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %105 n: array<i32, 11> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %106 i: i32 [storage=automatic];
// DEFAULT-NEXT:         va_start(%104);
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%105), const<i32>(0))), read<i32>(%94));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%105), const<i32>(1))), read<i32>(%95));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%105), const<i32>(2))), read<i32>(%96));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%105), const<i32>(3))), read<i32>(%97));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%105), const<i32>(4))), read<i32>(%98));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%105), const<i32>(5))), read<i32>(%99));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%105), const<i32>(6))), read<i32>(%100));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%105), const<i32>(7))), read<i32>(%101));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%105), const<i32>(8))), read<i32>(%102));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%105), const<i32>(9))), read<i32>(%103));
// DEFAULT-NEXT:         for %130
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%106, add<i32, overflow=ub>(const<i32>(9), const<i32>(1)));
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%106), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %154: i32 [synthetic] = read<i32>(%106);
// DEFAULT-NEXT:                 let %155: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%154), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%106, read<i32>(%155));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%105), read<i32>(%106))), va_arg<i32>(%104));
// DEFAULT-NEXT:                 va_arg<i32>(%104);
// DEFAULT-NEXT:         va_end(%104);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, ptr<i32>) -> void>(%8, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%131)), array_decay<ptr<i32>, length=Some(11)>(%105));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %107 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%12, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, ...) -> void>(%17, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, ...) -> void>(%23, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, ...) -> void>(%30, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, ...) -> void>(%38, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, ...) -> void>(%47, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, ...) -> void>(%57, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32, ...) -> void>(%68, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32, i32, ...) -> void>(%80, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, ...) -> void>(%93, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%7), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%6, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
