#include <stdarg.h>
#include <stdio.h>

static int ignore_rest(int first, ...) { return first * 3; }

static double promoted(int count, ...) {
  va_list ap;
  va_start(ap, count);
  double total = 0;
  for (int i = 0; i < count; i++) {
    total += va_arg(ap, double);
  }
  int letter = va_arg(ap, int);
  int small = va_arg(ap, int);
  va_end(ap);
  return total + letter + small;
}

static const char *pick(int index, ...) {
  va_list ap;
  va_start(ap, index);
  const char *chosen = 0;
  for (int i = 0; i <= index; i++) {
    chosen = va_arg(ap, const char *);
  }
  va_end(ap);
  return chosen;
}

static int next_pair(va_list *ap) {
  int left = va_arg(*ap, int);
  int right = va_arg(*ap, int);
  return left * right;
}

static int dot(int pairs, ...) {
  va_list ap;
  va_start(ap, pairs);
  int total = 0;
  for (int i = 0; i < pairs; i++) {
    total += next_pair(&ap);
  }
  va_end(ap);
  return total;
}

static long sum_longs(int count, ...) {
  va_list ap;
  va_start(ap, count);
  long total = 0;
  while (count-- > 0) {
    total += va_arg(ap, long);
  }
  va_end(ap);
  return total;
}

static long chain(int seed, ...) {
  va_list ap;
  va_start(ap, seed);
  long extra = va_arg(ap, long);
  unsigned long mask = va_arg(ap, unsigned long);
  va_end(ap);
  return sum_longs(3, (long)seed, extra, (long)(mask & 0xff)) +
         ignore_rest(seed, 1, 2.0, "three");
}

static void *same_pointer(void *first, ...) {
  va_list ap;
  va_start(ap, first);
  void *second = va_arg(ap, void *);
  va_end(ap);
  return second == first ? first : second;
}

int main(void) {
  char c = 'A';
  short s = -12;
  float f = 1.25f;
  int value = 9;
  printf("%d\n", ignore_rest(14));
  printf("%.3f\n", promoted(2, f, 2.5, c, s));
  printf("%s %s\n", pick(0, "zero", "one", "two"), pick(2, "zero", "one", "two"));
  printf("%d\n", dot(3, 1, 2, 3, 4, 5, 6));
  printf("%ld\n", chain(5, 1000000000000L, 0x1234UL));
  printf("%d\n", *(int *)same_pointer(&value, &value));
  return dot(1, 6, 7) % 40;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C-unwind" fn ignore_rest(first: i32, __va_args: ...) -> i32 {
// CHECK-NEXT:     first * 3
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C-unwind" fn promoted(count: i32, mut __va_args: ...) -> f64 {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'static> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             core::ffi::VaList<'_>,
// CHECK-NEXT:             core::ffi::VaList<'static>,
// CHECK-NEXT:         >(__va_args.clone())
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut total: f64 = (0 as i32) as f64;
// CHECK-NEXT:     for i in 0..count {
// CHECK-NEXT:         let [[V_0:__v[0-9]+]]: f64 = unsafe { ap.next_arg::<f64>() };
// CHECK-NEXT:         total += [[V_0]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let letter: i32 = unsafe { ap.next_arg::<i32>() };
// CHECK-NEXT:     let small: i32 = unsafe { ap.next_arg::<i32>() };
// CHECK-NEXT:     {}
// CHECK-NEXT:     total + (letter as f64) + (small as f64)
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C-unwind" fn pick(index: i32, mut __va_args: ...) -> *const i8 {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'static> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             core::ffi::VaList<'_>,
// CHECK-NEXT:             core::ffi::VaList<'static>,
// CHECK-NEXT:         >(__va_args.clone())
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut chosen: *const i8 = std::ptr::null::<i8>();
// CHECK-NEXT:     let mut i: i32 = 0;
// CHECK-NEXT:     while i <= index {
// CHECK-NEXT:         chosen = unsafe { ap.next_arg::<*const i8>() };
// CHECK-NEXT:         i += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     chosen
// CHECK-NEXT: }
// CHECK-NEXT: fn next_pair(ap: &mut core::ffi::VaList<'static>) -> i32 {
// CHECK-NEXT:     let left: i32 = unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(* (ap as * mut core::ffi::VaList <'static >)))
// CHECK-NEXT:             .next_arg::<i32>()
// CHECK-NEXT:     };
// CHECK-NEXT:     let right: i32 = unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(* (ap as * mut core::ffi::VaList <'static >)))
// CHECK-NEXT:             .next_arg::<i32>()
// CHECK-NEXT:     };
// CHECK-NEXT:     left * right
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C-unwind" fn dot(pairs: i32, mut __va_args: ...) -> i32 {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'static> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             core::ffi::VaList<'_>,
// CHECK-NEXT:             core::ffi::VaList<'static>,
// CHECK-NEXT:         >(__va_args.clone())
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     for i in 0..pairs {
// CHECK-NEXT:         let [[V_1:__v[0-9]+]]: i32 = next_pair(unsafe { &mut (*std::ptr::addr_of_mut!(ap)) });
// CHECK-NEXT:         total += [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C-unwind" fn sum_longs(mut count: i32, mut __va_args: ...) -> i64 {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'static> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             core::ffi::VaList<'_>,
// CHECK-NEXT:             core::ffi::VaList<'static>,
// CHECK-NEXT:         >(__va_args.clone())
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut total: i64 = 0;
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let [[V_2:__v[0-9]+]]: i32 = count;
// CHECK-NEXT:         count = [[V_2]] - 1;
// CHECK-NEXT:         if !([[V_2]] > 0) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:         let [[V_3:__v[0-9]+]]: i64 = unsafe { ap.next_arg::<i64>() };
// CHECK-NEXT:         total += [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C-unwind" fn chain(seed: i32, mut __va_args: ...) -> i64 {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'static> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             core::ffi::VaList<'_>,
// CHECK-NEXT:             core::ffi::VaList<'static>,
// CHECK-NEXT:         >(__va_args.clone())
// CHECK-NEXT:     };
// CHECK-NEXT:     let extra: i64 = unsafe { ap.next_arg::<i64>() };
// CHECK-NEXT:     let mask: u64 = unsafe { ap.next_arg::<u64>() };
// CHECK-NEXT:     {}
// CHECK-NEXT:     (unsafe { sum_longs(3 as i32, seed as i64, extra, (mask & 255) as i64) })
// CHECK-NEXT:         + ((unsafe { ignore_rest(seed, 1 as i32, 2.0f64, c"three".as_ptr() as *mut i8) })
// CHECK-NEXT:             as i64)
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C-unwind" fn same_pointer(first: *mut (), mut __va_args: ...) -> *mut () {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'static> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             core::ffi::VaList<'_>,
// CHECK-NEXT:             core::ffi::VaList<'static>,
// CHECK-NEXT:         >(__va_args.clone())
// CHECK-NEXT:     };
// CHECK-NEXT:     let second: *mut () = unsafe { ap.next_arg::<*mut ()>() };
// CHECK-NEXT:     {}
// CHECK-NEXT:     if second == first { first } else { second }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let c: i8 = 65;
// CHECK-NEXT:     let s: i16 = -12;
// CHECK-NEXT:     let f: f32 = 1.25f32;
// CHECK-NEXT:     let mut value: i32 = 9;
// CHECK-NEXT:     println!("{}", unsafe { ignore_rest(14 as i32) });
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%.3f\n".as_ptr(),
// CHECK-NEXT:             unsafe { promoted(2 as i32, f as f64, 2.5f64, c as i32, s as i32) },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%s %s\n".as_ptr(),
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 pick(
// CHECK-NEXT:                     0 as i32,
// CHECK-NEXT:                     c"zero".as_ptr() as *mut i8,
// CHECK-NEXT:                     c"one".as_ptr() as *mut i8,
// CHECK-NEXT:                     c"two".as_ptr() as *mut i8,
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 pick(
// CHECK-NEXT:                     2 as i32,
// CHECK-NEXT:                     c"zero".as_ptr() as *mut i8,
// CHECK-NEXT:                     c"one".as_ptr() as *mut i8,
// CHECK-NEXT:                     c"two".as_ptr() as *mut i8,
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}", unsafe { dot(3 as i32, 1 as i32, 2 as i32, 3 as i32, 4 as i32, 5 as i32, 6
// CHECK-NEXT:         as i32) }
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%ld\n".as_ptr(),
// CHECK-NEXT:             unsafe { chain(5 as i32, 1000000000000 as i64, 4660 as u64) },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}", unsafe { * ((unsafe { same_pointer(std::ptr::addr_of_mut!(value) as * mut
// CHECK-NEXT:         (), std::ptr::addr_of_mut!(value)) }) as * mut i32) }
// CHECK-NEXT:     );
// CHECK-NEXT:     (unsafe { dot(1 as i32, 6 as i32, 7 as i32) }) % 40
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
