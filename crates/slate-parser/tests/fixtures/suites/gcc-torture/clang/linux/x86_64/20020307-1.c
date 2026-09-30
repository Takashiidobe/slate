void abort(void);
void exit(int);

#define MASK(N) ((1UL << (N)) - 1)
#define BITS(N) ((1UL << ((N) - 1)) + 2)

#define FUNC(N)                                                                \
  void f##N(long j) {                                                          \
    if ((j & MASK(N)) >= BITS(N))                                              \
      abort();                                                                 \
  }

FUNC(3)
FUNC(4)
FUNC(5)
FUNC(6)
FUNC(7)
FUNC(8)
FUNC(9)
FUNC(10)
FUNC(11)
FUNC(12)
FUNC(13)
FUNC(14)
FUNC(15)
FUNC(16)
FUNC(17)
FUNC(18)
FUNC(19)
FUNC(20)
FUNC(21)
FUNC(22)
FUNC(23)
FUNC(24)
FUNC(25)
FUNC(26)
FUNC(27)
FUNC(28)
FUNC(29)
FUNC(30)
FUNC(31)

int main() {
  f3(0);
  f4(0);
  f5(0);
  f6(0);
  f7(0);
  f8(0);
  f9(0);
  f10(0);
  f11(0);
  f12(0);
  f13(0);
  f14(0);
  f15(0);
  f16(0);
  f17(0);
  f18(0);
  f19(0);
  f20(0);
  f21(0);
  f22(0);
  f23(0);
  f24(0);
  f25(0);
  f26(0);
  f27(0);
  f28(0);
  f29(0);
  f30(0);
  f31(0);

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
// DEFAULT-NEXT:     fn %[[VALUE_f3:[0-9]+]] @f3(%[[VALUE_j:[0-9]+]] j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE_j]])), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(3)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(3), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f4:[0-9]+]] @f4(%[[VALUE_j_2:[0-9]+]] j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE_j_2]])), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(4)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(4), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f5:[0-9]+]] @f5(%[[VALUE_j_3:[0-9]+]] j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE_j_3]])), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(5)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(5), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f6:[0-9]+]] @f6(%[[VALUE_j_4:[0-9]+]] j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE_j_4]])), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(6)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(6), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f7:[0-9]+]] @f7(%[[VALUE_j_5:[0-9]+]] j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE_j_5]])), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(7)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(7), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f8:[0-9]+]] @f8(%[[VALUE_j_6:[0-9]+]] j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE_j_6]])), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(8)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(8), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f9:[0-9]+]] @f9(%[[VALUE_j_7:[0-9]+]] j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE_j_7]])), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(9)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(9), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f10:[0-9]+]] @f10(%[[VALUE_j_8:[0-9]+]] j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE_j_8]])), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(10)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(10), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f11:[0-9]+]] @f11(%[[VALUE_j_9:[0-9]+]] j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE_j_9]])), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(11)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(11), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f12:[0-9]+]] @f12(%[[VALUE_j_10:[0-9]+]] j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE_j_10]])), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(12)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(12), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f13:[0-9]+]] @f13(%[[VALUE_j_11:[0-9]+]] j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE_j_11]])), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(13)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(13), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f14:[0-9]+]] @f14(%[[VALUE_j_12:[0-9]+]] j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE_j_12]])), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(14)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(14), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f15:[0-9]+]] @f15(%[[VALUE_j_13:[0-9]+]] j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE_j_13]])), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(15)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(15), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f16:[0-9]+]] @f16(%[[VALUE_j_14:[0-9]+]] j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE_j_14]])), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(16)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(16), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f17:[0-9]+]] @f17(%[[VALUE_j_15:[0-9]+]] j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE_j_15]])), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(17)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(17), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f18:[0-9]+]] @f18(%[[VALUE_j_16:[0-9]+]] j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE_j_16]])), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(18)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(18), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f19:[0-9]+]] @f19(%[[VALUE_j_17:[0-9]+]] j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE_j_17]])), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(19)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(19), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f20:[0-9]+]] @f20(%[[VALUE_j_18:[0-9]+]] j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE_j_18]])), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(20)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(20), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f21:[0-9]+]] @f21(%[[VALUE_j_19:[0-9]+]] j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE_j_19]])), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(21)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(21), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f22:[0-9]+]] @f22(%[[VALUE_j_20:[0-9]+]] j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE_j_20]])), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(22)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(22), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f23:[0-9]+]] @f23(%[[VALUE_j_21:[0-9]+]] j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE_j_21]])), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(23)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(23), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f24:[0-9]+]] @f24(%[[VALUE_j_22:[0-9]+]] j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE_j_22]])), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(24)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(24), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f25:[0-9]+]] @f25(%[[VALUE_j_23:[0-9]+]] j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE_j_23]])), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(25)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(25), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f26:[0-9]+]] @f26(%[[VALUE_j_24:[0-9]+]] j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE_j_24]])), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(26)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(26), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f27:[0-9]+]] @f27(%[[VALUE_j_25:[0-9]+]] j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE_j_25]])), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(27)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(27), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f28:[0-9]+]] @f28(%[[VALUE_j_26:[0-9]+]] j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE_j_26]])), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(28)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(28), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f29:[0-9]+]] @f29(%[[VALUE_j_27:[0-9]+]] j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE_j_27]])), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(29)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(29), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f30:[0-9]+]] @f30(%[[VALUE_j_28:[0-9]+]] j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE_j_28]])), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(30)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(30), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f31:[0-9]+]] @f31(%[[VALUE_j_29:[0-9]+]] j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE_j_29]])), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(31)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(31), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_f3]], widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_f4]], widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_f5]], widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_f6]], widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_f7]], widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_f8]], widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_f9]], widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_f10]], widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_f11]], widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_f12]], widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_f13]], widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_f14]], widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_f15]], widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_f16]], widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_f17]], widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_f18]], widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_f19]], widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_f20]], widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_f21]], widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_f22]], widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_f23]], widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_f24]], widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_f25]], widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_f26]], widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_f27]], widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_f28]], widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_f29]], widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_f30]], widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_f31]], widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
