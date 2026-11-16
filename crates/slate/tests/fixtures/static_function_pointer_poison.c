#include <stdio.h>

#define POISON_DELTA 0xdead000000000000UL
#define PTR_POISON ((void *)(0xeB9FUL + POISON_DELTA))

typedef unsigned long (*binary_fn)(unsigned long, unsigned long);

struct proto {
    binary_fn func;
    int gpl_only;
    const char *name;
};

static unsigned long add(unsigned long a, unsigned long b) { return a + b; }

const struct proto tail_proto = {.func = PTR_POISON, .gpl_only = 1, .name = "tail"};
const struct proto add_proto = {.func = add, .name = "add"};
const struct proto *protos[] = {&tail_proto, &add_proto};
binary_fn poisoned_hook = PTR_POISON;

int main(void) {
    for (int i = 0; i < 2; i++) {
        const struct proto *p = protos[i];
        if ((void *)p->func == PTR_POISON)
            printf("%s poison %d\n", p->name, p->gpl_only);
        else
            printf("%s %lu\n", p->name, p->func(2, 3));
    }
    printf("%d %d\n", tail_proto.gpl_only, (void *)poisoned_hook == PTR_POISON);
    poisoned_hook = add;
    printf("%lu\n", poisoned_hook(4, 5));
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct proto {
// CHECK-NEXT:     func: Option<unsafe extern "C-unwind" fn(u64, u64) -> u64>,
// CHECK-NEXT:     gpl_only: i32,
// CHECK-NEXT:     name: *const i8,
// CHECK-NEXT: }
// CHECK-NEXT: type __SlateImage_tail_proto = proto;
// CHECK-NEXT: type __SlateImage_poisoned_hook = Option<unsafe extern "C-unwind" fn(u64, u64) -> u64>;
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe impl<T> Sync for __SlateAlign16<T> {}
// CHECK-NEXT: #[repr(transparent)]
// CHECK-NEXT: struct __SlateConst<T>(T);
// CHECK-NEXT: unsafe impl<T> Sync for __SlateConst<T> {}
// CHECK-NEXT: extern "C-unwind" fn add(a: u64, b: u64) -> u64 {
// CHECK-NEXT:     a.wrapping_add(b)
// CHECK-NEXT: }
// CHECK-NEXT: static mut tail_proto: core::mem::MaybeUninit<__SlateImage_tail_proto> = unsafe {
// CHECK-NEXT:     let mut [[T_0:__t[0-9]+]]: core::mem::MaybeUninit<__SlateImage_tail_proto> = core::mem::MaybeUninit::zeroed();
// CHECK-NEXT:     let [[T_1:__t[0-9]+]] = [[T_0]].as_mut_ptr();
// CHECK-NEXT:     (*[[T_1]]).func = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             *mut (),
// CHECK-NEXT:             Option<unsafe extern "C-unwind" fn(u64, u64) -> u64>,
// CHECK-NEXT:         >(16045481047391005599u64 as *mut ())
// CHECK-NEXT:     };
// CHECK-NEXT:     (*[[T_1]]).gpl_only = 1i32;
// CHECK-NEXT:     (*[[T_1]]).name = (b"tail\0".as_ptr() as *mut i8) as *const i8;
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: static add_proto: __SlateConst<proto> = __SlateConst({
// CHECK-NEXT:     let mut [[T_0]]: proto = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].func = Some(add);
// CHECK-NEXT:     [[T_0]].name = (b"add\0".as_ptr() as *mut i8) as *const i8;
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: });
// CHECK-NEXT: static mut protos: __SlateAlign16<[*const proto; 2]> = __SlateAlign16([
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::ptr::addr_of!(
// CHECK-NEXT:             * (core::ptr::addr_of_mut!(tail_proto) as * mut __SlateImage_tail_proto)
// CHECK-NEXT:         )
// CHECK-NEXT:     },
// CHECK-NEXT:     unsafe { core::ptr::addr_of!(add_proto.0) },
// CHECK-NEXT: ]);
// CHECK-NEXT: static mut poisoned_hook: core::mem::MaybeUninit<__SlateImage_poisoned_hook> = unsafe {
// CHECK-NEXT:     let mut [[T_0]]: core::mem::MaybeUninit<__SlateImage_poisoned_hook> = core::mem::MaybeUninit::zeroed();
// CHECK-NEXT:     let [[T_1]] = [[T_0]].as_mut_ptr();
// CHECK-NEXT:     *[[T_1]] = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             *mut (),
// CHECK-NEXT:             Option<unsafe extern "C-unwind" fn(u64, u64) -> u64>,
// CHECK-NEXT:         >(16045481047391005599u64 as *mut ())
// CHECK-NEXT:     };
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     for i in 0i32..2i32 {
// CHECK-NEXT:         let p: *const proto = unsafe { protos.0[i as usize] };
// CHECK-NEXT:         if (unsafe {
// CHECK-NEXT:             core::mem::transmute::<
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn(u64, u64) -> u64>,
// CHECK-NEXT:                 *mut (),
// CHECK-NEXT:             >(unsafe { (*p).func })
// CHECK-NEXT:         }) == (16045481047391005599u64 as *mut ())
// CHECK-NEXT:         {
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 slate_support::printf(
// CHECK-NEXT:                     c"%s poison %d\n".as_ptr(),
// CHECK-NEXT:                     (*p).name,
// CHECK-NEXT:                     (*p).gpl_only,
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:             unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:         } else {
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 slate_support::printf(
// CHECK-NEXT:                     c"%s %lu\n".as_ptr(),
// CHECK-NEXT:                     (*p).name,
// CHECK-NEXT:                     unsafe { (*p).func }.unwrap()(2u64, 3u64),
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:             unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {}", unsafe { (* (core::ptr::addr_of_mut!(tail_proto) as * mut
// CHECK-NEXT:         __SlateImage_tail_proto)).gpl_only }, ((unsafe { core::mem::transmute::< Option <
// CHECK-NEXT:         unsafe extern "C-unwind" fn (u64, u64) -> u64 >, * mut () > (unsafe { *
// CHECK-NEXT:         (core::ptr::addr_of_mut!(poisoned_hook) as * mut __SlateImage_poisoned_hook) })
// CHECK-NEXT:         }) == (16045481047391005599u64 as * mut ())) as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(core::ptr::addr_of_mut!(poisoned_hook) as *mut __SlateImage_poisoned_hook) = Some(
// CHECK-NEXT:             add,
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%lu\n".as_ptr(),
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *(core::ptr::addr_of_mut!(poisoned_hook)
// CHECK-NEXT:                     as *mut __SlateImage_poisoned_hook)
// CHECK-NEXT:             }
// CHECK-NEXT:                 .unwrap()(4u64, 5u64),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
