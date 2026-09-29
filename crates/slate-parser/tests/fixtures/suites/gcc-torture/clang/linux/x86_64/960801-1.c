void abort(void);
void exit(int);

unsigned f() {
  long long          l2;
  unsigned short     us;
  unsigned long long ul;
  short              s2;

  ul = us = l2 = s2 = -1;
  return ul;
}

unsigned long long g() {
  long long          l2;
  unsigned short     us;
  unsigned long long ul;
  short              s2;

  ul = us = l2 = s2 = -1;
  return ul;
}

int main(void) {
  if (f() != (unsigned short)-1)
    abort();
  if (g() != (unsigned short)-1)
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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_l2:[0-9]+]] l2: i64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_us:[0-9]+]] us: u16 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ul:[0-9]+]] ul: u64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_s2:[0-9]+]] s2: i16 [storage=automatic];
// DEFAULT-NEXT:         write<i16>(%[[VALUE_s2]], truncate<i16, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         write<i64>(%[[VALUE_l2]], widen<i64, reason=assign>(truncate<i16, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         write<u16>(%[[VALUE_us]], reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(widen<i64, reason=assign>(truncate<i16, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_ul]], widen<u64, reason=assign>(reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(widen<i64, reason=assign>(truncate<i16, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))))));
// DEFAULT-NEXT:         return truncate<u32, reason=return, fits=unknown>(read<u64>(%[[VALUE_ul]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_g:[0-9]+]] @g() -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_l2_2:[0-9]+]] l2: i64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_us_2:[0-9]+]] us: u16 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ul_2:[0-9]+]] ul: u64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_s2_2:[0-9]+]] s2: i16 [storage=automatic];
// DEFAULT-NEXT:         write<i16>(%[[VALUE_s2_2]], truncate<i16, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         write<i64>(%[[VALUE_l2_2]], widen<i64, reason=assign>(truncate<i16, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         write<u16>(%[[VALUE_us_2]], reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(widen<i64, reason=assign>(truncate<i16, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_ul_2]], widen<u64, reason=assign>(reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(widen<i64, reason=assign>(truncate<i16, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))))));
// DEFAULT-NEXT:         return read<u64>(%[[VALUE_ul_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_f]]), reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn() -> u64>(%[[VALUE_g]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
