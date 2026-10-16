extern void abort(void);

typedef signed short   int16_t;
typedef unsigned short uint16_t;

int16_t logadd(int16_t *a, int16_t *b);
void    ba_compute_psd(int16_t start);

int16_t masktab[6] = {1, 2, 3, 4, 5};
int16_t psd[6]     = {50, 40, 30, 20, 10};
int16_t bndpsd[6]  = {1, 2, 3, 4, 5};

void ba_compute_psd(int16_t start) {
  int     i, j, k;
  int16_t lastbin = 4;

  j = start;
  k = masktab[start];

  bndpsd[k] = psd[j];
  j++;

  for (i = j; i < lastbin; i++) {
    bndpsd[k] = logadd(&bndpsd[k], &psd[j]);
    j++;
  }
}

int16_t logadd(int16_t *a, int16_t *b) { return *a + *b; }

int main(void) {
  int i;

  ba_compute_psd(0);

  if (bndpsd[1] != 140)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: static mut masktab: [i16; 6] = {
// CHECK-NEXT:     let mut [[T_0:__t[0-9]+]]: [i16; 6] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]][0] = (1 as i32) as i16;
// CHECK-NEXT:     [[T_0]][1] = (2 as i32) as i16;
// CHECK-NEXT:     [[T_0]][2] = (3 as i32) as i16;
// CHECK-NEXT:     [[T_0]][3] = (4 as i32) as i16;
// CHECK-NEXT:     [[T_0]][4] = (5 as i32) as i16;
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: static mut psd: [i16; 6] = {
// CHECK-NEXT:     let mut [[T_0]]: [i16; 6] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]][0] = (50 as i32) as i16;
// CHECK-NEXT:     [[T_0]][1] = (40 as i32) as i16;
// CHECK-NEXT:     [[T_0]][2] = (30 as i32) as i16;
// CHECK-NEXT:     [[T_0]][3] = (20 as i32) as i16;
// CHECK-NEXT:     [[T_0]][4] = (10 as i32) as i16;
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: static mut bndpsd: [i16; 6] = {
// CHECK-NEXT:     let mut [[T_0]]: [i16; 6] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]][0] = (1 as i32) as i16;
// CHECK-NEXT:     [[T_0]][1] = (2 as i32) as i16;
// CHECK-NEXT:     [[T_0]][2] = (3 as i32) as i16;
// CHECK-NEXT:     [[T_0]][3] = (4 as i32) as i16;
// CHECK-NEXT:     [[T_0]][4] = (5 as i32) as i16;
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: fn ba_compute_psd(mut start: i16) {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut j: i32 = start as i32;
// CHECK-NEXT:     let mut k: i32 = (unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(masktab) as *mut i16 }
// CHECK-NEXT:                 .offset((start as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     let mut lastbin: i16 = (4 as i32) as i16;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(bndpsd) as *mut i16 }.offset(k as isize)
// CHECK-NEXT:         } = unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(psd) as *mut i16 }.offset(j as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: i32 = j;
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:     j = [[V_1]];
// CHECK-NEXT:     i = j;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (lastbin as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(bndpsd) as *mut i16 }.offset(k as isize)
// CHECK-NEXT:             } = logadd(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(bndpsd) as *mut i16 }
// CHECK-NEXT:                         .offset(k as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(psd) as *mut i16 }.offset(j as isize)
// CHECK-NEXT:                 },
// CHECK-NEXT:             );
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = j;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:         j = [[V_3]];
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: i32 = [[V_4]] + (1 as i32);
// CHECK-NEXT:         i = [[V_5]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn logadd(mut a: *mut i16, mut b: *mut i16) -> i16 {
// CHECK-NEXT:     (((unsafe { *a }) as i32) + ((unsafe { *b }) as i32)) as i16
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     ba_compute_psd((0 as i32) as i16);
// CHECK-NEXT:     if ((unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(bndpsd) as *mut i16 }
// CHECK-NEXT:                 .offset((1 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     }) as i32) != (140 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
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
