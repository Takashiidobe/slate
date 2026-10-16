extern void abort(void);

union _D_rep {
  unsigned short rep[4];
  double         val;
};

int add(double *key, double *table) {
  unsigned i            = 0;
  double  *deletedEntry = 0;
  while (1) {
    double *entry = table + i;

    if (*entry == *key)
      break;

    union _D_rep _D_inf = {{0, 0, 0, 0x7ff0}};
    if (*entry != _D_inf.val)
      abort();

    union _D_rep _D_inf2 = {{0, 0, 0, 0x7ff0}};
    if (!_D_inf2.val)
      deletedEntry = entry;

    i++;
  }
  if (deletedEntry)
    *deletedEntry = 0.0;
  return 0;
}

int main() {
  union _D_rep infinit  = {{0, 0, 0, 0x7ff0}};
  double       table[2] = {infinit.val, 23};
  double       key      = 23;
  int          ret      = add(&key, table);
  return ret;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union _D_rep {
// CHECK-NEXT:     rep: [u16; 4],
// CHECK-NEXT:     val: f64,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn add(mut key: &f64, mut table: *mut f64) -> i32 {
// CHECK-NEXT:     let mut i: u32 = (0 as i32) as u32;
// CHECK-NEXT:     let mut deletedEntry: *mut f64 = std::ptr::null_mut::<f64>();
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (1 as i32) != (0 as i32) {
// CHECK-NEXT:         let mut entry: *mut f64 = unsafe { table.offset(i as isize) };
// CHECK-NEXT:         if (unsafe { *entry }) == unsafe { *(key as *const f64) } {
// CHECK-NEXT:             break '[[SLATE_BREAK_0]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut _D_inf: _D_rep = {
// CHECK-NEXT:             let mut [[T_0:__t[0-9]+]]: _D_rep = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_0]].rep = [
// CHECK-NEXT:                 ((0 as i32) as i16) as u16,
// CHECK-NEXT:                 ((0 as i32) as i16) as u16,
// CHECK-NEXT:                 ((0 as i32) as i16) as u16,
// CHECK-NEXT:                 ((32752 as i32) as i16) as u16,
// CHECK-NEXT:             ];
// CHECK-NEXT:             [[T_0]]
// CHECK-NEXT:         };
// CHECK-NEXT:         if (unsafe { *entry }) != unsafe { _D_inf.val } {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut _D_inf2: _D_rep = {
// CHECK-NEXT:             let mut [[T_1:__t[0-9]+]]: _D_rep = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_1]].rep = [
// CHECK-NEXT:                 ((0 as i32) as i16) as u16,
// CHECK-NEXT:                 ((0 as i32) as i16) as u16,
// CHECK-NEXT:                 ((0 as i32) as i16) as u16,
// CHECK-NEXT:                 ((32752 as i32) as i16) as u16,
// CHECK-NEXT:             ];
// CHECK-NEXT:             [[T_1]]
// CHECK-NEXT:         };
// CHECK-NEXT:         if !((unsafe { _D_inf2.val }) != 0.0f64) {
// CHECK-NEXT:             deletedEntry = entry;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: u32 = i;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: u32 = [[V_0]].wrapping_add((1 as i32) as u32);
// CHECK-NEXT:         i = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if deletedEntry != std::ptr::null_mut::<f64>() {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *deletedEntry = 0.0f64;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut infinit: _D_rep = {
// CHECK-NEXT:         let mut [[T_0]]: _D_rep = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].rep = [
// CHECK-NEXT:             ((0 as i32) as i16) as u16,
// CHECK-NEXT:             ((0 as i32) as i16) as u16,
// CHECK-NEXT:             ((0 as i32) as i16) as u16,
// CHECK-NEXT:             ((32752 as i32) as i16) as u16,
// CHECK-NEXT:         ];
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut table: __SlateAlign16<[f64; 2]> = __SlateAlign16([
// CHECK-NEXT:         unsafe { infinit.val },
// CHECK-NEXT:         (23 as i32) as f64,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut key: f64 = (23 as i32) as f64;
// CHECK-NEXT:     let mut ret: i32 = add(
// CHECK-NEXT:         unsafe { &(*std::ptr::addr_of_mut!(key)) },
// CHECK-NEXT:         table.0.as_mut_ptr() as *mut f64,
// CHECK-NEXT:     );
// CHECK-NEXT:     ret
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
