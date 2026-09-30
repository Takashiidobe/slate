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
// DEFAULT-NEXT:     type @type[[TYPE_fpt:[0-9]+]] fpt = ptr<fn() -> void>;
// DEFAULT-NEXT:     type @type[[TYPE_s:[0-9]+]] s = struct {
// DEFAULT-NEXT:         field0 p: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_u:[0-9]+]] u = union {
// DEFAULT-NEXT:         field0 p: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_type:[0-9]+]] type = i32;
// DEFAULT-NEXT:     type @type[[TYPE_typepp:[0-9]+]] typepp = ptr<ptr<i32>>;
// DEFAULT-NEXT:     type @type[[TYPE_ctype:[0-9]+]] ctype = i32;
// DEFAULT-NEXT:     type @type[[TYPE_ctype2:[0-9]+]] ctype2 = i32;
// DEFAULT-NEXT:     type @type[[TYPE_ctypepp:[0-9]+]] ctypepp = ptr<ptr<i32>>;
// DEFAULT-NEXT:     type @type[[TYPE_ctype2pp:[0-9]+]] ctype2pp = ptr<ptr<i32>>;
// DEFAULT-NEXT:     type @type[[TYPE_type_2:[0-9]+]] type = @type[[TYPE_s]];
// DEFAULT-NEXT:     type @type[[TYPE_typepp_2:[0-9]+]] typepp = ptr<ptr<@type[[TYPE_s]]>>;
// DEFAULT-NEXT:     type @type[[TYPE_ctype_2:[0-9]+]] ctype = @type[[TYPE_s]];
// DEFAULT-NEXT:     type @type[[TYPE_ctype2_2:[0-9]+]] ctype2 = @type[[TYPE_s]];
// DEFAULT-NEXT:     type @type[[TYPE_ctypepp_2:[0-9]+]] ctypepp = ptr<ptr<@type[[TYPE_s]]>>;
// DEFAULT-NEXT:     type @type[[TYPE_ctype2pp_2:[0-9]+]] ctype2pp = ptr<ptr<@type[[TYPE_s]]>>;
// DEFAULT-NEXT:     type @type[[TYPE_type_3:[0-9]+]] type = @type[[TYPE_u]];
// DEFAULT-NEXT:     type @type[[TYPE_typepp_3:[0-9]+]] typepp = ptr<ptr<@type[[TYPE_u]]>>;
// DEFAULT-NEXT:     type @type[[TYPE_ctype_3:[0-9]+]] ctype = @type[[TYPE_u]];
// DEFAULT-NEXT:     type @type[[TYPE_ctype2_3:[0-9]+]] ctype2 = @type[[TYPE_u]];
// DEFAULT-NEXT:     type @type[[TYPE_ctypepp_3:[0-9]+]] ctypepp = ptr<ptr<@type[[TYPE_u]]>>;
// DEFAULT-NEXT:     type @type[[TYPE_ctype2pp_3:[0-9]+]] ctype2pp = ptr<ptr<@type[[TYPE_u]]>>;
// DEFAULT-NEXT:     type @type[[TYPE_type_4:[0-9]+]] type = void;
// DEFAULT-NEXT:     type @type[[TYPE_typepp_4:[0-9]+]] typepp = ptr<ptr<void>>;
// DEFAULT-NEXT:     type @type[[TYPE_ctype_4:[0-9]+]] ctype = void;
// DEFAULT-NEXT:     type @type[[TYPE_ctype2_4:[0-9]+]] ctype2 = void;
// DEFAULT-NEXT:     type @type[[TYPE_ctypepp_4:[0-9]+]] ctypepp = ptr<ptr<void>>;
// DEFAULT-NEXT:     type @type[[TYPE_ctype2pp_4:[0-9]+]] ctype2pp = ptr<ptr<void>>;
// DEFAULT-NEXT:     type @type[[TYPE_type_5:[0-9]+]] type = ptr<const void>;
// DEFAULT-NEXT:     type @type[[TYPE_typepp_5:[0-9]+]] typepp = ptr<ptr<ptr<const void>>>;
// DEFAULT-NEXT:     type @type[[TYPE_ctype_5:[0-9]+]] ctype = ptr<const void>;
// DEFAULT-NEXT:     type @type[[TYPE_ctype2_5:[0-9]+]] ctype2 = ptr<const void>;
// DEFAULT-NEXT:     type @type[[TYPE_ctypepp_5:[0-9]+]] ctypepp = ptr<ptr<ptr<const void>>>;
// DEFAULT-NEXT:     type @type[[TYPE_ctype2pp_5:[0-9]+]] ctype2pp = ptr<ptr<ptr<const void>>>;
// DEFAULT-NEXT:     type @type[[TYPE_type_6:[0-9]+]] type = ptr<volatile i32>;
// DEFAULT-NEXT:     type @type[[TYPE_typepp_6:[0-9]+]] typepp = ptr<ptr<ptr<volatile i32>>>;
// DEFAULT-NEXT:     type @type[[TYPE_ctype_6:[0-9]+]] ctype = ptr<volatile i32>;
// DEFAULT-NEXT:     type @type[[TYPE_ctype2_6:[0-9]+]] ctype2 = ptr<volatile i32>;
// DEFAULT-NEXT:     type @type[[TYPE_ctypepp_6:[0-9]+]] ctypepp = ptr<ptr<ptr<volatile i32>>>;
// DEFAULT-NEXT:     type @type[[TYPE_ctype2pp_6:[0-9]+]] ctype2pp = ptr<ptr<ptr<volatile i32>>>;
// DEFAULT-NEXT:     type @type[[TYPE_type_7:[0-9]+]] type = ptr<const volatile i32>;
// DEFAULT-NEXT:     type @type[[TYPE_typepp_7:[0-9]+]] typepp = ptr<ptr<ptr<const volatile i32>>>;
// DEFAULT-NEXT:     type @type[[TYPE_ctype_7:[0-9]+]] ctype = ptr<const volatile i32>;
// DEFAULT-NEXT:     type @type[[TYPE_ctype2_7:[0-9]+]] ctype2 = ptr<const volatile i32>;
// DEFAULT-NEXT:     type @type[[TYPE_ctypepp_7:[0-9]+]] ctypepp = ptr<ptr<ptr<const volatile i32>>>;
// DEFAULT-NEXT:     type @type[[TYPE_ctype2pp_7:[0-9]+]] ctype2pp = ptr<ptr<ptr<const volatile i32>>>;
// DEFAULT-NEXT:     type @type[[TYPE_type_8:[0-9]+]] type = ptr<const void>;
// DEFAULT-NEXT:     type @type[[TYPE_typepp_8:[0-9]+]] typepp = ptr<ptr<ptr<const void>>>;
// DEFAULT-NEXT:     type @type[[TYPE_ctype_8:[0-9]+]] ctype = ptr<const void>;
// DEFAULT-NEXT:     type @type[[TYPE_ctype2_8:[0-9]+]] ctype2 = ptr<const void>;
// DEFAULT-NEXT:     type @type[[TYPE_ctypepp_8:[0-9]+]] ctypepp = ptr<ptr<ptr<const void>>>;
// DEFAULT-NEXT:     type @type[[TYPE_ctype2pp_8:[0-9]+]] ctype2pp = ptr<ptr<ptr<const void>>>;
// DEFAULT-NEXT:     type @type[[TYPE_type_9:[0-9]+]] type = ptr<const i32>;
// DEFAULT-NEXT:     type @type[[TYPE_typepp_9:[0-9]+]] typepp = ptr<ptr<ptr<const i32>>>;
// DEFAULT-NEXT:     type @type[[TYPE_ctype_9:[0-9]+]] ctype = ptr<const i32>;
// DEFAULT-NEXT:     type @type[[TYPE_ctype2_9:[0-9]+]] ctype2 = ptr<const i32>;
// DEFAULT-NEXT:     type @type[[TYPE_ctypepp_9:[0-9]+]] ctypepp = ptr<ptr<ptr<const i32>>>;
// DEFAULT-NEXT:     type @type[[TYPE_ctype2pp_9:[0-9]+]] ctype2pp = ptr<ptr<ptr<const i32>>>;
// DEFAULT-NEXT:     type @type[[TYPE_type_10:[0-9]+]] type = ptr<void>;
// DEFAULT-NEXT:     type @type[[TYPE_typepp_10:[0-9]+]] typepp = ptr<ptr<ptr<void>>>;
// DEFAULT-NEXT:     type @type[[TYPE_ctype_10:[0-9]+]] ctype = ptr<void>;
// DEFAULT-NEXT:     type @type[[TYPE_ctype2_10:[0-9]+]] ctype2 = ptr<void>;
// DEFAULT-NEXT:     type @type[[TYPE_ctypepp_10:[0-9]+]] ctypepp = ptr<ptr<ptr<void>>>;
// DEFAULT-NEXT:     type @type[[TYPE_ctype2pp_10:[0-9]+]] ctype2pp = ptr<ptr<ptr<void>>>;
// DEFAULT-NEXT:     type @type[[TYPE_type_11:[0-9]+]] type = ptr<volatile i32>;
// DEFAULT-NEXT:     type @type[[TYPE_typepp_11:[0-9]+]] typepp = ptr<ptr<ptr<volatile i32>>>;
// DEFAULT-NEXT:     type @type[[TYPE_ctype_11:[0-9]+]] ctype = ptr<volatile i32>;
// DEFAULT-NEXT:     type @type[[TYPE_ctype2_11:[0-9]+]] ctype2 = ptr<volatile i32>;
// DEFAULT-NEXT:     type @type[[TYPE_ctypepp_11:[0-9]+]] ctypepp = ptr<ptr<ptr<volatile i32>>>;
// DEFAULT-NEXT:     type @type[[TYPE_ctype2pp_11:[0-9]+]] ctype2pp = ptr<ptr<ptr<volatile i32>>>;
// DEFAULT-NEXT:     type @type[[TYPE_type_12:[0-9]+]] type = ptr<ptr<i32>>;
// DEFAULT-NEXT:     type @type[[TYPE_typepp_12:[0-9]+]] typepp = ptr<ptr<ptr<ptr<i32>>>>;
// DEFAULT-NEXT:     type @type[[TYPE_ctype_12:[0-9]+]] ctype = ptr<ptr<i32>>;
// DEFAULT-NEXT:     type @type[[TYPE_ctype2_12:[0-9]+]] ctype2 = ptr<ptr<i32>>;
// DEFAULT-NEXT:     type @type[[TYPE_ctypepp_12:[0-9]+]] ctypepp = ptr<ptr<ptr<ptr<i32>>>>;
// DEFAULT-NEXT:     type @type[[TYPE_ctype2pp_12:[0-9]+]] ctype2pp = ptr<ptr<ptr<ptr<i32>>>>;
// DEFAULT-NEXT:     type @type[[TYPE_type_13:[0-9]+]] type = ptr<fn() -> void>;
// DEFAULT-NEXT:     type @type[[TYPE_typepp_13:[0-9]+]] typepp = ptr<ptr<ptr<fn() -> void>>>;
// DEFAULT-NEXT:     type @type[[TYPE_ctype_13:[0-9]+]] ctype = ptr<fn() -> void>;
// DEFAULT-NEXT:     type @type[[TYPE_ctype2_13:[0-9]+]] ctype2 = ptr<fn() -> void>;
// DEFAULT-NEXT:     type @type[[TYPE_ctypepp_13:[0-9]+]] ctypepp = ptr<ptr<ptr<fn() -> void>>>;
// DEFAULT-NEXT:     type @type[[TYPE_ctype2pp_13:[0-9]+]] ctype2pp = ptr<ptr<ptr<fn() -> void>>>;
// DEFAULT-NEXT:     type @type[[TYPE_type_14:[0-9]+]] type = ptr<fn() -> void>;
// DEFAULT-NEXT:     type @type[[TYPE_typepp_14:[0-9]+]] typepp = ptr<ptr<ptr<fn() -> void>>>;
// DEFAULT-NEXT:     type @type[[TYPE_ctype_14:[0-9]+]] ctype = ptr<fn() -> void>;
// DEFAULT-NEXT:     type @type[[TYPE_ctype2_14:[0-9]+]] ctype2 = ptr<fn() -> void>;
// DEFAULT-NEXT:     type @type[[TYPE_ctypepp_14:[0-9]+]] ctypepp = ptr<ptr<ptr<fn() -> void>>>;
// DEFAULT-NEXT:     type @type[[TYPE_ctype2pp_14:[0-9]+]] ctype2pp = ptr<ptr<ptr<fn() -> void>>>;
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_c_vp:[0-9]+]] c_vp: ptr<const void> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_vp:[0-9]+]] vp: ptr<void> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_c_ip:[0-9]+]] c_ip: ptr<const i32> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_v_ip:[0-9]+]] v_ip: ptr<volatile i32> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ip:[0-9]+]] ip: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_c_cp:[0-9]+]] c_cp: ptr<const i8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_r_ipp:[0-9]+]] r_ipp: ptr<ptr<i32>> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_fp:[0-9]+]] fp: ptr<fn() -> void> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_sc:[0-9]+]] sc: i8 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_st:[0-9]+]] st: @type[[TYPE_s]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_un:[0-9]+]] un: @type[[TYPE_u]] [storage=automatic];
// DEFAULT-NEXT:         do %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_x:[0-9]+]] x: ptr<ptr<i32>> [storage=automatic] = null<ptr<ptr<i32>>>;
// DEFAULT-NEXT:                 let %[[VALUE_y:[0-9]+]] y: ptr<ptr<i32>> [storage=automatic] = null<ptr<ptr<i32>>>;
// DEFAULT-NEXT:                 let %[[VALUE_z:[0-9]+]] z: ptr<ptr<i32>> [storage=automatic] = null<ptr<ptr<i32>>>;
// DEFAULT-NEXT:                 write<ptr<ptr<i32>>>(%[[VALUE_x]], read<ptr<ptr<i32>>>(%[[VALUE_y]]));
// DEFAULT-NEXT:                 write<ptr<ptr<i32>>>(%[[VALUE_x]], read<ptr<ptr<i32>>>(%[[VALUE_z]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_x_2:[0-9]+]] x: ptr<ptr<@type[[TYPE_s]]>> [storage=automatic] = null<ptr<ptr<@type[[TYPE_s]]>>>;
// DEFAULT-NEXT:                 let %[[VALUE_y_2:[0-9]+]] y: ptr<ptr<@type[[TYPE_s]]>> [storage=automatic] = null<ptr<ptr<@type[[TYPE_s]]>>>;
// DEFAULT-NEXT:                 let %[[VALUE_z_2:[0-9]+]] z: ptr<ptr<@type[[TYPE_s]]>> [storage=automatic] = null<ptr<ptr<@type[[TYPE_s]]>>>;
// DEFAULT-NEXT:                 write<ptr<ptr<@type[[TYPE_s]]>>>(%[[VALUE_x_2]], read<ptr<ptr<@type[[TYPE_s]]>>>(%[[VALUE_y_2]]));
// DEFAULT-NEXT:                 write<ptr<ptr<@type[[TYPE_s]]>>>(%[[VALUE_x_2]], read<ptr<ptr<@type[[TYPE_s]]>>>(%[[VALUE_z_2]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE2:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_x_3:[0-9]+]] x: ptr<ptr<@type[[TYPE_u]]>> [storage=automatic] = null<ptr<ptr<@type[[TYPE_u]]>>>;
// DEFAULT-NEXT:                 let %[[VALUE_y_3:[0-9]+]] y: ptr<ptr<@type[[TYPE_u]]>> [storage=automatic] = null<ptr<ptr<@type[[TYPE_u]]>>>;
// DEFAULT-NEXT:                 let %[[VALUE_z_3:[0-9]+]] z: ptr<ptr<@type[[TYPE_u]]>> [storage=automatic] = null<ptr<ptr<@type[[TYPE_u]]>>>;
// DEFAULT-NEXT:                 write<ptr<ptr<@type[[TYPE_u]]>>>(%[[VALUE_x_3]], read<ptr<ptr<@type[[TYPE_u]]>>>(%[[VALUE_y_3]]));
// DEFAULT-NEXT:                 write<ptr<ptr<@type[[TYPE_u]]>>>(%[[VALUE_x_3]], read<ptr<ptr<@type[[TYPE_u]]>>>(%[[VALUE_z_3]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_x_4:[0-9]+]] x: ptr<ptr<void>> [storage=automatic] = null<ptr<ptr<void>>>;
// DEFAULT-NEXT:                 let %[[VALUE_y_4:[0-9]+]] y: ptr<ptr<void>> [storage=automatic] = null<ptr<ptr<void>>>;
// DEFAULT-NEXT:                 let %[[VALUE_z_4:[0-9]+]] z: ptr<ptr<void>> [storage=automatic] = null<ptr<ptr<void>>>;
// DEFAULT-NEXT:                 write<ptr<ptr<void>>>(%[[VALUE_x_4]], read<ptr<ptr<void>>>(%[[VALUE_y_4]]));
// DEFAULT-NEXT:                 write<ptr<ptr<void>>>(%[[VALUE_x_4]], read<ptr<ptr<void>>>(%[[VALUE_z_4]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE4:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_x_5:[0-9]+]] x: ptr<ptr<ptr<const void>>> [storage=automatic] = null<ptr<ptr<ptr<const void>>>>;
// DEFAULT-NEXT:                 let %[[VALUE_y_5:[0-9]+]] y: ptr<ptr<ptr<const void>>> [storage=automatic] = null<ptr<ptr<ptr<const void>>>>;
// DEFAULT-NEXT:                 let %[[VALUE_z_5:[0-9]+]] z: ptr<ptr<ptr<const void>>> [storage=automatic] = null<ptr<ptr<ptr<const void>>>>;
// DEFAULT-NEXT:                 write<ptr<ptr<ptr<const void>>>>(%[[VALUE_x_5]], read<ptr<ptr<ptr<const void>>>>(%[[VALUE_y_5]]));
// DEFAULT-NEXT:                 write<ptr<ptr<ptr<const void>>>>(%[[VALUE_x_5]], read<ptr<ptr<ptr<const void>>>>(%[[VALUE_z_5]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE5:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_x_6:[0-9]+]] x: ptr<ptr<ptr<volatile i32>>> [storage=automatic] = null<ptr<ptr<ptr<volatile i32>>>>;
// DEFAULT-NEXT:                 let %[[VALUE_y_6:[0-9]+]] y: ptr<ptr<ptr<volatile i32>>> [storage=automatic] = null<ptr<ptr<ptr<volatile i32>>>>;
// DEFAULT-NEXT:                 let %[[VALUE_z_6:[0-9]+]] z: ptr<ptr<ptr<volatile i32>>> [storage=automatic] = null<ptr<ptr<ptr<volatile i32>>>>;
// DEFAULT-NEXT:                 write<ptr<ptr<ptr<volatile i32>>>>(%[[VALUE_x_6]], read<ptr<ptr<ptr<volatile i32>>>>(%[[VALUE_y_6]]));
// DEFAULT-NEXT:                 write<ptr<ptr<ptr<volatile i32>>>>(%[[VALUE_x_6]], read<ptr<ptr<ptr<volatile i32>>>>(%[[VALUE_z_6]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE6:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_x_7:[0-9]+]] x: ptr<ptr<ptr<const volatile i32>>> [storage=automatic] = null<ptr<ptr<ptr<const volatile i32>>>>;
// DEFAULT-NEXT:                 let %[[VALUE_y_7:[0-9]+]] y: ptr<ptr<ptr<const volatile i32>>> [storage=automatic] = null<ptr<ptr<ptr<const volatile i32>>>>;
// DEFAULT-NEXT:                 let %[[VALUE_z_7:[0-9]+]] z: ptr<ptr<ptr<const volatile i32>>> [storage=automatic] = null<ptr<ptr<ptr<const volatile i32>>>>;
// DEFAULT-NEXT:                 write<ptr<ptr<ptr<const volatile i32>>>>(%[[VALUE_x_7]], read<ptr<ptr<ptr<const volatile i32>>>>(%[[VALUE_y_7]]));
// DEFAULT-NEXT:                 write<ptr<ptr<ptr<const volatile i32>>>>(%[[VALUE_x_7]], read<ptr<ptr<ptr<const volatile i32>>>>(%[[VALUE_z_7]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE7:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_x_8:[0-9]+]] x: ptr<ptr<ptr<const void>>> [storage=automatic] = null<ptr<ptr<ptr<const void>>>>;
// DEFAULT-NEXT:                 let %[[VALUE_y_8:[0-9]+]] y: ptr<ptr<ptr<const void>>> [storage=automatic] = null<ptr<ptr<ptr<const void>>>>;
// DEFAULT-NEXT:                 let %[[VALUE_z_8:[0-9]+]] z: ptr<ptr<ptr<const void>>> [storage=automatic] = null<ptr<ptr<ptr<const void>>>>;
// DEFAULT-NEXT:                 write<ptr<ptr<ptr<const void>>>>(%[[VALUE_x_8]], read<ptr<ptr<ptr<const void>>>>(%[[VALUE_y_8]]));
// DEFAULT-NEXT:                 write<ptr<ptr<ptr<const void>>>>(%[[VALUE_x_8]], read<ptr<ptr<ptr<const void>>>>(%[[VALUE_z_8]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE8:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_x_9:[0-9]+]] x: ptr<ptr<ptr<const i32>>> [storage=automatic] = null<ptr<ptr<ptr<const i32>>>>;
// DEFAULT-NEXT:                 let %[[VALUE_y_9:[0-9]+]] y: ptr<ptr<ptr<const i32>>> [storage=automatic] = null<ptr<ptr<ptr<const i32>>>>;
// DEFAULT-NEXT:                 let %[[VALUE_z_9:[0-9]+]] z: ptr<ptr<ptr<const i32>>> [storage=automatic] = null<ptr<ptr<ptr<const i32>>>>;
// DEFAULT-NEXT:                 write<ptr<ptr<ptr<const i32>>>>(%[[VALUE_x_9]], read<ptr<ptr<ptr<const i32>>>>(%[[VALUE_y_9]]));
// DEFAULT-NEXT:                 write<ptr<ptr<ptr<const i32>>>>(%[[VALUE_x_9]], read<ptr<ptr<ptr<const i32>>>>(%[[VALUE_z_9]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE9:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_x_10:[0-9]+]] x: ptr<ptr<ptr<void>>> [storage=automatic] = null<ptr<ptr<ptr<void>>>>;
// DEFAULT-NEXT:                 let %[[VALUE_y_10:[0-9]+]] y: ptr<ptr<ptr<void>>> [storage=automatic] = null<ptr<ptr<ptr<void>>>>;
// DEFAULT-NEXT:                 let %[[VALUE_z_10:[0-9]+]] z: ptr<ptr<ptr<void>>> [storage=automatic] = null<ptr<ptr<ptr<void>>>>;
// DEFAULT-NEXT:                 write<ptr<ptr<ptr<void>>>>(%[[VALUE_x_10]], read<ptr<ptr<ptr<void>>>>(%[[VALUE_y_10]]));
// DEFAULT-NEXT:                 write<ptr<ptr<ptr<void>>>>(%[[VALUE_x_10]], read<ptr<ptr<ptr<void>>>>(%[[VALUE_z_10]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE10:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_x_11:[0-9]+]] x: ptr<ptr<ptr<volatile i32>>> [storage=automatic] = null<ptr<ptr<ptr<volatile i32>>>>;
// DEFAULT-NEXT:                 let %[[VALUE_y_11:[0-9]+]] y: ptr<ptr<ptr<volatile i32>>> [storage=automatic] = null<ptr<ptr<ptr<volatile i32>>>>;
// DEFAULT-NEXT:                 let %[[VALUE_z_11:[0-9]+]] z: ptr<ptr<ptr<volatile i32>>> [storage=automatic] = null<ptr<ptr<ptr<volatile i32>>>>;
// DEFAULT-NEXT:                 write<ptr<ptr<ptr<volatile i32>>>>(%[[VALUE_x_11]], read<ptr<ptr<ptr<volatile i32>>>>(%[[VALUE_y_11]]));
// DEFAULT-NEXT:                 write<ptr<ptr<ptr<volatile i32>>>>(%[[VALUE_x_11]], read<ptr<ptr<ptr<volatile i32>>>>(%[[VALUE_z_11]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE11:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_x_12:[0-9]+]] x: ptr<ptr<ptr<ptr<i32>>>> [storage=automatic] = null<ptr<ptr<ptr<ptr<i32>>>>>;
// DEFAULT-NEXT:                 let %[[VALUE_y_12:[0-9]+]] y: ptr<ptr<ptr<ptr<i32>>>> [storage=automatic] = null<ptr<ptr<ptr<ptr<i32>>>>>;
// DEFAULT-NEXT:                 let %[[VALUE_z_12:[0-9]+]] z: ptr<ptr<ptr<ptr<i32>>>> [storage=automatic] = null<ptr<ptr<ptr<ptr<i32>>>>>;
// DEFAULT-NEXT:                 write<ptr<ptr<ptr<ptr<i32>>>>>(%[[VALUE_x_12]], read<ptr<ptr<ptr<ptr<i32>>>>>(%[[VALUE_y_12]]));
// DEFAULT-NEXT:                 write<ptr<ptr<ptr<ptr<i32>>>>>(%[[VALUE_x_12]], read<ptr<ptr<ptr<ptr<i32>>>>>(%[[VALUE_z_12]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE12:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_x_13:[0-9]+]] x: ptr<ptr<ptr<fn() -> void>>> [storage=automatic] = null<ptr<ptr<ptr<fn() -> void>>>>;
// DEFAULT-NEXT:                 let %[[VALUE_y_13:[0-9]+]] y: ptr<ptr<ptr<fn() -> void>>> [storage=automatic] = null<ptr<ptr<ptr<fn() -> void>>>>;
// DEFAULT-NEXT:                 let %[[VALUE_z_13:[0-9]+]] z: ptr<ptr<ptr<fn() -> void>>> [storage=automatic] = null<ptr<ptr<ptr<fn() -> void>>>>;
// DEFAULT-NEXT:                 write<ptr<ptr<ptr<fn() -> void>>>>(%[[VALUE_x_13]], read<ptr<ptr<ptr<fn() -> void>>>>(%[[VALUE_y_13]]));
// DEFAULT-NEXT:                 write<ptr<ptr<ptr<fn() -> void>>>>(%[[VALUE_x_13]], read<ptr<ptr<ptr<fn() -> void>>>>(%[[VALUE_z_13]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE13:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_x_14:[0-9]+]] x: ptr<ptr<ptr<fn() -> void>>> [storage=automatic] = null<ptr<ptr<ptr<fn() -> void>>>>;
// DEFAULT-NEXT:                 let %[[VALUE_y_14:[0-9]+]] y: ptr<ptr<ptr<fn() -> void>>> [storage=automatic] = null<ptr<ptr<ptr<fn() -> void>>>>;
// DEFAULT-NEXT:                 let %[[VALUE_z_14:[0-9]+]] z: ptr<ptr<ptr<fn() -> void>>> [storage=automatic] = null<ptr<ptr<ptr<fn() -> void>>>>;
// DEFAULT-NEXT:                 write<ptr<ptr<ptr<fn() -> void>>>>(%[[VALUE_x_14]], read<ptr<ptr<ptr<fn() -> void>>>>(%[[VALUE_y_14]]));
// DEFAULT-NEXT:                 write<ptr<ptr<ptr<fn() -> void>>>>(%[[VALUE_x_14]], read<ptr<ptr<ptr<fn() -> void>>>>(%[[VALUE_z_14]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
