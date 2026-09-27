/* Test for _Bool and <stdbool.h> in C99.  */
/* Origin: Joseph Myers <jsm28@cam.ac.uk> */
/* { dg-do run } */
/* { dg-options "-std=iso9899:1999 -pedantic-errors" } */

/* _Bool must be a builtin type.  */

_Bool foo;

#include <stdbool.h>

/* Three macros must be integer constant expressions suitable for use
   in #if.
*/

#if !defined(true) || (true != 1)
#error "bad stdbool true" /* { dg-bogus "#error" "bad stdbool.h" } */
#endif

#if !defined(false) || (false != 0)
#error "bad stdbool false" /* { dg-bogus "#error" "bad stdbool.h" } */
#endif

#if !defined(__bool_true_false_are_defined) || (__bool_true_false_are_defined != 1)
#error "bad stdbool __bool_true_false_are_defined" /* { dg-bogus "#error" "bad stdbool.h" } */
#endif

int a = true;
int b = false;
int c = __bool_true_false_are_defined;

struct foo
{
  _Bool a : 1;
} sf;

#define str(x) xstr(x)
#define xstr(x) #x


extern void abort (void);
extern void exit (int);
extern int strcmp (const char *, const char *);

int
main (void)
{
  /* The macro `bool' must expand to _Bool.  */
  const char *t = str (bool);
  _Bool u, v;
  if (strcmp (t, "_Bool"))
    abort ();
  if (a != 1 || b != 0 || c != 1)
    abort ();
  /* Casts to _Bool have a specified behavior.  */
  if ((int)(_Bool)2 != 1)
    abort ();
  if ((int)(_Bool)0.2 != 1)
    abort ();
  /* Pointers may be assigned to _Bool.  */
  if ((u = t) != 1)
    abort ();
  /* _Bool may be used to subscript arrays.  */
  u = 0;
  if (t[u] != '_')
    abort ();
  if (u[t] != '_')
    abort ();
  u = 1;
  if (t[u] != 'B')
    abort ();
  if (u[t] != 'B')
    abort ();
  /* Test increment and decrement operators.  */
  u = 0;
  if (u++ != 0)
    abort ();
  if (u != 1)
    abort ();
  if (u++ != 1)
    abort ();
  if (u != 1)
    abort ();
  u = 0;
  if (++u != 1)
    abort ();
  if (u != 1)
    abort ();
  if (++u != 1)
    abort ();
  if (u != 1)
    abort ();
  u = 0;
  if (u-- != 0)
    abort ();
  if (u != 1)
    abort ();
  if (u-- != 1)
    abort ();
  if (u != 0)
    abort ();
  u = 0;
  if (--u != 1)
    abort ();
  if (u != 1)
    abort ();
  if (--u != 0)
    abort ();
  if (u != 0)
    abort ();
  /* Test unary + - ~ !.  */
  u = 0;
  if (+u != 0)
    abort ();
  if (-u != 0)
    abort ();
  u = 1;
  if (+u != 1)
    abort ();
  if (-u != -1)
    abort ();
  u = 2;
  if (+u != 1)
    abort ();
  if (-u != -1)
    abort ();
  u = 0;
  if (~u != ~(int)0)
    abort ();
  u = 1;
  if (~u != ~(int)1)
    abort ();
  u = 0;
  if (!u != 1)
    abort ();
  u = 1;
  if (!u != 0)
    abort ();
  /* Test arithmetic * / % + - (which all apply promotions).  */
  u = 0;
  if (u + 2 != 2)
    abort ();
  u = 1;
  if (u * 4 != 4)
    abort ();
  if (u % 3 != 1)
    abort ();
  if (u / 1 != 1)
    abort ();
  if (4 / u != 4)
    abort ();
  if (u - 7 != -6)
    abort ();
  /* Test bitwise shift << >>.  */
  u = 1;
  if (u << 1 != 2)
    abort ();
  if (u >> 1 != 0)
    abort ();
  /* Test relational and equality operators < > <= >= == !=.  */
  u = 0;
  v = 0;
  if (u < v || u > v || !(u <= v) || !(u >= v) || !(u == v) || u != v)
    abort ();
  u = 0;
  v = 1;
  if (!(u < v) || u > v || !(u <= v) || u >= v || u == v || !(u != v))
    abort ();
  /* Test bitwise operators & ^ |.  */
  u = 1;
  if ((u | 2) != 3)
    abort ();
  if ((u ^ 3) != 2)
    abort ();
  if ((u & 1) != 1)
    abort ();
  if ((u & 0) != 0)
    abort ();
  /* Test logical && ||.  */
  u = 0;
  v = 1;
  if (!(u || v))
    abort ();
  if (!(v || u))
    abort ();
  if (u && v)
    abort ();
  if (v && u)
    abort ();
  u = 1;
  v = 1;
  if (!(u && v))
    abort ();
  /* Test conditional ? :.  */
  u = 0;
  if ((u ? 4 : 7) != 7)
    abort ();
  u = 1;
  v = 0;
  if ((1 ? u : v) != 1)
    abort ();
  if ((1 ? 4 : u) != 4)
    abort ();
  /* Test assignment operators = *= /= %= += -= <<= >>= &= ^= |=.  */
  if ((u = 2) != 1)
    abort ();
  if (u != 1)
    abort ();
  if ((u *= -1) != 1)
    abort ();
  if (u != 1)
    abort ();
  if ((u /= 2) != 0)
    abort ();
  if ((u += 3) != 1)
    abort ();
  if ((u -= 1) != 0)
    abort ();
  u = 1;
  if ((u <<= 4) != 1)
    abort ();
  if ((u >>= 1) != 0)
    abort ();
  u = 1;
  if ((u &= 0) != 0)
    abort ();
  if ((u |= 2) != 1)
    abort ();
  if ((u ^= 3) != 1)
    abort ();
  /* Test comma expressions.  */
  u = 1;
  if ((4, u) != 1)
    abort ();
  /* Test bitfields.  */
  {
    int i;
    for (i = 0; i < sizeof (struct foo); i++)
      *((unsigned char *)&sf + i) = (unsigned char) -1;
    sf.a = 1;
    if (sf.a != 1)
      abort ();
    sf.a = 0;
    if (sf.a != 0)
      abort ();
  }
  exit (0);
}

// SLATE-FILECHECK-FLAVOR gcc
// SLATE-FILECHECK-STD DEFAULT iso9899:1999
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
// DEFAULT-NEXT:     type @type0 foo = struct {
// DEFAULT-NEXT:         field0 a: bool : 1;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0], bit_offsets=[Some(0)], bit_units=[(0, 1)], field_units=[Some(0)]];
// DEFAULT-NEXT:     global %0 foo: bool [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 a: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %2 b: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %3 c: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %5 sf: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %17 .str17: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([95, 66, 111, 111, 108, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %18 .str18: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([95, 66, 111, 111, 108, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %6 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %7 @exit(%14 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %8 @strcmp(%15 <unnamed>: ptr<const i8>, %16 <unnamed>: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %9 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %10 t: ptr<const i8> [storage=automatic] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(6)>(%17));
// DEFAULT-NEXT:         let %11 u: bool [storage=automatic];
// DEFAULT-NEXT:         let %12 v: bool [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%8, read<ptr<const i8>>(%10), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%18))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(%1), const<i32>(1)), ne<i32>(read<i32>(%2), const<i32>(0))), ne<i32>(read<i32>(%3), const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=explicit>(ne<i32, reason=explicit>(const<i32>(2), const<i32>(0))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=explicit>(ne<f64, reason=explicit, exceptions=observable>(const<f64>(0.2), const<f64>(0.0))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         write<bool>(%11, ne<ptr<const i8>, reason=assign>(read<ptr<const i8>>(%10), null<ptr<const i8>>));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(ne<ptr<const i8>, reason=assign>(read<ptr<const i8>>(%10), null<ptr<const i8>>)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         write<bool>(%11, ne<i32, reason=assign>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%10), from_bool<i32, reason=promotion>(read<bool>(%11)))))), const<i32>(95))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%10), from_bool<i32, reason=promotion>(read<bool>(%11)))))), const<i32>(95))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         write<bool>(%11, ne<i32, reason=assign>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%10), from_bool<i32, reason=promotion>(read<bool>(%11)))))), const<i32>(66))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%10), from_bool<i32, reason=promotion>(read<bool>(%11)))))), const<i32>(66))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         write<bool>(%11, ne<i32, reason=assign>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:         let %20: bool [synthetic] = read<bool>(%11);
// DEFAULT-NEXT:         let %21: bool [synthetic] = ne<i32, reason=assign>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%20)), const<i32>(1)), const<i32>(0));
// DEFAULT-NEXT:         write<bool>(%11, read<bool>(%21));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%20)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%11)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         let %22: bool [synthetic] = read<bool>(%11);
// DEFAULT-NEXT:         let %23: bool [synthetic] = ne<i32, reason=assign>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%22)), const<i32>(1)), const<i32>(0));
// DEFAULT-NEXT:         write<bool>(%11, read<bool>(%23));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%22)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%11)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         write<bool>(%11, ne<i32, reason=assign>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:         let %24: bool [synthetic] = read<bool>(%11);
// DEFAULT-NEXT:         let %25: bool [synthetic] = ne<i32, reason=assign>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%24)), const<i32>(1)), const<i32>(0));
// DEFAULT-NEXT:         write<bool>(%11, read<bool>(%25));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%25)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%11)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         let %26: bool [synthetic] = read<bool>(%11);
// DEFAULT-NEXT:         let %27: bool [synthetic] = ne<i32, reason=assign>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%26)), const<i32>(1)), const<i32>(0));
// DEFAULT-NEXT:         write<bool>(%11, read<bool>(%27));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%27)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%11)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         write<bool>(%11, ne<i32, reason=assign>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:         let %28: bool [synthetic] = read<bool>(%11);
// DEFAULT-NEXT:         let %29: bool [synthetic] = ne<i32, reason=assign>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%28)), const<i32>(1)), const<i32>(0));
// DEFAULT-NEXT:         write<bool>(%11, read<bool>(%29));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%28)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%11)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         let %30: bool [synthetic] = read<bool>(%11);
// DEFAULT-NEXT:         let %31: bool [synthetic] = ne<i32, reason=assign>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%30)), const<i32>(1)), const<i32>(0));
// DEFAULT-NEXT:         write<bool>(%11, read<bool>(%31));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%30)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%11)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         write<bool>(%11, ne<i32, reason=assign>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:         let %32: bool [synthetic] = read<bool>(%11);
// DEFAULT-NEXT:         let %33: bool [synthetic] = ne<i32, reason=assign>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%32)), const<i32>(1)), const<i32>(0));
// DEFAULT-NEXT:         write<bool>(%11, read<bool>(%33));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%33)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%11)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         let %34: bool [synthetic] = read<bool>(%11);
// DEFAULT-NEXT:         let %35: bool [synthetic] = ne<i32, reason=assign>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%34)), const<i32>(1)), const<i32>(0));
// DEFAULT-NEXT:         write<bool>(%11, read<bool>(%35));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%35)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%11)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         write<bool>(%11, ne<i32, reason=assign>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%11)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<i32>(neg<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%11))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         write<bool>(%11, ne<i32, reason=assign>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%11)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<i32>(neg<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%11))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         write<bool>(%11, ne<i32, reason=assign>(const<i32>(2), const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%11)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<i32>(neg<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%11))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         write<bool>(%11, ne<i32, reason=assign>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(not<i32>(from_bool<i32, reason=promotion>(read<bool>(%11))), not<i32>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         write<bool>(%11, ne<i32, reason=assign>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(not<i32>(from_bool<i32, reason=promotion>(read<bool>(%11))), not<i32>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         write<bool>(%11, ne<i32, reason=assign>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(not<bool>(read<bool>(%11))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         write<bool>(%11, ne<i32, reason=assign>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(not<bool>(read<bool>(%11))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         write<bool>(%11, ne<i32, reason=assign>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%11)), const<i32>(2)), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         write<bool>(%11, ne<i32, reason=assign>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(mul<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%11)), const<i32>(4)), const<i32>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(from_bool<i32, reason=promotion>(read<bool>(%11)), const<i32>(3)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(from_bool<i32, reason=promotion>(read<bool>(%11)), const<i32>(1)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(4), from_bool<i32, reason=promotion>(read<bool>(%11))), const<i32>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%11)), const<i32>(7)), neg<i32, overflow=ub>(const<i32>(6)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         write<bool>(%11, ne<i32, reason=assign>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(from_bool<i32, reason=promotion>(read<bool>(%11)), const<i32>(1)), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(from_bool<i32, reason=promotion>(read<bool>(%11)), const<i32>(1)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         write<bool>(%11, ne<i32, reason=assign>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:         write<bool>(%12, ne<i32, reason=assign>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(lt<i32>(from_bool<i32, reason=promotion>(read<bool>(%11)), from_bool<i32, reason=promotion>(read<bool>(%12))), gt<i32>(from_bool<i32, reason=promotion>(read<bool>(%11)), from_bool<i32, reason=promotion>(read<bool>(%12)))), not<bool>(le<i32>(from_bool<i32, reason=promotion>(read<bool>(%11)), from_bool<i32, reason=promotion>(read<bool>(%12))))), not<bool>(ge<i32>(from_bool<i32, reason=promotion>(read<bool>(%11)), from_bool<i32, reason=promotion>(read<bool>(%12))))), not<bool>(eq<i32>(from_bool<i32, reason=promotion>(read<bool>(%11)), from_bool<i32, reason=promotion>(read<bool>(%12))))), ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%11)), from_bool<i32, reason=promotion>(read<bool>(%12))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         write<bool>(%11, ne<i32, reason=assign>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:         write<bool>(%12, ne<i32, reason=assign>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(not<bool>(lt<i32>(from_bool<i32, reason=promotion>(read<bool>(%11)), from_bool<i32, reason=promotion>(read<bool>(%12)))), gt<i32>(from_bool<i32, reason=promotion>(read<bool>(%11)), from_bool<i32, reason=promotion>(read<bool>(%12)))), not<bool>(le<i32>(from_bool<i32, reason=promotion>(read<bool>(%11)), from_bool<i32, reason=promotion>(read<bool>(%12))))), ge<i32>(from_bool<i32, reason=promotion>(read<bool>(%11)), from_bool<i32, reason=promotion>(read<bool>(%12)))), eq<i32>(from_bool<i32, reason=promotion>(read<bool>(%11)), from_bool<i32, reason=promotion>(read<bool>(%12)))), not<bool>(ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%11)), from_bool<i32, reason=promotion>(read<bool>(%12)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         write<bool>(%11, ne<i32, reason=assign>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(or<i32>(from_bool<i32, reason=promotion>(read<bool>(%11)), const<i32>(2)), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<i32>(xor<i32>(from_bool<i32, reason=promotion>(read<bool>(%11)), const<i32>(3)), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<i32>(and<i32>(from_bool<i32, reason=promotion>(read<bool>(%11)), const<i32>(1)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<i32>(and<i32>(from_bool<i32, reason=promotion>(read<bool>(%11)), const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         write<bool>(%11, ne<i32, reason=assign>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:         write<bool>(%12, ne<i32, reason=assign>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:         if not<bool>(logical_or<bool>(read<bool>(%11), read<bool>(%12)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if not<bool>(logical_or<bool>(read<bool>(%12), read<bool>(%11)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if logical_and<bool>(read<bool>(%11), read<bool>(%12))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if logical_and<bool>(read<bool>(%12), read<bool>(%11))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         write<bool>(%11, ne<i32, reason=assign>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:         write<bool>(%12, ne<i32, reason=assign>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:         if not<bool>(logical_and<bool>(read<bool>(%11), read<bool>(%12)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         write<bool>(%11, ne<i32, reason=assign>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(conditional<i32>(read<bool>(%11), const<i32>(4), const<i32>(7)), const<i32>(7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         write<bool>(%11, ne<i32, reason=assign>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:         write<bool>(%12, ne<i32, reason=assign>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), from_bool<i32, reason=promotion>(read<bool>(%11)), from_bool<i32, reason=promotion>(read<bool>(%12))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<i32>(conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), const<i32>(4), from_bool<i32, reason=promotion>(read<bool>(%11))), const<i32>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         write<bool>(%11, ne<i32, reason=assign>(const<i32>(2), const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(ne<i32, reason=assign>(const<i32>(2), const<i32>(0))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%11)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         let %36: bool [synthetic] = read<bool>(%11);
// DEFAULT-NEXT:         let %37: bool [synthetic] = ne<i32, reason=assign>(mul<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%36)), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0));
// DEFAULT-NEXT:         write<bool>(%11, read<bool>(%37));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%37)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%11)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         let %38: bool [synthetic] = read<bool>(%11);
// DEFAULT-NEXT:         let %39: bool [synthetic] = ne<i32, reason=assign>(div<i32, by_zero=ub, min_by_neg_one=ub>(from_bool<i32, reason=promotion>(read<bool>(%38)), const<i32>(2)), const<i32>(0));
// DEFAULT-NEXT:         write<bool>(%11, read<bool>(%39));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%39)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         let %40: bool [synthetic] = read<bool>(%11);
// DEFAULT-NEXT:         let %41: bool [synthetic] = ne<i32, reason=assign>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%40)), const<i32>(3)), const<i32>(0));
// DEFAULT-NEXT:         write<bool>(%11, read<bool>(%41));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%41)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         let %42: bool [synthetic] = read<bool>(%11);
// DEFAULT-NEXT:         let %43: bool [synthetic] = ne<i32, reason=assign>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%42)), const<i32>(1)), const<i32>(0));
// DEFAULT-NEXT:         write<bool>(%11, read<bool>(%43));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%43)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         write<bool>(%11, ne<i32, reason=assign>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:         let %44: bool [synthetic] = read<bool>(%11);
// DEFAULT-NEXT:         let %45: bool [synthetic] = ne<i32, reason=assign>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(from_bool<i32, reason=promotion>(read<bool>(%44)), const<i32>(4)), const<i32>(0));
// DEFAULT-NEXT:         write<bool>(%11, read<bool>(%45));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%45)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         let %46: bool [synthetic] = read<bool>(%11);
// DEFAULT-NEXT:         let %47: bool [synthetic] = ne<i32, reason=assign>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(from_bool<i32, reason=promotion>(read<bool>(%46)), const<i32>(1)), const<i32>(0));
// DEFAULT-NEXT:         write<bool>(%11, read<bool>(%47));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%47)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         write<bool>(%11, ne<i32, reason=assign>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:         let %48: bool [synthetic] = read<bool>(%11);
// DEFAULT-NEXT:         let %49: bool [synthetic] = ne<i32, reason=assign>(and<i32>(from_bool<i32, reason=promotion>(read<bool>(%48)), const<i32>(0)), const<i32>(0));
// DEFAULT-NEXT:         write<bool>(%11, read<bool>(%49));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%49)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         let %50: bool [synthetic] = read<bool>(%11);
// DEFAULT-NEXT:         let %51: bool [synthetic] = ne<i32, reason=assign>(or<i32>(from_bool<i32, reason=promotion>(read<bool>(%50)), const<i32>(2)), const<i32>(0));
// DEFAULT-NEXT:         write<bool>(%11, read<bool>(%51));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%51)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         let %52: bool [synthetic] = read<bool>(%11);
// DEFAULT-NEXT:         let %53: bool [synthetic] = ne<i32, reason=assign>(xor<i32>(from_bool<i32, reason=promotion>(read<bool>(%52)), const<i32>(3)), const<i32>(0));
// DEFAULT-NEXT:         write<bool>(%11, read<bool>(%53));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%53)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         write<bool>(%11, ne<i32, reason=assign>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:         const<i32>(4);
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%11)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %13 i: i32 [storage=automatic];
// DEFAULT-NEXT:             for %19
// DEFAULT-NEXT:                 init:
// DEFAULT-NEXT:                     write<i32>(%13, const<i32>(0));
// DEFAULT-NEXT:                 condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%13))), const<u64>(1))
// DEFAULT-NEXT:                 increment: {
// DEFAULT-NEXT:                     let %54: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:                     let %55: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%54), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%13, read<i32>(%55));
// DEFAULT-NEXT:                     yield void;
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:                 body:
// DEFAULT-NEXT:                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(pointer_cast<ptr<u8>, reason=explicit>(addr_of<ptr<@type0>>(%5)), read<i32>(%13))), reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:             write<bool>(bitfield0<unit=0, bytes=0..1, bits=0..1>(%5), ne<i32, reason=assign>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:             if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(bitfield0<unit=0, bytes=0..1, bits=0..1>(%5))), const<i32>(1))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:             write<bool>(bitfield0<unit=0, bytes=0..1, bits=0..1>(%5), ne<i32, reason=assign>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:             if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(bitfield0<unit=0, bytes=0..1, bits=0..1>(%5))), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%7, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
