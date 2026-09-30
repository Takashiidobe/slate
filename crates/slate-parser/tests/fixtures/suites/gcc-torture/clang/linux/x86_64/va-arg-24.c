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
// DEFAULT-NEXT:     type @type[[TYPE___gnuc_va_list:[0-9]+]] __gnuc_va_list = va_list;
// DEFAULT-NEXT:     type @type[[TYPE_va_list:[0-9]+]] va_list = va_list;
// DEFAULT-NEXT:     type @type[[TYPE_va_list_2:[0-9]+]] va_list = va_list;
// DEFAULT-NEXT:     global %[[VALUE_errors:[0-9]+]] errors: i32 [storage=static] = const<i32>(0) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 29> [storage=static] = code_units<array<i8, 29>>([32, 37, 115, 58, 32, 110, 91, 37, 100, 93, 32, 61, 32, 37, 100, 32, 101, 120, 112, 101, 99, 116, 101, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([118, 97, 114, 97, 114, 103, 115, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([118, 97, 114, 97, 114, 103, 115, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([118, 97, 114, 97, 114, 103, 115, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([118, 97, 114, 97, 114, 103, 115, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_6:[0-9]+]] .str[[VALUE_str_6]]: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([118, 97, 114, 97, 114, 103, 115, 52, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_7:[0-9]+]] .str[[VALUE_str_7]]: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([118, 97, 114, 97, 114, 103, 115, 53, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_8:[0-9]+]] .str[[VALUE_str_8]]: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([118, 97, 114, 97, 114, 103, 115, 54, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_9:[0-9]+]] .str[[VALUE_str_9]]: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([118, 97, 114, 97, 114, 103, 115, 55, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_10:[0-9]+]] .str[[VALUE_str_10]]: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([118, 97, 114, 97, 114, 103, 115, 56, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_11:[0-9]+]] .str[[VALUE_str_11]]: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([118, 97, 114, 97, 114, 103, 115, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE___status:[0-9]+]] __status: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_verify:[0-9]+]] @verify(%[[VALUE_tcase:[0-9]+]] tcase: ptr<const i8>, %[[VALUE_n:[0-9]+]] n: ptr<i32> [array=11]) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%[[VALUE_i]]), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_n]]), read<i32>(%[[VALUE_i]])))), read<i32>(%[[VALUE_i]]))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(29)>(%[[VALUE_str]])), read<ptr<const i8>>(%[[VALUE_tcase]]), read<i32>(%[[VALUE_i]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_n]]), read<i32>(%[[VALUE_i]])))), read<i32>(%[[VALUE_i]]));
// DEFAULT-NEXT:                         let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_errors]]);
// DEFAULT-NEXT:                         let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE3]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_errors]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_varargs0:[0-9]+]] @varargs0(%[[VALUE_q0:[0-9]+]] q0: i32, ...) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_n_2:[0-9]+]] n: array<i32, 11> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_i_2:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap]]);
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_2]]), const<i32>(0))), read<i32>(%[[VALUE_q0]]));
// DEFAULT-NEXT:         for %[[VALUE5:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], add<i32, overflow=ub>(const<i32>(0), const<i32>(1)));
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE6]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE7]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_2]]), read<i32>(%[[VALUE_i_2]]))), va_arg<i32>(%[[VALUE_ap]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap]]);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, ptr<i32>) -> void>(%[[VALUE_verify]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%[[VALUE_str_2]])), array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_varargs1:[0-9]+]] @varargs1(%[[VALUE_q0_2:[0-9]+]] q0: i32, %[[VALUE_q1:[0-9]+]] q1: i32, ...) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_2:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_n_3:[0-9]+]] n: array<i32, 11> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_i_3:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_2]]);
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_3]]), const<i32>(0))), read<i32>(%[[VALUE_q0_2]]));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_3]]), const<i32>(1))), read<i32>(%[[VALUE_q1]]));
// DEFAULT-NEXT:         for %[[VALUE8:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_3]], add<i32, overflow=ub>(const<i32>(1), const<i32>(1)));
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%[[VALUE_i_3]]), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE9:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_3]]);
// DEFAULT-NEXT:                 let %[[VALUE10:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE9]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_3]], read<i32>(%[[VALUE10]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_3]]), read<i32>(%[[VALUE_i_3]]))), va_arg<i32>(%[[VALUE_ap_2]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_2]]);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, ptr<i32>) -> void>(%[[VALUE_verify]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%[[VALUE_str_3]])), array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_varargs2:[0-9]+]] @varargs2(%[[VALUE_q0_3:[0-9]+]] q0: i32, %[[VALUE_q1_2:[0-9]+]] q1: i32, %[[VALUE_q2:[0-9]+]] q2: i32, ...) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_3:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_n_4:[0-9]+]] n: array<i32, 11> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_i_4:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_3]]);
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_4]]), const<i32>(0))), read<i32>(%[[VALUE_q0_3]]));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_4]]), const<i32>(1))), read<i32>(%[[VALUE_q1_2]]));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_4]]), const<i32>(2))), read<i32>(%[[VALUE_q2]]));
// DEFAULT-NEXT:         for %[[VALUE11:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_4]], add<i32, overflow=ub>(const<i32>(2), const<i32>(1)));
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%[[VALUE_i_4]]), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE12:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_4]]);
// DEFAULT-NEXT:                 let %[[VALUE13:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE12]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_4]], read<i32>(%[[VALUE13]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_4]]), read<i32>(%[[VALUE_i_4]]))), va_arg<i32>(%[[VALUE_ap_3]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_3]]);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, ptr<i32>) -> void>(%[[VALUE_verify]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%[[VALUE_str_4]])), array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_4]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_varargs3:[0-9]+]] @varargs3(%[[VALUE_q0_4:[0-9]+]] q0: i32, %[[VALUE_q1_3:[0-9]+]] q1: i32, %[[VALUE_q2_2:[0-9]+]] q2: i32, %[[VALUE_q3:[0-9]+]] q3: i32, ...) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_4:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_n_5:[0-9]+]] n: array<i32, 11> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_i_5:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_4]]);
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_5]]), const<i32>(0))), read<i32>(%[[VALUE_q0_4]]));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_5]]), const<i32>(1))), read<i32>(%[[VALUE_q1_3]]));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_5]]), const<i32>(2))), read<i32>(%[[VALUE_q2_2]]));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_5]]), const<i32>(3))), read<i32>(%[[VALUE_q3]]));
// DEFAULT-NEXT:         for %[[VALUE14:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_5]], add<i32, overflow=ub>(const<i32>(3), const<i32>(1)));
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%[[VALUE_i_5]]), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE15:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_5]]);
// DEFAULT-NEXT:                 let %[[VALUE16:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE15]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_5]], read<i32>(%[[VALUE16]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_5]]), read<i32>(%[[VALUE_i_5]]))), va_arg<i32>(%[[VALUE_ap_4]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_4]]);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, ptr<i32>) -> void>(%[[VALUE_verify]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%[[VALUE_str_5]])), array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_5]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_varargs4:[0-9]+]] @varargs4(%[[VALUE_q0_5:[0-9]+]] q0: i32, %[[VALUE_q1_4:[0-9]+]] q1: i32, %[[VALUE_q2_3:[0-9]+]] q2: i32, %[[VALUE_q3_2:[0-9]+]] q3: i32, %[[VALUE_q4:[0-9]+]] q4: i32, ...) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_5:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_n_6:[0-9]+]] n: array<i32, 11> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_i_6:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_5]]);
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_6]]), const<i32>(0))), read<i32>(%[[VALUE_q0_5]]));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_6]]), const<i32>(1))), read<i32>(%[[VALUE_q1_4]]));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_6]]), const<i32>(2))), read<i32>(%[[VALUE_q2_3]]));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_6]]), const<i32>(3))), read<i32>(%[[VALUE_q3_2]]));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_6]]), const<i32>(4))), read<i32>(%[[VALUE_q4]]));
// DEFAULT-NEXT:         for %[[VALUE17:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_6]], add<i32, overflow=ub>(const<i32>(4), const<i32>(1)));
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%[[VALUE_i_6]]), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE18:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_6]]);
// DEFAULT-NEXT:                 let %[[VALUE19:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE18]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_6]], read<i32>(%[[VALUE19]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_6]]), read<i32>(%[[VALUE_i_6]]))), va_arg<i32>(%[[VALUE_ap_5]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_5]]);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, ptr<i32>) -> void>(%[[VALUE_verify]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%[[VALUE_str_6]])), array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_6]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_varargs5:[0-9]+]] @varargs5(%[[VALUE_q0_6:[0-9]+]] q0: i32, %[[VALUE_q1_5:[0-9]+]] q1: i32, %[[VALUE_q2_4:[0-9]+]] q2: i32, %[[VALUE_q3_3:[0-9]+]] q3: i32, %[[VALUE_q4_2:[0-9]+]] q4: i32, %[[VALUE_q5:[0-9]+]] q5: i32, ...) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_6:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_n_7:[0-9]+]] n: array<i32, 11> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_i_7:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_6]]);
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_7]]), const<i32>(0))), read<i32>(%[[VALUE_q0_6]]));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_7]]), const<i32>(1))), read<i32>(%[[VALUE_q1_5]]));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_7]]), const<i32>(2))), read<i32>(%[[VALUE_q2_4]]));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_7]]), const<i32>(3))), read<i32>(%[[VALUE_q3_3]]));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_7]]), const<i32>(4))), read<i32>(%[[VALUE_q4_2]]));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_7]]), const<i32>(5))), read<i32>(%[[VALUE_q5]]));
// DEFAULT-NEXT:         for %[[VALUE20:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_7]], add<i32, overflow=ub>(const<i32>(5), const<i32>(1)));
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%[[VALUE_i_7]]), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE21:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_7]]);
// DEFAULT-NEXT:                 let %[[VALUE22:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE21]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_7]], read<i32>(%[[VALUE22]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_7]]), read<i32>(%[[VALUE_i_7]]))), va_arg<i32>(%[[VALUE_ap_6]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_6]]);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, ptr<i32>) -> void>(%[[VALUE_verify]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%[[VALUE_str_7]])), array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_7]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_varargs6:[0-9]+]] @varargs6(%[[VALUE_q0_7:[0-9]+]] q0: i32, %[[VALUE_q1_6:[0-9]+]] q1: i32, %[[VALUE_q2_5:[0-9]+]] q2: i32, %[[VALUE_q3_4:[0-9]+]] q3: i32, %[[VALUE_q4_3:[0-9]+]] q4: i32, %[[VALUE_q5_2:[0-9]+]] q5: i32, %[[VALUE_q6:[0-9]+]] q6: i32, ...) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_7:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_n_8:[0-9]+]] n: array<i32, 11> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_i_8:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_7]]);
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_8]]), const<i32>(0))), read<i32>(%[[VALUE_q0_7]]));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_8]]), const<i32>(1))), read<i32>(%[[VALUE_q1_6]]));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_8]]), const<i32>(2))), read<i32>(%[[VALUE_q2_5]]));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_8]]), const<i32>(3))), read<i32>(%[[VALUE_q3_4]]));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_8]]), const<i32>(4))), read<i32>(%[[VALUE_q4_3]]));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_8]]), const<i32>(5))), read<i32>(%[[VALUE_q5_2]]));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_8]]), const<i32>(6))), read<i32>(%[[VALUE_q6]]));
// DEFAULT-NEXT:         for %[[VALUE23:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_8]], add<i32, overflow=ub>(const<i32>(6), const<i32>(1)));
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%[[VALUE_i_8]]), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE24:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_8]]);
// DEFAULT-NEXT:                 let %[[VALUE25:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE24]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_8]], read<i32>(%[[VALUE25]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_8]]), read<i32>(%[[VALUE_i_8]]))), va_arg<i32>(%[[VALUE_ap_7]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_7]]);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, ptr<i32>) -> void>(%[[VALUE_verify]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%[[VALUE_str_8]])), array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_8]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_varargs7:[0-9]+]] @varargs7(%[[VALUE_q0_8:[0-9]+]] q0: i32, %[[VALUE_q1_7:[0-9]+]] q1: i32, %[[VALUE_q2_6:[0-9]+]] q2: i32, %[[VALUE_q3_5:[0-9]+]] q3: i32, %[[VALUE_q4_4:[0-9]+]] q4: i32, %[[VALUE_q5_3:[0-9]+]] q5: i32, %[[VALUE_q6_2:[0-9]+]] q6: i32, %[[VALUE_q7:[0-9]+]] q7: i32, ...) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_8:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_n_9:[0-9]+]] n: array<i32, 11> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_i_9:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_8]]);
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_9]]), const<i32>(0))), read<i32>(%[[VALUE_q0_8]]));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_9]]), const<i32>(1))), read<i32>(%[[VALUE_q1_7]]));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_9]]), const<i32>(2))), read<i32>(%[[VALUE_q2_6]]));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_9]]), const<i32>(3))), read<i32>(%[[VALUE_q3_5]]));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_9]]), const<i32>(4))), read<i32>(%[[VALUE_q4_4]]));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_9]]), const<i32>(5))), read<i32>(%[[VALUE_q5_3]]));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_9]]), const<i32>(6))), read<i32>(%[[VALUE_q6_2]]));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_9]]), const<i32>(7))), read<i32>(%[[VALUE_q7]]));
// DEFAULT-NEXT:         for %[[VALUE26:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_9]], add<i32, overflow=ub>(const<i32>(7), const<i32>(1)));
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%[[VALUE_i_9]]), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE27:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_9]]);
// DEFAULT-NEXT:                 let %[[VALUE28:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE27]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_9]], read<i32>(%[[VALUE28]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_9]]), read<i32>(%[[VALUE_i_9]]))), va_arg<i32>(%[[VALUE_ap_8]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_8]]);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, ptr<i32>) -> void>(%[[VALUE_verify]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%[[VALUE_str_9]])), array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_9]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_varargs8:[0-9]+]] @varargs8(%[[VALUE_q0_9:[0-9]+]] q0: i32, %[[VALUE_q1_8:[0-9]+]] q1: i32, %[[VALUE_q2_7:[0-9]+]] q2: i32, %[[VALUE_q3_6:[0-9]+]] q3: i32, %[[VALUE_q4_5:[0-9]+]] q4: i32, %[[VALUE_q5_4:[0-9]+]] q5: i32, %[[VALUE_q6_3:[0-9]+]] q6: i32, %[[VALUE_q7_2:[0-9]+]] q7: i32, %[[VALUE_q8:[0-9]+]] q8: i32, ...) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_9:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_n_10:[0-9]+]] n: array<i32, 11> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_i_10:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_9]]);
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_10]]), const<i32>(0))), read<i32>(%[[VALUE_q0_9]]));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_10]]), const<i32>(1))), read<i32>(%[[VALUE_q1_8]]));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_10]]), const<i32>(2))), read<i32>(%[[VALUE_q2_7]]));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_10]]), const<i32>(3))), read<i32>(%[[VALUE_q3_6]]));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_10]]), const<i32>(4))), read<i32>(%[[VALUE_q4_5]]));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_10]]), const<i32>(5))), read<i32>(%[[VALUE_q5_4]]));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_10]]), const<i32>(6))), read<i32>(%[[VALUE_q6_3]]));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_10]]), const<i32>(7))), read<i32>(%[[VALUE_q7_2]]));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_10]]), const<i32>(8))), read<i32>(%[[VALUE_q8]]));
// DEFAULT-NEXT:         for %[[VALUE29:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_10]], add<i32, overflow=ub>(const<i32>(8), const<i32>(1)));
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%[[VALUE_i_10]]), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE30:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_10]]);
// DEFAULT-NEXT:                 let %[[VALUE31:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE30]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_10]], read<i32>(%[[VALUE31]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_10]]), read<i32>(%[[VALUE_i_10]]))), va_arg<i32>(%[[VALUE_ap_9]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_9]]);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, ptr<i32>) -> void>(%[[VALUE_verify]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%[[VALUE_str_10]])), array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_10]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_varargs9:[0-9]+]] @varargs9(%[[VALUE_q0_10:[0-9]+]] q0: i32, %[[VALUE_q1_9:[0-9]+]] q1: i32, %[[VALUE_q2_8:[0-9]+]] q2: i32, %[[VALUE_q3_7:[0-9]+]] q3: i32, %[[VALUE_q4_6:[0-9]+]] q4: i32, %[[VALUE_q5_5:[0-9]+]] q5: i32, %[[VALUE_q6_4:[0-9]+]] q6: i32, %[[VALUE_q7_3:[0-9]+]] q7: i32, %[[VALUE_q8_2:[0-9]+]] q8: i32, %[[VALUE_q9:[0-9]+]] q9: i32, ...) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_10:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_n_11:[0-9]+]] n: array<i32, 11> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_i_11:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_10]]);
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_11]]), const<i32>(0))), read<i32>(%[[VALUE_q0_10]]));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_11]]), const<i32>(1))), read<i32>(%[[VALUE_q1_9]]));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_11]]), const<i32>(2))), read<i32>(%[[VALUE_q2_8]]));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_11]]), const<i32>(3))), read<i32>(%[[VALUE_q3_7]]));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_11]]), const<i32>(4))), read<i32>(%[[VALUE_q4_6]]));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_11]]), const<i32>(5))), read<i32>(%[[VALUE_q5_5]]));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_11]]), const<i32>(6))), read<i32>(%[[VALUE_q6_4]]));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_11]]), const<i32>(7))), read<i32>(%[[VALUE_q7_3]]));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_11]]), const<i32>(8))), read<i32>(%[[VALUE_q8_2]]));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_11]]), const<i32>(9))), read<i32>(%[[VALUE_q9]]));
// DEFAULT-NEXT:         for %[[VALUE32:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_11]], add<i32, overflow=ub>(const<i32>(9), const<i32>(1)));
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%[[VALUE_i_11]]), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE33:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_11]]);
// DEFAULT-NEXT:                 let %[[VALUE34:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE33]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_11]], read<i32>(%[[VALUE34]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_11]]), read<i32>(%[[VALUE_i_11]]))), va_arg<i32>(%[[VALUE_ap_10]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_10]]);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, ptr<i32>) -> void>(%[[VALUE_verify]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%[[VALUE_str_11]])), array_decay<ptr<i32>, length=Some(11)>(%[[VALUE_n_11]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%[[VALUE_varargs0]], const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, ...) -> void>(%[[VALUE_varargs1]], const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, ...) -> void>(%[[VALUE_varargs2]], const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, ...) -> void>(%[[VALUE_varargs3]], const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, ...) -> void>(%[[VALUE_varargs4]], const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, ...) -> void>(%[[VALUE_varargs5]], const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, ...) -> void>(%[[VALUE_varargs6]], const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32, ...) -> void>(%[[VALUE_varargs7]], const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32, i32, ...) -> void>(%[[VALUE_varargs8]], const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, ...) -> void>(%[[VALUE_varargs9]], const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_errors]]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
