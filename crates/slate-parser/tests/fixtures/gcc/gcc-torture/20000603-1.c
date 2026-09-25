/* It is not clear whether this test is conforming.  See DR#236
   http://www.open-std.org/jtc1/sc22/wg14/www/docs/dr_236.htm.  However,
   there seems to be consensus that the presence of a union to aggregate
   struct s1 and struct s2 should make it conforming.  */
void abort(void);

struct s1 {
  double d;
};
struct s2 {
  double d;
};
union u {
  struct s1 x;
  struct s2 y;
};

double f(struct s1 *a, struct s2 *b) {
  a->d = 1.0;
  return b->d + 1.0;
}

int main() {
  union u a;
  a.x.d = 0.0;
  if (f(&a.x, &a.y) != 2.0)
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
// DEFAULT-NEXT:     type @type0 s1 = struct {
// DEFAULT-NEXT:         field0 d: f64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type1 s2 = struct {
// DEFAULT-NEXT:         field0 d: f64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type2 u = union {
// DEFAULT-NEXT:         field0 x: @type0;
// DEFAULT-NEXT:         field1 y: @type1;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %4 @f(%5 a: ptr<@type0>, %6 b: ptr<@type1>) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<f64>(field0(deref(read<ptr<@type0>>(%5))), const<f64>(1.0));
// DEFAULT-NEXT:         return add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(field0(deref(read<ptr<@type1>>(%6)))), const<f64>(1.0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %8 a: @type2 [storage=automatic];
// DEFAULT-NEXT:         write<f64>(field0(field0(%8)), const<f64>(0.0));
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(ptr<@type0>, ptr<@type1>) -> f64>(%4, addr_of<ptr<@type0>>(field0(%8)), addr_of<ptr<@type1>>(field1(%8))), const<f64>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
