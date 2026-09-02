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

// LOWERING-DAG: struct Item
// LOWERING-DAG: value: i32
// LOWERING-DAG: weight: i32

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: #[unsafe(no_mangle)]
// REWRITES-DAG: pub extern "C" fn run_a() -> i32 {
// REWRITES-DAG: let mut items: aligned::Aligned<aligned::A16, [Item; 2]> = aligned::Aligned(
// REWRITES-DAG: [Item {
// REWRITES-DAG: value: 0,
// REWRITES-DAG: weight: 0,
// REWRITES-DAG: }; 2],
// REWRITES-DAG: );
// REWRITES-DAG: let mut total: i32 = 0;
// REWRITES-DAG: *items = [
// REWRITES-DAG: Item {
// REWRITES-DAG: value: 1,
// REWRITES-DAG: weight: 10,
// REWRITES-DAG: },
// REWRITES-DAG: Item {
// REWRITES-DAG: value: 2,
// REWRITES-DAG: weight: 20,
// REWRITES-DAG: },
// REWRITES-DAG: ];
// REWRITES-DAG: total = 0;
// REWRITES-DAG: for i in 0..2 {
// REWRITES-DAG: total = total + items[((i as i64) as usize)].value * items[((i as i64) as usize)].weight;
// REWRITES-DAG: }
// REWRITES-DAG: return total;
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
