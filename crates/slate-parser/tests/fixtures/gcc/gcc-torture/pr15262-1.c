/* PR 15262.
   The alias analyzer only considers relations between pointers and
   symbols.  If two pointers P and Q point to the same symbol S, then
   their respective memory tags will either be the same or they will
   have S in their alias set.

   However, if there are no common symbols between P and Q, TBAA will
   currently miss their alias relationship altogether.  */

void abort(void);

struct A {
  int t;
  int i;
};

int foo() { return 3; }

int main(void) {
  struct A loc, *locp;
  float    f, g, *p;
  int      T355, *T356;

  /* Avoid the partial hack in TBAA that would consider memory tags if
     the program had no addressable symbols.  */
  f = 3;
  g = 2;
  p = foo() ? &g : &f;
  if (*p > 0.0)
    g = 1;

  /* Store into *locp and cache its current value.  */
  locp    = __builtin_malloc(sizeof(*locp));
  locp->i = 10;
  T355    = locp->i;

  /* Take the address of one of locp's fields and write to it.  */
  T356  = &locp->i;
  *T356 = 1;

  /* Read the recently stored value.  If TBAA fails, this will appear
     as a redundant load that will be replaced with '10'.  */
  T355 = locp->i;
  if (T355 != 1)
    abort();

  return 0;
}


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
// DEFAULT-NEXT:     type @type0 A = struct {
// DEFAULT-NEXT:         field0 t: i32;
// DEFAULT-NEXT:         field1 i: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @foo() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(3);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @__builtin_malloc(%11 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %4 loc: @type0 [storage=automatic];
// DEFAULT-NEXT:         let %5 locp: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:         let %6 f: f32 [storage=automatic];
// DEFAULT-NEXT:         let %7 g: f32 [storage=automatic];
// DEFAULT-NEXT:         let %8 p: ptr<f32> [storage=automatic];
// DEFAULT-NEXT:         let %9 T355: i32 [storage=automatic];
// DEFAULT-NEXT:         let %10 T356: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         write<f32>(%6, int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(3)));
// DEFAULT-NEXT:         write<f32>(%7, int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2)));
// DEFAULT-NEXT:         write<ptr<f32>>(%8, conditional<ptr<f32>>(ne<i32>(call<i32, signature=fn() -> i32>(%2), const<i32>(0)), addr_of<ptr<f32>>(%7), addr_of<ptr<f32>>(%6)));
// DEFAULT-NEXT:         conditional<ptr<f32>>(ne<i32>(call<i32, signature=fn() -> i32>(%2), const<i32>(0)), addr_of<ptr<f32>>(%7), addr_of<ptr<f32>>(%6));
// DEFAULT-NEXT:         if gt<f64, exceptions=ignore>(float_widen<f64, reason=usual_arith>(read<f32>(deref(read<ptr<f32>>(%8)))), const<f64>(0.0))
// DEFAULT-NEXT:             write<f32>(%7, int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<ptr<@type0>>(%5, pointer_cast<ptr<@type0>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%12, const<u64>(8))));
// DEFAULT-NEXT:         pointer_cast<ptr<@type0>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%12, const<u64>(8)));
// DEFAULT-NEXT:         write<i32>(field1(deref(read<ptr<@type0>>(%5))), const<i32>(10));
// DEFAULT-NEXT:         write<i32>(%9, read<i32>(field1(deref(read<ptr<@type0>>(%5)))));
// DEFAULT-NEXT:         write<ptr<i32>>(%10, addr_of<ptr<i32>>(field1(deref(read<ptr<@type0>>(%5)))));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%10)), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%9, read<i32>(field1(deref(read<ptr<@type0>>(%5)))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%9), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
