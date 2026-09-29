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
// DEFAULT-NEXT:     type @type[[TYPE_A:[0-9]+]] A = struct {
// DEFAULT-NEXT:         field0 t: i32;
// DEFAULT-NEXT:         field1 i: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_B:[0-9]+]] B = struct {
// DEFAULT-NEXT:         field0 p: ptr<i32>;
// DEFAULT-NEXT:         field1 b: f32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %[[VALUE_X:[0-9]+]] X: f32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_b:[0-9]+]] b: @type[[TYPE_B]], %[[VALUE_q:[0-9]+]] q: ptr<@type[[TYPE_A]]>, %[[VALUE_h:[0-9]+]] h: ptr<f32>) -> i32 [linkage=external] [abi=sysv64(native_c, scalar, scalar) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: f32 [synthetic] = read<f32>(%[[VALUE_X]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(read<f32>(%[[VALUE0]]), read<f32>(deref(read<ptr<f32>>(%[[VALUE_h]]))));
// DEFAULT-NEXT:         write<f32>(%[[VALUE_X]], read<f32>(%[[VALUE1]]));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(field0(%[[VALUE_b]]))), const<i32>(3));
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_q]]))), const<i32>(2));
// DEFAULT-NEXT:         return read<i32>(deref(read<ptr<i32>>(field0(%[[VALUE_b]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: @type[[TYPE_A]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_b_2:[0-9]+]] b: @type[[TYPE_B]] [storage=automatic];
// DEFAULT-NEXT:         write<ptr<i32>>(field0(%[[VALUE_b_2]]), addr_of<ptr<i32>>(field0(%[[VALUE_a]])));
// DEFAULT-NEXT:         if eq<i32>(call<i32, signature=fn(@type[[TYPE_B]], ptr<@type[[TYPE_A]]>, ptr<f32>) -> i32, abi=sysv64(native_c, scalar, scalar) -> scalar>(%[[VALUE_foo]], copy<@type[[TYPE_B]], reason=arg>(read<@type[[TYPE_B]]>(%[[VALUE_b_2]])), addr_of<ptr<@type[[TYPE_A]]>>(%[[VALUE_a]]), addr_of<ptr<f32>>(%[[VALUE_X]])), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
