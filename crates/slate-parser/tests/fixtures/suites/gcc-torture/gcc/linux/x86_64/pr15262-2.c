/* PR 15262.  Similar to pr15262-1.c but with no obvious addresses
   being taken in function foo().  Without IPA, by only looking inside
   foo() we cannot tell for certain whether 'q' and 'b' alias each
   other.  */

void abort(void);

struct A {
  int t;
  int i;
};

struct B {
  int  *p;
  float b;
};

float X;

int foo(struct B b, struct A *q, float *h) {
  X      += *h;
  *(b.p)  = 3;
  q->t    = 2;
  return *(b.p);
}

int main(void) {
  struct A a;
  struct B b;

  b.p = &a.t;
  if (foo(b, &a, &X) == 3)
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
// DEFAULT-NEXT:     type @type1 B = struct {
// DEFAULT-NEXT:         field0 p: ptr<i32>;
// DEFAULT-NEXT:         field1 b: f32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %3 X: f32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %4 @foo(%5 b: @type1, %6 q: ptr<@type0>, %7 h: ptr<f32>) -> i32 [linkage=external] [abi=sysv64(coerce<i64, f32>, scalar, scalar) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %11: f32 [synthetic] = read<f32>(%3);
// DEFAULT-NEXT:         let %12: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(read<f32>(%11), read<f32>(deref(read<ptr<f32>>(%7))));
// DEFAULT-NEXT:         write<f32>(%3, read<f32>(%12));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(field0(%5))), const<i32>(3));
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type0>>(%6))), const<i32>(2));
// DEFAULT-NEXT:         return read<i32>(deref(read<ptr<i32>>(field0(%5))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %9 a: @type0 [storage=automatic];
// DEFAULT-NEXT:         let %10 b: @type1 [storage=automatic];
// DEFAULT-NEXT:         write<ptr<i32>>(field0(%10), addr_of<ptr<i32>>(field0(%9)));
// DEFAULT-NEXT:         if eq<i32>(call<i32, signature=fn(@type1, ptr<@type0>, ptr<f32>) -> i32, abi=sysv64(coerce<i64, f32>, scalar, scalar) -> scalar>(%4, copy<@type1, reason=arg>(read<@type1>(%10)), addr_of<ptr<@type0>>(%9), addr_of<ptr<f32>>(%3)), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
