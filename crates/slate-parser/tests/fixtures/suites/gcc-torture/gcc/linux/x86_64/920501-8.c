/* { dg-additional-options "-Wl,-u,_printf_float" { target newlib_nano_io } } */

void abort(void);
void exit(int);

#include <stdarg.h>
#include <stdio.h>

char buf[50];
int  va(int a, double b, int c, ...) {
  va_list ap;
  int     d, e, f, g, h, i, j, k, l, m, n, o, p;
  va_start(ap, c);

  d = va_arg(ap, int);
  e = va_arg(ap, int);
  f = va_arg(ap, int);
  g = va_arg(ap, int);
  h = va_arg(ap, int);
  i = va_arg(ap, int);
  j = va_arg(ap, int);
  k = va_arg(ap, int);
  l = va_arg(ap, int);
  m = va_arg(ap, int);
  n = va_arg(ap, int);
  o = va_arg(ap, int);
  p = va_arg(ap, int);

  sprintf(buf, "%d,%f,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", a, b, c, d, e,
          f, g, h, i, j, k, l, m, n, o, p);
  va_end(ap);
}

int main(void) {
  va(1, 1.0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
  if (__builtin_strcmp("1,1.000000,2,3,4,5,6,7,8,9,10,11,12,13,14,15", buf))
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
// DEFAULT-NEXT:     global %[[VALUE_buf:[0-9]+]] buf: array<i8, 50> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 48> [storage=static] = code_units<array<i8, 48>>([37, 100, 44, 37, 102, 44, 37, 100, 44, 37, 100, 44, 37, 100, 44, 37, 100, 44, 37, 100, 44, 37, 100, 44, 37, 100, 44, 37, 100, 44, 37, 100, 44, 37, 100, 44, 37, 100, 44, 37, 100, 44, 37, 100, 44, 37, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 45> [storage=static] = code_units<array<i8, 45>>([49, 44, 49, 46, 48, 48, 48, 48, 48, 48, 44, 50, 44, 51, 44, 52, 44, 53, 44, 54, 44, 55, 44, 56, 44, 57, 44, 49, 48, 44, 49, 49, 44, 49, 50, 44, 49, 51, 44, 49, 52, 44, 49, 53, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_sprintf:[0-9]+]] @sprintf(%[[VALUE___s:[0-9]+]] __s: ptr<i8> [restrict], %[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_va:[0-9]+]] @va(%[[VALUE_a:[0-9]+]] a: i32, %[[VALUE_b:[0-9]+]] b: f64, %[[VALUE_c:[0-9]+]] c: i32, ...) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_ap:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_d:[0-9]+]] d: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_e:[0-9]+]] e: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_f:[0-9]+]] f: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_g:[0-9]+]] g: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_h:[0-9]+]] h: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_k:[0-9]+]] k: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_l:[0-9]+]] l: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_m:[0-9]+]] m: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_n:[0-9]+]] n: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_o:[0-9]+]] o: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: i32 [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_d]], va_arg<i32>(%[[VALUE_ap]]));
// DEFAULT-NEXT:         va_arg<i32>(%[[VALUE_ap]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_e]], va_arg<i32>(%[[VALUE_ap]]));
// DEFAULT-NEXT:         va_arg<i32>(%[[VALUE_ap]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_f]], va_arg<i32>(%[[VALUE_ap]]));
// DEFAULT-NEXT:         va_arg<i32>(%[[VALUE_ap]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_g]], va_arg<i32>(%[[VALUE_ap]]));
// DEFAULT-NEXT:         va_arg<i32>(%[[VALUE_ap]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_h]], va_arg<i32>(%[[VALUE_ap]]));
// DEFAULT-NEXT:         va_arg<i32>(%[[VALUE_ap]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i]], va_arg<i32>(%[[VALUE_ap]]));
// DEFAULT-NEXT:         va_arg<i32>(%[[VALUE_ap]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_j]], va_arg<i32>(%[[VALUE_ap]]));
// DEFAULT-NEXT:         va_arg<i32>(%[[VALUE_ap]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_k]], va_arg<i32>(%[[VALUE_ap]]));
// DEFAULT-NEXT:         va_arg<i32>(%[[VALUE_ap]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_l]], va_arg<i32>(%[[VALUE_ap]]));
// DEFAULT-NEXT:         va_arg<i32>(%[[VALUE_ap]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_m]], va_arg<i32>(%[[VALUE_ap]]));
// DEFAULT-NEXT:         va_arg<i32>(%[[VALUE_ap]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_n]], va_arg<i32>(%[[VALUE_ap]]));
// DEFAULT-NEXT:         va_arg<i32>(%[[VALUE_ap]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_o]], va_arg<i32>(%[[VALUE_ap]]));
// DEFAULT-NEXT:         va_arg<i32>(%[[VALUE_ap]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_p]], va_arg<i32>(%[[VALUE_ap]]));
// DEFAULT-NEXT:         va_arg<i32>(%[[VALUE_ap]]);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<i8>, ptr<const i8>, ...) -> i32>(%[[VALUE_sprintf]], array_decay<ptr<i8>, length=Some(50)>(%[[VALUE_buf]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(48)>(%[[VALUE_str]])), read<i32>(%[[VALUE_a]]), read<f64>(%[[VALUE_b]]), read<i32>(%[[VALUE_c]]), read<i32>(%[[VALUE_d]]), read<i32>(%[[VALUE_e]]), read<i32>(%[[VALUE_f]]), read<i32>(%[[VALUE_g]]), read<i32>(%[[VALUE_h]]), read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_j]]), read<i32>(%[[VALUE_k]]), read<i32>(%[[VALUE_l]]), read<i32>(%[[VALUE_m]]), read<i32>(%[[VALUE_n]]), read<i32>(%[[VALUE_o]]), read<i32>(%[[VALUE_p]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_strcmp:[0-9]+]] @__builtin_strcmp(%[[VALUE1:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE2:[0-9]+]] <unnamed>: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(i32, f64, i32, ...) -> i32>(%[[VALUE_va]], const<i32>(1), const<f64>(1.0), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE___builtin_strcmp]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(45)>(%[[VALUE_str_2]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(50)>(%[[VALUE_buf]]))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
