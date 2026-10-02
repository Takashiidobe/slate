#include <pthread.h>
#include <stdatomic.h>
#include <stdint.h>
#include <stdio.h>

#define redisAtomic _Atomic
#define atomicIncr(var, count) atomic_fetch_add_explicit(&var, (count), memory_order_relaxed)
#define atomicGetIncr(var, oldvalue_var, count)                                \
  do {                                                                         \
    oldvalue_var = atomic_fetch_add_explicit(&var, (count), memory_order_relaxed); \
  } while (0)
#define atomicDecr(var, count) atomic_fetch_sub_explicit(&var, (count), memory_order_relaxed)

struct vset {
  int pending;
  redisAtomic int in_use;
  redisAtomic size_t bytes;
  redisAtomic uint32_t epoch;
};

static redisAtomic size_t used_memory = 0;
static long long stat_net_input_bytes = 0;
static uint32_t plain_mask = 0x5a;
static uint64_t plain_counter = 10;
static redisAtomic int threads_done = 0;

struct used_memory_entry {
  redisAtomic long long used_memory;
  char padding[56];
};

static struct used_memory_entry entries[4];

static void *worker(void *arg) {
  struct vset *set = arg;
  for (int i = 0; i < 10000; i++) {
    set->in_use++;
    atomicIncr(used_memory, 3);
    __atomic_fetch_add(&plain_counter, 2, __ATOMIC_RELAXED);
    atomicIncr(entries[i & 3].used_memory, 1);
  }
  atomic_fetch_add(&threads_done, 1);
  return NULL;
}

static int try_claim(redisAtomic uint64_t *slot, uint64_t want) {
  uint64_t expected = 0;
  while (!atomic_compare_exchange_weak_explicit(slot, &expected, want, memory_order_relaxed,
                                                memory_order_relaxed)) {
    if (expected != 0)
      return 0;
  }
  return 1;
}

int main(void) {
  struct vset set = {0};
  set.pending = 3;
  set.in_use = 1;
  set.in_use--;
  set.bytes += 40;
  set.bytes -= 8;
  set.epoch = 7;

  pthread_t threads[4];
  for (int i = 0; i < 4; i++)
    pthread_create(&threads[i], NULL, worker, &set);
  for (int i = 0; i < 4; i++)
    pthread_join(threads[i], NULL);
  printf("%d %zu %llu %d\n", set.in_use, (size_t)used_memory,
         (unsigned long long)plain_counter, (int)threads_done);
  for (int i = 0; i < 4; i++)
    printf("%lld ", (long long)entries[i].used_memory);
  printf("\n");

  size_t old;
  atomicGetIncr(used_memory, old, 5);
  atomicDecr(used_memory, 2);
  printf("%zu %zu\n", old, (size_t)used_memory);

  long long added = __atomic_add_fetch(&stat_net_input_bytes, 100, __ATOMIC_ACQUIRE);
  long long subbed = __atomic_sub_fetch(&stat_net_input_bytes, 30, __ATOMIC_SEQ_CST);
  printf("%lld %lld\n", added, subbed);

  uint32_t previous = atomic_exchange(&set.epoch, 9);
  uint32_t orred = atomic_fetch_or(&set.epoch, 0x30);
  uint32_t anded = atomic_fetch_and_explicit(&set.epoch, 0x3c, memory_order_acq_rel);
  uint32_t xorred = atomic_fetch_xor(&set.epoch, 0xff);
  uint32_t nand = __atomic_fetch_nand(&plain_mask, 0xf0, __ATOMIC_RELAXED);
  uint32_t nand_new = __atomic_nand_fetch(&plain_mask, 0x0f, __ATOMIC_RELAXED);
  printf("%u %u %u %u %u %u %u %u\n", previous, orred, anded, xorred, nand, nand_new,
         (uint32_t)set.epoch, plain_mask);

  redisAtomic int scaled = 3;
  scaled *= 7;
  scaled <<= 2;
  scaled /= 3;
  int post = scaled++;
  int pre = --scaled;
  printf("%d %d %d\n", (int)scaled, post, pre);

  redisAtomic double ratio = 1.5;
  ratio += 2.25;
  ratio *= 2;
  redisAtomic float small = 0.5f;
  float small_old = small++;
  printf("%.3f %.3f %.3f\n", (double)ratio, (double)small, (double)small_old);

  int values[4] = {10, 20, 30, 40};
  int *_Atomic cursor = values;
  cursor++;
  ++cursor;
  cursor++;
  int *before = cursor--;
  printf("%d %d\n", *cursor, *before);

  redisAtomic uint64_t slot = 0;
  int first = try_claim(&slot, 42);
  int second = try_claim(&slot, 43);
  uint64_t seen = 5;
  _Bool strong = atomic_compare_exchange_strong(&slot, &seen, 1);
  printf("%d %d %d %llu %llu\n", first, second, strong, (unsigned long long)seen,
         (unsigned long long)slot);

  int legacy = 4;
  int swapped = __sync_bool_compare_and_swap(&legacy, 4, 8);
  int observed = __sync_val_compare_and_swap(&legacy, 1, 2);
  int fetched = __sync_fetch_and_add(&legacy, 3);
  int lock = __sync_lock_test_and_set(&legacy, 100);
  __sync_lock_release(&legacy);
  printf("%d %d %d %d %d\n", swapped, observed, fetched, lock, legacy);

  _Bool flag = 0;
  _Bool was = __atomic_test_and_set(&flag, __ATOMIC_SEQ_CST);
  _Bool now = __atomic_test_and_set(&flag, __ATOMIC_SEQ_CST);
  printf("%d %d %d\n", was, now, set.pending);
  return 0;
}
