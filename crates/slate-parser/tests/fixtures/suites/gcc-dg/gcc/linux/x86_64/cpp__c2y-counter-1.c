/* N3457 - The __COUNTER__ predefined macro */
/* { dg-do run } */
/* { dg-options "-std=c2y -pedantic-errors" } */

#ifndef __COUNTER__
#error "__COUNTER__ not defined"
#endif

#define A(X) X + X
static_assert (A(__COUNTER__) == 0);
#define B(X)
B(__COUNTER__)
static_assert (__COUNTER__ == 1);
#define C(...) __VA_OPT__()
C(__COUNTER__)
static_assert (__COUNTER__ == 3);
#define D(...) __VA_OPT__(a)
int D(__COUNTER__) = 1;
static_assert (__COUNTER__ == 5);
#define E(X) #X
const char *b = E(__COUNTER__);
#define F(X) a##X
int F(__COUNTER__) = 2;
static_assert (__COUNTER__ == 6);
#define G(X) b##X = X
int G(__COUNTER__);
static_assert (__COUNTER__ == 8);
#if !defined(__COUNTER__) || (__COUNTER__ + 1 != __COUNTER__ + 0)
#error "Unexpected __COUNTER__ behavior")
#endif
static_assert (__COUNTER__ == 11);

extern int strcmp (const char *, const char *);
extern void abort ();

int
main ()
{
  if (a != 1
      || strcmp (b, "__COUNTER__")
      || a__COUNTER__ != 2
      || b__COUNTER__ != 7)
    abort ();
}

// SLATE-FILECHECK-STD DEFAULT c2y
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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([95, 95, 67, 79, 85, 78, 84, 69, 82, 95, 95, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(12)>(%[[VALUE_str]])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_a__COUNTER__:[0-9]+]] a__COUNTER__: i32 [storage=static] = const<i32>(2) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b__COUNTER__:[0-9]+]] b__COUNTER__: i32 [storage=static] = const<i32>(7) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([95, 95, 67, 79, 85, 78, 84, 69, 82, 95, 95, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_strcmp:[0-9]+]] @strcmp(%[[VALUE0:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_a]]), const<i32>(1))
// DEFAULT-NEXT:             write<bool>(%[[VALUE2]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE2]], ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE_strcmp]], read<ptr<const i8>>(%[[VALUE_b]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(12)>(%[[VALUE_str_2]]))), const<i32>(0)));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(read<bool>(%[[VALUE2]]), ne<i32>(read<i32>(%[[VALUE_a__COUNTER__]]), const<i32>(2))), ne<i32>(read<i32>(%[[VALUE_b__COUNTER__]]), const<i32>(7)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
