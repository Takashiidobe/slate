/* PR debug/59776 */
/* { dg-do run } */
/* { dg-options "-g" } */

#if defined(__ia64__) || defined(__s390__) || defined(__s390x__)
#define NOP "nop 0"
#elif defined(__MMIX__)
#define NOP "swym 0"
#elif defined(__or1k__)
#define NOP "l.nop"
#else
#define NOP "nop"
#endif

struct S {
  float f, g;
};

__attribute__((noipa)) void foo(struct S *p) {
  struct S s1,
      s2; /* { dg-final { gdb-test pr59776.c:17 "s1.f" "5.0" { xfail { aarch64*-*-* && { any-opts "-Og" } } } } } */
  s1 =
      *p; /* { dg-final { gdb-test pr59776.c:17 "s1.g" "6.0" { xfail { aarch64*-*-* && { any-opts "-Og" } } } } } */
  s2 =
      s1; /* { dg-final { gdb-test pr59776.c:17 "s2.f" "0.0" { xfail { aarch64*-*-* && { any-opts "-Og" } } } } } */
  *(int *)&s2.f =
      0; /* { dg-final { gdb-test pr59776.c:17 "s2.g" "6.0" { xfail { no-opts "-O0" } } } } */
  asm volatile(
      NOP
      :
      :
      : "memory"); /* { dg-final { gdb-test pr59776.c:20 "s1.f" "5.0" { xfail { aarch64*-*-* && { any-opts "-Og" } } } } } */
  asm volatile(
      NOP
      :
      :
      : "memory"); /* { dg-final { gdb-test pr59776.c:20 "s1.g" "6.0" { xfail { aarch64*-*-* && { any-opts "-Og" } } } } } */
  s2 =
      s1; /* { dg-final { gdb-test pr59776.c:20 "s2.f" "5.0" { xfail { aarch64*-*-* && { any-opts "-Og" } } } } } */
  asm volatile(
      NOP
      :
      :
      : "memory"); /* { dg-final { gdb-test pr59776.c:20 "s2.g" "6.0" { xfail { no-opts "-O0" } } } } */
  asm volatile(NOP : : : "memory");
}

int
main() {
  struct S x = {5.0f, 6.0f};
  foo(&x);
  return 0;
}



// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     type @type0 S = struct {
// DEFAULT-NEXT:         field0 f: f32;
// DEFAULT-NEXT:         field1 g: f32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     fn %1 @foo(%2 p: ptr<@type0>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %3 s1: @type0 [storage=automatic];
// DEFAULT-NEXT:         let %4 s2: @type0 [storage=automatic];
// DEFAULT-NEXT:         write<@type0>(%3, copy<@type0, reason=assign>(read<@type0>(deref(read<ptr<@type0>>(%2)))));
// DEFAULT-NEXT:         write<@type0>(%4, copy<@type0, reason=assign>(read<@type0>(%3)));
// DEFAULT-NEXT:         write<i32>(deref(pointer_cast<ptr<i32>, reason=explicit>(addr_of<ptr<f32>>(field0(%4)))), const<i32>(0));
// DEFAULT-NEXT:         asm volatile "nop" [dialect=att] [options=nostack] {
// DEFAULT-NEXT:             template: "nop";
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm volatile "nop" [dialect=att] [options=nostack] {
// DEFAULT-NEXT:             template: "nop";
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<@type0>(%4, copy<@type0, reason=assign>(read<@type0>(%3)));
// DEFAULT-NEXT:         asm volatile "nop" [dialect=att] [options=nostack] {
// DEFAULT-NEXT:             template: "nop";
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm volatile "nop" [dialect=att] [options=nostack] {
// DEFAULT-NEXT:             template: "nop";
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %6 x: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = const<f32>(5.0), field1 = const<f32>(6.0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>) -> void>(%1, addr_of<ptr<@type0>>(%6));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
