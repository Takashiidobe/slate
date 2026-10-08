/* { dg-do run { target bitint575 } } */
/* { dg-options "-std=gnu2y" } */

#define assert(e)  ((e) ? (void) 0 : __builtin_abort ())

void limits (void);

int
main (void)
{
  limits ();
}

void
limits (void)
{
  unsigned _BitInt (500) u;
  _BitInt (500) i;

  u = 0;
  u--;

  assert (_Maxof (unsigned _BitInt (500)) == u);
  assert (_Minof (unsigned _BitInt (500)) == 0);

  i = u >> 1;

  assert (_Maxof (_BitInt (500)) == i);
  assert (_Minof (_BitInt (500)) == -i-1);
}

void
type (void)
{
  _Generic (_Maxof (_BitInt (500)), _BitInt (500): 0);
  _Generic (_Minof (_BitInt (500)), _BitInt (500): 0);
  _Generic (_Maxof (unsigned _BitInt (500)), unsigned _BitInt (500): 0);
  _Generic (_Minof (unsigned _BitInt (500)), unsigned _BitInt (500): 0);
}

// SLATE-FILECHECK-STD DEFAULT gnu2y
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
// DEFAULT-NEXT:     fn %[[VALUE_limits:[0-9]+]] @limits() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_u:[0-9]+]] u: u500b [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i500b [storage=automatic];
// DEFAULT-NEXT:         write<u500b>(%[[VALUE_u]], reinterpret<u500b, reason=assign, fits=unknown>(widen<i500b, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: u500b [synthetic] = read<u500b>(%[[VALUE_u]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: u500b [synthetic] = sub<u500b, overflow=wrap>(read<u500b>(%[[VALUE0]]), reinterpret<u500b, reason=usual_arith, fits=unknown>(widen<i500b, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         write<u500b>(%[[VALUE_u]], read<u500b>(%[[VALUE1]]));
// DEFAULT-NEXT:         if eq<u500b>(const<u500b>(3273390607896141870013189696827599152216642046043064789483291368096133796404674554883270092325904157150886684127560071009217256545885393053328527589375), read<u500b>(%[[VALUE_u]]))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort:[0-9]+]]);
// DEFAULT-NEXT:         if eq<u500b>(const<u500b>(0), reinterpret<u500b, reason=usual_arith, fits=unknown>(widen<i500b, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         write<i500b>(%[[VALUE_i]], reinterpret<i500b, reason=assign, fits=unknown>(shr<u500b, amount_out_of_range=ub, fill=zero_extend>(read<u500b>(%[[VALUE_u]]), const<i32>(1))));
// DEFAULT-NEXT:         if eq<i500b>(const<i500b>(1636695303948070935006594848413799576108321023021532394741645684048066898202337277441635046162952078575443342063780035504608628272942696526664263794687), read<i500b>(%[[VALUE_i]]))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if eq<i500b>(const<i500b>(-1636695303948070935006594848413799576108321023021532394741645684048066898202337277441635046162952078575443342063780035504608628272942696526664263794688), sub<i500b, overflow=ub>(neg<i500b, overflow=ub>(read<i500b>(%[[VALUE_i]])), widen<i500b, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_limits]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_type:[0-9]+]] @type() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         const<i32>(0);
// DEFAULT-NEXT:         const<i32>(0);
// DEFAULT-NEXT:         const<i32>(0);
// DEFAULT-NEXT:         const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
