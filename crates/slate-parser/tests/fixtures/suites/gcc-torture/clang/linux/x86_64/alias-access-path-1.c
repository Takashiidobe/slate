/* Test that variable
     int val;
   may hold value of tyope "struct c" which has same size.
   This is valid in GIMPLE memory model.  */

struct a {
  int val;
} a = {1}, a2;
struct b {
  struct a a;
};
int val;
struct c {
  struct b b;
} *cptr = (void *)&val;

int main(void) {
  cptr->b.a = a;
  val       = 2;
  a2        = cptr->b.a;
  if (a2.val == a.val)
    __builtin_abort();
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
// DEFAULT-NEXT:     type @type[[TYPE_a:[0-9]+]] a = struct {
// DEFAULT-NEXT:         field0 val: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_b:[0-9]+]] b = struct {
// DEFAULT-NEXT:         field0 a: @type[[TYPE_a]];
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_c:[0-9]+]] c = struct {
// DEFAULT-NEXT:         field0 b: @type[[TYPE_b]];
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: @type[[TYPE_a]] [storage=static] = aggregate<@type[[TYPE_a]], zero_fill=false>(field0 = const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_a2:[0-9]+]] a2: @type[[TYPE_a]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_val:[0-9]+]] val: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_cptr:[0-9]+]] cptr: ptr<@type[[TYPE_c]]> [storage=static] = pointer_cast<ptr<@type[[TYPE_c]]>, reason=assign>(pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<i32>>(%[[VALUE_val]]))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<@type[[TYPE_a]]>(field0(field0(deref(read<ptr<@type[[TYPE_c]]>>(%[[VALUE_cptr]])))), copy<@type[[TYPE_a]], reason=assign>(read<@type[[TYPE_a]]>(%[[VALUE_a]])));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_val]], const<i32>(2));
// DEFAULT-NEXT:         write<@type[[TYPE_a]]>(%[[VALUE_a2]], copy<@type[[TYPE_a]], reason=assign>(read<@type[[TYPE_a]]>(field0(field0(deref(read<ptr<@type[[TYPE_c]]>>(%[[VALUE_cptr]])))))));
// DEFAULT-NEXT:         if eq<i32>(read<i32>(field0(%[[VALUE_a2]])), read<i32>(field0(%[[VALUE_a]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
