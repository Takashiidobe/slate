/* Test initialization of anonymous structures and unions (clarified in C2y by
   N3451).  */
/* { dg-do run } */
/* { dg-options "-std=c2y -pedantic-errors" } */

struct s { struct { int a, b; }; union { int c; }; } x = { 1, 2, 3 };
union u { struct { int x, y; }; };

struct s a = { 1, 2, 3 };
union u b = { 4, 5 };

extern void abort ();
extern void exit (int);

int
main ()
{
  if (a.a != 1 || a.b != 2 || a.c != 3)
    abort ();
  if (b.x != 4 || b.y != 5)
    abort ();
  exit (0);
}

// SLATE-FILECHECK-STD DEFAULT c2y
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
// DEFAULT-NEXT:         field0 <anonymous>: @type[[TYPE0:[0-9]+]];
// DEFAULT-NEXT:         field1 <anonymous>: @type[[TYPE1:[0-9]+]];
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE0]] = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE1]] = union {
// DEFAULT-NEXT:         field0 c: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_u:[0-9]+]] u = union {
// DEFAULT-NEXT:         field0 <anonymous>: @type[[TYPE2:[0-9]+]];
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE2]] = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 y: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     global %[[VALUE_x:[0-9]+]] x: @type[[TYPE_s]] [storage=static] = aggregate<@type[[TYPE_s]], zero_fill=false>(field0 = aggregate<@type[[TYPE0]], zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(2)), field1 = aggregate<@type[[TYPE1]], zero_fill=false>(field0 = const<i32>(3))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: @type[[TYPE_s]] [storage=static] = aggregate<@type[[TYPE_s]], zero_fill=false>(field0 = aggregate<@type[[TYPE0]], zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(2)), field1 = aggregate<@type[[TYPE1]], zero_fill=false>(field0 = const<i32>(3))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: @type[[TYPE_u]] [storage=static] = aggregate<@type[[TYPE_u]], zero_fill=false>(field0 = aggregate<@type[[TYPE2]], zero_fill=false>(field0 = const<i32>(4), field1 = const<i32>(5))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(field0(field0(%[[VALUE_a]]))), const<i32>(1)), ne<i32>(read<i32>(field1(field0(%[[VALUE_a]]))), const<i32>(2))), ne<i32>(read<i32>(field0(field1(%[[VALUE_a]]))), const<i32>(3)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(field0(field0(%[[VALUE_b]]))), const<i32>(4)), ne<i32>(read<i32>(field1(field0(%[[VALUE_b]]))), const<i32>(5)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
