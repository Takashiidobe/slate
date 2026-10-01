/* { dg-do run } */
/* { dg-options "-O2 -w" } */

extern void abort (void);

#if(__SIZEOF_INT__ >= 4)
# define TYPE      int
# define TYPED(a)  a

#elif(__SIZEOF_INT__ > 2)
# define TYPE      long
# define TYPED(a)  a##L

#else
# define TYPE      long long
# define TYPED(a)  a##LL
#endif


typedef TYPE __m128i __attribute__ ((__vector_size__ (16)));

__m128i
foo (void)
{
  __m128i x = { 0x11111111, 0x22222222, 0x44444444 };
  return x;
}

__m128i
bar (void)
{
  __m128i x = { 0x11111111, 0x22222222, 0x44444444 };
  return ~x;
}

int
main (void)
{
  union { __m128i v; TYPE i[sizeof (__m128i) / sizeof (TYPE)]; } u, v;
  int i;

  u.v = foo ();
  v.v = bar ();
  for (i = 0; i < sizeof (u.i) / sizeof (u.i[0]); i++)
    {
      if (u.i[i] != ~v.i[i])
	abort ();

      if (i < 3)
	{
	  if (u.i[i] != (TYPED (0x11111111) << i))
	    abort ();
	}
      else if (u.i[i])
	abort ();
    }
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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> vector<i32, 4> [linkage=external] [abi=sysv64() -> direct] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: vector<i32, 4> [storage=automatic] = aggregate<vector<i32, 4>, zero_fill=true>(index0 = const<i32>(286331153), index1 = const<i32>(572662306), index2 = const<i32>(1145324612));
// DEFAULT-NEXT:         return read<vector<i32, 4>>(%[[VALUE_x]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar() -> vector<i32, 4> [linkage=external] [abi=sysv64() -> direct] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_2:[0-9]+]] x: vector<i32, 4> [storage=automatic] = aggregate<vector<i32, 4>, zero_fill=true>(index0 = const<i32>(286331153), index1 = const<i32>(572662306), index2 = const<i32>(1145324612));
// DEFAULT-NEXT:         return not<vector<i32, 4>, elementwise=true>(read<vector<i32, 4>>(%[[VALUE_x_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_u:[0-9]+]] u: @type[[TYPE0]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_v:[0-9]+]] v: @type[[TYPE0]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         write<vector<i32, 4>>(field0(%[[VALUE_u]]), call<vector<i32, 4>, signature=fn() -> vector<i32, 4>, abi=sysv64() -> direct>(%[[VALUE_foo]]));
// DEFAULT-NEXT:         write<vector<i32, 4>>(field0(%[[VALUE_v]]), call<vector<i32, 4>, signature=fn() -> vector<i32, 4>, abi=sysv64() -> direct>(%[[VALUE_bar]]));
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
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(field1(%[[VALUE_u]])), read<i32>(%[[VALUE_i]])))), not<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(field1(%[[VALUE_v]])), read<i32>(%[[VALUE_i]]))))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     if lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(3))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(field1(%[[VALUE_u]])), read<i32>(%[[VALUE_i]])))), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(286331153), read<i32>(%[[VALUE_i]])))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(field1(%[[VALUE_u]])), read<i32>(%[[VALUE_i]])))), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
