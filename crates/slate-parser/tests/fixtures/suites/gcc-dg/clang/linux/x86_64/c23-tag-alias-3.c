/* { dg-do run { target lto } }
 * { dg-options "-std=c23 -O2" }
 */

/* These tests check that definitions of enums with 
 * the same underlying type can alias, even when
 * they are not compatible.  */

enum bar : long { A = 1, B = 3 };

int test_bar(enum bar* a, void* b)
{
	*a = A;

	enum foo : long { C = 2, D = 4 }* p = b;
	*p = B;

	return *a;
}


int main()
{
	enum bar z;

	if (B != test_bar(&z, &z))
		__builtin_abort();

	return 0;
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
// DEFAULT-NEXT:     type @type[[TYPE_bar:[0-9]+]] bar = enum : i64 {
// DEFAULT-NEXT:         %[[VALUE_A:[0-9]+]] A = const<@type[[TYPE_bar]]>(1);
// DEFAULT-NEXT:         %[[VALUE_B:[0-9]+]] B = const<@type[[TYPE_bar]]>(3);
// DEFAULT-NEXT:     } [size=8, align=8];
// DEFAULT-NEXT:     type @type[[TYPE_foo:[0-9]+]] foo = enum : i64 {
// DEFAULT-NEXT:         %[[VALUE_A]] C = const<@type[[TYPE_foo]]>(2);
// DEFAULT-NEXT:         %[[VALUE_B]] D = const<@type[[TYPE_foo]]>(4);
// DEFAULT-NEXT:     } [size=8, align=8];
// DEFAULT-NEXT:     fn %[[VALUE_test_bar:[0-9]+]] @test_bar(%[[VALUE_a:[0-9]+]] a: ptr<@type[[TYPE_bar]]>, %[[VALUE_b:[0-9]+]] b: ptr<void>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<@type[[TYPE_bar]]>(deref(read<ptr<@type[[TYPE_bar]]>>(%[[VALUE_a]])), const<@type[[TYPE_bar]]>(1));
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_foo]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE_foo]]>, reason=assign>(read<ptr<void>>(%[[VALUE_b]]));
// DEFAULT-NEXT:         write<@type[[TYPE_foo]]>(deref(read<ptr<@type[[TYPE_foo]]>>(%[[VALUE_p]])), int_to_enum<@type[[TYPE_foo]], reason=assign>(enum_to_int<i64, reason=promotion>(const<@type[[TYPE_bar]]>(3))));
// DEFAULT-NEXT:         return truncate<i32, reason=return, fits=unknown>(enum_to_int<i64, reason=promotion>(read<@type[[TYPE_bar]]>(deref(read<ptr<@type[[TYPE_bar]]>>(%[[VALUE_a]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_z:[0-9]+]] z: @type[[TYPE_bar]] [storage=automatic];
// DEFAULT-NEXT:         if ne<i64>(enum_to_int<i64, reason=promotion>(const<@type[[TYPE_bar]]>(3)), widen<i64, reason=usual_arith>(call<i32, signature=fn(ptr<@type[[TYPE_bar]]>, ptr<void>) -> i32>(%[[VALUE_test_bar]], addr_of<ptr<@type[[TYPE_bar]]>>(%[[VALUE_z]]), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type[[TYPE_bar]]>>(%[[VALUE_z]])))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
