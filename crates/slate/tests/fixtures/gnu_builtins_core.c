#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

struct GNUBuiltinOffset {
  unsigned char tag;
  int           values[3];
};

static int volatile gnu_builtin_never;

static int gnu_builtin_nonlocal(void) {
#if defined(__x86_64__) || defined(__i386__)
  void *buffer[5];
  if (__builtin_setjmp(buffer) == 0) {
    __builtin_longjmp(buffer, 1);
  }
#endif
  return 17;
}

static int gnu_builtin_addresses(void) {
  void *return_address = __builtin_return_address(0);
  void *extracted      = __builtin_extract_return_addr(return_address);
  void *restored       = __builtin_frob_return_addr(extracted);
  void *frame_address  = __builtin_frame_address(0);
  return (return_address != NULL) + (extracted != NULL) + (restored != NULL) +
         (frame_address != NULL);
}

static int gnu_builtin_compile_time(void) {
  int volatile runtime = 3;
  int selected         = __builtin_choose_expr(
      __builtin_types_compatible_p(int, signed int), 19, 23);
  int type_total = __builtin_types_compatible_p(int, int) +
                   __builtin_types_compatible_p(int, long);
  return __builtin_constant_p(5) + __builtin_constant_p(runtime) + selected +
         type_total;
}

static int gnu_builtin_control(void) {
  _Alignas(32) unsigned char storage[64] = {};
  unsigned char              code[8]     = {};
  void                      *assumed = __builtin_assume_aligned(storage, 32);
  int                        likely  = __builtin_expect(29, 29);
  int probable = __builtin_expect_with_probability(31, 31, 0.9);
  __builtin_assume(storage[0] == 0);
  __builtin_prefetch(storage + 16, 0, 3);
  __builtin___clear_cache((char *)code, (char *)code + sizeof(code));
  if (gnu_builtin_never) {
    __builtin_debugtrap();
  }
  if (gnu_builtin_never) {
    __builtin_trap();
  }
  if (gnu_builtin_never) {
    __builtin_unreachable();
  }
  return likely + probable + (assumed == storage);
}

static int gnu_builtin_sizes(void) {
  unsigned char object[24] = {};
  int           value      = 37;
  int          *address    = __builtin_addressof(value);
  uintptr_t     up         = __builtin_align_up((uintptr_t)65, 16);
  uintptr_t     down       = __builtin_align_down((uintptr_t)79, 16);
  return (int)__builtin_object_size(object, 0) +
         (int)__builtin_dynamic_object_size(object, 0) +
         (int)__builtin_offsetof(struct GNUBuiltinOffset, values[2]) +
         (address == &value) + (int)up + (int)down +
         __builtin_is_aligned(up, 16) + !__builtin_is_aligned(65, 16);
}

static int gnu_builtin_source(void) {
  return (__builtin_FUNCTION()[0] == 'g') + (__builtin_FILE()[0] != '\0') +
         (__builtin_LINE() > 0) + (__builtin_COLUMN() > 0);
}

int main(void) {
  printf("%d %d %d %d %d %d\n", gnu_builtin_compile_time(),
         gnu_builtin_control(), gnu_builtin_sizes(), gnu_builtin_source(),
         gnu_builtin_nonlocal(), gnu_builtin_addresses());
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(link_llvm_intrinsics)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: mod __slate_builtin_setjmp {
// CHECK-NEXT:     #[repr(C, align(16))]
// CHECK-NEXT:     pub struct Buffer([u8; 512]);
// CHECK-NEXT:     unsafe extern "C" {
// CHECK-NEXT:         pub fn _setjmp(env: *mut Buffer) -> i32;
// CHECK-NEXT:         pub fn _longjmp(env: *mut Buffer, value: i32);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT:     fn __builtin_extract_return_addr([[ARG_0]]: *mut ()) -> *mut ();
// CHECK-NEXT:     fn __builtin_frob_return_addr([[ARG_0]]: *mut ()) -> *mut ();
// CHECK-NEXT:     fn __builtin_assume_aligned([[ARG_0]]: *const (), [[ARG_1:arg[0-9]+]]: u64, ...) -> *mut ();
// CHECK-NEXT:     fn __builtin_expect([[ARG_0]]: i64, [[ARG_1]]: i64) -> i64;
// CHECK-NEXT:     fn __builtin_expect_with_probability([[ARG_0]]: i64, [[ARG_1]]: i64, [[ARG_2:arg[0-9]+]]: f64) -> i64;
// CHECK-NEXT:     fn __builtin_assume([[ARG_0]]: bool);
// CHECK-NEXT:     fn __builtin_prefetch([[ARG_0]]: *const (), ...);
// CHECK-NEXT:     fn __builtin_unreachable();
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "llvm-intrinsic" {
// CHECK-NEXT:     #[link_name = "llvm.frameaddress.p0"]
// CHECK-NEXT:     fn __slate_intrinsic_2d7c29df8fdf6378([[ARG_0]]: i32) -> *mut ();
// CHECK-NEXT:     #[link_name = "llvm.trap"]
// CHECK-NEXT:     fn __slate_intrinsic_46d8fd79c9d1f86c();
// CHECK-NEXT:     #[link_name = "llvm.clear_cache"]
// CHECK-NEXT:     fn __slate_intrinsic_7eb885185d29c159([[ARG_0]]: *mut (), [[ARG_1]]: *mut ());
// CHECK-NEXT:     #[link_name = "llvm.debugtrap"]
// CHECK-NEXT:     fn __slate_intrinsic_b996fac1ccf0610c();
// CHECK-NEXT:     #[link_name = "llvm.returnaddress.p0"]
// CHECK-NEXT:     fn __slate_intrinsic_ded315013ea6523([[ARG_0]]: i32) -> *mut ();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: #[repr(C, align(32))]
// CHECK-NEXT: struct __SlateAlign32<T>(T);
// CHECK-NEXT: static mut gnu_builtin_never: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn gnu_builtin_nonlocal() -> i32 {
// CHECK-NEXT:     let mut [[T_0:__t[0-9]+]]: __slate_builtin_setjmp::Buffer = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut buffer: __SlateAlign16<[*mut (); 5]> = __SlateAlign16([0 as *mut (); 5]);
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         *(buffer.0.as_mut_ptr() as *mut *mut __slate_builtin_setjmp::Buffer) = std::ptr::addr_of_mut!(
// CHECK-NEXT:             [[T_0]]
// CHECK-NEXT:         );
// CHECK-NEXT:         __slate_builtin_setjmp::_setjmp(std::ptr::addr_of_mut!([[T_0]])) as i32
// CHECK-NEXT:     }) == 0
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             __slate_builtin_setjmp::_longjmp(
// CHECK-NEXT:                 *(buffer.0.as_mut_ptr() as *mut *mut __slate_builtin_setjmp::Buffer),
// CHECK-NEXT:                 1i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     17
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_builtin_addresses() -> i32 {
// CHECK-NEXT:     let return_address: *mut () = unsafe {
// CHECK-NEXT:         __slate_intrinsic_ded315013ea6523(const { ((0 as i32) as u32) as i32 })
// CHECK-NEXT:     };
// CHECK-NEXT:     let extracted: *mut () = return_address;
// CHECK-NEXT:     let restored: *mut () = extracted;
// CHECK-NEXT:     let frame_address: *mut () = unsafe {
// CHECK-NEXT:         __slate_intrinsic_2d7c29df8fdf6378(const { ((0 as i32) as u32) as i32 })
// CHECK-NEXT:     };
// CHECK-NEXT:     (!return_address.is_null() as i32) + (!extracted.is_null() as i32)
// CHECK-NEXT:         + (!restored.is_null() as i32) + (!frame_address.is_null() as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_builtin_compile_time() -> i32 {
// CHECK-NEXT:     let selected: i32 = 19;
// CHECK-NEXT:     let type_total: i32 = 1 + 0;
// CHECK-NEXT:     1 + 0 + selected + type_total
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_builtin_control() -> i32 {
// CHECK-NEXT:     let mut storage: __SlateAlign32<[u8; 64]> = __SlateAlign32({
// CHECK-NEXT:         let mut [[T_0]]: [u8; 64] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     });
// CHECK-NEXT:     let mut code: [u8; 8] = {
// CHECK-NEXT:         let mut [[T_1:__t[0-9]+]]: [u8; 8] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let assumed: *mut () = storage.0.as_mut_ptr() as *mut ();
// CHECK-NEXT:     let likely: i32 = 29;
// CHECK-NEXT:     let probable: i32 = 31;
// CHECK-NEXT:     unsafe { std::hint::assert_unchecked(((storage.0[0] as u32) as i32) == 0) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __slate_intrinsic_7eb885185d29c159(
// CHECK-NEXT:             code.as_mut_ptr() as *mut (),
// CHECK-NEXT:             (code.as_mut_ptr() as *mut i8).add(8) as *mut (),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     if (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(gnu_builtin_never)) }) != 0 {
// CHECK-NEXT:         unsafe { __slate_intrinsic_b996fac1ccf0610c() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(gnu_builtin_never)) }) != 0 {
// CHECK-NEXT:         unsafe { __slate_intrinsic_46d8fd79c9d1f86c() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(gnu_builtin_never)) }) != 0 {
// CHECK-NEXT:         unsafe { std::hint::unreachable_unchecked() };
// CHECK-NEXT:     }
// CHECK-NEXT:     likely + probable + ((assumed == (storage.0.as_mut_ptr() as *mut ())) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_builtin_sizes() -> i32 {
// CHECK-NEXT:     __SlateAlign16({
// CHECK-NEXT:         let mut [[T_0]]: [u8; 24] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     });
// CHECK-NEXT:     let mut value: i32 = 37;
// CHECK-NEXT:     let address: *mut i32 = std::ptr::addr_of_mut!(value);
// CHECK-NEXT:     let mut up: u64 = 0;
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: u64 = 15u64;
// CHECK-NEXT:     up = 65u64.wrapping_add([[V_0]]) & ![[V_0]];
// CHECK-NEXT:     let mut down: u64 = 0;
// CHECK-NEXT:     down = 64u64;
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: u64 = 15u64;
// CHECK-NEXT:     24 + 24 + 12 + ((address == std::ptr::addr_of_mut!(value)) as i32)
// CHECK-NEXT:         + ((up as u32) as i32) + ((down as u32) as i32) + ((up & [[V_1]] == 0) as i32)
// CHECK-NEXT:         + ((1i32 != 0) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_builtin_source() -> i32 {
// CHECK-NEXT:     ((((unsafe { *(c"gnu_builtin_source".as_ptr() as *mut i8).add(0) }) as i32) == 103)
// CHECK-NEXT:         as i32)
// CHECK-NEXT:         + ((((unsafe {
// CHECK-NEXT:             *(c"{{.*}}tests/fixtures/gnu_builtins_core.c"
// CHECK-NEXT:                 .as_ptr() as *mut i8)
// CHECK-NEXT:                 .add(0)
// CHECK-NEXT:         }) as i32) != 0) as i32) + (true as i32) + (true as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {} {} {}", gnu_builtin_compile_time(), gnu_builtin_control(),
// CHECK-NEXT:         gnu_builtin_sizes(), gnu_builtin_source(), gnu_builtin_nonlocal(),
// CHECK-NEXT:         gnu_builtin_addresses()
// CHECK-NEXT:     );
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
