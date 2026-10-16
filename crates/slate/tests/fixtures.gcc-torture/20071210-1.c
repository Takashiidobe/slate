/* PR rtl-optimization/34302 */
/* { dg-require-effective-target label_values } */
/* { dg-require-effective-target indirect_jumps } */

extern void abort(void);

struct S {
  int n1, n2, n3, n4;
};

__attribute__((noinline)) struct S foo(int x, int y, int z) {
  if (x != 10 || y != 9 || z != 8)
    abort();
  struct S s = {1, 2, 3, 4};
  return s;
}

__attribute__((noinline)) void **bar(void **u, int *v) {
  void       **w = u;
  int         *s = v, x, y, z;
  void       **p, **q;
  static void *l[] = {&&lab1, &&lab1, &&lab2, &&lab3, &&lab4};

  if (!u)
    return l;

  q = *w++;
  goto *q;
lab2:
  p           = q;
  q           = *w++;
  x           = s[2];
  y           = s[1];
  z           = s[0];
  s          -= 1;
  struct S r  = foo(x, y, z);
  s[3]        = r.n1;
  s[2]        = r.n2;
  s[1]        = r.n3;
  s[0]        = r.n4;
  goto *q;
lab3:
  p     = q;
  q     = *w++;
  s    += 1;
  s[0]  = 23;
lab1:
  goto *q;
lab4:
  return 0;
}

int main(void) {
  void **u   = bar((void **)0, (int *)0);
  void  *t[] = {u[2], u[4]};
  int    s[] = {7, 8, 9, 10, 11, 12};
  if (bar(t, &s[1]) != (void **)0 || s[0] != 4 || s[1] != 3 || s[2] != 2 ||
      s[3] != 1 || s[4] != 11 || s[5] != 12)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/34302
// CHECK-NEXT: //!
// CHECK-NEXT: //! { dg-require-effective-target label_values }
// CHECK-NEXT: //!
// CHECK-NEXT: //! { dg-require-effective-target indirect_jumps }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
// CHECK-NEXT:     n1: i32,
// CHECK-NEXT:     n2: i32,
// CHECK-NEXT:     n3: i32,
// CHECK-NEXT:     n4: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: i32, mut y: i32, mut z: i32) -> S {
// CHECK-NEXT:     if x != (10 as i32) || y != (9 as i32) || z != (8 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut s: S = S {
// CHECK-NEXT:         n1: 1 as i32,
// CHECK-NEXT:         n2: 2 as i32,
// CHECK-NEXT:         n3: 3 as i32,
// CHECK-NEXT:         n4: 4 as i32,
// CHECK-NEXT:     };
// CHECK-NEXT:     return s;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut u: *mut *mut (), mut v: *mut i32) -> *mut *mut () {
// CHECK-NEXT:     let mut [[SLATE_STORAGE_0:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<*mut i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_0:__slate_slot_[0-9]+]]: *mut *mut i32 =
// CHECK-NEXT:         std::ptr::addr_of_mut!([[SLATE_STORAGE_0]]) as *mut *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_1:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<*mut i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_1:__slate_slot_[0-9]+]]: *mut *mut i32 =
// CHECK-NEXT:         std::ptr::addr_of_mut!([[SLATE_STORAGE_1]]) as *mut *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_2:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<*mut *mut ()> =
// CHECK-NEXT:         std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_2:__slate_slot_[0-9]+]]: *mut *mut *mut () =
// CHECK-NEXT:         std::ptr::addr_of_mut!([[SLATE_STORAGE_2]]) as *mut *mut *mut ();
// CHECK-NEXT:     let mut [[SLATE_STORAGE_3:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<*mut *mut ()> =
// CHECK-NEXT:         std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_3:__slate_slot_[0-9]+]]: *mut *mut *mut () =
// CHECK-NEXT:         std::ptr::addr_of_mut!([[SLATE_STORAGE_3]]) as *mut *mut *mut ();
// CHECK-NEXT:     let mut [[SLATE_STORAGE_4:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<S> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_4:__slate_slot_[0-9]+]]: *mut S = std::ptr::addr_of_mut!([[SLATE_STORAGE_4]]) as *mut S;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_5:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<*mut i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_5:__slate_slot_[0-9]+]]: *mut *mut i32 =
// CHECK-NEXT:         std::ptr::addr_of_mut!([[SLATE_STORAGE_5]]) as *mut *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_6:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<*mut i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_6:__slate_slot_[0-9]+]]: *mut *mut i32 =
// CHECK-NEXT:         std::ptr::addr_of_mut!([[SLATE_STORAGE_6]]) as *mut *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_7:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<*mut *mut ()> =
// CHECK-NEXT:         std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_7:__slate_slot_[0-9]+]]: *mut *mut *mut () =
// CHECK-NEXT:         std::ptr::addr_of_mut!([[SLATE_STORAGE_7]]) as *mut *mut *mut ();
// CHECK-NEXT:     let mut [[SLATE_STORAGE_8:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<*mut *mut ()> =
// CHECK-NEXT:         std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_8:__slate_slot_[0-9]+]]: *mut *mut *mut () =
// CHECK-NEXT:         std::ptr::addr_of_mut!([[SLATE_STORAGE_8]]) as *mut *mut *mut ();
// CHECK-NEXT:     let mut [[SLATE_STORAGE_9:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<*mut *mut ()> =
// CHECK-NEXT:         std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_9:__slate_slot_[0-9]+]]: *mut *mut *mut () =
// CHECK-NEXT:         std::ptr::addr_of_mut!([[SLATE_STORAGE_9]]) as *mut *mut *mut ();
// CHECK-NEXT:     let mut [[SLATE_STORAGE_10:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<*mut *mut ()> =
// CHECK-NEXT:         std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_10:__slate_slot_[0-9]+]]: *mut *mut *mut () =
// CHECK-NEXT:         std::ptr::addr_of_mut!([[SLATE_STORAGE_10]]) as *mut *mut *mut ();
// CHECK-NEXT:     let mut [[SLATE_STORAGE_11:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<*mut *mut ()> =
// CHECK-NEXT:         std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_11:__slate_slot_[0-9]+]]: *mut *mut *mut () =
// CHECK-NEXT:         std::ptr::addr_of_mut!([[SLATE_STORAGE_11]]) as *mut *mut *mut ();
// CHECK-NEXT:     let mut [[SLATE_STORAGE_12:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<*mut *mut ()> =
// CHECK-NEXT:         std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_12:__slate_slot_[0-9]+]]: *mut *mut *mut () =
// CHECK-NEXT:         std::ptr::addr_of_mut!([[SLATE_STORAGE_12]]) as *mut *mut *mut ();
// CHECK-NEXT:     let mut [[SLATE_STORAGE_13:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_13:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_13]]) as *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_14:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_14:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_14]]) as *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_15:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_15:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_15]]) as *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_16:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<*mut i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_16:__slate_slot_[0-9]+]]: *mut *mut i32 =
// CHECK-NEXT:         std::ptr::addr_of_mut!([[SLATE_STORAGE_16]]) as *mut *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_17:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<*mut *mut ()> =
// CHECK-NEXT:         std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_17:__slate_slot_[0-9]+]]: *mut *mut *mut () =
// CHECK-NEXT:         std::ptr::addr_of_mut!([[SLATE_STORAGE_17]]) as *mut *mut *mut ();
// CHECK-NEXT:     let mut __slate_state: usize = 6usize;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         '__slate_dispatch: loop {
// CHECK-NEXT:             match __slate_state {
// CHECK-NEXT:                 0 => {
// CHECK-NEXT:                     return std::ptr::null_mut::<*mut ()>();
// CHECK-NEXT:                 }
// CHECK-NEXT:                 1 => match *[[SLATE_SLOT_11]] as usize {
// CHECK-NEXT:                     9 => {
// CHECK-NEXT:                         __slate_state = 3usize;
// CHECK-NEXT:                         continue '__slate_dispatch;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     10 => {
// CHECK-NEXT:                         __slate_state = 2usize;
// CHECK-NEXT:                         continue '__slate_dispatch;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     11 => {
// CHECK-NEXT:                         __slate_state = 1usize;
// CHECK-NEXT:                         continue '__slate_dispatch;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     12 => {
// CHECK-NEXT:                         __slate_state = 0usize;
// CHECK-NEXT:                         continue '__slate_dispatch;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     _ => {
// CHECK-NEXT:                         unreachable!();
// CHECK-NEXT:                     }
// CHECK-NEXT:                 },
// CHECK-NEXT:                 2 => {
// CHECK-NEXT:                     *[[SLATE_SLOT_12]] = *[[SLATE_SLOT_11]];
// CHECK-NEXT:                     std::ptr::write([[SLATE_SLOT_3]], *[[SLATE_SLOT_17]]);
// CHECK-NEXT:                     std::ptr::write([[SLATE_SLOT_2]], unsafe {
// CHECK-NEXT:                         (*[[SLATE_SLOT_3]]).offset((1 as i32) as isize)
// CHECK-NEXT:                     });
// CHECK-NEXT:                     *[[SLATE_SLOT_17]] = *[[SLATE_SLOT_2]];
// CHECK-NEXT:                     *[[SLATE_SLOT_11]] = (unsafe { *(*[[SLATE_SLOT_3]]) }) as *mut *mut ();
// CHECK-NEXT:                     std::ptr::write([[SLATE_SLOT_1]], *[[SLATE_SLOT_16]]);
// CHECK-NEXT:                     std::ptr::write([[SLATE_SLOT_0]], unsafe {
// CHECK-NEXT:                         (*[[SLATE_SLOT_1]]).offset((1 as i32) as isize)
// CHECK-NEXT:                     });
// CHECK-NEXT:                     *[[SLATE_SLOT_16]] = *[[SLATE_SLOT_0]];
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         *unsafe { (*[[SLATE_SLOT_16]]).offset((0 as i32) as isize) } = 23 as i32;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     __slate_state = 1usize;
// CHECK-NEXT:                     continue '__slate_dispatch;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 3 => {
// CHECK-NEXT:                     *[[SLATE_SLOT_12]] = *[[SLATE_SLOT_11]];
// CHECK-NEXT:                     std::ptr::write([[SLATE_SLOT_8]], *[[SLATE_SLOT_17]]);
// CHECK-NEXT:                     std::ptr::write([[SLATE_SLOT_7]], unsafe {
// CHECK-NEXT:                         (*[[SLATE_SLOT_8]]).offset((1 as i32) as isize)
// CHECK-NEXT:                     });
// CHECK-NEXT:                     *[[SLATE_SLOT_17]] = *[[SLATE_SLOT_7]];
// CHECK-NEXT:                     *[[SLATE_SLOT_11]] = (unsafe { *(*[[SLATE_SLOT_8]]) }) as *mut *mut ();
// CHECK-NEXT:                     *[[SLATE_SLOT_15]] =
// CHECK-NEXT:                         unsafe { *unsafe { (*[[SLATE_SLOT_16]]).offset((2 as i32) as isize) } };
// CHECK-NEXT:                     *[[SLATE_SLOT_14]] =
// CHECK-NEXT:                         unsafe { *unsafe { (*[[SLATE_SLOT_16]]).offset((1 as i32) as isize) } };
// CHECK-NEXT:                     *[[SLATE_SLOT_13]] =
// CHECK-NEXT:                         unsafe { *unsafe { (*[[SLATE_SLOT_16]]).offset((0 as i32) as isize) } };
// CHECK-NEXT:                     std::ptr::write([[SLATE_SLOT_6]], *[[SLATE_SLOT_16]]);
// CHECK-NEXT:                     std::ptr::write([[SLATE_SLOT_5]], unsafe {
// CHECK-NEXT:                         (*[[SLATE_SLOT_6]]).offset(-((1 as i32) as isize))
// CHECK-NEXT:                     });
// CHECK-NEXT:                     *[[SLATE_SLOT_16]] = *[[SLATE_SLOT_5]];
// CHECK-NEXT:                     std::ptr::write(
// CHECK-NEXT:                         [[SLATE_SLOT_4]],
// CHECK-NEXT:                         foo(*[[SLATE_SLOT_15]], *[[SLATE_SLOT_14]], *[[SLATE_SLOT_13]]),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         *unsafe { (*[[SLATE_SLOT_16]]).offset((3 as i32) as isize) } =
// CHECK-NEXT:                             (*[[SLATE_SLOT_4]]).n1;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         *unsafe { (*[[SLATE_SLOT_16]]).offset((2 as i32) as isize) } =
// CHECK-NEXT:                             (*[[SLATE_SLOT_4]]).n2;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         *unsafe { (*[[SLATE_SLOT_16]]).offset((1 as i32) as isize) } =
// CHECK-NEXT:                             (*[[SLATE_SLOT_4]]).n3;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         *unsafe { (*[[SLATE_SLOT_16]]).offset((0 as i32) as isize) } =
// CHECK-NEXT:                             (*[[SLATE_SLOT_4]]).n4;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     match *[[SLATE_SLOT_11]] as usize {
// CHECK-NEXT:                         9 => {
// CHECK-NEXT:                             __slate_state = 3usize;
// CHECK-NEXT:                             continue '__slate_dispatch;
// CHECK-NEXT:                         }
// CHECK-NEXT:                         10 => {
// CHECK-NEXT:                             __slate_state = 2usize;
// CHECK-NEXT:                             continue '__slate_dispatch;
// CHECK-NEXT:                         }
// CHECK-NEXT:                         11 => {
// CHECK-NEXT:                             __slate_state = 1usize;
// CHECK-NEXT:                             continue '__slate_dispatch;
// CHECK-NEXT:                         }
// CHECK-NEXT:                         12 => {
// CHECK-NEXT:                             __slate_state = 0usize;
// CHECK-NEXT:                             continue '__slate_dispatch;
// CHECK-NEXT:                         }
// CHECK-NEXT:                         _ => {
// CHECK-NEXT:                             unreachable!();
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 4 => {
// CHECK-NEXT:                     std::ptr::write([[SLATE_SLOT_10]], *[[SLATE_SLOT_17]]);
// CHECK-NEXT:                     std::ptr::write([[SLATE_SLOT_9]], unsafe {
// CHECK-NEXT:                         (*[[SLATE_SLOT_10]]).offset((1 as i32) as isize)
// CHECK-NEXT:                     });
// CHECK-NEXT:                     *[[SLATE_SLOT_17]] = *[[SLATE_SLOT_9]];
// CHECK-NEXT:                     *[[SLATE_SLOT_11]] = (unsafe { *(*[[SLATE_SLOT_10]]) }) as *mut *mut ();
// CHECK-NEXT:                     match *[[SLATE_SLOT_11]] as usize {
// CHECK-NEXT:                         9 => {
// CHECK-NEXT:                             __slate_state = 3usize;
// CHECK-NEXT:                             continue '__slate_dispatch;
// CHECK-NEXT:                         }
// CHECK-NEXT:                         10 => {
// CHECK-NEXT:                             __slate_state = 2usize;
// CHECK-NEXT:                             continue '__slate_dispatch;
// CHECK-NEXT:                         }
// CHECK-NEXT:                         11 => {
// CHECK-NEXT:                             __slate_state = 1usize;
// CHECK-NEXT:                             continue '__slate_dispatch;
// CHECK-NEXT:                         }
// CHECK-NEXT:                         12 => {
// CHECK-NEXT:                             __slate_state = 0usize;
// CHECK-NEXT:                             continue '__slate_dispatch;
// CHECK-NEXT:                         }
// CHECK-NEXT:                         _ => {
// CHECK-NEXT:                             unreachable!();
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 5 => {
// CHECK-NEXT:                     return unsafe { std::ptr::addr_of_mut!(l.0) as *mut *mut () };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 6 => {
// CHECK-NEXT:                     std::ptr::write([[SLATE_SLOT_17]], u);
// CHECK-NEXT:                     std::ptr::write([[SLATE_SLOT_16]], v);
// CHECK-NEXT:                     if !(u != std::ptr::null_mut::<*mut ()>()) {
// CHECK-NEXT:                         __slate_state = 5usize;
// CHECK-NEXT:                         continue '__slate_dispatch;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         __slate_state = 4usize;
// CHECK-NEXT:                         continue '__slate_dispatch;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 _ => {
// CHECK-NEXT:                     unreachable!();
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut l: __SlateAlign16<[*mut (); 5]> = __SlateAlign16([
// CHECK-NEXT:     11usize as *mut (),
// CHECK-NEXT:     11usize as *mut (),
// CHECK-NEXT:     9usize as *mut (),
// CHECK-NEXT:     10usize as *mut (),
// CHECK-NEXT:     12usize as *mut (),
// CHECK-NEXT: ]);
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut u: *mut *mut () = bar(std::ptr::null_mut::<*mut ()>(), std::ptr::null_mut::<i32>());
// CHECK-NEXT:     let mut t: __SlateAlign16<[*mut (); 2]> = __SlateAlign16([
// CHECK-NEXT:         unsafe { *unsafe { u.offset((2 as i32) as isize) } },
// CHECK-NEXT:         unsafe { *unsafe { u.offset((4 as i32) as isize) } },
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut s: __SlateAlign16<[i32; 6]> = __SlateAlign16([
// CHECK-NEXT:         7 as i32, 8 as i32, 9 as i32, 10 as i32, 11 as i32, 12 as i32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     if bar(t.0.as_mut_ptr() as *mut *mut (), unsafe {
// CHECK-NEXT:         (s.0.as_mut_ptr() as *mut i32).offset((1 as i32) as isize)
// CHECK-NEXT:     }) != std::ptr::null_mut::<*mut ()>()
// CHECK-NEXT:         || (unsafe { *unsafe { (s.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } })
// CHECK-NEXT:             != (4 as i32)
// CHECK-NEXT:         || (unsafe { *unsafe { (s.0.as_mut_ptr() as *mut i32).offset((1 as i32) as isize) } })
// CHECK-NEXT:             != (3 as i32)
// CHECK-NEXT:         || (unsafe { *unsafe { (s.0.as_mut_ptr() as *mut i32).offset((2 as i32) as isize) } })
// CHECK-NEXT:             != (2 as i32)
// CHECK-NEXT:         || (unsafe { *unsafe { (s.0.as_mut_ptr() as *mut i32).offset((3 as i32) as isize) } })
// CHECK-NEXT:             != (1 as i32)
// CHECK-NEXT:         || (unsafe { *unsafe { (s.0.as_mut_ptr() as *mut i32).offset((4 as i32) as isize) } })
// CHECK-NEXT:             != (11 as i32)
// CHECK-NEXT:         || (unsafe { *unsafe { (s.0.as_mut_ptr() as *mut i32).offset((5 as i32) as isize) } })
// CHECK-NEXT:             != (12 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
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
