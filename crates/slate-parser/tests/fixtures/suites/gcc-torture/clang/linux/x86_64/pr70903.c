typedef unsigned char      V8 __attribute__((vector_size(32)));
typedef unsigned int       V32 __attribute__((vector_size(32)));
typedef unsigned long long V64 __attribute__((vector_size(32)));

static V32 __attribute__((noinline, noclone)) foo(V64 x) {
  V64 y = (V64)(V8){((V8)(V64){65535, x[0]})[1]};
  return (V32){y[0], 255};
}

int main() {
  V32 x = foo((V64){});
  //  __builtin_printf ("%08x %08x %08x %08x %08x %08x %08x %08x\n", x[0], x[1],
  //  x[2], x[3], x[4], x[5], x[6], x[7]);
  if (x[1] != 255)
    __builtin_abort();
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
// DEFAULT-NEXT:     type @type[[TYPE_V8:[0-9]+]] V8 = vector<u8, 32>;
// DEFAULT-NEXT:     type @type[[TYPE_V32:[0-9]+]] V32 = vector<u32, 8>;
// DEFAULT-NEXT:     type @type[[TYPE_V64:[0-9]+]] V64 = vector<u64, 4>;
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: vector<u64, 4>) -> vector<u32, 8> [linkage=internal] [inline=never] [definition=emitted] [abi=sysv64(byval<align=32>) -> direct] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y:[0-9]+]] y: vector<u64, 4> [storage=automatic] = vector_bit_cast<vector<u64, 4>, reason=explicit>(read<vector<u8, 32>>(compound_literal %[[VALUE0:[0-9]+]] [storage=automatic] = aggregate<vector<u8, 32>, zero_fill=true>(index0 = lane<u8>(vector_bit_cast<vector<u8, 32>, reason=explicit>(read<vector<u64, 4>>(compound_literal %[[VALUE1:[0-9]+]] [storage=automatic] = aggregate<vector<u64, 4>, zero_fill=true>(index0 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(65535))), index1 = read<u64>(lane(%[[VALUE_x]], const<i32>(0)))))), const<i32>(1)))));
// DEFAULT-NEXT:         return read<vector<u32, 8>>(compound_literal %[[VALUE2:[0-9]+]] [storage=automatic] = aggregate<vector<u32, 8>, zero_fill=true>(index0 = truncate<u32, reason=assign, fits=unknown>(read<u64>(lane(%[[VALUE_y]], const<i32>(0)))), index1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(255))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_x_2:[0-9]+]] x: vector<u32, 8> [storage=automatic] = call<vector<u32, 8>, signature=fn(vector<u64, 4>) -> vector<u32, 8>, abi=sysv64(byval<align=32>) -> direct>(%[[VALUE_foo]], read<vector<u64, 4>>(compound_literal %[[VALUE3:[0-9]+]] [storage=automatic] = aggregate<vector<u64, 4>, zero_fill=true>()));
// DEFAULT-NEXT:         if ne<u32>(read<u32>(lane(%[[VALUE_x_2]], const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(255)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
