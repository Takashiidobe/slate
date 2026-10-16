/* { dg-require-effective-target indirect_jumps } */

typedef long unsigned int size_t;
extern void               abort(void);
extern char              *strcpy(char *, const char *);
extern int                strcmp(const char *, const char *);
typedef __builtin_va_list va_list;
static const char         null[] = "(null)";
int                       g(char *s, const char *format, va_list ap) {
  const char        *f;
  const char        *string;
  char               spec;
  static const void *step0_jumps[] = {
      &&do_precision,
      &&do_form_integer,
      &&do_form_string,
  };
  f = format;
  if (*f == '\0')
    goto all_done;
  do {
    spec = (*++f);
    goto *(step0_jumps[2]);

  /* begin switch table. */
  do_precision:
    ++f;
    __builtin_va_arg(ap, int);
    spec = *f;
    goto *(step0_jumps[2]);

  do_form_integer:
    __builtin_va_arg(ap, unsigned long int);
    goto end;

  do_form_string:
    string = __builtin_va_arg(ap, const char *);
    strcpy(s, string);

  /* End of switch table. */
  end:
    ++f;
  } while (*f != '\0');

all_done:
  return 0;
}

void f(char *s, const char *f, ...) {
  va_list ap;
  __builtin_va_start(ap, f);
  g(s, f, ap);
  __builtin_va_end(ap);
}

int main(void) {
  char buf[10];
  f(buf, "%s", "asdf", 0);
  if (strcmp(buf, "asdf"))
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-require-effective-target indirect_jumps }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn strcpy([[V_0:__v[0-9]+]]: *mut i8, [[V_1:__v[0-9]+]]: *const i8) -> *mut i8;
// CHECK-NEXT:     fn strcmp([[V_2:__v[0-9]+]]: *const i8, [[V_3:__v[0-9]+]]: *const i8) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut null: [i8; 7] = [
// CHECK-NEXT:     40 as i8, 110 as i8, 117 as i8, 108 as i8, 108 as i8, 41 as i8, 0 as i8,
// CHECK-NEXT: ];
// CHECK-EMPTY:
// CHECK-NEXT: fn g(mut s: *mut i8, mut format: *const i8, mut ap: core::ffi::VaList<'_>) -> i32 {
// CHECK-NEXT:     let mut [[SLATE_STORAGE_0:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<*const i8> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_0:__slate_slot_[0-9]+]]: *mut *const i8 =
// CHECK-NEXT:         std::ptr::addr_of_mut!([[SLATE_STORAGE_0]]) as *mut *const i8;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_1:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<*const i8> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_1:__slate_slot_[0-9]+]]: *mut *const i8 =
// CHECK-NEXT:         std::ptr::addr_of_mut!([[SLATE_STORAGE_1]]) as *mut *const i8;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_2:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<*const i8> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_2:__slate_slot_[0-9]+]]: *mut *const i8 =
// CHECK-NEXT:         std::ptr::addr_of_mut!([[SLATE_STORAGE_2]]) as *mut *const i8;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_3:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<*const i8> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_3:__slate_slot_[0-9]+]]: *mut *const i8 =
// CHECK-NEXT:         std::ptr::addr_of_mut!([[SLATE_STORAGE_3]]) as *mut *const i8;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_4:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<*const i8> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_4:__slate_slot_[0-9]+]]: *mut *const i8 =
// CHECK-NEXT:         std::ptr::addr_of_mut!([[SLATE_STORAGE_4]]) as *mut *const i8;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_5:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<*const i8> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_5:__slate_slot_[0-9]+]]: *mut *const i8 =
// CHECK-NEXT:         std::ptr::addr_of_mut!([[SLATE_STORAGE_5]]) as *mut *const i8;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_6:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i8> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_6:__slate_slot_[0-9]+]]: *mut i8 = std::ptr::addr_of_mut!([[SLATE_STORAGE_6]]) as *mut i8;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_7:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<*const i8> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_7:__slate_slot_[0-9]+]]: *mut *const i8 =
// CHECK-NEXT:         std::ptr::addr_of_mut!([[SLATE_STORAGE_7]]) as *mut *const i8;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_8:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<*const i8> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_8:__slate_slot_[0-9]+]]: *mut *const i8 =
// CHECK-NEXT:         std::ptr::addr_of_mut!([[SLATE_STORAGE_8]]) as *mut *const i8;
// CHECK-NEXT:     let mut __slate_state: usize = 6usize;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         '__slate_dispatch: loop {
// CHECK-NEXT:             match __slate_state {
// CHECK-NEXT:                 0 => {
// CHECK-NEXT:                     return 0 as i32;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 1 => {
// CHECK-NEXT:                     std::ptr::write([[SLATE_SLOT_1]], *[[SLATE_SLOT_8]]);
// CHECK-NEXT:                     std::ptr::write([[SLATE_SLOT_0]], unsafe {
// CHECK-NEXT:                         (*[[SLATE_SLOT_1]]).offset((1 as i32) as isize)
// CHECK-NEXT:                     });
// CHECK-NEXT:                     *[[SLATE_SLOT_8]] = *[[SLATE_SLOT_0]];
// CHECK-NEXT:                     if ((unsafe { *(*[[SLATE_SLOT_8]]) }) as i32) != (0 as i32) {
// CHECK-NEXT:                         __slate_state = 5usize;
// CHECK-NEXT:                         continue '__slate_dispatch;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         __slate_state = 0usize;
// CHECK-NEXT:                         continue '__slate_dispatch;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 2 => {
// CHECK-NEXT:                     *[[SLATE_SLOT_7]] = unsafe { ap.next_arg::<*const i8>() };
// CHECK-NEXT:                     unsafe { strcpy(s, *[[SLATE_SLOT_7]]) };
// CHECK-NEXT:                     // End of switch table.
// CHECK-NEXT:                     __slate_state = 1usize;
// CHECK-NEXT:                     continue '__slate_dispatch;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 3 => {
// CHECK-NEXT:                     unsafe { ap.next_arg::<u64>() };
// CHECK-NEXT:                     __slate_state = 1usize;
// CHECK-NEXT:                     continue '__slate_dispatch;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 4 => {
// CHECK-NEXT:                     std::ptr::write([[SLATE_SLOT_3]], *[[SLATE_SLOT_8]]);
// CHECK-NEXT:                     std::ptr::write([[SLATE_SLOT_2]], unsafe {
// CHECK-NEXT:                         (*[[SLATE_SLOT_3]]).offset((1 as i32) as isize)
// CHECK-NEXT:                     });
// CHECK-NEXT:                     *[[SLATE_SLOT_8]] = *[[SLATE_SLOT_2]];
// CHECK-NEXT:                     unsafe { ap.next_arg::<i32>() };
// CHECK-NEXT:                     *[[SLATE_SLOT_6]] = unsafe { *(*[[SLATE_SLOT_8]]) };
// CHECK-NEXT:                     match (unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of_mut!(step0_jumps.0) as *mut *const () }
// CHECK-NEXT:                                 .offset((2 as i32) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }) as usize
// CHECK-NEXT:                     {
// CHECK-NEXT:                         8 => {
// CHECK-NEXT:                             __slate_state = 4usize;
// CHECK-NEXT:                             continue '__slate_dispatch;
// CHECK-NEXT:                         }
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
// CHECK-NEXT:                     std::ptr::write([[SLATE_SLOT_5]], *[[SLATE_SLOT_8]]);
// CHECK-NEXT:                     std::ptr::write([[SLATE_SLOT_4]], unsafe {
// CHECK-NEXT:                         (*[[SLATE_SLOT_5]]).offset((1 as i32) as isize)
// CHECK-NEXT:                     });
// CHECK-NEXT:                     *[[SLATE_SLOT_8]] = *[[SLATE_SLOT_4]];
// CHECK-NEXT:                     *[[SLATE_SLOT_6]] = unsafe { *(*[[SLATE_SLOT_4]]) };
// CHECK-NEXT:                     match (unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of_mut!(step0_jumps.0) as *mut *const () }
// CHECK-NEXT:                                 .offset((2 as i32) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }) as usize
// CHECK-NEXT:                     {
// CHECK-NEXT:                         8 => {
// CHECK-NEXT:                             __slate_state = 4usize;
// CHECK-NEXT:                             continue '__slate_dispatch;
// CHECK-NEXT:                         }
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
// CHECK-NEXT:                 6 => {
// CHECK-NEXT:                     *[[SLATE_SLOT_8]] = format;
// CHECK-NEXT:                     if ((unsafe { *(*[[SLATE_SLOT_8]]) }) as i32) == (0 as i32) {
// CHECK-NEXT:                         __slate_state = 0usize;
// CHECK-NEXT:                         continue '__slate_dispatch;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         __slate_state = 5usize;
// CHECK-NEXT:                         continue '__slate_dispatch;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 _ => {
// CHECK-NEXT:                     unreachable!();
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     // begin switch table.
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut step0_jumps: __SlateAlign16<[*const (); 3]> = __SlateAlign16([
// CHECK-NEXT:     (8usize as *mut ()) as *const (),
// CHECK-NEXT:     (9usize as *mut ()) as *const (),
// CHECK-NEXT:     (10usize as *mut ()) as *const (),
// CHECK-NEXT: ]);
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C-unwind" fn f(mut s: *mut i8, mut f: *const i8, mut __va_args: ...) {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     g(s, f, ap.clone());
// CHECK-NEXT:     {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut buf: [i8; 10] = [0 as i8; 10];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         f(
// CHECK-NEXT:             buf.as_mut_ptr() as *mut i8,
// CHECK-NEXT:             c"%s".as_ptr() as *const i8,
// CHECK-NEXT:             c"asdf".as_ptr() as *mut i8,
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     if (unsafe { strcmp(buf.as_mut_ptr() as *const i8, c"asdf".as_ptr() as *const i8) })
// CHECK-NEXT:         != (0 as i32)
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
