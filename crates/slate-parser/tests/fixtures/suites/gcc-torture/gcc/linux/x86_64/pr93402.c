/* PR rtl-optimization/93402 */

struct S {
  unsigned int       a;
  unsigned long long b;
};

__attribute__((noipa)) struct S foo(unsigned long long x) {
  struct S ret;
  ret.a = 0;
  ret.b = x * 11111111111ULL + 111111111111ULL;
  return ret;
}

int main() {
  struct S a = foo(1);
  if (a.a != 0 || a.b != 122222222222ULL)
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
// DEFAULT-NEXT:     type @type0 S = struct {
// DEFAULT-NEXT:         field0 a: u32;
// DEFAULT-NEXT:         field1 b: u64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     fn %1 @foo(%2 x: u64) -> @type0 [linkage=external] [abi=sysv64(scalar) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 ret: @type0 [storage=automatic];
// DEFAULT-NEXT:         write<u32>(field0(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<u64>(field1(%3), add<u64, overflow=wrap>(mul<u64, overflow=wrap>(read<u64>(%2), const<u64>(11111111111)), const<u64>(111111111111)));
// DEFAULT-NEXT:         return copy<@type0, reason=return>(read<@type0>(%3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %5 a: @type0 [storage=automatic] = copy<@type0, reason=assign>(call<@type0, signature=fn(u64) -> @type0, abi=sysv64(scalar) -> native_c>(%1, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(read<u32>(field0(%5)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), ne<u64>(read<u64>(field1(%5)), const<u64>(122222222222)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
