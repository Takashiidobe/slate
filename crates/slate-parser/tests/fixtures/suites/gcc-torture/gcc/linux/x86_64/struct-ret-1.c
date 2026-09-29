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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 d: f64;
// DEFAULT-NEXT:         field1 i: array<i32, 3>;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_B:[0-9]+]] B = @type[[TYPE0]];
// DEFAULT-NEXT:     type @type[[TYPE1:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 c: array<i8, 33>;
// DEFAULT-NEXT:         field1 c1: i8;
// DEFAULT-NEXT:     } [size=34, align=1, offsets=[0, 33]];
// DEFAULT-NEXT:     type @type[[TYPE_X:[0-9]+]] X = @type[[TYPE1]];
// DEFAULT-NEXT:     global %[[VALUE_out:[0-9]+]] out: array<i8, 100> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c1:[0-9]+]] c1: i8 [storage=static] = truncate<i8, reason=assign, fits=always>(const<i32>(97)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c2:[0-9]+]] c2: i8 [storage=static] = truncate<i8, reason=assign, fits=always>(const<i32>(127)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c3:[0-9]+]] c3: i8 [storage=static] = truncate<i8, reason=explicit, fits=unknown>(const<i32>(128)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c4:[0-9]+]] c4: i8 [storage=static] = truncate<i8, reason=explicit, fits=unknown>(const<i32>(255)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c5:[0-9]+]] c5: i8 [storage=static] = truncate<i8, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d1:[0-9]+]] d1: f64 [storage=static] = const<f64>(0.1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d2:[0-9]+]] d2: f64 [storage=static] = const<f64>(0.2) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d3:[0-9]+]] d3: f64 [storage=static] = const<f64>(0.3) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d4:[0-9]+]] d4: f64 [storage=static] = const<f64>(0.4) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d5:[0-9]+]] d5: f64 [storage=static] = const<f64>(0.5) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d6:[0-9]+]] d6: f64 [storage=static] = const<f64>(0.6) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d7:[0-9]+]] d7: f64 [storage=static] = const<f64>(0.7) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d8:[0-9]+]] d8: f64 [storage=static] = const<f64>(0.8) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d9:[0-9]+]] d9: f64 [storage=static] = const<f64>(0.9) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_B1:[0-9]+]] B1: @type[[TYPE0]] [storage=static] = aggregate<@type[[TYPE0]], zero_fill=false>(field0 = const<f64>(0.1), field1 = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2), index2 = const<i32>(3))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_B2:[0-9]+]] B2: @type[[TYPE0]] [storage=static] = aggregate<@type[[TYPE0]], zero_fill=false>(field0 = const<f64>(0.2), field1 = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(5), index1 = const<i32>(4), index2 = const<i32>(3))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_X1:[0-9]+]] X1: @type[[TYPE1]] [storage=static] = aggregate<@type[[TYPE1]], zero_fill=false>(field0 = code_units<array<i8, 33>>([97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 65, 66, 67, 68, 69, 70, 0]), field1 = truncate<i8, reason=assign, fits=always>(const<i32>(71))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_X2:[0-9]+]] X2: @type[[TYPE1]] [storage=static] = aggregate<@type[[TYPE1]], zero_fill=false>(field0 = code_units<array<i8, 33>>([49, 50, 51, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]), field1 = truncate<i8, reason=assign, fits=always>(const<i32>(57))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_X3:[0-9]+]] X3: @type[[TYPE1]] [storage=static] = aggregate<@type[[TYPE1]], zero_fill=false>(field0 = code_units<array<i8, 33>>([114, 101, 116, 117, 114, 110, 45, 114, 101, 116, 117, 114, 110, 45, 114, 101, 116, 117, 114, 110, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]), field1 = truncate<i8, reason=assign, fits=always>(const<i32>(82))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_xr:[0-9]+]] xr: @type[[TYPE1]] [storage=static] = aggregate<@type[[TYPE1]], zero_fill=false>(field0 = code_units<array<i8, 33>>([114, 101, 116, 117, 114, 110, 32, 118, 97, 108, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]), field1 = truncate<i8, reason=assign, fits=always>(const<i32>(82))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 63> [storage=static] = code_units<array<i8, 63>>([88, 32, 102, 40, 66, 44, 99, 104, 97, 114, 44, 100, 111, 117, 98, 108, 101, 44, 66, 41, 58, 40, 123, 37, 103, 44, 123, 37, 100, 44, 37, 100, 44, 37, 100, 125, 125, 44, 39, 37, 99, 39, 44, 37, 103, 44, 123, 37, 103, 44, 123, 37, 100, 44, 37, 100, 44, 37, 100, 125, 125, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_fp:[0-9]+]] fp: ptr<fn(@type[[TYPE0]], i8, f64, @type[[TYPE0]]) -> @type[[TYPE1]]> [storage=static] = addr_of<ptr<fn(@type[[TYPE0]], i8, f64, @type[[TYPE0]]) -> @type[[TYPE1]]>>(%[[VALUE_f:[0-9]+]]) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sprintf:[0-9]+]] @sprintf(%[[VALUE___s:[0-9]+]] __s: ptr<i8> [restrict], %[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strcpy:[0-9]+]] @strcpy(%[[VALUE___dest:[0-9]+]] __dest: ptr<i8> [restrict], %[[VALUE___src:[0-9]+]] __src: ptr<const i8> [restrict]) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strcmp:[0-9]+]] @strcmp(%[[VALUE___s1:[0-9]+]] __s1: ptr<const i8>, %[[VALUE___s2:[0-9]+]] __s2: ptr<const i8>) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_f]] @f(%[[VALUE_a:[0-9]+]] a: @type[[TYPE0]], %[[VALUE_b:[0-9]+]] b: i8, %[[VALUE_c:[0-9]+]] c: f64, %[[VALUE_d:[0-9]+]] d: @type[[TYPE0]]) -> @type[[TYPE1]] [linkage=external] [abi=sysv64(native_c, scalar, scalar, native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: @type[[TYPE1]] [storage=automatic];
// DEFAULT-NEXT:         write<@type[[TYPE1]]>(%[[VALUE_r]], copy<@type[[TYPE1]], reason=assign>(read<@type[[TYPE1]]>(%[[VALUE_xr]])));
// DEFAULT-NEXT:         write<i8>(field1(%[[VALUE_r]]), read<i8>(%[[VALUE_b]]));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<i8>, ptr<const i8>, ...) -> i32>(%[[VALUE_sprintf]], array_decay<ptr<i8>,
// DEFAULT-SAME: length=Some(100)>(%[[VALUE_out]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>,
// DEFAULT-SAME: length=Some(63)>(%[[VALUE_str]])),
// DEFAULT-SAME: read<f64>(field0(%[[VALUE_a]])), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>,
// DEFAULT-SAME: length=Some(3)>(field1(%[[VALUE_a]])), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>,
// DEFAULT-SAME: length=Some(3)>(field1(%[[VALUE_a]])), const<i32>(1)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>,
// DEFAULT-SAME: length=Some(3)>(field1(%[[VALUE_a]])), const<i32>(2)))), widen<i32,
// DEFAULT-SAME: reason=vararg>(read<i8>(%[[VALUE_b]])),
// DEFAULT-SAME: read<f64>(%[[VALUE_c]]),
// DEFAULT-SAME: read<f64>(field0(%[[VALUE_d]])), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>,
// DEFAULT-SAME: length=Some(3)>(field1(%[[VALUE_d]])), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>,
// DEFAULT-SAME: length=Some(3)>(field1(%[[VALUE_d]])), const<i32>(1)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>,
// DEFAULT-SAME: length=Some(3)>(field1(%[[VALUE_d]])), const<i32>(2)))));
// DEFAULT-NEXT:         return copy<@type[[TYPE1]], reason=return>(read<@type[[TYPE1]]>(%[[VALUE_r]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_Xr:[0-9]+]] Xr: @type[[TYPE1]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_tmp:[0-9]+]] tmp: array<i8, 100> [storage=automatic] [align=16];
// DEFAULT-NEXT:         write<@type[[TYPE1]]>(%[[VALUE_Xr]], copy<@type[[TYPE1]], reason=assign>(call<@type[[TYPE1]], signature=fn(@type[[TYPE0]], i8, f64, @type[[TYPE0]]) -> @type[[TYPE1]], abi=sysv64(native_c, scalar, scalar, native_c) -> native_c>(%[[VALUE_f]], copy<@type[[TYPE0]], reason=arg>(read<@type[[TYPE0]]>(%[[VALUE_B1]])), read<i8>(%[[VALUE_c2]]), read<f64>(%[[VALUE_d3]]), copy<@type[[TYPE0]], reason=arg>(read<@type[[TYPE0]]>(%[[VALUE_B2]])))));
// DEFAULT-NEXT:         copy<@type[[TYPE1]], reason=assign>(call<@type[[TYPE1]], signature=fn(@type[[TYPE0]], i8, f64, @type[[TYPE0]]) -> @type[[TYPE1]], abi=sysv64(native_c, scalar, scalar, native_c) -> native_c>(%[[VALUE_f]], copy<@type[[TYPE0]], reason=arg>(read<@type[[TYPE0]]>(%[[VALUE_B1]])), read<i8>(%[[VALUE_c2]]), read<f64>(%[[VALUE_d3]]), copy<@type[[TYPE0]], reason=arg>(read<@type[[TYPE0]]>(%[[VALUE_B2]]))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE_strcpy]], array_decay<ptr<i8>, length=Some(100)>(%[[VALUE_tmp]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(100)>(%[[VALUE_out]])));
// DEFAULT-NEXT:         write<i8>(field1(%[[VALUE_Xr]]), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(33)>(field0(%[[VALUE_Xr]])), const<i32>(0))), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<@type[[TYPE1]]>(%[[VALUE_Xr]], copy<@type[[TYPE1]], reason=assign>(call<@type[[TYPE1]], signature=fn(@type[[TYPE0]], i8, f64, @type[[TYPE0]]) -> @type[[TYPE1]], abi=sysv64(native_c, scalar, scalar, native_c) -> native_c>(read<ptr<fn(@type[[TYPE0]], i8, f64, @type[[TYPE0]]) -> @type[[TYPE1]]>>(%[[VALUE_fp]]), copy<@type[[TYPE0]], reason=arg>(read<@type[[TYPE0]]>(%[[VALUE_B1]])), read<i8>(%[[VALUE_c2]]), read<f64>(%[[VALUE_d3]]), copy<@type[[TYPE0]], reason=arg>(read<@type[[TYPE0]]>(%[[VALUE_B2]])))));
// DEFAULT-NEXT:         copy<@type[[TYPE1]], reason=assign>(call<@type[[TYPE1]], signature=fn(@type[[TYPE0]], i8, f64, @type[[TYPE0]]) -> @type[[TYPE1]], abi=sysv64(native_c, scalar, scalar, native_c) -> native_c>(read<ptr<fn(@type[[TYPE0]], i8, f64, @type[[TYPE0]]) -> @type[[TYPE1]]>>(%[[VALUE_fp]]), copy<@type[[TYPE0]], reason=arg>(read<@type[[TYPE0]]>(%[[VALUE_B1]])), read<i8>(%[[VALUE_c2]]), read<f64>(%[[VALUE_d3]]), copy<@type[[TYPE0]], reason=arg>(read<@type[[TYPE0]]>(%[[VALUE_B2]]))));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE_strcmp]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(100)>(%[[VALUE_tmp]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(100)>(%[[VALUE_out]]))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
