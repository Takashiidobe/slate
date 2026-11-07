#include <float.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "dylink.h"
#include "dylink_path.h"
#include "long_double_abi/ld_abi.h"

enum { LD_VALUE_BYTES = LDBL_MANT_DIG == 64 ? 10 : sizeof(long double) };

static void dump(const char *name, long double value) {
  unsigned char bytes[sizeof(long double)];
  memcpy(bytes, &value, sizeof value);
  printf("%s ", name);
  for (int i = LD_VALUE_BYTES; i-- > 0;) printf("%02x", bytes[i]);
  printf("\n");
}

static long double fused(long double a, long double b) { return a * b + a; }

#ifndef _WIN32
static long double scaled_sum(long double scale, int count, ...) {
  va_list ap;
  va_start(ap, count);
  long double total = scale;
  for (int i = 0; i < count; i++) total += va_arg(ap, long double) * scale;
  va_end(ap);
  return total;
}
#endif

// @check-fn-begin
int main(void) {
  void *lib = dl_open(DEP_PATH);
  if (!lib) {
    printf("open failed\n");
    return 1;
  }
  long double (*identity)(long double) = (long double (*)(long double))dl_sym(lib, "ld_identity");
  long double (*sum10)(long double, long double, long double, long double, long double,
                       long double, long double, long double, long double, long double) =
      (long double (*)(long double, long double, long double, long double, long double,
                       long double, long double, long double, long double,
                       long double))dl_sym(lib, "ld_sum10");
  long double (*mix)(int64_t, double, long double, uint32_t, long double, float, long double,
                     int, double, long double) =
      (long double (*)(int64_t, double, long double, uint32_t, long double, float, long double,
                       int, double, long double))dl_sym(lib, "ld_mix");
  struct ld_pair (*pair_swap)(struct ld_pair) =
      (struct ld_pair (*)(struct ld_pair))dl_sym(lib, "ld_pair_swap");
  struct ld_one (*one_half)(struct ld_one) =
      (struct ld_one (*)(struct ld_one))dl_sym(lib, "ld_one_half");
  struct ld_box (*box_bump)(struct ld_box) =
      (struct ld_box (*)(struct ld_box))dl_sym(lib, "ld_box_bump");
  long double (*call)(ld_binary_fn, long double, long double) =
      (long double (*)(ld_binary_fn, long double, long double))dl_sym(lib, "ld_call");
  long double (*vsum)(long double, int, ...) =
      (long double (*)(long double, int, ...))dl_sym(lib, "ld_vsum");
  size_t (*box_layout)(size_t *, size_t *) =
      (size_t (*)(size_t *, size_t *))dl_sym(lib, "ld_box_layout");
  long double (*vcall)(ld_variadic_fn, long double, long double) =
      (long double (*)(ld_variadic_fn, long double, long double))dl_sym(lib, "ld_vcall");
  if (!identity || !sum10 || !mix || !pair_swap || !one_half || !box_bump || !call || !vsum ||
      !vcall || !box_layout) {
    printf("resolve failed\n");
    return 2;
  }

  long double a = 0x1.0000000000000001p0L;
  long double b = 0x1.0000000000000002p0L;

  dump("identity", identity(0x1.123456789abcdef012345678p+40L));
  dump("sum10", sum10(0x1p+0L, 0x1p-1L, 0x1p-2L, 0x1p-3L, 0x1p-4L, 0x1p-5L, 0x1p-6L, 0x1p-7L,
                      0x1p-8L, 0x1p-65L));
  dump("mix", mix(-123456789, 0x1.8p+7, 0x1.123456789abcdefp+5L, 12345u, -0x1.987654321p-3L,
                  0.75f, 0x1.01p+9L, -77, -0.125, 0x1.ffffp-11L));

  struct ld_pair p = pair_swap((struct ld_pair){a, b});
  dump("pair.a", p.a);
  dump("pair.b", p.b);

  dump("one", one_half((struct ld_one){b}).v);

  struct ld_box box = box_bump((struct ld_box){0x21, a, 1000u});
  printf("box.tag %02x\n", box.tag);
  dump("box.x", box.x);
  printf("box.tail %u\n", (unsigned)box.tail);

  dump("call", call(fused, a, b));

  dump("vsum", vsum(0x1.8p0L, 3, a, 7.5L, b));
#ifndef _WIN32
  dump("vcall", vcall(scaled_sum, a, b));
#endif

  size_t x = 0;
  size_t tail = 0;
  size_t size = box_layout(&x, &tail);
  printf("layout %s\n", size == sizeof(struct ld_box) && x == offsetof(struct ld_box, x) &&
                                tail == offsetof(struct ld_box, tail)
                            ? "match"
                            : "mismatch");

  dl_close(lib);
  return 0;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu aarch64-unknown-linux-gnu x86_64-pc-windows-msvc
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: ld_pair = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_1:__slate_compound_[0-9]+]]: ld_one = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_2:__slate_compound_[0-9]+]]: ld_one = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_3:__slate_compound_[0-9]+]]: ld_box = unsafe { std::mem::zeroed() };
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     let lib: *mut () = dl_open!(c"./libdep.so".as_ptr() as *mut i8);
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     let lib: *mut () = dl_open!(c"./libdep.so".as_ptr() as *mut u8);
// X86_64-PC-WINDOWS-MSVC-NEXT:     let lib: *mut () = dl_open!(c".\\dep.dll".as_ptr() as *mut i8) as *mut ();
// CHECK-NEXT:     if lib.is_null() {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         println!("open failed");
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         println!("open failed");
// X86_64-PC-WINDOWS-MSVC-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe { slate_support::printf(c"open failed\n".as_ptr()) };
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         return 1;
// CHECK-NEXT:     }
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     let identity: Option<unsafe extern "C-unwind" fn(LongDouble) -> LongDouble> = unsafe {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         std::mem::transmute::<*mut (), Option<unsafe extern "C-unwind" fn(LongDouble) -> LongDouble>>(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             dl_sym!(lib, c"ld_identity".as_ptr() as *mut i8),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         )
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     let identity: Option<unsafe extern "C-unwind" fn(f128) -> f128> = unsafe {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         std::mem::transmute::<*mut (), Option<unsafe extern "C-unwind" fn(f128) -> f128>>(dl_sym!(
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             lib,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             c"ld_identity".as_ptr() as *mut u8
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         ))
// X86_64-PC-WINDOWS-MSVC-NEXT:     let identity: Option<unsafe extern "C-unwind" fn(f64) -> f64> = unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         std::mem::transmute::<
// X86_64-PC-WINDOWS-MSVC-NEXT:             Option<unsafe extern "C-unwind" fn() -> i64>,
// X86_64-PC-WINDOWS-MSVC-NEXT:             Option<unsafe extern "C-unwind" fn(f64) -> f64>,
// X86_64-PC-WINDOWS-MSVC-NEXT:         >(dl_sym!(lib, c"ld_identity".as_ptr() as *mut i8))
// CHECK-NEXT:     };
// CHECK-NEXT:     let sum10: Option<
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         unsafe extern "C-unwind" fn(
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         unsafe extern "C-unwind" fn(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             LongDouble,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             LongDouble,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             LongDouble,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             LongDouble,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             LongDouble,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             LongDouble,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             LongDouble,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             LongDouble,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             LongDouble,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             LongDouble,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         ) -> LongDouble,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         ) -> f128,
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe extern "C-unwind" fn(f64, f64, f64, f64, f64, f64, f64, f64, f64, f64) -> f64,
// CHECK-NEXT:     > = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             *mut (),
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             *mut (),
// X86_64-PC-WINDOWS-MSVC-NEXT:             Option<unsafe extern "C-unwind" fn() -> i64>,
// CHECK-NEXT:             Option<
// CHECK-NEXT:                 unsafe extern "C-unwind" fn(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     LongDouble,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     LongDouble,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     LongDouble,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     LongDouble,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     LongDouble,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     LongDouble,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     LongDouble,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     LongDouble,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     LongDouble,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     LongDouble,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 ) -> LongDouble,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 ) -> f128,
// X86_64-PC-WINDOWS-MSVC-NEXT:                     f64,
// X86_64-PC-WINDOWS-MSVC-NEXT:                     f64,
// X86_64-PC-WINDOWS-MSVC-NEXT:                     f64,
// X86_64-PC-WINDOWS-MSVC-NEXT:                     f64,
// X86_64-PC-WINDOWS-MSVC-NEXT:                     f64,
// X86_64-PC-WINDOWS-MSVC-NEXT:                     f64,
// X86_64-PC-WINDOWS-MSVC-NEXT:                     f64,
// X86_64-PC-WINDOWS-MSVC-NEXT:                     f64,
// X86_64-PC-WINDOWS-MSVC-NEXT:                     f64,
// X86_64-PC-WINDOWS-MSVC-NEXT:                     f64,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 ) -> f64,
// CHECK-NEXT:             >,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         >(dl_sym!(lib, c"ld_sum10".as_ptr() as *mut i8))
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         >(dl_sym!(lib, c"ld_sum10".as_ptr() as *mut u8))
// X86_64-PC-WINDOWS-MSVC-NEXT:         >(dl_sym!(lib, c"ld_sum10".as_ptr() as *mut i8))
// CHECK-NEXT:     };
// CHECK-NEXT:     let mix: Option<
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         unsafe extern "C-unwind" fn(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             i64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             f64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             LongDouble,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             u32,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             LongDouble,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             f32,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             LongDouble,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             i32,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             f64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             LongDouble,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         ) -> LongDouble,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         unsafe extern "C-unwind" fn(i64, f64, f128, u32, f128, f32, f128, i32, f64, f128) -> f128,
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe extern "C-unwind" fn(i64, f64, f64, u32, f64, f32, f64, i32, f64, f64) -> f64,
// CHECK-NEXT:     > = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             *mut (),
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             *mut (),
// X86_64-PC-WINDOWS-MSVC-NEXT:             Option<unsafe extern "C-unwind" fn() -> i64>,
// CHECK-NEXT:             Option<
// CHECK-NEXT:                 unsafe extern "C-unwind" fn(
// CHECK-NEXT:                     i64,
// CHECK-NEXT:                     f64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     LongDouble,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     f128,
// X86_64-PC-WINDOWS-MSVC-NEXT:                     f64,
// CHECK-NEXT:                     u32,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     LongDouble,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     f128,
// X86_64-PC-WINDOWS-MSVC-NEXT:                     f64,
// CHECK-NEXT:                     f32,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     LongDouble,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     f128,
// X86_64-PC-WINDOWS-MSVC-NEXT:                     f64,
// CHECK-NEXT:                     i32,
// CHECK-NEXT:                     f64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     LongDouble,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 ) -> LongDouble,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 ) -> f128,
// X86_64-PC-WINDOWS-MSVC-NEXT:                     f64,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 ) -> f64,
// CHECK-NEXT:             >,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         >(dl_sym!(lib, c"ld_mix".as_ptr() as *mut i8))
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         >(dl_sym!(lib, c"ld_mix".as_ptr() as *mut u8))
// X86_64-PC-WINDOWS-MSVC-NEXT:         >(dl_sym!(lib, c"ld_mix".as_ptr() as *mut i8))
// CHECK-NEXT:     };
// CHECK-NEXT:     let pair_swap: Option<unsafe extern "C-unwind" fn(ld_pair) -> ld_pair> = unsafe {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         std::mem::transmute::<*mut (), Option<unsafe extern "C-unwind" fn(ld_pair) -> ld_pair>>(
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         std::mem::transmute::<*mut (), Option<unsafe extern "C-unwind" fn(ld_pair) -> ld_pair>>(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             dl_sym!(lib, c"ld_pair_swap".as_ptr() as *mut i8),
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             dl_sym!(lib, c"ld_pair_swap".as_ptr() as *mut u8),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         )
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         )
// X86_64-PC-WINDOWS-MSVC-NEXT:         std::mem::transmute::<
// X86_64-PC-WINDOWS-MSVC-NEXT:             Option<unsafe extern "C-unwind" fn() -> i64>,
// X86_64-PC-WINDOWS-MSVC-NEXT:             Option<unsafe extern "C-unwind" fn(ld_pair) -> ld_pair>,
// X86_64-PC-WINDOWS-MSVC-NEXT:         >(dl_sym!(lib, c"ld_pair_swap".as_ptr() as *mut i8))
// CHECK-NEXT:     };
// CHECK-NEXT:     let one_half: Option<unsafe extern "C-unwind" fn(ld_one) -> ld_one> = unsafe {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         std::mem::transmute::<*mut (), Option<unsafe extern "C-unwind" fn(ld_one) -> ld_one>>(
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         std::mem::transmute::<*mut (), Option<unsafe extern "C-unwind" fn(ld_one) -> ld_one>>(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             dl_sym!(lib, c"ld_one_half".as_ptr() as *mut i8),
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             dl_sym!(lib, c"ld_one_half".as_ptr() as *mut u8),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         )
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         )
// X86_64-PC-WINDOWS-MSVC-NEXT:         std::mem::transmute::<
// X86_64-PC-WINDOWS-MSVC-NEXT:             Option<unsafe extern "C-unwind" fn() -> i64>,
// X86_64-PC-WINDOWS-MSVC-NEXT:             Option<unsafe extern "C-unwind" fn(ld_one) -> ld_one>,
// X86_64-PC-WINDOWS-MSVC-NEXT:         >(dl_sym!(lib, c"ld_one_half".as_ptr() as *mut i8))
// CHECK-NEXT:     };
// CHECK-NEXT:     let box_bump: Option<unsafe extern "C-unwind" fn(ld_box) -> ld_box> = unsafe {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         std::mem::transmute::<*mut (), Option<unsafe extern "C-unwind" fn(ld_box) -> ld_box>>(
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         std::mem::transmute::<*mut (), Option<unsafe extern "C-unwind" fn(ld_box) -> ld_box>>(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             dl_sym!(lib, c"ld_box_bump".as_ptr() as *mut i8),
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             dl_sym!(lib, c"ld_box_bump".as_ptr() as *mut u8),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         )
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         )
// X86_64-PC-WINDOWS-MSVC-NEXT:         std::mem::transmute::<
// X86_64-PC-WINDOWS-MSVC-NEXT:             Option<unsafe extern "C-unwind" fn() -> i64>,
// X86_64-PC-WINDOWS-MSVC-NEXT:             Option<unsafe extern "C-unwind" fn(ld_box) -> ld_box>,
// X86_64-PC-WINDOWS-MSVC-NEXT:         >(dl_sym!(lib, c"ld_box_bump".as_ptr() as *mut i8))
// CHECK-NEXT:     };
// CHECK-NEXT:     let call: Option<
// CHECK-NEXT:         unsafe extern "C-unwind" fn(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             Option<unsafe extern "C-unwind" fn(LongDouble, LongDouble) -> LongDouble>,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             LongDouble,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             LongDouble,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         ) -> LongDouble,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             Option<unsafe extern "C-unwind" fn(f128, f128) -> f128>,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         ) -> f128,
// X86_64-PC-WINDOWS-MSVC-NEXT:             Option<unsafe extern "C-unwind" fn(f64, f64) -> f64>,
// X86_64-PC-WINDOWS-MSVC-NEXT:             f64,
// X86_64-PC-WINDOWS-MSVC-NEXT:             f64,
// X86_64-PC-WINDOWS-MSVC-NEXT:         ) -> f64,
// CHECK-NEXT:     > = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             *mut (),
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             *mut (),
// X86_64-PC-WINDOWS-MSVC-NEXT:             Option<unsafe extern "C-unwind" fn() -> i64>,
// CHECK-NEXT:             Option<
// CHECK-NEXT:                 unsafe extern "C-unwind" fn(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     Option<unsafe extern "C-unwind" fn(LongDouble, LongDouble) -> LongDouble>,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     LongDouble,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     LongDouble,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 ) -> LongDouble,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     Option<unsafe extern "C-unwind" fn(f128, f128) -> f128>,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 ) -> f128,
// X86_64-PC-WINDOWS-MSVC-NEXT:                     Option<unsafe extern "C-unwind" fn(f64, f64) -> f64>,
// X86_64-PC-WINDOWS-MSVC-NEXT:                     f64,
// X86_64-PC-WINDOWS-MSVC-NEXT:                     f64,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 ) -> f64,
// CHECK-NEXT:             >,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         >(dl_sym!(lib, c"ld_call".as_ptr() as *mut i8))
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         >(dl_sym!(lib, c"ld_call".as_ptr() as *mut u8))
// X86_64-PC-WINDOWS-MSVC-NEXT:         >(dl_sym!(lib, c"ld_call".as_ptr() as *mut i8))
// CHECK-NEXT:     };
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     let vsum: Option<unsafe extern "C-unwind" fn(LongDouble, i32, ...) -> LongDouble> = unsafe {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         std::mem::transmute::<
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             *mut (),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             Option<unsafe extern "C-unwind" fn(LongDouble, i32, ...) -> LongDouble>,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         >(dl_sym!(lib, c"ld_vsum".as_ptr() as *mut i8))
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     let vsum: Option<unsafe extern "C-unwind" fn(f128, i32, ...) -> f128> = unsafe {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         std::mem::transmute::<*mut (), Option<unsafe extern "C-unwind" fn(f128, i32, ...) -> f128>>(
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             dl_sym!(lib, c"ld_vsum".as_ptr() as *mut u8),
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         )
// X86_64-PC-WINDOWS-MSVC-NEXT:     let vsum: Option<unsafe extern "C-unwind" fn(f64, i32, ...) -> f64> = unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         std::mem::transmute::<
// X86_64-PC-WINDOWS-MSVC-NEXT:             Option<unsafe extern "C-unwind" fn() -> i64>,
// X86_64-PC-WINDOWS-MSVC-NEXT:             Option<unsafe extern "C-unwind" fn(f64, i32, ...) -> f64>,
// X86_64-PC-WINDOWS-MSVC-NEXT:         >(dl_sym!(lib, c"ld_vsum".as_ptr() as *mut i8))
// CHECK-NEXT:     };
// CHECK-NEXT:     let box_layout: Option<unsafe extern "C-unwind" fn(*mut u64, *mut u64) -> u64> = unsafe {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         std::mem::transmute::<*mut (), Option<unsafe extern "C-unwind" fn(*mut u64, *mut u64) -> u64>>(
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         std::mem::transmute::<*mut (), Option<unsafe extern "C-unwind" fn(*mut u64, *mut u64) -> u64>>(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             dl_sym!(lib, c"ld_box_layout".as_ptr() as *mut i8),
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             dl_sym!(lib, c"ld_box_layout".as_ptr() as *mut u8),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         )
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         )
// X86_64-PC-WINDOWS-MSVC-NEXT:         std::mem::transmute::<
// X86_64-PC-WINDOWS-MSVC-NEXT:             Option<unsafe extern "C-unwind" fn() -> i64>,
// X86_64-PC-WINDOWS-MSVC-NEXT:             Option<unsafe extern "C-unwind" fn(*mut u64, *mut u64) -> u64>,
// X86_64-PC-WINDOWS-MSVC-NEXT:         >(dl_sym!(lib, c"ld_box_layout".as_ptr() as *mut i8))
// CHECK-NEXT:     };
// CHECK-NEXT:     let vcall: Option<
// CHECK-NEXT:         unsafe extern "C-unwind" fn(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             Option<unsafe extern "C-unwind" fn(LongDouble, i32, ...) -> LongDouble>,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             LongDouble,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             LongDouble,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         ) -> LongDouble,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             Option<unsafe extern "C-unwind" fn(f128, i32, ...) -> f128>,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         ) -> f128,
// X86_64-PC-WINDOWS-MSVC-NEXT:             Option<unsafe extern "C-unwind" fn(f64, i32, ...) -> f64>,
// X86_64-PC-WINDOWS-MSVC-NEXT:             f64,
// X86_64-PC-WINDOWS-MSVC-NEXT:             f64,
// X86_64-PC-WINDOWS-MSVC-NEXT:         ) -> f64,
// CHECK-NEXT:     > = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             *mut (),
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             *mut (),
// X86_64-PC-WINDOWS-MSVC-NEXT:             Option<unsafe extern "C-unwind" fn() -> i64>,
// CHECK-NEXT:             Option<
// CHECK-NEXT:                 unsafe extern "C-unwind" fn(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     Option<unsafe extern "C-unwind" fn(LongDouble, i32, ...) -> LongDouble>,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     LongDouble,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     LongDouble,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 ) -> LongDouble,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     Option<unsafe extern "C-unwind" fn(f128, i32, ...) -> f128>,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 ) -> f128,
// X86_64-PC-WINDOWS-MSVC-NEXT:                     Option<unsafe extern "C-unwind" fn(f64, i32, ...) -> f64>,
// X86_64-PC-WINDOWS-MSVC-NEXT:                     f64,
// X86_64-PC-WINDOWS-MSVC-NEXT:                     f64,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 ) -> f64,
// CHECK-NEXT:             >,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         >(dl_sym!(lib, c"ld_vcall".as_ptr() as *mut i8))
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         >(dl_sym!(lib, c"ld_vcall".as_ptr() as *mut u8))
// X86_64-PC-WINDOWS-MSVC-NEXT:         >(dl_sym!(lib, c"ld_vcall".as_ptr() as *mut i8))
// CHECK-NEXT:     };
// CHECK-NEXT:     if identity == None
// CHECK-NEXT:         || sum10 == None
// CHECK-NEXT:         || mix == None
// CHECK-NEXT:         || pair_swap == None
// CHECK-NEXT:         || one_half == None
// CHECK-NEXT:         || box_bump == None
// CHECK-NEXT:         || call == None
// CHECK-NEXT:         || vsum == None
// CHECK-NEXT:         || vcall == None
// CHECK-NEXT:         || box_layout == None
// CHECK-NEXT:     {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         println!("resolve failed");
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         println!("resolve failed");
// X86_64-PC-WINDOWS-MSVC-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe { slate_support::printf(c"resolve failed\n".as_ptr()) };
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         return 2;
// CHECK-NEXT:     }
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     let a: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     let b: LongDouble = LongDouble([1, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     let a: f128 = 1.00000000000000000005421010862427522f128;
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     let b: f128 = 1.00000000000000000010842021724855044f128;
// X86_64-PC-WINDOWS-MSVC-NEXT:     let a: f64 = 1.0f64;
// X86_64-PC-WINDOWS-MSVC-NEXT:     let b: f64 = 1.0f64;
// CHECK-NEXT:     dump(c"identity".as_ptr(), unsafe {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         __slate_icall__rf80_f80(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             unsafe {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 std::mem::transmute::<
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     Option<unsafe extern "C-unwind" fn(LongDouble) -> LongDouble>,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     *const (),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 >(identity)
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             },
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             LongDouble([120, 111, 94, 77, 60, 43, 26, 137, 39, 64]),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         )
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         identity.unwrap()(1177699121306.73777676052517360272276f128)
// X86_64-PC-WINDOWS-MSVC-NEXT:         identity.unwrap()(1177699121306.7378f64)
// CHECK-NEXT:     });
// CHECK-NEXT:     dump(c"sum10".as_ptr(), unsafe {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         __slate_icall__rf80_f80_f80_f80_f80_f80_f80_f80_f80_f80_f80(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             unsafe {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 std::mem::transmute::<
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     Option<
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                         unsafe extern "C-unwind" fn(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                             LongDouble,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                             LongDouble,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                             LongDouble,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                             LongDouble,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                             LongDouble,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                             LongDouble,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                             LongDouble,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                             LongDouble,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                             LongDouble,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                             LongDouble,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                         ) -> LongDouble,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     >,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     *const (),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 >(sum10)
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             },
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 254, 63]),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 253, 63]),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 252, 63]),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 251, 63]),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 250, 63]),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 249, 63]),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 248, 63]),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 247, 63]),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 190, 63]),
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         sum10.unwrap()(
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             1f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             0.5f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             0.25f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             0.125f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             0.0625f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             0.03125f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             0.015625f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             0.0078125f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             0.00390625f128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             2.71050543121376108501863200217485428E-20f128,
// X86_64-PC-WINDOWS-MSVC-NEXT:         sum10.unwrap()(
// X86_64-PC-WINDOWS-MSVC-NEXT:             1.0f64,
// X86_64-PC-WINDOWS-MSVC-NEXT:             0.5f64,
// X86_64-PC-WINDOWS-MSVC-NEXT:             0.25f64,
// X86_64-PC-WINDOWS-MSVC-NEXT:             0.125f64,
// X86_64-PC-WINDOWS-MSVC-NEXT:             0.0625f64,
// X86_64-PC-WINDOWS-MSVC-NEXT:             0.03125f64,
// X86_64-PC-WINDOWS-MSVC-NEXT:             0.015625f64,
// X86_64-PC-WINDOWS-MSVC-NEXT:             0.0078125f64,
// X86_64-PC-WINDOWS-MSVC-NEXT:             0.00390625f64,
// X86_64-PC-WINDOWS-MSVC-NEXT:             2.710505431213761e-20f64,
// CHECK-NEXT:         )
// CHECK-NEXT:     });
// CHECK-NEXT:     dump(c"mix".as_ptr(), unsafe {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         __slate_icall__rf80_i64_f64_f80_u32_f80_f32_f80_i32_f64_f80(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             unsafe {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 std::mem::transmute::<
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     Option<
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                         unsafe extern "C-unwind" fn(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                             i64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                             f64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                             LongDouble,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                             u32,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                             LongDouble,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                             f32,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                             LongDouble,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                             i32,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                             f64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                             LongDouble,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                         ) -> LongDouble,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     >,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     *const (),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 >(mix)
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             },
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         mix.unwrap()(
// X86_64-PC-WINDOWS-MSVC-NEXT:         mix.unwrap()(
// CHECK-NEXT:             -123456789i64,
// CHECK-NEXT:             192.0f64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             LongDouble([120, 111, 94, 77, 60, 43, 26, 137, 4, 64]),
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             34.2755555555555555258262501183708082f128,
// X86_64-PC-WINDOWS-MSVC-NEXT:             34.275555555555556f64,
// CHECK-NEXT:             12345u32,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             LongDouble([0, 0, 0, 8, 25, 42, 59, 204, 252, 191]),
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             -0.199444444444452528841793537139892578f128,
// X86_64-PC-WINDOWS-MSVC-NEXT:             -0.19944444444445253f64,
// CHECK-NEXT:             0.75f32,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             LongDouble([0, 0, 0, 0, 0, 0, 128, 128, 8, 64]),
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             514f128,
// X86_64-PC-WINDOWS-MSVC-NEXT:             514.0f64,
// CHECK-NEXT:             -77i32,
// CHECK-NEXT:             -0.125f64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             LongDouble([0, 0, 0, 0, 0, 128, 255, 255, 244, 63]),
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             9.76555049419403076171875E-4f128,
// X86_64-PC-WINDOWS-MSVC-NEXT:             0.0009765550494194031f64,
// CHECK-NEXT:         )
// CHECK-NEXT:     });
// CHECK-NEXT:     let p: ld_pair = unsafe {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         __slate_icall__rm32_m32(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             unsafe {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 std::mem::transmute::<
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     Option<unsafe extern "C-unwind" fn(ld_pair) -> ld_pair>,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     *const (),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 >(pair_swap)
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             },
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             *{
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 [[SLATE_COMPOUND_0]] = ld_pair { a: a, b: b };
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             },
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         )
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         pair_swap.unwrap()(*{
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             [[SLATE_COMPOUND_0]] = ld_pair { a: a, b: b };
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         })
// X86_64-PC-WINDOWS-MSVC-NEXT:         pair_swap.unwrap()(*{
// X86_64-PC-WINDOWS-MSVC-NEXT:             [[SLATE_COMPOUND_0]] = ld_pair { a: a, b: b };
// X86_64-PC-WINDOWS-MSVC-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// X86_64-PC-WINDOWS-MSVC-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     dump(c"pair.a".as_ptr(), p.a);
// CHECK-NEXT:     dump(c"pair.b".as_ptr(), p.b);
// CHECK-NEXT:     dump(c"one".as_ptr(), unsafe {
// CHECK-NEXT:         (*{
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             [[SLATE_COMPOUND_2]] = __slate_icall__rs80_s80(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 unsafe {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     std::mem::transmute::<
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                         Option<unsafe extern "C-unwind" fn(ld_one) -> ld_one>,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                         *const (),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     >(one_half)
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 },
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 *{
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     [[SLATE_COMPOUND_1]] = ld_one { v: b };
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     std::ptr::addr_of_mut!([[SLATE_COMPOUND_1]])
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 },
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             );
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             [[SLATE_COMPOUND_2]] = one_half.unwrap()(*{
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 [[SLATE_COMPOUND_1]] = ld_one { v: b };
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_1]])
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             });
// X86_64-PC-WINDOWS-MSVC-NEXT:             [[SLATE_COMPOUND_2]] = one_half.unwrap()(*{
// X86_64-PC-WINDOWS-MSVC-NEXT:                 [[SLATE_COMPOUND_1]] = ld_one { v: b };
// X86_64-PC-WINDOWS-MSVC-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_1]])
// X86_64-PC-WINDOWS-MSVC-NEXT:             });
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_2]])
// CHECK-NEXT:         })
// CHECK-NEXT:         .v
// CHECK-NEXT:     });
// CHECK-NEXT:     let r#box: ld_box = unsafe {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         __slate_icall__rm48_m48(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             unsafe {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 std::mem::transmute::<
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     Option<unsafe extern "C-unwind" fn(ld_box) -> ld_box>,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     *const (),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 >(box_bump)
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             },
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             *{
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 [[SLATE_COMPOUND_3]] = {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     let mut [[T_0:__t[0-9]+]]: ld_box = std::mem::zeroed();
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     [[T_0]].tag = 33u8;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     [[T_0]].x = a;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     [[T_0]].tail = 1000u32;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     [[T_0]]
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 };
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_3]])
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             },
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         )
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         box_bump.unwrap()(*{
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             [[SLATE_COMPOUND_3]] = {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 let mut [[T_0:__t[0-9]+]]: ld_box = std::mem::zeroed();
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 [[T_0]].tag = 33u8;
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 [[T_0]].x = a;
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 [[T_0]].tail = 1000u32;
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 [[T_0]]
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             };
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_3]])
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         })
// X86_64-PC-WINDOWS-MSVC-NEXT:         box_bump.unwrap()(*{
// X86_64-PC-WINDOWS-MSVC-NEXT:             [[SLATE_COMPOUND_3]] = {
// X86_64-PC-WINDOWS-MSVC-NEXT:                 let mut [[T_0:__t[0-9]+]]: ld_box = std::mem::zeroed();
// X86_64-PC-WINDOWS-MSVC-NEXT:                 [[T_0]].tag = 33u8;
// X86_64-PC-WINDOWS-MSVC-NEXT:                 [[T_0]].x = a;
// X86_64-PC-WINDOWS-MSVC-NEXT:                 [[T_0]].tail = 1000u32;
// X86_64-PC-WINDOWS-MSVC-NEXT:                 [[T_0]]
// X86_64-PC-WINDOWS-MSVC-NEXT:             };
// X86_64-PC-WINDOWS-MSVC-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_3]])
// X86_64-PC-WINDOWS-MSVC-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"box.tag %02x\n".as_ptr(), r#box.tag as i32) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     dump(c"box.x".as_ptr(), r#box.x);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"box.tail %u\n".as_ptr(), r#box.tail) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     dump(c"call".as_ptr(), unsafe {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         __slate_icall__rf80_fp_f80_f80(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             unsafe {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 std::mem::transmute::<
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     Option<
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                         unsafe extern "C-unwind" fn(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                             Option<
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                                 unsafe extern "C-unwind" fn(LongDouble, LongDouble) -> LongDouble,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                             >,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                             LongDouble,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                             LongDouble,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                         ) -> LongDouble,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     >,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     *const (),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 >(call)
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             },
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             unsafe {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 std::mem::transmute::<
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     Option<unsafe extern "C-unwind" fn(LongDouble, LongDouble) -> LongDouble>,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     *const (),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 >(unsafe {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     std::mem::transmute::<
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                         *const (),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                         Option<unsafe extern "C-unwind" fn(LongDouble, LongDouble) -> LongDouble>,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     >(__slate_ldfp_main_fused as *const ())
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 })
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             },
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             a,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             b,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         )
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         call.unwrap()(Some(fused), a, b)
// X86_64-PC-WINDOWS-MSVC-NEXT:         call.unwrap()(Some(fused), a, b)
// CHECK-NEXT:     });
// CHECK-NEXT:     dump(c"vsum".as_ptr(), unsafe {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         __slate_icall__rf80_f80_i32__f80_f80_f80(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             unsafe {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 std::mem::transmute::<
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     Option<unsafe extern "C-unwind" fn(LongDouble, i32, ...) -> LongDouble>,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     *const (),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 >(vsum)
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             },
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 63]),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             3i32,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             a,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             LongDouble([0, 0, 0, 0, 0, 0, 0, 240, 1, 64]),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             b,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         )
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         vsum.unwrap()(1.5f128, 3i32, a, 7.5f128, b)
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     });
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     });
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     dump(c"vcall".as_ptr(), unsafe {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     dump(c"vcall".as_ptr(), unsafe {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         __slate_icall__rf80_fp_f80_f80(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             unsafe {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 std::mem::transmute::<
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     Option<
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                         unsafe extern "C-unwind" fn(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                             Option<unsafe extern "C-unwind" fn(LongDouble, i32, ...) -> LongDouble>,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                             LongDouble,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                             LongDouble,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                         ) -> LongDouble,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     >,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     *const (),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 >(vcall)
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             },
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             unsafe {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 std::mem::transmute::<
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     Option<unsafe extern "C-unwind" fn(LongDouble, i32, ...) -> LongDouble>,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     *const (),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 >(unsafe {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     std::mem::transmute::<
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                         *const (),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                         Option<unsafe extern "C-unwind" fn(LongDouble, i32, ...) -> LongDouble>,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     >(__slate_ldfp_main_scaled_sum as *const ())
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 })
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             },
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             a,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             b,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         )
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         vcall.unwrap()(Some(scaled_sum), a, b)
// X86_64-PC-WINDOWS-MSVC-NEXT:         vsum.unwrap()(1.5f64, 3i32, a, 7.5f64, b)
// CHECK-NEXT:     });
// CHECK-NEXT:     let mut x: u64 = 0;
// CHECK-NEXT:     let mut tail: u64 = 0;
// CHECK-NEXT:     let size: u64 =
// CHECK-NEXT:         unsafe { box_layout.unwrap()(std::ptr::addr_of_mut!(x), std::ptr::addr_of_mut!(tail)) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"layout %s\n".as_ptr(),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             if size == (std::mem::size_of::<ld_box>() as u64)
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 && x == (std::mem::offset_of!(ld_box, x) as u64)
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 && tail == (std::mem::offset_of!(ld_box, tail) as u64)
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 c"match".as_ptr() as *mut i8
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             if size == 48 && x == 16 && tail == 32 {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 c"match".as_ptr() as *mut u8
// X86_64-PC-WINDOWS-MSVC-NEXT:             if size == (std::mem::size_of::<ld_box>() as u64)
// X86_64-PC-WINDOWS-MSVC-NEXT:                 && x == (std::mem::offset_of!(ld_box, x) as u64)
// X86_64-PC-WINDOWS-MSVC-NEXT:                 && tail == (std::mem::offset_of!(ld_box, tail) as u64)
// X86_64-PC-WINDOWS-MSVC-NEXT:             {
// X86_64-PC-WINDOWS-MSVC-NEXT:                 c"match".as_ptr() as *mut i8
// CHECK-NEXT:             } else {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 c"mismatch".as_ptr() as *mut i8
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 c"mismatch".as_ptr() as *mut u8
// X86_64-PC-WINDOWS-MSVC-NEXT:                 c"mismatch".as_ptr() as *mut i8
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     dl_close!(lib);
// CHECK-NEXT:     0
// CHECK: {{^}}}
// SLATE-FILECHECK-END
