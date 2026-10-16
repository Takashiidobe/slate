void abort(void);
void exit(int);

typedef __SIZE_TYPE__ size_t;
extern size_t         strlen(const char *s);

typedef struct A {
  int a, b;
} A;

typedef struct B {
  struct A **a;
  int        b;
} B;

A  *a;
int b = 1, c;
B   d[1];

void foo(A *x, const char *y, int z) { c = y[4] + z * 25; }

A *bar(const char *v, int w, int x, const char *y, int z) {
  if (w)
    abort();
  exit(0);
}

void test(const char *x, int *y) {
  foo(d->a[d->b], "test", 200);
  d->a[d->b] = bar(x, b ? 0 : 65536, strlen(x), "test", 201);
  d->a[d->b]->a++;
  if (y)
    d->a[d->b]->b = *y;
}

int main() {
  d->b = 0;
  d->a = &a;
  test("", 0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT:     fn strlen(s: *const i8) -> u64;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct A {
// CHECK-NEXT:     a: i32,
// CHECK-NEXT:     b: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct B {
// CHECK-NEXT:     a: *mut *mut A,
// CHECK-NEXT:     b: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut a: *mut A = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut b: i32 = 1 as i32;
// CHECK-NEXT: static mut c: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut d: __SlateAlign16<[B; 1]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-NEXT: fn foo(mut x: *mut A, mut y: *const i8, mut z: i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         c = ((unsafe { *unsafe { y.offset((4 as i32) as isize) } }) as i32)
// CHECK-NEXT:             + z * (25 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn bar(
// CHECK-NEXT:     mut v: *const i8,
// CHECK-NEXT:     mut w: i32,
// CHECK-NEXT:     mut x: i32,
// CHECK-NEXT:     mut y: *const i8,
// CHECK-NEXT:     mut z: i32,
// CHECK-NEXT: ) -> *mut A {
// CHECK-NEXT:     if w != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-NEXT: fn test(mut x: *const i8, mut y: *mut i32) {
// CHECK-NEXT:     foo(
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { (*unsafe { std::ptr::addr_of_mut!(d.0) as *mut B }).a }
// CHECK-NEXT:                     .offset(
// CHECK-NEXT:                         (unsafe {
// CHECK-NEXT:                             (*unsafe { std::ptr::addr_of_mut!(d.0) as *mut B }).b
// CHECK-NEXT:                         }) as isize,
// CHECK-NEXT:                     )
// CHECK-NEXT:             }
// CHECK-NEXT:         },
// CHECK-NEXT:         c"test".as_ptr() as *const i8,
// CHECK-NEXT:         200 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { (*unsafe { std::ptr::addr_of_mut!(d.0) as *mut B }).a }
// CHECK-NEXT:                 .offset(
// CHECK-NEXT:                     (unsafe { (*unsafe { std::ptr::addr_of_mut!(d.0) as *mut B }).b })
// CHECK-NEXT:                         as isize,
// CHECK-NEXT:                 )
// CHECK-NEXT:         } = bar(
// CHECK-NEXT:             x,
// CHECK-NEXT:             if (unsafe { b }) != (0 as i32) { 0 as i32 } else { 65536 as i32 },
// CHECK-NEXT:             ((unsafe { strlen(x) }) as u32) as i32,
// CHECK-NEXT:             c"test".as_ptr() as *const i8,
// CHECK-NEXT:             201 as i32,
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: *mut A = unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { (*unsafe { std::ptr::addr_of_mut!(d.0) as *mut B }).a }
// CHECK-NEXT:                 .offset(
// CHECK-NEXT:                     (unsafe { (*unsafe { std::ptr::addr_of_mut!(d.0) as *mut B }).b })
// CHECK-NEXT:                         as isize,
// CHECK-NEXT:                 )
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: i32 = unsafe { (*[[V_1]]).a };
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*[[V_1]]).a = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if y != std::ptr::null_mut::<i32>() {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 *unsafe {
// CHECK-NEXT:                     unsafe { (*unsafe { std::ptr::addr_of_mut!(d.0) as *mut B }).a }
// CHECK-NEXT:                         .offset(
// CHECK-NEXT:                             (unsafe {
// CHECK-NEXT:                                 (*unsafe { std::ptr::addr_of_mut!(d.0) as *mut B }).b
// CHECK-NEXT:                             }) as isize,
// CHECK-NEXT:                         )
// CHECK-NEXT:                 }
// CHECK-NEXT:             })
// CHECK-NEXT:                 .b = unsafe { *y };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe { std::ptr::addr_of_mut!(d.0) as *mut B }).b = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe { std::ptr::addr_of_mut!(d.0) as *mut B }).a = unsafe {
// CHECK-NEXT:             std::ptr::addr_of_mut!(a)
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     test(c"".as_ptr() as *const i8, std::ptr::null_mut::<i32>());
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
// SLATE-FILECHECK-END
