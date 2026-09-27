/* Test for designated initializers for anonymous structures and
   unions.  PR 10676.  */
/* { dg-do run } */
/* { dg-options "" } */

extern void abort (void);
extern void exit (int);

struct s
{
  int a;
  struct
  {
    int b;
    int c;
  };
  union
  {
    int d;
    struct
    {
      int e;
    };
  };
  struct
  {
    struct
    {
      struct
      {
	int f;
      };
    };
  };
};

struct s x =
  {
    .e = 5,
    .b = 4,
    .a = 3,
    .f = 7,
    .c = 9
  };

int
main (void)
{
  if (x.a != 3
      || x.b != 4
      || x.c != 9
      || x.d != 5
      || x.e != 5
      || x.f != 7)
    abort ();
  exit (0);
}

// SLATE-FILECHECK-FLAVOR gcc
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
// DEFAULT-NEXT:     type @type0 s = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 <anonymous>: @type1;
// DEFAULT-NEXT:         field2 <anonymous>: @type2;
// DEFAULT-NEXT:         field3 <anonymous>: @type4;
// DEFAULT-NEXT:     } [size=20, align=4, offsets=[0, 4, 12, 16]];
// DEFAULT-NEXT:     type @type1 = struct {
// DEFAULT-NEXT:         field0 b: i32;
// DEFAULT-NEXT:         field1 c: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type2 = union {
// DEFAULT-NEXT:         field0 d: i32;
// DEFAULT-NEXT:         field1 <anonymous>: @type3;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type3 = struct {
// DEFAULT-NEXT:         field0 e: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type4 = struct {
// DEFAULT-NEXT:         field0 <anonymous>: @type5;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type5 = struct {
// DEFAULT-NEXT:         field0 <anonymous>: @type6;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type6 = struct {
// DEFAULT-NEXT:         field0 f: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     global %9 x: @type0 [storage=static] = aggregate<@type0, zero_fill=false>(field0 = const<i32>(3), field1 = aggregate<@type1, zero_fill=false>(field0 = const<i32>(4), field1 = const<i32>(9)), field2 = aggregate<@type2, zero_fill=false>(field1 = aggregate<@type3, zero_fill=false>(field0 = const<i32>(5))), field3 = aggregate<@type4, zero_fill=false>(field0 = aggregate<@type5, zero_fill=false>(field0 = aggregate<@type6, zero_fill=false>(field0 = const<i32>(7))))) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%11 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(field0(%9)), const<i32>(3)), ne<i32>(read<i32>(field0(field1(%9))), const<i32>(4))), ne<i32>(read<i32>(field1(field1(%9))), const<i32>(9))), ne<i32>(read<i32>(field0(field2(%9))), const<i32>(5))), ne<i32>(read<i32>(field0(field1(field2(%9)))), const<i32>(5))), ne<i32>(read<i32>(field0(field0(field0(field3(%9))))), const<i32>(7)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
