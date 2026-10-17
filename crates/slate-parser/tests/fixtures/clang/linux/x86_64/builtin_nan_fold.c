float quiet_f = __builtin_nanf("");
float signaling_payload_f = __builtin_nansf("0x15");
double quiet_payload_d = __builtin_nan("0123");
double signaling_d = __builtin_nans("");
long double signaling_payload_l = __builtin_nansl("7");
_Float16 quiet_h = __builtin_nanf16("3");
__float128 signaling_q = __builtin_nansf128("1");
double dynamic_payload(const char *tag) { return __builtin_nan(tag); }
double invalid_payload = __builtin_nan("junk");

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
// DEFAULT-NEXT:     global %[[VALUE_quiet_f:[0-9]+]] quiet_f: f32 [storage=static] = const<f32>(bits=0x7fc00000) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_signaling_payload_f:[0-9]+]] signaling_payload_f: f32 [storage=static] = const<f32>(bits=0x7f800015) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_quiet_payload_d:[0-9]+]] quiet_payload_d: f64 [storage=static] = const<f64>(bits=0x7ff8000000000053) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_signaling_d:[0-9]+]] signaling_d: f64 [storage=static] = const<f64>(bits=0x7ff4000000000000) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_signaling_payload_l:[0-9]+]] signaling_payload_l: f80 [storage=static] = const<f80>(bits=0x7fff8000000000000007) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_quiet_h:[0-9]+]] quiet_h: f16 [storage=static] = const<f16>(bits=0x7e03) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_signaling_q:[0-9]+]] signaling_q: f128 [storage=static] = const<f128>(bits=0x7fff0000000000000000000000000001) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([106, 117, 110, 107, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_invalid_payload:[0-9]+]] invalid_payload: f64 [storage=static] = call<f64, signature=fn(ptr<const i8>) -> f64>(%[[VALUE___builtin_nan:[0-9]+]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str]]))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nan]] @__builtin_nan(%[[VALUE0:[0-9]+]] <unnamed>: ptr<const i8>) -> f64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_dynamic_payload:[0-9]+]] @dynamic_payload(%[[VALUE_tag:[0-9]+]] tag: ptr<const i8>) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(ptr<const i8>) -> f64>(%[[VALUE___builtin_nan]], read<ptr<const i8>>(%[[VALUE_tag]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
