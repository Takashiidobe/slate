/* We used to mis-compile this testcase as we did not know that
   &a+offsetof(b,a) was the same as &a.b */

void abort(void);

struct A {
  int t;
  int i;
};

void bar(float *p) { *p = 5.2; }

int foo(struct A *locp, int i, int str) {
  float f, g, *p;
  int   T355;
  int  *T356;
  /* Currently, the alias analyzer has limited support for handling
     aliases of structure fields when no other variables are aliased.
     Introduce additional aliases to confuse it.  */
  p = i ? &g : &f;
  bar(p);
  if (*p > 0.0)
    str = 1;

  T355  = locp->i;
  T356  = &locp->i;
  *T356 = str;
  T355  = locp->i;

  return T355;
}

int main(void) {
  struct A loc;
  int      str;

  loc.i = 2;
  str   = foo(&loc, 10, 3);
  if (str != 1)
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
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_p:[0-9]+]] p: ptr<f32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<f32>(deref(read<ptr<f32>>(%[[VALUE_p]])), float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(5.2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_locp:[0-9]+]] locp: ptr<@type[[TYPE_A]]>, %[[VALUE_i:[0-9]+]] i: i32, %[[VALUE_str:[0-9]+]] str: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_f:[0-9]+]] f: f32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_g:[0-9]+]] g: f32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p_2:[0-9]+]] p: ptr<f32> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_T355:[0-9]+]] T355: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_T356:[0-9]+]] T356: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<f32>>(%[[VALUE_p_2]], conditional<ptr<f32>>(ne<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0)), addr_of<ptr<f32>>(%[[VALUE_g]]), addr_of<ptr<f32>>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<f32>) -> void>(%[[VALUE_bar]], read<ptr<f32>>(%[[VALUE_p_2]]));
// DEFAULT-NEXT:         if gt<f64, exceptions=observable>(float_widen<f64, reason=usual_arith>(read<f32>(deref(read<ptr<f32>>(%[[VALUE_p_2]])))), const<f64>(0.0))
// DEFAULT-NEXT:             write<i32>(%[[VALUE_str]], const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_T355]], read<i32>(field1(deref(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_locp]])))));
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_T356]], addr_of<ptr<i32>>(field1(deref(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_locp]])))));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE_T356]])), read<i32>(%[[VALUE_str]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_T355]], read<i32>(field1(deref(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_locp]])))));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_T355]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_loc:[0-9]+]] loc: @type[[TYPE_A]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_str_2:[0-9]+]] str: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(field1(%[[VALUE_loc]]), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_str_2]], call<i32, signature=fn(ptr<@type[[TYPE_A]]>, i32, i32) -> i32>(%[[VALUE_foo]], addr_of<ptr<@type[[TYPE_A]]>>(%[[VALUE_loc]]), const<i32>(10), const<i32>(3)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type[[TYPE_A]]>, i32, i32) -> i32>(%[[VALUE_foo]], addr_of<ptr<@type[[TYPE_A]]>>(%[[VALUE_loc]]), const<i32>(10), const<i32>(3));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_str_2]]), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
