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
// DEFAULT-NEXT:     type @type[[TYPE_s:[0-9]+]] s = struct {
// DEFAULT-NEXT:         field0 c: array<i8, 1000>;
// DEFAULT-NEXT:     } [size=1000, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_t:[0-9]+]] t = struct {
// DEFAULT-NEXT:         field0 i: atomic i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_u:[0-9]+]] u = union {
// DEFAULT-NEXT:         field0 c: array<i8, 1000>;
// DEFAULT-NEXT:     } [size=1000, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_v:[0-9]+]] v = union {
// DEFAULT-NEXT:         field0 i: atomic i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     extern %[[VALUE_a:[0-9]+]] a: atomic i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_b:[0-9]+]] b: atomic ptr<i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ai1:[0-9]+]] ai1: atomic i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ai2:[0-9]+]] ai2: atomic i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_i1:[0-9]+]] i1: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ald1:[0-9]+]] ald1: volatile atomic f80 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ald2:[0-9]+]] ald2: atomic f80 [storage=static] [const] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ld1:[0-9]+]] ld1: f80 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_acd1:[0-9]+]] acd1: atomic complex<f64> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_acd2:[0-9]+]] acd2: atomic complex<f64> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d1:[0-9]+]] d1: complex<f64> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ab1:[0-9]+]] ab1: volatile atomic bool [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_p:[0-9]+]] p: ptr<i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ap:[0-9]+]] ap: atomic ptr<i32> [storage=static] [restrict] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_as1:[0-9]+]] as1: atomic @type[[TYPE_s]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_s1:[0-9]+]] s1: @type[[TYPE_s]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_at1:[0-9]+]] at1: atomic @type[[TYPE_t]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_atp1:[0-9]+]] atp1: ptr<atomic @type[[TYPE_t]]> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_t1:[0-9]+]] t1: @type[[TYPE_t]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_au1:[0-9]+]] au1: atomic @type[[TYPE_u]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_u1:[0-9]+]] u1: @type[[TYPE_u]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_av1:[0-9]+]] av1: atomic @type[[TYPE_v]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_v1:[0-9]+]] v1: @type[[TYPE_v]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_avp:[0-9]+]] avp: ptr<atomic void> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_aa:[0-9]+]] aa: atomic array<i32, 10> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_aiclp:[0-9]+]] aiclp: ptr<atomic i32> [storage=static] = addr_of<ptr<atomic i32>>(compound_literal %[[VALUE0:[0-9]+]] [storage=static] = const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_i2:[0-9]+]] i2: i32 [storage=static] = float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_xaip1:[0-9]+]] xaip1: ptr<atomic i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_xaip2:[0-9]+]] xaip2: ptr<volatile atomic i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_xvp1:[0-9]+]] xvp1: ptr<void> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE1:[0-9]+]] <unnamed>: atomic ptr<i32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_func:[0-9]+]] @func(%[[VALUE_al1:[0-9]+]] al1: volatile atomic i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32, atomic=seq_cst>(%[[VALUE_ai1]], read<i32, atomic=seq_cst>(%[[VALUE_ai2]]));
// DEFAULT-NEXT:         write<i32, atomic=seq_cst>(%[[VALUE_ai1]], read<i32>(%[[VALUE_i1]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i1]], read<i32, atomic=seq_cst>(%[[VALUE_ai2]]));
// DEFAULT-NEXT:         write<i32, atomic=seq_cst>(%[[VALUE_ai1]], float_to_int<i32, reason=assign, out_of_range=ub, exceptions=observable>(read<f80, atomic=seq_cst>(%[[VALUE_ald2]])));
// DEFAULT-NEXT:         write<f80, volatile, atomic=seq_cst>(%[[VALUE_ald1]], float_widen<f80, reason=assign>(complex_to_real<f64, reason=assign>(read<complex<f64>>(%[[VALUE_d1]]))));
// DEFAULT-NEXT:         write<f80>(%[[VALUE_ld1]], float_widen<f80, reason=assign>(complex_to_real<f64, reason=assign>(read<complex<f64>, atomic=seq_cst>(%[[VALUE_acd2]]))));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: complex<f64> [synthetic] = update<complex<f64>, result=new, atomic=seq_cst>(%[[VALUE_acd1]], add<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(old<complex<f64>>, int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(from_bool<i32, reason=promotion>(read<bool, volatile, atomic=seq_cst>(%[[VALUE_ab1]])))));
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: complex<f64> [synthetic] = update<complex<f64>, result=new, atomic=seq_cst>(%[[VALUE_acd2]], div<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(old<complex<f64>>, int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(read<i32, atomic=seq_cst>(%[[VALUE_ai1]]))));
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_p]], read<ptr<i32>, atomic=seq_cst>(%[[VALUE_ap]]));
// DEFAULT-NEXT:         write<ptr<i32>, atomic=seq_cst>(%[[VALUE_ap]], read<ptr<i32>>(%[[VALUE_p]]));
// DEFAULT-NEXT:         write<bool, volatile, atomic=seq_cst>(%[[VALUE_ab1]], ne<ptr<i32>, reason=assign>(read<ptr<i32>>(%[[VALUE_p]]), null<ptr<i32>>));
// DEFAULT-NEXT:         write<@type[[TYPE_s]], atomic=seq_cst>(%[[VALUE_as1]], copy<@type[[TYPE_s]], reason=assign>(read<@type[[TYPE_s]]>(%[[VALUE_s1]])));
// DEFAULT-NEXT:         write<@type[[TYPE_s]]>(%[[VALUE_s1]], copy<@type[[TYPE_s]], reason=assign>(read<@type[[TYPE_s]], atomic=seq_cst>(%[[VALUE_as1]])));
// DEFAULT-NEXT:         write<@type[[TYPE_t]], atomic=seq_cst>(%[[VALUE_at1]], copy<@type[[TYPE_t]], reason=assign>(read<@type[[TYPE_t]]>(%[[VALUE_t1]])));
// DEFAULT-NEXT:         write<@type[[TYPE_t]]>(%[[VALUE_t1]], copy<@type[[TYPE_t]], reason=assign>(read<@type[[TYPE_t]], atomic=seq_cst>(%[[VALUE_at1]])));
// DEFAULT-NEXT:         write<i32, atomic=seq_cst>(field0(%[[VALUE_t1]]), read<i32, atomic=seq_cst>(field0(%[[VALUE_at1]])));
// DEFAULT-NEXT:         write<i32, atomic=seq_cst>(field0(%[[VALUE_at1]]), read<i32, atomic=seq_cst>(field0(%[[VALUE_t1]])));
// DEFAULT-NEXT:         write<i32, atomic=seq_cst>(field0(deref(read<ptr<atomic @type[[TYPE_t]]>>(%[[VALUE_atp1]]))), read<i32, atomic=seq_cst>(field0(%[[VALUE_t1]])));
// DEFAULT-NEXT:         write<@type[[TYPE_u]], atomic=seq_cst>(%[[VALUE_au1]], copy<@type[[TYPE_u]], reason=assign>(read<@type[[TYPE_u]]>(%[[VALUE_u1]])));
// DEFAULT-NEXT:         write<@type[[TYPE_u]]>(%[[VALUE_u1]], copy<@type[[TYPE_u]], reason=assign>(read<@type[[TYPE_u]], atomic=seq_cst>(%[[VALUE_au1]])));
// DEFAULT-NEXT:         write<@type[[TYPE_v]], atomic=seq_cst>(%[[VALUE_av1]], copy<@type[[TYPE_v]], reason=assign>(read<@type[[TYPE_v]]>(%[[VALUE_v1]])));
// DEFAULT-NEXT:         write<@type[[TYPE_v]]>(%[[VALUE_v1]], copy<@type[[TYPE_v]], reason=assign>(read<@type[[TYPE_v]], atomic=seq_cst>(%[[VALUE_av1]])));
// DEFAULT-NEXT:         write<i32, atomic=seq_cst>(field0(%[[VALUE_v1]]), read<i32, atomic=seq_cst>(field0(%[[VALUE_av1]])));
// DEFAULT-NEXT:         write<i32, atomic=seq_cst>(field0(%[[VALUE_av1]]), read<i32, atomic=seq_cst>(field0(%[[VALUE_v1]])));
// DEFAULT-NEXT:         let %[[VALUE_ra1:[0-9]+]] ra1: volatile atomic i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         let %[[VALUE_ra2:[0-9]+]] ra2: volatile atomic i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:         write<i32, volatile, atomic=seq_cst>(%[[VALUE_ra1]], read<i32, volatile, atomic=seq_cst>(%[[VALUE_ra2]]));
// DEFAULT-NEXT:         write<i32, volatile, atomic=seq_cst>(%[[VALUE_ra2]], read<i32, volatile, atomic=seq_cst>(%[[VALUE_ra1]]));
// DEFAULT-NEXT:         write<i64, volatile, atomic=seq_cst>(%[[VALUE_al1]], widen<i64, reason=assign>(read<i32, volatile, atomic=seq_cst>(%[[VALUE_ra1]])));
// DEFAULT-NEXT:         write<i32, volatile, atomic=seq_cst>(%[[VALUE_ra2]], truncate<i32, reason=assign, fits=unknown>(read<i64, volatile, atomic=seq_cst>(%[[VALUE_al1]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_func2:[0-9]+]] @func2(%[[VALUE_i:[0-9]+]] i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_func3:[0-9]+]] @func3(%[[VALUE_i_2:[0-9]+]] i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(i32) -> i32>(%[[VALUE_func2]], truncate<i32, reason=arg, fits=unknown>(widen<i64, reason=explicit>(read<i32>(%[[VALUE_i_2]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_func4:[0-9]+]] @func4() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32, atomic=seq_cst>(deref(ptr_offset<ptr<atomic i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<atomic i32>, length=Some(10)>(%[[VALUE_aa]]), const<i32>(2))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_func5:[0-9]+]] @func5() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: f80 [synthetic] = update<f80, result=old, volatile, atomic=seq_cst>(%[[VALUE_ald1]], add<f80, rounding=nearest_even, exceptions=observable, contract=off>(old<f80>, int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: f80 [synthetic] = update<f80, result=old, volatile, atomic=seq_cst>(%[[VALUE_ald1]], sub<f80, rounding=nearest_even, exceptions=observable, contract=off>(old<f80>, int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: f80 [synthetic] = update<f80, result=new, volatile, atomic=seq_cst>(%[[VALUE_ald1]], add<f80, rounding=nearest_even, exceptions=observable, contract=off>(old<f80>, int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: f80 [synthetic] = update<f80, result=new, volatile, atomic=seq_cst>(%[[VALUE_ald1]], sub<f80, rounding=nearest_even, exceptions=observable, contract=off>(old<f80>, int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(%[[VALUE_ai1]], add<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(%[[VALUE_ai1]], sub<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(%[[VALUE_ai1]], add<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(%[[VALUE_ai1]], sub<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: bool [synthetic] = update<bool, result=old, volatile, atomic=seq_cst>(%[[VALUE_ab1]], ne<i32, reason=assign>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(old<bool>), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: bool [synthetic] = update<bool, result=old, volatile, atomic=seq_cst>(%[[VALUE_ab1]], ne<i32, reason=assign>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(old<bool>), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE14:[0-9]+]]: bool [synthetic] = update<bool, result=new, volatile, atomic=seq_cst>(%[[VALUE_ab1]], ne<i32, reason=assign>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(old<bool>), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE15:[0-9]+]]: bool [synthetic] = update<bool, result=new, volatile, atomic=seq_cst>(%[[VALUE_ab1]], ne<i32, reason=assign>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(old<bool>), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE16:[0-9]+]]: ptr<i32> [synthetic] = update<ptr<i32>, result=old, atomic=seq_cst>(%[[VALUE_ap]], ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(old<ptr<i32>>, const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE17:[0-9]+]]: ptr<i32> [synthetic] = update<ptr<i32>, result=old, atomic=seq_cst>(%[[VALUE_ap]], ptr_offset<ptr<i32>, subtract=true, element=i32, overflow=ub>(old<ptr<i32>>, const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE18:[0-9]+]]: ptr<i32> [synthetic] = update<ptr<i32>, result=new, atomic=seq_cst>(%[[VALUE_ap]], ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(old<ptr<i32>>, const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE19:[0-9]+]]: ptr<i32> [synthetic] = update<ptr<i32>, result=new, atomic=seq_cst>(%[[VALUE_ap]], ptr_offset<ptr<i32>, subtract=true, element=i32, overflow=ub>(old<ptr<i32>>, const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_func6:[0-9]+]] @func6() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i_3:[0-9]+]] i: i32 [storage=automatic] = read<i32, atomic=seq_cst>(deref(read<ptr<atomic i32>>(%[[VALUE_aiclp]])));
// DEFAULT-NEXT:         let %[[VALUE_p_2:[0-9]+]] p: ptr<atomic i32> [storage=automatic] = addr_of<ptr<atomic i32>>(%[[VALUE_ai2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_func7:[0-9]+]] @func7() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r]], truncate<i32, reason=assign, fits=unknown>(ptr_diff<i64, element=i32, same_array=required, overflow=ub>(read<ptr<atomic i32>>(%[[VALUE_xaip1]]), read<ptr<volatile atomic i32>>(%[[VALUE_xaip2]]))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r]], from_bool<i32, reason=assign>(lt<ptr<atomic i32>>(read<ptr<atomic i32>>(%[[VALUE_xaip1]]), pointer_cast<ptr<atomic i32>, reason=usual_arith>(read<ptr<volatile atomic i32>>(%[[VALUE_xaip2]])))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r]], from_bool<i32, reason=assign>(gt<ptr<atomic i32>>(read<ptr<atomic i32>>(%[[VALUE_xaip1]]), pointer_cast<ptr<atomic i32>, reason=usual_arith>(read<ptr<volatile atomic i32>>(%[[VALUE_xaip2]])))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r]], from_bool<i32, reason=assign>(le<ptr<atomic i32>>(read<ptr<atomic i32>>(%[[VALUE_xaip1]]), pointer_cast<ptr<atomic i32>, reason=usual_arith>(read<ptr<volatile atomic i32>>(%[[VALUE_xaip2]])))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r]], from_bool<i32, reason=assign>(ge<ptr<atomic i32>>(read<ptr<atomic i32>>(%[[VALUE_xaip1]]), pointer_cast<ptr<atomic i32>, reason=usual_arith>(read<ptr<volatile atomic i32>>(%[[VALUE_xaip2]])))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r]], from_bool<i32, reason=assign>(eq<ptr<atomic i32>>(read<ptr<atomic i32>>(%[[VALUE_xaip1]]), pointer_cast<ptr<atomic i32>, reason=usual_arith>(read<ptr<volatile atomic i32>>(%[[VALUE_xaip2]])))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r]], from_bool<i32, reason=assign>(ne<ptr<atomic i32>>(read<ptr<atomic i32>>(%[[VALUE_xaip1]]), pointer_cast<ptr<atomic i32>, reason=usual_arith>(read<ptr<volatile atomic i32>>(%[[VALUE_xaip2]])))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r]], from_bool<i32, reason=assign>(eq<ptr<atomic i32>>(read<ptr<atomic i32>>(%[[VALUE_xaip1]]), pointer_cast<ptr<atomic i32>, reason=usual_arith>(read<ptr<void>>(%[[VALUE_xvp1]])))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r]], from_bool<i32, reason=assign>(ne<ptr<atomic i32>>(read<ptr<atomic i32>>(%[[VALUE_xaip1]]), pointer_cast<ptr<atomic i32>, reason=usual_arith>(read<ptr<void>>(%[[VALUE_xvp1]])))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r]], from_bool<i32, reason=assign>(eq<ptr<void>>(read<ptr<void>>(%[[VALUE_xvp1]]), pointer_cast<ptr<void>, reason=usual_arith>(read<ptr<atomic i32>>(%[[VALUE_xaip1]])))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r]], from_bool<i32, reason=assign>(ne<ptr<void>>(read<ptr<void>>(%[[VALUE_xvp1]]), pointer_cast<ptr<void>, reason=usual_arith>(read<ptr<atomic i32>>(%[[VALUE_xaip1]])))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r]], from_bool<i32, reason=assign>(eq<ptr<atomic i32>>(read<ptr<atomic i32>>(%[[VALUE_xaip1]]), null<ptr<atomic i32>>)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r]], from_bool<i32, reason=assign>(eq<ptr<void>>(null<ptr<void>>, pointer_cast<ptr<void>, reason=usual_arith>(read<ptr<volatile atomic i32>>(%[[VALUE_xaip2]])))));
// DEFAULT-NEXT:         conditional<ptr<volatile atomic i32>>(ne<i32>(read<i32>(%[[VALUE_r]]), const<i32>(0)), pointer_cast<ptr<volatile atomic i32>, reason=usual_arith>(read<ptr<atomic i32>>(%[[VALUE_xaip1]])), read<ptr<volatile atomic i32>>(%[[VALUE_xaip2]]));
// DEFAULT-NEXT:         conditional<ptr<volatile void>>(ne<i32>(read<i32>(%[[VALUE_r]]), const<i32>(0)), pointer_cast<ptr<volatile void>, reason=usual_arith>(read<ptr<void>>(%[[VALUE_xvp1]])), pointer_cast<ptr<volatile void>, reason=usual_arith>(read<ptr<volatile atomic i32>>(%[[VALUE_xaip2]])));
// DEFAULT-NEXT:         conditional<ptr<volatile void>>(ne<i32>(read<i32>(%[[VALUE_r]]), const<i32>(0)), pointer_cast<ptr<volatile void>, reason=usual_arith>(read<ptr<volatile atomic i32>>(%[[VALUE_xaip2]])), pointer_cast<ptr<volatile void>, reason=usual_arith>(read<ptr<void>>(%[[VALUE_xvp1]])));
// DEFAULT-NEXT:         conditional<ptr<atomic i32>>(ne<i32>(read<i32>(%[[VALUE_r]]), const<i32>(0)), read<ptr<atomic i32>>(%[[VALUE_xaip1]]), null<ptr<atomic i32>>);
// DEFAULT-NEXT:         conditional<ptr<atomic i32>>(ne<i32>(read<i32>(%[[VALUE_r]]), const<i32>(0)), null<ptr<atomic i32>>, read<ptr<atomic i32>>(%[[VALUE_xaip1]]));
// DEFAULT-NEXT:         conditional<ptr<void>>(ne<i32>(read<i32>(%[[VALUE_r]]), const<i32>(0)), pointer_cast<ptr<void>, reason=usual_arith>(read<ptr<atomic i32>>(%[[VALUE_xaip1]])), conditional<ptr<void>>(ne<i32>(read<i32>(%[[VALUE_r]]), const<i32>(0)), pointer_cast<ptr<void>, reason=usual_arith>(read<ptr<atomic i32>>(%[[VALUE_xaip1]])), read<ptr<void>>(%[[VALUE_xvp1]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_func8:[0-9]+]] @func8() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE20:[0-9]+]]: ptr<i32> [synthetic] = update<ptr<i32>, result=new, atomic=seq_cst>(%[[VALUE_b]], ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(old<ptr<i32>>, const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE21:[0-9]+]]: ptr<i32> [synthetic] = update<ptr<i32>, result=new, atomic=seq_cst>(%[[VALUE_b]], ptr_offset<ptr<i32>, subtract=true, element=i32, overflow=ub>(old<ptr<i32>>, const<u64>(2)));
// DEFAULT-NEXT:         let %[[VALUE22:[0-9]+]]: ptr<i32> [synthetic] = update<ptr<i32>, result=new, atomic=seq_cst>(%[[VALUE_ap]], ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(old<ptr<i32>>, const<i32>(3)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_func9:[0-9]+]] @func9() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<ptr<i32>, atomic=seq_cst>(%[[VALUE_ap]], null<ptr<i32>>);
// DEFAULT-NEXT:         write<ptr<i32>, atomic=seq_cst>(%[[VALUE_ap]], null<ptr<i32>>);
// DEFAULT-NEXT:         write<ptr<void>>(%[[VALUE_xvp1]], pointer_cast<ptr<void>, reason=assign>(read<ptr<atomic @type[[TYPE_t]]>>(%[[VALUE_atp1]])));
// DEFAULT-NEXT:         write<ptr<atomic @type[[TYPE_t]]>>(%[[VALUE_atp1]], pointer_cast<ptr<atomic @type[[TYPE_t]]>, reason=assign>(read<ptr<void>>(%[[VALUE_xvp1]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fc0a:[0-9]+]] @fc0a(%[[VALUE23:[0-9]+]] <unnamed>: i32 [const]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fc0b:[0-9]+]] @fc0b(%[[VALUE24:[0-9]+]] <unnamed>: atomic i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fc1a:[0-9]+]] @fc1a(%[[VALUE_x:[0-9]+]] x: volatile i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fc1b:[0-9]+]] @fc1b(%[[VALUE_x_2:[0-9]+]] x: volatile atomic i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fc2a:[0-9]+]] @fc2a(%[[VALUE_x_3:[0-9]+]] x: i32 [const]) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fc2b:[0-9]+]] @fc2b(%[[VALUE_x_4:[0-9]+]] x: atomic i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fc3a:[0-9]+]] @fc3a(%[[VALUE_x_5:[0-9]+]] x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_x_6:[0-9]+]] x: volatile i16 [storage=automatic] = truncate<i16, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_5]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fc3b:[0-9]+]] @fc3b(%[[VALUE_x_7:[0-9]+]] x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_x_8:[0-9]+]] x: atomic i16 [storage=automatic] = truncate<i16, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_7]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fc4a:[0-9]+]] @fc4a(%[[VALUE_x_9:[0-9]+]] x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_x_10:[0-9]+]] x: i16 [storage=automatic] [const] = truncate<i16, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_9]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fc4b:[0-9]+]] @fc4b(%[[VALUE_x_11:[0-9]+]] x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_x_12:[0-9]+]] x: atomic i16 [storage=automatic] = truncate<i16, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_11]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_func10:[0-9]+]] @func10(%[[VALUE_p_3:[0-9]+]] p: ptr<atomic i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32, atomic=seq_cst>(deref(ptr_offset<ptr<atomic i32>, subtract=false, element=i32, overflow=ub>(read<ptr<atomic i32>>(%[[VALUE_p_3]]), div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(0), const<i32>(0)))), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE25:[0-9]+]]: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(deref(ptr_offset<ptr<atomic i32>, subtract=false, element=i32, overflow=ub>(read<ptr<atomic i32>>(%[[VALUE_p_3]]), div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(0), const<i32>(0)))), add<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         write<i32, atomic=seq_cst>(deref(read<ptr<atomic i32>>(%[[VALUE_p_3]])), div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE26:[0-9]+]]: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(deref(read<ptr<atomic i32>>(%[[VALUE_p_3]])), add<i32, overflow=ub>(old<i32>, div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(0), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
