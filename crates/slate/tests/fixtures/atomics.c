#include <stdatomic.h>
#include <stdio.h>

int main(void) {
  atomic_int a = 0;

  atomic_store(&a, 100);
  int loaded = atomic_load(&a);

  int fa   = atomic_fetch_add(&a, 5);    // returns 100, a = 105
  int fs   = atomic_fetch_sub(&a, 10);   // returns 105, a = 95
  int fand = atomic_fetch_and(&a, 0x3C); // 95 & 60 = 28
  int forr = atomic_fetch_or(&a, 0x01);  // 28 | 1 = 29
  int fxor = atomic_fetch_xor(&a, 0x0F); // 29 ^ 15 = 18

  int xchg_old = atomic_exchange(&a, 7); // returns 18, a = 7

  int expected = 7;
  int ok = atomic_compare_exchange_strong(&a, &expected, 42); // success, a = 42
  int expected2 = 999;
  int bad =
      atomic_compare_exchange_strong(&a, &expected2, 0); // fail, a stays 42

  atomic_thread_fence(memory_order_seq_cst);

  printf("%d %d %d %d %d %d %d %d %d %d %d %d\n", loaded, fa, fs, fand, forr,
         fxor, xchg_old, ok, expected, bad, expected2, (int)a);
  return 0;
}
// REWRITES-DAG: let a = std::sync::atomic::AtomicI32::new(0);
// REWRITES-DAG: a.store(100, std::sync::atomic::Ordering::SeqCst);
// REWRITES-DAG: let loaded: i32 = a.load(std::sync::atomic::Ordering::SeqCst);
// REWRITES-DAG: let fa: i32 = a.fetch_add(5, std::sync::atomic::Ordering::SeqCst);
// REWRITES-DAG: let xchg_old: i32 = a.swap(7, std::sync::atomic::Ordering::SeqCst);
// REWRITES-DAG: a.compare_exchange(expected, 42,
// REWRITES-DAG: expected = v;
// REWRITES-DAG: expected2 = v;
// REWRITES-NOT: _atomictmp
// REWRITES-NOT: atomic_temp
// REWRITES-NOT: cmpxchg_bool
// REWRITES-NOT: from_ptr
// REWRITES-NOT: let mut a
