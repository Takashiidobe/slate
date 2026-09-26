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
// DEFAULT-NEXT:     type @type0 S = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:         field2 c: i32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @foo2(%3 x: @type0, %4 y: @type0) -> i32 [linkage=external] [abi=sysv64(coerce<i64, i32>, coerce<i64, i32>) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(field0(%3)), const<i32>(3)), ne<i32>(read<i32>(field1(%3)), const<i32>(4))), ne<i32>(read<i32>(field2(%3)), const<i32>(5)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(field0(%4)), const<i32>(6)), ne<i32>(read<i32>(field1(%4)), const<i32>(7))), ne<i32>(read<i32>(field2(%4)), const<i32>(8)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @foo3(%6 x: @type0, %7 y: @type0, %8 z: @type0) -> i32 [linkage=external] [abi=sysv64(coerce<i64, i32>, coerce<i64, i32>, coerce<i64, i32>) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<i32, signature=fn(@type0, @type0) -> i32, abi=sysv64(coerce<i64, i32>, coerce<i64, i32>) -> scalar>(%2, copy<@type0, reason=arg>(read<@type0>(%6)), copy<@type0, reason=arg>(read<@type0>(%7)));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(field0(%8)), const<i32>(9)), ne<i32>(read<i32>(field1(%8)), const<i32>(10))), ne<i32>(read<i32>(field2(%8)), const<i32>(11)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @bar2(%10 x: @type0, %11 y: @type0) -> i32 [linkage=external] [abi=sysv64(coerce<i64, i32>, coerce<i64, i32>) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(@type0, @type0) -> i32, abi=sysv64(coerce<i64, i32>, coerce<i64, i32>) -> scalar>(%2, copy<@type0, reason=arg>(read<@type0>(%11)), copy<@type0, reason=arg>(read<@type0>(%10)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @bar3(%13 x: @type0, %14 y: @type0, %15 z: @type0) -> i32 [linkage=external] [abi=sysv64(coerce<i64, i32>, coerce<i64, i32>, coerce<i64, i32>) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(@type0, @type0, @type0) -> i32, abi=sysv64(coerce<i64, i32>, coerce<i64, i32>, coerce<i64, i32>) -> scalar>(%5, copy<@type0, reason=arg>(read<@type0>(%14)), copy<@type0, reason=arg>(read<@type0>(%13)), copy<@type0, reason=arg>(read<@type0>(%15)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @baz3(%17 x: @type0, %18 y: @type0, %19 z: @type0) -> i32 [linkage=external] [abi=sysv64(coerce<i64, i32>, coerce<i64, i32>, coerce<i64, i32>) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(@type0, @type0, @type0) -> i32, abi=sysv64(coerce<i64, i32>, coerce<i64, i32>, coerce<i64, i32>) -> scalar>(%5, copy<@type0, reason=arg>(read<@type0>(%18)), copy<@type0, reason=arg>(read<@type0>(%19)), copy<@type0, reason=arg>(read<@type0>(%17)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %21 a: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = const<i32>(3), field1 = const<i32>(4), field2 = const<i32>(5));
// DEFAULT-NEXT:         let %22 b: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = const<i32>(6), field1 = const<i32>(7), field2 = const<i32>(8));
// DEFAULT-NEXT:         let %23 c: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = const<i32>(9), field1 = const<i32>(10), field2 = const<i32>(11));
// DEFAULT-NEXT:         call<i32, signature=fn(@type0, @type0) -> i32, abi=sysv64(coerce<i64, i32>, coerce<i64, i32>) -> scalar>(%9, copy<@type0, reason=arg>(read<@type0>(%22)), copy<@type0, reason=arg>(read<@type0>(%21)));
// DEFAULT-NEXT:         call<i32, signature=fn(@type0, @type0, @type0) -> i32, abi=sysv64(coerce<i64, i32>, coerce<i64, i32>, coerce<i64, i32>) -> scalar>(%12, copy<@type0, reason=arg>(read<@type0>(%22)), copy<@type0, reason=arg>(read<@type0>(%21)), copy<@type0, reason=arg>(read<@type0>(%23)));
// DEFAULT-NEXT:         call<i32, signature=fn(@type0, @type0, @type0) -> i32, abi=sysv64(coerce<i64, i32>, coerce<i64, i32>, coerce<i64, i32>) -> scalar>(%16, copy<@type0, reason=arg>(read<@type0>(%23)), copy<@type0, reason=arg>(read<@type0>(%21)), copy<@type0, reason=arg>(read<@type0>(%22)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
