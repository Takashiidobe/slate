/* Test C23 constexpr.  Valid code, execution test.  */
/* { dg-do run } */
/* { dg-options "-std=c23 -pedantic-errors" } */
/* { dg-additional-sources "c23-constexpr-2b.c" } */

extern void abort(void);
extern void exit(int);

/* constexpr objects at file scope have internal linkage.  */
constexpr int a = 2;

struct s {
  int   a;
  float b;
  int   c[3];
};
constexpr struct s s1 = {2, 3, {4, 5, 6}};
constexpr struct s s2 = s1;
struct s           s3 = s2;

void check(const struct s *p) {
  if (p->a != 2 || p->b != 3 || p->c[0] != 4 || p->c[1] != 5 || p->c[2] != 6)
    abort();
}

int main() {
  constexpr struct s s4 = s1;
  struct s           s5 = s4;
  constexpr struct s s6 = {s1.a, s2.b, {4, 5, 6}};
  check(&s1);
  check(&s2);
  check(&s3);
  check(&s4);
  check(&s5);
  check(&s6);
  exit(0);
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
// DEFAULT-NEXT:     type @type0 s = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: f32;
// DEFAULT-NEXT:         field2 c: array<i32, 3>;
// DEFAULT-NEXT:     } [size=20, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     global %2 a: i32 [storage=static] [const] [constexpr] = const<i32>(2) [linkage=internal];
// DEFAULT-NEXT:     global %4 s1: @type0 [storage=static] [const] [constexpr] = aggregate<@type0, zero_fill=false>(field0 = const<i32>(2), field1 = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(3)), field2 = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(4), index1 = const<i32>(5), index2 = const<i32>(6))) [linkage=internal];
// DEFAULT-NEXT:     global %5 s2: @type0 [storage=static] [const] [constexpr] = copy<@type0, reason=assign>(read<@type0>(%4)) [linkage=internal];
// DEFAULT-NEXT:     global %6 s3: @type0 [storage=static] = copy<@type0, reason=assign>(read<@type0>(%5)) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%13 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %7 @check(%8 p: ptr<const @type0>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(field0(deref(read<ptr<const @type0>>(%8)))), const<i32>(2)), ne<f32, exceptions=ignore>(read<f32>(field1(deref(read<ptr<const @type0>>(%8)))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(3)))), ne<i32>(read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<const i32>, length=Some(3)>(field2(deref(read<ptr<const @type0>>(%8)))), const<i32>(0)))), const<i32>(4))), ne<i32>(read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<const i32>, length=Some(3)>(field2(deref(read<ptr<const @type0>>(%8)))), const<i32>(1)))), const<i32>(5))), ne<i32>(read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<const i32>, length=Some(3)>(field2(deref(read<ptr<const @type0>>(%8)))), const<i32>(2)))), const<i32>(6)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %10 s4: @type0 [storage=automatic] [const] [constexpr] = copy<@type0, reason=assign>(read<@type0>(%4));
// DEFAULT-NEXT:         let %11 s5: @type0 [storage=automatic] = copy<@type0, reason=assign>(read<@type0>(%10));
// DEFAULT-NEXT:         let %12 s6: @type0 [storage=automatic] [const] [constexpr] = aggregate<@type0, zero_fill=false>(field0 = read<i32>(field0(%4)), field1 = read<f32>(field1(%5)), field2 = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(4), index1 = const<i32>(5), index2 = const<i32>(6)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const @type0>) -> void>(%7, addr_of<ptr<const @type0>>(%4));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const @type0>) -> void>(%7, addr_of<ptr<const @type0>>(%5));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const @type0>) -> void>(%7, pointer_cast<ptr<const @type0>, reason=arg>(addr_of<ptr<@type0>>(%6)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const @type0>) -> void>(%7, addr_of<ptr<const @type0>>(%10));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const @type0>) -> void>(%7, pointer_cast<ptr<const @type0>, reason=arg>(addr_of<ptr<@type0>>(%11)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const @type0>) -> void>(%7, addr_of<ptr<const @type0>>(%12));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
