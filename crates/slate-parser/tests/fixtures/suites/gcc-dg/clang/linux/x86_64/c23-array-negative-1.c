/* Test C2y constraint against negative array indices does not apply in
   C23.  */
/* { dg-do compile } */
/* { dg-options "-std=c23 -pedantic-errors" } */

int a[1], b[10];
struct s { int a[2]; } x;
void *p;

void
f ()
{
  (void) a[0];
  (void) a[1];
  (void) a[12345];
  (void) a[-1];
  (void) a[-__LONG_LONG_MAX__];
  (void) b[0];
  (void) b[10];
  (void) b[12345];
  (void) b[-1];
  (void) b[-__LONG_LONG_MAX__];
  (void) x.a[0];
  (void) x.a[1];
  (void) x.a[12345];
  (void) x.a[-1];
  (void) x.a[-__LONG_LONG_MAX__];
  int c[1];
  (void) c[0];
  (void) c[1];
  (void) c[12345];
  (void) c[-1];
  (void) c[-__LONG_LONG_MAX__];
  (void) (*(int (*)[1]) p)[0];
  (void) (*(int (*)[1]) p)[1];
  (void) (*(int (*)[1]) p)[12345];
  (void) (*(int (*)[1]) p)[-1];
  (void) (*(int (*)[1]) p)[-__LONG_LONG_MAX__];
  /* This index is not an integer constant expression, so the constraint
     against negative indices does not apply even in C2y.  */
  (void) a[__LONG_LONG_MAX__ + 2];
  /* { dg-warning "integer overflow in expression" "overflow" { target *-*-* } .-1 } */
  /* Likewise, this is only an arithmetic constant expression, not an integer
     constant expression.  */
  (void) a[(int)-1.0];
}

// SLATE-FILECHECK-STD DEFAULT c23
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
// DEFAULT-NEXT:     type @type[[TYPE_s:[0-9]+]] s = struct {
// DEFAULT-NEXT:         field0 a: array<i32, 2>;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: array<i32, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: array<i32, 10> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_x:[0-9]+]] x: @type[[TYPE_s]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_p:[0-9]+]] p: ptr<void> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_a]]), const<i32>(0))));
// DEFAULT-NEXT:         read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_a]]), const<i32>(1))));
// DEFAULT-NEXT:         read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_a]]), const<i32>(12345))));
// DEFAULT-NEXT:         read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_a]]), neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_a]]), neg<i64, overflow=ub>(const<i64>(9223372036854775807)))));
// DEFAULT-NEXT:         read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(10)>(%[[VALUE_b]]), const<i32>(0))));
// DEFAULT-NEXT:         read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(10)>(%[[VALUE_b]]), const<i32>(10))));
// DEFAULT-NEXT:         read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(10)>(%[[VALUE_b]]), const<i32>(12345))));
// DEFAULT-NEXT:         read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(10)>(%[[VALUE_b]]), neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(10)>(%[[VALUE_b]]), neg<i64, overflow=ub>(const<i64>(9223372036854775807)))));
// DEFAULT-NEXT:         read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field0(%[[VALUE_x]])), const<i32>(0))));
// DEFAULT-NEXT:         read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field0(%[[VALUE_x]])), const<i32>(1))));
// DEFAULT-NEXT:         read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field0(%[[VALUE_x]])), const<i32>(12345))));
// DEFAULT-NEXT:         read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field0(%[[VALUE_x]])), neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field0(%[[VALUE_x]])), neg<i64, overflow=ub>(const<i64>(9223372036854775807)))));
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: array<i32, 1> [storage=automatic];
// DEFAULT-NEXT:         read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_c]]), const<i32>(0))));
// DEFAULT-NEXT:         read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_c]]), const<i32>(1))));
// DEFAULT-NEXT:         read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_c]]), const<i32>(12345))));
// DEFAULT-NEXT:         read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_c]]), neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_c]]), neg<i64, overflow=ub>(const<i64>(9223372036854775807)))));
// DEFAULT-NEXT:         read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(deref(pointer_cast<ptr<array<i32, 1>>, reason=explicit>(read<ptr<void>>(%[[VALUE_p]])))), const<i32>(0))));
// DEFAULT-NEXT:         read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(deref(pointer_cast<ptr<array<i32, 1>>, reason=explicit>(read<ptr<void>>(%[[VALUE_p]])))), const<i32>(1))));
// DEFAULT-NEXT:         read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(deref(pointer_cast<ptr<array<i32, 1>>, reason=explicit>(read<ptr<void>>(%[[VALUE_p]])))), const<i32>(12345))));
// DEFAULT-NEXT:         read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(deref(pointer_cast<ptr<array<i32, 1>>, reason=explicit>(read<ptr<void>>(%[[VALUE_p]])))), neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(deref(pointer_cast<ptr<array<i32, 1>>, reason=explicit>(read<ptr<void>>(%[[VALUE_p]])))), neg<i64, overflow=ub>(const<i64>(9223372036854775807)))));
// DEFAULT-NEXT:         read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_a]]), add<i64, overflow=ub>(const<i64>(9223372036854775807), widen<i64, reason=usual_arith>(const<i32>(2))))));
// DEFAULT-NEXT:         read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_a]]), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(neg<f64>(const<f64>(1.0))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
