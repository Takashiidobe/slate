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
// DEFAULT-NEXT:     type @type[[TYPE_Pair:[0-9]+]] Pair = struct {
// DEFAULT-NEXT:         field0 left: i32;
// DEFAULT-NEXT:         field1 right: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_Nested:[0-9]+]] Nested = struct {
// DEFAULT-NEXT:         field0 inner: @type[[TYPE_Pair]];
// DEFAULT-NEXT:         field1 tag: i32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_WithArray:[0-9]+]] WithArray = struct {
// DEFAULT-NEXT:         field0 data: array<i32, 3>;
// DEFAULT-NEXT:         field1 marker: i32;
// DEFAULT-NEXT:     } [size=16, align=4, offsets=[0, 12]];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_take_pair:[0-9]+]] @take_pair(%[[VALUE_p:[0-9]+]] p: @type[[TYPE_Pair]]) -> i32 [linkage=internal] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(mul<i32, overflow=ub>(read<i32>(field0(%[[VALUE_p]])), const<i32>(10)), read<i32>(field1(%[[VALUE_p]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_replace_left:[0-9]+]] @replace_left(%[[VALUE_p_2:[0-9]+]] p: @type[[TYPE_Pair]], %[[VALUE_v:[0-9]+]] v: i32) -> @type[[TYPE_Pair]] [linkage=internal] [abi=sysv64(native_c, scalar) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(field0(%[[VALUE_p_2]]), read<i32>(%[[VALUE_v]]));
// DEFAULT-NEXT:         return copy<@type[[TYPE_Pair]], reason=return>(read<@type[[TYPE_Pair]]>(%[[VALUE_p_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_nested_total:[0-9]+]] @nested_total(%[[VALUE_n:[0-9]+]] n: @type[[TYPE_Nested]]) -> i32 [linkage=internal] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(field0(field0(%[[VALUE_n]]))), read<i32>(field1(field0(%[[VALUE_n]])))), read<i32>(field1(%[[VALUE_n]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_array_value:[0-9]+]] @array_value(%[[VALUE_w:[0-9]+]] w: @type[[TYPE_WithArray]]) -> i32 [linkage=internal] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(field0(%[[VALUE_w]])), const<i32>(1)))), read<i32>(field1(%[[VALUE_w]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_p_3:[0-9]+]] p: @type[[TYPE_Pair]] [storage=automatic] = aggregate<@type[[TYPE_Pair]], zero_fill=false>(field0 = const<i32>(2), field1 = const<i32>(3));
// DEFAULT-NEXT:         let %[[VALUE_q:[0-9]+]] q: @type[[TYPE_Pair]] [storage=automatic] = copy<@type[[TYPE_Pair]], reason=assign>(call<@type[[TYPE_Pair]], signature=fn(@type[[TYPE_Pair]], i32) -> @type[[TYPE_Pair]], abi=sysv64(native_c, scalar) -> native_c>(%[[VALUE_replace_left]], copy<@type[[TYPE_Pair]], reason=arg>(read<@type[[TYPE_Pair]]>(%[[VALUE_p_3]])), const<i32>(7)));
// DEFAULT-NEXT:         let %[[VALUE_n_2:[0-9]+]] n: @type[[TYPE_Nested]] [storage=automatic] = aggregate<@type[[TYPE_Nested]], zero_fill=false>(field0 = aggregate<@type[[TYPE_Pair]], zero_fill=false>(field0 = const<i32>(4), field1 = const<i32>(5)), field1 = const<i32>(6));
// DEFAULT-NEXT:         let %[[VALUE_w_2:[0-9]+]] w: @type[[TYPE_WithArray]] [storage=automatic] = aggregate<@type[[TYPE_WithArray]], zero_fill=false>(field0 = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(8), index1 = const<i32>(9), index2 = const<i32>(10)), field1 = const<i32>(11));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str]])), call<i32, signature=fn(@type[[TYPE_Pair]]) -> i32, abi=sysv64(native_c) -> scalar>(%[[VALUE_take_pair]], copy<@type[[TYPE_Pair]], reason=arg>(read<@type[[TYPE_Pair]]>(%[[VALUE_p_3]]))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_2]])), call<i32, signature=fn(@type[[TYPE_Pair]]) -> i32, abi=sysv64(native_c) -> scalar>(%[[VALUE_take_pair]], copy<@type[[TYPE_Pair]], reason=arg>(read<@type[[TYPE_Pair]]>(%[[VALUE_q]]))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_3]])), call<i32, signature=fn(@type[[TYPE_Nested]]) -> i32, abi=sysv64(native_c) -> scalar>(%[[VALUE_nested_total]], copy<@type[[TYPE_Nested]], reason=arg>(read<@type[[TYPE_Nested]]>(%[[VALUE_n_2]]))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_4]])), call<i32, signature=fn(@type[[TYPE_WithArray]]) -> i32, abi=sysv64(native_c) -> scalar>(%[[VALUE_array_value]], copy<@type[[TYPE_WithArray]], reason=arg>(read<@type[[TYPE_WithArray]]>(%[[VALUE_w_2]]))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
