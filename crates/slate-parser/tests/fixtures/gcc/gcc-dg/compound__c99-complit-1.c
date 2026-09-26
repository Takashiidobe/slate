/* Test for compound literals: in C99 only.  Test for valid uses.  */
/* Origin: Joseph Myers <jsm28@cam.ac.uk> */
/* { dg-do run } */
/* { dg-options "-std=iso9899:1999 -pedantic-errors" } */

extern void abort(void);
extern void exit(int);

struct s {
  int a;
  int b;
};
union u {
  int c;
  int d;
};

int       *i0a = &(int){0};
int       *i0b = &(int){0};
int       *i1a = &(int){1};
int       *i1b = &(int){1};
const int *i0c = &(const int){0};

struct s       *s0 = &(struct s){1, 2};
struct s       *s1 = &(struct s){1, 2};
const struct s *s2 = &(const struct s){1, 2};

union u       *u0 = &(union u){3};
union u       *u1 = &(union u){3};
const union u *u2 = &(const union u){3};

int       *a0 = (int[]){1, 2, 3};
const int *a1 = (const int[]){1, 2, 3};

char *p = (char[]){"foo"};

int main(void) {
  if (i0a == i0b || i0a == i0c || i0b == i0c)
    abort();
  if (i1a == i1b)
    abort();
  if (*i0a != 0 || *i0b != 0 || *i1a != 1 || *i1b != 1 || *i0c != 0)
    abort();
  *i0a = 1;
  *i1a = 0;
  if (*i0a != 1 || *i0b != 0 || *i1a != 0 || *i1b != 1 || *i0c != 0)
    abort();
  if (s0 == s1 || s1 == s2 || s2 == s0)
    abort();
  if (s0->a != 1 || s0->b != 2 || s1->a != 1 || s1->b != 2 || s2->a != 1 ||
      s2->b != 2)
    abort();
  s0->a = 2;
  s1->b = 1;
  if (s0->a != 2 || s0->b != 2 || s1->a != 1 || s1->b != 1 || s2->a != 1 ||
      s2->b != 2)
    abort();
  if (u0 == u1 || u1 == u2 || u2 == u0)
    abort();
  if (u0->c != 3 || u1->c != 3 || u2->c != 3)
    abort();
  u0->d = 2;
  if (u0->d != 2 || u1->c != 3 || u2->c != 3)
    abort();
  if (a0 == a1)
    abort();
  if (a0[0] != 1 || a0[1] != 2 || a0[2] != 3 || a1[0] != 1 || a1[1] != 2 ||
      a1[2] != 3)
    abort();
  a0[0] = 3;
  if (a0[0] != 3 || a0[1] != 2 || a0[2] != 3 || a1[0] != 1 || a1[1] != 2 ||
      a1[2] != 3)
    abort();
  if (p[0] != 'f' || p[1] != 'o' || p[2] != 'o' || p[3] != 0)
    abort();
  p[0] = 'g';
  if (p[0] != 'g' || p[1] != 'o' || p[2] != 'o' || p[3] != 0)
    abort();
  if (sizeof((int[]){1, 2, 3}) != 3 * sizeof(int))
    abort();
  if (sizeof((int[]){[3] = 4}) != 4 * sizeof(int))
    abort();
  struct s *y;
  for (int i = 0; i < 3; i++) {
    struct s *x = &(struct s){1, i};
    if (x->a != 1 || x->b != i)
      abort();
    x->a++;
    x->b--;
    if (x->a != 2 || x->b != i - 1)
      abort();
    if (i && y != x)
      abort();
    y = x;
  }
  int *z;
  for (int i = 0; i < 4; i++) {
    int *x = (int[]){0, i, i + 2, i - 3};
    if (x[0] != 0 || x[1] != i || x[2] != i + 2 || x[3] != i - 3)
      abort();
    x[0]  = x[1];
    x[1] *= x[2];
    x[2] -= x[3];
    x[3] += 7;
    if (x[0] != i || x[1] != i * (i + 2) || x[2] != 5 || x[3] != i + 4)
      abort();
    if (i && z != x)
      abort();
    z = x;
  }
  (int){0}           = 1;
  (struct s){0, 1}.a = 3;
  (union u){3}.c     = 4;
  (int[]){1, 2}[0]   = 0;
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
// DEFAULT-NEXT:     type @type0 s = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type1 u = union {
// DEFAULT-NEXT:         field0 c: i32;
// DEFAULT-NEXT:         field1 d: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     global %4 i0a: ptr<i32> [storage=static] = addr_of<ptr<i32>>(compound_literal %26 [storage=static] = const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     global %5 i0b: ptr<i32> [storage=static] = addr_of<ptr<i32>>(compound_literal %27 [storage=static] = const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     global %6 i1a: ptr<i32> [storage=static] = addr_of<ptr<i32>>(compound_literal %28 [storage=static] = const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %7 i1b: ptr<i32> [storage=static] = addr_of<ptr<i32>>(compound_literal %29 [storage=static] = const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %8 i0c: ptr<const i32> [storage=static] = addr_of<ptr<const i32>>(compound_literal %30 [storage=static] = const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     global %9 s0: ptr<@type0> [storage=static] = addr_of<ptr<@type0>>(compound_literal %31 [storage=static] = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(2))) [linkage=external];
// DEFAULT-NEXT:     global %10 s1: ptr<@type0> [storage=static] = addr_of<ptr<@type0>>(compound_literal %32 [storage=static] = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(2))) [linkage=external];
// DEFAULT-NEXT:     global %11 s2: ptr<const @type0> [storage=static] = addr_of<ptr<const @type0>>(compound_literal %33 [storage=static] = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(2))) [linkage=external];
// DEFAULT-NEXT:     global %12 u0: ptr<@type1> [storage=static] = addr_of<ptr<@type1>>(compound_literal %34 [storage=static] = aggregate<@type1, zero_fill=false>(field0 = const<i32>(3))) [linkage=external];
// DEFAULT-NEXT:     global %13 u1: ptr<@type1> [storage=static] = addr_of<ptr<@type1>>(compound_literal %35 [storage=static] = aggregate<@type1, zero_fill=false>(field0 = const<i32>(3))) [linkage=external];
// DEFAULT-NEXT:     global %14 u2: ptr<const @type1> [storage=static] = addr_of<ptr<const @type1>>(compound_literal %36 [storage=static] = aggregate<@type1, zero_fill=false>(field0 = const<i32>(3))) [linkage=external];
// DEFAULT-NEXT:     global %15 a0: ptr<i32> [storage=static] = array_decay<ptr<i32>, length=Some(3)>(compound_literal %37 [storage=static] = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2), index2 = const<i32>(3))) [linkage=external];
// DEFAULT-NEXT:     global %16 a1: ptr<const i32> [storage=static] = array_decay<ptr<const i32>, length=Some(3)>(compound_literal %38 [storage=static] = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2), index2 = const<i32>(3))) [linkage=external];
// DEFAULT-NEXT:     global %17 p: ptr<i8> [storage=static] = array_decay<ptr<i8>, length=Some(4)>(compound_literal %39 [storage=static] = code_units<array<i8, 4>>([102, 111, 111, 0])) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%25 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %18 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(eq<ptr<i32>>(read<ptr<i32>>(%4), read<ptr<i32>>(%5)), eq<ptr<i32>>(read<ptr<i32>>(%4), pointer_cast<ptr<i32>, reason=usual_arith>(read<ptr<const i32>>(%8)))), eq<ptr<i32>>(read<ptr<i32>>(%5), pointer_cast<ptr<i32>, reason=usual_arith>(read<ptr<const i32>>(%8))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if eq<ptr<i32>>(read<ptr<i32>>(%6), read<ptr<i32>>(%7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(deref(read<ptr<i32>>(%4))), const<i32>(0)), ne<i32>(read<i32>(deref(read<ptr<i32>>(%5))), const<i32>(0))), ne<i32>(read<i32>(deref(read<ptr<i32>>(%6))), const<i32>(1))), ne<i32>(read<i32>(deref(read<ptr<i32>>(%7))), const<i32>(1))), ne<i32>(read<i32>(deref(read<ptr<const i32>>(%8))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%4)), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%6)), const<i32>(0));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(deref(read<ptr<i32>>(%4))), const<i32>(1)), ne<i32>(read<i32>(deref(read<ptr<i32>>(%5))), const<i32>(0))), ne<i32>(read<i32>(deref(read<ptr<i32>>(%6))), const<i32>(0))), ne<i32>(read<i32>(deref(read<ptr<i32>>(%7))), const<i32>(1))), ne<i32>(read<i32>(deref(read<ptr<const i32>>(%8))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(eq<ptr<@type0>>(read<ptr<@type0>>(%9), read<ptr<@type0>>(%10)), eq<ptr<@type0>>(read<ptr<@type0>>(%10), pointer_cast<ptr<@type0>, reason=usual_arith>(read<ptr<const @type0>>(%11)))), eq<ptr<const @type0>>(read<ptr<const @type0>>(%11), pointer_cast<ptr<const @type0>, reason=usual_arith>(read<ptr<@type0>>(%9))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(field0(deref(read<ptr<@type0>>(%9)))), const<i32>(1)), ne<i32>(read<i32>(field1(deref(read<ptr<@type0>>(%9)))), const<i32>(2))), ne<i32>(read<i32>(field0(deref(read<ptr<@type0>>(%10)))), const<i32>(1))), ne<i32>(read<i32>(field1(deref(read<ptr<@type0>>(%10)))), const<i32>(2))), ne<i32>(read<i32>(field0(deref(read<ptr<const @type0>>(%11)))), const<i32>(1))), ne<i32>(read<i32>(field1(deref(read<ptr<const @type0>>(%11)))), const<i32>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type0>>(%9))), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(field1(deref(read<ptr<@type0>>(%10))), const<i32>(1));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(field0(deref(read<ptr<@type0>>(%9)))), const<i32>(2)), ne<i32>(read<i32>(field1(deref(read<ptr<@type0>>(%9)))), const<i32>(2))), ne<i32>(read<i32>(field0(deref(read<ptr<@type0>>(%10)))), const<i32>(1))), ne<i32>(read<i32>(field1(deref(read<ptr<@type0>>(%10)))), const<i32>(1))), ne<i32>(read<i32>(field0(deref(read<ptr<const @type0>>(%11)))), const<i32>(1))), ne<i32>(read<i32>(field1(deref(read<ptr<const @type0>>(%11)))), const<i32>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(eq<ptr<@type1>>(read<ptr<@type1>>(%12), read<ptr<@type1>>(%13)), eq<ptr<@type1>>(read<ptr<@type1>>(%13), pointer_cast<ptr<@type1>, reason=usual_arith>(read<ptr<const @type1>>(%14)))), eq<ptr<const @type1>>(read<ptr<const @type1>>(%14), pointer_cast<ptr<const @type1>, reason=usual_arith>(read<ptr<@type1>>(%12))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(field0(deref(read<ptr<@type1>>(%12)))), const<i32>(3)), ne<i32>(read<i32>(field0(deref(read<ptr<@type1>>(%13)))), const<i32>(3))), ne<i32>(read<i32>(field0(deref(read<ptr<const @type1>>(%14)))), const<i32>(3)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i32>(field1(deref(read<ptr<@type1>>(%12))), const<i32>(2));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(field1(deref(read<ptr<@type1>>(%12)))), const<i32>(2)), ne<i32>(read<i32>(field0(deref(read<ptr<@type1>>(%13)))), const<i32>(3))), ne<i32>(read<i32>(field0(deref(read<ptr<const @type1>>(%14)))), const<i32>(3)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if eq<ptr<i32>>(read<ptr<i32>>(%15), pointer_cast<ptr<i32>, reason=usual_arith>(read<ptr<const i32>>(%16)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%15), const<i32>(0)))), const<i32>(1)), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%15), const<i32>(1)))), const<i32>(2))), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%15), const<i32>(2)))), const<i32>(3))), ne<i32>(read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(read<ptr<const i32>>(%16), const<i32>(0)))), const<i32>(1))), ne<i32>(read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(read<ptr<const i32>>(%16), const<i32>(1)))), const<i32>(2))), ne<i32>(read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(read<ptr<const i32>>(%16), const<i32>(2)))), const<i32>(3)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%15), const<i32>(0))), const<i32>(3));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%15), const<i32>(0)))), const<i32>(3)), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%15), const<i32>(1)))), const<i32>(2))), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%15), const<i32>(2)))), const<i32>(3))), ne<i32>(read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(read<ptr<const i32>>(%16), const<i32>(0)))), const<i32>(1))), ne<i32>(read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(read<ptr<const i32>>(%16), const<i32>(1)))), const<i32>(2))), ne<i32>(read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(read<ptr<const i32>>(%16), const<i32>(2)))), const<i32>(3)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%17), const<i32>(0))))), const<i32>(102)), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%17), const<i32>(1))))), const<i32>(111))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%17), const<i32>(2))))), const<i32>(111))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%17), const<i32>(3))))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%17), const<i32>(0))), truncate<i8, reason=assign, fits=always>(const<i32>(103)));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%17), const<i32>(0))))), const<i32>(103)), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%17), const<i32>(1))))), const<i32>(111))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%17), const<i32>(2))))), const<i32>(111))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%17), const<i32>(3))))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u64>(const<u64>(12), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))), const<u64>(4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u64>(const<u64>(16), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))), const<u64>(4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %19 y: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:         for %40
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %20 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%20), const<i32>(3))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %48: i32 [synthetic] = read<i32>(%20);
// DEFAULT-NEXT:                 let %49: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%48), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%20, read<i32>(%49));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %21 x: ptr<@type0> [storage=automatic] = addr_of<ptr<@type0>>(compound_literal %41 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1), field1 = read<i32>(%20)));
// DEFAULT-NEXT:                     if logical_or<bool>(ne<i32>(read<i32>(field0(deref(read<ptr<@type0>>(%21)))), const<i32>(1)), ne<i32>(read<i32>(field1(deref(read<ptr<@type0>>(%21)))), read<i32>(%20)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     let %50: ptr<@type0> [synthetic] = read<ptr<@type0>>(%21);
// DEFAULT-NEXT:                     let %51: i32 [synthetic] = read<i32>(field0(deref(read<ptr<@type0>>(%50))));
// DEFAULT-NEXT:                     let %52: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%51), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(field0(deref(read<ptr<@type0>>(%50))), read<i32>(%52));
// DEFAULT-NEXT:                     let %53: ptr<@type0> [synthetic] = read<ptr<@type0>>(%21);
// DEFAULT-NEXT:                     let %54: i32 [synthetic] = read<i32>(field1(deref(read<ptr<@type0>>(%53))));
// DEFAULT-NEXT:                     let %55: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%54), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(field1(deref(read<ptr<@type0>>(%53))), read<i32>(%55));
// DEFAULT-NEXT:                     if logical_or<bool>(ne<i32>(read<i32>(field0(deref(read<ptr<@type0>>(%21)))), const<i32>(2)), ne<i32>(read<i32>(field1(deref(read<ptr<@type0>>(%21)))), sub<i32, overflow=ub>(read<i32>(%20), const<i32>(1))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     if logical_and<bool>(ne<i32>(read<i32>(%20), const<i32>(0)), ne<ptr<@type0>>(read<ptr<@type0>>(%19), read<ptr<@type0>>(%21)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     write<ptr<@type0>>(%19, read<ptr<@type0>>(%21));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         let %22 z: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         for %42
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %23 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%23), const<i32>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %56: i32 [synthetic] = read<i32>(%23);
// DEFAULT-NEXT:                 let %57: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%56), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%23, read<i32>(%57));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %24 x: ptr<i32> [storage=automatic] = array_decay<ptr<i32>, length=Some(4)>(compound_literal %43 [storage=automatic] = aggregate<array<i32, 4>, zero_fill=false>(index0 = const<i32>(0), index1 = read<i32>(%23), index2 = add<i32, overflow=ub>(read<i32>(%23), const<i32>(2)), index3 = sub<i32, overflow=ub>(read<i32>(%23), const<i32>(3))));
// DEFAULT-NEXT:                     if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%24), const<i32>(0)))), const<i32>(0)), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%24), const<i32>(1)))), read<i32>(%23))), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%24), const<i32>(2)))), add<i32, overflow=ub>(read<i32>(%23), const<i32>(2)))), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%24), const<i32>(3)))), sub<i32, overflow=ub>(read<i32>(%23), const<i32>(3))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%24), const<i32>(0))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%24), const<i32>(1)))));
// DEFAULT-NEXT:                     let %58: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%24), const<i32>(1));
// DEFAULT-NEXT:                     let %59: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%58)));
// DEFAULT-NEXT:                     let %60: i32 [synthetic] = mul<i32, overflow=ub>(read<i32>(%59), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%24), const<i32>(2)))));
// DEFAULT-NEXT:                     write<i32>(deref(read<ptr<i32>>(%58)), read<i32>(%60));
// DEFAULT-NEXT:                     let %61: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%24), const<i32>(2));
// DEFAULT-NEXT:                     let %62: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%61)));
// DEFAULT-NEXT:                     let %63: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%62), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%24), const<i32>(3)))));
// DEFAULT-NEXT:                     write<i32>(deref(read<ptr<i32>>(%61)), read<i32>(%63));
// DEFAULT-NEXT:                     let %64: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%24), const<i32>(3));
// DEFAULT-NEXT:                     let %65: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%64)));
// DEFAULT-NEXT:                     let %66: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%65), const<i32>(7));
// DEFAULT-NEXT:                     write<i32>(deref(read<ptr<i32>>(%64)), read<i32>(%66));
// DEFAULT-NEXT:                     if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%24), const<i32>(0)))), read<i32>(%23)), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%24), const<i32>(1)))), mul<i32, overflow=ub>(read<i32>(%23), add<i32, overflow=ub>(read<i32>(%23), const<i32>(2))))), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%24), const<i32>(2)))), const<i32>(5))), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%24), const<i32>(3)))), add<i32, overflow=ub>(read<i32>(%23), const<i32>(4))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     if logical_and<bool>(ne<i32>(read<i32>(%23), const<i32>(0)), ne<ptr<i32>>(read<ptr<i32>>(%22), read<ptr<i32>>(%24)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     write<ptr<i32>>(%22, read<ptr<i32>>(%24));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         write<i32>(compound_literal %44 [storage=automatic] = const<i32>(0), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(field0(compound_literal %45 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = const<i32>(0), field1 = const<i32>(1))), const<i32>(3));
// DEFAULT-NEXT:         write<i32>(field0(compound_literal %46 [storage=automatic] = aggregate<@type1, zero_fill=false>(field0 = const<i32>(3))), const<i32>(4));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(compound_literal %47 [storage=automatic] = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2))), const<i32>(0))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
