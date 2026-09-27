/* Test for types of conditional expressions.  */
/* Origin: Joseph Myers <jsm28@cam.ac.uk> */
/* { dg-do compile } */
/* { dg-options "-std=iso9899:1999 -pedantic-errors" } */

/* Notes:

   (a) The rules are the same in both C standard versions, but C99 also
   gives us the "restrict" qualifier to play with.

   (b) Within the C standard, the value of a conditional expression can't
   have qualified type - but nor can this be detected.  Because of GCC's
   extended lvalues, the value may in GCC have qualified type if the
   arguments do.  So don't use the following macro with arguments of
   qualified type.

*/

/* Assertion that the type of a conditional expression between E1 and E2
   is T.  Checks the expression both ways round.  */
#define ASSERT_COND_TYPE(E1, E2, T)			\
	do {						\
	  typedef T type;				\
	  typedef type **typepp;			\
	  typedef __typeof(0 ? (E1) : (E2)) ctype;	\
	  typedef __typeof(0 ? (E2) : (E1)) ctype2;	\
	  typedef ctype **ctypepp;			\
	  typedef ctype2 **ctype2pp;			\
	  typepp x = 0;					\
	  ctypepp y = 0;				\
	  ctype2pp z = 0;				\
	  x = y;					\
	  x = z;					\
	} while (0)

void
foo (void)
{
  const void *c_vp;
  void *vp;
  const int *c_ip;
  volatile int *v_ip;
  int *ip;
  const char *c_cp;
  int *restrict *r_ipp;
  typedef void (*fpt)(void);
  fpt fp;
  signed char sc;
  struct s { int p; } st;
  union u { int p; } un;
  /* Arithmetic type.  */
  ASSERT_COND_TYPE (sc, sc, int);
  /* Structure and union.  */
  ASSERT_COND_TYPE (st, st, struct s);
  ASSERT_COND_TYPE (un, un, union u);
  /* Void.  */
  ASSERT_COND_TYPE ((void)0, (void)1, void);
  /* Pointers: examples from 6.5.15 paragraph 8.  */
  ASSERT_COND_TYPE (c_vp, c_ip, const void *);
  ASSERT_COND_TYPE (v_ip, 0, volatile int *);
  ASSERT_COND_TYPE (c_ip, v_ip, const volatile int *);
  ASSERT_COND_TYPE (vp, c_cp, const void *);
  ASSERT_COND_TYPE (ip, c_ip, const int *);
  ASSERT_COND_TYPE (vp, ip, void *);
  /* Null pointer constants.  */
  ASSERT_COND_TYPE (v_ip, (void *)0, volatile int *);
  ASSERT_COND_TYPE (r_ipp, (void *)0, int *restrict *);
  ASSERT_COND_TYPE (fp, 0, fpt);
  ASSERT_COND_TYPE (fp, (void *)0, fpt);
}

// SLATE-FILECHECK-FLAVOR gcc
// SLATE-FILECHECK-STD DEFAULT iso9899:1999
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
// DEFAULT-NEXT:     type @type1 s = struct {
// DEFAULT-NEXT:         field0 p: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type2 u = union {
// DEFAULT-NEXT:         field0 p: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type3 type = i32;
// DEFAULT-NEXT:     type @type4 typepp = ptr<ptr<i32>>;
// DEFAULT-NEXT:     type @type5 ctype = i32;
// DEFAULT-NEXT:     type @type6 ctype2 = i32;
// DEFAULT-NEXT:     type @type7 ctypepp = ptr<ptr<i32>>;
// DEFAULT-NEXT:     type @type8 ctype2pp = ptr<ptr<i32>>;
// DEFAULT-NEXT:     type @type9 type = @type1;
// DEFAULT-NEXT:     type @type10 typepp = ptr<ptr<@type1>>;
// DEFAULT-NEXT:     type @type11 ctype = @type1;
// DEFAULT-NEXT:     type @type12 ctype2 = @type1;
// DEFAULT-NEXT:     type @type13 ctypepp = ptr<ptr<@type1>>;
// DEFAULT-NEXT:     type @type14 ctype2pp = ptr<ptr<@type1>>;
// DEFAULT-NEXT:     type @type15 type = @type2;
// DEFAULT-NEXT:     type @type16 typepp = ptr<ptr<@type2>>;
// DEFAULT-NEXT:     type @type17 ctype = @type2;
// DEFAULT-NEXT:     type @type18 ctype2 = @type2;
// DEFAULT-NEXT:     type @type19 ctypepp = ptr<ptr<@type2>>;
// DEFAULT-NEXT:     type @type20 ctype2pp = ptr<ptr<@type2>>;
// DEFAULT-NEXT:     type @type21 type = void;
// DEFAULT-NEXT:     type @type22 typepp = ptr<ptr<void>>;
// DEFAULT-NEXT:     type @type23 ctype = void;
// DEFAULT-NEXT:     type @type24 ctype2 = void;
// DEFAULT-NEXT:     type @type25 ctypepp = ptr<ptr<void>>;
// DEFAULT-NEXT:     type @type26 ctype2pp = ptr<ptr<void>>;
// DEFAULT-NEXT:     type @type27 type = ptr<const void>;
// DEFAULT-NEXT:     type @type28 typepp = ptr<ptr<ptr<const void>>>;
// DEFAULT-NEXT:     type @type29 ctype = ptr<const void>;
// DEFAULT-NEXT:     type @type30 ctype2 = ptr<const void>;
// DEFAULT-NEXT:     type @type31 ctypepp = ptr<ptr<ptr<const void>>>;
// DEFAULT-NEXT:     type @type32 ctype2pp = ptr<ptr<ptr<const void>>>;
// DEFAULT-NEXT:     type @type33 type = ptr<volatile i32>;
// DEFAULT-NEXT:     type @type34 typepp = ptr<ptr<ptr<volatile i32>>>;
// DEFAULT-NEXT:     type @type35 ctype = ptr<volatile i32>;
// DEFAULT-NEXT:     type @type36 ctype2 = ptr<volatile i32>;
// DEFAULT-NEXT:     type @type37 ctypepp = ptr<ptr<ptr<volatile i32>>>;
// DEFAULT-NEXT:     type @type38 ctype2pp = ptr<ptr<ptr<volatile i32>>>;
// DEFAULT-NEXT:     type @type39 type = ptr<const volatile i32>;
// DEFAULT-NEXT:     type @type40 typepp = ptr<ptr<ptr<const volatile i32>>>;
// DEFAULT-NEXT:     type @type41 ctype = ptr<const volatile i32>;
// DEFAULT-NEXT:     type @type42 ctype2 = ptr<const volatile i32>;
// DEFAULT-NEXT:     type @type43 ctypepp = ptr<ptr<ptr<const volatile i32>>>;
// DEFAULT-NEXT:     type @type44 ctype2pp = ptr<ptr<ptr<const volatile i32>>>;
// DEFAULT-NEXT:     type @type45 type = ptr<const void>;
// DEFAULT-NEXT:     type @type46 typepp = ptr<ptr<ptr<const void>>>;
// DEFAULT-NEXT:     type @type47 ctype = ptr<const void>;
// DEFAULT-NEXT:     type @type48 ctype2 = ptr<const void>;
// DEFAULT-NEXT:     type @type49 ctypepp = ptr<ptr<ptr<const void>>>;
// DEFAULT-NEXT:     type @type50 ctype2pp = ptr<ptr<ptr<const void>>>;
// DEFAULT-NEXT:     type @type51 type = ptr<const i32>;
// DEFAULT-NEXT:     type @type52 typepp = ptr<ptr<ptr<const i32>>>;
// DEFAULT-NEXT:     type @type53 ctype = ptr<const i32>;
// DEFAULT-NEXT:     type @type54 ctype2 = ptr<const i32>;
// DEFAULT-NEXT:     type @type55 ctypepp = ptr<ptr<ptr<const i32>>>;
// DEFAULT-NEXT:     type @type56 ctype2pp = ptr<ptr<ptr<const i32>>>;
// DEFAULT-NEXT:     type @type57 type = ptr<void>;
// DEFAULT-NEXT:     type @type58 typepp = ptr<ptr<ptr<void>>>;
// DEFAULT-NEXT:     type @type59 ctype = ptr<void>;
// DEFAULT-NEXT:     type @type60 ctype2 = ptr<void>;
// DEFAULT-NEXT:     type @type61 ctypepp = ptr<ptr<ptr<void>>>;
// DEFAULT-NEXT:     type @type62 ctype2pp = ptr<ptr<ptr<void>>>;
// DEFAULT-NEXT:     type @type63 type = ptr<volatile i32>;
// DEFAULT-NEXT:     type @type64 typepp = ptr<ptr<ptr<volatile i32>>>;
// DEFAULT-NEXT:     type @type65 ctype = ptr<volatile i32>;
// DEFAULT-NEXT:     type @type66 ctype2 = ptr<volatile i32>;
// DEFAULT-NEXT:     type @type67 ctypepp = ptr<ptr<ptr<volatile i32>>>;
// DEFAULT-NEXT:     type @type68 ctype2pp = ptr<ptr<ptr<volatile i32>>>;
// DEFAULT-NEXT:     type @type69 type = ptr<ptr<i32>>;
// DEFAULT-NEXT:     type @type70 typepp = ptr<ptr<ptr<ptr<i32>>>>;
// DEFAULT-NEXT:     type @type71 ctype = ptr<ptr<i32>>;
// DEFAULT-NEXT:     type @type72 ctype2 = ptr<ptr<i32>>;
// DEFAULT-NEXT:     type @type73 ctypepp = ptr<ptr<ptr<ptr<i32>>>>;
// DEFAULT-NEXT:     type @type74 ctype2pp = ptr<ptr<ptr<ptr<i32>>>>;
// DEFAULT-NEXT:     type @type75 type = ptr<fn() -> void>;
// DEFAULT-NEXT:     type @type76 typepp = ptr<ptr<ptr<fn() -> void>>>;
// DEFAULT-NEXT:     type @type77 ctype = ptr<fn() -> void>;
// DEFAULT-NEXT:     type @type78 ctype2 = ptr<fn() -> void>;
// DEFAULT-NEXT:     type @type79 ctypepp = ptr<ptr<ptr<fn() -> void>>>;
// DEFAULT-NEXT:     type @type80 ctype2pp = ptr<ptr<ptr<fn() -> void>>>;
// DEFAULT-NEXT:     type @type81 type = ptr<fn() -> void>;
// DEFAULT-NEXT:     type @type82 typepp = ptr<ptr<ptr<fn() -> void>>>;
// DEFAULT-NEXT:     type @type83 ctype = ptr<fn() -> void>;
// DEFAULT-NEXT:     type @type84 ctype2 = ptr<fn() -> void>;
// DEFAULT-NEXT:     type @type85 ctypepp = ptr<ptr<ptr<fn() -> void>>>;
// DEFAULT-NEXT:     type @type86 ctype2pp = ptr<ptr<ptr<fn() -> void>>>;
// DEFAULT-NEXT:     fn %0 @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %1 c_vp: ptr<const void> [storage=automatic];
// DEFAULT-NEXT:         let %2 vp: ptr<void> [storage=automatic];
// DEFAULT-NEXT:         let %3 c_ip: ptr<const i32> [storage=automatic];
// DEFAULT-NEXT:         let %4 v_ip: ptr<volatile i32> [storage=automatic];
// DEFAULT-NEXT:         let %5 ip: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         let %6 c_cp: ptr<const i8> [storage=automatic];
// DEFAULT-NEXT:         let %7 r_ipp: ptr<ptr<i32>> [storage=automatic];
// DEFAULT-NEXT:         let %9 fp: ptr<fn() -> void> [storage=automatic];
// DEFAULT-NEXT:         let %10 sc: i8 [storage=automatic];
// DEFAULT-NEXT:         let %12 st: @type1 [storage=automatic];
// DEFAULT-NEXT:         let %14 un: @type2 [storage=automatic];
// DEFAULT-NEXT:         do %141
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %21 x: ptr<ptr<i32>> [storage=automatic] = null<ptr<ptr<i32>>>;
// DEFAULT-NEXT:                 let %22 y: ptr<ptr<i32>> [storage=automatic] = null<ptr<ptr<i32>>>;
// DEFAULT-NEXT:                 let %23 z: ptr<ptr<i32>> [storage=automatic] = null<ptr<ptr<i32>>>;
// DEFAULT-NEXT:                 write<ptr<ptr<i32>>>(%21, read<ptr<ptr<i32>>>(%22));
// DEFAULT-NEXT:                 write<ptr<ptr<i32>>>(%21, read<ptr<ptr<i32>>>(%23));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %142
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %30 x: ptr<ptr<@type1>> [storage=automatic] = null<ptr<ptr<@type1>>>;
// DEFAULT-NEXT:                 let %31 y: ptr<ptr<@type1>> [storage=automatic] = null<ptr<ptr<@type1>>>;
// DEFAULT-NEXT:                 let %32 z: ptr<ptr<@type1>> [storage=automatic] = null<ptr<ptr<@type1>>>;
// DEFAULT-NEXT:                 write<ptr<ptr<@type1>>>(%30, read<ptr<ptr<@type1>>>(%31));
// DEFAULT-NEXT:                 write<ptr<ptr<@type1>>>(%30, read<ptr<ptr<@type1>>>(%32));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %143
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %39 x: ptr<ptr<@type2>> [storage=automatic] = null<ptr<ptr<@type2>>>;
// DEFAULT-NEXT:                 let %40 y: ptr<ptr<@type2>> [storage=automatic] = null<ptr<ptr<@type2>>>;
// DEFAULT-NEXT:                 let %41 z: ptr<ptr<@type2>> [storage=automatic] = null<ptr<ptr<@type2>>>;
// DEFAULT-NEXT:                 write<ptr<ptr<@type2>>>(%39, read<ptr<ptr<@type2>>>(%40));
// DEFAULT-NEXT:                 write<ptr<ptr<@type2>>>(%39, read<ptr<ptr<@type2>>>(%41));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %144
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %48 x: ptr<ptr<void>> [storage=automatic] = null<ptr<ptr<void>>>;
// DEFAULT-NEXT:                 let %49 y: ptr<ptr<void>> [storage=automatic] = null<ptr<ptr<void>>>;
// DEFAULT-NEXT:                 let %50 z: ptr<ptr<void>> [storage=automatic] = null<ptr<ptr<void>>>;
// DEFAULT-NEXT:                 write<ptr<ptr<void>>>(%48, read<ptr<ptr<void>>>(%49));
// DEFAULT-NEXT:                 write<ptr<ptr<void>>>(%48, read<ptr<ptr<void>>>(%50));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %145
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %57 x: ptr<ptr<ptr<const void>>> [storage=automatic] = null<ptr<ptr<ptr<const void>>>>;
// DEFAULT-NEXT:                 let %58 y: ptr<ptr<ptr<const void>>> [storage=automatic] = null<ptr<ptr<ptr<const void>>>>;
// DEFAULT-NEXT:                 let %59 z: ptr<ptr<ptr<const void>>> [storage=automatic] = null<ptr<ptr<ptr<const void>>>>;
// DEFAULT-NEXT:                 write<ptr<ptr<ptr<const void>>>>(%57, read<ptr<ptr<ptr<const void>>>>(%58));
// DEFAULT-NEXT:                 write<ptr<ptr<ptr<const void>>>>(%57, read<ptr<ptr<ptr<const void>>>>(%59));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %146
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %66 x: ptr<ptr<ptr<volatile i32>>> [storage=automatic] = null<ptr<ptr<ptr<volatile i32>>>>;
// DEFAULT-NEXT:                 let %67 y: ptr<ptr<ptr<volatile i32>>> [storage=automatic] = null<ptr<ptr<ptr<volatile i32>>>>;
// DEFAULT-NEXT:                 let %68 z: ptr<ptr<ptr<volatile i32>>> [storage=automatic] = null<ptr<ptr<ptr<volatile i32>>>>;
// DEFAULT-NEXT:                 write<ptr<ptr<ptr<volatile i32>>>>(%66, read<ptr<ptr<ptr<volatile i32>>>>(%67));
// DEFAULT-NEXT:                 write<ptr<ptr<ptr<volatile i32>>>>(%66, read<ptr<ptr<ptr<volatile i32>>>>(%68));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %147
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %75 x: ptr<ptr<ptr<const volatile i32>>> [storage=automatic] = null<ptr<ptr<ptr<const volatile i32>>>>;
// DEFAULT-NEXT:                 let %76 y: ptr<ptr<ptr<const volatile i32>>> [storage=automatic] = null<ptr<ptr<ptr<const volatile i32>>>>;
// DEFAULT-NEXT:                 let %77 z: ptr<ptr<ptr<const volatile i32>>> [storage=automatic] = null<ptr<ptr<ptr<const volatile i32>>>>;
// DEFAULT-NEXT:                 write<ptr<ptr<ptr<const volatile i32>>>>(%75, read<ptr<ptr<ptr<const volatile i32>>>>(%76));
// DEFAULT-NEXT:                 write<ptr<ptr<ptr<const volatile i32>>>>(%75, read<ptr<ptr<ptr<const volatile i32>>>>(%77));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %148
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %84 x: ptr<ptr<ptr<const void>>> [storage=automatic] = null<ptr<ptr<ptr<const void>>>>;
// DEFAULT-NEXT:                 let %85 y: ptr<ptr<ptr<const void>>> [storage=automatic] = null<ptr<ptr<ptr<const void>>>>;
// DEFAULT-NEXT:                 let %86 z: ptr<ptr<ptr<const void>>> [storage=automatic] = null<ptr<ptr<ptr<const void>>>>;
// DEFAULT-NEXT:                 write<ptr<ptr<ptr<const void>>>>(%84, read<ptr<ptr<ptr<const void>>>>(%85));
// DEFAULT-NEXT:                 write<ptr<ptr<ptr<const void>>>>(%84, read<ptr<ptr<ptr<const void>>>>(%86));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %149
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %93 x: ptr<ptr<ptr<const i32>>> [storage=automatic] = null<ptr<ptr<ptr<const i32>>>>;
// DEFAULT-NEXT:                 let %94 y: ptr<ptr<ptr<const i32>>> [storage=automatic] = null<ptr<ptr<ptr<const i32>>>>;
// DEFAULT-NEXT:                 let %95 z: ptr<ptr<ptr<const i32>>> [storage=automatic] = null<ptr<ptr<ptr<const i32>>>>;
// DEFAULT-NEXT:                 write<ptr<ptr<ptr<const i32>>>>(%93, read<ptr<ptr<ptr<const i32>>>>(%94));
// DEFAULT-NEXT:                 write<ptr<ptr<ptr<const i32>>>>(%93, read<ptr<ptr<ptr<const i32>>>>(%95));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %150
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %102 x: ptr<ptr<ptr<void>>> [storage=automatic] = null<ptr<ptr<ptr<void>>>>;
// DEFAULT-NEXT:                 let %103 y: ptr<ptr<ptr<void>>> [storage=automatic] = null<ptr<ptr<ptr<void>>>>;
// DEFAULT-NEXT:                 let %104 z: ptr<ptr<ptr<void>>> [storage=automatic] = null<ptr<ptr<ptr<void>>>>;
// DEFAULT-NEXT:                 write<ptr<ptr<ptr<void>>>>(%102, read<ptr<ptr<ptr<void>>>>(%103));
// DEFAULT-NEXT:                 write<ptr<ptr<ptr<void>>>>(%102, read<ptr<ptr<ptr<void>>>>(%104));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %151
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %111 x: ptr<ptr<ptr<volatile i32>>> [storage=automatic] = null<ptr<ptr<ptr<volatile i32>>>>;
// DEFAULT-NEXT:                 let %112 y: ptr<ptr<ptr<volatile i32>>> [storage=automatic] = null<ptr<ptr<ptr<volatile i32>>>>;
// DEFAULT-NEXT:                 let %113 z: ptr<ptr<ptr<volatile i32>>> [storage=automatic] = null<ptr<ptr<ptr<volatile i32>>>>;
// DEFAULT-NEXT:                 write<ptr<ptr<ptr<volatile i32>>>>(%111, read<ptr<ptr<ptr<volatile i32>>>>(%112));
// DEFAULT-NEXT:                 write<ptr<ptr<ptr<volatile i32>>>>(%111, read<ptr<ptr<ptr<volatile i32>>>>(%113));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %152
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %120 x: ptr<ptr<ptr<ptr<i32>>>> [storage=automatic] = null<ptr<ptr<ptr<ptr<i32>>>>>;
// DEFAULT-NEXT:                 let %121 y: ptr<ptr<ptr<ptr<i32>>>> [storage=automatic] = null<ptr<ptr<ptr<ptr<i32>>>>>;
// DEFAULT-NEXT:                 let %122 z: ptr<ptr<ptr<ptr<i32>>>> [storage=automatic] = null<ptr<ptr<ptr<ptr<i32>>>>>;
// DEFAULT-NEXT:                 write<ptr<ptr<ptr<ptr<i32>>>>>(%120, read<ptr<ptr<ptr<ptr<i32>>>>>(%121));
// DEFAULT-NEXT:                 write<ptr<ptr<ptr<ptr<i32>>>>>(%120, read<ptr<ptr<ptr<ptr<i32>>>>>(%122));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %153
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %129 x: ptr<ptr<ptr<fn() -> void>>> [storage=automatic] = null<ptr<ptr<ptr<fn() -> void>>>>;
// DEFAULT-NEXT:                 let %130 y: ptr<ptr<ptr<fn() -> void>>> [storage=automatic] = null<ptr<ptr<ptr<fn() -> void>>>>;
// DEFAULT-NEXT:                 let %131 z: ptr<ptr<ptr<fn() -> void>>> [storage=automatic] = null<ptr<ptr<ptr<fn() -> void>>>>;
// DEFAULT-NEXT:                 write<ptr<ptr<ptr<fn() -> void>>>>(%129, read<ptr<ptr<ptr<fn() -> void>>>>(%130));
// DEFAULT-NEXT:                 write<ptr<ptr<ptr<fn() -> void>>>>(%129, read<ptr<ptr<ptr<fn() -> void>>>>(%131));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %154
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %138 x: ptr<ptr<ptr<fn() -> void>>> [storage=automatic] = null<ptr<ptr<ptr<fn() -> void>>>>;
// DEFAULT-NEXT:                 let %139 y: ptr<ptr<ptr<fn() -> void>>> [storage=automatic] = null<ptr<ptr<ptr<fn() -> void>>>>;
// DEFAULT-NEXT:                 let %140 z: ptr<ptr<ptr<fn() -> void>>> [storage=automatic] = null<ptr<ptr<ptr<fn() -> void>>>>;
// DEFAULT-NEXT:                 write<ptr<ptr<ptr<fn() -> void>>>>(%138, read<ptr<ptr<ptr<fn() -> void>>>>(%139));
// DEFAULT-NEXT:                 write<ptr<ptr<ptr<fn() -> void>>>>(%138, read<ptr<ptr<ptr<fn() -> void>>>>(%140));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
