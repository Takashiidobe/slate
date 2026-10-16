/* PR optimization/15296.  The delayed-branch scheduler caused code that
   SEGV:d for CRIS; a register was set to -1 in a delay-slot for the
   fall-through code, while that register held a pointer used in code at
   the branch target.  */

void abort(void);
void exit(int);

typedef __INTPTR_TYPE__ intptr_t;
typedef intptr_t        W;
union u0 {
  union u0 *r;
  W         i;
};
struct s1 {
  union u0 **m0;
  union u0   m1[4];
};

void f(void *, struct s1 *, const union u0 *, W, W, W)
    __attribute__((__noinline__));
void g(void *, char *) __attribute__((__noinline__));

void f(void *a, struct s1 *b, const union u0 *h, W v0, W v1, W v4) {
  union u0  *e  = 0;
  union u0  *k  = 0;
  union u0 **v5 = b->m0;
  union u0  *c  = b->m1;
  union u0 **d  = &v5[0];
l0:;
  if (v0 < v1)
    goto l0;
  if (v0 == 0)
    goto l3;
  v0 = v4;
  if (v0 != 0)
    goto l3;
  c[0].r = *d;
  v1     = -1;
  e      = c[0].r;
  if (e != 0)
    g(a, "");
  k    = e + 3;
  k->i = v1;
  goto l4;
l3:;
  c[0].i = v0;
  e      = c[1].r;
  if (e != 0)
    g(a, "");
  e = c[0].r;
  if (e == 0)
    g(a, "");
  k    = e + 2;
  k->r = c[1].r;
l4:;
}

void g(void *a, char *b) { abort(); }

int main() {
  union u0  uv[] = {{.i = 111}, {.i = 222}, {.i = 333}, {.i = 444}};
  struct s1 s    = {0, {{.i = 555}, {.i = 0}, {.i = 999}, {.i = 777}}};
  f(0, &s, 0, 20000, 10000, (W)uv);
  if (s.m1[0].i != (W)uv || s.m1[1].i != 0 || s.m1[2].i != 999 ||
      s.m1[3].i != 777 || uv[0].i != 111 || uv[1].i != 222 || uv[2].i != 0 ||
      uv[3].i != 444)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR optimization/15296.  The delayed-branch scheduler caused code that
// CHECK-NEXT: //! SEGV:d for CRIS; a register was set to -1 in a delay-slot for the
// CHECK-NEXT: //! fall-through code, while that register held a pointer used in code at
// CHECK-NEXT: //! the branch target.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union u0 {
// CHECK-NEXT:     r: *mut u0,
// CHECK-NEXT:     i: i64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct s1 {
// CHECK-NEXT:     m0: *mut *mut u0,
// CHECK-NEXT:     m1: [u0; 4],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: fn f(mut a: *mut (), mut b: &mut s1, mut h: *const u0, mut v0: i64, mut v1: i64, mut v4: i64) {
// CHECK-NEXT:     let mut [[SLATE_STORAGE_0:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<*mut *mut u0> =
// CHECK-NEXT:         std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_0:__slate_slot_[0-9]+]]: *mut *mut *mut u0 =
// CHECK-NEXT:         std::ptr::addr_of_mut!([[SLATE_STORAGE_0]]) as *mut *mut *mut u0;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_1:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<*mut u0> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_1:__slate_slot_[0-9]+]]: *mut *mut u0 = std::ptr::addr_of_mut!([[SLATE_STORAGE_1]]) as *mut *mut u0;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_2:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<*mut *mut u0> =
// CHECK-NEXT:         std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_2:__slate_slot_[0-9]+]]: *mut *mut *mut u0 =
// CHECK-NEXT:         std::ptr::addr_of_mut!([[SLATE_STORAGE_2]]) as *mut *mut *mut u0;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_3:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<*mut u0> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_3:__slate_slot_[0-9]+]]: *mut *mut u0 = std::ptr::addr_of_mut!([[SLATE_STORAGE_3]]) as *mut *mut u0;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_4:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<*mut u0> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_4:__slate_slot_[0-9]+]]: *mut *mut u0 = std::ptr::addr_of_mut!([[SLATE_STORAGE_4]]) as *mut *mut u0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::write([[SLATE_SLOT_4]], std::ptr::null_mut::<u0>());
// CHECK-NEXT:         std::ptr::write([[SLATE_SLOT_3]], std::ptr::null_mut::<u0>());
// CHECK-NEXT:         std::ptr::write([[SLATE_SLOT_2]], unsafe { (*(b as *mut s1)).m0 });
// CHECK-NEXT:         std::ptr::write([[SLATE_SLOT_1]], unsafe {
// CHECK-NEXT:             (*(b as *mut s1)).m1.as_mut_ptr() as *mut u0
// CHECK-NEXT:         });
// CHECK-NEXT:         std::ptr::write([[SLATE_SLOT_0]], unsafe {
// CHECK-NEXT:             (*[[SLATE_SLOT_2]]).offset((0 as i32) as isize)
// CHECK-NEXT:         });
// CHECK-NEXT:         loop {
// CHECK-NEXT:             {}
// CHECK-NEXT:             if !(v0 < v1) {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         '[[JOIN_0:__join_[0-9]+]]: {
// CHECK-NEXT:             if v0 == ((0 as i32) as i64) {
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 v0 = v4;
// CHECK-NEXT:                 if v0 != ((0 as i32) as i64) {
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe { (*[[SLATE_SLOT_1]]).offset((0 as i32) as isize) }).r =
// CHECK-NEXT:                             unsafe { *(*[[SLATE_SLOT_0]]) };
// CHECK-NEXT:                     }
// CHECK-NEXT:                     v1 = -(1 as i32) as i64;
// CHECK-NEXT:                     *[[SLATE_SLOT_4]] =
// CHECK-NEXT:                         unsafe { (*unsafe { (*[[SLATE_SLOT_1]]).offset((0 as i32) as isize) }).r };
// CHECK-NEXT:                     if *[[SLATE_SLOT_4]] != std::ptr::null_mut::<u0>() {
// CHECK-NEXT:                         g(a, c"".as_ptr() as *mut i8);
// CHECK-NEXT:                     }
// CHECK-NEXT:                     *[[SLATE_SLOT_3]] = unsafe { (*[[SLATE_SLOT_4]]).offset((3 as i32) as isize) };
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*(*[[SLATE_SLOT_3]])).i = v1;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     break '[[JOIN_0]];
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             {}
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*unsafe { (*[[SLATE_SLOT_1]]).offset((0 as i32) as isize) }).i = v0;
// CHECK-NEXT:             }
// CHECK-NEXT:             *[[SLATE_SLOT_4]] =
// CHECK-NEXT:                 unsafe { (*unsafe { (*[[SLATE_SLOT_1]]).offset((1 as i32) as isize) }).r };
// CHECK-NEXT:             if *[[SLATE_SLOT_4]] != std::ptr::null_mut::<u0>() {
// CHECK-NEXT:                 g(a, c"".as_ptr() as *mut i8);
// CHECK-NEXT:             }
// CHECK-NEXT:             *[[SLATE_SLOT_4]] =
// CHECK-NEXT:                 unsafe { (*unsafe { (*[[SLATE_SLOT_1]]).offset((0 as i32) as isize) }).r };
// CHECK-NEXT:             if *[[SLATE_SLOT_4]] == std::ptr::null_mut::<u0>() {
// CHECK-NEXT:                 g(a, c"".as_ptr() as *mut i8);
// CHECK-NEXT:             }
// CHECK-NEXT:             *[[SLATE_SLOT_3]] = unsafe { (*[[SLATE_SLOT_4]]).offset((2 as i32) as isize) };
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*(*[[SLATE_SLOT_3]])).r =
// CHECK-NEXT:                     unsafe { (*unsafe { (*[[SLATE_SLOT_1]]).offset((1 as i32) as isize) }).r };
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         {}
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn g(mut a: *mut (), mut b: *mut i8) {
// CHECK-NEXT:     unsafe { abort() };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut uv: __SlateAlign16<[u0; 4]> = __SlateAlign16([
// CHECK-NEXT:         {
// CHECK-NEXT:             let mut [[T_0:__t[0-9]+]]: u0 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_0]].i = (111 as i32) as i64;
// CHECK-NEXT:             [[T_0]]
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:             let mut [[T_1:__t[0-9]+]]: u0 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_1]].i = (222 as i32) as i64;
// CHECK-NEXT:             [[T_1]]
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:             let mut [[T_2:__t[0-9]+]]: u0 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_2]].i = (333 as i32) as i64;
// CHECK-NEXT:             [[T_2]]
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:             let mut [[T_3:__t[0-9]+]]: u0 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_3]].i = (444 as i32) as i64;
// CHECK-NEXT:             [[T_3]]
// CHECK-NEXT:         },
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut s: s1 = s1 {
// CHECK-NEXT:         m0: std::ptr::null_mut::<*mut u0>(),
// CHECK-NEXT:         m1: [
// CHECK-NEXT:             {
// CHECK-NEXT:                 let mut [[T_4:__t[0-9]+]]: u0 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:                 [[T_4]].i = (555 as i32) as i64;
// CHECK-NEXT:                 [[T_4]]
// CHECK-NEXT:             },
// CHECK-NEXT:             {
// CHECK-NEXT:                 let mut [[T_5:__t[0-9]+]]: u0 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:                 [[T_5]].i = (0 as i32) as i64;
// CHECK-NEXT:                 [[T_5]]
// CHECK-NEXT:             },
// CHECK-NEXT:             {
// CHECK-NEXT:                 let mut [[T_6:__t[0-9]+]]: u0 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:                 [[T_6]].i = (999 as i32) as i64;
// CHECK-NEXT:                 [[T_6]]
// CHECK-NEXT:             },
// CHECK-NEXT:             {
// CHECK-NEXT:                 let mut [[T_7:__t[0-9]+]]: u0 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:                 [[T_7]].i = (777 as i32) as i64;
// CHECK-NEXT:                 [[T_7]]
// CHECK-NEXT:             },
// CHECK-NEXT:         ],
// CHECK-NEXT:     };
// CHECK-NEXT:     f(
// CHECK-NEXT:         std::ptr::null_mut::<()>(),
// CHECK-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(s)) },
// CHECK-NEXT:         std::ptr::null::<u0>(),
// CHECK-NEXT:         (20000 as i32) as i64,
// CHECK-NEXT:         (10000 as i32) as i64,
// CHECK-NEXT:         (uv.0.as_mut_ptr() as *mut u0) as i64,
// CHECK-NEXT:     );
// CHECK-NEXT:     if (unsafe { (*unsafe { (s.m1.as_mut_ptr() as *mut u0).offset((0 as i32) as isize) }).i })
// CHECK-NEXT:         != ((uv.0.as_mut_ptr() as *mut u0) as i64)
// CHECK-NEXT:         || (unsafe { (*unsafe { (s.m1.as_mut_ptr() as *mut u0).offset((1 as i32) as isize) }).i })
// CHECK-NEXT:             != ((0 as i32) as i64)
// CHECK-NEXT:         || (unsafe { (*unsafe { (s.m1.as_mut_ptr() as *mut u0).offset((2 as i32) as isize) }).i })
// CHECK-NEXT:             != ((999 as i32) as i64)
// CHECK-NEXT:         || (unsafe { (*unsafe { (s.m1.as_mut_ptr() as *mut u0).offset((3 as i32) as isize) }).i })
// CHECK-NEXT:             != ((777 as i32) as i64)
// CHECK-NEXT:         || (unsafe { (*unsafe { (uv.0.as_mut_ptr() as *mut u0).offset((0 as i32) as isize) }).i })
// CHECK-NEXT:             != ((111 as i32) as i64)
// CHECK-NEXT:         || (unsafe { (*unsafe { (uv.0.as_mut_ptr() as *mut u0).offset((1 as i32) as isize) }).i })
// CHECK-NEXT:             != ((222 as i32) as i64)
// CHECK-NEXT:         || (unsafe { (*unsafe { (uv.0.as_mut_ptr() as *mut u0).offset((2 as i32) as isize) }).i })
// CHECK-NEXT:             != ((0 as i32) as i64)
// CHECK-NEXT:         || (unsafe { (*unsafe { (uv.0.as_mut_ptr() as *mut u0).offset((3 as i32) as isize) }).i })
// CHECK-NEXT:             != ((444 as i32) as i64)
// CHECK-NEXT:     {
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
