/* Test that __builtin_prefetch does no harm.

   Prefetch data using a variety of storage classes and address
   expressions.  */

void exit(int);

int  glob_int_arr[100];
int *glob_ptr_int = glob_int_arr;
int  glob_int     = 4;

static int  stat_int_arr[100];
static int *stat_ptr_int = stat_int_arr;
static int  stat_int;

struct S {
  int       a;
  short     b, c;
  char      d[8];
  struct S *next;
};

struct S  str;
struct S *ptr_str = &str;

/* Prefetch global variables using the address of the variable.  */

void simple_global() {
  __builtin_prefetch(glob_int_arr, 0, 0);
  __builtin_prefetch(glob_ptr_int, 0, 0);
  __builtin_prefetch(&glob_int, 0, 0);
}

/* Prefetch file-level static variables using the address of the variable.  */

void simple_file() {
  __builtin_prefetch(stat_int_arr, 0, 0);
  __builtin_prefetch(stat_ptr_int, 0, 0);
  __builtin_prefetch(&stat_int, 0, 0);
}

/* Prefetch local static variables using the address of the variable.  */

void simple_static_local() {
  static int  gx[100];
  static int *hx = gx;
  static int  ix;
  __builtin_prefetch(gx, 0, 0);
  __builtin_prefetch(hx, 0, 0);
  __builtin_prefetch(&ix, 0, 0);
}

/* Prefetch local stack variables using the address of the variable.  */

void simple_local() {
  int  gx[100];
  int *hx = gx;
  int  ix;
  __builtin_prefetch(gx, 0, 0);
  __builtin_prefetch(hx, 0, 0);
  __builtin_prefetch(&ix, 0, 0);
}

/* Prefetch arguments using the address of the variable.  */

void simple_arg(int g[100], int *h, int i) {
  __builtin_prefetch(g, 0, 0);
  __builtin_prefetch(h, 0, 0);
  __builtin_prefetch(&i, 0, 0);
}

/* Prefetch using address expressions involving global variables.  */

void expr_global(void) {
  __builtin_prefetch(&str, 0, 0);
  __builtin_prefetch(ptr_str, 0, 0);
  __builtin_prefetch(&str.b, 0, 0);
  __builtin_prefetch(&ptr_str->b, 0, 0);
  __builtin_prefetch(&str.d, 0, 0);
  __builtin_prefetch(&ptr_str->d, 0, 0);
  __builtin_prefetch(str.next, 0, 0);
  __builtin_prefetch(ptr_str->next, 0, 0);
  __builtin_prefetch(str.next->d, 0, 0);
  __builtin_prefetch(ptr_str->next->d, 0, 0);

  __builtin_prefetch(&glob_int_arr, 0, 0);
  __builtin_prefetch(glob_ptr_int, 0, 0);
  __builtin_prefetch(&glob_int_arr[2], 0, 0);
  __builtin_prefetch(&glob_ptr_int[3], 0, 0);
  __builtin_prefetch(glob_int_arr + 3, 0, 0);
  __builtin_prefetch(glob_int_arr + glob_int, 0, 0);
  __builtin_prefetch(glob_ptr_int + 5, 0, 0);
  __builtin_prefetch(glob_ptr_int + glob_int, 0, 0);
}

/* Prefetch using address expressions involving local variables.  */

void expr_local(void) {
  int       b[10];
  int      *pb = b;
  struct S  t;
  struct S *pt = &t;
  int       j  = 4;

  __builtin_prefetch(&t, 0, 0);
  __builtin_prefetch(pt, 0, 0);
  __builtin_prefetch(&t.b, 0, 0);
  __builtin_prefetch(&pt->b, 0, 0);
  __builtin_prefetch(&t.d, 0, 0);
  __builtin_prefetch(&pt->d, 0, 0);
  __builtin_prefetch(t.next, 0, 0);
  __builtin_prefetch(pt->next, 0, 0);
  __builtin_prefetch(t.next->d, 0, 0);
  __builtin_prefetch(pt->next->d, 0, 0);

  __builtin_prefetch(&b, 0, 0);
  __builtin_prefetch(pb, 0, 0);
  __builtin_prefetch(&b[2], 0, 0);
  __builtin_prefetch(&pb[3], 0, 0);
  __builtin_prefetch(b + 3, 0, 0);
  __builtin_prefetch(b + j, 0, 0);
  __builtin_prefetch(pb + 5, 0, 0);
  __builtin_prefetch(pb + j, 0, 0);
}

int main() {
  simple_global();
  simple_file();
  simple_static_local();
  simple_local();
  simple_arg(glob_int_arr, glob_ptr_int, glob_int);

  str.next = &str;
  expr_global();
  expr_local();

  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Test that __builtin_prefetch does no harm.
// CHECK-NEXT: //!
// CHECK-NEXT: //! Prefetch data using a variety of storage classes and address
// CHECK-NEXT: //! expressions.
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
// CHECK-NEXT: static mut glob_ptr_int: *mut i32 = unsafe { std::ptr::addr_of_mut!(glob_int_arr.0) as *mut i32 };
// CHECK-EMPTY:
// CHECK-NEXT: static mut glob_int: i32 = 4 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut stat_int_arr: __SlateAlign16<[i32; 100]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: static mut stat_ptr_int: *mut i32 = unsafe { std::ptr::addr_of_mut!(stat_int_arr.0) as *mut i32 };
// CHECK-EMPTY:
// CHECK-NEXT: static mut stat_int: i32 = unsafe { std::mem::zeroed() };
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
// CHECK-NEXT: static mut ptr_str: *mut S = unsafe { std::ptr::addr_of_mut!(str) };
// CHECK-EMPTY:
// CHECK-NEXT: // Prefetch global variables using the address of the variable.
// CHECK-NEXT: fn simple_global() {
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // Prefetch file-level static variables using the address of the variable.
// CHECK-NEXT: fn simple_file() {
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // Prefetch local static variables using the address of the variable.
// CHECK-NEXT: fn simple_static_local() {
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut gx: __SlateAlign16<[i32; 100]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: static mut hx: *mut i32 = unsafe { std::ptr::addr_of_mut!(gx.0) as *mut i32 };
// CHECK-EMPTY:
// CHECK-NEXT: static mut ix: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: // Prefetch local stack variables using the address of the variable.
// CHECK-NEXT: fn simple_local() {
// CHECK-NEXT:     let mut [[GX_0:gx_[0-9]+]]: __SlateAlign16<[i32; 100]> = __SlateAlign16([0 as i32; 100]);
// CHECK-NEXT:     [[GX_0]].0.as_mut_ptr() as *mut i32;
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // Prefetch arguments using the address of the variable.
// CHECK-NEXT: fn simple_arg(mut g: *mut i32, mut h: *mut i32, mut i: i32) {
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // Prefetch using address expressions involving global variables.
// CHECK-NEXT: fn expr_global() {
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
// CHECK-NEXT: // Prefetch using address expressions involving local variables.
// CHECK-NEXT: fn expr_local() {
// CHECK-NEXT:     let mut b: __SlateAlign16<[i32; 10]> = __SlateAlign16([0 as i32; 10]);
// CHECK-NEXT:     b.0.as_mut_ptr() as *mut i32;
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
// CHECK-NEXT:     simple_global();
// CHECK-NEXT:     simple_file();
// CHECK-NEXT:     simple_static_local();
// CHECK-NEXT:     simple_local();
// CHECK-NEXT:     simple_arg(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(glob_int_arr.0) as *mut i32 },
// CHECK-NEXT:         unsafe { glob_ptr_int },
// CHECK-NEXT:         unsafe { glob_int },
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         str.next = unsafe { std::ptr::addr_of_mut!(str) };
// CHECK-NEXT:     }
// CHECK-NEXT:     expr_global();
// CHECK-NEXT:     expr_local();
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
