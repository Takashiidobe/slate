typedef struct descriptor_dimension {
  int stride;
  int lbound;
  int ubound;
} descriptor_dimension;
typedef struct {
  int                 *data;
  int                  dtype;
  descriptor_dimension dim[7];
} gfc_array_i4;

void msum_i4(gfc_array_i4 *const retarray, gfc_array_i4 *const array,
             const int *const pdim) {
  int        count[7];
  int        extent[7];
  int       *dest;
  const int *base;
  int        dim;
  int        n;
  int        len;

  dim = (*pdim) - 1;
  len = array->dim[dim].ubound + 1 - array->dim[dim].lbound;

  for (n = 0; n < dim; n++) {
    extent[n] = array->dim[n].ubound + 1 - array->dim[n].lbound;
    count[n]  = 0;
  }

  dest = retarray->data;
  base = array->data;

  do {
    int result = 0;

    for (n = 0; n < len; n++, base++)
      result += *base;
    *dest = result;

    count[0]++;
    dest += 1;
  } while (count[0] != extent[0]);
}

int main() {
  int          rdata[3];
  int          adata[9];
  gfc_array_i4 retarray = {rdata, 265, {{1, 1, 3}}};
  gfc_array_i4 array    = {adata, 266, {{1, 1, 3}, {3, 1, 3}}};
  int          dim      = 2;
  msum_i4(&retarray, &array, &dim);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct descriptor_dimension {
// CHECK-NEXT:     stride: i32,
// CHECK-NEXT:     lbound: i32,
// CHECK-NEXT:     ubound: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     data: *mut i32,
// CHECK-NEXT:     dtype: i32,
// CHECK-NEXT:     dim: [descriptor_dimension; 7],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn msum_i4(
// CHECK-NEXT:     mut retarray: &[[SLATERECORD_0]],
// CHECK-NEXT:     mut array: &mut [[SLATERECORD_0]],
// CHECK-NEXT:     mut pdim: &i32,
// CHECK-NEXT: ) {
// CHECK-NEXT:     let mut count: __SlateAlign16<[i32; 7]> = __SlateAlign16([0 as i32; 7]);
// CHECK-NEXT:     let mut extent: __SlateAlign16<[i32; 7]> = __SlateAlign16([0 as i32; 7]);
// CHECK-NEXT:     let mut dest: *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut base: *const i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut dim: i32 = (unsafe { *(pdim as *const i32) }) - (1 as i32);
// CHECK-NEXT:     let mut n: i32 = 0 as i32;
// CHECK-NEXT:     let mut len: i32 = (unsafe {
// CHECK-NEXT:         (*unsafe {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*(array as *mut [[SLATERECORD_0]])).dim.as_mut_ptr()
// CHECK-NEXT:                     as *mut descriptor_dimension
// CHECK-NEXT:             }
// CHECK-NEXT:                 .offset(dim as isize)
// CHECK-NEXT:         })
// CHECK-NEXT:             .ubound
// CHECK-NEXT:     }) + (1 as i32)
// CHECK-NEXT:         - unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*(array as *mut [[SLATERECORD_0]])).dim.as_mut_ptr()
// CHECK-NEXT:                         as *mut descriptor_dimension
// CHECK-NEXT:                 }
// CHECK-NEXT:                     .offset(dim as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:                 .lbound
// CHECK-NEXT:         };
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while n < dim {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (extent.0.as_mut_ptr() as *mut i32).offset(n as isize) } = (unsafe {
// CHECK-NEXT:                 (*unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*(array as *mut [[SLATERECORD_0]])).dim.as_mut_ptr()
// CHECK-NEXT:                             as *mut descriptor_dimension
// CHECK-NEXT:                     }
// CHECK-NEXT:                         .offset(n as isize)
// CHECK-NEXT:                 })
// CHECK-NEXT:                     .ubound
// CHECK-NEXT:             }) + (1 as i32)
// CHECK-NEXT:                 - unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             (*(array as *mut [[SLATERECORD_0]])).dim.as_mut_ptr()
// CHECK-NEXT:                                 as *mut descriptor_dimension
// CHECK-NEXT:                         }
// CHECK-NEXT:                             .offset(n as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                         .lbound
// CHECK-NEXT:                 };
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (count.0.as_mut_ptr() as *mut i32).offset(n as isize) } = 0 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = n;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         n = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     dest = unsafe { (*(retarray as *const [[SLATERECORD_0]])).data };
// CHECK-NEXT:     base = (unsafe { (*(array as *mut [[SLATERECORD_0]])).data }) as *const i32;
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut result: i32 = 0 as i32;
// CHECK-NEXT:         n = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while n < len {
// CHECK-NEXT:             let mut [[V_2:__v[0-9]+]]: i32 = result;
// CHECK-NEXT:             let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + unsafe { *base };
// CHECK-NEXT:             result = [[V_3]];
// CHECK-NEXT:             let mut [[V_4:__v[0-9]+]]: i32 = n;
// CHECK-NEXT:             let mut [[V_5:__v[0-9]+]]: i32 = [[V_4]] + (1 as i32);
// CHECK-NEXT:             n = [[V_5]];
// CHECK-NEXT:             let mut [[V_6:__v[0-9]+]]: *const i32 = base;
// CHECK-NEXT:             let mut [[V_7:__v[0-9]+]]: *const i32 = unsafe { [[V_6]].offset((1 as i32) as isize) };
// CHECK-NEXT:             base = [[V_7]];
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *dest = result;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_8:__v[0-9]+]]: *mut i32 = unsafe {
// CHECK-NEXT:             (count.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize)
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_9:__v[0-9]+]]: i32 = unsafe { *[[V_8]] };
// CHECK-NEXT:         let mut [[V_10:__v[0-9]+]]: i32 = [[V_9]] + (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_8]] = [[V_10]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_11:__v[0-9]+]]: *mut i32 = dest;
// CHECK-NEXT:         let mut [[V_12:__v[0-9]+]]: *mut i32 = unsafe { [[V_11]].offset((1 as i32) as isize) };
// CHECK-NEXT:         dest = [[V_12]];
// CHECK-NEXT:         if !((unsafe {
// CHECK-NEXT:             *unsafe { (count.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) }
// CHECK-NEXT:         })
// CHECK-NEXT:             != unsafe {
// CHECK-NEXT:                 *unsafe {
// CHECK-NEXT:                     (extent.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:             })
// CHECK-NEXT:         {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut rdata: [i32; 3] = [0 as i32; 3];
// CHECK-NEXT:     let mut adata: __SlateAlign16<[i32; 9]> = __SlateAlign16([0 as i32; 9]);
// CHECK-NEXT:     let mut retarray: [[SLATERECORD_0]] = [[SLATERECORD_0]] {
// CHECK-NEXT:         data: rdata.as_mut_ptr() as *mut i32,
// CHECK-NEXT:         dtype: 265 as i32,
// CHECK-NEXT:         dim: {
// CHECK-NEXT:             let mut [[T_0:__t[0-9]+]]: [descriptor_dimension; 7] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_0]][0] = descriptor_dimension {
// CHECK-NEXT:                 stride: 1 as i32,
// CHECK-NEXT:                 lbound: 1 as i32,
// CHECK-NEXT:                 ubound: 3 as i32,
// CHECK-NEXT:             };
// CHECK-NEXT:             [[T_0]]
// CHECK-NEXT:         },
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut array: [[SLATERECORD_0]] = [[SLATERECORD_0]] {
// CHECK-NEXT:         data: adata.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:         dtype: 266 as i32,
// CHECK-NEXT:         dim: {
// CHECK-NEXT:             let mut [[T_1:__t[0-9]+]]: [descriptor_dimension; 7] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_1]][0] = descriptor_dimension {
// CHECK-NEXT:                 stride: 1 as i32,
// CHECK-NEXT:                 lbound: 1 as i32,
// CHECK-NEXT:                 ubound: 3 as i32,
// CHECK-NEXT:             };
// CHECK-NEXT:             [[T_1]][1] = descriptor_dimension {
// CHECK-NEXT:                 stride: 3 as i32,
// CHECK-NEXT:                 lbound: 1 as i32,
// CHECK-NEXT:                 ubound: 3 as i32,
// CHECK-NEXT:             };
// CHECK-NEXT:             [[T_1]]
// CHECK-NEXT:         },
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut dim: i32 = 2 as i32;
// CHECK-NEXT:     msum_i4(
// CHECK-NEXT:         unsafe { &(*std::ptr::addr_of_mut!(retarray)) },
// CHECK-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(array)) },
// CHECK-NEXT:         unsafe { &(*(std::ptr::addr_of_mut!(dim) as *const i32)) },
// CHECK-NEXT:     );
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
