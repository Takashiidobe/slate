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
// REWRITES-DAG: pub extern "C-unwind" fn run_a() -> i32 {
// REWRITES-DAG:     let mut items: aligned::Aligned<aligned::A16, [Item; 2]> = aligned::Aligned(
// REWRITES-DAG:         [Item {
// REWRITES-DAG:             value: 0,
// REWRITES-DAG:             weight: 0,
// REWRITES-DAG:         }; 2],
// REWRITES-DAG:     );
// REWRITES-DAG:     let mut total: i32 = 0;
// REWRITES-DAG:     let {{__v[0-9]+}}: [Item; 2] = [
// REWRITES-DAG:         Item {
// REWRITES-DAG:             value: 1,
// REWRITES-DAG:             weight: 10,
// REWRITES-DAG:         },
// REWRITES-DAG:         Item {
// REWRITES-DAG:             value: 2,
// REWRITES-DAG:             weight: 20,
// REWRITES-DAG:         },
// REWRITES-DAG:     ];
// REWRITES-DAG:     *items = {{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:     total = {{__v[0-9]+}};
// REWRITES-DAG:     let mut i: i32 = 0;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:     i = {{__v[0-9]+}};
// REWRITES-DAG:     loop {
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = i;
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 2;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// REWRITES-DAG:         if !{{__v[0-9]+}} {
// REWRITES-DAG:             break;
// REWRITES-DAG:         }
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = i;
// REWRITES-DAG:         let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = items[({{__v[0-9]+}} as usize)].value;
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = i;
// REWRITES-DAG:         let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = items[({{__v[0-9]+}} as usize)].weight;
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = total;
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// REWRITES-DAG:         total = {{__v[0-9]+}};
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = i;
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// REWRITES-DAG:         i = {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = total;
// REWRITES-DAG:     {{__v[0-9]+}}
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
