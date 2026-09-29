/* Test for _Atomic in C11.  Test of valid code.  See c11-atomic-2.c
   for more exhaustive tests of assignment cases.  */
/* { dg-do compile } */
/* { dg-options "-std=c11 -pedantic-errors" } */

/* The use of _Atomic as a qualifier, and of _Atomic (type-name), give
   the same type.  */
extern _Atomic int a;
extern _Atomic (int) a;
extern int *_Atomic b;
extern _Atomic (int *) b;
extern void f (int [_Atomic]);
extern void f (int *_Atomic);

/* _Atomic may be applied to arbitrary types, with or without other
   qualifiers, and assignments may be made as with non-atomic
   types.  Structure and union elements may be atomic.  */
_Atomic int ai1, ai2;
int i1;
volatile _Atomic long double ald1;
const _Atomic long double ald2;
long double ld1;
_Atomic _Complex double acd1, acd2;
_Complex double d1;
_Atomic volatile _Bool ab1;
int *p;
int *_Atomic restrict ap;
struct s { char c[1000]; };
_Atomic struct s as1;
struct s s1;
struct t { _Atomic int i; };
_Atomic struct t at1;
_Atomic struct t *atp1;
struct t t1;
union u { char c[1000]; };
_Atomic union u au1;
union u u1;
union v { _Atomic int i; };
_Atomic union v av1;
union v v1;

void
func (_Atomic volatile long al1)
{
  ai1 = ai2;
  ai1 = i1;
  i1 = ai2;
  ai1 = ald2;
  ald1 = d1;
  ld1 = acd2;
  acd1 += ab1;
  acd2 /= ai1;
  p = ap;
  ap = p;
  ab1 = p;
  as1 = s1;
  s1 = as1;
  at1 = t1;
  t1 = at1;
  /* It's unclear whether the undefined behavior (6.5.2.3#5) for
     accessing elements of atomic structures and unions is at
     translation or execution time; presume here that it's at
     execution time.  */
  t1.i = at1.i; /* { dg-warning "accessing a member .i. of an atomic structure" } */
  at1.i = t1.i; /* { dg-warning "accessing a member .i. of an atomic structure" } */
  atp1->i = t1.i; /* { dg-warning "accessing a member .i. of an atomic structure" } */
  au1 = u1;
  u1 = au1;
  av1 = v1;
  v1 = av1;
  v1.i = av1.i; /* { dg-warning "accessing a member .i. of an atomic union" } */
  av1.i = v1.i; /* { dg-warning "accessing a member .i. of an atomic union" } */
  /* _Atomic is valid on register variables, even if not particularly
     useful.  */
  register _Atomic volatile int ra1 = 1, ra2 = 2;
  ra1 = ra2;
  ra2 = ra1;
  /* And on parameters.  */
  al1 = ra1;
  ra2 = al1;
}

/* A function may return an atomic type.  */
_Atomic int
func2 (int i)
{
  return i;
}

/* Casts may specify atomic type.  */
int
func3 (int i)
{
  return func2 ((_Atomic long) i);
}

/* The _Atomic void type is valid.  */
_Atomic void *avp;

/* An array of atomic elements is valid (the elements being atomic,
   not the array).  */
_Atomic int aa[10];
int
func4 (void)
{
  return aa[2];
}

/* Increment and decrement are valid for atomic types when they are
   valid for non-atomic types.  */
void
func5 (void)
{
  ald1++;
  ald1--;
  ++ald1;
  --ald1;
  ai1++;
  ai1--;
  ++ai1;
  --ai1;
  ab1++;
  ab1--;
  ++ab1;
  --ab1;
  ap++;
  ap--;
  ++ap;
  --ap;
}

/* Compound literals may have atomic type.  */
_Atomic int *aiclp = &(_Atomic int) { 1 };

/* Test unary & and *.  */
void
func6 (void)
{
  int i = *aiclp;
  _Atomic int *p = &ai2;
}

/* Casts to atomic type are valid (although the _Atomic has little
   effect because the result is an rvalue).  */
int i2 = (_Atomic int) 1.0;

/* For pointer subtraction and comparisons, _Atomic does not count as
   a qualifier.  Likewise for conditional expressions.  */
_Atomic int *xaip1;
volatile _Atomic int *xaip2;
void *xvp1;

void
func7 (void)
{
  int r;
  r = xaip1 - xaip2;
  r = xaip1 < xaip2;
  r = xaip1 > xaip2;
  r = xaip1 <= xaip2;
  r = xaip1 >= xaip2;
  r = xaip1 == xaip2;
  r = xaip1 != xaip2;
  r = xaip1 == xvp1;
  r = xaip1 != xvp1;
  r = xvp1 == xaip1;
  r = xvp1 != xaip1;
  r = xaip1 == 0;
  r = ((void *) 0) == xaip2;
  (void) (r ? xaip1 : xaip2);
  (void) (r ? xvp1 : xaip2);
  (void) (r ? xaip2 : xvp1);
  (void) (r ? xaip1 : 0);
  (void) (r ? 0 : xaip1);
  /* The result of a conditional expression between a pointer to
     qualified or unqualified (but not atomic) void, and a pointer to
     an atomic type, is a pointer to appropriately qualified, not
     atomic, void.  As such, it is valid to use further in conditional
     expressions with other pointer types.  */
  (void) (r ? xaip1 : (r ? xaip1 : xvp1));
}

/* Pointer += and -= integer is valid.  */
void
func8 (void)
{
  b += 1;
  b -= 2ULL;
  ap += 3;
}

/* Various other cases of simple assignment are valid (some already
   tested above).  */
void
func9 (void)
{
  ap = 0;
  ap = (void *) 0;
  xvp1 = atp1;
  atp1 = xvp1;
}

/* Test compatibility of function types in cases where _Atomic matches
   (see c11-atomic-3.c for corresponding cases where it doesn't
   match).  */
void fc0a (int const);
void fc0a (int);
void fc0b (int _Atomic);
void fc0b (int _Atomic);
void fc1a (int);
void
fc1a (x)
     volatile int x;
{
}
void fc1b (_Atomic int);
void
fc1b (x)
     volatile _Atomic int x;
{
}
void
fc2a (x)
     const int x;
{
}
void fc2a (int); /* { dg-warning "follows non-prototype" } */
void
fc2b (x)
     _Atomic int x;
{
}
void fc2b (_Atomic int); /* { dg-warning "follows non-prototype" } */
void fc3a (int);
void
fc3a (x)
     volatile short x;
{
}
void fc3b (_Atomic int);
void
fc3b (x)
     _Atomic short x;
{
}
void
fc4a (x)
     const short x;
{
}
void fc4a (int); /* { dg-warning "follows non-prototype" } */
void
fc4b (x)
     _Atomic short x;
{
}
void fc4b (_Atomic int); /* { dg-warning "follows non-prototype" } */

/* Test cases involving C_MAYBE_CONST_EXPR work.  */
void
func10 (_Atomic int *p)
{
  p[0 / 0] = 1; /* { dg-warning "division by zero" } */
  p[0 / 0] += 1; /* { dg-warning "division by zero" } */
  *p = 0 / 0; /* { dg-warning "division by zero" } */
  *p += 0 / 0; /* { dg-warning "division by zero" } */
}

// SLATE-FILECHECK-STD DEFAULT c11
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
// DEFAULT-NEXT:         field0 c: array<i8, 1000>;
// DEFAULT-NEXT:     } [size=1000, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type1 t = struct {
// DEFAULT-NEXT:         field0 i: atomic i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type2 u = union {
// DEFAULT-NEXT:         field0 c: array<i8, 1000>;
// DEFAULT-NEXT:     } [size=1000, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type3 v = union {
// DEFAULT-NEXT:         field0 i: atomic i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     extern %0 a: atomic i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %1 b: atomic ptr<i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 ai1: atomic i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 ai2: atomic i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 i1: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 ald1: volatile atomic f80 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %7 ald2: atomic f80 [storage=static] [const] [linkage=external];
// DEFAULT-NEXT:     global %8 ld1: f80 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %9 acd1: atomic complex<f64> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %10 acd2: atomic complex<f64> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %11 d1: complex<f64> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %12 ab1: volatile atomic bool [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %13 p: ptr<i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %14 ap: atomic ptr<i32> [storage=static] [restrict] [linkage=external];
// DEFAULT-NEXT:     global %16 as1: atomic @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %17 s1: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %19 at1: atomic @type1 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %20 atp1: ptr<atomic @type1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %21 t1: @type1 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %23 au1: atomic @type2 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %24 u1: @type2 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %26 av1: atomic @type3 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %27 v1: @type3 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %36 avp: ptr<atomic void> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %37 aa: atomic array<i32, 10> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %40 aiclp: ptr<atomic i32> [storage=static] = addr_of<ptr<atomic i32>>(compound_literal %74 [storage=static] = const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %44 i2: i32 [storage=static] = float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.0)) [linkage=external];
// DEFAULT-NEXT:     global %45 xaip1: ptr<atomic i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %46 xaip2: ptr<volatile atomic i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %47 xvp1: ptr<void> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %2 @f(%72 <unnamed>: atomic ptr<i32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %28 @func(%29 al1: volatile atomic i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32, atomic=seq_cst>(%3, read<i32, atomic=seq_cst>(%4));
// DEFAULT-NEXT:         write<i32, atomic=seq_cst>(%3, read<i32>(%5));
// DEFAULT-NEXT:         write<i32>(%5, read<i32, atomic=seq_cst>(%4));
// DEFAULT-NEXT:         write<i32, atomic=seq_cst>(%3, float_to_int<i32, reason=assign, out_of_range=ub, exceptions=observable>(read<f80, atomic=seq_cst>(%7)));
// DEFAULT-NEXT:         write<f80, volatile, atomic=seq_cst>(%6, float_widen<f80, reason=assign>(complex_to_real<f64, reason=assign>(read<complex<f64>>(%11))));
// DEFAULT-NEXT:         write<f80>(%8, float_widen<f80, reason=assign>(complex_to_real<f64, reason=assign>(read<complex<f64>, atomic=seq_cst>(%10))));
// DEFAULT-NEXT:         let %91: complex<f64> [synthetic] = update<complex<f64>, result=new, atomic=seq_cst>(%9, add<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(old<complex<f64>>, int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%12)))));
// DEFAULT-NEXT:         let %92: complex<f64> [synthetic] = update<complex<f64>, result=new, atomic=seq_cst>(%10, div<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(old<complex<f64>>, int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(read<i32, atomic=seq_cst>(%3))));
// DEFAULT-NEXT:         write<ptr<i32>>(%13, read<ptr<i32>, atomic=seq_cst>(%14));
// DEFAULT-NEXT:         write<ptr<i32>, atomic=seq_cst>(%14, read<ptr<i32>>(%13));
// DEFAULT-NEXT:         write<bool, volatile, atomic=seq_cst>(%12, ne<ptr<i32>, reason=assign>(read<ptr<i32>>(%13), null<ptr<i32>>));
// DEFAULT-NEXT:         write<@type0, atomic=seq_cst>(%16, copy<@type0, reason=assign>(read<@type0>(%17)));
// DEFAULT-NEXT:         write<@type0>(%17, copy<@type0, reason=assign>(read<@type0, atomic=seq_cst>(%16)));
// DEFAULT-NEXT:         write<@type1, atomic=seq_cst>(%19, copy<@type1, reason=assign>(read<@type1>(%21)));
// DEFAULT-NEXT:         write<@type1>(%21, copy<@type1, reason=assign>(read<@type1, atomic=seq_cst>(%19)));
// DEFAULT-NEXT:         write<i32, atomic=seq_cst>(field0(%21), read<i32, atomic=seq_cst>(field0(%19)));
// DEFAULT-NEXT:         write<i32, atomic=seq_cst>(field0(%19), read<i32, atomic=seq_cst>(field0(%21)));
// DEFAULT-NEXT:         write<i32, atomic=seq_cst>(field0(deref(read<ptr<atomic @type1>>(%20))), read<i32, atomic=seq_cst>(field0(%21)));
// DEFAULT-NEXT:         write<@type2, atomic=seq_cst>(%23, copy<@type2, reason=assign>(read<@type2>(%24)));
// DEFAULT-NEXT:         write<@type2>(%24, copy<@type2, reason=assign>(read<@type2, atomic=seq_cst>(%23)));
// DEFAULT-NEXT:         write<@type3, atomic=seq_cst>(%26, copy<@type3, reason=assign>(read<@type3>(%27)));
// DEFAULT-NEXT:         write<@type3>(%27, copy<@type3, reason=assign>(read<@type3, atomic=seq_cst>(%26)));
// DEFAULT-NEXT:         write<i32, atomic=seq_cst>(field0(%27), read<i32, atomic=seq_cst>(field0(%26)));
// DEFAULT-NEXT:         write<i32, atomic=seq_cst>(field0(%26), read<i32, atomic=seq_cst>(field0(%27)));
// DEFAULT-NEXT:         let %30 ra1: volatile atomic i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         let %31 ra2: volatile atomic i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:         write<i32, volatile, atomic=seq_cst>(%30, read<i32, volatile, atomic=seq_cst>(%31));
// DEFAULT-NEXT:         write<i32, volatile, atomic=seq_cst>(%31, read<i32, volatile, atomic=seq_cst>(%30));
// DEFAULT-NEXT:         write<i64, volatile, atomic=seq_cst>(%29, widen<i64, reason=assign>(read<i32, volatile, atomic=seq_cst>(%30)));
// DEFAULT-NEXT:         write<i32, volatile, atomic=seq_cst>(%31, truncate<i32, reason=assign, fits=unknown>(read<i64, volatile, atomic=seq_cst>(%29)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %32 @func2(%33 i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%33);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %34 @func3(%35 i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(i32) -> i32>(%32, truncate<i32, reason=arg, fits=unknown>(widen<i64, reason=explicit>(read<i32>(%35))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %38 @func4() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32, atomic=seq_cst>(deref(ptr_offset<ptr<atomic i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<atomic i32>, length=Some(10)>(%37), const<i32>(2))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %39 @func5() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %93: f80 [synthetic] = update<f80, result=old, volatile, atomic=seq_cst>(%6, add<f80, rounding=nearest_even, exceptions=observable, contract=off>(old<f80>, int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:         let %94: f80 [synthetic] = update<f80, result=old, volatile, atomic=seq_cst>(%6, sub<f80, rounding=nearest_even, exceptions=observable, contract=off>(old<f80>, int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:         let %95: f80 [synthetic] = update<f80, result=new, volatile, atomic=seq_cst>(%6, add<f80, rounding=nearest_even, exceptions=observable, contract=off>(old<f80>, int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:         let %96: f80 [synthetic] = update<f80, result=new, volatile, atomic=seq_cst>(%6, sub<f80, rounding=nearest_even, exceptions=observable, contract=off>(old<f80>, int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:         let %97: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(%3, add<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         let %98: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(%3, sub<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         let %99: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(%3, add<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         let %100: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(%3, sub<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         let %101: bool [synthetic] = update<bool, result=old, volatile, atomic=seq_cst>(%12, ne<i32, reason=assign>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(old<bool>), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:         let %102: bool [synthetic] = update<bool, result=old, volatile, atomic=seq_cst>(%12, ne<i32, reason=assign>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(old<bool>), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:         let %103: bool [synthetic] = update<bool, result=new, volatile, atomic=seq_cst>(%12, ne<i32, reason=assign>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(old<bool>), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:         let %104: bool [synthetic] = update<bool, result=new, volatile, atomic=seq_cst>(%12, ne<i32, reason=assign>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(old<bool>), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:         let %105: ptr<i32> [synthetic] = update<ptr<i32>, result=old, atomic=seq_cst>(%14, ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(old<ptr<i32>>, const<i32>(1)));
// DEFAULT-NEXT:         let %106: ptr<i32> [synthetic] = update<ptr<i32>, result=old, atomic=seq_cst>(%14, ptr_offset<ptr<i32>, subtract=true, element=i32, overflow=ub>(old<ptr<i32>>, const<i32>(1)));
// DEFAULT-NEXT:         let %107: ptr<i32> [synthetic] = update<ptr<i32>, result=new, atomic=seq_cst>(%14, ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(old<ptr<i32>>, const<i32>(1)));
// DEFAULT-NEXT:         let %108: ptr<i32> [synthetic] = update<ptr<i32>, result=new, atomic=seq_cst>(%14, ptr_offset<ptr<i32>, subtract=true, element=i32, overflow=ub>(old<ptr<i32>>, const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %41 @func6() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %42 i: i32 [storage=automatic] = read<i32, atomic=seq_cst>(deref(read<ptr<atomic i32>>(%40)));
// DEFAULT-NEXT:         let %43 p: ptr<atomic i32> [storage=automatic] = addr_of<ptr<atomic i32>>(%4);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %48 @func7() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %49 r: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%49, truncate<i32, reason=assign, fits=unknown>(ptr_diff<i64, element=i32, same_array=required, overflow=ub>(read<ptr<atomic i32>>(%45), read<ptr<volatile atomic i32>>(%46))));
// DEFAULT-NEXT:         write<i32>(%49, from_bool<i32, reason=assign>(lt<ptr<atomic i32>>(read<ptr<atomic i32>>(%45), pointer_cast<ptr<atomic i32>, reason=usual_arith>(read<ptr<volatile atomic i32>>(%46)))));
// DEFAULT-NEXT:         write<i32>(%49, from_bool<i32, reason=assign>(gt<ptr<atomic i32>>(read<ptr<atomic i32>>(%45), pointer_cast<ptr<atomic i32>, reason=usual_arith>(read<ptr<volatile atomic i32>>(%46)))));
// DEFAULT-NEXT:         write<i32>(%49, from_bool<i32, reason=assign>(le<ptr<atomic i32>>(read<ptr<atomic i32>>(%45), pointer_cast<ptr<atomic i32>, reason=usual_arith>(read<ptr<volatile atomic i32>>(%46)))));
// DEFAULT-NEXT:         write<i32>(%49, from_bool<i32, reason=assign>(ge<ptr<atomic i32>>(read<ptr<atomic i32>>(%45), pointer_cast<ptr<atomic i32>, reason=usual_arith>(read<ptr<volatile atomic i32>>(%46)))));
// DEFAULT-NEXT:         write<i32>(%49, from_bool<i32, reason=assign>(eq<ptr<atomic i32>>(read<ptr<atomic i32>>(%45), pointer_cast<ptr<atomic i32>, reason=usual_arith>(read<ptr<volatile atomic i32>>(%46)))));
// DEFAULT-NEXT:         write<i32>(%49, from_bool<i32, reason=assign>(ne<ptr<atomic i32>>(read<ptr<atomic i32>>(%45), pointer_cast<ptr<atomic i32>, reason=usual_arith>(read<ptr<volatile atomic i32>>(%46)))));
// DEFAULT-NEXT:         write<i32>(%49, from_bool<i32, reason=assign>(eq<ptr<atomic i32>>(read<ptr<atomic i32>>(%45), pointer_cast<ptr<atomic i32>, reason=usual_arith>(read<ptr<void>>(%47)))));
// DEFAULT-NEXT:         write<i32>(%49, from_bool<i32, reason=assign>(ne<ptr<atomic i32>>(read<ptr<atomic i32>>(%45), pointer_cast<ptr<atomic i32>, reason=usual_arith>(read<ptr<void>>(%47)))));
// DEFAULT-NEXT:         write<i32>(%49, from_bool<i32, reason=assign>(eq<ptr<void>>(read<ptr<void>>(%47), pointer_cast<ptr<void>, reason=usual_arith>(read<ptr<atomic i32>>(%45)))));
// DEFAULT-NEXT:         write<i32>(%49, from_bool<i32, reason=assign>(ne<ptr<void>>(read<ptr<void>>(%47), pointer_cast<ptr<void>, reason=usual_arith>(read<ptr<atomic i32>>(%45)))));
// DEFAULT-NEXT:         write<i32>(%49, from_bool<i32, reason=assign>(eq<ptr<atomic i32>>(read<ptr<atomic i32>>(%45), null<ptr<atomic i32>>)));
// DEFAULT-NEXT:         write<i32>(%49, from_bool<i32, reason=assign>(eq<ptr<void>>(null<ptr<void>>, pointer_cast<ptr<void>, reason=usual_arith>(read<ptr<volatile atomic i32>>(%46)))));
// DEFAULT-NEXT:         conditional<ptr<volatile atomic i32>>(ne<i32>(read<i32>(%49), const<i32>(0)), pointer_cast<ptr<volatile atomic i32>, reason=usual_arith>(read<ptr<atomic i32>>(%45)), read<ptr<volatile atomic i32>>(%46));
// DEFAULT-NEXT:         conditional<ptr<volatile void>>(ne<i32>(read<i32>(%49), const<i32>(0)), pointer_cast<ptr<volatile void>, reason=usual_arith>(read<ptr<void>>(%47)), pointer_cast<ptr<volatile void>, reason=usual_arith>(read<ptr<volatile atomic i32>>(%46)));
// DEFAULT-NEXT:         conditional<ptr<volatile void>>(ne<i32>(read<i32>(%49), const<i32>(0)), pointer_cast<ptr<volatile void>, reason=usual_arith>(read<ptr<volatile atomic i32>>(%46)), pointer_cast<ptr<volatile void>, reason=usual_arith>(read<ptr<void>>(%47)));
// DEFAULT-NEXT:         conditional<ptr<atomic i32>>(ne<i32>(read<i32>(%49), const<i32>(0)), read<ptr<atomic i32>>(%45), null<ptr<atomic i32>>);
// DEFAULT-NEXT:         conditional<ptr<atomic i32>>(ne<i32>(read<i32>(%49), const<i32>(0)), null<ptr<atomic i32>>, read<ptr<atomic i32>>(%45));
// DEFAULT-NEXT:         conditional<ptr<void>>(ne<i32>(read<i32>(%49), const<i32>(0)), pointer_cast<ptr<void>, reason=usual_arith>(read<ptr<atomic i32>>(%45)), conditional<ptr<void>>(ne<i32>(read<i32>(%49), const<i32>(0)), pointer_cast<ptr<void>, reason=usual_arith>(read<ptr<atomic i32>>(%45)), read<ptr<void>>(%47)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %50 @func8() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %109: ptr<i32> [synthetic] = update<ptr<i32>, result=new, atomic=seq_cst>(%1, ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(old<ptr<i32>>, const<i32>(1)));
// DEFAULT-NEXT:         let %110: ptr<i32> [synthetic] = update<ptr<i32>, result=new, atomic=seq_cst>(%1, ptr_offset<ptr<i32>, subtract=true, element=i32, overflow=ub>(old<ptr<i32>>, const<u64>(2)));
// DEFAULT-NEXT:         let %111: ptr<i32> [synthetic] = update<ptr<i32>, result=new, atomic=seq_cst>(%14, ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(old<ptr<i32>>, const<i32>(3)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %51 @func9() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<ptr<i32>, atomic=seq_cst>(%14, null<ptr<i32>>);
// DEFAULT-NEXT:         write<ptr<i32>, atomic=seq_cst>(%14, null<ptr<i32>>);
// DEFAULT-NEXT:         write<ptr<void>>(%47, pointer_cast<ptr<void>, reason=assign>(read<ptr<atomic @type1>>(%20)));
// DEFAULT-NEXT:         write<ptr<atomic @type1>>(%20, pointer_cast<ptr<atomic @type1>, reason=assign>(read<ptr<void>>(%47)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %52 @fc0a(%75 <unnamed>: i32 [const]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %53 @fc0b(%77 <unnamed>: atomic i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %54 @fc1a(%55 x: volatile i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %56 @fc1b(%57 x: volatile atomic i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %58 @fc2a(%59 x: i32 [const]) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %60 @fc2b(%61 x: atomic i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %62 @fc3a(%84 x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %63 x: volatile i16 [storage=automatic] = truncate<i16, reason=arg, fits=unknown>(read<i32>(%84));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %64 @fc3b(%86 x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %65 x: atomic i16 [storage=automatic] = truncate<i16, reason=arg, fits=unknown>(read<i32>(%86));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %66 @fc4a(%87 x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %67 x: i16 [storage=automatic] [const] = truncate<i16, reason=arg, fits=unknown>(read<i32>(%87));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %68 @fc4b(%89 x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %69 x: atomic i16 [storage=automatic] = truncate<i16, reason=arg, fits=unknown>(read<i32>(%89));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %70 @func10(%71 p: ptr<atomic i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32, atomic=seq_cst>(deref(ptr_offset<ptr<atomic i32>, subtract=false, element=i32, overflow=ub>(read<ptr<atomic i32>>(%71), div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(0), const<i32>(0)))), const<i32>(1));
// DEFAULT-NEXT:         let %112: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(deref(ptr_offset<ptr<atomic i32>, subtract=false, element=i32, overflow=ub>(read<ptr<atomic i32>>(%71), div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(0), const<i32>(0)))), add<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         write<i32, atomic=seq_cst>(deref(read<ptr<atomic i32>>(%71)), div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:         let %113: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(deref(read<ptr<atomic i32>>(%71)), add<i32, overflow=ub>(old<i32>, div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(0), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
