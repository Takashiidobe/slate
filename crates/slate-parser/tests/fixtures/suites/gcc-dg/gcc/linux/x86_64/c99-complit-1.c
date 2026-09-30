/* Test for compound literals: in C99 only.  Test for valid uses.  */
/* Origin: Joseph Myers <jsm28@cam.ac.uk> */
/* { dg-do run } */
/* { dg-options "-std=iso9899:1999 -pedantic-errors" } */

extern void abort (void);
extern void exit (int);

struct s { int a; int b; };
union u { int c; int d; };

int *i0a = &(int) { 0 };
int *i0b = &(int) { 0 };
int *i1a = &(int) { 1 };
int *i1b = &(int) { 1 };
const int *i0c = &(const int) { 0 };

struct s *s0 = &(struct s) { 1, 2 };
struct s *s1 = &(struct s) { 1, 2 };
const struct s *s2 = &(const struct s) { 1, 2 };

union u *u0 = &(union u) { 3 };
union u *u1 = &(union u) { 3 };
const union u *u2 = &(const union u) { 3 };

int *a0 = (int []) { 1, 2, 3 };
const int *a1 = (const int []) { 1, 2, 3 };

char *p = (char []){ "foo" };

int
main (void)
{
  if (i0a == i0b || i0a == i0c || i0b == i0c)
    abort ();
  if (i1a == i1b)
    abort ();
  if (*i0a != 0 || *i0b != 0 || *i1a != 1 || *i1b != 1 || *i0c != 0)
    abort ();
  *i0a = 1;
  *i1a = 0;
  if (*i0a != 1 || *i0b != 0 || *i1a != 0 || *i1b != 1 || *i0c != 0)
    abort ();
  if (s0 == s1 || s1 == s2 || s2 == s0)
    abort ();
  if (s0->a != 1 || s0->b != 2 || s1->a != 1 || s1->b != 2
      || s2->a != 1 || s2->b != 2)
    abort ();
  s0->a = 2;
  s1->b = 1;
  if (s0->a != 2 || s0->b != 2 || s1->a != 1 || s1->b != 1
      || s2->a != 1 || s2->b != 2)
    abort ();
  if (u0 == u1 || u1 == u2 || u2 == u0)
    abort ();
  if (u0->c != 3 || u1->c != 3 || u2->c != 3)
    abort ();
  u0->d = 2;
  if (u0->d != 2 || u1->c != 3 || u2->c != 3)
    abort ();
  if (a0 == a1)
    abort ();
  if (a0[0] != 1 || a0[1] != 2 || a0[2] != 3
      || a1[0] != 1 || a1[1] != 2 || a1[2] != 3)
    abort ();
  a0[0] = 3;
  if (a0[0] != 3 || a0[1] != 2 || a0[2] != 3
      || a1[0] != 1 || a1[1] != 2 || a1[2] != 3)
    abort ();
  if (p[0] != 'f' || p[1] != 'o' || p[2] != 'o' || p[3] != 0)
    abort ();
  p[0] = 'g';
  if (p[0] != 'g' || p[1] != 'o' || p[2] != 'o' || p[3] != 0)
    abort ();
  if (sizeof((int []) { 1, 2 ,3 }) != 3 * sizeof(int))
    abort ();
  if (sizeof((int []) { [3] = 4 }) != 4 * sizeof(int))
    abort ();
  struct s *y;
  for (int i = 0; i < 3; i++) {
    struct s *x = &(struct s) { 1, i };
    if (x->a != 1 || x->b != i)
      abort ();
    x->a++;
    x->b--;
    if (x->a != 2 || x->b != i - 1)
      abort ();
    if (i && y != x)
      abort ();
    y = x;
  }
  int *z;
  for (int i = 0; i < 4; i++) {
    int *x = (int []){ 0, i, i + 2, i - 3 };
    if (x[0] != 0 || x[1] != i || x[2] != i + 2 || x[3] != i - 3)
      abort ();
    x[0] = x[1];
    x[1] *= x[2];
    x[2] -= x[3];
    x[3] += 7;
    if (x[0] != i || x[1] != i * (i + 2) || x[2] != 5 || x[3] != i + 4)
      abort ();
    if (i && z != x)
      abort ();
    z = x;
  }
  (int) { 0 } = 1;
  (struct s) { 0, 1 }.a = 3;
  (union u) { 3 }.c = 4;
  (int []){ 1, 2 }[0] = 0;
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
// DEFAULT-NEXT:     type @type[[TYPE_s:[0-9]+]] s = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_u:[0-9]+]] u = union {
// DEFAULT-NEXT:         field0 c: i32;
// DEFAULT-NEXT:         field1 d: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     global %[[VALUE_i0a:[0-9]+]] i0a: ptr<i32> [storage=static] = addr_of<ptr<i32>>(compound_literal %[[VALUE0:[0-9]+]] [storage=static] = const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_i0b:[0-9]+]] i0b: ptr<i32> [storage=static] = addr_of<ptr<i32>>(compound_literal %[[VALUE1:[0-9]+]] [storage=static] = const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_i1a:[0-9]+]] i1a: ptr<i32> [storage=static] = addr_of<ptr<i32>>(compound_literal %[[VALUE2:[0-9]+]] [storage=static] = const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_i1b:[0-9]+]] i1b: ptr<i32> [storage=static] = addr_of<ptr<i32>>(compound_literal %[[VALUE3:[0-9]+]] [storage=static] = const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_i0c:[0-9]+]] i0c: ptr<const i32> [storage=static] = addr_of<ptr<const i32>>(compound_literal %[[VALUE4:[0-9]+]] [storage=static] = const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_s0:[0-9]+]] s0: ptr<@type[[TYPE_s]]> [storage=static] = addr_of<ptr<@type[[TYPE_s]]>>(compound_literal %[[VALUE5:[0-9]+]] [storage=static] = aggregate<@type[[TYPE_s]], zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(2))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_s1:[0-9]+]] s1: ptr<@type[[TYPE_s]]> [storage=static] = addr_of<ptr<@type[[TYPE_s]]>>(compound_literal %[[VALUE6:[0-9]+]] [storage=static] = aggregate<@type[[TYPE_s]], zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(2))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_s2:[0-9]+]] s2: ptr<const @type[[TYPE_s]]> [storage=static] = addr_of<ptr<const @type[[TYPE_s]]>>(compound_literal %[[VALUE7:[0-9]+]] [storage=static] = aggregate<@type[[TYPE_s]], zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(2))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_u0:[0-9]+]] u0: ptr<@type[[TYPE_u]]> [storage=static] = addr_of<ptr<@type[[TYPE_u]]>>(compound_literal %[[VALUE8:[0-9]+]] [storage=static] = aggregate<@type[[TYPE_u]], zero_fill=false>(field0 = const<i32>(3))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_u1:[0-9]+]] u1: ptr<@type[[TYPE_u]]> [storage=static] = addr_of<ptr<@type[[TYPE_u]]>>(compound_literal %[[VALUE9:[0-9]+]] [storage=static] = aggregate<@type[[TYPE_u]], zero_fill=false>(field0 = const<i32>(3))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_u2:[0-9]+]] u2: ptr<const @type[[TYPE_u]]> [storage=static] = addr_of<ptr<const @type[[TYPE_u]]>>(compound_literal %[[VALUE10:[0-9]+]] [storage=static] = aggregate<@type[[TYPE_u]], zero_fill=false>(field0 = const<i32>(3))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_a0:[0-9]+]] a0: ptr<i32> [storage=static] = array_decay<ptr<i32>, length=Some(3)>(compound_literal %[[VALUE11:[0-9]+]] [storage=static] = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2), index2 = const<i32>(3))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_a1:[0-9]+]] a1: ptr<const i32> [storage=static] = array_decay<ptr<const i32>, length=Some(3)>(compound_literal %[[VALUE12:[0-9]+]] [storage=static] = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2), index2 = const<i32>(3))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_p:[0-9]+]] p: ptr<i8> [storage=static] = array_decay<ptr<i8>, length=Some(4)>(compound_literal %[[VALUE13:[0-9]+]] [storage=static] = code_units<array<i8, 4>>([102, 111, 111, 0])) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE14:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(eq<ptr<i32>>(read<ptr<i32>>(%[[VALUE_i0a]]), read<ptr<i32>>(%[[VALUE_i0b]])), eq<ptr<i32>>(read<ptr<i32>>(%[[VALUE_i0a]]), pointer_cast<ptr<i32>, reason=usual_arith>(read<ptr<const i32>>(%[[VALUE_i0c]])))), eq<ptr<i32>>(read<ptr<i32>>(%[[VALUE_i0b]]), pointer_cast<ptr<i32>, reason=usual_arith>(read<ptr<const i32>>(%[[VALUE_i0c]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if eq<ptr<i32>>(read<ptr<i32>>(%[[VALUE_i1a]]), read<ptr<i32>>(%[[VALUE_i1b]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(deref(read<ptr<i32>>(%[[VALUE_i0a]]))), const<i32>(0)), ne<i32>(read<i32>(deref(read<ptr<i32>>(%[[VALUE_i0b]]))), const<i32>(0))), ne<i32>(read<i32>(deref(read<ptr<i32>>(%[[VALUE_i1a]]))), const<i32>(1))), ne<i32>(read<i32>(deref(read<ptr<i32>>(%[[VALUE_i1b]]))), const<i32>(1))), ne<i32>(read<i32>(deref(read<ptr<const i32>>(%[[VALUE_i0c]]))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE_i0a]])), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE_i1a]])), const<i32>(0));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(deref(read<ptr<i32>>(%[[VALUE_i0a]]))), const<i32>(1)), ne<i32>(read<i32>(deref(read<ptr<i32>>(%[[VALUE_i0b]]))), const<i32>(0))), ne<i32>(read<i32>(deref(read<ptr<i32>>(%[[VALUE_i1a]]))), const<i32>(0))), ne<i32>(read<i32>(deref(read<ptr<i32>>(%[[VALUE_i1b]]))), const<i32>(1))), ne<i32>(read<i32>(deref(read<ptr<const i32>>(%[[VALUE_i0c]]))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(eq<ptr<@type[[TYPE_s]]>>(read<ptr<@type[[TYPE_s]]>>(%[[VALUE_s0]]), read<ptr<@type[[TYPE_s]]>>(%[[VALUE_s1]])), eq<ptr<@type[[TYPE_s]]>>(read<ptr<@type[[TYPE_s]]>>(%[[VALUE_s1]]), pointer_cast<ptr<@type[[TYPE_s]]>, reason=usual_arith>(read<ptr<const @type[[TYPE_s]]>>(%[[VALUE_s2]])))), eq<ptr<const @type[[TYPE_s]]>>(read<ptr<const @type[[TYPE_s]]>>(%[[VALUE_s2]]), pointer_cast<ptr<const @type[[TYPE_s]]>, reason=usual_arith>(read<ptr<@type[[TYPE_s]]>>(%[[VALUE_s0]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(field0(deref(read<ptr<@type[[TYPE_s]]>>(%[[VALUE_s0]])))), const<i32>(1)), ne<i32>(read<i32>(field1(deref(read<ptr<@type[[TYPE_s]]>>(%[[VALUE_s0]])))), const<i32>(2))), ne<i32>(read<i32>(field0(deref(read<ptr<@type[[TYPE_s]]>>(%[[VALUE_s1]])))), const<i32>(1))), ne<i32>(read<i32>(field1(deref(read<ptr<@type[[TYPE_s]]>>(%[[VALUE_s1]])))), const<i32>(2))), ne<i32>(read<i32>(field0(deref(read<ptr<const @type[[TYPE_s]]>>(%[[VALUE_s2]])))), const<i32>(1))), ne<i32>(read<i32>(field1(deref(read<ptr<const @type[[TYPE_s]]>>(%[[VALUE_s2]])))), const<i32>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type[[TYPE_s]]>>(%[[VALUE_s0]]))), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(field1(deref(read<ptr<@type[[TYPE_s]]>>(%[[VALUE_s1]]))), const<i32>(1));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(field0(deref(read<ptr<@type[[TYPE_s]]>>(%[[VALUE_s0]])))), const<i32>(2)), ne<i32>(read<i32>(field1(deref(read<ptr<@type[[TYPE_s]]>>(%[[VALUE_s0]])))), const<i32>(2))), ne<i32>(read<i32>(field0(deref(read<ptr<@type[[TYPE_s]]>>(%[[VALUE_s1]])))), const<i32>(1))), ne<i32>(read<i32>(field1(deref(read<ptr<@type[[TYPE_s]]>>(%[[VALUE_s1]])))), const<i32>(1))), ne<i32>(read<i32>(field0(deref(read<ptr<const @type[[TYPE_s]]>>(%[[VALUE_s2]])))), const<i32>(1))), ne<i32>(read<i32>(field1(deref(read<ptr<const @type[[TYPE_s]]>>(%[[VALUE_s2]])))), const<i32>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(eq<ptr<@type[[TYPE_u]]>>(read<ptr<@type[[TYPE_u]]>>(%[[VALUE_u0]]), read<ptr<@type[[TYPE_u]]>>(%[[VALUE_u1]])), eq<ptr<@type[[TYPE_u]]>>(read<ptr<@type[[TYPE_u]]>>(%[[VALUE_u1]]), pointer_cast<ptr<@type[[TYPE_u]]>, reason=usual_arith>(read<ptr<const @type[[TYPE_u]]>>(%[[VALUE_u2]])))), eq<ptr<const @type[[TYPE_u]]>>(read<ptr<const @type[[TYPE_u]]>>(%[[VALUE_u2]]), pointer_cast<ptr<const @type[[TYPE_u]]>, reason=usual_arith>(read<ptr<@type[[TYPE_u]]>>(%[[VALUE_u0]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(field0(deref(read<ptr<@type[[TYPE_u]]>>(%[[VALUE_u0]])))), const<i32>(3)), ne<i32>(read<i32>(field0(deref(read<ptr<@type[[TYPE_u]]>>(%[[VALUE_u1]])))), const<i32>(3))), ne<i32>(read<i32>(field0(deref(read<ptr<const @type[[TYPE_u]]>>(%[[VALUE_u2]])))), const<i32>(3)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i32>(field1(deref(read<ptr<@type[[TYPE_u]]>>(%[[VALUE_u0]]))), const<i32>(2));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(field1(deref(read<ptr<@type[[TYPE_u]]>>(%[[VALUE_u0]])))), const<i32>(2)), ne<i32>(read<i32>(field0(deref(read<ptr<@type[[TYPE_u]]>>(%[[VALUE_u1]])))), const<i32>(3))), ne<i32>(read<i32>(field0(deref(read<ptr<const @type[[TYPE_u]]>>(%[[VALUE_u2]])))), const<i32>(3)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if eq<ptr<i32>>(read<ptr<i32>>(%[[VALUE_a0]]), pointer_cast<ptr<i32>, reason=usual_arith>(read<ptr<const i32>>(%[[VALUE_a1]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_a0]]), const<i32>(0)))), const<i32>(1)), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_a0]]), const<i32>(1)))), const<i32>(2))), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_a0]]), const<i32>(2)))), const<i32>(3))), ne<i32>(read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(read<ptr<const i32>>(%[[VALUE_a1]]), const<i32>(0)))), const<i32>(1))), ne<i32>(read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(read<ptr<const i32>>(%[[VALUE_a1]]), const<i32>(1)))), const<i32>(2))), ne<i32>(read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(read<ptr<const i32>>(%[[VALUE_a1]]), const<i32>(2)))), const<i32>(3)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_a0]]), const<i32>(0))), const<i32>(3));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_a0]]), const<i32>(0)))), const<i32>(3)), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_a0]]), const<i32>(1)))), const<i32>(2))), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_a0]]), const<i32>(2)))), const<i32>(3))), ne<i32>(read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(read<ptr<const i32>>(%[[VALUE_a1]]), const<i32>(0)))), const<i32>(1))), ne<i32>(read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(read<ptr<const i32>>(%[[VALUE_a1]]), const<i32>(1)))), const<i32>(2))), ne<i32>(read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(read<ptr<const i32>>(%[[VALUE_a1]]), const<i32>(2)))), const<i32>(3)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_p]]), const<i32>(0))))), const<i32>(102)), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_p]]), const<i32>(1))))), const<i32>(111))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_p]]), const<i32>(2))))), const<i32>(111))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_p]]), const<i32>(3))))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_p]]), const<i32>(0))), truncate<i8, reason=assign, fits=always>(const<i32>(103)));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_p]]), const<i32>(0))))), const<i32>(103)), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_p]]), const<i32>(1))))), const<i32>(111))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_p]]), const<i32>(2))))), const<i32>(111))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_p]]), const<i32>(3))))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(const<u64>(12), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))), const<u64>(4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(const<u64>(16), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))), const<u64>(4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE_y:[0-9]+]] y: ptr<@type[[TYPE_s]]> [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE15:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(3))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE16:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE17:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE16]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE17]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_x:[0-9]+]] x: ptr<@type[[TYPE_s]]> [storage=automatic] = addr_of<ptr<@type[[TYPE_s]]>>(compound_literal %[[VALUE18:[0-9]+]] [storage=automatic] = aggregate<@type[[TYPE_s]], zero_fill=false>(field0 = const<i32>(1), field1 = read<i32>(%[[VALUE_i]])));
// DEFAULT-NEXT:                     if logical_or<bool>(ne<i32>(read<i32>(field0(deref(read<ptr<@type[[TYPE_s]]>>(%[[VALUE_x]])))), const<i32>(1)), ne<i32>(read<i32>(field1(deref(read<ptr<@type[[TYPE_s]]>>(%[[VALUE_x]])))), read<i32>(%[[VALUE_i]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     let %[[VALUE19:[0-9]+]]: ptr<@type[[TYPE_s]]> [synthetic] = read<ptr<@type[[TYPE_s]]>>(%[[VALUE_x]]);
// DEFAULT-NEXT:                     let %[[VALUE20:[0-9]+]]: i32 [synthetic] = read<i32>(field0(deref(read<ptr<@type[[TYPE_s]]>>(%[[VALUE19]]))));
// DEFAULT-NEXT:                     let %[[VALUE21:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE20]]), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(field0(deref(read<ptr<@type[[TYPE_s]]>>(%[[VALUE19]]))), read<i32>(%[[VALUE21]]));
// DEFAULT-NEXT:                     let %[[VALUE22:[0-9]+]]: ptr<@type[[TYPE_s]]> [synthetic] = read<ptr<@type[[TYPE_s]]>>(%[[VALUE_x]]);
// DEFAULT-NEXT:                     let %[[VALUE23:[0-9]+]]: i32 [synthetic] = read<i32>(field1(deref(read<ptr<@type[[TYPE_s]]>>(%[[VALUE22]]))));
// DEFAULT-NEXT:                     let %[[VALUE24:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE23]]), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(field1(deref(read<ptr<@type[[TYPE_s]]>>(%[[VALUE22]]))), read<i32>(%[[VALUE24]]));
// DEFAULT-NEXT:                     if logical_or<bool>(ne<i32>(read<i32>(field0(deref(read<ptr<@type[[TYPE_s]]>>(%[[VALUE_x]])))), const<i32>(2)), ne<i32>(read<i32>(field1(deref(read<ptr<@type[[TYPE_s]]>>(%[[VALUE_x]])))), sub<i32, overflow=ub>(read<i32>(%[[VALUE_i]]), const<i32>(1))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     if logical_and<bool>(ne<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0)), ne<ptr<@type[[TYPE_s]]>>(read<ptr<@type[[TYPE_s]]>>(%[[VALUE_y]]), read<ptr<@type[[TYPE_s]]>>(%[[VALUE_x]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     write<ptr<@type[[TYPE_s]]>>(%[[VALUE_y]], read<ptr<@type[[TYPE_s]]>>(%[[VALUE_x]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         let %[[VALUE_z:[0-9]+]] z: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE25:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i_2:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE26:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE27:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE26]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE27]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_x_2:[0-9]+]] x: ptr<i32> [storage=automatic] = array_decay<ptr<i32>, length=Some(4)>(compound_literal %[[VALUE28:[0-9]+]] [storage=automatic] = aggregate<array<i32, 4>, zero_fill=false>(index0 = const<i32>(0), index1 = read<i32>(%[[VALUE_i_2]]), index2 = add<i32, overflow=ub>(read<i32>(%[[VALUE_i_2]]), const<i32>(2)), index3 = sub<i32, overflow=ub>(read<i32>(%[[VALUE_i_2]]), const<i32>(3))));
// DEFAULT-NEXT:                     if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_x_2]]), const<i32>(0)))), const<i32>(0)), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_x_2]]), const<i32>(1)))), read<i32>(%[[VALUE_i_2]]))), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_x_2]]), const<i32>(2)))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_2]]), const<i32>(2)))), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_x_2]]), const<i32>(3)))), sub<i32, overflow=ub>(read<i32>(%[[VALUE_i_2]]), const<i32>(3))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_x_2]]), const<i32>(0))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_x_2]]), const<i32>(1)))));
// DEFAULT-NEXT:                     let %[[VALUE29:[0-9]+]]: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_x_2]]), const<i32>(1));
// DEFAULT-NEXT:                     let %[[VALUE30:[0-9]+]]: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%[[VALUE29]])));
// DEFAULT-NEXT:                     let %[[VALUE31:[0-9]+]]: i32 [synthetic] = mul<i32, overflow=ub>(read<i32>(%[[VALUE30]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_x_2]]), const<i32>(2)))));
// DEFAULT-NEXT:                     write<i32>(deref(read<ptr<i32>>(%[[VALUE29]])), read<i32>(%[[VALUE31]]));
// DEFAULT-NEXT:                     let %[[VALUE32:[0-9]+]]: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_x_2]]), const<i32>(2));
// DEFAULT-NEXT:                     let %[[VALUE33:[0-9]+]]: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%[[VALUE32]])));
// DEFAULT-NEXT:                     let %[[VALUE34:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE33]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_x_2]]), const<i32>(3)))));
// DEFAULT-NEXT:                     write<i32>(deref(read<ptr<i32>>(%[[VALUE32]])), read<i32>(%[[VALUE34]]));
// DEFAULT-NEXT:                     let %[[VALUE35:[0-9]+]]: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_x_2]]), const<i32>(3));
// DEFAULT-NEXT:                     let %[[VALUE36:[0-9]+]]: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%[[VALUE35]])));
// DEFAULT-NEXT:                     let %[[VALUE37:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE36]]), const<i32>(7));
// DEFAULT-NEXT:                     write<i32>(deref(read<ptr<i32>>(%[[VALUE35]])), read<i32>(%[[VALUE37]]));
// DEFAULT-NEXT:                     if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_x_2]]), const<i32>(0)))), read<i32>(%[[VALUE_i_2]])), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_x_2]]), const<i32>(1)))), mul<i32, overflow=ub>(read<i32>(%[[VALUE_i_2]]), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_2]]), const<i32>(2))))), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_x_2]]), const<i32>(2)))), const<i32>(5))), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_x_2]]), const<i32>(3)))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_2]]), const<i32>(4))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     if logical_and<bool>(ne<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(0)), ne<ptr<i32>>(read<ptr<i32>>(%[[VALUE_z]]), read<ptr<i32>>(%[[VALUE_x_2]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     write<ptr<i32>>(%[[VALUE_z]], read<ptr<i32>>(%[[VALUE_x_2]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         write<i32>(compound_literal %[[VALUE38:[0-9]+]] [storage=automatic] = const<i32>(0), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(field0(compound_literal %[[VALUE39:[0-9]+]] [storage=automatic] = aggregate<@type[[TYPE_s]], zero_fill=false>(field0 = const<i32>(0), field1 = const<i32>(1))), const<i32>(3));
// DEFAULT-NEXT:         write<i32>(field0(compound_literal %[[VALUE40:[0-9]+]] [storage=automatic] = aggregate<@type[[TYPE_u]], zero_fill=false>(field0 = const<i32>(3))), const<i32>(4));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(compound_literal %[[VALUE41:[0-9]+]] [storage=automatic] = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2))), const<i32>(0))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
