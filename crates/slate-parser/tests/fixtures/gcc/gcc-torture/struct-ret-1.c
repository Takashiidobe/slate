#include <stdio.h>
#include <string.h>

void abort(void);
void exit(int);

char out[100];

typedef struct {
  double d;
  int    i[3];
} B;
typedef struct {
  char c[33], c1;
} X;

char c1 = 'a';
char c2 = 127;
char c3 = (char)128;
char c4 = (char)255;
char c5 = -1;

double d1 = 0.1;
double d2 = 0.2;
double d3 = 0.3;
double d4 = 0.4;
double d5 = 0.5;
double d6 = 0.6;
double d7 = 0.7;
double d8 = 0.8;
double d9 = 0.9;

B B1 = {0.1, {1, 2, 3}};
B B2 = {0.2, {5, 4, 3}};
X X1 = {"abcdefghijklmnopqrstuvwxyzABCDEF", 'G'};
X X2 = {"123", '9'};
X X3 = {"return-return-return", 'R'};

X f(B a, char b, double c, B d) {
  static X xr = {"return val", 'R'};
  X        r;
  r    = xr;
  r.c1 = b;
  sprintf(out, "X f(B,char,double,B):({%g,{%d,%d,%d}},'%c',%g,{%g,{%d,%d,%d}})",
          a.d, a.i[0], a.i[1], a.i[2], b, c, d.d, d.i[0], d.i[1], d.i[2]);
  return r;
}

X (*fp)(B, char, double, B) = &f;

int main(void) {
  X    Xr;
  char tmp[100];

  Xr = f(B1, c2, d3, B2);
  strcpy(tmp, out);
  Xr.c[0] = Xr.c1 = '\0';
  Xr              = (*fp)(B1, c2, d3, B2);
  if (strcmp(tmp, out))
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
// DEFAULT-NEXT:     type @type0 = struct {
// DEFAULT-NEXT:         field0 d: f64;
// DEFAULT-NEXT:         field1 i: array<i32, 3>;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type1 B = @type0;
// DEFAULT-NEXT:     type @type2 = struct {
// DEFAULT-NEXT:         field0 c: array<i8, 33>;
// DEFAULT-NEXT:         field1 c1: i8;
// DEFAULT-NEXT:     } [size=34, align=1, offsets=[0, 33]];
// DEFAULT-NEXT:     type @type3 X = @type2;
// DEFAULT-NEXT:     global %5 out: array<i8, 100> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %10 c1: i8 [storage=static] = truncate<i8, reason=assign, fits=always>(const<i32>(97)) [linkage=external];
// DEFAULT-NEXT:     global %11 c2: i8 [storage=static] = truncate<i8, reason=assign, fits=always>(const<i32>(127)) [linkage=external];
// DEFAULT-NEXT:     global %12 c3: i8 [storage=static] = truncate<i8, reason=explicit, fits=unknown>(const<i32>(128)) [linkage=external];
// DEFAULT-NEXT:     global %13 c4: i8 [storage=static] = truncate<i8, reason=explicit, fits=unknown>(const<i32>(255)) [linkage=external];
// DEFAULT-NEXT:     global %14 c5: i8 [storage=static] = truncate<i8, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=external];
// DEFAULT-NEXT:     global %15 d1: f64 [storage=static] = const<f64>(0.1) [linkage=external];
// DEFAULT-NEXT:     global %16 d2: f64 [storage=static] = const<f64>(0.2) [linkage=external];
// DEFAULT-NEXT:     global %17 d3: f64 [storage=static] = const<f64>(0.3) [linkage=external];
// DEFAULT-NEXT:     global %18 d4: f64 [storage=static] = const<f64>(0.4) [linkage=external];
// DEFAULT-NEXT:     global %19 d5: f64 [storage=static] = const<f64>(0.5) [linkage=external];
// DEFAULT-NEXT:     global %20 d6: f64 [storage=static] = const<f64>(0.6) [linkage=external];
// DEFAULT-NEXT:     global %21 d7: f64 [storage=static] = const<f64>(0.7) [linkage=external];
// DEFAULT-NEXT:     global %22 d8: f64 [storage=static] = const<f64>(0.8) [linkage=external];
// DEFAULT-NEXT:     global %23 d9: f64 [storage=static] = const<f64>(0.9) [linkage=external];
// DEFAULT-NEXT:     global %24 B1: @type0 [storage=static] = aggregate<@type0, zero_fill=false>(field0 = const<f64>(0.1), field1 = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2), index2 = const<i32>(3))) [linkage=external];
// DEFAULT-NEXT:     global %25 B2: @type0 [storage=static] = aggregate<@type0, zero_fill=false>(field0 = const<f64>(0.2), field1 = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(5), index1 = const<i32>(4), index2 = const<i32>(3))) [linkage=external];
// DEFAULT-NEXT:     global %26 X1: @type2 [storage=static] = aggregate<@type2, zero_fill=false>(field0 = code_units<array<i8, 33>>([97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 65, 66, 67, 68, 69, 70, 0]), field1 = truncate<i8, reason=assign, fits=always>(const<i32>(71))) [linkage=external];
// DEFAULT-NEXT:     global %27 X2: @type2 [storage=static] = aggregate<@type2, zero_fill=false>(field0 = code_units<array<i8, 33>>([49, 50, 51, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]), field1 = truncate<i8, reason=assign, fits=always>(const<i32>(57))) [linkage=external];
// DEFAULT-NEXT:     global %28 X3: @type2 [storage=static] = aggregate<@type2, zero_fill=false>(field0 = code_units<array<i8, 33>>([114, 101, 116, 117, 114, 110, 45, 114, 101, 116, 117, 114, 110, 45, 114, 101, 116, 117, 114, 110, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]), field1 = truncate<i8, reason=assign, fits=always>(const<i32>(82))) [linkage=external];
// DEFAULT-NEXT:     global %34 xr: @type2 [storage=static] = aggregate<@type2, zero_fill=false>(field0 = code_units<array<i8, 33>>([114, 101, 116, 117, 114, 110, 32, 118, 97, 108, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]), field1 = truncate<i8, reason=assign, fits=always>(const<i32>(82))) [linkage=internal];
// DEFAULT-NEXT:     global %47 .str47: array<i8, 63> [storage=static] = code_units<array<i8, 63>>([88, 32, 102, 40, 66, 44, 99, 104, 97, 114, 44, 100, 111, 117, 98, 108, 101, 44, 66, 41, 58, 40, 123, 37, 103, 44, 123, 37, 100, 44, 37, 100, 44, 37, 100, 125, 125, 44, 39, 37, 99, 39, 44, 37, 103, 44, 123, 37, 103, 44, 123, 37, 100, 44, 37, 100, 44, 37, 100, 125, 125, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %36 fp: ptr<fn(@type0, i8, f64, @type0) -> @type2> [storage=static] = addr_of<ptr<fn(@type0, i8, f64, @type0) -> @type2>>(%29) [linkage=external];
// DEFAULT-NEXT:     fn %0 @sprintf(%40 __s: ptr<i8> [restrict], %41 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @strcpy(%42 __dest: ptr<i8> [restrict], %43 __src: ptr<const i8> [restrict]) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %2 @strcmp(%44 __s1: ptr<const i8>, %45 __s2: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %3 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %4 @exit(%46 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %29 @f(%30 a: @type0, %31 b: i8, %32 c: f64, %33 d: @type0) -> @type2 [linkage=external] [abi=sysv64(native_c, scalar, scalar, native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %35 r: @type2 [storage=automatic];
// DEFAULT-NEXT:         write<@type2>(%35, copy<@type2, reason=assign>(read<@type2>(%34)));
// DEFAULT-NEXT:         write<i8>(field1(%35), read<i8>(%31));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<i8>, ptr<const i8>, ...) -> i32>(sprintf, array_decay<ptr<i8>, length=Some(100)>(%5), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(63)>(%47)), read<f64>(field0(%30)), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(field1(%30)), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(field1(%30)), const<i32>(1)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(field1(%30)), const<i32>(2)))), widen<i32, reason=vararg>(read<i8>(%31)), read<f64>(%32), read<f64>(field0(%33)), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(field1(%33)), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(field1(%33)), const<i32>(1)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(field1(%33)), const<i32>(2)))));
// DEFAULT-NEXT:         return copy<@type2, reason=return>(read<@type2>(%35));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %37 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %38 Xr: @type2 [storage=automatic];
// DEFAULT-NEXT:         let %39 tmp: array<i8, 100> [storage=automatic] [align=16];
// DEFAULT-NEXT:         write<@type2>(%38, copy<@type2, reason=assign>(call<@type2, signature=fn(@type0, i8, f64, @type0) -> @type2, abi=sysv64(native_c, scalar, scalar, native_c) -> native_c>(%29, copy<@type0, reason=arg>(read<@type0>(%24)), read<i8>(%11), read<f64>(%17), copy<@type0, reason=arg>(read<@type0>(%25)))));
// DEFAULT-NEXT:         copy<@type2, reason=assign>(call<@type2, signature=fn(@type0, i8, f64, @type0) -> @type2, abi=sysv64(native_c, scalar, scalar, native_c) -> native_c>(%29, copy<@type0, reason=arg>(read<@type0>(%24)), read<i8>(%11), read<f64>(%17), copy<@type0, reason=arg>(read<@type0>(%25))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(strcpy, array_decay<ptr<i8>, length=Some(100)>(%39), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(100)>(%5)));
// DEFAULT-NEXT:         write<i8>(field1(%38), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(33)>(field0(%38)), const<i32>(0))), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<@type2>(%38, copy<@type2, reason=assign>(call<@type2, signature=fn(@type0, i8, f64, @type0) -> @type2, abi=sysv64(native_c, scalar, scalar, native_c) -> native_c>(read<ptr<fn(@type0, i8, f64, @type0) -> @type2>>(%36), copy<@type0, reason=arg>(read<@type0>(%24)), read<i8>(%11), read<f64>(%17), copy<@type0, reason=arg>(read<@type0>(%25)))));
// DEFAULT-NEXT:         copy<@type2, reason=assign>(call<@type2, signature=fn(@type0, i8, f64, @type0) -> @type2, abi=sysv64(native_c, scalar, scalar, native_c) -> native_c>(read<ptr<fn(@type0, i8, f64, @type0) -> @type2>>(%36), copy<@type0, reason=arg>(read<@type0>(%24)), read<i8>(%11), read<f64>(%17), copy<@type0, reason=arg>(read<@type0>(%25))));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(strcmp, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(100)>(%39)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(100)>(%5))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
