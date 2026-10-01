/* PR middle-end/32912 */
/* { dg-do run } */
/* { dg-options "-O2 -w" } */
/* { dg-additional-options "-fno-common" { target hppa*-*-hpux* } } */
/* { dg-additional-options "-msse" { target { i?86-*-* x86_64-*-* } } } */
/* { dg-require-effective-target sse_runtime { target { i?86-*-* x86_64-*-* } } } */

extern void abort (void);

typedef int __m128i __attribute__ ((__vector_size__ (16)));

__m128i a, b, c, d, e, f;

void
foo (__m128i x)
{
  a = x ^ ~x;
  b = ~x ^ x;
  c = x | ~x;
  d = ~x | x;
  e = x & ~x;
  f = ~x & x;
}

int
main (void)
{
  union { __m128i v; int i[sizeof (__m128i) / sizeof (int)]; } u;
  int i;

  for (i = 0; i < sizeof (u.i) / sizeof (u.i[0]); i++)
    u.i[i] = i * 49 - 36;
  foo (u.v);
#define check(x, val) \
  u.v = (x); \
  for (i = 0; i < sizeof (u.i) / sizeof (u.i[0]); i++) \
    if (u.i[i] != (val)) \
      abort ()

  check (a, ~0);
  check (b, ~0);
  check (c, ~0);
  check (d, ~0);
  check (e, 0);
  check (f, 0);
  return 0;
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
// DEFAULT-NEXT:     type @type[[TYPE___m128i:[0-9]+]] __m128i = vector<i32, 4>;
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = union {
// DEFAULT-NEXT:         field0 v: vector<i32, 4>;
// DEFAULT-NEXT:         field1 i: array<i32, 4>;
// DEFAULT-NEXT:     } [size=16, align=16, offsets=[0, 0]];
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: vector<i32, 4> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: vector<i32, 4> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: vector<i32, 4> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: vector<i32, 4> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: vector<i32, 4> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: vector<i32, 4> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: vector<i32, 4>) -> void [linkage=external] [abi=sysv64(direct) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<vector<i32, 4>>(%[[VALUE_a]], xor<vector<i32, 4>, elementwise=true>(read<vector<i32, 4>>(%[[VALUE_x]]), not<vector<i32, 4>, elementwise=true>(read<vector<i32, 4>>(%[[VALUE_x]]))));
// DEFAULT-NEXT:         write<vector<i32, 4>>(%[[VALUE_b]], xor<vector<i32, 4>, elementwise=true>(not<vector<i32, 4>, elementwise=true>(read<vector<i32, 4>>(%[[VALUE_x]])), read<vector<i32, 4>>(%[[VALUE_x]])));
// DEFAULT-NEXT:         write<vector<i32, 4>>(%[[VALUE_c]], or<vector<i32, 4>, elementwise=true>(read<vector<i32, 4>>(%[[VALUE_x]]), not<vector<i32, 4>, elementwise=true>(read<vector<i32, 4>>(%[[VALUE_x]]))));
// DEFAULT-NEXT:         write<vector<i32, 4>>(%[[VALUE_d]], or<vector<i32, 4>, elementwise=true>(not<vector<i32, 4>, elementwise=true>(read<vector<i32, 4>>(%[[VALUE_x]])), read<vector<i32, 4>>(%[[VALUE_x]])));
// DEFAULT-NEXT:         write<vector<i32, 4>>(%[[VALUE_e]], and<vector<i32, 4>, elementwise=true>(read<vector<i32, 4>>(%[[VALUE_x]]), not<vector<i32, 4>, elementwise=true>(read<vector<i32, 4>>(%[[VALUE_x]]))));
// DEFAULT-NEXT:         write<vector<i32, 4>>(%[[VALUE_f]], and<vector<i32, 4>, elementwise=true>(not<vector<i32, 4>, elementwise=true>(read<vector<i32, 4>>(%[[VALUE_x]])), read<vector<i32, 4>>(%[[VALUE_x]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_u:[0-9]+]] u: @type[[TYPE0]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i]]))), div<u64, by_zero=ub>(const<u64>(16), const<u64>(4)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(field1(%[[VALUE_u]])), read<i32>(%[[VALUE_i]]))), sub<i32, overflow=ub>(mul<i32, overflow=ub>(read<i32>(%[[VALUE_i]]), const<i32>(49)), const<i32>(36)));
// DEFAULT-NEXT:         call<void, signature=fn(vector<i32, 4>) -> void, abi=sysv64(direct) -> void>(%[[VALUE_foo]], read<vector<i32, 4>>(field0(%[[VALUE_u]])));
// DEFAULT-NEXT:         write<vector<i32, 4>>(field0(%[[VALUE_u]]), read<vector<i32, 4>>(%[[VALUE_a]]));
// DEFAULT-NEXT:         for %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i]]))), div<u64, by_zero=ub>(const<u64>(16), const<u64>(4)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(field1(%[[VALUE_u]])), read<i32>(%[[VALUE_i]])))), not<i32>(const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<vector<i32, 4>>(field0(%[[VALUE_u]]), read<vector<i32, 4>>(%[[VALUE_b]]));
// DEFAULT-NEXT:         for %[[VALUE6:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i]]))), div<u64, by_zero=ub>(const<u64>(16), const<u64>(4)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE8:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE7]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE8]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(field1(%[[VALUE_u]])), read<i32>(%[[VALUE_i]])))), not<i32>(const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<vector<i32, 4>>(field0(%[[VALUE_u]]), read<vector<i32, 4>>(%[[VALUE_c]]));
// DEFAULT-NEXT:         for %[[VALUE9:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i]]))), div<u64, by_zero=ub>(const<u64>(16), const<u64>(4)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE10:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE11:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE10]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE11]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(field1(%[[VALUE_u]])), read<i32>(%[[VALUE_i]])))), not<i32>(const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<vector<i32, 4>>(field0(%[[VALUE_u]]), read<vector<i32, 4>>(%[[VALUE_d]]));
// DEFAULT-NEXT:         for %[[VALUE12:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i]]))), div<u64, by_zero=ub>(const<u64>(16), const<u64>(4)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE13:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE14:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE13]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE14]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(field1(%[[VALUE_u]])), read<i32>(%[[VALUE_i]])))), not<i32>(const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<vector<i32, 4>>(field0(%[[VALUE_u]]), read<vector<i32, 4>>(%[[VALUE_e]]));
// DEFAULT-NEXT:         for %[[VALUE15:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i]]))), div<u64, by_zero=ub>(const<u64>(16), const<u64>(4)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE16:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE17:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE16]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE17]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(field1(%[[VALUE_u]])), read<i32>(%[[VALUE_i]])))), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<vector<i32, 4>>(field0(%[[VALUE_u]]), read<vector<i32, 4>>(%[[VALUE_f]]));
// DEFAULT-NEXT:         for %[[VALUE18:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i]]))), div<u64, by_zero=ub>(const<u64>(16), const<u64>(4)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE19:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE20:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE19]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE20]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(field1(%[[VALUE_u]])), read<i32>(%[[VALUE_i]])))), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
