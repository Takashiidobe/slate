int printf(const char *, ...);
struct spinlock { int held; };
struct mutex { int owner; int depth; };
#define __overload static inline __attribute__((overloadable, always_inline))
__overload void lock_ctx(struct spinlock *lock) { lock->held = 1; }
__overload void lock_ctx(struct mutex *lock) { lock->owner = 7; lock->depth++; }
__overload int held(const struct spinlock *lock) { return lock->held; }
__overload int held(const struct mutex *lock) { return lock->owner * 10 + lock->depth; }
static int __attribute__((overloadable)) pick(int x) { return x + 1; }
static int __attribute__((overloadable)) pick(double x) { return (int)(x * 2); }
static int __attribute__((overloadable)) pick(void *p) { return p != 0; }
int __attribute__((overloadable)) scale(int x) { return x * 3; }
int __attribute__((overloadable)) scale(long x) { return (int)(x * 5); }
int pick__1 = 40;
int main(void) {
  struct spinlock s = {0};
  struct mutex m = {0, 1};
  lock_ctx(&s);
  lock_ctx(&m);
  printf("%d %d\n", held(&s), held(&m));
  printf("%d %d %d %d\n", pick('a'), pick(1.5f), pick((void *)&s), pick__1);
  printf("%d %d\n", scale(2), scale(2L));
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
// CHECK-NEXT: struct spinlock {
// CHECK-NEXT:     held: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct mutex {
// CHECK-NEXT:     owner: i32,
// CHECK-NEXT:     depth: i32,
// CHECK-NEXT: }
// CHECK-NEXT: fn lock_ctx__1(lock: &mut spinlock) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(lock as *mut spinlock)).held = 1;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn lock_ctx__2(lock: &mut mutex) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(lock as *mut mutex)).owner = 7;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*lock).depth = (*lock).depth + 1;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn held__1(lock: &spinlock) -> i32 {
// CHECK-NEXT:     unsafe { (*(lock as *const spinlock)).held }
// CHECK-NEXT: }
// CHECK-NEXT: fn held__2(lock: &mutex) -> i32 {
// CHECK-NEXT:     (unsafe { (*(lock as *const mutex)).owner }) * 10
// CHECK-NEXT:         + unsafe { (*(lock as *const mutex)).depth }
// CHECK-NEXT: }
// CHECK-NEXT: fn [[PICK__1_0:pick__[0-9]+_[0-9]+]](x: i32) -> i32 {
// CHECK-NEXT:     x + 1
// CHECK-NEXT: }
// CHECK-NEXT: fn pick__2(x: f64) -> i32 {
// CHECK-NEXT:     (x * (2i32 as f64)) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn pick__3(p: *mut ()) -> i32 {
// CHECK-NEXT:     !p.is_null() as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn scale__1(x: i32) -> i32 {
// CHECK-NEXT:     x * 3
// CHECK-NEXT: }
// CHECK-NEXT: fn scale__2(x: i64) -> i32 {
// CHECK-NEXT:     (x * 5) as i32
// CHECK-NEXT: }
// CHECK-NEXT: static mut pick__1: i32 = 40;
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut s: spinlock = spinlock { held: 0 };
// CHECK-NEXT:     let mut m: mutex = mutex { owner: 0, depth: 1 };
// CHECK-NEXT:     lock_ctx__1(unsafe { &mut (*std::ptr::addr_of_mut!(s)) });
// CHECK-NEXT:     lock_ctx__2(unsafe { &mut (*std::ptr::addr_of_mut!(m)) });
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %d\n".as_ptr(),
// CHECK-NEXT:             held__1(&(*(std::ptr::addr_of_mut!(s) as *const spinlock))),
// CHECK-NEXT:             held__2(&(*(std::ptr::addr_of_mut!(m) as *const mutex))),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %d %d %d\n".as_ptr(),
// CHECK-NEXT:             [[PICK__1_0]](97i32),
// CHECK-NEXT:             pick__2(1.5f32 as f64),
// CHECK-NEXT:             pick__3(std::ptr::addr_of_mut!(s) as *mut ()),
// CHECK-NEXT:             pick__1,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { printf(c"%d %d\n".as_ptr(), scale__1(2i32), scale__2(2i64)) };
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
