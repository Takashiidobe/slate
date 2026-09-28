/* Test for bitfield alignment in structs and unions.  */
/* { dg-do run { target pcc_bitfield_type_matters } }  */
/* { dg-options "-O2" }  */

extern void abort (void);
extern void exit (int);

typedef long la __attribute__((aligned (8)));

struct A
{
  char a;
  union UA
  {
    char x;
    la y : 6;
  } b;
  char c;
} a;

struct B
{
  char a;
  union UB
  {
    char x;
    long y : 6 __attribute__((aligned (8)));
  } b;
  char c;
} b;

struct C
{
  char a;
  struct UC
  {
    la y : 6;
  } b;
  char c;
} c;

struct D
{
  char a;
  struct UD
  {
    long y : 6 __attribute__((aligned (8)));
  } b;
  char c;
} d;

int main (void)
{
  if (sizeof (a) != sizeof (b))
    abort ();
  if (sizeof (a) != sizeof (c))
    abort ();
  if (sizeof (a) != sizeof (d))
    abort ();
  if ((&a.c - &a.a) != (&b.c - &b.a))
    abort ();
  if ((&a.c - &a.a) != (&c.c - &c.a))
    abort ();
  if ((&a.c - &a.a) != (&d.c - &d.a))
    abort ();
  exit (0);
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
// DEFAULT-NEXT:     type @type0 la = i64;
// DEFAULT-NEXT:     type @type1 A = struct {
// DEFAULT-NEXT:         field0 a: i8;
// DEFAULT-NEXT:         field1 b: @type2;
// DEFAULT-NEXT:         field2 c: i8;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     type @type2 UA = union {
// DEFAULT-NEXT:         field0 x: i8;
// DEFAULT-NEXT:         field1 y: i64 : 6;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0], bit_offsets=[None, Some(0)], bit_units=[(0, 1)], field_units=[None, Some(0)]];
// DEFAULT-NEXT:     type @type3 B = struct {
// DEFAULT-NEXT:         field0 a: i8;
// DEFAULT-NEXT:         field1 b: @type4;
// DEFAULT-NEXT:         field2 c: i8;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     type @type4 UB = union {
// DEFAULT-NEXT:         field0 x: i8;
// DEFAULT-NEXT:         field1 y: i64 : 6;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0], bit_offsets=[None, Some(0)], bit_units=[(0, 1)], field_units=[None, Some(0)]];
// DEFAULT-NEXT:     type @type5 C = struct {
// DEFAULT-NEXT:         field0 a: i8;
// DEFAULT-NEXT:         field1 b: @type6;
// DEFAULT-NEXT:         field2 c: i8;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     type @type6 UC = struct {
// DEFAULT-NEXT:         field0 y: i64 : 6;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0], bit_offsets=[Some(0)], bit_units=[(0, 1)], field_units=[Some(0)]];
// DEFAULT-NEXT:     type @type7 D = struct {
// DEFAULT-NEXT:         field0 a: i8;
// DEFAULT-NEXT:         field1 b: @type8;
// DEFAULT-NEXT:         field2 c: i8;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     type @type8 UD = struct {
// DEFAULT-NEXT:         field0 y: i64 : 6;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0], bit_offsets=[Some(0)], bit_units=[(0, 1)], field_units=[Some(0)]];
// DEFAULT-NEXT:     global %5 a: @type1 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %8 b: @type3 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %11 c: @type5 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %14 d: @type7 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%16 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %15 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<u64>(const<u64>(24), const<u64>(24))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u64>(const<u64>(24), const<u64>(24))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u64>(const<u64>(24), const<u64>(24))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i64>(ptr_diff<i64, element=i8, same_array=required, overflow=ub>(addr_of<ptr<i8>>(field2(%5)), addr_of<ptr<i8>>(field0(%5))), ptr_diff<i64, element=i8, same_array=required, overflow=ub>(addr_of<ptr<i8>>(field2(%8)), addr_of<ptr<i8>>(field0(%8))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i64>(ptr_diff<i64, element=i8, same_array=required, overflow=ub>(addr_of<ptr<i8>>(field2(%5)), addr_of<ptr<i8>>(field0(%5))), ptr_diff<i64, element=i8, same_array=required, overflow=ub>(addr_of<ptr<i8>>(field2(%11)), addr_of<ptr<i8>>(field0(%11))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i64>(ptr_diff<i64, element=i8, same_array=required, overflow=ub>(addr_of<ptr<i8>>(field2(%5)), addr_of<ptr<i8>>(field0(%5))), ptr_diff<i64, element=i8, same_array=required, overflow=ub>(addr_of<ptr<i8>>(field2(%14)), addr_of<ptr<i8>>(field0(%14))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
