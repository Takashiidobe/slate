#include <stdio.h>

struct Pair {
  int left;
  int right;
};

struct Nested {
  struct Pair inner;
  int         tag;
};

struct WithArray {
  int data[3];
  int marker;
};

static int take_pair(struct Pair p) { return p.left * 10 + p.right; }

static struct Pair replace_left(struct Pair p, int v) {
  p.left = v;
  return p;
}

static int nested_total(struct Nested n) {
  return n.inner.left + n.inner.right + n.tag;
}

static int array_value(struct WithArray w) { return w.data[1] + w.marker; }

int main(void) {
  struct Pair      p = {2, 3};
  struct Pair      q = replace_left(p, 7);
  struct Nested    n = {{4, 5}, 6};
  struct WithArray w = {{8, 9, 10}, 11};
  printf("%d\n", take_pair(p));
  printf("%d\n", take_pair(q));
  printf("%d\n", nested_total(n));
  printf("%d\n", array_value(w));
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
// DEFAULT-NEXT:     type @type0 Pair = struct {
// DEFAULT-NEXT:         field0 left: i32;
// DEFAULT-NEXT:         field1 right: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type1 Nested = struct {
// DEFAULT-NEXT:         field0 inner: @type0;
// DEFAULT-NEXT:         field1 tag: i32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type2 WithArray = struct {
// DEFAULT-NEXT:         field0 data: array<i32, 3>;
// DEFAULT-NEXT:         field1 marker: i32;
// DEFAULT-NEXT:     } [size=16, align=4, offsets=[0, 12]];
// DEFAULT-NEXT:     global %19 .str19: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %20 .str20: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %21 .str21: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %22 .str22: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%18 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %4 @take_pair(%5 p: @type0) -> i32 [linkage=internal] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(mul<i32, overflow=ub>(read<i32>(field0(%5)), const<i32>(10)), read<i32>(field1(%5)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @replace_left(%7 p: @type0, %8 v: i32) -> @type0 [linkage=internal] [abi=sysv64(native_c, scalar) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(field0(%7), read<i32>(%8));
// DEFAULT-NEXT:         return copy<@type0, reason=return>(read<@type0>(%7));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @nested_total(%10 n: @type1) -> i32 [linkage=internal] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(field0(field0(%10))), read<i32>(field1(field0(%10)))), read<i32>(field1(%10)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @array_value(%12 w: @type2) -> i32 [linkage=internal] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(field0(%12)), const<i32>(1)))), read<i32>(field1(%12)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %14 p: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = const<i32>(2), field1 = const<i32>(3));
// DEFAULT-NEXT:         let %15 q: @type0 [storage=automatic] = copy<@type0, reason=assign>(call<@type0, signature=fn(@type0, i32) -> @type0, abi=sysv64(native_c, scalar) -> native_c>(%6, copy<@type0, reason=arg>(read<@type0>(%14)), const<i32>(7)));
// DEFAULT-NEXT:         let %16 n: @type1 [storage=automatic] = aggregate<@type1, zero_fill=false>(field0 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(4), field1 = const<i32>(5)), field1 = const<i32>(6));
// DEFAULT-NEXT:         let %17 w: @type2 [storage=automatic] = aggregate<@type2, zero_fill=false>(field0 = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(8), index1 = const<i32>(9), index2 = const<i32>(10)), field1 = const<i32>(11));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%19)), call<i32, signature=fn(@type0) -> i32, abi=sysv64(native_c) -> scalar>(%4, copy<@type0, reason=arg>(read<@type0>(%14))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%20)), call<i32, signature=fn(@type0) -> i32, abi=sysv64(native_c) -> scalar>(%4, copy<@type0, reason=arg>(read<@type0>(%15))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%21)), call<i32, signature=fn(@type1) -> i32, abi=sysv64(native_c) -> scalar>(%9, copy<@type1, reason=arg>(read<@type1>(%16))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%22)), call<i32, signature=fn(@type2) -> i32, abi=sysv64(native_c) -> scalar>(%11, copy<@type2, reason=arg>(read<@type2>(%17))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
