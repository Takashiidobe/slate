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

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn pthread_create(
// CHECK-NEXT:         [[ARG_0:arg[0-9]+]]: *mut u64,
// CHECK-NEXT:         [[ARG_1:arg[0-9]+]]: *const slate_support::pthread_attr_t,
// CHECK-NEXT:         [[ARG_2:arg[0-9]+]]: Option<unsafe extern "C-unwind" fn(*mut ()) -> *mut ()>,
// CHECK-NEXT:         [[ARG_3:arg[0-9]+]]: *mut (),
// CHECK-NEXT:     ) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct vset {
// CHECK-NEXT:     pending: i32,
// CHECK-NEXT:     in_use: i32,
// CHECK-NEXT:     bytes: u64,
// CHECK-NEXT:     epoch: u32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut used_memory: u64 = 0;
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: unsafe impl<T> Sync for __SlateAlign16<T> {}
// CHECK-EMPTY:
// CHECK-NEXT: static mut stat_net_input_bytes: i64 = 0;
// CHECK-EMPTY:
// CHECK-NEXT: static mut plain_mask: u32 = 90;
// CHECK-EMPTY:
// CHECK-NEXT: static mut plain_counter: u64 = 10;
// CHECK-EMPTY:
// CHECK-NEXT: static mut threads_done: i32 = 0;
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct used_memory_entry {
// CHECK-NEXT:     used_memory: i64,
// CHECK-NEXT:     padding: [i8; 56],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut entries: __SlateAlign16<[used_memory_entry; 4]> = __SlateAlign16(
// CHECK-NEXT:     unsafe { core::mem::zeroed() },
// CHECK-NEXT: );
// CHECK-EMPTY:
// CHECK-NEXT: use slate_support::pthread_join;
// CHECK-EMPTY:
// CHECK-NEXT: extern "C-unwind" fn worker(arg: *mut ()) -> *mut () {
// CHECK-NEXT:     let set: *mut vset = arg as *mut vset;
// CHECK-NEXT:     for i in 0i32..10000i32 {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                     core::ptr::addr_of_mut!((*set).in_use) as *mut i32,
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .fetch_add(1i32, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             core::sync::atomic::AtomicU64::from_ptr(
// CHECK-NEXT:                     core::ptr::addr_of_mut!(used_memory) as *mut u64,
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .fetch_add(3u64, core::sync::atomic::Ordering::Relaxed)
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             core::sync::atomic::AtomicU64::from_ptr(
// CHECK-NEXT:                     core::ptr::addr_of_mut!(plain_counter) as *mut u64,
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .fetch_add(2u64, core::sync::atomic::Ordering::Relaxed)
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             core::sync::atomic::AtomicI64::from_ptr(
// CHECK-NEXT:                     core::ptr::addr_of_mut!(entries.0[(i & 3) as usize].used_memory)
// CHECK-NEXT:                         as *mut i64,
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .fetch_add(1i64, core::sync::atomic::Ordering::Relaxed)
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(threads_done) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_add(1i32, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     };
// CHECK-NEXT:     core::ptr::null_mut::<()>()
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn try_claim(slot: *mut u64, want: u64) -> i32 {
// CHECK-NEXT:     let mut expected: u64 = 0;
// CHECK-NEXT:     while !unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]] = core::ptr::addr_of_mut!(expected);
// CHECK-NEXT:         let [[T_1:__t[0-9]+]] = want;
// CHECK-NEXT:         match core::sync::atomic::AtomicU64::from_ptr(slot as *mut u64)
// CHECK-NEXT:             .compare_exchange_weak(
// CHECK-NEXT:                 *[[T_0]],
// CHECK-NEXT:                 [[T_1]],
// CHECK-NEXT:                 core::sync::atomic::Ordering::Relaxed,
// CHECK-NEXT:                 core::sync::atomic::Ordering::Relaxed,
// CHECK-NEXT:             ) {
// CHECK-NEXT:             Ok(_) => true,
// CHECK-NEXT:             Err([[T_2:__t[0-9]+]]) => {
// CHECK-NEXT:                 *[[T_0]] = [[T_2]];
// CHECK-NEXT:                 false
// CHECK-NEXT:             },
// CHECK-NEXT:         }
// CHECK-NEXT:     } {
// CHECK-NEXT:         if expected != 0 {
// CHECK-NEXT:             return 0;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     1
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut set: vset = {
// CHECK-NEXT:         let mut [[T_0]]: vset = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].pending = 0i32;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     set.pending = 3;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(set.in_use) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .store(1i32, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(set.in_use) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_sub(1i32, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicU64::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(set.bytes) as *mut u64,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_add(40u64, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:             .wrapping_add(40)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicU64::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(set.bytes) as *mut u64,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_sub(8u64, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:             .wrapping_sub(8)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicU32::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(set.epoch) as *mut u32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .store(7u32, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut threads: __SlateAlign16<[u64; 4]> = __SlateAlign16([0u64; 4]);
// CHECK-NEXT:     for i in 0i32..4i32 {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             pthread_create(
// CHECK-NEXT:                 threads.0.as_mut_ptr().offset(i as isize),
// CHECK-NEXT:                 core::ptr::null::<slate_support::pthread_attr_t>(),
// CHECK-NEXT:                 Some(worker),
// CHECK-NEXT:                 core::ptr::addr_of_mut!(set) as *mut (),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     for i in 0i32..4i32 {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             pthread_join(
// CHECK-NEXT:                 threads.0[i as usize] as u64,
// CHECK-NEXT:                 core::ptr::null_mut::<*mut ()>() as *mut *mut core::ffi::c_void,
// CHECK-NEXT:             ) as i32
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%d %zu %llu %d\n".as_ptr(),
// CHECK-NEXT:             core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                     core::ptr::addr_of!(set.in_use) as *mut i32,
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .load(core::sync::atomic::Ordering::SeqCst),
// CHECK-NEXT:             core::sync::atomic::AtomicU64::from_ptr(
// CHECK-NEXT:                     core::ptr::addr_of!(used_memory) as *mut u64,
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .load(core::sync::atomic::Ordering::SeqCst),
// CHECK-NEXT:             plain_counter as u64,
// CHECK-NEXT:             core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                     core::ptr::addr_of!(threads_done) as *mut i32,
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .load(core::sync::atomic::Ordering::SeqCst),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     for i in 0i32..4i32 {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(
// CHECK-NEXT:                 c"%lld ".as_ptr(),
// CHECK-NEXT:                 core::sync::atomic::AtomicI64::from_ptr(
// CHECK-NEXT:                         core::ptr::addr_of!(entries.0[i as usize].used_memory)
// CHECK-NEXT:                             as *mut i64,
// CHECK-NEXT:                     )
// CHECK-NEXT:                     .load(core::sync::atomic::Ordering::SeqCst),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("");
// CHECK-NEXT:     let mut old: u64 = 0;
// CHECK-NEXT:     old = unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicU64::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(used_memory) as *mut u64,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_add(5u64, core::sync::atomic::Ordering::Relaxed)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicU64::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(used_memory) as *mut u64,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_sub(2u64, core::sync::atomic::Ordering::Relaxed)
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%zu %zu\n".as_ptr(),
// CHECK-NEXT:             old,
// CHECK-NEXT:             core::sync::atomic::AtomicU64::from_ptr(
// CHECK-NEXT:                     core::ptr::addr_of!(used_memory) as *mut u64,
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .load(core::sync::atomic::Ordering::SeqCst),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let added: i64 = unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicI64::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(stat_net_input_bytes) as *mut i64,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_add(100i64, core::sync::atomic::Ordering::Acquire)
// CHECK-NEXT:             .wrapping_add(100)
// CHECK-NEXT:     };
// CHECK-NEXT:     let subbed: i64 = unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicI64::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(stat_net_input_bytes) as *mut i64,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_sub(30i64, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:             .wrapping_sub(30)
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%lld %lld\n".as_ptr(), added, subbed) };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let previous: u32 = unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicU32::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(set.epoch) as *mut u32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .swap(9u32, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     };
// CHECK-NEXT:     let orred: u32 = unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicU32::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(set.epoch) as *mut u32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_or(48u32, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     };
// CHECK-NEXT:     let anded: u32 = unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicU32::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(set.epoch) as *mut u32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_and(60u32, core::sync::atomic::Ordering::AcqRel)
// CHECK-NEXT:     };
// CHECK-NEXT:     let xorred: u32 = unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicU32::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(set.epoch) as *mut u32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_xor(255u32, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     };
// CHECK-NEXT:     let nand: u32 = unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicU32::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(plain_mask) as *mut u32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_nand(240u32, core::sync::atomic::Ordering::Relaxed)
// CHECK-NEXT:     };
// CHECK-NEXT:     let nand_new: u32 = unsafe {
// CHECK-NEXT:         !(core::sync::atomic::AtomicU32::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(plain_mask) as *mut u32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_nand(15u32, core::sync::atomic::Ordering::Relaxed) & 15)
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%u %u %u %u %u %u %u %u\n".as_ptr(),
// CHECK-NEXT:             previous,
// CHECK-NEXT:             orred,
// CHECK-NEXT:             anded,
// CHECK-NEXT:             xorred,
// CHECK-NEXT:             nand,
// CHECK-NEXT:             nand_new,
// CHECK-NEXT:             core::sync::atomic::AtomicU32::from_ptr(
// CHECK-NEXT:                     core::ptr::addr_of!(set.epoch) as *mut u32,
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .load(core::sync::atomic::Ordering::SeqCst),
// CHECK-NEXT:             plain_mask,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let mut scaled: i32 = 3;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_3:__t[0-9]+]] = core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(scaled) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_update(
// CHECK-NEXT:                 core::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 core::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 |[[T_4:__t[0-9]+]]| Some([[T_4]] * 7),
// CHECK-NEXT:             )
// CHECK-NEXT:             .unwrap_or_else(|[[T_4]]| [[T_4]]);
// CHECK-NEXT:         [[T_3]] * 7
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_5:__t[0-9]+]] = core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(scaled) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_update(
// CHECK-NEXT:                 core::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 core::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 |[[T_6:__t[0-9]+]]| Some([[T_6]] << 2i32),
// CHECK-NEXT:             )
// CHECK-NEXT:             .unwrap_or_else(|[[T_6]]| [[T_6]]);
// CHECK-NEXT:         [[T_5]] << 2i32
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_7:__t[0-9]+]] = core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(scaled) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_update(
// CHECK-NEXT:                 core::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 core::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 |[[T_8:__t[0-9]+]]| Some([[T_8]] / 3),
// CHECK-NEXT:             )
// CHECK-NEXT:             .unwrap_or_else(|[[T_8]]| [[T_8]]);
// CHECK-NEXT:         [[T_7]] / 3
// CHECK-NEXT:     };
// CHECK-NEXT:     let post: i32 = unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(scaled) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_add(1i32, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     };
// CHECK-NEXT:     let pre: i32 = unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(scaled) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_sub(1i32, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:             .wrapping_sub(1)
// CHECK-NEXT:     };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}",
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                     core::ptr::addr_of!(scaled) as *mut i32,
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .load(core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:         },
// CHECK-NEXT:         post,
// CHECK-NEXT:         pre
// CHECK-NEXT:     );
// CHECK-NEXT:     let mut ratio: f64 = 1.5f64;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_9:__t[0-9]+]] = core::sync::atomic::AtomicU64::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(ratio) as *mut u64,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_update(
// CHECK-NEXT:                 core::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 core::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 |[[T_10:__t[0-9]+]]| Some((f64::from_bits([[T_10]]) + 2.25f64).to_bits()),
// CHECK-NEXT:             )
// CHECK-NEXT:             .unwrap_or_else(|[[T_10]]| [[T_10]]);
// CHECK-NEXT:         f64::from_bits([[T_9]]) + 2.25f64
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_11:__t[0-9]+]] = core::sync::atomic::AtomicU64::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(ratio) as *mut u64,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_update(
// CHECK-NEXT:                 core::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 core::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 |[[T_12:__t[0-9]+]]| Some((f64::from_bits([[T_12]]) * (2i32 as f64)).to_bits()),
// CHECK-NEXT:             )
// CHECK-NEXT:             .unwrap_or_else(|[[T_12]]| [[T_12]]);
// CHECK-NEXT:         f64::from_bits([[T_11]]) * (2i32 as f64)
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut small: f32 = 0.5f32;
// CHECK-NEXT:     let small_old: f32 = unsafe {
// CHECK-NEXT:         f32::from_bits(
// CHECK-NEXT:             core::sync::atomic::AtomicU32::from_ptr(
// CHECK-NEXT:                     core::ptr::addr_of_mut!(small) as *mut u32,
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .fetch_update(
// CHECK-NEXT:                     core::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                     core::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                     |[[T_13:__t[0-9]+]]| Some((f32::from_bits([[T_13]]) + (1i32 as f32)).to_bits()),
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .unwrap_or_else(|[[T_13]]| [[T_13]]),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%.3f %.3f %.3f\n".as_ptr(),
// CHECK-NEXT:             f64::from_bits(
// CHECK-NEXT:                 core::sync::atomic::AtomicU64::from_ptr(
// CHECK-NEXT:                         core::ptr::addr_of!(ratio) as *mut u64,
// CHECK-NEXT:                     )
// CHECK-NEXT:                     .load(core::sync::atomic::Ordering::SeqCst),
// CHECK-NEXT:             ),
// CHECK-NEXT:             f32::from_bits(
// CHECK-NEXT:                 core::sync::atomic::AtomicU32::from_ptr(
// CHECK-NEXT:                         core::ptr::addr_of!(small) as *mut u32,
// CHECK-NEXT:                     )
// CHECK-NEXT:                     .load(core::sync::atomic::Ordering::SeqCst),
// CHECK-NEXT:             ) as f64,
// CHECK-NEXT:             small_old as f64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let mut values: __SlateAlign16<[i32; 4]> = __SlateAlign16(
// CHECK-NEXT:         [10i32, 20i32, 30i32, 40i32],
// CHECK-NEXT:     );
// CHECK-NEXT:     let mut cursor: *mut i32 = values.0.as_mut_ptr();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicPtr::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(cursor) as *mut *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_update(
// CHECK-NEXT:                 core::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 core::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 |[[T_14:__t[0-9]+]]| Some(([[T_14]] as *mut i32).add(1) as *mut i32),
// CHECK-NEXT:             )
// CHECK-NEXT:             .unwrap_or_else(|[[T_14]]| [[T_14]]) as *mut i32
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_15:__t[0-9]+]] = core::sync::atomic::AtomicPtr::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(cursor) as *mut *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_update(
// CHECK-NEXT:                 core::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 core::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 |[[T_16:__t[0-9]+]]| Some(([[T_16]] as *mut i32).add(1) as *mut i32),
// CHECK-NEXT:             )
// CHECK-NEXT:             .unwrap_or_else(|[[T_16]]| [[T_16]]);
// CHECK-NEXT:         ([[T_15]] as *mut i32).add(1)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicPtr::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(cursor) as *mut *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_update(
// CHECK-NEXT:                 core::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 core::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 |[[T_17:__t[0-9]+]]| Some(([[T_17]] as *mut i32).add(1) as *mut i32),
// CHECK-NEXT:             )
// CHECK-NEXT:             .unwrap_or_else(|[[T_17]]| [[T_17]]) as *mut i32
// CHECK-NEXT:     };
// CHECK-NEXT:     let before: *mut i32 = unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicPtr::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(cursor) as *mut *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_update(
// CHECK-NEXT:                 core::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 core::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 |[[T_18:__t[0-9]+]]| Some(([[T_18]] as *mut i32).sub(1) as *mut i32),
// CHECK-NEXT:             )
// CHECK-NEXT:             .unwrap_or_else(|[[T_18]]| [[T_18]]) as *mut i32
// CHECK-NEXT:     };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {}",
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *(core::sync::atomic::AtomicPtr::from_ptr(
// CHECK-NEXT:                     core::ptr::addr_of!(cursor) as *mut *mut i32,
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .load(core::sync::atomic::Ordering::SeqCst) as *mut i32)
// CHECK-NEXT:         },
// CHECK-NEXT:         unsafe { *before }
// CHECK-NEXT:     );
// CHECK-NEXT:     let mut slot: u64 = 0;
// CHECK-NEXT:     let first: i32 = try_claim(core::ptr::addr_of_mut!(slot), 42u64);
// CHECK-NEXT:     let second: i32 = try_claim(core::ptr::addr_of_mut!(slot), 43u64);
// CHECK-NEXT:     let mut seen: u64 = 5;
// CHECK-NEXT:     let strong: bool = unsafe {
// CHECK-NEXT:         let [[T_19:__t[0-9]+]] = core::ptr::addr_of_mut!(seen);
// CHECK-NEXT:         match core::sync::atomic::AtomicU64::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(slot) as *mut u64,
// CHECK-NEXT:             )
// CHECK-NEXT:             .compare_exchange(
// CHECK-NEXT:                 *[[T_19]],
// CHECK-NEXT:                 1u64,
// CHECK-NEXT:                 core::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 core::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:             ) {
// CHECK-NEXT:             Ok(_) => true,
// CHECK-NEXT:             Err([[T_20:__t[0-9]+]]) => {
// CHECK-NEXT:                 *[[T_19]] = [[T_20]];
// CHECK-NEXT:                 false
// CHECK-NEXT:             },
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%d %d %d %llu %llu\n".as_ptr(),
// CHECK-NEXT:             first,
// CHECK-NEXT:             second,
// CHECK-NEXT:             strong as i32,
// CHECK-NEXT:             seen as u64,
// CHECK-NEXT:             core::sync::atomic::AtomicU64::from_ptr(
// CHECK-NEXT:                     core::ptr::addr_of!(slot) as *mut u64,
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .load(core::sync::atomic::Ordering::SeqCst) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let mut legacy: i32 = 4;
// CHECK-NEXT:     let swapped: i32 = (unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(legacy) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .compare_exchange(
// CHECK-NEXT:                 4i32,
// CHECK-NEXT:                 8i32,
// CHECK-NEXT:                 core::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 core::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:             )
// CHECK-NEXT:             .is_ok()
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     let observed: i32 = unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(legacy) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .compare_exchange(
// CHECK-NEXT:                 1i32,
// CHECK-NEXT:                 2i32,
// CHECK-NEXT:                 core::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 core::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:             )
// CHECK-NEXT:             .unwrap_or_else(|[[T_21:__t[0-9]+]]| [[T_21]])
// CHECK-NEXT:     };
// CHECK-NEXT:     let fetched: i32 = unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(legacy) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_add(3i32, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     };
// CHECK-NEXT:     let lock: i32 = unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(legacy) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .swap(100i32, core::sync::atomic::Ordering::Acquire)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(legacy) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .store(0i32, core::sync::atomic::Ordering::Release)
// CHECK-NEXT:     };
// CHECK-NEXT:     println!("{} {} {} {} {}", swapped, observed, fetched, lock, legacy);
// CHECK-NEXT:     let mut flag: bool = false;
// CHECK-NEXT:     let was: bool = unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicBool::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(flag) as *mut bool,
// CHECK-NEXT:             )
// CHECK-NEXT:             .swap(true, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     };
// CHECK-NEXT:     let now: bool = unsafe {
// CHECK-NEXT:         core::sync::atomic::AtomicBool::from_ptr(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(flag) as *mut bool,
// CHECK-NEXT:             )
// CHECK-NEXT:             .swap(true, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     };
// CHECK-NEXT:     println!("{} {} {}", was as i32, now as i32, set.pending);
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
