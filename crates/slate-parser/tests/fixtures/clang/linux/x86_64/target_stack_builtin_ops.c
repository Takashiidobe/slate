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
// DEFAULT-NEXT:     global %62 .str62: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %2 @printf(%39 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %6 @memset(%40 __s: ptr<void>, %41 __c: i32, %42 __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %45 @__builtin___clear_cache(%43 <unnamed>: ptr<void>, %44 <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %47 @__builtin_prefetch(%46 <unnamed>: ptr<const void>, ...) -> void [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %10 @cache_prefetch_probe(%11 x: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %12 bytes: array<i8, 16> [storage=automatic] [align=16] = aggregate<array<i8, 16>, zero_fill=true>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(16)>(%12), const<i32>(0))), truncate<i8, reason=explicit, fits=unknown>(read<i32>(%11)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ptr<void>) -> void>(%45, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%12)), pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(16)>(%12), const<u64>(16))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%47, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(16)>(%12), const<i32>(1))), const<i32>(0), const<i32>(3));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(16)>(%12), const<i32>(0))))), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %49 @__builtin_frame_address(%48 <unnamed>: u32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %13 @frame_probe() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %14 frame: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(u32) -> ptr<void>>(%49, reinterpret<u32, reason=arg, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<ptr<void>>(read<ptr<void>>(%14), null<ptr<void>>));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @clear_padding_probe() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %16 value: @type1 [storage=automatic];
// DEFAULT-NEXT:         let %17 bits: @type2 [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%6, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type1>>(%16)), const<i32>(255), const<u64>(8));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%6, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type2>>(%17)), const<i32>(255), const<u64>(1));
// DEFAULT-NEXT:         write<u8>(field0(%16), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(7))));
// DEFAULT-NEXT:         write<u32>(field1(%16), reinterpret<u32, reason=assign, fits=always>(const<i32>(11)));
// DEFAULT-NEXT:         write<u8>(bitfield0<unit=0, bytes=0..1, bits=0..3>(%17), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(7))));
// DEFAULT-NEXT:         write<u8>(bitfield2<unit=0, bytes=0..1, bits=5..8>(%17), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(7))));
// DEFAULT-NEXT:         let %18 padding: ptr<u8> [storage=automatic] = pointer_cast<ptr<u8>, reason=explicit>(addr_of<ptr<@type1>>(%16));
// DEFAULT-NEXT:         write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%18), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%18), const<i32>(2))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%18), const<i32>(3))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         let %63: ptr<u8> [synthetic] = pointer_cast<ptr<u8>, reason=explicit>(addr_of<ptr<@type2>>(%17));
// DEFAULT-NEXT:         let %64: u8 [synthetic] = read<u8>(deref(read<ptr<u8>>(%63)));
// DEFAULT-NEXT:         let %65: u8 [synthetic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%64))), const<i32>(231))));
// DEFAULT-NEXT:         write<u8>(deref(read<ptr<u8>>(%63)), read<u8>(%65));
// DEFAULT-NEXT:         let %19 bytes: ptr<u8> [storage=automatic] = pointer_cast<ptr<u8>, reason=explicit>(addr_of<ptr<@type1>>(%16));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(10), add<i32, overflow=ub>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%19), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%19), const<i32>(2))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%19), const<i32>(3)))))))), from_bool<i32, reason=promotion>(eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(pointer_cast<ptr<u8>, reason=explicit>(addr_of<ptr<@type2>>(%17)))))), const<i32>(231))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %52 @__builtin_frexp(%50 <unnamed>: f64, %51 <unnamed>: ptr<i32>) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %55 @__builtin_frexpf(%53 <unnamed>: f32, %54 <unnamed>: ptr<i32>) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %20 @frexp_probe() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %21 input: volatile f64 [storage=automatic] = const<f64>(12.0);
// DEFAULT-NEXT:         let %22 input_f: volatile f32 [storage=automatic] = const<f32>(8.0);
// DEFAULT-NEXT:         let %23 exponent_d: i32 [storage=automatic];
// DEFAULT-NEXT:         let %24 exponent_f: i32 [storage=automatic];
// DEFAULT-NEXT:         let %25 fraction_d: f64 [storage=automatic] = call<f64, signature=fn(f64, ptr<i32>) -> f64>(%52, read<f64, volatile>(%21), addr_of<ptr<i32>>(%23));
// DEFAULT-NEXT:         let %26 fraction_f: f32 [storage=automatic] = call<f32, signature=fn(f32, ptr<i32>) -> f32>(%55, read<f32, volatile>(%22), addr_of<ptr<i32>>(%24));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(100), add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(10), from_bool<i32, reason=promotion>(eq<f64, exceptions=ignore>(read<f64>(%25), const<f64>(0.75)))), read<i32>(%23))), mul<i32, overflow=ub>(const<i32>(10), from_bool<i32, reason=promotion>(eq<f32, exceptions=ignore>(read<f32>(%26), const<f32>(0.5))))), read<i32>(%24));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %57 @__builtin_elementwise_cosh(%56 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %59 @__builtin_elementwise_sinh(%58 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %61 @__builtin_elementwise_tanh(%60 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %27 @hyperbolic_probe() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %28 input: volatile f64 [storage=automatic] = const<f64>(0.0);
// DEFAULT-NEXT:         let %29 c: f64 [storage=automatic] = call<f64, signature=fn(f64) -> f64>(%57, read<f64, volatile>(%28));
// DEFAULT-NEXT:         let %30 s: f64 [storage=automatic] = call<f64, signature=fn(f64) -> f64>(%59, read<f64, volatile>(%28));
// DEFAULT-NEXT:         let %31 t: f64 [storage=automatic] = call<f64, signature=fn(f64) -> f64>(%61, read<f64, volatile>(%28));
// DEFAULT-NEXT:         let %32 vector_input: volatile vector<f64, 2> [storage=automatic] = aggregate<vector<f64, 2>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(0.0));
// DEFAULT-NEXT:         let %33 vc: vector<f64, 2> [storage=automatic] = call<vector<f64, 2>, signature=fn(vector<f64, 2>) -> vector<f64, 2>, abi=sysv64(direct) -> direct>(%57, read<vector<f64, 2>, volatile>(%32));
// DEFAULT-NEXT:         let %34 vs: vector<f64, 2> [storage=automatic] = call<vector<f64, 2>, signature=fn(vector<f64, 2>) -> vector<f64, 2>, abi=sysv64(direct) -> direct>(%59, read<vector<f64, 2>, volatile>(%32));
// DEFAULT-NEXT:         let %35 vt: vector<f64, 2> [storage=automatic] = call<vector<f64, 2>, signature=fn(vector<f64, 2>) -> vector<f64, 2>, abi=sysv64(direct) -> direct>(%61, read<vector<f64, 2>, volatile>(%32));
// DEFAULT-NEXT:         let %36 vector_ok: i32 [storage=automatic] = from_bool<i32, reason=assign>(logical_and<bool>(logical_and<bool>(logical_and<bool>(logical_and<bool>(logical_and<bool>(eq<f64, exceptions=ignore>(read<f64>(lane(%33, const<i32>(0))), const<f64>(1.0)), eq<f64, exceptions=ignore>(read<f64>(lane(%33, const<i32>(1))), const<f64>(1.0))), eq<f64, exceptions=ignore>(read<f64>(lane(%34, const<i32>(0))), const<f64>(0.0))), eq<f64, exceptions=ignore>(read<f64>(lane(%34, const<i32>(1))), const<f64>(0.0))), eq<f64, exceptions=ignore>(read<f64>(lane(%35, const<i32>(0))), const<f64>(0.0))), eq<f64, exceptions=ignore>(read<f64>(lane(%35, const<i32>(1))), const<f64>(0.0))));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(1000), read<i32>(%36)), mul<i32, overflow=ub>(const<i32>(100), from_bool<i32, reason=promotion>(eq<f64, exceptions=ignore>(read<f64>(%29), const<f64>(1.0))))), mul<i32, overflow=ub>(const<i32>(10), from_bool<i32, reason=promotion>(eq<f64, exceptions=ignore>(read<f64>(%30), const<f64>(0.0))))), from_bool<i32, reason=promotion>(eq<f64, exceptions=ignore>(read<f64>(%31), const<f64>(0.0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %37 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %38 input: volatile i32 [storage=automatic] = const<i32>(7);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%2, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%62)), call<i32, signature=fn(i32) -> i32>(%10, read<i32, volatile>(%38)), call<i32, signature=fn() -> i32>(%13), call<i32, signature=fn() -> i32>(%15), call<i32, signature=fn() -> i32>(%20), call<i32, signature=fn() -> i32>(%27));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
