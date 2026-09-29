/* This is a modfied version of va-arg-9.c to test va_copy.  */

#include <stdarg.h>

void abort(void);
void exit(int);

#ifndef va_copy
#define va_copy __va_copy
#endif

extern __SIZE_TYPE__ strlen(const char *);

int to_hex(unsigned int a) {
  static char hex[] = "0123456789abcdef";

  if (a > 15)
    abort();
  return hex[a];
}

void fap(int i, char *format, va_list ap) {
  va_list apc;
  char   *formatc;

  va_copy(apc, ap);
  formatc = format;

  if (strlen(format) != 16 - i)
    abort();
  while (*format)
    if (*format++ != to_hex(va_arg(ap, int)))
      abort();
  while (*formatc)
    if (*formatc++ != to_hex(va_arg(apc, int)))
      abort();
}

void f0(char *format, ...) {
  va_list ap;

  va_start(ap, format);
  fap(0, format, ap);
  va_end(ap);
}

void f1(int a1, char *format, ...) {
  va_list ap;

  va_start(ap, format);
  fap(1, format, ap);
  va_end(ap);
}

void f2(int a1, int a2, char *format, ...) {
  va_list ap;

  va_start(ap, format);
  fap(2, format, ap);
  va_end(ap);
}

void f3(int a1, int a2, int a3, char *format, ...) {
  va_list ap;

  va_start(ap, format);
  fap(3, format, ap);
  va_end(ap);
}

void f4(int a1, int a2, int a3, int a4, char *format, ...) {
  va_list ap;

  va_start(ap, format);
  fap(4, format, ap);
  va_end(ap);
}

void f5(int a1, int a2, int a3, int a4, int a5, char *format, ...) {
  va_list ap;

  va_start(ap, format);
  fap(5, format, ap);
  va_end(ap);
}

void f6(int a1, int a2, int a3, int a4, int a5, int a6, char *format, ...) {
  va_list ap;

  va_start(ap, format);
  fap(6, format, ap);
  va_end(ap);
}

void f7(int a1, int a2, int a3, int a4, int a5, int a6, int a7, char *format,
        ...) {
  va_list ap;

  va_start(ap, format);
  fap(7, format, ap);
  va_end(ap);
}

void f8(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8,
        char *format, ...) {
  va_list ap;

  va_start(ap, format);
  fap(8, format, ap);
  va_end(ap);
}

void f9(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9,
        char *format, ...) {
  va_list ap;

  va_start(ap, format);
  fap(9, format, ap);
  va_end(ap);
}

void f10(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9,
         int a10, char *format, ...) {
  va_list ap;

  va_start(ap, format);
  fap(10, format, ap);
  va_end(ap);
}

void f11(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9,
         int a10, int a11, char *format, ...) {
  va_list ap;

  va_start(ap, format);
  fap(11, format, ap);
  va_end(ap);
}

void f12(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9,
         int a10, int a11, int a12, char *format, ...) {
  va_list ap;

  va_start(ap, format);
  fap(12, format, ap);
  va_end(ap);
}

void f13(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9,
         int a10, int a11, int a12, int a13, char *format, ...) {
  va_list ap;

  va_start(ap, format);
  fap(13, format, ap);
  va_end(ap);
}

void f14(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9,
         int a10, int a11, int a12, int a13, int a14, char *format, ...) {
  va_list ap;

  va_start(ap, format);
  fap(14, format, ap);
  va_end(ap);
}

void f15(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9,
         int a10, int a11, int a12, int a13, int a14, int a15, char *format,
         ...) {
  va_list ap;

  va_start(ap, format);
  fap(15, format, ap);
  va_end(ap);
}

int main(void) {
  char *f = "0123456789abcdef";

  f0(f + 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
  f1(0, f + 1, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
  f2(0, 1, f + 2, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
  f3(0, 1, 2, f + 3, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
  f4(0, 1, 2, 3, f + 4, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
  f5(0, 1, 2, 3, 4, f + 5, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
  f6(0, 1, 2, 3, 4, 5, f + 6, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
  f7(0, 1, 2, 3, 4, 5, 6, f + 7, 7, 8, 9, 10, 11, 12, 13, 14, 15);
  f8(0, 1, 2, 3, 4, 5, 6, 7, f + 8, 8, 9, 10, 11, 12, 13, 14, 15);
  f9(0, 1, 2, 3, 4, 5, 6, 7, 8, f + 9, 9, 10, 11, 12, 13, 14, 15);
  f10(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, f + 10, 10, 11, 12, 13, 14, 15);
  f11(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, f + 11, 11, 12, 13, 14, 15);
  f12(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, f + 12, 12, 13, 14, 15);
  f13(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, f + 13, 13, 14, 15);
  f14(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, f + 14, 14, 15);
  f15(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, f + 15, 15);

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
// DEFAULT-NEXT:     global %[[VALUE_hex:[0-9]+]] hex: array<i8, 17> [storage=static] [align=16] = code_units<array<i8, 17>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 97, 98, 99, 100, 101, 102, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 17> [storage=static] = code_units<array<i8, 17>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 97, 98, 99, 100, 101, 102, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_strlen:[0-9]+]] @strlen(%[[VALUE1:[0-9]+]] <unnamed>: ptr<const i8>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_to_hex:[0-9]+]] @to_hex(%[[VALUE_a:[0-9]+]] a: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if gt<u32>(read<u32>(%[[VALUE_a]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return widen<i32, reason=return>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(17)>(%[[VALUE_hex]]), read<u32>(%[[VALUE_a]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fap:[0-9]+]] @fap(%[[VALUE_i:[0-9]+]] i: i32, %[[VALUE_format:[0-9]+]] format: ptr<i8>, %[[VALUE_ap:[0-9]+]] ap: va_list) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_apc:[0-9]+]] apc: va_list [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_formatc:[0-9]+]] formatc: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         va_copy(%[[VALUE_apc]], %[[VALUE_ap]]);
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_formatc]], read<ptr<i8>>(%[[VALUE_format]]));
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_format]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(const<i32>(16), read<i32>(%[[VALUE_i]])))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         while %[[VALUE2:[0-9]+]] ne<i8>(read<i8>(deref(read<ptr<i8>>(%[[VALUE_format]]))), const<i8>(0))
// DEFAULT-NEXT:             let %[[VALUE3:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_format]]);
// DEFAULT-NEXT:             let %[[VALUE4:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE3]]), const<i32>(1));
// DEFAULT-NEXT:             write<ptr<i8>>(%[[VALUE_format]], read<ptr<i8>>(%[[VALUE4]]));
// DEFAULT-NEXT:             if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%[[VALUE3]])))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_to_hex]], reinterpret<u32, reason=arg, fits=unknown>(va_arg<i32>(%[[VALUE_ap]]))))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         while %[[VALUE5:[0-9]+]] ne<i8>(read<i8>(deref(read<ptr<i8>>(%[[VALUE_formatc]]))), const<i8>(0))
// DEFAULT-NEXT:             let %[[VALUE6:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_formatc]]);
// DEFAULT-NEXT:             let %[[VALUE7:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE6]]), const<i32>(1));
// DEFAULT-NEXT:             write<ptr<i8>>(%[[VALUE_formatc]], read<ptr<i8>>(%[[VALUE7]]));
// DEFAULT-NEXT:             if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%[[VALUE6]])))), call<i32, signature=fn(u32) -> i32>(%[[VALUE_to_hex]], reinterpret<u32, reason=arg, fits=unknown>(va_arg<i32>(%[[VALUE_apc]]))))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f0:[0-9]+]] @f0(%[[VALUE_format_2:[0-9]+]] format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_2:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_2]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%[[VALUE_fap]], const<i32>(0), read<ptr<i8>>(%[[VALUE_format_2]]), read<va_list>(%[[VALUE_ap_2]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f1:[0-9]+]] @f1(%[[VALUE_a1:[0-9]+]] a1: i32, %[[VALUE_format_3:[0-9]+]] format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_3:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_3]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%[[VALUE_fap]], const<i32>(1), read<ptr<i8>>(%[[VALUE_format_3]]), read<va_list>(%[[VALUE_ap_3]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_3]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f2:[0-9]+]] @f2(%[[VALUE_a1_2:[0-9]+]] a1: i32, %[[VALUE_a2:[0-9]+]] a2: i32, %[[VALUE_format_4:[0-9]+]] format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_4:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_4]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%[[VALUE_fap]], const<i32>(2), read<ptr<i8>>(%[[VALUE_format_4]]), read<va_list>(%[[VALUE_ap_4]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_4]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f3:[0-9]+]] @f3(%[[VALUE_a1_3:[0-9]+]] a1: i32, %[[VALUE_a2_2:[0-9]+]] a2: i32, %[[VALUE_a3:[0-9]+]] a3: i32, %[[VALUE_format_5:[0-9]+]] format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_5:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_5]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%[[VALUE_fap]], const<i32>(3), read<ptr<i8>>(%[[VALUE_format_5]]), read<va_list>(%[[VALUE_ap_5]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_5]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f4:[0-9]+]] @f4(%[[VALUE_a1_4:[0-9]+]] a1: i32, %[[VALUE_a2_3:[0-9]+]] a2: i32, %[[VALUE_a3_2:[0-9]+]] a3: i32, %[[VALUE_a4:[0-9]+]] a4: i32, %[[VALUE_format_6:[0-9]+]] format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_6:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_6]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%[[VALUE_fap]], const<i32>(4), read<ptr<i8>>(%[[VALUE_format_6]]), read<va_list>(%[[VALUE_ap_6]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_6]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f5:[0-9]+]] @f5(%[[VALUE_a1_5:[0-9]+]] a1: i32, %[[VALUE_a2_4:[0-9]+]] a2: i32, %[[VALUE_a3_3:[0-9]+]] a3: i32, %[[VALUE_a4_2:[0-9]+]] a4: i32, %[[VALUE_a5:[0-9]+]] a5: i32, %[[VALUE_format_7:[0-9]+]] format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_7:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_7]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%[[VALUE_fap]], const<i32>(5), read<ptr<i8>>(%[[VALUE_format_7]]), read<va_list>(%[[VALUE_ap_7]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_7]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f6:[0-9]+]] @f6(%[[VALUE_a1_6:[0-9]+]] a1: i32, %[[VALUE_a2_5:[0-9]+]] a2: i32, %[[VALUE_a3_4:[0-9]+]] a3: i32, %[[VALUE_a4_3:[0-9]+]] a4: i32, %[[VALUE_a5_2:[0-9]+]] a5: i32, %[[VALUE_a6:[0-9]+]] a6: i32, %[[VALUE_format_8:[0-9]+]] format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_8:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_8]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%[[VALUE_fap]], const<i32>(6), read<ptr<i8>>(%[[VALUE_format_8]]), read<va_list>(%[[VALUE_ap_8]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_8]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f7:[0-9]+]] @f7(%[[VALUE_a1_7:[0-9]+]] a1: i32, %[[VALUE_a2_6:[0-9]+]] a2: i32, %[[VALUE_a3_5:[0-9]+]] a3: i32, %[[VALUE_a4_4:[0-9]+]] a4: i32, %[[VALUE_a5_3:[0-9]+]] a5: i32, %[[VALUE_a6_2:[0-9]+]] a6: i32, %[[VALUE_a7:[0-9]+]] a7: i32, %[[VALUE_format_9:[0-9]+]] format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_9:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_9]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%[[VALUE_fap]], const<i32>(7), read<ptr<i8>>(%[[VALUE_format_9]]), read<va_list>(%[[VALUE_ap_9]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_9]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f8:[0-9]+]] @f8(%[[VALUE_a1_8:[0-9]+]] a1: i32, %[[VALUE_a2_7:[0-9]+]] a2: i32, %[[VALUE_a3_6:[0-9]+]] a3: i32, %[[VALUE_a4_5:[0-9]+]] a4: i32, %[[VALUE_a5_4:[0-9]+]] a5: i32, %[[VALUE_a6_3:[0-9]+]] a6: i32, %[[VALUE_a7_2:[0-9]+]] a7: i32, %[[VALUE_a8:[0-9]+]] a8: i32, %[[VALUE_format_10:[0-9]+]] format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_10:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_10]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%[[VALUE_fap]], const<i32>(8), read<ptr<i8>>(%[[VALUE_format_10]]), read<va_list>(%[[VALUE_ap_10]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_10]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f9:[0-9]+]] @f9(%[[VALUE_a1_9:[0-9]+]] a1: i32, %[[VALUE_a2_8:[0-9]+]] a2: i32, %[[VALUE_a3_7:[0-9]+]] a3: i32, %[[VALUE_a4_6:[0-9]+]] a4: i32, %[[VALUE_a5_5:[0-9]+]] a5: i32, %[[VALUE_a6_4:[0-9]+]] a6: i32, %[[VALUE_a7_3:[0-9]+]] a7: i32, %[[VALUE_a8_2:[0-9]+]] a8: i32, %[[VALUE_a9:[0-9]+]] a9: i32, %[[VALUE_format_11:[0-9]+]] format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_11:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_11]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%[[VALUE_fap]], const<i32>(9), read<ptr<i8>>(%[[VALUE_format_11]]), read<va_list>(%[[VALUE_ap_11]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_11]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f10:[0-9]+]] @f10(%[[VALUE_a1_10:[0-9]+]] a1: i32, %[[VALUE_a2_9:[0-9]+]] a2: i32, %[[VALUE_a3_8:[0-9]+]] a3: i32, %[[VALUE_a4_7:[0-9]+]] a4: i32, %[[VALUE_a5_6:[0-9]+]] a5: i32, %[[VALUE_a6_5:[0-9]+]] a6: i32, %[[VALUE_a7_4:[0-9]+]] a7: i32, %[[VALUE_a8_3:[0-9]+]] a8: i32, %[[VALUE_a9_2:[0-9]+]] a9: i32, %[[VALUE_a10:[0-9]+]] a10: i32, %[[VALUE_format_12:[0-9]+]] format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_12:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_12]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%[[VALUE_fap]], const<i32>(10), read<ptr<i8>>(%[[VALUE_format_12]]), read<va_list>(%[[VALUE_ap_12]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_12]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f11:[0-9]+]] @f11(%[[VALUE_a1_11:[0-9]+]] a1: i32, %[[VALUE_a2_10:[0-9]+]] a2: i32, %[[VALUE_a3_9:[0-9]+]] a3: i32, %[[VALUE_a4_8:[0-9]+]] a4: i32, %[[VALUE_a5_7:[0-9]+]] a5: i32, %[[VALUE_a6_6:[0-9]+]] a6: i32, %[[VALUE_a7_5:[0-9]+]] a7: i32, %[[VALUE_a8_4:[0-9]+]] a8: i32, %[[VALUE_a9_3:[0-9]+]] a9: i32, %[[VALUE_a10_2:[0-9]+]] a10: i32, %[[VALUE_a11:[0-9]+]] a11: i32, %[[VALUE_format_13:[0-9]+]] format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_13:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_13]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%[[VALUE_fap]], const<i32>(11), read<ptr<i8>>(%[[VALUE_format_13]]), read<va_list>(%[[VALUE_ap_13]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_13]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f12:[0-9]+]] @f12(%[[VALUE_a1_12:[0-9]+]] a1: i32, %[[VALUE_a2_11:[0-9]+]] a2: i32, %[[VALUE_a3_10:[0-9]+]] a3: i32, %[[VALUE_a4_9:[0-9]+]] a4: i32, %[[VALUE_a5_8:[0-9]+]] a5: i32, %[[VALUE_a6_7:[0-9]+]] a6: i32, %[[VALUE_a7_6:[0-9]+]] a7: i32, %[[VALUE_a8_5:[0-9]+]] a8: i32, %[[VALUE_a9_4:[0-9]+]] a9: i32, %[[VALUE_a10_3:[0-9]+]] a10: i32, %[[VALUE_a11_2:[0-9]+]] a11: i32, %[[VALUE_a12:[0-9]+]] a12: i32, %[[VALUE_format_14:[0-9]+]] format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_14:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_14]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%[[VALUE_fap]], const<i32>(12), read<ptr<i8>>(%[[VALUE_format_14]]), read<va_list>(%[[VALUE_ap_14]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_14]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f13:[0-9]+]] @f13(%[[VALUE_a1_13:[0-9]+]] a1: i32, %[[VALUE_a2_12:[0-9]+]] a2: i32, %[[VALUE_a3_11:[0-9]+]] a3: i32, %[[VALUE_a4_10:[0-9]+]] a4: i32, %[[VALUE_a5_9:[0-9]+]] a5: i32, %[[VALUE_a6_8:[0-9]+]] a6: i32, %[[VALUE_a7_7:[0-9]+]] a7: i32, %[[VALUE_a8_6:[0-9]+]] a8: i32, %[[VALUE_a9_5:[0-9]+]] a9: i32, %[[VALUE_a10_4:[0-9]+]] a10: i32, %[[VALUE_a11_3:[0-9]+]] a11: i32, %[[VALUE_a12_2:[0-9]+]] a12: i32, %[[VALUE_a13:[0-9]+]] a13: i32, %[[VALUE_format_15:[0-9]+]] format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_15:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_15]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%[[VALUE_fap]], const<i32>(13), read<ptr<i8>>(%[[VALUE_format_15]]), read<va_list>(%[[VALUE_ap_15]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_15]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f14:[0-9]+]] @f14(%[[VALUE_a1_14:[0-9]+]] a1: i32, %[[VALUE_a2_13:[0-9]+]] a2: i32, %[[VALUE_a3_12:[0-9]+]] a3: i32, %[[VALUE_a4_11:[0-9]+]] a4: i32, %[[VALUE_a5_10:[0-9]+]] a5: i32, %[[VALUE_a6_9:[0-9]+]] a6: i32, %[[VALUE_a7_8:[0-9]+]] a7: i32, %[[VALUE_a8_7:[0-9]+]] a8: i32, %[[VALUE_a9_6:[0-9]+]] a9: i32, %[[VALUE_a10_5:[0-9]+]] a10: i32, %[[VALUE_a11_4:[0-9]+]] a11: i32, %[[VALUE_a12_3:[0-9]+]] a12: i32, %[[VALUE_a13_2:[0-9]+]] a13: i32, %[[VALUE_a14:[0-9]+]] a14: i32, %[[VALUE_format_16:[0-9]+]] format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_16:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_16]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%[[VALUE_fap]], const<i32>(14), read<ptr<i8>>(%[[VALUE_format_16]]), read<va_list>(%[[VALUE_ap_16]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_16]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f15:[0-9]+]] @f15(%[[VALUE_a1_15:[0-9]+]] a1: i32, %[[VALUE_a2_14:[0-9]+]] a2: i32, %[[VALUE_a3_13:[0-9]+]] a3: i32, %[[VALUE_a4_12:[0-9]+]] a4: i32, %[[VALUE_a5_11:[0-9]+]] a5: i32, %[[VALUE_a6_10:[0-9]+]] a6: i32, %[[VALUE_a7_9:[0-9]+]] a7: i32, %[[VALUE_a8_8:[0-9]+]] a8: i32, %[[VALUE_a9_7:[0-9]+]] a9: i32, %[[VALUE_a10_6:[0-9]+]] a10: i32, %[[VALUE_a11_5:[0-9]+]] a11: i32, %[[VALUE_a12_4:[0-9]+]] a12: i32, %[[VALUE_a13_3:[0-9]+]] a13: i32, %[[VALUE_a14_2:[0-9]+]] a14: i32, %[[VALUE_a15:[0-9]+]] a15: i32, %[[VALUE_format_17:[0-9]+]] format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_17:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_17]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%[[VALUE_fap]], const<i32>(15), read<ptr<i8>>(%[[VALUE_format_17]]), read<va_list>(%[[VALUE_ap_17]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_17]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_f:[0-9]+]] f: ptr<i8> [storage=automatic] = array_decay<ptr<i8>, length=Some(17)>(%[[VALUE_str]]);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i8>, ...) -> void>(%[[VALUE_f0]], ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_f]]), const<i32>(0)), const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, ...) -> void>(%[[VALUE_f1]], const<i32>(0), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_f]]), const<i32>(1)), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, ptr<i8>, ...) -> void>(%[[VALUE_f2]], const<i32>(0), const<i32>(1), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_f]]), const<i32>(2)), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, ptr<i8>, ...) -> void>(%[[VALUE_f3]], const<i32>(0), const<i32>(1), const<i32>(2), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_f]]), const<i32>(3)), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, ptr<i8>, ...) -> void>(%[[VALUE_f4]], const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_f]]), const<i32>(4)), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, ptr<i8>, ...) -> void>(%[[VALUE_f5]], const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_f]]), const<i32>(5)), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, ptr<i8>, ...) -> void>(%[[VALUE_f6]], const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_f]]), const<i32>(6)), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, ptr<i8>, ...) -> void>(%[[VALUE_f7]], const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_f]]), const<i32>(7)), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32, ptr<i8>, ...) -> void>(%[[VALUE_f8]], const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_f]]), const<i32>(8)), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32, i32, ptr<i8>, ...) -> void>(%[[VALUE_f9]], const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_f]]), const<i32>(9)), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, ptr<i8>, ...) -> void>(%[[VALUE_f10]], const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_f]]), const<i32>(10)), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, ptr<i8>, ...) -> void>(%[[VALUE_f11]], const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_f]]), const<i32>(11)), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, ptr<i8>, ...) -> void>(%[[VALUE_f12]], const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_f]]), const<i32>(12)), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, ptr<i8>, ...) -> void>(%[[VALUE_f13]], const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_f]]), const<i32>(13)), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, ptr<i8>, ...) -> void>(%[[VALUE_f14]], const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_f]]), const<i32>(14)), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, ptr<i8>, ...) -> void>(%[[VALUE_f15]], const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_f]]), const<i32>(15)), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
