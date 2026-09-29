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
// DEFAULT-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_U:[0-9]+]] U = union {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     fn %[[VALUE_fn1:[0-9]+]] @fn1(%[[VALUE_p:[0-9]+]] p: atomic @type[[TYPE_S]]) -> i32 [linkage=external] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_e:[0-9]+]] e: i32 [storage=automatic] = from_bool<i32, reason=assign>(logical_and<bool>(ne<i32>(const<i32>(0), const<i32>(0)), ne<i32>(read<i32, atomic=seq_cst>(field0(%[[VALUE_p]])), const<i32>(0))));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32, atomic=seq_cst>(field0(%[[VALUE_p]])), read<i32>(%[[VALUE_e]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2:[0-9]+]] @fn2(%[[VALUE_p_2:[0-9]+]] p: ptr<atomic @type[[TYPE_S]]>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_e_2:[0-9]+]] e: i32 [storage=automatic] = from_bool<i32, reason=assign>(logical_or<bool>(ne<i32>(const<i32>(1), const<i32>(0)), ne<i32>(read<i32, atomic=seq_cst>(field0(deref(read<ptr<atomic @type[[TYPE_S]]>>(%[[VALUE_p_2]])))), const<i32>(0))));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32, atomic=seq_cst>(field0(deref(read<ptr<atomic @type[[TYPE_S]]>>(%[[VALUE_p_2]])))), read<i32>(%[[VALUE_e_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3:[0-9]+]] @fn3(%[[VALUE_p_3:[0-9]+]] p: atomic @type[[TYPE_S]], %[[VALUE_x:[0-9]+]] x: i32) -> void [linkage=external] [abi=sysv64(native_c, scalar) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32, atomic=seq_cst>(field0(%[[VALUE_p_3]]), read<i32>(%[[VALUE_x]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn4:[0-9]+]] @fn4(%[[VALUE_p_4:[0-9]+]] p: ptr<atomic @type[[TYPE_S]]>, %[[VALUE_x_2:[0-9]+]] x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32, atomic=seq_cst>(field0(deref(read<ptr<atomic @type[[TYPE_S]]>>(%[[VALUE_p_4]]))), read<i32>(%[[VALUE_x_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn5:[0-9]+]] @fn5(%[[VALUE_p_5:[0-9]+]] p: atomic @type[[TYPE_S]]) -> i32 [linkage=external] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_s:[0-9]+]] s: @type[[TYPE_S]] [storage=automatic] = copy<@type[[TYPE_S]], reason=assign>(read<@type[[TYPE_S]], atomic=seq_cst>(%[[VALUE_p_5]]));
// DEFAULT-NEXT:         return read<i32>(field0(%[[VALUE_s]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn6:[0-9]+]] @fn6(%[[VALUE_p_6:[0-9]+]] p: ptr<atomic @type[[TYPE_S]]>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_s_2:[0-9]+]] s: @type[[TYPE_S]] [storage=automatic] = copy<@type[[TYPE_S]], reason=assign>(read<@type[[TYPE_S]], atomic=seq_cst>(deref(read<ptr<atomic @type[[TYPE_S]]>>(%[[VALUE_p_6]]))));
// DEFAULT-NEXT:         return read<i32>(field0(%[[VALUE_s_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn7:[0-9]+]] @fn7(%[[VALUE_p_7:[0-9]+]] p: atomic @type[[TYPE_U]]) -> i32 [linkage=external] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_e_3:[0-9]+]] e: i32 [storage=automatic] = from_bool<i32, reason=assign>(logical_and<bool>(ne<i32>(const<i32>(0), const<i32>(0)), ne<i32>(read<i32, atomic=seq_cst>(field0(%[[VALUE_p_7]])), const<i32>(0))));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32, atomic=seq_cst>(field0(%[[VALUE_p_7]])), read<i32>(%[[VALUE_e_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn8:[0-9]+]] @fn8(%[[VALUE_p_8:[0-9]+]] p: ptr<atomic @type[[TYPE_U]]>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_e_4:[0-9]+]] e: i32 [storage=automatic] = from_bool<i32, reason=assign>(logical_or<bool>(ne<i32>(const<i32>(1), const<i32>(0)), ne<i32>(read<i32, atomic=seq_cst>(field0(deref(read<ptr<atomic @type[[TYPE_U]]>>(%[[VALUE_p_8]])))), const<i32>(0))));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32, atomic=seq_cst>(field0(deref(read<ptr<atomic @type[[TYPE_U]]>>(%[[VALUE_p_8]])))), read<i32>(%[[VALUE_e_4]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn9:[0-9]+]] @fn9(%[[VALUE_p_9:[0-9]+]] p: atomic @type[[TYPE_U]], %[[VALUE_x_3:[0-9]+]] x: i32) -> void [linkage=external] [abi=sysv64(native_c, scalar) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32, atomic=seq_cst>(field0(%[[VALUE_p_9]]), read<i32>(%[[VALUE_x_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn10:[0-9]+]] @fn10(%[[VALUE_p_10:[0-9]+]] p: ptr<atomic @type[[TYPE_U]]>, %[[VALUE_x_4:[0-9]+]] x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32, atomic=seq_cst>(field0(deref(read<ptr<atomic @type[[TYPE_U]]>>(%[[VALUE_p_10]]))), read<i32>(%[[VALUE_x_4]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn11:[0-9]+]] @fn11(%[[VALUE_p_11:[0-9]+]] p: atomic @type[[TYPE_U]]) -> i32 [linkage=external] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_s_3:[0-9]+]] s: @type[[TYPE_U]] [storage=automatic] = copy<@type[[TYPE_U]], reason=assign>(read<@type[[TYPE_U]], atomic=seq_cst>(%[[VALUE_p_11]]));
// DEFAULT-NEXT:         return read<i32>(field0(%[[VALUE_s_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn12:[0-9]+]] @fn12(%[[VALUE_p_12:[0-9]+]] p: ptr<atomic @type[[TYPE_U]]>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_s_4:[0-9]+]] s: @type[[TYPE_U]] [storage=automatic] = copy<@type[[TYPE_U]], reason=assign>(read<@type[[TYPE_U]], atomic=seq_cst>(deref(read<ptr<atomic @type[[TYPE_U]]>>(%[[VALUE_p_12]]))));
// DEFAULT-NEXT:         return read<i32>(field0(%[[VALUE_s_4]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
