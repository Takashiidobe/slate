// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

typedef int v4si __attribute__((vector_size(16)));
typedef unsigned char v16qu __attribute__((vector_size(16)));
typedef double v2df __attribute__((vector_size(16)));

int integer_reductions(v4si v) {
  return __builtin_reduce_add(v) + __builtin_reduce_mul(v) + __builtin_reduce_and(v)
       + __builtin_reduce_or(v) + __builtin_reduce_xor(v);
}

unsigned char byte_sum(v16qu v) {
  return __builtin_reduce_add(v);
}

int ordered_int(v4si v) {
  return __builtin_reduce_max(v) - __builtin_reduce_min(v);
}

double ordered_double(v2df v) {
  return __builtin_reduce_max(v) + __builtin_reduce_min(v) + __builtin_reduce_maximum(v)
       + __builtin_reduce_minimum(v);
}

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type[[TYPE_v4si:[0-9]+]] v4si = vector<i32, 4>;
// IR-NEXT:     type @type[[TYPE_v16qu:[0-9]+]] v16qu = vector<u8, 16>;
// IR-NEXT:     type @type[[TYPE_v2df:[0-9]+]] v2df = vector<f64, 2>;
// IR-NEXT:     fn %[[VALUE___builtin_reduce_add:[0-9]+]] @__builtin_reduce_add(%[[VALUE0:[0-9]+]] <unnamed>: vector<i32, 4>) -> i32 [linkage=external] [memory=none] [abi=sysv64(direct) -> scalar];
// IR-NEXT:     fn %[[VALUE___builtin_reduce_mul:[0-9]+]] @__builtin_reduce_mul(%[[VALUE1:[0-9]+]] <unnamed>: vector<i32, 4>) -> i32 [linkage=external] [memory=none] [abi=sysv64(direct) -> scalar];
// IR-NEXT:     fn %[[VALUE___builtin_reduce_and:[0-9]+]] @__builtin_reduce_and(%[[VALUE2:[0-9]+]] <unnamed>: vector<i32, 4>) -> i32 [linkage=external] [memory=none] [abi=sysv64(direct) -> scalar];
// IR-NEXT:     fn %[[VALUE___builtin_reduce_or:[0-9]+]] @__builtin_reduce_or(%[[VALUE3:[0-9]+]] <unnamed>: vector<i32, 4>) -> i32 [linkage=external] [memory=none] [abi=sysv64(direct) -> scalar];
// IR-NEXT:     fn %[[VALUE___builtin_reduce_xor:[0-9]+]] @__builtin_reduce_xor(%[[VALUE4:[0-9]+]] <unnamed>: vector<i32, 4>) -> i32 [linkage=external] [memory=none] [abi=sysv64(direct) -> scalar];
// IR-NEXT:     fn %[[VALUE_integer_reductions:[0-9]+]] @integer_reductions(%[[VALUE_v:[0-9]+]] v: vector<i32, 4>) -> i32 [linkage=external] [abi=sysv64(direct) -> scalar] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(call<i32, signature=fn(vector<i32, 4>) -> i32, abi=sysv64(direct) -> scalar>(%[[VALUE___builtin_reduce_add]], read<vector<i32, 4>>(%[[VALUE_v]])), call<i32, signature=fn(vector<i32, 4>) -> i32, abi=sysv64(direct) -> scalar>(%[[VALUE___builtin_reduce_mul]], read<vector<i32, 4>>(%[[VALUE_v]]))), call<i32, signature=fn(vector<i32, 4>) -> i32, abi=sysv64(direct) -> scalar>(%[[VALUE___builtin_reduce_and]], read<vector<i32, 4>>(%[[VALUE_v]]))), call<i32, signature=fn(vector<i32, 4>) -> i32, abi=sysv64(direct) -> scalar>(%[[VALUE___builtin_reduce_or]], read<vector<i32, 4>>(%[[VALUE_v]]))), call<i32, signature=fn(vector<i32, 4>) -> i32, abi=sysv64(direct) -> scalar>(%[[VALUE___builtin_reduce_xor]], read<vector<i32, 4>>(%[[VALUE_v]])));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_byte_sum:[0-9]+]] @byte_sum(%[[VALUE_v_2:[0-9]+]] v: vector<u8, 16>) -> u8 [linkage=external] [abi=sysv64(direct) -> scalar] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<u8, signature=fn(vector<u8, 16>) -> u8, abi=sysv64(direct) -> scalar>(%[[VALUE___builtin_reduce_add]], read<vector<u8, 16>>(%[[VALUE_v_2]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE___builtin_reduce_max:[0-9]+]] @__builtin_reduce_max(%[[VALUE5:[0-9]+]] <unnamed>: vector<i32, 4>) -> i32 [linkage=external] [memory=none] [abi=sysv64(direct) -> scalar];
// IR-NEXT:     fn %[[VALUE___builtin_reduce_min:[0-9]+]] @__builtin_reduce_min(%[[VALUE6:[0-9]+]] <unnamed>: vector<i32, 4>) -> i32 [linkage=external] [memory=none] [abi=sysv64(direct) -> scalar];
// IR-NEXT:     fn %[[VALUE_ordered_int:[0-9]+]] @ordered_int(%[[VALUE_v_3:[0-9]+]] v: vector<i32, 4>) -> i32 [linkage=external] [abi=sysv64(direct) -> scalar] [fallthrough=ub_if_used] {
// IR-NEXT:         return sub<i32, overflow=ub>(call<i32, signature=fn(vector<i32, 4>) -> i32, abi=sysv64(direct) -> scalar>(%[[VALUE___builtin_reduce_max]], read<vector<i32, 4>>(%[[VALUE_v_3]])), call<i32, signature=fn(vector<i32, 4>) -> i32, abi=sysv64(direct) -> scalar>(%[[VALUE___builtin_reduce_min]], read<vector<i32, 4>>(%[[VALUE_v_3]])));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE___builtin_reduce_maximum:[0-9]+]] @__builtin_reduce_maximum(%[[VALUE7:[0-9]+]] <unnamed>: vector<f64, 2>) -> f64 [linkage=external] [memory=none] [abi=sysv64(direct) -> scalar];
// IR-NEXT:     fn %[[VALUE___builtin_reduce_minimum:[0-9]+]] @__builtin_reduce_minimum(%[[VALUE8:[0-9]+]] <unnamed>: vector<f64, 2>) -> f64 [linkage=external] [memory=none] [abi=sysv64(direct) -> scalar];
// IR-NEXT:     fn %[[VALUE_ordered_double:[0-9]+]] @ordered_double(%[[VALUE_v_4:[0-9]+]] v: vector<f64, 2>) -> f64 [linkage=external] [abi=sysv64(direct) -> scalar] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(call<f64, signature=fn(vector<f64, 2>) -> f64, abi=sysv64(direct) -> scalar>(%[[VALUE___builtin_reduce_max]], read<vector<f64, 2>>(%[[VALUE_v_4]])), call<f64, signature=fn(vector<f64, 2>) -> f64, abi=sysv64(direct) -> scalar>(%[[VALUE___builtin_reduce_min]], read<vector<f64, 2>>(%[[VALUE_v_4]]))), call<f64, signature=fn(vector<f64, 2>) -> f64, abi=sysv64(direct) -> scalar>(%[[VALUE___builtin_reduce_maximum]], read<vector<f64, 2>>(%[[VALUE_v_4]]))), call<f64, signature=fn(vector<f64, 2>) -> f64, abi=sysv64(direct) -> scalar>(%[[VALUE___builtin_reduce_minimum]], read<vector<f64, 2>>(%[[VALUE_v_4]])));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
