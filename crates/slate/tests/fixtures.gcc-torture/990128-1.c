extern void abort(void);
extern void exit(int);
extern int  printf(const char *, ...);

struct s {
  struct s *n;
}       *p;
struct s ss;
#define MAX 10
struct s sss[MAX];
int      count = 0;

void sub(struct s *p, struct s **pp);
int  look(struct s *p, struct s **pp);

int main(void) {
  struct s *pp;
  struct s *next;
  int       i;

  p    = &ss;
  next = p;
  for (i = 0; i < MAX; i++) {
    next->n = &sss[i];
    next    = next->n;
  }
  next->n = 0;

  sub(p, &pp);
  if (count != MAX + 2)
    abort();

  exit(0);
}

void sub(struct s *p, struct s **pp) {
  for (; look(p, pp);) {
    if (p)
      p = p->n;
    else
      break;
  }
}

int look(struct s *p, struct s **pp) {
  for (; p; p = p->n)
    ;
  *pp = p;
  count++;
  return (1);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT:     fn printf([[V_1:__v[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct s {
// CHECK-NEXT:     n: *mut s,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut p: *mut s = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut ss: s = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut sss: __SlateAlign16<[s; 10]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-NEXT: static mut count: i32 = 0 as i32;
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut pp: *mut s = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut next: *mut s = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         p = unsafe { std::ptr::addr_of_mut!(ss) };
// CHECK-NEXT:     }
// CHECK-NEXT:     next = unsafe { p };
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (10 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*next).n = unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(sss.0) as *mut s }.offset(i as isize)
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         next = unsafe { (*next).n };
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:         i = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*next).n = std::ptr::null_mut::<s>();
// CHECK-NEXT:     }
// CHECK-NEXT:     sub(unsafe { p }, unsafe { &mut (*std::ptr::addr_of_mut!(pp)) });
// CHECK-NEXT:     if (unsafe { count }) != (10 as i32) + (2 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: fn sub(mut [[P_0:p_[0-9]+]]: *mut s, mut pp: &mut *mut s) {
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while look([[P_0]], pp) != (0 as i32) {
// CHECK-NEXT:         if [[P_0]] != std::ptr::null_mut::<s>() {
// CHECK-NEXT:             [[P_0]] = unsafe { (*[[P_0]]).n };
// CHECK-NEXT:         } else {
// CHECK-NEXT:             break '[[SLATE_BREAK_1]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn look(mut [[P_1:p_[0-9]+]]: *mut s, mut pp: &mut *mut s) -> i32 {
// CHECK-NEXT:     '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while [[P_1]] != std::ptr::null_mut::<s>() {
// CHECK-NEXT:         {}
// CHECK-NEXT:         [[P_1]] = unsafe { (*[[P_1]]).n };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(pp as *mut *mut s) = [[P_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: i32 = unsafe { count };
// CHECK-NEXT:     let mut [[V_5:__v[0-9]+]]: i32 = [[V_4]] + (1 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         count = [[V_5]];
// CHECK-NEXT:     }
// CHECK-NEXT:     1 as i32
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
