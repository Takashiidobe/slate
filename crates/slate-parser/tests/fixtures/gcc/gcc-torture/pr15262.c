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
// DEFAULT-NEXT:     type @type0 A = struct {
// DEFAULT-NEXT:         field0 t: i32;
// DEFAULT-NEXT:         field1 i: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @bar(%3 p: ptr<f32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<f32>(deref(read<ptr<f32>>(%3)), float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(5.2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @foo(%5 locp: ptr<@type0>, %6 i: i32, %7 str: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %8 f: f32 [storage=automatic];
// DEFAULT-NEXT:         let %9 g: f32 [storage=automatic];
// DEFAULT-NEXT:         let %10 p: ptr<f32> [storage=automatic];
// DEFAULT-NEXT:         let %11 T355: i32 [storage=automatic];
// DEFAULT-NEXT:         let %12 T356: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<f32>>(%10, conditional<ptr<f32>>(ne<i32>(read<i32>(%6), const<i32>(0)), addr_of<ptr<f32>>(%9), addr_of<ptr<f32>>(%8)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<f32>) -> void>(%2, read<ptr<f32>>(%10));
// DEFAULT-NEXT:         if gt<f64, exceptions=ignore>(float_widen<f64, reason=usual_arith>(read<f32>(deref(read<ptr<f32>>(%10)))), const<f64>(0.0))
// DEFAULT-NEXT:             write<i32>(%7, const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%11, read<i32>(field1(deref(read<ptr<@type0>>(%5)))));
// DEFAULT-NEXT:         write<ptr<i32>>(%12, addr_of<ptr<i32>>(field1(deref(read<ptr<@type0>>(%5)))));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%12)), read<i32>(%7));
// DEFAULT-NEXT:         write<i32>(%11, read<i32>(field1(deref(read<ptr<@type0>>(%5)))));
// DEFAULT-NEXT:         return read<i32>(%11);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %14 loc: @type0 [storage=automatic];
// DEFAULT-NEXT:         let %15 str: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(field1(%14), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(%15, call<i32, signature=fn(ptr<@type0>, i32, i32) -> i32>(%4, addr_of<ptr<@type0>>(%14), const<i32>(10), const<i32>(3)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type0>, i32, i32) -> i32>(%4, addr_of<ptr<@type0>>(%14), const<i32>(10), const<i32>(3));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%15), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
