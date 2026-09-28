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
// DEFAULT-NEXT:     type @type0 a = struct {
// DEFAULT-NEXT:         field0 val: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type1 b = struct {
// DEFAULT-NEXT:         field0 a: @type0;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type2 c = struct {
// DEFAULT-NEXT:         field0 b: @type1;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     global %1 a: @type0 [storage=static] = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %2 a2: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 val: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 cptr: ptr<@type2> [storage=static] = pointer_cast<ptr<@type2>, reason=assign>(pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<i32>>(%4))) [linkage=external];
// DEFAULT-NEXT:     fn %8 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<@type0>(field0(field0(deref(read<ptr<@type2>>(%6)))), copy<@type0, reason=assign>(read<@type0>(%1)));
// DEFAULT-NEXT:         write<i32>(%4, const<i32>(2));
// DEFAULT-NEXT:         write<@type0>(%2, copy<@type0, reason=assign>(read<@type0>(field0(field0(deref(read<ptr<@type2>>(%6)))))));
// DEFAULT-NEXT:         if eq<i32>(read<i32>(field0(%2)), read<i32>(field0(%1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
