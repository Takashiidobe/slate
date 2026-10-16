/* PR middle-end/19874 */
typedef enum { A, B, C, D } E;

struct S {
  E __attribute__((mode(__byte__))) a;
  E __attribute__((mode(__byte__))) b;
  E __attribute__((mode(__byte__))) c;
  E __attribute__((mode(__byte__))) d;
};

extern void abort(void);
extern void exit(int);

E foo(struct S *s) {
  if (s->a != s->b)
    abort();
  if (s->c != C)
    abort();
  return s->d;
}

int main(void) {
  struct S s[2];
  s[0].a = B;
  s[0].b = B;
  s[0].c = C;
  s[0].d = D;
  s[1].a = D;
  s[1].b = C;
  s[1].c = B;
  s[1].d = A;
  if (foo(s) != D)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // PR middle-end/19874
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
// CHECK-NEXT:     a: u8,
// CHECK-NEXT:     b: u8,
// CHECK-NEXT:     c: u8,
// CHECK-NEXT:     d: u8,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut s: *mut S) -> u32 {
// CHECK-NEXT:     if (((unsafe { (*s).a }) as u32) as i32) != (((unsafe { (*s).b }) as u32) as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (((unsafe { (*s).c }) as u32) as i32) != (2 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     (unsafe { (*s).d }) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut s: [S; 2] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe { (s.as_mut_ptr() as *mut S).offset((0 as i32) as isize) }).a =
// CHECK-NEXT:             ((1 as i32) as i8) as u8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe { (s.as_mut_ptr() as *mut S).offset((0 as i32) as isize) }).b =
// CHECK-NEXT:             ((1 as i32) as i8) as u8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe { (s.as_mut_ptr() as *mut S).offset((0 as i32) as isize) }).c =
// CHECK-NEXT:             ((2 as i32) as i8) as u8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe { (s.as_mut_ptr() as *mut S).offset((0 as i32) as isize) }).d =
// CHECK-NEXT:             ((3 as i32) as i8) as u8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe { (s.as_mut_ptr() as *mut S).offset((1 as i32) as isize) }).a =
// CHECK-NEXT:             ((3 as i32) as i8) as u8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe { (s.as_mut_ptr() as *mut S).offset((1 as i32) as isize) }).b =
// CHECK-NEXT:             ((2 as i32) as i8) as u8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe { (s.as_mut_ptr() as *mut S).offset((1 as i32) as isize) }).c =
// CHECK-NEXT:             ((1 as i32) as i8) as u8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe { (s.as_mut_ptr() as *mut S).offset((1 as i32) as isize) }).d =
// CHECK-NEXT:             ((0 as i32) as i8) as u8;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (foo(s.as_mut_ptr() as *mut S) as u32) != ((3 as i32) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
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
