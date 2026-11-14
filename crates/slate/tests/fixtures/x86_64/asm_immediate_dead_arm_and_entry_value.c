// { dg-options "-O2" }
#include <stdio.h>

struct key {
    int enabled;
};

struct key keys[2][3] = {{{1}, {2}, {3}}, {{4}, {5}, {6}}};
unsigned long word = 0x28;
struct page {
    char data[64];
} page;
static char table[32];

static inline __attribute__((always_inline)) int branch(struct key *key) {
    __asm__ goto("jmp %l[yes]\n\t# %c0" : : "i"(key) : : yes);
    return 0;
yes:
    return 1;
}

static inline int hook(unsigned char pf, unsigned int nr) {
    if (__builtin_constant_p(pf) && __builtin_constant_p(nr) && !branch(&keys[pf][nr]))
        return 1;
    return keys[pf][nr].enabled + 10;
}

static inline __attribute__((always_inline)) int constant_bit(long nr) {
    int out;
    __asm__("movl %1, %0" : "=r"(out) : "i"(1 << (nr & 7)));
    return (word & (unsigned long)out) != 0;
}

static inline __attribute__((always_inline)) int test_bit(long nr) {
    return __builtin_constant_p(nr) ? constant_bit(nr) : (int)((word >> nr) & 1);
}

static inline __attribute__((always_inline)) void *rip_rel(void *p) {
    __asm__("leaq %c1(%%rip), %0" : "=r"(p) : "i"(p));
    return p;
}

static inline __attribute__((always_inline)) char *rip_rel_after(void *p) {
    void *original = p;
    __asm__("leaq %c1(%%rip), %0" : "=r"(p) : "i"(p));
    return (char *)p + (original == p);
}

void handler(void) {}

int main(int argc, char **argv) {
    void *h = 0;
    if (0)
        h = rip_rel(handler);
    printf("%d %d\n", hook(argc, 1), hook(1, 2));
    printf("%d %d %d\n", test_bit(argc + 2), test_bit(3), test_bit(4));
    printf("%d %d\n", rip_rel((void *)(unsigned long)&page) == (void *)&page, rip_rel(table + 3) == table + 3);
    printf("%td %d\n", rip_rel_after(table + 1) - table, h == 0);
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-O2" }
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
// CHECK-NEXT: static mut keys: __SlateAlign16<{{\[\[}}key; 3]; 2]> = __SlateAlign16([
// CHECK-NEXT:     [
// CHECK-NEXT:         key { enabled: 1i32 },
// CHECK-NEXT:         key { enabled: 2i32 },
// CHECK-NEXT:         key { enabled: 3i32 },
// CHECK-NEXT:     ],
// CHECK-NEXT:     [
// CHECK-NEXT:         key { enabled: 4i32 },
// CHECK-NEXT:         key { enabled: 5i32 },
// CHECK-NEXT:         key { enabled: 6i32 },
// CHECK-NEXT:     ],
// CHECK-NEXT: ]);
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut word: u64 = 40;
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct page {
// CHECK-NEXT:     data: [i8; 64],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut page: page = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut table: __SlateAlign16<[i8; 32]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: fn hook(pf: u8, nr: u32) -> i32 {
// CHECK-NEXT:     (unsafe { keys.0[(pf as i32) as usize][nr as usize].enabled }) + 10
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test_bit(nr: i64) -> i32 {
// CHECK-NEXT:     ((unsafe { word }) >> nr & 1) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn rip_rel__const1(mut p: *mut ()) -> *mut () {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u64;
// CHECK-NEXT:         core::arch::asm!("leaq {1}(%rip), {0:r}", lateout(reg) [[T_0]], sym page, options(pure, nomem, nostack, att_syntax));
// CHECK-NEXT:         p = unsafe { std::mem::transmute::<u64, *mut ()>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     p
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn rip_rel__const2(mut p: *mut ()) -> *mut () {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u64;
// CHECK-NEXT:         core::arch::asm!("leaq {1}+3(%rip), {0:r}", lateout(reg) [[T_0]], sym table, options(pure, nomem, nostack, att_syntax));
// CHECK-NEXT:         p = unsafe { std::mem::transmute::<u64, *mut ()>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     p
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn rip_rel_after__const3(mut p: *mut ()) -> *mut i8 {
// CHECK-NEXT:     let original: *mut () = p;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u64;
// CHECK-NEXT:         core::arch::asm!("leaq {1}+1(%rip), {0:r}", lateout(reg) [[T_0]], sym table, options(pure, nomem, nostack, att_syntax));
// CHECK-NEXT:         p = unsafe { std::mem::transmute::<u64, *mut ()>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { (p as *mut i8).offset(((original == p) as i32) as isize) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn handler() {}
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main(argc: i32, argv: *mut *mut i8) -> i32 {
// CHECK-NEXT:     let h: *mut () = std::ptr::null_mut::<()>();
// CHECK-NEXT:     println!("{} {}", hook(argc as u8, 1u32), hook(1u8, 2u32));
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}",
// CHECK-NEXT:         test_bit((argc + 2) as i64),
// CHECK-NEXT:         test_bit(3i64),
// CHECK-NEXT:         test_bit(4i64)
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {}",
// CHECK-NEXT:         (rip_rel__const1(((unsafe { std::ptr::addr_of_mut!(page) }) as u64) as *mut ())
// CHECK-NEXT:             == ((unsafe { std::ptr::addr_of_mut!(page) }) as *mut ())) as i32,
// CHECK-NEXT:         (rip_rel__const2(
// CHECK-NEXT:             (unsafe { (std::ptr::addr_of_mut!(table.0) as *mut i8).add(3) }) as *mut ()
// CHECK-NEXT:         ) == ((unsafe { (std::ptr::addr_of_mut!(table.0) as *mut i8).add(3) }) as *mut ()))
// CHECK-NEXT:             as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%td %d\n".as_ptr(),
// CHECK-NEXT:             rip_rel_after__const3((std::ptr::addr_of_mut!(table.0) as *mut i8).add(1) as *mut ())
// CHECK-NEXT:                 .offset_from((std::ptr::addr_of_mut!(table.0) as *mut i8) as *mut i8)
// CHECK-NEXT:                 as i64,
// CHECK-NEXT:             h.is_null() as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main(argc as _, argv as _) }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
