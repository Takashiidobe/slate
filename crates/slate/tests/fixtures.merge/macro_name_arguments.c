#include <stdio.h>
typedef unsigned long st_data_t;
struct ent { unsigned hash; st_data_t key; struct ent *next; };
#define EQUAL(a, b) ((a) == (b))
#define PTR_NOT_EQUAL(ptr, hash_val, key) \
((ptr) != 0 && ((ptr)->hash != (hash_val) || !EQUAL((key), (ptr)->key)))
#define FIND_ENTRY(ptr, hash_val) do { \
    if (PTR_NOT_EQUAL(ptr, hash_val, key)) { \
        while (PTR_NOT_EQUAL((ptr)->next, hash_val, key)) { ptr = ptr->next; } \
        ptr = ptr->next; \
    } \
} while (0)
int find(struct ent *head, unsigned h, st_data_t key) {
    struct ent *p = head;
    FIND_ENTRY(p, h);
    return p != 0;
}
static int find_main(void) {
    struct ent c = {2, 30, 0}, b = {1, 20, &c}, a = {0, 10, &b};
    printf("%d %d %d\n", find(&a, 2, 30), find(&a, 1, 20), find(&a, 9, 99));
    return 0;
}
struct slot { int num; int (*func)(int); };
#define PUSH(s, func) do { (s)->num = 1; (s)->func = (func); } while (0)
static int twice(int x) { return x * 2; }
static int run(int n) {
    struct slot s;
    int (*func)(int);
    int total = 0;
    func = 0;
    if (n > 5) goto big;
    func = twice;
    if (func) { PUSH(&s, func); }
    total = s.func(n) + s.num;
    goto done;
big:
    func = twice;
    PUSH(&s, func);
    total = s.func(n * 10);
done:
    return total;
}
int main(void) {
    find_main();
    printf("%d %d\n", run(3), run(7));
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}target linux-x86_64: 1 compiles
// CHECK-NEXT:   macro_name_arguments.c: [] 1 compiled
// CHECK-NEXT: target windows-x86_64: 1 compiles
// CHECK-NEXT:   macro_name_arguments.c: [] 1 compiled
// CHECK-NEXT: total: 2 compiles across 2 targets
// CHECK-NEXT: unit macro_name_arguments: 15 items, 2 gated, 0 duplicated functions
// CHECK-NEXT:   spliced fn:find_main: 2 variants
// CHECK-NEXT:   spliced fn:twice: 2 variants
// CHECK-NEXT:   spliced fn:__slate_main: 2 variants
// CHECK-NEXT: macros: 4 function-like, 3 lifted at least once (expansions counted per distinct compile)
// CHECK-NEXT:   inline name_argument_in_template: 4
// CHECK-NEXT:   macro EQUAL: 4 lifted, inline:
// CHECK-NEXT:   macro FIND_ENTRY: 2 lifted, inline:
// CHECK-NEXT:   macro PTR_NOT_EQUAL: 0 lifted, inline: name_argument_in_template 4
// CHECK-NEXT:   macro PUSH: 4 lifted, inline:
// CHECK-NEXT: projection: 12 checked, 0 not translatable, 0 mismatches
// CHECK-NEXT: signatures: 0 shared symbols, 0 mismatches
// CHECK-NEXT: exit: 0
// CHECK-NEXT: file main.rs
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(not(any(
// CHECK-NEXT:     all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"),
// CHECK-NEXT:     all(target_arch = "x86_64", windows, target_env = "msvc")
// CHECK-NEXT: )))]
// CHECK-NEXT: compile_error!("this crate was not translated for this target");
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: mod __slate_oldnames {
// CHECK-NEXT:     #[link(name = "oldnames")]
// CHECK-NEXT:     unsafe extern "C" {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg_attr(
// CHECK-NEXT:     unix,
// CHECK-NEXT:     unsafe(link_section = ".text.slate_distinct.macro_name_arguments.twice")
// CHECK-NEXT: )]
// CHECK-NEXT: extern "C-unwind" fn twice(x: i32) -> i32 {
// CHECK-NEXT:     x * 2
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct ent {
// CHECK-NEXT:     hash: u32,
// CHECK-NEXT:     key: c_ulong,
// CHECK-NEXT:     next: *mut ent,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct slot {
// CHECK-NEXT:     num: i32,
// CHECK-NEXT:     func: Option<unsafe extern "C-unwind" fn(i32) -> i32>,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn find(mut p: *mut ent, h: u32, key: c_ulong) -> i32 {
// CHECK-NEXT:     FIND_ENTRY!(p, h, key);
// CHECK-NEXT:     !p.is_null() as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     find_main();
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d %d\n".as_ptr(), run(3i32), run(7i32)) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("{} {}", run(3i32), run(7i32));
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn find_main() -> i32 {
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let mut c: ent = ent {
// CHECK-NEXT:         hash: 2,
// CHECK-NEXT:         key: 30,
// CHECK-NEXT:         next: std::ptr::null_mut::<ent>(),
// CHECK-NEXT:     };
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let mut b: ent = ent {
// CHECK-NEXT:         hash: 1,
// CHECK-NEXT:         key: 20,
// CHECK-NEXT:         next: std::ptr::addr_of_mut!(c),
// CHECK-NEXT:     };
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let mut a: ent = ent {
// CHECK-NEXT:         hash: 0,
// CHECK-NEXT:         key: 10,
// CHECK-NEXT:         next: std::ptr::addr_of_mut!(b),
// CHECK-NEXT:     };
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(
// CHECK-NEXT:                 c"%d %d %d\n".as_ptr(),
// CHECK-NEXT:                 find(std::ptr::addr_of_mut!(a), 2u32, (30 as c_ulong)),
// CHECK-NEXT:                 find(std::ptr::addr_of_mut!(a), 1u32, (20 as c_ulong)),
// CHECK-NEXT:                 find(std::ptr::addr_of_mut!(a), 9u32, (99 as c_ulong)),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     let mut c: ent = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: ent = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].hash = 2u32;
// CHECK-NEXT:         [[T_0]].key = (30 as c_ulong);
// CHECK-NEXT:         [[T_0]].next = std::ptr::null_mut::<ent>();
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     let mut b: ent = {
// CHECK-NEXT:         let mut [[T_1:__t[0-9]+]]: ent = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_1]].hash = 1u32;
// CHECK-NEXT:         [[T_1]].key = (20 as c_ulong);
// CHECK-NEXT:         [[T_1]].next = std::ptr::addr_of_mut!(c);
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     let mut a: ent = {
// CHECK-NEXT:         let mut [[T_2:__t[0-9]+]]: ent = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_2]].hash = 0u32;
// CHECK-NEXT:         [[T_2]].key = (10 as c_ulong);
// CHECK-NEXT:         [[T_2]].next = std::ptr::addr_of_mut!(b);
// CHECK-NEXT:         [[T_2]]
// CHECK-NEXT:     };
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!(
// CHECK-NEXT:             "{} {} {}",
// CHECK-NEXT:             find(std::ptr::addr_of_mut!(a), 2u32, (30 as c_ulong)),
// CHECK-NEXT:             find(std::ptr::addr_of_mut!(a), 1u32, (20 as c_ulong)),
// CHECK-NEXT:             find(std::ptr::addr_of_mut!(a), 9u32, (99 as c_ulong))
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn run(n: i32) -> i32 {
// CHECK-NEXT:     let mut total: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut func: std::mem::MaybeUninit<Option<unsafe extern "C-unwind" fn(i32) -> i32>> =
// CHECK-NEXT:         std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_0:__slate_slot_[0-9]+]]: *mut Option<unsafe extern "C-unwind" fn(i32) -> i32> =
// CHECK-NEXT:         std::ptr::addr_of_mut!(func) as *mut Option<unsafe extern "C-unwind" fn(i32) -> i32>;
// CHECK-NEXT:     let mut s: std::mem::MaybeUninit<slot> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_1:__slate_slot_[0-9]+]]: *mut slot = std::ptr::addr_of_mut!(s) as *mut slot;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         total = 0;
// CHECK-NEXT:         *[[SLATE_SLOT_0]] = None;
// CHECK-NEXT:         if n > 5 {
// CHECK-NEXT:             *[[SLATE_SLOT_0]] = Some(twice);
// CHECK-NEXT:             PUSH!(std::ptr::addr_of_mut!(*[[SLATE_SLOT_1]]), *[[SLATE_SLOT_0]]);
// CHECK-NEXT:             total = (*[[SLATE_SLOT_1]]).func.unwrap()(n * 10);
// CHECK-NEXT:         } else {
// CHECK-NEXT:             *[[SLATE_SLOT_0]] = Some(twice);
// CHECK-NEXT:             if *[[SLATE_SLOT_0]] != None {
// CHECK-NEXT:                 PUSH!(std::ptr::addr_of_mut!(*[[SLATE_SLOT_1]]), *[[SLATE_SLOT_0]]);
// CHECK-NEXT:             }
// CHECK-NEXT:             total = (*[[SLATE_SLOT_1]]).func.unwrap()(n) + (*[[SLATE_SLOT_1]]).num;
// CHECK-NEXT:         }
// CHECK-NEXT:         return total;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! EQUAL {
// CHECK-NEXT:     ($a:expr, $b:expr) => {
// CHECK-NEXT:         ($a) == $b
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! FIND_ENTRY {
// CHECK-NEXT:     ($ptr:expr, $hash_val:expr, $key:expr) => {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             if !$ptr.is_null()
// CHECK-NEXT:                 && ((unsafe { (*$ptr).hash }) != $hash_val || !EQUAL!($key, unsafe { (*$ptr).key }))
// CHECK-NEXT:             {
// CHECK-NEXT:                 while !unsafe { (*$ptr).next }.is_null()
// CHECK-NEXT:                     && ((unsafe { (*(*$ptr).next).hash }) != $hash_val
// CHECK-NEXT:                         || !EQUAL!($key, unsafe { (*(*$ptr).next).key }))
// CHECK-NEXT:                 {
// CHECK-NEXT:                     $ptr = unsafe { (*$ptr).next };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 $ptr = unsafe { (*$ptr).next };
// CHECK-NEXT:             }
// CHECK-NEXT:             if true {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! PUSH {
// CHECK-NEXT:     ($s:expr, $func:expr) => {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*$s).num = 1;
// CHECK-NEXT:             }
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*$s).func = $func;
// CHECK-NEXT:             }
// CHECK-NEXT:             if true {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: use core::ffi::c_char;
// CHECK-NEXT: use core::ffi::c_ulong;
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
