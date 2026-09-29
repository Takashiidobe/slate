/* Test that sibling call is not used if there is an argument overlap.  */

extern void abort(void);

struct S {
  int a, b, c;
};

int foo2(struct S x, struct S y) {
  if (x.a != 3 || x.b != 4 || x.c != 5)
    abort();
  if (y.a != 6 || y.b != 7 || y.c != 8)
    abort();
  return 0;
}

int foo3(struct S x, struct S y, struct S z) {
  foo2(x, y);
  if (z.a != 9 || z.b != 10 || z.c != 11)
    abort();
  return 0;
}

int bar2(struct S x, struct S y) { return foo2(y, x); }

int bar3(struct S x, struct S y, struct S z) { return foo3(y, x, z); }

int baz3(struct S x, struct S y, struct S z) { return foo3(y, z, x); }

int main(void) {
  struct S a = {3, 4, 5}, b = {6, 7, 8}, c = {9, 10, 11};

  bar2(b, a);
  bar3(b, a, c);
  baz3(c, a, b);
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
// DEFAULT-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:         field2 c: i32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo2:[0-9]+]] @foo2(%[[VALUE_x:[0-9]+]] x: @type[[TYPE_S]], %[[VALUE_y:[0-9]+]] y: @type[[TYPE_S]]) -> i32 [linkage=external] [abi=sysv64(native_c, native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(field0(%[[VALUE_x]])), const<i32>(3)), ne<i32>(read<i32>(field1(%[[VALUE_x]])), const<i32>(4))), ne<i32>(read<i32>(field2(%[[VALUE_x]])), const<i32>(5)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(field0(%[[VALUE_y]])), const<i32>(6)), ne<i32>(read<i32>(field1(%[[VALUE_y]])), const<i32>(7))), ne<i32>(read<i32>(field2(%[[VALUE_y]])), const<i32>(8)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo3:[0-9]+]] @foo3(%[[VALUE_x_2:[0-9]+]] x: @type[[TYPE_S]], %[[VALUE_y_2:[0-9]+]] y: @type[[TYPE_S]], %[[VALUE_z:[0-9]+]] z: @type[[TYPE_S]]) -> i32 [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<i32, signature=fn(@type[[TYPE_S]], @type[[TYPE_S]]) -> i32, abi=sysv64(native_c, native_c) -> scalar>(%[[VALUE_foo2]], copy<@type[[TYPE_S]], reason=arg>(read<@type[[TYPE_S]]>(%[[VALUE_x_2]])), copy<@type[[TYPE_S]], reason=arg>(read<@type[[TYPE_S]]>(%[[VALUE_y_2]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(field0(%[[VALUE_z]])), const<i32>(9)), ne<i32>(read<i32>(field1(%[[VALUE_z]])), const<i32>(10))), ne<i32>(read<i32>(field2(%[[VALUE_z]])), const<i32>(11)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar2:[0-9]+]] @bar2(%[[VALUE_x_3:[0-9]+]] x: @type[[TYPE_S]], %[[VALUE_y_3:[0-9]+]] y: @type[[TYPE_S]]) -> i32 [linkage=external] [abi=sysv64(native_c, native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(@type[[TYPE_S]], @type[[TYPE_S]]) -> i32, abi=sysv64(native_c, native_c) -> scalar>(%[[VALUE_foo2]], copy<@type[[TYPE_S]], reason=arg>(read<@type[[TYPE_S]]>(%[[VALUE_y_3]])), copy<@type[[TYPE_S]], reason=arg>(read<@type[[TYPE_S]]>(%[[VALUE_x_3]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar3:[0-9]+]] @bar3(%[[VALUE_x_4:[0-9]+]] x: @type[[TYPE_S]], %[[VALUE_y_4:[0-9]+]] y: @type[[TYPE_S]], %[[VALUE_z_2:[0-9]+]] z: @type[[TYPE_S]]) -> i32 [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(@type[[TYPE_S]], @type[[TYPE_S]], @type[[TYPE_S]]) -> i32, abi=sysv64(native_c, native_c, native_c) -> scalar>(%[[VALUE_foo3]], copy<@type[[TYPE_S]], reason=arg>(read<@type[[TYPE_S]]>(%[[VALUE_y_4]])), copy<@type[[TYPE_S]], reason=arg>(read<@type[[TYPE_S]]>(%[[VALUE_x_4]])), copy<@type[[TYPE_S]], reason=arg>(read<@type[[TYPE_S]]>(%[[VALUE_z_2]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_baz3:[0-9]+]] @baz3(%[[VALUE_x_5:[0-9]+]] x: @type[[TYPE_S]], %[[VALUE_y_5:[0-9]+]] y: @type[[TYPE_S]], %[[VALUE_z_3:[0-9]+]] z: @type[[TYPE_S]]) -> i32 [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(@type[[TYPE_S]], @type[[TYPE_S]], @type[[TYPE_S]]) -> i32, abi=sysv64(native_c, native_c, native_c) -> scalar>(%[[VALUE_foo3]], copy<@type[[TYPE_S]], reason=arg>(read<@type[[TYPE_S]]>(%[[VALUE_y_5]])), copy<@type[[TYPE_S]], reason=arg>(read<@type[[TYPE_S]]>(%[[VALUE_z_3]])), copy<@type[[TYPE_S]], reason=arg>(read<@type[[TYPE_S]]>(%[[VALUE_x_5]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: @type[[TYPE_S]] [storage=automatic] = aggregate<@type[[TYPE_S]], zero_fill=false>(field0 = const<i32>(3), field1 = const<i32>(4), field2 = const<i32>(5));
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: @type[[TYPE_S]] [storage=automatic] = aggregate<@type[[TYPE_S]], zero_fill=false>(field0 = const<i32>(6), field1 = const<i32>(7), field2 = const<i32>(8));
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: @type[[TYPE_S]] [storage=automatic] = aggregate<@type[[TYPE_S]], zero_fill=false>(field0 = const<i32>(9), field1 = const<i32>(10), field2 = const<i32>(11));
// DEFAULT-NEXT:         call<i32, signature=fn(@type[[TYPE_S]], @type[[TYPE_S]]) -> i32, abi=sysv64(native_c, native_c) -> scalar>(%[[VALUE_bar2]], copy<@type[[TYPE_S]], reason=arg>(read<@type[[TYPE_S]]>(%[[VALUE_b]])), copy<@type[[TYPE_S]], reason=arg>(read<@type[[TYPE_S]]>(%[[VALUE_a]])));
// DEFAULT-NEXT:         call<i32, signature=fn(@type[[TYPE_S]], @type[[TYPE_S]], @type[[TYPE_S]]) -> i32, abi=sysv64(native_c, native_c, native_c) -> scalar>(%[[VALUE_bar3]], copy<@type[[TYPE_S]], reason=arg>(read<@type[[TYPE_S]]>(%[[VALUE_b]])), copy<@type[[TYPE_S]], reason=arg>(read<@type[[TYPE_S]]>(%[[VALUE_a]])), copy<@type[[TYPE_S]], reason=arg>(read<@type[[TYPE_S]]>(%[[VALUE_c]])));
// DEFAULT-NEXT:         call<i32, signature=fn(@type[[TYPE_S]], @type[[TYPE_S]], @type[[TYPE_S]]) -> i32, abi=sysv64(native_c, native_c, native_c) -> scalar>(%[[VALUE_baz3]], copy<@type[[TYPE_S]], reason=arg>(read<@type[[TYPE_S]]>(%[[VALUE_c]])), copy<@type[[TYPE_S]], reason=arg>(read<@type[[TYPE_S]]>(%[[VALUE_a]])), copy<@type[[TYPE_S]], reason=arg>(read<@type[[TYPE_S]]>(%[[VALUE_b]])));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
