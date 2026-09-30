/* Test C23 enumerations with fixed underlying type.  Valid code.  */
/* { dg-do run } */
/* { dg-options "-std=c23 -pedantic-errors" } */

/* Check a type while defining an enum (via a diagnostic for incompatible
   pointer types if the wrong type was chosen).  */
#define TYPE_CHECK(cst, type)						\
  cst ## _type_check = sizeof (1 ? (type *) 0 : (typeof (cst) *) 0)

extern int i;

enum e1 : short { e1a = __SHRT_MAX__,
    TYPE_CHECK (e1a, short),
    e1z = (long long) 0,
    TYPE_CHECK (e1z, enum e1),
    e1b = -__SHRT_MAX__ - 1,
    e1c,
    TYPE_CHECK (e1c, enum e1) };
extern enum e1 e1v;
extern typeof (e1a) e1v;
extern typeof (e1b) e1v;
extern typeof (e1c) e1v;
extern typeof (e1z) e1v;
extern short e1v;
static_assert (e1a == __SHRT_MAX__);
static_assert (e1b == -__SHRT_MAX__ - 1);
static_assert (e1c == -__SHRT_MAX__);
static_assert (e1a > 0);
static_assert (e1b < 0);
static_assert (e1c < 0);
static_assert (e1z == 0);
extern typeof (+e1v) i;
extern typeof (+e1a) i;
extern typeof (e1a + e1b) i;
enum e1 : short;
enum e1 : volatile short;
enum e1 : _Atomic short;
enum e1 : typeof (short);

enum e2 : bool { b0, b1, b0a = 0, b1a = 1 };
extern enum e2 e2v;
extern typeof (b0) e2v;
extern typeof (b0a) e2v;
extern typeof (b1) e2v;
extern typeof (b1a) e2v;
extern bool e2v;
extern typeof (+e2v) i;
extern typeof (+b0) i;
static_assert (b0 == 0);
static_assert (b1 == 1);
static_assert (b0a == 0);
static_assert (b1a == 1);

enum e3 : volatile const _Atomic unsigned short;
enum e3 : unsigned short { e3a, e3b };
extern enum e3 e3v;
extern typeof (e3a) e3v;
extern typeof (e3b) e3v;
extern unsigned short e3v;

/* The enum type is complete from the end of the first enum type specifier
   (which is nested inside another enum type specifier in this example).  */
enum e4 : typeof ((enum e4 : long { e4a = sizeof (enum e4) })0, 0L);
extern enum e4 e4v;
extern typeof (e4a) e4v;
extern long e4v;

enum e5 : unsigned int;
extern enum e5 e5v;
extern typeof (e5v + e5v) e5v;
extern unsigned int e5v;

enum : unsigned short { e6a, e6b, TYPE_CHECK (e6a, unsigned short) } e6v;
extern typeof (e6a) e6v;
extern typeof (e6b) e6v;
extern unsigned short e6v;

struct s1;
struct s2 { int a; };
union u1;
union u2 { int a; };
enum xe1 { XE1 };
enum xe2 : long long { XE2 };
enum xe3 : unsigned long;

void
f ()
{
  /* Tags can be redeclared in an inner scope.  */
  enum s1 : char;
  enum s2 : int { S2 };
  enum u1 : long { U1 };
  enum u2 : unsigned char;
  enum xe1 : long long;
  enum xe2 : short;
  enum xe3 : char { XE3 };
  static_assert (sizeof (enum xe3) == 1);
  static_assert (sizeof (enum xe2) == sizeof (short));
  static_assert (sizeof (enum xe1) == sizeof (long long));
}

void *p;
typeof (nullptr) np;

extern void abort (void);
extern void exit (int);

int
main ()
{
  /* Conversions to enums with fixed underlying type have the same semantics as
     converting to the underlying type.  */
  volatile enum e1 e1vm;
  volatile enum e2 e2vm;
  e1vm = __LONG_LONG_MAX__; /* { dg-warning "overflow" } */
  if (e1vm != (short) __LONG_LONG_MAX__)
    abort ();
  e2vm = 10;
  if (e2vm != 1)
    abort ();
  e2vm = 0;
  if (e2vm != 0)
    abort ();
  /* Arithmetic on enums with fixed underlying type has the same semantics as
     arithmetic on the underlying type; in particular, the special semantics
     for bool apply to enums with bool as fixed underlying type.  */
  if (e2vm++ != 0)
    abort ();
  if (e2vm != 1)
    abort ();
  if (e2vm++ != 1)
    abort ();
  if (e2vm != 1)
    abort ();
  if (e2vm-- != 1)
    abort ();
  if (e2vm != 0)
    abort ();
  if (e2vm-- != 0)
    abort ();
  if (e2vm != 1)
    abort ();
  if (++e2vm != 1)
    abort ();
  if (e2vm != 1)
    abort ();
  e2vm = 0;
  if (++e2vm != 1)
    abort ();
  if (e2vm != 1)
    abort ();
  if (--e2vm != 0)
    abort ();
  if (e2vm != 0)
    abort ();
  if (--e2vm != 1)
    abort ();
  if (e2vm != 1)
    abort ();
  e2vm = p;
  e2vm = np;
  e2vm = (bool) p;
  e2vm = (bool) np;
  if (e2vm != 0)
    abort ();
  exit (0);
}

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
// DEFAULT-NEXT:     type @type[[TYPE_e1:[0-9]+]] e1 = enum : i16 {
// DEFAULT-NEXT:         %[[VALUE_e1a:[0-9]+]] e1a = const<@type[[TYPE_e1]]>(32767);
// DEFAULT-NEXT:         %[[VALUE_e1a_type_check:[0-9]+]] e1a_type_check = const<@type[[TYPE_e1]]>(8);
// DEFAULT-NEXT:         %[[VALUE_e1z:[0-9]+]] e1z = const<@type[[TYPE_e1]]>(0);
// DEFAULT-NEXT:         %[[VALUE_e1z_type_check:[0-9]+]] e1z_type_check = const<@type[[TYPE_e1]]>(8);
// DEFAULT-NEXT:         %[[VALUE_e1b:[0-9]+]] e1b = const<@type[[TYPE_e1]]>(-32768);
// DEFAULT-NEXT:         %[[VALUE_e1c:[0-9]+]] e1c = const<@type[[TYPE_e1]]>(-32767);
// DEFAULT-NEXT:         %[[VALUE_e1c_type_check:[0-9]+]] e1c_type_check = const<@type[[TYPE_e1]]>(8);
// DEFAULT-NEXT:     } [size=2, align=2];
// DEFAULT-NEXT:     type @type[[TYPE_e2:[0-9]+]] e2 = enum : bool {
// DEFAULT-NEXT:         %[[VALUE_e1a]] b0 = const<@type[[TYPE_e2]]>(0);
// DEFAULT-NEXT:         %[[VALUE_e1a_type_check]] b1 = const<@type[[TYPE_e2]]>(1);
// DEFAULT-NEXT:         %[[VALUE_e1z]] b0a = const<@type[[TYPE_e2]]>(0);
// DEFAULT-NEXT:         %[[VALUE_e1z_type_check]] b1a = const<@type[[TYPE_e2]]>(1);
// DEFAULT-NEXT:     } [size=1, align=1];
// DEFAULT-NEXT:     type @type[[TYPE_e3:[0-9]+]] e3 = enum : u16 {
// DEFAULT-NEXT:         %[[VALUE_e1a]] e3a = const<@type[[TYPE_e3]]>(0);
// DEFAULT-NEXT:         %[[VALUE_e1a_type_check]] e3b = const<@type[[TYPE_e3]]>(1);
// DEFAULT-NEXT:     } [size=2, align=2];
// DEFAULT-NEXT:     type @type[[TYPE_e4:[0-9]+]] e4 = enum : i64 {
// DEFAULT-NEXT:         %[[VALUE_e1a]] e4a = const<@type[[TYPE_e4]]>(8);
// DEFAULT-NEXT:     } [size=8, align=8];
// DEFAULT-NEXT:     type @type[[TYPE_e5:[0-9]+]] e5 = enum : u32 incomplete [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = enum : u16 {
// DEFAULT-NEXT:         %[[VALUE_e1a]] e6a = const<@type[[TYPE0]]>(0);
// DEFAULT-NEXT:         %[[VALUE_e1a_type_check]] e6b = const<@type[[TYPE0]]>(1);
// DEFAULT-NEXT:         %[[VALUE_e1z]] e6a_type_check = const<@type[[TYPE0]]>(8);
// DEFAULT-NEXT:     } [size=2, align=2];
// DEFAULT-NEXT:     type @type[[TYPE_s1:[0-9]+]] s1 = struct incomplete;
// DEFAULT-NEXT:     type @type[[TYPE_s2:[0-9]+]] s2 = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_u1:[0-9]+]] u1 = union incomplete;
// DEFAULT-NEXT:     type @type[[TYPE_u2:[0-9]+]] u2 = union {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_xe1:[0-9]+]] xe1 = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_e1a]] XE1 = const<i32>(0);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_xe2:[0-9]+]] xe2 = enum : i64 {
// DEFAULT-NEXT:         %[[VALUE_e1a]] XE2 = const<@type[[TYPE_xe2]]>(0);
// DEFAULT-NEXT:     } [size=8, align=8];
// DEFAULT-NEXT:     type @type[[TYPE_xe3:[0-9]+]] xe3 = enum : u64 incomplete [size=8, align=8];
// DEFAULT-NEXT:     type @type[[TYPE_s1_2:[0-9]+]] s1 = enum : i8 incomplete [size=1, align=1];
// DEFAULT-NEXT:     type @type[[TYPE_s2_2:[0-9]+]] s2 = enum : i32 {
// DEFAULT-NEXT:         %[[VALUE_e1a]] S2 = const<@type[[TYPE_s2_2]]>(0);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_u1_2:[0-9]+]] u1 = enum : i64 {
// DEFAULT-NEXT:         %[[VALUE_e1a]] U1 = const<@type[[TYPE_u1_2]]>(0);
// DEFAULT-NEXT:     } [size=8, align=8];
// DEFAULT-NEXT:     type @type[[TYPE_u2_2:[0-9]+]] u2 = enum : u8 incomplete [size=1, align=1];
// DEFAULT-NEXT:     type @type[[TYPE_xe1_2:[0-9]+]] xe1 = enum : i64 incomplete [size=8, align=8];
// DEFAULT-NEXT:     type @type[[TYPE_xe2_2:[0-9]+]] xe2 = enum : i16 incomplete [size=2, align=2];
// DEFAULT-NEXT:     type @type[[TYPE_xe3_2:[0-9]+]] xe3 = enum : i8 {
// DEFAULT-NEXT:         %[[VALUE_e1a]] XE3 = const<@type[[TYPE_xe3_2]]>(0);
// DEFAULT-NEXT:     } [size=1, align=1];
// DEFAULT-NEXT:     extern %[[VALUE_e1a]] i: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_e1v:[0-9]+]] e1v: @type[[TYPE_e1]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_e2v:[0-9]+]] e2v: @type[[TYPE_e2]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_e3v:[0-9]+]] e3v: @type[[TYPE_e3]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_e4v:[0-9]+]] e4v: @type[[TYPE_e4]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_e5v:[0-9]+]] e5v: @type[[TYPE_e5]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e6v:[0-9]+]] e6v: @type[[TYPE0]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_p:[0-9]+]] p: ptr<void> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_np:[0-9]+]] np: ptr<void> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_e1vm:[0-9]+]] e1vm: volatile @type[[TYPE_e1]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_e2vm:[0-9]+]] e2vm: volatile @type[[TYPE_e2]] [storage=automatic];
// DEFAULT-NEXT:         write<@type[[TYPE_e1]], volatile>(%[[VALUE_e1vm]], int_to_enum<@type[[TYPE_e1]], reason=assign>(truncate<i16, reason=assign, fits=unknown>(const<i64>(9223372036854775807))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(enum_to_int<i16, reason=promotion>(read<@type[[TYPE_e1]], volatile>(%[[VALUE_e1vm]]))), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(const<i64>(9223372036854775807))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<@type[[TYPE_e2]], volatile>(%[[VALUE_e2vm]], int_to_enum<@type[[TYPE_e2]], reason=assign>(ne<i32, reason=assign>(const<i32>(10), const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(enum_to_int<bool, reason=promotion>(read<@type[[TYPE_e2]], volatile>(%[[VALUE_e2vm]]))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<@type[[TYPE_e2]], volatile>(%[[VALUE_e2vm]], int_to_enum<@type[[TYPE_e2]], reason=assign>(ne<i32, reason=assign>(const<i32>(0), const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(enum_to_int<bool, reason=promotion>(read<@type[[TYPE_e2]], volatile>(%[[VALUE_e2vm]]))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: @type[[TYPE_e2]] [synthetic] = read<@type[[TYPE_e2]], volatile>(%[[VALUE_e2vm]]);
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: @type[[TYPE_e2]] [synthetic] = int_to_enum<@type[[TYPE_e2]], reason=assign>(ne<i32, reason=assign>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(enum_to_int<bool, reason=promotion>(read<@type[[TYPE_e2]]>(%[[VALUE1]]))), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:         write<@type[[TYPE_e2]], volatile>(%[[VALUE_e2vm]], read<@type[[TYPE_e2]]>(%[[VALUE2]]));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(enum_to_int<bool, reason=promotion>(read<@type[[TYPE_e2]]>(%[[VALUE1]]))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(enum_to_int<bool, reason=promotion>(read<@type[[TYPE_e2]], volatile>(%[[VALUE_e2vm]]))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: @type[[TYPE_e2]] [synthetic] = read<@type[[TYPE_e2]], volatile>(%[[VALUE_e2vm]]);
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: @type[[TYPE_e2]] [synthetic] = int_to_enum<@type[[TYPE_e2]], reason=assign>(ne<i32, reason=assign>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(enum_to_int<bool, reason=promotion>(read<@type[[TYPE_e2]]>(%[[VALUE3]]))), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:         write<@type[[TYPE_e2]], volatile>(%[[VALUE_e2vm]], read<@type[[TYPE_e2]]>(%[[VALUE4]]));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(enum_to_int<bool, reason=promotion>(read<@type[[TYPE_e2]]>(%[[VALUE3]]))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(enum_to_int<bool, reason=promotion>(read<@type[[TYPE_e2]], volatile>(%[[VALUE_e2vm]]))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: @type[[TYPE_e2]] [synthetic] = read<@type[[TYPE_e2]], volatile>(%[[VALUE_e2vm]]);
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: @type[[TYPE_e2]] [synthetic] = int_to_enum<@type[[TYPE_e2]], reason=assign>(ne<i32, reason=assign>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(enum_to_int<bool, reason=promotion>(read<@type[[TYPE_e2]]>(%[[VALUE5]]))), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:         write<@type[[TYPE_e2]], volatile>(%[[VALUE_e2vm]], read<@type[[TYPE_e2]]>(%[[VALUE6]]));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(enum_to_int<bool, reason=promotion>(read<@type[[TYPE_e2]]>(%[[VALUE5]]))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(enum_to_int<bool, reason=promotion>(read<@type[[TYPE_e2]], volatile>(%[[VALUE_e2vm]]))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: @type[[TYPE_e2]] [synthetic] = read<@type[[TYPE_e2]], volatile>(%[[VALUE_e2vm]]);
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: @type[[TYPE_e2]] [synthetic] = int_to_enum<@type[[TYPE_e2]], reason=assign>(ne<i32, reason=assign>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(enum_to_int<bool, reason=promotion>(read<@type[[TYPE_e2]]>(%[[VALUE7]]))), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:         write<@type[[TYPE_e2]], volatile>(%[[VALUE_e2vm]], read<@type[[TYPE_e2]]>(%[[VALUE8]]));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(enum_to_int<bool, reason=promotion>(read<@type[[TYPE_e2]]>(%[[VALUE7]]))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(enum_to_int<bool, reason=promotion>(read<@type[[TYPE_e2]], volatile>(%[[VALUE_e2vm]]))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: @type[[TYPE_e2]] [synthetic] = read<@type[[TYPE_e2]], volatile>(%[[VALUE_e2vm]]);
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: @type[[TYPE_e2]] [synthetic] = int_to_enum<@type[[TYPE_e2]], reason=assign>(ne<i32, reason=assign>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(enum_to_int<bool, reason=promotion>(read<@type[[TYPE_e2]]>(%[[VALUE9]]))), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:         write<@type[[TYPE_e2]], volatile>(%[[VALUE_e2vm]], read<@type[[TYPE_e2]]>(%[[VALUE10]]));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(enum_to_int<bool, reason=promotion>(read<@type[[TYPE_e2]]>(%[[VALUE10]]))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(enum_to_int<bool, reason=promotion>(read<@type[[TYPE_e2]], volatile>(%[[VALUE_e2vm]]))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<@type[[TYPE_e2]], volatile>(%[[VALUE_e2vm]], int_to_enum<@type[[TYPE_e2]], reason=assign>(ne<i32, reason=assign>(const<i32>(0), const<i32>(0))));
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: @type[[TYPE_e2]] [synthetic] = read<@type[[TYPE_e2]], volatile>(%[[VALUE_e2vm]]);
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: @type[[TYPE_e2]] [synthetic] = int_to_enum<@type[[TYPE_e2]], reason=assign>(ne<i32, reason=assign>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(enum_to_int<bool, reason=promotion>(read<@type[[TYPE_e2]]>(%[[VALUE11]]))), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:         write<@type[[TYPE_e2]], volatile>(%[[VALUE_e2vm]], read<@type[[TYPE_e2]]>(%[[VALUE12]]));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(enum_to_int<bool, reason=promotion>(read<@type[[TYPE_e2]]>(%[[VALUE12]]))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(enum_to_int<bool, reason=promotion>(read<@type[[TYPE_e2]], volatile>(%[[VALUE_e2vm]]))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: @type[[TYPE_e2]] [synthetic] = read<@type[[TYPE_e2]], volatile>(%[[VALUE_e2vm]]);
// DEFAULT-NEXT:         let %[[VALUE14:[0-9]+]]: @type[[TYPE_e2]] [synthetic] = int_to_enum<@type[[TYPE_e2]], reason=assign>(ne<i32, reason=assign>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(enum_to_int<bool, reason=promotion>(read<@type[[TYPE_e2]]>(%[[VALUE13]]))), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:         write<@type[[TYPE_e2]], volatile>(%[[VALUE_e2vm]], read<@type[[TYPE_e2]]>(%[[VALUE14]]));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(enum_to_int<bool, reason=promotion>(read<@type[[TYPE_e2]]>(%[[VALUE14]]))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(enum_to_int<bool, reason=promotion>(read<@type[[TYPE_e2]], volatile>(%[[VALUE_e2vm]]))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE15:[0-9]+]]: @type[[TYPE_e2]] [synthetic] = read<@type[[TYPE_e2]], volatile>(%[[VALUE_e2vm]]);
// DEFAULT-NEXT:         let %[[VALUE16:[0-9]+]]: @type[[TYPE_e2]] [synthetic] = int_to_enum<@type[[TYPE_e2]], reason=assign>(ne<i32, reason=assign>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(enum_to_int<bool, reason=promotion>(read<@type[[TYPE_e2]]>(%[[VALUE15]]))), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:         write<@type[[TYPE_e2]], volatile>(%[[VALUE_e2vm]], read<@type[[TYPE_e2]]>(%[[VALUE16]]));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(enum_to_int<bool, reason=promotion>(read<@type[[TYPE_e2]]>(%[[VALUE16]]))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(enum_to_int<bool, reason=promotion>(read<@type[[TYPE_e2]], volatile>(%[[VALUE_e2vm]]))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<@type[[TYPE_e2]], volatile>(%[[VALUE_e2vm]], int_to_enum<@type[[TYPE_e2]], reason=assign>(ne<ptr<void>, reason=assign>(read<ptr<void>>(%[[VALUE_p]]), null<ptr<void>>)));
// DEFAULT-NEXT:         write<@type[[TYPE_e2]], volatile>(%[[VALUE_e2vm]], int_to_enum<@type[[TYPE_e2]], reason=assign>(ne<ptr<void>, reason=assign>(read<ptr<void>>(%[[VALUE_np]]), null<ptr<void>>)));
// DEFAULT-NEXT:         write<@type[[TYPE_e2]], volatile>(%[[VALUE_e2vm]], int_to_enum<@type[[TYPE_e2]], reason=assign>(ne<ptr<void>, reason=explicit>(read<ptr<void>>(%[[VALUE_p]]), null<ptr<void>>)));
// DEFAULT-NEXT:         write<@type[[TYPE_e2]], volatile>(%[[VALUE_e2vm]], int_to_enum<@type[[TYPE_e2]], reason=assign>(ne<ptr<void>, reason=explicit>(read<ptr<void>>(%[[VALUE_np]]), null<ptr<void>>)));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(enum_to_int<bool, reason=promotion>(read<@type[[TYPE_e2]], volatile>(%[[VALUE_e2vm]]))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
