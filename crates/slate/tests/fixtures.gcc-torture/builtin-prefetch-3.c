/* Test that __builtin_prefetch does no harm.

   Prefetch data using a variety of storage classes and address
   expressions with volatile variables and pointers.  */

void exit(int);

int          glob_int_arr[100];
int          glob_int = 4;
volatile int glob_vol_int_arr[100];
int *volatile glob_vol_ptr_int              = glob_int_arr;
volatile int *glob_ptr_vol_int              = glob_vol_int_arr;
volatile int *volatile glob_vol_ptr_vol_int = glob_vol_int_arr;
volatile int glob_vol_int;

static int          stat_int_arr[100];
static volatile int stat_vol_int_arr[100];
static int *volatile stat_vol_ptr_int              = stat_int_arr;
static volatile int *stat_ptr_vol_int              = stat_vol_int_arr;
static volatile int *volatile stat_vol_ptr_vol_int = stat_vol_int_arr;
static volatile int stat_vol_int;

struct S {
  int       a;
  short     b, c;
  char      d[8];
  struct S *next;
};

struct S          str;
volatile struct S vol_str;
struct S *volatile vol_ptr_str              = &str;
volatile struct S *ptr_vol_str              = &vol_str;
volatile struct S *volatile vol_ptr_vol_str = &vol_str;

/* Prefetch volatile global variables using the address of the variable.  */

void simple_vol_global() {
  __builtin_prefetch(glob_vol_int_arr, 0, 0);
  __builtin_prefetch(glob_vol_ptr_int, 0, 0);
  __builtin_prefetch(glob_ptr_vol_int, 0, 0);
  __builtin_prefetch(glob_vol_ptr_vol_int, 0, 0);
  __builtin_prefetch(&glob_vol_int, 0, 0);
}

/* Prefetch volatile static variables using the address of the variable.  */

void simple_vol_file() {
  __builtin_prefetch(stat_vol_int_arr, 0, 0);
  __builtin_prefetch(stat_vol_ptr_int, 0, 0);
  __builtin_prefetch(stat_ptr_vol_int, 0, 0);
  __builtin_prefetch(stat_vol_ptr_vol_int, 0, 0);
  __builtin_prefetch(&stat_vol_int, 0, 0);
}

/* Prefetch using address expressions involving volatile global variables.  */

void expr_vol_global(void) {
  __builtin_prefetch(&vol_str, 0, 0);
  __builtin_prefetch(ptr_vol_str, 0, 0);
  __builtin_prefetch(vol_ptr_str, 0, 0);
  __builtin_prefetch(vol_ptr_vol_str, 0, 0);
  __builtin_prefetch(&vol_str.b, 0, 0);
  __builtin_prefetch(&ptr_vol_str->b, 0, 0);
  __builtin_prefetch(&vol_ptr_str->b, 0, 0);
  __builtin_prefetch(&vol_ptr_vol_str->b, 0, 0);
  __builtin_prefetch(&vol_str.d, 0, 0);
  __builtin_prefetch(&vol_ptr_str->d, 0, 0);
  __builtin_prefetch(&ptr_vol_str->d, 0, 0);
  __builtin_prefetch(&vol_ptr_vol_str->d, 0, 0);
  __builtin_prefetch(vol_str.next, 0, 0);
  __builtin_prefetch(vol_ptr_str->next, 0, 0);
  __builtin_prefetch(ptr_vol_str->next, 0, 0);
  __builtin_prefetch(vol_ptr_vol_str->next, 0, 0);
  __builtin_prefetch(vol_str.next->d, 0, 0);
  __builtin_prefetch(vol_ptr_str->next->d, 0, 0);
  __builtin_prefetch(ptr_vol_str->next->d, 0, 0);
  __builtin_prefetch(vol_ptr_vol_str->next->d, 0, 0);

  __builtin_prefetch(&glob_vol_int_arr, 0, 0);
  __builtin_prefetch(glob_vol_ptr_int, 0, 0);
  __builtin_prefetch(glob_ptr_vol_int, 0, 0);
  __builtin_prefetch(glob_vol_ptr_vol_int, 0, 0);
  __builtin_prefetch(&glob_vol_int_arr[2], 0, 0);
  __builtin_prefetch(&glob_vol_ptr_int[3], 0, 0);
  __builtin_prefetch(&glob_ptr_vol_int[3], 0, 0);
  __builtin_prefetch(&glob_vol_ptr_vol_int[3], 0, 0);
  __builtin_prefetch(glob_vol_int_arr + 3, 0, 0);
  __builtin_prefetch(glob_vol_int_arr + glob_vol_int, 0, 0);
  __builtin_prefetch(glob_vol_ptr_int + 5, 0, 0);
  __builtin_prefetch(glob_ptr_vol_int + 5, 0, 0);
  __builtin_prefetch(glob_vol_ptr_vol_int + 5, 0, 0);
  __builtin_prefetch(glob_vol_ptr_int + glob_vol_int, 0, 0);
  __builtin_prefetch(glob_ptr_vol_int + glob_vol_int, 0, 0);
  __builtin_prefetch(glob_vol_ptr_vol_int + glob_vol_int, 0, 0);
}

int main() {
  simple_vol_global();
  simple_vol_file();

  str.next     = &str;
  vol_str.next = &str;
  expr_vol_global();

  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Test that __builtin_prefetch does no harm.
// CHECK-NEXT: //!
// CHECK-NEXT: //! Prefetch data using a variety of storage classes and address
// CHECK-NEXT: //! expressions with volatile variables and pointers.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT:     fn __builtin_prefetch([[V_1:__v[0-9]+]]: *const (), ...);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut glob_int_arr: __SlateAlign16<[i32; 100]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut glob_int: i32 = 4 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut glob_vol_int_arr: __SlateAlign16<[i32; 100]> =
// CHECK-NEXT:     __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: static mut glob_vol_ptr_int: *mut i32 =
// CHECK-NEXT:     unsafe { std::ptr::addr_of_mut!(glob_int_arr.0) as *mut i32 };
// CHECK-EMPTY:
// CHECK-NEXT: static mut glob_ptr_vol_int: *mut i32 =
// CHECK-NEXT:     unsafe { std::ptr::addr_of_mut!(glob_vol_int_arr.0) as *mut i32 };
// CHECK-EMPTY:
// CHECK-NEXT: static mut glob_vol_ptr_vol_int: *mut i32 =
// CHECK-NEXT:     unsafe { std::ptr::addr_of_mut!(glob_vol_int_arr.0) as *mut i32 };
// CHECK-EMPTY:
// CHECK-NEXT: static mut glob_vol_int: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut stat_int_arr: __SlateAlign16<[i32; 100]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: static mut stat_vol_int_arr: __SlateAlign16<[i32; 100]> =
// CHECK-NEXT:     __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: static mut stat_vol_ptr_int: *mut i32 =
// CHECK-NEXT:     unsafe { std::ptr::addr_of_mut!(stat_int_arr.0) as *mut i32 };
// CHECK-EMPTY:
// CHECK-NEXT: static mut stat_ptr_vol_int: *mut i32 =
// CHECK-NEXT:     unsafe { std::ptr::addr_of_mut!(stat_vol_int_arr.0) as *mut i32 };
// CHECK-EMPTY:
// CHECK-NEXT: static mut stat_vol_ptr_vol_int: *mut i32 =
// CHECK-NEXT:     unsafe { std::ptr::addr_of_mut!(stat_vol_int_arr.0) as *mut i32 };
// CHECK-EMPTY:
// CHECK-NEXT: static mut stat_vol_int: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
// CHECK-NEXT:     a: i32,
// CHECK-NEXT:     b: i16,
// CHECK-NEXT:     c: i16,
// CHECK-NEXT:     d: [i8; 8],
// CHECK-NEXT:     next: *mut S,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut str: S = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut vol_str: S = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut vol_ptr_str: *mut S = unsafe { std::ptr::addr_of_mut!(str) };
// CHECK-EMPTY:
// CHECK-NEXT: static mut ptr_vol_str: *mut S = unsafe { std::ptr::addr_of_mut!(vol_str) };
// CHECK-EMPTY:
// CHECK-NEXT: static mut vol_ptr_vol_str: *mut S = unsafe { std::ptr::addr_of_mut!(vol_str) };
// CHECK-EMPTY:
// CHECK-NEXT: // Prefetch volatile global variables using the address of the variable.
// CHECK-NEXT: fn simple_vol_global() {
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // Prefetch volatile static variables using the address of the variable.
// CHECK-NEXT: fn simple_vol_file() {
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // Prefetch using address expressions involving volatile global variables.
// CHECK-NEXT: fn expr_vol_global() {
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     simple_vol_global();
// CHECK-NEXT:     simple_vol_file();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         str.next = unsafe { std::ptr::addr_of_mut!(str) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::write_volatile(std::ptr::addr_of_mut!(vol_str.next), unsafe {
// CHECK-NEXT:             std::ptr::addr_of_mut!(str)
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     expr_vol_global();
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
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
// SLATE-FILECHECK-END
