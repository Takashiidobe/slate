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
// DEFAULT-NEXT:     type @type[[TYPE_fpt:[0-9]+]] fpt = ptr<fn() -> void>;
// DEFAULT-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 y: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_u:[0-9]+]] u = union {
// DEFAULT-NEXT:         field0 p: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_T1:[0-9]+]] T1 = i8;
// DEFAULT-NEXT:     type @type[[TYPE_T2:[0-9]+]] T2 = i8;
// DEFAULT-NEXT:     type @type[[TYPE_T1pp:[0-9]+]] T1pp = ptr<ptr<i8>>;
// DEFAULT-NEXT:     type @type[[TYPE_T2pp:[0-9]+]] T2pp = ptr<ptr<i8>>;
// DEFAULT-NEXT:     type @type[[TYPE_T1a:[0-9]+]] T1a = i8;
// DEFAULT-NEXT:     type @type[[TYPE_T1b:[0-9]+]] T1b = i8;
// DEFAULT-NEXT:     type @type[[TYPE_T2a:[0-9]+]] T2a = i8;
// DEFAULT-NEXT:     type @type[[TYPE_T2b:[0-9]+]] T2b = i8;
// DEFAULT-NEXT:     type @type[[TYPE_T1app:[0-9]+]] T1app = ptr<ptr<i8>>;
// DEFAULT-NEXT:     type @type[[TYPE_T1bpp:[0-9]+]] T1bpp = ptr<ptr<i8>>;
// DEFAULT-NEXT:     type @type[[TYPE_T2app:[0-9]+]] T2app = ptr<ptr<i8>>;
// DEFAULT-NEXT:     type @type[[TYPE_T2bpp:[0-9]+]] T2bpp = ptr<ptr<i8>>;
// DEFAULT-NEXT:     type @type[[TYPE_T1_2:[0-9]+]] T1 = ptr<void>;
// DEFAULT-NEXT:     type @type[[TYPE_T2_2:[0-9]+]] T2 = i8;
// DEFAULT-NEXT:     type @type[[TYPE_T1pp_2:[0-9]+]] T1pp = ptr<ptr<ptr<void>>>;
// DEFAULT-NEXT:     type @type[[TYPE_T2pp_2:[0-9]+]] T2pp = ptr<ptr<i8>>;
// DEFAULT-NEXT:     type @type[[TYPE_T1a_2:[0-9]+]] T1a = ptr<void>;
// DEFAULT-NEXT:     type @type[[TYPE_T1b_2:[0-9]+]] T1b = ptr<void>;
// DEFAULT-NEXT:     type @type[[TYPE_T2a_2:[0-9]+]] T2a = i8;
// DEFAULT-NEXT:     type @type[[TYPE_T2b_2:[0-9]+]] T2b = i8;
// DEFAULT-NEXT:     type @type[[TYPE_T1app_2:[0-9]+]] T1app = ptr<ptr<ptr<void>>>;
// DEFAULT-NEXT:     type @type[[TYPE_T1bpp_2:[0-9]+]] T1bpp = ptr<ptr<ptr<void>>>;
// DEFAULT-NEXT:     type @type[[TYPE_T2app_2:[0-9]+]] T2app = ptr<ptr<i8>>;
// DEFAULT-NEXT:     type @type[[TYPE_T2bpp_2:[0-9]+]] T2bpp = ptr<ptr<i8>>;
// DEFAULT-NEXT:     type @type[[TYPE_T1_3:[0-9]+]] T1 = i32;
// DEFAULT-NEXT:     type @type[[TYPE_T2_3:[0-9]+]] T2 = i32;
// DEFAULT-NEXT:     type @type[[TYPE_T1pp_3:[0-9]+]] T1pp = ptr<ptr<i32>>;
// DEFAULT-NEXT:     type @type[[TYPE_T2pp_3:[0-9]+]] T2pp = ptr<ptr<i32>>;
// DEFAULT-NEXT:     type @type[[TYPE_T1a_3:[0-9]+]] T1a = i32;
// DEFAULT-NEXT:     type @type[[TYPE_T1b_3:[0-9]+]] T1b = i32;
// DEFAULT-NEXT:     type @type[[TYPE_T2a_3:[0-9]+]] T2a = i32;
// DEFAULT-NEXT:     type @type[[TYPE_T2b_3:[0-9]+]] T2b = i32;
// DEFAULT-NEXT:     type @type[[TYPE_T1app_3:[0-9]+]] T1app = ptr<ptr<i32>>;
// DEFAULT-NEXT:     type @type[[TYPE_T1bpp_3:[0-9]+]] T1bpp = ptr<ptr<i32>>;
// DEFAULT-NEXT:     type @type[[TYPE_T2app_3:[0-9]+]] T2app = ptr<ptr<i32>>;
// DEFAULT-NEXT:     type @type[[TYPE_T2bpp_3:[0-9]+]] T2bpp = ptr<ptr<i32>>;
// DEFAULT-NEXT:     type @type[[TYPE_T1_4:[0-9]+]] T1 = f64;
// DEFAULT-NEXT:     type @type[[TYPE_T2_4:[0-9]+]] T2 = fn() -> i32;
// DEFAULT-NEXT:     type @type[[TYPE_T1pp_4:[0-9]+]] T1pp = ptr<ptr<f64>>;
// DEFAULT-NEXT:     type @type[[TYPE_T2pp_4:[0-9]+]] T2pp = ptr<ptr<fn() -> i32>>;
// DEFAULT-NEXT:     type @type[[TYPE_T1a_4:[0-9]+]] T1a = f64;
// DEFAULT-NEXT:     type @type[[TYPE_T1b_4:[0-9]+]] T1b = f64;
// DEFAULT-NEXT:     type @type[[TYPE_T2a_4:[0-9]+]] T2a = fn() -> i32;
// DEFAULT-NEXT:     type @type[[TYPE_T2b_4:[0-9]+]] T2b = fn() -> i32;
// DEFAULT-NEXT:     type @type[[TYPE_T1app_4:[0-9]+]] T1app = ptr<ptr<f64>>;
// DEFAULT-NEXT:     type @type[[TYPE_T1bpp_4:[0-9]+]] T1bpp = ptr<ptr<f64>>;
// DEFAULT-NEXT:     type @type[[TYPE_T2app_4:[0-9]+]] T2app = ptr<ptr<fn() -> i32>>;
// DEFAULT-NEXT:     type @type[[TYPE_T2bpp_4:[0-9]+]] T2bpp = ptr<ptr<fn() -> i32>>;
// DEFAULT-NEXT:     type @type[[TYPE_T1_5:[0-9]+]] T1 = f32;
// DEFAULT-NEXT:     type @type[[TYPE_T2_5:[0-9]+]] T2 = i32;
// DEFAULT-NEXT:     type @type[[TYPE_T1pp_5:[0-9]+]] T1pp = ptr<ptr<f32>>;
// DEFAULT-NEXT:     type @type[[TYPE_T2pp_5:[0-9]+]] T2pp = ptr<ptr<i32>>;
// DEFAULT-NEXT:     type @type[[TYPE_T1a_5:[0-9]+]] T1a = f32;
// DEFAULT-NEXT:     type @type[[TYPE_T1b_5:[0-9]+]] T1b = f32;
// DEFAULT-NEXT:     type @type[[TYPE_T2a_5:[0-9]+]] T2a = i32;
// DEFAULT-NEXT:     type @type[[TYPE_T2b_5:[0-9]+]] T2b = i32;
// DEFAULT-NEXT:     type @type[[TYPE_T1app_5:[0-9]+]] T1app = ptr<ptr<f32>>;
// DEFAULT-NEXT:     type @type[[TYPE_T1bpp_5:[0-9]+]] T1bpp = ptr<ptr<f32>>;
// DEFAULT-NEXT:     type @type[[TYPE_T2app_5:[0-9]+]] T2app = ptr<ptr<i32>>;
// DEFAULT-NEXT:     type @type[[TYPE_T2bpp_5:[0-9]+]] T2bpp = ptr<ptr<i32>>;
// DEFAULT-NEXT:     type @type[[TYPE_T1_6:[0-9]+]] T1 = i32;
// DEFAULT-NEXT:     type @type[[TYPE_T2_6:[0-9]+]] T2 = f32;
// DEFAULT-NEXT:     type @type[[TYPE_T1pp_6:[0-9]+]] T1pp = ptr<ptr<i32>>;
// DEFAULT-NEXT:     type @type[[TYPE_T2pp_6:[0-9]+]] T2pp = ptr<ptr<f32>>;
// DEFAULT-NEXT:     type @type[[TYPE_T1a_6:[0-9]+]] T1a = i32;
// DEFAULT-NEXT:     type @type[[TYPE_T1b_6:[0-9]+]] T1b = i32;
// DEFAULT-NEXT:     type @type[[TYPE_T2a_6:[0-9]+]] T2a = f32;
// DEFAULT-NEXT:     type @type[[TYPE_T2b_6:[0-9]+]] T2b = f32;
// DEFAULT-NEXT:     type @type[[TYPE_T1app_6:[0-9]+]] T1app = ptr<ptr<i32>>;
// DEFAULT-NEXT:     type @type[[TYPE_T1bpp_6:[0-9]+]] T1bpp = ptr<ptr<i32>>;
// DEFAULT-NEXT:     type @type[[TYPE_T2app_6:[0-9]+]] T2app = ptr<ptr<f32>>;
// DEFAULT-NEXT:     type @type[[TYPE_T2bpp_6:[0-9]+]] T2bpp = ptr<ptr<f32>>;
// DEFAULT-NEXT:     type @type[[TYPE_T1_7:[0-9]+]] T1 = ptr<fn() -> void>;
// DEFAULT-NEXT:     type @type[[TYPE_T2_7:[0-9]+]] T2 = @type[[TYPE_S]];
// DEFAULT-NEXT:     type @type[[TYPE_T1pp_7:[0-9]+]] T1pp = ptr<ptr<ptr<fn() -> void>>>;
// DEFAULT-NEXT:     type @type[[TYPE_T2pp_7:[0-9]+]] T2pp = ptr<ptr<@type[[TYPE_S]]>>;
// DEFAULT-NEXT:     type @type[[TYPE_T1a_7:[0-9]+]] T1a = ptr<fn() -> void>;
// DEFAULT-NEXT:     type @type[[TYPE_T1b_7:[0-9]+]] T1b = ptr<fn() -> void>;
// DEFAULT-NEXT:     type @type[[TYPE_T2a_7:[0-9]+]] T2a = @type[[TYPE_S]];
// DEFAULT-NEXT:     type @type[[TYPE_T2b_7:[0-9]+]] T2b = @type[[TYPE_S]];
// DEFAULT-NEXT:     type @type[[TYPE_T1app_7:[0-9]+]] T1app = ptr<ptr<ptr<fn() -> void>>>;
// DEFAULT-NEXT:     type @type[[TYPE_T1bpp_7:[0-9]+]] T1bpp = ptr<ptr<ptr<fn() -> void>>>;
// DEFAULT-NEXT:     type @type[[TYPE_T2app_7:[0-9]+]] T2app = ptr<ptr<@type[[TYPE_S]]>>;
// DEFAULT-NEXT:     type @type[[TYPE_T2bpp_7:[0-9]+]] T2bpp = ptr<ptr<@type[[TYPE_S]]>>;
// DEFAULT-NEXT:     type @type[[TYPE_T1_8:[0-9]+]] T1 = @type[[TYPE_u]];
// DEFAULT-NEXT:     type @type[[TYPE_T2_8:[0-9]+]] T2 = @type[[TYPE_u]];
// DEFAULT-NEXT:     type @type[[TYPE_T1pp_8:[0-9]+]] T1pp = ptr<ptr<@type[[TYPE_u]]>>;
// DEFAULT-NEXT:     type @type[[TYPE_T2pp_8:[0-9]+]] T2pp = ptr<ptr<@type[[TYPE_u]]>>;
// DEFAULT-NEXT:     type @type[[TYPE_T1a_8:[0-9]+]] T1a = @type[[TYPE_u]];
// DEFAULT-NEXT:     type @type[[TYPE_T1b_8:[0-9]+]] T1b = @type[[TYPE_u]];
// DEFAULT-NEXT:     type @type[[TYPE_T2a_8:[0-9]+]] T2a = @type[[TYPE_u]];
// DEFAULT-NEXT:     type @type[[TYPE_T2b_8:[0-9]+]] T2b = @type[[TYPE_u]];
// DEFAULT-NEXT:     type @type[[TYPE_T1app_8:[0-9]+]] T1app = ptr<ptr<@type[[TYPE_u]]>>;
// DEFAULT-NEXT:     type @type[[TYPE_T1bpp_8:[0-9]+]] T1bpp = ptr<ptr<@type[[TYPE_u]]>>;
// DEFAULT-NEXT:     type @type[[TYPE_T2app_8:[0-9]+]] T2app = ptr<ptr<@type[[TYPE_u]]>>;
// DEFAULT-NEXT:     type @type[[TYPE_T2bpp_8:[0-9]+]] T2bpp = ptr<ptr<@type[[TYPE_u]]>>;
// DEFAULT-NEXT:     type @type[[TYPE_T1_9:[0-9]+]] T1 = @type[[TYPE_u]];
// DEFAULT-NEXT:     type @type[[TYPE_T2_9:[0-9]+]] T2 = fn() -> i32;
// DEFAULT-NEXT:     type @type[[TYPE_T1pp_9:[0-9]+]] T1pp = ptr<ptr<@type[[TYPE_u]]>>;
// DEFAULT-NEXT:     type @type[[TYPE_T2pp_9:[0-9]+]] T2pp = ptr<ptr<fn() -> i32>>;
// DEFAULT-NEXT:     type @type[[TYPE_T1a_9:[0-9]+]] T1a = @type[[TYPE_u]];
// DEFAULT-NEXT:     type @type[[TYPE_T1b_9:[0-9]+]] T1b = @type[[TYPE_u]];
// DEFAULT-NEXT:     type @type[[TYPE_T2a_9:[0-9]+]] T2a = fn() -> i32;
// DEFAULT-NEXT:     type @type[[TYPE_T2b_9:[0-9]+]] T2b = fn() -> i32;
// DEFAULT-NEXT:     type @type[[TYPE_T1app_9:[0-9]+]] T1app = ptr<ptr<@type[[TYPE_u]]>>;
// DEFAULT-NEXT:     type @type[[TYPE_T1bpp_9:[0-9]+]] T1bpp = ptr<ptr<@type[[TYPE_u]]>>;
// DEFAULT-NEXT:     type @type[[TYPE_T2app_9:[0-9]+]] T2app = ptr<ptr<fn() -> i32>>;
// DEFAULT-NEXT:     type @type[[TYPE_T2bpp_9:[0-9]+]] T2bpp = ptr<ptr<fn() -> i32>>;
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_bad:[0-9]+]] @bad() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_good:[0-9]+]] @good() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_sc1:[0-9]+]] sc1: i8 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_sc2:[0-9]+]] sc2: i8 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_v1:[0-9]+]] v1: ptr<void> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_dd:[0-9]+]] dd: f64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_f:[0-9]+]] f: f32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_triple:[0-9]+]] triple: ptr<fn() -> void> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_pour:[0-9]+]] pour: @type[[TYPE_S]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_some:[0-9]+]] some: @type[[TYPE_S]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_sugar:[0-9]+]] sugar: @type[[TYPE_S]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_united:[0-9]+]] united: @type[[TYPE_u]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_nations:[0-9]+]] nations: @type[[TYPE_u]] [storage=automatic];
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(const<i32>(0), const<i32>(0)), not<bool>(ne<i32>(const<i32>(5), const<i32>(0)))), not<bool>(ne<i32>(const<i32>(3), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         do %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_t1:[0-9]+]] t1: ptr<ptr<i8>> [storage=automatic] = null<ptr<ptr<i8>>>;
// DEFAULT-NEXT:                 let %[[VALUE_t2:[0-9]+]] t2: ptr<ptr<i8>> [storage=automatic] = null<ptr<ptr<i8>>>;
// DEFAULT-NEXT:                 let %[[VALUE_t1a:[0-9]+]] t1a: ptr<ptr<i8>> [storage=automatic] = null<ptr<ptr<i8>>>;
// DEFAULT-NEXT:                 let %[[VALUE_t1b:[0-9]+]] t1b: ptr<ptr<i8>> [storage=automatic] = null<ptr<ptr<i8>>>;
// DEFAULT-NEXT:                 let %[[VALUE_t2a:[0-9]+]] t2a: ptr<ptr<i8>> [storage=automatic] = null<ptr<ptr<i8>>>;
// DEFAULT-NEXT:                 let %[[VALUE_t2b:[0-9]+]] t2b: ptr<ptr<i8>> [storage=automatic] = null<ptr<ptr<i8>>>;
// DEFAULT-NEXT:                 write<ptr<ptr<i8>>>(%[[VALUE_t1]], read<ptr<ptr<i8>>>(%[[VALUE_t1a]]));
// DEFAULT-NEXT:                 write<ptr<ptr<i8>>>(%[[VALUE_t1]], read<ptr<ptr<i8>>>(%[[VALUE_t1b]]));
// DEFAULT-NEXT:                 write<ptr<ptr<i8>>>(%[[VALUE_t2]], read<ptr<ptr<i8>>>(%[[VALUE_t2a]]));
// DEFAULT-NEXT:                 write<ptr<ptr<i8>>>(%[[VALUE_t2]], read<ptr<ptr<i8>>>(%[[VALUE_t2b]]));
// DEFAULT-NEXT:                 read<ptr<ptr<i8>>>(%[[VALUE_t1]]);
// DEFAULT-NEXT:                 read<ptr<ptr<i8>>>(%[[VALUE_t2]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE2:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_t1_2:[0-9]+]] t1: ptr<ptr<ptr<void>>> [storage=automatic] = null<ptr<ptr<ptr<void>>>>;
// DEFAULT-NEXT:                 let %[[VALUE_t2_2:[0-9]+]] t2: ptr<ptr<i8>> [storage=automatic] = null<ptr<ptr<i8>>>;
// DEFAULT-NEXT:                 let %[[VALUE_t1a_2:[0-9]+]] t1a: ptr<ptr<ptr<void>>> [storage=automatic] = null<ptr<ptr<ptr<void>>>>;
// DEFAULT-NEXT:                 let %[[VALUE_t1b_2:[0-9]+]] t1b: ptr<ptr<ptr<void>>> [storage=automatic] = null<ptr<ptr<ptr<void>>>>;
// DEFAULT-NEXT:                 let %[[VALUE_t2a_2:[0-9]+]] t2a: ptr<ptr<i8>> [storage=automatic] = null<ptr<ptr<i8>>>;
// DEFAULT-NEXT:                 let %[[VALUE_t2b_2:[0-9]+]] t2b: ptr<ptr<i8>> [storage=automatic] = null<ptr<ptr<i8>>>;
// DEFAULT-NEXT:                 write<ptr<ptr<ptr<void>>>>(%[[VALUE_t1_2]], read<ptr<ptr<ptr<void>>>>(%[[VALUE_t1a_2]]));
// DEFAULT-NEXT:                 write<ptr<ptr<ptr<void>>>>(%[[VALUE_t1_2]], read<ptr<ptr<ptr<void>>>>(%[[VALUE_t1b_2]]));
// DEFAULT-NEXT:                 write<ptr<ptr<i8>>>(%[[VALUE_t2_2]], read<ptr<ptr<i8>>>(%[[VALUE_t2a_2]]));
// DEFAULT-NEXT:                 write<ptr<ptr<i8>>>(%[[VALUE_t2_2]], read<ptr<ptr<i8>>>(%[[VALUE_t2b_2]]));
// DEFAULT-NEXT:                 read<ptr<ptr<ptr<void>>>>(%[[VALUE_t1_2]]);
// DEFAULT-NEXT:                 read<ptr<ptr<i8>>>(%[[VALUE_t2_2]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_t1_3:[0-9]+]] t1: ptr<ptr<i32>> [storage=automatic] = null<ptr<ptr<i32>>>;
// DEFAULT-NEXT:                 let %[[VALUE_t2_3:[0-9]+]] t2: ptr<ptr<i32>> [storage=automatic] = null<ptr<ptr<i32>>>;
// DEFAULT-NEXT:                 let %[[VALUE_t1a_3:[0-9]+]] t1a: ptr<ptr<i32>> [storage=automatic] = null<ptr<ptr<i32>>>;
// DEFAULT-NEXT:                 let %[[VALUE_t1b_3:[0-9]+]] t1b: ptr<ptr<i32>> [storage=automatic] = null<ptr<ptr<i32>>>;
// DEFAULT-NEXT:                 let %[[VALUE_t2a_3:[0-9]+]] t2a: ptr<ptr<i32>> [storage=automatic] = null<ptr<ptr<i32>>>;
// DEFAULT-NEXT:                 let %[[VALUE_t2b_3:[0-9]+]] t2b: ptr<ptr<i32>> [storage=automatic] = null<ptr<ptr<i32>>>;
// DEFAULT-NEXT:                 write<ptr<ptr<i32>>>(%[[VALUE_t1_3]], read<ptr<ptr<i32>>>(%[[VALUE_t1a_3]]));
// DEFAULT-NEXT:                 write<ptr<ptr<i32>>>(%[[VALUE_t1_3]], read<ptr<ptr<i32>>>(%[[VALUE_t1b_3]]));
// DEFAULT-NEXT:                 write<ptr<ptr<i32>>>(%[[VALUE_t2_3]], read<ptr<ptr<i32>>>(%[[VALUE_t2a_3]]));
// DEFAULT-NEXT:                 write<ptr<ptr<i32>>>(%[[VALUE_t2_3]], read<ptr<ptr<i32>>>(%[[VALUE_t2b_3]]));
// DEFAULT-NEXT:                 read<ptr<ptr<i32>>>(%[[VALUE_t1_3]]);
// DEFAULT-NEXT:                 read<ptr<ptr<i32>>>(%[[VALUE_t2_3]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE4:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_t1_4:[0-9]+]] t1: ptr<ptr<f64>> [storage=automatic] = null<ptr<ptr<f64>>>;
// DEFAULT-NEXT:                 let %[[VALUE_t2_4:[0-9]+]] t2: ptr<ptr<fn() -> i32>> [storage=automatic] = null<ptr<ptr<fn() -> i32>>>;
// DEFAULT-NEXT:                 let %[[VALUE_t1a_4:[0-9]+]] t1a: ptr<ptr<f64>> [storage=automatic] = null<ptr<ptr<f64>>>;
// DEFAULT-NEXT:                 let %[[VALUE_t1b_4:[0-9]+]] t1b: ptr<ptr<f64>> [storage=automatic] = null<ptr<ptr<f64>>>;
// DEFAULT-NEXT:                 let %[[VALUE_t2a_4:[0-9]+]] t2a: ptr<ptr<fn() -> i32>> [storage=automatic] = null<ptr<ptr<fn() -> i32>>>;
// DEFAULT-NEXT:                 let %[[VALUE_t2b_4:[0-9]+]] t2b: ptr<ptr<fn() -> i32>> [storage=automatic] = null<ptr<ptr<fn() -> i32>>>;
// DEFAULT-NEXT:                 write<ptr<ptr<f64>>>(%[[VALUE_t1_4]], read<ptr<ptr<f64>>>(%[[VALUE_t1a_4]]));
// DEFAULT-NEXT:                 write<ptr<ptr<f64>>>(%[[VALUE_t1_4]], read<ptr<ptr<f64>>>(%[[VALUE_t1b_4]]));
// DEFAULT-NEXT:                 write<ptr<ptr<fn() -> i32>>>(%[[VALUE_t2_4]], read<ptr<ptr<fn() -> i32>>>(%[[VALUE_t2a_4]]));
// DEFAULT-NEXT:                 write<ptr<ptr<fn() -> i32>>>(%[[VALUE_t2_4]], read<ptr<ptr<fn() -> i32>>>(%[[VALUE_t2b_4]]));
// DEFAULT-NEXT:                 read<ptr<ptr<f64>>>(%[[VALUE_t1_4]]);
// DEFAULT-NEXT:                 read<ptr<ptr<fn() -> i32>>>(%[[VALUE_t2_4]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE5:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_t1_5:[0-9]+]] t1: ptr<ptr<f32>> [storage=automatic] = null<ptr<ptr<f32>>>;
// DEFAULT-NEXT:                 let %[[VALUE_t2_5:[0-9]+]] t2: ptr<ptr<i32>> [storage=automatic] = null<ptr<ptr<i32>>>;
// DEFAULT-NEXT:                 let %[[VALUE_t1a_5:[0-9]+]] t1a: ptr<ptr<f32>> [storage=automatic] = null<ptr<ptr<f32>>>;
// DEFAULT-NEXT:                 let %[[VALUE_t1b_5:[0-9]+]] t1b: ptr<ptr<f32>> [storage=automatic] = null<ptr<ptr<f32>>>;
// DEFAULT-NEXT:                 let %[[VALUE_t2a_5:[0-9]+]] t2a: ptr<ptr<i32>> [storage=automatic] = null<ptr<ptr<i32>>>;
// DEFAULT-NEXT:                 let %[[VALUE_t2b_5:[0-9]+]] t2b: ptr<ptr<i32>> [storage=automatic] = null<ptr<ptr<i32>>>;
// DEFAULT-NEXT:                 write<ptr<ptr<f32>>>(%[[VALUE_t1_5]], read<ptr<ptr<f32>>>(%[[VALUE_t1a_5]]));
// DEFAULT-NEXT:                 write<ptr<ptr<f32>>>(%[[VALUE_t1_5]], read<ptr<ptr<f32>>>(%[[VALUE_t1b_5]]));
// DEFAULT-NEXT:                 write<ptr<ptr<i32>>>(%[[VALUE_t2_5]], read<ptr<ptr<i32>>>(%[[VALUE_t2a_5]]));
// DEFAULT-NEXT:                 write<ptr<ptr<i32>>>(%[[VALUE_t2_5]], read<ptr<ptr<i32>>>(%[[VALUE_t2b_5]]));
// DEFAULT-NEXT:                 read<ptr<ptr<f32>>>(%[[VALUE_t1_5]]);
// DEFAULT-NEXT:                 read<ptr<ptr<i32>>>(%[[VALUE_t2_5]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE6:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_t1_6:[0-9]+]] t1: ptr<ptr<i32>> [storage=automatic] = null<ptr<ptr<i32>>>;
// DEFAULT-NEXT:                 let %[[VALUE_t2_6:[0-9]+]] t2: ptr<ptr<f32>> [storage=automatic] = null<ptr<ptr<f32>>>;
// DEFAULT-NEXT:                 let %[[VALUE_t1a_6:[0-9]+]] t1a: ptr<ptr<i32>> [storage=automatic] = null<ptr<ptr<i32>>>;
// DEFAULT-NEXT:                 let %[[VALUE_t1b_6:[0-9]+]] t1b: ptr<ptr<i32>> [storage=automatic] = null<ptr<ptr<i32>>>;
// DEFAULT-NEXT:                 let %[[VALUE_t2a_6:[0-9]+]] t2a: ptr<ptr<f32>> [storage=automatic] = null<ptr<ptr<f32>>>;
// DEFAULT-NEXT:                 let %[[VALUE_t2b_6:[0-9]+]] t2b: ptr<ptr<f32>> [storage=automatic] = null<ptr<ptr<f32>>>;
// DEFAULT-NEXT:                 write<ptr<ptr<i32>>>(%[[VALUE_t1_6]], read<ptr<ptr<i32>>>(%[[VALUE_t1a_6]]));
// DEFAULT-NEXT:                 write<ptr<ptr<i32>>>(%[[VALUE_t1_6]], read<ptr<ptr<i32>>>(%[[VALUE_t1b_6]]));
// DEFAULT-NEXT:                 write<ptr<ptr<f32>>>(%[[VALUE_t2_6]], read<ptr<ptr<f32>>>(%[[VALUE_t2a_6]]));
// DEFAULT-NEXT:                 write<ptr<ptr<f32>>>(%[[VALUE_t2_6]], read<ptr<ptr<f32>>>(%[[VALUE_t2b_6]]));
// DEFAULT-NEXT:                 read<ptr<ptr<i32>>>(%[[VALUE_t1_6]]);
// DEFAULT-NEXT:                 read<ptr<ptr<f32>>>(%[[VALUE_t2_6]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE7:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_t1_7:[0-9]+]] t1: ptr<ptr<ptr<fn() -> void>>> [storage=automatic] = null<ptr<ptr<ptr<fn() -> void>>>>;
// DEFAULT-NEXT:                 let %[[VALUE_t2_7:[0-9]+]] t2: ptr<ptr<@type[[TYPE_S]]>> [storage=automatic] = null<ptr<ptr<@type[[TYPE_S]]>>>;
// DEFAULT-NEXT:                 let %[[VALUE_t1a_7:[0-9]+]] t1a: ptr<ptr<ptr<fn() -> void>>> [storage=automatic] = null<ptr<ptr<ptr<fn() -> void>>>>;
// DEFAULT-NEXT:                 let %[[VALUE_t1b_7:[0-9]+]] t1b: ptr<ptr<ptr<fn() -> void>>> [storage=automatic] = null<ptr<ptr<ptr<fn() -> void>>>>;
// DEFAULT-NEXT:                 let %[[VALUE_t2a_7:[0-9]+]] t2a: ptr<ptr<@type[[TYPE_S]]>> [storage=automatic] = null<ptr<ptr<@type[[TYPE_S]]>>>;
// DEFAULT-NEXT:                 let %[[VALUE_t2b_7:[0-9]+]] t2b: ptr<ptr<@type[[TYPE_S]]>> [storage=automatic] = null<ptr<ptr<@type[[TYPE_S]]>>>;
// DEFAULT-NEXT:                 write<ptr<ptr<ptr<fn() -> void>>>>(%[[VALUE_t1_7]], read<ptr<ptr<ptr<fn() -> void>>>>(%[[VALUE_t1a_7]]));
// DEFAULT-NEXT:                 write<ptr<ptr<ptr<fn() -> void>>>>(%[[VALUE_t1_7]], read<ptr<ptr<ptr<fn() -> void>>>>(%[[VALUE_t1b_7]]));
// DEFAULT-NEXT:                 write<ptr<ptr<@type[[TYPE_S]]>>>(%[[VALUE_t2_7]], read<ptr<ptr<@type[[TYPE_S]]>>>(%[[VALUE_t2a_7]]));
// DEFAULT-NEXT:                 write<ptr<ptr<@type[[TYPE_S]]>>>(%[[VALUE_t2_7]], read<ptr<ptr<@type[[TYPE_S]]>>>(%[[VALUE_t2b_7]]));
// DEFAULT-NEXT:                 read<ptr<ptr<ptr<fn() -> void>>>>(%[[VALUE_t1_7]]);
// DEFAULT-NEXT:                 read<ptr<ptr<@type[[TYPE_S]]>>>(%[[VALUE_t2_7]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE8:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_t1_8:[0-9]+]] t1: ptr<ptr<@type[[TYPE_u]]>> [storage=automatic] = null<ptr<ptr<@type[[TYPE_u]]>>>;
// DEFAULT-NEXT:                 let %[[VALUE_t2_8:[0-9]+]] t2: ptr<ptr<@type[[TYPE_u]]>> [storage=automatic] = null<ptr<ptr<@type[[TYPE_u]]>>>;
// DEFAULT-NEXT:                 let %[[VALUE_t1a_8:[0-9]+]] t1a: ptr<ptr<@type[[TYPE_u]]>> [storage=automatic] = null<ptr<ptr<@type[[TYPE_u]]>>>;
// DEFAULT-NEXT:                 let %[[VALUE_t1b_8:[0-9]+]] t1b: ptr<ptr<@type[[TYPE_u]]>> [storage=automatic] = null<ptr<ptr<@type[[TYPE_u]]>>>;
// DEFAULT-NEXT:                 let %[[VALUE_t2a_8:[0-9]+]] t2a: ptr<ptr<@type[[TYPE_u]]>> [storage=automatic] = null<ptr<ptr<@type[[TYPE_u]]>>>;
// DEFAULT-NEXT:                 let %[[VALUE_t2b_8:[0-9]+]] t2b: ptr<ptr<@type[[TYPE_u]]>> [storage=automatic] = null<ptr<ptr<@type[[TYPE_u]]>>>;
// DEFAULT-NEXT:                 write<ptr<ptr<@type[[TYPE_u]]>>>(%[[VALUE_t1_8]], read<ptr<ptr<@type[[TYPE_u]]>>>(%[[VALUE_t1a_8]]));
// DEFAULT-NEXT:                 write<ptr<ptr<@type[[TYPE_u]]>>>(%[[VALUE_t1_8]], read<ptr<ptr<@type[[TYPE_u]]>>>(%[[VALUE_t1b_8]]));
// DEFAULT-NEXT:                 write<ptr<ptr<@type[[TYPE_u]]>>>(%[[VALUE_t2_8]], read<ptr<ptr<@type[[TYPE_u]]>>>(%[[VALUE_t2a_8]]));
// DEFAULT-NEXT:                 write<ptr<ptr<@type[[TYPE_u]]>>>(%[[VALUE_t2_8]], read<ptr<ptr<@type[[TYPE_u]]>>>(%[[VALUE_t2b_8]]));
// DEFAULT-NEXT:                 read<ptr<ptr<@type[[TYPE_u]]>>>(%[[VALUE_t1_8]]);
// DEFAULT-NEXT:                 read<ptr<ptr<@type[[TYPE_u]]>>>(%[[VALUE_t2_8]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE9:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_t1_9:[0-9]+]] t1: ptr<ptr<@type[[TYPE_u]]>> [storage=automatic] = null<ptr<ptr<@type[[TYPE_u]]>>>;
// DEFAULT-NEXT:                 let %[[VALUE_t2_9:[0-9]+]] t2: ptr<ptr<fn() -> i32>> [storage=automatic] = null<ptr<ptr<fn() -> i32>>>;
// DEFAULT-NEXT:                 let %[[VALUE_t1a_9:[0-9]+]] t1a: ptr<ptr<@type[[TYPE_u]]>> [storage=automatic] = null<ptr<ptr<@type[[TYPE_u]]>>>;
// DEFAULT-NEXT:                 let %[[VALUE_t1b_9:[0-9]+]] t1b: ptr<ptr<@type[[TYPE_u]]>> [storage=automatic] = null<ptr<ptr<@type[[TYPE_u]]>>>;
// DEFAULT-NEXT:                 let %[[VALUE_t2a_9:[0-9]+]] t2a: ptr<ptr<fn() -> i32>> [storage=automatic] = null<ptr<ptr<fn() -> i32>>>;
// DEFAULT-NEXT:                 let %[[VALUE_t2b_9:[0-9]+]] t2b: ptr<ptr<fn() -> i32>> [storage=automatic] = null<ptr<ptr<fn() -> i32>>>;
// DEFAULT-NEXT:                 write<ptr<ptr<@type[[TYPE_u]]>>>(%[[VALUE_t1_9]], read<ptr<ptr<@type[[TYPE_u]]>>>(%[[VALUE_t1a_9]]));
// DEFAULT-NEXT:                 write<ptr<ptr<@type[[TYPE_u]]>>>(%[[VALUE_t1_9]], read<ptr<ptr<@type[[TYPE_u]]>>>(%[[VALUE_t1b_9]]));
// DEFAULT-NEXT:                 write<ptr<ptr<fn() -> i32>>>(%[[VALUE_t2_9]], read<ptr<ptr<fn() -> i32>>>(%[[VALUE_t2a_9]]));
// DEFAULT-NEXT:                 write<ptr<ptr<fn() -> i32>>>(%[[VALUE_t2_9]], read<ptr<ptr<fn() -> i32>>>(%[[VALUE_t2b_9]]));
// DEFAULT-NEXT:                 read<ptr<ptr<@type[[TYPE_u]]>>>(%[[VALUE_t1_9]]);
// DEFAULT-NEXT:                 read<ptr<ptr<fn() -> i32>>>(%[[VALUE_t2_9]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(field1(%[[VALUE_pour]]), const<i32>(69));
// DEFAULT-NEXT:         write<@type[[TYPE_S]]>(%[[VALUE_sugar]], copy<@type[[TYPE_S]], reason=assign>(read<@type[[TYPE_S]]>(%[[VALUE_pour]])));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(field1(%[[VALUE_sugar]])), const<i32>(69))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<f32>(%[[VALUE_f]], const<f32>(3.5));
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(read<f32>(%[[VALUE_f]]), const<f32>(3.5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_good]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
