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
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn pthread_create(
// CHECK-NEXT:         __newthread: *mut u64,
// CHECK-NEXT:         __attr: *const pthread_attr_t,
// CHECK-NEXT:         __start_routine: Option<unsafe extern "C-unwind" fn(*mut ()) -> *mut ()>,
// CHECK-NEXT:         __arg: *mut (),
// CHECK-NEXT:     ) -> i32;
// CHECK-NEXT:     fn pthread_join(__th: u64, __thread_return: *mut *mut ()) -> i32;
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union pthread_attr_t {
// CHECK-NEXT:     __size: [i8; 56],
// CHECK-NEXT:     __align: i64,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct vset {
// CHECK-NEXT:     pending: i32,
// CHECK-NEXT:     in_use: i32,
// CHECK-NEXT:     bytes: u64,
// CHECK-NEXT:     epoch: u32,
// CHECK-NEXT: }
// CHECK-NEXT: static mut used_memory: u64 = ((0 as i32) as i64) as u64;
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut stat_net_input_bytes: i64 = (0 as i32) as i64;
// CHECK-NEXT: static mut plain_mask: u32 = (90 as i32) as u32;
// CHECK-NEXT: static mut plain_counter: u64 = ((10 as i32) as i64) as u64;
// CHECK-NEXT: static mut threads_done: i32 = 0 as i32;
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct used_memory_entry {
// CHECK-NEXT:     used_memory: i64,
// CHECK-NEXT:     padding: [i8; 56],
// CHECK-NEXT: }
// CHECK-NEXT: static mut entries: __SlateAlign16<[used_memory_entry; 4]> = __SlateAlign16(unsafe {
// CHECK-NEXT:     std::mem::zeroed()
// CHECK-NEXT: });
// CHECK-NEXT: extern "C-unwind" fn worker(arg: *mut ()) -> *mut () {
// CHECK-NEXT:     let set: *mut vset = arg as *mut vset;
// CHECK-NEXT:     let mut i: i32 = 0;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < 10000 {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             let [[T_0:__t[0-9]+]] = 1 as i32;
// CHECK-NEXT:             let [[T_1:__t[0-9]+]] = std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                     std::ptr::addr_of_mut!((* set).in_use) as *mut i32,
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .fetch_add([[T_0]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:             [[T_1]]
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             let [[T_2:__t[0-9]+]] = 3 as u64;
// CHECK-NEXT:             let [[T_3:__t[0-9]+]] = std::sync::atomic::AtomicU64::from_ptr(
// CHECK-NEXT:                     (unsafe { std::ptr::addr_of_mut!(used_memory) }) as *mut u64,
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .fetch_add([[T_2]], std::sync::atomic::Ordering::Relaxed);
// CHECK-NEXT:             [[T_3]]
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             let [[T_4:__t[0-9]+]] = 2 as u64;
// CHECK-NEXT:             let [[T_5:__t[0-9]+]] = std::sync::atomic::AtomicU64::from_ptr(
// CHECK-NEXT:                     (unsafe { std::ptr::addr_of_mut!(plain_counter) }) as *mut u64,
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .fetch_add([[T_4]], std::sync::atomic::Ordering::Relaxed);
// CHECK-NEXT:             [[T_5]]
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             let [[T_6:__t[0-9]+]] = 1 as i64;
// CHECK-NEXT:             let [[T_7:__t[0-9]+]] = std::sync::atomic::AtomicI64::from_ptr(
// CHECK-NEXT:                     (unsafe {
// CHECK-NEXT:                         std::ptr::addr_of_mut!(entries.0[(i & 3) as usize].used_memory)
// CHECK-NEXT:                     }) as *mut i64,
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .fetch_add([[T_6]], std::sync::atomic::Ordering::Relaxed);
// CHECK-NEXT:             [[T_7]]
// CHECK-NEXT:         };
// CHECK-NEXT:         i += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_8:__t[0-9]+]] = 1 as i32;
// CHECK-NEXT:         let [[T_9:__t[0-9]+]] = std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 (unsafe { std::ptr::addr_of_mut!(threads_done) }) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_add([[T_8]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:         [[T_9]]
// CHECK-NEXT:     };
// CHECK-NEXT:     std::ptr::null_mut::<()>()
// CHECK-NEXT: }
// CHECK-NEXT: fn try_claim(slot: *mut u64, want: u64) -> i32 {
// CHECK-NEXT:     let mut expected: u64 = 0;
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let [[V_0:__v[0-9]+]]: bool = unsafe {
// CHECK-NEXT:             let [[T_1]] = std::ptr::addr_of_mut!(expected);
// CHECK-NEXT:             let [[T_0]] = want;
// CHECK-NEXT:             let [[T_3]] = std::sync::atomic::AtomicU64::from_ptr(slot as *mut u64)
// CHECK-NEXT:                 .compare_exchange_weak(
// CHECK-NEXT:                     *[[T_1]],
// CHECK-NEXT:                     [[T_0]],
// CHECK-NEXT:                     std::sync::atomic::Ordering::Relaxed,
// CHECK-NEXT:                     std::sync::atomic::Ordering::Relaxed,
// CHECK-NEXT:                 );
// CHECK-NEXT:             match [[T_3]] {
// CHECK-NEXT:                 Ok(_) => true,
// CHECK-NEXT:                 Err([[T_2]]) => {
// CHECK-NEXT:                     *[[T_1]] = [[T_2]];
// CHECK-NEXT:                     false
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:         if !(![[V_0]]) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:         if expected != 0 {
// CHECK-NEXT:             return 0;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     1
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut set: vset = {
// CHECK-NEXT:         let mut [[T_1]]: vset = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_1]].pending = 0 as i32;
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// CHECK-NEXT:     set.pending = 3;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(set.in_use) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .store(1 as i32, std::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_3]] = 1 as i32;
// CHECK-NEXT:         let [[T_0]] = std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(set.in_use) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_sub([[T_3]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_5]] = 40 as u64;
// CHECK-NEXT:         let [[T_2]] = std::sync::atomic::AtomicU64::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(set.bytes) as *mut u64,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_add([[T_5]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:         [[T_2]].wrapping_add([[T_5]])
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_7]] = 8 as u64;
// CHECK-NEXT:         let [[T_4]] = std::sync::atomic::AtomicU64::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(set.bytes) as *mut u64,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_sub([[T_7]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:         [[T_4]].wrapping_sub([[T_7]])
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::sync::atomic::AtomicU32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(set.epoch) as *mut u32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .store(7 as u32, std::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut threads: __SlateAlign16<[u64; 4]> = __SlateAlign16([0 as u64; 4]);
// CHECK-NEXT:     for i in 0 as i32..4 as i32 {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             pthread_create(
// CHECK-NEXT:                 unsafe { (threads.0.as_mut_ptr() as *mut u64).offset(i as isize) },
// CHECK-NEXT:                 std::ptr::null::<pthread_attr_t>(),
// CHECK-NEXT:                 Some(worker),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(set) as *mut (),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     for i in 0 as i32..4 as i32 {
// CHECK-NEXT:         unsafe { pthread_join(threads.0[i as usize], std::ptr::null_mut::<*mut ()>()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %zu %llu %d\n".as_ptr(),
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                         std::ptr::addr_of!(set.in_use) as *mut i32,
// CHECK-NEXT:                     )
// CHECK-NEXT:                     .load(std::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:             },
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::sync::atomic::AtomicU64::from_ptr(
// CHECK-NEXT:                         std::ptr::addr_of!(used_memory) as *mut u64,
// CHECK-NEXT:                     )
// CHECK-NEXT:                     .load(std::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:             },
// CHECK-NEXT:             unsafe { plain_counter },
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                         std::ptr::addr_of!(threads_done) as *mut i32,
// CHECK-NEXT:                     )
// CHECK-NEXT:                     .load(std::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     for i in 0 as i32..4 as i32 {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"%lld ".as_ptr(),
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::sync::atomic::AtomicI64::from_ptr(
// CHECK-NEXT:                             std::ptr::addr_of!(entries.0[i as usize].used_memory)
// CHECK-NEXT:                                 as *mut i64,
// CHECK-NEXT:                         )
// CHECK-NEXT:                         .load(std::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("");
// CHECK-NEXT:     let mut old: u64 = 0;
// CHECK-NEXT:     '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         old = unsafe {
// CHECK-NEXT:             let [[T_9]] = 5 as u64;
// CHECK-NEXT:             let [[T_6]] = std::sync::atomic::AtomicU64::from_ptr(
// CHECK-NEXT:                     (unsafe { std::ptr::addr_of_mut!(used_memory) }) as *mut u64,
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .fetch_add([[T_9]], std::sync::atomic::Ordering::Relaxed);
// CHECK-NEXT:             [[T_6]]
// CHECK-NEXT:         };
// CHECK-NEXT:         if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_10:__t[0-9]+]] = 2 as u64;
// CHECK-NEXT:         let [[T_8]] = std::sync::atomic::AtomicU64::from_ptr(
// CHECK-NEXT:                 (unsafe { std::ptr::addr_of_mut!(used_memory) }) as *mut u64,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_sub([[T_10]], std::sync::atomic::Ordering::Relaxed);
// CHECK-NEXT:         [[T_8]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%zu %zu\n".as_ptr(),
// CHECK-NEXT:             old,
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::sync::atomic::AtomicU64::from_ptr(
// CHECK-NEXT:                         std::ptr::addr_of!(used_memory) as *mut u64,
// CHECK-NEXT:                     )
// CHECK-NEXT:                     .load(std::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut added: i64 = 0;
// CHECK-NEXT:     added = unsafe {
// CHECK-NEXT:         let [[T_11:__t[0-9]+]] = 100 as i64;
// CHECK-NEXT:         let [[T_12:__t[0-9]+]] = std::sync::atomic::AtomicI64::from_ptr(
// CHECK-NEXT:                 (unsafe { std::ptr::addr_of_mut!(stat_net_input_bytes) }) as *mut i64,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_add([[T_11]], std::sync::atomic::Ordering::Acquire);
// CHECK-NEXT:         [[T_12]].wrapping_add([[T_11]])
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut subbed: i64 = 0;
// CHECK-NEXT:     subbed = unsafe {
// CHECK-NEXT:         let [[T_13:__t[0-9]+]] = 30 as i64;
// CHECK-NEXT:         let [[T_14:__t[0-9]+]] = std::sync::atomic::AtomicI64::from_ptr(
// CHECK-NEXT:                 (unsafe { std::ptr::addr_of_mut!(stat_net_input_bytes) }) as *mut i64,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_sub([[T_13]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:         [[T_14]].wrapping_sub([[T_13]])
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld %lld\n".as_ptr(), added, subbed) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut previous: u32 = 0;
// CHECK-NEXT:     previous = unsafe {
// CHECK-NEXT:         let [[T_15:__t[0-9]+]] = 9 as u32;
// CHECK-NEXT:         let [[T_16:__t[0-9]+]] = std::sync::atomic::AtomicU32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(set.epoch) as *mut u32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .swap([[T_15]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:         [[T_16]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut orred: u32 = 0;
// CHECK-NEXT:     orred = unsafe {
// CHECK-NEXT:         let [[T_17:__t[0-9]+]] = 48 as u32;
// CHECK-NEXT:         let [[T_18:__t[0-9]+]] = std::sync::atomic::AtomicU32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(set.epoch) as *mut u32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_or([[T_17]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:         [[T_18]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut anded: u32 = 0;
// CHECK-NEXT:     anded = unsafe {
// CHECK-NEXT:         let [[T_19:__t[0-9]+]] = 60 as u32;
// CHECK-NEXT:         let [[T_20:__t[0-9]+]] = std::sync::atomic::AtomicU32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(set.epoch) as *mut u32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_and([[T_19]], std::sync::atomic::Ordering::AcqRel);
// CHECK-NEXT:         [[T_20]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut xorred: u32 = 0;
// CHECK-NEXT:     xorred = unsafe {
// CHECK-NEXT:         let [[T_21:__t[0-9]+]] = 255 as u32;
// CHECK-NEXT:         let [[T_22:__t[0-9]+]] = std::sync::atomic::AtomicU32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(set.epoch) as *mut u32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_xor([[T_21]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:         [[T_22]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut nand: u32 = 0;
// CHECK-NEXT:     nand = unsafe {
// CHECK-NEXT:         let [[T_23:__t[0-9]+]] = 240 as u32;
// CHECK-NEXT:         let [[T_24:__t[0-9]+]] = std::sync::atomic::AtomicU32::from_ptr(
// CHECK-NEXT:                 (unsafe { std::ptr::addr_of_mut!(plain_mask) }) as *mut u32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_nand([[T_23]], std::sync::atomic::Ordering::Relaxed);
// CHECK-NEXT:         [[T_24]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut nand_new: u32 = 0;
// CHECK-NEXT:     nand_new = unsafe {
// CHECK-NEXT:         let [[T_25:__t[0-9]+]] = 15 as u32;
// CHECK-NEXT:         let [[T_26:__t[0-9]+]] = std::sync::atomic::AtomicU32::from_ptr(
// CHECK-NEXT:                 (unsafe { std::ptr::addr_of_mut!(plain_mask) }) as *mut u32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_nand([[T_25]], std::sync::atomic::Ordering::Relaxed);
// CHECK-NEXT:         !([[T_26]] & [[T_25]])
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%u %u %u %u %u %u %u %u\n".as_ptr(),
// CHECK-NEXT:             previous,
// CHECK-NEXT:             orred,
// CHECK-NEXT:             anded,
// CHECK-NEXT:             xorred,
// CHECK-NEXT:             nand,
// CHECK-NEXT:             nand_new,
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::sync::atomic::AtomicU32::from_ptr(
// CHECK-NEXT:                         std::ptr::addr_of!(set.epoch) as *mut u32,
// CHECK-NEXT:                     )
// CHECK-NEXT:                     .load(std::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:             },
// CHECK-NEXT:             unsafe { plain_mask },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut scaled: i32 = 3;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_27:__t[0-9]+]] = std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(scaled) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_update(
// CHECK-NEXT:                 std::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 std::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 |[[T_28:__t[0-9]+]]| Some([[T_28]] * 7),
// CHECK-NEXT:             )
// CHECK-NEXT:             .unwrap_or_else(|[[T_28]]| [[T_28]]);
// CHECK-NEXT:         [[T_27]] * 7
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_29:__t[0-9]+]] = std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(scaled) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_update(
// CHECK-NEXT:                 std::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 std::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 |[[T_30:__t[0-9]+]]| Some([[T_30]] << (2 as i32)),
// CHECK-NEXT:             )
// CHECK-NEXT:             .unwrap_or_else(|[[T_30]]| [[T_30]]);
// CHECK-NEXT:         [[T_29]] << (2 as i32)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_31:__t[0-9]+]] = std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(scaled) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_update(
// CHECK-NEXT:                 std::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 std::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 |[[T_32:__t[0-9]+]]| Some([[T_32]] / 3),
// CHECK-NEXT:             )
// CHECK-NEXT:             .unwrap_or_else(|[[T_32]]| [[T_32]]);
// CHECK-NEXT:         [[T_31]] / 3
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut post: i32 = 0;
// CHECK-NEXT:     post = unsafe {
// CHECK-NEXT:         let [[T_33:__t[0-9]+]] = 1 as i32;
// CHECK-NEXT:         let [[T_34:__t[0-9]+]] = std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(scaled) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_add([[T_33]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:         [[T_34]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut pre: i32 = 0;
// CHECK-NEXT:     pre = unsafe {
// CHECK-NEXT:         let [[T_35:__t[0-9]+]] = 1 as i32;
// CHECK-NEXT:         let [[T_36:__t[0-9]+]] = std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(scaled) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_sub([[T_35]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:         [[T_36]].wrapping_sub([[T_35]])
// CHECK-NEXT:     };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}", unsafe {
// CHECK-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of!(scaled) as * mut i32)
// CHECK-NEXT:         .load(std::sync::atomic::Ordering::SeqCst) }, post, pre
// CHECK-NEXT:     );
// CHECK-NEXT:     let mut ratio: f64 = 1.5f64;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_37:__t[0-9]+]] = std::sync::atomic::AtomicU64::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(ratio) as *mut u64,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_update(
// CHECK-NEXT:                 std::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 std::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 |[[T_38:__t[0-9]+]]| Some((f64::from_bits([[T_38]]) + 2.25f64).to_bits()),
// CHECK-NEXT:             )
// CHECK-NEXT:             .unwrap_or_else(|[[T_38]]| [[T_38]]);
// CHECK-NEXT:         f64::from_bits([[T_37]]) + 2.25f64
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_39:__t[0-9]+]] = std::sync::atomic::AtomicU64::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(ratio) as *mut u64,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_update(
// CHECK-NEXT:                 std::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 std::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 |[[T_40:__t[0-9]+]]| Some((f64::from_bits([[T_40]]) * ((2 as i32) as f64)).to_bits()),
// CHECK-NEXT:             )
// CHECK-NEXT:             .unwrap_or_else(|[[T_40]]| [[T_40]]);
// CHECK-NEXT:         f64::from_bits([[T_39]]) * ((2 as i32) as f64)
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut small: f32 = 0.5f32;
// CHECK-NEXT:     let mut small_old: f32 = 0 as f32;
// CHECK-NEXT:     small_old = unsafe {
// CHECK-NEXT:         let [[T_41:__t[0-9]+]] = std::sync::atomic::AtomicU32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(small) as *mut u32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_update(
// CHECK-NEXT:                 std::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 std::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 |[[T_42:__t[0-9]+]]| Some((f32::from_bits([[T_42]]) + ((1 as i32) as f32)).to_bits()),
// CHECK-NEXT:             )
// CHECK-NEXT:             .unwrap_or_else(|[[T_42]]| [[T_42]]);
// CHECK-NEXT:         f32::from_bits([[T_41]])
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%.3f %.3f %.3f\n".as_ptr(),
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 f64::from_bits(
// CHECK-NEXT:                     std::sync::atomic::AtomicU64::from_ptr(
// CHECK-NEXT:                             std::ptr::addr_of!(ratio) as *mut u64,
// CHECK-NEXT:                         )
// CHECK-NEXT:                         .load(std::sync::atomic::Ordering::SeqCst),
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:             (unsafe {
// CHECK-NEXT:                 f32::from_bits(
// CHECK-NEXT:                     std::sync::atomic::AtomicU32::from_ptr(
// CHECK-NEXT:                             std::ptr::addr_of!(small) as *mut u32,
// CHECK-NEXT:                         )
// CHECK-NEXT:                         .load(std::sync::atomic::Ordering::SeqCst),
// CHECK-NEXT:                 )
// CHECK-NEXT:             }) as f64,
// CHECK-NEXT:             small_old as f64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut values: __SlateAlign16<[i32; 4]> = __SlateAlign16([
// CHECK-NEXT:         10 as i32,
// CHECK-NEXT:         20 as i32,
// CHECK-NEXT:         30 as i32,
// CHECK-NEXT:         40 as i32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut cursor: *mut i32 = values.0.as_mut_ptr() as *mut i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_43:__t[0-9]+]] = std::sync::atomic::AtomicPtr::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(cursor) as *mut *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_update(
// CHECK-NEXT:                 std::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 std::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 |[[T_44:__t[0-9]+]]| Some(
// CHECK-NEXT:                     (unsafe { ([[T_44]] as *mut i32).offset(1 as isize) }) as *mut i32,
// CHECK-NEXT:                 ),
// CHECK-NEXT:             )
// CHECK-NEXT:             .unwrap_or_else(|[[T_44]]| [[T_44]]);
// CHECK-NEXT:         [[T_43]] as *mut i32
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_45:__t[0-9]+]] = std::sync::atomic::AtomicPtr::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(cursor) as *mut *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_update(
// CHECK-NEXT:                 std::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 std::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 |[[T_46:__t[0-9]+]]| Some(
// CHECK-NEXT:                     (unsafe { ([[T_46]] as *mut i32).offset(1 as isize) }) as *mut i32,
// CHECK-NEXT:                 ),
// CHECK-NEXT:             )
// CHECK-NEXT:             .unwrap_or_else(|[[T_46]]| [[T_46]]);
// CHECK-NEXT:         unsafe { ([[T_45]] as *mut i32).offset(1 as isize) }
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_47:__t[0-9]+]] = std::sync::atomic::AtomicPtr::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(cursor) as *mut *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_update(
// CHECK-NEXT:                 std::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 std::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 |[[T_48:__t[0-9]+]]| Some(
// CHECK-NEXT:                     (unsafe { ([[T_48]] as *mut i32).offset(1 as isize) }) as *mut i32,
// CHECK-NEXT:                 ),
// CHECK-NEXT:             )
// CHECK-NEXT:             .unwrap_or_else(|[[T_48]]| [[T_48]]);
// CHECK-NEXT:         [[T_47]] as *mut i32
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: *mut i32 = unsafe {
// CHECK-NEXT:         let [[T_49:__t[0-9]+]] = std::sync::atomic::AtomicPtr::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(cursor) as *mut *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_update(
// CHECK-NEXT:                 std::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 std::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 |[[T_50:__t[0-9]+]]| Some(
// CHECK-NEXT:                     (unsafe { ([[T_50]] as *mut i32).offset(-(1 as isize)) }) as *mut i32,
// CHECK-NEXT:                 ),
// CHECK-NEXT:             )
// CHECK-NEXT:             .unwrap_or_else(|[[T_50]]| [[T_50]]);
// CHECK-NEXT:         [[T_49]] as *mut i32
// CHECK-NEXT:     };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {}", unsafe { * unsafe {
// CHECK-NEXT:         std::sync::atomic::AtomicPtr::from_ptr(std::ptr::addr_of!(cursor) as * mut * mut
// CHECK-NEXT:         i32).load(std::sync::atomic::Ordering::SeqCst) as * mut i32 } }, unsafe { *
// CHECK-NEXT:         [[V_1]] }
// CHECK-NEXT:     );
// CHECK-NEXT:     let mut slot: u64 = 0;
// CHECK-NEXT:     let first: i32 = try_claim(std::ptr::addr_of_mut!(slot), 42 as u64);
// CHECK-NEXT:     let second: i32 = try_claim(std::ptr::addr_of_mut!(slot), 43 as u64);
// CHECK-NEXT:     let mut seen: u64 = 5;
// CHECK-NEXT:     let mut strong: bool = false;
// CHECK-NEXT:     strong = unsafe {
// CHECK-NEXT:         let [[T_51:__t[0-9]+]] = std::ptr::addr_of_mut!(seen);
// CHECK-NEXT:         let [[T_52:__t[0-9]+]] = 1 as u64;
// CHECK-NEXT:         let [[T_53:__t[0-9]+]] = std::sync::atomic::AtomicU64::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(slot) as *mut u64,
// CHECK-NEXT:             )
// CHECK-NEXT:             .compare_exchange(
// CHECK-NEXT:                 *[[T_51]],
// CHECK-NEXT:                 [[T_52]],
// CHECK-NEXT:                 std::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 std::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:             );
// CHECK-NEXT:         match [[T_53]] {
// CHECK-NEXT:             Ok(_) => true,
// CHECK-NEXT:             Err([[T_54:__t[0-9]+]]) => {
// CHECK-NEXT:                 *[[T_51]] = [[T_54]];
// CHECK-NEXT:                 false
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %d %d %llu %llu\n".as_ptr(),
// CHECK-NEXT:             first,
// CHECK-NEXT:             second,
// CHECK-NEXT:             strong as i32,
// CHECK-NEXT:             seen,
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::sync::atomic::AtomicU64::from_ptr(
// CHECK-NEXT:                         std::ptr::addr_of!(slot) as *mut u64,
// CHECK-NEXT:                     )
// CHECK-NEXT:                     .load(std::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut legacy: i32 = 4;
// CHECK-NEXT:     let mut swapped: i32 = 0;
// CHECK-NEXT:     let [[V_2:__v[0-9]+]]: bool = unsafe {
// CHECK-NEXT:         let [[T_55:__t[0-9]+]] = 4 as i32;
// CHECK-NEXT:         let [[T_56:__t[0-9]+]] = 8 as i32;
// CHECK-NEXT:         let [[T_57:__t[0-9]+]] = std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(legacy) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .compare_exchange(
// CHECK-NEXT:                 [[T_55]],
// CHECK-NEXT:                 [[T_56]],
// CHECK-NEXT:                 std::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 std::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:             );
// CHECK-NEXT:         [[T_57]].is_ok()
// CHECK-NEXT:     };
// CHECK-NEXT:     swapped = [[V_2]] as i32;
// CHECK-NEXT:     let mut observed: i32 = 0;
// CHECK-NEXT:     observed = unsafe {
// CHECK-NEXT:         let [[T_58:__t[0-9]+]] = 1 as i32;
// CHECK-NEXT:         let [[T_59:__t[0-9]+]] = 2 as i32;
// CHECK-NEXT:         let [[T_60:__t[0-9]+]] = std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(legacy) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .compare_exchange(
// CHECK-NEXT:                 [[T_58]],
// CHECK-NEXT:                 [[T_59]],
// CHECK-NEXT:                 std::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:                 std::sync::atomic::Ordering::SeqCst,
// CHECK-NEXT:             );
// CHECK-NEXT:         [[T_60]].unwrap_or_else(|[[T_61:__t[0-9]+]]| [[T_61]])
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut fetched: i32 = 0;
// CHECK-NEXT:     fetched = unsafe {
// CHECK-NEXT:         let [[T_62:__t[0-9]+]] = 3 as i32;
// CHECK-NEXT:         let [[T_63:__t[0-9]+]] = std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(legacy) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_add([[T_62]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:         [[T_63]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut lock: i32 = 0;
// CHECK-NEXT:     lock = unsafe {
// CHECK-NEXT:         let [[T_64:__t[0-9]+]] = 100 as i32;
// CHECK-NEXT:         let [[T_65:__t[0-9]+]] = std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(legacy) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .swap([[T_64]], std::sync::atomic::Ordering::Acquire);
// CHECK-NEXT:         [[T_65]]
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(legacy) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .store(0 as i32, std::sync::atomic::Ordering::Release)
// CHECK-NEXT:     };
// CHECK-NEXT:     println!("{} {} {} {} {}", swapped, observed, fetched, lock, legacy);
// CHECK-NEXT:     let mut flag: bool = (0 as i32) != (0 as i32);
// CHECK-NEXT:     let mut was: bool = false;
// CHECK-NEXT:     was = unsafe {
// CHECK-NEXT:         let [[T_66:__t[0-9]+]] = true as bool;
// CHECK-NEXT:         let [[T_67:__t[0-9]+]] = std::sync::atomic::AtomicBool::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(flag) as *mut bool,
// CHECK-NEXT:             )
// CHECK-NEXT:             .swap([[T_66]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:         [[T_67]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut now: bool = false;
// CHECK-NEXT:     now = unsafe {
// CHECK-NEXT:         let [[T_68:__t[0-9]+]] = true as bool;
// CHECK-NEXT:         let [[T_69:__t[0-9]+]] = std::sync::atomic::AtomicBool::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(flag) as *mut bool,
// CHECK-NEXT:             )
// CHECK-NEXT:             .swap([[T_68]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:         [[T_69]]
// CHECK-NEXT:     };
// CHECK-NEXT:     println!("{} {} {}", was as i32, now as i32, set.pending);
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
