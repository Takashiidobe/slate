/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-cfg" } */

#include <limits.h>

void this_comparison_is_false (void);
void this_comparison_is_true (void);
void this_comparison_is_not_decidable (void);

void bla1eq (int var)
{
  if (var + 10 == INT_MIN + 9)
    this_comparison_is_false ();
}

void bla2eq (int var)
{
  if (var + 10 == INT_MIN + 10)
    this_comparison_is_not_decidable ();
}

void bla3eq (int var)
{
  if (var - 10 == INT_MAX - 9)
    this_comparison_is_false ();
}

void bla4eq (int var)
{
  if (var - 10 == INT_MAX - 10)
    this_comparison_is_not_decidable ();
}

void bla1ne (int var)
{
  if (var + 10 != INT_MIN + 9)
    this_comparison_is_true ();
}

void bla2ne (int var)
{
  if (var + 10 != INT_MIN + 10)
    this_comparison_is_not_decidable ();
}

void bla3ne (int var)
{
  if (var - 10 != INT_MAX - 9)
    this_comparison_is_true ();
}

void bla4ne (int var)
{
  if (var - 10 != INT_MAX - 10)
    this_comparison_is_not_decidable ();
}

void bla1lt (int var)
{
  if (var + 10 < INT_MIN + 10)
    this_comparison_is_false ();
}

void bla2lt (int var)
{
  if (var + 10 < INT_MIN + 11)
    this_comparison_is_not_decidable ();
}

void bla3lt (int var)
{
  if (var - 10 < INT_MAX - 9)
    this_comparison_is_true ();
}

void bla4lt (int var)
{
  if (var - 10 < INT_MAX - 10)
    this_comparison_is_not_decidable ();
}

void bla1le (int var)
{
  if (var + 10 <= INT_MIN + 9)
    this_comparison_is_false ();
}

void bla2le (int var)
{
  if (var + 10 <= INT_MIN + 10)
    this_comparison_is_not_decidable ();
}

void bla3le (int var)
{
  if (var - 10 <= INT_MAX - 10)
    this_comparison_is_true ();
}

void bla4le (int var)
{
  if (var - 10 <= INT_MAX - 11)
    this_comparison_is_not_decidable ();
}

void bla1gt (int var)
{
  if (var + 10 > INT_MIN + 9)
    this_comparison_is_true ();
}

void bla2gt (int var)
{
  if (var + 10 > INT_MIN + 10)
    this_comparison_is_not_decidable ();
}

void bla3gt (int var)
{
  if (var - 10 > INT_MAX - 10)
    this_comparison_is_false ();
}

void bla4gt (int var)
{
  if (var - 10 > INT_MAX - 11)
    this_comparison_is_not_decidable ();
}

void bla1ge (int var)
{
  if (var + 10 >= INT_MIN + 10)
    this_comparison_is_true ();
}

void bla2ge (int var)
{
  if (var + 10 >= INT_MIN + 11)
    this_comparison_is_not_decidable ();
}

void bla3ge (int var)
{
  if (var - 11 >= INT_MAX - 10)
    this_comparison_is_false ();
}

void bla4ge (int var)
{
  if (var - 10 >= INT_MAX - 10)
    this_comparison_is_not_decidable ();
}

/* { dg-final { scan-tree-dump-times "this_comparison_is_false" 0 "cfg" } } */
/* { dg-final { scan-tree-dump-times "this_comparison_is_true" 6 "cfg" } } */
/* { dg-final { scan-tree-dump-times "this_comparison_is_not_decidable" 12 "cfg" } } */
/* { dg-final { scan-tree-dump-times "if " 12 "cfg" } } */

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
// DEFAULT-NEXT:     fn %[[VALUE_this_comparison_is_false:[0-9]+]] @this_comparison_is_false() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_this_comparison_is_true:[0-9]+]] @this_comparison_is_true() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_this_comparison_is_not_decidable:[0-9]+]] @this_comparison_is_not_decidable() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_bla1eq:[0-9]+]] @bla1eq(%[[VALUE_var:[0-9]+]] var: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<i32>(add<i32, overflow=ub>(read<i32>(%[[VALUE_var]]), const<i32>(10)), add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(9)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_this_comparison_is_false]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bla2eq:[0-9]+]] @bla2eq(%[[VALUE_var_2:[0-9]+]] var: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<i32>(add<i32, overflow=ub>(read<i32>(%[[VALUE_var_2]]), const<i32>(10)), add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(10)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_this_comparison_is_not_decidable]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bla3eq:[0-9]+]] @bla3eq(%[[VALUE_var_3:[0-9]+]] var: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<i32>(sub<i32, overflow=ub>(read<i32>(%[[VALUE_var_3]]), const<i32>(10)), sub<i32, overflow=ub>(const<i32>(2147483647), const<i32>(9)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_this_comparison_is_false]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bla4eq:[0-9]+]] @bla4eq(%[[VALUE_var_4:[0-9]+]] var: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<i32>(sub<i32, overflow=ub>(read<i32>(%[[VALUE_var_4]]), const<i32>(10)), sub<i32, overflow=ub>(const<i32>(2147483647), const<i32>(10)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_this_comparison_is_not_decidable]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bla1ne:[0-9]+]] @bla1ne(%[[VALUE_var_5:[0-9]+]] var: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(add<i32, overflow=ub>(read<i32>(%[[VALUE_var_5]]), const<i32>(10)), add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(9)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_this_comparison_is_true]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bla2ne:[0-9]+]] @bla2ne(%[[VALUE_var_6:[0-9]+]] var: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(add<i32, overflow=ub>(read<i32>(%[[VALUE_var_6]]), const<i32>(10)), add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(10)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_this_comparison_is_not_decidable]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bla3ne:[0-9]+]] @bla3ne(%[[VALUE_var_7:[0-9]+]] var: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(sub<i32, overflow=ub>(read<i32>(%[[VALUE_var_7]]), const<i32>(10)), sub<i32, overflow=ub>(const<i32>(2147483647), const<i32>(9)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_this_comparison_is_true]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bla4ne:[0-9]+]] @bla4ne(%[[VALUE_var_8:[0-9]+]] var: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(sub<i32, overflow=ub>(read<i32>(%[[VALUE_var_8]]), const<i32>(10)), sub<i32, overflow=ub>(const<i32>(2147483647), const<i32>(10)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_this_comparison_is_not_decidable]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bla1lt:[0-9]+]] @bla1lt(%[[VALUE_var_9:[0-9]+]] var: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if lt<i32>(add<i32, overflow=ub>(read<i32>(%[[VALUE_var_9]]), const<i32>(10)), add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(10)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_this_comparison_is_false]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bla2lt:[0-9]+]] @bla2lt(%[[VALUE_var_10:[0-9]+]] var: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if lt<i32>(add<i32, overflow=ub>(read<i32>(%[[VALUE_var_10]]), const<i32>(10)), add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(11)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_this_comparison_is_not_decidable]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bla3lt:[0-9]+]] @bla3lt(%[[VALUE_var_11:[0-9]+]] var: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if lt<i32>(sub<i32, overflow=ub>(read<i32>(%[[VALUE_var_11]]), const<i32>(10)), sub<i32, overflow=ub>(const<i32>(2147483647), const<i32>(9)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_this_comparison_is_true]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bla4lt:[0-9]+]] @bla4lt(%[[VALUE_var_12:[0-9]+]] var: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if lt<i32>(sub<i32, overflow=ub>(read<i32>(%[[VALUE_var_12]]), const<i32>(10)), sub<i32, overflow=ub>(const<i32>(2147483647), const<i32>(10)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_this_comparison_is_not_decidable]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bla1le:[0-9]+]] @bla1le(%[[VALUE_var_13:[0-9]+]] var: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if le<i32>(add<i32, overflow=ub>(read<i32>(%[[VALUE_var_13]]), const<i32>(10)), add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(9)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_this_comparison_is_false]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bla2le:[0-9]+]] @bla2le(%[[VALUE_var_14:[0-9]+]] var: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if le<i32>(add<i32, overflow=ub>(read<i32>(%[[VALUE_var_14]]), const<i32>(10)), add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(10)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_this_comparison_is_not_decidable]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bla3le:[0-9]+]] @bla3le(%[[VALUE_var_15:[0-9]+]] var: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if le<i32>(sub<i32, overflow=ub>(read<i32>(%[[VALUE_var_15]]), const<i32>(10)), sub<i32, overflow=ub>(const<i32>(2147483647), const<i32>(10)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_this_comparison_is_true]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bla4le:[0-9]+]] @bla4le(%[[VALUE_var_16:[0-9]+]] var: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if le<i32>(sub<i32, overflow=ub>(read<i32>(%[[VALUE_var_16]]), const<i32>(10)), sub<i32, overflow=ub>(const<i32>(2147483647), const<i32>(11)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_this_comparison_is_not_decidable]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bla1gt:[0-9]+]] @bla1gt(%[[VALUE_var_17:[0-9]+]] var: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if gt<i32>(add<i32, overflow=ub>(read<i32>(%[[VALUE_var_17]]), const<i32>(10)), add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(9)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_this_comparison_is_true]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bla2gt:[0-9]+]] @bla2gt(%[[VALUE_var_18:[0-9]+]] var: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if gt<i32>(add<i32, overflow=ub>(read<i32>(%[[VALUE_var_18]]), const<i32>(10)), add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(10)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_this_comparison_is_not_decidable]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bla3gt:[0-9]+]] @bla3gt(%[[VALUE_var_19:[0-9]+]] var: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if gt<i32>(sub<i32, overflow=ub>(read<i32>(%[[VALUE_var_19]]), const<i32>(10)), sub<i32, overflow=ub>(const<i32>(2147483647), const<i32>(10)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_this_comparison_is_false]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bla4gt:[0-9]+]] @bla4gt(%[[VALUE_var_20:[0-9]+]] var: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if gt<i32>(sub<i32, overflow=ub>(read<i32>(%[[VALUE_var_20]]), const<i32>(10)), sub<i32, overflow=ub>(const<i32>(2147483647), const<i32>(11)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_this_comparison_is_not_decidable]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bla1ge:[0-9]+]] @bla1ge(%[[VALUE_var_21:[0-9]+]] var: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<i32>(add<i32, overflow=ub>(read<i32>(%[[VALUE_var_21]]), const<i32>(10)), add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(10)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_this_comparison_is_true]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bla2ge:[0-9]+]] @bla2ge(%[[VALUE_var_22:[0-9]+]] var: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<i32>(add<i32, overflow=ub>(read<i32>(%[[VALUE_var_22]]), const<i32>(10)), add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(11)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_this_comparison_is_not_decidable]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bla3ge:[0-9]+]] @bla3ge(%[[VALUE_var_23:[0-9]+]] var: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<i32>(sub<i32, overflow=ub>(read<i32>(%[[VALUE_var_23]]), const<i32>(11)), sub<i32, overflow=ub>(const<i32>(2147483647), const<i32>(10)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_this_comparison_is_false]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bla4ge:[0-9]+]] @bla4ge(%[[VALUE_var_24:[0-9]+]] var: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<i32>(sub<i32, overflow=ub>(read<i32>(%[[VALUE_var_24]]), const<i32>(10)), sub<i32, overflow=ub>(const<i32>(2147483647), const<i32>(10)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_this_comparison_is_not_decidable]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
