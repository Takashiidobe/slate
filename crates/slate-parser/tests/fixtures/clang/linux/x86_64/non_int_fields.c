#include <stdio.h>

struct Mixed {
  char          tag;
  unsigned char code;
  float         ratio;
  double        total;
};

union Scalar {
  char          tag;
  unsigned char code;
  float         ratio;
  double        total;
};

static int struct_values(void) {
  struct Mixed m;
  m.tag   = 'A';
  m.code  = 200;
  m.ratio = 1.5f;
  m.total = 2.25;
  printf("%c\n", m.tag);
  printf("%u\n", m.code);
  printf("%f\n", m.ratio);
  printf("%f\n", m.total);
  return m.code;
}

static double union_double(double value) {
  union Scalar s;
  s.total = value;
  return s.total;
}

static int union_char(char value) {
  union Scalar s;
  s.tag = value;
  return s.tag;
}

int main(void) {
  printf("%d\n", struct_values());
  printf("%f\n", union_double(4.5));
  printf("%d\n", union_char('Z'));
  return 0;
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
// DEFAULT-NEXT:     type @type0 Mixed = struct {
// DEFAULT-NEXT:         field0 tag: i8;
// DEFAULT-NEXT:         field1 code: u8;
// DEFAULT-NEXT:         field2 ratio: f32;
// DEFAULT-NEXT:         field3 total: f64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 1, 4, 8]];
// DEFAULT-NEXT:     type @type1 Scalar = union {
// DEFAULT-NEXT:         field0 tag: i8;
// DEFAULT-NEXT:         field1 code: u8;
// DEFAULT-NEXT:         field2 ratio: f32;
// DEFAULT-NEXT:         field3 total: f64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0, 0, 0]];
// DEFAULT-NEXT:     global %14 .str14: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 99, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %15 .str15: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %16 .str16: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 102, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %17 .str17: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 102, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %18 .str18: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %19 .str19: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 102, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %20 .str20: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%13 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %4 @struct_values() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %5 m: @type0 [storage=automatic];
// DEFAULT-NEXT:         write<i8>(field0(%5), truncate<i8, reason=assign, fits=always>(const<i32>(65)));
// DEFAULT-NEXT:         write<u8>(field1(%5), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(200))));
// DEFAULT-NEXT:         write<f32>(field2(%5), const<f32>(1.5));
// DEFAULT-NEXT:         write<f64>(field3(%5), const<f64>(2.25));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%14)), widen<i32, reason=vararg>(read<i8>(field0(%5))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%15)), reinterpret<i32, reason=vararg, fits=unknown>(widen<u32, reason=vararg>(read<u8>(field1(%5)))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%16)), float_widen<f64, reason=vararg>(read<f32>(field2(%5))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%17)), read<f64>(field3(%5)));
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(widen<u32, reason=return>(read<u8>(field1(%5))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @union_double(%7 value: f64) -> f64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %8 s: @type1 [storage=automatic];
// DEFAULT-NEXT:         write<f64>(field3(%8), read<f64>(%7));
// DEFAULT-NEXT:         return read<f64>(field3(%8));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @union_char(%10 value: i8) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %11 s: @type1 [storage=automatic];
// DEFAULT-NEXT:         write<i8>(field0(%11), read<i8>(%10));
// DEFAULT-NEXT:         return widen<i32, reason=return>(read<i8>(field0(%11)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%18)), call<i32, signature=fn() -> i32>(%4));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%19)), call<f64, signature=fn(f64) -> f64>(%6, const<f64>(4.5)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%20)), call<i32, signature=fn(i8) -> i32>(%9, truncate<i8, reason=arg, fits=always>(const<i32>(90))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
