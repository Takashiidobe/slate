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
// DEFAULT-NEXT:     type @type[[TYPE_A:[0-9]+]] A = struct {
// DEFAULT-NEXT:         field0 t: i32;
// DEFAULT-NEXT:         field1 i: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(3);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_malloc:[0-9]+]] @__builtin_malloc(%[[VALUE0:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_loc:[0-9]+]] loc: @type[[TYPE_A]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_locp:[0-9]+]] locp: ptr<@type[[TYPE_A]]> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_f:[0-9]+]] f: f32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_g:[0-9]+]] g: f32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<f32> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_T355:[0-9]+]] T355: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_T356:[0-9]+]] T356: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         write<f32>(%[[VALUE_f]], int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(3)));
// DEFAULT-NEXT:         write<f32>(%[[VALUE_g]], int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)));
// DEFAULT-NEXT:         write<ptr<f32>>(%[[VALUE_p]], conditional<ptr<f32>>(ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_foo]]), const<i32>(0)), addr_of<ptr<f32>>(%[[VALUE_g]]), addr_of<ptr<f32>>(%[[VALUE_f]])));
// DEFAULT-NEXT:         conditional<ptr<f32>>(ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_foo]]), const<i32>(0)), addr_of<ptr<f32>>(%[[VALUE_g]]), addr_of<ptr<f32>>(%[[VALUE_f]]));
// DEFAULT-NEXT:         if gt<f64, exceptions=observable>(float_widen<f64, reason=usual_arith>(read<f32>(deref(read<ptr<f32>>(%[[VALUE_p]])))), const<f64>(0.0))
// DEFAULT-NEXT:             write<f32>(%[[VALUE_g]], int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_A]]>>(%[[VALUE_locp]], pointer_cast<ptr<@type[[TYPE_A]]>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE___builtin_malloc]], const<u64>(8))));
// DEFAULT-NEXT:         pointer_cast<ptr<@type[[TYPE_A]]>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE___builtin_malloc]], const<u64>(8)));
// DEFAULT-NEXT:         write<i32>(field1(deref(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_locp]]))), const<i32>(10));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_T355]], read<i32>(field1(deref(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_locp]])))));
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_T356]], addr_of<ptr<i32>>(field1(deref(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_locp]])))));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE_T356]])), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_T355]], read<i32>(field1(deref(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_locp]])))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_T355]]), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
