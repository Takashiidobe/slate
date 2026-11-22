// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ARGS -fblocks --dump-ir --compact-ir

typedef int (^binop)(int, int);
typedef struct pair {
  int a;
  int b;
} pair;

int (^global_block)(void) = ^{ return 42; };
int *(^ptr_block)(int *) = ^int *(int *p) { return p; };
pair (^make_pair)(int) = ^(int v) {
  pair p = {v, v + 1};
  return p;
};

int run(binop op, int a, int b) { return op(a, b); }

binop make_adder(int k) {
  return ^int(int x, int y) { return x + y + k; };
}

int counter(void) {
  int base = 10;
  __block int total = 0;
  int (^add)(int, int) = ^(int x, int y) { return x + y + base; };
  void (^bump)(void) = ^{ total += 1; };
  bump();
  return run(add, 1, 2) + total;
}

int nested(int a) {
  int b = a * 2;
  int (^outer)(int) = ^(int c) {
    int (^inner)(int) = ^(int d) { return a + b + c + d; };
    return inner(c) + b;
  };
  return outer(3);
}

int conversions(binop f, void *p) {
  binop g = 0;
  binop h = f;
  void *raw = (void *)f;
  binop back = (binop)p;
  if (f && !g)
    return h == f;
  return f != 0 && raw != 0 && back == f;
}

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
// DEFAULT-NEXT:     type @type[[TYPE_binop:[0-9]+]] binop = block<fn(i32, i32) -> i32>;
// DEFAULT-NEXT:     type @type[[TYPE_pair:[0-9]+]] pair = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_pair_2:[0-9]+]] pair = @type[[TYPE_pair]];
// DEFAULT-NEXT:     global %[[VALUE_global_block:[0-9]+]] global_block: block<fn() -> i32> [storage=static] = block_literal<block<fn() -> i32>, %[[VALUE0:[0-9]+]], captures=[]> [linkage=external];
// DEFAULT-NEXT:     block_fn %[[VALUE0]]() -> i32 {
// DEFAULT-NEXT:         return const<i32>(42);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     global %[[VALUE_ptr_block:[0-9]+]] ptr_block: block<fn(ptr<i32>) -> ptr<i32>> [storage=static] = block_literal<block<fn(ptr<i32>) -> ptr<i32>>, %[[VALUE1:[0-9]+]], captures=[]> [linkage=external];
// DEFAULT-NEXT:     block_fn %[[VALUE1]](%[[VALUE_p:[0-9]+]] p: ptr<i32>) -> ptr<i32> {
// DEFAULT-NEXT:         return read<ptr<i32>>(%[[VALUE_p]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     global %[[VALUE_make_pair:[0-9]+]] make_pair: block<fn(i32) -> @type[[TYPE_pair]]> [storage=static] = block_literal<block<fn(i32) -> @type[[TYPE_pair]]>, %[[VALUE2:[0-9]+]], captures=[]> [c="pair (^)(int)"] [linkage=external] [c="pair (^)(int)"];
// DEFAULT-NEXT:     block_fn %[[VALUE2]](%[[VALUE_v:[0-9]+]] v: i32) -> @type[[TYPE_pair]] {
// DEFAULT-NEXT:         let %[[VALUE_p_2:[0-9]+]] p: @type[[TYPE_pair]] [storage=automatic] = aggregate<@type[[TYPE_pair]], zero_fill=false>(field0 = read<i32>(%[[VALUE_v]]), field1 = add<i32>(read<i32>(%[[VALUE_v]]), const<i32>(1))) [c="pair"];
// DEFAULT-NEXT:         return copy<@type[[TYPE_pair]], reason=return>(read<@type[[TYPE_pair]]>(%[[VALUE_p_2]]) [c="pair"]) [c="pair"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_run:[0-9]+]] @run(%[[VALUE_op:[0-9]+]] op: block<fn(i32, i32) -> i32> [c="binop"], %[[VALUE_a:[0-9]+]] a: i32, %[[VALUE_b:[0-9]+]] b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] [c="int(binop, int, int)"] {
// DEFAULT-NEXT:         return call<i32>(read<block<fn(i32, i32) -> i32>>(%[[VALUE_op]]) [c="binop"], read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_make_adder:[0-9]+]] @make_adder(%[[VALUE_k:[0-9]+]] k: i32) -> block<fn(i32, i32) -> i32> [linkage=external] [fallthrough=ub_if_used] [c="binop(int)"] {
// DEFAULT-NEXT:         return block_literal<block<fn(i32, i32) -> i32>, %[[VALUE3:[0-9]+]], captures=[%[[VALUE_k]]]> [c="binop"];
// DEFAULT-NEXT:         block_fn %[[VALUE3]](%[[VALUE_x:[0-9]+]] x: i32, %[[VALUE_y:[0-9]+]] y: i32) -> i32 {
// DEFAULT-NEXT:             return add<i32>(add<i32>(read<i32>(%[[VALUE_x]]), read<i32>(%[[VALUE_y]])), read<i32>(%[[VALUE_k]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_counter:[0-9]+]] @counter() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_base:[0-9]+]] base: i32 [storage=automatic] = const<i32>(10);
// DEFAULT-NEXT:         let %[[VALUE_total:[0-9]+]] total: i32 [storage=automatic] [byref] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_add:[0-9]+]] add: block<fn(i32, i32) -> i32> [storage=automatic] = block_literal<block<fn(i32, i32) -> i32>, %[[VALUE4:[0-9]+]], captures=[%[[VALUE_base]]]>;
// DEFAULT-NEXT:         block_fn %[[VALUE4]](%[[VALUE_x_2:[0-9]+]] x: i32, %[[VALUE_y_2:[0-9]+]] y: i32) -> i32 {
// DEFAULT-NEXT:             return add<i32>(add<i32>(read<i32>(%[[VALUE_x_2]]), read<i32>(%[[VALUE_y_2]])), read<i32>(%[[VALUE_base]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %[[VALUE_bump:[0-9]+]] bump: block<fn() -> void> [storage=automatic] = block_literal<block<fn() -> void>, %[[VALUE5:[0-9]+]], captures=[%[[VALUE_total]] byref]>;
// DEFAULT-NEXT:         block_fn %[[VALUE5]]() -> void {
// DEFAULT-NEXT:             let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:             let %[[VALUE7:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE6]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE7]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         call<void>(read<block<fn() -> void>>(%[[VALUE_bump]]));
// DEFAULT-NEXT:         return add<i32>(call<i32>(%[[VALUE_run]], read<block<fn(i32, i32) -> i32>>(%[[VALUE_add]]) [c="binop"], const<i32>(1), const<i32>(2)), read<i32>(%[[VALUE_total]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_nested:[0-9]+]] @nested(%[[VALUE_a_2:[0-9]+]] a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_b_2:[0-9]+]] b: i32 [storage=automatic] = mul<i32>(read<i32>(%[[VALUE_a_2]]), const<i32>(2));
// DEFAULT-NEXT:         let %[[VALUE_outer:[0-9]+]] outer: block<fn(i32) -> i32> [storage=automatic] = block_literal<block<fn(i32) -> i32>, %[[VALUE8:[0-9]+]], captures=[%[[VALUE_a_2]], %[[VALUE_b_2]]]>;
// DEFAULT-NEXT:         block_fn %[[VALUE8]](%[[VALUE_c:[0-9]+]] c: i32) -> i32 {
// DEFAULT-NEXT:             let %[[VALUE_inner:[0-9]+]] inner: block<fn(i32) -> i32> [storage=automatic] = block_literal<block<fn(i32) -> i32>, %[[VALUE9:[0-9]+]], captures=[%[[VALUE_a_2]], %[[VALUE_b_2]], %[[VALUE_c]]]>;
// DEFAULT-NEXT:             block_fn %[[VALUE9]](%[[VALUE_d:[0-9]+]] d: i32) -> i32 {
// DEFAULT-NEXT:                 return add<i32>(add<i32>(add<i32>(read<i32>(%[[VALUE_a_2]]), read<i32>(%[[VALUE_b_2]])), read<i32>(%[[VALUE_c]])), read<i32>(%[[VALUE_d]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             return add<i32>(call<i32>(read<block<fn(i32) -> i32>>(%[[VALUE_inner]]), read<i32>(%[[VALUE_c]])), read<i32>(%[[VALUE_b_2]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return call<i32>(read<block<fn(i32) -> i32>>(%[[VALUE_outer]]), const<i32>(3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_conversions:[0-9]+]] @conversions(%[[VALUE_f:[0-9]+]] f: block<fn(i32, i32) -> i32> [c="binop"], %[[VALUE_p_3:[0-9]+]] p: ptr<void>) -> i32 [linkage=external] [fallthrough=ub_if_used] [c="int(binop, void *)"] {
// DEFAULT-NEXT:         let %[[VALUE_g:[0-9]+]] g: block<fn(i32, i32) -> i32> [storage=automatic] = null<block<fn(i32, i32) -> i32>> [c="binop"] [c="binop"];
// DEFAULT-NEXT:         let %[[VALUE_h:[0-9]+]] h: block<fn(i32, i32) -> i32> [storage=automatic] = read<block<fn(i32, i32) -> i32>>(%[[VALUE_f]]) [c="binop"] [c="binop"];
// DEFAULT-NEXT:         let %[[VALUE_raw:[0-9]+]] raw: ptr<void> [storage=automatic] = pointer_cast<ptr<void>>(read<block<fn(i32, i32) -> i32>>(%[[VALUE_f]]) [c="binop"]);
// DEFAULT-NEXT:         let %[[VALUE_back:[0-9]+]] back: block<fn(i32, i32) -> i32> [storage=automatic] = pointer_cast<block<fn(i32, i32) -> i32>>(read<ptr<void>>(%[[VALUE_p_3]])) [c="binop"] [c="binop"];
// DEFAULT-NEXT:         if logical_and<bool>(ne<block<fn(i32, i32) -> i32>>(read<block<fn(i32, i32) -> i32>>(%[[VALUE_f]]) [c="binop"], null<block<fn(i32, i32) -> i32>>), not<bool>(ne<block<fn(i32, i32) -> i32>>(read<block<fn(i32, i32) -> i32>>(%[[VALUE_g]]) [c="binop"], null<block<fn(i32, i32) -> i32>>)))
// DEFAULT-NEXT:             return from_bool<i32>(eq<block<fn(i32, i32) -> i32>>(read<block<fn(i32, i32) -> i32>>(%[[VALUE_h]]) [c="binop"], read<block<fn(i32, i32) -> i32>>(%[[VALUE_f]]) [c="binop"]));
// DEFAULT-NEXT:         return from_bool<i32>(logical_and<bool>(logical_and<bool>(ne<block<fn(i32, i32) -> i32>>(read<block<fn(i32, i32) -> i32>>(%[[VALUE_f]]) [c="binop"], null<block<fn(i32, i32) -> i32>> [c="binop"]), ne<ptr<void>>(read<ptr<void>>(%[[VALUE_raw]]), null<ptr<void>>)), eq<block<fn(i32, i32) -> i32>>(read<block<fn(i32, i32) -> i32>>(%[[VALUE_back]]) [c="binop"], read<block<fn(i32, i32) -> i32>>(%[[VALUE_f]]) [c="binop"])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
