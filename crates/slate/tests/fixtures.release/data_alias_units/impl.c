// { dg-options "-fPIE" }
static const unsigned char sbox[4] = {1, 2, 3, 4};
extern const unsigned char crypto_sbox[4] __attribute__((alias("sbox")));

int table[3] = {10, 20, 30};
extern int table_alias[3] __attribute__((alias("table")));
extern int weak_table[3] __attribute__((weak, alias("table")));

int sbox_at(int i) { return sbox[i]; }

__asm__(".section .data.alias_refs,\"aw\"\n.balign 8\n.globl alias_refs\nalias_refs:\n.quad crypto_sbox\n.quad table_alias\n.previous");

void emit_alias_operand_refs(void) {
  __asm__ volatile(".pushsection .data.alias_operand_refs,\"aw\"\n.balign 8\n.globl alias_operand_refs\nalias_operand_refs:\n.quad %c0\n.quad %c1\n.popsection"
                   :
                   : "i"(crypto_sbox), "i"(table_alias));
}

int read_sbox_memory(void) {
  unsigned int value;
  __asm__("movzbl %1, %0" : "=r"(value) : "m"(crypto_sbox));
  return value;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}
// CHECK-NEXT: /// { dg-options "-fPIE" }
// CHECK-NEXT: static sbox: [u8; 4] = [1, 2, 3, 4];
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: static mut table: [i32; 3] = [10, 20, 30];
// CHECK-EMPTY:
// CHECK-NEXT: core::arch::global_asm!(".globl crypto_sbox\n.set crypto_sbox, {0}", sym sbox);
// CHECK-EMPTY:
// CHECK-NEXT: core::arch::global_asm!(".globl table_alias\n.set table_alias, {0}", sym table);
// CHECK-EMPTY:
// CHECK-NEXT: core::arch::global_asm!(".weak weak_table\n.set weak_table, {0}", sym table);
// CHECK-EMPTY:
// CHECK-NEXT: core::arch::global_asm!(
// CHECK-NEXT:     ".section .data.alias_refs,\"aw\"\n.balign 8\n.globl alias_refs\nalias_refs:\n.quad crypto_sbox\n.quad table_alias\n.previous",
// CHECK-NEXT:     options(att_syntax),
// CHECK-NEXT: );
// CHECK-EMPTY:
// CHECK-NEXT: pub fn sbox_at(i: i32) -> i32 {
// CHECK-NEXT:     (unsafe { sbox[i as usize] }) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: pub fn emit_alias_operand_refs() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             ".pushsection .data.alias_operand_refs,\"aw\"\n.balign 8\n.globl alias_operand_refs\nalias_operand_refs:\n.quad crypto_sbox\n.quad table_alias\n.popsection",
// CHECK-NEXT:             options(nostack, att_syntax),
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: pub fn read_sbox_memory() -> i32 {
// CHECK-NEXT:     let mut value: u32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "movzbl crypto_sbox(%rip), {0:e}",
// CHECK-NEXT:             lateout(reg) [[T_0]],
// CHECK-NEXT:             options(pure, readonly, nostack, att_syntax),
// CHECK-NEXT:         );
// CHECK-NEXT:         value = unsafe { core::mem::transmute::<u32, u32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     value as i32
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
