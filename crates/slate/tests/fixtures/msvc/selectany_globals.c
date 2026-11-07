#ifdef _WIN32
void abort(void);

typedef struct {
    unsigned long Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char Data4[8];
} GUID;

typedef struct {
    char tag;
    double scale;
    short pair[2];
} Mixed;

typedef union {
    unsigned char bytes[4];
    unsigned int word;
} Word;

extern __declspec(selectany) int counter = 30;
__declspec(selectany) const GUID clsid = {0x3c374a40, 0xbae4, 0x11cf, {0xbf, 0x7d, 0x00, 0xaa, 0x00, 0x69, 0x46, 0xee}};
extern __declspec(selectany) const unsigned int max_buffers = 8;
__declspec(selectany) Mixed mixed = {'m', -2.5, {-7, 300}};
__declspec(selectany) Word word = {{1, 2, 3, 4}};
__declspec(selectany) __declspec(align(32)) long long aligned = -1;
__declspec(selectany) int partial[6] = {1, 2};
__declspec(selectany) float ratio = 0.75f;
__declspec(selectany) unsigned char zeroed[3];
__declspec(selectany) const char banner[] = "slate";

static int bump(int *value) { return *value += 12; }

int main(void) {
    const unsigned char *raw = (const unsigned char *)&clsid;
    if (clsid.Data1 != 0x3c374a40 || clsid.Data2 != 0xbae4 || clsid.Data3 != 0x11cf || clsid.Data4[7] != 0xee)
        abort();
    if (raw[0] != 0x40 || raw[4] != 0xe4 || raw[15] != 0xee || sizeof clsid != 16)
        abort();
    if (bump(&counter) != 42 || counter != 42 || max_buffers != 8)
        abort();
    if (mixed.tag != 'm' || mixed.scale != -2.5 || mixed.pair[0] != -7 || mixed.pair[1] != 300)
        abort();
    mixed.scale *= 2;
    if (mixed.scale != -5.0)
        abort();
    if (word.word != 0x04030201)
        abort();
    if (((unsigned long long)&aligned & 31) != 0 || aligned != -1)
        abort();
    if (partial[0] != 1 || partial[1] != 2 || partial[5] != 0)
        abort();
    partial[5] = 9;
    if (partial[5] != 9 || ratio != 0.75f || zeroed[0] | zeroed[1] | zeroed[2])
        abort();
    if (sizeof banner != 6 || banner[0] != 's' || banner[4] != 'e' || banner[5] != 0)
        abort();
    return 0;
}

#else
int main(void) { return 0; }
#endif

/* { dg-options "/Zc:preprocessor" } */

// SLATE-FILECHECK-BEGIN x86_64-pc-windows-msvc i686-pc-windows-msvc aarch64-pc-windows-msvc
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     static mut counter: i32;
// CHECK-NEXT:     static mut clsid: __SlateRecord_selectany_globals_c_9;
// CHECK-NEXT:     static mut max_buffers: u32;
// CHECK-NEXT:     static mut mixed: __SlateRecord_selectany_globals_c_15;
// CHECK-NEXT:     static mut word: __SlateRecord_selectany_globals_c_20;
// CHECK-NEXT:     static mut aligned: i64;
// CHECK-NEXT:     static mut partial: [i32; 6];
// CHECK-NEXT:     static mut ratio: f32;
// CHECK-NEXT:     static mut zeroed: [u8; 3];
// CHECK-NEXT:     static mut banner: [i8; 6];
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __SlateRecord_selectany_globals_c_9 {
// CHECK-NEXT:     Data1: u32,
// CHECK-NEXT:     Data2: u16,
// CHECK-NEXT:     Data3: u16,
// CHECK-NEXT:     Data4: [u8; 8],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __SlateRecord_selectany_globals_c_15 {
// CHECK-NEXT:     tag: i8,
// CHECK-NEXT:     scale: f64,
// CHECK-NEXT:     pair: [i16; 2],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union __SlateRecord_selectany_globals_c_20 {
// CHECK-NEXT:     bytes: [u8; 4],
// CHECK-NEXT:     word: u32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// X86_64-PC-WINDOWS-MSVC-NEXT: core::arch::global_asm!(".section .data$counter,\"dw\",discard,counter\n.globl counter\n.p2align 2\ncounter:\n.long {}\n.text", const 30 as i32);
// I686-PC-WINDOWS-MSVC-NEXT: core::arch::global_asm!(".section .data$_counter,\"dw\",discard,_counter\n.globl _counter\n.p2align 2\n_counter:\n.long {}\n.text", const 30 as i32);
// AARCH64-PC-WINDOWS-MSVC-NEXT: core::arch::global_asm!(".section .data$counter,\"dw\",discard,counter\n.globl counter\n.p2align 2\ncounter:\n.long {}\n.text", const 30 as i32);
// CHECK-EMPTY:
// X86_64-PC-WINDOWS-MSVC-NEXT: core::arch::global_asm!(".section .rdata$clsid,\"dr\",discard,clsid\n.globl clsid\n.p2align 2\nclsid:\n.long {}\n.short {}\n.short {}\n.byte {}\n.byte {}\n.byte {}\n.byte {}\n.byte {}\n.byte {}\n.byte {}\n.byte {}\n.text", const (1010256448 as i32) as u32, const (47844 as i32) as u16, const (4559 as i32) as u16, const (191 as i32) as u8, const (125 as i32) as u8, const (0 as i32) as u8, const (170 as i32) as u8, const (0 as i32) as u8, const (105 as i32) as u8, const (70 as i32) as u8, const (238 as i32) as u8);
// I686-PC-WINDOWS-MSVC-NEXT: core::arch::global_asm!(".section .rdata$_clsid,\"dr\",discard,_clsid\n.globl _clsid\n.p2align 2\n_clsid:\n.long {}\n.short {}\n.short {}\n.byte {}\n.byte {}\n.byte {}\n.byte {}\n.byte {}\n.byte {}\n.byte {}\n.byte {}\n.text", const (1010256448 as i32) as u32, const (47844 as i32) as u16, const (4559 as i32) as u16, const (191 as i32) as u8, const (125 as i32) as u8, const (0 as i32) as u8, const (170 as i32) as u8, const (0 as i32) as u8, const (105 as i32) as u8, const (70 as i32) as u8, const (238 as i32) as u8);
// AARCH64-PC-WINDOWS-MSVC-NEXT: core::arch::global_asm!(".section .rdata$clsid,\"dr\",discard,clsid\n.globl clsid\n.p2align 2\nclsid:\n.long {}\n.short {}\n.short {}\n.byte {}\n.byte {}\n.byte {}\n.byte {}\n.byte {}\n.byte {}\n.byte {}\n.byte {}\n.text", const (1010256448 as i32) as u32, const (47844 as i32) as u16, const (4559 as i32) as u16, const (191 as i32) as u8, const (125 as i32) as u8, const (0 as i32) as u8, const (170 as i32) as u8, const (0 as i32) as u8, const (105 as i32) as u8, const (70 as i32) as u8, const (238 as i32) as u8);
// CHECK-EMPTY:
// X86_64-PC-WINDOWS-MSVC-NEXT: core::arch::global_asm!(".section .rdata$max_buffers,\"dr\",discard,max_buffers\n.globl max_buffers\n.p2align 2\nmax_buffers:\n.long {}\n.text", const (8 as i32) as u32);
// I686-PC-WINDOWS-MSVC-NEXT: core::arch::global_asm!(".section .rdata$_max_buffers,\"dr\",discard,_max_buffers\n.globl _max_buffers\n.p2align 2\n_max_buffers:\n.long {}\n.text", const (8 as i32) as u32);
// AARCH64-PC-WINDOWS-MSVC-NEXT: core::arch::global_asm!(".section .rdata$max_buffers,\"dr\",discard,max_buffers\n.globl max_buffers\n.p2align 2\nmax_buffers:\n.long {}\n.text", const (8 as i32) as u32);
// CHECK-EMPTY:
// X86_64-PC-WINDOWS-MSVC-NEXT: core::arch::global_asm!(".section .data$mixed,\"dw\",discard,mixed\n.globl mixed\n.p2align 3\nmixed:\n.byte {}\n.zero 7\n.quad {}\n.short {}\n.short {}\n.zero 4\n.text", const (109 as i32) as i8, const (-2.5f64).to_bits(), const -(7 as i32) as i16, const (300 as i32) as i16);
// I686-PC-WINDOWS-MSVC-NEXT: core::arch::global_asm!(".section .data$_mixed,\"dw\",discard,_mixed\n.globl _mixed\n.p2align 3\n_mixed:\n.byte {}\n.zero 7\n.quad {}\n.short {}\n.short {}\n.zero 4\n.text", const (109 as i32) as i8, const (-2.5f64).to_bits(), const -(7 as i32) as i16, const (300 as i32) as i16);
// AARCH64-PC-WINDOWS-MSVC-NEXT: core::arch::global_asm!(".section .data$mixed,\"dw\",discard,mixed\n.globl mixed\n.p2align 3\nmixed:\n.byte {}\n.zero 7\n.quad {}\n.short {}\n.short {}\n.zero 4\n.text", const (109 as i32) as i8, const (-2.5f64).to_bits(), const -(7 as i32) as i16, const (300 as i32) as i16);
// CHECK-EMPTY:
// X86_64-PC-WINDOWS-MSVC-NEXT: core::arch::global_asm!(".section .data$word,\"dw\",discard,word\n.globl word\n.p2align 2\nword:\n.byte {}\n.byte {}\n.byte {}\n.byte {}\n.text", const (1 as i32) as u8, const (2 as i32) as u8, const (3 as i32) as u8, const (4 as i32) as u8);
// I686-PC-WINDOWS-MSVC-NEXT: core::arch::global_asm!(".section .data$_word,\"dw\",discard,_word\n.globl _word\n.p2align 2\n_word:\n.byte {}\n.byte {}\n.byte {}\n.byte {}\n.text", const (1 as i32) as u8, const (2 as i32) as u8, const (3 as i32) as u8, const (4 as i32) as u8);
// AARCH64-PC-WINDOWS-MSVC-NEXT: core::arch::global_asm!(".section .data$word,\"dw\",discard,word\n.globl word\n.p2align 2\nword:\n.byte {}\n.byte {}\n.byte {}\n.byte {}\n.text", const (1 as i32) as u8, const (2 as i32) as u8, const (3 as i32) as u8, const (4 as i32) as u8);
// CHECK-EMPTY:
// X86_64-PC-WINDOWS-MSVC-NEXT: core::arch::global_asm!(".section .data$aligned,\"dw\",discard,aligned\n.globl aligned\n.p2align 5\naligned:\n.quad {}\n.text", const -(1 as i32) as i64);
// I686-PC-WINDOWS-MSVC-NEXT: core::arch::global_asm!(".section .data$_aligned,\"dw\",discard,_aligned\n.globl _aligned\n.p2align 5\n_aligned:\n.quad {}\n.text", const -(1 as i32) as i64);
// AARCH64-PC-WINDOWS-MSVC-NEXT: core::arch::global_asm!(".section .data$aligned,\"dw\",discard,aligned\n.globl aligned\n.p2align 5\naligned:\n.quad {}\n.text", const -(1 as i32) as i64);
// CHECK-EMPTY:
// X86_64-PC-WINDOWS-MSVC-NEXT: core::arch::global_asm!(".section .data$partial,\"dw\",discard,partial\n.globl partial\n.p2align 4\npartial:\n.long {}\n.long {}\n.zero 16\n.text", const 1 as i32, const 2 as i32);
// I686-PC-WINDOWS-MSVC-NEXT: core::arch::global_asm!(".section .data$_partial,\"dw\",discard,_partial\n.globl _partial\n.p2align 2\n_partial:\n.long {}\n.long {}\n.zero 16\n.text", const 1 as i32, const 2 as i32);
// AARCH64-PC-WINDOWS-MSVC-NEXT: core::arch::global_asm!(".section .data$partial,\"dw\",discard,partial\n.globl partial\n.p2align 2\npartial:\n.long {}\n.long {}\n.zero 16\n.text", const 1 as i32, const 2 as i32);
// CHECK-EMPTY:
// X86_64-PC-WINDOWS-MSVC-NEXT: core::arch::global_asm!(".section .data$ratio,\"dw\",discard,ratio\n.globl ratio\n.p2align 2\nratio:\n.long {}\n.text", const 0.75f32.to_bits());
// I686-PC-WINDOWS-MSVC-NEXT: core::arch::global_asm!(".section .data$_ratio,\"dw\",discard,_ratio\n.globl _ratio\n.p2align 2\n_ratio:\n.long {}\n.text", const 0.75f32.to_bits());
// AARCH64-PC-WINDOWS-MSVC-NEXT: core::arch::global_asm!(".section .data$ratio,\"dw\",discard,ratio\n.globl ratio\n.p2align 2\nratio:\n.long {}\n.text", const 0.75f32.to_bits());
// CHECK-EMPTY:
// CHECK-NEXT: core::arch::global_asm!(
// X86_64-PC-WINDOWS-MSVC-NEXT:     ".section .data$zeroed,\"dw\",discard,zeroed\n.globl zeroed\n.p2align 0\nzeroed:\n.zero 3\n.text"
// I686-PC-WINDOWS-MSVC-NEXT:     ".section .data$_zeroed,\"dw\",discard,_zeroed\n.globl _zeroed\n.p2align 0\n_zeroed:\n.zero 3\n.text"
// AARCH64-PC-WINDOWS-MSVC-NEXT:     ".section .data$zeroed,\"dw\",discard,zeroed\n.globl zeroed\n.p2align 0\nzeroed:\n.zero 3\n.text"
// CHECK-NEXT: );
// CHECK-EMPTY:
// CHECK-NEXT: core::arch::global_asm!(
// X86_64-PC-WINDOWS-MSVC-NEXT:     ".section .rdata$banner,\"dr\",discard,banner\n.globl banner\n.p2align 0\nbanner:\n.byte 115\n.byte 108\n.byte 97\n.byte 116\n.byte 101\n.byte 0\n.text"
// I686-PC-WINDOWS-MSVC-NEXT:     ".section .rdata$_banner,\"dr\",discard,_banner\n.globl _banner\n.p2align 0\n_banner:\n.byte 115\n.byte 108\n.byte 97\n.byte 116\n.byte 101\n.byte 0\n.text"
// AARCH64-PC-WINDOWS-MSVC-NEXT:     ".section .rdata$banner,\"dr\",discard,banner\n.globl banner\n.p2align 0\nbanner:\n.byte 115\n.byte 108\n.byte 97\n.byte 116\n.byte 101\n.byte 0\n.text"
// CHECK-NEXT: );
// CHECK-EMPTY:
// CHECK-NEXT: fn bump([[V_0:__v[0-9]+]]: *mut i32) -> i32 {
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: i32 = (unsafe { *[[V_0]] }) + 12;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[V_0]] = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     [[V_1]]
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let raw: *const u8 = (unsafe { std::ptr::addr_of!(clsid) }) as *const u8;
// CHECK-NEXT:     if (unsafe { clsid.Data1 }) != 1010256448
// CHECK-NEXT:         || ((unsafe { clsid.Data2 }) as i32) != 47844
// CHECK-NEXT:         || ((unsafe { clsid.Data3 }) as i32) != 4559
// CHECK-NEXT:         || ((unsafe { clsid.Data4[7] }) as i32) != 238
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((unsafe { *raw.add(0) }) as i32) != 64
// CHECK-NEXT:         || ((unsafe { *raw.add(4) }) as i32) != 228
// CHECK-NEXT:         || ((unsafe { *raw.add(15) }) as i32) != 238
// X86_64-PC-WINDOWS-MSVC-NEXT:         || (std::mem::size_of::<__SlateRecord_selectany_globals_c_9>() as u64) != 16
// I686-PC-WINDOWS-MSVC-NEXT:         || (std::mem::size_of::<__SlateRecord_selectany_globals_c_9>() as u32) != 16
// AARCH64-PC-WINDOWS-MSVC-NEXT:         || (std::mem::size_of::<__SlateRecord_selectany_globals_c_9>() as u64) != 16
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if bump(unsafe { std::ptr::addr_of_mut!(counter) }) != 42
// CHECK-NEXT:         || (unsafe { counter }) != 42
// CHECK-NEXT:         || (unsafe { max_buffers }) != 8
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((unsafe { mixed.tag }) as i32) != 109
// CHECK-NEXT:         || (unsafe { mixed.scale }) != -2.5f64
// CHECK-NEXT:         || ((unsafe { mixed.pair[0] }) as i32) != -7
// CHECK-NEXT:         || ((unsafe { mixed.pair[1] }) as i32) != 300
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         mixed.scale = mixed.scale * (2i32 as f64);
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { mixed.scale }) != -5.0f64 {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { word.word }) != 67305985 {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((unsafe { std::ptr::addr_of_mut!(aligned) }) as u64) & 31 != 0 || (unsafe { aligned }) != -1
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { partial[0] }) != 1 || (unsafe { partial[1] }) != 2 || (unsafe { partial[5] }) != 0
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         partial[5] = 9;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { partial[5] }) != 9
// CHECK-NEXT:         || (unsafe { ratio }) != 0.75f32
// CHECK-NEXT:         || ((unsafe { zeroed[0] }) as i32)
// CHECK-NEXT:             | ((unsafe { zeroed[1] }) as i32)
// CHECK-NEXT:             | ((unsafe { zeroed[2] }) as i32)
// CHECK-NEXT:             != 0
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// X86_64-PC-WINDOWS-MSVC-NEXT:     if (std::mem::size_of::<[i8; 6]>() as u64) != 6
// I686-PC-WINDOWS-MSVC-NEXT:     if (std::mem::size_of::<[i8; 6]>() as u32) != 6
// AARCH64-PC-WINDOWS-MSVC-NEXT:     if (std::mem::size_of::<[i8; 6]>() as u64) != 6
// CHECK-NEXT:         || ((unsafe { banner[0] }) as i32) != 115
// CHECK-NEXT:         || ((unsafe { banner[4] }) as i32) != 101
// CHECK-NEXT:         || ((unsafe { banner[5] }) as i32) != 0
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-options "/Zc:preprocessor" }
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
