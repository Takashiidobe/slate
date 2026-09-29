/* Test C23 attribute syntax.  Test GNU attributes appertain to
   appropriate constructs.  */
/* { dg-do compile } */
/* { dg-require-alias "" } */
/* { dg-options "-std=gnu23" } */

void f (void) {};

[[gnu::alias("f")]] void g (void); /* { dg-error "only weak" "" { target *-*-darwin* } } */

void [[gnu::alias("f")]] h (void); /* { dg-warning "does not apply to types" } */

struct [[gnu::packed]] s { int a; char b; };
_Static_assert (sizeof (struct s) == (sizeof (int) + sizeof (char)));

int
f2 (void)
{
  [[gnu::deprecated]] int a = 1;
  return a; /* { dg-warning "deprecated" } */
}

int
f3 (void)
{
  int a [[gnu::deprecated]] = 1;
  return a; /* { dg-warning "deprecated" } */
}

struct s2 { [[gnu::deprecated]] int a; int b [[gnu::deprecated]]; } x;

int
f4 (void)
{
  return x.a; /* { dg-warning "deprecated" } */
}

int
f5 (void)
{
  return x.b; /* { dg-warning "deprecated" } */
}

enum e { E1 [[gnu::deprecated]] };

enum e
f6 (void)
{
  return E1; /* { dg-warning "deprecated" } */
}

int
f7 ([[gnu::deprecated]] int y)
{
  return y; /* { dg-warning "deprecated" } */
}

union [[gnu::deprecated]] u { int x; };

void
f8 (void)
{
  union u var; /* { dg-warning "deprecated" } */
}

enum [[gnu::deprecated]] edep { E2 };

void
f9 (void)
{
  enum edep var; /* { dg-warning "deprecated" } */
}

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
// DEFAULT-NEXT:     type @type[[TYPE_s:[0-9]+]] s = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i8;
// DEFAULT-NEXT:     } [size=5, align=1, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_s2:[0-9]+]] s2 = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_e:[0-9]+]] e = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_E1:[0-9]+]] E1 = const<i32>(0);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_u:[0-9]+]] u = union {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_edep:[0-9]+]] edep = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_E1]] E2 = const<i32>(0);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     global %[[VALUE_x:[0-9]+]] x: @type[[TYPE_s2]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_E1]] @f() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_g:[0-9]+]] @g() -> void [linkage=external] [alias="f"];
// DEFAULT-NEXT:     fn %[[VALUE_h:[0-9]+]] @h() -> void [linkage=external] [alias="f"];
// DEFAULT-NEXT:     fn %[[VALUE_f2:[0-9]+]] @f2() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_a]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f3:[0-9]+]] @f3() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_a_2:[0-9]+]] a: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_a_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f4:[0-9]+]] @f4() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(field0(%[[VALUE_x]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f5:[0-9]+]] @f5() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(field1(%[[VALUE_x]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f6:[0-9]+]] @f6() -> @type[[TYPE_e]] [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return int_to_enum<@type[[TYPE_e]], reason=return>(reinterpret<u32, reason=return, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f7:[0-9]+]] @f7(%[[VALUE_y:[0-9]+]] y: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_y]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f8:[0-9]+]] @f8() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_var:[0-9]+]] var: @type[[TYPE_u]] [storage=automatic];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f9:[0-9]+]] @f9() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_var_2:[0-9]+]] var: @type[[TYPE_edep]] [storage=automatic];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
