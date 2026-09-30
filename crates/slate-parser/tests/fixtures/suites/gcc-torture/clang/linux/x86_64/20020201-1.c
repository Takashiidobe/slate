/* Test whether division by constant works properly.  */

extern void abort(void);
extern void exit(int);

unsigned char      cx = 7;
unsigned short     sx = 14;
unsigned int       ix = 21;
unsigned long      lx = 28;
unsigned long long Lx = 35;

int main() {
  unsigned char      cy;
  unsigned short     sy;
  unsigned int       iy;
  unsigned long      ly;
  unsigned long long Ly;

  cy = cx / 6;
  if (cy != 1)
    abort();
  cy = cx % 6;
  if (cy != 1)
    abort();

  sy = sx / 6;
  if (sy != 2)
    abort();
  sy = sx % 6;
  if (sy != 2)
    abort();

  iy = ix / 6;
  if (iy != 3)
    abort();
  iy = ix % 6;
  if (iy != 3)
    abort();

  ly = lx / 6;
  if (ly != 4)
    abort();
  ly = lx % 6;
  if (ly != 4)
    abort();

  Ly = Lx / 6;
  if (Ly != 5)
    abort();
  Ly = Lx % 6;
  if (Ly != 5)
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
// DEFAULT-NEXT:     global %[[VALUE_cx:[0-9]+]] cx: u8 [storage=static] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(7))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_sx:[0-9]+]] sx: u16 [storage=static] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(14))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ix:[0-9]+]] ix: u32 [storage=static] = reinterpret<u32, reason=assign, fits=always>(const<i32>(21)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_lx:[0-9]+]] lx: u64 [storage=static] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(28))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_Lx:[0-9]+]] Lx: u64 [storage=static] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(35))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_cy:[0-9]+]] cy: u8 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_sy:[0-9]+]] sy: u16 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_iy:[0-9]+]] iy: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ly:[0-9]+]] ly: u64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_Ly:[0-9]+]] Ly: u64 [storage=automatic];
// DEFAULT-NEXT:         write<u8>(%[[VALUE_cy]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(div<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_cx]]))), const<i32>(6)))));
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_cy]]))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u8>(%[[VALUE_cy]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_cx]]))), const<i32>(6)))));
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_cy]]))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u16>(%[[VALUE_sy]], reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(div<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_sx]]))), const<i32>(6)))));
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_sy]]))), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u16>(%[[VALUE_sy]], reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_sx]]))), const<i32>(6)))));
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_sy]]))), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_iy]], div<u32, by_zero=ub>(read<u32>(%[[VALUE_ix]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(6))));
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%[[VALUE_iy]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(3)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_iy]], rem<u32, by_zero=ub>(read<u32>(%[[VALUE_ix]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(6))));
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%[[VALUE_iy]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(3)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u64>(%[[VALUE_ly]], div<u64, by_zero=ub>(read<u64>(%[[VALUE_lx]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(6)))));
// DEFAULT-NEXT:         if ne<u64>(read<u64>(%[[VALUE_ly]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u64>(%[[VALUE_ly]], rem<u64, by_zero=ub>(read<u64>(%[[VALUE_lx]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(6)))));
// DEFAULT-NEXT:         if ne<u64>(read<u64>(%[[VALUE_ly]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u64>(%[[VALUE_Ly]], div<u64, by_zero=ub>(read<u64>(%[[VALUE_Lx]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(6)))));
// DEFAULT-NEXT:         if ne<u64>(read<u64>(%[[VALUE_Ly]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(5))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u64>(%[[VALUE_Ly]], rem<u64, by_zero=ub>(read<u64>(%[[VALUE_Lx]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(6)))));
// DEFAULT-NEXT:         if ne<u64>(read<u64>(%[[VALUE_Ly]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(5))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
