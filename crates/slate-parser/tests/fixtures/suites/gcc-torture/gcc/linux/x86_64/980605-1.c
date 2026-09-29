/* { dg-add-options stack_size } */

#include <stdio.h>

void abort(void);
void exit(int);

#ifndef STACK_SIZE
#define STACK_SIZE 200000
#endif

__inline__ static int dummy(int x) {
  int y;
  y = (long)(x * 4711.3);
  return y;
}

int getval(void);

int f2(double x) {
  unsigned short s;
  int            a, b, c, d, e, f, g, h, i, j;

  a = getval();
  b = getval();
  c = getval();
  d = getval();
  e = getval();
  f = getval();
  g = getval();
  h = getval();
  i = getval();
  j = getval();

  s = x;

  return a + b + c + d + e + f + g + h + i + j + s;
}

int x = 1;

int getval(void) { return x++; }

char buf[10];

void f() {
  char ar[STACK_SIZE / 2];
  int  a, b, c, d, e, f, g, h, i, j, k;

  a = getval();
  b = getval();
  c = getval();
  d = getval();
  e = getval();
  f = getval();
  g = getval();
  h = getval();
  i = getval();
  j = getval();

  k = f2(17.0);

  sprintf(buf, "%d\n", a + b + c + d + e + f + g + h + i + j + k);
  if (a + b + c + d + e + f + g + h + i + j + k != 227)
    abort();
}

int main(void) {
  f();
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
// DEFAULT-NEXT:     global %[[VALUE_x:[0-9]+]] x: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_buf:[0-9]+]] buf: array<i8, 10> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_sprintf:[0-9]+]] @sprintf(%[[VALUE___s:[0-9]+]] __s: ptr<i8> [restrict], %[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_dummy:[0-9]+]] @dummy(%[[VALUE_x_2:[0-9]+]] x: i32) -> i32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y:[0-9]+]] y: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%[[VALUE_y]], truncate<i32, reason=assign, fits=unknown>(float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=observable>(mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(read<i32>(%[[VALUE_x_2]])), const<f64>(4711.3)))));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_y]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_getval:[0-9]+]] @getval() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE1]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f2:[0-9]+]] @f2(%[[VALUE_x_3:[0-9]+]] x: f64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_s:[0-9]+]] s: u16 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_d:[0-9]+]] d: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_e:[0-9]+]] e: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_f:[0-9]+]] f: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_g:[0-9]+]] g: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_h:[0-9]+]] h: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%[[VALUE_a]], call<i32, signature=fn() -> i32>(%[[VALUE_getval]]));
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_getval]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_b]], call<i32, signature=fn() -> i32>(%[[VALUE_getval]]));
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_getval]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_c]], call<i32, signature=fn() -> i32>(%[[VALUE_getval]]));
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_getval]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_d]], call<i32, signature=fn() -> i32>(%[[VALUE_getval]]));
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_getval]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_e]], call<i32, signature=fn() -> i32>(%[[VALUE_getval]]));
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_getval]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_f]], call<i32, signature=fn() -> i32>(%[[VALUE_getval]]));
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_getval]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_g]], call<i32, signature=fn() -> i32>(%[[VALUE_getval]]));
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_getval]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_h]], call<i32, signature=fn() -> i32>(%[[VALUE_getval]]));
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_getval]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i]], call<i32, signature=fn() -> i32>(%[[VALUE_getval]]));
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_getval]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_j]], call<i32, signature=fn() -> i32>(%[[VALUE_getval]]));
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_getval]]);
// DEFAULT-NEXT:         write<u16>(%[[VALUE_s]], float_to_int<u16, reason=assign, out_of_range=ub, exceptions=observable>(read<f64>(%[[VALUE_x_3]])));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]])), read<i32>(%[[VALUE_c]])), read<i32>(%[[VALUE_d]])), read<i32>(%[[VALUE_e]])), read<i32>(%[[VALUE_f]])), read<i32>(%[[VALUE_g]])), read<i32>(%[[VALUE_h]])), read<i32>(%[[VALUE_i]])), read<i32>(%[[VALUE_j]])), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_s]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f_2:[0-9]+]] @f() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ar:[0-9]+]] ar: array<i8, 100000> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_a_2:[0-9]+]] a: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_b_2:[0-9]+]] b: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_c_2:[0-9]+]] c: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_d_2:[0-9]+]] d: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_e_2:[0-9]+]] e: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_f_3:[0-9]+]] f: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_g_2:[0-9]+]] g: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_h_2:[0-9]+]] h: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i_2:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_j_2:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_k:[0-9]+]] k: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%[[VALUE_a_2]], call<i32, signature=fn() -> i32>(%[[VALUE_getval]]));
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_getval]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_b_2]], call<i32, signature=fn() -> i32>(%[[VALUE_getval]]));
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_getval]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_c_2]], call<i32, signature=fn() -> i32>(%[[VALUE_getval]]));
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_getval]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_d_2]], call<i32, signature=fn() -> i32>(%[[VALUE_getval]]));
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_getval]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_e_2]], call<i32, signature=fn() -> i32>(%[[VALUE_getval]]));
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_getval]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_f_3]], call<i32, signature=fn() -> i32>(%[[VALUE_getval]]));
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_getval]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_g_2]], call<i32, signature=fn() -> i32>(%[[VALUE_getval]]));
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_getval]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_h_2]], call<i32, signature=fn() -> i32>(%[[VALUE_getval]]));
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_getval]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i_2]], call<i32, signature=fn() -> i32>(%[[VALUE_getval]]));
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_getval]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_j_2]], call<i32, signature=fn() -> i32>(%[[VALUE_getval]]));
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_getval]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_k]], call<i32, signature=fn(f64) -> i32>(%[[VALUE_f2]], const<f64>(17.0)));
// DEFAULT-NEXT:         call<i32, signature=fn(f64) -> i32>(%[[VALUE_f2]], const<f64>(17.0));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<i8>, ptr<const i8>, ...) -> i32>(%[[VALUE_sprintf]], array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_buf]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str]])), add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(%[[VALUE_a_2]]), read<i32>(%[[VALUE_b_2]])), read<i32>(%[[VALUE_c_2]])), read<i32>(%[[VALUE_d_2]])), read<i32>(%[[VALUE_e_2]])), read<i32>(%[[VALUE_f_3]])), read<i32>(%[[VALUE_g_2]])), read<i32>(%[[VALUE_h_2]])), read<i32>(%[[VALUE_i_2]])), read<i32>(%[[VALUE_j_2]])), read<i32>(%[[VALUE_k]])));
// DEFAULT-NEXT:         if ne<i32>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(%[[VALUE_a_2]]), read<i32>(%[[VALUE_b_2]])), read<i32>(%[[VALUE_c_2]])), read<i32>(%[[VALUE_d_2]])), read<i32>(%[[VALUE_e_2]])), read<i32>(%[[VALUE_f_3]])), read<i32>(%[[VALUE_g_2]])), read<i32>(%[[VALUE_h_2]])), read<i32>(%[[VALUE_i_2]])), read<i32>(%[[VALUE_j_2]])), read<i32>(%[[VALUE_k]])), const<i32>(227))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_f_2]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
