/* PR middle-end/92815 - a variant of gcc.dg/builtin-object-size-20.c
   prepared for all targets, irregardless if they pack or not
   the structs by default.
   { dg-do compile }
   { dg-options "-O -Wall -fdump-tree-optimized" } */

#define ASSERT(expr) ((expr) ? (void)0 : fail (__LINE__))
#define bos0(expr) __builtin_object_size (expr, 1)
#define bos1(expr) __builtin_object_size (expr, 1)
#define bos2(expr) __builtin_object_size (expr, 2)
#define bos3(expr) __builtin_object_size (expr, 3)

typedef __SIZE_TYPE__  size_t;


extern void fail (int);


/* Verify sizes of a struct with a flexible array member and no padding.  */

struct ACX { char n, a[]; };

struct ACX ac0 = { };
struct ACX ac1 = { 1, { 1 } };
struct ACX ac2 = { 2, { 1, 2 } };
struct ACX ac3 = { 3, { 1, 2, 3 } };

extern struct ACX eacx;

void facx (void)
{
  ASSERT (bos0 (&ac0) == sizeof ac0);
  ASSERT (bos0 (&ac1) == 2);
  ASSERT (bos0 (&ac2) == 3);
  ASSERT (bos0 (&ac3) == 4);
  ASSERT (bos0 (&eacx) == (size_t)-1);

  ASSERT (bos1 (&ac0) == sizeof ac0);
  ASSERT (bos1 (&ac1) == 2);
  ASSERT (bos1 (&ac2) == 3);
  ASSERT (bos1 (&ac3) == 4);
  ASSERT (bos1 (&eacx) == (size_t)-1);

  ASSERT (bos2 (&ac0) == sizeof ac0);
  ASSERT (bos2 (&ac1) == 2);
  ASSERT (bos2 (&ac2) == 3);
  ASSERT (bos2 (&ac3) == 4);
  ASSERT (bos2 (&eacx) == sizeof eacx);

  ASSERT (bos3 (&ac0) == sizeof ac0);
  ASSERT (bos3 (&ac1) == 2);
  ASSERT (bos3 (&ac2) == 3);
  ASSERT (bos3 (&ac3) == 4);
  ASSERT (bos3 (&eacx) == sizeof eacx);
}

/* Also verify sizes of a struct with a zero length array member.  */

struct A0C0 { char n, a[0]; };

struct A0C0 a0c0 = { };
extern struct A0C0 ea0c0;

void fa0c0 (void)
{
  ASSERT (bos0 (&a0c0) == sizeof a0c0);
  ASSERT (bos0 (&ea0c0) == sizeof ea0c0);

  ASSERT (bos1 (&a0c0) == sizeof a0c0);
  ASSERT (bos1 (&a0c0) == sizeof ea0c0);

  ASSERT (bos2 (&a0c0) == sizeof a0c0);
  ASSERT (bos2 (&a0c0) == sizeof ea0c0);

  ASSERT (bos3 (&a0c0) == sizeof a0c0);
  ASSERT (bos3 (&a0c0) == sizeof ea0c0);
}

/* { dg-final { scan-tree-dump-not "fail" "optimized" } } */

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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_ACX:[0-9]+]] ACX = struct {
// DEFAULT-NEXT:         field0 n: i8;
// DEFAULT-NEXT:         field1 a: array<i8, incomplete>;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     type @type[[TYPE_A0C0:[0-9]+]] A0C0 = struct {
// DEFAULT-NEXT:         field0 n: i8;
// DEFAULT-NEXT:         field1 a: array<i8, 0>;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     global %[[VALUE_ac0:[0-9]+]] ac0: @type[[TYPE_ACX]] [storage=static] = aggregate<@type[[TYPE_ACX]], zero_fill=true>() [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ac1:[0-9]+]] ac1: @type[[TYPE_ACX]] [storage=static] = aggregate<@type[[TYPE_ACX]], zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), field1 = aggregate<array<i8, 1>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(1)))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ac2:[0-9]+]] ac2: @type[[TYPE_ACX]] [storage=static] = aggregate<@type[[TYPE_ACX]], zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(2)), field1 = aggregate<array<i8, 2>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(2)))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ac3:[0-9]+]] ac3: @type[[TYPE_ACX]] [storage=static] = aggregate<@type[[TYPE_ACX]], zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(3)), field1 = aggregate<array<i8, 3>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(2)), index2 = truncate<i8, reason=assign, fits=always>(const<i32>(3)))) [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_eacx:[0-9]+]] eacx: @type[[TYPE_ACX]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_a0c0:[0-9]+]] a0c0: @type[[TYPE_A0C0]] [storage=static] = aggregate<@type[[TYPE_A0C0]], zero_fill=true>() [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_ea0c0:[0-9]+]] ea0c0: @type[[TYPE_A0C0]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fail:[0-9]+]] @fail(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_object_size:[0-9]+]] @__builtin_object_size(%[[VALUE1:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE2:[0-9]+]] <unnamed>: i32) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_facx:[0-9]+]] @facx() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_ACX]]>>(%[[VALUE_ac0]])), const<i32>(1)), const<u64>(1))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%[[VALUE_fail]], const<i32>(32));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_ACX]]>>(%[[VALUE_ac1]])), const<i32>(1)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%[[VALUE_fail]], const<i32>(33));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_ACX]]>>(%[[VALUE_ac2]])), const<i32>(1)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%[[VALUE_fail]], const<i32>(34));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_ACX]]>>(%[[VALUE_ac3]])), const<i32>(1)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%[[VALUE_fail]], const<i32>(35));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_ACX]]>>(%[[VALUE_eacx]])), const<i32>(1)), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%[[VALUE_fail]], const<i32>(36));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_ACX]]>>(%[[VALUE_ac0]])), const<i32>(1)), const<u64>(1))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%[[VALUE_fail]], const<i32>(38));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_ACX]]>>(%[[VALUE_ac1]])), const<i32>(1)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%[[VALUE_fail]], const<i32>(39));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_ACX]]>>(%[[VALUE_ac2]])), const<i32>(1)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%[[VALUE_fail]], const<i32>(40));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_ACX]]>>(%[[VALUE_ac3]])), const<i32>(1)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%[[VALUE_fail]], const<i32>(41));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_ACX]]>>(%[[VALUE_eacx]])), const<i32>(1)), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%[[VALUE_fail]], const<i32>(42));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_ACX]]>>(%[[VALUE_ac0]])), const<i32>(2)), const<u64>(1))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%[[VALUE_fail]], const<i32>(44));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_ACX]]>>(%[[VALUE_ac1]])), const<i32>(2)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%[[VALUE_fail]], const<i32>(45));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_ACX]]>>(%[[VALUE_ac2]])), const<i32>(2)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%[[VALUE_fail]], const<i32>(46));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_ACX]]>>(%[[VALUE_ac3]])), const<i32>(2)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%[[VALUE_fail]], const<i32>(47));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_ACX]]>>(%[[VALUE_eacx]])), const<i32>(2)), const<u64>(1))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%[[VALUE_fail]], const<i32>(48));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_ACX]]>>(%[[VALUE_ac0]])), const<i32>(3)), const<u64>(1))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%[[VALUE_fail]], const<i32>(50));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_ACX]]>>(%[[VALUE_ac1]])), const<i32>(3)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%[[VALUE_fail]], const<i32>(51));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_ACX]]>>(%[[VALUE_ac2]])), const<i32>(3)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%[[VALUE_fail]], const<i32>(52));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_ACX]]>>(%[[VALUE_ac3]])), const<i32>(3)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%[[VALUE_fail]], const<i32>(53));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_ACX]]>>(%[[VALUE_eacx]])), const<i32>(3)), const<u64>(1))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%[[VALUE_fail]], const<i32>(54));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fa0c0:[0-9]+]] @fa0c0() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_A0C0]]>>(%[[VALUE_a0c0]])), const<i32>(1)), const<u64>(1))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%[[VALUE_fail]], const<i32>(66));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_A0C0]]>>(%[[VALUE_ea0c0]])), const<i32>(1)), const<u64>(1))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%[[VALUE_fail]], const<i32>(67));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_A0C0]]>>(%[[VALUE_a0c0]])), const<i32>(1)), const<u64>(1))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%[[VALUE_fail]], const<i32>(69));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_A0C0]]>>(%[[VALUE_a0c0]])), const<i32>(1)), const<u64>(1))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%[[VALUE_fail]], const<i32>(70));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_A0C0]]>>(%[[VALUE_a0c0]])), const<i32>(2)), const<u64>(1))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%[[VALUE_fail]], const<i32>(72));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_A0C0]]>>(%[[VALUE_a0c0]])), const<i32>(2)), const<u64>(1))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%[[VALUE_fail]], const<i32>(73));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_A0C0]]>>(%[[VALUE_a0c0]])), const<i32>(3)), const<u64>(1))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%[[VALUE_fail]], const<i32>(75));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_A0C0]]>>(%[[VALUE_a0c0]])), const<i32>(3)), const<u64>(1))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%[[VALUE_fail]], const<i32>(76));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
