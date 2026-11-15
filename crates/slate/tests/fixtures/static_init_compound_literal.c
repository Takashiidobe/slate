int printf(const char *, ...);

struct key {
  int enabled;
  union {
    unsigned long type;
    void *entries;
  };
};
struct key_false {
  struct key key;
};
#define KEY_FALSE_INIT (struct key_false){.key = {.enabled = 0, {.type = 1UL}}}

struct lock {
  unsigned int raw;
  const char *name;
};
#define LOCK_INIT(n) ((struct lock){.raw = 1, .name = #n})

struct tracepoint {
  const char *name;
  struct key_false key;
  int (*reg)(void);
};

union word {
  unsigned int value;
  unsigned char bytes[4];
};

struct ratelimit {
  struct lock lock;
  int interval;
  int burst;
};

static int reg_fn(void) { return 5; }

struct key_false single = KEY_FALSE_INIT;
static struct lock spin = LOCK_INIT(spin);
struct tracepoint tracepoint = {.name = "tp", .key = KEY_FALSE_INIT, .reg = reg_fn};
struct lock locks[2] = {LOCK_INIT(a), LOCK_INIT(b)};
union word word = (union word){.value = 0x01020304u};
struct lock *spin_address = &(struct lock){.raw = 9, .name = "addr"};

static int ratelimited(void) {
  static struct ratelimit rs = {.lock = (struct lock){.raw = 2, .name = "rs"}, .interval = 10, .burst = 3};
  rs.burst--;
  return rs.lock.raw * 100 + rs.interval + rs.burst;
}

int main(void) {
  printf("%d %lu %u %s\n", single.key.enabled, single.key.type, spin.raw, spin.name);
  printf("%s %lu %d\n", tracepoint.name, tracepoint.key.key.type, tracepoint.reg());
  printf("%s %s %u\n", locks[0].name, locks[1].name, word.value);
  printf("%u %s\n", spin_address->raw, spin_address->name);
  int first = ratelimited();
  printf("%d %d\n", first, ratelimited());
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct key {
// CHECK-NEXT:     enabled: i32,
// CHECK-NEXT:     [[SLATE_ANON_0:__slate_anon_[0-9]+]]: __SlateRecord_static_init_compound_literal_c_5,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union __SlateRecord_static_init_compound_literal_c_5 {
// CHECK-NEXT:     r#type: u64,
// CHECK-NEXT:     entries: *mut (),
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct key_false {
// CHECK-NEXT:     key: key,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct lock {
// CHECK-NEXT:     raw: u32,
// CHECK-NEXT:     name: *const i8,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct tracepoint {
// CHECK-NEXT:     name: *const i8,
// CHECK-NEXT:     key: key_false,
// CHECK-NEXT:     reg: Option<unsafe extern "C-unwind" fn() -> i32>,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union word {
// CHECK-NEXT:     value: u32,
// CHECK-NEXT:     bytes: [u8; 4],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct ratelimit {
// CHECK-NEXT:     lock: lock,
// CHECK-NEXT:     interval: i32,
// CHECK-NEXT:     burst: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.reg_fn")]
// CHECK-NEXT: extern "C-unwind" fn reg_fn() -> i32 {
// CHECK-NEXT:     5
// CHECK-NEXT: }
// CHECK-NEXT: static mut single: key_false = key_false {
// CHECK-NEXT:     key: {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: key = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].enabled = 0i32;
// CHECK-NEXT:         [[T_0]].[[SLATE_ANON_0]] = __SlateRecord_static_init_compound_literal_c_5 {
// CHECK-NEXT:             r#type: 1u64,
// CHECK-NEXT:         };
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     },
// CHECK-NEXT: };
// CHECK-NEXT: static mut spin: lock = {
// CHECK-NEXT:     let mut [[T_0]]: lock = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].raw = 1u32;
// CHECK-NEXT:     [[T_0]].name = (b"spin\0".as_ptr() as *mut i8) as *const i8;
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: static mut tracepoint: tracepoint = tracepoint {
// CHECK-NEXT:     name: (b"tp\0".as_ptr() as *mut i8) as *const i8,
// CHECK-NEXT:     key: key_false {
// CHECK-NEXT:         key: {
// CHECK-NEXT:             let mut [[T_0]]: key = unsafe { core::mem::zeroed() };
// CHECK-NEXT:             [[T_0]].enabled = 0i32;
// CHECK-NEXT:             [[T_0]].[[SLATE_ANON_0]] = __SlateRecord_static_init_compound_literal_c_5 {
// CHECK-NEXT:                 r#type: 1u64,
// CHECK-NEXT:             };
// CHECK-NEXT:             [[T_0]]
// CHECK-NEXT:         },
// CHECK-NEXT:     },
// CHECK-NEXT:     reg: Some(reg_fn),
// CHECK-NEXT: };
// CHECK-NEXT: static mut locks: __SlateAlign16<[lock; 2]> = __SlateAlign16([
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut [[T_0]]: lock = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].raw = 1u32;
// CHECK-NEXT:         [[T_0]].name = (b"a\0".as_ptr() as *mut i8) as *const i8;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut [[T_1:__t[0-9]+]]: lock = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         [[T_1]].raw = 1u32;
// CHECK-NEXT:         [[T_1]].name = (b"b\0".as_ptr() as *mut i8) as *const i8;
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     },
// CHECK-NEXT: ]);
// CHECK-NEXT: static mut word: word = word { value: 16909060u32 };
// CHECK-NEXT: static mut spin_address: *mut lock = unsafe {
// CHECK-NEXT:     core::ptr::addr_of_mut!(__slate_compound_spin_address_0)
// CHECK-NEXT: };
// CHECK-NEXT: fn ratelimited() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         rs.burst = rs.burst - 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { rs.lock.raw }
// CHECK-NEXT:         .wrapping_mul(100)
// CHECK-NEXT:         .wrapping_add((unsafe { rs.interval }) as u32)
// CHECK-NEXT:         .wrapping_add((unsafe { rs.burst }) as u32) as i32
// CHECK-NEXT: }
// CHECK-NEXT: static mut rs: ratelimit = ratelimit {
// CHECK-NEXT:     lock: {
// CHECK-NEXT:         let mut [[T_0]]: lock = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].raw = 2u32;
// CHECK-NEXT:         [[T_0]].name = (b"rs\0".as_ptr() as *mut i8) as *const i8;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     },
// CHECK-NEXT:     interval: 10i32,
// CHECK-NEXT:     burst: 3i32,
// CHECK-NEXT: };
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %lu %u %s\n".as_ptr(),
// CHECK-NEXT:             single.key.enabled,
// CHECK-NEXT:             single.key.[[SLATE_ANON_0]].r#type,
// CHECK-NEXT:             spin.raw,
// CHECK-NEXT:             spin.name,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%s %lu %d\n".as_ptr(),
// CHECK-NEXT:             tracepoint.name,
// CHECK-NEXT:             tracepoint.key.key.[[SLATE_ANON_0]].r#type,
// CHECK-NEXT:             unsafe { tracepoint.reg }.unwrap()(),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(c"%s %s %u\n".as_ptr(), locks.0[0].name, locks.0[1].name, word.value)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { printf(c"%u %s\n".as_ptr(), (*spin_address).raw, (*spin_address).name) };
// CHECK-NEXT:     let first: i32 = ratelimited();
// CHECK-NEXT:     unsafe { printf(c"%d %d\n".as_ptr(), first, ratelimited()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: static mut __slate_compound_spin_address_0: lock = {
// CHECK-NEXT:     let mut [[T_0]]: lock = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].raw = 9u32;
// CHECK-NEXT:     [[T_0]].name = (b"addr\0".as_ptr() as *mut i8) as *const i8;
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
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
