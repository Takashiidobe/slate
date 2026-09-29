/* Test C11 _Alignof returning minimum alignment for a type.  PR
   52023.  */
/* { dg-do run } */
/* { dg-options "-std=c11" } */

extern void abort (void);
extern void exit (int);

#define CHECK_ALIGN(TYPE)			\
  do						\
    {						\
      struct { char c; TYPE v; } x;		\
      if (_Alignof (TYPE) > __alignof__ (x.v))	\
	abort ();				\
    }						\
  while (0)

int
main (void)
{
  CHECK_ALIGN (_Bool);
  CHECK_ALIGN (char);
  CHECK_ALIGN (signed char);
  CHECK_ALIGN (unsigned char);
  CHECK_ALIGN (signed short);
  CHECK_ALIGN (unsigned short);
  CHECK_ALIGN (signed int);
  CHECK_ALIGN (unsigned int);
  CHECK_ALIGN (signed long);
  CHECK_ALIGN (unsigned long);
  CHECK_ALIGN (signed long long);
  CHECK_ALIGN (unsigned long long);
  CHECK_ALIGN (float);
  CHECK_ALIGN (double);
  CHECK_ALIGN (long double);
  CHECK_ALIGN (_Complex float);
  CHECK_ALIGN (_Complex double);
  CHECK_ALIGN (_Complex long double);
  exit (0);
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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 v: bool;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     type @type[[TYPE1:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 v: i8;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     type @type[[TYPE2:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 v: i8;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     type @type[[TYPE3:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 v: u8;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     type @type[[TYPE4:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 v: i16;
// DEFAULT-NEXT:     } [size=4, align=2, offsets=[0, 2]];
// DEFAULT-NEXT:     type @type[[TYPE5:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 v: u16;
// DEFAULT-NEXT:     } [size=4, align=2, offsets=[0, 2]];
// DEFAULT-NEXT:     type @type[[TYPE6:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 v: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE7:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 v: u32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE8:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 v: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE9:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 v: u64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE10:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 v: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE11:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 v: u64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE12:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 v: f32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE13:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 v: f64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE14:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 v: f80;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 16]];
// DEFAULT-NEXT:     type @type[[TYPE15:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 v: complex<f32>;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE16:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 v: complex<f64>;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE17:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 v: complex<f80>;
// DEFAULT-NEXT:     } [size=48, align=16, offsets=[0, 16]];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         do %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_x:[0-9]+]] x: @type[[TYPE0]] [storage=automatic];
// DEFAULT-NEXT:                 if gt<u64>(const<u64>(1), const<u64>(1))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE2:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_x_2:[0-9]+]] x: @type[[TYPE1]] [storage=automatic];
// DEFAULT-NEXT:                 if gt<u64>(const<u64>(1), const<u64>(1))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_x_3:[0-9]+]] x: @type[[TYPE2]] [storage=automatic];
// DEFAULT-NEXT:                 if gt<u64>(const<u64>(1), const<u64>(1))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE4:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_x_4:[0-9]+]] x: @type[[TYPE3]] [storage=automatic];
// DEFAULT-NEXT:                 if gt<u64>(const<u64>(1), const<u64>(1))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE5:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_x_5:[0-9]+]] x: @type[[TYPE4]] [storage=automatic];
// DEFAULT-NEXT:                 if gt<u64>(const<u64>(2), const<u64>(2))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE6:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_x_6:[0-9]+]] x: @type[[TYPE5]] [storage=automatic];
// DEFAULT-NEXT:                 if gt<u64>(const<u64>(2), const<u64>(2))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE7:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_x_7:[0-9]+]] x: @type[[TYPE6]] [storage=automatic];
// DEFAULT-NEXT:                 if gt<u64>(const<u64>(4), const<u64>(4))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE8:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_x_8:[0-9]+]] x: @type[[TYPE7]] [storage=automatic];
// DEFAULT-NEXT:                 if gt<u64>(const<u64>(4), const<u64>(4))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE9:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_x_9:[0-9]+]] x: @type[[TYPE8]] [storage=automatic];
// DEFAULT-NEXT:                 if gt<u64>(const<u64>(8), const<u64>(8))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE10:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_x_10:[0-9]+]] x: @type[[TYPE9]] [storage=automatic];
// DEFAULT-NEXT:                 if gt<u64>(const<u64>(8), const<u64>(8))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE11:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_x_11:[0-9]+]] x: @type[[TYPE10]] [storage=automatic];
// DEFAULT-NEXT:                 if gt<u64>(const<u64>(8), const<u64>(8))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE12:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_x_12:[0-9]+]] x: @type[[TYPE11]] [storage=automatic];
// DEFAULT-NEXT:                 if gt<u64>(const<u64>(8), const<u64>(8))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE13:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_x_13:[0-9]+]] x: @type[[TYPE12]] [storage=automatic];
// DEFAULT-NEXT:                 if gt<u64>(const<u64>(4), const<u64>(4))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE14:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_x_14:[0-9]+]] x: @type[[TYPE13]] [storage=automatic];
// DEFAULT-NEXT:                 if gt<u64>(const<u64>(8), const<u64>(8))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE15:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_x_15:[0-9]+]] x: @type[[TYPE14]] [storage=automatic];
// DEFAULT-NEXT:                 if gt<u64>(const<u64>(16), const<u64>(16))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE16:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_x_16:[0-9]+]] x: @type[[TYPE15]] [storage=automatic];
// DEFAULT-NEXT:                 if gt<u64>(const<u64>(4), const<u64>(4))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE17:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_x_17:[0-9]+]] x: @type[[TYPE16]] [storage=automatic];
// DEFAULT-NEXT:                 if gt<u64>(const<u64>(8), const<u64>(8))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE18:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_x_18:[0-9]+]] x: @type[[TYPE17]] [storage=automatic];
// DEFAULT-NEXT:                 if gt<u64>(const<u64>(16), const<u64>(16))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
