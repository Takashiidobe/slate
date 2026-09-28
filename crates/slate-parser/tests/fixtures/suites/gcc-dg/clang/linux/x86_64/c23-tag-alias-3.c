/* { dg-do run { target lto } }
 * { dg-options "-std=c23 -O2" }
 */

/* These tests check that definitions of enums with 
 * the same underlying type can alias, even when
 * they are not compatible.  */

enum bar : long { A = 1, B = 3 };

int test_bar(enum bar *a, void *b) {
  *a = A;

  enum foo : long { C = 2, D = 4 } *p = b;
  *p                                  = B;

  return *a;
}

int main() {
  enum bar z;

  if (B != test_bar(&z, &z))
    __builtin_abort();

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
// DEFAULT-NEXT:     type @type0 bar = enum : i64 {
// DEFAULT-NEXT:         %0 A = const<@type0>(1);
// DEFAULT-NEXT:         %1 B = const<@type0>(3);
// DEFAULT-NEXT:     } [size=8, align=8];
// DEFAULT-NEXT:     type @type1 foo = enum : i64 {
// DEFAULT-NEXT:         %0 C = const<@type1>(2);
// DEFAULT-NEXT:         %1 D = const<@type1>(4);
// DEFAULT-NEXT:     } [size=8, align=8];
// DEFAULT-NEXT:     fn %3 @test_bar(%4 a: ptr<@type0>, %5 b: ptr<void>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<@type0>(deref(read<ptr<@type0>>(%4)), const<@type0>(1));
// DEFAULT-NEXT:         let %9 p: ptr<@type1> [storage=automatic] = pointer_cast<ptr<@type1>, reason=assign>(read<ptr<void>>(%5));
// DEFAULT-NEXT:         write<@type1>(deref(read<ptr<@type1>>(%9)), int_to_enum<@type1, reason=assign>(enum_to_int<i64, reason=promotion>(const<@type0>(3))));
// DEFAULT-NEXT:         return truncate<i32, reason=return, fits=unknown>(enum_to_int<i64, reason=promotion>(read<@type0>(deref(read<ptr<@type0>>(%4)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %11 z: @type0 [storage=automatic];
// DEFAULT-NEXT:         if ne<i64>(enum_to_int<i64, reason=promotion>(const<@type0>(3)), widen<i64, reason=usual_arith>(call<i32, signature=fn(ptr<@type0>, ptr<void>) -> i32>(%3, addr_of<ptr<@type0>>(%11), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type0>>(%11)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%12);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
