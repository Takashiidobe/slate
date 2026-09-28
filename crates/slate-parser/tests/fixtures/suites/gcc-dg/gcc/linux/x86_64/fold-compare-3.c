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
// DEFAULT-NEXT:     fn %0 @this_comparison_is_false() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @this_comparison_is_true() -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @this_comparison_is_not_decidable() -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @bla1eq(%4 var: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<i32>(add<i32, overflow=ub>(read<i32>(%4), const<i32>(10)), add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(9)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @bla2eq(%6 var: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<i32>(add<i32, overflow=ub>(read<i32>(%6), const<i32>(10)), add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(10)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @bla3eq(%8 var: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<i32>(sub<i32, overflow=ub>(read<i32>(%8), const<i32>(10)), sub<i32, overflow=ub>(const<i32>(2147483647), const<i32>(9)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @bla4eq(%10 var: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<i32>(sub<i32, overflow=ub>(read<i32>(%10), const<i32>(10)), sub<i32, overflow=ub>(const<i32>(2147483647), const<i32>(10)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @bla1ne(%12 var: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(add<i32, overflow=ub>(read<i32>(%12), const<i32>(10)), add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(9)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @bla2ne(%14 var: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(add<i32, overflow=ub>(read<i32>(%14), const<i32>(10)), add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(10)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @bla3ne(%16 var: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(sub<i32, overflow=ub>(read<i32>(%16), const<i32>(10)), sub<i32, overflow=ub>(const<i32>(2147483647), const<i32>(9)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @bla4ne(%18 var: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(sub<i32, overflow=ub>(read<i32>(%18), const<i32>(10)), sub<i32, overflow=ub>(const<i32>(2147483647), const<i32>(10)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @bla1lt(%20 var: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if lt<i32>(add<i32, overflow=ub>(read<i32>(%20), const<i32>(10)), add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(10)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @bla2lt(%22 var: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if lt<i32>(add<i32, overflow=ub>(read<i32>(%22), const<i32>(10)), add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(11)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @bla3lt(%24 var: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if lt<i32>(sub<i32, overflow=ub>(read<i32>(%24), const<i32>(10)), sub<i32, overflow=ub>(const<i32>(2147483647), const<i32>(9)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %25 @bla4lt(%26 var: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if lt<i32>(sub<i32, overflow=ub>(read<i32>(%26), const<i32>(10)), sub<i32, overflow=ub>(const<i32>(2147483647), const<i32>(10)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %27 @bla1le(%28 var: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if le<i32>(add<i32, overflow=ub>(read<i32>(%28), const<i32>(10)), add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(9)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %29 @bla2le(%30 var: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if le<i32>(add<i32, overflow=ub>(read<i32>(%30), const<i32>(10)), add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(10)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %31 @bla3le(%32 var: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if le<i32>(sub<i32, overflow=ub>(read<i32>(%32), const<i32>(10)), sub<i32, overflow=ub>(const<i32>(2147483647), const<i32>(10)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %33 @bla4le(%34 var: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if le<i32>(sub<i32, overflow=ub>(read<i32>(%34), const<i32>(10)), sub<i32, overflow=ub>(const<i32>(2147483647), const<i32>(11)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %35 @bla1gt(%36 var: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if gt<i32>(add<i32, overflow=ub>(read<i32>(%36), const<i32>(10)), add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(9)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %37 @bla2gt(%38 var: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if gt<i32>(add<i32, overflow=ub>(read<i32>(%38), const<i32>(10)), add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(10)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %39 @bla3gt(%40 var: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if gt<i32>(sub<i32, overflow=ub>(read<i32>(%40), const<i32>(10)), sub<i32, overflow=ub>(const<i32>(2147483647), const<i32>(10)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %41 @bla4gt(%42 var: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if gt<i32>(sub<i32, overflow=ub>(read<i32>(%42), const<i32>(10)), sub<i32, overflow=ub>(const<i32>(2147483647), const<i32>(11)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %43 @bla1ge(%44 var: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<i32>(add<i32, overflow=ub>(read<i32>(%44), const<i32>(10)), add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(10)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %45 @bla2ge(%46 var: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<i32>(add<i32, overflow=ub>(read<i32>(%46), const<i32>(10)), add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(11)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %47 @bla3ge(%48 var: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<i32>(sub<i32, overflow=ub>(read<i32>(%48), const<i32>(11)), sub<i32, overflow=ub>(const<i32>(2147483647), const<i32>(10)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %49 @bla4ge(%50 var: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ge<i32>(sub<i32, overflow=ub>(read<i32>(%50), const<i32>(10)), sub<i32, overflow=ub>(const<i32>(2147483647), const<i32>(10)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
