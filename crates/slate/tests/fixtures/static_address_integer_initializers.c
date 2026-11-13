#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

typedef int (*op)(int, int);

static int add(int a, int b) { return a + b; }
static int sub(int a, int b) { return a - b; }

static int data[4] = {1, 2, 3, 4};

static size_t kernels[] = {(size_t)add, (size_t)sub, 0};
size_t exported_kernels[2] = {(size_t)sub, (size_t)&add};
static _Alignas(32) uintptr_t aligned[2] = {(uintptr_t)add, 5};
static uintptr_t scalar = (uintptr_t)&add;
static uintptr_t third = (uintptr_t)&data[2];
static uintptr_t past_first = (uintptr_t)data + sizeof(int);
static intptr_t before_last = (intptr_t)&data[3] - (intptr_t)sizeof(int);
static const uintptr_t constant_handle = (uintptr_t)sub;

struct entry {
  int tag;
  uintptr_t fn;
  long bias;
};

static struct entry entries[2] = {{7, (uintptr_t)sub, 3}, {.fn = (uintptr_t)add, .tag = 1}};

struct outer {
  char label;
  struct entry inner;
  uintptr_t slots[2];
};

static struct outer nested = {'n', {4, (uintptr_t)add, 9}, {(uintptr_t)&data[1], 0}};

int main(void) {
  int sum = 0;
  for (int i = 0; kernels[i]; i++)
    sum += ((op)kernels[i])(10, i + 1);
  sum += ((op)exported_kernels[0])(9, 4) + ((op)exported_kernels[1])(9, 4);
  sum += ((op)aligned[0])(1, (int)aligned[1]) + (int)(((uintptr_t)aligned & 31) == 0);
  sum += ((op)scalar)(1, 2);
  sum += *(int *)third + *(int *)past_first + *(int *)before_last;
  sum += ((op)constant_handle)(20, 6);
  for (int i = 0; i < 2; i++)
    sum += ((op)entries[i].fn)(entries[i].tag, (int)entries[i].bias);
  sum += nested.label + ((op)nested.inner.fn)(nested.inner.tag, (int)nested.inner.bias);
  sum += *(int *)nested.slots[0] + (int)nested.slots[1];
  size_t *cursor = kernels;
  sum += ((op)cursor[1])(5, 1);
  kernels[2] = (size_t)add;
  sum += kernels[2] == (size_t)add;
  kernels[2] = 0;
  data[2] = 30;
  sum += *(int *)third;
  printf("%d\n", sum);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.add")]
// CHECK-NEXT: extern "C-unwind" fn add(a: i32, b: i32) -> i32 {
// CHECK-NEXT:     a + b
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.sub")]
// CHECK-NEXT: extern "C-unwind" fn sub(a: i32, b: i32) -> i32 {
// CHECK-NEXT:     a - b
// CHECK-NEXT: }
// CHECK-NEXT: type __SlateImage_kernels = __SlateAlign16<[u64; 3]>;
// CHECK-NEXT: type __SlateImage_exported_kernels = __SlateAlign16<[u64; 2]>;
// CHECK-NEXT: type __SlateImage_aligned = __SlateAlign32<[u64; 2]>;
// CHECK-NEXT: type __SlateImage_scalar = u64;
// CHECK-NEXT: type __SlateImage_third = u64;
// CHECK-NEXT: type __SlateImage_past_first = u64;
// CHECK-NEXT: type __SlateImage_before_last = i64;
// CHECK-NEXT: type __SlateImage_constant_handle = u64;
// CHECK-NEXT: type __SlateImage_entries = __SlateAlign16<[entry; 2]>;
// CHECK-NEXT: type __SlateImage_nested = outer;
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: #[repr(C, align(32))]
// CHECK-NEXT: struct __SlateAlign32<T>(T);
// CHECK-NEXT: static mut data: __SlateAlign16<[i32; 4]> = __SlateAlign16([1i32, 2i32, 3i32, 4i32]);
// CHECK-NEXT: static mut kernels: std::mem::MaybeUninit<__SlateImage_kernels> = unsafe {
// CHECK-NEXT:     let mut [[T_0:__t[0-9]+]]: std::mem::MaybeUninit<__SlateImage_kernels> = std::mem::MaybeUninit::zeroed();
// CHECK-NEXT:     let [[T_1:__t[0-9]+]] = [[T_0]].as_mut_ptr();
// CHECK-NEXT:     (std::ptr::addr_of_mut!((* [[T_1]]).0[0]) as *mut *const ())
// CHECK-NEXT:         .write(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn(i32, i32) -> i32>,
// CHECK-NEXT:                 *const (),
// CHECK-NEXT:             >(Some(add))
// CHECK-NEXT:         });
// CHECK-NEXT:     (std::ptr::addr_of_mut!((* [[T_1]]).0[1]) as *mut *const ())
// CHECK-NEXT:         .write(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn(i32, i32) -> i32>,
// CHECK-NEXT:                 *const (),
// CHECK-NEXT:             >(Some(sub))
// CHECK-NEXT:         });
// CHECK-NEXT:     (*[[T_1]]).0[2] = 0u64;
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: static mut exported_kernels: std::mem::MaybeUninit<__SlateImage_exported_kernels> = unsafe {
// CHECK-NEXT:     let mut [[T_0]]: std::mem::MaybeUninit<__SlateImage_exported_kernels> = std::mem::MaybeUninit::zeroed();
// CHECK-NEXT:     let [[T_1]] = [[T_0]].as_mut_ptr();
// CHECK-NEXT:     (std::ptr::addr_of_mut!((* [[T_1]]).0[0]) as *mut *const ())
// CHECK-NEXT:         .write(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn(i32, i32) -> i32>,
// CHECK-NEXT:                 *const (),
// CHECK-NEXT:             >(Some(sub))
// CHECK-NEXT:         });
// CHECK-NEXT:     (std::ptr::addr_of_mut!((* [[T_1]]).0[1]) as *mut *const ())
// CHECK-NEXT:         .write(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn(i32, i32) -> i32>,
// CHECK-NEXT:                 *const (),
// CHECK-NEXT:             >(Some(add))
// CHECK-NEXT:         });
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: static mut aligned: std::mem::MaybeUninit<__SlateImage_aligned> = unsafe {
// CHECK-NEXT:     let mut [[T_0]]: std::mem::MaybeUninit<__SlateImage_aligned> = std::mem::MaybeUninit::zeroed();
// CHECK-NEXT:     let [[T_1]] = [[T_0]].as_mut_ptr();
// CHECK-NEXT:     (std::ptr::addr_of_mut!((* [[T_1]]).0[0]) as *mut *const ())
// CHECK-NEXT:         .write(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn(i32, i32) -> i32>,
// CHECK-NEXT:                 *const (),
// CHECK-NEXT:             >(Some(add))
// CHECK-NEXT:         });
// CHECK-NEXT:     (*[[T_1]]).0[1] = 5u64;
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: static mut scalar: std::mem::MaybeUninit<__SlateImage_scalar> = unsafe {
// CHECK-NEXT:     let mut [[T_0]]: std::mem::MaybeUninit<__SlateImage_scalar> = std::mem::MaybeUninit::zeroed();
// CHECK-NEXT:     let [[T_1]] = [[T_0]].as_mut_ptr();
// CHECK-NEXT:     (std::ptr::addr_of_mut!(* [[T_1]]) as *mut *const ())
// CHECK-NEXT:         .write(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn(i32, i32) -> i32>,
// CHECK-NEXT:                 *const (),
// CHECK-NEXT:             >(Some(add))
// CHECK-NEXT:         });
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: static mut third: std::mem::MaybeUninit<__SlateImage_third> = unsafe {
// CHECK-NEXT:     let mut [[T_0]]: std::mem::MaybeUninit<__SlateImage_third> = std::mem::MaybeUninit::zeroed();
// CHECK-NEXT:     let [[T_1]] = [[T_0]].as_mut_ptr();
// CHECK-NEXT:     (std::ptr::addr_of_mut!(* [[T_1]]) as *mut *const ())
// CHECK-NEXT:         .write(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 *mut i32,
// CHECK-NEXT:                 *const (),
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(data.0) as *mut i32 }
// CHECK-NEXT:                     .offset(2i32 as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         });
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: static mut past_first: std::mem::MaybeUninit<__SlateImage_past_first> = unsafe {
// CHECK-NEXT:     let mut [[T_0]]: std::mem::MaybeUninit<__SlateImage_past_first> = std::mem::MaybeUninit::zeroed();
// CHECK-NEXT:     let [[T_1]] = [[T_0]].as_mut_ptr();
// CHECK-NEXT:     (std::ptr::addr_of_mut!(* [[T_1]]) as *mut *const ())
// CHECK-NEXT:         .write(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     *mut i32,
// CHECK-NEXT:                     *const (),
// CHECK-NEXT:                 >(unsafe { std::ptr::addr_of_mut!(data.0) as *mut i32 })
// CHECK-NEXT:             }
// CHECK-NEXT:                 .wrapping_byte_offset(4isize),
// CHECK-NEXT:         );
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: static mut before_last: std::mem::MaybeUninit<__SlateImage_before_last> = unsafe {
// CHECK-NEXT:     let mut [[T_0]]: std::mem::MaybeUninit<__SlateImage_before_last> = std::mem::MaybeUninit::zeroed();
// CHECK-NEXT:     let [[T_1]] = [[T_0]].as_mut_ptr();
// CHECK-NEXT:     (std::ptr::addr_of_mut!(* [[T_1]]) as *mut *const ())
// CHECK-NEXT:         .write(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     *mut i32,
// CHECK-NEXT:                     *const (),
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(data.0) as *mut i32 }
// CHECK-NEXT:                         .offset(3i32 as isize)
// CHECK-NEXT:                 })
// CHECK-NEXT:             }
// CHECK-NEXT:                 .wrapping_byte_offset(-4isize),
// CHECK-NEXT:         );
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: static mut constant_handle: std::mem::MaybeUninit<__SlateImage_constant_handle> = unsafe {
// CHECK-NEXT:     let mut [[T_0]]: std::mem::MaybeUninit<__SlateImage_constant_handle> = std::mem::MaybeUninit::zeroed();
// CHECK-NEXT:     let [[T_1]] = [[T_0]].as_mut_ptr();
// CHECK-NEXT:     (std::ptr::addr_of_mut!(* [[T_1]]) as *mut *const ())
// CHECK-NEXT:         .write(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn(i32, i32) -> i32>,
// CHECK-NEXT:                 *const (),
// CHECK-NEXT:             >(Some(sub))
// CHECK-NEXT:         });
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct entry {
// CHECK-NEXT:     tag: i32,
// CHECK-NEXT:     r#fn: u64,
// CHECK-NEXT:     bias: i64,
// CHECK-NEXT: }
// CHECK-NEXT: static mut entries: std::mem::MaybeUninit<__SlateImage_entries> = unsafe {
// CHECK-NEXT:     let mut [[T_0]]: std::mem::MaybeUninit<__SlateImage_entries> = std::mem::MaybeUninit::zeroed();
// CHECK-NEXT:     let [[T_1]] = [[T_0]].as_mut_ptr();
// CHECK-NEXT:     (*[[T_1]]).0[0].tag = 7i32;
// CHECK-NEXT:     (std::ptr::addr_of_mut!((* [[T_1]]).0[0].r#fn) as *mut *const ())
// CHECK-NEXT:         .write(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn(i32, i32) -> i32>,
// CHECK-NEXT:                 *const (),
// CHECK-NEXT:             >(Some(sub))
// CHECK-NEXT:         });
// CHECK-NEXT:     (*[[T_1]]).0[0].bias = 3i64;
// CHECK-NEXT:     (*[[T_1]]).0[1].tag = 1i32;
// CHECK-NEXT:     (std::ptr::addr_of_mut!((* [[T_1]]).0[1].r#fn) as *mut *const ())
// CHECK-NEXT:         .write(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn(i32, i32) -> i32>,
// CHECK-NEXT:                 *const (),
// CHECK-NEXT:             >(Some(add))
// CHECK-NEXT:         });
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct outer {
// CHECK-NEXT:     label: i8,
// CHECK-NEXT:     inner: entry,
// CHECK-NEXT:     slots: [u64; 2],
// CHECK-NEXT: }
// CHECK-NEXT: static mut nested: std::mem::MaybeUninit<__SlateImage_nested> = unsafe {
// CHECK-NEXT:     let mut [[T_0]]: std::mem::MaybeUninit<__SlateImage_nested> = std::mem::MaybeUninit::zeroed();
// CHECK-NEXT:     let [[T_1]] = [[T_0]].as_mut_ptr();
// CHECK-NEXT:     (*[[T_1]]).label = 110i8;
// CHECK-NEXT:     (*[[T_1]]).inner.tag = 4i32;
// CHECK-NEXT:     (std::ptr::addr_of_mut!((* [[T_1]]).inner.r#fn) as *mut *const ())
// CHECK-NEXT:         .write(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn(i32, i32) -> i32>,
// CHECK-NEXT:                 *const (),
// CHECK-NEXT:             >(Some(add))
// CHECK-NEXT:         });
// CHECK-NEXT:     (*[[T_1]]).inner.bias = 9i64;
// CHECK-NEXT:     (std::ptr::addr_of_mut!((* [[T_1]]).slots[0]) as *mut *const ())
// CHECK-NEXT:         .write(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 *mut i32,
// CHECK-NEXT:                 *const (),
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(data.0) as *mut i32 }
// CHECK-NEXT:                     .offset(1i32 as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:         });
// CHECK-NEXT:     (*[[T_1]]).slots[1] = 0u64;
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut sum: i32 = 0;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut i: i32 = 0;
// CHECK-NEXT:         while (unsafe {
// CHECK-NEXT:             (*(std::ptr::addr_of_mut!(kernels) as *mut __SlateImage_kernels))
// CHECK-NEXT:                 .0[i as usize]
// CHECK-NEXT:         }) != 0
// CHECK-NEXT:         {
// CHECK-NEXT:             let [[V_0:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         usize,
// CHECK-NEXT:                         Option<unsafe extern "C-unwind" fn(i32, i32) -> i32>,
// CHECK-NEXT:                     >(
// CHECK-NEXT:                         (*(std::ptr::addr_of_mut!(kernels) as *mut __SlateImage_kernels))
// CHECK-NEXT:                             .0[i as usize] as usize,
// CHECK-NEXT:                     )
// CHECK-NEXT:                 }
// CHECK-NEXT:                     .unwrap()(10i32, i + 1)
// CHECK-NEXT:             };
// CHECK-NEXT:             sum += [[V_0]];
// CHECK-NEXT:             i += 1;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     sum
// CHECK-NEXT:         += (unsafe {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     usize,
// CHECK-NEXT:                     Option<unsafe extern "C-unwind" fn(i32, i32) -> i32>,
// CHECK-NEXT:                 >(
// CHECK-NEXT:                     (*(std::ptr::addr_of_mut!(exported_kernels)
// CHECK-NEXT:                         as *mut __SlateImage_exported_kernels))
// CHECK-NEXT:                         .0[0] as usize,
// CHECK-NEXT:                 )
// CHECK-NEXT:             }
// CHECK-NEXT:                 .unwrap()(9i32, 4i32)
// CHECK-NEXT:         })
// CHECK-NEXT:             + unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         usize,
// CHECK-NEXT:                         Option<unsafe extern "C-unwind" fn(i32, i32) -> i32>,
// CHECK-NEXT:                     >(
// CHECK-NEXT:                         (*(std::ptr::addr_of_mut!(exported_kernels)
// CHECK-NEXT:                             as *mut __SlateImage_exported_kernels))
// CHECK-NEXT:                             .0[1] as usize,
// CHECK-NEXT:                     )
// CHECK-NEXT:                 }
// CHECK-NEXT:                     .unwrap()(9i32, 4i32)
// CHECK-NEXT:             };
// CHECK-NEXT:     sum
// CHECK-NEXT:         += (unsafe {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     usize,
// CHECK-NEXT:                     Option<unsafe extern "C-unwind" fn(i32, i32) -> i32>,
// CHECK-NEXT:                 >(
// CHECK-NEXT:                     (*(std::ptr::addr_of_mut!(aligned) as *mut __SlateImage_aligned))
// CHECK-NEXT:                         .0[0] as usize,
// CHECK-NEXT:                 )
// CHECK-NEXT:             }
// CHECK-NEXT:                 .unwrap()(
// CHECK-NEXT:                 1i32,
// CHECK-NEXT:                 (*(std::ptr::addr_of_mut!(aligned) as *mut __SlateImage_aligned)).0[1]
// CHECK-NEXT:                     as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         })
// CHECK-NEXT:             + ((((unsafe {
// CHECK-NEXT:                 std::ptr::addr_of_mut!(
// CHECK-NEXT:                     (* (std::ptr::addr_of_mut!(aligned) as * mut __SlateImage_aligned)).0
// CHECK-NEXT:                 ) as *mut u64
// CHECK-NEXT:             }) as u64) & 31 == 0) as i32);
// CHECK-NEXT:     sum
// CHECK-NEXT:         += unsafe {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     usize,
// CHECK-NEXT:                     Option<unsafe extern "C-unwind" fn(i32, i32) -> i32>,
// CHECK-NEXT:                 >(*(std::ptr::addr_of_mut!(scalar) as *mut __SlateImage_scalar) as usize)
// CHECK-NEXT:             }
// CHECK-NEXT:                 .unwrap()(1i32, 2i32)
// CHECK-NEXT:         };
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: i32 = sum
// CHECK-NEXT:         + ((unsafe {
// CHECK-NEXT:             *(*(std::ptr::addr_of_mut!(third) as *mut __SlateImage_third) as *mut i32)
// CHECK-NEXT:         })
// CHECK-NEXT:             + unsafe {
// CHECK-NEXT:                 *(*(std::ptr::addr_of_mut!(past_first) as *mut __SlateImage_past_first)
// CHECK-NEXT:                     as *mut i32)
// CHECK-NEXT:             }
// CHECK-NEXT:             + unsafe {
// CHECK-NEXT:                 *(*(std::ptr::addr_of_mut!(before_last) as *mut __SlateImage_before_last)
// CHECK-NEXT:                     as *mut i32)
// CHECK-NEXT:             });
// CHECK-NEXT:     sum = [[V_1]];
// CHECK-NEXT:     sum
// CHECK-NEXT:         += unsafe {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     usize,
// CHECK-NEXT:                     Option<unsafe extern "C-unwind" fn(i32, i32) -> i32>,
// CHECK-NEXT:                 >(
// CHECK-NEXT:                     *(std::ptr::addr_of_mut!(constant_handle)
// CHECK-NEXT:                         as *mut __SlateImage_constant_handle) as usize,
// CHECK-NEXT:                 )
// CHECK-NEXT:             }
// CHECK-NEXT:                 .unwrap()(20i32, 6i32)
// CHECK-NEXT:         };
// CHECK-NEXT:     for i in 0i32..2i32 {
// CHECK-NEXT:         let [[V_2:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     usize,
// CHECK-NEXT:                     Option<unsafe extern "C-unwind" fn(i32, i32) -> i32>,
// CHECK-NEXT:                 >(
// CHECK-NEXT:                     (*(std::ptr::addr_of_mut!(entries) as *mut __SlateImage_entries))
// CHECK-NEXT:                         .0[i as usize]
// CHECK-NEXT:                         .r#fn as usize,
// CHECK-NEXT:                 )
// CHECK-NEXT:             }
// CHECK-NEXT:                 .unwrap()(
// CHECK-NEXT:                 (*(std::ptr::addr_of_mut!(entries) as *mut __SlateImage_entries))
// CHECK-NEXT:                     .0[i as usize]
// CHECK-NEXT:                     .tag,
// CHECK-NEXT:                 (*(std::ptr::addr_of_mut!(entries) as *mut __SlateImage_entries))
// CHECK-NEXT:                     .0[i as usize]
// CHECK-NEXT:                     .bias as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         sum += [[V_2]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let [[V_3:__v[0-9]+]]: i32 = ((unsafe {
// CHECK-NEXT:         (*(std::ptr::addr_of_mut!(nested) as *mut __SlateImage_nested)).label
// CHECK-NEXT:     }) as i32)
// CHECK-NEXT:         + unsafe {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     usize,
// CHECK-NEXT:                     Option<unsafe extern "C-unwind" fn(i32, i32) -> i32>,
// CHECK-NEXT:                 >(
// CHECK-NEXT:                     (*(std::ptr::addr_of_mut!(nested) as *mut __SlateImage_nested))
// CHECK-NEXT:                         .inner
// CHECK-NEXT:                         .r#fn as usize,
// CHECK-NEXT:                 )
// CHECK-NEXT:             }
// CHECK-NEXT:                 .unwrap()(
// CHECK-NEXT:                 (*(std::ptr::addr_of_mut!(nested) as *mut __SlateImage_nested))
// CHECK-NEXT:                     .inner
// CHECK-NEXT:                     .tag,
// CHECK-NEXT:                 (*(std::ptr::addr_of_mut!(nested) as *mut __SlateImage_nested))
// CHECK-NEXT:                     .inner
// CHECK-NEXT:                     .bias as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     sum += [[V_3]];
// CHECK-NEXT:     let [[V_4:__v[0-9]+]]: i32 = sum
// CHECK-NEXT:         + ((unsafe {
// CHECK-NEXT:             *((*(std::ptr::addr_of_mut!(nested) as *mut __SlateImage_nested)).slots[0]
// CHECK-NEXT:                 as *mut i32)
// CHECK-NEXT:         })
// CHECK-NEXT:             + ((unsafe {
// CHECK-NEXT:                 (*(std::ptr::addr_of_mut!(nested) as *mut __SlateImage_nested)).slots[1]
// CHECK-NEXT:             }) as i32));
// CHECK-NEXT:     sum = [[V_4]];
// CHECK-NEXT:     let cursor: *mut u64 = unsafe {
// CHECK-NEXT:         std::ptr::addr_of_mut!(
// CHECK-NEXT:             (* (std::ptr::addr_of_mut!(kernels) as * mut __SlateImage_kernels)).0
// CHECK-NEXT:         ) as *mut u64
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[V_5:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 usize,
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn(i32, i32) -> i32>,
// CHECK-NEXT:             >(*cursor.add(1) as usize)
// CHECK-NEXT:         }
// CHECK-NEXT:             .unwrap()(5i32, 1i32)
// CHECK-NEXT:     };
// CHECK-NEXT:     sum += [[V_5]];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(std::ptr::addr_of_mut!(kernels) as *mut __SlateImage_kernels)).0[2] = (unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn(i32, i32) -> i32>,
// CHECK-NEXT:                 usize,
// CHECK-NEXT:             >(Some(add))
// CHECK-NEXT:         }) as u64;
// CHECK-NEXT:     }
// CHECK-NEXT:     let [[V_6:__v[0-9]+]]: i32 = sum;
// CHECK-NEXT:     sum = [[V_6]]
// CHECK-NEXT:         + (((unsafe {
// CHECK-NEXT:             (*(std::ptr::addr_of_mut!(kernels) as *mut __SlateImage_kernels)).0[2]
// CHECK-NEXT:         })
// CHECK-NEXT:             == ((unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     Option<unsafe extern "C-unwind" fn(i32, i32) -> i32>,
// CHECK-NEXT:                     usize,
// CHECK-NEXT:                 >(Some(add))
// CHECK-NEXT:             }) as u64)) as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(std::ptr::addr_of_mut!(kernels) as *mut __SlateImage_kernels)).0[2] = 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         data.0[2] = 30;
// CHECK-NEXT:     }
// CHECK-NEXT:     let [[V_7:__v[0-9]+]]: i32 = sum
// CHECK-NEXT:         + unsafe {
// CHECK-NEXT:             *(*(std::ptr::addr_of_mut!(third) as *mut __SlateImage_third) as *mut i32)
// CHECK-NEXT:         };
// CHECK-NEXT:     sum = [[V_7]];
// CHECK-NEXT:     println!("{}", sum);
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
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
