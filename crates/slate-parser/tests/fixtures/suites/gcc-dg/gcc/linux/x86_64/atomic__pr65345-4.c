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
// DEFAULT-NEXT:     global %[[VALUE_i:[0-9]+]] i: atomic f32 [storage=static] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(5)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_j:[0-9]+]] j: atomic f32 [storage=static] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2)) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fn1:[0-9]+]] @fn1(%[[VALUE_a:[0-9]+]] a: ptr<f32> [array=%[[VALUE0:[0-9]+]]]) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<f32, atomic=seq_cst>(%[[VALUE_i]], int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE0]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=observable>(int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2:[0-9]+]] @fn2(%[[VALUE_a_2:[0-9]+]] a: ptr<f32> [array=%[[VALUE1:[0-9]+]]]) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: f32 [synthetic] = update<f32, result=new, atomic=seq_cst>(%[[VALUE_i]], add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2))));
// DEFAULT-NEXT:         let %[[VALUE1]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=observable>(read<f32>(%[[VALUE2]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3:[0-9]+]] @fn3(%[[VALUE_a_3:[0-9]+]] a: ptr<f32> [array=%[[VALUE3:[0-9]+]]]) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: f32 [synthetic] = update<f32, result=new, atomic=seq_cst>(%[[VALUE_i]], add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE3]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=observable>(read<f32>(%[[VALUE4]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn4:[0-9]+]] @fn4(%[[VALUE_a_4:[0-9]+]] a: ptr<f32> [array=%[[VALUE5:[0-9]+]]]) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: f32 [synthetic] = update<f32, result=new, atomic=seq_cst>(%[[VALUE_i]], add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE5]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=observable>(read<f32>(%[[VALUE6]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn5:[0-9]+]] @fn5(%[[VALUE_a_5:[0-9]+]] a: ptr<vla<f32, %[[VALUE7:[0-9]+]]>> [array=%[[VALUE8:[0-9]+]]]) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: f32 [synthetic] = update<f32, result=new, atomic=seq_cst>(%[[VALUE_i]], add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE8]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=observable>(read<f32>(%[[VALUE9]]))));
// DEFAULT-NEXT:         write<f32, atomic=seq_cst>(%[[VALUE_j]], int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(10)));
// DEFAULT-NEXT:         let %[[VALUE7]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=observable>(int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(10)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn6:[0-9]+]] @fn6(%[[VALUE_a_6:[0-9]+]] a: ptr<vla<f32, %[[VALUE10:[0-9]+]]>> [array=%[[VALUE11:[0-9]+]]]) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<f32, atomic=seq_cst>(%[[VALUE_i]], int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(7)));
// DEFAULT-NEXT:         let %[[VALUE11]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=observable>(int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(7)))));
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: f32 [synthetic] = update<f32, result=old, atomic=seq_cst>(%[[VALUE_j]], sub<f32, rounding=nearest_even, exceptions=observable, contract=fast>(old<f32>, int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE10]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=observable>(read<f32>(%[[VALUE12]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a_7:[0-9]+]] a: array<f32, 10> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_aa:[0-9]+]] aa: array<array<f32, 10>, 10> [storage=automatic] [align=16];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<f32>) -> void>(%[[VALUE_fn1]], array_decay<ptr<f32>, length=Some(10)>(%[[VALUE_a_7]]));
// DEFAULT-NEXT:         if not<bool>(eq<f32, exceptions=observable>(read<f32, atomic=seq_cst>(%[[VALUE_i]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<f32>) -> void>(%[[VALUE_fn2]], array_decay<ptr<f32>, length=Some(10)>(%[[VALUE_a_7]]));
// DEFAULT-NEXT:         if not<bool>(eq<f32, exceptions=observable>(read<f32, atomic=seq_cst>(%[[VALUE_i]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<f32>) -> void>(%[[VALUE_fn3]], array_decay<ptr<f32>, length=Some(10)>(%[[VALUE_a_7]]));
// DEFAULT-NEXT:         if not<bool>(eq<f32, exceptions=observable>(read<f32, atomic=seq_cst>(%[[VALUE_i]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(3))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<f32>) -> void>(%[[VALUE_fn4]], array_decay<ptr<f32>, length=Some(10)>(%[[VALUE_a_7]]));
// DEFAULT-NEXT:         if not<bool>(eq<f32, exceptions=observable>(read<f32, atomic=seq_cst>(%[[VALUE_i]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(4))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<vla<f32, *>>) -> void>(%[[VALUE_fn5]], pointer_cast<ptr<vla<f32, *>>, reason=arg>(array_decay<ptr<array<f32, 10>>, length=Some(10)>(%[[VALUE_aa]])));
// DEFAULT-NEXT:         if not<bool>(eq<f32, exceptions=observable>(read<f32, atomic=seq_cst>(%[[VALUE_i]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(5))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if not<bool>(eq<f32, exceptions=observable>(read<f32, atomic=seq_cst>(%[[VALUE_j]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(10))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<vla<f32, *>>) -> void>(%[[VALUE_fn6]], pointer_cast<ptr<vla<f32, *>>, reason=arg>(array_decay<ptr<array<f32, 10>>, length=Some(10)>(%[[VALUE_aa]])));
// DEFAULT-NEXT:         if not<bool>(eq<f32, exceptions=observable>(read<f32, atomic=seq_cst>(%[[VALUE_i]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(7))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if not<bool>(eq<f32, exceptions=observable>(read<f32, atomic=seq_cst>(%[[VALUE_j]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(9))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
