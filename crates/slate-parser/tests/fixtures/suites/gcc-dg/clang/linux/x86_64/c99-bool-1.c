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
// DEFAULT-NEXT:     type @type[[TYPE_foo:[0-9]+]] foo = struct {
// DEFAULT-NEXT:         field0 a: bool : 1;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0], bit_offsets=[Some(0)], bit_units=[(0, 1)], field_units=[Some(0)]];
// DEFAULT-NEXT:     global %[[VALUE_foo:[0-9]+]] foo: bool [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_sf:[0-9]+]] sf: @type[[TYPE_foo]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([95, 66, 111, 111, 108, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([95, 66, 111, 111, 108, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_strcmp:[0-9]+]] @strcmp(%[[VALUE1:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE2:[0-9]+]] <unnamed>: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_t:[0-9]+]] t: ptr<const i8> [storage=automatic] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str]]));
// DEFAULT-NEXT:         let %[[VALUE_u:[0-9]+]] u: bool [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_v:[0-9]+]] v: bool [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE_strcmp]], read<ptr<const i8>>(%[[VALUE_t]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_2]]))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(%[[VALUE_a]]), const<i32>(1)), ne<i32>(read<i32>(%[[VALUE_b]]), const<i32>(0))), ne<i32>(read<i32>(%[[VALUE_c]]), const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=explicit>(ne<i32, reason=explicit>(const<i32>(2), const<i32>(0))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=explicit>(ne<f64, reason=explicit, exceptions=ignore>(const<f64>(0.2), const<f64>(0.0))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: bool [synthetic] = ne<ptr<const i8>, reason=assign>(read<ptr<const i8>>(%[[VALUE_t]]), null<ptr<const i8>>);
// DEFAULT-NEXT:         write<bool>(%[[VALUE_u]], read<bool>(%[[VALUE3]]));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE3]])), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<bool>(%[[VALUE_u]], ne<i32, reason=assign>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE_t]]), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_u]])))))), const<i32>(95))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE_t]]), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_u]])))))), const<i32>(95))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<bool>(%[[VALUE_u]], ne<i32, reason=assign>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE_t]]), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_u]])))))), const<i32>(66))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE_t]]), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_u]])))))), const<i32>(66))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<bool>(%[[VALUE_u]], ne<i32, reason=assign>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: bool [synthetic] = read<bool>(%[[VALUE_u]]);
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: bool [synthetic] = ne<i32, reason=assign>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE4]])), const<i32>(1)), const<i32>(0));
// DEFAULT-NEXT:         write<bool>(%[[VALUE_u]], read<bool>(%[[VALUE5]]));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE4]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_u]])), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: bool [synthetic] = read<bool>(%[[VALUE_u]]);
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: bool [synthetic] = ne<i32, reason=assign>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE6]])), const<i32>(1)), const<i32>(0));
// DEFAULT-NEXT:         write<bool>(%[[VALUE_u]], read<bool>(%[[VALUE7]]));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE6]])), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_u]])), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<bool>(%[[VALUE_u]], ne<i32, reason=assign>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: bool [synthetic] = read<bool>(%[[VALUE_u]]);
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: bool [synthetic] = ne<i32, reason=assign>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE8]])), const<i32>(1)), const<i32>(0));
// DEFAULT-NEXT:         write<bool>(%[[VALUE_u]], read<bool>(%[[VALUE9]]));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE9]])), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_u]])), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: bool [synthetic] = read<bool>(%[[VALUE_u]]);
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: bool [synthetic] = ne<i32, reason=assign>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE10]])), const<i32>(1)), const<i32>(0));
// DEFAULT-NEXT:         write<bool>(%[[VALUE_u]], read<bool>(%[[VALUE11]]));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE11]])), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_u]])), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<bool>(%[[VALUE_u]], ne<i32, reason=assign>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: bool [synthetic] = read<bool>(%[[VALUE_u]]);
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: bool [synthetic] = ne<i32, reason=assign>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE12]])), const<i32>(1)), const<i32>(0));
// DEFAULT-NEXT:         write<bool>(%[[VALUE_u]], read<bool>(%[[VALUE13]]));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE12]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_u]])), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE14:[0-9]+]]: bool [synthetic] = read<bool>(%[[VALUE_u]]);
// DEFAULT-NEXT:         let %[[VALUE15:[0-9]+]]: bool [synthetic] = ne<i32, reason=assign>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE14]])), const<i32>(1)), const<i32>(0));
// DEFAULT-NEXT:         write<bool>(%[[VALUE_u]], read<bool>(%[[VALUE15]]));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE14]])), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_u]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<bool>(%[[VALUE_u]], ne<i32, reason=assign>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE16:[0-9]+]]: bool [synthetic] = read<bool>(%[[VALUE_u]]);
// DEFAULT-NEXT:         let %[[VALUE17:[0-9]+]]: bool [synthetic] = ne<i32, reason=assign>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE16]])), const<i32>(1)), const<i32>(0));
// DEFAULT-NEXT:         write<bool>(%[[VALUE_u]], read<bool>(%[[VALUE17]]));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE17]])), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_u]])), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE18:[0-9]+]]: bool [synthetic] = read<bool>(%[[VALUE_u]]);
// DEFAULT-NEXT:         let %[[VALUE19:[0-9]+]]: bool [synthetic] = ne<i32, reason=assign>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE18]])), const<i32>(1)), const<i32>(0));
// DEFAULT-NEXT:         write<bool>(%[[VALUE_u]], read<bool>(%[[VALUE19]]));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE19]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_u]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<bool>(%[[VALUE_u]], ne<i32, reason=assign>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_u]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(neg<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_u]]))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<bool>(%[[VALUE_u]], ne<i32, reason=assign>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_u]])), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(neg<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_u]]))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<bool>(%[[VALUE_u]], ne<i32, reason=assign>(const<i32>(2), const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_u]])), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(neg<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_u]]))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<bool>(%[[VALUE_u]], ne<i32, reason=assign>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(not<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_u]]))), not<i32>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<bool>(%[[VALUE_u]], ne<i32, reason=assign>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(not<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_u]]))), not<i32>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<bool>(%[[VALUE_u]], ne<i32, reason=assign>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(not<bool>(read<bool>(%[[VALUE_u]]))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<bool>(%[[VALUE_u]], ne<i32, reason=assign>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(not<bool>(read<bool>(%[[VALUE_u]]))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<bool>(%[[VALUE_u]], ne<i32, reason=assign>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_u]])), const<i32>(2)), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<bool>(%[[VALUE_u]], ne<i32, reason=assign>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(mul<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_u]])), const<i32>(4)), const<i32>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_u]])), const<i32>(3)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_u]])), const<i32>(1)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(4), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_u]]))), const<i32>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_u]])), const<i32>(7)), neg<i32, overflow=ub>(const<i32>(6)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<bool>(%[[VALUE_u]], ne<i32, reason=assign>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_u]])), const<i32>(1)), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_u]])), const<i32>(1)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<bool>(%[[VALUE_u]], ne<i32, reason=assign>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:         write<bool>(%[[VALUE_v]], ne<i32, reason=assign>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(lt<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_u]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_v]]))), gt<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_u]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_v]])))), not<bool>(le<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_u]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_v]]))))), not<bool>(ge<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_u]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_v]]))))), not<bool>(eq<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_u]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_v]]))))), ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_u]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_v]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<bool>(%[[VALUE_u]], ne<i32, reason=assign>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:         write<bool>(%[[VALUE_v]], ne<i32, reason=assign>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(not<bool>(lt<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_u]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_v]])))), gt<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_u]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_v]])))), not<bool>(le<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_u]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_v]]))))), ge<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_u]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_v]])))), eq<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_u]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_v]])))), not<bool>(ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_u]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_v]])))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<bool>(%[[VALUE_u]], ne<i32, reason=assign>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(or<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_u]])), const<i32>(2)), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(xor<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_u]])), const<i32>(3)), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(and<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_u]])), const<i32>(1)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(and<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_u]])), const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<bool>(%[[VALUE_u]], ne<i32, reason=assign>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:         write<bool>(%[[VALUE_v]], ne<i32, reason=assign>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:         if not<bool>(logical_or<bool>(read<bool>(%[[VALUE_u]]), read<bool>(%[[VALUE_v]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(logical_or<bool>(read<bool>(%[[VALUE_v]]), read<bool>(%[[VALUE_u]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_and<bool>(read<bool>(%[[VALUE_u]]), read<bool>(%[[VALUE_v]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_and<bool>(read<bool>(%[[VALUE_v]]), read<bool>(%[[VALUE_u]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<bool>(%[[VALUE_u]], ne<i32, reason=assign>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:         write<bool>(%[[VALUE_v]], ne<i32, reason=assign>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:         if not<bool>(logical_and<bool>(read<bool>(%[[VALUE_u]]), read<bool>(%[[VALUE_v]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<bool>(%[[VALUE_u]], ne<i32, reason=assign>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(conditional<i32>(read<bool>(%[[VALUE_u]]), const<i32>(4), const<i32>(7)), const<i32>(7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<bool>(%[[VALUE_u]], ne<i32, reason=assign>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:         write<bool>(%[[VALUE_v]], ne<i32, reason=assign>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_u]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_v]]))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), const<i32>(4), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_u]]))), const<i32>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE20:[0-9]+]]: bool [synthetic] = ne<i32, reason=assign>(const<i32>(2), const<i32>(0));
// DEFAULT-NEXT:         write<bool>(%[[VALUE_u]], read<bool>(%[[VALUE20]]));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE20]])), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_u]])), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE21:[0-9]+]]: bool [synthetic] = read<bool>(%[[VALUE_u]]);
// DEFAULT-NEXT:         let %[[VALUE22:[0-9]+]]: bool [synthetic] = ne<i32, reason=assign>(mul<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE21]])), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0));
// DEFAULT-NEXT:         write<bool>(%[[VALUE_u]], read<bool>(%[[VALUE22]]));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE22]])), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_u]])), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE23:[0-9]+]]: bool [synthetic] = read<bool>(%[[VALUE_u]]);
// DEFAULT-NEXT:         let %[[VALUE24:[0-9]+]]: bool [synthetic] = ne<i32, reason=assign>(div<i32, by_zero=ub, min_by_neg_one=ub>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE23]])), const<i32>(2)), const<i32>(0));
// DEFAULT-NEXT:         write<bool>(%[[VALUE_u]], read<bool>(%[[VALUE24]]));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE24]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE25:[0-9]+]]: bool [synthetic] = read<bool>(%[[VALUE_u]]);
// DEFAULT-NEXT:         let %[[VALUE26:[0-9]+]]: bool [synthetic] = ne<i32, reason=assign>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE25]])), const<i32>(3)), const<i32>(0));
// DEFAULT-NEXT:         write<bool>(%[[VALUE_u]], read<bool>(%[[VALUE26]]));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE26]])), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE27:[0-9]+]]: bool [synthetic] = read<bool>(%[[VALUE_u]]);
// DEFAULT-NEXT:         let %[[VALUE28:[0-9]+]]: bool [synthetic] = ne<i32, reason=assign>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE27]])), const<i32>(1)), const<i32>(0));
// DEFAULT-NEXT:         write<bool>(%[[VALUE_u]], read<bool>(%[[VALUE28]]));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE28]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<bool>(%[[VALUE_u]], ne<i32, reason=assign>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE29:[0-9]+]]: bool [synthetic] = read<bool>(%[[VALUE_u]]);
// DEFAULT-NEXT:         let %[[VALUE30:[0-9]+]]: bool [synthetic] = ne<i32, reason=assign>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE29]])), const<i32>(4)), const<i32>(0));
// DEFAULT-NEXT:         write<bool>(%[[VALUE_u]], read<bool>(%[[VALUE30]]));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE30]])), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE31:[0-9]+]]: bool [synthetic] = read<bool>(%[[VALUE_u]]);
// DEFAULT-NEXT:         let %[[VALUE32:[0-9]+]]: bool [synthetic] = ne<i32, reason=assign>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE31]])), const<i32>(1)), const<i32>(0));
// DEFAULT-NEXT:         write<bool>(%[[VALUE_u]], read<bool>(%[[VALUE32]]));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE32]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<bool>(%[[VALUE_u]], ne<i32, reason=assign>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE33:[0-9]+]]: bool [synthetic] = read<bool>(%[[VALUE_u]]);
// DEFAULT-NEXT:         let %[[VALUE34:[0-9]+]]: bool [synthetic] = ne<i32, reason=assign>(and<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE33]])), const<i32>(0)), const<i32>(0));
// DEFAULT-NEXT:         write<bool>(%[[VALUE_u]], read<bool>(%[[VALUE34]]));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE34]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE35:[0-9]+]]: bool [synthetic] = read<bool>(%[[VALUE_u]]);
// DEFAULT-NEXT:         let %[[VALUE36:[0-9]+]]: bool [synthetic] = ne<i32, reason=assign>(or<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE35]])), const<i32>(2)), const<i32>(0));
// DEFAULT-NEXT:         write<bool>(%[[VALUE_u]], read<bool>(%[[VALUE36]]));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE36]])), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE37:[0-9]+]]: bool [synthetic] = read<bool>(%[[VALUE_u]]);
// DEFAULT-NEXT:         let %[[VALUE38:[0-9]+]]: bool [synthetic] = ne<i32, reason=assign>(xor<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE37]])), const<i32>(3)), const<i32>(0));
// DEFAULT-NEXT:         write<bool>(%[[VALUE_u]], read<bool>(%[[VALUE38]]));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE38]])), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<bool>(%[[VALUE_u]], ne<i32, reason=assign>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:         const<i32>(4);
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_u]])), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:             for %[[VALUE39:[0-9]+]]
// DEFAULT-NEXT:                 init:
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:                 condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i]]))), const<u64>(1))
// DEFAULT-NEXT:                 increment: {
// DEFAULT-NEXT:                     let %[[VALUE40:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                     let %[[VALUE41:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE40]]), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE41]]));
// DEFAULT-NEXT:                     yield void;
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:                 body:
// DEFAULT-NEXT:                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(pointer_cast<ptr<u8>, reason=explicit>(addr_of<ptr<@type[[TYPE_foo]]>>(%[[VALUE_sf]])), read<i32>(%[[VALUE_i]]))), reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:             write<bool>(bitfield0<unit=0, bytes=0..1, bits=0..1>(%[[VALUE_sf]]), ne<i32, reason=assign>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:             if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(bitfield0<unit=0, bytes=0..1, bits=0..1>(%[[VALUE_sf]]))), const<i32>(1))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             write<bool>(bitfield0<unit=0, bytes=0..1, bits=0..1>(%[[VALUE_sf]]), ne<i32, reason=assign>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:             if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(bitfield0<unit=0, bytes=0..1, bits=0..1>(%[[VALUE_sf]]))), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
