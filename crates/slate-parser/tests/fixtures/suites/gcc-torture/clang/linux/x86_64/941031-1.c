void abort(void);
void exit(int);

typedef long mpt;

int f(mpt us, mpt vs) {
  long aus;
  long avs;

  aus = us >= 0 ? us : -us;
  avs = vs >= 0 ? vs : -vs;

  if (aus < avs) {
    long t = aus;
    aus    = avs;
    avs    = aus;
  }

  return avs;
}

int main(void) {
  if (f((mpt)3, (mpt)17) != 17)
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
// DEFAULT-NEXT:     type @type[[TYPE_mpt:[0-9]+]] mpt = i64;
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_us:[0-9]+]] us: i64, %[[VALUE_vs:[0-9]+]] vs: i64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_aus:[0-9]+]] aus: i64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_avs:[0-9]+]] avs: i64 [storage=automatic];
// DEFAULT-NEXT:         write<i64>(%[[VALUE_aus]], conditional<i64>(ge<i64>(read<i64>(%[[VALUE_us]]), widen<i64, reason=usual_arith>(const<i32>(0))), read<i64>(%[[VALUE_us]]), neg<i64, overflow=ub>(read<i64>(%[[VALUE_us]]))));
// DEFAULT-NEXT:         write<i64>(%[[VALUE_avs]], conditional<i64>(ge<i64>(read<i64>(%[[VALUE_vs]]), widen<i64, reason=usual_arith>(const<i32>(0))), read<i64>(%[[VALUE_vs]]), neg<i64, overflow=ub>(read<i64>(%[[VALUE_vs]]))));
// DEFAULT-NEXT:         if lt<i64>(read<i64>(%[[VALUE_aus]]), read<i64>(%[[VALUE_avs]]))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_t:[0-9]+]] t: i64 [storage=automatic] = read<i64>(%[[VALUE_aus]]);
// DEFAULT-NEXT:                 write<i64>(%[[VALUE_aus]], read<i64>(%[[VALUE_avs]]));
// DEFAULT-NEXT:                 write<i64>(%[[VALUE_avs]], read<i64>(%[[VALUE_aus]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return truncate<i32, reason=return, fits=unknown>(read<i64>(%[[VALUE_avs]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64, i64) -> i32>(%[[VALUE_f]], widen<i64, reason=explicit>(const<i32>(3)), widen<i64, reason=explicit>(const<i32>(17))), const<i32>(17))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
