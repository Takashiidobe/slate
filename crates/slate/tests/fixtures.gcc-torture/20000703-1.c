void abort(void);
void exit(int);
struct baz {
  char         a[17];
  char         b[3];
  unsigned int c;
  unsigned int d;
};

void foo(struct baz *p, unsigned int c, unsigned int d) {
  __builtin_memcpy(p->b, "abc", 3);
  p->c = c;
  p->d = d;
}

void bar(struct baz *p, unsigned int c, unsigned int d) {
  ({
    void *s = (p);
    __builtin_memset(s, '\0', sizeof(struct baz));
    s;
  });
  __builtin_memcpy(p->a, "01234567890123456", 17);
  __builtin_memcpy(p->b, "abc", 3);
  p->c = c;
  p->d = d;
}

int main() {
  struct baz p;
  foo(&p, 71, 18);
  if (p.c != 71 || p.d != 18)
    abort();
  bar(&p, 59, 26);
  if (p.c != 59 || p.d != 26)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT:     #[link_name = "memcpy"]
// CHECK-NEXT:     fn __builtin_memcpy([[V_1:__v[0-9]+]]: *mut (), [[V_2:__v[0-9]+]]: *const (), [[V_3:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT:     #[link_name = "memset"]
// CHECK-NEXT:     fn __builtin_memset([[V_4:__v[0-9]+]]: *mut (), [[V_5:__v[0-9]+]]: i32, [[V_6:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct baz {
// CHECK-NEXT:     a: [i8; 17],
// CHECK-NEXT:     b: [i8; 3],
// CHECK-NEXT:     c: u32,
// CHECK-NEXT:     d: u32,
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(mut p: &mut baz, mut c: u32, mut d: u32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memcpy(
// CHECK-NEXT:             (unsafe { (*(p as *mut baz)).b.as_mut_ptr() as *mut i8 }) as *mut (),
// CHECK-NEXT:             b"abc\0".as_ptr() as *const (),
// CHECK-NEXT:             ((3 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(p as *mut baz)).c = c;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(p as *mut baz)).d = d;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn bar(mut p: *mut baz, mut c: u32, mut d: u32) {
// CHECK-NEXT:     let mut [[V_7:__v[0-9]+]]: *mut ();
// CHECK-NEXT:     let mut s: *mut () = p as *mut ();
// CHECK-NEXT:     unsafe { __builtin_memset(s, 0 as i32, 28 as u64) };
// CHECK-NEXT:     [[V_7]] = s;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memcpy(
// CHECK-NEXT:             (unsafe { (*p).a.as_mut_ptr() as *mut i8 }) as *mut (),
// CHECK-NEXT:             b"01234567890123456\0".as_ptr() as *const (),
// CHECK-NEXT:             ((17 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memcpy(
// CHECK-NEXT:             (unsafe { (*p).b.as_mut_ptr() as *mut i8 }) as *mut (),
// CHECK-NEXT:             b"abc\0".as_ptr() as *const (),
// CHECK-NEXT:             ((3 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*p).c = c;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*p).d = d;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut p: baz = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     foo(
// CHECK-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(p)) },
// CHECK-NEXT:         (71 as i32) as u32,
// CHECK-NEXT:         (18 as i32) as u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     if p.c != ((71 as i32) as u32) || p.d != ((18 as i32) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     bar(std::ptr::addr_of_mut!(p), (59 as i32) as u32, (26 as i32) as u32);
// CHECK-NEXT:     if p.c != ((59 as i32) as u32) || p.d != ((26 as i32) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
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
