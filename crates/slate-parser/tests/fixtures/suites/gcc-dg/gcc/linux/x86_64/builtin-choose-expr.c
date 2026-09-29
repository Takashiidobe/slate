/* { dg-do run } */
/* { dg-options "-O1 -Wall" } */

#define choose __builtin_choose_expr

/* Check the type of __builtin_choose_expr between E1 and E2, both
   ways round and with both 0 and 1 as the condition.  */
#define ASSERT_COND_TYPE(E1, E2)				\
        do {							\
          typedef __typeof(E1) T1;				\
          typedef __typeof(E2) T2;				\
          typedef T1 **T1pp;					\
          typedef T2 **T2pp;					\
          typedef __typeof(choose (1, (E1), (E2))) T1a;		\
          typedef __typeof(choose (0, (E2), (E1))) T1b;		\
          typedef __typeof(choose (1, (E2), (E1))) T2a;		\
          typedef __typeof(choose (0, (E1), (E2))) T2b;		\
          typedef T1a **T1app;					\
          typedef T1b **T1bpp;					\
          typedef T2a **T2app;					\
          typedef T2b **T2bpp;					\
          T1pp t1 = 0;						\
          T2pp t2 = 0;						\
          T1app t1a = 0;					\
          T1bpp t1b = 0;					\
          T2app t2a = 0;					\
          T2bpp t2b = 0;					\
          t1 = t1a;						\
          t1 = t1b;						\
          t2 = t2a;						\
          t2 = t2b;						\
          (void) t1;						\
          (void) t2;						\
        } while (0)


extern void abort ();
extern void exit (int);

void bad ()
{
  abort ();
}

void good ()
{
  exit (0);
}

int main (void)
{
  signed char sc1, sc2;
  void *v1;
  int i, j;
  double dd;
  float f;
  typedef void (*fpt)(void);
  fpt triple;
  struct S { int x, y; } pour, some, sugar;
  union u { int p; } united, nations;

  if (__builtin_choose_expr (0, 12, 0)
      || !__builtin_choose_expr (45, 5, 0)
      || !__builtin_choose_expr (45, 3, 0))
    abort ();

  ASSERT_COND_TYPE (sc1, sc2);
  ASSERT_COND_TYPE (v1, sc1);
  ASSERT_COND_TYPE (i, j);
  ASSERT_COND_TYPE (dd, main);
  ASSERT_COND_TYPE ((float)dd, i);
  ASSERT_COND_TYPE (4, f);
  ASSERT_COND_TYPE (triple, some);
  ASSERT_COND_TYPE (united, nations);
  ASSERT_COND_TYPE (nations, main);

  pour.y = 69;
  __builtin_choose_expr (0, bad (), sugar) = pour;
  if (sugar.y != 69)
    abort ();

  __builtin_choose_expr (sizeof (int), f, bad ()) = 3.5F;

  if (f != 3.5F)
    abort ();

  __builtin_choose_expr (1, good, bad)();

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
// DEFAULT-NEXT:     type @type0 fpt = ptr<fn() -> void>;
// DEFAULT-NEXT:     type @type1 S = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 y: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type2 u = union {
// DEFAULT-NEXT:         field0 p: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type3 T1 = i8;
// DEFAULT-NEXT:     type @type4 T2 = i8;
// DEFAULT-NEXT:     type @type5 T1pp = ptr<ptr<i8>>;
// DEFAULT-NEXT:     type @type6 T2pp = ptr<ptr<i8>>;
// DEFAULT-NEXT:     type @type7 T1a = i8;
// DEFAULT-NEXT:     type @type8 T1b = i8;
// DEFAULT-NEXT:     type @type9 T2a = i8;
// DEFAULT-NEXT:     type @type10 T2b = i8;
// DEFAULT-NEXT:     type @type11 T1app = ptr<ptr<i8>>;
// DEFAULT-NEXT:     type @type12 T1bpp = ptr<ptr<i8>>;
// DEFAULT-NEXT:     type @type13 T2app = ptr<ptr<i8>>;
// DEFAULT-NEXT:     type @type14 T2bpp = ptr<ptr<i8>>;
// DEFAULT-NEXT:     type @type15 T1 = ptr<void>;
// DEFAULT-NEXT:     type @type16 T2 = i8;
// DEFAULT-NEXT:     type @type17 T1pp = ptr<ptr<ptr<void>>>;
// DEFAULT-NEXT:     type @type18 T2pp = ptr<ptr<i8>>;
// DEFAULT-NEXT:     type @type19 T1a = ptr<void>;
// DEFAULT-NEXT:     type @type20 T1b = ptr<void>;
// DEFAULT-NEXT:     type @type21 T2a = i8;
// DEFAULT-NEXT:     type @type22 T2b = i8;
// DEFAULT-NEXT:     type @type23 T1app = ptr<ptr<ptr<void>>>;
// DEFAULT-NEXT:     type @type24 T1bpp = ptr<ptr<ptr<void>>>;
// DEFAULT-NEXT:     type @type25 T2app = ptr<ptr<i8>>;
// DEFAULT-NEXT:     type @type26 T2bpp = ptr<ptr<i8>>;
// DEFAULT-NEXT:     type @type27 T1 = i32;
// DEFAULT-NEXT:     type @type28 T2 = i32;
// DEFAULT-NEXT:     type @type29 T1pp = ptr<ptr<i32>>;
// DEFAULT-NEXT:     type @type30 T2pp = ptr<ptr<i32>>;
// DEFAULT-NEXT:     type @type31 T1a = i32;
// DEFAULT-NEXT:     type @type32 T1b = i32;
// DEFAULT-NEXT:     type @type33 T2a = i32;
// DEFAULT-NEXT:     type @type34 T2b = i32;
// DEFAULT-NEXT:     type @type35 T1app = ptr<ptr<i32>>;
// DEFAULT-NEXT:     type @type36 T1bpp = ptr<ptr<i32>>;
// DEFAULT-NEXT:     type @type37 T2app = ptr<ptr<i32>>;
// DEFAULT-NEXT:     type @type38 T2bpp = ptr<ptr<i32>>;
// DEFAULT-NEXT:     type @type39 T1 = f64;
// DEFAULT-NEXT:     type @type40 T2 = fn() -> i32;
// DEFAULT-NEXT:     type @type41 T1pp = ptr<ptr<f64>>;
// DEFAULT-NEXT:     type @type42 T2pp = ptr<ptr<fn() -> i32>>;
// DEFAULT-NEXT:     type @type43 T1a = f64;
// DEFAULT-NEXT:     type @type44 T1b = f64;
// DEFAULT-NEXT:     type @type45 T2a = fn() -> i32;
// DEFAULT-NEXT:     type @type46 T2b = fn() -> i32;
// DEFAULT-NEXT:     type @type47 T1app = ptr<ptr<f64>>;
// DEFAULT-NEXT:     type @type48 T1bpp = ptr<ptr<f64>>;
// DEFAULT-NEXT:     type @type49 T2app = ptr<ptr<fn() -> i32>>;
// DEFAULT-NEXT:     type @type50 T2bpp = ptr<ptr<fn() -> i32>>;
// DEFAULT-NEXT:     type @type51 T1 = f32;
// DEFAULT-NEXT:     type @type52 T2 = i32;
// DEFAULT-NEXT:     type @type53 T1pp = ptr<ptr<f32>>;
// DEFAULT-NEXT:     type @type54 T2pp = ptr<ptr<i32>>;
// DEFAULT-NEXT:     type @type55 T1a = f32;
// DEFAULT-NEXT:     type @type56 T1b = f32;
// DEFAULT-NEXT:     type @type57 T2a = i32;
// DEFAULT-NEXT:     type @type58 T2b = i32;
// DEFAULT-NEXT:     type @type59 T1app = ptr<ptr<f32>>;
// DEFAULT-NEXT:     type @type60 T1bpp = ptr<ptr<f32>>;
// DEFAULT-NEXT:     type @type61 T2app = ptr<ptr<i32>>;
// DEFAULT-NEXT:     type @type62 T2bpp = ptr<ptr<i32>>;
// DEFAULT-NEXT:     type @type63 T1 = i32;
// DEFAULT-NEXT:     type @type64 T2 = f32;
// DEFAULT-NEXT:     type @type65 T1pp = ptr<ptr<i32>>;
// DEFAULT-NEXT:     type @type66 T2pp = ptr<ptr<f32>>;
// DEFAULT-NEXT:     type @type67 T1a = i32;
// DEFAULT-NEXT:     type @type68 T1b = i32;
// DEFAULT-NEXT:     type @type69 T2a = f32;
// DEFAULT-NEXT:     type @type70 T2b = f32;
// DEFAULT-NEXT:     type @type71 T1app = ptr<ptr<i32>>;
// DEFAULT-NEXT:     type @type72 T1bpp = ptr<ptr<i32>>;
// DEFAULT-NEXT:     type @type73 T2app = ptr<ptr<f32>>;
// DEFAULT-NEXT:     type @type74 T2bpp = ptr<ptr<f32>>;
// DEFAULT-NEXT:     type @type75 T1 = ptr<fn() -> void>;
// DEFAULT-NEXT:     type @type76 T2 = @type1;
// DEFAULT-NEXT:     type @type77 T1pp = ptr<ptr<ptr<fn() -> void>>>;
// DEFAULT-NEXT:     type @type78 T2pp = ptr<ptr<@type1>>;
// DEFAULT-NEXT:     type @type79 T1a = ptr<fn() -> void>;
// DEFAULT-NEXT:     type @type80 T1b = ptr<fn() -> void>;
// DEFAULT-NEXT:     type @type81 T2a = @type1;
// DEFAULT-NEXT:     type @type82 T2b = @type1;
// DEFAULT-NEXT:     type @type83 T1app = ptr<ptr<ptr<fn() -> void>>>;
// DEFAULT-NEXT:     type @type84 T1bpp = ptr<ptr<ptr<fn() -> void>>>;
// DEFAULT-NEXT:     type @type85 T2app = ptr<ptr<@type1>>;
// DEFAULT-NEXT:     type @type86 T2bpp = ptr<ptr<@type1>>;
// DEFAULT-NEXT:     type @type87 T1 = @type2;
// DEFAULT-NEXT:     type @type88 T2 = @type2;
// DEFAULT-NEXT:     type @type89 T1pp = ptr<ptr<@type2>>;
// DEFAULT-NEXT:     type @type90 T2pp = ptr<ptr<@type2>>;
// DEFAULT-NEXT:     type @type91 T1a = @type2;
// DEFAULT-NEXT:     type @type92 T1b = @type2;
// DEFAULT-NEXT:     type @type93 T2a = @type2;
// DEFAULT-NEXT:     type @type94 T2b = @type2;
// DEFAULT-NEXT:     type @type95 T1app = ptr<ptr<@type2>>;
// DEFAULT-NEXT:     type @type96 T1bpp = ptr<ptr<@type2>>;
// DEFAULT-NEXT:     type @type97 T2app = ptr<ptr<@type2>>;
// DEFAULT-NEXT:     type @type98 T2bpp = ptr<ptr<@type2>>;
// DEFAULT-NEXT:     type @type99 T1 = @type2;
// DEFAULT-NEXT:     type @type100 T2 = fn() -> i32;
// DEFAULT-NEXT:     type @type101 T1pp = ptr<ptr<@type2>>;
// DEFAULT-NEXT:     type @type102 T2pp = ptr<ptr<fn() -> i32>>;
// DEFAULT-NEXT:     type @type103 T1a = @type2;
// DEFAULT-NEXT:     type @type104 T1b = @type2;
// DEFAULT-NEXT:     type @type105 T2a = fn() -> i32;
// DEFAULT-NEXT:     type @type106 T2b = fn() -> i32;
// DEFAULT-NEXT:     type @type107 T1app = ptr<ptr<@type2>>;
// DEFAULT-NEXT:     type @type108 T1bpp = ptr<ptr<@type2>>;
// DEFAULT-NEXT:     type @type109 T2app = ptr<ptr<fn() -> i32>>;
// DEFAULT-NEXT:     type @type110 T2bpp = ptr<ptr<fn() -> i32>>;
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%183 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @bad() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @good() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %5 sc1: i8 [storage=automatic];
// DEFAULT-NEXT:         let %6 sc2: i8 [storage=automatic];
// DEFAULT-NEXT:         let %7 v1: ptr<void> [storage=automatic];
// DEFAULT-NEXT:         let %8 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %9 j: i32 [storage=automatic];
// DEFAULT-NEXT:         let %10 dd: f64 [storage=automatic];
// DEFAULT-NEXT:         let %11 f: f32 [storage=automatic];
// DEFAULT-NEXT:         let %13 triple: ptr<fn() -> void> [storage=automatic];
// DEFAULT-NEXT:         let %15 pour: @type1 [storage=automatic];
// DEFAULT-NEXT:         let %16 some: @type1 [storage=automatic];
// DEFAULT-NEXT:         let %17 sugar: @type1 [storage=automatic];
// DEFAULT-NEXT:         let %19 united: @type2 [storage=automatic];
// DEFAULT-NEXT:         let %20 nations: @type2 [storage=automatic];
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(const<i32>(0), const<i32>(0)), not<bool>(ne<i32>(const<i32>(5), const<i32>(0)))), not<bool>(ne<i32>(const<i32>(3), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         do %184
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %33 t1: ptr<ptr<i8>> [storage=automatic] = null<ptr<ptr<i8>>>;
// DEFAULT-NEXT:                 let %34 t2: ptr<ptr<i8>> [storage=automatic] = null<ptr<ptr<i8>>>;
// DEFAULT-NEXT:                 let %35 t1a: ptr<ptr<i8>> [storage=automatic] = null<ptr<ptr<i8>>>;
// DEFAULT-NEXT:                 let %36 t1b: ptr<ptr<i8>> [storage=automatic] = null<ptr<ptr<i8>>>;
// DEFAULT-NEXT:                 let %37 t2a: ptr<ptr<i8>> [storage=automatic] = null<ptr<ptr<i8>>>;
// DEFAULT-NEXT:                 let %38 t2b: ptr<ptr<i8>> [storage=automatic] = null<ptr<ptr<i8>>>;
// DEFAULT-NEXT:                 write<ptr<ptr<i8>>>(%33, read<ptr<ptr<i8>>>(%35));
// DEFAULT-NEXT:                 write<ptr<ptr<i8>>>(%33, read<ptr<ptr<i8>>>(%36));
// DEFAULT-NEXT:                 write<ptr<ptr<i8>>>(%34, read<ptr<ptr<i8>>>(%37));
// DEFAULT-NEXT:                 write<ptr<ptr<i8>>>(%34, read<ptr<ptr<i8>>>(%38));
// DEFAULT-NEXT:                 read<ptr<ptr<i8>>>(%33);
// DEFAULT-NEXT:                 read<ptr<ptr<i8>>>(%34);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %185
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %51 t1: ptr<ptr<ptr<void>>> [storage=automatic] = null<ptr<ptr<ptr<void>>>>;
// DEFAULT-NEXT:                 let %52 t2: ptr<ptr<i8>> [storage=automatic] = null<ptr<ptr<i8>>>;
// DEFAULT-NEXT:                 let %53 t1a: ptr<ptr<ptr<void>>> [storage=automatic] = null<ptr<ptr<ptr<void>>>>;
// DEFAULT-NEXT:                 let %54 t1b: ptr<ptr<ptr<void>>> [storage=automatic] = null<ptr<ptr<ptr<void>>>>;
// DEFAULT-NEXT:                 let %55 t2a: ptr<ptr<i8>> [storage=automatic] = null<ptr<ptr<i8>>>;
// DEFAULT-NEXT:                 let %56 t2b: ptr<ptr<i8>> [storage=automatic] = null<ptr<ptr<i8>>>;
// DEFAULT-NEXT:                 write<ptr<ptr<ptr<void>>>>(%51, read<ptr<ptr<ptr<void>>>>(%53));
// DEFAULT-NEXT:                 write<ptr<ptr<ptr<void>>>>(%51, read<ptr<ptr<ptr<void>>>>(%54));
// DEFAULT-NEXT:                 write<ptr<ptr<i8>>>(%52, read<ptr<ptr<i8>>>(%55));
// DEFAULT-NEXT:                 write<ptr<ptr<i8>>>(%52, read<ptr<ptr<i8>>>(%56));
// DEFAULT-NEXT:                 read<ptr<ptr<ptr<void>>>>(%51);
// DEFAULT-NEXT:                 read<ptr<ptr<i8>>>(%52);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %186
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %69 t1: ptr<ptr<i32>> [storage=automatic] = null<ptr<ptr<i32>>>;
// DEFAULT-NEXT:                 let %70 t2: ptr<ptr<i32>> [storage=automatic] = null<ptr<ptr<i32>>>;
// DEFAULT-NEXT:                 let %71 t1a: ptr<ptr<i32>> [storage=automatic] = null<ptr<ptr<i32>>>;
// DEFAULT-NEXT:                 let %72 t1b: ptr<ptr<i32>> [storage=automatic] = null<ptr<ptr<i32>>>;
// DEFAULT-NEXT:                 let %73 t2a: ptr<ptr<i32>> [storage=automatic] = null<ptr<ptr<i32>>>;
// DEFAULT-NEXT:                 let %74 t2b: ptr<ptr<i32>> [storage=automatic] = null<ptr<ptr<i32>>>;
// DEFAULT-NEXT:                 write<ptr<ptr<i32>>>(%69, read<ptr<ptr<i32>>>(%71));
// DEFAULT-NEXT:                 write<ptr<ptr<i32>>>(%69, read<ptr<ptr<i32>>>(%72));
// DEFAULT-NEXT:                 write<ptr<ptr<i32>>>(%70, read<ptr<ptr<i32>>>(%73));
// DEFAULT-NEXT:                 write<ptr<ptr<i32>>>(%70, read<ptr<ptr<i32>>>(%74));
// DEFAULT-NEXT:                 read<ptr<ptr<i32>>>(%69);
// DEFAULT-NEXT:                 read<ptr<ptr<i32>>>(%70);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %187
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %87 t1: ptr<ptr<f64>> [storage=automatic] = null<ptr<ptr<f64>>>;
// DEFAULT-NEXT:                 let %88 t2: ptr<ptr<fn() -> i32>> [storage=automatic] = null<ptr<ptr<fn() -> i32>>>;
// DEFAULT-NEXT:                 let %89 t1a: ptr<ptr<f64>> [storage=automatic] = null<ptr<ptr<f64>>>;
// DEFAULT-NEXT:                 let %90 t1b: ptr<ptr<f64>> [storage=automatic] = null<ptr<ptr<f64>>>;
// DEFAULT-NEXT:                 let %91 t2a: ptr<ptr<fn() -> i32>> [storage=automatic] = null<ptr<ptr<fn() -> i32>>>;
// DEFAULT-NEXT:                 let %92 t2b: ptr<ptr<fn() -> i32>> [storage=automatic] = null<ptr<ptr<fn() -> i32>>>;
// DEFAULT-NEXT:                 write<ptr<ptr<f64>>>(%87, read<ptr<ptr<f64>>>(%89));
// DEFAULT-NEXT:                 write<ptr<ptr<f64>>>(%87, read<ptr<ptr<f64>>>(%90));
// DEFAULT-NEXT:                 write<ptr<ptr<fn() -> i32>>>(%88, read<ptr<ptr<fn() -> i32>>>(%91));
// DEFAULT-NEXT:                 write<ptr<ptr<fn() -> i32>>>(%88, read<ptr<ptr<fn() -> i32>>>(%92));
// DEFAULT-NEXT:                 read<ptr<ptr<f64>>>(%87);
// DEFAULT-NEXT:                 read<ptr<ptr<fn() -> i32>>>(%88);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %188
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %105 t1: ptr<ptr<f32>> [storage=automatic] = null<ptr<ptr<f32>>>;
// DEFAULT-NEXT:                 let %106 t2: ptr<ptr<i32>> [storage=automatic] = null<ptr<ptr<i32>>>;
// DEFAULT-NEXT:                 let %107 t1a: ptr<ptr<f32>> [storage=automatic] = null<ptr<ptr<f32>>>;
// DEFAULT-NEXT:                 let %108 t1b: ptr<ptr<f32>> [storage=automatic] = null<ptr<ptr<f32>>>;
// DEFAULT-NEXT:                 let %109 t2a: ptr<ptr<i32>> [storage=automatic] = null<ptr<ptr<i32>>>;
// DEFAULT-NEXT:                 let %110 t2b: ptr<ptr<i32>> [storage=automatic] = null<ptr<ptr<i32>>>;
// DEFAULT-NEXT:                 write<ptr<ptr<f32>>>(%105, read<ptr<ptr<f32>>>(%107));
// DEFAULT-NEXT:                 write<ptr<ptr<f32>>>(%105, read<ptr<ptr<f32>>>(%108));
// DEFAULT-NEXT:                 write<ptr<ptr<i32>>>(%106, read<ptr<ptr<i32>>>(%109));
// DEFAULT-NEXT:                 write<ptr<ptr<i32>>>(%106, read<ptr<ptr<i32>>>(%110));
// DEFAULT-NEXT:                 read<ptr<ptr<f32>>>(%105);
// DEFAULT-NEXT:                 read<ptr<ptr<i32>>>(%106);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %189
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %123 t1: ptr<ptr<i32>> [storage=automatic] = null<ptr<ptr<i32>>>;
// DEFAULT-NEXT:                 let %124 t2: ptr<ptr<f32>> [storage=automatic] = null<ptr<ptr<f32>>>;
// DEFAULT-NEXT:                 let %125 t1a: ptr<ptr<i32>> [storage=automatic] = null<ptr<ptr<i32>>>;
// DEFAULT-NEXT:                 let %126 t1b: ptr<ptr<i32>> [storage=automatic] = null<ptr<ptr<i32>>>;
// DEFAULT-NEXT:                 let %127 t2a: ptr<ptr<f32>> [storage=automatic] = null<ptr<ptr<f32>>>;
// DEFAULT-NEXT:                 let %128 t2b: ptr<ptr<f32>> [storage=automatic] = null<ptr<ptr<f32>>>;
// DEFAULT-NEXT:                 write<ptr<ptr<i32>>>(%123, read<ptr<ptr<i32>>>(%125));
// DEFAULT-NEXT:                 write<ptr<ptr<i32>>>(%123, read<ptr<ptr<i32>>>(%126));
// DEFAULT-NEXT:                 write<ptr<ptr<f32>>>(%124, read<ptr<ptr<f32>>>(%127));
// DEFAULT-NEXT:                 write<ptr<ptr<f32>>>(%124, read<ptr<ptr<f32>>>(%128));
// DEFAULT-NEXT:                 read<ptr<ptr<i32>>>(%123);
// DEFAULT-NEXT:                 read<ptr<ptr<f32>>>(%124);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %190
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %141 t1: ptr<ptr<ptr<fn() -> void>>> [storage=automatic] = null<ptr<ptr<ptr<fn() -> void>>>>;
// DEFAULT-NEXT:                 let %142 t2: ptr<ptr<@type1>> [storage=automatic] = null<ptr<ptr<@type1>>>;
// DEFAULT-NEXT:                 let %143 t1a: ptr<ptr<ptr<fn() -> void>>> [storage=automatic] = null<ptr<ptr<ptr<fn() -> void>>>>;
// DEFAULT-NEXT:                 let %144 t1b: ptr<ptr<ptr<fn() -> void>>> [storage=automatic] = null<ptr<ptr<ptr<fn() -> void>>>>;
// DEFAULT-NEXT:                 let %145 t2a: ptr<ptr<@type1>> [storage=automatic] = null<ptr<ptr<@type1>>>;
// DEFAULT-NEXT:                 let %146 t2b: ptr<ptr<@type1>> [storage=automatic] = null<ptr<ptr<@type1>>>;
// DEFAULT-NEXT:                 write<ptr<ptr<ptr<fn() -> void>>>>(%141, read<ptr<ptr<ptr<fn() -> void>>>>(%143));
// DEFAULT-NEXT:                 write<ptr<ptr<ptr<fn() -> void>>>>(%141, read<ptr<ptr<ptr<fn() -> void>>>>(%144));
// DEFAULT-NEXT:                 write<ptr<ptr<@type1>>>(%142, read<ptr<ptr<@type1>>>(%145));
// DEFAULT-NEXT:                 write<ptr<ptr<@type1>>>(%142, read<ptr<ptr<@type1>>>(%146));
// DEFAULT-NEXT:                 read<ptr<ptr<ptr<fn() -> void>>>>(%141);
// DEFAULT-NEXT:                 read<ptr<ptr<@type1>>>(%142);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %191
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %159 t1: ptr<ptr<@type2>> [storage=automatic] = null<ptr<ptr<@type2>>>;
// DEFAULT-NEXT:                 let %160 t2: ptr<ptr<@type2>> [storage=automatic] = null<ptr<ptr<@type2>>>;
// DEFAULT-NEXT:                 let %161 t1a: ptr<ptr<@type2>> [storage=automatic] = null<ptr<ptr<@type2>>>;
// DEFAULT-NEXT:                 let %162 t1b: ptr<ptr<@type2>> [storage=automatic] = null<ptr<ptr<@type2>>>;
// DEFAULT-NEXT:                 let %163 t2a: ptr<ptr<@type2>> [storage=automatic] = null<ptr<ptr<@type2>>>;
// DEFAULT-NEXT:                 let %164 t2b: ptr<ptr<@type2>> [storage=automatic] = null<ptr<ptr<@type2>>>;
// DEFAULT-NEXT:                 write<ptr<ptr<@type2>>>(%159, read<ptr<ptr<@type2>>>(%161));
// DEFAULT-NEXT:                 write<ptr<ptr<@type2>>>(%159, read<ptr<ptr<@type2>>>(%162));
// DEFAULT-NEXT:                 write<ptr<ptr<@type2>>>(%160, read<ptr<ptr<@type2>>>(%163));
// DEFAULT-NEXT:                 write<ptr<ptr<@type2>>>(%160, read<ptr<ptr<@type2>>>(%164));
// DEFAULT-NEXT:                 read<ptr<ptr<@type2>>>(%159);
// DEFAULT-NEXT:                 read<ptr<ptr<@type2>>>(%160);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %192
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %177 t1: ptr<ptr<@type2>> [storage=automatic] = null<ptr<ptr<@type2>>>;
// DEFAULT-NEXT:                 let %178 t2: ptr<ptr<fn() -> i32>> [storage=automatic] = null<ptr<ptr<fn() -> i32>>>;
// DEFAULT-NEXT:                 let %179 t1a: ptr<ptr<@type2>> [storage=automatic] = null<ptr<ptr<@type2>>>;
// DEFAULT-NEXT:                 let %180 t1b: ptr<ptr<@type2>> [storage=automatic] = null<ptr<ptr<@type2>>>;
// DEFAULT-NEXT:                 let %181 t2a: ptr<ptr<fn() -> i32>> [storage=automatic] = null<ptr<ptr<fn() -> i32>>>;
// DEFAULT-NEXT:                 let %182 t2b: ptr<ptr<fn() -> i32>> [storage=automatic] = null<ptr<ptr<fn() -> i32>>>;
// DEFAULT-NEXT:                 write<ptr<ptr<@type2>>>(%177, read<ptr<ptr<@type2>>>(%179));
// DEFAULT-NEXT:                 write<ptr<ptr<@type2>>>(%177, read<ptr<ptr<@type2>>>(%180));
// DEFAULT-NEXT:                 write<ptr<ptr<fn() -> i32>>>(%178, read<ptr<ptr<fn() -> i32>>>(%181));
// DEFAULT-NEXT:                 write<ptr<ptr<fn() -> i32>>>(%178, read<ptr<ptr<fn() -> i32>>>(%182));
// DEFAULT-NEXT:                 read<ptr<ptr<@type2>>>(%177);
// DEFAULT-NEXT:                 read<ptr<ptr<fn() -> i32>>>(%178);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(field1(%15), const<i32>(69));
// DEFAULT-NEXT:         write<@type1>(%17, copy<@type1, reason=assign>(read<@type1>(%15)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(field1(%17)), const<i32>(69))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<f32>(%11, const<f32>(3.5));
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(read<f32>(%11), const<f32>(3.5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
