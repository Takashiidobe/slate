// @rewrite-fn-begin
int run_a(void) {
  struct Item {
    int value;
    int weight;
  };

  struct Item items[] = {{1, 10}, {2, 20}};

  int total = 0;
  for (int i = 0; i < 2; i++) {
    total += items[i].value * items[i].weight;
  }
  return total;
}
// @rewrite-fn-end

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: #[unsafe(no_mangle)]
// REWRITES-DAG: pub extern "C" fn run_a() -> i32 {
// REWRITES-DAG: let mut __retval: i32 = 0;
// REWRITES-DAG: let mut items: aligned::Aligned<aligned::A16, [Item; 2]> = aligned::Aligned([Item { value: 0, weight: 0 }; 2]);
// REWRITES-DAG: let mut total: i32 = 0;
// REWRITES-DAG: *items = [Item { value: 1, weight: 10 }, Item { value: 2, weight: 20 }];
// REWRITES-DAG: total = 0;
// REWRITES-DAG: {
// REWRITES-DAG: let mut i: i32 = 0;
// REWRITES-DAG: i = 0;
// REWRITES-DAG: loop {
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = 2;
// REWRITES-DAG: if !(i < {{_v[0-9]+}}) {
// REWRITES-DAG: break;
// REWRITES-DAG: }
// REWRITES-DAG: {
// REWRITES-DAG: total = total + items[((i as i64) as usize)].value * items[((i as i64) as usize)].weight;
// REWRITES-DAG: }
// REWRITES-DAG: i = i + 1;
// REWRITES-DAG: }
// REWRITES-DAG: }
// REWRITES-DAG: __retval = total;
// REWRITES-DAG: return __retval;
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites

// LOWERING-DAG: struct Item
// LOWERING-DAG: value: i32
// LOWERING-DAG: weight: i32
