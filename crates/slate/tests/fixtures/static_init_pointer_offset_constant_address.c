int printf(const char *, ...);
#define POISON_POINTER_DELTA 0xdead000000000000UL
#define LIST_POISON1 ((void *)0x100 + POISON_POINTER_DELTA)
#define LIST_POISON2 ((void *)0x122 + POISON_POINTER_DELTA)
#define SKB_LIST_POISON_NEXT ((void *)(0x800 + POISON_POINTER_DELTA))
extern const int sysctl_vals[];
const int sysctl_vals[] = {0, 1, 2, 100};
extern const int unsized_vals[];
#define SYSCTL_ONE ((void *)&sysctl_vals[1])
#define SYSCTL_HUNDRED ((void *)&sysctl_vals[3])
struct list_head { struct list_head *next, *prev; };
struct ctl { const char *name; void *min; void *max; };
static struct list_head poisoned = {LIST_POISON1, LIST_POISON2};
static void *skb_next = SKB_LIST_POISON_NEXT;
static char *char_offset = (char *)0x1000 + 0x20;
static int *int_offset = (int *)0x1000 + 4;
static int *back = (int *)0x1000 - 2;
static struct ctl table[] = {{"one", SYSCTL_ONE, SYSCTL_HUNDRED}, {0, 0, 0}};
static const int *later = &unsized_vals[2];
static const int *past = sysctl_vals + 4;
const int unsized_vals[] = {7, 8, 9};
int main(void) {
  printf("%lx %lx %lx\n", (unsigned long)poisoned.next, (unsigned long)poisoned.prev, (unsigned long)skb_next);
  printf("%lx %lx %lx\n", (unsigned long)char_offset, (unsigned long)int_offset, (unsigned long)back);
  printf("%s %d %d\n", table[0].name, *(int *)table[0].min, *(int *)table[0].max);
  printf("%d %d\n", *later, past == sysctl_vals + 4);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: static sysctl_vals: __SlateAlign16<[i32; 4]> = __SlateAlign16([
// CHECK-NEXT:     0i32, 1i32, 2i32, 100i32,
// CHECK-NEXT: ]);
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct list_head {
// CHECK-NEXT:     next: *mut list_head,
// CHECK-NEXT:     prev: *mut list_head,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: unsafe impl<T> Sync for __SlateAlign16<T> {}
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct ctl {
// CHECK-NEXT:     name: *const i8,
// CHECK-NEXT:     min: *mut (),
// CHECK-NEXT:     max: *mut (),
// CHECK-NEXT: }
// CHECK-NEXT: static mut poisoned: list_head = list_head {
// CHECK-NEXT:     next: ((unsafe {
// CHECK-NEXT:         ((256i32 as *mut ()) as *const u8)
// CHECK-NEXT:             .wrapping_offset(16045481047390945280u64 as isize)
// CHECK-NEXT:     }) as *mut ()) as *mut list_head,
// CHECK-NEXT:     prev: ((unsafe {
// CHECK-NEXT:         ((290i32 as *mut ()) as *const u8)
// CHECK-NEXT:             .wrapping_offset(16045481047390945280u64 as isize)
// CHECK-NEXT:     }) as *mut ()) as *mut list_head,
// CHECK-NEXT: };
// CHECK-NEXT: static mut skb_next: *mut () = 16045481047390947328u64 as *mut ();
// CHECK-NEXT: static mut char_offset: *mut i8 = (4096i32 as *mut i8).wrapping_offset(32i32 as isize);
// CHECK-NEXT: static mut int_offset: *mut i32 = (4096i32 as *mut i32).wrapping_offset(4i32 as isize);
// CHECK-NEXT: static mut back: *mut i32 = (4096i32 as *mut i32).wrapping_offset(-(2i32 as isize));
// CHECK-NEXT: static mut table: __SlateAlign16<[ctl; 2]> = __SlateAlign16([
// CHECK-NEXT:     ctl {
// CHECK-NEXT:         name: (b"one\0".as_ptr() as *mut i8) as *const i8,
// CHECK-NEXT:         min: unsafe { core::ptr::addr_of!(sysctl_vals.0) as *const i32 }
// CHECK-NEXT:             .wrapping_offset(1i32 as isize) as *mut (),
// CHECK-NEXT:         max: unsafe { core::ptr::addr_of!(sysctl_vals.0) as *const i32 }
// CHECK-NEXT:             .wrapping_offset(3i32 as isize) as *mut (),
// CHECK-NEXT:     },
// CHECK-NEXT:     ctl {
// CHECK-NEXT:         name: core::ptr::null::<i8>(),
// CHECK-NEXT:         min: core::ptr::null_mut::<()>(),
// CHECK-NEXT:         max: core::ptr::null_mut::<()>(),
// CHECK-NEXT:     },
// CHECK-NEXT: ]);
// CHECK-NEXT: static mut later: *const i32 = unsafe { core::ptr::addr_of!(unsized_vals) as *const i32 }
// CHECK-NEXT:     .wrapping_offset(2i32 as isize);
// CHECK-NEXT: static mut past: *const i32 = unsafe { core::ptr::addr_of!(sysctl_vals.0) as *const i32 }
// CHECK-NEXT:     .wrapping_offset(4i32 as isize);
// CHECK-NEXT: static unsized_vals: [i32; 3] = [7, 8, 9];
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%lx %lx %lx\n".as_ptr(),
// CHECK-NEXT:             poisoned.next as u64,
// CHECK-NEXT:             poisoned.prev as u64,
// CHECK-NEXT:             skb_next as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%lx %lx %lx\n".as_ptr(),
// CHECK-NEXT:             char_offset as u64,
// CHECK-NEXT:             int_offset as u64,
// CHECK-NEXT:             back as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%s %d %d\n".as_ptr(),
// CHECK-NEXT:             table.0[0].name,
// CHECK-NEXT:             *(table.0[0].min as *mut i32),
// CHECK-NEXT:             *(table.0[0].max as *mut i32),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %d\n".as_ptr(),
// CHECK-NEXT:             *later,
// CHECK-NEXT:             (past == (core::ptr::addr_of!(sysctl_vals.0) as *const i32).add(4)) as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     0
// CHECK-NEXT: }
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
