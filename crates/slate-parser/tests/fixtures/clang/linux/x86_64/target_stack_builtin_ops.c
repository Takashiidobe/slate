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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_padded:[0-9]+]] padded = struct {
// DEFAULT-NEXT:         field0 byte: u8;
// DEFAULT-NEXT:         field1 word: u32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_bit_padded:[0-9]+]] bit_padded = struct {
// DEFAULT-NEXT:         field0 low: u8 : 3;
// DEFAULT-NEXT:         field1 <anonymous>: u8 : 2;
// DEFAULT-NEXT:         field2 high: u8 : 3;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0, 0, 0], bit_offsets=[Some(0), Some(3), Some(5)], bit_units=[(0, 1)], field_units=[Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_double2:[0-9]+]] double2 = vector<f64, 2>;
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_memset:[0-9]+]] @memset(%[[VALUE___s:[0-9]+]] __s: ptr<void>, %[[VALUE___c:[0-9]+]] __c: i32, %[[VALUE___n:[0-9]+]] __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin___clear_cache:[0-9]+]] @__builtin___clear_cache(%[[VALUE0:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_prefetch:[0-9]+]] @__builtin_prefetch(%[[VALUE2:[0-9]+]] <unnamed>: ptr<const void>, ...) -> void [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_cache_prefetch_probe:[0-9]+]] @cache_prefetch_probe(%[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_bytes:[0-9]+]] bytes: array<i8, 16> [storage=automatic] [align=16] = aggregate<array<i8, 16>, zero_fill=true>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_bytes]]), const<i32>(0))), truncate<i8, reason=explicit, fits=unknown>(read<i32>(%[[VALUE_x]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ptr<void>) -> void>(%[[VALUE___builtin___clear_cache]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_bytes]])), pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_bytes]]), const<u64>(16))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_bytes]]), const<i32>(1))), const<i32>(0), const<i32>(3));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_bytes]]), const<i32>(0))))), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_frame_address:[0-9]+]] @__builtin_frame_address(%[[VALUE3:[0-9]+]] <unnamed>: u32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_frame_probe:[0-9]+]] @frame_probe() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_frame:[0-9]+]] frame: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(u32) -> ptr<void>>(%[[VALUE___builtin_frame_address]], reinterpret<u32, reason=arg, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<ptr<void>>(read<ptr<void>>(%[[VALUE_frame]]), null<ptr<void>>));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_clear_padding_probe:[0-9]+]] @clear_padding_probe() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_value:[0-9]+]] value: @type[[TYPE_padded]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_bits:[0-9]+]] bits: @type[[TYPE_bit_padded]] [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type[[TYPE_padded]]>>(%[[VALUE_value]])), const<i32>(255), const<u64>(8));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type[[TYPE_bit_padded]]>>(%[[VALUE_bits]])), const<i32>(255), const<u64>(1));
// DEFAULT-NEXT:         write<u8>(field0(%[[VALUE_value]]), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(7))));
// DEFAULT-NEXT:         write<u32>(field1(%[[VALUE_value]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(11)));
// DEFAULT-NEXT:         write<u8>(bitfield0<unit=0, bytes=0..1, bits=0..3>(%[[VALUE_bits]]), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(7))));
// DEFAULT-NEXT:         write<u8>(bitfield2<unit=0, bytes=0..1, bits=5..8>(%[[VALUE_bits]]), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(7))));
// DEFAULT-NEXT:         let %[[VALUE_padding:[0-9]+]] padding: ptr<u8> [storage=automatic] = pointer_cast<ptr<u8>, reason=explicit>(addr_of<ptr<@type[[TYPE_padded]]>>(%[[VALUE_value]]));
// DEFAULT-NEXT:         write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE_padding]]), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE_padding]]), const<i32>(2))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE_padding]]), const<i32>(3))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: ptr<u8> [synthetic] = pointer_cast<ptr<u8>, reason=explicit>(addr_of<ptr<@type[[TYPE_bit_padded]]>>(%[[VALUE_bits]]));
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: u8 [synthetic] = read<u8>(deref(read<ptr<u8>>(%[[VALUE4]])));
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: u8 [synthetic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE5]]))), const<i32>(231))));
// DEFAULT-NEXT:         write<u8>(deref(read<ptr<u8>>(%[[VALUE4]])), read<u8>(%[[VALUE6]]));
// DEFAULT-NEXT:         let %[[VALUE_bytes_2:[0-9]+]] bytes: ptr<u8> [storage=automatic] = pointer_cast<ptr<u8>, reason=explicit>(addr_of<ptr<@type[[TYPE_padded]]>>(%[[VALUE_value]]));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(10), add<i32, overflow=ub>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE_bytes_2]]), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE_bytes_2]]), const<i32>(2))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE_bytes_2]]), const<i32>(3)))))))), from_bool<i32, reason=promotion>(eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(pointer_cast<ptr<u8>, reason=explicit>(addr_of<ptr<@type[[TYPE_bit_padded]]>>(%[[VALUE_bits]])))))), const<i32>(231))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_frexp:[0-9]+]] @__builtin_frexp(%[[VALUE7:[0-9]+]] <unnamed>: f64, %[[VALUE8:[0-9]+]] <unnamed>: ptr<i32>) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_frexpf:[0-9]+]] @__builtin_frexpf(%[[VALUE9:[0-9]+]] <unnamed>: f32, %[[VALUE10:[0-9]+]] <unnamed>: ptr<i32>) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_frexp_probe:[0-9]+]] @frexp_probe() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_input:[0-9]+]] input: volatile f64 [storage=automatic] = const<f64>(12.0);
// DEFAULT-NEXT:         let %[[VALUE_input_f:[0-9]+]] input_f: volatile f32 [storage=automatic] = const<f32>(8.0);
// DEFAULT-NEXT:         let %[[VALUE_exponent_d:[0-9]+]] exponent_d: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_exponent_f:[0-9]+]] exponent_f: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_fraction_d:[0-9]+]] fraction_d: f64 [storage=automatic] = call<f64, signature=fn(f64, ptr<i32>) -> f64>(%[[VALUE___builtin_frexp]], read<f64, volatile>(%[[VALUE_input]]), addr_of<ptr<i32>>(%[[VALUE_exponent_d]]));
// DEFAULT-NEXT:         let %[[VALUE_fraction_f:[0-9]+]] fraction_f: f32 [storage=automatic] = call<f32, signature=fn(f32, ptr<i32>) -> f32>(%[[VALUE___builtin_frexpf]], read<f32, volatile>(%[[VALUE_input_f]]), addr_of<ptr<i32>>(%[[VALUE_exponent_f]]));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(100), add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(10), from_bool<i32, reason=promotion>(eq<f64, exceptions=ignore>(read<f64>(%[[VALUE_fraction_d]]), const<f64>(0.75)))), read<i32>(%[[VALUE_exponent_d]]))), mul<i32, overflow=ub>(const<i32>(10), from_bool<i32, reason=promotion>(eq<f32, exceptions=ignore>(read<f32>(%[[VALUE_fraction_f]]), const<f32>(0.5))))), read<i32>(%[[VALUE_exponent_f]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_elementwise_cosh:[0-9]+]] @__builtin_elementwise_cosh(%[[VALUE11:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_elementwise_sinh:[0-9]+]] @__builtin_elementwise_sinh(%[[VALUE12:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_elementwise_tanh:[0-9]+]] @__builtin_elementwise_tanh(%[[VALUE13:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_elementwise_cosh_2:[0-9]+]] @__builtin_elementwise_cosh(%[[VALUE14:[0-9]+]] <unnamed>: vector<f64, 2>) -> vector<f64, 2> [linkage=external] [memory=none] [abi=sysv64(direct) -> direct];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_elementwise_sinh_2:[0-9]+]] @__builtin_elementwise_sinh(%[[VALUE15:[0-9]+]] <unnamed>: vector<f64, 2>) -> vector<f64, 2> [linkage=external] [memory=none] [abi=sysv64(direct) -> direct];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_elementwise_tanh_2:[0-9]+]] @__builtin_elementwise_tanh(%[[VALUE16:[0-9]+]] <unnamed>: vector<f64, 2>) -> vector<f64, 2> [linkage=external] [memory=none] [abi=sysv64(direct) -> direct];
// DEFAULT-NEXT:     fn %[[VALUE_hyperbolic_probe:[0-9]+]] @hyperbolic_probe() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_input_2:[0-9]+]] input: volatile f64 [storage=automatic] = const<f64>(0.0);
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: f64 [storage=automatic] = call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_elementwise_cosh]], read<f64, volatile>(%[[VALUE_input_2]]));
// DEFAULT-NEXT:         let %[[VALUE_s:[0-9]+]] s: f64 [storage=automatic] = call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_elementwise_sinh]], read<f64, volatile>(%[[VALUE_input_2]]));
// DEFAULT-NEXT:         let %[[VALUE_t:[0-9]+]] t: f64 [storage=automatic] = call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_elementwise_tanh]], read<f64, volatile>(%[[VALUE_input_2]]));
// DEFAULT-NEXT:         let %[[VALUE_vector_input:[0-9]+]] vector_input: volatile vector<f64, 2> [storage=automatic] = aggregate<vector<f64, 2>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(0.0));
// DEFAULT-NEXT:         let %[[VALUE_vc:[0-9]+]] vc: vector<f64, 2> [storage=automatic] = call<vector<f64, 2>, signature=fn(vector<f64, 2>) -> vector<f64, 2>, abi=sysv64(direct) -> direct>(%[[VALUE___builtin_elementwise_cosh_2]], read<vector<f64, 2>, volatile>(%[[VALUE_vector_input]]));
// DEFAULT-NEXT:         let %[[VALUE_vs:[0-9]+]] vs: vector<f64, 2> [storage=automatic] = call<vector<f64, 2>, signature=fn(vector<f64, 2>) -> vector<f64, 2>, abi=sysv64(direct) -> direct>(%[[VALUE___builtin_elementwise_sinh_2]], read<vector<f64, 2>, volatile>(%[[VALUE_vector_input]]));
// DEFAULT-NEXT:         let %[[VALUE_vt:[0-9]+]] vt: vector<f64, 2> [storage=automatic] = call<vector<f64, 2>, signature=fn(vector<f64, 2>) -> vector<f64, 2>, abi=sysv64(direct) -> direct>(%[[VALUE___builtin_elementwise_tanh_2]], read<vector<f64, 2>, volatile>(%[[VALUE_vector_input]]));
// DEFAULT-NEXT:         let %[[VALUE_vector_ok:[0-9]+]] vector_ok: i32 [storage=automatic] = from_bool<i32, reason=assign>(logical_and<bool>(logical_and<bool>(logical_and<bool>(logical_and<bool>(logical_and<bool>(eq<f64, exceptions=ignore>(read<f64>(lane(%[[VALUE_vc]], const<i32>(0))), const<f64>(1.0)), eq<f64, exceptions=ignore>(read<f64>(lane(%[[VALUE_vc]], const<i32>(1))), const<f64>(1.0))), eq<f64, exceptions=ignore>(read<f64>(lane(%[[VALUE_vs]], const<i32>(0))), const<f64>(0.0))), eq<f64, exceptions=ignore>(read<f64>(lane(%[[VALUE_vs]], const<i32>(1))), const<f64>(0.0))), eq<f64, exceptions=ignore>(read<f64>(lane(%[[VALUE_vt]], const<i32>(0))), const<f64>(0.0))), eq<f64, exceptions=ignore>(read<f64>(lane(%[[VALUE_vt]], const<i32>(1))), const<f64>(0.0))));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(1000), read<i32>(%[[VALUE_vector_ok]])), mul<i32, overflow=ub>(const<i32>(100), from_bool<i32, reason=promotion>(eq<f64, exceptions=ignore>(read<f64>(%[[VALUE_c]]), const<f64>(1.0))))), mul<i32, overflow=ub>(const<i32>(10), from_bool<i32, reason=promotion>(eq<f64, exceptions=ignore>(read<f64>(%[[VALUE_s]]), const<f64>(0.0))))), from_bool<i32, reason=promotion>(eq<f64, exceptions=ignore>(read<f64>(%[[VALUE_t]]), const<f64>(0.0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_input_3:[0-9]+]] input: volatile i32 [storage=automatic] = const<i32>(7);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_str]])), call<i32, signature=fn(i32) -> i32>(%[[VALUE_cache_prefetch_probe]], read<i32, volatile>(%[[VALUE_input_3]])), call<i32, signature=fn() -> i32>(%[[VALUE_frame_probe]]), call<i32, signature=fn() -> i32>(%[[VALUE_clear_padding_probe]]), call<i32, signature=fn() -> i32>(%[[VALUE_frexp_probe]]), call<i32, signature=fn() -> i32>(%[[VALUE_hyperbolic_probe]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
