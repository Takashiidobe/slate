#include <stdbool.h>
#include <stdio.h>

static int gnu_builtin_atomic(void) {
  int value = 0;
  int expected;
  int total = 0;
  bool flag = false;
  __atomic_store_n(&value, 10, __ATOMIC_SEQ_CST);
  total += __atomic_load_n(&value, __ATOMIC_SEQ_CST);
  total += __atomic_exchange_n(&value, 20, __ATOMIC_SEQ_CST);
  expected = 20;
  total += __atomic_compare_exchange_n(&value, &expected, 30, false,
                                       __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
  total += __atomic_fetch_add(&value, 5, __ATOMIC_SEQ_CST);
  total += __atomic_fetch_sub(&value, 3, __ATOMIC_SEQ_CST);
  total += __atomic_fetch_and(&value, 31, __ATOMIC_SEQ_CST);
  total += __atomic_fetch_or(&value, 64, __ATOMIC_SEQ_CST);
  total += __atomic_fetch_xor(&value, 3, __ATOMIC_SEQ_CST);
  total += __atomic_fetch_nand(&value, 127, __ATOMIC_SEQ_CST);
  value = 10;
  total += __atomic_add_fetch(&value, 2, __ATOMIC_SEQ_CST);
  total += __atomic_sub_fetch(&value, 3, __ATOMIC_SEQ_CST);
  total += __atomic_and_fetch(&value, 7, __ATOMIC_SEQ_CST);
  total += __atomic_or_fetch(&value, 16, __ATOMIC_SEQ_CST);
  total += __atomic_xor_fetch(&value, 3, __ATOMIC_SEQ_CST);
  total += __atomic_nand_fetch(&value, 31, __ATOMIC_SEQ_CST);
  total += !__atomic_test_and_set(&flag, __ATOMIC_SEQ_CST);
  total += __atomic_test_and_set(&flag, __ATOMIC_SEQ_CST);
  __atomic_clear(&flag, __ATOMIC_SEQ_CST);
  total += !flag;
  __atomic_thread_fence(__ATOMIC_SEQ_CST);
  __atomic_signal_fence(__ATOMIC_SEQ_CST);
  total += __atomic_always_lock_free(sizeof(int), &value);
  total += __atomic_is_lock_free(sizeof(int), &value);
  return total;
}

static int gnu_builtin_sync(void) {
  int value = 10;
  int total = 0;
  total += __sync_fetch_and_add(&value, 2);
  total += __sync_fetch_and_sub(&value, 3);
  total += __sync_fetch_and_and(&value, 7);
  total += __sync_fetch_and_or(&value, 16);
  total += __sync_fetch_and_xor(&value, 3);
  total += __sync_fetch_and_nand(&value, 31);
  value = 10;
  total += __sync_add_and_fetch(&value, 2);
  total += __sync_sub_and_fetch(&value, 3);
  total += __sync_bool_compare_and_swap(&value, 9, 20);
  total += __sync_val_compare_and_swap(&value, 20, 30);
  total += __sync_lock_test_and_set(&value, 40);
  __sync_lock_release(&value);
  total += value;
  total += __sync_swap(&value, 50);
  __sync_synchronize();
  return total;
}

int main(void) {
  printf("%d %d\n", gnu_builtin_atomic(), gnu_builtin_sync());
  return 0;
}
