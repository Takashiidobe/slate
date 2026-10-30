/* { dg-options "-fopenmp" } */
#include <stdio.h>

int omp_get_thread_num(void);
int omp_get_num_threads(void);
int omp_get_level(void);

struct pair {
  int a;
  int b;
};

int global_hits[8];

static int team_size(int requested) {
  int sizes[8] = {0};
  int team = 0;
#pragma omp parallel num_threads(requested)
  {
    int id = omp_get_thread_num();
    sizes[id] = omp_get_num_threads();
    global_hits[id] += 1;
  }
  for (int i = 0; i < 8; i++)
    if (sizes[i])
      team += 1;
  return team;
}

static int vla_sum(int n) {
  int values[n];
  int total = 0;
#pragma omp parallel num_threads(4)
  {
    int id = omp_get_thread_num();
    if (id < n)
      values[id] = id * n;
  }
  for (int i = 0; i < n; i++)
    total += values[i];
  return total;
}

static int bump(void) {
  static int calls;
#pragma omp parallel num_threads(4)
  if (omp_get_thread_num() == 1)
    calls += 5;
  return calls;
}

int main(void) {
  printf("vla %d %d\n", vla_sum(3), vla_sum(4));
  int first = bump();
  int second = bump();
  printf("static %d %d\n", first, second);

  int reached = 0;
#pragma omp parallel num_threads(4)
  {
    if (omp_get_thread_num() != 3)
      goto out;
    reached = 1;
  out:;
  }
  printf("goto %d\n", reached);

  int sizes[8] = {0};
#pragma omp parallel
  sizes[omp_get_thread_num()] = omp_get_num_threads();
  printf("default %d %d %d %d %d\n", sizes[0], sizes[1], sizes[2], sizes[3],
         sizes[4]);

  printf("teams %d %d %d\n", team_size(1), team_size(2), team_size(3));
  printf("hits %d %d %d %d\n", global_hits[0], global_hits[1], global_hits[2],
         global_hits[3]);

  int serial = 0;
  int off = 0;
#pragma omp parallel if (off) num_threads(4)
  serial = omp_get_num_threads();
  printf("if %d\n", serial);

  int p = 5;
  int written[4] = {0};
#pragma omp parallel private(p) num_threads(4)
  {
    p = omp_get_thread_num() * 10;
    written[omp_get_thread_num()] = p + 1;
  }
  printf("private %d %d %d %d %d\n", p, written[0], written[1], written[2],
         written[3]);

  int f = 7;
  struct pair pr = {1, 2};
  int seen[4] = {0};
#pragma omp parallel firstprivate(f, pr) num_threads(4)
  {
    int id = omp_get_thread_num();
    f += id;
    pr.b += f;
    seen[id] = pr.a * 100 + pr.b;
  }
  printf("firstprivate %d %d %d %d %d %d\n", f, pr.b, seen[0], seen[1],
         seen[2], seen[3]);

  int shared = 0;
  double scale = 2.5;
#pragma omp parallel shared(shared) num_threads(4)
  {
    if (omp_get_thread_num() == 2)
      shared = (int)(scale * 4);
  }
  printf("shared %d\n", shared);

  int outer = 1;
#pragma omp parallel num_threads(2)
  {
    int inner = omp_get_thread_num();
#pragma omp parallel num_threads(2) firstprivate(inner)
    if (omp_get_thread_num() == 0 && inner == 1)
      outer = 40 + omp_get_level();
  }
  printf("nested %d\n", outer);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-fopenmp" }
// CHECK-NEXT: mod __slate_libgomp {
// CHECK-NEXT:     use std::ffi::c_void;
// CHECK-EMPTY:
// CHECK-NEXT:     #[link(name = "gomp")]
// CHECK-NEXT:     unsafe extern "C" {
// CHECK-NEXT:         fn GOMP_parallel(
// CHECK-NEXT:             function: unsafe extern "C" fn(*mut c_void),
// CHECK-NEXT:             data: *mut c_void,
// CHECK-NEXT:             num_threads: u32,
// CHECK-NEXT:             flags: u32,
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-EMPTY:
// CHECK-NEXT:     unsafe extern "C" fn region<F: Fn()>(data: *mut c_void) {
// CHECK-NEXT:         unsafe { (*(data as *const F))() }
// CHECK-NEXT:     }
// CHECK-EMPTY:
// CHECK-NEXT:     pub fn parallel<F: Fn()>(num_threads: u32, body: F) {
// CHECK-NEXT:         unsafe { GOMP_parallel(region::<F>, &raw const body as *mut c_void, num_threads, 0) }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT:     fn omp_get_thread_num() -> i32;
// CHECK-NEXT:     fn omp_get_num_threads() -> i32;
// CHECK-NEXT:     fn omp_get_level() -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct pair {
// CHECK-NEXT:     a: i32,
// CHECK-NEXT:     b: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut global_hits: __SlateAlign16<[i32; 8]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: fn team_size(requested: i32) -> i32 {
// CHECK-NEXT:     let mut sizes: __SlateAlign16<[i32; 8]> = __SlateAlign16({
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: [i32; 8] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]][0] = 0i32;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     });
// CHECK-NEXT:     let mut team: i32 = 0;
// CHECK-NEXT:     let __slate_omp_0 = std::ptr::addr_of_mut!(sizes.0);
// CHECK-NEXT:     __slate_libgomp::parallel(requested as u32, || unsafe {
// CHECK-NEXT:         {
// CHECK-NEXT:             let mut id: i32 = omp_get_thread_num();
// CHECK-NEXT:             (*__slate_omp_0)[id as usize] = omp_get_num_threads();
// CHECK-NEXT:             let mut [[V_0:__v[0-9]+]]: *mut i32 =
// CHECK-NEXT:                 (std::ptr::addr_of_mut!(global_hits.0) as *mut i32).offset(id as isize);
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: i32 = *[[V_0]];
// CHECK-NEXT:             let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + 1;
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *[[V_0]] = [[V_2]];
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     });
// CHECK-NEXT:     for i in 0i32..8i32 {
// CHECK-NEXT:         if sizes.0[i as usize] != 0 {
// CHECK-NEXT:             team += 1;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     team
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn vla_sum(mut n: i32) -> i32 {
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: u64 = (n as i64) as u64;
// CHECK-NEXT:     let mut values: Vec<i32> = {
// CHECK-NEXT:         let mut __slate_vla: Vec<i32> = Vec::with_capacity([[V_3]] as usize);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             __slate_vla.as_mut_ptr().write_bytes(0u8, [[V_3]] as usize);
// CHECK-NEXT:         }
// CHECK-NEXT:         __slate_vla
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     let __slate_omp_1 = std::ptr::addr_of_mut!(n);
// CHECK-NEXT:     let __slate_omp_2 = values.as_mut_ptr() as *mut [i32; 0];
// CHECK-NEXT:     __slate_libgomp::parallel(4u32, || unsafe {
// CHECK-NEXT:         {
// CHECK-NEXT:             let mut id: i32 = omp_get_thread_num();
// CHECK-NEXT:             if id < *__slate_omp_1 {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *(__slate_omp_2 as *mut i32).offset(id as isize) = id * *__slate_omp_1;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     });
// CHECK-NEXT:     let mut i: i32 = 0;
// CHECK-NEXT:     while i < n {
// CHECK-NEXT:         let [[V_2]]: i32 = total;
// CHECK-NEXT:         total = [[V_2]] + unsafe { *(values.as_mut_ptr() as *mut i32).offset(i as isize) };
// CHECK-NEXT:         i += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bump() -> i32 {
// CHECK-NEXT:     __slate_libgomp::parallel(4u32, || unsafe {
// CHECK-NEXT:         if omp_get_thread_num() == 1 {
// CHECK-NEXT:             let mut [[V_3]]: i32 = calls;
// CHECK-NEXT:             let mut [[V_0]]: i32 = [[V_3]] + 5;
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 calls = [[V_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     });
// CHECK-NEXT:     unsafe { calls }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut calls: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("vla {} {}", vla_sum(3i32), vla_sum(4i32));
// CHECK-NEXT:     let first: i32 = bump();
// CHECK-NEXT:     let second: i32 = bump();
// CHECK-NEXT:     println!("static {} {}", first, second);
// CHECK-NEXT:     let mut reached: i32 = 0;
// CHECK-NEXT:     let __slate_omp_0 = std::ptr::addr_of_mut!(reached);
// CHECK-NEXT:     __slate_libgomp::parallel(4u32, || unsafe {
// CHECK-NEXT:         let mut __slate_state: usize = 2usize;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             '__slate_dispatch: loop {
// CHECK-NEXT:                 match __slate_state {
// CHECK-NEXT:                     0 => {
// CHECK-NEXT:                         {}
// CHECK-NEXT:                         break '__slate_dispatch;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     1 => {
// CHECK-NEXT:                         *__slate_omp_0 = 1i32;
// CHECK-NEXT:                         __slate_state = 0usize;
// CHECK-NEXT:                         continue '__slate_dispatch;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     2 => {
// CHECK-NEXT:                         if omp_get_thread_num() != 3 {
// CHECK-NEXT:                             __slate_state = 0usize;
// CHECK-NEXT:                             continue '__slate_dispatch;
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             __slate_state = 1usize;
// CHECK-NEXT:                             continue '__slate_dispatch;
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                     _ => {
// CHECK-NEXT:                         unreachable!();
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     });
// CHECK-NEXT:     println!("goto {}", reached);
// CHECK-NEXT:     let mut sizes: __SlateAlign16<[i32; 8]> = __SlateAlign16({
// CHECK-NEXT:         let mut [[T_0]]: [i32; 8] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]][0] = 0i32;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     });
// CHECK-NEXT:     let __slate_omp_1 = std::ptr::addr_of_mut!(sizes.0);
// CHECK-NEXT:     __slate_libgomp::parallel(0u32, || unsafe {
// CHECK-NEXT:         (*__slate_omp_1)[omp_get_thread_num() as usize] = omp_get_num_threads();
// CHECK-NEXT:     });
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "default {} {} {} {} {}",
// CHECK-NEXT:         sizes.0[0], sizes.0[1], sizes.0[2], sizes.0[3], sizes.0[4]
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "teams {} {} {}",
// CHECK-NEXT:         team_size(1i32),
// CHECK-NEXT:         team_size(2i32),
// CHECK-NEXT:         team_size(3i32)
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "hits {} {} {} {}",
// CHECK-NEXT:         unsafe { global_hits.0[0] },
// CHECK-NEXT:         unsafe { global_hits.0[1] },
// CHECK-NEXT:         unsafe { global_hits.0[2] },
// CHECK-NEXT:         unsafe { global_hits.0[3] }
// CHECK-NEXT:     );
// CHECK-NEXT:     let mut serial: i32 = 0;
// CHECK-NEXT:     let off: i32 = 0;
// CHECK-NEXT:     let __slate_omp_2 = std::ptr::addr_of_mut!(serial);
// CHECK-NEXT:     __slate_libgomp::parallel(if off != 0 { 4u32 } else { 1u32 }, || unsafe {
// CHECK-NEXT:         *__slate_omp_2 = omp_get_num_threads();
// CHECK-NEXT:     });
// CHECK-NEXT:     println!("if {}", serial);
// CHECK-NEXT:     let p: i32 = 5;
// CHECK-NEXT:     let mut written: __SlateAlign16<[i32; 4]> = __SlateAlign16({
// CHECK-NEXT:         let mut [[T_1:__t[0-9]+]]: [i32; 4] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_1]][0] = 0i32;
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     });
// CHECK-NEXT:     let __slate_omp_3 = std::ptr::addr_of_mut!(written.0);
// CHECK-NEXT:     __slate_libgomp::parallel(4u32, || unsafe {
// CHECK-NEXT:         let mut p: i32 = 0i32;
// CHECK-NEXT:         {
// CHECK-NEXT:             p = omp_get_thread_num() * 10;
// CHECK-NEXT:             (*__slate_omp_3)[omp_get_thread_num() as usize] = p + 1;
// CHECK-NEXT:         }
// CHECK-NEXT:     });
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "private {} {} {} {} {}",
// CHECK-NEXT:         p, written.0[0], written.0[1], written.0[2], written.0[3]
// CHECK-NEXT:     );
// CHECK-NEXT:     let f: i32 = 7;
// CHECK-NEXT:     let pr: pair = pair { a: 1, b: 2 };
// CHECK-NEXT:     let mut seen: __SlateAlign16<[i32; 4]> = __SlateAlign16({
// CHECK-NEXT:         let mut [[T_2:__t[0-9]+]]: [i32; 4] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_2]][0] = 0i32;
// CHECK-NEXT:         [[T_2]]
// CHECK-NEXT:     });
// CHECK-NEXT:     let __slate_omp_4 = std::ptr::addr_of_mut!(seen.0);
// CHECK-NEXT:     let __slate_omp_first_5 = std::ptr::addr_of!(f);
// CHECK-NEXT:     let __slate_omp_first_6 = std::ptr::addr_of!(pr);
// CHECK-NEXT:     __slate_libgomp::parallel(4u32, || unsafe {
// CHECK-NEXT:         let mut f: i32 = 0i32;
// CHECK-NEXT:         f = std::ptr::read(__slate_omp_first_5);
// CHECK-NEXT:         let mut pr: pair = std::mem::zeroed();
// CHECK-NEXT:         pr = std::ptr::read(__slate_omp_first_6);
// CHECK-NEXT:         {
// CHECK-NEXT:             let mut id: i32 = omp_get_thread_num();
// CHECK-NEXT:             let mut [[V_4:__v[0-9]+]]: i32 = f;
// CHECK-NEXT:             let mut [[V_5:__v[0-9]+]]: i32 = [[V_4]] + id;
// CHECK-NEXT:             f = [[V_5]];
// CHECK-NEXT:             let mut [[V_6:__v[0-9]+]]: i32 = pr.b;
// CHECK-NEXT:             let mut [[V_7:__v[0-9]+]]: i32 = [[V_6]] + f;
// CHECK-NEXT:             pr.b = [[V_7]];
// CHECK-NEXT:             (*__slate_omp_4)[id as usize] = pr.a * 100 + pr.b;
// CHECK-NEXT:         }
// CHECK-NEXT:     });
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "firstprivate {} {} {} {} {} {}",
// CHECK-NEXT:         f, pr.b, seen.0[0], seen.0[1], seen.0[2], seen.0[3]
// CHECK-NEXT:     );
// CHECK-NEXT:     let mut shared: i32 = 0;
// CHECK-NEXT:     let mut scale: f64 = 2.5f64;
// CHECK-NEXT:     let __slate_omp_11 = std::ptr::addr_of_mut!(shared);
// CHECK-NEXT:     let __slate_omp_12 = std::ptr::addr_of_mut!(scale);
// CHECK-NEXT:     __slate_libgomp::parallel(4u32, || unsafe {
// CHECK-NEXT:         {
// CHECK-NEXT:             if omp_get_thread_num() == 2 {
// CHECK-NEXT:                 *__slate_omp_11 = (*__slate_omp_12 * (4i32 as f64)) as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     });
// CHECK-NEXT:     println!("shared {}", shared);
// CHECK-NEXT:     let mut outer: i32 = 1;
// CHECK-NEXT:     let __slate_omp_13 = std::ptr::addr_of_mut!(outer);
// CHECK-NEXT:     __slate_libgomp::parallel(2u32, || unsafe {
// CHECK-NEXT:         {
// CHECK-NEXT:             let mut inner: i32 = omp_get_thread_num();
// CHECK-NEXT:             {
// CHECK-NEXT:                 let __slate_omp_13 = std::ptr::addr_of_mut!(*__slate_omp_13);
// CHECK-NEXT:                 let __slate_omp_first_14 = std::ptr::addr_of!(inner);
// CHECK-NEXT:                 __slate_libgomp::parallel(2u32, || {
// CHECK-NEXT:                     let mut inner: i32 = 0i32;
// CHECK-NEXT:                     inner = std::ptr::read(__slate_omp_first_14);
// CHECK-NEXT:                     if omp_get_thread_num() == 0 && inner == 1 {
// CHECK-NEXT:                         *__slate_omp_13 = 40 + omp_get_level();
// CHECK-NEXT:                     }
// CHECK-NEXT:                 });
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     });
// CHECK-NEXT:     println!("nested {}", outer);
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
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
