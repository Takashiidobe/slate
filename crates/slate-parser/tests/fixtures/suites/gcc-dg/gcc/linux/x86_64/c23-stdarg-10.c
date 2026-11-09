/* PR c/107980 */
/* { dg-do compile } */
/* { dg-options "-std=c23 -pedantic-errors" } */

#include <stdarg.h>

int i;

void
f0 (...)
{
  va_list ap;
  va_start (ap);
  va_end (ap);
}

void
f1 (...)
{
  va_list ap;
  va_start (ap, i);				/* { dg-warning "optional second parameter of 'va_start' not last named argument" } */
  va_end (ap);
}

void
f2 (...)
{
  int j = 0;
  va_list ap;
  va_start (ap, j);				/* { dg-warning "optional second parameter of 'va_start' not last named argument" } */
  va_end (ap);
}

void
f3 (int k, int l, ...)
{
  va_list ap;
  va_start (ap, k);				/* { dg-warning "optional second parameter of 'va_start' not last named argument" } */
  va_end (ap);
}

void
f4 (int k, int l, ...)
{
  va_list ap;
  va_start (ap, l);
  va_end (ap);
}

void
f5 (int k, int l, ...)
{
  va_list ap;
  va_start (ap, (int) l);			/* { dg-warning "'va_start' macro used with additional arguments other than identifier of the last named argument" } */
  va_end (ap);
}

void
f6 (int k, int l, ...)
{
  va_list ap;
  va_start (ap, l + 0);				/* { dg-warning "'va_start' macro used with additional arguments other than identifier of the last named argument" } */
  va_end (ap);
}

void
f7 (int k, int l, ...)
{
  va_list ap;
  va_start (ap, ()()(), [][][], {}{}{}, *+-/1({[_*_]})%&&!?!?);	/* { dg-warning "'va_start' macro used with additional arguments other than identifier of the last named argument" } */
  va_end (ap);
}

void
f8 (...)
{
  va_list ap;
  va_start (ap,);				/* { dg-warning "'va_start' macro used with additional arguments other than identifier of the last named argument" } */
  va_end (ap);
}

void
f9 (int k, int l, ...)
{
  va_list ap;
  va_start (ap, k+l+****2);			/* { dg-warning "'va_start' macro used with additional arguments other than identifier of the last named argument" } */
  va_end (ap);
}

void
f10 (register int m, ...)
{
  va_list ap;
  va_start (ap, m);				/* { dg-warning "undefined behavior when second parameter of 'va_start' is declared with 'register' storage" } */
  va_end (ap);
}

void
f11 (int k, int l, ...)
{
  va_list ap;
  va_start (ap, ()()()[[[}}});			/* { dg-warning "'va_start' macro used with additional arguments other than identifier of the last named argument" } */
  va_end (ap);
}

void
f12 (int k, int l, ...)
{
  va_list ap;
  va_start (ap, ]]]]]]{{{{{{);			/* { dg-warning "'va_start' macro used with additional arguments other than identifier of the last named argument" } */
  va_end (ap);
}

// SLATE-FILECHECK-STD DEFAULT c23
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
// DEFAULT-NEXT:     type @type[[TYPE_va_list:[0-9]+]] va_list = va_list [c="__gnuc_va_list"];
// DEFAULT-NEXT:     global %[[VALUE_i:[0-9]+]] i: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f0:[0-9]+]] @f0(...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap:[0-9]+]] ap: va_list [storage=automatic] [c="va_list"];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap]]);
// DEFAULT-NEXT:         va_end(%[[VALUE_ap]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f1:[0-9]+]] @f1(...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_2:[0-9]+]] ap: va_list [storage=automatic] [c="va_list"];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_2]]);
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f2:[0-9]+]] @f2(...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_ap_3:[0-9]+]] ap: va_list [storage=automatic] [c="va_list"];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_3]]);
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_3]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f3:[0-9]+]] @f3(%[[VALUE_k:[0-9]+]] k: i32, %[[VALUE_l:[0-9]+]] l: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_4:[0-9]+]] ap: va_list [storage=automatic] [c="va_list"];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_4]]);
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_4]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f4:[0-9]+]] @f4(%[[VALUE_k_2:[0-9]+]] k: i32, %[[VALUE_l_2:[0-9]+]] l: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_5:[0-9]+]] ap: va_list [storage=automatic] [c="va_list"];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_5]]);
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_5]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f5:[0-9]+]] @f5(%[[VALUE_k_3:[0-9]+]] k: i32, %[[VALUE_l_3:[0-9]+]] l: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_6:[0-9]+]] ap: va_list [storage=automatic] [c="va_list"];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_6]]);
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_6]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f6:[0-9]+]] @f6(%[[VALUE_k_4:[0-9]+]] k: i32, %[[VALUE_l_4:[0-9]+]] l: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_7:[0-9]+]] ap: va_list [storage=automatic] [c="va_list"];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_7]]);
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_7]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f7:[0-9]+]] @f7(%[[VALUE_k_5:[0-9]+]] k: i32, %[[VALUE_l_5:[0-9]+]] l: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_8:[0-9]+]] ap: va_list [storage=automatic] [c="va_list"];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_8]]);
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_8]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f8:[0-9]+]] @f8(...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_9:[0-9]+]] ap: va_list [storage=automatic] [c="va_list"];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_9]]);
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_9]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f9:[0-9]+]] @f9(%[[VALUE_k_6:[0-9]+]] k: i32, %[[VALUE_l_6:[0-9]+]] l: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_10:[0-9]+]] ap: va_list [storage=automatic] [c="va_list"];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_10]]);
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_10]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f10:[0-9]+]] @f10(%[[VALUE_m:[0-9]+]] m: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_11:[0-9]+]] ap: va_list [storage=automatic] [c="va_list"];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_11]]);
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_11]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f11:[0-9]+]] @f11(%[[VALUE_k_7:[0-9]+]] k: i32, %[[VALUE_l_7:[0-9]+]] l: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_12:[0-9]+]] ap: va_list [storage=automatic] [c="va_list"];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_12]]);
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_12]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f12:[0-9]+]] @f12(%[[VALUE_k_8:[0-9]+]] k: i32, %[[VALUE_l_8:[0-9]+]] l: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_13:[0-9]+]] ap: va_list [storage=automatic] [c="va_list"];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_13]]);
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_13]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
