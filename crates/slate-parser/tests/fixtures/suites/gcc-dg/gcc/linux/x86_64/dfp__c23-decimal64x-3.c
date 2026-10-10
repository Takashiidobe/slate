/* Test _Decimal64x in C23 mode - builtins.  */
/* { dg-options "-std=c23 -pedantic-errors" } */

_Decimal64x a = __builtin_infd64x ();
_Decimal64x b = __builtin_nand64x ("");
_Decimal64x c = __builtin_nansd64x ("");
_Decimal64x d = -42.D64x;
_Decimal64x e = 5.25d64x;

int
main ()
{
  if (__builtin_fabsd64x (-2.5d64x) != 2.5D64x
      || __builtin_fabsd64x (42.25D64x) != 42.25d64x
      || __builtin_fabsd64x (d) != 42.d64x
      || __builtin_fabsd64x (e) != 5.25D64x
      || __builtin_isinf (42.d64x)
      || __builtin_isnan (0.d64x)
      || !__builtin_isinf (__builtin_infd64x ())
      || !__builtin_isnan (__builtin_nand64x (""))
      /* || !__builtin_isinf (a) */
      || !__builtin_isnan (b)
      || !__builtin_isnan (c))
    __builtin_abort ();
}

// SLATE-FILECHECK-STD DEFAULT c23
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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: d128 [storage=static] = call<d128, signature=fn() -> d128>(%[[VALUE___builtin_infd64x:[0-9]+]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: d128 [storage=static] = call<d128, signature=fn(ptr<const i8>) -> d128>(%[[VALUE___builtin_nand64x:[0-9]+]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str]]))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: d128 [storage=static] = call<d128, signature=fn(ptr<const i8>) -> d128>(%[[VALUE___builtin_nansd64x:[0-9]+]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_2]]))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: d128 [storage=static] = neg<d128>(const<d128>(42.)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: d128 [storage=static] = const<d128>(5.25) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_infd64x]] @__builtin_infd64x() -> d128 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nand64x]] @__builtin_nand64x(%[[VALUE0:[0-9]+]] <unnamed>: ptr<const i8>) -> d128 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nansd64x]] @__builtin_nansd64x(%[[VALUE1:[0-9]+]] <unnamed>: ptr<const i8>) -> d128 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_fabsd64x:[0-9]+]] @__builtin_fabsd64x(%[[VALUE2:[0-9]+]] <unnamed>: d128) -> d128 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<d128, exceptions=observable>(call<d128, signature=fn(d128) -> d128>(%[[VALUE___builtin_fabsd64x]], neg<d128>(const<d128>(2.5))), const<d128>(2.5)), ne<d128, exceptions=observable>(call<d128, signature=fn(d128) -> d128>(%[[VALUE___builtin_fabsd64x]], const<d128>(42.25)), const<d128>(42.25))), ne<d128, exceptions=observable>(call<d128, signature=fn(d128) -> d128>(%[[VALUE___builtin_fabsd64x]], read<d128>(%[[VALUE_d]])), const<d128>(42.))), ne<d128, exceptions=observable>(call<d128, signature=fn(d128) -> d128>(%[[VALUE___builtin_fabsd64x]], read<d128>(%[[VALUE_e]])), const<d128>(5.25))), float_class<bool, test=infinite>(const<d128>(42.))), float_class<bool, test=nan>(const<d128>(0.))), not<bool>(float_class<bool, test=infinite>(call<d128, signature=fn() -> d128>(%[[VALUE___builtin_infd64x]])))), not<bool>(float_class<bool, test=nan>(call<d128, signature=fn(ptr<const i8>) -> d128>(%[[VALUE___builtin_nand64x]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_3]])))))), not<bool>(float_class<bool, test=nan>(read<d128>(%[[VALUE_b]])))), not<bool>(float_class<bool, test=nan>(read<d128>(%[[VALUE_c]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
