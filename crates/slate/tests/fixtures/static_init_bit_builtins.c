int printf(const char *, ...);

#define cpu_to_be32(x) ((unsigned int)__builtin_bswap32((unsigned int)(x)))
#define cpu_to_be16(x) ((unsigned short)__builtin_bswap16((unsigned short)(x)))
#define bf_shf(mask) (__builtin_ffsll(mask) - 1)
#define FIELD_PREP(mask, value) (((unsigned int)(value) << bf_shf(mask)) & (mask))

struct packet_type {
  unsigned short type;
  int priority;
};

struct pll_state {
  unsigned int ctrl[2];
};

static unsigned int ic_myaddr = cpu_to_be32(0xc0a80001u);
static struct packet_type ip_packet = {.type = cpu_to_be16(0x0800), .priority = 1};
static const struct pll_state pll = {
    .ctrl = {FIELD_PREP(0xff00u, 0x12) | FIELD_PREP(0x000fu, 3), FIELD_PREP(0xf0000000u, 5)}};
static const int widths[] = {__builtin_clz(1u), __builtin_ctzll(1ull << 40), __builtin_popcountl(0xf0f0ul),
                             __builtin_parity(7u), __builtin_clrsb(-1)};

static int counter(void) {
  static int base = __builtin_ffs(0x100);
  return base++;
}

int main(void) {
  printf("%08x %04x %d\n", ic_myaddr, ip_packet.type, ip_packet.priority);
  printf("%08x %08x\n", pll.ctrl[0], pll.ctrl[1]);
  for (int i = 0; i < 5; i++)
    printf("%d ", widths[i]);
  int first = counter();
  printf("\n%d %d\n", first, counter());
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct packet_type {
// CHECK-NEXT:     r#type: u16,
// CHECK-NEXT:     priority: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct pll_state {
// CHECK-NEXT:     ctrl: [u32; 2],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: unsafe impl<T> Sync for __SlateAlign16<T> {}
// CHECK-EMPTY:
// CHECK-NEXT: static mut ic_myaddr: u32 = 16820416;
// CHECK-EMPTY:
// CHECK-NEXT: static mut ip_packet: packet_type = {
// CHECK-NEXT:     let mut [[T_0:__t[0-9]+]]: packet_type = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].r#type = 8u16;
// CHECK-NEXT:     [[T_0]].priority = 1i32;
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-EMPTY:
// CHECK-NEXT: static pll: pll_state = pll_state {
// CHECK-NEXT:     ctrl: [4611u32, 1342177280u32],
// CHECK-NEXT: };
// CHECK-EMPTY:
// CHECK-NEXT: static widths: __SlateAlign16<[i32; 5]> = __SlateAlign16(
// CHECK-NEXT:     [31i32, 40i32, 8i32, 1i32, 31i32],
// CHECK-NEXT: );
// CHECK-EMPTY:
// CHECK-NEXT: fn counter() -> i32 {
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: i32 = unsafe { base };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         base = [[V_0]] + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     [[V_0]]
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut base: i32 = 9;
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%08x %04x %d\n".as_ptr(),
// CHECK-NEXT:             ic_myaddr,
// CHECK-NEXT:             ip_packet.r#type as i32,
// CHECK-NEXT:             ip_packet.priority,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { printf(c"%08x %08x\n".as_ptr(), pll.ctrl[0], pll.ctrl[1]) };
// CHECK-NEXT:     for i in 0i32..5i32 {
// CHECK-NEXT:         unsafe { printf(c"%d ".as_ptr(), widths.0[i as usize]) };
// CHECK-NEXT:     }
// CHECK-NEXT:     let first: i32 = counter();
// CHECK-NEXT:     unsafe { printf(c"\n%d %d\n".as_ptr(), first, counter()) };
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
