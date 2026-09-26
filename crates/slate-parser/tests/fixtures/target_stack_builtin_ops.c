#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

struct padded {
  unsigned char byte;
  unsigned int  word;
};

struct bit_padded {
  unsigned char low  : 3;
  unsigned char      : 2;
  unsigned char high : 3;
};

typedef double double2 __attribute__((ext_vector_type(2)));

static int cache_prefetch_probe(int x) {
  char bytes[16] = {0};
  bytes[0]       = (char)x;
  __builtin___clear_cache(bytes, bytes + sizeof(bytes));
  __builtin_prefetch(bytes + 1, 0, 3);
  return bytes[0] + 1;
}

static int frame_probe(void) {
  void *frame = __builtin_frame_address(0);
  return frame != 0;
}

static int clear_padding_probe(void) {
  struct padded     value;
  struct bit_padded bits;
  memset(&value, 0xff, sizeof(value));
  memset(&bits, 0xff, sizeof(bits));
  value.byte = 7;
  value.word = 11;
  bits.low   = 7;
  bits.high  = 7;
#if __has_builtin(__builtin_clear_padding)
  __builtin_clear_padding(&value);
  __builtin_clear_padding(&bits);
#else
  unsigned char *padding   = (unsigned char *)&value;
  padding[1]               = 0;
  padding[2]               = 0;
  padding[3]               = 0;
  *(unsigned char *)&bits &= 0xe7;
#endif
  unsigned char *bytes = (unsigned char *)&value;
  return 10 * (bytes[1] + bytes[2] + bytes[3]) +
         (*(unsigned char *)&bits == 0xe7);
}

static int frexp_probe(void) {
  volatile double input   = 12.0;
  volatile float  input_f = 8.0f;
  int             exponent_d;
  int             exponent_f;
  double          fraction_d = __builtin_frexp(input, &exponent_d);
  float           fraction_f = __builtin_frexpf(input_f, &exponent_f);
  return 100 * (10 * (fraction_d == 0.75) + exponent_d) +
         10 * (fraction_f == 0.5f) + exponent_f;
}

static int hyperbolic_probe(void) {
  volatile double input = 0.0;
#if __has_builtin(__builtin_elementwise_cosh)
  double           c            = __builtin_elementwise_cosh(input);
  double           s            = __builtin_elementwise_sinh(input);
  double           t            = __builtin_elementwise_tanh(input);
  volatile double2 vector_input = {0.0, 0.0};
  double2          vc           = __builtin_elementwise_cosh(vector_input);
  double2          vs           = __builtin_elementwise_sinh(vector_input);
  double2          vt           = __builtin_elementwise_tanh(vector_input);
  int              vector_ok = vc[0] == 1.0 && vc[1] == 1.0 && vs[0] == 0.0 &&
                               vs[1] == 0.0 && vt[0] == 0.0 && vt[1] == 0.0;
#else
  double c         = cosh(input);
  double s         = sinh(input);
  double t         = tanh(input);
  int    vector_ok = 1;
#endif
  return 1000 * vector_ok + 100 * (c == 1.0) + 10 * (s == 0.0) + (t == 0.0);
}

int main(void) {
  volatile int input = 7;
  printf("%d %d %d %d %d\n", cache_prefetch_probe(input), frame_probe(),
         clear_padding_probe(), frexp_probe(), hyperbolic_probe());
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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     type @type1 padded = struct {
// DEFAULT-NEXT:         field0 byte: u8;
// DEFAULT-NEXT:         field1 word: u32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type2 bit_padded = struct {
// DEFAULT-NEXT:         field0 low: u8 : 3;
// DEFAULT-NEXT:         field1 <anonymous>: u8 : 2;
// DEFAULT-NEXT:         field2 high: u8 : 3;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0, 0, 0], bit_offsets=[Some(0), Some(3), Some(5)], bit_units=[(0, 1)], field_units=[Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type3 double2 = vector<f64, 2>;
// DEFAULT-NEXT:     global %39 .str39: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%35 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @memset(%36 __s: ptr<void>, %37 __c: i32, %38 __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %6 @cache_prefetch_probe(%7 x: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %8 bytes: array<i8, 16> [storage=automatic] [align=16] = aggregate<array<i8, 16>, zero_fill=true>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(16)>(%8), const<i32>(0))), truncate<i8, reason=explicit, fits=unknown>(read<i32>(%7)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ptr<void>) -> void>(__builtin___clear_cache, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%8)), pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(16)>(%8), const<u64>(16))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(__builtin_prefetch, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(16)>(%8), const<i32>(1))), const<i32>(0), const<i32>(3));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(16)>(%8), const<i32>(0))))), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @frame_probe() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %10 frame: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(u32) -> ptr<void>>(__builtin_frame_address, reinterpret<u32, reason=arg, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<ptr<void>>(read<ptr<void>>(%10), null<ptr<void>>));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @clear_padding_probe() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %12 value: @type1 [storage=automatic];
// DEFAULT-NEXT:         let %13 bits: @type2 [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type1>>(%12)), const<i32>(255), const<u64>(8));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type2>>(%13)), const<i32>(255), const<u64>(1));
// DEFAULT-NEXT:         write<u8>(field0(%12), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(7))));
// DEFAULT-NEXT:         write<u32>(field1(%12), reinterpret<u32, reason=assign, fits=always>(const<i32>(11)));
// DEFAULT-NEXT:         write<u8>(bitfield0<unit=0, bytes=0..1, bits=0..3>(%13), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(7))));
// DEFAULT-NEXT:         write<u8>(bitfield2<unit=0, bytes=0..1, bits=5..8>(%13), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(7))));
// DEFAULT-NEXT:         let %14 padding: ptr<u8> [storage=automatic] = pointer_cast<ptr<u8>, reason=explicit>(addr_of<ptr<@type1>>(%12));
// DEFAULT-NEXT:         write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%14), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%14), const<i32>(2))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%14), const<i32>(3))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         let %40: ptr<u8> [synthetic] = pointer_cast<ptr<u8>, reason=explicit>(addr_of<ptr<@type2>>(%13));
// DEFAULT-NEXT:         let %41: u8 [synthetic] = read<u8>(deref(read<ptr<u8>>(%40)));
// DEFAULT-NEXT:         let %42: u8 [synthetic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%41))), const<i32>(231))));
// DEFAULT-NEXT:         write<u8>(deref(read<ptr<u8>>(%40)), read<u8>(%42));
// DEFAULT-NEXT:         let %15 bytes: ptr<u8> [storage=automatic] = pointer_cast<ptr<u8>, reason=explicit>(addr_of<ptr<@type1>>(%12));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(10), add<i32, overflow=ub>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%15), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%15), const<i32>(2))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%15), const<i32>(3)))))))), from_bool<i32, reason=promotion>(eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(pointer_cast<ptr<u8>, reason=explicit>(addr_of<ptr<@type2>>(%13)))))), const<i32>(231))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @frexp_probe() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %17 input: volatile f64 [storage=automatic] = const<f64>(12.0);
// DEFAULT-NEXT:         let %18 input_f: volatile f32 [storage=automatic] = const<f32>(8.0);
// DEFAULT-NEXT:         let %19 exponent_d: i32 [storage=automatic];
// DEFAULT-NEXT:         let %20 exponent_f: i32 [storage=automatic];
// DEFAULT-NEXT:         let %21 fraction_d: f64 [storage=automatic] = call<f64, signature=fn(f64, ptr<i32>) -> f64>(__builtin_frexp, read<f64, volatile>(%17), addr_of<ptr<i32>>(%19));
// DEFAULT-NEXT:         let %22 fraction_f: f32 [storage=automatic] = call<f32, signature=fn(f32, ptr<i32>) -> f32>(__builtin_frexpf, read<f32, volatile>(%18), addr_of<ptr<i32>>(%20));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(100), add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(10), from_bool<i32, reason=promotion>(eq<f64, exceptions=ignore>(read<f64>(%21), const<f64>(0.75)))), read<i32>(%19))), mul<i32, overflow=ub>(const<i32>(10), from_bool<i32, reason=promotion>(eq<f32, exceptions=ignore>(read<f32>(%22), const<f32>(0.5))))), read<i32>(%20));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @hyperbolic_probe() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %24 input: volatile f64 [storage=automatic] = const<f64>(0.0);
// DEFAULT-NEXT:         let %25 c: f64 [storage=automatic] = call<f64, signature=fn(f64) -> f64>(__builtin_elementwise_cosh, read<f64, volatile>(%24));
// DEFAULT-NEXT:         let %26 s: f64 [storage=automatic] = call<f64, signature=fn(f64) -> f64>(__builtin_elementwise_sinh, read<f64, volatile>(%24));
// DEFAULT-NEXT:         let %27 t: f64 [storage=automatic] = call<f64, signature=fn(f64) -> f64>(__builtin_elementwise_tanh, read<f64, volatile>(%24));
// DEFAULT-NEXT:         let %28 vector_input: volatile vector<f64, 2> [storage=automatic] = aggregate<vector<f64, 2>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(0.0));
// DEFAULT-NEXT:         let %29 vc: vector<f64, 2> [storage=automatic] = call<vector<f64, 2>, signature=fn(vector<f64, 2>) -> vector<f64, 2>, abi=sysv64(direct) -> direct>(__builtin_elementwise_cosh, read<vector<f64, 2>, volatile>(%28));
// DEFAULT-NEXT:         let %30 vs: vector<f64, 2> [storage=automatic] = call<vector<f64, 2>, signature=fn(vector<f64, 2>) -> vector<f64, 2>, abi=sysv64(direct) -> direct>(__builtin_elementwise_sinh, read<vector<f64, 2>, volatile>(%28));
// DEFAULT-NEXT:         let %31 vt: vector<f64, 2> [storage=automatic] = call<vector<f64, 2>, signature=fn(vector<f64, 2>) -> vector<f64, 2>, abi=sysv64(direct) -> direct>(__builtin_elementwise_tanh, read<vector<f64, 2>, volatile>(%28));
// DEFAULT-NEXT:         let %32 vector_ok: i32 [storage=automatic] = from_bool<i32, reason=assign>(logical_and<bool>(logical_and<bool>(logical_and<bool>(logical_and<bool>(logical_and<bool>(eq<f64, exceptions=ignore>(read<f64>(lane(%29, const<i32>(0))), const<f64>(1.0)), eq<f64, exceptions=ignore>(read<f64>(lane(%29, const<i32>(1))), const<f64>(1.0))), eq<f64, exceptions=ignore>(read<f64>(lane(%30, const<i32>(0))), const<f64>(0.0))), eq<f64, exceptions=ignore>(read<f64>(lane(%30, const<i32>(1))), const<f64>(0.0))), eq<f64, exceptions=ignore>(read<f64>(lane(%31, const<i32>(0))), const<f64>(0.0))), eq<f64, exceptions=ignore>(read<f64>(lane(%31, const<i32>(1))), const<f64>(0.0))));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(1000), read<i32>(%32)), mul<i32, overflow=ub>(const<i32>(100), from_bool<i32, reason=promotion>(eq<f64, exceptions=ignore>(read<f64>(%25), const<f64>(1.0))))), mul<i32, overflow=ub>(const<i32>(10), from_bool<i32, reason=promotion>(eq<f64, exceptions=ignore>(read<f64>(%26), const<f64>(0.0))))), from_bool<i32, reason=promotion>(eq<f64, exceptions=ignore>(read<f64>(%27), const<f64>(0.0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %33 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %34 input: volatile i32 [storage=automatic] = const<i32>(7);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%39)), call<i32, signature=fn(i32) -> i32>(%6, read<i32, volatile>(%34)), call<i32, signature=fn() -> i32>(%9), call<i32, signature=fn() -> i32>(%11), call<i32, signature=fn() -> i32>(%16), call<i32, signature=fn() -> i32>(%23));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
