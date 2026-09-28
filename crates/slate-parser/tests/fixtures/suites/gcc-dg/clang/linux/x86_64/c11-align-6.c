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
// DEFAULT-NEXT:     type @type0 = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 v: bool;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     type @type1 = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 v: i8;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     type @type2 = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 v: i8;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     type @type3 = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 v: u8;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     type @type4 = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 v: i16;
// DEFAULT-NEXT:     } [size=4, align=2, offsets=[0, 2]];
// DEFAULT-NEXT:     type @type5 = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 v: u16;
// DEFAULT-NEXT:     } [size=4, align=2, offsets=[0, 2]];
// DEFAULT-NEXT:     type @type6 = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 v: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type7 = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 v: u32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type8 = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 v: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type9 = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 v: u64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type10 = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 v: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type11 = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 v: u64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type12 = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 v: f32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type13 = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 v: f64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type14 = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 v: f80;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 16]];
// DEFAULT-NEXT:     type @type15 = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 v: complex<f32>;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type16 = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 v: complex<f64>;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type17 = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 v: complex<f80>;
// DEFAULT-NEXT:     } [size=48, align=16, offsets=[0, 16]];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%39 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         do %40
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %4 x: @type0 [storage=automatic];
// DEFAULT-NEXT:                 if gt<u64>(const<u64>(1), const<u64>(1))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %41
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %6 x: @type1 [storage=automatic];
// DEFAULT-NEXT:                 if gt<u64>(const<u64>(1), const<u64>(1))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %42
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %8 x: @type2 [storage=automatic];
// DEFAULT-NEXT:                 if gt<u64>(const<u64>(1), const<u64>(1))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %43
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %10 x: @type3 [storage=automatic];
// DEFAULT-NEXT:                 if gt<u64>(const<u64>(1), const<u64>(1))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %44
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %12 x: @type4 [storage=automatic];
// DEFAULT-NEXT:                 if gt<u64>(const<u64>(2), const<u64>(2))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %45
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %14 x: @type5 [storage=automatic];
// DEFAULT-NEXT:                 if gt<u64>(const<u64>(2), const<u64>(2))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %46
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %16 x: @type6 [storage=automatic];
// DEFAULT-NEXT:                 if gt<u64>(const<u64>(4), const<u64>(4))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %47
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %18 x: @type7 [storage=automatic];
// DEFAULT-NEXT:                 if gt<u64>(const<u64>(4), const<u64>(4))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %48
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %20 x: @type8 [storage=automatic];
// DEFAULT-NEXT:                 if gt<u64>(const<u64>(8), const<u64>(8))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %49
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %22 x: @type9 [storage=automatic];
// DEFAULT-NEXT:                 if gt<u64>(const<u64>(8), const<u64>(8))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %50
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %24 x: @type10 [storage=automatic];
// DEFAULT-NEXT:                 if gt<u64>(const<u64>(8), const<u64>(8))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %51
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %26 x: @type11 [storage=automatic];
// DEFAULT-NEXT:                 if gt<u64>(const<u64>(8), const<u64>(8))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %52
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %28 x: @type12 [storage=automatic];
// DEFAULT-NEXT:                 if gt<u64>(const<u64>(4), const<u64>(4))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %53
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %30 x: @type13 [storage=automatic];
// DEFAULT-NEXT:                 if gt<u64>(const<u64>(8), const<u64>(8))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %54
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %32 x: @type14 [storage=automatic];
// DEFAULT-NEXT:                 if gt<u64>(const<u64>(16), const<u64>(16))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %55
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %34 x: @type15 [storage=automatic];
// DEFAULT-NEXT:                 if gt<u64>(const<u64>(4), const<u64>(4))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %56
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %36 x: @type16 [storage=automatic];
// DEFAULT-NEXT:                 if gt<u64>(const<u64>(8), const<u64>(8))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %57
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %38 x: @type17 [storage=automatic];
// DEFAULT-NEXT:                 if gt<u64>(const<u64>(16), const<u64>(16))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
