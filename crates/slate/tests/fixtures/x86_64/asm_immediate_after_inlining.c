// { dg-options "-O2" }
#include <stdio.h>

struct key {
    int enabled;
};

struct key k1 = {1};
struct key k2 = {2};

static inline __attribute__((always_inline)) const char *key_address(struct key *key, _Bool next) {
    const char *address;
    __asm__("leaq %c1(%%rip), %0" : "=r"(address) : "i"(&((char *)key)[next]));
    return address;
}

static inline __attribute__((always_inline)) int branch(struct key *key, _Bool likely) {
    __asm__ goto("jmp %l[yes]\n\t.pushsection .data.slate_keys,\"aw\"\n\t.quad %c0 + %c1\n\t.popsection"
                 : : "i"(key), "i"(likely) : : yes);
    return 0;
yes:
    return 1;
}

static inline __attribute__((always_inline)) int bit(int nr) {
    int out;
    __asm__("movl %1, %0" : "=r"(out) : "i"(1 << (nr & 7)));
    return out;
}

static inline __attribute__((always_inline)) const char *outer(struct key *key) {
    return key_address(key, 0);
}

struct holder {
    struct key key_field;
} holder = {{3}};

#define HOLDER_BRANCH(h) branch(&(h)->key_field, 1)
#define KEY_BRANCH(k) branch(k, 0)

#define FLAGGED(f)                                                   \
    ({                                                               \
        __auto_type __flags = 0x100 | (f);                           \
        int __out;                                                   \
        __asm__("movl %1, %0" : "=r"(__out) : "i"(__flags));         \
        __out;                                                       \
    })

int main(void) {
    printf("%td %td %td\n", key_address(&k1, 1) - (const char *)&k1,
           key_address(&k2, 0) - (const char *)&k2, outer(&k2) - (const char *)&k2);
    printf("%d %d %d\n", branch(&k1, 1), branch(&k2, 0), branch(&k1, 1));
    printf("%d %d\n", bit(9), bit(3));
    printf("%d %d\n", FLAGGED(3), FLAGGED(1 << 4));
    printf("%d %d\n", HOLDER_BRANCH(&holder), KEY_BRANCH(&k2));
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-O2" }
// CHECK-NEXT: macro_rules! HOLDER_BRANCH {
// CHECK-NEXT:     ($h:expr) => {
// CHECK-NEXT:         branch__const8(unsafe { core::ptr::addr_of_mut!((*$h).key_field) }, true)
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! KEY_BRANCH {
// CHECK-NEXT:     ($k:expr) => {
// CHECK-NEXT:         branch__const9($k, false)
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct key {
// CHECK-NEXT:     enabled: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut k1: key = key { enabled: 1i32 };
// CHECK-EMPTY:
// CHECK-NEXT: static mut k2: key = key { enabled: 2i32 };
// CHECK-EMPTY:
// CHECK-NEXT: fn key_address__const1(key: *mut key, next: bool) -> *const i8 {
// CHECK-NEXT:     let mut address: *const i8 = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u64;
// CHECK-NEXT:         core::arch::asm!("leaq {1}+1(%rip), {0:r}", lateout(reg) [[T_0]], sym k1, options(pure, nomem, nostack, att_syntax));
// CHECK-NEXT:         address = unsafe { core::mem::transmute::<u64, *const i8>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     address
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn key_address__const2(key: *mut key, next: bool) -> *const i8 {
// CHECK-NEXT:     let mut address: *const i8 = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u64;
// CHECK-NEXT:         core::arch::asm!("leaq {1}(%rip), {0:r}", lateout(reg) [[T_0]], sym k2, options(pure, nomem, nostack, att_syntax));
// CHECK-NEXT:         address = unsafe { core::mem::transmute::<u64, *const i8>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     address
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn branch__const4(key: *mut key, likely: bool) -> i32 {
// CHECK-NEXT:     let mut __slate_state: usize = 2usize;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             match __slate_state {
// CHECK-NEXT:                 0 => {
// CHECK-NEXT:                     return 1;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 1 => {
// CHECK-NEXT:                     return 0;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 2 => {
// CHECK-NEXT:                     __slate_state = 1usize;
// CHECK-NEXT:                     core::arch::asm!("jmp {2}\n\t.pushsection .data.slate_keys,\"aw\"\n\t.quad {0} + {1}\n\t.popsection", sym k1, const 1 as u8, label {
// CHECK-NEXT: __slate_state = 0usize;
// CHECK-NEXT: }, options(nostack, att_syntax));
// CHECK-NEXT:                     continue;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 _ => {
// CHECK-NEXT:                     unreachable!();
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { core::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn branch__const5(key: *mut key, likely: bool) -> i32 {
// CHECK-NEXT:     let mut __slate_state: usize = 2usize;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             match __slate_state {
// CHECK-NEXT:                 0 => {
// CHECK-NEXT:                     return 1;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 1 => {
// CHECK-NEXT:                     return 0;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 2 => {
// CHECK-NEXT:                     __slate_state = 1usize;
// CHECK-NEXT:                     core::arch::asm!("jmp {2}\n\t.pushsection .data.slate_keys,\"aw\"\n\t.quad {0} + {1}\n\t.popsection", sym k2, const 0 as u8, label {
// CHECK-NEXT: __slate_state = 0usize;
// CHECK-NEXT: }, options(nostack, att_syntax));
// CHECK-NEXT:                     continue;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 _ => {
// CHECK-NEXT:                     unreachable!();
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { core::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn branch__const8(key: *mut key, likely: bool) -> i32 {
// CHECK-NEXT:     let mut __slate_state: usize = 2usize;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             match __slate_state {
// CHECK-NEXT:                 0 => {
// CHECK-NEXT:                     return 1;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 1 => {
// CHECK-NEXT:                     return 0;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 2 => {
// CHECK-NEXT:                     __slate_state = 1usize;
// CHECK-NEXT:                     core::arch::asm!("jmp {2}\n\t.pushsection .data.slate_keys,\"aw\"\n\t.quad {0} + {1}\n\t.popsection", sym holder, const 1 as u8, label {
// CHECK-NEXT: __slate_state = 0usize;
// CHECK-NEXT: }, options(nostack, att_syntax));
// CHECK-NEXT:                     continue;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 _ => {
// CHECK-NEXT:                     unreachable!();
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { core::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn branch__const9(key: *mut key, likely: bool) -> i32 {
// CHECK-NEXT:     let mut __slate_state: usize = 2usize;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             match __slate_state {
// CHECK-NEXT:                 0 => {
// CHECK-NEXT:                     return 1;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 1 => {
// CHECK-NEXT:                     return 0;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 2 => {
// CHECK-NEXT:                     __slate_state = 1usize;
// CHECK-NEXT:                     core::arch::asm!("jmp {2}\n\t.pushsection .data.slate_keys,\"aw\"\n\t.quad {0} + {1}\n\t.popsection", sym k2, const 0 as u8, label {
// CHECK-NEXT: __slate_state = 0usize;
// CHECK-NEXT: }, options(nostack, att_syntax));
// CHECK-NEXT:                     continue;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 _ => {
// CHECK-NEXT:                     unreachable!();
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { core::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bit__const6(nr: i32) -> i32 {
// CHECK-NEXT:     let mut out: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!("movl ${1}, {0:e}", lateout(reg) [[T_0]], const 2 as i32, options(pure, nomem, nostack, att_syntax));
// CHECK-NEXT:         out = unsafe { core::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     out
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bit__const7(nr: i32) -> i32 {
// CHECK-NEXT:     let mut out: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!("movl ${1}, {0:e}", lateout(reg) [[T_0]], const 8 as i32, options(pure, nomem, nostack, att_syntax));
// CHECK-NEXT:         out = unsafe { core::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     out
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn outer__const3(key: *mut key) -> *const i8 {
// CHECK-NEXT:     key_address__const2(key, false)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct holder {
// CHECK-NEXT:     key_field: key,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut holder: holder = holder {
// CHECK-NEXT:     key_field: key { enabled: 3i32 },
// CHECK-NEXT: };
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%td %td %td\n".as_ptr(),
// CHECK-NEXT:             key_address__const1(core::ptr::addr_of_mut!(k1), true)
// CHECK-NEXT:                 .offset_from(core::ptr::addr_of_mut!(k1) as *const i8) as i64,
// CHECK-NEXT:             key_address__const2(core::ptr::addr_of_mut!(k2), false)
// CHECK-NEXT:                 .offset_from(core::ptr::addr_of_mut!(k2) as *const i8) as i64,
// CHECK-NEXT:             outer__const3(core::ptr::addr_of_mut!(k2))
// CHECK-NEXT:                 .offset_from(core::ptr::addr_of_mut!(k2) as *const i8) as i64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}",
// CHECK-NEXT:         branch__const4(unsafe { core::ptr::addr_of_mut!(k1) }, true),
// CHECK-NEXT:         branch__const5(unsafe { core::ptr::addr_of_mut!(k2) }, false),
// CHECK-NEXT:         branch__const4(unsafe { core::ptr::addr_of_mut!(k1) }, true)
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{} {}", bit__const6(9i32), bit__const7(3i32));
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: i32;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __out: i32 = 0;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             let [[T_0]]: u32;
// CHECK-NEXT:             core::arch::asm!("movl ${1}, {0:e}", lateout(reg) [[T_0]], const 259 as i32, options(pure, nomem, nostack, att_syntax));
// CHECK-NEXT:             __out = unsafe { core::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:         }
// CHECK-NEXT:         [[V_0]] = __out;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i32;
// CHECK-NEXT:     let mut __out: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_1:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!("movl ${1}, {0:e}", lateout(reg) [[T_1]], const 272 as i32, options(pure, nomem, nostack, att_syntax));
// CHECK-NEXT:         __out = unsafe { core::mem::transmute::<u32, i32>([[T_1]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     [[V_1]] = __out;
// CHECK-NEXT:     println!("{} {}", [[V_0]], [[V_1]]);
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {}",
// CHECK-NEXT:         HOLDER_BRANCH!(unsafe { core::ptr::addr_of_mut!(holder) }),
// CHECK-NEXT:         KEY_BRANCH!(unsafe { core::ptr::addr_of_mut!(k2) })
// CHECK-NEXT:     );
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
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
