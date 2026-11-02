int inner(int x) { return x; }

int outer(int n) {
  int acc = 0;
  static int calls;
  int inner(int x) {
    calls++;
    acc += x;
    return acc;
  }
  int twice(int x) {
    inner(x);
    return inner(x);
  }
  twice(n);
  return acc;
}

void declarators(void) {
  int (*nested(int x))(char) { return 0; }
  nested(1);
}

int levels(int n) {
  int base = n;
  int middle(int k) {
    int scale = k;
    int innermost(void) { return base * scale; }
    return innermost();
  }
  return middle(2);
}

int mutual(int n) {
  int depth = 0;
  auto int odd(int);
  int even(int k) {
    depth++;
    return k == 0 ? 1 : odd(k - 1);
  }
  int odd(int k) { return k == 0 ? 0 : even(k - 1); }
  return even(n) + depth;
}

int shadowed(int x) {
  int first(void) { return x; }
  {
    int first(void) { return x + 1; }
    return first();
  }
}

// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     global %[[VALUE_calls:[0-9]+]] calls: i32 [storage=static] [linkage=internal];
// IR-NEXT:     fn %[[VALUE_inner:[0-9]+]] @inner(%[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<i32>(%[[VALUE_x]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_outer:[0-9]+]] @outer(%[[VALUE_n:[0-9]+]] n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_acc:[0-9]+]] acc: i32 [storage=automatic] = const<i32>(0);
// IR-NEXT:         nested_fn %[[VALUE_inner_2:[0-9]+]] @inner(%[[VALUE_x_2:[0-9]+]] x: i32) -> i32 [captures=%[[VALUE_acc]]: i32] {
// IR-NEXT:             let %[[VALUE0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_calls]]);
// IR-NEXT:             let %[[VALUE1:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE0]]), const<i32>(1));
// IR-NEXT:             write<i32>(%[[VALUE_calls]], read<i32>(%[[VALUE1]]));
// IR-NEXT:             let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_acc]]);
// IR-NEXT:             let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE2]]), read<i32>(%[[VALUE_x_2]]));
// IR-NEXT:             write<i32>(%[[VALUE_acc]], read<i32>(%[[VALUE3]]));
// IR-NEXT:             return read<i32>(%[[VALUE_acc]]);
// IR-NEXT:         }
// IR-NEXT:         nested_fn %[[VALUE_twice:[0-9]+]] @twice(%[[VALUE_x_3:[0-9]+]] x: i32) -> i32 [captures=%[[VALUE_acc]]: i32] {
// IR-NEXT:             call<i32>(%[[VALUE_inner_2]], read<i32>(%[[VALUE_x_3]]));
// IR-NEXT:             return call<i32>(%[[VALUE_inner_2]], read<i32>(%[[VALUE_x_3]]));
// IR-NEXT:         }
// IR-NEXT:         call<i32>(%[[VALUE_twice]], read<i32>(%[[VALUE_n]]));
// IR-NEXT:         return read<i32>(%[[VALUE_acc]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_declarators:[0-9]+]] @declarators() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         nested_fn %[[VALUE_nested:[0-9]+]] @nested(%[[VALUE_x_4:[0-9]+]] x: i32) -> ptr<fn(i8) -> i32> [captures=] {
// IR-NEXT:             return null<ptr<fn(i8) -> i32>>;
// IR-NEXT:         }
// IR-NEXT:         call<ptr<fn(i8) -> i32>>(%[[VALUE_nested]], const<i32>(1));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_levels:[0-9]+]] @levels(%[[VALUE_n_2:[0-9]+]] n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_base:[0-9]+]] base: i32 [storage=automatic] = read<i32>(%[[VALUE_n_2]]);
// IR-NEXT:         nested_fn %[[VALUE_middle:[0-9]+]] @middle(%[[VALUE_k:[0-9]+]] k: i32) -> i32 [captures=%[[VALUE_base]]: i32] {
// IR-NEXT:             let %[[VALUE_scale:[0-9]+]] scale: i32 [storage=automatic] = read<i32>(%[[VALUE_k]]);
// IR-NEXT:             nested_fn %[[VALUE_innermost:[0-9]+]] @innermost() -> i32 [captures=%[[VALUE_base]]: i32, %[[VALUE_scale]]: i32] {
// IR-NEXT:                 return mul<i32>(read<i32>(%[[VALUE_base]]), read<i32>(%[[VALUE_scale]]));
// IR-NEXT:             }
// IR-NEXT:             return call<i32>(%[[VALUE_innermost]]);
// IR-NEXT:         }
// IR-NEXT:         return call<i32>(%[[VALUE_middle]], const<i32>(2));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_mutual:[0-9]+]] @mutual(%[[VALUE_n_3:[0-9]+]] n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_depth:[0-9]+]] depth: i32 [storage=automatic] = const<i32>(0);
// IR-NEXT:         nested_fn %[[VALUE_even:[0-9]+]] @even(%[[VALUE_k_2:[0-9]+]] k: i32) -> i32 [captures=%[[VALUE_depth]]: i32] {
// IR-NEXT:             let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_depth]]);
// IR-NEXT:             let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE4]]), const<i32>(1));
// IR-NEXT:             write<i32>(%[[VALUE_depth]], read<i32>(%[[VALUE5]]));
// IR-NEXT:             let %[[VALUE6:[0-9]+]]: i32 [synthetic];
// IR-NEXT:             if eq<i32>(read<i32>(%[[VALUE_k_2]]), const<i32>(0))
// IR-NEXT:                 write<i32>(%[[VALUE6]], const<i32>(1));
// IR-NEXT:             else
// IR-NEXT:                 write<i32>(%[[VALUE6]], call<i32>(%[[VALUE_odd:[0-9]+]], sub<i32>(read<i32>(%[[VALUE_k_2]]), const<i32>(1))));
// IR-NEXT:             return read<i32>(%[[VALUE6]]);
// IR-NEXT:         }
// IR-NEXT:         nested_fn %[[VALUE_odd]] @odd(%[[VALUE_k_3:[0-9]+]] k: i32) -> i32 [captures=%[[VALUE_depth]]: i32] {
// IR-NEXT:             let %[[VALUE7:[0-9]+]]: i32 [synthetic];
// IR-NEXT:             if eq<i32>(read<i32>(%[[VALUE_k_3]]), const<i32>(0))
// IR-NEXT:                 write<i32>(%[[VALUE7]], const<i32>(0));
// IR-NEXT:             else
// IR-NEXT:                 write<i32>(%[[VALUE7]], call<i32>(%[[VALUE_even]], sub<i32>(read<i32>(%[[VALUE_k_3]]), const<i32>(1))));
// IR-NEXT:             return read<i32>(%[[VALUE7]]);
// IR-NEXT:         }
// IR-NEXT:         return add<i32>(call<i32>(%[[VALUE_even]], read<i32>(%[[VALUE_n_3]])), read<i32>(%[[VALUE_depth]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_shadowed:[0-9]+]] @shadowed(%[[VALUE_x_5:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         nested_fn %[[VALUE_first:[0-9]+]] @first() -> i32 [captures=%[[VALUE_x_5]]: i32] {
// IR-NEXT:             return read<i32>(%[[VALUE_x_5]]);
// IR-NEXT:         }
// IR-NEXT:         {
// IR-NEXT:             nested_fn %[[VALUE_first_2:[0-9]+]] @first() -> i32 [captures=%[[VALUE_x_5]]: i32] {
// IR-NEXT:                 return add<i32>(read<i32>(%[[VALUE_x_5]]), const<i32>(1));
// IR-NEXT:             }
// IR-NEXT:             return call<i32>(%[[VALUE_first_2]]);
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
