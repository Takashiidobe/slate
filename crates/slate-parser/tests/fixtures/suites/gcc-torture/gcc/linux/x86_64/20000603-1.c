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
// DEFAULT-NEXT:     type @type[[TYPE_s1:[0-9]+]] s1 = struct {
// DEFAULT-NEXT:         field0 d: f64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_s2:[0-9]+]] s2 = struct {
// DEFAULT-NEXT:         field0 d: f64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_u:[0-9]+]] u = union {
// DEFAULT-NEXT:         field0 x: @type[[TYPE_s1]];
// DEFAULT-NEXT:         field1 y: @type[[TYPE_s2]];
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_a:[0-9]+]] a: ptr<@type[[TYPE_s1]]>, %[[VALUE_b:[0-9]+]] b: ptr<@type[[TYPE_s2]]>) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<f64>(field0(deref(read<ptr<@type[[TYPE_s1]]>>(%[[VALUE_a]]))), const<f64>(1.0));
// DEFAULT-NEXT:         return add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(field0(deref(read<ptr<@type[[TYPE_s2]]>>(%[[VALUE_b]])))), const<f64>(1.0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a_2:[0-9]+]] a: @type[[TYPE_u]] [storage=automatic];
// DEFAULT-NEXT:         write<f64>(field0(field0(%[[VALUE_a_2]])), const<f64>(0.0));
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(ptr<@type[[TYPE_s1]]>, ptr<@type[[TYPE_s2]]>) -> f64>(%[[VALUE_f]], addr_of<ptr<@type[[TYPE_s1]]>>(field0(%[[VALUE_a_2]])), addr_of<ptr<@type[[TYPE_s2]]>>(field1(%[[VALUE_a_2]]))), const<f64>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
