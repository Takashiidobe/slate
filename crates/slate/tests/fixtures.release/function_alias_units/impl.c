int        target(int value) { return value + 7; }
static int hidden(int value) { return value * 3; }

extern int aliased(int) __attribute__((alias("target")));
int        from_static(int) __attribute__((alias("hidden")));
int        weak_alias(int) __attribute__((weak, alias("target")));

int local_identity(void) { return aliased == target; }

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}core::arch::global_asm!(".globl aliased\n.set aliased, {}", sym target);
// CHECK-EMPTY:
// CHECK-NEXT: core::arch::global_asm!(".globl from_static\n.set from_static, {}", sym hidden);
// CHECK-EMPTY:
// CHECK-NEXT: core::arch::global_asm!(".weak weak_alias\n.set weak_alias, {}", sym target);
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.impl.target")]
// CHECK-NEXT: extern "C-unwind" fn target(value: i32) -> i32 {
// CHECK-NEXT:     value + 7
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn hidden(value: i32) -> i32 {
// CHECK-NEXT:     value * 3
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: pub fn local_identity() -> i32 {
// CHECK-NEXT:     (({
// CHECK-NEXT:         let __slate_function: Option<unsafe extern "C-unwind" fn(i32) -> i32> = Some(target);
// CHECK-NEXT:         __slate_function
// CHECK-NEXT:     }) == Some(target)) as i32
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
