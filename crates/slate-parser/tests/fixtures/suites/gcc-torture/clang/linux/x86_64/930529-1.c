/* { dg-options { "-fwrapv" } } */

extern void abort(void);
extern void exit(int);

int dd(int x, int d) { return x / d; }

int main() {
  int i;
  for (i = -3; i <= 3; i++) {
    if (dd(i, 1) != i / 1)
      abort();
    if (dd(i, 2) != i / 2)
      abort();
    if (dd(i, 3) != i / 3)
      abort();
    if (dd(i, 4) != i / 4)
      abort();
    if (dd(i, 5) != i / 5)
      abort();
    if (dd(i, 6) != i / 6)
      abort();
    if (dd(i, 7) != i / 7)
      abort();
    if (dd(i, 8) != i / 8)
      abort();
  }
  for (i = ((unsigned)~0 >> 1) - 3; i <= ((unsigned)~0 >> 1) + 3; i++) {
    if (dd(i, 1) != i / 1)
      abort();
    if (dd(i, 2) != i / 2)
      abort();
    if (dd(i, 3) != i / 3)
      abort();
    if (dd(i, 4) != i / 4)
      abort();
    if (dd(i, 5) != i / 5)
      abort();
    if (dd(i, 6) != i / 6)
      abort();
    if (dd(i, 7) != i / 7)
      abort();
    if (dd(i, 8) != i / 8)
      abort();
  }
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
// DEFAULT-NEXT:     fn %[[VALUE_dd:[0-9]+]] @dd(%[[VALUE_x:[0-9]+]] x: i32, %[[VALUE_d:[0-9]+]] d: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_x]]), read<i32>(%[[VALUE_d]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], neg<i32, overflow=ub>(const<i32>(3)));
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%[[VALUE_i]]), const<i32>(3))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_dd]], read<i32>(%[[VALUE_i]]), const<i32>(1)), div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_i]]), const<i32>(1)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_dd]], read<i32>(%[[VALUE_i]]), const<i32>(2)), div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_i]]), const<i32>(2)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_dd]], read<i32>(%[[VALUE_i]]), const<i32>(3)), div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_i]]), const<i32>(3)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_dd]], read<i32>(%[[VALUE_i]]), const<i32>(4)), div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_i]]), const<i32>(4)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_dd]], read<i32>(%[[VALUE_i]]), const<i32>(5)), div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_i]]), const<i32>(5)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_dd]], read<i32>(%[[VALUE_i]]), const<i32>(6)), div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_i]]), const<i32>(6)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_dd]], read<i32>(%[[VALUE_i]]), const<i32>(7)), div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_i]]), const<i32>(7)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_dd]], read<i32>(%[[VALUE_i]]), const<i32>(8)), div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_i]]), const<i32>(8)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %[[VALUE4:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], reinterpret<i32, reason=assign, fits=unknown>(sub<u32, overflow=wrap>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(reinterpret<u32, reason=explicit, fits=unknown>(not<i32>(const<i32>(0))), const<i32>(1)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(3)))));
// DEFAULT-NEXT:             condition: le<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%[[VALUE_i]])), add<u32, overflow=wrap>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(reinterpret<u32, reason=explicit, fits=unknown>(not<i32>(const<i32>(0))), const<i32>(1)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(3))))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE5]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE6]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_dd]], read<i32>(%[[VALUE_i]]), const<i32>(1)), div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_i]]), const<i32>(1)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_dd]], read<i32>(%[[VALUE_i]]), const<i32>(2)), div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_i]]), const<i32>(2)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_dd]], read<i32>(%[[VALUE_i]]), const<i32>(3)), div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_i]]), const<i32>(3)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_dd]], read<i32>(%[[VALUE_i]]), const<i32>(4)), div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_i]]), const<i32>(4)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_dd]], read<i32>(%[[VALUE_i]]), const<i32>(5)), div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_i]]), const<i32>(5)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_dd]], read<i32>(%[[VALUE_i]]), const<i32>(6)), div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_i]]), const<i32>(6)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_dd]], read<i32>(%[[VALUE_i]]), const<i32>(7)), div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_i]]), const<i32>(7)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_dd]], read<i32>(%[[VALUE_i]]), const<i32>(8)), div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_i]]), const<i32>(8)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
