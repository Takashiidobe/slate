/* PR c/69002 */
/* Test we diagnose accessing elements of atomic structures or unions,
   which is undefined behavior (C11 6.5.2.3#5).  */
/* { dg-do compile } */
/* { dg-options "-std=c11 -pedantic-errors" } */

struct S { int x; };
union U { int x; };

int
fn1 (_Atomic struct S p)
{
  int e = 0 && p.x;
  return p.x + e; /* { dg-warning "accessing a member .x. of an atomic structure" } */
}

int
fn2 (_Atomic struct S *p)
{
  int e = 1 || p->x;
  return p->x + e; /* { dg-warning "accessing a member .x. of an atomic structure" } */
}

void
fn3 (_Atomic struct S p, int x)
{
  p.x = x; /* { dg-warning "accessing a member .x. of an atomic structure" } */
}

void
fn4 (_Atomic struct S *p, int x)
{
  p->x = x; /* { dg-warning "accessing a member .x. of an atomic structure" } */
}

int
fn5 (_Atomic struct S p)
{
  /* This is OK: Members can be safely accessed using a non-atomic
     object which is assigned to or from the atomic object.  */
  struct S s = p;
  return s.x;
}

int
fn6 (_Atomic struct S *p)
{
  struct S s = *p;
  return s.x;
}

int
fn7 (_Atomic union U p)
{
  int e = 0 && p.x;
  return p.x + e; /* { dg-warning "accessing a member .x. of an atomic union" } */
}

int
fn8 (_Atomic union U *p)
{
  int e = 1 || p->x;
  return p->x + e; /* { dg-warning "accessing a member .x. of an atomic union" } */
}

void
fn9 (_Atomic union U p, int x)
{
  p.x = x; /* { dg-warning "accessing a member .x. of an atomic union" } */
}

void
fn10 (_Atomic union U *p, int x)
{
  p->x = x; /* { dg-warning "accessing a member .x. of an atomic union" } */
}

int
fn11 (_Atomic union U p)
{
  /* This is OK: Members can be safely accessed using a non-atomic
     object which is assigned to or from the atomic object.  */
  union U s = p;
  return s.x;
}

int
fn12 (_Atomic union U *p)
{
  union U s = *p;
  return s.x;
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
// DEFAULT-NEXT:     type @type0 S = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type1 U = union {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     fn %2 @fn1(%3 p: atomic @type0) -> i32 [linkage=external] [abi=sysv64(coerce<i32>) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %4 e: i32 [storage=automatic] = from_bool<i32, reason=assign>(logical_and<bool>(ne<i32>(const<i32>(0), const<i32>(0)), ne<i32>(read<i32, atomic=seq_cst>(field0(%3)), const<i32>(0))));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32, atomic=seq_cst>(field0(%3)), read<i32>(%4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @fn2(%6 p: ptr<atomic @type0>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %7 e: i32 [storage=automatic] = from_bool<i32, reason=assign>(logical_or<bool>(ne<i32>(const<i32>(1), const<i32>(0)), ne<i32>(read<i32, atomic=seq_cst>(field0(deref(read<ptr<atomic @type0>>(%6)))), const<i32>(0))));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32, atomic=seq_cst>(field0(deref(read<ptr<atomic @type0>>(%6)))), read<i32>(%7));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @fn3(%9 p: atomic @type0, %10 x: i32) -> void [linkage=external] [abi=sysv64(coerce<i32>, scalar) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32, atomic=seq_cst>(field0(%9), read<i32>(%10));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @fn4(%12 p: ptr<atomic @type0>, %13 x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32, atomic=seq_cst>(field0(deref(read<ptr<atomic @type0>>(%12))), read<i32>(%13));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @fn5(%15 p: atomic @type0) -> i32 [linkage=external] [abi=sysv64(coerce<i32>) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %16 s: @type0 [storage=automatic] = copy<@type0, reason=assign>(read<@type0, atomic=seq_cst>(%15));
// DEFAULT-NEXT:         return read<i32>(field0(%16));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @fn6(%18 p: ptr<atomic @type0>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %19 s: @type0 [storage=automatic] = copy<@type0, reason=assign>(read<@type0, atomic=seq_cst>(deref(read<ptr<atomic @type0>>(%18))));
// DEFAULT-NEXT:         return read<i32>(field0(%19));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @fn7(%21 p: atomic @type1) -> i32 [linkage=external] [abi=sysv64(coerce<i32>) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %22 e: i32 [storage=automatic] = from_bool<i32, reason=assign>(logical_and<bool>(ne<i32>(const<i32>(0), const<i32>(0)), ne<i32>(read<i32, atomic=seq_cst>(field0(%21)), const<i32>(0))));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32, atomic=seq_cst>(field0(%21)), read<i32>(%22));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @fn8(%24 p: ptr<atomic @type1>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %25 e: i32 [storage=automatic] = from_bool<i32, reason=assign>(logical_or<bool>(ne<i32>(const<i32>(1), const<i32>(0)), ne<i32>(read<i32, atomic=seq_cst>(field0(deref(read<ptr<atomic @type1>>(%24)))), const<i32>(0))));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32, atomic=seq_cst>(field0(deref(read<ptr<atomic @type1>>(%24)))), read<i32>(%25));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %26 @fn9(%27 p: atomic @type1, %28 x: i32) -> void [linkage=external] [abi=sysv64(coerce<i32>, scalar) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32, atomic=seq_cst>(field0(%27), read<i32>(%28));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %29 @fn10(%30 p: ptr<atomic @type1>, %31 x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32, atomic=seq_cst>(field0(deref(read<ptr<atomic @type1>>(%30))), read<i32>(%31));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %32 @fn11(%33 p: atomic @type1) -> i32 [linkage=external] [abi=sysv64(coerce<i32>) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %34 s: @type1 [storage=automatic] = copy<@type1, reason=assign>(read<@type1, atomic=seq_cst>(%33));
// DEFAULT-NEXT:         return read<i32>(field0(%34));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %35 @fn12(%36 p: ptr<atomic @type1>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %37 s: @type1 [storage=automatic] = copy<@type1, reason=assign>(read<@type1, atomic=seq_cst>(deref(read<ptr<atomic @type1>>(%36))));
// DEFAULT-NEXT:         return read<i32>(field0(%37));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
