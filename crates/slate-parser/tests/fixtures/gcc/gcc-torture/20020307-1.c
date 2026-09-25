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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%61 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @f3(%3 j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%3)), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(3)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(3), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @f4(%5 j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%5)), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(4)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(4), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @f5(%7 j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%7)), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(5)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(5), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @f6(%9 j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%9)), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(6)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(6), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @f7(%11 j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%11)), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(7)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(7), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @f8(%13 j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%13)), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(8)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(8), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @f9(%15 j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%15)), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(9)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(9), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @f10(%17 j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%17)), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(10)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(10), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @f11(%19 j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%19)), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(11)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(11), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @f12(%21 j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%21)), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(12)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(12), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @f13(%23 j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%23)), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(13)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(13), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %24 @f14(%25 j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%25)), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(14)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(14), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %26 @f15(%27 j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%27)), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(15)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(15), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %28 @f16(%29 j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%29)), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(16)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(16), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %30 @f17(%31 j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%31)), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(17)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(17), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %32 @f18(%33 j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%33)), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(18)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(18), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %34 @f19(%35 j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%35)), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(19)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(19), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %36 @f20(%37 j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%37)), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(20)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(20), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %38 @f21(%39 j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%39)), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(21)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(21), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %40 @f22(%41 j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%41)), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(22)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(22), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %42 @f23(%43 j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%43)), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(23)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(23), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %44 @f24(%45 j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%45)), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(24)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(24), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %46 @f25(%47 j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%47)), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(25)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(25), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %48 @f26(%49 j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%49)), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(26)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(26), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %50 @f27(%51 j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%51)), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(27)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(27), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %52 @f28(%53 j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%53)), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(28)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(28), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %54 @f29(%55 j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%55)), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(29)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(29), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %56 @f30(%57 j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%57)), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(30)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(30), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %58 @f31(%59 j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%59)), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(31)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(const<i32>(31), const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %60 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%2, widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%4, widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%6, widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%8, widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%10, widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%12, widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%14, widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%16, widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%18, widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%20, widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%22, widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%24, widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%26, widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%28, widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%30, widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%32, widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%34, widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%36, widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%38, widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%40, widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%42, widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%44, widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%46, widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%48, widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%50, widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%52, widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%54, widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%56, widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%58, widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
