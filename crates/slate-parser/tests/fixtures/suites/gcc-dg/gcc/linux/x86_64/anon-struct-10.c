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
// DEFAULT-NEXT:         field1 <anonymous>: @type[[TYPE0:[0-9]+]];
// DEFAULT-NEXT:         field2 <anonymous>: @type[[TYPE1:[0-9]+]];
// DEFAULT-NEXT:         field3 <anonymous>: @type[[TYPE3:[0-9]+]];
// DEFAULT-NEXT:     } [size=20, align=4, offsets=[0, 4, 12, 16]];
// DEFAULT-NEXT:     type @type[[TYPE0]] = struct {
// DEFAULT-NEXT:         field0 b: i32;
// DEFAULT-NEXT:         field1 c: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE1]] = union {
// DEFAULT-NEXT:         field0 d: i32;
// DEFAULT-NEXT:         field1 <anonymous>: @type[[TYPE2:[0-9]+]];
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE2]] = struct {
// DEFAULT-NEXT:         field0 e: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE3]] = struct {
// DEFAULT-NEXT:         field0 <anonymous>: @type[[TYPE4:[0-9]+]];
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE4]] = struct {
// DEFAULT-NEXT:         field0 <anonymous>: @type[[TYPE5:[0-9]+]];
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE5]] = struct {
// DEFAULT-NEXT:         field0 f: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_x:[0-9]+]] x: @type[[TYPE_s]] [storage=static] = aggregate<@type[[TYPE_s]], zero_fill=false>(field0 = const<i32>(3), field1 = aggregate<@type[[TYPE0]], zero_fill=false>(field0 = const<i32>(4), field1 = const<i32>(9)), field2 = aggregate<@type[[TYPE1]], zero_fill=false>(field1 = aggregate<@type[[TYPE2]], zero_fill=false>(field0 = const<i32>(5))), field3 = aggregate<@type[[TYPE3]], zero_fill=false>(field0 = aggregate<@type[[TYPE4]], zero_fill=false>(field0 = aggregate<@type[[TYPE5]], zero_fill=false>(field0 = const<i32>(7))))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(field0(%[[VALUE_x]])), const<i32>(3)), ne<i32>(read<i32>(field0(field1(%[[VALUE_x]]))), const<i32>(4))), ne<i32>(read<i32>(field1(field1(%[[VALUE_x]]))), const<i32>(9))), ne<i32>(read<i32>(field0(field2(%[[VALUE_x]]))), const<i32>(5))), ne<i32>(read<i32>(field0(field1(field2(%[[VALUE_x]])))), const<i32>(5))), ne<i32>(read<i32>(field0(field0(field0(field3(%[[VALUE_x]]))))), const<i32>(7)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
