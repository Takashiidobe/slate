static const unsigned char sbox[4] = {1, 2, 3, 4};
extern const unsigned char crypto_sbox[4] __attribute__((alias("sbox")));

int table[3] = {10, 20, 30};
extern int table_alias[3] __attribute__((alias("table")));
extern int weak_table[3] __attribute__((weak, alias("table")));

int sbox_at(int i) { return sbox[i]; }

__asm__(".section .data.alias_refs,\"aw\"\n.balign 8\n.globl alias_refs\nalias_refs:\n.quad crypto_sbox\n.quad table_alias\n.previous");

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}static mut sbox: [u8; 4] = [1, 2, 3, 4];
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: static mut table: [i32; 3] = [10, 20, 30];
// CHECK-EMPTY:
// CHECK-NEXT: core::arch::global_asm!(".globl crypto_sbox\n.set crypto_sbox, {}", sym sbox);
// CHECK-EMPTY:
// CHECK-NEXT: core::arch::global_asm!(".globl table_alias\n.set table_alias, {}", sym table);
// CHECK-EMPTY:
// CHECK-NEXT: core::arch::global_asm!(".weak weak_table\n.set weak_table, {}", sym table);
// CHECK-EMPTY:
// CHECK-NEXT: core::arch::global_asm!(
// CHECK-NEXT:     ".section .data.alias_refs,\"aw\"\n.balign 8\n.globl alias_refs\nalias_refs:\n.quad crypto_sbox\n.quad table_alias\n.previous",
// CHECK-NEXT:     options(att_syntax)
// CHECK-NEXT: );
// CHECK-EMPTY:
// CHECK-NEXT: pub fn sbox_at(i: i32) -> i32 {
// CHECK-NEXT:     (unsafe { sbox[i as usize] }) as i32
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
