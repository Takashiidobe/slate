/* PR c/65345 */
/* { dg-do run } */
/* { dg-options "" } */

#define CHECK(X) if (!(X)) __builtin_abort ()

_Atomic float i = 5;
_Atomic float j = 2;

void
fn1 (float a[(int) (i = 0)])
{
}

void
fn2 (float a[(int) (i += 2)])
{
}

void
fn3 (float a[(int) ++i])
{
}

void
fn4 (float a[(int) ++i])
{
}

void
fn5 (float a[(int) ++i][(int) (j = 10)])
{
}

void
fn6 (float a[(int) (i = 7)][(int) j--])
{
}

int
main ()
{
  float a[10];
  float aa[10][10];
  fn1 (a);
  CHECK (i == 0);
  fn2 (a);
  CHECK (i == 2);
  fn3 (a);
  CHECK (i == 3);
  fn4 (a);
  CHECK (i == 4);
  fn5 (aa);
  CHECK (i == 5);
  CHECK (j == 10);
  fn6 (aa);
  CHECK (i == 7);
  CHECK (j == 9);
}

// SLATE-FILECHECK-FLAVOR gcc
// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     global %0 i: atomic f32 [storage=static] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(5)) [linkage=external];
// DEFAULT-NEXT:     global %1 j: atomic f32 [storage=static] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2)) [linkage=external];
// DEFAULT-NEXT:     fn %2 @fn1(%3 a: ptr<f32> [array=%17]) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<f32, atomic=seq_cst>(%0, int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)));
// DEFAULT-NEXT:         let %17: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=observable>(int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @fn2(%5 a: ptr<f32> [array=%18]) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %26: f32 [synthetic] = update<f32, result=new, atomic=seq_cst>(%0, add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2))));
// DEFAULT-NEXT:         let %18: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=observable>(read<f32>(%26))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @fn3(%7 a: ptr<f32> [array=%19]) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %27: f32 [synthetic] = update<f32, result=new, atomic=seq_cst>(%0, add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:         let %19: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=observable>(read<f32>(%27))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @fn4(%9 a: ptr<f32> [array=%20]) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %28: f32 [synthetic] = update<f32, result=new, atomic=seq_cst>(%0, add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:         let %20: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=observable>(read<f32>(%28))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @fn5(%11 a: ptr<vla<f32, %22>> [array=%21]) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %29: f32 [synthetic] = update<f32, result=new, atomic=seq_cst>(%0, add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:         let %21: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=observable>(read<f32>(%29))));
// DEFAULT-NEXT:         write<f32, atomic=seq_cst>(%1, int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(10)));
// DEFAULT-NEXT:         let %22: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=observable>(int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(10)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @fn6(%13 a: ptr<vla<f32, %24>> [array=%23]) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<f32, atomic=seq_cst>(%0, int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(7)));
// DEFAULT-NEXT:         let %23: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=observable>(int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(7)))));
// DEFAULT-NEXT:         let %30: f32 [synthetic] = update<f32, result=old, atomic=seq_cst>(%1, sub<f32, rounding=nearest_even, exceptions=observable, contract=fast>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:         let %24: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=observable>(read<f32>(%30))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %25 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %14 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %15 a: array<f32, 10> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %16 aa: array<array<f32, 10>, 10> [storage=automatic] [align=16];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<f32>) -> void>(%2, array_decay<ptr<f32>, length=Some(10)>(%15));
// DEFAULT-NEXT:         if not<bool>(eq<f32, exceptions=observable>(read<f32, atomic=seq_cst>(%0), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<f32>) -> void>(%4, array_decay<ptr<f32>, length=Some(10)>(%15));
// DEFAULT-NEXT:         if not<bool>(eq<f32, exceptions=observable>(read<f32, atomic=seq_cst>(%0), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<f32>) -> void>(%6, array_decay<ptr<f32>, length=Some(10)>(%15));
// DEFAULT-NEXT:         if not<bool>(eq<f32, exceptions=observable>(read<f32, atomic=seq_cst>(%0), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(3))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<f32>) -> void>(%8, array_decay<ptr<f32>, length=Some(10)>(%15));
// DEFAULT-NEXT:         if not<bool>(eq<f32, exceptions=observable>(read<f32, atomic=seq_cst>(%0), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(4))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<vla<f32, *>>) -> void>(%10, pointer_cast<ptr<vla<f32, *>>, reason=arg>(array_decay<ptr<array<f32, 10>>, length=Some(10)>(%16)));
// DEFAULT-NEXT:         if not<bool>(eq<f32, exceptions=observable>(read<f32, atomic=seq_cst>(%0), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(5))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if not<bool>(eq<f32, exceptions=observable>(read<f32, atomic=seq_cst>(%1), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(10))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<vla<f32, *>>) -> void>(%12, pointer_cast<ptr<vla<f32, *>>, reason=arg>(array_decay<ptr<array<f32, 10>>, length=Some(10)>(%16)));
// DEFAULT-NEXT:         if not<bool>(eq<f32, exceptions=observable>(read<f32, atomic=seq_cst>(%0), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(7))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if not<bool>(eq<f32, exceptions=observable>(read<f32, atomic=seq_cst>(%1), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(9))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
