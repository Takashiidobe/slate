/* PR target/107131 */

__attribute__((noipa)) unsigned long long foo(unsigned char o) {
  unsigned long long t1 = -(long long)(o == 0);
  unsigned long long t2 = -(long long)(t1 > 10439075533421201520ULL);
  unsigned long long t3 = -(long long)(t1 <= t2);
  return t3;
}

int main() {
  if (foo(0) != -1ULL)
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
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_o:[0-9]+]] o: u8) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_t1:[0-9]+]] t1: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(neg<i64, overflow=ub>(from_bool<i64, reason=explicit>(eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_o]]))), const<i32>(0)))));
// DEFAULT-NEXT:         let %[[VALUE_t2:[0-9]+]] t2: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(neg<i64, overflow=ub>(from_bool<i64, reason=explicit>(gt<u64>(read<u64>(%[[VALUE_t1]]), const<u64>(10439075533421201520)))));
// DEFAULT-NEXT:         let %[[VALUE_t3:[0-9]+]] t3: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(neg<i64, overflow=ub>(from_bool<i64, reason=explicit>(le<u64>(read<u64>(%[[VALUE_t1]]), read<u64>(%[[VALUE_t2]])))));
// DEFAULT-NEXT:         return read<u64>(%[[VALUE_t3]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(u8) -> u64>(%[[VALUE_foo]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(0)))), neg<u64, overflow=wrap>(const<u64>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
