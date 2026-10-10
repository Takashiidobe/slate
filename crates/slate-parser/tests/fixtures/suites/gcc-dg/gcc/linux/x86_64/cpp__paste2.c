/* Copyright (C) 2000 Free Software Foundation, Inc.  */

/* { dg-do run } */
/* { dg-options "-std=c99 -pedantic-errors" } */

/* Test ## behavior and corner cases thoroughly.  The macro expander
   failed many of these during development.  */

#ifndef __WCHAR_TYPE__
#define __WCHAR_TYPE__ int
#endif
typedef __WCHAR_TYPE__ wchar_t;

extern int strcmp (const char *, const char *);
#if DEBUG
extern int puts (const char *);
#else
#define puts(X)
#endif
extern void abort (void);
#define err(str) do { puts(str); abort(); } while (0)

#define EMPTY
#define str(x) #x
#define xstr(x) str(x)
#define glue(x, y) x ## y
#define xglue(x, y) glue (x, y)
#define glue3(x, y, z) x ## y ## z
#define glue_var(x, ...) x ## __VA_ARGS__

#define __muldi3 __NDW(mul, 3 = 50)
#define __NDW(a,b) __ ## a ## di ## b
#define m3 NDW()
#define NDW(x) m3 ## x = 50
#define five 5
#define fifty int fif ## ty

/* Defines a function called glue, returning what it is passed.  */
int glue (glue,) (int x)
{
  return x;
}

int main ()
{
  /* m3 and __muldi3 would sometimes cause an infinite loop.  Ensure
     we only expand fifty once.  */
  fifty = 50, m3, __muldi3;

  /* General glue and macro expanding test.  */
  int five0 = xglue (glue (fi, ve), 0);

  /* Tests only first and last tokens are pasted, and pasting to form
     the != operator.  Should expand to: if (five0 != 50).  */
  if (glue3 (fi, ve0 !,= glue (EMPTY 5, 0)))
    err ("five0 != 50");

  /* Test varags pasting, and pasting to form the >> operator.  */
  if (glue_var(50 >, > 1 != 25))
    err ("Operator >> pasting");

  /* The LHS should not attempt to expand twice, and thus becomes a
     call to the function glue.  */
  if (glue (gl, ue) (12) != 12)
    err ("Recursive macros");

  /* Test placemarker pasting.  The glued lines should all appear
     neatly in the same column and below each other, though we don't
     test that here.  */
  {
    int glue3(a, b, ) = 1, glue3(a,,) = 1;
    glue3(a, , b)++;
    glue3(, a, b)++;
    glue3(,a,)++;
    glue3(,,a)++;
    if (a != 3 || ab != 3 glue3(,,))
      err ("Placemarker pasting");
  }

  /* Test that macros in arguments are not expanded.  */
  {
    int glue (EMPTY,1) = 123, glue (T, EMPTY) = 123;
    if (EMPTY1 != 123 || TEMPTY != 123)
      err ("Pasted arguments macro expanding");
  }

  /* Test various paste combinations.  */
  {
    const wchar_t* wc_array = glue(L, "wide string");
    wchar_t wc = glue(L, 'w');
    const char * hh = xstr(xglue(glue(%, :), glue(%, :)));
    int array glue (<, :) 1 glue (:, >) = glue(<, %) 1 glue(%, >);
    int x = 4;

    if (array[0] != 1)
      err ("Digraph pasting");

    x glue (>>, =) 1;		/* 2 */
    x glue (<<, =) 1;		/* 4 */
    x glue (*, =) 2;		/* 8 */
    x glue (+, =) 100;		/* 108 */
    x glue (-, =) 50;		/* 58 */
    x glue (/, =) 2;		/* 29 */
    x glue (%, =) 20;		/* 9 */
    x glue (&, =) 254;		/* 8 */
    x glue (|, =) 16;		/* 24 */
    x glue (^, =) 18;		/* 10 */
    
    if (x != 10 || 0 glue (>, =) 1 glue (|, |) 1 glue (<, =) 0)
      err ("Various operator pasting");
    if (strcmp (hh, "%:%:"))
      err ("Pasted digraph spelling");
    if ((glue (., 1) glue (!, =) .1))
      err ("Pasted numbers 1");
    /* glue3 here will only work if we paste left-to-right.  If a
       future implementation does not do this, change the test.  */
    if (glue3 (1.0e, +, 1) != 10.0)
      err ("Pasted numbers 2");
  }

  return 0;
}

// SLATE-FILECHECK-STD DEFAULT c99
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
// DEFAULT-NEXT:     type @type[[TYPE_wchar_t:[0-9]+]] wchar_t = i32;
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i32, 12> [storage=static] = code_units<array<i32, 12>>([119, 105, 100, 101, 32, 115, 116, 114, 105, 110, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([37, 58, 37, 58, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([37, 58, 37, 58, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_strcmp:[0-9]+]] @strcmp(%[[VALUE0:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_glue:[0-9]+]] @glue(%[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(unprototyped) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_fifty:[0-9]+]] fifty: i32 [storage=automatic] = const<i32>(50);
// DEFAULT-NEXT:         let %[[VALUE_m3:[0-9]+]] m3: i32 [storage=automatic] = const<i32>(50);
// DEFAULT-NEXT:         let %[[VALUE___muldi3:[0-9]+]] __muldi3: i32 [storage=automatic] = const<i32>(50);
// DEFAULT-NEXT:         let %[[VALUE_five0:[0-9]+]] five0: i32 [storage=automatic] = const<i32>(50);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_five0]]), const<i32>(50))
// DEFAULT-NEXT:             do %[[VALUE2:[0-9]+]]
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     ;
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(const<i32>(50), const<i32>(1)), const<i32>(25))
// DEFAULT-NEXT:             do %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     ;
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_glue]], const<i32>(12)), const<i32>(12))
// DEFAULT-NEXT:             do %[[VALUE4:[0-9]+]]
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     ;
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_ab:[0-9]+]] ab: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:             let %[[VALUE_a:[0-9]+]] a: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:             let %[[VALUE5:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_ab]]);
// DEFAULT-NEXT:             let %[[VALUE6:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE5]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_ab]], read<i32>(%[[VALUE6]]));
// DEFAULT-NEXT:             let %[[VALUE7:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_ab]]);
// DEFAULT-NEXT:             let %[[VALUE8:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE7]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_ab]], read<i32>(%[[VALUE8]]));
// DEFAULT-NEXT:             let %[[VALUE9:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// DEFAULT-NEXT:             let %[[VALUE10:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE9]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE10]]));
// DEFAULT-NEXT:             let %[[VALUE11:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// DEFAULT-NEXT:             let %[[VALUE12:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE11]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE12]]));
// DEFAULT-NEXT:             if logical_or<bool>(ne<i32>(read<i32>(%[[VALUE_a]]), const<i32>(3)), ne<i32>(read<i32>(%[[VALUE_ab]]), const<i32>(3)))
// DEFAULT-NEXT:                 do %[[VALUE13:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         ;
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_EMPTY1:[0-9]+]] EMPTY1: i32 [storage=automatic] = const<i32>(123);
// DEFAULT-NEXT:             let %[[VALUE_TEMPTY:[0-9]+]] TEMPTY: i32 [storage=automatic] = const<i32>(123);
// DEFAULT-NEXT:             if logical_or<bool>(ne<i32>(read<i32>(%[[VALUE_EMPTY1]]), const<i32>(123)), ne<i32>(read<i32>(%[[VALUE_TEMPTY]]), const<i32>(123)))
// DEFAULT-NEXT:                 do %[[VALUE14:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         ;
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_wc_array:[0-9]+]] wc_array: ptr<const i32> [storage=automatic] = pointer_cast<ptr<const i32>, reason=assign>(array_decay<ptr<i32>, length=Some(12)>(%[[VALUE_str]]));
// DEFAULT-NEXT:             let %[[VALUE_wc:[0-9]+]] wc: i32 [storage=automatic] = const<i32>(119);
// DEFAULT-NEXT:             let %[[VALUE_hh:[0-9]+]] hh: ptr<const i8> [storage=automatic] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_2]]));
// DEFAULT-NEXT:             let %[[VALUE_array:[0-9]+]] array: array<i32, 1> [storage=automatic] = aggregate<array<i32, 1>, zero_fill=false>(index0 = const<i32>(1));
// DEFAULT-NEXT:             let %[[VALUE_x_2:[0-9]+]] x: i32 [storage=automatic] = const<i32>(4);
// DEFAULT-NEXT:             if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_array]]), const<i32>(0)))), const<i32>(1))
// DEFAULT-NEXT:                 do %[[VALUE15:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         ;
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             let %[[VALUE16:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x_2]]);
// DEFAULT-NEXT:             let %[[VALUE17:[0-9]+]]: i32 [synthetic] = shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%[[VALUE16]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_x_2]], read<i32>(%[[VALUE17]]));
// DEFAULT-NEXT:             let %[[VALUE18:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x_2]]);
// DEFAULT-NEXT:             let %[[VALUE19:[0-9]+]]: i32 [synthetic] = shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i32>(%[[VALUE18]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_x_2]], read<i32>(%[[VALUE19]]));
// DEFAULT-NEXT:             let %[[VALUE20:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x_2]]);
// DEFAULT-NEXT:             let %[[VALUE21:[0-9]+]]: i32 [synthetic] = mul<i32, overflow=ub>(read<i32>(%[[VALUE20]]), const<i32>(2));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_x_2]], read<i32>(%[[VALUE21]]));
// DEFAULT-NEXT:             let %[[VALUE22:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x_2]]);
// DEFAULT-NEXT:             let %[[VALUE23:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE22]]), const<i32>(100));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_x_2]], read<i32>(%[[VALUE23]]));
// DEFAULT-NEXT:             let %[[VALUE24:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x_2]]);
// DEFAULT-NEXT:             let %[[VALUE25:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE24]]), const<i32>(50));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_x_2]], read<i32>(%[[VALUE25]]));
// DEFAULT-NEXT:             let %[[VALUE26:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x_2]]);
// DEFAULT-NEXT:             let %[[VALUE27:[0-9]+]]: i32 [synthetic] = div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE26]]), const<i32>(2));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_x_2]], read<i32>(%[[VALUE27]]));
// DEFAULT-NEXT:             let %[[VALUE28:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x_2]]);
// DEFAULT-NEXT:             let %[[VALUE29:[0-9]+]]: i32 [synthetic] = rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE28]]), const<i32>(20));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_x_2]], read<i32>(%[[VALUE29]]));
// DEFAULT-NEXT:             let %[[VALUE30:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x_2]]);
// DEFAULT-NEXT:             let %[[VALUE31:[0-9]+]]: i32 [synthetic] = and<i32>(read<i32>(%[[VALUE30]]), const<i32>(254));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_x_2]], read<i32>(%[[VALUE31]]));
// DEFAULT-NEXT:             let %[[VALUE32:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x_2]]);
// DEFAULT-NEXT:             let %[[VALUE33:[0-9]+]]: i32 [synthetic] = or<i32>(read<i32>(%[[VALUE32]]), const<i32>(16));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_x_2]], read<i32>(%[[VALUE33]]));
// DEFAULT-NEXT:             let %[[VALUE34:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x_2]]);
// DEFAULT-NEXT:             let %[[VALUE35:[0-9]+]]: i32 [synthetic] = xor<i32>(read<i32>(%[[VALUE34]]), const<i32>(18));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_x_2]], read<i32>(%[[VALUE35]]));
// DEFAULT-NEXT:             if logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(%[[VALUE_x_2]]), const<i32>(10)), ge<i32>(const<i32>(0), const<i32>(1))), le<i32>(const<i32>(1), const<i32>(0)))
// DEFAULT-NEXT:                 do %[[VALUE36:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         ;
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE_strcmp]], read<ptr<const i8>>(%[[VALUE_hh]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_3]]))), const<i32>(0))
// DEFAULT-NEXT:                 do %[[VALUE37:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         ;
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             if ne<f64, exceptions=observable>(const<f64>(0.1), const<f64>(0.1))
// DEFAULT-NEXT:                 do %[[VALUE38:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         ;
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             if ne<f64, exceptions=observable>(const<f64>(10.0), const<f64>(10.0))
// DEFAULT-NEXT:                 do %[[VALUE39:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         ;
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
