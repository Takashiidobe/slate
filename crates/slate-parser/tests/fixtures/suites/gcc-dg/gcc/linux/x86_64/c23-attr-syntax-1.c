/* Test C23 attribute syntax.  Basic tests of valid uses of empty
   attributes.  */
/* { dg-do compile } */
/* { dg-options "-std=c23 -pedantic-errors" } */

[ [ ] ] [[]];

[[]] int [[]] a [[]] = 123;

int f([[]] int x [[]], [[]] long [[]], short [[]] *[[]] [3] [[]],
      int [[]] (int)[[]], int (*)(int)[[]]) [[]] [[]];

int g [[]] [2] [[]] [3] [[]];

int *[[]] const *[[]] volatile *[[]] *const p;

int *[[]][[]] q = 0;

struct [[]] s;
union [[]][[]] u;

struct [[]] s2 { [[]] long [[]] *a[[]] [3] [[]] [4], b[[]]; };

union [[]] u2 { [[]] long [[]] *a[[]] [3] [[]] [4]; };

int z = sizeof (int [[]]);

enum [[]] { E1 [[]][[]], E2[[]][[]] = 3 };
enum [[]] e { E3 = 4, E4 [[]] };

void
func (void) [[]]
{
  [[]] int var;
  [[]] { }
  [[]] switch (a) { [[]] case 1: [[]] case 2: [[]] default: [[]] var = 3; }
  [[]] x : [[]] y: [[]] var = 1;
  [[]];
  int [[]] var2;
  [[]] if (a) [[]] (void) 0; else [[]] (void) 1;
  [[]] while (0) [[]] var = 2;
  [[]] do [[]] var = 3; while (0);
  for ([[]] int zz = 1; zz < 10; zz++)
    {
      [[]] var2 = 8;
      [[]] continue;
      [[]] break;
    }
  if (a) [[]] goto x;
  [[]] return;
}

void func2 () [[]];

void func3 () [[]] { }

// SLATE-FILECHECK-STD DEFAULT c23
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
// DEFAULT-NEXT:     type @type[[TYPE_s:[0-9]+]] s = struct incomplete;
// DEFAULT-NEXT:     type @type[[TYPE_u:[0-9]+]] u = union incomplete;
// DEFAULT-NEXT:     type @type[[TYPE_s2:[0-9]+]] s2 = struct {
// DEFAULT-NEXT:         field0 a: array<array<ptr<i64>, 4>, 3>;
// DEFAULT-NEXT:         field1 b: i64;
// DEFAULT-NEXT:     } [size=104, align=8, offsets=[0, 96]];
// DEFAULT-NEXT:     type @type[[TYPE_u2:[0-9]+]] u2 = union {
// DEFAULT-NEXT:         field0 a: array<array<ptr<i64>, 4>, 3>;
// DEFAULT-NEXT:     } [size=96, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_E1:[0-9]+]] E1 = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_E2:[0-9]+]] E2 = const<i32>(3);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_e:[0-9]+]] e = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_E1]] E3 = const<i32>(4);
// DEFAULT-NEXT:         %[[VALUE_E2]] E4 = const<i32>(5);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     global %[[VALUE_E1]] a: i32 [storage=static] = const<i32>(123) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g:[0-9]+]] g: array<array<i32, 3>, 2> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_p:[0-9]+]] p: ptr<ptr<volatile ptr<const ptr<i32>>>> [storage=static] [const] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_q:[0-9]+]] q: ptr<i32> [storage=static] = null<ptr<i32>> [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_z:[0-9]+]] z: i32 [storage=static] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(4))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_x:[0-9]+]] x: i32, %[[VALUE0:[0-9]+]] <unnamed>: i64, %[[VALUE1:[0-9]+]] <unnamed>: ptr<ptr<i16>> [array=3], %[[VALUE2:[0-9]+]] <unnamed>: ptr<fn(i32) -> i32>, %[[VALUE3:[0-9]+]] <unnamed>: ptr<fn(i32) -> i32>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_func:[0-9]+]] @func() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_var:[0-9]+]] var: i32 [storage=automatic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         switch %[[VALUE4:[0-9]+]] read<i32>(%[[VALUE_E1]])
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %[[VALUE4]] const<i32>(1):
// DEFAULT-NEXT:                     case %[[VALUE4]] const<i32>(2):
// DEFAULT-NEXT:                         default %[[VALUE4]]:
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_var]], const<i32>(3));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         label %[[VALUE_x_2:[0-9]+]] x:
// DEFAULT-NEXT:             label %[[VALUE_y:[0-9]+]] y:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_var]], const<i32>(1));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         let %[[VALUE_var2:[0-9]+]] var2: i32 [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_E1]]), const<i32>(0))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             const<i32>(1);
// DEFAULT-NEXT:         while %[[VALUE5:[0-9]+]] ne<i32>(const<i32>(0), const<i32>(0))
// DEFAULT-NEXT:             write<i32>(%[[VALUE_var]], const<i32>(2));
// DEFAULT-NEXT:         do %[[VALUE6:[0-9]+]]
// DEFAULT-NEXT:             write<i32>(%[[VALUE_var]], const<i32>(3));
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         for %[[VALUE7:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_zz:[0-9]+]] zz: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_zz]]), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE8:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_zz]]);
// DEFAULT-NEXT:                 let %[[VALUE9:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE8]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_zz]], read<i32>(%[[VALUE9]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_var2]], const<i32>(8));
// DEFAULT-NEXT:                     continue %[[VALUE7]];
// DEFAULT-NEXT:                     break %[[VALUE7]];
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_E1]]), const<i32>(0))
// DEFAULT-NEXT:             goto %[[VALUE_x_2]];
// DEFAULT-NEXT:         return;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_func2:[0-9]+]] @func2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_func3:[0-9]+]] @func3() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
