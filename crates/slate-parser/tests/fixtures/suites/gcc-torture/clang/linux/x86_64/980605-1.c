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
// DEFAULT-NEXT:     global %22 x: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %23 buf: array<i8, 10> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %41 .str41: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %2 @sprintf(%38 __s: ptr<i8> [restrict], %39 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %3 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %4 @exit(%40 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %5 @dummy(%6 x: i32) -> i32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %7 y: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%7, truncate<i32, reason=assign, fits=unknown>(float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(read<i32>(%6)), const<f64>(4711.3)))));
// DEFAULT-NEXT:         return read<i32>(%7);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @getval() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %42: i32 [synthetic] = read<i32>(%22);
// DEFAULT-NEXT:         let %43: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%42), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%22, read<i32>(%43));
// DEFAULT-NEXT:         return read<i32>(%42);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @f2(%10 x: f64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %11 s: u16 [storage=automatic];
// DEFAULT-NEXT:         let %12 a: i32 [storage=automatic];
// DEFAULT-NEXT:         let %13 b: i32 [storage=automatic];
// DEFAULT-NEXT:         let %14 c: i32 [storage=automatic];
// DEFAULT-NEXT:         let %15 d: i32 [storage=automatic];
// DEFAULT-NEXT:         let %16 e: i32 [storage=automatic];
// DEFAULT-NEXT:         let %17 f: i32 [storage=automatic];
// DEFAULT-NEXT:         let %18 g: i32 [storage=automatic];
// DEFAULT-NEXT:         let %19 h: i32 [storage=automatic];
// DEFAULT-NEXT:         let %20 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %21 j: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%12, call<i32, signature=fn() -> i32>(%8));
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%8);
// DEFAULT-NEXT:         write<i32>(%13, call<i32, signature=fn() -> i32>(%8));
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%8);
// DEFAULT-NEXT:         write<i32>(%14, call<i32, signature=fn() -> i32>(%8));
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%8);
// DEFAULT-NEXT:         write<i32>(%15, call<i32, signature=fn() -> i32>(%8));
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%8);
// DEFAULT-NEXT:         write<i32>(%16, call<i32, signature=fn() -> i32>(%8));
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%8);
// DEFAULT-NEXT:         write<i32>(%17, call<i32, signature=fn() -> i32>(%8));
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%8);
// DEFAULT-NEXT:         write<i32>(%18, call<i32, signature=fn() -> i32>(%8));
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%8);
// DEFAULT-NEXT:         write<i32>(%19, call<i32, signature=fn() -> i32>(%8));
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%8);
// DEFAULT-NEXT:         write<i32>(%20, call<i32, signature=fn() -> i32>(%8));
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%8);
// DEFAULT-NEXT:         write<i32>(%21, call<i32, signature=fn() -> i32>(%8));
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%8);
// DEFAULT-NEXT:         write<u16>(%11, float_to_int<u16, reason=assign, out_of_range=ub, exceptions=ignore>(read<f64>(%10)));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(%12), read<i32>(%13)), read<i32>(%14)), read<i32>(%15)), read<i32>(%16)), read<i32>(%17)), read<i32>(%18)), read<i32>(%19)), read<i32>(%20)), read<i32>(%21)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%11))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %24 @f() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %25 ar: array<i8, 100000> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %26 a: i32 [storage=automatic];
// DEFAULT-NEXT:         let %27 b: i32 [storage=automatic];
// DEFAULT-NEXT:         let %28 c: i32 [storage=automatic];
// DEFAULT-NEXT:         let %29 d: i32 [storage=automatic];
// DEFAULT-NEXT:         let %30 e: i32 [storage=automatic];
// DEFAULT-NEXT:         let %31 f: i32 [storage=automatic];
// DEFAULT-NEXT:         let %32 g: i32 [storage=automatic];
// DEFAULT-NEXT:         let %33 h: i32 [storage=automatic];
// DEFAULT-NEXT:         let %34 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %35 j: i32 [storage=automatic];
// DEFAULT-NEXT:         let %36 k: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%26, call<i32, signature=fn() -> i32>(%8));
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%8);
// DEFAULT-NEXT:         write<i32>(%27, call<i32, signature=fn() -> i32>(%8));
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%8);
// DEFAULT-NEXT:         write<i32>(%28, call<i32, signature=fn() -> i32>(%8));
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%8);
// DEFAULT-NEXT:         write<i32>(%29, call<i32, signature=fn() -> i32>(%8));
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%8);
// DEFAULT-NEXT:         write<i32>(%30, call<i32, signature=fn() -> i32>(%8));
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%8);
// DEFAULT-NEXT:         write<i32>(%31, call<i32, signature=fn() -> i32>(%8));
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%8);
// DEFAULT-NEXT:         write<i32>(%32, call<i32, signature=fn() -> i32>(%8));
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%8);
// DEFAULT-NEXT:         write<i32>(%33, call<i32, signature=fn() -> i32>(%8));
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%8);
// DEFAULT-NEXT:         write<i32>(%34, call<i32, signature=fn() -> i32>(%8));
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%8);
// DEFAULT-NEXT:         write<i32>(%35, call<i32, signature=fn() -> i32>(%8));
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%8);
// DEFAULT-NEXT:         write<i32>(%36, call<i32, signature=fn(f64) -> i32>(%9, const<f64>(17.0)));
// DEFAULT-NEXT:         call<i32, signature=fn(f64) -> i32>(%9, const<f64>(17.0));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<i8>, ptr<const i8>, ...) -> i32>(%2, array_decay<ptr<i8>, length=Some(10)>(%23), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%41)), add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(%26), read<i32>(%27)), read<i32>(%28)), read<i32>(%29)), read<i32>(%30)), read<i32>(%31)), read<i32>(%32)), read<i32>(%33)), read<i32>(%34)), read<i32>(%35)), read<i32>(%36)));
// DEFAULT-NEXT:         if ne<i32>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(%26), read<i32>(%27)), read<i32>(%28)), read<i32>(%29)), read<i32>(%30)), read<i32>(%31)), read<i32>(%32)), read<i32>(%33)), read<i32>(%34)), read<i32>(%35)), read<i32>(%36)), const<i32>(227))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %37 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%24);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%4, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
